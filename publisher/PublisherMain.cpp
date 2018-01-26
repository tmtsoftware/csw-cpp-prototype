#include <iostream>
#include <cpp_redis/cpp_redis>
#include <zconf.h>
#include "RedisPublisher.h"
#include "EventGenerator.h"
#include "ProtocolBufferEventGenerator.h"
#include "KafkaPublisher.h"

int main(int argc, char **argv) {
    int opt;
    ServicePublisher *pub;
    EventGenerator *evGen;

    std::string protocol = "protobuf";
    std::string service = "redis";

    while ((opt = getopt(argc, argv, "p:s:")) != -1) {
        switch (opt) {
            case 'p': protocol = optarg;
                break;
            case 's': service = optarg;
                break;
            default:
                fprintf(stderr,
                "Usage: %s\n"
                "-s <service>    One of: redis, kafka\n"
                "-p <protocol>   One of: protobuf\n",
                argv[0]);
                exit(1);
        }
    }

    try {
        if (protocol == "protobuf") {
            evGen = new ProtocolBufferEventGenerator();
        } else {
            std::cerr << "Invalid protocol.  Must be one of: protobuf, avro" << std::endl;
            exit(1);
        }

        if (service == "redis") {
            pub = new RedisPublisher();
        } else if (service == "kafka") {
            pub = new KafkaPublisher();
        } else {
            std::cerr << "Invalid service.  Must be one of: redis, kafka" << std::endl;
            exit(1);
        }

        pub->connect();


        int ii = 123;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (true) {
            evGen->constructSampleEvent();
//            pub->publish("testEvent", evGen->getEventString());

            pub->publish("testInt", std::to_string(ii));
            ii++;
            sleep(1);
        }
#pragma clang diagnostic pop
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}