#include "Vehicle.h"
#include <iostream>


static std::uniform_real_distribution<double> uniformDistDouble{0.0, 1.0};

Vehicle::Vehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability, const std::string& identifier)
    : cruiseSpeed(cruiseSpeed),
      batteryCapacity(batteryCapacity),
      timeToCharge(timeToCharge),
      energyUseAtCruise(energyUseAtCruise),
      passengerCount(passengerCount),
      faultProbabilityPerHour(faultProbability),
      identifier(identifier),
      stats(),
      rd(),
      mtGen(rd())
{
    std::cout << "Vehicle " << identifier << " constructed with parameters." << std::endl;

    range = (batteryCapacity / energyUseAtCruise);
    
    flightTimeInMs = (range / cruiseSpeed) * kHrsToMs;
}


void Vehicle::process(VehicleState state, uint64_t start, uint64_t endTime)
{
    switch (state)
    {
    case VehicleState::FLIGHT_OVER:
    {        
        stats.totalFlights++;
        stats.totalFlightTimeInMs += (endTime - start);
        stats.totalDistance += range;

        checkFault(start,endTime);
        
    }
        /* code */
        break;
    
    case VehicleState::CHARGE_OVER:
        {
            stats.numCharges++;
            stats.totalChargeTimeInMs += (endTime - start);
        }
        break;
        case VehicleState::WAIT_FOR_CHARGING:
        {
            stats.numWaitingForCharges++;
            stats.totalWaitForChargingTimeInMs += (endTime - start);
        }
        break;

    default:
        break;
    }

    currState = state;
}

Vehicle::~Vehicle()
{
    std::cout << "Vehicle " << identifier << " destructed." << std::endl;
}

void Vehicle::checkFault(uint64_t startTime, uint64_t endTime)
{
    double randNum = uniformDistDouble(mtGen);
    double faultProb = faultProbabilityPerHour * static_cast<double>(((endTime - startTime) / kHrsToMs));
    if(randNum <= faultProb)
    {
        stats.totalNumFaults++;
    }
}
