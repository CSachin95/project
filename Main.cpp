#include "Simulator.h"

#include "Capacitor.h"
#include "Resistor.h"
#include "VoltageSource.h"

class Battery : public Device
{
    /*
        (int1)         v2        v3
      v1  o------VVV----o-VVV----o-VVV-----o(nodepos)  vpos
        + |             |    |   |    |
          |             '-||-'   '-||-'
          |
 Vin(soc)( ) o(int2) ibatt
          |
          |
          |
          '-------------------------------o(nodeneg)  vneg

               [ v1    ]  int1
               [ v2    ]  int2
           x = [ v3    ]  int3
               [ ibatt ]  int4
               [ vpos  ]  nodepos
               [ vneg  ]  nodeneg

     */

public:
    // Constructor (external nodes and params):
    Battery(int nodepos, int nodeneg, double soci);

    // Device interface:
    void Step(double t, double dt);
    void Init();

    // Viewable functions:
    double GetVoltage();
    double GetCurrent();
    double GetSOC();

    // f(soc) functions:
    double GetVin(double soc);

    double GetR1(double soc);
    double GetR2(double soc);
    double GetR3(double soc);

    double GetC1(double soc);
    double GetC2(double soc);

    // Member variables:
    int nodepos;
    int nodeneg;
    int int1;
    int int2;
    int int3;
    int int4;
    double soci;
    double soc;  // state of charge
};
Battery::Battery(int nodepos, int nodeneg, double soci)
{
    this->nodepos = nodepos;
    this->nodeneg = nodeneg;
    this->soci = soci;
}
void Battery::Init()
{
    int1 = GetNextNode();
    int2 = GetNextNode();
    int3 = GetNextNode();
    int4 = GetNextNode();
    soc = soci;
}
void Battery::Step(double t, double h)
{
    double Vin = GetVin(soc);
    double g1 = 1 / GetR1(soc);
    double g2 = 1 / GetR2(soc);
    double g3 = 1 / GetR3(soc);

    double wh = 8.1;
    double C1 = GetC1(soc);
    double C2 = GetC2(soc);

    //const double h1 = 0.001; // (h value not given so keep it at 0.001)

    // R1: 
    AddJacobian(int1, int1, g1);
    AddJacobian(int2, int1, -g1);
    AddJacobian(int1, int2, -g1);
    AddJacobian(int2, int2, g1);
    // R2:
    AddJacobian(int2, int2, g2);
    AddJacobian(int3, int1, -g2);
    AddJacobian(int2, int3, -g2);
    AddJacobian(int3, int3, g2);
    // R3:
    AddJacobian(int3, int3, g3);
    AddJacobian(int4, int3, -g3);
    AddJacobian(int3, int4, -g3);
    AddJacobian(int4, int4, g3);

    //Vin  (int5=nodepos) (int6=nodeneg)
    AddJacobian(int1, int1, 0);
    AddJacobian(nodeneg, int1, 0);
    AddJacobian(nodepos, int1, 1);

    AddJacobian(int1, nodeneg, 0);
    AddJacobian(nodeneg, nodeneg, 0);
    AddJacobian(nodepos, nodeneg, -1);

    AddJacobian(int1, nodepos, 1);
    AddJacobian(nodeneg, nodepos, -1);
    AddJacobian(nodepos, nodepos, 0);
    //AddBEquivalent(int2, Vin);

    //C1   
    AddJacobian(int2, int2, C1 / h);
    AddJacobian(int3, int2, -C1 / h);
    AddJacobian(int2, int3, -C1 / h);
    AddJacobian(int3, int3, C1 / h);
    //AddBEquivalent(int2, Vin);

    //C2
    AddJacobian(int3, int3, C2 / h);
    AddJacobian(int4, int3, -C2 / h);
    AddJacobian(int3, int4, -C2 / h);
    AddJacobian(int4, int4, -C2 / h);
    // AddBEquivalent(int2, Vin);

    // update soc:
    soc = soc + GetVoltage() * GetCurrent() * h / (wh * 3600);
}



    double Battery::GetVoltage()
{
    return GetStateDifference(nodepos, nodeneg);
}
double Battery::GetCurrent()
{
    return GetState(int4); //int4 is ibatt
}
double Battery::GetSOC()
{
    return soc;
}


// 𝑿(𝑺𝑶𝑪) = 𝑨𝒆^𝒌⋅𝑺𝑶𝑪 + 𝒂𝟎 + 𝒂𝟏𝑺𝑶𝑪 + 𝒂𝟐𝑺𝑶𝑪^𝟐 + 𝒂𝟑𝑺𝑶𝑪^3
double Battery::GetVin(double soc)
{
    //return 3.8 * soc;  // simple linear model
    return  ( - 1.031) * exp(-35.0 * soc) + 3.685 + 0.2165 * soc + (-0.1178) * (soc) * exp(2) + 0.3201 * (soc) * exp(3);
}

double Battery::GetR1(double soc)
{
    //return 0.1 + (1 - soc) * 0.01;  // simple linear model
    return (0.1562) * exp (-24.37 * soc) + 0.07446 + 0 * (soc)+ 0 * (soc) * exp(2) + 0 * (soc) * exp(3);
}
double Battery::GetR2(double soc)
{
    return (0.3208) * exp (-29.14 * soc) + 0.04669 + 0 * (soc)+ 0 * (soc) * exp(2) + 0 * (soc) * exp(3);
}
double Battery::GetR3(double soc)
{
    return (6.603) * exp (-155.2 * soc) + 0.04984 + 0 * (soc)+ 0 * (soc) * exp(2) + 0 * (soc) * exp(3);
}

double Battery::GetC1(double soc)
{
    return (-752.9) * exp (-13.51 * soc) + 703.6 + 0 * (soc)+0 * (soc) * exp(2) + 0 * (soc) * exp(3);
}
double Battery::GetC2(double soc)
{
    return (-6056.0) * exp (-27.12 * soc) + 4475.0 + 0 * (soc)+0 * (soc) * exp(2) + 0 * (soc) * exp(3);
}
