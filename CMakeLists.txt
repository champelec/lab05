cmake_minimum_required(VERSION 3.10)
project(lab05)

option(BUILD_TESTS "Build tests" OFF)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

if(CODE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(WARNING "Code coverage not supported by this compiler")
    endif()
endif()

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()

    set(gtest_disable_warnings ON CACHE BOOL "" FORCE)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=maybe-uninitialized")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=maybe-uninitialized")

    add_subdirectory(third-party/gtest)

    file(GLOB LAB05_TEST_SOURCES tests/*.cpp)

    add_executable(check ${LAB05_TEST_SOURCES})
    target_link_libraries(check banking gtest_main gmock)

    target_include_directories(check PRIVATE banking)

    add_test(NAME check COMMAND check)
endif()
