#ifndef _CFEDiffereintegrator_h_
#define _CFEDiffereintegrator_h_

#include "ShiftRegister2.h"
#include <vector>

using namespace std;

class CFEDiffereintegrator
{
private:
	double I_alpha;
	double CoeffA[10];
	double CoeffB[10];
	
public:
	vector<double> outputArray;

public:
	CFEDiffereintegrator(double alpha);
	~CFEDiffereintegrator(void);
	void getWeights(void);
	void printWeights(void);
	void differintegrate(vector<double>& inputArray, int inputArraySize);
};

#endif
