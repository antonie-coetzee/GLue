#ifndef _cvilvsb_H
#define _cvilvsb_H
/*
	© Copyright 1997 by National Instruments Corp.
	All rights reserved

	$Id: //NIComponents/codemart/export/9.2/9.2.0f0/includes/NI/Win32/cvilvsb.h#1 $

*/

extern void __stdcall GetLVSBHeader (void);

/*
The following describes how to make a LabVIEW CIN in CVI 4.0.1 on
Windows 95 or NT.

1.	Create a CVI project

	  Note: Currently LabView only supplies library and object files for
			MSVC and Symantec compatibility modes, so WATCOM or Borland
			cannot be used.

	  Add the following files:
		- The source file(s) for your CIN
		- cvilvsb.h

	  If Using MSVC compatibility Mode in CVI, add:
		- lvsb.lib	   (from the LabVIEW CINTOOLS\WIN32 directory)
		- labview.lib  (from the LabVIEW CINTOOLS\WIN32 directory)
		- getcin.obj   (from the LabVIEW CINTOOLS\WIN32 directory)

	  If Using Symantec compatibility Mode in CVI, add:
		- lvsb.sym.lib	   (from the LabVIEW CINTOOLS\WIN32 directory)
		- labview.sym.lib  (from the LabVIEW CINTOOLS\WIN32 directory)
		- getcin.sym.obj   (from the LabVIEW CINTOOLS\WIN32 directory)

	Save the project.

2.	In the source file(s) for your CIN,	 put the following statement
	BEFORE any struct definitions that will be passed to LabVIEW:

		#pragma pack(1)

	Put the following statement AFTER these definitions:

		#pragma pack()

3.	Add the following statements to your main CIN source file:

		#include "cvilvsb.h"
		void *dummyPointer = GetLVSBHeader;

4.	In the Build menu of the CVI Project window, change the Target
	to Dynamic Link Library.

5.	Select the Include Paths command in the Options menu of the
	Project window.	 Add the CINTOOLS directory to the
	project include paths.

6.	Select the Create Dynamic Link Library command in the Build menu
	of the Project window.	The project should compile and link successfully,
	and then a dialog appears.	In the dialog, do the following:

	a.	Set the pathname of the DLL to create.

	b.	In the Export section, click on the Change button.	In the dialog,
		set the Export What ring control to Include File Symbols.
		Then checkmark cvilsvb.h in the list.

	c.	Click on the OK button.

7.	In a DOS box do the following:

	a. Change directory to the location of your built DLL.

	b. Run the following command "c:\labview\cintools\win32\lvsbutil mydll",
		where c:\labview is the directory where LabVIEW was installed and
		mydll is the name of your DLL without the .DLL extension.

 */


#endif	/* _cvilvsb_H */
