
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"

int main()
{
	const double h = 0.01; //Timestep 0.1sec
	const double tmax = 10.0; //Run time 10 sec

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("batt SOC", "Rload Current", "Rload Voltage");

	Simulator simulator(1, 0);
	
	Battery batt(1, 0, 0.9); //SOC 90%
	Resistor Rload(1, 0, 0.1); //Rload 0.1 Ohms 

	simulator.AddDevice(batt);
	simulator.AddDevice(Rload);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{

		plotter.AddRow(simulator.GetTime(), batt.GetSOC());  

		simulator.Step();
	}

	plotter.Plot();

	return 0;
}