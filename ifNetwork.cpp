#include "ifNetwork.h"
#include "ifNeuron.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


const double epsilon = 0.00001;
gsl_rng *r;

IFNetwork::IFNetwork(int N, bool symmetric, bool pruning, bool local_pruning, double w0_ee, double g_ei, double g_ie, double input_rate, double start_measure_time, std::string sim_number, int seed)
{

	// select random number generator
	gsl_rng_env_setup();
	r = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, seed);

	m_t = 0;
	m_dt = 0.1;

	m_pruning = pruning;
	m_local_pruning = local_pruning;

	m_N = N;
	m_Ni = N/5;
	m_Ne = m_N - m_Ni;
	m_Ninp = m_Ne;
	m_Npruned = 0;

	m_sum_out_EE = 0;
	m_sum_out_EI = 0;
	m_sum_in_EE = 0;
	m_sum_in_EI = 0;
	m_sum_out_IE = 0;
	m_sum_out_II = 0;
	m_sum_in_IE = 0;
	m_sum_in_II = 0;

	m_conn_prob_ee = 0.1;
	m_conn_prob_ei = 0.1;
	m_conn_prob_ie = 0.1;
	m_conn_prob_ii = 0.1;
	m_conn_prob_stim = 0.05;
	m_w0_ee = w0_ee;
	m_w0_ei = w0_ee*g_ei;
	m_w0_ie = w0_ee*g_ie;
	m_w0_ii = w0_ee*2;
	m_w0_stim = 0.05;

	m_input_rate = input_rate;

	m_eta = 0.001;
	m_gamma = 0.004;
	m_tau_HL = 10000;
	m_alpha = 0.12;
	m_B = 0.001;
	m_delta = 0.00002;
	m_sigma_ee_conn = 100;
	m_sigma_ei_conn = 100;
	m_sigma_ie_conn = 10;
	m_sigma_ii_conn = 10;
	m_H = m_gamma;

	m_expHL = exp(-m_dt/m_tau_HL);

	m_start_measure_time = start_measure_time;

	m_sum_mean_v = 0;
	m_sum_mean_v2 = 0;

	m_Neuron.reserve(N);
	m_spiked.reserve(N);

	print_parameters();

	std::cout << "####### initializing neurons" << std::endl;
	init_ex_neurons(symmetric);
	init_in_neurons(symmetric);
	adjust_connections();
	init_input_layer();
	init_spiked_vector();

	std::cout << "####### generating network connections" << std::endl;
	generate_connections();
	generate_input_connections();

	std::cout << "####### initiating simulation" << std::endl;
}


IFNetwork::~IFNetwork()
{
	gsl_rng_free(r);
}


void IFNetwork::init_ex_neurons(bool symmetric)
{
	if(symmetric)
	{
		for(int i = 0; i < m_Ne; i++)
		{
			double alpha = 1.0; //gsl_ran_flat(r, 0.2, 1.0);
			IFNeuron t_neuron = IFNeuron(true, 0, alpha, m_dt);
			Node t_node;

			t_node.n = t_neuron;
			t_node.k_out_E = m_conn_prob_ee*m_Ne;
			t_node.k_out_I = m_conn_prob_ei*m_Ni;
			t_node.k_in_E = m_conn_prob_ee*m_Ne;
			t_node.k_in_I = m_conn_prob_ie*m_Ni;
			t_node.L = m_gamma;
			t_node.sum_v = 0;
			t_node.sum_v2 = 0;
			t_node.n_spikes = 0;
			t_node.n_pos.reserve(t_node.k_out_E + t_node.k_out_I);

			m_sum_out_EE += t_node.k_out_E;
			m_sum_out_EI += t_node.k_out_I;
			m_sum_in_EE += t_node.k_in_E;
			m_sum_in_EI += t_node.k_in_I;

			m_Neuron.push_back(t_node);
		}
	}
	else
	{
		for(int i = 0; i < m_Ne; i++)
		{
			double alpha = 1.0; //gsl_ran_flat(r, 0.2, 1.0);
			IFNeuron t_neuron = IFNeuron(true, 0, alpha, m_dt);
			Node t_node;

			t_node.n = t_neuron;
			t_node.k_out_E = gsl_ran_binomial(r, m_conn_prob_ee, m_Ne);
			t_node.k_out_I = gsl_ran_binomial(r, m_conn_prob_ei, m_Ni);
			t_node.k_in_E = gsl_ran_binomial(r, m_conn_prob_ee, m_Ne);
			t_node.k_in_I = gsl_ran_binomial(r, m_conn_prob_ie, m_Ni);
			t_node.L = m_gamma;
			t_node.sum_v = 0;
			t_node.sum_v2 = 0;
			t_node.n_spikes = 0;
			t_node.n_pos.reserve(t_node.k_out_E + t_node.k_out_I);

			m_sum_out_EE += t_node.k_out_E;
			m_sum_out_EI += t_node.k_out_I;
			m_sum_in_EE += t_node.k_in_E;
			m_sum_in_EI += t_node.k_in_I;

			m_Neuron.push_back(t_node);
		}
	}
}



void IFNetwork::init_in_neurons(bool symmetric)
{
	if(symmetric)
	{
		for(int i = m_Ne; i < m_N; i++)
		{
			double alpha = 1.0;
			IFNeuron t_neuron = IFNeuron(false, 0, alpha, m_dt);
			Node t_node;

			t_node.n = t_neuron;
			t_node.k_out_E = m_conn_prob_ie*m_Ne;
			t_node.k_out_I = m_conn_prob_ii*m_Ni;
			t_node.k_in_E = m_conn_prob_ei*m_Ne;
			t_node.k_in_I = m_conn_prob_ii*m_Ni;
			t_node.L = m_gamma;
			t_node.sum_v = 0;
			t_node.sum_v2 = 0;
			t_node.n_spikes = 0;
			t_node.n_pos.reserve(t_node.k_out_E + t_node.k_out_I);

			m_sum_out_IE += t_node.k_out_E;
			m_sum_out_II += t_node.k_out_I;
			m_sum_in_IE += t_node.k_in_E;
			m_sum_in_II += t_node.k_in_I;


			m_Neuron.push_back(t_node);
		}
	}
	else
	{
		for(int i = m_Ne; i < m_N; i++)
		{
			double alpha = 1.0;
			IFNeuron t_neuron = IFNeuron(false, 0, alpha, m_dt);
			Node t_node;

			t_node.n = t_neuron;
			t_node.k_out_E = gsl_ran_binomial(r, m_conn_prob_ie, m_Ne);
			t_node.k_out_I = gsl_ran_binomial(r, m_conn_prob_ii, m_Ni);
			t_node.L = m_gamma;
			t_node.sum_v = 0;
			t_node.sum_v2 = 0;
			t_node.n_spikes = 0;
			t_node.n_pos.reserve(t_node.k_out_E + t_node.k_out_I);

			m_sum_out_IE += t_node.k_out_E;
			m_sum_out_II += t_node.k_out_I;
			m_sum_in_IE += t_node.k_in_E;
			m_sum_in_II += t_node.k_in_I;

			m_Neuron.push_back(t_node);
		}
	}
}


void IFNetwork::adjust_connections()
{
	/** Adjust EE conections. */
	adjust_EE();

	/** Adjust EI conections. */
	adjust_EI();

	/** Adjust IE conections. */
	adjust_IE();

	/** Adjust II conections. */
	adjust_II();
}


void IFNetwork::adjust_EE()
{
	if(m_sum_out_EE > m_sum_in_EE)
	{
		while(m_sum_out_EE > m_sum_in_EE)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ne;
			m_Neuron[r_neuron].k_in_E++;
			m_sum_in_EE++;
		}
	}
	else if(m_sum_out_EE < m_sum_in_EE)
	{
		while(m_sum_out_EE < m_sum_in_EE)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ne;

			if(m_Neuron[r_neuron].k_in_E > m_conn_prob_ee*m_Ne*0.2)
			{
				m_Neuron[r_neuron].k_in_E--;
				m_sum_in_EE--;
			}
		}
	}
}


void IFNetwork::adjust_EI()
{
	if(m_sum_out_EI > m_sum_in_EI)
	{
		while(m_sum_out_EI > m_sum_in_EI)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ni + m_Ne;
			m_Neuron[r_neuron].k_in_E++;
			m_sum_in_EI++;
		}
	}
	else if(m_sum_out_EI < m_sum_in_EI)
	{
		while(m_sum_out_EI < m_sum_in_EI)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ni + m_Ne;

			if(m_Neuron[r_neuron].k_in_E > m_conn_prob_ei*m_Ne*0.2)
			{
				m_Neuron[r_neuron].k_in_E--;
				m_sum_in_EI--;
			}
		}
	}
}


void IFNetwork::adjust_IE()
{
	if(m_sum_out_IE > m_sum_in_IE)
	{
		while(m_sum_out_IE > m_sum_in_IE)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ne;
			m_Neuron[r_neuron].k_in_I++;
			m_sum_in_IE++;
		}
	}
	else if(m_sum_out_IE < m_sum_in_IE)
	{
		while(m_sum_out_IE < m_sum_in_IE)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ne;

			if(m_Neuron[r_neuron].k_in_I > m_conn_prob_ie*m_Ne*0.2)
			{
				m_Neuron[r_neuron].k_in_I--;
				m_sum_in_IE--;
			}
		}
	}
}


void IFNetwork::adjust_II()
{
	if(m_sum_out_II > m_sum_in_II)
	{
		while(m_sum_out_II > m_sum_in_II)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ni + m_Ne;
			m_Neuron[r_neuron].k_in_I++;
			m_sum_in_II++;
		}
	}
	else if(m_sum_out_II < m_sum_in_II)
	{
		while(m_sum_out_II < m_sum_in_II)
		{
			int r_neuron = gsl_rng_uniform(r)*m_Ni + m_Ne;

			if(m_Neuron[r_neuron].k_in_I > m_conn_prob_ii*m_Ne*0.2)
			{
				m_Neuron[r_neuron].k_in_I--;
				m_sum_in_II--;
			}
		}
	}
}


void IFNetwork::init_spiked_vector()
{
	for (int i = 0; i < m_N; i++)
	{
		m_spiked.push_back(false);
	}

	for (int i = 0; i < m_Ninp; i++)
	{
		m_ispiked.push_back(false);
	}
}


void IFNetwork::init_input_layer()
{
	for (int i = 0; i < m_Ninp; i++)
	{
		Poisson t_node;

		t_node.r = m_input_rate;
		t_node.k_out_E = gsl_ran_binomial(r, m_conn_prob_stim, m_Ne);
		t_node.k_out_I = gsl_ran_binomial(r, m_conn_prob_stim, m_Ni);
		t_node.n_pos.reserve(t_node.k_out_E + t_node.k_out_I);

		m_input.push_back(t_node);
	}
}


void IFNetwork::generate_connections()
{

	std::vector<Support> v_support_pre;
	std::vector<Support> v_support_pos;
	std::vector<Support>::iterator it;
	int conn_EE_left = m_sum_out_EE;
	int conn_EI_left = m_sum_out_EI;
	int conn_IE_left = m_sum_out_IE;
	int conn_II_left = m_sum_out_II;
	int i;
	int pre_index;
	double delay_const = sqrt(8)/63.;


	/** Set connections for excitatory neuron. */
	v_support_pre.reserve(m_Ne);
	v_support_pos.reserve(m_Ne);
	generate_support_vector(true, true, v_support_pre, v_support_pos);
	i = 0;
	std::cout << "####### generating EE connections" << std::endl;
	while(conn_EE_left > 0)
	{
		/** Set EE connection. */
		//std::cout << conn_EE_left << std::endl;
		pre_index = i%m_Ne;
		if(v_support_pre[pre_index].k_available > 0)
		{
			int pos_index;
			int r_neuron;
			double pre_x = pre_index%64;
			double pre_y = pre_index/64;
			double pos_x;
			double pos_y;
			double deltax;
			double deltay;
			double radius;

			bool selected = false;
			while(!selected)
			{
				pos_index = floor(gsl_rng_uniform(r)*v_support_pos.size());
				r_neuron = v_support_pos[pos_index].index;
				pos_x = r_neuron%64;
				pos_y = r_neuron/64;
				deltax = sqrt((pre_x - pos_x)*(pre_x - pos_x));
				deltay = sqrt((pre_y - pos_y)*(pre_y - pos_y));

				/** Shortest x distance in periodic boundary condition */
				if(deltax > 32)
				{
					deltax = 64 - deltax;
				}

				/** Shortest y distance in periodic boundary condition */
				if(deltay > 32)
				{
					deltay = 64 - deltay;
				}

				radius = sqrt(deltax*deltax + deltay*deltay);
				if(gsl_rng_uniform(r) < gsl_ran_gaussian_pdf(radius, m_sigma_ee_conn) + (m_Ne - v_support_pos.size())*(0.3/m_Ne))
					selected = true;
			}



			Connection t_conn;

			t_conn.index = r_neuron;
			t_conn.w = m_w0_ee;
			t_conn.delay = delay_const*radius;
			m_Neuron[pre_index].n_pos.push_back(t_conn);

			v_support_pre[pre_index].k_available--;
			v_support_pos[pos_index].k_available--;
			if(v_support_pos[pos_index].k_available == 0)
			{
				it = v_support_pos.begin();
				v_support_pos.erase(it + pos_index);
			}

			conn_EE_left--;
		}

		i++;
	}

	std::cout << "####### generating EI connections" << std::endl;
	v_support_pre.clear();
	v_support_pos.clear();
	v_support_pre.reserve(m_Ne);
	v_support_pos.reserve(m_Ni);
	generate_support_vector(true, false, v_support_pre, v_support_pos);
	i = 0;
	while(conn_EI_left > 0)
	{
		/** Set EI connection. */
		pre_index = i%m_Ne;
		if(v_support_pre[pre_index].k_available > 0)
		{
			int pos_index;
			int r_neuron;
			double pre_x = pre_index%64;
			double pre_y = pre_index/64;
			double pos_x;
			double pos_y;
			double deltax;
			double deltay;
			double radius;

			bool selected = false;
			while(!selected)
			{
				pos_index = floor(gsl_rng_uniform(r)*v_support_pos.size());
				r_neuron = v_support_pos[pos_index].index;
				pos_x = 0.5 + 2*((r_neuron - m_Ne)%32);
				pos_y = 0.5 + 2*((r_neuron - m_Ne)/32);
				deltax = sqrt((pre_x - pos_x)*(pre_x - pos_x));
				deltay = sqrt((pre_y - pos_y)*(pre_y - pos_y));

				/** Shortest x distance in periodic boundary condition */
				if(deltax > 32)
				{
					deltax = 64 - deltax;
				}

				/** Shortest y distance in periodic boundary condition */
				if(deltay > 32)
				{
					deltay = 64 - deltay;
				}

				radius = sqrt(deltax*deltax + deltay*deltay);
				if(gsl_rng_uniform(r) < gsl_ran_gaussian_pdf(radius, m_sigma_ei_conn) + (m_Ni - v_support_pos.size())*(0.3/m_Ni))
					selected = true;
			}

			Connection t_conn;

			t_conn.index = r_neuron;
			t_conn.w = m_w0_ei;
			t_conn.delay = delay_const*radius;
			m_Neuron[pre_index].n_pos.push_back(t_conn);

			v_support_pre[pre_index].k_available--;
			v_support_pos[pos_index].k_available--;
			if(v_support_pos[pos_index].k_available == 0)
			{
				it = v_support_pos.begin();
				v_support_pos.erase(it + pos_index);
			}

			conn_EI_left--;
		}

		i++;
	}

	/** Set inhibitory connections. */
	std::cout << "####### generating IE connections" << std::endl;
	v_support_pre.clear();
	v_support_pos.clear();
	v_support_pre.reserve(m_Ni);
	v_support_pos.reserve(m_Ne);
	generate_support_vector(false, true, v_support_pre, v_support_pos);
	i = 0;
	while(conn_IE_left > 0)
	{
		/** Set IE connection. */
		pre_index = i%m_Ni;
		if(v_support_pre[pre_index].k_available > 0)
		{
			double pre_x = 0.5 + 2*(pre_index%32);
			double pre_y = 0.5 + 2*(pre_index/32);
			int pos_index;
			int r_neuron;
			double pos_x;
			double pos_y;
			double deltax;
			double deltay;
			double radius;

			bool selected = false;
			while(!selected)
			{
				pos_index = floor(gsl_rng_uniform(r)*v_support_pos.size());
				r_neuron = v_support_pos[pos_index].index;
				pos_x = r_neuron%64;
				pos_y = r_neuron/64;
				deltax = sqrt((pre_x - pos_x)*(pre_x - pos_x));
				deltay = sqrt((pre_y - pos_y)*(pre_y - pos_y));

				/** Shortest x distance in periodic boundary condition */
				if(deltax > 32)
				{
					deltax = 64 - deltax;
				}

				/** Shortest y distance in periodic boundary condition */
				if(deltay > 32)
				{
					deltay = 64 - deltay;
				}

				radius = sqrt(deltax*deltax + deltay*deltay);
				if(gsl_rng_uniform(r) < gsl_ran_gaussian_pdf(radius, m_sigma_ie_conn) + (m_Ne - v_support_pos.size())*(0.3/m_Ne))
					selected = true;
			}

			Connection t_conn;

			t_conn.index = r_neuron;
			t_conn.w = m_w0_ie;
			t_conn.delay = delay_const*radius;
			m_Neuron[pre_index + m_Ne].n_pos.push_back(t_conn);

			v_support_pre[pre_index].k_available--;
			v_support_pos[pos_index].k_available--;
			if(v_support_pos[pos_index].k_available == 0)
			{
				it = v_support_pos.begin();
				v_support_pos.erase(it + pos_index);
			}

			conn_IE_left--;
		}

		i++;
	}

	std::cout << "####### generating II connections" << std::endl;
	v_support_pre.clear();
	v_support_pos.clear();
	v_support_pre.reserve(m_Ni);
	v_support_pos.reserve(m_Ni);
	generate_support_vector(false, false, v_support_pre, v_support_pos);
	i = 0;
	while(conn_II_left > 0)
	{
		/** Set II connection. */
		pre_index = i%m_Ni;
		if(v_support_pre[pre_index].k_available > 0)
		{
			int pos_index;
			int r_neuron;
			double pre_x = 0.5 + 2*(pre_index%32);
			double pre_y = 0.5 + 2*(pre_index/32);
			double pos_x;
			double pos_y;
			double deltax;
			double deltay;
			double radius;

			bool selected = false;
			while(!selected)
			{
				pos_index = floor(gsl_rng_uniform(r)*v_support_pos.size());
				r_neuron = v_support_pos[pos_index].index;
				pos_x = 0.5 + 2*((r_neuron - m_Ne)%32);
				pos_y = 0.5 + 2*((r_neuron - m_Ne)/32);
				deltax = sqrt((pre_x - pos_x)*(pre_x - pos_x));
				deltay = sqrt((pre_y - pos_y)*(pre_y - pos_y));

				/** Shortest x distance in periodic boundary condition */
				if(deltax > 32)
				{
					deltax = 64 - deltax;
				}

				/** Shortest y distance in periodic boundary condition */
				if(deltay > 32)
				{
					deltay = 64 - deltay;
				}

				radius = sqrt(deltax*deltax + deltay*deltay);
				if(gsl_rng_uniform(r) < gsl_ran_gaussian_pdf(radius, m_sigma_ii_conn) + (m_Ni - v_support_pos.size())*(0.3/m_Ni))
					selected = true;
			}

			Connection t_conn;

			t_conn.index = r_neuron;
			t_conn.w = m_w0_ii;
			t_conn.delay = delay_const*radius;
			m_Neuron[pre_index + m_Ne].n_pos.push_back(t_conn);

			v_support_pre[pre_index].k_available--;
			v_support_pos[pos_index].k_available--;
			if(v_support_pos[pos_index].k_available == 0)
			{
				it = v_support_pos.begin();
				v_support_pos.erase(it + pos_index);
			}

			conn_II_left--;
		}

		i++;
	}
}


void IFNetwork::generate_input_connections()
{
	/** Set connections for input layer neurons. */
	for(int i = 0; i < m_Ninp; i++)
	{
		/** Set Ex connection. */
		int j = 0;
		while(j < m_input[i].k_out_E)
		{
			int r_neuron = floor(gsl_rng_uniform(r)*m_Ne);

			if(!(is_in_vector(r_neuron, m_input[i].n_pos)) && (r_neuron != i))
			{
				Connection t_conn;

				t_conn.index = r_neuron;
				t_conn.w = m_w0_stim;
				m_input[i].n_pos.push_back(t_conn);

				j++;
			}
		}

		/** Set In connection. */
		j = 0;
		while(j < m_input[i].k_out_I)
		{
			int r_neuron = m_Ne + floor(gsl_rng_uniform(r)*m_Ni);

			if(!(is_in_vector(r_neuron, m_input[i].n_pos)) && (r_neuron != i))
			{
				Connection t_conn;

				t_conn.index = r_neuron;
				t_conn.w = m_w0_stim;
				m_input[i].n_pos.push_back(t_conn);

				j++;
			}
		}
	}
}


bool IFNetwork::is_in_vector(int n, const std::vector<Connection> &v)
{
	std::vector<Connection>::const_iterator i;

	for (i = v.begin(); i < v.end(); i++)
	{
		if((*i).index == n)
			return true;
	}

	return false;
}


void IFNetwork::update()
{
	//std::cout << m_t << std::endl;

	update_input_layer();
	distribute_input_spikes();
	check_spikes();
	distribute_spikes();

	//STDP();
	//iSTDP();
	//iSTDP_global_factor();
	//iSTDP_local_factor();

	for(int i = 0; i < m_N; i++)
	{
		m_spiked[i] = m_Neuron[i].n.update(m_dt);
		m_Neuron[i].n.activity(m_spiked[i]);
	}

	if(m_pruning)
	{
		if((std::remainder(m_t, 1000) < 0.001) && (std::remainder(m_t, 1000) > -0.001))
		{
			int nrep = 0;
			while((nrep < 5) && (m_Npruned < m_Ni))
			{
				pruning(m_local_pruning);
				nrep++;
			}
		}
	}

	if(m_t >= m_start_measure_time)
		sum_membrane_potential();

	m_t += m_dt;
}


void IFNetwork::update_input_layer()
{
	double r_number;

	for (int i = 0; i < m_Ninp; i++)
	{
		r_number = gsl_rng_uniform(r);

		if(r_number < m_input[i].r*m_dt)
		{
			m_ispiked[i] = true;
		}
		else
		{
			m_ispiked[i] = false;
		}
	}
}


void IFNetwork::STDP()
{
	for (int i = 0; i < m_Ne; i++)
	{
		bool pre_spiked = m_spiked[i];
		double pre_trace = m_Neuron[i].n.get_trace();

		for(int j = m_Neuron[i].k_out_E; j < m_Neuron[i].k_out_E + m_Neuron[i].k_out_I; j++)
		{
			int pos_index = m_Neuron[i].n_pos[j].index;
			bool pos_spiked = m_spiked[pos_index];
			double w_new = m_Neuron[i].n_pos[j].w;

			if(pre_spiked)
			{
				double pos_trace_minus = m_Neuron[pos_index].n.get_trace_minus();

				w_new += (-m_B*pos_trace_minus + m_delta)*m_dt;
			}

			if(pos_spiked)
			{
				double pre_trace_plus = m_Neuron[i].n.get_trace_plus();
				double pos_trace_slow_prev = m_Neuron[i].n.get_trace_slow_prev();

				w_new += m_B*pre_trace_plus*pos_trace_slow_prev*m_dt;
			}

			if((w_new < 10) && (w_new > 0))
			{
				m_Neuron[i].n_pos[j].w = w_new;
			}
		}
	}
}


void IFNetwork::iSTDP()
{
	for(int i = m_Ne; i < m_N; i++)
	{
		bool pre_spiked = m_spiked[i];
		double pre_trace = m_Neuron[i].n.get_trace();

		for(int j = 0; j < m_Neuron[i].k_out_E; j++)
		{
			int pos_index = m_Neuron[i].n_pos[j].index;
			bool pos_spiked = m_spiked[pos_index];
			double w_new = m_Neuron[i].n_pos[j].w;

			if(pre_spiked)
			{
				double pos_trace = m_Neuron[pos_index].n.get_trace();
				w_new += m_eta*(pos_trace - m_alpha)*m_dt;
			}

			if(pos_spiked)
			{
				w_new += m_eta*pre_trace*m_dt;
			}

			if((w_new < 15) && (w_new > 0))
			{
				m_Neuron[i].n_pos[j].w = w_new;
			}
		}
	}
}


void IFNetwork::iSTDP_global_factor()
{
	update_global_in_factor();

	for(int i = m_Ne; i < m_N; i++)
	{
		bool pre_spiked = m_spiked[i];
		double pre_trace = m_Neuron[i].n.get_trace();

		for(int j = 0; j < m_Neuron[i].k_out_E; j++)
		{
			int pos_index = m_Neuron[i].n_pos[j].index;
			bool pos_spiked = m_spiked[pos_index];
			double w_new = m_Neuron[i].n_pos[j].w;

			if(pre_spiked)
			{
				double pos_trace = m_Neuron[pos_index].n.get_trace();
				w_new += m_eta*(m_H - m_gamma)*(pos_trace + 1)*m_dt;
			}

			if(pos_spiked)
			{
				w_new += m_eta*(m_H - m_gamma)*pre_trace*m_dt;
			}

			if((w_new < 5) && (w_new > 0))
			{
				m_Neuron[i].n_pos[j].w = w_new;
			}
		}
	}
}



void IFNetwork::iSTDP_local_factor()
{
	update_local_in_factor();

	for(int i = m_Ne; i < m_N; i++)
	{
		bool pre_spiked = m_spiked[i];
		double pre_trace = m_Neuron[i].n.get_trace();

		for(int j = 0; j < m_Neuron[i].k_out_E; j++)
		{
			int pos_index = m_Neuron[i].n_pos[j].index;
			bool pos_spiked = m_spiked[pos_index];
			double w_new = m_Neuron[i].n_pos[j].w;

			if(pre_spiked)
			{
				double pos_trace = m_Neuron[pos_index].n.get_trace();
				w_new += m_eta*(m_Neuron[i].L - m_gamma)*(pos_trace + 1)*m_dt;
			}

			if(pos_spiked)
			{
				w_new += m_eta*(m_Neuron[i].L - m_gamma)*pre_trace*m_dt;
			}

			if((w_new < 5) && (w_new > 0))
			{
				m_Neuron[i].n_pos[j].w = w_new;
			}
		}
	}
}


void IFNetwork::update_global_in_factor()
{
	m_H += -m_H*m_dt/m_tau_HL;

	for (int i = 0; i < m_Ne; i++)
	{
		if(m_spiked[i])
		{
			m_H += 1./m_Ne;
		}
	}
}


void IFNetwork::update_local_in_factor()
{
	for(int i = 0; i < m_Ni; i++)
	{
		m_Neuron[m_Ne + i].L = m_Neuron[m_Ne + i].L*m_expHL;

		int S = 0;
		int l = i/32;
		S += (int)m_spiked[2*i + 64*l];
		S += (int)m_spiked[2*i + 64*l + 1];
		S += (int)m_spiked[2*i + 64*(l + 1)];
		S += (int)m_spiked[2*i + 64*(l + 1) + 1];

		m_Neuron[m_Ne + i].L += S/4.;
	}

}


void IFNetwork::check_spikes()
{
	for (int i = 0; i < m_N; i++)
	{
		if(m_spiked[i])
		{
			double STP_x = m_Neuron[i].n.get_STP_x();
			double STP_u = m_Neuron[i].n.get_STP_u();

			if(m_t >= m_start_measure_time)
			{
				m_Neuron[i].n_spikes++;
			}

			for(int j = 0; j < m_Neuron[i].k_out_E + m_Neuron[i].k_out_I; j++)
			{
				bool excitatory = m_Neuron[i].n.is_excitatory();
				int n_pos = m_Neuron[i].n_pos[j].index;
				double w = m_Neuron[i].n_pos[j].w;

				if(excitatory)
					w *= STP_u*STP_x; //STP scalled weight.

				double t = m_t + m_Neuron[i].n_pos[j].delay;

				insert_event(excitatory, t, n_pos, w);
			}
		}
	}
}


void IFNetwork::insert_event(bool excitatory, double t, int n_pos, double w)
{
	Event ev_temp;

	ev_temp.excitatory = excitatory;
	ev_temp.t = t;
	ev_temp.n_pos = n_pos;
	ev_temp.w = w;

	if(m_events.empty())
	{
		m_events.push_back(ev_temp);
	}
	else
	{
		std::list<Event>::iterator it = m_events.end()--;

		while((*it).t >= t && it != m_events.begin())
		{
			it--;
		}

		m_events.insert(it++, ev_temp);

	}
}


void IFNetwork::distribute_input_spikes()
{
	for (int i = 0; i < m_Ninp; i++)
	{
		if(m_ispiked[i])
		{
			for(int j = 0; j < m_input[i].k_out_E + m_input[i].k_out_I; j++)
			{
				int n_pos = m_input[i].n_pos[j].index;
				double w = m_input[i].n_pos[j].w;

				m_Neuron[n_pos].n.inject_ex_current(w);
			}
		}
	}
}


void IFNetwork::distribute_spikes()
{
	std::list<Event>::iterator it = m_events.begin();

	while((m_t >= (*it).t) && (it != m_events.end()))
	{
		bool excitatory = (*it).excitatory;
		double w = (*it).w;
		int n_pos = (*it).n_pos;

		if(excitatory)
		{
			m_Neuron[n_pos].n.inject_ex_current(w);

			m_events.erase(it++);
		}
		else
		{
			m_Neuron[n_pos].n.inject_in_current(w);

			m_events.erase(it++);
		}
	}
}


void IFNetwork::pruning(bool local)
{
	if(local)
	{
		bool pruned = false;
		int i = m_Ne;
		while(!pruned)
		{
			if(!m_Neuron[i].n.is_silenced())
			{
				m_Neuron[i].n.silence_neuron();
				pruned = true;
			}
			else
			{
				i++;
			}
		}
	}
	else
	{
		bool pruned = false;
		while(!pruned)
		{
			int i = (int)(m_Ne + gsl_rng_uniform(r)*m_Ni);
			if(!m_Neuron[i].n.is_silenced())
			{
				m_Neuron[i].n.silence_neuron();
				pruned = true;
			}
		}
	}

	m_Npruned++;
}


void IFNetwork::sum_membrane_potential()
{
	double temp_mean_v = 0;

	for(int i = 0; i < m_Ne; i++)
	{
		double temp_v = m_Neuron[i].n.get_V();

		temp_mean_v += temp_v;
		m_Neuron[i].sum_v += temp_v;
		m_Neuron[i].sum_v2 += temp_v*temp_v;
	}

	temp_mean_v = temp_mean_v/m_Ne;

	m_sum_mean_v += temp_mean_v;
	m_sum_mean_v2 += temp_mean_v*temp_mean_v;
}


void IFNetwork::change_input_rate(double r)
{
	for (int i = 0; i < m_Ninp; i++)
	{
		m_input[i].r = r;
	}
}


void IFNetwork::print_spikes(std::ofstream &file)
{
	for(int i = 0; i < m_N; i++)
	{
		if(m_spiked[i])
		{
			file << m_t << " " << i << " " << m_spiked[i] << std::endl;
		}
	}

}


void IFNetwork::print_potentials(std::ofstream &file)
{
	file << m_t << " " << m_Neuron[0].n.get_V() << " " << m_Neuron[1].n.get_V() << " " << m_Neuron[2].n.get_V() << " " << m_Neuron[m_Ne].n.get_V() << std::endl;
}


void IFNetwork::print_weights(std::ofstream &file)
{
	file << m_t << " " << m_Neuron[0].n_pos[0].w << " " << m_Neuron[m_Ne].n_pos[0].w << " " << \
	m_Neuron[m_Ne].n_pos[1].w << " " << m_Neuron[m_Ne].n_pos[2].w << " " << \
	m_Neuron[m_Ne].n_pos[3].w << " " << m_Neuron[m_Ne].n_pos[4].w << " " << \
	m_Neuron[m_Ne+1].n_pos[0].w << " " << m_Neuron[m_Ne+1].n_pos[2].w << " " << \
	m_Neuron[m_Ne+1].n_pos[2].w << " " << m_Neuron[m_Ne+1].n_pos[3].w << " " << \
	m_Neuron[m_Ne+1].n_pos[4].w << " " << m_Neuron[m_Ne+19].n_pos[0].w << " " << \
	m_Neuron[m_Ne+20].n_pos[0].w << " " << m_Neuron[m_Ne+21].n_pos[0].w << " " << m_Neuron[m_N-1].n_pos[0].w << " " << \
	m_Neuron[m_N-1].n_pos[1].w << " " << m_Neuron[m_N-1].n_pos[2].w << " " << m_Neuron[m_N-1].n_pos[3].w << " " << \
	m_Neuron[m_N-1].n_pos[15].w << " " << m_Neuron[m_N-1].n_pos[30].w << " " << m_Neuron[m_N-1].n_pos[40].w << std::endl;
}


void IFNetwork::print_weightMatrix(std::ofstream &file)
{
	for (int i = 0; i < m_N; i++)
	{
		std::vector<double> auxconnections (m_N);
		std::fill(auxconnections.begin(), auxconnections.end(), 0);

		for (int j = 0; j < (m_Neuron[i].k_out_E + m_Neuron[i].k_out_I); j++)
		{
			int pos_index = m_Neuron[i].n_pos[j].index;
			double pos_w = m_Neuron[i].n_pos[j].w;

			auxconnections[pos_index] = pos_w;
		}

		for (int j = 0; j < m_N; j++)
		{
			file << auxconnections[j] << " ";
		}

		file << std::endl;
	}

	file << std::endl << std::endl;
}


void IFNetwork::print_currents(std::ofstream &file)
{
	file << m_t << " " << m_Neuron[0].n.get_ex_current() << " " << m_Neuron[0].n.get_in_current() << " " << \
	m_Neuron[100].n.get_ex_current() << " " << m_Neuron[100].n.get_in_current() << " " << \
	m_Neuron[200].n.get_ex_current() << " " << m_Neuron[200].n.get_in_current() << " " << \
	m_Neuron[1000].n.get_ex_current() << " " << m_Neuron[1000].n.get_in_current() << " " << \
	m_Neuron[1800].n.get_ex_current() << " " << m_Neuron[1800].n.get_in_current() << " " << \
	m_Neuron[2000].n.get_ex_current() << " " << m_Neuron[2000].n.get_in_current() << " " << \
	m_Neuron[2500].n.get_ex_current() << " " << m_Neuron[2500].n.get_in_current() << " " << \
	m_Neuron[3700].n.get_ex_current() << " " << m_Neuron[3700].n.get_in_current() << " " << \
	m_Neuron[3500].n.get_ex_current() << " " << m_Neuron[3500].n.get_in_current() << " " << \
	m_Neuron[4000].n.get_ex_current() << " " << m_Neuron[4000].n.get_in_current() << " " << \
	m_Neuron[4090].n.get_ex_current() << " " << m_Neuron[4090].n.get_in_current() << " " << \
	m_Neuron[4200].n.get_ex_current() << " " << m_Neuron[4200].n.get_in_current() << " " << \
	m_Neuron[4390].n.get_ex_current() << " " << m_Neuron[4390].n.get_in_current() << " " << \
	m_Neuron[4690].n.get_ex_current() << " " << m_Neuron[4690].n.get_in_current() << " " << \
	m_Neuron[4990].n.get_ex_current() << " " << m_Neuron[4990].n.get_in_current() << " " << \
	m_Neuron[5000].n.get_ex_current() << " " << m_Neuron[5000].n.get_in_current() << std::endl;
}


void IFNetwork::print_parameters()
{
	std::cout << "####### N = " << m_N  << "    Ne = " << m_Ne << "    Ninp = " << m_Ninp << std::endl;
	std::cout << "####### CProb (EE, EI, IE, II) = (" << m_conn_prob_ee << ", " << m_conn_prob_ei << ", " << m_conn_prob_ie << ", " << m_conn_prob_ii << ")" << "    CSProb = " << m_conn_prob_stim << std::endl;
	std::cout << "####### w0_ee = " << m_w0_ee << "    w0_ie = " << m_w0_ie << "    w0_s = " << m_w0_stim << std::endl;
	std::cout << "####### r = " << m_input_rate << std::endl;
	std::cout << "#######" << std::endl;
}


void IFNetwork::print_connections(std::ofstream &file)
{
	for(int i = 0; i < m_N; i++)
	{
		int nConn = m_Neuron[i].k_out_E + m_Neuron[i].k_out_I;
		for (int j = 0; j < nConn; j++)
		{
			file << m_Neuron[i].n_pos[j].index << " ";
		}
		file << std::endl;
	}
}


double IFNetwork::calculate_syncronization()
{
	double T = m_t - m_start_measure_time;
	double sum_var_v = 0;
	double var_mean_v = 0;

	for(int i = 0; i < m_Ne; i++)
	{
		double sum_v = m_Neuron[i].sum_v;
		double sum_v2 = m_Neuron[i].sum_v2;

		sum_var_v += (sum_v2 - (sum_v*sum_v)*m_dt/T)*m_dt/T;
	}

	sum_var_v = sum_var_v/m_Ne;

	var_mean_v = (m_sum_mean_v2 - (m_sum_mean_v*m_sum_mean_v)*m_dt/T)*m_dt/T;

	return sqrt(var_mean_v/sum_var_v);
}


double IFNetwork::calculate_mean_fire_rate()
{
	double sum_fire_rate = 0;
	double measure_time = m_t - m_start_measure_time;

	for(int i = 0; i < m_Ne; i++)
		sum_fire_rate += m_Neuron[i].n_spikes/measure_time;

	return sum_fire_rate/m_Ne;
}


double IFNetwork::calculate_fano_factor()
{
	unsigned long int sum_spikes = 0;
	unsigned long int sum_spikes2 = 0;

	for(int i = 0; i < m_Ne; i++)
	{
		sum_spikes += m_Neuron[i].n_spikes;
		sum_spikes2 += m_Neuron[i].n_spikes*m_Neuron[i].n_spikes;
		//std::cout << m_Neuron[i].n_spikes << " " << std::endl;
	}

	double mean_spikes = (sum_spikes*1.0)/m_Ne;
	double var_spikes = (sum_spikes2*1.0)/m_Ne - (mean_spikes*mean_spikes);
	//std::cout << var_spikes << " " << mean_spikes << std::endl;

	return var_spikes/mean_spikes;
}


void IFNetwork::generate_support_vector(bool excitatory_pre, bool excitatory_pos, std::vector<Support> &v_pre, std::vector<Support> &v_pos)
{
	if(excitatory_pre && excitatory_pos)
	{
		//Excitatory send to excitatory
		for(int i = 0; i < m_Ne; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_out_E;

			v_pre.push_back(element);
		}
		//Excitatory receive from excitatory
		for(int i = 0; i < m_Ne; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_in_E;

			v_pos.push_back(element);
		}
	}
	else if(!excitatory_pre && excitatory_pos)
	{
		//Inhibitory send to excitatory
		for(int i = m_Ne; i < m_N; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_out_E;

			v_pre.push_back(element);
		}
		//Excitatory receive from inhibitory
		for(int i = 0; i < m_Ne; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_in_I;

			v_pos.push_back(element);
		}
	}
	else if(excitatory_pre && !excitatory_pos)
	{
		//Excitatory send to inhibitory
		for(int i = 0; i < m_Ne; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_out_I;

			v_pre.push_back(element);
		}
		//Inhibitory receive from excitatory
		for(int i = m_Ne; i < m_N; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_in_E;

			v_pos.push_back(element);
		}
	}
	else
	{
		//Inhibitory send to inhibitory
		for(int i = m_Ne; i < m_N; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_out_I;

			v_pre.push_back(element);
		}
		//Inhibitory receive from inhibitory
		for(int i = m_Ne; i < m_N; i++)
		{
			Support element;

			element.index = i;
			element.k_available = m_Neuron[i].k_in_I;

			v_pos.push_back(element);
		}
	}
}


void IFNetwork::get_spiked(int n, int &pre_spikes, int &pos_spikes, double &pre_trace, double &pos_trace)
{
	pre_spikes = 0;

	if(m_spiked[n])
		pre_spikes += 1;

	pre_trace = m_Neuron[n].n.get_trace();

	pos_spikes = 0;
	pos_trace = 0;

	for(int i = 0; i < m_Neuron[n].k_out_E; i++)
	{
		int pos_index = m_Neuron[n].n_pos[i].index;

		if(m_spiked[pos_index])
			pos_spikes += 1;

		pos_trace += m_Neuron[pos_index].n.get_trace();
	}
}
