#include "TestProperties.h"



//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
TestProperties::TestProperties(void)
{
	//default text to be sent trough the COM port, when user does not specify a
	//string or a file
	//char szDefaultTransferText[] = " Oh Freunde, nicht diese Töne!\n"
	//								"Sondern laßt uns angenehmere anstimmen,\n"
	//								"und freudenvollere.\n"
	//								"Freude! Freude!\n"
	//								"Freude, schöner Götterfunken\n"
	//								"Tochter aus Elysium,\n"
	//								"Wir betreten feuertrunken,\n"
	//								"Himmlische, dein Heiligtum.\n"
	//								"Deine Zauber binden wieder,\n"
	//								"Was die Mode streng geteilt;\n"
	//								"Alle Menschen werden Brüder,\n"
	//								"Wo dein sanfter Flügel weilt.\n";
}
//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
TestProperties::~TestProperties(void)
{

}


/*
HERE OR IN OTHER CLASS??????????????????
//------------------------------------------------------------------------------
//	Open file winapi dialog box
//  http://msdn.microsoft.com/en-us/library/ms646829%28v=vs.85%29.aspx#open_file
//	Parameters:
//	 IN:
//		-  -> 
//	 OUT:
//		- 
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------



OPENFILENAME ofn;       // common dialog box structure
char szFile[260];       // buffer for file name
HWND hwnd;              // owner window
HANDLE hf;              // file handle

// Initialize OPENFILENAME
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFile = szFile;
// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
// use the contents of szFile to initialize itself.
ofn.lpstrFile[0] = '\0';
ofn.nMaxFile = sizeof(szFile);
ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
ofn.nFilterIndex = 1;
ofn.lpstrFileTitle = NULL;
ofn.nMaxFileTitle = 0;
ofn.lpstrInitialDir = NULL;
ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 

if (GetOpenFileName(&ofn)==TRUE) 
    hf = CreateFile(ofn.lpstrFile, 
                    GENERIC_READ,
                    0,
                    (LPSECURITY_ATTRIBUTES) NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    (HANDLE) NULL);

					*/