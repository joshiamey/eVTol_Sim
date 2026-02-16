#include "VehicleType.h"
#include <iostream>
using namespace std;

VehicleType::VehicleType(VhType type):
    vehicleType(type),
    typeStats(),
    vehicleList()
{
}

void VehicleType::addVehicle(const Vehicle* vh)
{
    vehicleList.push_back(vh);
    
    typeStats.passengersPerVehicle = vh->getPassengerCount(); // TODO , betterway to initialize this only once
    typeStats.numVehicles++;
}

void VehicleType::evaluateStats()
{   
    // First evaluate
    for(auto& vehicle : vehicleList)
    {   
        const auto& vhStats = vehicle->getStats();
        typeStats.totalFlights += vhStats.totalFlights;
        typeStats.totalFaults += vhStats.totalNumFaults;
        typeStats.totalDistanceMiles += vhStats.totalDistance;
        typeStats.totalNumCharges += vhStats.numCharges;
        typeStats.totalFlightTimeInMs += vhStats.totalFlightTimeInMs;
        typeStats.totalChargeTimeInMs += vhStats.totalChargeTimeInMs;
        typeStats.totalWaitForChargeTimeInMs += vhStats.totalWaitForChargingTimeInMs;
        typeStats.totalNumWaitForCharges += vhStats.numWaitingForCharges; 
    }

    // Calculcate average and total Passenger miles
    typeStats.totalPassengerMiles =  typeStats.totalDistanceMiles * typeStats.passengersPerVehicle;

    if (typeStats.totalFlights > 0)
    {
        typeStats.avgDistancePerFlightInMiles = (typeStats.totalDistanceMiles / static_cast<double>(typeStats.totalFlights));
        typeStats.avgFlightTimePerFlightInHrs = ((static_cast<double>(typeStats.totalFlightTimeInMs) / typeStats.totalFlights) / kHrsToMs);
    }

    if (typeStats.totalNumCharges > 0)
    {
        typeStats.avgTimeChargingPerSessionInHrs = ((static_cast<double>(typeStats.totalChargeTimeInMs) / typeStats.totalNumCharges) / kHrsToMs);
    }

    if(typeStats.totalNumWaitForCharges > 0)
    {
        typeStats.avgWaitTimePerChargeInHrs = ((static_cast<double>(typeStats.totalWaitForChargeTimeInMs) / typeStats.totalNumWaitForCharges) / kHrsToMs);
    }
}

void VehicleType::printStats()
{
    // Then print
    cout << "=================================================" << endl;
    cout << "Statistics for vehicle type: " << getStringFromType() << endl;
    cout << "=================================================" << endl;
    cout << "Number of vehicles: " << typeStats.numVehicles << endl;
    cout << "Total passenger miles: " << typeStats.totalPassengerMiles << endl;
    cout << "Total flights: " << typeStats.totalFlights << endl;
    cout << "Total faults: " << typeStats.totalFaults << endl;
    cout << "Total Num Wait for Chargers: "<< typeStats.totalNumWaitForCharges << endl;
    cout << "Average distance per flight (miles): " << typeStats.avgDistancePerFlightInMiles << endl;
    cout << "Average flight time per flight (hours): " << typeStats.avgFlightTimePerFlightInHrs << endl;
    cout << "Average time charging per session (hours): " << typeStats.avgTimeChargingPerSessionInHrs << endl;
    cout << "Average wait time per charge (hours): " << typeStats.avgWaitTimePerChargeInHrs << endl;
    cout << "=================================================" << endl;
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
