; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CParam
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Fractional.h"
LastPage=0

ClassCount=8
Class1=CFracApp
Class2=CFracDoc
Class3=CFracView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_PARAMETERS
Resource2=IDD_PAGE_SELECT
Class5=CAboutDlg
Class6=CParam
Resource3=IDR_MAINFRAME
Class7=CPageSelect
Resource4=IDD_ABOUTBOX
Class8=CMeasurement
Resource5=IDD_MEASUREMENT

[CLS:CFracApp]
Type=0
HeaderFile=Fractional.h
ImplementationFile=Fractional.cpp
Filter=N
LastObject=CFracApp

[CLS:CFracDoc]
Type=0
HeaderFile=FracDoc.h
ImplementationFile=FracDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CFracDoc

[CLS:CFracView]
Type=0
HeaderFile=FracView.h
ImplementationFile=FracView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CFracView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_FILE_PRINT



[CLS:CAboutDlg]
Type=0
HeaderFile=Fractional.cpp
ImplementationFile=Fractional.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_SAVETIMESERIES
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_OPERATION_SETUP
Command12=ID_OPERATION_RUN
Command13=ID_OPERATIONS_MEASUREMENTSETUP
Command14=ID_OPERATIONS_MEASURE
Command15=ID_POWERSPECTRUM
Command16=ID_VIEW_PAGE
Command17=ID_APP_ABOUT
CommandCount=17

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_ZOOMIN
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_LEFT
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_RIGHT
Command16=ID_ZOOMOUT
Command17=ID_EDIT_CUT
Command18=ID_EDIT_UNDO
CommandCount=18

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_OPERATION_SETUP
Command6=ID_OPERATION_RUN
Command7=ID_OPERATIONS_MEASUREMENTSETUP
Command8=ID_OPERATIONS_MEASURE
Command9=ID_POWERSPECTRUM
Command10=ID_VIEW_PAGE
Command11=ID_TIMEVIEW
Command12=ID_FREQUENCYVIEW
Command13=ID_ZOOMIN
Command14=ID_ZOOMOUT
Command15=ID_APP_ABOUT
CommandCount=15

[DLG:IDD_PARAMETERS]
Type=1
Class=CParam
ControlCount=42
Control1=IDOK,button,1342242817
Control2=IDC_DURATION,edit,1350631552
Control3=IDC_NUM_INTERVALS,edit,1350631552
Control4=IDC_FLD_AMPL,edit,1350631552
Control5=IDC_FREQUENCY,edit,1350631552
Control6=IDC_STEP_AMPL,edit,1350631552
Control7=IDC_STEP_RATE,edit,1350631552
Control8=IDC_P_ZERO,edit,1350631552
Control9=IDC_EXPONENT,edit,1350631552
Control10=IDC_TAU,edit,1350631552
Control11=IDC_K,edit,1350631552
Control12=IDCANCEL,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_AC_ON,button,1342242819
Control27=IDC_PULSED_ON,button,1342242819
Control28=IDC_GATED,button,1342242819
Control29=IDC_STATIC,button,1342177287
Control30=IDC_E_FLD,button,1342242819
Control31=IDC_POLAR,button,1342242819
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_ORDER,edit,1350631552
Control35=IDC_SIGMA,edit,1350631552
Control36=IDC_STATIC,static,1342308352
Control37=IDC_DUTY_CYCLE,edit,1350631552
Control38=IDC_STATIC,static,1342308352
Control39=IDC_RI,edit,1350631552
Control40=IDC_BIPOLAR,button,1342242819
Control41=IDC_STATIC,static,1342308352
Control42=IDC_INDUCTANCE,edit,1350631552

[CLS:CParam]
Type=0
HeaderFile=Param.h
ImplementationFile=Param.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CParam

[DLG:IDD_PAGE_SELECT]
Type=1
Class=CPageSelect
ControlCount=12
Control1=IDC_PAGE,button,1342373897
Control2=IDC_VFRACTOR,button,1342242825
Control3=IDC_CURRENT,button,1342242825
Control4=IDC_INTEGRAL,button,1342242825
Control5=IDC_RADIO1,button,1342177289
Control6=IDC_RADIO3,button,1342177289
Control7=IDC_SPECTRUM,button,1342308361
Control8=IDC_FLDPSD,button,1342177289
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287

[CLS:CPageSelect]
Type=0
HeaderFile=PageSelect.h
ImplementationFile=PageSelect.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PAGE

[DLG:IDD_MEASUREMENT]
Type=1
Class=CMeasurement
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_DELAY,edit,1350631552
Control6=IDC_CAPTURE,edit,1350631552

[CLS:CMeasurement]
Type=0
HeaderFile=Measurement.h
ImplementationFile=Measurement.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_DELAY

