INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER csw-param-set.proto)
ADD_LIBRARY(csw_protobuf ${PROTO_HEADER} ${PROTO_SRC})
target_include_directories(csw_protobuf PUBLIC ${CMAKE_CURRENT_BINARY_DIR})