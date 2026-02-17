#include <iostream>
#include "StatsRunner.h"

StatsRunner::StatsRunner(std::mt19937& mtGen):
    VhTypeAggregateStatsMap(),
    vehicleStatsMap(),
    vehicleStore(),
    mtGen(mtGen)
{
}

void StatsRunner::processFlightOverEvent(const Vehicle *vehicle, uint64_t flightStartTime, uint64_t flightEndTime)
{
    uint32_t vehicleId = vehicle->getId();
    VehicleStats& stats = vehicleStatsMap[vehicleId];

    stats.totalFlights++;
    stats.totalFlightTimeInMs += (flightEndTime - flightStartTime);
    stats.totalDistanceMiles += vehicle->getRange();

    checkFault(vehicle, flightStartTime, flightEndTime);
}

void StatsRunner::processChargeOverEvent(const Vehicle *vehicle, uint64_t chargeStartTime, uint64_t chargeEndTime)
{
    uint32_t vehicleId = vehicle->getId();
    VehicleStats& stats = vehicleStatsMap[vehicleId];

    stats.numCharges++;
    stats.totalChargeTimeInMs += (chargeEndTime - chargeStartTime);
}

void StatsRunner::processWaitForChargeEvent(const Vehicle *vehicle, uint64_t waitStartTime, uint64_t waitEndTime)
{
    uint32_t vehicleId = vehicle->getId();
    VehicleStats& stats = vehicleStatsMap[vehicleId];

    stats.numWaitingForCharges++;
    stats.totalWaitForChargingTimeInMs += (waitEndTime - waitStartTime);
}

void StatsRunner::checkFault(const Vehicle *vehicle, uint64_t flightStartTime, uint64_t flightEndTime)
{
    // Calculate the mean (average) number of faults expected for this specific flight duration.
    double flightDurationInHrs = static_cast<double>(flightEndTime - flightStartTime) / static_cast<double>(kHrsToMs);
    double mean_faults_for_flight = vehicle->getFaultProbabilityPerHour() * flightDurationInHrs;

    // Create a new Poisson distribution specifically for this flight's duration.
    // Note: The distribution object itself is very lightweight.
    std::poisson_distribution<int> dist(mean_faults_for_flight);

    // Generate the number of faults that occurred during this flight.
    int num_faults = dist(mtGen);

    // Add the generated number of faults to the stats.
    if (num_faults > 0)
    {
        uint32_t vehicleId = vehicle->getId();
        VehicleStats& stats = vehicleStatsMap[vehicleId];
        stats.totalNumFaults += num_faults;
    }
}

void StatsRunner::addVehicle(const Vehicle *vehicle)
{
    uint32_t vehicleId = vehicle->getId();
    VhType vehicleType = vehicle->getType();

    // Store the vehicle for easy lookup later
    vehicleStore[vehicleId] = vehicle;

    vehicleStatsMap[vehicleId] = VehicleStats(); // Initialize stats for this vehicle

    // If this is the first time we see this vehicle type, initialize its aggregate stats
    if (VhTypeAggregateStatsMap.find(vehicleType) == VhTypeAggregateStatsMap.end())
    {
        VhTypeAggregateStatsMap[vehicleType] = VhTypeAggregateStats();
    }

    // Increment the count of vehicles for this type and set passengers per vehicle
    VhTypeAggregateStats& aggStats = VhTypeAggregateStatsMap[vehicleType];
    aggStats.numVehicles++;
    aggStats.passengersPerVehicle = vehicle->getPassengerCount();

}

void StatsRunner::evaluateAggregateStats()
{
    for (const auto& pair : vehicleStatsMap)
    {
        const Vehicle* vehicle = vehicleStore.at(pair.first);
        const VehicleStats& stats = pair.second;
        VhType type = vehicle->getType();
        VhTypeAggregateStats& agg = VhTypeAggregateStatsMap[type];

        agg.numVehicles++;
        agg.passengersPerVehicle = vehicle->getPassengerCount();

        agg.totalFlights += stats.totalFlights;
        agg.totalFaults += stats.totalNumFaults;
        agg.totalDistanceMiles += stats.totalDistanceMiles;
        agg.totalNumCharges += stats.numCharges;
        agg.totalFlightTimeInMs += stats.totalFlightTimeInMs;
        agg.totalChargeTimeInMs += stats.totalChargeTimeInMs;
        agg.totalNumWaitForCharges += stats.numWaitingForCharges;
        agg.totalWaitForChargeTimeInMs += stats.totalWaitForChargingTimeInMs;
    }

    for (auto& pair : VhTypeAggregateStatsMap)
    {
        VhTypeAggregateStats& agg = pair.second;

        if (agg.totalFlights > 0)
        {
            agg.avgDistancePerFlightInMiles = agg.totalDistanceMiles / static_cast<double>(agg.totalFlights);
            agg.avgFlightTimePerFlightInHrs = (static_cast<double>(agg.totalFlightTimeInMs) / static_cast<double>(agg.totalFlights)) / kHrsToMs;
        }

        if (agg.totalNumCharges > 0)
        {
            agg.avgTimeChargingPerSessionInHrs = (static_cast<double>(agg.totalChargeTimeInMs) / static_cast<double>(agg.totalNumCharges)) / kHrsToMs;
        }

        if (agg.totalNumWaitForCharges > 0)
        {
            agg.avgWaitTimePerChargeInHrs = (static_cast<double>(agg.totalWaitForChargeTimeInMs) / static_cast<double>(agg.totalNumWaitForCharges)) / kHrsToMs;
        }

        agg.totalPassengerMiles = agg.totalDistanceMiles * agg.passengersPerVehicle;
    }
}

void StatsRunner::printAggregateStats()
{
    for (const auto& pair : VhTypeAggregateStatsMap)
    {
        VhType type = pair.first;
        const VhTypeAggregateStats& agg = pair.second;

        std::cout << "=================================================" << std::endl;
        std::cout << "Statistics for vehicle type: " << vhTypeToString(type) << std::endl; // You can replace this with a proper string representation
        std::cout << "=================================================" << std::endl;
        std::cout << "Number of vehicles: " << agg.numVehicles << std::endl;
        std::cout << "Passengers per vehicle: " << agg.passengersPerVehicle << std::endl;
        std::cout << "Total passenger miles: " << agg.totalPassengerMiles << std::endl;
        std::cout << "Total flights: " << agg.totalFlights << std::endl;
        std::cout << "Total faults: " << agg.totalFaults << std::endl;
        std::cout << "Total distance flown (miles): " << agg.totalDistanceMiles << std::endl;
        std::cout << "Average distance per flight (miles): " << agg.avgDistancePerFlightInMiles << std::endl;
        std::cout << "Average flight time per flight (hrs): " << agg.avgFlightTimePerFlightInHrs << std::endl;
        std::cout << "Total number of charges: " << agg.totalNumCharges << std::endl;
        std::cout << "Average time charging per session (hrs): " << agg.avgTimeChargingPerSessionInHrs << std::endl;
        std::cout << "Total number of wait for charges: " << agg.totalNumWaitForCharges << std::endl;
        std::cout << "Average wait time per charge (hrs): " << agg.avgWaitTimePerChargeInHrs << std::endl;
    }
}
