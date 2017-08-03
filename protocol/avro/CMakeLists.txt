set(CSW_AVRO_ROOT csw-param-set)
set(CSW_AVRO_INPUT ${CSW_AVRO_ROOT}.json)
set(CSW_AVRO_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/protocol/avro/${CSW_AVRO_ROOT}.hh)
add_custom_target(generate_csw_avro
        COMMAND avrogencpp -i ${CSW_AVRO_INPUT} -o ${CSW_AVRO_OUTPUT}
)
