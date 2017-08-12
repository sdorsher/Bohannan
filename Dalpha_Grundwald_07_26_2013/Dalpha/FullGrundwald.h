#include "AvgShiftRegister.h"
#include "ShiftRegister2.h"

#include <string>
#include <vector>

using namespace std;

#pragma once
class FullGrundwald
{
public:
	FullGrundwald(void);
	~FullGrundwald(void);

protected:
	int Nmax; // maximum time
	vector<double> outputData;

	vector<double> weights;

	double alpha;
	double deltaT;

	ShiftRegister2 historyRegister; //the average shift register storing the input data as history

	int parameterSize;
	vector<double> parameters;

public:
	void setProperties(double alpha0, double deltaT0, int Nmax0);

	void setFunctionParameters(vector<double>& functionParameters, int fnParameterSize);
	void zeroHistory(void);
	void differintegrate(vector<double>& inputData);
	void print(string filename, vector<double>& inputData);
	void computeWeights(void);

protected:
	double inputFunction(int N);

};
