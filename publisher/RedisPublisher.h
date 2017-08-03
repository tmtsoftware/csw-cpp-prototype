//
// Created by Jason Weiss on 7/7/17.
//

#ifndef CSW_CPP_TEST_REDISPUBLISHER_H
#define CSW_CPP_TEST_REDISPUBLISHER_H


#include <string>
#include "ServicePublisher.h"
#include <cpp_redis/redis_client.hpp>

class RedisPublisher : public ServicePublisher {
public:
    RedisPublisher();
    void connect() override;
    void publish(std::string key, std::string value) override;
private:
    cpp_redis::redis_client redisClient;
    static void handleDisconnect(cpp_redis::redis_client&);
    static void handlePublishResponse(cpp_redis::reply &reply);

};


#endif //CSW_CPP_TEST_REDISPUBLISHER_H
