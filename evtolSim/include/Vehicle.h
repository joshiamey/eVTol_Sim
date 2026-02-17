#ifndef VEHICLE_H
#define VEHICLE_H
#include <cstdint>
#include <string>
#include <random>
#include "VehicleSpecifications.h"


class Vehicle 
{
public:
    // Constructors
    Vehicle(uint32_t id,VhType type, const VehicleSpecifications& spec);
    int getId() const { return vehicleId; }
    VhType getType() const { return vehicleType; }

    uint64_t getFlightTimeInMs() const { return flightTimeInMs;}
    uint64_t getChargeTimeInMs() const { return chargeTimeInMs; }
    double getRange() const { return range; }
    int getPassengerCount() const { return specifications.getPassengerCount(); }
    double getFaultProbabilityPerHour() const { return specifications.getFaultProbabilityPerHour(); }
    // Destructor
    ~Vehicle() = default;

private:

    const VehicleSpecifications& specifications;
    uint32_t vehicleId;
    VhType vehicleType;
    double range;
    uint64_t flightTimeInMs;
    uint64_t chargeTimeInMs;
};

#endif // VEHICLE_H
