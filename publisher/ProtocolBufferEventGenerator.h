//
// Created by Jason Weiss on 7/21/17.
//

#ifndef CSW_CPP_TEST_PROTOCOLBUFFEREVENTGENERATOR_H
#define CSW_CPP_TEST_PROTOCOLBUFFEREVENTGENERATOR_H

#include "EventGenerator.h"
#include "csw-param-set.pb.h"

class ProtocolBufferEventGenerator : public EventGenerator {
public:
    ProtocolBufferEventGenerator();
    void constructSampleEvent() override;
};

#endif //CSW_CPP_TEST_PROTOCOLBUFFEREVENTGENERATOR