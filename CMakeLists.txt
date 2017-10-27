cmake_minimum_required (VERSION 2.6)
project (neuron)

set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR} include ${gtest_SOURCE_DIR})

add_executable (neuron src/network.cpp src/neuron.cpp src/test_twoNeurons.cpp)
add_executable (neuron_unittest src/neuron.cpp src/network.cpp gtest/neuron_unittest.cpp)

target_link_libraries(neuron_unittest gtest gtest_main)
add_test(neuron_unittest neuron_unittest)
