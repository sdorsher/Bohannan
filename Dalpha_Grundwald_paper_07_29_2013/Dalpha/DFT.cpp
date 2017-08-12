#include "DFT.h"
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

DFT::DFT(void)
{
}


DFT::~DFT(void)
{
}

void DFT::fourierTransform(double (*ft)[2], double (*ff)[2], int sizef)
{
	double pi = 3.14159265;
	//note: not an FFT
	for(int n=0; n<sizef; n++)
	{
		ff[n][0]=0.0;
		ff[n][1]=0.0;
		for(int k=0; k<sizef; k++)
		{
			double arg = 2.0*pi*((double) n)*((double) k)/((double) sizef);
			ff[n][0]+=ft[k][0]*cos(arg)+ft[k][1]*sin(arg);
			ff[n][1]+=ft[k][1]*cos(arg)-ft[k][0]*sin(arg);
		}
	}

}

void DFT::inverseFourierTransform(double (*ft)[2], double (*ff)[2], int sizef)
{// untested
	double pi = 3.14159265;

	for(int n=0; n<sizef; n++)
	{
		ft[n][0]=0.0;
		ft[n][1]=0.0;
		for(int k=0; k<sizef; k++)
		{
			ft[n][0]+= ff[k][0]*cos(2*pi*n*k/sizef) - ff[k][1]*sin(2*pi*n*k/sizef);
			ft[n][1] += ff[k][1] * cos(2*pi*n*k/sizef) + ff[k][0] * sin(2*pi*n*k/sizef);
			ft[n][0] *= 1.0/((double) sizef);
			ft[n][1] *=  1.0/((double)sizef);
		}
	}
}

void DFT::ASD(double* asd, double (*ff)[2], int sizef, double deltaT)
{
	for(int n=0; n<sizef; n++)
	{
		asd[n]=pow(deltaT, 0.5)*sqrt(ff[n][0]*ff[n][0]+ff[n][1]*ff[n][1])/pow(sizef, 0.5);
	}
}

void DFT::phase(double* phases, double (*ff)[2], int sizef)
{
	const double TOL = 0.0001;
	const double pi =3.14159265;
	for(int n=0; n<sizef; n++)
	{
		if((fabs(ff[n][0])<TOL)&&(fabs(ff[n][1])<TOL))
		{
			phases[n]=0.0;
		}
		else
		{
			phases[n] = atan2(ff[n][1],ff[n][0]);
			phases[n] = phases[n] * 2.0/pi;
		}
	}
}

void DFT::getFreqs(double* freqs, double deltaT, int sizef)
{
	double freqSample = 1.0/deltaT;
	double freqMin = freqSample/sizef;
	for(int n = 0; n< sizef; n++) 
	{
		freqs[n]=freqMin*n;
	}
}

int DFT::getMaxASDindex(double* ASD, int sizef)
{
	// only considers valid half of fourier transform plot assuming real ft. 
	double maximum = 0.0;
	int maxIndex =0;
	for (int n = 1; n<sizef/2; n++)
	{
		if(ASD[n]>maximum)
		{
			maximum = ASD[n];
			maxIndex = n;
		}
	}

	return maxIndex;
}

double DFT::amplitudeSinusoid(double* ASD, int sizef, int index, double deltaT)
{//works for sinusoids
	double ftproduct = ASD[index]/pow(deltaT, 0.5)*pow(sizef,0.5);
	double amplitude = ftproduct/((double) sizef)*2.0;
	return amplitude;
}

double DFT::phaseSinusoid(double* phase, int index)
{
	return phase[index];
}
