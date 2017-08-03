//
// Created by Jason Weiss on 7/22/17.
//

#ifndef CSW_CPP_TEST_KAFKAPUBLISHER_H
#define CSW_CPP_TEST_KAFKAPUBLISHER_H


#include <string>
#include <unordered_map>
#include "rdkafkacpp.h"
#include "ServicePublisher.h"

class KafkaPublisher : public ServicePublisher {
public:
    KafkaPublisher();
    void connect() override;
    void publish(std::string key, std::string value) override;
private:
    RdKafka::Producer *kafkaProducer;
    std::unordered_map<std::string, RdKafka::Topic *> topicMap;
    int32_t partition = RdKafka::Topic::PARTITION_UA;

};


#endif //CSW_CPP_TEST_KAFKAPUBLISHER_H
