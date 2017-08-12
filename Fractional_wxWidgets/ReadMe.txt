========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : Fractional
========================================================================


AppWizard created th3 Fractional application template.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your Fractional application.

This first list of files describe the Foundation Classes that build the
    application. They are rarely, if ever, edited directly. The Document
    and View files are the ones where the users puts in the specifics
    of the application.

Fractional.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CFracApp application class.
    This file is rarely, if ever edited manually.

Fractional.cpp
    This is the main application source file that contains the application
    class CFracApp.
    This file is rarely, if ever edited manually.

Fractional.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Developer Studio.
    This file is rarely, if ever edited manually.

res\Fractional.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file Fractional.rc.
    You can modify graphical icons, tools buttons, etc. via the graphical
    editer.

res\Fractional.rc2
    This file contains resources that are not edited by Microsoft 
	Developer Studio.  You should place all resources not
	editable by the resource editor in this file.

Fractional.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the
    CMainFrame class.  Edit this toolbar bitmap along with the
    array in MainFrm.cpp to add more toolbar buttons.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Fractional.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Developer Studio reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC40XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC40DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.


/////////////////////////////////////////////////////////////////////////////

AppWizard created one document type and one view.
These are the files that contain most of the working code. The document
does the computation. The view interfaces with the operator/user. The 
view is the window and displays the results of the computation.

FracDoc.h, FracDoc.cpp - the Document
    These files contain your CFracDoc class.  
    The document does the computation and stores the results in a set of 
    arrays that represent the physical signals. It does the initialization 
    of the model parameters on start up.

    The dynamic storage arrays and parameters are defined in the .h file.
    The parameter initialization is done in the constructor.

    There are extra objects in the FracDoc files. These define Fractance
    behavior, as well as Fractional Order Operator behavior.

    The working code is mainly in the CFracDoc::OnOperationRun() routine.
    Data may be saved using the SaveTimeSeries() function.

    Saving the parameters so they can be loaded for use again later is
    done via the Serialize() function. Serialize is never called by the 
    user program. It is called when the Save or SaveAs buttons are pushed
    on the tool bar.

FracView.h, FracView.cpp - the View of the Document
    These files contain your CFracView class.
    CFracView objects are used to view CFracDoc objects.
    The view is the interface to the user. It accesses the arrays from
    the document and displays them according to what ever rules are 
    written. It also displays the parameters used in any given simulation.
    The screen image can be printed is WYSIWYG format.
    The shape and scaling of the view is defined in the .h file.
    The main graphical output is done in the CFracView::OnDraw(CDC* pDC)
    section. Relabeling of the various arrays can be done here.


/////////////////////////////////////////////////////////////////////////////

Other program files created manually.

Measurement.h, Measurement.cpp
    THis is an extension of the C/C++ math library to allow computation of
    phase angles.

Noise.h, Noise.cpp
    THis routine generates Gausian or alpha-stable noise that can be applied
    to a simulated input signal.

Nrutil.h, Nrutil.cpp
    This is an extension of the standard math routines of C/C++. It includes
    an automatic scaling algorithm for ploting in the view.

Param.h, Param.cpp
     This routine connects with a dialog box to allow viewing and entering
     the runtime parameters of the model implemented in the document.

PageSelect.h, PageSelect.cpp
    This routine adjusts the view settings for the specific output graph
    desired. It connects to a dialog box for selection of settings.