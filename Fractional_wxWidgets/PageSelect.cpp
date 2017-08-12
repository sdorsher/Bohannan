// PageSelect.cpp : implementation file
//

#include "stdafx.h"
#include "Fractional.h"
#include "PageSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelect dialog


CPageSelect::CPageSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelect)
	m_PageNo = 0;
	m_PSD_No = 0;
	m_Loop_No = 0;
	//}}AFX_DATA_INIT
}


void CPageSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelect)
	DDX_Radio(pDX, IDC_PAGE, m_PageNo);
	DDX_Radio(pDX, IDC_SPECTRUM, m_PSD_No);
	DDX_Radio(pDX, IDC_HYSTERESIS, m_Loop_No);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelect, CDialog)
	//{{AFX_MSG_MAP(CPageSelect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelect message handlers
