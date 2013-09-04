#include "FixedTest.h"

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
FixedTest::FixedTest(void)
{
	defaultText[0] = "Freude, schoener Goetterfunken,";
	defaultText[1] = "Tochter aus Elysium!";
	defaultText[2] = "Wir betreten feuertrunken,";
	defaultText[3] = "Himmlische, Dein Heiligthum.";
	defaultText[4] = "Deine Zauber binden wieder,";
	defaultText[5] = "Was die Mode streng getheilt,";
	defaultText[6] = "Alle Menschen werden Brueder,";
	defaultText[7] = "Wo Dein sanfter Fluegel weilt";
	bTransmitionError = false;
}

//------------------------------------------------------------------------------
//	Constructor for new fixed test
//	Parameters:
//	 IN:
//		- TestStruct * testStruct -> Test structure for the fixed test
//------------------------------------------------------------------------------
FixedTest::FixedTest(TestStruct *testStruct)
{
	this->testStruct = testStruct;
	masterCom.sPort = this->testStruct->sMasterPort;
	slaveCom.sPort = this->testStruct->sSlavePort;

	_iExitCode = ERROR_SUCCESS;

	bTransmitionError = false;

	defaultText[0] = "Freude, schoener Goetterfunken,";
	defaultText[1] = "Tochter aus Elysium!";
	defaultText[2] = "Wir betreten feuertrunken,";
	defaultText[3] = "Himmlische, Dein Heiligthum.";
	defaultText[4] = "Deine Zauber binden wieder,";
	defaultText[5] = "Was die Mode streng getheilt,";
	defaultText[6] = "Alle Menschen werden Brueder,";
	defaultText[7] = "Wo Dein sanfter Fluegel weilt";
	//later slave port
	
	//Protocol!!!!!
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
FixedTest::~FixedTest(void)
{
}


//------------------------------------------------------------------------------
//	set the text to transfer in a vector
//	Parameters:
//	 IN:
//		- int iTextMode -> text mode
//------------------------------------------------------------------------------
void FixedTest::setTextVector(int iTextMode)
{
	//delete all elements just in case
	vTextToSend.erase(vTextToSend.begin(), vTextToSend.end());


	//DEFAULT_VALUE for default string
	//ID_BT_LOAD    for user .txt
	//ID_BT_TEXT    for user string
	int index;


	switch(iTextMode)
	{
		case DEFAULT_VALUE:
			for (index = 0; index < TEXT_LENGTH; index ++)
			{
				vTextToSend.push_back(defaultText[index]);
			}
			break;

		case ID_BT_LOAD:
			_iError = transFile.openFile(this->testStruct->sFilePath);
			if(_iError == ERROR_SUCCESS)
			{
				_iError = transFile.readTransferFile();
				if(_iError == ERROR_SUCCESS)
				{
					vTextToSend = transFile.vTranferFileLines;
				}

				transFile.closeFile();
			}
			break;

		case ID_BT_TEXT:
			vTextToSend.push_back(this->testStruct->sTextToTransfer);
			break;
	}


}



//------------------------------------------------------------------------------
//	starts a single test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
int FixedTest::startSingleTest()
{
	masterHCom = masterCom.openPort(masterCom.sPort);
	if (INVALID_HANDLE_VALUE == masterHCom)
	{
		_iError = GetLastError();
		clog << "Error opening Port " << masterCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _iError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		masterPortComm.setComHandle(masterHCom);

		_iError = masterCom.getDCB();
		if (_iError == ERROR_SUCCESS)
		{
			_iTimeOut = masterCom.calculateTimeOut(testStruct->iParity, testStruct->iStopbits, testStruct->iBaud);
	
			clog << "Port time out: " << _iTimeOut << " ms" << endl;

			_iError = masterCom.setTimeOuts(_iTimeOut);
			if(_iError == ERROR_SUCCESS)
			{
				//change dcb to user input
				masterCom.dcb.BaudRate = testStruct->iBaud; 
				masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
				masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
				
				//set all the protocol settings
				setProtocol(testStruct->iProtocol, &masterCom);
				
				printTestSettings();

				//set the port dcb with the new settings
				_iError = masterCom.setDCB();
				if(_iError == ERROR_SUCCESS)
				{
					//Communication Loop
					//send and recieve data
					clog<<"-----------------------------------------"<<endl;
					clog << "Transmition started" << endl;
					clog << tools.printTime()     << flush;
					clog<<"-----------------------------------------"<<endl;
					
					ivTestErrors.clear();

					for (unsigned int index = 0; index < vTextToSend.size();
						 index++)
					{
						clog << "\n\nText line nr.: " << index+1 << endl;
						_iTestError = communicate(vTextToSend.at(index), true);
						if(_iTestError != ERROR_SUCCESS)
						{
							clog << "Error communicating. Error "<< _iTestError
								 << endl;
						}
							ivTestErrors.push_back(_iTestError);

					}
					clog<<"-----------------------------------------"<<endl;
					clog << "Transmition finished"<<endl;
					clog << tools.printTime() << flush;
					clog<<"-----------------------------------------"<<endl;
					
					tools.printErrorVector(true, ivTestErrors);

					if (bTransmitionError == true)
					{
						clog<<"Error/s transmitting information"<<endl;
						clog<<"Proceed to close ports...\n\n\n"<<endl;
					}
					else
					{
						clog<<"Transmition finished successfully\n"<<endl;
					}
					//----------------------------------------------------------
				}	 //setDCB
				else
				{
					clog << "Error setting the master port DCB" << endl;
					masterCom.closePort();
					return _iError;
				}//setDCB

			}	 //setTimeouts
			else
			{
				clog << "Error setting the master port timeouts" << endl;
				masterCom.closePort();
				return _iError;
			}//setTimeouts

		}	 //getDCB
		else
		{
			clog << "Error getting the master port DCB" << endl;
			masterCom.closePort();
			return _iError;
		}//getDCB

		_iError = masterCom.closePort();
		if(_iError != ERROR_SUCCESS)
		{
			clog << "Error closing " << masterCom.sPort << endl;
			if (bTransmitionError == true)
				clog << "Error in tranismition" << endl;	
			return _iError;
		}

		if (bTransmitionError == true)
			return _iTestError;
		else
			return ERROR_SUCCESS;
	}
}


//------------------------------------------------------------------------------
//	starts a double test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
int FixedTest::startDoubleTest()
{
	//Prepare the master for communications
	masterHCom = masterCom.openPort(masterCom.sPort);
	if (INVALID_HANDLE_VALUE == masterHCom)
	{
		_iError = GetLastError();
		clog << "Error opening Port " << masterCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _iError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		masterPortComm.setComHandle(masterHCom);

		_iError = masterCom.getDCB();
		if (_iError == ERROR_SUCCESS)
		{
			_iTimeOut = masterCom.calculateTimeOut(testStruct->iParity, testStruct->iStopbits, testStruct->iBaud);
	
			clog << "Port time out: " << _iTimeOut << " ms" << endl;

			_iError = masterCom.setTimeOuts(_iTimeOut);
			if(_iError != ERROR_SUCCESS)
			{
				clog << "Error setting the master port timeouts" << endl;
				masterCom.closePort();
				return _iError;
			}
		}
		else
		{
			clog << "Error getting the master port DCB" << endl;
			masterCom.closePort();
			return _iError;
		}
	}

	//Prepare the slave for communications
	slaveHCom = slaveCom.openPort(slaveCom.sPort);
	if (INVALID_HANDLE_VALUE == slaveHCom)
	{
		_iError = GetLastError();
		clog << "Error opening Port " << slaveCom.sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _iError << endl;

		return ERROR_PORT_OPEN;
	}
	else
	{
		slavePortComm.setComHandle(slaveHCom);

		_iError = slaveCom.getDCB();
		if (_iError == ERROR_SUCCESS)
		{
			//no need to recalculate time out, settings are the same
			//_iTimeOut = slaveCom.calculateTimeOut(testStruct->iParity,
			// testStruct->iStopbits, testStruct->iBaud);
	
			clog << "Port time out: " << _iTimeOut << " ms" << endl;

			_iError = slaveCom.setTimeOuts(_iTimeOut);
			if(_iError != ERROR_SUCCESS)
			{
				clog << "Error setting the slave port timeouts" << endl;
				masterCom.closePort();
				slaveCom.closePort();
				return _iError;
			}
		}
		else
		{
			clog << "Error getting the slave port DCB" << endl;
			masterCom.closePort();
			slaveCom.closePort();
			return _iError;
		}
	}

	//if everything ok, then set the dcb and communicate
	//change dcb to user input
	masterCom.dcb.BaudRate = testStruct->iBaud; 
	masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
	masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//set all the protocol settings
	setProtocol(testStruct->iProtocol, &masterCom);

	slaveCom.dcb.BaudRate = testStruct->iBaud; 
	slaveCom.dcb.Parity   = (BYTE)testStruct->iParity;
	slaveCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//set all the protocol settings
	setProtocol(testStruct->iProtocol, &slaveCom);

	printTestSettings();

	//set the master & slave port dcb with the new settings
	_iError = masterCom.setDCB();
	if(_iError == ERROR_SUCCESS)
	{
		_iError = slaveCom.setDCB();
		if(_iError != ERROR_SUCCESS)
		{
			clog << "Error setting the slave port DCB" << endl;
			masterCom.closePort();
			slaveCom.closePort();
			return _iError;
		}
	}
	else
	{
		clog << "Error setting the master port DCB" << endl;
		masterCom.closePort();
		slaveCom.closePort();
		return _iError;
	}
	

	//if everything ok then communicate
	clog<<"-----------------------------------------"<<endl;
	clog << "Transmition started" << endl;
	clog << tools.printTime()     << flush;
	clog<<"-----------------------------------------"<<endl;
	//----------------------------------------------------------
	//send and recieve data
	for (unsigned int index = 0; index < vTextToSend.size(); index++)
	{
		//	Double mode => true, true
		_iTestError = communicate(vTextToSend.at(index), false);
		if(_iTestError != ERROR_SUCCESS)
		{
			clog << "Error communicating. Error: "<< _iTestError << endl;
		}
	}
	
	clog<<"-----------------------------------------"<<endl;
	clog << "Transmition finished"<<endl;
	clog<<"-----------------------------------------"<<endl;

	if (bTransmitionError == true)
	{
		clog<<"Error/s transmitting information"<<endl;
		clog<<"Proceed to close ports...\n\n\n"<<endl;
	}
	else
	{
		clog<<"Transmition finished successfully\n\n\n"<<endl;
	}

	

	_iError = masterCom.closePort();
	if(_iError != ERROR_SUCCESS)
	{
		clog << "Error closing " << masterCom.sPort << endl;
		if (bTransmitionError == true)
			clog << "Error in tranismition" << endl;	
		return _iError;
	}

	_iError = slaveCom.closePort();
	if(_iError != ERROR_SUCCESS)
	{
		clog << "Error closing " << masterCom.sPort << endl;
		if (bTransmitionError == true)
			clog << "Error in tranismition" << endl;	
		return _iError;
	}


	if (bTransmitionError == true)
			return _iTestError;
		else
			return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	starts a single maser test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
//implement time outs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int FixedTest::startMasterSlaveTest(bool bMaster)
{
	return ERROR_TODO;
	//masterHCom = masterCom.openPort(masterCom.sPort);
	//if (INVALID_HANDLE_VALUE == masterHCom)
	//{
	//	_iError = GetLastError();
	//	clog << "Error opening Port " << masterCom.sPort
	//		 << ". Invalid Handle was returned. System Error : "
	//		 << _iError << endl;

	//	return ERROR_PORT_OPEN;
	//}
	//else
	//{
	//	masterPortComm.setComHandle(masterHCom);

	//	_iError = masterCom.getDCB();
	//	if (_iError == ERROR_SUCCESS)
	//	{
	//		_iError = masterCom.setTimeOuts();
	//		if(_iError == ERROR_SUCCESS)
	//		{
	//			//change dcb to user input
	//			masterCom.dcb.BaudRate = testStruct->iBaud; 
	//			masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
	//			masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//			//set all the protocol settings
				//setProtocol(testStruct->iProtocol, &masterCom);

	//			printTestSettings();

	//			//set the port dcb with the new settings
	//			_iError = masterCom.setDCB();
	//			if(_iError == ERROR_SUCCESS)
	//			{
	//				
	//				//Communication Loop
	//				//send and recieve data
	//				
	//				//send info
	//				//wait to get it back
	//				
	//				for (unsigned int index = 0; index < vTextToSend.size();
	//					 index++)
	//				{
	//					if(bMaster)
	//						_iError = communicateMaster(vTextToSend.at(index));
	//					else
	//						_iError = communicateSlave(vTextToSend.at(index));

	//					if(_iError != ERROR_SUCCESS)
	//					{
	//						clog << "Error communicating!!!!!!"<<endl;
	//					}
	//				}
	//				clog<<"-----------------------------------------"<<endl;
	//				clog << "Transmition finished"<<endl;
	//				clog<<"-----------------------------------------"<<endl;
	//				if (bTransmitionError == true)
	//				{
	//					clog<<"Error/s transmitting information"<<endl;
	//					clog<<"Please check the file for exact lines"<<endl;
	//					clog<<"Proceed to close ports...\n\n\n"<<endl;
	//				}
	//				else
	//				{
	//					clog<<"Transmition finished successfully\n\n\n"<<endl;
	//				}
	//				//----------------------------------------------------------
	//			}	 //setDCB
	//			else
	//			{
	//				clog << "Error setting the master port DCB" << endl;
	//				masterCom.closePort();
	//				return _iError;
	//			}//setDCB

	//		}	 //setTimeouts
	//		else
	//		{
	//			clog << "Error setting the master port timeouts" << endl;
	//			masterCom.closePort();
	//			return _iError;
	//		}//setTimeouts

	//	}	 //getDCB
	//	else
	//	{
	//		clog << "Error getting the master port DCB" << endl;
	//		masterCom.closePort();
	//		return _iError;
	//	}//getDCB

	//	_iError = masterCom.closePort();
	//	if(_iError != ERROR_SUCCESS)
	//	{
	//		clog << "Error closing " << masterCom.sPort << endl;
	//		if (bTransmitionError == true)
	//			clog << "Error in tranismition" << endl;	
	//		return _iError;
	//	}

	//	if (bTransmitionError == true)
	//		return ERROR_TRANS_INFO;
	//	else
	//		return ERROR_SUCCESS;
	//}
}


//------------------------------------------------------------------------------
//	starts a single slave test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
//implement time outs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int FixedTest::startSlaveTest()
{
	return ERROR_TODO;
	//masterHCom = masterCom.openPort(masterCom.sPort);
	//if (INVALID_HANDLE_VALUE == masterHCom)
	//{
	//	_iError = GetLastError();
	//	clog << "Error opening Port " << masterCom.sPort
	//		 << ". Invalid Handle was returned. System Error : "
	//		 << _iError << endl;

	//	return ERROR_PORT_OPEN;
	//}
	//else
	//{
	//	masterPortComm.setComHandle(masterHCom);

	//	_iError = masterCom.getDCB();
	//	if (_iError == ERROR_SUCCESS)
	//	{
	//		_iError = masterCom.setTimeOuts();
	//		if(_iError == ERROR_SUCCESS)
	//		{
	//			//change dcb to user input
	//			masterCom.dcb.BaudRate = testStruct->iBaud; 
	//			masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
	//			masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//			//set all the protocol settings
				//setProtocol(testStruct->iProtocol, &masterCom);

	//			printTestSettings();

	//			//set the port dcb with the new settings
	//			_iError = masterCom.setDCB();
	//			if(_iError == ERROR_SUCCESS)
	//			{
	//				
	//				//Communication Loop
	//				//send and recieve data
	//				
	//				//send info
	//				//wait to get it back
	//				
	//				for (unsigned int index = 0; index < vTextToSend.size();
	//					 index++)
	//				{
	//					_iError = communicateSlave(vTextToSend.at(index));
	//					if(_iError != ERROR_SUCCESS)
	//					{
	//						clog << "Error communicating!!!!!!"<<endl;
	//					}
	//				}
	//				clog<<"-----------------------------------------"<<endl;
	//				clog << "Transmition finished"<<endl;
	//				clog<<"-----------------------------------------"<<endl;
	//				if (bTransmitionError == true)
	//				{
	//					clog<<"Error/s transmitting information"<<endl;
	//					clog<<"Please check the file for exact lines"<<endl;
	//					clog<<"Proceed to close ports...\n\n\n"<<endl;
	//				}
	//				else
	//				{
	//					clog<<"Transmition finished successfully\n\n\n"<<endl;
	//				}
	//				//----------------------------------------------------------
	//			}	 //setDCB
	//			else
	//			{
	//				clog << "Error setting the master port DCB" << endl;
	//				masterCom.closePort();
	//				return _iError;
	//			}//setDCB

	//		}	 //setTimeouts
	//		else
	//		{
	//			clog << "Error setting the master port timeouts" << endl;
	//			masterCom.closePort();
	//			return _iError;
	//		}//setTimeouts

	//	}	 //getDCB
	//	else
	//	{
	//		clog << "Error getting the master port DCB" << endl;
	//		masterCom.closePort();
	//		return _iError;
	//	}//getDCB

	//	_iError = masterCom.closePort();
	//	if(_iError != ERROR_SUCCESS)
	//	{
	//		clog << "Error closing " << masterCom.sPort << endl;
	//		if (bTransmitionError == true)
	//			clog << "Error in tranismition" << endl;	
	//		return _iError;
	//	}

	//	if (bTransmitionError == true)
	//		return ERROR_TRANS_INFO;
	//	else
	//		return ERROR_SUCCESS;
	//}
}


//------------------------------------------------------------------------------
//	starts the communication for the test
//	Parameters:
//	 IN:
//		- string sSendData -> data to send in the transmition
//		- bool bMaster	-> determine if master or slave
//Return: if write error, read error or success
//------------------------------------------------------------------------------
int FixedTest::communicate(string sSendData, bool bMaster)
{
	bool bRead;
	bTransmitionError = false;


	if (bMaster)		//Single and Master mode
	{					//if true & false then read and write to master port
		bRead = bMaster;		
	}
	else			//Double mode
	{				//if true & true then write to master and read slave
		bMaster = true;
		bRead = !bMaster;
	}

	//start sending something
	if (true == sendData(bMaster, sSendData))
	{
		clog <<"write was true"<<endl;
		MessageBoxA(NULL, sSendData.c_str(), "COM SENT", MB_OK);

		string sTemp = getData(bRead, sSendData);

		if (sTemp != ERROR_TRANSMITION)
		{
			if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
			{
				clog << "read was true"<<endl;
				return ERROR_SUCCESS;
			}
			else
			{
				clog << "String recieved is NOT equal to the sent string"<<endl;
				clog << "- " << sSendData << endl;
				clog << "- " << sTemp << endl;
				bTransmitionError = true;
				return ERROR_READ_PORT;
			}
		}
		else
		{
			clog <<"read was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			bTransmitionError = true;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmitionError = true;
		return ERROR_WRITE_PORT;
	}//sendData

}

//------------------------------------------------------------------------------
//	Communications for master mode
//	Parameters:
//	 IN:
//		- string sSendData -> text to send
//Return: error write or read port, wait error or success
//------------------------------------------------------------------------------
int FixedTest::communicateMaster(string sSendData)
{
	string sTemp;
	int iWait = 0;
	_iExitCode = ERROR_SUCCESS;

	//start sending something
	if (true == sendData(true, sSendData))
	{
		clog <<"write was true"<<endl;
		MessageBoxA(NULL, sSendData.c_str(), "COM 1 SENT", MB_OK);

		do
		{
			sTemp = getData(true, sSendData);
			if (sTemp == ERROR_TRANSMITION)
			{
				tools.wait(10);
				iWait++;

				if(iWait == 5)
				{
					clog << "Error waiting on slave's response" << endl;
					_iExitCode = ERROR_WAIT_SLAVE;
					break;
				}
			}
			else
				break;
			
		}
		while(true);
		

		if (sTemp != ERROR_TRANSMITION && _iExitCode != ERROR_WAIT_SLAVE)
		{
			if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
			{
				clog << "read  was true"<<endl;
				
			}
			else
			{
				clog << "String recieved is NOT equal to the sent string"<<endl;
				clog << "- " << sSendData << endl;
				clog << "- " << sTemp << endl;
			}

			return ERROR_SUCCESS;

		}
		else
		{
			clog <<"read was false. Error waiting  for slave's response"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			bTransmitionError = true;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmitionError = true;
		return ERROR_WRITE_PORT;
	}//sendData

}


//------------------------------------------------------------------------------
//	Communications for slave mode
//	Parameters:
//	 IN:
//		- string sSendData -> text to send
//Return: error write or read port, wait error or success
//------------------------------------------------------------------------------
int FixedTest::communicateSlave(string sSendData)
{
	string sTemp;
	int iWait = 0;
	_iExitCode = ERROR_SUCCESS;

	//start reading something
	do
	{
		sTemp = getData(true, sSendData);
		if (sTemp == ERROR_TRANSMITION)
		{
			tools.wait(10);
			iWait++;

			if(iWait == 5)
			{
				clog << "Error waiting on master to send information" << endl;
				_iExitCode = ERROR_WAIT_MASTER;
				break;
			}
		}
		else
			break;
			
	}
	while(true);

	
	if(sTemp != ERROR_TRANSMITION && _iExitCode != ERROR_WAIT_MASTER)
	{
		if (true == sendData(true, sSendData))
		{
			clog <<"write was true"<<endl;
			MessageBoxA(NULL, sSendData.c_str(), "COM 1 SENT", MB_OK);

			return ERROR_SUCCESS;
		}	 //sendData
		else
		{
			clog <<"write was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			bTransmitionError = true;
			return ERROR_WRITE_PORT;
		}//sendData
		
		//NO NEED TO COMPARE; JUST SEND IT BACK
		//if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
		//{
		//	clog << "read  was true"<<endl;
		//		
		//}
		//else
		//{
		//	clog << "String recieved is NOT equal to the sent string"<<endl;
		//	clog << "- " << sSendData << endl;
		//	clog << "- " << sTemp << endl;
		//	bTransmitionError = true;
		//}
	}
	else
	{
		clog <<"read was false. Error waiting  for master to send information"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmitionError = true;
		return ERROR_READ_PORT;
	}//getData
}


//------------------------------------------------------------------------------
//	Read the data from the opened port
//	Parameters:
//	 IN:
//		- bool MasterSlave -> determines if master or slave port
//		- string sSendData -> text sent, used to know the lenght of the text to
//								read
//Return: string read from the port
//------------------------------------------------------------------------------
string FixedTest::getData(bool MasterSlave, string sSendData)
{
	
	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port
	if (MasterSlave)
	{
		clog << "--> master reads buffer" << endl;
		if (true == masterPortComm.readData(empfang, sSendData.size(), _iTimeOut) )
			return empfang;
		else
			return ERROR_TRANSMITION;
	}
	//else Double test ->send data true master and read slave
	else
	{
		clog << "--> slave reads buffer" << endl;
		if (true == slavePortComm.readData(empfang, sSendData.size(), _iTimeOut) )
			return empfang;
		else
			return ERROR_TRANSMITION;
	}
}


//------------------------------------------------------------------------------
//	Writes sSendData to the port
//	Parameters:
//	 IN:
//		- bool MasterSlave -> determines if master or slave port
//		- string sSendData -> text to send
//Return: error or success
//------------------------------------------------------------------------------
bool FixedTest::sendData(bool MasterSlave, string sSendData)
{
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port

	if (MasterSlave)
	{
		clog << "--> master writes" << endl;
		return masterPortComm.writeData(sSendData.c_str(), sSendData.size());
	}
	
	//else Double test -> write a reply if success or fail
	else
	{
		clog << "--> slave writes" << endl;
		return slavePortComm.writeData(sSendData.c_str(), sSendData.size());
	}
}


//------------------------------------------------------------------------------
//	Prints the set test settings
//------------------------------------------------------------------------------
void FixedTest::printTestSettings()
{
	int iStop = 2;
	if(testStruct->iStopbits == ONESTOPBIT)
		iStop = 1;

	clog << "\nCommunication Settings"                     << endl;
	clog << "----------------------"                       << endl;
	clog << " * Port        " << testStruct->sMasterPort   << endl;
	clog << " * Baud rate   " << testStruct->iBaud         << endl;
	clog << " * Parity      " << testStruct->iParity       << endl;
	clog << " * Stopbits    " << iStop					   << endl;
	clog << " * Databits    " << testStruct->iDatabits	   << endl;
	clog << " * Protocol    " << testStruct->iProtocol     << endl;
	clog << "----------------------\n"					   << endl;
	//clog << "port " <<  << endl;
	//clog << "port " <<  << endl;

}

//------------------------------------------------------------------------------
//	Set the protocol settings in the dcb according to the test mode
//	Parameters:
//	 IN:
//		- int iProtocol -> protocol mode
//		- Com * com -> pointer to the com object to test
//------------------------------------------------------------------------------
void FixedTest::setProtocol(int iProtocol, Com *com)
{
	switch(iProtocol)
	{
		case 0:
			com->dcb.fOutX = true;
			com->dcb.fInX = true;
			com->dcb.fOutxCtsFlow = false;
			com->dcb.fOutxDsrFlow = false;
			com->dcb.fDsrSensitivity = false;
			com->dcb.fRtsControl = RTS_CONTROL_DISABLE;
			com->dcb.fDtrControl = DTR_CONTROL_DISABLE;
			break;

		case 1:
			com->dcb.fOutX = false;
			com->dcb.fInX = false;
			com->dcb.fOutxCtsFlow = true;
			com->dcb.fOutxDsrFlow = true;
			com->dcb.fDsrSensitivity = true;
			com->dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
			com->dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
			break;

		case 2:
			com->dcb.fOutX = false;
			com->dcb.fInX = false;
			com->dcb.fOutxCtsFlow = false;
			com->dcb.fOutxDsrFlow = false;
			com->dcb.fDsrSensitivity = false;
			com->dcb.fRtsControl = RTS_CONTROL_ENABLE;
			com->dcb.fDtrControl = RTS_CONTROL_ENABLE;
			break;
	}
}