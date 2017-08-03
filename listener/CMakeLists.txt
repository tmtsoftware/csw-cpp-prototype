set(SOURCE_FILES ListenerMain.cpp RedisSubscriber.cpp RedisSubscriber.h ProtocolBufferEventDecoder.cpp EventDecoder.h ServiceSubscriber.h KafkaCppSubscriber.cpp KafkaCppSubscriber.h ProtocolBufferEventDecoder.h KafkaSubscriber.cpp KafkaSubscriber.h)
add_executable(csw_cpp_test_listener ${SOURCE_FILES})
target_link_libraries(csw_cpp_test_listener csw_protobuf ${REDIS_LIBRARIES} ${PROTOBUF_LIBRARIES} ${KAFKA_LIBRARIES} )
