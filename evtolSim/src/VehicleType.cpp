#include "VehicleType.h"
#include <iostream>
using namespace std;

VehicleType::VehicleType(VhType type):
    vehicleType(type),
    typeStats(),
    vehicleList()
{
}

void VehicleType::addVehicle(double cruiseSpeed, double batteryCapacity, double timeToCharge, double energyUseAtCruise, int passengerCount, double faultProbability, const std::string& identifier)
{
    vehicleList.push_back(make_unique<Vehicle>(cruiseSpeed,batteryCapacity,timeToCharge,
    energyUseAtCruise,passengerCount,faultProbability,identifier));
    
    typeStats.passengersPerVehicle = passengerCount; // TODO , betterway to initialize this only once
    typeStats.numVehicles++;
}

void VehicleType::evaluateAndPrintStats()
{   
    // First evaluate
    for(auto& vehicle : vehicleList)
    {
        typeStats.totalFlights += vehicle->stats.totalFlights;
        typeStats.totalFaults += vehicle->stats.totalNumFaults;
        typeStats.totalDistanceMiles += vehicle->stats.totalDistance;
        typeStats.totalNumCharges += vehicle->stats.numCharges;
        typeStats.totalFlightTimeInMs += vehicle->stats.totalFlightTimeInMs;
        typeStats.totalChargeTimeInMs += vehicle->stats.totalChargeTimeInMs;
        typeStats.totalWaitForChargeTimeInMs += vehicle->stats.totalWaitForChargingTimeInMs;
        typeStats.totalNumWaitForCharges += vehicle->stats.numWaitingForCharges; 
    }

    // Calculcate average and total Passenger miles
    typeStats.totalPassengerMiles = (typeStats.numVehicles * typeStats.totalDistanceMiles * typeStats.passengersPerVehicle);

    if (typeStats.totalFlights > 0)
    {
        typeStats.avgDistancePerFlightInMiles = (typeStats.totalDistanceMiles / typeStats.totalFlights);
        typeStats.avgFlightTimePerFlightInHrs = ((typeStats.totalFlightTimeInMs / typeStats.totalFlights) / kHrsToMs);
    }

    if (typeStats.totalNumCharges > 0)
    {
        typeStats.avgTimeChargingPerSessionInHrs = ((typeStats.totalChargeTimeInMs / typeStats.totalNumCharges) / kHrsToMs);
        typeStats.avgWaitTimePerChargeInHrs = ((typeStats.totalWaitForChargeTimeInMs / typeStats.totalNumCharges) / kHrsToMs);
    }
    
    // Then print
    cout << "=================================================" << endl;
    cout << "Statistics for vehicle type: " << getStringFromType() << endl;
    cout << "=================================================" << endl;
    cout << "Number of vehicles: " << typeStats.numVehicles << endl;
    cout << "Total passenger miles: " << typeStats.totalPassengerMiles << endl;
    cout << "Total flights: " << typeStats.totalFlights << endl;
    cout << "Total faults: " << typeStats.totalFaults << endl;
    cout << "Average distance per flight (miles): " << typeStats.avgDistancePerFlightInMiles << endl;
    cout << "Average flight time per flight (hours): " << typeStats.avgFlightTimePerFlightInHrs << endl;
    cout << "Average time charging per session (hours): " << typeStats.avgTimeChargingPerSessionInHrs << endl;
    cout << "Average wait time per charge (hours): " << typeStats.avgWaitTimePerChargeInHrs << endl;
    cout << "=================================================" << endl;
}

std::vector<std::unique_ptr<Vehicle>> &VehicleType::getVehicleList()
{
    return vehicleList;
}

const char* VehicleType::getStringFromType()
{
    switch (vehicleType)
    {
        case VhType::ALPHA:
            return "Alpha";
        case VhType::BRAVO:
            return "Bravo";
        case VhType::CHARLIE:
            return "Charlie";
        case VhType::DELTA:
            return "Delta";
        case VhType::ECHO:
            return "Echo";
        default:
            return "Unknown";
    }
}
