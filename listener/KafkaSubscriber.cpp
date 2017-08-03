//
// Created by Jason Weiss on 7/27/17.
//

#include <iostream>
#include <zconf.h>
#include "KafkaSubscriber.h"

static int exit_eof = 0;
static int wait_eof = 0;  /* number of partitions awaiting EOF */
static int quiet = 0;
static 	enum {
    OUTPUT_HEXDUMP,
    OUTPUT_RAW,
} output = OUTPUT_HEXDUMP;

static void hexdump (FILE *fp, const char *name, const void *ptr, size_t len) {
    const char *p = (const char *)ptr;
    unsigned int of = 0;


    if (name)
        fprintf(fp, "%s hexdump (%zd bytes):\n", name, len);

    for (of = 0 ; of < len ; of += 16) {
        char hexen[16*3+1];
        char charen[16+1];
        int hof = 0;

        int cof = 0;
        int i;

        for (i = of ; i < (int)of + 16 && i < (int)len ; i++) {
            hof += sprintf(hexen+hof, "%02x ", p[i] & 0xff);
            cof += sprintf(charen+cof, "%c",
                           isprint((int)p[i]) ? p[i] : '.');
        }
        fprintf(fp, "%08x: %-48s %-16s\n",
                of, hexen, charen);
    }
}

void KafkaSubscriber::connect() {
    char hostname[128];
    char errstr[512];
    char tmp[16];
    char brokers[] = "localhost:9092";
    rd_kafka_topic_conf_t *topic_conf;
    rd_kafka_conf_t *conf = rd_kafka_conf_new();

    /* Quick termination */
    snprintf(tmp, sizeof(tmp), "%i", SIGIO);
    rd_kafka_conf_set(conf, "internal.termination.signal", tmp, NULL, 0);

    /* Topic configuration */
    topic_conf = rd_kafka_topic_conf_new();


    if (gethostname(hostname, sizeof(hostname))) {
        fprintf(stderr, "%% Failed to lookup hostname\n");
        return; // TODO throw exception
    }

    if (rd_kafka_conf_set(conf, "client.id", hostname,
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        return; // TODO throw exception
    }

    if (rd_kafka_conf_set(conf, "group.id", "foo",
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        return; // TODO throw exception
    }

    /**

    if (rd_kafka_conf_set(conf, "bootstrap.servers", "host1:9092,host2:9092",
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        return;  // TODO throw exception
    }
    */

    /* Consumer groups always use broker based offset storage */
    if (rd_kafka_topic_conf_set(topic_conf, "offset.store.method",
                                "broker",
                                errstr, sizeof(errstr)) !=
        RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        return;
    }

    /* Set default topic config for pattern-matched topics. */
    rd_kafka_conf_set_default_topic_conf(conf, topic_conf);


/* Create Kafka consumer handle */
    if (!(rkHandle = rd_kafka_new(RD_KAFKA_CONSUMER, conf,
                            errstr, sizeof(errstr)))) {
        fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
        return; // TODO throw exception
    }

    /* Add brokers */
    if (rd_kafka_brokers_add(rkHandle, brokers) == 0) {
        fprintf(stderr, "%% No valid brokers specified\n");
        exit(1);
    }

    /* Redirect rd_kafka_poll() to consumer_poll() */
    rd_kafka_poll_set_consumer(rkHandle);


    connected=true;
}

void KafkaSubscriber::subscribe(std::vector<std::string> keys) {
    if (connected) {
        // convert to rd_kafka_topic_partition_list_t
        // TODO need to be able to add topics
        topics = rd_kafka_topic_partition_list_new(keys.size());
        for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); ++key) {
            rd_kafka_topic_partition_list_add(topics, (*key).c_str(), -1);
        }


        rd_kafka_resp_err_t err;
        if ((err = rd_kafka_subscribe(rkHandle, topics))) {
            fprintf(stderr, "%% Failed to start consuming topics: %s\n", rd_kafka_err2str(err));
            return;  /* TODO throw exception */
        }
    }
}

void KafkaSubscriber::run() {
    if (connected) {
        while (!should_exit) {
            rd_kafka_message_t *rkmessage = rd_kafka_consumer_poll(rkHandle, 500);
            if (rkmessage) {
                msg_process(rkmessage);
                rd_kafka_message_destroy(rkmessage);
            }
        }

    }
}

void KafkaSubscriber::stop() {
    rd_kafka_resp_err_t err;

    should_exit = true;
    err = rd_kafka_consumer_close(rkHandle);
    if (err)
        fprintf(stderr, "%% Failed to close consumer: %s\n", rd_kafka_err2str(err));
    else
        fprintf(stderr, "%% Consumer closed\n");

    rd_kafka_topic_partition_list_destroy(topics);

    /* Destroy handle */
    rd_kafka_destroy(rkHandle);

    /* Let background threads clean up and terminate cleanly. */
    int run = 5;
    while (run-- > 0 && rd_kafka_wait_destroyed(1000) == -1)
        printf("Waiting for librdkafka to decommission\n");
    if (run <= 0)
        rd_kafka_dump(stdout, rkHandle);

}

void KafkaSubscriber::handleSubscribeMessage(const std::string chan, const std::string message) {
    std::cout << "MESSAGE " << chan << ": "  << message << std::endl;
    if (message == "stop") {
        stop();
        return;
    }


}

void KafkaSubscriber::msg_process(rd_kafka_message_t *rkmessage) {
    if (rkmessage->err) {
        if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
            fprintf(stderr,
                    "%% Consumer reached end of %s [%d] "
                            "message queue at offset %lld\n",
                    rd_kafka_topic_name(rkmessage->rkt),
                    rkmessage->partition, rkmessage->offset);

            if (exit_eof && --wait_eof == 0) {
                fprintf(stderr, "%% All partition(s) reached EOF: exiting\n");
                //run = 0;
            }

            return;
        }

        if (rkmessage->rkt)
            fprintf(stderr, "%% Consume error for "
                            "topic \"%s\" [%d] "
                            "offset %lld: %s\n",
                    rd_kafka_topic_name(rkmessage->rkt),
                    rkmessage->partition,
                    rkmessage->offset,
                    rd_kafka_message_errstr(rkmessage));
        else
            fprintf(stderr, "%% Consumer error: %s: %s\n",
                    rd_kafka_err2str(rkmessage->err),
                    rd_kafka_message_errstr(rkmessage));

        if (rkmessage->err == RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION ||
            rkmessage->err == RD_KAFKA_RESP_ERR__UNKNOWN_TOPIC)
            //run = 0;
            return;
    }

    if (!quiet) {
        fprintf(stdout, "%% Message (topic %s [%d], "
                        "offset %lld, %zd bytes):\n",
                rd_kafka_topic_name(rkmessage->rkt),
                rkmessage->partition,
                rkmessage->offset, rkmessage->len);

        if (rkmessage->key_len) {

            if (output == OUTPUT_HEXDUMP)
                hexdump(stdout, "Message Key",
                        rkmessage->key, rkmessage->key_len);
            else
                printf("Key: %.*s\n",
                       (int) rkmessage->key_len, (char *) rkmessage->key);
        }

        if (output == OUTPUT_HEXDUMP)
            hexdump(stdout, "Message Payload",
                    rkmessage->payload, rkmessage->len);
        else
            printf("%.*s\n",
                   (int) rkmessage->len, (char *) rkmessage->payload);
    }
    const std::string *chan = new std::string(rd_kafka_topic_name(rkmessage->rkt));
    const std::string *message = new std::string((char *)rkmessage->payload, rkmessage->len);
    handleSubscribeMessage(*chan, *message);

}

KafkaSubscriber::KafkaSubscriber(EventDecoder *ed) : ServiceSubscriber(ed) {
}
