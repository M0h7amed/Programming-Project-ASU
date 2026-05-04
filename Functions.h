#include <iostream>
#include <string>
#include "classes.h"
#pragma once
using namespace std;



float computeCost(const Motor& m, const Gearbox& g)
{
    float total_mass     = m.mass_kg     + g.mass_kg;
    float total_diameter = m.diameter_mm + g.diameter_mm;
    float total_width    = m.width_mm    + g.width_mm;
    return total_mass + total_diameter / 100.0 + total_width / 100.0;
}