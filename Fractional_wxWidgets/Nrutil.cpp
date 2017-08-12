/////////////////////////////////////////////////////////////////////////////
//Numerical Recipes routines
#include "stdafx.h"
#include <math.h>
#include "nrutil.h"

void SineFit(double& freq, double& timestep, UINT& delay_ndx, 
		UINT& capture_ndx, CArray<double, double&>& y, double* a)
{
	// timestep = 1/frequency
	// N = number of points to consider in making estimation
	// y = array of points to be analyzed (offset pointer)
	// to = time delay to start of sampling
	// a = two element array = sine and cosine amplitude
	// Algorithm taken from Numerical Recipes in C.
	static double period, dd, ee, th, ct, st; 
	static double aa11, aa12, aa21, aa22;
	static double bb1, bb2;
	static double old_ct;
	static double xx,yy,zz,uu;
	period = 1.0/freq;
	dd = 2.0*DSQR(sin(PI*freq*timestep));
	ee = sin(TwoPI*freq*timestep);
	th = TwoPI*freq*delay_ndx*timestep;
	ct = cos(th);
	st = sin(th);
	aa11 = aa12 = aa21 = aa22 = 0.0;
	bb1 = bb2 = 0.0;

	for(UINT m = delay_ndx; m<(delay_ndx+capture_ndx); m++)
	{
		aa11 += ct*ct;
		aa12 += ct*st;
		aa22 += st*st;
		bb1 += ct* y[m];
		bb2 += st* y[m];
		old_ct = ct;
		ct = ct - (dd*ct + ee*st);
		st = st - (dd*st - ee*old_ct);
	}
	xx = 1.0/(aa11 - aa12*aa12/aa22);
	uu = 1.0/(aa22 - aa12*aa12/aa11);
	yy = (-aa12*uu/aa11);
	zz = (-aa12*xx/aa22);
	a[1] = xx*bb1 + yy*bb2;		// sin part
	a[0] = zz*bb1 + uu*bb2;		// cos part
	return;
}

void cfft(CArray<COMPLEX, COMPLEX&>& data, UINT nn, int isign) {
	
	UINT i,j,m,n;
	UINT mmax,istep;
	COMPLEX temp, w, wp;
	double theta, wtemp;
	n = nn;

//first shuffle the data set by bit reversal
	j = 0;
	for(i=0;i<n;i++) {
		if(j>i) {
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
		m=n>>1;
		while(m>=1 && j>=m) {
			j-=m;
			m>>=1;
		}
		j+=m;
	}

//This is the Danielson-Lanczos section of the routine
	mmax = 1;
	while (n>mmax) {
		istep = mmax<<1;
		theta = isign*(PI/mmax);
		wtemp = sin(0.5*theta);
		wp = COMPLEX(-2.0*wtemp*wtemp, sin(theta));
		w = COMPLEX(1.0,0.0);

		for(m=0;m<mmax;m++) {
			for(i=m;i<n;i+=istep) {
				j=i+mmax;
				temp = w*data[j];
				data[j] = data[i]-temp;
				data[i] += temp;
			}
			w = w*wp + w;
		}
		mmax = istep;
	}
}

/*  *********   GRAPHICS SUBROUTINES   **********            */

/*            *** Scaling Subroutine ***                     */
int plot_scale(double d_small, double d_large, double& d_left,
		double& d_right, double& increment)
{
/* All scaling is based on normalizing the values to
    a number between 1.0 and 10.0 times a power of 10. */

/* The scaling matrix:
    First entry is the normalized limit for values which
    would correspond to an increment value in the second
    column.  e.g. a normalized value of 4.2 is more than
    2.0 but less than 5.0, so would use the 5.0 line with
    0.5 as the scaling increment value.  Similarly, a value
    of 1.2 would use 0.2 as an increment value.         */

  static double scale_matrix[4][2] = {
       { 1.0,  0.1},
       { 2.0,  0.2},
       { 5.0,  0.5},
       {10.0,  1.0}
              };

  double  fraction, int_part;
  double  power_of_10;
  double  d_normalized;
  double  d_convert, temp;
  double  l_temp, r_temp, i_temp;
  int n;

/* Start by making sure the input values are in the correct order  */
  if(d_small>d_large) 
    { temp = d_large;
      d_large = d_small;
      d_small = temp;
     }
       
/* Check for which of the five possible cases applies:       */
/*      -,-  |  -,0   |  -,+  |  0,+  |  +,+                 */

  if(d_large>0.0 && d_small>=0.0)
   {
/* In this part of the program, 0.0 is included in the plot */
/*  if the smaller value is less than 0.6 times the larger. */

//     if(d_small < 0.6*d_large) d_small = 0.0;

/* Convert to logarithm base 10                           */
       d_convert = log10(fabs(d_large-d_small));

/* Split the term into a mantissa and exponent to create  */
/*   a normalized y value.                                */
/* The multiplier 2.302585 allows a base 10 logarithm     */
/*   to be used in an "e to the x" expression.            */

     d_normalized = exp(2.302585*modf(d_convert,&power_of_10));

/*  Correct for the representation of numbers less than   */
/*    1.0 to ensure all normalized numbers are between    */
/*           1.0 <= d_normalized < 10.0                   */

       if (d_normalized<1.0)
         {
          d_normalized = d_normalized *10.0;
          power_of_10 = power_of_10 - 1.0;
         }

/* Find the correct increment size for the normalized data   */
       n = 0;
       while (d_normalized > scale_matrix[n][0]) n++;
       increment = scale_matrix[n][1] * pow(10.0, power_of_10);

/* Find the right end point as a multiple of increments      */
       fraction=modf(d_large/(increment),&int_part);
       if(fraction>0.0001) int_part++;
       d_right = (int_part)*(increment);

/* Find the left end as a multiple of increments            */
       fraction=modf(d_small/(increment),&int_part);
       d_left = (int_part)*(increment);
   }

/* Case where y_small < 0.0 and y_large > 0.0              */
   if (d_small < 0.0 && d_large > 0.0)
   {

/* Scale based on total distance                         */
       d_convert = log10(fabs(d_large-d_small));
       d_normalized = exp(2.302585 * modf(d_convert, &power_of_10));

/*  Correct for C's representation of numbers less than 1.0 */
       if (d_normalized<1.0)
         {
          d_normalized = d_normalized *10.0;
          power_of_10 = power_of_10 - 1.0;
         }

       n = 0;
       while (d_normalized > scale_matrix[n][0]) n++;
       increment = scale_matrix[n][1] * pow(10.0, power_of_10);
       fraction=modf(d_large/(increment),&int_part);
       if(fraction>0.0001) int_part++;
       d_right = (int_part)*(increment);
       fraction=modf(fabs(d_small)/(increment),&int_part);
       if(fraction>0.0001) int_part++;
       d_left = -(int_part)*(increment);
    }

/* Lastly, the minus, minus cases                           */
/* Handled recursively by reflecting the values about zero  */
/*  and recalling the subroutine                            */

  if(d_small<0.0 && d_large<=0.0)
   {
   r_temp= fabs(d_small);
   l_temp= fabs(d_large);
   plot_scale(l_temp, r_temp, l_temp, r_temp, i_temp);
   d_left = (-r_temp);
   d_right = (-l_temp);
   increment = i_temp;
   }

/* At this point, check if both y_large and y_small        */
/*  are equal to zero                                      */
   if (d_large==0.0 && d_small==0.0)
   {
   AfxMessageBox("Problem with the values in the plot",MB_OK);
   return 1;
   }
return 0;
} 

double H(double x)
{
	if(x<=0.0) return 0.0;
	else return 1.0;
}

double FC(double vc, double alpha, double beta, double vt)
{
	double result = (-beta)*vc + 0.5*(beta-alpha)*(fabs(vc+vt)-fabs(vc-vt));
	return result;
}