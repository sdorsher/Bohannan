// Noise.cpp: implementation of the CNoise class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Nrutil.h"
#include "Noise.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoise::CNoise()
{
//Start the random number generator
	idum = (-1)*(int)(((unsigned) time(NULL))& 0xFFFFFF);
	ran2();

// Initialize alpha stable parameters.
	alpha = 2.0;	//place holder for generalized non-gaussian 
					// statistics
	one_over_alpha = 1.0/alpha;
	one_minus_alpha = 1.0 - alpha;
	one_minus_alpha_over_alpha = (1.0 - alpha)/alpha;
}

CNoise::~CNoise()
{

}

//Allow user to define new alpha parameter
void CNoise::SetAlpha(double Alpha)
{
	alpha = Alpha;
	one_over_alpha = 1.0/alpha;
	one_minus_alpha = 1.0 - alpha;
	one_minus_alpha_over_alpha = (1.0 - alpha)/alpha;
}

//Random Number Generators
//ran1()

//ran2()
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-12
#define RNMX (1.0-EPS)

double CNoise::ran2()
{
	int j;
	long k;
	static int idum2=123456789;
	static int iy=0;
	static int iv[NTAB];
	double temp;

	if (idum <= 0) {
		if (-idum < 1) idum=1;
		else idum = (-idum);
		idum2=idum;
		for (j=NTAB+7;j>=0;j--) {
			k=idum/IQ1;
			idum=IA1*(idum-k*IQ1)-k*IR1;
			if (idum < 0) idum += IM1;
			if (j < NTAB) iv[j] = idum;
		}
		iy=iv[0];
	}
	k=idum/IQ1;
	idum=IA1*(idum-k*IQ1)-k*IR1;
	if (idum < 0) idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

double CNoise::gasdev()
{
	static int iset=0;
	static double gset;
	double fac,rsq,v1,v2;

	if  (iset == 0) {
		do {
			v1=2.0*ran2()-1.0;
			v2=2.0*ran2()-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	} else {
		iset=0;
		return gset;
	}
}

double CNoise::cauchy()
{
	double x = PI*(ran2()-0.5);
	return DMAX(-NMax,DMIN(tan(x),NMax));
}

double CNoise::aStable()
{
	double w = ran2();
	double x = PI*(ran2()-0.5);
	double y = (sin(alpha*x)/pow(cos(x),one_over_alpha))*
		pow(-cos(one_minus_alpha*x)/log(w),one_minus_alpha_over_alpha);
	return DMAX(-NMax,DMIN(y,NMax));
}