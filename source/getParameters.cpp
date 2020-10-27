#include "getParameters.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

InputParameters::InputParameters(std::string p_file_name)
{
	std::ifstream p_file;

	p_file.open(p_file_name);

	get_parameters_from_file(p_file);

	p_file.close();
}

InputParameters::~InputParameters()
{
}

void InputParameters::get_parameters_from_file(std::ifstream &p_file)
{
	std::string var_name; 

	// Get network size.
	p_file >> var_name;
	p_file >> m_N;

	// Get EE weight.
	p_file >> var_name;
	p_file >> m_w0_EE;

	// Get stim weight.
	p_file >> var_name;
	p_file >> m_w0_stim;

	// Get input rate.
	p_file >> var_name;
	p_file >> m_input_rate;

	// Get input amplitude.
	p_file >> var_name;
	p_file >> m_input_amplitude;

	// Get input frequency.
	p_file >> var_name;
	p_file >> m_input_freq;

	// Get start measure time.
	p_file >> var_name;
	p_file >> m_start_measure_time;

	// Get alpha_E.
	p_file >> var_name;
	p_file >> m_alpha_E;

	// Get alpha_I.
	p_file >> var_name;
	p_file >> m_alpha_I;

	// Get is alpha_E random.
	p_file >> var_name;
	p_file >> m_is_alpha_E_random;

	// Get is alpha_I random.
	p_file >> var_name;
	p_file >> m_is_alpha_I_random;
}