cmake_minimum_required(VERSION 3.10)
project(banking)

add_library(banking STATIC Account.cpp Account.h Transaction.cpp Transaction.h)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
