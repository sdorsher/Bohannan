#if !defined(AFX_PARAM_H__4CE2D154_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
#define AFX_PARAM_H__4CE2D154_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Param.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParam dialog

class CParam : public CDialog
{
// Construction
public:
	CParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParam)
	enum { IDD = IDD_PARAMETERS };
	double	m_Duration;
	int		m_Num_Intervals;
	double	m_Fld_Amplitude;
	double	m_Frequency;
	double	m_P_Zero;
	double	m_I_alpha;
	double  m_I_tau;
	double	m_I_gain;
	double	m_Ts;
	BOOL	m_AC_ON;
	BOOL	m_PULSED_ON;
	double	m_Step_Amplitude;
	double	m_Step_Frequency;
	BOOL	m_Gated;
	BOOL	m_E_Fld;
	BOOL	m_Polar;
	double	m_Sigma;
	double	m_Duty_Cycle;
	double	m_Noise_Order;
	BOOL	m_Bipolar;
	double  m_Friction;
	int		m_ControlType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParam)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeFriction();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAM_H__4CE2D154_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
