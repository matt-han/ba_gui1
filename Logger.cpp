#include "Logger.h"

//------------------------------------------------------------------------------
//	Constructor for logger
//	Parameters:
//	 IN:
//		- bool bLog    -> true for logging events, false for redictecting clog
//						  to NULL
//		- string sPort -> COM port for which events will be logged
//		- int iMasterSlave -> indicates wheter it is a master or slave port
//------------------------------------------------------------------------------
Logger::Logger(bool bLog, string sPort, int iMasterSlave)
{
	iError = ERROR_SUCCESS;
	backup = NULL;
	iError = log(bLog, sPort, iMasterSlave);
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Logger::~Logger(void)
{
	clog << "closing logger" << endl;

	//set clog back to default
	if(backup != NULL)
		clog.rdbuf(backup);

	filestr.close();
}


//------------------------------------------------------------------------------
//	Functions that creates the log file
//	Parameters:
//	 IN:
//		- bool bLog    -> true for logging events, false for redictecting clog
//						  to NULL
//		- string sPort -> COM port for which events will be logged
//		- int iMasterSlave -> indicates wheter it is a master or slave port
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
int Logger::log(bool bLog, string sPort, int iMasterSlave)
{
	//backup the clog system default
	backup = clog.rdbuf();
	bool bFileExists = true;

	string sMasterSlave;
	string sDestFolder = getenv("TEMP"); 
	string sCompName   = getenv("COMPUTERNAME");

	if(0 == sPort.compare(0,1,"\\"))
		sPort.erase(0, sPort.find_last_of("\\")+1);

	if(iMasterSlave == 3)
		sMasterSlave = "Slave_";
	else
		sMasterSlave = "Master_";

	int iCount = 1;

	if(bLog == true)
	{
		do
		{
			//create file path
			path =  sDestFolder;
			path += "\\Serial_Port_Tester_";
			path += sCompName;
			path += "_";
			path += sMasterSlave;
			path += sPort;
			path += "_";
			path += tools.convertToString(iCount);
			//temp =  tools.parseTime(tools.printTime() );
			//path += temp;
			path += ".txt";

			//logPath = path.c_str();

			//check if file exists
			ifstream file(path.c_str());
			
			if(file.good())
				iCount++;
			else
				bFileExists = false;

			file.close();

		}while(bFileExists);


		//create/open file
		//filestr.open(logPath, ios::out | ios::app);
		filestr.open(path.c_str(), ios::out | ios::app);
		
		if (!filestr.is_open())
		{
			_iError = GetLastError();
			string sError = "couldn't create log file. System Error: ";
			sError.append(tools.convertToString(_iError));
			MessageBoxA(NULL, sError.c_str(), WINDOW_TITLE, MB_OK | MB_ICONERROR );
			cout << sError << endl;
			return ERROR_LOG;
		}
		else
		{
			//redirect clog to opened file
			psbuf = filestr.rdbuf();
			clog.rdbuf(psbuf);

			//44
			clog << endl << endl;
			clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			
			clog << "++  " << flush;
			clog.width(37);
			clog.setf(ios::left, ios::adjustfield);
			clog << tools.printTime()<< flush;
			clog << " ++" << endl;

			clog << "++  " << flush;
			clog.width(37);
			clog.setf(ios::left, ios::adjustfield);
			clog << "WN Serial COM Port Tester"<< flush;
			clog << " ++" << endl;
			
			clog << "++  Version " << flush;
			clog.width(29);
			clog.setf(ios::left, ios::adjustfield);
			clog  << VERSION << flush;
			clog << " ++" << endl;
			
			clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;


			//clog << endl << endl;
			//clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			//clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			//clog << "++  " << tools.printTime()		<< flush;
			//clog << "++  WN Serial COM Port Tester             ++"	<< endl;
			//clog << "++  Version " << VERSION						<< endl;
			//clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
			//clog << "++++++++++++++++++++++++++++++++++++++++++++"	<< endl;
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
//void Logger::closelog(bool bLog)
//{
//	////set clog back to default
//	//clog.rdbuf(backup);
//	//
//	//if(bLog == true)
//	//{
//	//	filestr.close();
//	//}
//}