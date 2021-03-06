//=============================================================================
//
// CSerial.cpp: implementation of the C2_ComInfo class.
//=============================================================================


#include "CSerial.h"
//#if !defined(NDEBUG)
//#define NDEBUG
#include <assert.h>

//=============================================================================
// ComInfo
//=============================================================================
C2_ComInfo::C2_ComInfo(void)
{
	m_NumbPort	= 0;	// Name of COM port
	m_IdBaudRat = 3;	// [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
	m_IdBitData = 3;	// [5,6,7,8]
	m_IdBitStop = 0;	// [1, 1.5, 2] 
	m_IdParity	= 0;	// [None, Odd, Even, Mark, Space]
	m_IdFlwCtrl = 2;	// [Xon/Xoff, Hardware, None ]
}

C2_ComInfo::~C2_ComInfo(void)
{
}
void C2_ComInfo::Update(C2_ComInfo *pComInfo)
{
	m_NumbPort	= pComInfo->m_NumbPort ;
	m_IdBaudRat	= pComInfo->m_IdBaudRat;
	m_IdBitData	= pComInfo->m_IdBitData;
	m_IdBitStop	= pComInfo->m_IdBitStop;
	m_IdParity	= pComInfo->m_IdParity ;
	m_IdFlwCtrl	= pComInfo->m_IdFlwCtrl;
}

bool C2_ComInfo::Compare(C2_ComInfo *pComInfo)
{
	if ((m_IdBaudRat != pComInfo->m_IdBaudRat) ||
		(m_IdBitData != pComInfo->m_IdBitData) ||
		(m_IdBitStop != pComInfo->m_IdBitStop) ||
		(m_IdParity	 != pComInfo->m_IdParity ) ||
		(m_IdFlwCtrl != pComInfo->m_IdFlwCtrl))
		return false;
	else
		return true ;
}

//-----------------------------------------------------------------------------
//	 Defending on ComboBox input
//   Baud rates at which the communication device operates
//-----------------------------------------------------------------------------
DWORD C2_ComInfo::GetBaudRate()	                  // return DWORD
{
	DWORD	nRate[8]={CBR_1200 , CBR_2400 , CBR_19200,                                                                                       CBR_4800 , CBR_9600, 
		              CBR_38400, CBR_57600, CBR_115200};
	//	DWORD	nRate[8]={1200 , 2400 , 4800 , 9600,
	//					  19200, 38400, 57600, 115200};

	if (m_IdBaudRat < 8)
		return nRate[m_IdBaudRat];
	else
		return CBR_9600;
}

BYTE C2_ComInfo::GetBitsData()
{
	BYTE nBits[4] = { 5, 6, 7, 8};

	if (m_IdBitData < 4)
		return nBits[m_IdBitData];
	else
		return 8;
}

BYTE C2_ComInfo::GetBitsStop()
{
	BYTE nStop[3] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};
	//	BYTE nStop[3] = { 1, 2, 2};

	if (m_IdBitStop < 3)
		return nStop[m_IdBitStop];
	else
		return ONESTOPBIT;
}


BYTE C2_ComInfo::GetBitParity()	// None, Mark, Odd
{
	BYTE nStop[5] = {NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY};

	if (m_IdBitStop < 5)
		return nStop[m_IdBitStop];
	else
		return NOPARITY;
}

//=============================================================================
// C2_ComPort Class
//=============================================================================

C2_ComPort::C2_ComPort(void)
{
// 	m_IsOpen	= false;
// 	m_IsSend	= false;
// 	m_IsRecv	= false;

	m_Status	= STAT_PORT_NONE;
	m_NumPort	= 0;
	m_TimeOut	= 0;

	m_nLenTx	= 0; 
	m_nLenRx	= 0;
	m_nLenOn	= 0;
	memset(m_BuffTx, 0, SIZE_PORT_SEND);
	memset(m_BuffRx, 0, SIZE_PORT_RECV);
	memset(m_BuffOn, 0, 4096);

	m_pWndThread= NULL;
	m_pOwner	= NULL;
	m_hComm		= NULL;
	m_IsThreadOn= false;

	// initialize overlapped structure members to zero
	m_OverLap.Offset	= 0;
	m_OverLap.OffsetHigh= 0;

	// create events
	m_OverLap.hEvent= NULL;
	m_hEvtWrite		= NULL;
	m_hEvtClose		= NULL;
}

C2_ComPort::~C2_ComPort(void)
{
	do { SetEvent(m_hEvtClose); }
	while (m_IsThreadOn);

	TRACE("Thread ended\n");
}
//-----------------------------------------------------------------------------
//	Initialize the port. This can be port 1 to 10.
//-----------------------------------------------------------------------------
bool C2_ComPort::OnCreate(C2_ComInfo *pInfo, CWnd *pOwner)
{
	assert((pInfo->m_NumbPort >  0) && (pInfo->m_NumbPort <= MAX_NUMB_PORT));
	assert(pOwner != NULL);

	if ((pInfo->m_NumbPort <= 0) || (pInfo->m_NumbPort > MAX_NUMB_PORT))
		return false;

	m_Status = STAT_PORT_ERRO;	// Begin = Error of port

	// 0: if the thread is alive: Kill it
	if (m_IsThreadOn)
	{
		do { SetEvent(m_hEvtClose); }
		while (m_IsThreadOn);
		TRACE("Thread ended\n");
	}

	// 1: Create events: Overlap, write, read, close
	if (m_OverLap.hEvent)
		ResetEvent(m_OverLap.hEvent);    //Sets the specified event object to the nonsignaled state.
	m_OverLap.hEvent= CreateEvent(NULL, true, false, NULL);

	if (m_hEvtWrite)
		ResetEvent(m_hEvtWrite);
	m_hEvtWrite		= CreateEvent(NULL, true, false, NULL);
	
	if (m_hEvtClose)
		ResetEvent(m_hEvtClose);
	m_hEvtClose		= CreateEvent(NULL, true, false, NULL);

	// 2: initialize the event objects
	m_hEvtArray[0]= m_hEvtClose	;		// highest priority
	m_hEvtArray[1]= m_OverLap.hEvent;	// event of receive 
	m_hEvtArray[2]= m_hEvtWrite	;		// event of send data 

	// initialize critical section
	InitializeCriticalSection(&m_csCommSync);
	
	// set buffer size for writing and save the owner
	memset(m_BuffTx, 0, SIZE_PORT_SEND);
	memset(m_BuffRx, 0, SIZE_PORT_RECV);

	m_NumPort	= pInfo->m_NumbPort;
	m_pOwner	= pOwner;
	m_nEvtPort	= EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;

	// now it critical!
	EnterCriticalSection(&m_csCommSync);

	// 3: if the port is already opened: close it
	if (m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings 
	CString	szPort;
	szPort.Format(_T("\\\\.\\COM%d"), pInfo->m_NumbPort);	

	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
		return false;

	// 4: set the timeout values
	COMMTIMEOUTS	nTimeout;

	GetCommTimeouts(m_hComm, &nTimeout);
	nTimeout.ReadIntervalTimeout		= MAXWORD;	// Maximum time between read chars
	nTimeout.ReadTotalTimeoutMultiplier = 0;        // Multiplier of characters = 0
	nTimeout.ReadTotalTimeoutConstant	= 0;	    // Constant in milliseconds = 1000
	nTimeout.WriteTotalTimeoutMultiplier= 0;        // Multiplier of characters	= 0
	nTimeout.WriteTotalTimeoutConstant	= 0;	    // Constant in milliseconds = 1000
	// configure
	if (SetCommTimeouts(m_hComm, &nTimeout))
	{						   
		if (SetCommMask(m_hComm, m_nEvtPort))
		{
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!

				m_dcb.BaudRate	= pInfo->GetBaudRate ();
				m_dcb.ByteSize	= pInfo->GetBitsData ();
				m_dcb.Parity	= pInfo->GetBitParity();
				m_dcb.StopBits	= pInfo->GetBitsStop ();
			
				if (SetCommState(m_hComm, &m_dcb))
				{}	// normal operation... continue
				else
					ProcMsgError("SetCommState()");				
			}
			else
				ProcMsgError("GetCommState()");
		}
		else
			ProcMsgError("SetCommMask()");
	}
	else
		ProcMsgError("SetCommTimeouts()");

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	LeaveCriticalSection(&m_csCommSync);   // release critical section
	StartWatch();                          // Starting of watching comport

// 	m_IsOpen = true;
// 	m_IsRecv = false;
// 	m_IsSend = false;

	m_Status = STAT_PORT_IDLE;             // Open & Idle
	m_NumPort= pInfo->m_NumbPort;

	return true;
}

//-----------------------------------------------------------------------------
//	Hieu: Close and free COM port
//-----------------------------------------------------------------------------
void C2_ComPort::OnClose()
{	
	if (m_Status == STAT_PORT_NONE)
		return;

	StopsWatch();

	delete m_pWndThread;
	m_pWndThread = NULL;
	m_IsThreadOn = false;	

	SetCommMask(m_hComm, 0);
	EscapeCommFunction(m_hComm, CLRDTR);

	// purge any outstanding reads/writes and close device handle
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_RXABORT |	PURGE_TXCLEAR | PURGE_RXCLEAR );

	CloseHandle(m_hComm);

	m_hComm	 = NULL ;
	m_Status = STAT_PORT_NONE;

	m_nLenTx = 0;
	m_nLenRx = 0;

// 	m_IsOpen = false;
// 	m_IsRecv = false;
// 	m_IsSend = false;	
}

//-----------------------------------------------------------------------------
//	Hieu: Reset COM port state
//-----------------------------------------------------------------------------
void C2_ComPort::OnReset(C2_ComInfo *pInfo)
{
	m_Status = STAT_PORT_ERRO;

	if (GetCommState(m_hComm, &m_dcb))
	{
		m_dcb.BaudRate	= pInfo->GetBaudRate ();
		m_dcb.ByteSize	= pInfo->GetBitsData ();
		m_dcb.Parity	= pInfo->GetBitParity();
		m_dcb.StopBits	= pInfo->GetBitsStop ();
		m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!

		if (SetCommState(m_hComm, &m_dcb))
		    m_Status = STAT_PORT_IDLE;
		else
			ProcMsgError("SetCommState()");
	}
	else
		ProcMsgError("GetCommState()");

	m_nLenRx = 0;
	m_nLenTx = 0;
}

//-----------------------------------------------------------------------------
//	start comm watching
//-----------------------------------------------------------------------------
bool C2_ComPort::StartWatch()
{
	if (!(m_pWndThread = AfxBeginThread(CommThread, this)))
		return false;

	TRACE("Thread started\n");
	return true;	
}

//-----------------------------------------------------------------------------
//	Restart the comm thread
//-----------------------------------------------------------------------------
bool C2_ComPort::ResetWatch()
{
	TRACE("Thread resumed\n");
	m_pWndThread->ResumeThread();

	return true;	
}

//-----------------------------------------------------------------------------
//	Suspend the comm thread
//-----------------------------------------------------------------------------
bool C2_ComPort::StopsWatch()
{
	TRACE("Thread suspended\n");

	if (m_pWndThread)
		m_pWndThread->SuspendThread();

	return true;	
}

//-----------------------------------------------------------------------------
//	The CommThread Function.
//-----------------------------------------------------------------------------
UINT C2_ComPort::CommThread(LPVOID pParam)
{
	C2_ComPort *pPort = (C2_ComPort *)pParam;
	
	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	pPort->m_IsThreadOn	= true;
		
	// Misc. variables
	COMSTAT * nComStat = new COMSTAT;
	DWORD	nEvent	= 0;		
	DWORD	nComEvt	= 0;
	DWORD	dwError	= 0;
	BOOL	bResult	= true;
		
	// Clear comm buffers at startup
	if (pPort->m_hComm)		// check if the port is opened
		PurgeComm(pPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT );

	// begin forever loop. This loop will run as long as the thread is alive.
	for (;;) 
	{ 
		bResult = WaitCommEvent(pPort->m_hComm, &nEvent, &pPort->m_OverLap);
		if (!bResult)  
		{ 
			dwError = GetLastError();
			switch (dwError) 
			{ 
				case ERROR_IO_PENDING:	
					break;
					
				case 87:				
					break;
				
				default:					
					pPort->ProcMsgError("WaitCommEvent()");					
					break;
			}
		}
		else
		{
			//bResult = ClearCommError(pPort->m_hComm, &dwError, &nComStat);
			bResult = ClearCommError(pPort->m_hComm, &dwError, nComStat);
			if (nComStat->cbInQue == 0)
				continue;
		}

		nEvent = WaitForMultipleObjects(3, pPort->m_hEvtArray, false, INFINITE);
		switch (nEvent)
		{
			case 0:		// close event
			 	pPort->m_IsThreadOn	= false;
				AfxEndThread(100);
				break;	
				
			case 1:		// read event
				CWnd *pOwner;
				pOwner = pPort->m_pOwner;

				GetCommMask(pPort->m_hComm, &nComEvt);

			/*	if (nComEvt & EV_CTS   )
					::SendMessage(pOwner->m_hWnd, WMS_COMM_CTS	 , 0, pPort->m_NumPort);
				if (nComEvt & EV_RXFLAG)
					::SendMessage(pOwner->m_hWnd, WMS_COMM_RXFLAG, 0, pPort->m_NumPort);
				if (nComEvt & EV_BREAK )
					::SendMessage(pOwner->m_hWnd, WMS_COMM_BREAK , 0, pPort->m_NumPort);
				if (nComEvt & EV_ERR   )
					::SendMessage(pOwner->m_hWnd, WMS_COMM_ERR	 , 0, pPort->m_NumPort);
				if (nComEvt & EV_RING  )
					::SendMessage(pOwner->m_hWnd, WMS_COMM_RING	 , 0, pPort->m_NumPort);				
			*/	if (nComEvt & EV_RXCHAR)
					ReceiChar(pPort, nComStat);
				break;
				
			case 2:		// write event
				WriteChar(pPort);
				
				GetCommMask(pPort->m_hComm, &nComEvt);
				if (nComEvt & EV_TXEMPTY)
				{
					pPort->m_Status = STAT_PORT_IDLE;
					pPort->m_TimeOut= 0;
				}
				break;

			default:
				break;
		} // end switch
	} // close forever loop
}

//-----------------------------------------------------------------------------
//	If there is a error, give the right message
//-----------------------------------------------------------------------------
void C2_ComPort::ProcMsgError(char *szErr)
{
	CString	szStr;
	CString	szMsg;

	szStr = szErr;	
	szMsg.Format(_T("Lỗi ứng dụng: %s - Port: %d"), szStr, m_NumPort);
	::AfxMessageBox(szMsg);
	return;

/*	char *	szTmp = new char[255];
	LPVOID	lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL, GetLastError(),
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// Default language
				  (LPTSTR) &lpMsgBuf, 0, NULL );

	sprintf_s(szTmp, 254, "Chu y: %s Failed with the following error: \n%s\nPort: %d\n", 
			  szErr, lpMsgBuf, m_NumPort);

	::MessageBoxW(NULL, (LPTSTR)szTmp, _T("Loi ung dung"), MB_ICONSTOP);
	LocalFree(lpMsgBuf);
	delete []szTmp; */
}

//-----------------------------------------------------------------------------
//	Write a character.
//-----------------------------------------------------------------------------
void C2_ComPort::WriteChar(C2_ComPort *pPort)
{
	BOOL	bWrite	= true;
	BOOL	bResult	= true;
	DWORD	nByteTx	= 0;

	ResetEvent(pPort->m_hEvtWrite);

	// Gain ownership of the critical section
	EnterCriticalSection(&pPort->m_csCommSync);

	if (bWrite)
	{
		// Init variables
		pPort->m_OverLap.Offset		= 0;
		pPort->m_OverLap.OffsetHigh	= 0;

		// Clear buffer
		PurgeComm(pPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(pPort->m_hComm,		// Handle to COMM Port
							pPort->m_BuffTx,	// Pointer to message buffer in calling function
							pPort->m_nLenTx,	// Length of message to send
							&nByteTx,			// Where to store the number of bytes sent
							&pPort->m_OverLap);	// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:	// continue to GetOverlappedResults()
					nByteTx	= 0;
					bWrite	= false;
					break;

				default:				// all other error codes
					pPort->ProcMsgError("WriteFile()");
					break;
			}
		} 
		else
			LeaveCriticalSection(&pPort->m_csCommSync);
		
	} // end if(bWrite)

	if (!bWrite)
	{
		bWrite  = true;	
		bResult = GetOverlappedResult( pPort->m_hComm,	// Handle to COMM port 
									  &pPort->m_OverLap,// Overlapped structure
									  &nByteTx,			// Stores number of bytes sent
									  true); 			// Wait flag

		LeaveCriticalSection(&pPort->m_csCommSync);

		// deal with the error code 
		if (!bResult)  
			pPort->ProcMsgError("GetOverlappedResults() in WriteFile()");
		
	} // end if (!bWrite)

	// Verify that the data size send equals what we tried to send
	if ((int)nByteTx != pPort->m_nLenTx)
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", 
			  nByteTx, pPort->m_nLenTx);
	}
}

//-----------------------------------------------------------------------------
// Character received. Inform the owner
//-----------------------------------------------------------------------------
void C2_ComPort::ReceiChar(C2_ComPort *pPort, COMSTAT * nComStat)
{
	BOOL	bRead	= true; 
	BOOL	bResult = true;
	DWORD	dwError = 0;
	DWORD	nByteRx	= 0;
	unsigned char szCharRx;

	for (;;) 
	{ 
		// Check have receive byte
		EnterCriticalSection(&pPort->m_csCommSync);
		//bResult = ClearCommError(pPort->m_hComm, &dwError, &nComStat);
		bResult = ClearCommError(pPort->m_hComm, &dwError, nComStat);
		
		LeaveCriticalSection(&pPort->m_csCommSync);
		if (nComStat->cbInQue == 0)
			break;
		
		EnterCriticalSection(&pPort->m_csCommSync);
		if (bRead)
		{
			// Co the sua lai de doc nhieu byte 
			bResult = ReadFile(pPort->m_hComm,		// Handle to COMM port 
							   &szCharRx,			// RX Buffer Pointer
							   1,					// Read one byte
							   &nByteRx,			// Stores number of bytes read
							   &pPort->m_OverLap);	// pointer to the m_OverLap structure
			
			// deal with the error code 
			if (!bResult)  
			{ 
				dwError = GetLastError();
				switch (dwError) 
				{ 
					case ERROR_IO_PENDING: 	
						bRead = false;
						break;
						
					default:
						pPort->ProcMsgError("ReadFile()");
						break;
				}
			}
			else
				bRead = true;

		}  // close if (bRead)

		if (!bRead)
		{
			bRead	= true;
			bResult = GetOverlappedResult( pPort->m_hComm,	// Handle to COMM port 
										  &pPort->m_OverLap,// Overlapped structure
										  &nByteRx,			// Stores number of bytes read
										  true); 			// Wait flag

			// deal with the error code 
			if (!bResult) 			
				pPort->ProcMsgError("GetOverlappedResults() in ReadFile()");			
		}  // close if (!bRead)

		LeaveCriticalSection(&pPort->m_csCommSync);
		
		// Save recv char to buff
		pPort->m_BuffRx[pPort->m_nLenRx] = szCharRx;
		pPort->m_nLenRx ++;
	//	pPort->m_Status |= STAT_PORT_RECV;

		CheckRecv(pPort);
	} // end forever loop
}

//-----------------------------------------------------------------------------
// HieuDQ: Check data of COM port - this function used defending on format of message
// This class used for NMEA0183 sentences: $RA....<CR><LF>
//-----------------------------------------------------------------------------
void C2_ComPort::CheckRecv(C2_ComPort *pPort)
{	
	// 1: Full buffer, but not have END bytes 
	if (pPort->m_nLenRx >= SIZE_PORT_RECV)		// Note: buffer of length > SIZE_PORT_RECV 
	{
		pPort->m_nLenRx = 0;
		return;
	}
	
	else // Check over buffer length
	if (pPort->m_nLenRx == (SIZE_PORT_RECV-1))
	{
		pPort->m_nLenOn	= pPort->m_nLenRx;
		memcpy(pPort->m_BuffOn, pPort->m_BuffRx, pPort->m_nLenOn);
		pPort->m_nLenRx = 0;

		// marker for error buffer - view only
		memset(pPort->m_BuffOn, 0x00, 4);
		::SendMessage((pPort->m_pOwner)->m_hWnd, WMS_COMM_RXCHAR,			// HWND of port, RX char
			(WPARAM)(pPort->m_TypLink << 8) | pPort->m_NumPort,	            // Type, Number of port
			(LPARAM) pPort->m_nLenOn);							            // Length of block
		return;
	}

	if (pPort->m_nLenRx < 2)
		return;

	// 2: Check for get a data block
	u_char szChar[2];		// 2 byte Head or 2 byte End of buff

	szChar[0] = pPort->m_BuffRx[pPort->m_nLenRx-2];
	szChar[1] = pPort->m_BuffRx[pPort->m_nLenRx-1];

	// 2.1 Check the ARPA block
	if (pPort->m_TypLink == TYPE_LINK_RADA)
	{
		//Header of ARPA =($)
		if (szChar[1] == '$') 
		{
			pPort->m_BuffRx[0] = szChar[1];		// Reset buffer to begin, dua ki hieu !,& len dau chuoi de bat dau xu ly			
			pPort->m_nLenRx = 1;
			return;
		}

		// check END bytes
		if ((szChar[0] != 0x0D) || (szChar[1] != 0x0A))
			return;                              // Kiem tra ky tu ket thuc cua chuoi ky tu, neu ko phai la 0D or OA thi return

		pPort->m_nLenOn	= pPort->m_nLenRx - 2;
		memcpy(pPort->m_BuffOn, pPort->m_BuffRx, pPort->m_nLenOn);
		pPort->m_nLenRx = 0;

		::SendMessage((pPort->m_pOwner)->m_hWnd, WMS_COMM_RXCHAR,
			(WPARAM)(pPort->m_TypLink << 8) | pPort->m_NumPort,
			(LPARAM) pPort->m_nLenOn);	
	}
	else  // Check the AIS block 
	if(pPort->m_TypLink == TYPE_LINK_AIS)
		{
			//Header of AIS =(!)
			if(szChar[1] == '!')
			{
				pPort->m_BuffRx[0] = szChar[1];    // Reset buffer to begin
				pPort->m_nLenRx = 1;
				return;
			}
			//Check END bytes
			if((szChar[0]!= 0x0D) || (szChar[1] != 0x0A))
				return;
			pPort->m_nLenOn = pPort->m_nLenRx - 2;
			memcpy(pPort->m_BuffOn, pPort->m_BuffRx, pPort->m_nLenOn);
			pPort->m_nLenRx = 0;

			::SendMessage((pPort->m_pOwner)->m_hWnd, WMS_COMM_RXCHAR,
				(WPARAM)(pPort->m_TypLink << 8) | pPort->m_NumPort,
				(LPARAM)(pPort->m_nLenOn));
		}
	
} 

//-----------------------------------------------------------------------------
//	Hieu: Send/Recv data to COM port
//-----------------------------------------------------------------------------
void C2_ComPort::SendBlock(char *szBuff, int nLen)
{
	memset(m_BuffTx, 0, SIZE_PORT_SEND);

	m_nLenTx = (nLen < SIZE_PORT_SEND)? nLen:SIZE_PORT_SEND;
	memcpy(m_BuffTx, szBuff, m_nLenTx);

	// set event for write
	SetEvent(m_hEvtWrite);

	m_Status  = STAT_PORT_SEND;
	m_IsSend  = true;
}

int C2_ComPort::ReadBlock(char *szBuff, int nLeng)
{
	if (nLeng < m_nLenOn)
	{
		int	nTemp = m_nLenOn - nLeng;

		memcpy(szBuff, m_BuffOn, nLeng);			// Copy until nLeng		
		memcpy(m_BuffOn, &m_BuffOn[nLeng], nTemp);	// Move end of buffer
		m_nLenOn = nTemp;
	}
	else
	{
		memcpy(szBuff, m_BuffOn, m_nLenOn);			// Copy all buffer		
		nLeng	 = m_nLenOn;
		m_nLenOn = 0;
	}

	m_Status |= STAT_PORT_RECV;		// This port is finish receive
	return nLeng;
}

//-----------------------------------------------------------------------------
//	Hieu: Check status of COM port
//-----------------------------------------------------------------------------
bool C2_ComPort::CheckStatus(BYTE nNumPort)
{
//	assert((nNumPort > 0) && (nNumPort < MAX_NUMB_PORT));
	if ((nNumPort <= 0) || (nNumPort > MAX_NUMB_PORT))
		return false;
		
	HANDLE	hComm = NULL;		// Handle of COM port
	CString	szPort;

	szPort.Format(_T("\\\\.\\COM%d"), nNumPort);
	hComm = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
							   OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hComm == INVALID_HANDLE_VALUE)
		return false;
	else
	{
		CloseHandle(hComm);
		hComm = NULL;
		return true;
	}
}

//-----------------------------------------------------------------------------
// Check status of remote port
//-----------------------------------------------------------------------------
bool C2_ComPort::CheckRemote()
{
	DWORD	dwStatus;
	
	GetCommModemStatus(m_hComm, &dwStatus);

	if ((dwStatus & MS_CTS_ON) || (dwStatus & MS_DSR_ON))
		return true ;
	else
		return false;
}
