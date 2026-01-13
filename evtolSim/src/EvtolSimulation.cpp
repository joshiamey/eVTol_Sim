#include "EvtolSimulation.h"
#include <algorithm>
#include <random>

using namespace std;

EvtolSimulation::EvtolSimulation(int numVehicleTypes, int vehicleCount, int chargerCount, double durationInHrs):
    numVehicleTypes(numVehicleTypes),
    numVehicles(vehicleCount),
    numChargers(chargerCount),
    simDurationInHrs(durationInHrs),
    simVehicles(),
    rd(),
    mtGen(rd())
{
    simDurationInMs = durationInHrs * kHrsInMs;

    int numVehiclesPerType = (numVehicles/numVehicleTypes);

    // Generate list of vehicles
    for(int j = 1 ; j <=numVehiclesPerType ; ++j)
    {
        simVehicles.emplace_back(new Vehicle(120,320,0.6,1.6,4,0.25,"Alpha"));
    }

    for(int j = 1 ; j <=numVehiclesPerType ; ++j)
    {
        simVehicles.emplace_back(new Vehicle(100,100,0.2,1.5,5,0.10,"Bravo"));
    }

    for(int j = 1 ; j <=numVehiclesPerType ; ++j)
    {
        simVehicles.emplace_back(new Vehicle(160,220,0.8,2.2,3,0.05,"Charlie"));
    }

    for(int j = 1 ; j <=numVehiclesPerType ; ++j)
    {
        simVehicles.emplace_back(new Vehicle(90,120,0.62,0.8,2,0.22,"Delta"));
    }

    for(int j = 1 ; j <=numVehiclesPerType ; ++j)
    { 
        simVehicles.emplace_back(new Vehicle(30,150,0.3,5.8,2,0.61,"Echo"));
    } 

    // Randomize the vehicle list
    std::shuffle(simVehicles.begin(), simVehicles.end(), mtGen);
}

void EvtolSimulation::initializeSimulation()
{
    //Initialize Simulation
    //Schedule Flight over events based on start clock of 0.0 hours
    //for all vehicles

    for(auto& vehicle : simVehicles)
    {
        eventQueue.emplace(EventType::FLIGHT_OVER,vehicle->getFlightTimeInMs(),vehicle);
    }
}

void EvtolSimulation::runSimulation()
{
    uint32_t chargersInUse = 0;
    uint64_t current = 0;

    while(!eventQueue.empty())
    {
        VehicleSimEvent event = eventQueue.top();
        eventQueue.pop();
        
        current = event.getEventTime();
        if (current > simDurationInMs) {
            break; 
        }

        Vehicle* vehicle = event.getVehicle();

        switch (event.getEventType())
        {
            case EventType::FLIGHT_OVER:
            {
                uint64_t flightStartTime = current - vehicle->getFlightTimeInMs();
                vehicle->process(VehicleState::FLIGHT_OVER, flightStartTime, current);

                if(chargersInUse < numChargers)
                {
                    ++chargersInUse;
                    uint64_t chargeFinishTime = current + vehicle->getChargeTimeInMs();
                    eventQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, vehicle);
                }
                else
                {
                    chargingQueue.emplace(std::make_pair(current, vehicle));
                }
            }
            break;
                
            case EventType::CHARGE_OVER:
            {
                uint64_t chargeStartTime = current - vehicle->getChargeTimeInMs();
                vehicle->process(VehicleState::CHARGE_OVER, chargeStartTime, current);

                eventQueue.emplace(EventType::FLIGHT_OVER, current + vehicle->getFlightTimeInMs(), vehicle);
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
                        eventQueue.emplace(EventType::CHARGE_OVER, chargeFinishTime, waitVehicle);
                    }
                }
            }
            break;
        
            default:
                break;
        }
    }
}

EvtolSimulation::~EvtolSimulation()
{
    for (auto* p : simVehicles)
    {
        delete p;
    }
}
