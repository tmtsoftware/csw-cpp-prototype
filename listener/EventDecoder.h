//
// Created by Jason Weiss on 7/7/17.
//

#ifndef CSW_CPP_TEST_EVENTDECODER_H
#define CSW_CPP_TEST_EVENTDECODER_H


#include <string>

class EventDecoder {
public:
    virtual void decodeTestEvent(const std::string msg) = 0;
};


#endif //CSW_CPP_TEST_EVENTDECODER_H
