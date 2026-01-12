#ifndef SIM_SCHEDULER_H
#define SIM_SCHEDULER_H

// This class implements Simulation scheduler , it is event based scheduler 

enum class EventType
{
    FLIGHT_OVER,
    CHARGE_OVER,
    FAULT
};

class SimEvent
{
    EventType eventType; // Type of evTol simulation event
    double eventTime; // Time at which event will be processed
};

class SimScheduler
{

};

#endif