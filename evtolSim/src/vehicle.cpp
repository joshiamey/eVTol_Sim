#include "vehicle.h"
#include <iostream>

Vehicle::Vehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability, const std::string& identifier)
    : cruiseSpeed(cruiseSpeed),
      batteryCapacity(batteryCapacity),
      timeToCharge(timeToCharge),
      energyUseAtCruise(energyUseAtCruise),
      passengerCount(passengerCount),
      faultProbability(faultProbability),
      identifier(identifier)
{
    std::cout << "Vehicle " << identifier << " constructed with parameters." << std::endl;

    range = (batteryCapacity / energyUseAtCruise);
    
    flightTime = (range / cruiseSpeed);
}

Vehicle::~Vehicle()
{
    std::cout << "Vehicle " << identifier << " destructed." << std::endl;
}