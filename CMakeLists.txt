cmake_minimum_required (VERSION 2.6)
project (neuron)

set(CMAKE_CXX_FLAGS "-03 -W -Wall -pedantic -std=c++11")
add_executable (neuron test_oneNeuron.cpp neuron.cpp neuron.hpp)
