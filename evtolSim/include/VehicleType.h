#pragma once

#include "Vehicle.h"
#include <vector>
#include <memory>

enum class VhType
{
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO
};

struct VhTypeStats
{
    VhTypeStats() : avgFlightTimePerFlightInHrs(0.0),
                    avgDistancePerFlightInMiles(0.0),
                    avgTimeChargingPerSessionInHrs(0.0),
                    avgWaitTimePerChargeInHrs(0.0),
                    totalFaults(0),
                    totalPassengerMiles(0),
                    totalFlights(0),
                    totalFlightTimeInMs(0),
                    totalDistanceMiles(0.0),
                    totalWaitForChargeTimeInMs(0),
                    totalNumCharges(0.0),
                    totalChargeTimeInMs(0),
                    totalNumWaitForCharges(0),
                    passengersPerVehicle(0),
                    numVehicles(0) {}
    double avgFlightTimePerFlightInHrs;
    double avgDistancePerFlightInMiles;
    double avgTimeChargingPerSessionInHrs;
    double avgWaitTimePerChargeInHrs;
    uint64_t totalFaults;
    uint64_t totalPassengerMiles;

    uint64_t totalFlights;
    uint64_t totalFlightTimeInMs;
    double totalDistanceMiles;
    uint64_t totalWaitForChargeTimeInMs;
    double totalNumCharges;
    uint64_t totalChargeTimeInMs;
    uint64_t totalNumWaitForCharges;
    uint32_t passengersPerVehicle;
    uint64_t numVehicles;
};

class VehicleType
{
    public:
        VehicleType(VhType type);

        void addVehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability);

        void evaluateAndPrintStats();

        std::vector<std::unique_ptr<Vehicle>>& getVehicleList();

    private:
        const char* getStringFromType();
        const uint64_t kHrsToMs = 3600 * 1000; // 1 hour = 3600 * 1000 milliseconds
        VhType vehicleType;
        VhTypeStats typeStats;
        std::vector<std::unique_ptr<Vehicle>> vehicleList;        
};