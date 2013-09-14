#include "FixedTest.h"

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
//FixedTest::FixedTest(void)
//{
//	defaultText[0] = "Freude, schoener Goetterfunken,";
//	defaultText[1] = "Tochter aus Elysium!";
//	defaultText[2] = "Wir betreten feuertrunken,";
//	defaultText[3] = "Himmlische, Dein Heiligthum.";
//	defaultText[4] = "Deine Zauber binden wieder,";
//	defaultText[5] = "Was die Mode streng getheilt,";
//	defaultText[6] = "Alle Menschen werden Brueder,";
//	defaultText[7] = "Wo Dein sanfter Fluegel weilt";
//	bTransmitionError = false;
//}

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

	_ESC = char(27);
	_ACK = char(6);

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
				vTextToSend.push_back(tools.replaceASCII(defaultText[index]));
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
			vTextToSend.push_back(tools.replaceASCII(this->testStruct->sTextToTransfer));
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
			_iError = setPortSettings(masterCom);

			if(_iError == ERROR_SUCCESS)
			{
				printTestSettings();

				//Communication Loop
				//send and recieve data
				clog<<"-----------------------------------------"<<endl;
				clog << "Transmition started" << endl;
				clog << tools.printTime()     << endl;
				clog<<"-----------------------------------------"<<endl;
					
				ivTestErrors.clear();

				for (unsigned int index = 0; index < vTextToSend.size();
						index++)
				{
					//calculate the time per line to send
					_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size()+1);
					
					clog << "\n\nText line nr.: " << index + 1 << endl;
					clog << "time out: " << _iLineTimeOut << " ms" << endl;
					
					_iTestError = communicate(vTextToSend.at(index), true);
					
					if(_iTestError != ERROR_SUCCESS)
					{
						clog << "Error communicating. Error "<< _iTestError
								<< endl;
						if(testStruct->bStopOnError == true)
						{
							masterCom.closePort();
							return _iError;
						}
					}
					
					ivTestErrors.push_back(_iTestError);

				}

				clog<<"\n-----------------------------------------"<<endl;
				clog << "Transmition finished"<<endl;
				clog << tools.printTime() << endl;
				clog<<"-----------------------------------------"<<endl;
					
				tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);

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
			}//set port settings
			else
			{
				clog << "Error setting the master port" << endl;
				masterCom.closePort();
				return _iError;
			}

		}
		else
		{
			clog << "Error getting the master port DCB" << endl;
			masterCom.closePort();
			return _iError;
		}//getDCB



		//transmition finished, clean up
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
			_iError = setPortSettings(masterCom);
			if(_iError != ERROR_SUCCESS)
			{
				clog << "Error setting the master port settings" << endl;
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

	//master is ready, now prepare the slave

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
			_iError = setPortSettings(slaveCom);
			if(_iError != ERROR_SUCCESS)
			{
				clog << "Error setting the slave port settings" << endl;
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


	//slave is ready, start testing


	printTestSettings();



	//start the communication
	clog<<"-----------------------------------------"<<endl;
	clog << "Transmition started" << endl;
	clog << tools.printTime()     << endl;
	clog<<"-----------------------------------------"<<endl;

	ivTestErrors.clear();

	//send and recieve data
	for (unsigned int index = 0; index < vTextToSend.size(); index++)
	{
		_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size()+1);

		clog << "\n\nText line nr.: " << index + 1 << endl;
		clog << "time out: " << _iLineTimeOut << " ms" << endl;

		_iTestError = communicate(vTextToSend.at(index), false);

		if(_iTestError != ERROR_SUCCESS)
		{
			clog << "Error communicating. Error: "<< _iTestError << endl;
			if(testStruct->bStopOnError == true)
			{
				masterCom.closePort();
				slaveCom.closePort();
				return _iError;
			}
		}

		ivTestErrors.push_back(_iTestError);
	}
	
	clog<<"\n-----------------------------------------"<<endl;
	clog << "Transmition finished"<<endl;
	clog<<"-----------------------------------------"<<endl;

	tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);


	//check for errors
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
//	set the port properties to default
//	calculates the timeouts
//	sets the timeouts
//	edits the DCB
//	sets the DCB
//Returns if succeded or failure
//------------------------------------------------------------------------------
int FixedTest::setPortDefaultSettings(Com &com)
{
	_iTimeOut = com.calculateTimeOut(ODDPARITY, ONESTOPBIT, 8, B_9600);

	_iError = com.setTimeOuts(_iTimeOut);

	if(_iError == ERROR_SUCCESS)
	{
		//change dcb to user input
		com.dcb.BaudRate = B_9600; 
		com.dcb.Parity   = ODDPARITY;
		com.dcb.StopBits = ONESTOPBIT;
		com.dcb.ByteSize = 8;
				
		//set all the protocol to hardware
		setProtocol(1, &com);
	}
	else
		return _iError;

	_iError = com.setDCB();

	return _iError;
}



//------------------------------------------------------------------------------
//	set the port properties to current test settings
//	calculates the timeouts
//	sets the timeouts
//	edits the DCB
//	sets the DCB
//Returns if succeded or failure
//------------------------------------------------------------------------------
int FixedTest::setPortSettings(Com &com)
{
	_iTimeOut = com.calculateTimeOut(testStruct->iParity, testStruct->iStopbits,
									testStruct->iDatabits, testStruct->iBaud);

	_iError = com.setTimeOuts(_iTimeOut);

	if(_iError == ERROR_SUCCESS)
	{
		masterCom.dcb.BaudRate = testStruct->iBaud; 
		masterCom.dcb.Parity   = (BYTE)testStruct->iParity;
		masterCom.dcb.StopBits = (BYTE)testStruct->iStopbits;
		masterCom.dcb.ByteSize = (BYTE)testStruct->iDatabits;
				
		//set all the protocol settings
		setProtocol(testStruct->iProtocol, &com);
	}
	else
		return _iError;

	_iError = com.setDCB();

	return _iError;
}



//------------------------------------------------------------------------------
//	starts a single maser test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
int FixedTest::startMasterTest()
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
			_iError = setPortDefaultSettings(masterCom);
			if(_iError == ERROR_SUCCESS)
			{
				//dcb is set
				//
				// NOW SYNC WITH THE SLAVE
				_iError = syncMaster();
				if(_iError == ERROR_SUCCESS)
				{
				//if sync was ok, send the test settings for the current test
					_sTestInformation = createTestInformation();

					//send the test information

					//wait for ACK from slave for successfuly recieved
					//esc for bad data




					//CLOSE and reopen PORT FIRST?????????


					//set this port current test settings
					setPortSettings(masterCom);

					//send the actual data
					//calculate the time per line to send
					//_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size()+1);
				}
			
			}
			else
			{
				clog << "Error setting master default settings" << endl;
				return _iError;
			}

		//		if(_iError == ERROR_SUCCESS)
		//		{
		//			//Communication Loop
		//			//send and recieve data
		//			clog<<"-----------------------------------------"<<endl;
		//			clog << "Transmition started" << endl;
		//			clog << tools.printTime()     << endl;
		//			clog<<"-----------------------------------------"<<endl;
		//			
		//			ivTestErrors.clear();

		//			for (unsigned int index = 0; index < vTextToSend.size();
		//				 index++)
		//			{
		//				_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size()+1);
		//				clog << "\n\nText line nr.: " << index+1 << endl;
		//				clog << "time out: " << _iLineTimeOut << " ms" << endl;
		//				_iTestError = communicate(vTextToSend.at(index), true);
		//				if(_iTestError != ERROR_SUCCESS)
		//				{
		//					clog << "Error communicating. Error "<< _iTestError
		//						 << endl;
		//				}
		//					ivTestErrors.push_back(_iTestError);

		//			}
		//			clog<<"\n-----------------------------------------"<<endl;
		//			clog << "Transmition finished"<<endl;
		//			clog << tools.printTime() << endl;
		//			clog<<"-----------------------------------------"<<endl;
		//			
		//			tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);

		//			if (bTransmitionError == true)
		//			{
		//				clog<<"Error/s transmitting information"<<endl;
		//				clog<<"Proceed to close ports...\n\n\n"<<endl;
		//			}
		//			else
		//			{
		//				clog<<"Transmition finished successfully\n"<<endl;
		//			}
		//		//----------------------------------------------------------


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
//	master send ESC to slave and waits for ACK
//Returns if slave is sync
//------------------------------------------------------------------------------
int FixedTest::syncMaster()
{
	int iError = ERROR_SYNC;
	int iCount = 0;


	_iLineTimeOut = _iTimeOut * 2;

	clog<<"-----------------------------------------"<<endl;
	clog << "Sync Master with Slave" << endl;
	clog << tools.printTime()     << endl;
	clog<<"-----------------------------------------"<<endl;

	if (true == sendData(true, _ESC) )
	{
		clog <<"ESC sent\n"<<endl;

		do // five times
		{
			clog << "Sync attempt : " << iCount << endl;

			string sTemp = getData(true, _ESC);

			if (sTemp != ERROR_TRANSMITION)
			{
				if(0 == strcmp(sTemp.c_str(), _ACK.c_str()) )
				{
					clog << "ACK recieved\n"<<endl;
					iError = ERROR_SUCCESS;
				}
				else
				{
					clog << "No ACK recieved"<<endl;

				}
			}

			iCount++;

		}while(iError != ERROR_SUCCESS && iCount < 5);

			
		return iError;

		//else
		//{
		//	clog <<"read was false"<<endl;
		//	clog<<"-----------------------------------------"<<endl;
		//	clog<<"-----------------------------------------"<<endl;
		//	bTransmitionError = true;
		//	return ERROR_READ_PORT;
		//}//getData

	}
	else
	{
		clog <<"Could not send ESC signal"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		return ERROR_WRITE_PORT;
	}//sendData
}

//------------------------------------------------------------------------------
//	slave wait for ESC and sends ACK to master
//Returns if slave is sync
//------------------------------------------------------------------------------
int FixedTest::syncSlave()
{
	int iError = ERROR_SYNC;
	int iCount = 0;


	_iLineTimeOut = _iTimeOut * 2;

	clog<<"-----------------------------------------"<<endl;
	clog << "Sync Slave with Master" << endl;
	clog << tools.printTime()     << endl;
	clog<<"-----------------------------------------"<<endl;

	do // five times
	{
		clog << "Sync attempt : " << iCount << endl;

		string sTemp = getData(true, _ESC);

		if (sTemp != ERROR_TRANSMITION)
		{
			if(0 == strcmp(sTemp.c_str(), _ESC.c_str()) )
			{
				clog << "ESC recieved, reply with ACK\n"<<endl;
				iError = ERROR_SUCCESS;
			}
			else
				clog << "No ESC recieved"<<endl;
		}
		else
			clog << "Nothing recieved"<<endl;

		iCount++;

	}while(iError != ERROR_SUCCESS && iCount < 5);

	//if not success, then did not read the ESC... error in sync
	if(iError != ERROR_SUCCESS)
		return ERROR_SYNC;

	if (true == sendData(true, _ACK) )
	{
		clog <<"ACK sent\n"<<endl;
		return ERROR_SUCCESS;
	}
	else
	{
		clog <<"Could not send ACK signal"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		return ERROR_WRITE_PORT;
	}//sendData
}


//------------------------------------------------------------------------------
//	starts a single slave test
//Returns if test succeded or transmition failure
//------------------------------------------------------------------------------
int FixedTest::startSlaveTest()
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
	return ERROR_TODO;
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
//	bool bRead;
	bTransmitionError = false;
	string sTemp = "";

	//if (bMaster)		//Single and Master mode
	//{					//if true & false then read and write to master port
	//	bRead = bMaster;		
	//}
	//else				//Double mode
	//{					//if true & true then write to master and read slave
	//	//bMaster = true;
	//	//bRead = !bMaster;
	//	bRead = bMaster;
	//}

	//send the data
	if (true == sendData(bMaster, sSendData))
	{
		clog <<"write was true\n"<<endl;

		//read the data
		sTemp = getData(bMaster, sSendData);

		if (sTemp != ERROR_TRANSMITION)
		{
			clog << "sSendData " << sSendData << endl;
			clog << "sTemp     " << sTemp << endl;
			if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
			{
				clog << "read was true\n"<<endl;
				return ERROR_SUCCESS;
			}
			else
			{
				clog << "String recieved is NOT equal to the sent string"<<endl;
				clog << "- " << sSendData << endl;
				clog << "- " << sTemp << endl;
				bTransmitionError = true;
				return ERROR_CMP_STR;
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
	int iCounter = 0;
	int iError = ERROR_SUCCESS;

	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port
	//do
	//{
		if (MasterSlave)
		{

			clog << "--> master reads buffer" << endl;
			if (true == masterPortComm.readData(empfang, sSendData.size()) )
				iError = ERROR_SUCCESS;
			else
				iError = ERROR_READ_PORT;
		}
		//else Double test ->send data true master and read slave
		else
		{
			clog << "--> slave reads buffer" << endl;
			if (true == slavePortComm.readData(empfang, sSendData.size()) )
				iError = ERROR_SUCCESS;
			else
				iError = ERROR_READ_PORT;
		}
		
		//if(iError == ERROR_READ_PORT)
		//	iCounter++;
		//else
		//	break;

	//}while(iCounter < 5);
	
	
	if(iError != ERROR_SUCCESS)
		return ERROR_TRANSMITION;
	else
		return empfang;


	//if (MasterSlave)
	//{
	//	clog << "--> master reads buffer" << endl;
	//	if (true == masterPortComm.readData(empfang, sSendData.size(), _iLineTimeOut) )
	//	{
	//		if(empfang == "")
	//			return ERROR_TRANSMITION;
	//		else
	//			return empfang;
	//	}
	//	else
	//		return ERROR_TRANSMITION;
	//}
	////else Double test ->send data true master and read slave
	//else
	//{
	//	clog << "--> slave reads buffer" << endl;
	//	if (true == slavePortComm.readData(empfang, sSendData.size(), _iLineTimeOut) )
	//		return empfang;
	//	else
	//		return ERROR_TRANSMITION;
	//}
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


//------------------------------------------------------------------------------
//	creates a string with the test parameter for the master to send to the slave
//  baud;parity;stopbits;databits;protocol
//Returns a string with the parameter separated by a ;
//------------------------------------------------------------------------------
string FixedTest::createTestInformation()
{
	string sTestInfo = tools.convertToString(testStruct->iBaud);
	sTestInfo.append(";");
	sTestInfo.append(tools.convertToString(testStruct->iParity));
	sTestInfo.append(";");
	sTestInfo.append(tools.convertToString(testStruct->iStopbits));
	sTestInfo.append(";");
	sTestInfo.append(tools.convertToString(testStruct->iDatabits));
	sTestInfo.append(";");
	sTestInfo.append(tools.convertToString(testStruct->iProtocol));
	return sTestInfo;
}


//------------------------------------------------------------------------------
//	parse a string with the test parameter for the next test, only slave uses this
//
//  baud;parity;stopbits;databits;protocol
//
//------------------------------------------------------------------------------
int FixedTest::setTestInformation(string sTestInfo)
{
	string sBaud, sParity, sStopbits, sDatabits, sProtocol;
	int iTemp;

	sBaud = sTestInfo.substr(0, sTestInfo.find_first_of(";"));
	sTestInfo.erase(0, sTestInfo.find_first_of(";")+1);

	sParity = sTestInfo.substr(0, sTestInfo.find_first_of(";"));
	sTestInfo.erase(0, sTestInfo.find_first_of(";")+1);

	sStopbits = sTestInfo.substr(0, sTestInfo.find_first_of(";"));
	sTestInfo.erase(0, sTestInfo.find_first_of(";")+1);

	sDatabits = sTestInfo.substr(0, sTestInfo.find_first_of(";"));
	sTestInfo.erase(0, sTestInfo.find_first_of(";")+1);

	sProtocol = sTestInfo.substr(0, sTestInfo.npos);

	//check the read info is correct or pausible
	//baud rate
	iTemp = masterCom.translateBaudrate(sBaud);
	if(iTemp > 0)
		testStruct->iBaud = iTemp;
	else
	 return ERROR_PARSE_SLAVE;
	
	//parity
	if(sParity == "0")
		testStruct->iParity = NOPARITY;
	else if(sParity == "1")
		testStruct->iParity = ODDPARITY;
	else if(sParity == "2")
		testStruct->iParity = EVENPARITY;
	else
		return ERROR_PARSE_SLAVE;

	//stopbits
	if(sStopbits == "0")
		testStruct->iStopbits = ONESTOPBIT;
	else if(sStopbits == "2")
		testStruct->iStopbits = TWOSTOPBITS;
	else
		return ERROR_PARSE_SLAVE;

	//databits
	if(sDatabits == "7")
		testStruct->iDatabits = 7;
	else if(sDatabits == "8")
		testStruct->iDatabits = 8;
	else
		return ERROR_PARSE_SLAVE;

	//parity
	if(sProtocol == "0")
		testStruct->iProtocol = 0;
	else if(sProtocol == "1")
		testStruct->iProtocol = 1;
	else if(sProtocol == "2")
		testStruct->iProtocol = 2;
	else
		return ERROR_PARSE_SLAVE;


	return ERROR_SUCCESS;
}





//##############################################################################
//				NOT USING.......
//##############################################################################

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
		MessageBoxA(NULL, sSendData.c_str(), WINDOW_TITLE, MB_OK);

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
			MessageBoxA(NULL, sSendData.c_str(), WINDOW_TITLE, MB_OK);

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

