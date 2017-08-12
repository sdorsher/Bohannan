#ifndef _IIR_CFEdifferintegrator_h_
#define _IIR_CFEdifferintegrator_h_

#include "ShiftRegister2.h"
#include <vector>
#include "PolyRatioFromContFrac.h"

using namespace std;

class IIR_CFEdifferintegrator
{
private:
	double I_alpha;
	double deltaT;
//	double CoeffA[10];
//	double CoeffB[10];
	PolyRatioFromContFrac PQ;
	int CFE_SIZE; // number of coefficients in the numerator OR denominator. half the number of registers. 
	
public:
	vector<double> outputArray;

public:
	IIR_CFEdifferintegrator(double alpha, int numCoeffinNumerator, double deltaT0);
	~IIR_CFEdifferintegrator(void);
	void getWeights(void);
	void printWeights(void);
	void differintegrate(vector<double>& inputArray, int inputArraySize);
	//void rescale(double deltaT, double tSample);
};

#endif
