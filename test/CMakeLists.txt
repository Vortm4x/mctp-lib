project(mctp-test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

include(import_catch2.cmake)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


add_library(${PROJECT_NAME}-dump
    unit/dump.cpp
)
target_include_directories(${PROJECT_NAME}-dump PRIVATE
    ${PROJECT_ROOT}
)
target_link_libraries(${PROJECT_NAME}-dump PUBLIC
    mctp
)

add_executable(${PROJECT_NAME}
# SERIAL
    unit/serial/serial-rx-fsm.cpp
    unit/serial/serial-frame-rx.cpp
    unit/serial/serial-pkt-tx.cpp
# CORE
    unit/core/core-control-req-tx.cpp
    unit/core/core-single-pkt-tx.cpp
    unit/core/core-multi-pkt-tx.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_ROOT}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    mctp
    ${PROJECT_NAME}-dump
    Catch2::Catch2WithMain
)

catch_discover_tests(${PROJECT_NAME})
