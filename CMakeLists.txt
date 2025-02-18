cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(mctp C)

enable_testing()

set(CMAKE_C_STANDARD 11)
set(C_STANDARD_REQUIRED TRUE)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library (${PROJECT_NAME} STATIC
    src/core.c
    src/control.c
    src/serial.c
    src/bus.c
    src/packet.c
    src/packet_queue.c
    src/message_queue.c
    src/crc16.c
)

target_include_directories (${PROJECT_NAME}
    PUBLIC ${PROJECT_ROOT}/include
)

include(CTest)

target_link_libraries(mctp PUBLIC gcov)

add_subdirectory(test)
