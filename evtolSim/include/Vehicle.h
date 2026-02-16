#ifndef VEHICLE_H
#define VEHICLE_H
#include <cstdint>
#include <string>
#include <random>


enum class VehicleState
{
    FLIGHT_OVER,
    WAIT_FOR_CHARGING,
    CHARGE_OVER
};

struct VehicleStats
{
    // required status
    uint32_t totalNumFaults;
    uint32_t totalFlights;
    uint64_t totalFlightTimeInMs;
    double totalDistance;
    uint64_t totalWaitForChargingTimeInMs;
    uint32_t numWaitingForCharges;
    uint32_t numCharges;
    uint64_t totalChargeTimeInMs;

    VehicleStats() : totalNumFaults(0), totalFlights(0), totalFlightTimeInMs(0), totalDistance(0), totalWaitForChargingTimeInMs(0), numWaitingForCharges(0), numCharges(0), totalChargeTimeInMs(0) {}

};

struct VehicleSpecifications {
    double cruiseSpeed, batteryCapacity, timeToCharge;
    double energyUseAtCruise, faultProbability;
    int passengerCount;
};



class Vehicle 
{
public:
    // Constructors
    Vehicle(const VehicleSpecifications& spec);

    // Returns the Range of the Vehicle based on energyUse and battery capacity
    inline const double getRange() const
    {
        return range;
    }

    // Get flight time , time taken to cover entire range at the given cruise speed
    inline const uint64_t getFlightTimeInMs() const
    {
        return flightTimeInMs;
    }

    inline const uint64_t getChargeTimeInMs() const
    {
        return timeToCharge * kHrsToMs;
    }

    inline const double getFaultPerHour() const 
    {
        return faultProbabilityPerHour;
    }

    inline const int getPassengerCount() const
    {
        return passengerCount;
    }

    inline const VehicleStats& getStats() const
    {
        return stats;
    }

    void process(VehicleState state, uint64_t start, uint64_t endTime) ;
    // Destructor
    ~Vehicle() = default;

private:

    void checkFault(uint64_t startTime, uint64_t endTime);
    const uint64_t kHrsToMs = 3600 * 1000; // 1 hour = 3600 * 1000 milliseconds
    double cruiseSpeed;
    double batteryCapacity;
    double timeToCharge;
    double energyUseAtCruise;
    int passengerCount;
    double faultProbabilityPerHour;
    VehicleState currState;
    double range;
    uint64_t flightTimeInMs;
    std::random_device rd;
    std::mt19937 mtGen;
    VehicleStats stats;
};

#endif // VEHICLE_H
