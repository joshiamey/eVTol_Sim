cmake_minimum_required(VERSION 3.10)

project(evtolSim)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(evtolSim/include)

add_executable(evtolSim evtolSim/src/main.cpp evtolSim/src/vehicle.cpp)
