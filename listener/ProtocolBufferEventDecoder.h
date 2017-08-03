//
// Created by Jason Weiss on 7/24/17.
//

#ifndef CSW_CPP_TEST_PROTOCOLBUFFEREVENTDECODER_H
#define CSW_CPP_TEST_PROTOCOLBUFFEREVENTDECODER_H

#include <string>
#include "csw-param-set.pb.h"
#include "EventDecoder.h"

class ProtocolBufferEventDecoder : public EventDecoder {
public:
    void decodeTestEvent(const std::string msg);
private:
    static std::string parseTimestamp(google::protobuf::Timestamp ts);
};


#endif //CSW_CPP_TEST_PROTOCOLBUFFEREVENTDECODER_H
