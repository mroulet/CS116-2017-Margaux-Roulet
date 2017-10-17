cmake_minimum_required (VERSION 2.6)
project (neuron)

set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11")
add_executable (neuron test_twoNeuron.cpp neuron.cpp neuron.hpp)
