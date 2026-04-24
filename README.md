# Part 2: Motor and Gearbox Selection Optimization

## Contributors
* Essam Mohamed
* Yousef Yasser

## Objective
This branch implements the second core module of the Single-Link Robot project. It focuses on selecting the most efficient motor and gearbox combination from a predefined database based on the required torque, required speed, and a specific optimization priority.

## Key Implementations
* Actuator Database Integration: Managed the data structures representing the available motors and gearboxes, including their physical specifications (mass, diameter, width, torque, speed, gear ratio, and efficiency).
* Required Parameter Calculations: Programmed the logic to calculate the required torque based on the link's finalized dimensions, payload mass, and angular acceleration.
* Filtering Algorithms: Created the logic to iterate through the database and filter out any motor-gearbox pairs that do not meet the minimum required output torque and output speed.
* Cost Function Optimization: Implemented an algorithm to evaluate all valid pairs using a custom cost function (minimizing a combination of mass, diameter, and width) to select the absolute best combination.

## Output Ready for Integration
The module successfully outputs the optimal motor-gearbox combination and its exact specifications. This seamlessly links with Part 1 and completes the final stage of the program's automated design process.

