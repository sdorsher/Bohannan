#pragma once

#include <vector>
#include "Nrutil.h"

using namespace std;

class Fractance2
{
public:
	Fractance2();
	~Fractance2();
	Fractance2(UINT size);
	
	double& operator [](int index);
	void RemoveAll();
	void SetSize(UINT size);
	double FractDeriv(UINT n);
	double PartSum(UINT n);
	
	double Alpha();
	double Tau();

protected:
	vector<double> history;
	double alpha;
	double tau;
	double multiplier;
};