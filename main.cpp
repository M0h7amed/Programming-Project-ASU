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


bool try_again = 1; //try again variable to ask the user if they want to try again with different material selection and design optimization
int main()
{
    while (try_again==1)//while loop to allow the user to try again with different material selection and design optimization
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
            cout << "Enter density (g/cm3): ";      cin >> d;

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

  //part 2 - motor and gearbox selection optimization

    vector<Motor> motors_vector =
  //assign a vector named "motors_vector"
    {
        Motor("Maxon EC-i 40 (70W)",  0.180, 8500, 0.340, 40.0, 73.0),
        Motor("Maxon RE 35 (90W)",    0.253, 6500, 0.530, 35.0f, 94.0),
        Motor("Maxon DCX 26 l (40W)",   0.0469, 10600, 0.131, 26.0, 57.0)
        //asigning : n=name, t=torque_Nm, s=speed_rpm, m=mass_kg, d=diameter_mm, w=width_mm in motor constructor
    };
                                                                                                                  // 3 motors and 3 gearboxes are defined 
    vector<Gearbox> gearboxes_vector =
    //assign a vector named "gearboxes_vector"
    {
        Gearbox("Maxon GP 32 C (ratio 14:1)", 14.0, 0.75, 0.210, 32.0, 31.5),
        //asigning : n=name, R=ratio, e=efficiency, m=mass_kg, d=diameter_mm, w=width_mm in gearbox constructor
        Gearbox("Maxon GP 42 C (ratio 26:1)", 26.0, 0.72, 0.370, 42.0, 49.5),
        Gearbox(" Maxon GPX 26 (ratio 16:1) 2 Stages",  16.0, 0.81, 0.090, 26.0, 35.0)
    };

    cout << "\n------------------------------------------------------\n";
    cout << "            Motor & Gearbox Selection  \n";
    

    vector<Combination> valid_pairs; //define a vector of type Combination to store the valid motor-gearbox pairs that meet the requirements of torque and speed
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
        cout << "  Output Torque : " << T_output << " N.m\n";
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
if (valid_pairs.empty())
{
    // No combination satisfied both torque and speed requirements
    cout << "\n  No valid motor-gearbox combination found.\n"
         << "  Consider motors with higher rated torque or larger gear ratios.\n\n\n\n";
} 
else {
    // Find the lowest cost among all approved combinations
    Combination best = valid_pairs[0]; // start by assuming first valid pair is best
    for (const auto& combo : valid_pairs)
        if (combo.cost < best.cost) best = combo; // replace if a cheaper one is found

    // Print the winner
    cout << "\n  OPTIMAL COMBINATION (lowest cost among valid pairs)\n";
    (*best.motor).show();
    cout << "\n";  //follow the address of the motor and gearbox in the best combination and call the show() function to display their data
    (*best.gearbox).show(); //pointer to the gearbox in the best combination and call the show() function to display its data
    cout << "\n";
    cout << "  Output Torque : " << best.output_torque_Nm << " N.m\n";
    cout << "  Output Speed  : " << best.output_speed_rpm << " RPM\n";
    cout << "  Total Cost    : " << best.cost             << "\n";
}
                   cout<<"Do you want to try again ? (1 for yes, 0 for no): \n";
                    cin>>try_again;                  //try again variable to ask the user if they want to try again with different material selection and design optimization
    }
return 0;
}