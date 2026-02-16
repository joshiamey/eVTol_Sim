#include "EvtolSimulation.h"
#include <algorithm>
#include <random>

using namespace std;

EvtolSimulation::EvtolSimulation(int vehicleCount, int chargerCount, double durationInHrs):
    numVehicles(vehicleCount),
    numChargers(chargerCount),
    simDurationInHrs(durationInHrs),
    simVehicles(),
    rd(),
    mtGen(rd()),
    vhTypeMap()
{
    simDurationInMs = durationInHrs * kHrsInMs;
        // Add vehicle types
    vhTypeMap.emplace(VhType::ALPHA, VhType::ALPHA);
    vhTypeMap.emplace(VhType::BRAVO, VhType::BRAVO);
    vhTypeMap.emplace(VhType::CHARLIE, VhType::CHARLIE);
    vhTypeMap.emplace(VhType::DELTA, VhType::DELTA);
    vhTypeMap.emplace(VhType::ECHO, VhType::ECHO);


    std::vector<VhType> vehicleTypes;
    for (auto const& pair : vhTypeMap)
    {
        vehicleTypes.push_back(pair.first);
    }

    static const std::map<VhType, VehicleSpecifications> kVehicleSpecMap = {
    {VhType::ALPHA, {120, 320, 0.6, 1.6, 0.25, 4}},
    {VhType::BRAVO, {100, 100, 0.2, 1.5, 0.10, 5}},
    {VhType::CHARLIE, {160, 220, 0.8, 2.2, 0.05, 3}},
    {VhType::DELTA, {90, 120, 0.62, 0.8, 0.22, 2}},
    {VhType::ECHO, {30, 150, 0.3, 5.8, 0.61, 2}}
    };
    // Create uniform distribution based on number of types of vehicles
    std::uniform_int_distribution<int> dist(0, static_cast<int>(vehicleTypes.size() - 1));

    for (int i = 0; i < numVehicles; ++i)
    {
        // Get a type based on random number generated
        // and add the vehicle of that type
        VhType vType = vehicleTypes[dist(mtGen)];
        VehicleType& vhType = vhTypeMap.at(vType);
        const VehicleSpecifications& spec = kVehicleSpecMap.at(vType);

        simVehicles.push_back(make_unique<Vehicle>(spec));
        vhType.addVehicle(simVehicles.back().get());
    } 
}

void EvtolSimulation::initializeSimulation()
{
    //Initialize Simulation
    //Schedule Flight over events based on start clock of 0.0 hours
    //for all vehicles

    for(auto& vehicle : simVehicles)
    {
        eventPriorityQueue.emplace(EventType::FLIGHT_OVER,vehicle->getFlightTimeInMs(),vehicle.get());
    }
}

void EvtolSimulation::runSimulation()
{
    uint32_t chargersInUse = 0;
    uint64_t current = 0;

    // Process events added to the priority queue, events are added based on time 
    // one with least time will be processed
    while(!eventPriorityQueue.empty())
    {
        VehicleSimEvent event = eventPriorityQueue.top();
        eventPriorityQueue.pop();
        
        current = event.getEventTime();
        if (current > simDurationInMs) {
            break; 
        }

        Vehicle* vehicle = event.getVehicle();

        switch (event.getEventType())
        {   

            // if flight over event , get start time and calculate the stats
            // also calculate the fault (check vehicle process function)
            // Add charge over event next or add the vehicle to the charging queue
            case EventType::FLIGHT_OVER:
            {
                uint64_t flightStartTime = current - vehicle->getFlightTimeInMs();
                vehicle->process(VehicleState::FLIGHT_OVER, flightStartTime, current);

                if(chargersInUse < numChargers)
                {
                    ++chargersInUse;
                    uint64_t chargeFinishTime = current + vehicle->getChargeTimeInMs();
                    eventPriorityQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, vehicle);
                }
                else
                {
                    chargingQueue.emplace(std::make_pair(current, vehicle));
                }
            }
            break;
            
            // get start time and calculate charge stats based on time
            // if charger becomes available , pick the vehicle in front of the queue
            // process wait charge time stats for that waiting vehicle
            
            case EventType::CHARGE_OVER:
            {
                uint64_t chargeStartTime = current - vehicle->getChargeTimeInMs();
                vehicle->process(VehicleState::CHARGE_OVER, chargeStartTime, current);

                eventPriorityQueue.emplace(EventType::FLIGHT_OVER, current + vehicle->getFlightTimeInMs(), vehicle);
                --chargersInUse;

                if(!chargingQueue.empty())
                {
                    if (chargersInUse < numChargers)
                    {
                        auto waitForChargePair = chargingQueue.front();
                        chargingQueue.pop();
                        
                        uint64_t waitStartTime = waitForChargePair.first;
                        Vehicle* waitVehicle = waitForChargePair.second;

                        waitVehicle->process(VehicleState::WAIT_FOR_CHARGING, waitStartTime, current);
                        
                        ++chargersInUse;
                        uint64_t chargeFinishTime = current + waitVehicle->getChargeTimeInMs();
                        eventPriorityQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, waitVehicle);
                    }
                }
            }
            break;
        
            default:
                break;
        }
    }

    // Print the stats after the simulation run

    for(auto& elem : vhTypeMap)
    {
        VehicleType& vhType = elem.second;
        vhType.evaluateStats();
        vhType.printStats();
    }
}

