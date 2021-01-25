#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "../source/ifNetwork.h"

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		std::cout << "No seed argument given." << std::endl;
		return 1;
	}

	srand(time(NULL));
	const bool pruning = false;
	const bool local_pruning = false;
	const int N = 5120;
	const bool k_homogeneous = true;
	const double g_ei = 0;
	const double g_ie = 0;
	const double w0_ee = 0.75;
	const double input_rate = 0.005;
	const double start_measure_time = 550000.;

	//std::ofstream raster_file;
	//raster_file.open("raster_uncoupled_0.75.dat");

	//std::ofstream connections_file;
	//connections_file.open("connections.dat");
	
	IFNetwork netspike(k_homogeneous, pruning, local_pruning, g_ei, g_ie, "test", atoi(argv[1]));

	//netspike.print_connections(connections_file);

	for(int i = 0; i <	100; i++)
	{
		std::cout << i*0.1 << std::endl;
		//if(i == 90000)
		//	netspike.change_input_rate(0);

		netspike.update();
		//netspike.print_spikes(raster_file);
	}

	//std::cout << "# " << netspike.calculate_syncronization() << " " << netspike.calculate_mean_fire_rate() << " " << netspike.calculate_fano_factor() << std::endl;

	return 0;
}
