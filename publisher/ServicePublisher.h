//
// Created by Jason Weiss on 7/21/17.
//

#ifndef CSW_CPP_TEST_SERVICEPUBLISHER_H
#define CSW_CPP_TEST_SERVICEPUBLISHER_H


class ServicePublisher {
public:
    virtual void connect() = 0;
    virtual void publish(std::string key, std::string value) = 0;

};

#endif //CSW_CPP_TEST_SERVICEPUBLISHER_H
