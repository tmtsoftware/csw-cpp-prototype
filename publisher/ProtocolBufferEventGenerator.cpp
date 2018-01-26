//
// Created by Jason Weiss on 7/21/17.
//

#include "ProtocolBufferEventGenerator.h"
#include <google/protobuf/util/time_util.h>
#include <csw-param-set.pb.h>


ProtocolBufferEventGenerator::ProtocolBufferEventGenerator() {
    eventInt=123;
}

void ProtocolBufferEventGenerator::constructSampleEvent() {
    tmt::services::Event ev;
    tmt::services::IntItem *ii1;
    tmt::services::FloatItem *fi1;
    tmt::services::Parameter *p1;
    tmt::services::Parameter *p2;

    ev.set_prefix("my.test.event");
    google::protobuf::Timestamp *evts = ev.mutable_eventtime();
    *evts = google::protobuf::util::TimeUtil::GetCurrentTime();

    p1 = ev.add_parameters();
    p2 = ev.add_parameters();

    ii1 = p1->mutable_intitem();
    fi1 = p2->mutable_floatitem();

    ii1->set_keyname("testint");
    ii1->add_values(eventInt);
    ii1->set_units("foos");

    fi1->set_keyname("testfloat");
    fi1->add_values(4.56);
    fi1->set_units("bars");

    eventString = ev.SerializeAsString();

    eventInt++;
}
