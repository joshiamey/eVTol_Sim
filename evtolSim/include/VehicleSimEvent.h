#ifndef SIM_SCHEDULER_H
#define SIM_SCHEDULER_H

#include <cstdint>
#include "Vehicle.h"

enum class EventType
{
    FLIGHT_OVER,
    CHARGE_OVER,
    FAULT
};

class VehicleSimEvent
{
public:
    VehicleSimEvent(EventType type, uint64_t time, Vehicle* vh)
        : eventType(type), eventTimeInMs(time), vehicle(vh) {}

    EventType getEventType() const { return eventType; }
    uint64_t getEventTime() const { return eventTimeInMs; }
    Vehicle* getVehicle() const { return vehicle; }

    // For comparison purpose
    bool operator>(const VehicleSimEvent& other) const
    {
        return eventTimeInMs > other.eventTimeInMs;
    }

private:
    EventType eventType; // Type of evTol simulation event
    uint64_t eventTimeInMs; // Time at which event will be processed
    Vehicle* vehicle;
};


#endif