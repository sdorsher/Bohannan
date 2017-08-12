// FracDoc.h : interface of the CFracDoc class
//
/////////////////////////////////////////////////////////////////////////////
// Part of the validation of the fractional order state
//  space model. This simulates the Quanser RFJ arm
//
//

#if !defined(AFX_FRACDOC_H__4CE2D14A_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
#define AFX_FRACDOC_H__4CE2D14A_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "math.h"
#include "nrutil.h"
#include "Param.h"
#include "Measurement.h"
#include "Noise.h"

class ShiftRegister : public CObject
{
public:
	ShiftRegister();
	~ShiftRegister();
	ShiftRegister(UINT size);
	UINT Size;
	void RemoveAll();
	void SetSize(UINT size);
	void Shift();
	int k;		// current zero pointer index
	CArray<double, double&> storage;
	double& operator [](int index);
};

class Fractance : public CObject
{
public:
	Fractance();
	~Fractance();
	Fractance(UINT size);
	UINT Size;
	CArray<double, double&> history;
	double& operator [](int index);
	void RemoveAll();
	void SetSize(UINT size);
	double FractDeriv(UINT n);
	double PartSum(UINT n);
	double Memory(UINT n);
	double alpha;
	double tau;
	double multiplier;
};

class FractOp : public CObject
{
public:
	FractOp();
	double FractDeriv(UINT n, Fractance &F);
	double PartSum(UINT n, Fractance &F);
	//double LnGamma(double xx);
	double multiplier;
	double order;
	double tau;
	void RemoveAll();
	void SetSize(UINT size);
	static double cof[6];
};

class CFracDoc : public CDocument
{
protected: // create from serialization only
	CFracDoc();
	DECLARE_DYNCREATE(CFracDoc)

// Attributes
protected:
	CSize	m_sizeDoc;
	CFracApp* app;
	CNoise	prn;
	double (CNoise::*fcn)();	//pointer to the right noise source
	CParam	pDlg;
	CMeasurement mDlg;

public:
// Version number to document method being use
	CString Version;
	CString LabelX;
	CString LabelY;

// value arrays and size information
//  The A and B matrices are seen as fractional order operators
	FractOp     A[5][5];	// operators over the states
	FractOp		B[5];		// operators over the input variable(s)
	FractOp		K;

// The states record the values of the state variables. A fractance
//  object can return derivatives of arbitrary order over its history.

	Fractance	X[5];		// the drive and internal states 
	Fractance	Y;			// The total response angle
	Fractance	R;			// The input requirement angle
	Fractance	E;			// The error signal
	Fractance   UD;			// The digital control signal
	double r[10];
	double CoeffA[10];
	double CoeffB[10];

	
	double		AH[5][5];	// history contribution for each element
							//  of the A operator element acting on
							//  the state histories.

	double		BH[5];		// history contribution from the input

	// The digital IIR filter for FOC approximation
	
	ShiftRegister CX;
	ShiftRegister CY;

	// The computational arrays for RFJ simulation
	#define	U			X[0]
	#define	Theta		X[1]
	#define	ThetaDot	X[2]
	#define Phi			X[3]
	#define PhiDot		X[4]
	#define ANALOG		0
	#define DIGITAL		1
	double	Drive;
	double  TEMP;
	UINT    TEMP_NDX;

	CArray<double, double&> TimeArray;
	CArray<COMPLEX, COMPLEX&> ComplexData;
	CArray<double, double&> FreqArray;
	CArray<double, double&> PSDArray1;
	CArray<double, double&> PSDArray2;
	CArray<CTime, CTime&> TimeStampArray;

	int nNumSamples;
	int nNumPTSamples;
	int nNumSPECSamples;

	UINT	N;		// number of simulation time intervals
	//double	alpha1;	// the initial fractional control exponent
	//double	alpha2;	// the temporary fractional exponent
	double	y0;		// initial polarization
	double	epsilon[2]; // epsilon prime and double prime
						// recovered from the simulation
	double  sticky;
	double	tau;	// time scale for the fractor device
	double  I_alpha;
	double  I_gain;
	double  I_tau;
	double  P_0;
	double	T;		// simulation duration
	double  dt;		// time step
	double  TS;		// digital sampling time
	int     count;
	int	    interval;

	BOOL	ac_On;
	double	VG;		// Input Amplitude
	double	frequency; // frequency of applied field
	double	omega;	// angular frequency of applied field
	double	Period;	// E field oscillation period

	double	order;
	double  sigma;
	BOOL	b_Efld_noise;
	BOOL	b_Pol_noise;

	BOOL	Pulsed_On;
	double	step_frequency;
	double	step_amplitude;
	double	P_Period;
	double	P_duty_cycle;
	int		Controller;
	
	BOOL	gated;	// use the pulsed signal to 
					// gate the ac signal on and off

	BOOL	bipolar;

	BOOL	b_run;	// switch to indicate valid data available 
					// for drawing in view.
	BOOL	b_measured; // switch to indicate if the permittivity
					// has been measured for this run
	BOOL	b_In_Progress;
	BOOL	b_newData;

	double	delay;	// delay time for start of measurement
	double	capture;	// length of time to capture for measurement
	UINT		delay_index;
	UINT		capture_index;

// Process time tracking
	CTime	StartTime;
	CTime	EndTime;
	CTimeSpan	ElapsedTime;
	bool	bPauseFlag;
	bool	bStopFlag;

// Operations
public:
	CSize	GetDocSize() {return m_sizeDoc;}
	void	ComputeSpectrum();
	void	ResetSpectralData();

private:
	void	DeleteContents();
	void	bailout();
//	double	PartSum(CArray<double, double&>& x, int n, double q);
//	double	FracDeriv(CArray<double, double&>& x, int n, double q);

	CString mFileName;
	CFileException	mfileException;
	static char BASED_CODE szTimeFilter[];	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFracDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFracDoc();
        
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	InitDocument();
    virtual BOOL OnOpenDocument(const char* pszPathName);
	void	PumpMessages();

// Generated message map functions
protected:
	//{{AFX_MSG(CFracDoc)
	afx_msg void OnOperationRun();
	afx_msg void OnOperationSetup();
	afx_msg void OnUpdateOperationSetup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOperationRun(CCmdUI* pCmdUI);
	afx_msg void OnFileSavetimeseries();
	afx_msg void OnUpdateFileSavetimeseries(CCmdUI* pCmdUI);
	afx_msg void OnOperationsMeasurementSetUp();
	afx_msg void OnUpdateOperationsMeasurementSetUp(CCmdUI* pCmdUI);
	afx_msg void OnOperationsMeasure();
	afx_msg void OnUpdateOperationsMeasure(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRACDOC_H__4CE2D14A_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
