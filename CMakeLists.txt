cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(mctp C)

set(CMAKE_C_STANDARD 17)
set(C_STANDARD_REQUIRED ON)


set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_DEBUG $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>)

option(BUILD_TESTING "Enable testing feature" ON)


add_library (
    ${PROJECT_NAME} SHARED
    src/crc16.c
    src/bus.c
)

target_include_directories (
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_ROOT}/include
)

set(SANITIZER
    -fsanitize=address
    -fno-omit-frame-pointer
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wformat
    -Wconversion
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wshadow
    -pedantic
    $<${PROJECT_DEBUG}:${SANITIZER}>
)

if(BUILD_TESTING)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        "--coverage"
    )

    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        gcov
    )

    include(CTest)
    add_subdirectory(test)
endif()
