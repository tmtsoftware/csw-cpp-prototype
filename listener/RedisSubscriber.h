//
// Created by Jason Weiss on 7/7/17.
//

#ifndef CSW_CPP_TEST_REDISSUBSCRIBER_H
#define CSW_CPP_TEST_REDISSUBSCRIBER_H

#include <string>
#include <cpp_redis/redis_client.hpp>
#include <cpp_redis/redis_subscriber.hpp>
#include "ServiceSubscriber.h"

class RedisSubscriber : public ServiceSubscriber {
public:
    explicit RedisSubscriber(EventDecoder *ed);
    void connect() override;
    void subscribe(std::vector<std::string> keys) override;
    void subscribeAll();
    void run() override;
    void stop() override;
private:
    cpp_redis::redis_subscriber redisClient;
    static void handleDisconnect(cpp_redis::redis_subscriber&);
    void handleSubscribeMessage(const std::string chan, const std::string message) override;
    static void handleSubscribeResponse(int64_t nb_channel_subscribed);
    volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
    cpp_redis::redis_subscriber::subscribe_callback_t messageCallback;

};


#endif //CSW_CPP_TEST_REDISSUBSCRIBER_H
