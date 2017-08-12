#pragma once

#include "AvgShiftRegister.h"

#include <string>
#include <vector>

using namespace std;

class Grundwald
{
public:
	Grundwald(void);
	~Grundwald(void);

protected:
	int Nmax; // maximum time
	vector<double> outputData;

	vector<double> weightsThis; //gamma. multiply the factor due to this bin.
	vector<double> weightsPrev; // beta. multiply the factor due to all previous bins combined
	int partitionMax;
	double alpha; //diffintegral index
	double deltaT; //time step
	vector<int> partition;
	int partitionSize; // same size as avg registers and summed coefficients
	string partitionType;

	AvgShiftRegister historyRegister; //the average shift register storing the input data as history
	int parameterSize;
	vector<double> parameters;

public:
	void setProperties(double alpha0, double deltaT0, int Nmax0);
	void setPartition(int type, int numRegisters);
	void setFunctionParameters(vector<double>& functionParameters, int fnParameterSize);
	void zeroHistory(void);
	void differintegrate(vector<double>& inputData);
	void print(string filename, vector<double>& inputData);
	void computeWeights(void);

private:
	double inputFunction(int N);
};
	

