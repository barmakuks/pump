cmake_minimum_required(VERSION 3.8.2)

SET (TARGET_NAME pump)

project(${TARGET_NAME})

enable_language(C CXX ASM)

set(PORT "/dev/ttyACM0" CACHE STRING "Port to deploy /dev/ttyACM0 or /dev/ttyUSB0" )

get_board_id(board_id nano atmega328old)
#get_board_id(board_id uno atmega328)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_arduino_executable(${TARGET_NAME} ${board_id} ${SOURCES} ${HEADERS})

upload_arduino_target(${TARGET_NAME} "${board_id}" ${PORT})
