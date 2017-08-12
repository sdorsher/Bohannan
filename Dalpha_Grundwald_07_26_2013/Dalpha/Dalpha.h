#pragma once
#include "PolyRatioFromContFrac.h"
#include "AvgShiftRegister.h"
#include <string>
#include <vector>
#include "time.h"

using namespace std;

class Dalpha
{
public:
	vector<double> outputData;
	vector<double> outputDtiming;

private:
	int Nmax; // maximum time	
	vector<double> weights;
	int partitionMax;
	double alpha;
	double deltaT;
	vector<int> partition;

	int partitionSize; // same size as avg registers and summed coefficients
	string partitionType;

	AvgShiftRegister historyRegister; //the average shift register storing the input data as history
	int parameterSize;
	vector<double> parameters;

public:
	Dalpha(void);
	~Dalpha(void);
	void setProperties(double alpha0, double deltaT0, int Nmax0);
	void setPartition(int type, int numRegisters);

	//void setFunctionParameters(vector<double>& functionParameters, int fnParameterSize);
	void zeroHistory(void);
	void differintegrate(vector<double>& inputData);
	//void differintegrateTiming(vector<double>& inputD);
	void print(string filename, vector<double>& inputData);
	void computeWeights(void);
	double getWeight(int N);
	int getBinMax(int N);
	int getNumPerBin(int N);

	//double inputFunction(int N);
	void updateHistory(double inputDatum);
	double differintegrateOnce(void);
	double partSumOnce(void);
	void setPresentValue(double y);
};

