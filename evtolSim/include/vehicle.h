#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle 
{
public:
    // Constructors
    Vehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability, const std::string& identifier);

    // Returns the Range of the Vehicle based on energyUse and battery capacity
    inline double getRange()
    {
        return range;
    }

    // Get flight time , time taken to cover entire range at the given cruise speed
    inline double getFlightTime()
    {
        return flightTime;
    }

    inline double getChargeTime()
    {
        return timeToCharge;
    }

    inline double getFaultPerHour()
    {
        return faultProbability;
    }
    // Destructor
    ~Vehicle();

private:
    double cruiseSpeed;
    double batteryCapacity;
    double timeToCharge;
    double energyUseAtCruise;
    int passengerCount;
    double faultProbability;
    std::string identifier;
    double range;
    double flightTime;
};

#endif // VEHICLE_H
