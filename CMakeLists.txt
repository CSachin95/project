cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

project(Project)

add_executable(Project "Main.cpp" "Simulator.h" "Matrix.h" "Plotter.h"
                "VoltageSource.h" "Resistor.h" "Diode.h" "Battery.h")
