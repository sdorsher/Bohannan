// Noise.h: interface for the CNoise class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOISE_H__59D7CA40_6782_11D3_8019_00105A9399D0__INCLUDED_)
#define AFX_NOISE_H__59D7CA40_6782_11D3_8019_00105A9399D0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#define NMax 5.0e5

class CNoise : public CObject  
{
public:
	CNoise();
	virtual ~CNoise();
	void   SetAlpha(double Aplha);
	double ran2();
	double gasdev();
	double cauchy();
	double aStable();

private:
	double alpha;	//the statistical order
	double one_minus_alpha;
	double one_over_alpha;
	double one_minus_alpha_over_alpha;
	int idum;		//the prn seed

};

#endif // !defined(AFX_NOISE_H__59D7CA40_6782_11D3_8019_00105A9399D0__INCLUDED_)
