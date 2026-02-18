#include "EvtolSimulation.h"
#include "Vehicle.h"
#include <algorithm>
#include <random>

using namespace std;

EvtolSimulation::EvtolSimulation(int vehicleCount, int chargerCount, double durationInHrs):
    numVehicles(vehicleCount),
    simDurationInHrs(durationInHrs),
    simVehicles(),
    rd(),
    mtGen(rd()),
    statsRunner(mtGen),
    chargerStation(chargerCount)
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
    uint64_t currentTime = 0;

    // Process events added to the priority queue, events are added based on time 
    // one with least time will be processed
    while(!eventPriorityQueue.empty())
    {
        VehicleSimEvent event = eventPriorityQueue.top();
        eventPriorityQueue.pop();
        
        currentTime = event.getEventTime();
        if (currentTime > simDurationInMs) {
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
                uint64_t flightStartTime = currentTime - vehicle->getFlightTimeInMs();
                statsRunner.processFlightOverEvent(vehicle, flightStartTime, currentTime);

                if(chargerStation.acquireCharger(vehicle,currentTime))
                {
                    uint64_t chargeFinishTime = currentTime + vehicle->getChargeTimeInMs();
                    eventPriorityQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, vehicle);
                }
            }
            break;
            
            // get start time and calculate charge stats based on time
            // if charger becomes available , pick the vehicle in front of the queue
            // process wait charge time stats for that waiting vehicle
            
            case EventType::CHARGE_OVER:
            {
                uint64_t chargeStartTime = currentTime - vehicle->getChargeTimeInMs();
                statsRunner.processChargeOverEvent(vehicle, chargeStartTime, currentTime);

                eventPriorityQueue.emplace(EventType::FLIGHT_OVER, currentTime + vehicle->getFlightTimeInMs(), vehicle);

                uint64_t waitStartTime = 0;
                Vehicle* waitVehicle = chargerStation.releaseCharger(&waitStartTime);

                if(waitVehicle != nullptr)
                {
                    statsRunner.processWaitForChargeEvent(waitVehicle, waitStartTime, currentTime);
                    
                    // Enqueue charge over event for that wait vehicle
                    if(chargerStation.acquireCharger(waitVehicle,currentTime))
                    {
                        uint64_t chargeFinishTime = currentTime + waitVehicle->getChargeTimeInMs();
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

    statsRunner.evaluateAggregateStats();
    statsRunner.printAggregateStats();
}

