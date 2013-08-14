#include "FixedTest.h"


FixedTest::FixedTest(void)
{
	defaultText[0] = "Freude, schöner Götterfunken,";
	defaultText[1] = "Tochter aus Elysium!";
	defaultText[2] = "Wir betreten feuertrunken,";
	defaultText[3] = "Himmlische, Dein Heiligthum.";
	defaultText[4] = "Deine Zauber binden wieder,";
	defaultText[5] = "Was die Mode streng getheilt,";
	defaultText[6] = "Alle Menschen werden Brüder,";
	defaultText[7] = "Wo Dein sanfter Flügel weilt";

}

FixedTest::FixedTest(TestStruct *testStruct)
{

	//test = "abcd_efgh";

	this->testStruct = testStruct;
	masterCom.sPort = this->testStruct->sMasterPort;
	dwExitCode = ERROR_SUCCESS;



	defaultText[0] = "Freude, schöner Götterfunken,";
	defaultText[1] = "Tochter aus Elysium!";
	defaultText[2] = "Wir betreten feuertrunken,";
	defaultText[3] = "Himmlische, Dein Heiligthum.";
	defaultText[4] = "Deine Zauber binden wieder,";
	defaultText[5] = "Was die Mode streng getheilt,";
	defaultText[6] = "Alle Menschen werden Brüder,";
	defaultText[7] = "Wo Dein sanfter Flügel weilt";
	//later slave port
	
	//Protocol!!!!!
	//com.dcb. = testStruct
}

FixedTest::~FixedTest(void)
{
}


void FixedTest::setTextVector(int iTextMode)
{
	//delete all elements just in case
	vTextToSend.erase(vTextToSend.begin(), vTextToSend.end());


	// 0 for default string
	//1 for user .txt
	//2 for user string
	int index;


	switch(iTextMode)
	{
		case 0:
			for (index = 0; index < TEXT_LENGTH; index ++)
			{
				vTextToSend.push_back(defaultText[index]);
			}
			break;

		case 1:
			_dwError = transFile.openFile(this->testStruct->sFilePath);
			if(_dwError == ERROR_SUCCESS)
			{
				_dwError = transFile.readTransferFile();
				if(_dwError == ERROR_SUCCESS)
				{
					vTextToSend = transFile.vTranferFileLines;
				}

				transFile.closeFile();
			}
			break;

		case 2:
			vTextToSend.push_back(this->testStruct->sTextToTransfer);
			break;
	}

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
					//------------------------------------------------------------------------------
					for (unsigned int index = 0; index < vTextToSend.size(); index++)
					{
						_dwError = communicate(vTextToSend.at(index));
						if(_dwError != ERROR_SUCCESS)
						{
							clog << "Error communicating!!!!!!"<<endl;
							clog << "line : "<< vTextToSend.at(index) << endl;
						}
					}
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


long FixedTest::communicate(string sSendData)
{
	//start sending something
	if (true == sendData(true, sSendData))
	{
		clog <<"write was true"<<endl;
		MessageBoxA(NULL, sSendData.c_str(), "COM 1 SENT", MB_OK);

		string sTemp = getData(true, sSendData);
		if (sTemp != ERROR_TRANSMITION)
		{
			if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
			{
				clog << "read was true"<<endl;
				
			}
			else
			{
				clog << "String recieved is NOT equal to the sent string" << endl;
				MessageBoxA(NULL, (LPCSTR)empfang, "ERROR, COM 1 GOT",
							MB_OK);
			}
			
			clog << "Finish!"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			
			return ERROR_SUCCESS;

		}
		else
		{
			clog <<"read was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
							
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
	//if (true == sendData(true))
	//{
	//	clog <<"write was true"<<endl;
	//	MessageBoxA(NULL, test.c_str(), "COM 1 SENT", MB_OK);

	//	//read from the slave
	//	if(true == getData(false))
	//	{
	//		clog << "read was true"<<endl;
	//		MessageBoxA(NULL, (LPCSTR)empfang, "COM 1 GOT", MB_OK);

	//		clog << "SUCCESS!!!"<<endl;
	//		clog<<"-----------------------------------------"<<endl;
	//		clog<<"-----------------------------------------"<<endl;
	//	}
	//	else
	//	{
	//		clog <<"read was false"<<endl;
	//		clog<<"-----------------------------------------"<<endl;
	//		clog<<"-----------------------------------------"<<endl;
	//		masterCom.closePort();
	//		slaveCom.closePort();
	//		return ERROR_READ_PORT;
	//	}
	//}
	//else
	//{
	//	clog <<"write was false"<<endl;
	//	clog<<"-----------------------------------------"<<endl;
	//	clog<<"-----------------------------------------"<<endl;
	//	masterCom.closePort();
	//	slaveCom.closePort();
	//	return ERROR_WRITE_PORT;
	//}

	masterCom.closePort();
	slaveCom.closePort();
	return ERROR_SUCCESS;
}


string FixedTest::getData(bool MasterSlave, string sSendData)
{
	
	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port
	if (MasterSlave)
	{
		if (true == masterPortComm.readData(empfang, sSendData.size()) )
			return empfang;
		else
			return ERROR_TRANSMITION;
	}
	//else Double test ->send data true master and read slave
	else
	{
		if (true == slavePortComm.readData(empfang, sSendData.size()) )
			return empfang;
		else
			return ERROR_TRANSMITION;
	}
}


bool FixedTest::sendData(bool MasterSlave, string sSendData)
{
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port

	if (MasterSlave)
		return masterPortComm.writeData(sSendData.c_str(), sSendData.size());
	
	//else Double test -> write a reply if success or fail
	else
		return slavePortComm.writeData(sSendData.c_str(), sSendData.size());
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

