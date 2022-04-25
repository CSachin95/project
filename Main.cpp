
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"

float nodepos;
float nodeneg;
float R1;
float R2;
float R3;
float C1;
float C2;

int main()
{
	const double h = 1e-6;
	const double tmax = 5e-3;
	const double Va = 10;
	const double f = 1000;
	const double R1a = R1;
	const double R2a = R2;
	const double R3a = R3;
	const double C1a = C1;
	const double C2a = C2;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vin (V)", "iVbatt (A)", "vout (V)");

	Simulator simulator(nodepos, nodeneg);

	VoltageSource V1(1, 0, 0, Va, f);

	simulator.AddDevice(V1);
	simulator.AddDevice(R1a);
	simulator.AddDevice(R2a);
	simulator.AddDevice(R3a);
	simulator.AddDevice(C1a);
	simulator.AddDevice(C2a);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), V1.GetVoltage(),
			R1a.GetCurrent(), R2a.GetCurrent(), R3a.GetCurrent(), C1a.GetVoltage(), C2a.GetVoltage());

		simulator.Step();
	}

	plotter.Plot();

	return 0;
}