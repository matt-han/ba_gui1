/*
 *	Class to open and close a log file
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include "Constants.h"
#include "Tools.h"

#include <string>
#include <fstream>

using namespace std;

class Logger
{
public:
	Logger(bool log, string sPort);
	~Logger(void);

//------------------------------------------------------------------------------
//Methods
	int log(bool bLog, string sPort);
	void closelog(bool bLog);


private:
//------------------------------------------------------------------------------
//Variables	
	int _iError;
	string path, temp;
	const char * logPath;
	streambuf * backup, * psbuf;
	ofstream filestr;
	Tools tools;


};
#endif