# Programming-Project-ASU
# Single-Link Robot Design and Optimization 🤖⚙️

## Overview
[span_0](start_span)This project is a C++ application developed to automate the structural design and actuator selection for a one-link robot actuated by a rotating DC motor[span_0](end_span). The program bridges mechanical engineering principles with algorithmic problem-solving to ensure the robot is both safe and cost-effective.

## Core Features
The program is divided into two primary optimization modules:

* **Part 1: Stress Analysis and Link Optimization**
  * [span_1](start_span)[span_2](start_span)Calculates bending moments and maximum stress considering the link's weight, payload mass, and inertial forces from angular acceleration[span_1](end_span)[span_2](end_span).
  * [span_3](start_span)[span_4](start_span)Utilizes an iterative loop to automatically reduce link dimensions (rectangular or circular cross-sections) by 1% per iteration until the stress falls safely just below the material's yield strength[span_3](end_span)[span_4](end_span).
* **Part 2: Motor and Gearbox Selection**
  * [span_5](start_span)[span_6](start_span)Computes the required torque and speed for the optimized link[span_5](end_span)[span_6](end_span).
  * [span_7](start_span)Uses searching and sorting algorithms to filter a predefined database of motors and gearboxes[span_7](end_span).
  * [span_8](start_span)[span_9](start_span)Selects the optimal combination by minimizing a custom cost function based on component mass, diameter, and width[span_8](end_span)[span_9](end_span).

## Technical Skills & Concepts Applied
* **Language:** C++
* **[span_10](start_span)[span_11](start_span)Programming Concepts:** Object-Oriented Programming (OOP), Custom Classes (Materials, Motors, Gearboxes), Iterative Loops, Conditional Logic[span_10](end_span)[span_11](end_span).
* **[span_12](start_span)[span_13](start_span)Engineering Concepts:** Mechanical Stress Analysis, Parametric Optimization, Actuator Sizing[span_12](end_span)[span_13](end_span).
* **[span_14](start_span)Collaboration:** Version control using Git and GitHub workflows[span_14](end_span).
