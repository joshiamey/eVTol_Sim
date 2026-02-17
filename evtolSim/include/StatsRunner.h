#pragma once

#include "Vehicle.h"
#include <cstdint>
#include <map>
#include <vector>
#include <random>

/// Per-vehicle raw counters (accumulated during simulation)
struct VehicleStats
{
    uint32_t totalFlights = 0;
    uint32_t totalNumFaults = 0;
    uint64_t totalFlightTimeInMs = 0;
    double   totalDistanceMiles = 0.0;
    uint32_t numCharges = 0;
    uint64_t totalChargeTimeInMs = 0;
    uint32_t numWaitingForCharges = 0;
    uint64_t totalWaitForChargingTimeInMs = 0;
};

/// Per-type aggregated + derived stats (computed after simulation)
struct VhTypeAggregateStats
{
    uint64_t numVehicles = 0;
    int      passengersPerVehicle = 0;

    // Totals (summed from individual vehicles)
    uint64_t totalFlights = 0;
    uint64_t totalFaults = 0;
    double   totalDistanceMiles = 0.0;
    uint64_t totalNumCharges = 0;
    uint64_t totalFlightTimeInMs = 0;
    uint64_t totalChargeTimeInMs = 0;
    uint64_t totalNumWaitForCharges = 0;
    uint64_t totalWaitForChargeTimeInMs = 0;

    // Derived (computed from totals)
    double avgFlightTimePerFlightInHrs = 0.0;
    double avgDistancePerFlightInMiles = 0.0;
    double avgTimeChargingPerSessionInHrs = 0.0;
    double avgWaitTimePerChargeInHrs = 0.0;
    double totalPassengerMiles = 0.0;
};

class StatsRunner
{
public:
    StatsRunner(std::mt19937& mtGen);    

    void processFlightOverEvent(const Vehicle* vehicle, uint64_t flightStartTime, uint64_t flightEndTime);
    void processChargeOverEvent(const Vehicle* vehicle, uint64_t chargeStartTime, uint64_t chargeEndTime);
    void processWaitForChargeEvent(const Vehicle* vehicle, uint64_t waitStartTime, uint64_t waitEndTime);   

    void checkFault(const Vehicle* vehicle, uint64_t flightStartTime, uint64_t flightEndTime);

    void addVehicle(const Vehicle* vehicle);

    void evaluateAggregateStats();

    void printAggregateStats();

    inline const VhTypeAggregateStats& getAggregateStatsForType(VhType type) const
    {
        return VhTypeAggregateStatsMap.at(type);
    }

    inline const VehicleStats& getStatsForVehicle(uint32_t vehicleId) const
    {
        return vehicleStatsMap.at(vehicleId);
    }

private:
    std::map<VhType, VhTypeAggregateStats> VhTypeAggregateStatsMap; // Map of vehicle type to its aggregate stats
    std::map<uint32_t, VehicleStats> vehicleStatsMap; // Map of vehicle ID to its stats
    std::map<uint32_t, const Vehicle*> vehicleStore; // Map of vehicle ID to its type for easy lookup
    std::mt19937& mtGen;
};
