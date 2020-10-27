#include <iostream>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


gsl_rng *r;

int main(int argc, char const *argv[])
{
	gsl_rng_env_setup();
	r = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, 687246985);

	std::cout << gsl_ran_gaussian_pdf(50,10) << std::endl;
}