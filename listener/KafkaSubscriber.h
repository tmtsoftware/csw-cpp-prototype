//
// Created by Jason Weiss on 7/27/17.
//

#ifndef CSW_CPP_TEST_CONFLUENTSUBSCRIBER_H
#define CSW_CPP_TEST_CONFLUENTSUBSCRIBER_H

#include <rdkafka.h>
#include "ServiceSubscriber.h"
#include "rdkafkacpp.h"

class KafkaSubscriber : public ServiceSubscriber {
public:
    explicit KafkaSubscriber(EventDecoder *ed);
    ~KafkaSubscriber() {stop();}
    void connect() override;
    void subscribe(std::vector<std::string> keys) override;
    void run() override;
    void stop() override;
private:
    rd_kafka_t *rkHandle;
    rd_kafka_topic_partition_list_t *topics;
    volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
    void handleSubscribeMessage(const std::string chan, const std::string message) override;
    void msg_process(rd_kafka_message_t *message);

};


#endif //CSW_CPP_TEST_CONFLUENTSUBSCRIBER_H
