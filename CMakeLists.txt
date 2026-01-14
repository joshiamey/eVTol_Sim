cmake_minimum_required(VERSION 3.10)

project(evtolSim)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(evtolSim/include)

add_executable(evtolSim evtolSim/src/main.cpp)

target_sources(evtolSim PRIVATE
    evtolSim/src/Vehicle.cpp
    evtolSim/src/EvtolSimulation.cpp
    evtolSim/src/VehicleType.cpp
)
