//
// Created by Jason Weiss on 7/7/17.
//

#include "RedisPublisher.h"

void RedisPublisher::connect() {

    redisClient.connect("127.0.0.1", 6379, [](cpp_redis::redis_client& client) {
        RedisPublisher::handleDisconnect(client);
    });
}

void RedisPublisher::publish(std::string key, std::string value) {
    redisClient.set(key, value, [](cpp_redis::reply &reply) {
        handlePublishResponse(reply);
    });
    redisClient.publish(key, value, [](cpp_redis::reply &reply) {
        handlePublishResponse(reply);
    });
    redisClient.commit();
}

void RedisPublisher::handlePublishResponse(cpp_redis::reply &reply) {
    std::cout << "set testInt: " << reply << std::endl;
}

void RedisPublisher::handleDisconnect(cpp_redis::redis_client& client) {
    std::cout << "client disconnected (disconnection handler)" << std::endl;
}

RedisPublisher::RedisPublisher() {

}


