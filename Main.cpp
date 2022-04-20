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
    double GetR(double soc);
    double GetC(double soc);

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
    double g = 1 / GetR(soc);
    double wh = 8.1;
    double C= GetC(soc);
    const double h = 0.001; // (h value not given so keep it at 0.001)

    // R1:
    AddJacobian(int1, int1, g);
    AddJacobian(int2, int1, -g);
    AddJacobian(int1, int2, -g);
    AddJacobian(int2, int2, g);
    // R2:
    AddJacobian(int2, int2, g);
    AddJacobian(int3, int1, -g);
    AddJacobian(int2, int3, -g);
    AddJacobian(int3, int3, g);
    // R3:
    AddJacobian(int3, int3, g);
    AddJacobian(int4, int3, -g);
    AddJacobian(int3, int4, -g);
    AddJacobian(int4, int4, g);
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
    AddJacobian(int2, int2, C/h);
    AddJacobian(int3, int2, -C/h);
    AddJacobian(int2, int3, -C/h);
    AddJacobian(int3, int3, C/h);
    //AddBEquivalent(int2, Vin);
    
    //C2
    AddJacobian(int3, int3, C/h);
    AddJacobian(int4, int3, -C/h);
    AddJacobian(int3, int4, -C/h);
    AddJacobian(int4, int4, -C/h);
   // AddBEquivalent(int2, Vin);
   
   // update soc:
    soc = soc + GetVoltage() * GetCurrent() * h / (wh * 3600);
}