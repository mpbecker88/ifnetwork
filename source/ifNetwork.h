#ifndef IFNETWORK_H
#define IFNETWORK_H

#include "ifNeuron.h"
#include <fstream>
#include <string>
#include <vector>
#include <list>

class IFNetwork
{
public:
	IFNetwork(bool k_homogeneous, bool pruning, bool local_pruning, double g_ei, double g_ie, std::string sim_number, int seed);
	~IFNetwork();
	void update();
	void change_input_rate(double r);
	void print_connections(std::ofstream &file);
	void print_spikes(std::ofstream &file);
	void print_potentials(std::ofstream &file);
	void print_weights(std::ofstream &file);
	void print_weightMatrix(std::ofstream &file);
	void print_currents(std::ofstream &file);
	void set_time_step(double dt){m_dt = dt;};
	double calculate_syncronization();
	double calculate_mean_fire_rate();
	double calculate_fano_factor();
	void get_spiked(int n, int &pre_spikes, int &pos_spikes, double &pre_trace, double &pos_trace);


private:
	bool m_pruning; // Pruning protocol is active?
	bool m_local_pruning; // Pruning is local?
	int m_N; // Network size.
	int m_Ne; // Number of excitatory neurons.
	int m_Ni; // Number of inhibitory neurons.
	int m_Ninp; // Number of neurons in the input layer.
	int m_Npruned; // Number of pruned inhibitory neurons.
	int m_sum_out_EE; // Out degree sum from excitatory-excitatory connections.
	int m_sum_out_EI; // Out degree sum from excitatory-inhibitory connections.
	int m_sum_in_EE; // In degree sum from excitatory-excitatory connections.
	int m_sum_in_EI; // In degree sum from excitatory-inhibitory connections.
	int m_sum_out_IE; // Out degree sum from inhibitory-excitatory connections.
	int m_sum_out_II; // Out degree sum from inhibitory-inhibitory connections.
	int m_sum_in_IE; // In degree sum from inhibitory-excitatory connections.
	int m_sum_in_II; // In degree sum from inhibitory-inhibitory connections.
	double m_t; // Time variable.
	double m_dt; // Euler time step (default dt=0.1).
	double m_conn_prob_ee; // Probability of any connection EE.
	double m_conn_prob_ei; // Probability of any connection EI.
	double m_conn_prob_ie; // Probability of any connection IE.
	double m_conn_prob_ii; // Probability of any connection II.
	double m_conn_prob_stim; // Probability of connection from the input layer.
	double m_w0_ee; // Initial weight for EE connection.
	double m_w0_ei; // Initial weight for EI connection.
	double m_w0_ie; // Initial weight for IE connection.
	double m_w0_ii; // Initial weight for II connection.
	double m_w0_stim; // Initial weight for the input layer connection.
	double m_sum_mean_v; // Sum of the average membrane potential.
	double m_sum_mean_v2; // Sum of the squared average membrane potential.
	double m_start_measure_time; // Starting time for syncronization mesures.
	double m_input_rate; //Rate o firing of the input layer.
	double m_input_amplitude; // Amplitude of input rate oscilation (0 for constant).
	double m_input_frequency; // Frequency of input rate oscilation.
	double m_alpha_E;  // AMPA/NMDA channel ratio for excitatory targets.
	double m_alpha_I;  // AMPA/NMDA channel ratio for inhibitory targets.
	bool m_is_alpha_E_random; // Is the AMPA/NMDA channel ratio for excitatory targets random?
	bool m_is_alpha_I_random; // Is the AMPA/NMDA channel ratio for inhibitory targets random?
	double m_eta; //Learning rate for inhibitory plasticity.
	double m_alpha; //Depression factor for inhibitory plasticity.
	double m_B; //STDP doublet and triplet learning rate.
	double m_delta; //STDP transmitter-induced learning rate.
	double m_sigma_ee_conn; //Gaussian variance for ee connections.
	double m_sigma_ei_conn; //Gaussian variance for ei connections.
	double m_sigma_ie_conn; //Gaussian variance for ie connections.
	double m_sigma_ii_conn; //Gaussian variance for ii connections.
	double m_H; //Global inhibitory factor.
	double m_tau_HL; //Time constant for H and L.
	double m_gamma; //Target population rate.
	double m_expHL; //Exponential factor for H and L.

	/** Pos-sinaptic connection struct. */
	struct Connection
	{
		/** Index of the neuron. */
		int index;

		/** Weight of the connection. */
		double w;

		/** Sinaptic delay. */
		double delay;
	};

	/** Network node. */
	struct Node
	{
		/** Neuron class. */
		IFNeuron n;

		/** Network connections. */
		std::vector<Connection> n_pos;

		/** Excitatory out-degree of the node. */
		int k_out_E;

		/** Inhibitory out-degree of the node. */
		int k_out_I;
	
		/** Excitatory in-degree of the node. */
		int k_in_E;

		/** Inhibitory in-degree of the node. */
		int k_in_I;

		/** Local inhibitory factor. */
		double L;

		/** Sum of the membrane potential in the measure time. */
		double sum_v;

		/** Sum of the squared membrane potential in the measure time. */
		double sum_v2;

		/** Sum of spikes in the measure time. */
		int n_spikes;
	};

	/** Poisson node. */
	struct Poisson
	{
		/** Firing rate. */
		double r;

		/** Network connections. */
		std::vector<Connection> n_pos;

		/** Excitatory out-degree of the node. */
		int k_out_E;

		/** Inhibitory out-degree of the node. */
		int k_out_I;
	};

	/** Support structure used to generate network  connections. */ 
	struct Support
	{
		/** Neuron index in the network vector. */
		int index;

		/** Number of available connections to receive. */
		int k_available;
	};

	/** Event structure. */
	struct Event
	{
		/** Is the neuron excitatory? */
		bool excitatory;

		/** Event time. */
		double t;

		/** Pos-synaptic neuron. */
		int n_pos;

		/** Event weight. */
		double w;
	};

	std::vector<Node> m_Neuron; // Vector containing the neurons.
	std::vector<Poisson> m_input; // Vector containing the poisson input neurons.
	std::vector<bool> m_spiked; // Vector informing if neuron spiked at the time step.
	std::vector<bool> m_ispiked; // Vector informing if input layer neuron spiked at the time step.
	std::list<Event> m_events; // List containing spike events.


	void init_ex_neurons(bool k_homogeneous);
	void init_in_neurons(bool k_homogeneous);
	void init_spiked_vector();
	void init_input_layer();
	void adjust_connections();
	void adjust_EE();
	void adjust_EI();
	void adjust_IE();
	void adjust_II();
	void generate_connections();
	void generate_input_connections();
	void update_input_layer();
	void STDP();
	void iSTDP();
	void iSTDP_global_factor();
	void iSTDP_local_factor();
	void update_global_in_factor();
	void update_local_in_factor();
	void check_spikes();
	void insert_event(bool excitatory, double t, int n_pos, double w);
	void distribute_input_spikes();
	void distribute_spikes();
	void pruning(bool local);
	void sum_membrane_potential();
	void print_parameters();
	bool is_in_vector(int n, const std::vector<Connection> &v);
	void generate_support_vector(bool excitatory_pre, bool excitatory_pos, std::vector<Support> &v_pre, std::vector<Support> &v_pos);

};



#endif