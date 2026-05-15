#include <iostream>
#include <string>
#pragma once
using namespace std;

//part 2 - motor and gearbox selection optimization
class Motor
{
public:
    string name;
    float torque_Nm;
    float speed_rpm;
    float mass_kg;                //assigning mass, diameter and width to each motor and gearbox to be used in cost function
    float diameter_mm;
    float width_mm;
                                  

    Motor(string n, float t, float s, float m, float d, float w) //constructor

        : name(n), torque_Nm(t), speed_rpm(s), //asigning : n=name, t=torque_Nm, s=speed_rpm, etc . in motor constructor
          mass_kg(m), diameter_mm(d), width_mm(w) {}

    void show() const //functions to display motor and gearbox data ONLY and not to change it

    {
        cout << "  Motor      : " << name << "\n";
        cout << "  Torque     : " << torque_Nm   << " N.m\n";
        cout << "  Speed      : " << speed_rpm   << " RPM\n";
        cout << "  Mass       : " << mass_kg     << " kg\n";
        cout << "  Diameter   : " << diameter_mm << " mm\n";
        cout << "  Width      : " << width_mm    << " mm\n";
    }
};





class Material
{
public:
string name;
double yieldStrength; //MPa
double density;  //g/cm^3

void setData(string n, double y, double d) //used to add new material to the database
   {
    name = n;
    yieldStrength = y;
    density = d;
   }
};





class Gearbox
{
public:
    string name;
    float ratio;
    float efficiency;
    float mass_kg;
    float diameter_mm;
    float width_mm;

    Gearbox(string n, float r, float e, float m, float d, float w)
        : name(n), ratio(r), efficiency(e),
          mass_kg(m), diameter_mm(d), width_mm(w) {}

    void show() const
    {
        cout << "  Gearbox    : " << name        << "\n";
        cout << "  Ratio      : " << ratio       << "\n";
        cout << "  Efficiency : " << efficiency * 100 << " %\n";
        cout << "  Mass       : " << mass_kg     << " kg\n";
        cout << "  Diameter   : " << diameter_mm << " mm\n";
        cout << "  Width      : " << width_mm    << " mm\n";
    }
};

struct Combination
{
    const Motor*   motor;
    const Gearbox* gearbox;
    float output_torque_Nm;  //like an ID for each competitor
    float output_speed_rpm;
    float cost;
};