//Utility routines adapted from Numerical Recipes

#ifndef _NR_UTILS_H_
#define  _NR_UTILS_H_

#include <complex>
#include <vector>

using namespace std;

#define UINT unsigned int

#define PI 3.141592653589793
#define TwoPI 6.28318530717959

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

static double dminarg1, dminarg2;
#define DMIN(a,b) (dminarg1=(a),dminarg2=(b),(dminarg1)<(dminarg2) ? (dminarg1) : (dminarg2))

static double dmaxarg1, dmaxarg2;
#define DMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1)>(dmaxarg2) ? (dmaxarg1) : (dmaxarg2))

static int iminarg1, iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1)<(iminarg2) ? (iminarg1) : (iminarg2))

static int imaxarg1, imaxarg2;
#define IMAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1)>(imaxarg2) ? (imaxarg1) : (imaxarg2))

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

int plot_scale(double d_small, double d_large, double& p_lower, double& p_upper, double& increment);

void cfft(vector<complex<double>>& Data, UINT nn, int isign);

void SineFit(double freq, double timestep, UINT delay_ndx, UINT capture_ndx, std::vector<double>& y, double* a);

double H(double x);

double FC(double vc, double alpha, double beta, double vt);
#endif
