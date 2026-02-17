#include "Vehicle.h"
#include <iostream>
#include <cmath>

Vehicle::Vehicle(uint32_t id, VhType type, const VehicleSpecifications &spec):
    specifications(spec),
    vehicleId(id),
    vehicleType(type)
{
    // Calculate range
    range = (spec.getBatteryCapacity() / spec.getEnergyUseAtCruise());
    
    flightTimeInMs = (range / spec.getCruiseSpeed()) * kHrsToMs;
    chargeTimeInMs = spec.getTimeToCharge() * kHrsToMs;
}
