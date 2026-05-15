#include <iostream>
#include <string>
#include "classes.h"
#pragma once
using namespace std;



float computeCost(const Motor& m, const Gearbox& g) //cost finding 
{
    float total_mass     = m.mass_kg     + g.mass_kg;
    float total_diameter = m.diameter_mm + g.diameter_mm;
    float total_width    = m.width_mm    + g.width_mm;
    return total_mass + total_diameter / 100.0 + total_width / 100.0;
}


void savefile(Material* database, int currentCount)//function to save the materials in the database to a text file named "saved materials.txt" when the user adds a new material to the database
{
    ofstream file("saved materials.txt");
    if (file.is_open()) {
        for (int i = 0; i < currentCount; i++)
            file << database[i].name << "|"
                 << database[i].yieldStrength << "|"
                 << database[i].density << "\n";
        file.close();
    }
}
