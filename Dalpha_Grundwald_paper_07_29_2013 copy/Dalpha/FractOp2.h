#pragma once
#include "Fractance2.h"

class FractOp2 
{
public:
	FractOp2();
	~FractOp2();
	double FractDeriv(UINT n, Fractance2 &F);
	double PartSum(UINT n, Fractance2 &F);
	
protected:
	double multiplier;
	double order;
	double tau;
	void RemoveAll();
	void SetSize(UINT size);
	static double cof[6];
};