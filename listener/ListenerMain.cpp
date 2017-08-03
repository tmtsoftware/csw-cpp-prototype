#include <iostream>
#include <cpp_redis/cpp_redis>
#include <getopt.h>
#include "RedisSubscriber.h"
#include "KafkaCppSubscriber.h"
#include "ProtocolBufferEventDecoder.h"
#include "KafkaSubscriber.h"

ServiceSubscriber *sub;

void
sigint_handler(int) {
    sub->stop();
}


int main(int argc, char **argv) {
    int opt;
    std::vector<std::string> topics;
    std::string protocol = "protobuf";
    std::string service = "redis";
    EventDecoder *evDec;

    while ((opt = getopt(argc, argv, "p:s:")) != -1) {
        switch (opt) {
            case 'p': protocol = optarg;
                break;
            case 's': service = optarg;
                break;
            default:
                fprintf(stderr,
                        "Usage: %s\n"
                                "\t-s <service>    One of: redis, kafka, kafkacpp\n"
                                "\t-p <protocol>   One of: protobuf\n",
                        argv[0]
                        );
                exit(1);
        }
    }

    if (protocol == "protobuf") {
        evDec = new ProtocolBufferEventDecoder();
    } else {
        std::cerr << "Invalid protocol.  Must be one of: protobuf" << std::endl;
        exit(1);
    }

    if (service == "redis") {
        sub = new RedisSubscriber(evDec);
    } else if (service == "kafkacpp") {
        sub = new KafkaCppSubscriber(evDec);
    } else if (service == "kafka") {
        sub = new KafkaSubscriber(evDec);
    } else {
        std::cerr << "Invalid service.  Must be one of: redis, kafka, kafkacpp" << std::endl;
        exit(1);
    }

    sub->connect();

    //topics.emplace_back("testEvent");
    topics.push_back("testInt");

    sub->subscribe(topics);
    signal(SIGINT, &sigint_handler);

    sub->run();


    return 0;
}