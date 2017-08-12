// Part of the validation of the fractional order state
//  space model using the numerical technique of El-Mesiry,
//  etal. 
//
// FracView.h : interface of the CFracView class
//
// Part of the validation of the fractional order state
//  space model. This simulates the Quanser RFJ arm
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRACVIEW_H__4CE2D14C_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
#define AFX_FRACVIEW_H__4CE2D14C_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "math.h"
#include "nrutil.h"
#include "Param.h"

#define LINE1 -200
#define LINE2 -400
#define LINE3 -600
#define LINE4 -800
#define LINE5 -1000
#define LINE6 -1200
#define LINE7 -1400
#define LINE8 -1600
#define LINE9 -1800
#define LINE10 -2000
#define COL1   970
#define COL2  2550
#define COL3  4150
#define COL4  5900
#define XMAX  7350
#define XMIN  1350
#define XRANGE  6000
#define YMAX  -2500
#define YMIN  -6500
#define YRANGE  4000
#define xXtitle 4350
#define yXtitle -6800
#define xYtitle 850
#define yYtitle -4500


class CFracView : public CScrollView
{
protected: // create from serialization only
	CFracView();
	DECLARE_DYNCREATE(CFracView)

// Attributes
public:
	CFracDoc* GetDocument();
	CString sz;
	CString Label;
	CString LabelX;
	CString LabelY;

	CPen*	p_OldPen;
	CPen	m_RedPen;
	CPen	m_BlackPen;
	CFont	m_Screen_y_Font;
	CFont	m_Print_y_Font;
	CFont	m_xTitleFont;
	CFont	m_CommentFont;
	CFont	m_InfoFont;
	CFont*	p_OldFont;
	int		m_PageNo;
	int		m_PSD_No;
	int		m_Loop_No;
	enum	{TIME,FREQUENCY,HYSTERESIS} m_PlotType;
	
	CArray<double, double&>	x;
	CArray<double, double&>	y;
	CArray<double, double&> PSDArray;
	double	x_max;
	double	x_min;
	double	x_upper;
	double	x_lower;
	double	x_range;
	double	x_tic_inc;	
	double	v_max;
	double	v_min;
	double	v_upper;
	double	v_lower;
	double	v_range;
	double	v_tic_inc;
	double	t_max;
	double	t_min;
	double	t_upper;
	double	t_lower;
	double	t_range;
	double	t_tic_inc;	
	
// for zooming into the plots
	UINT	time_start;
	UINT	time_end;
	UINT	time_frame;

// Operations
public:
	void DrawFig1(CDC* pDC, CFracDoc* pDoc);
	void DrawFig2(CDC* pDC, CFracDoc* pDoc);
	void DrawFig3(CDC* pDC, CFracDoc* pDoc);
	void plot_t_axis(CDC* pDC);
	void plot_x_axis(CDC* pDC);
	void plot_v_axis(CDC* pDC);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFracView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFracView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFracView)
	afx_msg void OnViewPage();
	afx_msg void OnUpdateViewPage(CCmdUI* pCmdUI);
	afx_msg void OnPowerspectrum();
	afx_msg void OnUpdatePowerspectrum(CCmdUI* pCmdUI);
	afx_msg void OnFrequencyview();
	afx_msg void OnUpdateFrequencyview(CCmdUI* pCmdUI);

	afx_msg void OnHysteresisview();
	afx_msg void OnUpdateHysteresisview(CCmdUI* pCmdUI);

	afx_msg void OnTimeview();
	afx_msg void OnUpdateTimeview(CCmdUI* pCmdUI);
	afx_msg void OnLeft();
	afx_msg void OnUpdateLeft(CCmdUI* pCmdUI);
	afx_msg void OnRight();
	afx_msg void OnUpdateRight(CCmdUI* pCmdUI);
	afx_msg void OnZoomout();
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
	afx_msg void OnZoomin();
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnNextPane();
	afx_msg void OnUpdateNextPane(CCmdUI* pCmdUI);
	afx_msg void OnPrevPane();
	afx_msg void OnUpdatePrevPane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FracView.cpp
inline CFracDoc* CFracView::GetDocument()
   { return (CFracDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRACVIEW_H__4CE2D14C_F9E0_11D3_99CA_DFBA240B2162__INCLUDED_)
