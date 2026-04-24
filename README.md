# Part 1: Stress Analysis and Link Dimension Optimization

## Contributors
* Mahmoud Samy
* Ahmed El Sayed

## Objective
This branch implements the first core module of the Single-Link Robot project. It determines the safest and most optimal dimensions for the robot link by analyzing the stresses caused by the link's own weight, the payload mass at the end effector, and the inertial forces due to angular acceleration.

## Key Implementations
* Mathematical Models: Successfully translated the mechanical engineering equations into C++ logic to calculate:
  * Link Mass (for both rectangular and circular cross-sections) based on material density.
  * Bending Moment (incorporating static weight and dynamic inertial forces).
  * Moment of Inertia and Maximum Bending Stress.
* Iterative Optimization Loop: Programmed the core while loop that iteratively adjusts the link's dimensions. 
  * If the calculated stress exceeds the yield strength, the dimensions are increased.
  * If the stress is safely below the limit, the program dynamically reduces the dimensions by 1 percent per iteration to save weight and material.
  * The loop stops the moment the stress drops just below the material's yield strength.

## Output Ready for Integration
The module is tested and successfully outputs the final optimized link dimensions, the final mass of the link, and the calculated stress value. These variables are now ready to be passed into the Part 2 module (Motor Selection).
