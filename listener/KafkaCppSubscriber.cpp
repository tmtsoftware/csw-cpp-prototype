//
// Created by Jason Weiss on 7/22/17.
//

#include <iostream>
#include "KafkaCppSubscriber.h"

void KafkaCppSubscriber::handleSubscribeMessage(const std::string chan, const std::string message) {
    std::cout << "MESSAGE " << chan << ": "  << message << std::endl;
}

void KafkaCppSubscriber::msg_consume(RdKafka::Message* message, void* opaque) {
    switch (message->err()) {
        case RdKafka::ERR__TIMED_OUT:
            break;

        case RdKafka::ERR_NO_ERROR:
            /* Real message */
            if (verbosity >= 3)
                std::cerr << "Read msg at offset " << message->offset() << std::endl;
            RdKafka::MessageTimestamp ts;
            ts = message->timestamp();
            if (verbosity >= 2 &&
                ts.type != RdKafka::MessageTimestamp::MSG_TIMESTAMP_NOT_AVAILABLE) {
                std::string tsname = "?";
                if (ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_CREATE_TIME)
                    tsname = "create time";
                else if (ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_LOG_APPEND_TIME)
                    tsname = "log append time";
                std::cout << "Timestamp: " << tsname << " " << ts.timestamp << std::endl;
            }
            std::cout << *message->key();
            //printf("%.*s\n",
            //       static_cast<int>(message->len()),
            //       static_cast<const char *>(message->payload()));
            //handleSubscribeMessage(*(message->key()), static_cast<const char*>(message->payload()));
            break;

        case RdKafka::ERR__PARTITION_EOF:
            /* Last message */
            std::cerr << "%% EOF reached for all  partition(s)" << std::endl;
            break;

        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            should_exit = true;
            break;

        default:
            /* Errors */
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            should_exit = true;
    }

}



void KafkaCppSubscriber::connect() {
    std::string brokers = "localhost";
    std::string errstr;
    bool debug = false;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    /*
      * Set configuration properties
      */
    conf->set("metadata.broker.list", brokers, errstr);

    if (debug) {
        if (conf->set("debug", "all", errstr) != RdKafka::Conf::CONF_OK) {
            std::cerr << errstr << std::endl;
            return;
        }
    }

    if (conf->set("group.id",  "myGroup", errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << errstr << std::endl;
        return;
    }

    /*
     * Consumer mode
     */

    /*
     * Create consumer using accumulated global configuration.
     */
    kafkaClient = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!kafkaClient) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;

    std::cout << "% Created consumer " << kafkaClient->name() << std::endl;

}

void KafkaCppSubscriber::subscribe(std::vector<std::string> keys) {
    /*
* Subscribe to topics
*/
    RdKafka::ErrorCode err = kafkaClient->subscribe(keys);
    if (err) {
        std::cerr << "Failed to subscribe to " << keys.size() << " topics: "
                  << RdKafka::err2str(err) << std::endl;
        exit(1);
    }

}

void KafkaCppSubscriber::run() {

    /*
     * Consume messages
     */
    while (!should_exit) {
        RdKafka::Message *msg = kafkaClient->consume(1000);
        msg_consume(msg, NULL);
        delete msg;
    }

}

void KafkaCppSubscriber::stop() {
    should_exit = true;
    kafkaClient->close();
    delete kafkaClient;
    RdKafka::wait_destroyed(5000);
}

KafkaCppSubscriber::KafkaCppSubscriber(EventDecoder *ed) : ServiceSubscriber(ed) {}
