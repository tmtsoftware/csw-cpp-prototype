set(SOURCE_FILES PublisherMain.cpp EventGenerator.h ProtocolBufferEventGenerator.cpp ProtocolBufferEventGenerator.h ServicePublisher.h RedisPublisher.cpp RedisPublisher.h KafkaPublisher.cpp KafkaPublisher.h)
add_executable(csw_cpp_test_publisher ${SOURCE_FILES})
target_link_libraries(csw_cpp_test_publisher csw_protobuf ${REDIS_LIBRARIES} ${PROTOBUF_LIBRARIES} ${KAFKA_LIBRARIES} )
