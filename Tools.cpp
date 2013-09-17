#include "Tools.h"


//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Tools::Tools(void)
{
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Tools::~Tools(void)
{
}


//------------------------------------------------------------------------------
//	Convert a TCHAR array to a string
//	Parameters:
//	 IN:
//		- TCHAR Array[] -> array to be converted to string
//	Return: converted string
//------------------------------------------------------------------------------
string Tools::convertToString(TCHAR szArray[])
{
	stringstream ss;
	int i = 0;
	while ( szArray[i] != '\0')
	{
		ss << char(szArray[i]);
		i++;
	}
	return ss.str();
}


//------------------------------------------------------------------------------
//	Convert a char array to a string
//	Parameters:
//	 IN:
//		- char * pArray -> array to be converted to string
//	Return: converted string
//------------------------------------------------------------------------------
string Tools::convertToString(char * pArray)
{
	stringstream ss;
		ss << pArray;
	return ss.str();
}


//------------------------------------------------------------------------------
//	Convert a integer to a string
//	Parameters:
//	 IN:
//		- int iNumber -> array to be converted to string
//	Return: converted string
//------------------------------------------------------------------------------
string Tools::convertToString(int iNumber)
{
	stringstream ss;
	ss << iNumber;
	return ss.str();
}


//------------------------------------------------------------------------------
//	Gets the system time 
//	Return: string containing the system time
//------------------------------------------------------------------------------
string Tools::printTime()
{
	//struct tm *localTime;
	time_t Time;
	
	time(&Time);
	//localTime = localtime(&Time);
	
	//return asctime(localTime);
	string sTemp = ctime(&Time);
	sTemp = sTemp.erase(24,1);
	return sTemp;
}


//------------------------------------------------------------------------------
//	Parse the system time
//	Parameters:
//	 IN:
//		- string s -> string containing the system time
//	Return: parsed string of the system time
//------------------------------------------------------------------------------
string Tools::parseTime(string s)
{
	unsigned int position1;
	unsigned int position2;
	bool bpos1 = true;
	bool bpos2 = true;
	while(bpos1)
	{
		position1 = s.find_first_of(":");
		if (position1 != s.npos)
			s.replace(position1, 1,"xx");
		else
			bpos1 = false;
	}

	while(bpos2)
	{
		position2 = s.find_first_of(" ");
		if (position2 != s.npos)
			s.replace(position2, 1,"_");
		else
			bpos2 = false;
	}
	
	//if (position != s.npos)
	//	s.erase(position ,s.npos);
	//
	//s.erase(remove_if(s.begin(), s.end(), isspace),s.end());
	//
	return s;
}


//------------------------------------------------------------------------------
//	Deletes spaces and '#' from the given string
//	Parameters:
//	 IN:
//		- string s -> string to be parsed
//	Return: parsed string, without white spaces or '#'
//------------------------------------------------------------------------------
string Tools::delSpacesAndComents(string s)
{
	unsigned int position = s.find_first_of("#");
	
	if (position != s.npos)
		s.erase(position ,s.npos);
	
	s.erase(remove_if(s.begin(), s.end(), isspace),s.end());
	
	return s;
}


//------------------------------------------------------------------------------
//	Parses the command line argunments and saves it in a string vector
//	Parameters:
//	 IN:
//		- LPSTR pCmdLine -> command line arguments
//	Return: string vector with the command line arguments
//------------------------------------------------------------------------------
vector<string> Tools::parseCmdLine(LPSTR pCmdLine)
{
	vector<string> svParameters;
	string sTemp = "";
	string sCmdLine = pCmdLine;
	int i = 0;


	int iLength = sCmdLine.length();

	for (int j = 0; i < iLength ; j++)
	{
		//clog << "j " << j << endl;
		while(i < iLength)
		{
			//if not a space, save it to sTemp
			if (0 == isspace(sCmdLine.at(i)) )
			{
				sTemp.append( 1, sCmdLine.at(i) );
				//clog << "i " << sCmdLine.at(i) << "-" << endl;
				i++;
			}
			else
			{
				i++;
				break;
			}
		}

		if (sTemp != "")
		{
			//clog << "stemp " << sTemp << "-" << endl << endl;
			svParameters.push_back(sTemp);
			sTemp = "";
		}
		
	}

	return svParameters;
}


//------------------------------------------------------------------------------
//	Waits 'x' * 10 miliseconds
//	Parameters:
//	 IN:
//		- int x -> specifies how often 10 ms will be waited
//------------------------------------------------------------------------------
void Tools::wait(int x)
{
	for (int i = 0; i < x; i++)
	{
		clog << "." << flush;
		Sleep(10);
	}
	clog << endl;
}


//------------------------------------------------------------------------------
//	Prints the error vector to the log file or cmd line
//	Parameters:
//	 IN:
//		- bool bPrint -> true for log file, false for message box
//		- vector int ivTestError -> error codes from tests
//------------------------------------------------------------------------------
void Tools::printErrorVector(bool bPrint, vector<int> ivTestErrors)
{
	if(bPrint)
	{
		clog << "   Test Results   " << endl;
		clog << "__________________" << endl;
		clog << "LineNr. | ExitCode" << endl;
		//clog << "------------------" << endl;

		for(int i = 0; i < ivTestErrors.size(); i++)
		{
			clog.width(7);
			clog << i+1 << flush;
			clog << " | " << ivTestErrors.at(i) << endl;

		}
		clog << endl << endl;
	}
	else
	{
		int iSuccess = 0, iFailure = 0;
		 string sResult  = "Test Results\n\n";
		
		string sSuccess = "Successful tests  : ";
		string sFailure = "Failed tests          : ";
			
		for(int i = 0; i < ivTestErrors.size(); i++)
		{
			if( ERROR_SUCCESS == ivTestErrors.at(i) )
				iSuccess++;
			else
				iFailure++;

		}

		sResult.append("Number of tests : ");

		sSuccess.append(convertToString(iSuccess));
		sSuccess.append("\n");
		
		sFailure.append(convertToString(iFailure));
		//sFailure.append("\n");

		sResult.append( convertToString(ivTestErrors.size()) );
		sResult.append("\n");
		sResult.append(sSuccess);
		sResult.append(sFailure);

		MessageBoxA(NULL, sResult.c_str(), WINDOW_TITLE, MB_OK);
	}
}


//------------------------------------------------------------------------------
//	Replaces the ASCII values in the string
//	Parameters:
//	 IN:
//		- string sToSend -> string read from the GUI or INI
// Returns the input string with parsed ASCII characters
//------------------------------------------------------------------------------
string Tools::replaceASCII(string sToSend)
{
	// example
	// \0D -> CR

	string sParsed;
	string sTemp;
	string sSlash = "\\";
	int iCharVal = 0;



	for(int i = 0; i < sToSend.length();)
	{
		//if char at i == "\\"
		if(0 == sToSend.compare(i,1,"\\"))
			//strcmp("\\", sToSend.at(i) )
		{
			sTemp = sToSend.at(i+1);
			//sTemp.append(sToSend.at(i+2));
			sTemp.insert(sTemp.end(), sToSend.at(i+2) );
			//delete the 3 chars of the ASCII
			//sToSend.erase(i,3);

			//convert string with hex number to int
			iCharVal = strtol(sTemp.c_str(), NULL, 16);
			if(iCharVal == 0)
			{
				clog << "Couldn't convert " << sTemp << " to a char" << endl;
				clog << "continue without converting " << endl;
				return "";
			}

			//append the read&parsed char
			sParsed.insert(sParsed.end(), char(iCharVal) );
				//append(char(iCharVal));

			i = i + 3;
		}
		else //append the char to the return string
		{
			sParsed.insert(sParsed.end(), sToSend.at(i) );
				//append(sToSend.at(i));
			i++;
		}
	}

	return sParsed;
}


//------------------------------------------------------------------------------
//	Replaces the error code with an text explanaition
//	Parameters:
//	 IN:
//		- int iError  -> error code
// Returns the error code explanaition / meaning
//------------------------------------------------------------------------------
string Tools::errorCodeParser(int iError)
{
	string sError;
	switch(iError)
	{
		case ERROR_CMD_SYNTAX:
			sError = "Wrong command line syntax. Please check the command line (spaces in the file path?)";
			break;

		case ERROR_CREATE_GUI:
			sError = "Error creating GUI, programm aborted";
			break;

		case ERROR_PORT_OPEN:
			sError = "Could not open the given COM Port. Is other program using the port?";
			break;

		case ERROR_CLOSE_PORT:
			sError = "Could not close the given COM Port. Is other program using the port? See log file, if available, for more information";
			break;

		case ERROR_GET_DCB:
			sError = "Could not get the DCB structure from the opened port. See log file, if available, for more information";
			break;

		case ERROR_BAUDRATE:
			sError = "Error getting or decoding Baud rates for opened port. See log file, if available, for more information";
			break;

		case ERROR_SET_TIMEOUTS:
			sError =  "Error setting the timeouts for the chosen baud rate in the opened port. See log file, if available, for more information";
			break;

		case ERROR_LOG:
			sError =  "Error creating log file. Do you have write permissions?";
			break;

		case ERROR_INI:
			sError =  "Error in test configuration file. Please see log file, if available, for more information on the error";
			break;

		case ERROR_SET_DCB:
			sError = "Error setting the edited DCB structure for the opened Port. Probably incopatible settings. See log file, if available, for more information";
			break;

		case ERROR_INPUT:
			sError =  "A parameter in the GUI is incorrect. Please see pop up windows for more information";
			break;

		case ERROR_NO_FILE:
			sError = "Test configuration file given does not exist or is empty";
			break;

		case ERROR_FILE_NOT_OPEN:
			sError = "File to transfer in test could not be open";
			break;

		case ERROR_EMPTY_FILE:
			sError = "File to transfer in test is empty. Please select another file";
			break;

		case ERROR_BAUD_MINMAX:
			sError = "MAX baud rate has to be higher than the MIN baud rate. Please edit the test configuration file or the GUI input";
			break;

		case ERROR_MISSING_PAR:
			sError = "Missing parameter to save the test configuration file. Please see pop up windows for more information";
			break;

		case ERROR_PARSE:
			sError = "Error parsing a parameter read from the test configuration file. For more information on which parameter caused the error, see log file, if available.";
			break;

		case ERROR_READ_PORT:
			sError = "Error reading from port while testing. See log file, if available, for more information on the error.";
			break;

		case ERROR_WRITE_PORT:
			sError = "Error writing to the port while testing. See log file, if available, for more information on the error.";
			break;

		case ERROR_CMP_STR:
			sError = "Send and received strings in test are not equal. See log file, if available, for more details";
			break;

		case ERROR_WAIT_SLAVE:
			sError = "TO DO";
			break;

		case ERROR_WAIT_MASTER:
			sError = "TO DO";
			break;

		case ERROR_SYNC:
			sError = "TO DO";
			break;

		case ERROR_PARSE_SLAVE:
			sError = "TO DO";
			break;

		default:
			sError = "Unknow error code. Maybe a windows system error code? Error : ";
			sError.append(convertToString(iError));
			break;

	}

	return sError;

}