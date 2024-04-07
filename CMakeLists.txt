cmake_minimum_required(VERSION 3.0.0)
project(staxtest VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 20)
add_executable(staxtest main.cpp)


add_test(NAME staxtest
         COMMAND $<TARGET_FILE:staxtest>)