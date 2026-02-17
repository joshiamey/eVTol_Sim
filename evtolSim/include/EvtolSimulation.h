#ifndef EVTOL_SIMULATION_H
#define EVTOL_SIMULATION_H

#include "VehicleSimEvent.h"
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <map>
#include "StatsRunner.h"

class EvtolSimulation
{
    public:

        EvtolSimulation(int vehicleCount,int chargerCount,double durationInHrs);

        void initializeSimulation();

        void runSimulation();

        ~EvtolSimulation() = default;

    private:
        double simDurationInHrs; // amount of time to simulate
        uint64_t simDurationInMs;
        int numVehicles; // total vehicles to be simulated
        int numChargers;
        // Min-heap of simulation events, ordered by time
        std::priority_queue<VehicleSimEvent, std::vector<VehicleSimEvent>, std::greater<VehicleSimEvent>> eventPriorityQueue;
        std::vector<std::unique_ptr<Vehicle>> simVehicles;
        std::queue<std::pair<uint64_t,Vehicle*>>chargingQueue;
        StatsRunner statsRunner;
        std::random_device rd;
        std::mt19937 mtGen;
};

#endif // EVTOL_H
