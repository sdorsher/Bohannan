#pragma once
class DFT
{
public:
	DFT(void);
	~DFT(void);
	void fourierTransform(double (*ft) [2], double (*ff) [2], int sizef);
	void inverseFourierTransform(double (*ft) [2], double (*ff) [2], int sizef);
	void phase(double* phases, double (* ff)[2], int sizef);
	void ASD(double* asd, double (*ff)[2], int sizef, double deltaT);
	void getFreqs(double* freqs, double deltaT, int sizef);
	int getMaxASDindex(double* ASD, int sizef);
	double amplitudeSinusoid(double* ASD, int sizef, int index, double deltaT);
	double phaseSinusoid(double* phase, int index);
};

