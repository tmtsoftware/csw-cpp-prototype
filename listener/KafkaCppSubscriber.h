//
// Created by Jason Weiss on 7/22/17.
//

#ifndef CSW_CPP_TEST_KAFKASUBSCRIBER_H
#define CSW_CPP_TEST_KAFKASUBSCRIBER_H


#include "ServiceSubscriber.h"
#include "rdkafkacpp.h"
#include <atomic>

class KafkaCppSubscriber : public ServiceSubscriber {
public:
    explicit KafkaCppSubscriber(EventDecoder *ed);
    ~KafkaCppSubscriber() {stop();}
    void connect() override;
    void subscribe(std::vector<std::string> keys) override;
    void run() override;
    void stop() override;
private:
    RdKafka::KafkaConsumer *kafkaClient;
    void handleSubscribeMessage(const std::string chan, const std::string message) override;
    int verbosity = 4;
    volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
    void msg_consume(RdKafka::Message* message, void* opaque);
};


#endif //CSW_CPP_TEST_KAFKASUBSCRIBER_H
