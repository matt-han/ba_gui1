#include "Logger.h"

//------------------------------------------------------------------------------
//	Constructor for logger
//	Parameters:
//	 IN:
//		- bool bLog    -> true for logging events, false for redictecting clog
//						  to NULL
//		- string sPort -> COM port for which events will be logged
//------------------------------------------------------------------------------
Logger::Logger(bool bLog, string sPort)
{
	log(bLog, sPort);
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Logger::~Logger(void)
{
}


//------------------------------------------------------------------------------
//	Functions that creates the log file
//	Parameters:
//	 IN:
//		- bool bLog    -> true for logging events, false for redictecting clog
//						  to NULL
//		- string sPort -> COM port for which events will be logged
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
int Logger::log(bool bLog, string sPort)
{
	//backup the clog system default
	backup = clog.rdbuf();
	
	if(bLog == true)
	{
		//create file path
		path =  getenv("TEMP");
		path += "\\WN_COM_Port_Tester_";
		path += sPort;
		path += "_";
		//temp =  tools.parseTime(tools.printTime() );
		//path += temp;
		path += ".txt";

		logPath = path.c_str();

		//create/open file
		filestr.open(logPath, ios::out | ios::app);
		
		if (!filestr.is_open())
		{
			_iError = GetLastError();
			clog << "couldn't create log file" << endl;
			return ERROR_LOG;
		}
		else
		{
			//redirect clog to opened file
			psbuf = filestr.rdbuf();
			clog.rdbuf(psbuf);
			
			clog << "\n\n++++++++++++++++++++++++++++++++++++++++++++" << endl;
			clog << "+   " << tools.printTime() << flush;
			clog << "+   " << "WN COM Port Tester" << endl;
			clog << "+   " << VERSION << endl;
			clog << "++++++++++++++++++++++++++++++++++++++++++++" << endl;
		}
	}
	else
	{
		//clog redirected to NULL for no logfile
		clog.rdbuf(NULL);
	}

	return ERROR_SUCCESS;
}
//------------------------------------------------------------------------------
//	If a log file was opened, the function closes it, and always redirects 
//	clog to the system default
//	Parameters:
//	 IN:
//		- bool bLog    -> true for logging events, false for redictecting clog
//------------------------------------------------------------------------------
void Logger::closelog(bool bLog)
{
	//set clog back to default
	clog.rdbuf(backup);
	
	if(bLog == true)
	{
		filestr.close();
	}
}