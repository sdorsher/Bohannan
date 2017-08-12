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


void PolyRatioFromContFrac::initialize(double r, int n)
{
	m_timePoints = n+1;

	m_pP = new double[m_timePoints][RECURSION_HIST_PTS];
	m_pQ = new double[m_timePoints][RECURSION_HIST_PTS];
	coeffPQ = new double[m_timePoints][2];


	for(int k=0; k<m_timePoints; k++)
	{
		for(int m=0; m<RECURSION_HIST_PTS; m++)
		{
			m_pP[k][m]=0.0;
			m_pQ[k][m]=0.0;
		}
	}

//	m_pP[0][2]=1.0;//initialize P_-1 to 1 -- no longer do this now that using P"
	m_pQ[0][1]=1.0;// initialize Q_0 to 1
	m_pP[0][1]=1.0;// initialize P_0 to 1

	m_r = r;
	m_n = n;
	m_j = 0;

}

void PolyRatioFromContFrac::calculatePolynomials(void)
{
	if(m_n>0)
	{
		firstIteratePolynomials();
	}
	if(m_n>1)
	{
		for(int k = 2; k<=m_n; k++) {
			iteratePolynomials();
		}
	}

}


void PolyRatioFromContFrac::firstIteratePolynomials(void)
{ //This routine handles the k=1 step for polynomial generation. 

	m_pP[0][0]=0.5;
	m_pP[1][0]=-0.5*m_r;
	m_pQ[0][0]=0.5;
	m_pQ[1][0]=0.5*m_r;
	m_j=1; //maximum power of z^-j is j=1
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

	for(int k=0; k<=m_j; k++)
	{
		for(int m=RECURSION_HIST_PTS-1; m>0; m--)
		{	
			m_pP[k][m]=m_pP[k][m-1]; // shift the current polynomial to the previous polynomial 
			m_pQ[k][m]=m_pQ[k][m-1]; // and shift the previous polynomial to two steps ago
		}
		m_pP[k][0]=0.0; // clear the current polynomial
		m_pQ[k][0]=0.0;
	}
	m_j++; // increment the index of the current polynomial

	double coeffAshift, coeffBconst;
	double rr= (double) m_r;
	double jj = (double) m_j;
	coeffAshift=(1.0/4.0)*(-1.0 + 1.0/jj + rr*rr/(jj*(jj-1.0))); 
	coeffBconst=1.0 - 1.0/(2.0*jj); 

	for(int k=0; k<=m_j; k++)
	{
		

		m_pP[k][0]+=coeffBconst*m_pP[k][1];
		m_pP[k+2][0]+=coeffAshift*m_pP[k][2];
		m_pQ[k][0]+=coeffBconst*m_pQ[k][1];
		m_pQ[k+2][0]+=coeffAshift*m_pQ[k][2];
		
	}

}

void PolyRatioFromContFrac::rescale(void)
{// rescale the coefficients so that the z^0 term of P is equal to 1. 
	for(int jj = 0; jj<m_timePoints; jj++)
	{
		coeffPQ[jj][0]=m_pP[jj][0]/m_pP[0][0];
		coeffPQ[jj][1]=m_pQ[jj][0]/m_pP[0][0];
	}

}

void PolyRatioFromContFrac::printCoeff(void)
{
	ofstream fs;
	fs.open("recursionCFEcoeff.txt");
	for(int jj=0; jj<m_timePoints; jj++)
	{
		fs<< jj<< "\t" <<coeffPQ[jj][0] <<"\t" << coeffPQ[jj][1] << endl;
	}
	fs.close();
}

