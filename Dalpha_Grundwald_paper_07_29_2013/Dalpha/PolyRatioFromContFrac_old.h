/* Convert continued fractions of the form b_0 + a_0/(b_1 +(a_1/(b_2+(a_2/...
* into a ratio of polynomials of the form P/Q to the same order expansionOrder using recursion
* relations taken from the CRC Concise Encyclopedia of Mathematics by Eric Weisstein.
* The continued fraction expansion was derived from Expansions of Certain Functions 
* equation 2.13. 
*/

#pragma once
class PolyRatioFromContFrac
{
public:
	static const int TIME_POINTS = 1000;
	static const int RECURSION_HIST_PTS=3; // number of indicies into the past the P and Q arrays are stored. 
	//Three is recommended since the current and past two are needed for recursion but usually only the current is of interest 

private:
	double m_r; // exponent power in s^r
	int m_n; // expansion order
	int m_j; // index of the expansion corresponding to the data contained in the current variables
	int m_timePoints; //number of time points stored within each P and Q time series. corresponds to z^-k

public:
	double (*m_pP)[RECURSION_HIST_PTS]; //numerator polynomial coefficients of the expansion of the continued fraction of ((1-z^-1)/(1+z^-1))^r
	double (*m_pQ)[RECURSION_HIST_PTS]; // denomonator polynomial coefficients. z^-k terms where k is the index.
	double (*coeffPQ)[2];

public:
	PolyRatioFromContFrac(void);
	~PolyRatioFromContFrac(void);
	void initialize(double r, int n);
	void calculatePolynomials(void);
	void rescale(void);
	void printCoeff(void);
private:
	void firstIteratePolynomials(void);
	void iteratePolynomials(void);
};
