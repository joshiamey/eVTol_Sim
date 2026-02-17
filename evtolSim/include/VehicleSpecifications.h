#pragma once

#include <cstdint>
#include <map>


enum class VhType
{
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO
};

static const uint64_t kHrsToMs = 3600 * 1000; // 1 hour = 3600 * 1000 milliseconds

class VehicleSpecifications
{
public:
    VehicleSpecifications(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, double faultProbability, int passengerCount):
        cruiseSpeed(cruiseSpeed),
        batteryCapacity(batteryCapacity),
        timeToCharge(timeToCharge),
        energyUseAtCruise(energyUseAtCruise),
        faultProbability(faultProbability),
        passengerCount(passengerCount) {}

    inline const double getCruiseSpeed() const
    { 
        return cruiseSpeed;
    }
    inline const double getBatteryCapacity() const
    {
        return batteryCapacity;
    }                   
    inline const double getTimeToCharge() const
    {   
        return timeToCharge;
    }
    inline const double getEnergyUseAtCruise() const
    {       
         return energyUseAtCruise;
    }    

    inline const int getPassengerCount() const
    {
        return passengerCount;
    }

    inline const double getFaultProbabilityPerHour() const
    {
        return faultProbability;
    }

private:
    double cruiseSpeed;
    double batteryCapacity ; 
    double timeToCharge ;
    double energyUseAtCruise;
    double faultProbability ;
    int passengerCount;
};

/// Single source of truth for all vehicle specs — no magic numbers scattered in code
inline const std::map<VhType, VehicleSpecifications>& getVehicleSpecMap()
{
    static const std::map<VhType, VehicleSpecifications> specs = {
        {VhType::ALPHA,   {120, 320, 0.60, 1.6, 0.25, 4}},
        {VhType::BRAVO,   {100, 100, 0.20, 1.5, 0.10, 5}},
        {VhType::CHARLIE, {160, 220, 0.80, 2.2, 0.05, 3}},
        {VhType::DELTA,   { 90, 120, 0.62, 0.8, 0.22, 2}},
        {VhType::ECHO,    { 30, 150, 0.30, 5.8, 0.61, 2}},
    };

    return specs;
}

inline const char* vhTypeToString(VhType type)
{
    switch (type)
    {
        case VhType::ALPHA:   return "Alpha";
        case VhType::BRAVO:   return "Bravo";
        case VhType::CHARLIE: return "Charlie";
        case VhType::DELTA:   return "Delta";
        case VhType::ECHO:    return "Echo";
        default:              return "Unknown";
    }
}