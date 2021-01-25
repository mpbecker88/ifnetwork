#include <iostream>
#include <fstream>
#include <string>
#include "../source/ifNeuron.h"

double dt = 0.1;

int main()
{
	double alpha = 0.5;
	IFNeuron ifn(true, 0, alpha, dt);
	bool spiked;

	for(int i = 0; i < 5000; i++)
	{

		if(i==30)
		{
			ifn.inject_ex_current(10.1);
		}
		else if(i==30)
		{
			ifn.inject_in_current(0.1);
		}
		else if(i==200)
		{
			ifn.inject_ex_current(0.);
		}

		spiked = ifn.update(dt);

		std::cout << i*dt << ' ' << ifn.get_V() << " " << ifn.get_ampa_conductance() << " " << ifn.get_nmda_conductance() << " " << ifn.get_gaba_conductance() << " " << ifn.get_threshold() << std::endl;
	}
}
