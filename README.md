# eVTOL Simulation Problem

## Assumptions

- Each vehicle starts the simulation with a fully-charged battery.
- Each vehicle instantaneously reaches Cruise Speed.
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the charger after running out of battery power.
- Using Milliseconds as unit of time.

## Approach

- Created a Discrete event based scheduler. Based on assumptions, created the below events:
  - Flight over event by calculating time to complete the range distance for battery capacity.
  - Charge over event by calculating time required to charge.
- On every event, call the vehicle's process function to record stats.

## Build Instructions

This project uses CMake to build the simulation executable.

1.  **Navigate to the project root directory.**

2.  **Create a build directory and navigate into it:**
    ```sh
    mkdir -p build
    cd build
    ```

3.  **Run CMake to configure the project.** Point it to the `evtolSim` directory where the `CMakeLists.txt` file is located:
    ```sh
    cmake ../evtolSim
    ```

4.  **Compile the project using Make:**
    ```sh
    make
    ```

5.  **Run the simulation executable:**
    ```sh
    ./evtolSim
    ```

## Build Instructions
