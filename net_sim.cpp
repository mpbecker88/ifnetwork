#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "ifNetwork.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	std::ofstream ind_file;
	std::ofstream rate_file;
	std::ofstream sync_file;
	std::ofstream fano_file;
	std::ofstream s_rate_file;
	std::ofstream s_sync_file;
	std::ofstream s_fano_file;

	const int N = 5000;
	const int n = 100;
	const bool sync = true;
	const double w_base = 0.4;
	const double g = 2.5;
	const double input_rate = 0.05;
	const double start_measure_time = 500.;

	double mean_delay = 0.0;
	double sigma_delay = 0.0;

	double sync_matrix[11][11];
	double rate_matrix[11][11];
	double fano_matrix[11][11];
	double s_fano_matrix[11][11];
	double s_sync_matrix[11][11];
	double s_rate_matrix[11][11];
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

	for(int i = 0; i <= 10; i++)
	{
		for(int j = 0; j <= 10; j++)
		{
			sync_matrix[i][j] = -1;
			rate_matrix[i][j] = -1;
			s_sync_matrix[i][j] = -1;
			s_rate_matrix[i][j] = -1;
		}
	}


	ind_file.open("individuals_sync_rate.dat");


	for(mean_delay = 0; mean_delay <= 1.01; mean_delay += 0.1)
	{
		for(sigma_delay = 0; sigma_delay <= mean_delay + 0.01; sigma_delay += 0.1)
		{
			double sum_rate = 0;
			double sum_rate2 = 0;
			double sum_sync = 0;
			double sum_sync2 = 0;
			double sum_fano = 0;
			double sum_fano2 = 0;

			for (int i = 0; i < n; ++i)
			{
				std::cout << mean_delay << " " << sigma_delay << " " << i << std::endl;

				IFNetwork netspike(N, sync, mean_delay, sigma_delay, w_base, g, input_rate, start_measure_time, std::to_string(i), rand() % 100000);

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
				ind_file << mean_delay << " " << sigma_delay << " " << v_sync[i] << " ";

				sum_rate += v_rate[i];
				sum_rate2 += v_rate[i]*v_rate[i];
				ind_file << v_rate[i] << " ";
				

				sum_fano += v_fano[i];
				sum_fano2 += v_fano[i]*v_fano[i];
				ind_file << v_fano[i] << std::endl;
			}

			mean_sync = sum_sync/(n*1.0);
			sigma_sync = sqrt((sum_sync2/(n*1.0)) - mean_sync*mean_sync);

			mean_rate = sum_rate/(n*1.0);
			sigma_rate = sqrt((sum_rate2/(n*1.0)) - mean_rate*mean_rate);

			mean_fano = sum_fano/(n*1.0);
			sigma_fano = sqrt((sum_fano2/(n*1.0)) - mean_fano*mean_fano);

			sync_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = mean_sync;
			rate_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = mean_rate;
			fano_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = mean_fano;
			s_sync_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = sigma_sync;
			s_rate_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = sigma_rate;
			s_fano_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] = sigma_fano;

			std::cout << std::endl << (int)std::round(mean_delay*10) << " " << (int)std::round(sigma_delay*10) << " " << sync_matrix[(int)std::round(mean_delay*10)][(int)std::round(sigma_delay*10)] << std::endl << std::endl; 

			v_sync.clear();
			v_rate.clear();
			v_fano.clear();

		}

	}

	ind_file.close();

	sync_file.open("sync.dat");
	rate_file.open("rate.dat");
	fano_file.open("fano.dat");
	s_sync_file.open("sigma_sync.dat");
	s_rate_file.open("sigma_rate.dat");
	s_fano_file.open("sigma_fano.dat");


	for(int i = 10; i >= 0; i--)
	{
		for(int j = 0; j <= 10; j++)
		{
			if(sync_matrix[i][j] >= 0)
				sync_file << sync_matrix[i][j] << " ";
			else
				sync_file << "NaN ";

			if(s_sync_matrix[i][j] >= 0)
				s_sync_file << s_sync_matrix[i][j] << " ";
			else
				s_sync_file << "NaN ";

			if(rate_matrix[i][j] >= 0)
				rate_file << rate_matrix[i][j] << " ";
			else
				rate_file << "NaN ";

			if(s_rate_matrix[i][j] >= 0)
				s_rate_file << s_rate_matrix[i][j] << " ";
			else
				s_rate_file << "NaN ";

			if(fano_matrix[i][j] >= 0)
				fano_file << fano_matrix[i][j] << " ";
			else
				fano_file << "NaN ";

			if(s_fano_matrix[i][j] >= 0)
				s_fano_file << s_fano_matrix[i][j] << " ";
			else
				s_fano_file << "NaN ";
		}
		
		sync_file << std::endl;
		rate_file << std::endl;
		fano_file << std::endl;
		s_sync_file << std::endl;
		s_rate_file << std::endl;
		s_fano_file << std::endl;
	}


	sync_file.close();
	rate_file.close();
	fano_file.close();
	s_sync_file.close();
	s_rate_file.close();
	s_fano_file.close();

	return 0;
}