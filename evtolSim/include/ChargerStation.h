# pragma once

#include <queue>
#include "Vehicle.h"

class ChargerStation
{
public:

    ChargerStation(int numChargers);

    inline bool isChargerAvailable() const
    {
        return chargersInUse < numChargers;
    }

    bool acquireCharger(Vehicle* vehicle, uint64_t currentTime);
    
    Vehicle* releaseCharger(uint64_t *chargeWaitTime);

    inline int getNumChargersInUse() const
    {
        return chargersInUse;
    }

private:
    int numChargers;
    int chargersInUse;
    std::queue<std::pair<uint64_t,Vehicle*>>chargingQueue;
};