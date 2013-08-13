#include "FixedTest.h"


FixedTest::FixedTest(void)
{
}

FixedTest::FixedTest(TestStruct *testStruct)
{
	this->testStruct = testStruct;
	com.sPort = this->testStruct->sMasterPort;
	//later slave port
	
	startTest();
	//Protocol!!!!!
	//com.dcb. = testStruct
}

FixedTest::~FixedTest(void)
{
}


long FixedTest::startTest()
{
	hCom = com.openPort(com.sPort);
	if (INVALID_HANDLE_VALUE == hCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << com.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _dwError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		portComm.setComHandle(hCom);

		_dwError = com.getDCB();
		if (_dwError == ERROR_SUCCESS)
		{
			_dwError = com.setTimeOuts();
			if(_dwError == ERROR_SUCCESS)
			{
				//change dcb to user input
				com.dcb.BaudRate = testStruct->iBaud; 
				com.dcb.Parity   = testStruct->iParity;
				com.dcb.StopBits = testStruct->iStopbits;

				printTestSettings();

				//set the port dcb with the new settings
				_dwError = com.setDCB();
				if(_dwError == ERROR_SUCCESS)
				{
					//start sending something
					_dwError = communicate();
					if(_dwError == ERROR_SUCCESS)
					{
						clog << "SUCCESS!!!"<<endl;
						clog<<"-----------------------------------------"<<endl;
						clog<<"-----------------------------------------"<<endl;
					}
					else 
						return _dwError;
				}
			}
		}
		com.closePort();
	}

}


long FixedTest::communicate()
{
	

	char test[5] = "abcd";
	char empfang[10];
	if (true == portComm.writeData(test, sizeof(test)) )
	{
		clog <<"write was true"<<endl;
		MessageBoxA(NULL, (LPCSTR)test, "COM 1 SENT", MB_OK);

		if (true == portComm.readData(empfang, sizeof(empfang)) )
		{
			clog << "read was true"<<endl;
			clog << empfang << endl;
			MessageBoxA(NULL, (LPCSTR)empfang, "COM 1 GOT", MB_OK);
			return ERROR_SUCCESS;
		}
		else
		{
			clog <<"read was false"<<endl;
		}
	}
	else
	{
		clog <<"write was false"<<endl;
	}

	return -100;
}


void FixedTest::printTestSettings()
{
	clog << "\n#####################################################" << endl;
	clog << "Communication Settings" << endl;
	clog << "-------------------------------" << endl;
	clog << "port   " << com.sPort << endl;
	clog << "baud   " << com.dcb.BaudRate << endl;
	clog << "parity " << com.dcb.Parity << endl;
	clog << "stop   " << com.dcb.StopBits << endl;
	clog << "proto  " << testStruct->iProtocol << endl;
	clog << "#####################################################\n"<< endl;
	//clog << "port " <<  << endl;
	//clog << "port " <<  << endl;

}