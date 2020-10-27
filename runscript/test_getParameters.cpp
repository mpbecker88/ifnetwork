#include <iostream>
#include <fstream>

#include "../source/getParameters.h"

int main()
{
	InputParameters inp_par("parameters.dat");

	std::cout << "w0_EE " << inp_par.get_w0_EE() << std::endl;
	std::cout << "input_rate " << inp_par.get_input_rate() << std::endl;
	std::cout << "input_amplitude  " <<  inp_par.get_input_amplitude() << std::endl;
	std::cout << "input_freq " <<  inp_par.get_input_frequency() << std::endl;
	std::cout << "smt " <<  inp_par.get_start_measure_time() << std::endl;
	std::cout << "alpha_E " <<  inp_par.get_alpha_E() << std::endl;
	std::cout << "alpha_I " <<  inp_par.get_alpha_I() << std::endl;

	return 0;
}
