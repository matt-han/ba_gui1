#include "PortHandler.h"


PortHandler::PortHandler(HANDLE hCom)
{
	this->hCom = hCom;
}

PortHandler::PortHandler(void)
{
}


PortHandler::~PortHandler(void)
{
}

//
//
//
//Read from opened port
bool PortHandler::readData()
{
	DWORD dwRead;
	DWORD dwRes;

	BOOL fWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};

	// Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL)
	// Error creating overlapped event; abort.
	{
		clog<<"error creating overlapped event, abort"<<endl;
		return false;
	}

	if (!fWaitingOnRead)
	{
		// Issue read operation.
		if (!ReadFile(hCom, _lpBuf, READ_BUF_SIZE, &dwRead, &osReader))
		{
			if (GetLastError() != ERROR_IO_PENDING)     // read not delayed?
				// Error in communications; report it.
				clog << "error in communications" << endl;
			else
				fWaitingOnRead = TRUE;
		}
	}
	else
	{    
		// read completed immediately
		//HandleASuccessfulRead(_lpBuf, dwRead);
		clog << "1. read successfully finished"<<endl;
	}


	if (fWaitingOnRead)
	{
		dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
		switch(dwRes)
		{
			// Read completed.
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(hCom, &osReader, &dwRead, FALSE))
				// Error in communications; report it.
					clog << "error in communications" << endl;
				else
				// Read completed successfully.
				//HandleASuccessfulRead(_lpBuf, dwRead);
				clog << "2. read successfully finished" << endl;
				
				//  Reset flag so that another opertion can be issued.
				fWaitingOnRead = FALSE;
				break;

			case WAIT_TIMEOUT:
				// Operation isn't complete yet. fWaitingOnRead flag isn't
				// changed since I'll loop back around, and I don't want
				// to issue another read until the first one finishes.
				//
				// This is a good time to do some background work.
				break;                       

			default:
				// Error in the WaitForSingleObject; abort.
				// This indicates a problem with the OVERLAPPED structure's
				// event handle.
				clog << "Error while reading in the WaitForSingleObject,\n problem with the overlapped stucture handle" << endl;
				return false;
				break;
		}//switch
	}//if fWaitingOnRead

	return true;
}
//
//
//
//Write lpBuf to opened port
bool PortHandler::writeData(char * lpBuf, DWORD dwToWrite)
{
	OVERLAPPED osWrite = {0};
	DWORD dwWritten;
	BOOL fRes;

	// Create this writes OVERLAPPED structure hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (osWrite.hEvent == NULL)
		// Error creating overlapped event handle.
		return FALSE;

	// Issue write.
	if (!WriteFile(hCom, lpBuf, dwToWrite, &dwWritten, &osWrite))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			// WriteFile failed, but it isn't delayed. Report error and abort.
			fRes = FALSE;
		}
		else
		{
			// Write is pending.
			if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, TRUE))
				fRes = FALSE;
			else
				// Write operation completed successfully.
				fRes = TRUE;
		}
	}
	else
		// WriteFile completed immediately.
		fRes = TRUE;

	CloseHandle(osWrite.hEvent);
	return fRes;
}


int PortHandler::checkPortStatus()
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