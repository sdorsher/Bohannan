#if !defined(AFX_MEASUREMENT_H__4DB8A2C0_03EF_11D4_8019_00105A9399D0__INCLUDED_)
#define AFX_MEASUREMENT_H__4DB8A2C0_03EF_11D4_8019_00105A9399D0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Measurement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeasurement dialog

class CMeasurement : public CDialog
{
// Construction
public:
	CMeasurement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasurement)
	enum { IDD = IDD_MEASUREMENT };
	double	m_Delay;
	double	m_Capture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasurement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasurement)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREMENT_H__4DB8A2C0_03EF_11D4_8019_00105A9399D0__INCLUDED_)
