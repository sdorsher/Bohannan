#include "Fractance2.h"

/////////////////////////////////////////////////////////////////////////////
// Fractance
// Implements signal history arrays
// These allow application of Fractional Order Operators (FractOp)
//  to give back current values of the derivatives.

Fractance2::Fractance2()
{
	alpha = -0.5;
	multiplier = 1.0;
}

Fractance2::Fractance2(UINT size)
{
	history.resize(size);
	alpha = -0.5;
	multiplier = 1.0;
}

Fractance2::~Fractance2()
{
	history.clear();
}

double& Fractance2::operator [] (int index)
{
	return history[index];
}

void Fractance2::RemoveAll()
{
	history.clear();
}

void Fractance2::SetSize(unsigned int size)
{
	history.resize(size);
}

double Fractance2::FractDeriv(UINT n)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-alpha-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<=n;k++) 
		PS = history[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

double Fractance2::PartSum(UINT n)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-alpha-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<n;k++) 
		PS = history[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

double Fractance2::Alpha()
{
	return alpha;
}

double Fractance2::Tau()
{
	return tau;
}