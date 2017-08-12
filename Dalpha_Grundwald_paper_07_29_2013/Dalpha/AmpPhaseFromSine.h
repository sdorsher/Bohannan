#pragma once
class AmpPhaseFromSine
{
public:
	AmpPhaseFromSine(void);
	~AmpPhaseFromSine(void);
	double getAmplitude(double* data, int dataSize);
	double getPhaseDifference(double* data, double* refData, int dataSize, double deltaT, double frequency);
	int getMaxIndex(double* data, int dataSize);
};

