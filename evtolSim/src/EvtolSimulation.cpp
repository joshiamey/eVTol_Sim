#include "EvtolSimulation.h"
#include "Vehicle.h"
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
    statsRunner(mtGen)
{
    simDurationInMs = durationInHrs * kHrsToMs;
    const auto& specificationsmap = getVehicleSpecMap();


    std::vector<VhType> vehicleTypes;
    for (auto const& pair : specificationsmap)
    {
        vehicleTypes.push_back(pair.first);
    }

    // Create uniform distribution based on number of types of vehicles
    std::uniform_int_distribution<int> dist(0, static_cast<int>(vehicleTypes.size() - 1));

    for (int i = 0; i < numVehicles; ++i)
    {
        // Get a type based on random number generated
        // and add the vehicle of that type
        VhType vType = vehicleTypes[dist(mtGen)];
        const VehicleSpecifications& spec = specificationsmap.at(vType);

        simVehicles.push_back(make_unique<Vehicle>(i, vType, spec));
        statsRunner.addVehicle(simVehicles.back().get());
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
                statsRunner.processFlightOverEvent(vehicle, flightStartTime, current);

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
                statsRunner.processChargeOverEvent(vehicle, chargeStartTime, current);

                eventPriorityQueue.emplace(EventType::FLIGHT_OVER, current + vehicle->getFlightTimeInMs(), vehicle);
                --chargersInUse;

                if(!chargingQueue.empty())
                {

                    auto waitForChargePair = chargingQueue.front();
                    chargingQueue.pop();
                    
                    uint64_t waitStartTime = waitForChargePair.first;
                    Vehicle* waitVehicle = waitForChargePair.second;

                    statsRunner.processWaitForChargeEvent(waitVehicle, waitStartTime, current);
                    
                    ++chargersInUse;
                    uint64_t chargeFinishTime = current + waitVehicle->getChargeTimeInMs();
                    eventPriorityQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, waitVehicle);                    
                }
            }
            break;
        
            default:
                break;
        }
    }

    // Print the stats after the simulation run

    statsRunner.evaluateAggregateStats();
    statsRunner.printAggregateStats();
}

