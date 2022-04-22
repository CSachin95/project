
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"

float nodepos;
float nodeneg;

int main()
{
	Battery GetC1();
	Battery GetC2();
	Battery GetR1();
	Battery GetR2();
	Battery GetR3();
	Battery GetV1();

	Simulator simulator(nodeneg, nodepos);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), Battery.GetC1(),
			R1.GetCurrent(), C1.GetVoltage());

		simulator.Step();
	}

	plotter.Plot();

	return 0;
}