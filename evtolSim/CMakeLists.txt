cmake_minimum_required(VERSION 3.10)

project(evtolSim)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(evtolSim src/main.cpp)

target_sources(evtolSim PRIVATE
    src/Vehicle.cpp
    src/EvtolSimulation.cpp
    src/VehicleType.cpp
)
