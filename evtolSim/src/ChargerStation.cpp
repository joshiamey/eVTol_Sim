#include "ChargerStation.h"

using namespace std;

ChargerStation::ChargerStation(int numChargers):
    numChargers(numChargers),
    chargersInUse(0),
    chargingQueue()
{
}

bool ChargerStation::acquireCharger(Vehicle* vehicle, uint64_t currentTime)
{
    if (isChargerAvailable())
    {
        ++chargersInUse;
        return true;
    }

    chargingQueue.emplace(currentTime, vehicle);
    return false;    
}

Vehicle* ChargerStation::releaseCharger(uint64_t *chargeWaitTime)
{
    --chargersInUse;

    if(!chargingQueue.empty())
    {
        auto& pair = chargingQueue.front();
        chargingQueue.pop();
        *chargeWaitTime = pair.first;

        return pair.second;
    }

    return nullptr;
}