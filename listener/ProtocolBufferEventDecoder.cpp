//
// Created by Jason Weiss on 7/7/17.
//

#include <sstream>   // stringstream
#include <iomanip>   // put_time
#include <iostream>
#include <google/protobuf/repeated_field.h>

#include "ProtocolBufferEventDecoder.h"

std::string ProtocolBufferEventDecoder::parseTimestamp(google::protobuf::Timestamp ts) {
    struct timespec tv;
    tm *t;
    tv.tv_sec = ts.seconds();
    tv.tv_nsec = ts.nanos();

    t = localtime(&tv.tv_sec);

    std::stringstream buffer;

     buffer << std::put_time(t, "%F %T");

    return buffer.str();
};

void ProtocolBufferEventDecoder::decodeTestEvent(const std::string msg) {
    tmt::services::Event ev;
    ev.ParseFromString(msg);

    std::cout << "Event prefix: " << ev.prefix() << std::endl;
    google::protobuf::Timestamp evts = ev.eventtime();
    std::cout << "Event timestamp: " << parseTimestamp(evts) << std::endl;
    google::protobuf::RepeatedPtrField<tmt::services::Parameter> parameters = ev.parameters();
    google::protobuf::RepeatedPtrField<tmt::services::Parameter>::iterator paramIterator;
    for (paramIterator = parameters.begin(); paramIterator != parameters.end(); paramIterator++) {
        tmt::services::Parameter param = *paramIterator;
        if (param.has_intitem()) {
            tmt::services::IntItem item = param.intitem();
            std::cout << "Parameter key = " << item.keyname() << std::endl;
            std::cout << "units = " << item.units() << ", value(s) = [";
            for (int ii = 0; ii<item.values_size(); ii++) {
                std::cout << item.values(ii);
                if (ii==item.values_size()-1) {
                    std::cout << "]" << std::endl;
                } else {
                    std::cout << ",";
                }
            }
        } else if (param.has_floatitem()) {
            tmt::services::FloatItem item = param.floatitem();
            std::cout << "Parameter key = " << item.keyname() << std::endl;
            std::cout << "units = " << item.units() << ", value(s) = [";
            for (int ii = 0; ii<item.values_size(); ii++) {
                std::cout << item.values(ii);
                if (ii==item.values_size()-1) {
                    std::cout << "]" << std::endl;
                } else {
                    std::cout << ",";
                }
            }
        }

    }
}
