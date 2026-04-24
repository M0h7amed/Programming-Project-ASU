#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;


class Motor
{
public:
    string name;
    float torque_Nm;
    float speed_rpm;
    float mass_kg;
    float diameter_mm;
    float width_mm;

    Motor(string n, float t, float s, float m, float d, float w)
        : name(n), torque_Nm(t), speed_rpm(s),
          mass_kg(m), diameter_mm(d), width_mm(w) {}

    void show() const

    {
        cout << "  Motor      : " << name << "\n";
        cout << "  Torque     : " << torque_Nm   << " N'm\n";
        cout << "  Speed      : " << speed_rpm   << " RPM\n";
        cout << "  Mass       : " << mass_kg     << " kg\n";
        cout << "  Diameter   : " << diameter_mm << " mm\n";
        cout << "  Width      : " << width_mm    << " mm\n";
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
    const Motor * motor;
    const Gearbox * gearbox;
    float output_torque_Nm;
    float output_speed_rpm;
    float cost;
};

float computeCost(const Motor& m, const Gearbox& g)
{
    float total_mass     = m.mass_kg     + g.mass_kg;
    float total_diameter = m.diameter_mm + g.diameter_mm;
    float total_width    = m.width_mm    + g.width_mm;
    return total_mass + total_diameter / 100.0 + total_width / 100.0;
}

const float g=9.81;

int main()

{

    vector<Motor> motors = {
        Motor("Maxon EC-i 40 (70W)",  0.180, 8500, 0.340, 40.0, 73.0),
        Motor("Maxon RE 35 (90W)",  0.253, 6500, 0.530, 35.0, 94.0)
    };


    vector<Gearbox> gearboxes =
    {
        Gearbox("Maxon GP 32 C (ratio 14:1)",  14.0, 0.75, 0.210, 32.0, 31.5),
        Gearbox("Maxon GP 42 C (ratio 26:1)",  26.0, 0.72, 0.370, 42.0, 49.5)
    };

    cout <<"------------------------------------------------------\n";
    cout << "   PART 2 - Motor & Gearbox Selection Optimization\n";
    cout <<"------------------------------------------------------\n";

    float ml,L,mp,alpha_max,omega_required;

    cout << "Enter link mass  ml (kg): ";
    cin >> ml;
    cout << "Enter link length  L (mm): ";
    cin >> L;
    cout << "Enter payload mass mp (kg): ";
    cin >> mp;
    cout << "Enter max angular acc.  alpha_max (rad/s*s): ";
    cin >> alpha_max;
    cout << "Enter required output speed Omega (RPM): ";
    cin >> omega_required;


    float T_required =(ml*g*(L/2))+(mp*g*L+ml*(L/2)*(L/2)*alpha_max)+(mp*L*L*alpha_max);

    cout <<"------------------------------------------------------\n\n";
    cout << " Required Torque  : " << T_required    <<endl;
    cout << "  Required Speed   : " << omega_required <<endl;
    cout <<"\n------------------------------------------------------\n";

    vector<Combination> valid_pairs;

    cout << "\n  Evaluating all motor-gearbox combinations...\n\n";

    for (const auto& motor : motors)
        {
        for (const auto& gearbox : gearboxes)
          {

            float T_output = motor.torque_Nm * gearbox.ratio * gearbox.efficiency;
            float w_output = motor.speed_rpm / gearbox.ratio;
            float cost = computeCost(motor, gearbox);
            cout <<"---------------------------------------------\n";
            cout << "  " << motor.name << " + " << gearbox.name << endl;
            cout << "    Output Torque : "<< T_output <<endl;
            cout << "Output Speed : "<< w_output <<endl;
            cout << "Cost : " << cost<< endl;

            if (T_output >= T_required && w_output >= omega_required)
                {
                valid_pairs.push_back({&motor, &gearbox, T_output, w_output, cost});
                cout << "Approved"<<endl;
                cout <<"---------------------------------------------\n";
                } else
                      {
                       cout << "Rejected";
                       if (T_output < T_required)
                       cout << " [Torque insufficient]";
                       if (w_output < omega_required)
                       cout << " [Speed insufficient]";
                       cout << endl;
                     }
          }
        }



    if (valid_pairs.empty())
        {
        cout << "\n  No valid motor-gearbox combination found.\n"
             << "  Consider motors with higher rated torque or gearboxes with a larger gear ratio.\n";
        }
    else
      {
        Combination best = valid_pairs[0];
        for (const auto& combo : valid_pairs)
            {
            if (combo.cost < best.cost)
                best = combo;
            }

        cout << "\n  OPTIMAL COMBINATION (lowest cost among valid pairs)\n";
        best.motor->show();
        cout << endl;
        best.gearbox->show();
        cout << endl;
        cout << "  Output Torque  : " << best.output_torque_Nm << " N'm\n";
        cout << "  Output Speed   : " << best.output_speed_rpm << " RPM\n";
        cout << "  Total Cost     : " << best.cost << endl;

    }

    return 0;
}
