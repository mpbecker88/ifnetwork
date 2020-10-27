#include "getParameters.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

gsl_rng *r;

InputParameters::InputParameters(std::string p_file_name)
{
	gsl_rng_env_setup();
	r = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, time(NULL));

	std::ifstream p_file;

	p_file.open(p_file_name);

	get_parameters_from_file(p_file);

	p_file.close();
}

InputParameters::~InputParameters()
{
	gsl_rng_free(r);
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

double InputParameters::get_alpha_E()
{
	if(m_is_alpha_E_random)
	{
		return gsl_ran_flat(r, 0.2, 1.0);
	}
	else
	{
		return m_alpha_E;
	}
}

double InputParameters::get_alpha_I()
{
	if(m_is_alpha_I_random)
	{
		return gsl_ran_flat(r, 0.2, 1.0);
	}
	else
	{
		return m_alpha_I;
	}
}