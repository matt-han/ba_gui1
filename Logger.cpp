#include "Logger.h"

Logger::Logger(bool bLog, string sPort)
{
	log(bLog, sPort);
}
//Erzeugt eine log Datei und schreibt in dieser
//+++++++++++++++++++++++++++++++++++++++++++++
long Logger::log(bool bLog, string sPort)
{
	backup = clog.rdbuf();
	
	if(bLog == true)
	{
		path =  getenv("TEMP");
		path += "\\WN_COM_Port_Tester_";
		path += sPort;
		path += "_";
		//temp =  tools.parseTime(tools.printTime() );
		//path += temp;
		path += ".txt";

		logPath = path.c_str();

		filestr.open(logPath, ios::out | ios::app);
		
		if (!filestr.is_open())
		{
			error = GetLastError();
			clog << "couldn't create log file" << endl;
			return 6;
		}
		else
		{
			psbuf = filestr.rdbuf();
			clog.rdbuf(psbuf);
			
			clog << "\n\n++++++++++++++++++++++++++++++++++++++++++++" << endl;
			clog << "+   " << tools.printTime() << flush;
			clog << "+   " << "WN COm Port Tester" << endl;
			clog << "+   " << VERSION << endl;
			clog << "++++++++++++++++++++++++++++++++++++++++++++" << endl;
		}
	}
	else
	{
		//clog redirected to NULL for no logfile
		clog.rdbuf(NULL);
	}
	return 0;
}
//Schliesst log Datei wenn eine geoeffnet worden ist
//++++++++++++++++++++++++++++++++++++++++++++++++++
void Logger::closelog(bool bLog)
{
	clog.rdbuf(backup);
	
	if(bLog == true)
	{
		filestr.close();
	}
}

Logger::~Logger(void)
{
}
