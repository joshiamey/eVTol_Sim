#include "Vehicle.h"
#include <iostream>
#include <cmath>



Vehicle::Vehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability)
    : cruiseSpeed(cruiseSpeed),
      batteryCapacity(batteryCapacity),
      timeToCharge(timeToCharge),
      energyUseAtCruise(energyUseAtCruise),
      passengerCount(passengerCount),
      faultProbabilityPerHour(faultProbability),
      stats(),
      rd(),
      mtGen(rd())
{
    // Calculate range
    range = (batteryCapacity / energyUseAtCruise);
    
    flightTimeInMs = (range / cruiseSpeed) * kHrsToMs;
}


void Vehicle::process(VehicleState state, uint64_t start, uint64_t endTime)
{
    switch (state)
    {
        case VehicleState::FLIGHT_OVER:
        {        
            stats.totalFlights++;
            stats.totalFlightTimeInMs += (endTime - start);
            stats.totalDistance += range;

            checkFault(start,endTime);
            
        }
        /* code */
        break;
    
        case VehicleState::CHARGE_OVER:
            {
                stats.numCharges++;
                stats.totalChargeTimeInMs += (endTime - start);
            }
        break;
        
        case VehicleState::WAIT_FOR_CHARGING:
            {
                stats.numWaitingForCharges++;
                stats.totalWaitForChargingTimeInMs += (endTime - start);
            }
        break;

    default:
        break;
    }

    currState = state;
}

void Vehicle::checkFault(uint64_t startTime, uint64_t endTime)
{
    // Calculate the mean (average) number of faults expected for this specific flight duration.
    double flightDurationInHrs = static_cast<double>((endTime - startTime) / kHrsToMs);
    double mean_faults_for_flight = faultProbabilityPerHour * flightDurationInHrs;

    // Create a new Poisson distribution specifically for this flight's duration.
    // Note: The distribution object itself is very lightweight.
    std::poisson_distribution<int> dist(mean_faults_for_flight);

    // Generate the number of faults that occurred during this flight.
    int num_faults = dist(mtGen);

    // Add the generated number of faults to the stats.
    if (num_faults > 0)
    {
        stats.totalNumFaults += num_faults;
    }
}
