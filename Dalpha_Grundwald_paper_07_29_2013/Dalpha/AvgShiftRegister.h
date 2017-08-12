#pragma once

#include <vector>

using namespace std;

class AvgShiftRegister
{
protected:	
	vector<double> data;
	vector<int> partition;
    vector<double> midpoints;
	vector<int> dataCount;
	
	unsigned int filling; //indicates which bin is currently filling. Set to greater than dataSize if all bins are filled.

public:
	AvgShiftRegister(void);
	~AvgShiftRegister(void);

	int dataSize();
	double dataAt(int n);
	double dataCountAt(int n);
	void setDataAt(int n, double value);

	double midpointAt(int n);
	void getMidpoints(void);

	void setPartition(vector<int>& newPartition, int newPartitionSize);
	void AvgShift(double);
	void zeroHistory(void);
};

