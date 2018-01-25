cmake_minimum_required(VERSION 3.7)
project(csw_cpp_test)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-g -Wall")


# CSW PROTOBUF
find_package(Protobuf REQUIRED)
if(${PROTOBUF_FOUND})
    message(${Protobuf_LIBRARIES})
endif()

# REDIS LIBRARIES
#set(CPP_REDIS_ROOT ../external/cpp_redis)
set(CPP_REDIS_ROOT /usr/local)
add_library(cpp_redis STATIC IMPORTED)
#set_property(TARGET cpp_redis PROPERTY IMPORTED_LOCATION ${CPP_REDIS_ROOT}/cmake-build-debug/lib/libcpp_redis.a)
set_property(TARGET cpp_redis PROPERTY IMPORTED_LOCATION ${CPP_REDIS_ROOT}/lib/libcpp_redis.a)
add_library(cpp_redis_tacopie STATIC IMPORTED)
#set_property(TARGET cpp_redis_tacopie PROPERTY IMPORTED_LOCATION ${CPP_REDIS_ROOT}/cmake-build-debug/lib/libtacopie.a)
set_property(TARGET cpp_redis_tacopie PROPERTY IMPORTED_LOCATION ${CPP_REDIS_ROOT}/lib/libtacopie.a)
include_directories(${CPP_REDIS_ROOT}/includes/)

set(REDIS_LIBRARIES cpp_redis cpp_redis_tacopie)

# KAFKA LIBRARIES
include_directories(/usr/local/include/librdkafka)

find_package(Threads REQUIRED)

find_package(ZLIB REQUIRED)
if (${ZLIB_FOUND})
    message("${ZLIB_LIBRARIES}")
endif()

find_package(OpenSSL REQUIRED)
if (${OpenSSL_FOUND})
    message("${OpenSSL_LIBRARIES}")
endif()

set(KAFKA_LIBRARIES ${OpenSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} rdkafka++ rdkafka)

add_subdirectory(protocol/protobuf)
add_subdirectory(protocol/avro)
add_subdirectory(publisher)
add_subdirectory(listener)
