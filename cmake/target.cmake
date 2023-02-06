# Create target executable
add_executable(${PROJECT_NAME} ${C_SRCS_TARGET} ${C_SRCS} ${CXX_SRCS} ${ASM_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
target_compile_definitions(${PROJECT_NAME} PRIVATE -DUSE_LIBOPENCM3=1)
set(TARGET_LINK_OPTIONS
    -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map
    -Wl,-T,${MCU_LINKER_SCRIPT}
    -lc
    -lgcc
    -lnosys)
target_link_options(${PROJECT_NAME} PRIVATE ${TARGET_LINK_OPTIONS})
target_link_libraries(${PROJECT_NAME} ${LIB_OPENCM3_MCU})

add_custom_target(flash_targets
                  DEPENDS ${PROJECT_NAME}
                  COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
                  COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
                  COMMENT "Creating hex and bin files for flashing ")

add_custom_command(TARGET flash_targets POST_BUILD
                    COMMAND gdb-multiarch -batch -ex "set confirm off"
                            -ex "set architecture arm"
                            -ex "file ${PROJECT_NAME}.elf"
                            -ex "target extended-remote /dev/ttyACM0"
                            -ex "mon sw"
                            -ex "att 1"
                            -ex "load"
                            -ex "kill")