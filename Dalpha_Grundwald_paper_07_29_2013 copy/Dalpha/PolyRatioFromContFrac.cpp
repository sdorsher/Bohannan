#include "PolyRatioFromContFrac.h"
#include<iostream>
#include<fstream>
using namespace std;
//current algorithm can create coefficients up to an index of m_n=3791. P_3791 or m_timePoints= 3792 terms.

PolyRatioFromContFrac::PolyRatioFromContFrac(void)
{
}

PolyRatioFromContFrac::~PolyRatioFromContFrac(void)
{
}


void PolyRatioFromContFrac::initialize(double alphaExp, int numCoeffinNumerator)
{
	//m_timePoints = ;

	ultimateExpnOrder = numCoeffinNumerator-1;

	Pnow.resize(ultimateExpnOrder+1);
	Qnow.resize(ultimateExpnOrder+1);
	Pminus1.resize(ultimateExpnOrder+1);
	Qminus1.resize(ultimateExpnOrder+1);
	Pminus2.resize(ultimateExpnOrder+1);
	Qminus2.resize(ultimateExpnOrder+1);
/*	m_pP = new double[ultimateExpnOrder][RECURSION_HIST_PTS];
	m_pQ = new double[ultimateExpnOrder][RECURSION_HIST_PTS];
	coeffPQ = new double[ultimateExpnOrder][2];
	*/

	for(int k=0; k<=ultimateExpnOrder; k++)
	{
		/*for(int m=0; m<RECURSION_HIST_PTS; m++)
		{
			m_pP[k][m]=0.0;
			m_pQ[k][m]=0.0;
		}*/

		//set all polynomials to zero (at all orders of z^-k)
		Pnow[k]=0.0;
		Qnow[k]=0.0;
		Pminus1[k]=0.0;
		Qminus1[k]=0.0;
		Pminus2[k]=0.0;
		Qminus2[k]=0.0;

	}

//	m_pP[0][2]=1.0;//initialize P_-1 to 1 -- no longer do this now that using P"
	Qnow[0]=1.0;// initialize the 0th order of expansion of Q (Q_0) to 1
	Pnow[0]=1.0;// initialize P_0 to 1.

	alpha = alphaExp;
	currentExpnOrder = 0;

}

void PolyRatioFromContFrac::calculatePolynomials(void)
{
	if(ultimateExpnOrder>0)
	{
		firstIteratePolynomials();
	}
	if(ultimateExpnOrder>1)
	{

		for(int k = 2; k<=ultimateExpnOrder; k++) {
			iteratePolynomials();
		}
	}

}


void PolyRatioFromContFrac::firstIteratePolynomials(void)
{ //This routine handles the k=1 step for polynomial generation. 

	//update the history
	Pminus1[0]=1.0;
	Qminus1[0]=1.0;
	
	//set the P_1 and Q_1 polynomials
	Pnow[0]=0.5;
	Pnow[1]=-0.5*alpha;
	Qnow[0]=0.5;
	Qnow[1]=0.5*alpha;
	currentExpnOrder=1; //maximum power of z^-j is j=1
}


void PolyRatioFromContFrac::iteratePolynomials(void)
{
	 
	// P_-1 =1, P_0 = 1, Q_-1 = 0, Q_0 = 1 in the case of this CFE
	// for this CFE, P_k= b_k P_(k-1) + a_k P_(k-2)
	// a_k = r^2 - (k-1)^2
	// b_k = (2k-1) z
	// to get in form that is causal,
	// P_j'=(z^-k)P_k
	// to rescale to be able to calculate coefficients to greater n so numbers don't blow up so fast, 
	//	P_j" = P_j'/((2^j)*(j!))
	//P_j"=(1-1/(2j))P_(j-1)"+(r^2/(4j(j-1))-1/4+1/(4j))(z^-2)P_(j-2)" 
	// Q always obeys same recursion relation as P, with different initial conditions
	//from notebook on 8/7/12 normalized by 2^-j/j!

	for(int k=0; k<=currentExpnOrder; k++)
	{
/*		for(int m=RECURSION_HIST_PTS-1; m>0; m--)
		{	
			m_pP[k][m]=m_pP[k][m-1]; // shift the current polynomial to the previous polynomial 
			m_pQ[k][m]=m_pQ[k][m-1]; // and shift the previous polynomial to two steps ago
		}
*/
		//shift the current polynomial to the previous polynomial
		Pminus2[k]=Pminus1[k];
		Qminus2[k]=Qminus1[k];
		Pminus1[k]=Pnow[k];
		Qminus1[k]=Qnow[k];


		Pnow[k]=0.0; // clear the current polynomial
		Qnow[k]=0.0;
	}
	currentExpnOrder++; // increment the index of the current polynomial

	double coeff2shift, coeff1const; //coefficients in front of the Pminus1 and Pminus2 terms that either shift by z^-2 or hold constant
	double jj = (double) currentExpnOrder;
	coeff2shift=(1.0/4.0)*(-1.0 + 1.0/jj + alpha*alpha/(jj*(jj-1.0))); 
	coeff1const=1.0 - 1.0/(2.0*jj); 

	for(int k=0; k<=currentExpnOrder; k++)
	{
		

		Pnow[k]+=coeff1const*Pminus1[k];
		Qnow[k]+=coeff1const*Qminus1[k];
		if(k<=currentExpnOrder-2)
		{
				Pnow[k+2]+=coeff2shift*Pminus2[k];
				Qnow[k+2]+=coeff2shift*Qminus2[k];
		}
		
	}

}

void PolyRatioFromContFrac::rescale(void)
{// rescale the coefficients so that the z^0 term of P is equal to 1. 
	double scaleFac = Pnow[0];
	for(int k = 0; k<=ultimateExpnOrder; k++)
	{
		Pnow[k]=Pnow[k]/scaleFac;
		Qnow[k]=Qnow[k]/scaleFac;
	}

}

void PolyRatioFromContFrac::printCoeff(void)
{
	ofstream fs;
	fs.open("recursionCFEcoeff.txt");
	for(int k=0; k<=ultimateExpnOrder; k++)
	{
		fs<< k<< "\t" <<Pnow[k] <<"\t" << Qnow[k] << endl;
	}
	fs.close();
}

