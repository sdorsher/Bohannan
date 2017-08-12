// FracView.cpp : implementation of the CFracView class
//
// Part of the validation of the fractional order state
//  space model. This simulates the Quanser RFJ arm
//
//
#include "stdafx.h"
#include "Fractional.h"

#include "FracDoc.h"
#include "FracView.h"
#include "PageSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFracView

IMPLEMENT_DYNCREATE(CFracView, CScrollView)

BEGIN_MESSAGE_MAP(CFracView, CScrollView)
	//{{AFX_MSG_MAP(CFracView)
	ON_COMMAND(ID_VIEW_PAGE, OnViewPage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PAGE, OnUpdateViewPage)

	ON_COMMAND(ID_POWERSPECTRUM, OnPowerspectrum)
	ON_UPDATE_COMMAND_UI(ID_POWERSPECTRUM, OnUpdatePowerspectrum)

	ON_COMMAND(ID_FREQUENCYVIEW, OnFrequencyview)
	ON_UPDATE_COMMAND_UI(ID_FREQUENCYVIEW, OnUpdateFrequencyview)

	ON_COMMAND(ID_HYSTERESISVIEW, OnHysteresisview)
	ON_UPDATE_COMMAND_UI(ID_HYSTERESISVIEW, OnUpdateHysteresisview)

	ON_COMMAND(ID_TIMEVIEW, OnTimeview)
	ON_UPDATE_COMMAND_UI(ID_TIMEVIEW, OnUpdateTimeview)

	ON_COMMAND(ID_LEFT, OnLeft)
	ON_UPDATE_COMMAND_UI(ID_LEFT, OnUpdateLeft)
	ON_COMMAND(ID_RIGHT, OnRight)
	ON_UPDATE_COMMAND_UI(ID_RIGHT, OnUpdateRight)
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomout)
	ON_COMMAND(ID_ZOOMIN, OnZoomin)
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomin)
	ON_COMMAND(ID_NEXT_PANE, OnNextPane)
	ON_UPDATE_COMMAND_UI(ID_NEXT_PANE, OnUpdateNextPane)
	ON_COMMAND(ID_PREV_PANE, OnPrevPane)
	ON_UPDATE_COMMAND_UI(ID_PREV_PANE, OnUpdatePrevPane)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFracView construction/destruction

CFracView::CFracView()
{
	m_Print_y_Font.CreateFont(180, 80, 2700, 0, FW_NORMAL,
		0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH | TMPF_TRUETYPE | FF_ROMAN,
		"Times New Roman");
	m_Screen_y_Font.CreateFont(180, 80, 2700, 0, FW_NORMAL,
		0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH | TMPF_TRUETYPE | FF_ROMAN,
		"Times New Roman");
	m_xTitleFont.CreateFont(180, 80, 0, 0, 0, 0, 0, 0, 0, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		VARIABLE_PITCH | FF_ROMAN | TMPF_TRUETYPE, "Times New Roman");
	m_CommentFont.CreateFont(140, 65, 0, 0, 0, 1, 0, 0, 0, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		VARIABLE_PITCH | FF_SWISS | TMPF_TRUETYPE, "Swiss Italic");
	m_InfoFont.CreateFont(180, 80, 0, 0, 0, 0, 0, 0, 0, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		VARIABLE_PITCH | FF_ROMAN | TMPF_TRUETYPE, "Times New Roman");
	
	m_PageNo = 1;
	m_PSD_No = 1;
	m_Loop_No = 0;
	m_PlotType = TIME;
}

CFracView::~CFracView()
{
}

BOOL CFracView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
//

void CFracView::OnInitialUpdate()
{
	CFracDoc* pDoc = GetDocument();	
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_HIENGLISH, GetDocument()->GetDocSize());
	time_start = 0;
	time_frame = pDoc->N;
}

/////////////////////////////////////////////////////////////////////////////
// CFracView drawing

void CFracView::OnDraw(CDC* pDC)
{
	CFracDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_BlackPen.CreatePen(PS_SOLID,20,RGB(0,0,0));
	p_OldPen = pDC->SelectObject(&m_BlackPen);
	m_RedPen.CreatePen(PS_SOLID,20,RGB(255,0,0));
	
	p_OldFont = pDC->SelectObject(&m_InfoFont);
	sz.Format("Version: %s", pDoc->Version);
	pDC->TextOut(COL3+750, LINE1, sz);

	sz.Format("Duration: %g", pDoc->T);
	pDC->TextOut(COL1, LINE2, sz);
	sz.Format("Time Step: %g", pDoc->dt);
	pDC->TextOut(COL2, LINE2, sz);

	if(pDoc->Controller == 0)
		sz.Format("Type = Analog");
	else
		sz.Format("Type = Digital");
	pDC->TextOut(COL2, LINE6, sz);

	sz.Format("# Intervals: %d",pDoc->N);
	pDC->TextOut(COL3+500, LINE2, sz);

	sz.Format("Input");
	pDC->TextOut(COL3, LINE3, sz);
	if(pDoc->ac_On) {
		sz.Format("ac Ampl = %g",pDoc->VG);
		pDC->TextOut(COL3, LINE4, sz);
		sz.Format("ac Freq = %g",pDoc->frequency);
		pDC->TextOut(COL3, LINE5, sz);
	}

	else if(pDoc->Pulsed_On) {
		sz.Format("step Ampl = %g",pDoc->step_amplitude);
		pDC->TextOut(COL3, LINE4, sz);
		sz.Format("step Freq = %g",pDoc->step_frequency);
		pDC->TextOut(COL3, LINE5, sz);
		sz.Format("interval = %d",pDoc->interval);
		pDC->TextOut(COL3, LINE6, sz);
	}

	sz.Format("Control");
	pDC->TextOut(COL1,LINE3, sz);
	sz.Format("I alpha = %g",pDoc->I_alpha);
	pDC->TextOut(COL1, LINE4, sz);
	sz.Format("I gain = %g",pDoc->I_gain);
	pDC->TextOut(COL1, LINE5, sz);
	sz.Format("I_tau = %g",(pDoc->I_tau));
	pDC->TextOut(COL1, LINE6, sz);

	sz.Format("Coefficient");
	pDC->TextOut(COL2, LINE3, sz);
	sz.Format("P_0 = %g",(pDoc->P_0));
	pDC->TextOut(COL2, LINE4, sz);
	sz.Format("Friction = %g",(pDoc->sticky));
	pDC->TextOut(COL2, LINE5, sz);

	sz.Format("Noise");
	pDC->TextOut(COL4,LINE3, sz);
	if(pDoc->b_Efld_noise) sz.Format("Type = Field");
	else if(pDoc->b_Pol_noise) sz.Format("Type = Polarization");
	else sz.Format("Type = None");
	pDC->TextOut(COL4, LINE4, sz);
	sz.Format("order = %g",pDoc->order);
	pDC->TextOut(COL4, LINE5, sz);
	sz.Format("sigma = %g",pDoc->sigma);
	pDC->TextOut(COL4, LINE6, sz);

//	sz.Format("Measurement delay = %g  capture = %g",pDoc->delay, pDoc->capture);
//	pDC->TextOut(COL2+250, LINE8, sz);
//	if(pDoc->b_measured)
//	{
//		sz.Format("epsilon[0] = %g  epsilon[1] = %g",pDoc->epsilon[0], pDoc->epsilon[1]);
//		pDC->TextOut(COL2+250, LINE9, sz);
//	}

	if(m_PlotType == TIME) DrawFig1(pDC, pDoc);
	else if(m_PlotType == FREQUENCY) DrawFig2(pDC, pDoc);
	else DrawFig3(pDC, pDoc);	// HYSTERSIS

	pDC->SelectObject(p_OldPen);		//restore the old pen
	m_BlackPen.DeleteObject();
	m_RedPen.DeleteObject();
	pDC->SelectObject(p_OldFont);
}

void CFracView::DrawFig1(CDC* pDC, CFracDoc* pDoc)
{
	int		x_pt, y_pt;

	y.RemoveAll();
	Label.Format("Time Domain");	
	LabelX.Format("Time [sec]");

	switch (m_PageNo) {
	case 0:
		LabelY.Format("R");	
		y.Copy(pDoc->R.history);		// Use the input array
		break;
	case 1:
		LabelY.Format("Theta");
		y.Copy(pDoc->X[1].history);		// Use the output array
		break;
	case 2:
		LabelY.Format("Phi");
		y.Copy(pDoc->X[2].history);		// Use the Current array
		break;
	case 3:
		LabelY.Format("Theta Dot");
		y.Copy(pDoc->X[3].history);		// Use the Integral array
		break;
	case 4:
		LabelY.Format("Phi Dot");
		y.Copy(pDoc->X[4].history);		// Use the integrated charge array
		break;
	case 5:
		LabelY.Format("Total Angle (Y)");
		y.Copy(pDoc->Y.history);		// Use the integrated charge array
		break;
	case 6:
		LabelY.Format("Drive (U)");
		y.Copy(pDoc->X[0].history);		// Use the integrated charge array
		break;
	case 7:
		LabelY.Format("Error (E)");
		y.Copy(pDoc->E.history);		// Use the integrated charge array
		break;
	default:
		LabelY.Format("Input");
		y.Copy(pDoc->R.history);		// Use the input array
	}

	pDC->TextOut(COL2, LINE1, Label);
	if(!pDoc->b_run) return;

	sz = pDoc->ElapsedTime.Format("Elapsed Time");
	pDC->TextOut(COL1, LINE8, sz);
	sz = pDoc->ElapsedTime.Format(" %H:%M:%S");
	pDC->TextOut(COL1, LINE9, sz);
	CTime t = CTime::GetCurrentTime();
	sz = pDoc->ElapsedTime.Format("Current Time");
	pDC->TextOut(COL2, LINE8, sz);
	sz = t.Format("  %B %d, %Y, %H:%M:%S");
	pDC->TextOut(COL2, LINE9, sz);

	if(pDoc->b_measured) {
		sz.Format("Measured eps' = %g eps'' = %g",pDoc->epsilon[0],pDoc->epsilon[1]);
		pDC->TextOut(COL1, LINE7, sz);
	}

// find the section to plot
	if(pDoc->b_newData)
	{
		time_frame = pDoc->N;
		time_start = 0;
		time_end = pDoc->N;
		pDoc->b_newData = FALSE;
	}
	else
	{
		time_frame = IMIN(time_frame,pDoc->N);
		time_start = IMAX(IMIN(time_start,pDoc->N-time_frame),0);
		time_end = IMIN(time_start+time_frame,pDoc->N);
	}

//scale the y axis
	v_max=y[time_start];
	v_min=v_max;
	for(UINT i=time_start; i<=time_end; i++)
	{
		if(y[i]>v_max) v_max=y[i];
		if(y[i]<v_min) v_min=y[i];
	}
	if(v_max==v_min) v_max += 5.0e-5;
	plot_scale(v_min, v_max, v_lower, v_upper, v_tic_inc);
	v_range = v_upper - v_lower;

//scale the time axis
	t_max = ((double)time_end)*pDoc->dt;
	t_min = ((double)time_start)*pDoc->dt;
	plot_scale(t_min, t_max, t_lower, t_upper, t_tic_inc);
	t_range = t_upper - t_lower;

//Draw the axis as determined by scaling (in black)
	plot_t_axis(pDC);
	plot_v_axis(pDC);	

//Plot the time function to go over the axes and grid
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_RedPen);

	y_pt = (int)(((y[time_start]-v_lower)/v_range)*YRANGE)+YMIN;
	x_pt = (int)(((t_min-t_lower)/t_range)*XRANGE)+XMIN;
	pDC->MoveTo(x_pt,y_pt);
	for (UINT i=time_start; i<=time_end; i++)
	{
		double timemk = (double)(i)*(pDoc->dt);
		y_pt = (int)(((y[i]-v_lower)/v_range)*YRANGE)+YMIN;
		x_pt = (int)(((timemk-t_lower)/t_range)*XRANGE)+XMIN;
		pDC->LineTo(x_pt,y_pt);
	}
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_BlackPen);

//Now draw the title and labels
	pDC->SelectObject(&m_xTitleFont);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOut(xXtitle, yXtitle , LabelX, LabelX.GetLength());
	
	if(pDC->IsPrinting()) pDC->SelectObject(&m_Print_y_Font);
	else pDC->SelectObject(&m_Screen_y_Font);

	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
	pDC->TextOut(xYtitle, yYtitle, LabelY, LabelY.GetLength());
}

void CFracView::DrawFig2(CDC* pDC, CFracDoc* pDoc)
{
	int		x_pt, y_pt;
	Label.Format("Power Spectrum");
	LabelX.Format("Frequency");
	
	switch (m_PSD_No) {
	case 0:
		LabelY.Format("Hub");
		break;
	case 1:
		LabelY.Format("Arm");
		break;
	default:
		LabelY.Format("Total");
	}

	pDC->TextOut(COL2, LINE1, Label);
	if(!pDoc->b_run) return;

	sz = pDoc->ElapsedTime.Format("Elapsed Time");
	pDC->TextOut(COL1, LINE8, sz);
	sz = pDoc->ElapsedTime.Format("  %H:%M:%S");
	pDC->TextOut(COL1, LINE9, sz);

	if(pDoc->b_measured) {
		sz.Format("Measured eps' = %g eps'' = %g",pDoc->epsilon[0],pDoc->epsilon[1]);
		pDC->TextOut(COL2+250, LINE9, sz);
	}

//otherwise see if the spectrum has been computed
// If not, tell the document to compute it.

	if(pDoc->nNumSPECSamples <= 0) pDoc->ComputeSpectrum();

	UINT M = pDoc->nNumSPECSamples-1;
	//PSDArray1 is the E field
	//PSDArray2 is the Polarization
	PSDArray.RemoveAll();
	if (m_PSD_No == 0) PSDArray.Copy(pDoc->PSDArray2);
	else PSDArray.Copy(pDoc->PSDArray1);

	v_max=PSDArray[1];
	v_min=v_max;
	for(UINT i=1; i<=M; i++)
	{
		if(PSDArray[i]>v_max) v_max=PSDArray[i];
		if(PSDArray[i]<v_min) v_min=PSDArray[i];
	}

	v_max = 10.0*log10(v_max);	//convert to dB
	v_min = 10.0*log10(v_min);

	plot_scale(v_min, v_max, v_lower, v_upper, v_tic_inc);
	v_range = v_upper - v_lower;

	t_max = pDoc->FreqArray[M];
	t_min = pDoc->FreqArray[1];

	t_max = log10(t_max);	//convert to log scale
	t_min = log10(t_min);

	plot_scale(t_min, t_max, t_lower, t_upper, t_tic_inc);
	t_range = t_upper - t_lower;
	
//Establish the clipping region
	// 
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_RedPen);
	y_pt = (int)(((10.0*log10(PSDArray[1])-v_lower)/v_range)*YRANGE)+YMIN;
	x_pt = (int)(((log10(pDoc->FreqArray[1])-t_lower)/t_range)*XRANGE)+XMIN;
	pDC->MoveTo(x_pt,y_pt);
	for (UINT i=2; i<=M; i++)
	{
		y_pt = (int)(((10.0*log10(PSDArray[i])-v_lower)/v_range)*YRANGE)+YMIN;
		x_pt = (int)(((log10(pDoc->FreqArray[i])-t_lower)/t_range)*XRANGE)+XMIN;
		if(x_pt > XMAX) break;
		pDC->LineTo(x_pt,y_pt);
	}
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_BlackPen);

	//Draw the axis as determined by scaling (in black)
	pDC->SelectObject(&m_InfoFont);
	plot_t_axis(pDC);
	plot_v_axis(pDC);	

	pDC->SelectObject(&m_xTitleFont);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	LabelX.Format("log frequency");
	pDC->TextOut(xXtitle, yXtitle, LabelX, LabelX.GetLength());
	
	if(pDC->IsPrinting()) pDC->SelectObject(&m_Print_y_Font);
	else pDC->SelectObject(&m_Screen_y_Font);
	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
	LabelY.Format("relative power (dB)");
	pDC->TextOut(xYtitle, yYtitle, LabelY, LabelY.GetLength());

}

void CFracView::DrawFig3(CDC* pDC, CFracDoc* pDoc)
{
	int		x_pt, y_pt;

	x.RemoveAll();
	y.RemoveAll();

	switch (m_Loop_No) {
	case 0:
		y.Copy(pDoc->X[2].history);		// Use the I array
		x.Copy(pDoc->X[1].history);		// Use the voltage array
		Label.Format("Hysteresis: VF vs I");
		LabelY.Format("I");
		LabelX.Format("VF");
		break;
	case 1:
		y.Copy(pDoc->X[4].history);		// Use the Q array
		x.Copy(pDoc->X[1].history);		// Use the voltage array
		Label.Format("Hysteresis: VF vs Q");
		LabelY.Format("Q");
		LabelX.Format("VF");
		break;
	case 2:
		y.Copy(pDoc->X[4].history);		// Use the Q array
		x.Copy(pDoc->X[3].history);		// Use the flux array
		Label.Format("Hysteresis: Q vs Flux");
		LabelY.Format("Q");
		LabelX.Format("Flux");
		break;
	default:
		y.Copy(pDoc->X[2].history);		// Use the I array
		x.Copy(pDoc->X[1].history);		// Use the voltage array
		Label.Format("Hysteresis: VF vs I");
		LabelY.Format("I");
		LabelX.Format("VF");
	}

	pDC->TextOut(COL2, LINE1, Label);
	if(!pDoc->b_run) return;

	sz = pDoc->ElapsedTime.Format("Elapsed Time");
	pDC->TextOut(COL1, LINE8, sz);
	sz = pDoc->ElapsedTime.Format(" %H:%M:%S");
	pDC->TextOut(COL1, LINE9, sz);
	CTime t = CTime::GetCurrentTime();
	sz = pDoc->ElapsedTime.Format("Current Time");
	pDC->TextOut(COL2, LINE8, sz);
	sz = t.Format("  %B %d, %Y, %H:%M:%S");
	pDC->TextOut(COL2, LINE9, sz);

//	if(pDoc->b_measured) {
//		sz.Format("Measured eps' = %g eps'' = %g",pDoc->epsilon[0],pDoc->epsilon[1]);
//		pDC->TextOut(COL2+250, LINE9, sz);
//	}

// find the section to plot
	if(pDoc->b_newData)
	{
		time_frame = pDoc->N;
		time_start = 0;
		time_end = pDoc->N;
		pDoc->b_newData = FALSE;
	}
	else
	{
		time_frame = IMIN(time_frame,pDoc->N);
		time_start = IMAX(IMIN(time_start,pDoc->N-time_frame),0);
		time_end = IMIN(time_start+time_frame,pDoc->N);
	}

//scale the VF axis
	v_max=y[time_start];
	v_min=v_max;
	for(UINT i=time_start; i<=time_end; i++)
	{
		if(y[i]>v_max) v_max=y[i];
		if(y[i]<v_min) v_min=y[i];
	}
	if(v_max==v_min) v_max += 5.0e-5;
	plot_scale(v_min, v_max, v_lower, v_upper, v_tic_inc);
	v_range = v_upper - v_lower;

//scale the current axis
	x_max=x[time_start];
	x_min=x_max;
	for(UINT i=time_start; i<=time_end; i++)
	{
		if(x[i]>x_max) x_max=x[i];
		if(x[i]<x_min) x_min=x[i];
	}
	if(x_max==x_min) x_max += 5.0e-5;
	plot_scale(x_min, x_max, x_lower, x_upper, x_tic_inc);
	x_range = x_upper - x_lower;

//Draw the axis as determined by scaling (in black)
	plot_v_axis(pDC);
	plot_x_axis(pDC);	

//Plot the time function to go over the axes and grid
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_RedPen);

	y_pt = (int)(((y[time_start]-v_lower)/v_range)*YRANGE)+YMIN;
	x_pt = (int)(((x[time_start]-x_lower)/x_range)*XRANGE)+XMIN;
	pDC->MoveTo(x_pt,y_pt);
	for (UINT i=time_start; i<=time_end; i++)
	{
		//double timemk = (double)(i)*(pDoc->dt);
		y_pt = (int)(((y[i]-v_lower)/v_range)*YRANGE)+YMIN;
		x_pt = (int)(((x[i]-x_lower)/x_range)*XRANGE)+XMIN;
		pDC->LineTo(x_pt,y_pt);
	}
	if(!pDC->IsPrinting()) pDC->SelectObject(&m_BlackPen);

//Now draw the title and labels
	pDC->SelectObject(&m_xTitleFont);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOut(xXtitle, yXtitle , LabelX, LabelX.GetLength());
	
	if(pDC->IsPrinting()) pDC->SelectObject(&m_Print_y_Font);
	else pDC->SelectObject(&m_Screen_y_Font);

	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
	pDC->TextOut(xYtitle, yYtitle, LabelY, LabelY.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
// CFracView printing

BOOL CFracView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFracView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFracView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFracView diagnostics

#ifdef _DEBUG
void CFracView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFracView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFracDoc* CFracView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFracDoc)));
	return (CFracDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFracView message handlers

void CFracView::OnRight()
{
	CFracDoc* pDoc = GetDocument();
	if(pDoc->b_run)
	{
		time_start = IMIN((time_start+time_frame),pDoc->N-time_frame);
		time_start = IMAX(time_start,0);
		RedrawWindow();
	}
}

void CFracView::OnLeft()
{
	CFracDoc* pDoc = GetDocument();
	if(pDoc->b_run)
	{
		time_start = IMAX((time_start-time_frame),0);
		RedrawWindow();
	}
}

void CFracView::OnViewPage() 
{
	CPageSelect pDlg;
	pDlg.m_PageNo = m_PageNo;
	pDlg.m_PSD_No = m_PSD_No;
	pDlg.m_Loop_No = m_Loop_No;
	if(pDlg.DoModal() == IDOK)
	{
		m_PageNo = pDlg.m_PageNo;
		m_PSD_No = pDlg.m_PSD_No;
		m_Loop_No = pDlg.m_Loop_No;
		RedrawWindow();
	}
}

void CFracView::OnUpdateViewPage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CFracView::OnFrequencyview() 
{
	m_PlotType = FREQUENCY;
	RedrawWindow();
}

void CFracView::OnUpdateFrequencyview(CCmdUI* pCmdUI) 
{
	CFracDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->b_run);
}

void CFracView::OnHysteresisview() 
{
	m_PlotType = HYSTERESIS;
	RedrawWindow();
}

void CFracView::OnUpdateHysteresisview(CCmdUI* pCmdUI) 
{
	CFracDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->b_run);
}

void CFracView::OnTimeview() 
{
	m_PlotType = TIME;
	RedrawWindow();
}

void CFracView::OnUpdateTimeview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CFracView::OnPowerspectrum() 
{
	CFracDoc* pDoc = GetDocument();
	m_PSD_No = 0;
	m_PlotType = FREQUENCY;
	pDoc->ComputeSpectrum();
	RedrawWindow();
}

void CFracView::OnUpdatePowerspectrum(CCmdUI* pCmdUI) 
{
	CFracDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->b_run);
}


void CFracView::OnZoomout() 
{
	CFracDoc* pDoc = GetDocument();
	time_frame = IMIN(time_frame*2,pDoc->N);
	RedrawWindow();
}

void CFracView::OnZoomin() 
{
	time_frame = IMAX(time_frame/2,8);
	RedrawWindow();
}

void CFracView::OnUpdateZoomout(CCmdUI* pCmdUI) 
{
	CFracDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->b_run);
}

void CFracView::OnUpdateZoomin(CCmdUI* pCmdUI) 
{
	CFracDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->b_run);
}

void CFracView::OnUpdateLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CFracView::OnUpdateRight(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}


void CFracView::OnNextPane() 
{
	if(m_PlotType == TIME) m_PageNo = (m_PageNo+1)%6;
	else m_PSD_No = (m_PSD_No+1)%2;
	RedrawWindow();
}

void CFracView::OnUpdateNextPane(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CFracView::OnPrevPane() 
{
	if(m_PlotType == TIME) m_PageNo = (m_PageNo+7)%6;
	else m_PSD_No = (m_PSD_No+3)%2;
	RedrawWindow();
}

void CFracView::OnUpdatePrevPane(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////
// Plotting routines
///////////////////////////////////////////////////////

//Plot the t (horizontal) axis
void CFracView::plot_t_axis(CDC* pDC)
{
	CPen	AxisPen;
	CPen	GridPen;
	CPen*	pOldPen;
	double t_tic;
	int	t_pt;
	AxisPen.CreatePen(PS_SOLID,20,RGB(0,0,0));
	GridPen.CreatePen(PS_SOLID,1,RGB(0,0,0));	
	pOldPen = pDC->SelectObject(&AxisPen);
	pDC->MoveTo(XMIN,YMIN);
	pDC->LineTo(XMAX,YMIN);
	pDC->SelectObject(&GridPen);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	
	for(t_tic = t_lower;t_tic<=(t_upper+0.01*t_range); t_tic += t_tic_inc)
	{
	 sz.Format("%3g",t_tic);
	 t_pt = (int) (((t_tic-t_lower)/t_range)*XRANGE + XMIN);
	 pDC->MoveTo(t_pt,YMIN-20);
	 pDC->LineTo(t_pt,YMAX);
	 pDC->TextOut(t_pt,YMIN-25, sz, sz.GetLength());
	 }        
	 pDC->SelectObject(pOldPen);
	 AxisPen.DeleteObject();
	 GridPen.DeleteObject();
}  


//Plot the x (horizontal) axis
void CFracView::plot_x_axis(CDC* pDC)
{
	CPen	AxisPen;
	CPen	GridPen;
	CPen*	pOldPen;
	double x_tic;
	int	x_pt;
	AxisPen.CreatePen(PS_SOLID,20,RGB(0,0,0));
	GridPen.CreatePen(PS_SOLID,1,RGB(0,0,0));	
	pOldPen = pDC->SelectObject(&AxisPen);
	pDC->MoveTo(XMIN,YMIN);
	pDC->LineTo(XMAX,YMIN);
	pDC->SelectObject(&GridPen);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	
	for(x_tic = x_lower;x_tic<=(x_upper+0.01*x_range); x_tic += x_tic_inc)
	{
	 sz.Format("%.9g",(fabs(x_tic)>1.0e-5*x_range) ? (x_tic) : (0.0));
	 x_pt = (int) (((x_tic-x_lower)/x_range)*XRANGE + XMIN);
	 pDC->MoveTo(x_pt,YMIN-20);
	 pDC->LineTo(x_pt,YMAX);
	 pDC->TextOut(x_pt,YMIN-25, sz, sz.GetLength());
	 }        
	 pDC->SelectObject(pOldPen);
	 AxisPen.DeleteObject();
	 GridPen.DeleteObject();
}  

//Plot the v (vertical) axis
void CFracView::plot_v_axis(CDC* pDC)
{
	CPen	AxisPen;
	CPen	GridPen;
	CPen*	pOldPen;
	double v_tic;
	int	v_pt;
	AxisPen.CreatePen(PS_SOLID,20,RGB(0,0,0));
	GridPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	pOldPen = pDC->SelectObject(&AxisPen);	
	pDC->MoveTo(XMIN,YMIN);
	pDC->LineTo(XMIN,YMAX);
	pDC->SelectObject(&GridPen);
	pDC->SetTextAlign(TA_RIGHT | TA_BASELINE);
	
	for(v_tic = v_lower;v_tic<=(v_upper+0.01*v_range); v_tic += v_tic_inc)
	{
	 sz.Format("%.9g",(fabs(v_tic)>1.0e-5*v_range) ? (v_tic) : (0.0));
	 v_pt = (int) (((v_tic-v_lower)/v_range)*YRANGE + YMIN);
	 pDC->MoveTo(XMIN-20, v_pt);
	 pDC->LineTo(XMAX, v_pt);
	 pDC->TextOut(XMIN-35, v_pt, sz, sz.GetLength());
	 }
	 pDC->SelectObject(pOldPen);
	 AxisPen.DeleteObject();
	 GridPen.DeleteObject();
}      
