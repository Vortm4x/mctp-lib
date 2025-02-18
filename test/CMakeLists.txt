project(mctp-test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

include(import_catch2.cmake)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(${PROJECT_NAME}
    main.cpp
    dump.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    mctp
    Catch2::Catch2WithMain
)

target_compile_options(mctp PRIVATE
    "-fprofile-arcs"
    "-ftest-coverage"
)


include(CTest)
catch_discover_tests(${PROJECT_NAME})
