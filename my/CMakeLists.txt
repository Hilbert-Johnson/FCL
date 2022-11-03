set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.6)
project(fcl_test)
add_executable(test-fcl test-fcl.cpp)


target_link_libraries(test-fcl fcl ccd)