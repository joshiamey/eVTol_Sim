# eVTOL Simulation Problem

## Objective

Show us your awesome skills! We want to see your design, coding, documentation, testing, and communication abilties first hand!

## Details

This work must be coded in C++, and submitted through GitHub, BitBucket, or Gitlab. Upload the documented code to your personal account and send us a link. We may look at the git history to see how you use version control.

At Joby we love good OOP design. Please exercise the basic tenets of OOP, using relevant principles where appropriate. We’re looking for robust, testable, modular, and well-documented code.

Unit Testing is a big part of our development process. Please include just a few examples of unit tests or how your code is testable. Tests don't necessarily need to run in any test framework, but please be ready to talk about what and how you tested your code.

To dos and notes are OK. If you want to incorporate an idea that makes sense in a real project, but would take too much time to implement, just leave a note. For example:

```cpp
// Todo: Refactor database to use hashmap rather than raw array for
// <xyz> reasons..
```

## Problem Statement

There are five companies developing eVTOL aircraft. The vehicle produced by each manufacturer has different characteristics. Six distinct properties are laid out in the below table:

| Company Name    | Cruise Speed (mph) | Battery Capacity (kWh) | Time to Charge (hours) | Energy use at Cruise (kWh/mile) | Passenger Count | Probability of fault per hour |
|-----------------|--------------------|------------------------|------------------------|---------------------------------|-----------------|-------------------------------|
| Alpha Company   | 120                | 320                    | 0.6                    | 1.6                             | 4               | 0.25                          |
| Bravo Company   | 100                | 100                    | 0.2                    | 1.5                             | 5               | 0.10                          |
| Charlie Company | 160                | 220                    | 0.8                    | 2.2                             | 3               | 0.05                          |
| Delta Company   | 90                 | 120                    | 0.62                   | 0.8                             | 2               | 0.22                          |
| Echo Company    | 30                 | 150                    | 0.3                    | 5.8                             | 2               | 0.61                          |

You will simulate using these vehicle for 3 hours. Of course your simulation should take much less time than that. 20 total vehicles should be deployed, and a random number of each type of vehicle should be used (with the total between all five types being 20).

There are only three chargers available for all 20 vehicles! A single charger can only be used by one vehicle at a time. Assume the chargers can charge the batteries in the Time to Charge time listed for each vehicle.

Keep track of the following statistics per vehicle type:
- average flight time per flight
- average distance traveled per flight
- average time charging per charge session
- total number of faults
- total number of passenger miles.
  For example, if there are 2 vehicles carrying 4 passengers on a vehicle that cruises for 1 hour at 100 mph, total number of passenger miles is 2 * 4 * 1 * 100 = 800.

Assume that:
- Each vehicle starts the simulation with a fully-charged battery
- Each vehicle instantaneously reaches Cruise Speed
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the charger after running out of battery power.

Please include the statistics recorded during at least one run of the simulation in your submission.

Please do not hesitate to reach out to ask any questions about the problem! However, if you find yourself making an assumption that we haven’t explained explicitly, you may simply document your assumption. It’s interesting to us to hear what assumptions you made and why.