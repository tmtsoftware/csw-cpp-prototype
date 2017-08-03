//
// Created by Jason Weiss on 7/22/17.
//

#include <iostream>
#include "KafkaPublisher.h"


class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb (RdKafka::Message &message) {
        std::cout << "Message delivery for (" << message.len() << " bytes): " <<
                  message.errstr() << std::endl;
        if (message.key())
            std::cout << "Key: " << *(message.key()) << ";" << std::endl;
    }
};


KafkaPublisher::KafkaPublisher() {

}

void KafkaPublisher::connect() {
    std::string brokers = "localhost";
    std::string errstr;
    std::string topic_str = "testTopic";
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

    if (conf->set("group.id", "myGroup", errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << errstr << std::endl;
        return;
    }

    ExampleDeliveryReportCb ex_dr_cb;

    /* Set delivery report callback */
    conf->set("dr_cb", &ex_dr_cb, errstr);

    /*
     * Create producer using accumulated global configuration.
     */
    kafkaProducer = RdKafka::Producer::create(conf, errstr);
    if (!kafkaProducer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        return;
    }

    std::cout << "% Created producer " << kafkaProducer->name() << std::endl;

}

void KafkaPublisher::publish(std::string key, std::string value) {
    std::string errstr;

    RdKafka::Topic *topic;

    // get Topic from Map

    //. see if topic exists in map
    if (topicMap.find(key) == topicMap.end()) {
        // if not, create it and put it in map

        // create handle
        RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
        topic = RdKafka::Topic::create(kafkaProducer, key, tconf, errstr);
        if (!topic) {
            std::cerr << "Failed to create topic: " << errstr << std::endl;
            return;
        }

        // insert into map
        topicMap[key] = topic;

    } else {
        // if it is in map, grab it
        topic = topicMap[key];
    }

    /*
     * Produce message
     */
    RdKafka::ErrorCode resp = kafkaProducer->produce(topic, partition,
                              RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                              const_cast<char *>(value.c_str()), value.size(),
                              NULL, NULL);
    if (resp != RdKafka::ERR_NO_ERROR)
        std::cerr << "% Produce failed: " <<
                  RdKafka::err2str(resp) << std::endl;
    else
        std::cerr << "% Produced message (" << value.size() << " bytes)" <<
                  std::endl;

    kafkaProducer->poll(0);

    while (kafkaProducer->outq_len() > 0) {
        std::cerr << "Waiting for " << kafkaProducer->outq_len() << std::endl;
        kafkaProducer->poll(1000);
    }


}