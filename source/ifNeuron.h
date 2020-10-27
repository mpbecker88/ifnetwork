#ifndef IFNEURON_H
#define IFNEURON_H

class IFNeuron
{
public:
	IFNeuron();
	IFNeuron(bool ex, double IexInit, double alpha, double dt);
	~IFNeuron();

	bool update(double dt);
	void inject_ex_current(double w);
	void inject_in_current(double w);
	void inject_current(double I);
	void activity(bool spiked);
	void silence_neuron();

	double get_V(){return m_S.V;};
	double get_A(){return m_S.A;};
	double get_trace(){return m_S.trace;};
	double get_trace_plus(){return m_S.trace_plus;};
	double get_trace_minus(){return m_S.trace_minus;};
	double get_trace_slow(){return m_S.trace_slow;};
	double get_trace_minus_prev(){return m_S.trace_minus_prev;};
	double get_trace_slow_prev(){return m_S.trace_slow_prev;};
	double get_in_current(){return m_C.g_GABA*(m_P.VIn - m_S.V);};
	double get_ex_current(){return (m_P.alpha*m_C.g_AMPA + (1 - m_P.alpha)*m_C.g_NMDA)*(m_P.VEx - m_S.V);};
	double get_STP_u(){return m_S.STP_u;};
	double get_STP_x(){return m_S.STP_x;};
	double get_threshold(){return m_S.th;};
	bool is_excitatory(){return m_excitatory;};
	bool is_silenced(){return m_S.silenced;};

private:
	/** Is the neuron excitatory? */
	bool m_excitatory;

	/** Exponential factors for calculations. */
	double m_expAMPA;
	double m_expGABA;
	double m_expx;
	double m_expu;
	double m_expTr;
	double m_expThr;

	struct Parameters
	{
	    /** Membrane time constant in ms. */
	    double tau;

	    /** Time constant of AMPA excitatory condutance in ms. */
	    double tauAMPA;

	    /** Time constant of NMDA excitatory condutance in ms. */
	    double tauNMDA;

	    /** Time constant of GABA inhibitory condutance in ms. */
	    double tauGABA;

	    /** Refractory period in ms. */
	    double tauR;

	    /** Time constant of STP x variable in ms. */
	    double taud;

	    /** Time constant of STP u variable in ms. */
	    double tauf;

	    /** Time constant of inhibitory plasticity trace in ms. */
	    double tauSTDP;

	    /** Time constant of activity sensor in ms. */
	    double tauA;

	    /** Time constant of STDP plus trace variable. */
	    double tauPlus;

	    /** Time constant of STDP minus trace variable. */
	    double tauMinus;

	    /** Time constant of STDP slow trace variable. */
	    double tauSlow;

	    /** Time constant for threshold variable. */
	    double tauThr;

	    /** Resting potential in mV. */
	    double Vrest;

	    /** Excitatory reversal potential in mV. */
	    double VEx;

	    /** Inhibitory reversal potential in mV. */
	    double VIn;

	    /** Reset value of the membrane potential, in mV. */
	    double Vreset;

	    /** Threshold resting state in mV. */
	    double thRest;

	    /** After spike threshold in mV. */
	    double thSpike;

	    /** Homeostase activity. */
	    double Agoal;

	    /** External current in pA */
	    double Ie;

	    /** STP u stable point. */
	    double U;

	    /** AMPA/NMDA channels ratio. */
	    double alpha;
	};

	struct State
	{
		/** Membrane potential. */
		double V;

		/** Neuron activity. */
		double A;

		/** Spike threshold. */
		double th;

		/** STP depression state variables. */
		double STP_x;

		/** STP potentiation state variables. */
		double STP_u;

		/** Inhibitory trace variable. */
		double trace;

		/** STDP plus trace. */
		double trace_plus;

		/** STDP minus trace. */
		double trace_minus;

		/** STDP slow trace. */
		double trace_slow;

		/** Auxiliary STDP minus trace (previous time step). */
		double trace_minus_prev;

		/** Auxiliary STDP slow trace (previous time step). */
		double trace_slow_prev;

		/** Refractory steps remaining. */
		int r;

		/** Flag blocking neuron update. */
		bool silenced;

	};

	struct Condutances
	{
		/** AMPA excitatory condutânce in nS. */
		double g_AMPA;

		/** NMDA excitatory condutânce in nS. */
		double g_NMDA;

		/** GABA inhibitory condutânce in nS. */
		double g_GABA;
	};

	Parameters m_P;
	State m_S;
	Condutances m_C;

	void AMPA_cond();
	void NMDA_cond(double dt);
	void GABA_cond();
	void update_threshold();
	void update_trace_variables(bool spiked);
};

#endif
