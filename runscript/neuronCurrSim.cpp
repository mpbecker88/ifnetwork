#include <iostream>
#include <fstream>
#include <string>
#include "../source/ifNeuron.h"

double dt = 0.1;

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		std::cout << "No current file name given." << std::endl;
		return 1;
	}

	double alpha = 0.2;
	IFNeuron ifn(true, 0, alpha, dt);
	bool spiked;

	std::ifstream currFile;
	currFile.open(argv[1]);
	if(currFile.fail())
	{
		std::cout << "Error reading file." << std::endl;
		return 1;
	}

	double currSum = 0;
	for(int i = 0; i < 100000; i++)
	{
		double eCurr;
		double iCurr;
		double trash;

		int j = 0;
		int cindex = std::stoi(argv[2]);

		for(; j < 2*cindex + 1; j++)
		{
			currFile >> trash;
		}

		currFile >> eCurr;
		currFile >> iCurr;

		j += 2;

		for(; j < 33; j++)
			currFile >> trash;

		//std::cout << eCurr << " " << iCurr << std::endl;

		currSum += eCurr + iCurr;
		ifn.inject_current(eCurr + iCurr);

		/*else if(i==250)
		{
			ifn.inject_ex_current(0.);
			spiked = true;
		}
		else if(i==600)
		{
			ifn.inject_in_current(0.);
			spiked = true;
		}
		else
			spiked = false;
		*/
		spiked = ifn.update(dt);

		if(i == 0)
			std::cout << i*dt << ' ' << ifn.get_V() << ' ' << currSum  << ' ' << eCurr + iCurr << ' ' << spiked << std::endl;
		else
			std::cout << i*dt << ' ' << ifn.get_V() << ' ' << currSum/i  << ' ' << eCurr + iCurr << ' ' << spiked << std::endl;
		//std::cout << i*dt << ' ' << ifn.get_V() << ' ' << ifn.get_ex_current() << ' ' << ifn.get_in_current() << " " << ifn.get_STP_x() << " " << ifn.get_STP_u() << " " << ifn.get_threshold() << std::endl;
	}
}
