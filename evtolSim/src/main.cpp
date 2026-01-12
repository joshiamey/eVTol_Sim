#include <iostream>
#include "vehicle.h"

int main() {
    std::cout << "Hello, eVTOL!" << std::endl;
    Vehicle myVehicle(200.0, 100.0, 2.5, 50.0, 4, 0.01, "eVTOL-001");
    return 0;
}
