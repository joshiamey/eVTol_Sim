#ifndef EVTOL_SIMULATION_H
#define EVTOL_SIMULATION_H

#include "VehicleSimEvent.h"
#include <queue>
#include <vector>
#include <functional>
#include <iostream>
#include <random>
#include <unordered_set>

class EvtolSimulation
{
    public:

        EvtolSimulation(int numVehicleTypes,int vehicleCount,int chargerCount,double durationInHrs);

        void initializeSimulation();

        void runSimulation();

        ~EvtolSimulation();

    private:
        const uint64_t kHrsInMs = 3600 * 1000;
        double simDurationInHrs; // amount of time to simulate
        uint64_t simDurationInMs;
        int numVehicles; // total vehicles to be simulated
        int numChargers;
        int numVehicleTypes; // Different Types of vehicles
        // Min-heap of simulation events, ordered by time
        std::priority_queue<VehicleSimEvent, std::vector<VehicleSimEvent>, std::greater<VehicleSimEvent>> eventQueue;
        std::vector<Vehicle*> simVehicles;    
        std::queue<std::pair<uint64_t,Vehicle*>>chargingQueue;
        std::random_device rd;
        std::mt19937 mtGen;

};

#endif // EVTOL_H
