//
// Created by Jason Weiss on 7/22/17.
//

#include <string>
#include <vector>
#include "EventDecoder.h"

#ifndef CSW_CPP_TEST_SERVICESUBSCRIBER_H
#define CSW_CPP_TEST_SERVICESUBSCRIBER_H

class ServiceSubscriber {
public:
    explicit ServiceSubscriber(EventDecoder *ed) : eventDecoder(ed) {};
    virtual void connect() = 0;
    virtual void subscribe(std::vector<std::string> keys) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
protected:
    EventDecoder *eventDecoder;
    bool connected = false;
private:
    virtual void handleSubscribeMessage(const std::string chan, const std::string message) = 0;
};

#endif //CSW_CPP_TEST_SERVICESUBSCRIBER_H

