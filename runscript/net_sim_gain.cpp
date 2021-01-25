#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "../source/ifNetwork.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	std::ofstream g_file;

	const int N = 5000;
	const int n = 100;
	const bool k_homogeneous = false;
	const double w_base = 0.4;
	const double input_rate = 0.05;
	const double start_measure_time = 500.;

	double mean_delay = 1.0;
	double sigma_delay = 1.0;

	double mean_sync;
	double sigma_sync;

	double mean_rate;
	double sigma_rate;

	double mean_fano;
	double sigma_fano;

	std::vector<double> v_sync;
	std::vector<double> v_rate;
	std::vector<double> v_fano;

	v_sync.reserve(n);
	v_rate.reserve(n);
	v_fano.reserve(n);

	g_file.open("g_sync_rate.dat");


	for(double g = 1.0; g <= 5.01; g += 0.1)
	{
		double sum_rate = 0;
		double sum_rate2 = 0;
		double sum_sync = 0;
		double sum_sync2 = 0;
		double sum_fano = 0;
		double sum_fano2 = 0;

		for (int i = 0; i < n; ++i)
		{
			std::cout << g << " " << i << std::endl;

			IFNetwork netspike(N, k_homogeneous, mean_delay, sigma_delay, w_base, g, input_rate, start_measure_time, std::to_string(i), rand() % 100000);

			for(double t = 0; t < 1000; t += 0.1)
			{
				netspike.update();
			}

			v_sync.push_back(netspike.calculate_syncronization());
			v_rate.push_back(netspike.calculate_mean_fire_rate());
			v_fano.push_back(netspike.calculate_fano_factor());
		}


		for(int i = 0; i < n; i++)
		{
			sum_sync += v_sync[i];
			sum_sync2 += v_sync[i]*v_sync[i];

			sum_rate += v_rate[i];
			sum_rate2 += v_rate[i]*v_rate[i];

			sum_fano += v_fano[i];
			sum_fano2 += v_fano[i]*v_fano[i];
		}

		mean_sync = sum_sync/(n*1.0);
		sigma_sync = sqrt((sum_sync2/(n*1.0)) - mean_sync*mean_sync);

		mean_rate = sum_rate/(n*1.0);
		sigma_rate = sqrt((sum_rate2/(n*1.0)) - mean_rate*mean_rate);

		mean_fano = sum_fano/(n*1.0);
		sigma_rate = sqrt((sum_fano2/(n*1.0)) - mean_fano*mean_fano);

		g_file << g << " " << mean_sync << " " << sigma_sync << " " << mean_rate << " " << sigma_rate << " " << mean_fano << " " << sigma_fano << std::endl;


		v_sync.clear();
		v_rate.clear();
		v_fano.clear();

	}


	g_file.close();
	return 0;
}
