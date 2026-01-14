#include "EvtolSimulation.h"

int main() {
    // Parameters
    const int vehicleCount = 20;
    const int chargerCount = 3;
    const double durationInHrs = 3.0;

    // Create and run simulation
    EvtolSimulation simulation(vehicleCount, chargerCount, durationInHrs);
    simulation.initializeSimulation();
    simulation.runSimulation();

    return 0;
}
