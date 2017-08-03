//
// Created by Jason Weiss on 7/7/17.
//

#ifndef CSW_CPP_TEST_EVENTGENERATOR_H
#define CSW_CPP_TEST_EVENTGENERATOR_H


#include <string>

class EventGenerator {
public:
    virtual void constructSampleEvent() = 0;
    std::string getEventString() {
        return eventString;
    }

protected:
    std::string eventString;
    int eventInt=0;
};


#endif //CSW_CPP_TEST_EVENTGENERATOR_H
