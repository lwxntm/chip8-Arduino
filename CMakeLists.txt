# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("pio_in_clion" C CXX)

include(CMakeListsPrivate.txt)

include_directories("./.pio/libdeps/nodemcu-32s/U8g2/src")
#include_directories("./.pio/libdeps/nodemcu-32s/DHT sensor library")
#include_directories("./.pio/libdeps/nodemcu-32s/Adafruit Unified Sensor")
#include_directories("./.pio/libdeps/nodemcu-32s/Arduino-ST7789-Library")
#include_directories("./.pio/libdeps/nodemcu-32s/Adafruit GFX Library")
#include_directories("./.pio/libdeps/nodemcu-32s/Adafruit BusIO")

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
    Production ALL
    COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    Debug ALL
    COMMAND platformio -c clion run --target debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST}  src/CPU.cpp src/CPU.h src/ArrayStack.h src/roms.h)
