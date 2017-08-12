#include "FractOp2.h"


/////////////////////////////////////////////////////////////////////////////
// FractOp
// Implements the Fractional Order Operator
// Used to designate the order of the operator
//  and the multiplication factor used in 
//  FOC state space matrices

FractOp2::FractOp2()
{
	//multiplier = 1.0;
	//order = 0.0;
}

FractOp2::~FractOp2()
{
}


double FractOp2::PartSum(UINT n, Fractance2 &F)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-F.Alpha()-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<n;k++) PS = F[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

/*
double FractOp2::LnGamma(double xx)
{
	UINT j;
	double x, y, tmp, ser;
	y = xx;
	x = xx;
	tmp = x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser = 1.000000000190015;
	for(j=0;j<5;j++) ser += cof[j]/(++y);
	return -tmp+log(2.5066282746310005*ser/x);
}
*/

double FractOp2::FractDeriv(UINT n, Fractance2 &F)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-F.Alpha()-1;
	denominator = n;
	PS = F[0];
	for(UINT k=0;k<n;k++) PS = F[k+1] + PS*(numerator-k)/(denominator-k);
	return PS;
}

