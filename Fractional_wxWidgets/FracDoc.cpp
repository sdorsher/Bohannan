// FracDoc.cpp : implementation of the CFracDoc class
//  and its fractional order operator classes
//
// Part of the validation of the fractional order state
//  space model. This simulates the Quanser RFJ arm
//


#include "stdafx.h"
#include "Fractional.h"
#include "MainFrm.h"
#include "FracDoc.h"
#include "math.h"

#include "nrutil.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ShiftRegister
// Implementation of a shift register for use with digital control system
//

ShiftRegister::ShiftRegister()
{
	k = 0;
	Size = 0;
}

ShiftRegister::~ShiftRegister()
{
	RemoveAll();
}

ShiftRegister::ShiftRegister(UINT size)
{
	k = 0;
	Size = size;
	storage.SetSize(Size);
	for(UINT j=0;j<Size;j++) storage[j] = 0.0;
}

void ShiftRegister::Shift()
{
	k = k--;
	if(k<0) k = Size-1;
}

double& ShiftRegister::operator [] (int index)
{
	int j;
	j = (k+index)%Size;
	return storage[j];
}

void ShiftRegister::RemoveAll()
{
	storage.RemoveAll();
}

void ShiftRegister::SetSize(UINT size)
{
	storage.SetSize(size);
	Size = size;
	for(UINT j=0;j<Size;j++) storage[j] = 0.0;
}

/////////////////////////////////////////////////////////////////////////////
// Fractance
// Implements signal history arrays
// These allow application of Fractional Order Operators (FractOp)
//  to give back current values of the derivatives.

Fractance::Fractance()
{
	Size = 0;
	alpha = -0.75;
	multiplier = 1.0;
}

Fractance::Fractance(UINT size)
{
	Size = size;
	history.SetSize(size);
	alpha = -0.75;
	multiplier = 1.0;
}

Fractance::~Fractance()
{
	history.RemoveAll();
}

double& Fractance::operator [] (int index)
{
	return history[index];
}

void Fractance::RemoveAll()
{
	history.RemoveAll();
}

void Fractance::SetSize(UINT size)
{
	Size = size;
	history.SetSize(size);
}

double Fractance::FractDeriv(UINT n)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-alpha-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<=n;k++) PS = history[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

double Fractance::PartSum(UINT n)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-alpha-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<n;k++) PS = history[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

/////////////////////////////////////////////////////////////////////////////
// FractOp
// Implements the Fractional Order Operator
// Used to designate the order of the operator
//  and the multiplication factor used in 
//  FOC state space matrices

FractOp::FractOp()
{
	//multiplier = 1.0;
	//order = 0.0;
}

double FractOp::PartSum(UINT n, Fractance &F)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-order-1;
	denominator = n;
	PS = 0.0;
	for(UINT k=0;k<n;k++) PS = F.history[k] + PS*(numerator-k)/(denominator-k);
	return PS;
}

/*
double FractOp::LnGamma(double xx)
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

double FractOp::FractDeriv(UINT n, Fractance &F)
{
	double PS;
	double numerator;
	double denominator;
	numerator = n-order-1;
	denominator = n;
	PS = F.history[0];
	for(UINT k=0;k<n;k++) PS = F.history[k+1] + PS*(numerator-k)/(denominator-k);
	return PS;
}


////////////////////////////////////////////////////////////////////////////
// CFracDoc

IMPLEMENT_DYNCREATE(CFracDoc, CDocument)

BEGIN_MESSAGE_MAP(CFracDoc, CDocument)
	//{{AFX_MSG_MAP(CFracDoc)
	ON_COMMAND(ID_OPERATION_RUN, OnOperationRun)
	ON_COMMAND(ID_OPERATION_SETUP, OnOperationSetup)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_SETUP, OnUpdateOperationSetup)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_RUN, OnUpdateOperationRun)
	ON_COMMAND(ID_FILE_SAVETIMESERIES, OnFileSavetimeseries)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVETIMESERIES, OnUpdateFileSavetimeseries)
	ON_COMMAND(ID_OPERATIONS_MEASUREMENTSETUP, OnOperationsMeasurementSetUp)
	ON_UPDATE_COMMAND_UI(ID_OPERATIONS_MEASUREMENTSETUP, OnUpdateOperationsMeasurementSetUp)
	ON_COMMAND(ID_OPERATIONS_MEASURE, OnOperationsMeasure)
	ON_UPDATE_COMMAND_UI(ID_OPERATIONS_MEASURE, OnUpdateOperationsMeasure)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFracDoc construction/destruction

CFracDoc::CFracDoc()
{
// Establish the default values
	N = 5000;		// default num of steps
	T = 2.5;		// default duration (sec)
	dt = T/N;		// the time step
	TS = 10.0 * dt;	// default sample time
	interval = int(TS/dt);

	sticky = 0.05;
	I_tau = 0.016;
	I_alpha = (-0.35);
	I_gain = 0.08;
	P_0 = 2.5;
	Controller = 0;

	// set up default noise source
	fcn = &CNoise::gasdev;
	order = 2.0;
	sigma = 0.0;
	prn.SetAlpha(2.0);
	b_Efld_noise = FALSE;
	b_Pol_noise = FALSE;
	
	delay_index = N/2;
	capture_index = N/4;
	delay = dt*delay_index;
	capture = dt*capture_index;

	b_measured = FALSE;
	epsilon[0] = 0.0;
	epsilon[1] = 0.0;

	ac_On = FALSE;
	VG = 0.2;			// default amplitude
	Period = TwoPI;		// default period
	frequency = 0.4;
	omega = TwoPI*frequency;

	Pulsed_On = TRUE;
	step_amplitude = VG;
	step_frequency = 0.2;
	P_Period = 4.0 / step_frequency;
	P_duty_cycle = 0.5;		// 50% duty cycle
	bipolar = TRUE;

	gated = FALSE;			// Add the inputs
	
	b_run = FALSE;			// indicate no valid data for drawing
	bPauseFlag = false;		// not paused
	bStopFlag = false;		// not stopped
	b_In_Progress = FALSE;
	b_newData =FALSE;
	ElapsedTime = 0;
	Version = "3.0 - Direct";
}

CFracDoc::~CFracDoc()
{

}

/////////////////////////////////////////////////////////////////////////////
// Initialization

void CFracDoc::InitDocument()
{
	app = (CFracApp*)AfxGetApp();
	m_sizeDoc = CSize(7000,9000);
	//default sizes for parameters
}

BOOL CFracDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	InitDocument();

	return TRUE;
}

BOOL CFracDoc::OnOpenDocument(const char* pszPathName)
{
	if(!CDocument::OnOpenDocument(pszPathName))
		return FALSE;
	InitDocument();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clean up

void CFracDoc::DeleteContents()
{
	b_run = FALSE;
	b_measured = FALSE;
	epsilon[0] = 0.0;
	epsilon[1] = 0.0;

	X[0].RemoveAll();	// input history
	X[1].RemoveAll();
	X[2].RemoveAll();
	X[3].RemoveAll();
	X[4].RemoveAll();
	E.RemoveAll();
	Y.RemoveAll();
	R.RemoveAll();
	UD.RemoveAll();

	ComplexData.RemoveAll();
	FreqArray.RemoveAll();
	PSDArray1.RemoveAll();
	PSDArray2.RemoveAll();
	nNumSamples = 0;
	nNumPTSamples = 0;
	nNumSPECSamples = 0;

	CX.RemoveAll();
	CY.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CFracDoc serialization

void CFracDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << N << T << dt << y0;
		ar << ac_On << VG << frequency << omega << Period << bipolar;
		ar << Pulsed_On << step_frequency << step_amplitude << P_duty_cycle;
		ar << gated << delay << delay_index << capture << capture_index;
		ar << sticky << I_alpha << I_gain << I_tau << P_0;
	}
	else
	{
		ar >> N >> T >> dt >> y0;
		ar << ac_On >> VG >> frequency >> omega >> Period >> bipolar;
		ar >> Pulsed_On >> step_frequency >> step_amplitude >> P_duty_cycle;
		ar >> gated >> delay >> delay_index >> capture >> capture_index;
		ar >> sticky >> I_alpha >> I_gain >> I_tau >> P_0;
		b_run = FALSE;
		b_measured = FALSE;
		b_newData = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFracDoc File Handlers

void CFracDoc::OnFileSavetimeseries() 
{
	CString sz;
	CStdioFile	timeFile;
	CFileDialog	fdlg(FALSE, ".dat", GetTitle(), OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, szTimeFilter, NULL);
	if(fdlg.DoModal() == IDOK)
	{
		mFileName = fdlg.GetPathName();
		if(!timeFile.Open(mFileName, CFile::modeReadWrite |
			CFile::modeCreate, &mfileException))
		{
			TRACE("Can't open file %s, error = #u\n",mFileName,
				mfileException.m_cause);
			return;
		}
		SetTitle(fdlg.GetFileTitle());
		sz.Format("Time,R,U,Theta,Phi,ThetaDot,PhiDot,Total Angle,Error,Digital Control\n");
		timeFile.WriteString(sz);
		for(UINT k = 1;k<=N;k++)
		{
			sz.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n",
				k*dt,R[k],X[0][k],X[1][k],X[2][k],X[3][k],X[4][k],Y[k],E[k],UD[k]);
			timeFile.WriteString(sz);
		}
	}
}

void CFracDoc::OnUpdateFileSavetimeseries(CCmdUI* pCmdUI) 
{
	if(b_run) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CFracDoc diagnostics

#ifdef _DEBUG
void CFracDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFracDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFracDoc commands

void CFracDoc::OnOperationRun() 
{
	//unsigned int j,k;

	if(b_In_Progress) return;
	b_In_Progress = TRUE;
	b_run = FALSE;
	b_measured = FALSE;

	CMainFrame* pFrmWnd = (CMainFrame*) AfxGetMainWnd();
	pFrmWnd->m_wndStatusBar.SetPaneText(0,"Running");
	pFrmWnd->m_wndStatusBar.SetPaneText(1,"Field");
	StartTime = CTime::GetCurrentTime();
	SetTitle("Amoeba Simulation");

// clear out any old data arrays
	ResetSpectralData();

// build working arrays
	X[0].SetSize(N+1);
	X[0].alpha = 0.0;

	X[1].SetSize(N+1);
	X[1].alpha = 0.0;

	X[2].SetSize(N+1);
	X[2].alpha = 0.0;

	X[3].SetSize(N+1);
	X[3].alpha = (0.0);

	X[4].SetSize(N+1);
	X[4].alpha = (0.0);

	E.SetSize(N+1);
	E.alpha = 0.0;

	R.SetSize(N+1);
	R.alpha = 0.0;

	Y.SetSize(N+1);
	Y.alpha = 0.0;

	UD.SetSize(N+1);
	UD.alpha = 0.0;

	CX.SetSize(10);
	CY.SetSize(10);

	pFrmWnd->m_wndStatusBar.SetPaneText(1,"Input");
	PumpMessages();

// Generate the coefficients for the digital FOC
	r[0] = 1.0;
	r[1] = fabs(I_alpha);
	for(UINT j=2;j<10;j++) r[j] = r[j-1]*r[1];
	CoeffA[0] = r[0];
	CoeffA[1] = r[1];
	CoeffA[2] = (-2.11764706)*r[0] + 0.470588235*r[2];
	CoeffA[3] = (-1.78431373)*r[1] + 0.137254902*r[3];
	CoeffA[4] = 1.482352941*r[0] - 0.68627451*r[2] + 0.02745098*r[4];
	CoeffA[5] = 0.976472213*r[1] - 0.1568275*r[3] + 0.003921569*r[5];
	CoeffA[6] = (-0.3800905)*r[0] - 0.276118653*r[2] -0.0231272*r[4] + 0.000402212*r[6];
	CoeffA[7] = (-0.16664512)*r[1] +0.042131724*r[3] - 0.00221217*r[5] + (2.87294e-5)*r[7];
	CoeffA[8] = 0.025915261*r[0] - 0.02640105*r[2] + 0.003491933*r[4] - 0.00012798*r[6] + (1.30588e-6)*r[8];
	CoeffA[9] = 0.00427912*r[1] - 0.00152295*r[3] + 0.000126758*r[5] - (3.4824e-6)*r[7] + (2.90196e-8)*r[9];
	
	double sign = 1.0;
	for(UINT j= 0;j<10;j++) 
	{
		CoeffB[j] = sign * CoeffA[j];
		sign = (-sign);
	}

// Generate the Input Angle Requirement signal

	for(UINT n=0;n<=N;n++) Y[n] = 0.0;
	for(UINT n=0;n<=N;n++) X[0][n] = 0.0;

	R[0] = 0.0;
	if(Pulsed_On && ac_On && gated) {
		for(UINT n=1;n<=N;n++) {
			if(fmod(n*dt,P_Period)<(0.5*P_Period)) R[n] = VG*sin(n*dt*omega);
			else X[0][n] = 0.0;
		}
	}
	if(Pulsed_On && !gated) for(UINT n=1;n<=N;n++) {
		if(fmod(n*dt,P_Period)<(P_duty_cycle*P_Period)) R[n] = VG;
		else 
		if(bipolar)	R[n] = -VG; //(-step_amplitude);
		else R[n] = 0.0;
	}

	if(ac_On && !gated) for(UINT n=1;n<=N;n++) 
		R[n] = VG*sin(n*dt*omega);

	if(b_Efld_noise) for(UINT n=0;n<=N;n++) 
		R[n] += sigma*(prn.*fcn)();

// Generate the A and B matrix elements
	for(UINT n=0;n<=N;n++)
	{
		A[1][1].multiplier = 0.0;
		A[1][1].order = 0.0;
		A[1][1].tau = 1.0;

		A[1][2].multiplier = 0.0;
		A[1][2].order = 0.0;
		A[1][2].tau = 1.0;

		A[1][3].multiplier = 1.0;
		A[1][3].order = 0.0;
		A[1][3].tau = 1.0;

		A[1][4].multiplier = 0.0;
		A[1][4].order = 0.0;
		A[1][4].tau = 1.0;

		A[2][1].multiplier = 0.0;
		A[2][1].order = 0.0;
		A[2][1].tau = 1.0;

		A[2][2].multiplier = 0.0;
		A[2][2].order = 0.0;
		A[2][2].tau = 1.0;

		A[2][3].multiplier = 0.0;
		A[2][3].order = 0.0;
		A[2][3].tau = 1.0;

		A[2][4].multiplier = 1.0;
		A[2][4].order = 0.0;
		A[2][4].tau = 1.0;

		A[3][1].multiplier = 0.0;
		A[3][1].order = 0.0;
		A[3][1].tau = 1.0;

		A[3][2].multiplier = 735.0;
		A[3][2].order = 0.0;
		A[3][2].tau = 1.0;

		A[3][3].multiplier = (-48.2);
		A[3][3].order = 0.0;
		A[3][3].tau = 1.0;

		A[3][4].multiplier = 0.0;
		A[3][4].order = 0.0;
		A[3][4].tau = 1.0;

		A[4][1].multiplier = 0.0;
		A[4][1].order = 0.0;
		A[4][1].tau = 1.0;

		A[4][2].multiplier = (-1022.0);
		A[4][2].order = 0.0;
		A[4][2].tau = 1.0;

		A[4][3].multiplier = (48.2);
		A[4][3].order = 0.0;
		A[4][3].tau = 1.0;

		A[4][4].multiplier = 0.0;
		A[4][4].order = 0.0;
		A[4][4].tau = 1.0;


		B[1].multiplier = 0.0;
		B[1].order = 0.0;
		B[1].tau = 1.0;

		B[2].multiplier = 0.0;
		B[2].order = 0.0;
		B[2].tau = 1.0;

		B[3].multiplier = 89.784;
		B[3].order = 0.0;
		B[3].tau = 1.0;

		B[4].multiplier = (-89.783);
		B[4].order = 0.0;
		B[4].tau = 1.0;

		K.multiplier = I_gain;
		K.order = I_alpha;
		K.tau = 1.0;
	}

	pFrmWnd->m_wndStatusBar.SetPaneText(1,"Output");
	PumpMessages();

// Calculate the current, with and without noise.
// From the history of the current, calculate the voltage.
// This scheme uses the direct integration method. 

// Note: this method assumes an uninitialized system. 
//  VG starts from 0 V state.
// For consistency with standard notation, only the non-zero index elements are used.
//  The zero index row and zero index column are for storing other variables.
// The zero index of the state vector stores the input signal.

// The following is for fractional order modeling
// Clear any history from previous runs
	for(UINT j=0;j<=4;j++)
	{
		BH[j] = 0.0;
		for(UINT k=0;k<=4;k++) AH[j][k] = 0.0;
	}

	// Establish the initial condition of 0.0 at t = 0 for this run
	for(UINT j=0;j<=4;j++)
		X[j][0] = 0.0;

	UD[0] = 0.0;
	E[0] = 0.0;
		
	// iterate over time
		count = 2;
		interval = int(TS/dt);
		if(interval < 1) interval = 1;

	for(UINT n=0;n<N;n++) 
	{
	//	Find the current error value
		E[n+1] = R[n]-Y[n];
	//	Compute the system input by processing the error through the controller
		U[n+1] = P_0 * E[n+1] + I_gain * K.FractDeriv(n+1,E)*pow((I_tau/dt),K.order);

	// At this point we introduce the digital controller
		if(count == 0)	// update the digital control signal
		{
			CX.Shift();
			CY.Shift();
			CX[0] = E[n+1];
			UD[n+1] = 0.0;
			for(UINT j=0;j<10;j++) UD[n+1] += CoeffA[j]*CX[j];
			for(UINT j=1;j<10;j++) UD[n+1] -= CoeffB[j]*CY[j];
			UD[n+1] = I_gain*pow(TS/(2*I_tau),r[1])*UD[n+1] + P_0 * CX[0];
			CY[0] = UD[n+1];
			count = interval;
		}
		else
		{
			count--;
			UD[n+1] = UD[n];
		}

		if(Controller == ANALOG)
			Drive = U[n+1];
		else
			Drive = UD[n+1];

	// Precalculate the effects of history for the integral. This is the main 
	//  difference from conventional state space calculations. Each element of 
	//  the A matrix represents a contribution from one of the state histories 
	//  (bycolumn) to the integrated value of a new state (by row). Each 
	//  state is represented by its entire history, so is an array rather than 
	//  a scalar value.

		for(UINT j=1;j<=4;j++)	// by row
		{
			// First any history effects from the input (B Matrix)
			double dt_alpha = pow(B[j].tau/dt,B[j].order-1.0);

			// Insert the nonlinear friction effect here
			// if the drive signal isn't large enough, then the gear sticks.
			if(Drive < sticky) TEMP = 0.0;
			else TEMP = B[j].multiplier;

			if(B[j].order == 0.0)
				BH[j] += TEMP * Drive*dt;
			else
				BH[j] = TEMP * B[j].PartSum(n+1,U)*dt_alpha;

			// Next the history contributions from the states themselves (A Matrix)
			for(UINT k=1;k<=4;k++)	// by column
			{
				double dt_alpha = pow(A[j][k].tau/dt,A[j][k].order-1.0);
				if(A[j][k].order == 0.0)
					AH[j][k] = AH[j][k] + A[j][k].multiplier*X[k][n]*dt;
				else
					AH[j][k] = A[j][k].multiplier * A[j][k].PartSum(n+1,X[k])*dt_alpha;
			}
		}

	// Predictor step
		for(UINT j=1;j<=4;j++)	// by row
		{
			X[j][n+1] = BH[j];	// initialize for accumulation
			for(UINT k=1;k<=4;k++)	// by column
			{
				double dt_alpha = pow(A[j][k].tau/dt,A[j][k].order-1.0);
				X[j][n+1] += A[j][k].multiplier*(X[k][n]*dt_alpha) + AH[j][k];
			}
		}


	// Corrector step
		for(UINT j=1;j<=4;j++)	// by row
		{
			X[j][n+1] = BH[j];	// initialize for accumulation
			for(UINT k=1;k<=4;k++)	// by column
			{
				double dt_alpha = pow(A[j][k].tau/dt,A[j][k].order-1.0);
				X[j][n+1] += A[j][k].multiplier*(X[k][n+1]*dt_alpha) + AH[j][k];
			}
		}

		Y[n+1] = X[1][n+1] + X[2][n+1];

	// (An evaluation step would go here if there were other functions involved)


// Install the implicit integration here

	}

	SetTitle("RFJ Simulation");

	PumpMessages();

	EndTime = CTime::GetCurrentTime();
	ElapsedTime = EndTime - StartTime;
	
	pFrmWnd->m_wndStatusBar.SetPaneText(0,"Ready");
	pFrmWnd->m_wndStatusBar.SetPaneText(1,"");
	b_newData = TRUE;
	b_run = TRUE;
	b_In_Progress = FALSE;
	UpdateAllViews(NULL);
}

void CFracDoc::OnOperationsMeasure() 
{
	if(!b_run || Pulsed_On) return;

	// first check if the measurement delay and capture times 
	// are compatible
	if( (delay_index+capture_index)>N )
	{
		AfxMessageBox("Outside Limits",MB_OK);
		OnOperationsMeasurementSetUp();
	}
	if( (delay_index+capture_index)>N )
	{
		delay_index = N/2;
		capture_index = N/4;
	}
	delay = delay_index*dt;
	capture = capture_index*dt;

	// then calculate the sin and cos components
	SineFit(frequency, dt, delay_index, capture_index, X[2].history, epsilon);
	b_measured = TRUE;
	UpdateAllViews(NULL);
}


////////////////////////////////
// Event handlers

void CFracDoc::OnOperationSetup() 
{
	if(b_In_Progress) return;
	pDlg.m_Friction = sticky;
	pDlg.m_Duration = T;
	pDlg.m_I_alpha = I_alpha;
	pDlg.m_I_tau = I_tau;
	pDlg.m_I_gain = I_gain;
	pDlg.m_P_Zero = P_0;
	pDlg.m_Num_Intervals = N;
	pDlg.m_Ts = TS;
	pDlg.m_Frequency = frequency;
	pDlg.m_Fld_Amplitude = VG;
	pDlg.m_AC_ON = ac_On;
	pDlg.m_PULSED_ON = Pulsed_On;
	pDlg.m_Step_Amplitude = step_amplitude;
	pDlg.m_Step_Frequency = step_frequency;
	pDlg.m_Duty_Cycle = 100.0*P_duty_cycle;
	pDlg.m_Gated = gated;
	pDlg.m_Bipolar = bipolar;
	pDlg.m_Noise_Order = order;
	pDlg.m_Sigma = sigma;
	pDlg.m_Polar = b_Pol_noise;
	pDlg.m_E_Fld = b_Efld_noise;
	pDlg.m_ControlType = Controller;

	if(pDlg.DoModal() == IDOK)
	{
		sticky = pDlg.m_Friction;
		T = pDlg.m_Duration;
		I_alpha = pDlg.m_I_alpha;
		I_gain = pDlg.m_I_gain;
		I_tau = pDlg.m_I_tau;
		P_0 = pDlg.m_P_Zero;
		TS = pDlg.m_Ts;
		N = pDlg.m_Num_Intervals;
		frequency = pDlg.m_Frequency;
		ac_On = pDlg.m_AC_ON;
		Pulsed_On = pDlg.m_PULSED_ON;
		step_amplitude = pDlg.m_Step_Amplitude;
		step_frequency = pDlg.m_Step_Frequency;
		P_duty_cycle = pDlg.m_Duty_Cycle/100.0;
		gated = pDlg.m_Gated;
		bipolar = pDlg.m_Bipolar;
		VG = pDlg.m_Fld_Amplitude;
		omega = TwoPI*frequency;
		Period = 1.0/frequency;
		order = pDlg.m_Noise_Order;
		sigma = pDlg.m_Sigma;
		b_Pol_noise = pDlg.m_Polar;
		b_Efld_noise = pDlg.m_E_Fld;
		Controller = pDlg.m_ControlType;

// pick the appropriate random number algorithm
		if(order == 0.0) fcn = &CNoise::ran2;
		else if(order == 2.0) fcn = &CNoise::gasdev;
		else if(order == 1.0) fcn = &CNoise::cauchy;
		else fcn = &CNoise::aStable;
		prn.SetAlpha(order);

// Establish the driving fucntion
		ac_On = pDlg.m_AC_ON;
		Pulsed_On = pDlg.m_PULSED_ON;
		step_amplitude = pDlg.m_Step_Amplitude;
		step_frequency = pDlg.m_Step_Frequency;
		P_duty_cycle = 0.01*pDlg.m_Duty_Cycle;
		gated = pDlg.m_Gated && Pulsed_On && ac_On;
		bipolar = pDlg.m_Bipolar;
		P_Period = 1.0 / step_frequency;

		N = (N < 4) ? 4 : N;
		dt = T/N;
		b_run = FALSE;
		ResetSpectralData();
		UpdateAllViews(NULL);
	}
	//If.multiplier = K;
	//If.alpha = alpha1;
}

void CFracDoc::OnOperationsMeasurementSetUp()
{
	mDlg.m_Delay = delay;
	mDlg.m_Capture = capture;
	if(mDlg.DoModal() == IDOK)
	{
		delay = mDlg.m_Delay;
		delay_index = (int) (delay/dt);
		capture = mDlg.m_Capture;
		capture_index = (int) (capture/dt);
	}	
}

void CFracDoc::ComputeSpectrum()
{
	if(!b_run) return;

// calculate the number of points available between the delay
// marker and the end of the sample set.
	if( (delay_index+capture_index) > N )
	{
		AfxMessageBox("Outside Limits",MB_OK);
		OnOperationsMeasurementSetUp();
	}
	if( (delay_index+capture_index)>N )
	{
		delay_index = N/2;
		capture_index = N/4;
	}
	delay = delay_index*dt;
	capture = capture_index*dt;

	UINT NN = N - delay_index;

// Make NN a power of 2
	int m = 0;
	while(NN>1)
	{
		NN = NN>>1;
		m++;
	}
	NN = NN<<m;

	UINT M2 = NN/2;

	ComplexData.SetSize(NN+1);
	FreqArray.SetSize(NN+1);
	PSDArray1.SetSize(NN+1);
	PSDArray2.SetSize(NN+1);
	
	double ave = 0.0;
	for(UINT j = 0;j<NN; j++) ave += X[1][delay_index+j];
	ave = ave/(double) NN;
	double Wss = 0.0;
	for(UINT k=0;k<NN;k++)
	{
		double wi = 0.5f*(1.0f-cos(2.0f*PI*k/(double)NN));
		Wss += wi*wi;
		ComplexData[k] = wi*COMPLEX(X[4][delay_index+k],(X[1][delay_index+k]-ave));
	}
// compute the transform for both signals
	cfft(ComplexData,NN,1);

// uncouple the fft of the field from the polarization
	for(UINT i=1;i<=(M2-1);i++)
	{
		COMPLEX XX = 0.5*(ComplexData[i]+conj(ComplexData[NN-i]));
		COMPLEX YY = 0.5*(ComplexData[i]-conj(ComplexData[NN-i]));
		PSDArray1[i] = dt*(norm(XX))/Wss;
		PSDArray2[i] = dt*(norm(YY))/Wss;
		//CSDArray[i] = DeltaT*(X*conj(Y))/Wss;	//COMPLEX
		FreqArray[i] = (double)i/(NN*dt);
	}
	nNumSPECSamples = NN/2;		//tell the view we have stuff to plot
}

void CFracDoc::ResetSpectralData()
{
	nNumSamples = 0;
	nNumSPECSamples = 0;
	ComplexData.RemoveAll();
	PSDArray1.RemoveAll();
	PSDArray2.RemoveAll();
	FreqArray.RemoveAll();
}

void CFracDoc::OnUpdateOperationSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!b_In_Progress);	
}

void CFracDoc::OnUpdateOperationRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!b_In_Progress);
}

void CFracDoc::OnUpdateOperationsMeasurementSetUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!b_In_Progress);
}

void CFracDoc::OnUpdateOperationsMeasure(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(b_run);	
}

///////////////////////////////////////////////
// Helper Routines
///////////////////////////////////////////////

void CFracDoc::PumpMessages()
{
	MSG message;
	while(::PeekMessage(&message, NULL, 0,0, PM_REMOVE))
	{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
	while(bPauseFlag)
	{
		::GetMessage(&message, NULL, 0, 0);
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}	//stay here until a continue message is recieved
}


///////////////////////////////////////////////////

void CFracDoc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

///////////////////////////////////////////////////

char BASED_CODE CFracDoc::szTimeFilter[] = "Time Series Data (*.csv)|*.csv||";
//
double FractOp::cof[] = {76.1800917146, -86.50532032941677,
		24.01409824083091, -1.231739572450155,
		0.12086509386617e-2, -0.5395239384953e-5};
