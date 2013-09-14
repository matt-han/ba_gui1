#include "PortCommunications.h"

//------------------------------------------------------------------------------
//	Constructor
//	Parameters:
//	 IN:
//		- HANDLE hCom -> opened COM port handle
//------------------------------------------------------------------------------
PortCommunications::PortCommunications(HANDLE hCom)
{
	this->hCom = hCom;
}


//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
PortCommunications::PortCommunications(void)
{
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
PortCommunications::~PortCommunications(void)
{
}

void PortCommunications::setComHandle(HANDLE hCom)
{
	this->hCom = hCom;
}
//------------------------------------------------------------------------------
//	Read from opened port
//	Parameters:
//	 IN:
//		- char * lpBuf ->pointer to a buffer to save the read characters
//		- DWORD dwSize -> amount of character to be read from port
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
bool PortCommunications::readData(char * lpBuf, DWORD dwSize)
{
//	int iCounter = 0;
//	int iErr;
//	DWORD dwRead;
//	DWORD dwRes;
//
//	BOOL fWaitingOnRead = FALSE;
//	OVERLAPPED osReader = {0};
//
//	// Create the overlapped event. Must be closed before exiting
//	// to avoid a handle leak.
//	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//	if (osReader.hEvent == NULL)
//	{
//		// Error creating overlapped event; abort.
//		clog<<"error creating overlapped event, abort"<<endl;
//		CloseHandle(osReader.hEvent);
//		return FALSE;
//	}
//
//
//	do
//	{
//		clog << "...Read attempt number: " << iCounter + 1 << endl;
//
//		//if not waiting on read operation, then read
//		if (!fWaitingOnRead)
//		{
//			// Issue read operation.
//			if (!ReadFile(hCom, lpBuf, dwSize, &dwRead, &osReader))
//			{
//				//if not true
//				iErr = GetLastError();
//				if (iErr != ERROR_IO_PENDING)     // read not delayed?
//					// Error in communications; report it.
//					clog << "Error reading port. System Error: " << iErr << endl;
//				else
//					fWaitingOnRead = TRUE;
//			}
//			else
//			{    
//				// read completed immediately
//				CloseHandle(osReader.hEvent);
//				return TRUE;
//			}
//		}
//		
//
//
//		if (fWaitingOnRead)
//		{
//			//dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
//			dwRes = WaitForSingleObject(osReader.hEvent, iReadTimeOut);
//
//			switch(dwRes)
//			{
//				// Read completed.
//				case WAIT_OBJECT_0:
//					if (!GetOverlappedResult(hCom, &osReader, &dwRead, FALSE))
//					{
//						// Error in communications; report it.
//						iErr = GetLastError();
//						//fRes = FALSE;
//						clog << "Error reading port. System Error: " << iErr << endl;
//						CloseHandle(osReader.hEvent);
//						return FALSE;
//					}
//					else
//					{
//						// Read completed successfully.
//						CloseHandle(osReader.hEvent);
//						return TRUE;
//					}
//					//  Reset flag so that another read operation can be issued.
//					fWaitingOnRead = FALSE;
//					break;
//
//				case WAIT_TIMEOUT:
//					// Operation isn't complete yet. fWaitingOnRead flag isn't
//					// changed since I'll loop back around, and I don't want
//					// to issue another read until the first one finishes.
//					//
//					// This is a good time to do some background work.
//					clog << "wait_timeout"<<endl;
//					break;                       
//
//				default:
//					// Error in the WaitForSingleObject; abort.
//					// This indicates a problem with the OVERLAPPED structure's
//					// event handle.
//					clog << "Error while reading in the WaitForSingleObject,\n"
//						<< "problem with the overlapped stucture handle" << endl;
//					CloseHandle(osReader.hEvent);
//					return FALSE;
//			}//switch
//
//			
//
//		}//if fWaitingOnRead
//	
//		iCounter++;
//
//	}while(iCounter < 5);
//	
//	if(iCounter == 5)
//	{
//		CloseHandle(osReader.hEvent);
//		return FALSE;
//	}
//	else
//	{
//		CloseHandle(osReader.hEvent);
//		return TRUE;
//	}
//
//

	int iCounter = 0;
	int iErr;
	DWORD dwRead;
	DWORD dwRes;
	char ourBuf[100]={0};
	int ourCount=dwSize;
	char *ourPtr=lpBuf;

	BOOL fWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};

	// Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL)
	{
		// Error creating overlapped event; abort.
		clog<<"error creating overlapped event, abort"<<endl;
		CloseHandle(osReader.hEvent);
		return FALSE;
	}


		//clog << "...Read attempt number: " << iCounter + 1 << endl;
	while(true)
	{
		//if not waiting on read operation, then read
		if (!fWaitingOnRead)
		{
			// Issue read operation.
			clog << "Read Calling details: " << hCom << endl;
			if (!ReadFile(hCom, ourBuf, dwSize, NULL, &osReader))
			{
				//if not true
				iErr = GetLastError();
				if (iErr != ERROR_IO_PENDING)     // read not delayed?
				{	// Error in communications; report it.
					clog << "Error reading port. System Error: " << iErr << endl;
					return FALSE;
				}
				else
				{
					clog << "operation not completed yet. ourBuf ->" << ourBuf << endl;
					fWaitingOnRead = TRUE;
				}
			}
			else
			{    
				// read completed immediately
				clog << "read completed immediately" << endl;
				CloseHandle(osReader.hEvent);
				return TRUE;
			}
		}	

	
		if (osReader.hEvent == NULL)
		{
			clog << "Unexpected NULL object " << endl;
			return FALSE;
		}

		dwRes = WaitForSingleObject(osReader.hEvent, WAIT_FOR_READ_OBJ);
		
		switch(dwRes)
		{
				
			case WAIT_OBJECT_0:  // Read completed. The state of the specified object is signaled
				if (!GetOverlappedResult(hCom, &osReader, &dwRead, FALSE))
				{
					// Error in communications; report it.
					iErr = GetLastError();
					//fRes = FALSE;
					clog << "Error reading port. System Error: " << iErr << endl;
					CloseHandle(osReader.hEvent);
					return FALSE;
				}

					// Read completed successfully.
					//CloseHandle(osReader.hEvent);



				clog << "---- getoverlapped was ok" << endl;
				
				if(dwRead == 0)
				{
					clog << "no data available to be read. Buffer empty" << endl;
					return FALSE;
				}


				//reset for next read, this one was successful
				iCounter = 0;
				
				memcpy(ourPtr,ourBuf,dwRead);
				ourPtr+=dwRead;
				clog << "operation completed . dwRead " << dwRead << " dwSize " << dwSize << " ourBuf " << ourBuf << endl;
				ourCount-=dwRead;
					
				if(ourCount<=0) 
				{
					CloseHandle(osReader.hEvent);
					return TRUE;
				}
				//  Reset flag so that another read operation can be issued.
				fWaitingOnRead = FALSE;
				break;

			case WAIT_TIMEOUT:	//the time out interval elapsed, and the objects state is nonsignaled

				// Operation isn't complete yet. fWaitingOnRead flag isn't
				// changed since I'll loop back around, and I don't want
				// to issue another read until the first one finishes.
				//
				// This is a good time to do some background work.
					
				//clog << "wait_timeout"<<endl;
				iCounter++;
				if(iCounter == 5)
				{
					clog << "to many timeouts. ABORT" << endl;
					return FALSE;
				}
				clog << "operation isnt complete yet, carry on..."<<endl;

				break;                       

			default:
				// Error in the WaitForSingleObject; abort.
				// This indicates a problem with the OVERLAPPED structure's
				// event handle.
				clog << "Error while reading in the WaitForSingleObject,\n"
					<< "problem with the overlapped stucture handle" << endl;
				iErr = GetLastError();
				clog << "Unexpected Error WaitForSingleObject. System Error: " << iErr << endl;
				CloseHandle(osReader.hEvent);
				return FALSE;
		}//switch
		 
		
	}//while(iCouner)

}


//------------------------------------------------------------------------------
//	Wirte to opened port
//	Parameters:
//	 IN:
//		- char * lpBuf -> pointer to a buffer with the characters to be written
//		- DWORD dwSize -> amount of character to be written to the port
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
bool PortCommunications::writeData(const char * lpBuf, DWORD dwSize)
{
	int iCounter = 0;
	int iErr;
	OVERLAPPED osWrite = {0};
	DWORD dwWritten;
	DWORD dwRes;
	BOOL  fRes = FALSE;

	//DWORD dwTransfer = sizeof(&lpBuf);

	// Create this write operation's OVERLAPPED structure hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
		// Error creating overlapped event handle.
		return FALSE;

	do
	{
		clog << "...Write attempt number: " << iCounter + 1 << endl;
	   // Issue write
		if (!WriteFile(hCom, lpBuf, dwSize, &dwWritten, &osWrite))
		{
			iErr = GetLastError();
			if (iErr != ERROR_IO_PENDING)
			{ 
				// WriteFile failed, but it isn't delayed. Report error.
				fRes = FALSE;
				iCounter++;
				clog << "Error writing to port. System Error: " << iErr << endl;
			}
			else
			{
				// Write is pending.
				dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
				switch(dwRes)
				{
					// Overlapped event has been signaled. 
					case WAIT_OBJECT_0:
						if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, FALSE))
						{
							iErr = GetLastError();
							fRes = FALSE;
							iCounter++;
							clog << "Error writing to port. System Error: " << iErr << endl;
						}
						else
						{
						
							if (dwWritten != dwSize)
							{
								// The write operation timed out. I now need to 
								// decide if I want to abort or retry. If I retry, 
								// I need to send only the bytes that weren't sent. 
								// If I want to abort, I would just set fRes to 
								// FALSE and return.

								clog << "The write operation timed out" << endl;
								fRes = FALSE;
								iCounter++;
							}
							else
							{
								//clog << "Write operation completed successfully" << endl;
								fRes = TRUE;
							}
						}
						break;
            
					default:
						// An error has occurred in WaitForSingleObject. This usually 
						// indicates a problem with the overlapped event handle.
						iErr = GetLastError();
						clog <<"Write error in WaitForSingleObject.\nThis usually "
							 <<"indicates a problem with the overlapped "
							 << "event handle." << endl;
						clog << "Error writing to port. System Error: " << iErr << endl;
						fRes = FALSE;
						iCounter++;
						break;
				}//switch

			}//else to error pending

		}//writefile
		else
		{
			// WriteFile completed immediately.

			if (dwWritten != dwSize) {
				// The write operation timed out. I now need to 
				// decide if I want to abort or retry. If I retry, 
				// I need to send only the bytes that weren't sent. 
				// If I want to abort, then I would just set fRes to 
				// FALSE and return.
				clog << "The write operation timed out" << endl;
				fRes = FALSE;
				iCounter++;
			}
			else
				fRes = TRUE;
		}

		
		if(fRes == TRUE)
		{
			CloseHandle(osWrite.hEvent);
			return fRes;
		}

	}while(iCounter < 5);

	CloseHandle(osWrite.hEvent);

	return fRes;
}


//BOOL PortCommunications::ReadFromCOMPort(unsigned char* c_Data)
//{
//    BOOL error;
//
//    DWORD numberOfBytesRead = 0;
//    error = ReadFile(hCom,
//                c_Data,
//                (DWORD)10,                 // Number Of Bytes To Read,
//                &numberOfBytesRead,
//                &ovStruct);
//
//	if (error == 0)
//	{
//		error = GetLastError();
//		clog << "System error reading from port: " << error << endl;
//		return FALSE;
//	}
//    if(numberOfBytesRead == 0)
//        return FALSE;
//
//    c_Data[numberOfBytesRead] = '\0' ;
//    //SetDlgItemText(LBL_VIS_SLABEL, (LPCTSTR)c_Data);
//
//    return TRUE;
//}
//
//BOOL PortCommunications::WriteToCOMPort(unsigned char* c_Data)
//{
//    BOOL fSuccess;
//	BOOL error;
//    DWORD numberOfBytesWritten=0;
//
//    // send the bytes out on the wire
//    if (INVALID_HANDLE_VALUE == hCom)
//    {
//        MessageBox(NULL,L"Error: Cannot send. Port closed!\n",L"ERROR", MB_OK);   // busy shutting down the app
//        return FALSE;
//    }
//
//	clog<<"================================="<<endl;
//	clog << "hCom " << hCom << endl;
//	clog << "c_Data " << c_Data << endl;
//
//    fSuccess = WriteFile(this->hCom,
//                c_Data,
//                sizeof(c_Data),                 //  Number Of Bytes To Write,
//                &numberOfBytesWritten,
//                &ovStruct);
//
//	
//	clog << "numberOfBytesWritten " << numberOfBytesWritten << endl;
//
//    if(0 == fSuccess)
//    {       
//		error = GetLastError();
//		clog << "System error writting to port: " << error << endl;
//
//		MessageBox(NULL, L"Error: Could not Write the data to RS232 port!",L"ERROR", MB_OK|MB_ICONEXCLAMATION);
//        return FALSE;
//    }
//    return TRUE;
//}

//
//
//{
//	OVERLAPPED osWrite = {0};
//	DWORD dwWritten;
//	BOOL fRes;
//
//	// Create this writes OVERLAPPED structure hEvent.
//	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//	
//	if (osWrite.hEvent == NULL)
//	{
//		// Error creating overlapped event handle.
//		clog << "Error creating overlapped event handle" << endl;
//		return FALSE;
//	}
//
//	// Issue write.
//	if (!WriteFile(hCom, lpBuf, sizeof(lpBuf), &dwWritten, &osWrite))
//	{
//		if (GetLastError() != ERROR_IO_PENDING)
//		{
//			// WriteFile failed, but it isn't delayed. Report error and abort.
//			fRes = FALSE;
//		}
//		else
//		{
//			// Write is pending.
//			if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, TRUE))
//				fRes = FALSE;
//			else
//				// Write operation completed successfully.
//				fRes = TRUE;
//		}
//	}
//	else
//		// WriteFile completed immediately.
//		fRes = TRUE;
//
//	CloseHandle(osWrite.hEvent);
//	return fRes;
//}

/*
int PortCommunications::checkPortStatus()
{
	#define STATUS_CHECK_TIMEOUT      500   // Milliseconds

	DWORD	dwRes;
	DWORD	dwCommEvent;
	DWORD	dwStoredFlags;
	DWORD	dwOvRes;

	BOOL fWaitingOnStat = FALSE;
	OVERLAPPED osStatus = {0};

	dwStoredFlags = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING |\
	EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY ;

	//org			comHandle
	if (!SetCommMask(hCom, dwStoredFlags))
		// error setting communications mask; abort
		return 0;

	osStatus.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (osStatus.hEvent == NULL)
		// error creating event; abort
		return 0;

	for ( ; ; )
	{
		// Issue a status event check if one hasn't been issued already.
		if (!fWaitingOnStat)
		{
			if (!WaitCommEvent(hCom, &dwCommEvent, &osStatus))
			{
				if (GetLastError() == ERROR_IO_PENDING)
					bWaitingOnStatusHandle = TRUE;
				else
					// error in WaitCommEvent; abort
					break;
			}
			else
				// WaitCommEvent returned immediately.
				// Deal with status event as appropriate.
				ReportStatusEvent(dwCommEvent); 
		}

		// Check on overlapped operation.
		if (fWaitingOnStat)
		{
			// Wait a little while for an event to occur.
			dwRes = WaitForSingleObject(osStatus.hEvent, STATUS_CHECK_TIMEOUT);
			switch(dwRes)
			{
				// Event occurred.
				case WAIT_OBJECT_0: 
					if (!GetOverlappedResult(hCom, &osStatus, &dwOvRes, FALSE))
						// An error occurred in the overlapped operation;
						// call GetLastError to find out what it was
						// and abort if it is fatal.
						;
					else
						// Status event is stored in the event flag
						// specified in the original WaitCommEvent call.
						// Deal with the status event as appropriate.
						ReportStatusEvent(dwCommEvent);

					// Set fWaitingOnStat flag to indicate that a new
					// WaitCommEvent is to be issued.
					fWaitingOnStat = FALSE;
					break;

				case WAIT_TIMEOUT:
					// Operation isn't complete yet. fWaitingOnStatusHandle flag 
					// isn't changed since I'll loop back around and I don't want
					// to issue another WaitCommEvent until the first one finishes.
					//
					// This is a good time to do some background work.
					//DoBackgroundWork();
					break;                       

				default:
					// Error in the WaitForSingleObject; abort
					// This indicates a problem with the OVERLAPPED structure's
					// event handle.
					CloseHandle(osStatus.hEvent);
					return 0;
			}//switch
		}
	}//for

	CloseHandle(osStatus.hEvent);
}

*/