# --- MCU SPECIFICS

# --- Fill in here ---
  # Set your MCU version
set(MCU_ARCH cortex-m3)
set(MCU_OPENCM3_NAME stm32/f1)
set(MCU_NAME stm32f1)
  # Set the location of your linker script
set(MCU_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.ld)
# --- Until here ---
message(STATUS "building for MCU type: ${MCU_NAME}")
string(TOUPPER ${MCU_NAME} MCU_VERSION_UPPER)
set(LIB_OPENCM3_MCU libopencm3_static_${MCU_NAME})


# --- LIBOPENCM3 LIBRARY

# Build libopencm3 if needed
set(libopencm3_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/libopencm3)
add_custom_target(libopencm3 make TARGETS=${MCU_OPENCM3_NAME} WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})

# Create static opencm3 library for specific MCU
add_library(${LIB_OPENCM3_MCU} STATIC IMPORTED)
set_property(TARGET ${LIB_OPENCM3_MCU} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET ${LIB_OPENCM3_MCU} PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_${MCU_NAME}.a)
add_dependencies(${LIB_OPENCM3_MCU} libopencm3)
target_link_directories(${LIB_OPENCM3_MCU} INTERFACE ${libopencm3_SOURCE_DIR}/lib)
target_compile_definitions(${LIB_OPENCM3_MCU} INTERFACE -D${MCU_VERSION_UPPER})

# Set MCU specific flags
set(COMPILE_OPTIONS
  --static
  -nostartfiles
  -fno-common
  -mcpu=${MCU_ARCH}
  -mthumb
  # -mfloat-abi=hard
  # -mfpu=fpv4-sp-d16
)
target_compile_options(${LIB_OPENCM3_MCU} INTERFACE ${COMPILE_OPTIONS})
target_link_options(${LIB_OPENCM3_MCU} INTERFACE ${COMPILE_OPTIONS})