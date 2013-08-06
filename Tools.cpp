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
	struct tm *localTime;
	time_t Time;
	
	time(&Time);
	localTime = localtime(&Time);
	
	return asctime(localTime);
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
//	Waits 'x' * 100 miliseconds
//	Parameters:
//	 IN:
//		- int x -> specifies how often 100 ms will be waited
//------------------------------------------------------------------------------
void Tools::warte(int x)
{
	for (int i = 0; i < x; i++)
	{
		clog << "." << flush;
		Sleep(100);
	}
	clog << endl;
}