#include "FullGrundwald.h"
#include <fstream>


FullGrundwald::FullGrundwald(void)
{
}


FullGrundwald::~FullGrundwald(void)
{
}

void FullGrundwald::setProperties(double alpha0, double deltaT0, int Nmax0)
{
	alpha = alpha0;
	deltaT = deltaT0;
	Nmax = Nmax0;
	outputData.resize(Nmax);
}

void FullGrundwald::zeroHistory(void)
	// does not zero the weights
{
	for(int n = 0; n<Nmax; n++)
	{
		outputData[n] = 0.0;
	}
	historyRegister.SetSize(Nmax);
}

void FullGrundwald::computeWeights(void)
{
	weights.resize(Nmax);
	for(int i = 1; i< Nmax; i++)
	{
		weights[i] = (i-alpha-1.0)/((double) i);
	}
	weights[0] = 1.0;
}

void FullGrundwald::differintegrate(vector<double>& inputData)
{	
	for(int k = 0; k<Nmax; k++)
	{
		outputData[k] =0.0;
		historyRegister.Shift();
		historyRegister[0] = inputData[k];
		for(int i = Nmax-1; i>=0; i--)
		{
			outputData[k] += historyRegister[i];
			outputData[k] *= weights[i];
		}
		outputData[k]*=pow(deltaT,-alpha);
	}
}

void FullGrundwald::print(string filename, vector<double>& inputData)
{
	ofstream fs;
	fs.open(filename);
	for(int N=0; N< Nmax; N++) 
	{
		fs << N*deltaT << "\t" << inputData[N] << "\t" << outputData[N] << endl;
	}
	fs.close();
}