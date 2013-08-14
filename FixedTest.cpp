#include "FixedTest.h"


FixedTest::FixedTest(void)
{
}

FixedTest::FixedTest(TestStruct *testStruct)
{

	//test = "abcd_efgh";

	this->testStruct = testStruct;
	masterCom.sPort = this->testStruct->sMasterPort;
	dwExitCode = ERROR_SUCCESS;

	//later slave port
	
	//Protocol!!!!!
	//com.dcb. = testStruct
}

FixedTest::~FixedTest(void)
{
}


long FixedTest::startSingleTest()
{
	test = "Niklas sollte sich eine Beschaeftigung suchen";


	masterHCom = masterCom.openPort(masterCom.sPort);
	if (INVALID_HANDLE_VALUE == masterHCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << masterCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _dwError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		masterPortComm.setComHandle(masterHCom);

		_dwError = masterCom.getDCB();
		if (_dwError == ERROR_SUCCESS)
		{
			_dwError = masterCom.setTimeOuts();
			if(_dwError == ERROR_SUCCESS)
			{
				//change dcb to user input
				masterCom.dcb.BaudRate = testStruct->iBaud; 
				masterCom.dcb.Parity   = testStruct->iParity;
				masterCom.dcb.StopBits = testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//masterCom.dcb.StopBits = testStruct->iStopbits;
				printTestSettings();

				//set the port dcb with the new settings
				_dwError = masterCom.setDCB();
				if(_dwError == ERROR_SUCCESS)
				{

					//start sending something
					if (true == sendData(true))
					{
						clog <<"write was true"<<endl;
						MessageBoxA(NULL, test.c_str(), "COM 1 SENT", MB_OK);

						if (true == getData(true))
						{
							clog << "read was true"<<endl;
							MessageBoxA(NULL, (LPCSTR)empfang, "COM 1 GOT",
										MB_OK);
							clog << "SUCCESS!!!"<<endl;
							clog<<"-----------------------------------------"
								<<endl;
							clog<<"-----------------------------------------"
								<<endl;
						}
						else
						{
							clog <<"read was false"<<endl;
							clog<<"-----------------------------------------"
								<<endl;
							clog<<"-----------------------------------------"
								<<endl;
							
							masterCom.closePort();
							return ERROR_READ_PORT;
						}//getData

					}	 //sendData
					else
					{
						clog <<"write was false"<<endl;
						clog<<"-----------------------------------------"<<endl;
						clog<<"-----------------------------------------"<<endl;
						masterCom.closePort();
						return ERROR_WRITE_PORT;
					}//sendData

				}	 //setDCB
				else
				{
					clog << "Error setting the master port DCB" << endl;
					masterCom.closePort();
					return _dwError;
				}//setDCB

			}	 //setTimeouts
			else
			{
				clog << "Error setting the master port timeouts" << endl;
				masterCom.closePort();
				return _dwError;
			}//setTimeouts

		}	 //getDCB
		else
		{
			clog << "Error getting the master port DCB" << endl;
			masterCom.closePort();
			return _dwError;
		}//getDCB

		_dwError = masterCom.closePort();
		return _dwError;
	}

}


//!!!!!!TEST!!!!!
long FixedTest::startDoubleTest()
{
	test = "Fixed test, Double Comm";


	//Prepare the master for communications
	masterHCom = masterCom.openPort(masterCom.sPort);
	if (INVALID_HANDLE_VALUE == masterHCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << masterCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _dwError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		masterPortComm.setComHandle(masterHCom);

		_dwError = masterCom.getDCB();
		if (_dwError == ERROR_SUCCESS)
		{
			_dwError = masterCom.setTimeOuts();
			if(_dwError != ERROR_SUCCESS)
			{
				clog << "Error setting the master port timeouts" << endl;
				masterCom.closePort();
				return _dwError;
			}
		}
		else
		{
			clog << "Error getting the master port DCB" << endl;
			masterCom.closePort();
			return _dwError;
		}
	}

	//Prepare the slave for communications
	slaveHCom = slaveCom.openPort(slaveCom.sPort);
	if (INVALID_HANDLE_VALUE == slaveHCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << slaveCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _dwError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		slavePortComm.setComHandle(slaveHCom);

		_dwError = slaveCom.getDCB();
		if (_dwError == ERROR_SUCCESS)
		{
			_dwError = slaveCom.setTimeOuts();
			if(_dwError != ERROR_SUCCESS)
			{
				clog << "Error setting the slave port timeouts" << endl;
				masterCom.closePort();
				slaveCom.closePort();
				return _dwError;
			}
		}
		else
		{
			clog << "Error getting the slave port DCB" << endl;
			masterCom.closePort();
			slaveCom.closePort();
			return _dwError;
		}
	}

	//if everything ok, then set the dcb and communicate
	//change dcb to user input
	masterCom.dcb.BaudRate = testStruct->iBaud; 
	masterCom.dcb.Parity   = testStruct->iParity;
	masterCom.dcb.StopBits = testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//masterCom.dcb.StopBits = testStruct->iStopbits;

	slaveCom.dcb.BaudRate = testStruct->iBaud; 
	slaveCom.dcb.Parity   = testStruct->iParity;
	slaveCom.dcb.StopBits = testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//slaveCom.dcb.StopBits = testStruct->iStopbits;

	printTestSettings();

	//set the master & slave port dcb with the new settings
	_dwError = masterCom.setDCB();
	if(_dwError == ERROR_SUCCESS)
	{
		_dwError = slaveCom.setDCB();
		if(_dwError != ERROR_SUCCESS)
		{
			clog << "Error setting the slave port DCB" << endl;
			masterCom.closePort();
			slaveCom.closePort();
			return _dwError;
		}
	}
	else
	{
		clog << "Error setting the master port DCB" << endl;
		masterCom.closePort();
		slaveCom.closePort();
		return _dwError;
	}
	

	//if everything ok then communicate
	//send data trough the master
	if (true == sendData(true))
	{
		clog <<"write was true"<<endl;
		MessageBoxA(NULL, test.c_str(), "COM 1 SENT", MB_OK);

		//read from the slave
		if(true == getData(false))
		{
			clog << "read was true"<<endl;
			MessageBoxA(NULL, (LPCSTR)empfang, "COM 1 GOT", MB_OK);

			clog << "SUCCESS!!!"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
		}
		else
		{
			clog <<"read was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			masterCom.closePort();
			slaveCom.closePort();
			return ERROR_READ_PORT;
		}
	}
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		masterCom.closePort();
		slaveCom.closePort();
		return ERROR_WRITE_PORT;
	}

	masterCom.closePort();
	slaveCom.closePort();
	return ERROR_SUCCESS;
}


bool FixedTest::getData(bool MasterSlave)
{
	
	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port
	if (MasterSlave)
		return masterPortComm.readData(empfang, test.size());
	
	//else Double test ->send data true master and read slave
	else
		return slavePortComm.readData(empfang, test.size());

}


bool FixedTest::sendData(bool MasterSlave)
{
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port

	if (MasterSlave)
		return masterPortComm.writeData(test.c_str(), test.size());
	
	//else Double test -> write a reply if success or fail
	else
		return slavePortComm.writeData(test.c_str(), test.size());
}


void FixedTest::printTestSettings()
{
	clog << "\n#####################################################" << endl;
	clog << "Communication Settings" << endl;
	clog << "-------------------------------" << endl;
	clog << "port   " << masterCom.sPort << endl;
	clog << "baud   " << masterCom.dcb.BaudRate << endl;
	clog << "parity " << masterCom.dcb.Parity << endl;
	clog << "stop   " << masterCom.dcb.StopBits << endl;
	clog << "proto  " << testStruct->iProtocol << endl;
	clog << "#####################################################\n"<< endl;
	//clog << "port " <<  << endl;
	//clog << "port " <<  << endl;

}


long FixedTest::startMasterSlaveTest()
{
	MessageBoxA(NULL, "not programmed yet", "!!!!!!!!!",
									MB_OK);
	return ERROR_SUCCESS;
}

