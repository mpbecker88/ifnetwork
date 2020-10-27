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
	std::ofstream potential_file;
	std::ofstream raster_file;
	std::ofstream connectivity_file;
	std::ofstream weights_file;
	std::ofstream connections_file;
	std::ofstream weightMatrixBegin_file;
	std::ofstream weightMatrixEnd_file;
	std::ofstream currents_file;

	potential_file.open("potential.dat");
	raster_file.open("raster.dat");
	weights_file.open("weights_evolution.dat");
	connections_file.open("connections.dat");
	weightMatrixBegin_file.open("weight_matrix_begin.dat");
	weightMatrixEnd_file.open("weight_matrix_end.dat");
	currents_file.open("currents.dat");

	srand(time(NULL));
	const bool pruning = false;
	const bool local_pruning = false;
	const int N = 5120;
	const bool symmetric = true;
	const double g_ei = 1;
	const double g_ie = 1;
	const double w0_ee = 0.1;
	const double input_rate = 0.005;
	const double start_measure_time = 500.;

	IFNetwork netspike(N, symmetric, pruning, local_pruning, w0_ee, g_ei, g_ie, input_rate, start_measure_time, "test", atoi(argv[1]));
	netspike.print_connections(connections_file);
	netspike.print_weightMatrix(weightMatrixBegin_file);

	for(int i = 0; i <	100000; i++)
	{
		int pre_spikes, pos_spikes;
		double pre_trace, pos_trace;

		if(!(i%10000))
			std::cout << i*0.1 << std::endl;

		netspike.update();

		netspike.print_spikes(raster_file);
		netspike.print_potentials(potential_file);
		netspike.print_currents(currents_file);
		netspike.print_weights(weights_file);
	}
	netspike.print_weightMatrix(weightMatrixEnd_file);

	std::cout << "# " << netspike.calculate_syncronization() << " " << netspike.calculate_mean_fire_rate() << " " << netspike.calculate_fano_factor() << std::endl;

	return 0;
}
