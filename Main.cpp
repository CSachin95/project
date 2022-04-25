
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"

int main()
{
	const double h = 0.01;
	const double tmax = 1.0;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("batt SOC", "Rload Current", "Rload Voltage");

	Simulator simulator(1, 0);
	
	Battery batt(1, 0, 0.9);
	Resistor Rload(1, 0, 1.0);

	simulator.AddDevice(batt);
	simulator.AddDevice(Rload);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		simulator.Step();

		plotter.AddRow(simulator.GetTime(), batt.GetSOC(), Rload.GetCurrent(), Rload.GetVoltage());  

		
	}

	plotter.Plot();

	return 0;
}