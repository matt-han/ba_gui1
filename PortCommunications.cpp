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

	while(true)
	{
		//if not waiting on read operation, then read
		if (!fWaitingOnRead)
		{
			// Issue read operation.
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
					clog << "\noperation not completed yet. buffer ->" << ourBuf << endl;
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
			// Read completed. The state of the specified object is signaled	
			case WAIT_OBJECT_0:  
				if (!GetOverlappedResult(hCom, &osReader, &dwRead, FALSE))
				{
					// Error in communications; report it.
					iErr = GetLastError();
					clog << "Error reading port. System Error: " << iErr << endl;
					CloseHandle(osReader.hEvent);
					return FALSE;
				}

				clog << "GetOverlappedResult was ok" << endl;
				
				if(dwRead == 0)
				{
					clog << "No data available to be read. Buffer empty" << endl;
					CloseHandle(osReader.hEvent);
					return FALSE;
				}

				//reset for next read, this was successful
				iCounter = 0;
				
				memcpy(ourPtr, ourBuf, dwRead);
				ourPtr   += dwRead;
				ourCount -= dwRead;
					
				if(ourCount <= 0) 
				{
					clog << "read operation completed" << endl;
					CloseHandle(osReader.hEvent);
					return TRUE;
				}

				//  Reset flag so that another read operation can be issued.
				fWaitingOnRead = FALSE;
				break;

			case WAIT_TIMEOUT:
				//the time out interval elapsed, and the objects state is nonsignaled

				// Operation isn't complete yet. fWaitingOnRead flag isn't
				// changed since I'll loop back around, and I don't want
				// to issue another read until the first one finishes.
				iCounter++;
				if(iCounter == 5)
				{
					clog << "to many object timeouts. ABORT" << endl;
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

	}//while
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
								// The write operation timed out.
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
						// An error has occurred in WaitForSingleObject.
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
				// The write operation timed out.
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