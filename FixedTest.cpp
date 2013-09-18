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
//	bTransmissionError = false;
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

	bTransmissionError = false;

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
//Returns if test succeded or Transmission failure
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
				//send and receive data
				clog<<"-----------------------------------------"<<endl;
				clog << "Transmission started" << endl;
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
						clog << "Error communicating. Error : "<< _iTestError << "\n"
							<< tools.errorCodeParser(_iTestError) << endl;
						if(testStruct->bStopOnError == true)
						{
							masterCom.closePort();
							return _iError;
						}
					}
					
					ivTestErrors.push_back(_iTestError);

				}

				clog<<"\n-----------------------------------------"<<endl;
				clog << "Transmission finished"<<endl;
				clog << tools.printTime() << endl;
				clog<<"-----------------------------------------"<<endl;
					
				tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);

				if (bTransmissionError == true)
				{
					clog<<"Error/s transmitting information"<<endl;
					clog<<"Proceed to close ports...\n\n\n"<<endl;
				}
				else
				{
					clog<<"Transmission finished successfully\n"<<endl;
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
			clog << "Error getting the master port settings" << endl;
			masterCom.closePort();
			return _iError;
		}//getDCB



		//Transmission finished, clean up
		_iError = masterCom.closePort();
		if(_iError != ERROR_SUCCESS)
		{
			clog << "Error closing " << masterCom.sPort << endl;
			if (bTransmissionError == true)
				clog << "Error in tranismition" << endl;	
			return _iError;
		}

		if (bTransmissionError == true)
			return _iTestError;
		else
			return ERROR_SUCCESS;
	}
}


//------------------------------------------------------------------------------
//	starts a double test
//Returns if test succeded or Transmission failure
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
			clog << "Error getting the master port settings" << endl;
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
			clog << "Error getting the slave port settings" << endl;
			masterCom.closePort();
			slaveCom.closePort();
			return _iError;
		}
	}


	//slave is ready, start testing


	printTestSettings();



	//start the communication
	clog<<"-----------------------------------------"<<endl;
	clog << "Transmission started" << endl;
	clog << tools.printTime()     << endl;
	clog<<"-----------------------------------------"<<endl;

	ivTestErrors.clear();

	//send and receive data
	for (unsigned int index = 0; index < vTextToSend.size(); index++)
	{
		_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size()+1);

		clog << "\n\nText line nr.: " << index + 1 << endl;
		clog << "time out: " << _iLineTimeOut << " ms" << endl;

		_iTestError = communicate(vTextToSend.at(index), false);

		if(_iTestError != ERROR_SUCCESS)
		{
			clog << "Error communicating. Error : "<< _iTestError << "\n"
				 << tools.errorCodeParser(_iTestError) << endl;
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
	clog << "Transmission finished"<<endl;
	clog<<"-----------------------------------------"<<endl;

	tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);


	//check for errors
	if (bTransmissionError == true)
	{
		clog<<"Error/s transmitting information"<<endl;
		clog<<"Proceed to close ports...\n\n\n"<<endl;
	}
	else
	{
		clog<<"Transmission finished successfully\n\n\n"<<endl;
	}


	_iError = masterCom.closePort();
	if(_iError != ERROR_SUCCESS)
	{
		clog << "Error closing " << masterCom.sPort << endl;
		if (bTransmissionError == true)
			clog << "Error in tranismition" << endl;	
		return _iError;
	}

	_iError = slaveCom.closePort();
	if(_iError != ERROR_SUCCESS)
	{
		clog << "Error closing " << masterCom.sPort << endl;
		if (bTransmissionError == true)
			clog << "Error in tranismition" << endl;	
		return _iError;
	}


	if (bTransmissionError == true)
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
		com.dcb.BaudRate = testStruct->iBaud; 
		com.dcb.Parity   = (BYTE)testStruct->iParity;
		com.dcb.StopBits = (BYTE)testStruct->iStopbits;
		com.dcb.ByteSize = (BYTE)testStruct->iDatabits;
				
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
//Returns if test succeded or Transmission failure
//------------------------------------------------------------------------------
int FixedTest::startMasterTest()
{
	clog << "I'M THE MASTER" << endl;
	string sTestString = "teststring";
	string sTestSettings, sHeader;
	int iLineNumber = 0;
	bTransmissionError = false;

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
		//Configure and sync the master with the slave
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
					clog <<"\n\nSYNC SUCCESS!\n\n"<<endl;

					//if sync was ok, create the string with the test settings
					//for the current test
					sTestSettings = createTestInformation();

					//header <LineNumber-textlength>
					sHeader = createHeader(iLineNumber, sTestSettings.length());
					
					//i have the header and the test settings.
					//send the header and get an ack
					_iError = sendAndSync(sHeader);
					if(_iError == ERROR_SUCCESS)
					{
						clog << "Send header and get ack: OK\n" << endl;

						//send the test settings
						_iError = sendAndSync(sTestSettings);
						if(_iError != ERROR_SUCCESS)
						{
							clog << "Error sending the test settings to the slave and"
								 << " waiting for a response" << endl;
							clog << "Error : " << _iError << endl;
							clog << tools.errorCodeParser(_iError) << endl;
						}
						else
							clog << "Send test settings and get ack: OK" << endl;
					}
					else
					{
						clog << "Error sending the header to the slave and"
							 << " waiting for a response" << endl;
						clog << "Error : " << _iError << endl;
						clog << tools.errorCodeParser(_iError) << endl;
					}
				}//sync master
				else
				{
					clog << "Error syncronizing master and slave" << endl;
				}
			
			}
			else
			{
				clog << "Error setting master default settings" << endl;
			}

		}	 //getDCB
		else
		{
			clog << "Error getting the master port settings" << endl;
		}//getDCB



		//IF ALL THE SYNCS AND SETTTINGS WERE OK
		//NOW COMMUNICATE

		tools.wait(1);
		if(_iError == ERROR_SUCCESS)
		{
			//set the actual test settings
			_iError = setPortSettings(masterCom);
			if(_iError == ERROR_SUCCESS)
			{

				//print the test information
				printTestSettings();					
				

				//sync with the new settings
				clog << "trying to sync with the new settings! " << endl;
				_iError = syncMaster();
				if(_iError == ERROR_SUCCESS)
				{
					//Communication Loop
					//send and recieve data
					clog<<"-----------------------------------------"<<endl;
					clog << "Transmission started" << endl;
					clog << tools.printTime()     << endl;
					clog<<"-----------------------------------------"<<endl;
						
					ivTestErrors.clear();
					for (unsigned int index = 0; index < vTextToSend.size();
					index++)
					{
						_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size() + 1);
						//_iLineTimeOut = _iTimeOut * (sTestString.size() + 1); 
						clog << "\n\nText line nr.: " << "1" << endl;
						clog << "time out: " << _iLineTimeOut << " ms" << endl;
		
						//create header for current line
						//sHeader = createHeader(1, sTestString.size() );
						sHeader = createHeader(1, vTextToSend.at(index).size() );

						//SEND HEADER
						sendAndSync(sHeader);

						//SEND TEXT
						//_iTestError = communicateMaster(sTestString);
						_iTestError = communicateMaster(vTextToSend.at(index));

						if(_iTestError != ERROR_SUCCESS)
						{
							clog << "Error communicating. Error : "<< _iTestError << "\n"
								 << tools.errorCodeParser(_iTestError) << endl;
							bTransmissionError = true;
							if(testStruct->bStopOnError)
								break;
						}

						ivTestErrors.push_back(_iTestError);

					}//for

					clog<<"\n-----------------------------------------"<<endl;
					clog << "Transmission finished"<<endl;
					clog << tools.printTime() << endl;
					clog<<"-----------------------------------------"<<endl;

					tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);

					if (bTransmissionError == true)
					{
						clog<<"Error/s transmitting information"<<endl;
						clog<<"Proceed to close ports...\n\n\n"<<endl;
					}
					else
					{
						clog<<"Transmission finished successfully\n"<<endl;
					}

				}//syncMaster			
			}//set test setting

		}//if preparations were ok


		//else no test ocurred and close the port

		
		if(ERROR_SUCCESS != masterCom.closePort())
		{
			clog << "Error closing " << masterCom.sPort << endl;

			if (bTransmissionError == true)
				clog << "Error in tranismition, see logs for more information" << endl;	

			return ERROR_CLOSE_PORT;
		}

		return _iError;
	}

	//no more code needed from here on

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

	clog << "		-----------------------------------------" << endl;
	clog << "		Sync Master with Slave" << endl;
	clog << "		" << tools.printTime()     << endl;
	clog << "		-----------------------------------------" << endl;

	if (true == sendData(true, _ESC) )
	{
		clog <<"		ESC sent\n" << endl;

		do // ten times
		{
			clog << "		_____________________" << endl;
			clog << "		Sync attempt : " << iCount + 1 << endl;

			string sTemp = getData(true, 1);

			if (sTemp != ERROR_TRANSMISSION)
			{
				if(0 == strcmp(sTemp.c_str(), _ACK.c_str()) )
				{
					clog << "		ACK received\n" << endl;
					iError = ERROR_SUCCESS;
				}
				else
				{
					clog << "		No ACK received" << endl;

				}
			}

			iCount++;

		}while(iError != ERROR_SUCCESS && iCount < 10);

			
		return iError;

	}
	else
	{
		clog <<"		Could not send ESC signal" << endl;
		clog<<"		-----------------------------------------" << endl;
		clog<<"		-----------------------------------------" << endl;
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

	clog << "		-----------------------------------------" << endl;
	clog << "		Sync Slave with Master" << endl;
	clog << "		" << tools.printTime()     << endl;
	clog << "		-----------------------------------------" << endl;

	do // ten times
	{
		clog << "		_____________________" << endl;
		clog << "		Sync attempt : " << iCount + 1 << endl;

		string sTemp = getData(true, 1);

		if (sTemp != ERROR_TRANSMISSION)
		{
			if(0 == strcmp(sTemp.c_str(), _ESC.c_str()) )
			{
				clog << "		ESC received, reply with ACK\n" << endl;
				iError = ERROR_SUCCESS;
			}
			else
				clog << "		No ESC received" << endl;
		}
		else
			clog << "		Nothing received" << endl;

		iCount++;

	}while(iError != ERROR_SUCCESS && iCount < 10);

	//if not success, then did not read the ESC... error in sync
	if(iError != ERROR_SUCCESS)
		return ERROR_SYNC;

	if (true == sendData(true, _ACK) )
	{
		clog <<"		ACK sent\n" << endl;
		return ERROR_SUCCESS;
	}
	else
	{
		clog << "		Could not send ACK signal" << endl;
		clog << "		-----------------------------------------" << endl;
		clog << "		-----------------------------------------" << endl;
		return ERROR_WRITE_PORT;
	}//sendData
}


//------------------------------------------------------------------------------
//	starts a single slave test
//Returns if test succeded or Transmission failure
//------------------------------------------------------------------------------
int FixedTest::startSlaveTest()
{
	bTransmissionError = false;
	//variable names involve master, because it is the main port in this instance,
	//in the overall test, this port is the slave
	clog << "I'M THE SLAVE" << endl;

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
				// NOW SYNC WITH THE MASTER
				_iError = syncSlave();
				if(_iError == ERROR_SUCCESS)
				{
					clog <<"\n\nSYNC SUCCESS!\n\n"<<endl;

					//master sends header, read it and respond with ACK
					_iError = getAndSync(HEADER_LENGTH);
					if(_iError == ERROR_SUCCESS)
					{
						//decode the header
						clog << "_sReceivedFromMaster " << _sReceivedFromMaster << endl;
						_iError = parseHeader(_sReceivedFromMaster);
						if(_iError == ERROR_SUCCESS)
						{
							clog << "Header received and parsed, wait for test information" << endl;

							//get the test settings
							clog << "_iTextLength " << _iTextLength << endl;
							_iError = getAndSync(_iTextLength);
							if(_iError == ERROR_SUCCESS)
							{
								clog << "_sReceivedFromMaster " << _sReceivedFromMaster << endl;
								_iError = setTestInformation(_sReceivedFromMaster);
								if(_iError == ERROR_SUCCESS)
								{
									clog << "ready to start test and receive information" << endl;
								}
								else
									clog << "Test information received was corrupt" << endl;
							
							}//get test settings
							else
								clog << "Error gettings test settings and syncing with master" << endl;
						
						}//parse Header
						else
							clog << "Header information was corrupt" << endl;

					}//get header
					else
						clog << "Error getting header and syncing with master" << endl;

				}//syncSlave
				else
					clog << "Error syncing slave with master" << endl;

			}//setPortDefaultSettings
			else
				clog << "Error setting slaves default settings" << endl;
		
		}//getDCB
		else
		{
			clog << "Error getting the slave port settings" << endl;
			//masterCom.closePort();
			//return _iError;
		}//getDCB
	}

	//slave is synced and ready to receive test information
	//IF ALL THE SYNCS AND SETTTINGS WERE OK
	//NOW COMMUNICATE
	tools.wait(1);
	if(_iError == ERROR_SUCCESS)
	{
		//set the actual test settings
		_iError = setPortSettings(masterCom);
		if(_iError == ERROR_SUCCESS)
		{
			//print the test information
			printTestSettings();					
				
			//sync with the new settings
			clog << "trying to sync with the new settings! " << endl;
			_iError = syncSlave();
			if(_iError == ERROR_SUCCESS)
			{
				//Communication Loop
				//send and receive data
				clog<<"-----------------------------------------"<<endl;
				clog << "Transmission started" << endl;
				clog << tools.printTime()     << endl;
				clog<<"-----------------------------------------"<<endl;
						
				ivTestErrors.clear();
				for (unsigned int index = 0; index < vTextToSend.size();
				index++)
				{
				//_iLineTimeOut = _iTimeOut * (vTextToSend.at(index).size() + 1);
					
					//get Header
					_iError = getAndSync(HEADER_LENGTH);
					if(_iError == ERROR_SUCCESS)
					{
						//decode the header
						_iError = parseHeader(_sReceivedFromMaster);
						if(_iError == ERROR_SUCCESS)
						{
							clog << "Header received and parsed" << endl;

							//get the transfer text
							_iTestError = communicateSlave(_iTextLength); //read and resend
							if(_iTestError != ERROR_SUCCESS)
							{
								clog << "Error communicating. Error : "<< _iTestError << "\n"
								 << tools.errorCodeParser(_iTestError) << endl;
								bTransmissionError = true;
								if(testStruct->bStopOnError)
									break;
							}

							ivTestErrors.push_back(_iTestError);
						}
						else
						{
							clog << "Error parsing header" << endl;
							break;
						}
					}
					else
					{
						clog << "Error getting the header" << endl;
						break;
					}
					
				}//for

				clog<<"\n-----------------------------------------"<<endl;
				clog << "Transmission finished"<<endl;
				clog << tools.printTime() << endl;
				clog<<"-----------------------------------------"<<endl;
				
				tools.printErrorVector(testStruct->bLoggerState, ivTestErrors);

				if (bTransmissionError == true)
				{
					clog<<"Error/s transmitting information"<<endl;
					clog<<"Proceed to close ports...\n\n\n"<<endl;
				}
				else
				{
					clog<<"Transmission finished successfully\n"<<endl;
				}
			}//syncMaster			
		}//set test setting

	}//if preparations were ok




	_iError = masterCom.closePort();
	if(_iError != ERROR_SUCCESS)
	{
		clog << "Error closing " << masterCom.sPort << endl;
		if (bTransmissionError == true)
			clog << "Error in tranismition" << endl;	
		return _iError;
	}

	if (bTransmissionError == true)
		return _iTestError;
	else
		return ERROR_SUCCESS;
	

}


//------------------------------------------------------------------------------
//	starts the communication for the test
//	Parameters:
//	 IN:
//		- string sSendData -> data to send in the Transmission
//		- bool bMaster	-> determine if master or slave
//Return: if write error, read error or success
//------------------------------------------------------------------------------
int FixedTest::communicate(string sSendData, bool bMaster) 
{
//	bool bRead;
	bTransmissionError = false;
	bool bRead;
	string sTemp = "";

	if (bMaster)		//Single and Master mode
	{					//if true then read and write to master port
		bRead = bMaster;		
	}
	else				//Double mode
	{					//if false then write to master and read slave
		bMaster = true;
		//bRead = !bMaster;
		bRead = false;
	}

	//send the data
	if (true == sendData(bMaster, sSendData))
	{
		clog <<"write was true\n"<<endl;

		//read the data
		sTemp = getData(bRead, sSendData.size() );

		if (sTemp != ERROR_TRANSMISSION)
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
				clog << "String received is NOT equal to the sent string"<<endl;
				clog << "- " << sSendData << endl;
				clog << "- " << sTemp << endl;
				bTransmissionError = true;
				return ERROR_CMP_STR;
			}
		}
		else
		{
			clog <<"read was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			bTransmissionError = true;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmissionError = true;
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
string FixedTest::getData(bool MasterSlave, int iDataSize)
{
	int iError = ERROR_SUCCESS;

	SecureZeroMemory(empfang, sizeof(empfang));
	
	//Single test -> send and read information from the same port
	//MasterSlave test -> send and read reply from the same port

	if (MasterSlave)
	{

		clog << "\n--> main port reads buffer" << endl;
		if (true == masterPortComm.readData(empfang, iDataSize) )
			iError = ERROR_SUCCESS;
		else
			iError = ERROR_READ_PORT;
	}
	//else Double test ->send data true master and read slave
	else
	{
		clog << "--> slave reads buffer" << endl;
		if (true == slavePortComm.readData(empfang, iDataSize) )
			iError = ERROR_SUCCESS;
		else
			iError = ERROR_READ_PORT;
	}	
	
	if(iError != ERROR_SUCCESS)
		return ERROR_TRANSMISSION;
	else
		return empfang;
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
		clog << "\n--> main port writes" << endl;
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
//Return success or parse error
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

//------------------------------------------------------------------------------
//	creates a string with the line number and textlenght for the next to arrive
//	information, only slave uses this
//	 IN:
//		- int iLineNumber -> line number
//		- int iTextLength-> text length
//
//	<LineNumber-textlength>
//Return success or parse error
//------------------------------------------------------------------------------
string FixedTest::createHeader(int iLineNumber, int iTextLength)
{
	string sHeader = "<";

	if(iLineNumber >= 0 && iLineNumber < 10)
		sHeader.append("00");
	else if(iLineNumber >= 10 && iLineNumber < 100)
		sHeader.append("0");

	sHeader.append(tools.convertToString(iLineNumber));

	sHeader.append("-");


	if(iTextLength >= 0 && iTextLength < 10)
		sHeader.append("00");
	else if(iTextLength >= 10 && iTextLength < 100)
		sHeader.append("0");

	sHeader.append(tools.convertToString(iTextLength));

	sHeader.append(">");

	return sHeader;
}


//------------------------------------------------------------------------------
//	parse a string with the line number and textlenght for the next to arrive
//	information, only slave uses this
//	 IN:
//		- string sHeader -> received header with line number and text length
//
//  <LineNumber-textlength>
//Return success or parse error
//------------------------------------------------------------------------------
int FixedTest::parseHeader(string sHeader)
{
	string sLineNumber, sTextLength, sTemp;

	sTemp = sHeader.substr(0,1);
	if(sTemp == "<")
	{
		sTemp = sHeader.substr(sHeader.length() - 1, 1);
		if(sTemp == ">")
		{
			sHeader.erase(0, 1);
			sHeader.erase(sHeader.length() -1, 1);

			sLineNumber = sHeader.substr(0, sHeader.find_first_of("-"));
			sTextLength = sHeader.substr(sHeader.find_first_of("-") + 1, sHeader.npos);
		}
		else
		{
			return ERROR_MSG_HEADER;
		}
	}
	else
	{
		return ERROR_MSG_HEADER;
	}

	_iLineNumber = strtol(sLineNumber.c_str(), NULL, 10);
	_iTextLength = strtol(sTextLength.c_str(), NULL, 10);

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	send a string and waits for the slave ACK response
//	 IN:
//		- string sInformation -> string to send
//
//Return success or  error writing or syncing
//------------------------------------------------------------------------------
int FixedTest::sendAndSync(string sInformation)
{
	bool bStop	 = false;
	bool bError = false;
	int iCounter = 0;
//	int iError	 = 1;
	string sTemp = "";

	bError = sendData(true, sInformation);
	if(bError)
	{
		clog << "Master sent : " << sInformation << endl;
		
		do
		{
			clog << "Master waits for acknowledge" << endl;

			sTemp = getData(true, 1);
			if (sTemp != ERROR_TRANSMISSION)
			{
				if(sTemp == _ACK)
					return ERROR_SUCCESS;
				else if(sTemp != "")
				{
					clog << "	Master received something but not an acknowledge" << endl;
					clog << "	received :" << sTemp << endl;
					//return ERROR_SYNC;
				}
			}

			iCounter++;
			if(iCounter == 10)
				bStop = true;

			tools.wait(1);

		}while(!bStop);


		clog << "	waited for slaves response, none arrived" << endl;
		return ERROR_SYNC;
	}
	else
	{
		clog << "	Error sending information to slave" << endl;
		clog << "	header :" << sInformation << endl;
		return ERROR_WRITE_PORT;
	}

}


//------------------------------------------------------------------------------
//	reads a string and reponds with ACK
//	saves the string in _sReceivedFromMaster
//Return success or  error writing or syncing
//------------------------------------------------------------------------------
int FixedTest::getAndSync(int iLength)
{
	bool bStop	 = false;
	int iCounter = 0;
	int iError	 = 1;
	string sTemp = "";


	do
	{
		
		sTemp = getData(true, iLength);
		if (sTemp != ERROR_TRANSMISSION)
		{
			if(sTemp != "")
			{
				clog << "	Slave received something" << endl;
				clog << "	received : " << sTemp << endl;
				_sReceivedFromMaster = sTemp;
				iError = ERROR_SUCCESS;
				break;
			}
			else
				clog << "slave buffer empty" << endl;
		}
		//else
		//{
		//	clog << "	Error getting acknowledge from slave after sending the information"<<endl;
		//	clog << "	- " << sInformation << endl;
		//}


		iCounter++;
		if(iCounter == 10)
		{
			bStop = true;
			iError = ERROR_READ_PORT;
		}

		tools.wait(1);
	}while(!bStop);


	if(iError == ERROR_SUCCESS)
	{
		if(true == sendData(true, _ACK))
		{
			clog << "	Acknowledge sent" << endl;
		}
		else
		{
			clog << "	Error sending acknowledge to master" << endl;
			return ERROR_WRITE_PORT;
		}
	}

	return iError;

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
	if (true == sendData(true, sSendData) )
	{
		clog << "+ write was true"<<endl;
		clog << "+ Master sent : " << sSendData << endl;

		do
		{
			sTemp = getData(true, sSendData.size());
			if (sTemp == ERROR_TRANSMISSION)
			{
				tools.wait(1); //wait 10 ms
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
		

		if (sTemp != ERROR_TRANSMISSION && _iExitCode != ERROR_WAIT_SLAVE)
		{
			if(0 == strcmp(sTemp.c_str(), sSendData.c_str()) )
			{
				clog << "read  was true"<<endl;
				
			}
			else
			{
				clog << "String received is NOT equal to the sent string"<<endl;
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
			bTransmissionError = true;
			return ERROR_READ_PORT;
		}//getData

	}	 //sendData
	else
	{
		clog <<"write was false"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmissionError = true;
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
int FixedTest::communicateSlave(int iLength)
{
	string sTemp;
	int iWait = 0;
	_iExitCode = 1;

	//start reading something
	do
	{
		sTemp = getData(true, iLength);

		if (sTemp != ERROR_TRANSMISSION)
		{
			if(sTemp != "")
			{
				clog << "	Slave received something" << endl;
				clog << "	received : " << sTemp << endl;
				_sReceivedFromMaster = sTemp;
				_iExitCode = ERROR_SUCCESS;
				break;
			}
			else
				clog << "slave buffer empty" << endl;
			
		}
		
		iWait++;
		
		if(iWait == 5)
		{
			clog << "Error waiting on master to send information" << endl;
			_iExitCode = ERROR_WAIT_MASTER;
			break;
		}

		tools.wait(1); //10 ms	
	}
	while(true);

	
	if(sTemp != ERROR_TRANSMISSION && _iExitCode == ERROR_SUCCESS)
	{
		if (true == sendData(true, sTemp))
		{
			clog <<"- write was true"<<endl;
			clog << "- Slave sent : " << sTemp << endl;
			//MessageBoxA(NULL, sTemp.c_str(), WINDOW_TITLE, MB_OK);

			return ERROR_SUCCESS;
		}	 //sendData
		else
		{
			clog <<"write was false"<<endl;
			clog<<"-----------------------------------------"<<endl;
			clog<<"-----------------------------------------"<<endl;
			bTransmissionError = true;
			return ERROR_WRITE_PORT;
		}//sendData
	}
	else
	{
		clog <<"read was false. Error waiting  for master to send information"<<endl;
		clog<<"-----------------------------------------"<<endl;
		clog<<"-----------------------------------------"<<endl;
		bTransmissionError = true;
		return ERROR_READ_PORT;
	}//getData
}

