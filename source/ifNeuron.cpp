#include "ifNeuron.h"
#include <iostream>
#include <cmath>


IFNeuron::IFNeuron()
{
	double dt = 0.1;
	m_excitatory = true;

	// initialize parameters
	m_P.tau = 20.;
	m_P.tauAMPA = 5.;
	m_P.tauNMDA = 100.;
	m_P.tauGABA = 10.;
	m_P.tauA = 100000.;
	m_P.tauR = 1;
	m_P.taud = 200.;
	m_P.tauf = 600.;
	m_P.tauSTDP = 20.;
	m_P.tauSTDP = 20.;
	m_P.tauPlus = 20.;
	m_P.tauMinus = 20.;
	m_P.tauSlow = 100.;
	m_P.tauThr = 2.;

	m_P.Vrest = -60.;
	m_P.VEx = 0.;
	m_P.VIn = -80.;
	m_P.Vreset = -60.;
	m_P.thRest = -50.;
	m_P.thSpike = 50.;

	m_P.Agoal = 1.;
	m_P.Ie = 0;
	m_P.U = 0.2;

	if(m_excitatory)
	{
		m_P.alpha = 0.2;
	}
	else
	{
		m_P.alpha = 0.3;
	}

	// initialize state
	m_S.V = m_P.Vrest;
	m_S.A = 0.;
	m_S.th = -50.;
	m_S.STP_x = 1.;
	m_S.STP_u = m_P.U;
	m_S.trace = 0;
	m_S.trace_plus = 0;
	m_S.trace_minus = 0;
	m_S.trace_slow = 0;
	m_S.trace_minus_prev = 0;
	m_S.trace_slow_prev = 0;
	m_S.r = 0;
	m_S.silenced = false;

	//initialize condutances
	m_C.g_AMPA = 0.;
	m_C.g_NMDA = 0.;
	m_C.g_GABA = 0.;

	//Initialize exponential factors
	m_expAMPA = exp(-dt/m_P.tauAMPA);
	m_expGABA = exp(-dt/m_P.tauGABA);
	m_expx = exp(-dt/m_P.taud);
	m_expu = exp(-dt/m_P.tauf);
	m_expTr = exp(-dt/m_P.tauSTDP);
	m_expThr = exp(-dt/m_P.tauThr);
}

IFNeuron::IFNeuron(bool ex, double IexInit, double alpha, double dt)
{
	m_excitatory = ex;

	// initialize parameters
	m_P.tau = 20.;
	m_P.tauAMPA = 5.;
	m_P.tauNMDA = 100.;
	m_P.tauGABA = 10.;
	m_P.tauA = 100000.;
	m_P.tauR = 1;
	m_P.taud = 200.;
	m_P.tauf = 600.;
	m_P.tauSTDP = 20.;
	m_P.tauSTDP = 20.;
	m_P.tauPlus = 20.;
	m_P.tauMinus = 20.;
	m_P.tauSlow = 100.;
	m_P.tauThr = 2;

	m_P.Vrest = -60.;
	m_P.VEx = 0.;
	m_P.VIn = -80.;
	m_P.Vreset = -60;
	m_P.thRest = -50.;
	m_P.thSpike = 50.;

	m_P.Agoal = 1.;
	m_P.Ie = IexInit;
	m_P.U = 0.2;
	m_P.alpha = alpha;

	// initialize state
	m_S.V = m_P.Vrest;
	m_S.A = 0.;
	m_S.th = -50.;
	m_S.STP_x = 1.;
	m_S.STP_u = m_P.U;
	m_S.trace = 0;
	m_S.trace_minus = 0;
	m_S.trace_slow = 0;
	m_S.trace_minus_prev = 0;
	m_S.trace_slow_prev = 0;
	m_S.r = 0;
	m_S.silenced = false;

	//initialize condutances
	m_C.g_AMPA = 0.;
	m_C.g_NMDA = 0.;
	m_C.g_GABA = 0.;

	//Initialize exponential factors
	m_expAMPA = exp(-dt/m_P.tauAMPA);
	m_expGABA = exp(-dt/m_P.tauGABA);
	m_expx = exp(-dt/m_P.taud);
	m_expu = exp(-dt/m_P.tauf);
	m_expTr = exp(-dt/m_P.tauSTDP);
	m_expThr = exp(-dt/m_P.tauThr);
}

IFNeuron::~IFNeuron(){}

bool IFNeuron::update(double dt)
{
	bool spiked = false;

	if(m_S.silenced)	//Neuron is silenced, don't update.
	{
		return spiked;
	}

	if(m_S.r <= 0)
	{
		// neuron not refractory
		m_S.V += ((m_P.Vrest - m_S.V) + (m_P.alpha*m_C.g_AMPA + (1 - m_P.alpha)*m_C.g_NMDA)*(m_P.VEx - m_S.V) + m_C.g_GABA*(m_P.VIn - m_S.V) + m_P.Ie)*dt/m_P.tau;

		if(m_S.V > m_S.th)
		{
			//spiked
			m_S.r = (int)ceil(m_P.tauR/dt);
			m_S.th = m_P.thSpike;
			m_S.V = m_P.Vreset;
			spiked = true;
		}
	}
	else
	{
		m_S.r--;
		//m_S.V = m_P.Vreset;
	}

	NMDA_cond(dt);
	AMPA_cond();
	GABA_cond();
	update_threshold();
	update_trace_variables(spiked);
	activity(spiked);

	return spiked;
}

void IFNeuron::AMPA_cond()
{
	m_C.g_AMPA = m_C.g_AMPA*m_expAMPA;
}

void IFNeuron::NMDA_cond(double dt)
{
	m_C.g_NMDA += (-m_C.g_NMDA + m_C.g_AMPA)*dt/m_P.tauNMDA;
}

void IFNeuron::GABA_cond()
{
	m_C.g_GABA = m_C.g_GABA*m_expGABA;
}

void IFNeuron::update_threshold()
{
	m_S.th = m_P.thRest + (m_S.th - m_P.thRest)*m_expThr;
}

void IFNeuron::update_trace_variables(bool spiked)
{
	double xtemp = m_S.STP_x;
	double utemp = m_S.STP_u;
	double tracetemp = m_S.trace;

	/*
	m_S.trace_minus_prev = m_S.trace_minus;
	m_S.trace_slow_prev = m_S.trace_slow;
	*/

	m_S.STP_x = 1 - (1 - xtemp)*m_expx;
	m_S.STP_u = m_P.U - (m_P.U - utemp)*m_expu;
	m_S.trace = m_S.trace*m_expTr;
	/*
	m_S.trace_plus += -m_S.trace_plus*dt/m_P.tauPlus;
	m_S.trace_minus += -m_S.trace_minus*dt/m_P.tauMinus;
	m_S.trace_slow += -m_S.trace_slow*dt/m_P.tauSlow;
	*/
	if(spiked)
	{
		m_S.STP_x += -utemp*xtemp;
		m_S.STP_u += m_P.U*(1 - utemp);
		m_S.trace += 1;
		/*
		m_S.trace_plus += 1;
		m_S.trace_minus += 1;
		m_S.trace_slow += 1;
		*/
	}
}

void IFNeuron::inject_ex_current(double w)
{
	if(m_S.r <= 0)
		m_C.g_AMPA += w;
}

void IFNeuron::inject_in_current(double w)
{
	if(m_S.r <= 0)
		m_C.g_GABA += w;
}

void IFNeuron::inject_current(double I)
{
	m_P.Ie = I; 
}

void IFNeuron::silence_neuron()
{
	m_S.silenced = true;
}

void IFNeuron::activity(bool spiked)
{
	if(spiked)
		m_S.A += -m_S.A/m_P.tauA + 1;
	else
		m_S.A += -m_S.A/m_P.tauA;
}
