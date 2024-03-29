#include <iostream>
#include <fstream>
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

	std::ofstream sync_file, firerate_file, fano_file;
	sync_file.open("sync.dat");
	firerate_file.open("firerate.dat");
	fano_file.open("fano.dat");

	srand(time(NULL));
	const bool pruning = false;
	const bool local_pruning = false;
	const bool k_homogeneous = false;

	double w0_ee = 0.1;
	double g_ei = 1.0;
	while(g_ei < 4.1)
	{
		double g_ie = 1.0;
		while(g_ie < 4.1)
		{
			std::cout << g_ei << " " << g_ie << std::endl;
			//std::ofstream potential_file;
			std::ofstream raster_file;
			std::ofstream connectivity_file;
			//std::ofstream weights_file;
			//std::ofstream weightMatrix_file;
			std::ofstream currents_file;

			//potential_file.open("potential" + std::to_string(g_ei) + "_" + std::to_string(g_ie) + ".dat");
			raster_file.open("raster" + std::to_string(g_ei) + "_" + std::to_string(g_ie) + ".dat");
			//weightMatrix_file.open("weight_matrix" + std::to_string(g_ei) + "_" + std::to_string(g_ie) + ".dat");
			currents_file.open("currents" + std::to_string(g_ei) + "_" + std::to_string(g_ie) + ".dat");

			IFNetwork netspike(k_homogeneous, pruning, local_pruning, g_ei, g_ie, "sim", atoi(argv[1]));
			//netspike.print_weightMatrix(weightMatrix_file);

			for(int i = 0; i <	500000; i++)
			{
				int pre_spikes, pos_spikes;
				double pre_trace, pos_trace;

				if(!(i%10000))
					std::cout << i*0.1 << std::endl;

				netspike.update();

				netspike.print_spikes(raster_file);
				//netspike.print_potentials(potential_file);
				netspike.print_currents(currents_file);
			}

			sync_file << netspike.calculate_syncronization() << " ";
			firerate_file << netspike.calculate_mean_fire_rate() << " ";
			fano_file << netspike.calculate_fano_factor() << " ";

			//potential_file.close();
			raster_file.close();
			connectivity_file.close();
			//weights_file.close();
			//weightMatrix_file.close();
			currents_file.close();

			g_ie += 0.25;
		}

		sync_file << std::endl;
		firerate_file << std::endl;
		fano_file << std::endl;

		g_ei += 0.25;
	}

	sync_file.close();
	firerate_file.close();
	fano_file.close();

	return 0;
}
