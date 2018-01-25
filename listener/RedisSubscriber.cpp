//
// Created by Jason Weiss on 7/7/17.
//

#include "RedisSubscriber.h"
#include "ProtocolBufferEventDecoder.h"

using namespace std::placeholders;

void RedisSubscriber::connect() {

//    redisClient.connect("127.0.0.1", 6379, [](cpp_redis::subscriber& client) {
//        RedisSubscriber::handleDisconnect(client);
//    });

    redisClient.connect("127.0.0.1", 6379, [](const std::string& host, std::size_t port, cpp_redis::subscriber::connect_state status) {
        if (status == cpp_redis::subscriber::connect_state::dropped) {
            std::cout << "client disconnected from " << host << ":" << port << std::endl;
        }
    });

    if (redisClient.is_connected()) {
        std::cout << "Subscriber connected." << std::endl;
    }

}

void RedisSubscriber::subscribe(std::vector<std::string> keys) {
    std::vector<std::string>::iterator key;
    for (key = keys.begin(); key < keys.end(); key++) {
        std::string s = *key;
        std::cout << "subscribing to " << *key << std::endl;
        redisClient.subscribe(*key, [this](const std::string &chan, const std::string &msg) {
            std::cout << chan << "=" << msg << std::endl;
            this->handleSubscribeMessage(chan, msg);
        }, [](int64_t nb_channel_subscribed) {
            RedisSubscriber::handleSubscribeResponse(nb_channel_subscribed);
        });
    }
    redisClient.commit();
}
void RedisSubscriber::subscribeAll() {
    redisClient.psubscribe("*", [this](const std::string& chan, const std::string& msg) {
        this->handleSubscribeMessage(chan, msg);
    }, [](int64_t nb_channel_subscribed) {
        RedisSubscriber::handleSubscribeResponse(nb_channel_subscribed);
    });

    redisClient.commit();

}

void RedisSubscriber::handleDisconnect(cpp_redis::subscriber &) {
    std::cout << "sub disconnected (disconnection handler)" << std::endl;
}

void RedisSubscriber::handleSubscribeMessage(const std::string chan, const std::string msg) {
    if (chan == "testEvent") {
        eventDecoder->decodeTestEvent(msg);
    } else {
        std::cout << "MESSAGE " << chan << ": "  << msg << std::endl;
    }
}

void RedisSubscriber::handleSubscribeResponse(int64_t nb_channel_subscribed) {
    std::cout << "Channel subscribed: " << nb_channel_subscribed << std::endl;
}

void RedisSubscriber::run() {
    while (!should_exit) {};
}

void RedisSubscriber::stop() {
    should_exit = true;
}

RedisSubscriber::RedisSubscriber(EventDecoder *ed) : ServiceSubscriber(ed) {}
