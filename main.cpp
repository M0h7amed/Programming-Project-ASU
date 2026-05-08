#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "classes.h"
#include "Functions.h"
#include <vector>

using namespace std;

// Constants
const double g = 9.81, pi = 3.14159;

struct Combination
{
    const Motor*   motor;
    const Gearbox* gearbox;
    float output_torque_Nm;  //like an ID for each competitor
    float output_speed_rpm;
    float cost;
};



double Masslink(int type, double b, double h, double L, double r, double density)
{
    double Density = density * 1000; // g/cm³ → kg/m³
    double volume  = (type == 1) ? (b * h * L) : (pi * pow(r, 2) * L);
    return Density * volume;
}

double Inertia(int type, double b, double h, double r)
{
    if (type == 1)
        return (b * pow(h, 3)) / 12;
    else
        return (pi * pow(r, 4)) / 4;
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

    cout << "Max angular acceleration alpha_max (rad/s²): "; 
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
            cerr << "Invalid choice.\n";
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

        if (stress > m.yieldStrength * 0.98) {
            if (type == 1) { b *= 1.01; h *= 1.01; }
            else             r *= 1.01; //لازم يتعدل يا اخونا 
        } else if (stress < m.yieldStrength * 0.95) {
            if (type == 1) { b *= 0.95; h *= 0.95; }
            else             r *= 0.95;
        } else {
            optimized = true;
        }
    }

    float T_required = static_cast<float>(
        (ml * g * (L / 2))
      + (mp * g * L)
      + (ml * (L / 2) * (L / 2) * alpha_max)
      + (mp * L * L * alpha_max)
    );

    cout << "\nOptimized results:\n";
    if (type == 1)
        cout << "Final dimensions : b = " << b << " m,  h = " << h << " m\n";
    else
        cout << "Final dimensions : r = " << r << " m\n";

    cout << "Final link mass  : " << ml     << " kg\n";
    cout << "Final stress     : " << stress << " MPa\n";
    cout << "Required torque  : " << T_required    << " N·m\n";
    cout << "Required speed   : " << omega_required << " RPM\n";

   
    T_required_out     = T_required;
    omega_required_out = static_cast<float>(omega_required);
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


int main()
{
    
    float T_required = 0, omega_required = 0;

    int currentCount = 0;
    Material* database = nullptr;

    ifstream infile("saved materials.txt");
    if (infile.is_open()) {
        string line;
        while (getline(infile, line))
            if (!line.empty()) currentCount++;

        infile.clear();
        infile.seekg(0);

        database = new Material[currentCount];
        for (int i = 0; i < currentCount; i++) {
            string n, ys_str, d_str;
            if (getline(infile, n, '|') &&
                getline(infile, ys_str, '|') &&
                getline(infile, d_str))
            {
                database[i].setData(n, stod(ys_str), stod(d_str));
            }
        }
        infile.close();
        cout << "Database loaded. Total materials: " << currentCount << "\n";
    } else {
        currentCount = 9;
        database = new Material[currentCount];
        database[0].setData("Cast Iron",       130,  7.30);
        database[1].setData("Copper Nickel",   130,  8.94);
        database[2].setData("Brass",           200,  8.73);
        database[3].setData("Aluminum",        241,  2.70);
        database[4].setData("Steel",           247,  7.58);
        database[5].setData("Acrylic",          72,  1.16);
        database[6].setData("Copper",           70,  8.92);
        database[7].setData("Stainless Steel", 275,  7.86);
        database[8].setData("Tungsten",        941, 19.25);

        ofstream file("saved materials.txt");
        for (int i = 0; i < currentCount; i++)
            file << database[i].name << "|"
                 << database[i].yieldStrength << "|"
                 << database[i].density << "\n";
        file.close();
    }


    int choice;//variable to ask the user to select between displaying the materials in the database or adding a new material to the database
  
    cout << "\nMaterial Selection\n";
    cout << "1- Display All Materials\n";
    cout << "2- Add a Custom Material\n";
    cout << "Enter selection: ";
    cin  >> choice;

    switch (choice)
    {
        case 1://case 1 to display all materials in the database and ask the user to select one for design optimization
        {
            for (int i = 0; i < currentCount; i++)
                cout << i + 1 << ". " << database[i].name << "\n";
            int selection;
            cin >> selection;
            
            design(database[selection - 1], T_required, omega_required);
            break;
        }
        case 2://case 2 to add a new material to the database and then optimize the design based on the new material added by the user
        {
            Material* temp = new Material[currentCount + 1];
            for (int i = 0; i < currentCount; i++) temp[i] = database[i];

            string n;
            double y, d;
            cout << "Enter material name: ";
            cin.ignore();
            getline(cin, n);
            cout << "Enter yield strength (MPa): "; cin >> y;
            cout << "Enter density (g/cm³): ";      cin >> d;

            temp[currentCount].setData(n, y, d);
            delete[] database;
            database = temp;
            currentCount++;
            savefile(database, currentCount);
            design(database[currentCount - 1], T_required, omega_required);
            break;
        }
        default:
            cout << "Invalid choice.\n";
    }

    delete[] database;

  
    vector<Motor> motors_vector =
  //assign a vector named "motors_vector"
    {
        Motor("Maxon EC-i 40 (70W)",  0.180f, 8500, 0.340f, 40.0f, 73.0f),
        Motor("Maxon RE 35 (90W)",    0.253f, 6500, 0.530f, 35.0f, 94.0f)
    };
                                                                         // 2 motors and 2 gearboxes are defined 
    vector<Gearbox> gearboxes_vector =
    //assign a vector named "gearboxes_vector"
    {
        Gearbox("Maxon GP 32 C (ratio 14:1)", 14.0f, 0.75f, 0.210f, 32.0f, 31.5f),
        Gearbox("Maxon GP 42 C (ratio 26:1)", 26.0f, 0.72f, 0.370f, 42.0f, 49.5f)
    };

    cout << "\n------------------------------------------------------\n";
    cout << "   PART 2 - Motor & Gearbox Selection Optimization\n";
    cout << "------------------------------------------------------\n";
    cout << " Required Torque : " << T_required    << " N·m\n";//from user input in part 1 baseed on optimized design of ml
    cout << " Required Speed  : " << omega_required << " RPM\n";//from user input in part 1
    cout << "------------------------------------------------------\n";

    vector<Combination> valid_pairs;
    cout << "\n  Evaluating all motor-gearbox combinations...\n\n";

    // Test every possible motor + gearbox combination 
for (const auto& motor : motors_vector)//means for each motor in the motors_vector, do the following loop for each gearbox in the gearboxes_vector
 {
    for (const auto& gearbox : gearboxes_vector) {

        // Calculate what this combination actually delivers
        float T_output = motor.torque_Nm * gearbox.ratio * gearbox.efficiency; 
        float w_output = motor.speed_rpm / gearbox.ratio;                      
        float cost     = computeCost(motor, gearbox);                           

        // Print the results for this pair
        cout << "---------------------------------------------\n";
        cout << "  " << motor.name << " + " << gearbox.name << "\n";
        cout << "  Output Torque : " << T_output << " N·m\n";
        cout << "  Output Speed  : " << w_output << " RPM\n";
        cout << "  Cost          : " << cost     << "\n";

        // Check if this combination meets BOTH requirements
        if (T_output >= T_required && w_output >= omega_required) {
            valid_pairs.push_back({&motor, &gearbox, T_output, w_output, cost}); // save it as a valid option
            cout << "  >> Approved\n";
        } else {
            // Rejected - print which requirement(s) it failed
            cout << "  >> Rejected";
            //reason for rejection
            if (T_output < T_required)    cout << " [Torque insufficient]";
            if (w_output < omega_required) cout << " [Speed insufficient]";
            cout << "\n";
        }
        cout << "---------------------------------------------\n";
    }
}

// After testing all pairs, pick the best one
if (valid_pairs.empty()) {
    // No combination satisfied both torque and speed requirements
    cout << "\n  No valid motor-gearbox combination found.\n"
         << "  Consider motors with higher rated torque or larger gear ratios.\n";
} else {
    // Find the lowest cost among all approved combinations
    Combination best = valid_pairs[0]; // start by assuming first valid pair is best
    for (const auto& combo : valid_pairs)
        if (combo.cost < best.cost) best = combo; // replace if a cheaper one is found

    // Print the winner
    cout << "\n  OPTIMAL COMBINATION (lowest cost among valid pairs)\n";
    best.motor->show();
    cout << "\n";  //follow the address of the motor and gearbox in the best combination and call the show() function to display their data
    best.gearbox->show();
    cout << "\n";
    cout << "  Output Torque : " << best.output_torque_Nm << " N·m\n";
    cout << "  Output Speed  : " << best.output_speed_rpm << " RPM\n";
    cout << "  Total Cost    : " << best.cost             << "\n";
}

return 0;
}