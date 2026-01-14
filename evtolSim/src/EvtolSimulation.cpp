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


    int numVehiclesPerType = (numVehicles/static_cast<int>(vhTypeMap.size()));

    for(auto& elem : vhTypeMap)
    {
        VhType vType = elem.first;
        VehicleType& vhType = elem.second;
        for(int j = 1 ; j <=numVehiclesPerType ; ++j)
        {
            switch (vType)
            {
                case VhType::ALPHA:
                {
                    vhType.addVehicle(120,320,0.6,1.6,4,0.25,"Alpha");                    
                    break;
                }
                case VhType::BRAVO:
                {
                    vhType.addVehicle(100,100,0.2,1.5,5,0.10,"Bravo");                    
                    break;
                }
                case VhType::CHARLIE:
                {
                    vhType.addVehicle(160,220,0.8,2.2,3,0.05,"Charlie");                    
                    break;
                }
                case VhType::DELTA:
                {
                    vhType.addVehicle(90,120,0.62,0.8,2,0.22,"Delta");                    
                    break;
                }
                case VhType::ECHO:
                {
                    vhType.addVehicle(30,150,0.3,5.8,2,0.61,"Echo");                    
                    break;
                }
                default:
                    break;
            }
        }
    } 

    for(auto& elem : vhTypeMap)
    {
        VehicleType& vhType = elem.second;

        for(auto& vehicle : vhType.getVehicleList())
        {
            // get the raw pointer
            simVehicles.push_back(vehicle.get());
        }
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

    // Print the stats after the simulation run

    for(auto& elem : vhTypeMap)
    {
        VehicleType& vhType = elem.second;
        // TODO separate evaluate and print
        vhType.evaluateAndPrintStats();
    }

EvtolSimulation::~EvtolSimulation()
{

}
