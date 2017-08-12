#include "AmpPhaseFromSine.h"
#include <string>
using namespace std;


AmpPhaseFromSine::AmpPhaseFromSine(void)
{
}


AmpPhaseFromSine::~AmpPhaseFromSine(void)
{
}

double AmpPhaseFromSine::getAmplitude(double* data, int dataSize)
{
	int maxIndex = getMaxIndex(data, dataSize);
	return data[maxIndex];
}

double AmpPhaseFromSine::getPhaseDifference(double* data, double* refData, int dataSize, double deltaT, double frequency)
{
	const double pi = 3.14159265;
	int maxIndexData = getMaxIndex(data, dataSize);
	int maxIndexRefData = getMaxIndex(refData, dataSize); // reference data
	double period = 1.0/frequency;
	double phase = ((double) (maxIndexData - maxIndexRefData))/period*deltaT*2.0*pi; // fraction of period in offset
	while(phase<0)
	{
		phase=phase+2.0*pi;
	}
	while(phase>2.0*pi)
	{
		phase = phase - 2.0*pi;
	}
	return phase;
}


int AmpPhaseFromSine::getMaxIndex(double* data, int dataSize)
{// tested on 9/11/12. Works for amplitude one sinusoid with frequency one and phase 0 or pi.

	// start from maximum range of data to ensure there are no transients
	bool endOfDataSet= false;
	bool firstMaxFound =false;
	bool firstDownwardSlope; //does the data start on a downward slope?
	
	if(data[dataSize-1]>data[dataSize-2]) 
	{
		firstDownwardSlope = true;
	}
	else
	{
		firstDownwardSlope = false;
	}
	int n = dataSize-1;
	int localMax = 0;
	while((!endOfDataSet)&&(!firstMaxFound))
	{
		if ((!firstDownwardSlope)&&(data[n]>data[n-1]))
		{
			// then n is the first maximum at the end of the data series
			localMax = n;
			firstMaxFound = true;
		}
		if ((firstDownwardSlope)&&(data[n]<data[n-1]))
		{
			// then n is the first minimum at the end of the data series and we are now on 
			// an upward slope
			firstDownwardSlope = false;
		}
		if(n>=1)
		{
			n--;
		}
		else
		{
			endOfDataSet = true;
		}
	}
	return localMax;
}