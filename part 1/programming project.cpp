#include <iostream>
#include <string>
#include <cmath>
#include<string>
#include<fstream>

using namespace std;

//Constants

double g = 9.81, pi = 3.14159;

//Classes

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

//functions


double Masslink (int type, double b, double h, double L, double r, double density) // b is width and h is height and L is length and r is radius
 {
// Convert density from g/cm^3 to kg/m^3
double Density = density * 1000;
double volume;

  if (type == 1)
{
        volume = b * h * L; // Rectangular cross section
}
    else
    {
        volume = pi * pow(r,2) * L; // Circler cross section
    }
    return Density * volume; // output of function is mass of link(kg)
}

double Inertia(int type, double b, double h, double r)
{
    if (type == 1)
     {
        return (b * pow(h, 3)) / 12; // output of function is inertia(m4)
     }
      else
     {
        return (pi * pow(r, 4)) / 4; // output of function is inertia(m4)
     }
}
  // function to perform design logic

  void design(Material m)
  {
    int type;
    cout<<"choose cross section type:\n 1- rectangular \n 2- circler \n " ;
    cin>>type;

    double L,b,h,r,mp,alpha = 0;
                cout << "Link Length L (m): ";
                cin >> L;
                cout << "Payload mass mp (kg): ";
                cin >> mp;
                cout << "Max angular acceleration alpha (rad/s2): ";
                cin >> alpha;

                if (type == 1)
                    {
                    cout << "Initial Width b and Height h (mm): ";
                    cin >> b >> h;
                    }
                else
                {
                    cout << "Initial Radius r (mm): ";
                    cin >> r ;
                }
    bool optimized = false ;
    double stress, ml,c,density ;

    while(!optimized)
    {

    ml = Masslink(type,b,h,L,r,density);

    double M = (ml*g*(L/2))+(mp*g*L)+(ml*pow((L/2),2)*alpha)+(mp*pow(L,2)*alpha); //output is bending moment(N.m)
    double I = Inertia(type,b,h,r);

    if (type==1)
    {
        c = h/2 ;

    }
    else
    {
        c = r ;
    }

    stress = ((M*c)/I)/pow(10,6);

    if(stress > m.yieldStrength*0.98)
    {
        if (type==1)
        {
            b*=1.01 , h*=1.01 ;
        }
        else
        {
            r*=1.01 ;
        }

    }
    else if (stress<m.yieldStrength*0.95)
    {
        if (type==1)
        {
           b*=0.95 , h*=0.95 ;
        }
        else
        {
             r*=0.95 ;
        }
    }
    else
    {
        optimized = true ;
    }
    }

cout<<"optimized results\n" ;
if (type==1)
{
    cout<<"final dimensions : \n"<<b<<"m\n"<<h<<"m" ;
}
else
{
    cout<<"final dimensions : "<<r<<"m" ;
}

cout<<"\nfinal link mass : "<<ml<<"Kg" ;
cout<<"\nfinal stress : "<<stress<<"Mpa" ;
  }
  void savefile(Material* database, int currentCount )
  {
      ofstream file("saved materials.txt");
      if(file.is_open())
      {
        for(int i=0;i<currentCount;i++)
        {
            file<<database[i].name<<"|" <<database[i].yieldStrength<<"|"<<database[i].density<<endl ;
        }
        file.close();
      }
  }



int main()
{
    // user inputs for design
   int currentCount = 0;
    Material* database = 0;
    ifstream infile("saved materials.txt");
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            if (!line.empty()) currentCount++;
        }

        infile.clear();
        infile.seekg(0);

        database = new Material[currentCount];
        for (int i = 0; i < currentCount; i++)
        {
            string n, ys_str, d_str;
            if (getline(infile, n, '|') && getline(infile, ys_str, '|') && getline(infile, d_str))
            {
                database[i].setData(n, stod(ys_str), stod(d_str));
            }
        }
        infile.close();
        cout << "Database loaded. Total materials: " << currentCount << endl;
    }
    else {
        currentCount=9;
        database=new Material[currentCount];
   database[0].setData("cast iron",130,7.3);
   database[1].setData("copper nickel",130,8.94);
   database[2].setData("brass",200,8.73);
   database[3].setData("Aluminum",241,2.7);
   database[4].setData("steel",247,7.58);
   database[5].setData("acrylic",72,1.16);
   database[6].setData("copper",70,8.92);
   database[7].setData("stainless steel",275,7.86);
   database[8].setData("Tungsten",941,19.25);

   ofstream file("saved materials.txt");
   for(int i=0;i<currentCount;i++)
   {
        file<<database[i].name<<"|" <<database[i].yieldStrength<<"|"<<database[i].density<<endl ;
   }

   file.close();

    }

   //now we see if the user want to choose from the database or add new material
   int choice;
    while (true) {
        cout << "material selection \n" ;
        cout << "1- Display All Materials \n" ;
        cout << "2- add a Custom Material \n";
        cout << "enter selection: ";
        cin >> choice;

        switch (choice){
            case 1: //choosing from the database
                {
                for (int i = 0; i < currentCount; i++)
                {
                cout <<i+1<<"."<< database[i].name << endl;
                }
                int selection ;
                cin>> selection;
                Material m = database[selection-1];
               design(database[selection -1]);
               break ;
                }
            case 2:
                {
                Material* temp = new Material[currentCount + 1];
                for(int i =0;i<currentCount;i++) temp[i]=database[i];
                string n ;
                double y,d ;
                cout<<" enter material name:";
                cin.ignore();
                getline(cin,n);
                cout<<"\nenter yield strength(Mpa):";
                cin>>y;
                cout<<"\nenter density(g/cm3):" ;
                cin>>d ;

                temp[currentCount].setData(n,y,d);
                delete[] database;
                database=temp;
                currentCount++ ;
                savefile(database,currentCount);
                design(database[currentCount -1]);
                break;
            }

       }




break;
                  }
                  delete[] database;

return 0 ;
}

