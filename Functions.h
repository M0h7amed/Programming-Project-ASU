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


double Masslink(int type, double b, double h, double L, double r, double density)
{
    double Density = density * 1000; // g/cm³ → kg/m³
    double volume ;
    switch(type)
    {
        case 1:
            volume = b * h * L;
            break;
        case 2:
            volume = pi * pow(r, 2) * L;
            break;
        default:
            cout << "Invalid cross-section type.\n";
            return 0;
    }
    return Density * volume;
}

double Inertia(int type, double b, double h, double r)
{
   switch(type)
   {
       case 1:
           return (b * pow(h, 3)) / 12.0;
           break; // Rectangular section
       case 2:
           return (pi * pow(r, 4)) / 4.0; 
           break;// Circular section
       default:
           cout << "Invalid cross-section type.\n";
              return 0;
   }
}


void design(Material m, float& T_required_out, float& omega_required_out)//function to optimize the design of the link based on the material selected by the user and the requirements of torque and speed
{
    int type;
    cout << "Choose cross-section type:\n 1- Rectangular\n 2- Circular\n";
    cin  >> type;

    double L, b , h , r , mp, ml, omega_required, alpha_max;

    cout << "Link Length L (m): ";
              cin >> L;

    cout << "Payload mass mp (kg): ";    
              cin >> mp;

    cout << "Enter link mass ml (kg): ";   
              cin >> ml;

    cout << "Max angular acceleration alpha_max (rad/s2): "; 
              cin >> alpha_max;

    cout << "Enter required output speed Omega (RPM): ";     
              cin >> omega_required;

//switch case to ask for initial dimensions based on the type of cross section selected by the user
    switch (type) 
    {
        case 1:
            cout << "Initial Width b and Height h (m): ";
            cin  >> b >> h;
            break;
        case 2:
            cout << "Initial Radius r (m): ";
            cin  >> r;
            break;
        default:
            cout << "Invalid choice.\n";
          break;
    
    }

    bool   optimized = false;
    double stress = 0, c = 0;

    
    double density = m.density;

    while (!optimized)
    {
       
        ml = Masslink(type, b, h, L, r, density);

        double M = (ml * g * (L / 2))
                 + (mp * g * L)
                 + (ml * pow(L / 2, 2) * alpha_max)
                 + (mp * pow(L,   2) * alpha_max);

        double I = Inertia(type, b, h, r);
        c = (type == 1) ? h / 2 : r;

        stress = (M * c / I) / 1e6; // Pa → MPa

       switch(type)
       {
           case 1:
               if (stress > m.yieldStrength) {
                   b *= 1.05; h *= 1.05; // increase dimensions by 5%
               } else if(stress < 0.8 * m.yieldStrength) {
                   b *= 0.95; h *= 0.95; // decrease dimensions by 5% if stress is significantly below yield strength
               } else {
                   optimized = true;
               }
               break;
           case 2:
               if (stress > m.yieldStrength) {
                   r *= 1.05; // increase radius by 5%
               } else if(stress < 0.8 * m.yieldStrength) {
                   r *= 0.95; // decrease radius by 5% if stress is significantly below yield strength
               } else {
                   optimized = true;
               }
               break;
       }
    }

    float T_required = (
        (ml * g * (L / 2))
      + (mp * g * L)
      + (ml * (L / 2) * (L / 2) * alpha_max)
      + (mp * L * L * alpha_max)
    );

    cout << "\nOptimized results:\n";
switch(type)
    {
        case 1:
            cout << "Final dimensions : b = " << b << " m,  h = " << h << " m\n";
            break;
        case 2:
            cout << "Final dimensions : r = " << r << " m\n";
            break;
    }

    cout << "Final link mass  : " << ml     << " kg\n";
    cout << "Final stress     : " << stress << " MPa\n";
    cout << "Required torque  : " << T_required    << " N.m\n";
    cout << "Required speed   : " << omega_required << " RPM\n";

   
    T_required_out     = T_required;
    omega_required_out = (omega_required);
}