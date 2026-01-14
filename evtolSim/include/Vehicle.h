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

typedef struct VehicleStats_t
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

} VehicleStats;

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
    inline uint64_t getFlightTimeInMs()
    {
        return flightTimeInMs;
    }

    inline uint64_t getChargeTimeInMs()
    {
        return timeToCharge * kHrsToMs;
    }

    inline double getFaultPerHour()
    {
        return faultProbabilityPerHour;
    }

    void process(VehicleState state, uint64_t start, uint64_t endTime) ;
    // Destructor
    ~Vehicle();

    VehicleStats stats;

private:

    void checkFault(uint64_t startTime, uint64_t endTime);
    const uint64_t kHrsToMs = 3600 * 1000; // 1 hour = 3600 * 1000 milliseconds
    double cruiseSpeed;
    double batteryCapacity;
    double timeToCharge;
    double energyUseAtCruise;
    int passengerCount;
    double faultProbabilityPerHour;
    std::string identifier;
    VehicleState currState;
    double range;
    uint64_t flightTimeInMs;
    std::random_device rd;
    std::mt19937 mtGen;
};

#endif // VEHICLE_H
