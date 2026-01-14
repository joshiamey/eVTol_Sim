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
