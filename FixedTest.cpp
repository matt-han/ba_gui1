#include "FixedTest.h"


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

FixedTest::FixedTest(TestStruct *testStruct)
{
	this->testStruct = testStruct;
	masterCom.sPort = this->testStruct->sMasterPort;
	slaveCom.sPort = this->testStruct->sSlavePort;

	dwExitCode = ERROR_SUCCESS;

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
				masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
				masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//masterCom.dcb.StopBits = testStruct->iStopbits;

				printTestSettings();

				//set the port dcb with the new settings
				_dwError = masterCom.setDCB();
				if(_dwError == ERROR_SUCCESS)
				{
					//Communication Loop
					//send and recieve data
					for (unsigned int index = 0; index < vTextToSend.size();
						 index++)
					{
						_dwError = communicate(vTextToSend.at(index),
												true);
						if(_dwError != ERROR_SUCCESS)
						{
							clog << "Error communicating!!!!!!"<<endl;
						}
					}
					clog<<"-----------------------------------------"<<endl;
					clog << "Transmition finished"<<endl;
					clog<<"-----------------------------------------"<<endl;
					if (bTransmitionError == true)
					{
						clog<<"Error/s transmitting information"<<endl;
						clog<<"Please check the file for exact lines"<<endl;
						clog<<"Proceed to close ports...\n\n\n"<<endl;
					}
					else
					{
						clog<<"Transmition finished successfully\n\n\n"<<endl;
					}
					//----------------------------------------------------------
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
		if(_dwError != ERROR_SUCCESS)
		{
			clog << "Error closing " << masterCom.sPort << endl;
			if (bTransmitionError == true)
				clog << "Error in tranismition" << endl;	
			return _dwError;
		}

		if (bTransmitionError == true)
			return ERROR_TRANS_INFO;
		else
			return ERROR_SUCCESS;
	}
}


long FixedTest::startDoubleTest()
{
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
	masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
	masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//masterCom.dcb.StopBits = testStruct->iStopbits;

	slaveCom.dcb.BaudRate = testStruct->iBaud; 
	slaveCom.dcb.Parity   = (BYTE)testStruct->iParity;
	slaveCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
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

	//----------------------------------------------------------
	//send and recieve data
	for (unsigned int index = 0; index < vTextToSend.size(); index++)
	{
		//	Double mode => true, true
		_dwError = communicate(vTextToSend.at(index), false);
		if(_dwError != ERROR_SUCCESS)
		{
			clog << "Error communicating!!!!!!"<<endl;
		}
	}
	
	clog<<"-----------------------------------------"<<endl;
	clog << "Transmition finished"<<endl;
	clog<<"-----------------------------------------"<<endl;

	if (bTransmitionError == true)
	{
		clog<<"Error/s transmitting information"<<endl;
		clog<<"Please check the file for exact lines"<<endl;
		clog<<"Proceed to close ports...\n\n\n"<<endl;
	}
	else
	{
		clog<<"Transmition finished successfully\n\n\n"<<endl;
	}

	masterCom.closePort();
	slaveCom.closePort();
	return ERROR_SUCCESS;
}


long FixedTest::startMasterSlaveTest(bool bMaster)
{
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
				masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
				masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
	//IMPLEMENT PROTOCOL
	//masterCom.dcb.StopBits = testStruct->iStopbits;

				printTestSettings();

				//set the port dcb with the new settings
				_dwError = masterCom.setDCB();
				if(_dwError == ERROR_SUCCESS)
				{
					
					//Communication Loop
					//send and recieve data
					
					//send info
					//wait to get it back
					
					for (unsigned int index = 0; index < vTextToSend.size();
						 index++)
					{
						if(bMaster)
							_dwError = communicateMaster(vTextToSend.at(index));
						else
							_dwError = communicateSlave(vTextToSend.at(index));

						if(_dwError != ERROR_SUCCESS)
						{
							clog << "Error communicating!!!!!!"<<endl;
						}
					}
					clog<<"-----------------------------------------"<<endl;
					clog << "Transmition finished"<<endl;
					clog<<"-----------------------------------------"<<endl;
					if (bTransmitionError == true)
					{
						clog<<"Error/s transmitting information"<<endl;
						clog<<"Please check the file for exact lines"<<endl;
						clog<<"Proceed to close ports...\n\n\n"<<endl;
					}
					else
					{
						clog<<"Transmition finished successfully\n\n\n"<<endl;
					}
					//----------------------------------------------------------
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
		if(_dwError != ERROR_SUCCESS)
		{
			clog << "Error closing " << masterCom.sPort << endl;
			if (bTransmitionError == true)
				clog << "Error in tranismition" << endl;	
			return _dwError;
		}

		if (bTransmitionError == true)
			return ERROR_TRANS_INFO;
		else
			return ERROR_SUCCESS;
	}
}

//
//long FixedTest::startSlaveTest()
//{
//	masterHCom = masterCom.openPort(masterCom.sPort);
//	if (INVALID_HANDLE_VALUE == masterHCom)
//	{
//		_dwError = GetLastError();
//		clog << "Error opening Port " << masterCom.sPort
//			 << ". Invalid Handle was returned. System Error : "
//			 << _dwError << endl;
//
//		return ERROR_PORT_OPEN;
//	}
//	else
//	{
//		masterPortComm.setComHandle(masterHCom);
//
//		_dwError = masterCom.getDCB();
//		if (_dwError == ERROR_SUCCESS)
//		{
//			_dwError = masterCom.setTimeOuts();
//			if(_dwError == ERROR_SUCCESS)
//			{
//				//change dcb to user input
//				masterCom.dcb.BaudRate = testStruct->iBaud; 
//				masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
//				masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
//	//IMPLEMENT PROTOCOL
//	//masterCom.dcb.StopBits = testStruct->iStopbits;
//
//				printTestSettings();
//
//				//set the port dcb with the new settings
//				_dwError = masterCom.setDCB();
//				if(_dwError == ERROR_SUCCESS)
//				{
//					
//					//Communication Loop
//					//send and recieve data
//					
//					//send info
//					//wait to get it back
//					
//					for (unsigned int index = 0; index < vTextToSend.size();
//						 index++)
//					{
//						_dwError = communicateSlave(vTextToSend.at(index));
//						if(_dwError != ERROR_SUCCESS)
//						{
//							clog << "Error communicating!!!!!!"<<endl;
//						}
//					}
//					clog<<"-----------------------------------------"<<endl;
//					clog << "Transmition finished"<<endl;
//					clog<<"-----------------------------------------"<<endl;
//					if (bTransmitionError == true)
//					{
//						clog<<"Error/s transmitting information"<<endl;
//						clog<<"Please check the file for exact lines"<<endl;
//						clog<<"Proceed to close ports...\n\n\n"<<endl;
//					}
//					else
//					{
//						clog<<"Transmition finished successfully\n\n\n"<<endl;
//					}
//					//----------------------------------------------------------
//				}	 //setDCB
//				else
//				{
//					clog << "Error setting the master port DCB" << endl;
//					masterCom.closePort();
//					return _dwError;
//				}//setDCB
//
//			}	 //setTimeouts
//			else
//			{
//				clog << "Error setting the master port timeouts" << endl;
//				masterCom.closePort();
//				return _dwError;
//			}//setTimeouts
//
//		}	 //getDCB
//		else
//		{
//			clog << "Error getting the master port DCB" << endl;
//			masterCom.closePort();
//			return _dwError;
//		}//getDCB
//
//		_dwError = masterCom.closePort();
//		if(_dwError != ERROR_SUCCESS)
//		{
//			clog << "Error closing " << masterCom.sPort << endl;
//			if (bTransmitionError == true)
//				clog << "Error in tranismition" << endl;	
//			return _dwError;
//		}
//
//		if (bTransmitionError == true)
//			return ERROR_TRANS_INFO;
//		else
//			return ERROR_SUCCESS;
//	}
//}


long FixedTest::communicate(string sSendData, bool bMaster)
{
	bool bRead;

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
		MessageBoxA(NULL, sSendData.c_str(), "COM 1 SENT", MB_OK);

		string sTemp = getData(bRead, sSendData);
		if (sTemp != ERROR_TRANSMITION)
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
				bTransmitionError = true;
			}

			return ERROR_SUCCESS;

		}
		else
		{
			clog <<"read was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		return ERROR_WRITE_PORT;
	}//sendData

}


long FixedTest::communicateMaster(string sSendData)
{
	string sTemp;
	int iWait = 0;
	_dwExitCode = ERROR_SUCCESS;

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
					_dwExitCode = ERROR_WAIT_SLAVE;
					break;
				}
			}
			else
				break;
			
		}
		while(true);
		

		if (sTemp != ERROR_TRANSMITION && _dwExitCode != ERROR_WAIT_SLAVE)
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
				bTransmitionError = true;
			}

			return ERROR_SUCCESS;

		}
		else
		{
			clog <<"read was false. Error waiting  for slave's response"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		return ERROR_WRITE_PORT;
	}//sendData

}


long FixedTest::communicateSlave(string sSendData)
{
	string sTemp;
	int iWait = 0;
	_dwExitCode = ERROR_SUCCESS;

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
				_dwExitCode = ERROR_WAIT_MASTER;
				break;
			}
		}
		else
			break;
			
	}
	while(true);

	
	if(sTemp != ERROR_TRANSMITION && _dwExitCode != ERROR_WAIT_MASTER)
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
		return ERROR_READ_PORT;
	}//getData
}



string FixedTest::getData(bool MasterSlave, string sSendData)
{
	
	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port
	if (MasterSlave)
	{
		if (true == masterPortComm.readData(empfang, sSendData.size()) )
		{
			clog << "--> master reads buffer" << endl;
			return empfang;
		}
		else
			return ERROR_TRANSMITION;
	}
	//else Double test ->send data true master and read slave
	else
	{
		if (true == slavePortComm.readData(empfang, sSendData.size()) )
		{
			clog << "--> slave reads buffer" << endl;
			return empfang;
		}
		else
			return ERROR_TRANSMITION;
	}
}


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


void FixedTest::printTestSettings()
{
	clog << "#####################################################" << endl;
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

