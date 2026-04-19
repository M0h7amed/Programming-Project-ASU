# Programming-Project-ASU
# Single-Link Robot Design and Optimization 🤖⚙️

## Overview
This project is a C++ application developed to automate the structural design and actuator selection for a one-link robot actuated by a rotating DC motor. The program bridges mechanical engineering principles with algorithmic problem-solving to ensure the robot is both safe and cost-effective.

## Core Features
The program is divided into two primary optimization modules:

* **Part 1: Stress Analysis and Link Optimization**
  * Calculates bending moments and maximum stress considering the link's weight, payload mass, and inertial forces from angular acceleration.
  * Utilizes an iterative loop to automatically reduce link dimensions (rectangular or circular cross-sections) by 1% per iteration until the stress falls safely just below the material's yield strength.
* **Part 2: Motor and Gearbox Selection**
  * Computes the required torque and speed for the optimized link.
  * Uses searching and sorting algorithms to filter a predefined database of motors and gearboxes.
  * Selects the optimal combination by minimizing a custom cost function based on component mass, diameter, and width.

## Technical Skills & Concepts Applied
* **Language:** C++
* **Programming Concepts:** Object-Oriented Programming (OOP), Custom Classes (Materials, Motors, Gearboxes), Iterative Loops, Conditional Logic.
* **Engineering Concepts:** Mechanical Stress Analysis, Parametric Optimization, Actuator Sizing.

