cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(mctp C)

option(BUILD_TESTING "Enable testing feature" ON)

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
    src/packet_queue_map.c
    src/crc16.c
)

target_include_directories (${PROJECT_NAME}
    PUBLIC ${PROJECT_ROOT}/include
)

if(BUILD_TESTING)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "--coverage"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        gcov
    )

    include(CTest)
    add_subdirectory(test)
endif()
