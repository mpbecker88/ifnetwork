#ifndef GETPARAMETERS_H
#define GETPARAMETERS_H

#include <fstream>
#include <string>

class InputParameters
{
public:
	InputParameters(std::string parameters_file);
	~InputParameters();
	int get_N(){return m_N;};
	double get_w0_EE(){return m_w0_EE;}
	double get_w0_stim(){return m_w0_stim;}
	double get_input_rate(){return m_input_rate;};
	double get_input_amplitude(){return m_input_amplitude;}
	double get_input_frequency(){return m_input_freq;}
	double get_start_measure_time(){return m_start_measure_time;};
	double get_alpha_E(){return m_alpha_E;};
	double get_alpha_I(){return m_alpha_I;};
	bool is_alpha_E_random(){return m_is_alpha_E_random;};
	bool is_alpha_I_random(){return m_is_alpha_I_random;};


private:
	int m_N;  // Network size.
	double m_w0_EE;  // Initial weight for EE connection.
	double m_w0_stim;  // Initial weight from the external input.
	double m_input_rate;  // Rate o firing of the input layer.
	double m_input_amplitude;  // Amplitude of input oscilation (0 for constant input).
	double m_input_freq;  // Frequency of input oscilation;
	double m_start_measure_time;  // Starting time for syncronization mesures.
	double m_alpha_E;  // AMPA/NMDA channel ratio for excitatory targets.
	double m_alpha_I;  // AMPA/NMDA channel ratio for inhibitory targets.
	bool m_is_alpha_E_random; // Is the AMPA/NMDA channel ratio for excitatory targets random?
	bool m_is_alpha_I_random; // Is the AMPA/NMDA channel ratio for inhibitory targets random?

	void get_parameters_from_file(std::ifstream &file_name);
};


#endif