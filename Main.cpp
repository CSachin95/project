
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"



int main(void)
{
	const double h = 1e-6;
	const double tmax = 5e-3;
	const double Va = 10;
	const double f = 1000;
	const double Ra = R1;
	const double Rb = R2;
	const double Rc = R3;
	const double Ca = C1;
	const double Cb = C2;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vin (V)", "iBatt (A)", "vout (V)");

	Simulator simulator(nodepos, nodeneg);

	VoltageSource V1(int1, nodeneg, 0, Va, f);
	Resistor R1(int1, int2, Ra);
	Resistor R2(int2, int3, Rb);
	Resistor R3(int3, nodepos, Rc);
	Capacitor C1(int2, int3, Ca);
	Capacitor C2(int3, nodepos, Cb);

	simulator.AddDevice(V1);
	simulator.AddDevice(R1);
	simulator.AddDevice(R2);
	simulator.AddDevice(R3);
	simulator.AddDevice(C1);
	simulator.AddDevice(C2);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), V1.GetVoltage(),
			R1.GetCurrent(), C1.GetVoltage());

		simulator.Step();
	}

	plotter.Plot();

	return 0;
}