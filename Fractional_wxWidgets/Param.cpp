// Param.cpp : implementation file
//

#include "stdafx.h"
#include "Fractional.h"
#include "Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParam dialog


CParam::CParam(CWnd* pParent /*=NULL*/)
	: CDialog(CParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParam)
	m_Duration = 2.5;
	m_Num_Intervals = 5000;
	m_Fld_Amplitude = 0.0;
	m_Frequency = 1.0;
	m_P_Zero = 1.0;
	m_I_alpha = 0.0;
	m_I_tau = 0.016;
	m_I_gain = 0.08;
	m_Ts = 0.01;
	m_AC_ON = TRUE;
	m_PULSED_ON = FALSE;
	m_Step_Amplitude = 0.0;
	m_Step_Frequency = 1.0;
	m_Gated = FALSE;
	m_E_Fld = FALSE;
	m_Polar = FALSE;
	m_Sigma = 0.0;
	m_Duty_Cycle = 0.50;
	m_Noise_Order = 1.0;
	m_Friction = 0.05;
	m_Bipolar = FALSE;
	m_ControlType = 0;

	//}}AFX_DATA_INIT
}


void CParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParam)
	DDX_Text(pDX, IDC_DURATION, m_Duration);
	DDV_MinMaxDouble(pDX, m_Duration, 1.e-006, 1000000.);
	DDX_Text(pDX, IDC_NUM_INTERVALS, m_Num_Intervals);
	DDV_MinMaxInt(pDX, m_Num_Intervals, 20, 100000);
	DDX_Text(pDX, IDC_FLD_AMPL, m_Fld_Amplitude);
	DDV_MinMaxDouble(pDX, m_Fld_Amplitude, 0., 100.);
	DDX_Text(pDX, IDC_FREQUENCY, m_Frequency);
	DDV_MinMaxDouble(pDX, m_Frequency, 1.e-006, 1000000.);
	DDX_Text(pDX, IDC_TS, m_Ts);
	DDV_MinMaxDouble(pDX, m_Ts, 1.e-006, 10.);
	DDX_Text(pDX, IDC_P_ZERO, m_P_Zero);
	DDV_MinMaxDouble(pDX, m_P_Zero, 0., 100.);
	DDX_Text(pDX, IDC_EXPONENT1, m_I_alpha);
	DDV_MinMaxDouble(pDX, m_I_alpha, -2.0, 2.0);
	DDX_Text(pDX, IDC_EXPONENT2, m_I_gain);
	DDV_MinMaxDouble(pDX, m_I_gain, 0.0, 200.0);
	DDX_Text(pDX, IDC_EXPONENT3, m_I_tau);
	DDV_MinMaxDouble(pDX, m_I_tau, 0.0001, 2.0);
	DDX_Check(pDX, IDC_AC_ON, m_AC_ON);
	DDX_Check(pDX, IDC_PULSED_ON, m_PULSED_ON);
	DDX_Text(pDX, IDC_STEP_AMPL, m_Step_Amplitude);
	DDV_MinMaxDouble(pDX, m_Step_Amplitude, 0., 100000.);
	DDX_Text(pDX, IDC_STEP_RATE, m_Step_Frequency);
	DDV_MinMaxDouble(pDX, m_Step_Frequency, 1.e-006, 10000000.);
	DDX_Check(pDX, IDC_GATED, m_Gated);
	DDX_Check(pDX, IDC_E_FLD, m_E_Fld);
	DDX_Check(pDX, IDC_POLAR, m_Polar);
	DDX_Text(pDX, IDC_SIGMA, m_Sigma);
	DDV_MinMaxDouble(pDX, m_Sigma, 0., 100.);
	DDX_Text(pDX, IDC_DUTY_CYCLE, m_Duty_Cycle);
	DDV_MinMaxDouble(pDX, m_Duty_Cycle, 1.e-002, 99.9);
	DDX_Text(pDX, IDC_ORDER, m_Noise_Order);
	DDV_MinMaxDouble(pDX, m_Noise_Order, 0., 2.);
	DDX_Text(pDX, IDC_FRICTION, m_Friction);
	DDV_MinMaxDouble(pDX, m_Friction, 0.0, 10.0);
	DDX_Check(pDX, IDC_BIPOLAR, m_Bipolar);
	DDX_Radio(pDX, IDC_CONTROL, m_ControlType);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParam, CDialog)
	//{{AFX_MSG_MAP(CParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParam message handlers
