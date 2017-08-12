// Measurement.cpp : implementation file
//

#include "stdafx.h"
#include "Fractional.h"
#include "Measurement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasurement dialog


CMeasurement::CMeasurement(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasurement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasurement)
	m_Delay = 0.0;
	m_Capture = 0.0;
	//}}AFX_DATA_INIT
}


void CMeasurement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurement)
	DDX_Text(pDX, IDC_DELAY, m_Delay);
	DDV_MinMaxDouble(pDX, m_Delay, 0., 1000000.);
	DDX_Text(pDX, IDC_CAPTURE, m_Capture);
	DDV_MinMaxDouble(pDX, m_Capture, 0., 1000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasurement, CDialog)
	//{{AFX_MSG_MAP(CMeasurement)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasurement message handlers
