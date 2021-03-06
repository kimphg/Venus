//=============================================================================
//
// CLocal.cpp: implementation of the C2_Local class.
//
//	Update 23-06-2011 by Dang Quang Hieu
//	AIC Corporation
//=============================================================================

//#include "StdAfx.h"
#include "CLocal.h"
#include <math.h>

//=============================================================================
//	C2_Local class
//=============================================================================
C2_Local::C2_Local()
{
//	m_szRoot	= "";
//	m_szFile	= "";
//	m_szAccoun	= "";
//	m_szPasswd	= "";

//	m_LinkType	= 0;		// 0:Ethernet, 1:Modem
//	m_szDialup	= "";
//	m_IsCheck	= false;
//	m_SelAdap	= 0;

//	m_UserRole	= 0;
//	m_szPwdTec	= "";
//	m_szPwdSup	= "";

//	m_AlTmPlan	= 30;
//	m_AlTmShip	= 120;
//	m_AlRgPlan	= 100;
//	m_AlRgShip	= 50;
//	m_AlTrPlan	= 0xFF;
//	m_AlTrShip	= 0xFF;
//	m_AlarZone	= 0x0F;

//	m_ReplType	= 0x0F;
//	m_ReplIden	= 0x0F;
//	m_RecdTime	= 600;
//	m_ReplTAN	= 0;

//	m_ComRada   = 0;                 // Serial port of AIS
//	m_ComAIS    = 0;                 // Serial port of Rada ARPA

//	m_RadaPnt.m_Lat	 = long(20.827 * (1<<21)/45);
//	m_RadaPnt.m_Long = long(106.72 * (1<<21)/45);

	m_preFrag = 0;
	m_NumFrag = 1;
	m_Leng	  = 0;	
}

C2_Local::~C2_Local()
{}

//-----------------------------------------------------------------------------
// by [1/128s]
//long C2_Local::GetTimeUTC()
//{
//	SYSTEMTIME	nTime;

//	GetLocalTime(&nTime);
//	return (((nTime.wHour*3600 + nTime.wMinute*60 + nTime.wSecond) << 7) +
//			 (nTime.wMilliseconds << 4)/125);
//}

//-----------------------------------------------------------------------------
//	Return Number of Adapters: 3 IP max
//-----------------------------------------------------------------------------
//int C2_Local::GetAddrLocal(BYTE nMax, QString *szAddr)
//{
//	int		 nAdap;
//	char	 szName[255];
//	char	 szIP[16];
//	HOSTENT *pHostEnt;
//	LPSTR	 lpszIP;
//	struct	 sockaddr_in sAddr;

//	memset(szName, 0, 255);
//	if (gethostname(szName, sizeof(szName)) == SOCKET_ERROR)
//		return 0;

//	if (0 == (pHostEnt = gethostbyname(szName)))
//		return 0;

//	nAdap = 0;
//	while (pHostEnt->h_addr_list[nAdap] && (nAdap < nMax))
//	{
//		memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdap], pHostEnt->h_length);
//		lpszIP = inet_ntoa(sAddr.sin_addr);

//		if (lpszIP)
//		{
//			strcpy_s(szIP, 16, (char *)lpszIP);
//			szAddr[nAdap] = szIP;
//		}
//		nAdap ++;
//	}

//	return nAdap;
//}

//-----------------------------------------------------------------------------
// Get Name of PC
//QString C2_Local::GetNameHost()
//{
//	TCHAR	szHost[255];
//	QString	szName;
//	DWORD	nSize;

//	nSize = sizeof(szHost);
//	if (GetComputerName((LPWSTR)szHost, &nSize))
//		szName = szHost;
//	else
//		szName = "";

//	return szName;
//}

//-----------------------------------------------------------------------------
//bool C2_Local::CheckPwd(BYTE nRole, QString szPwd)
//{
//	if (nRole == 1)	// Technical
//	{
//		if (szPwd.Compare(m_szPwdTec) == 0)
//			return true ;
//		else
//			return false;
//	}
//	else
//	if (nRole == 2)	// Supervisor
//	{
//		if (szPwd.Compare(m_szPwdSup) == 0)
//			return true;
//		else
//			return false;
//	}
//	else			// Operator
//		return true;
//}

////-----------------------------------------------------------------------------
//void C2_Local::ChangePwd(BYTE nRole, QString szNewPwd)
//{
//	if (nRole == 1)
//		m_szPwdTec = szNewPwd;
//	else
//	if (nRole == 2)
//		m_szPwdSup = szNewPwd;

//	SaveFile();
//}

//-----------------------------------------------------------------------------
//	Get file info
//-----------------------------------------------------------------------------
//bool C2_Local::LoadFile()
//{
//	CFile	hFile ;				// Handle of file
//	QString	szPath;

//	szPath = m_szRoot + _T("\\") + m_szFile;
//	if (!hFile.Open(szPath, CFile::modeRead))	// No file / Not load
//		return false;

//	BYTE	szBuff[512];		// Max = 512 byte
//	BYTE	szTemp[64] ;
//	int		nPos, nLen ;
//	bool	bOpen = true;		// Open file

//	nLen = hFile.Read(szBuff, 512);
//	hFile.Close();

//	// 0: Check header: (8 byte = 0xFF)
//	nPos = 0;
//	while ((nPos < 8) && bOpen)
//	{
//		if (szBuff[nPos] != 0xFF)
//			bOpen = false;		// Error of Header
//		else
//			nPos ++;
//	}

//	// 1: Check CAT - LEN (1+2 byte) and (5 byte = 0x00)
//	if (bOpen)
//	{
//		if (szBuff[nPos] != 5)	// CAT = 5: this File Info
//			bOpen = false;		// Error of CAT
//		else
//		if (nLen != ((szBuff[nPos+1] << 8) | szBuff[nPos+2]))
//			bOpen = false;		// Error of LEN

//		nPos += 3;

//		int	i = 0;
//		while ((i < 5) && bOpen)
//		{
//			if (szBuff[nPos+i] != 0x00)
//				bOpen = false;	// Error of End header
//			else
//				i ++;
//		}
//		nPos += 5;
//	}

//	if (!bOpen)
//		return false;

//	::MakeStrCode(&szBuff[nPos], 512-nPos);

//	// 1: Password of Login user	- 32 byte
//	memcpy(szTemp, &szBuff[nPos], 16);		// Technical
//	szTemp[16]	= 0x00;
//	m_szPwdTec  = szTemp;
//	nPos += 16;

//	memcpy(szTemp, &szBuff[nPos], 16);		// Supervisor
//	szTemp[16]	= 0x00;
//	m_szPwdSup  = szTemp;
//	nPos += 16;

//	// 2: IP address of Server	- 40 byte
//	memcpy(m_HostServ.m_LocAddr, &szBuff[nPos], 4);
//	nPos += 4;
//	memcpy(m_HostServ.m_RmtAddr, &szBuff[nPos], 4);
//	nPos += 4;

//	memcpy(szTemp, &szBuff[nPos], 16);		// Account  = 16 byte
//	szTemp[16]	= 0;
//	m_szAccoun	= szTemp;
//	nPos += 16;

//	memcpy(szTemp, &szBuff[nPos], 16);		// Password = 16 byte
//	szTemp[16]	= 0;
//	m_szPasswd	= szTemp;
//	nPos += 16;

//	// 3: Dial-up type		- 38 byte
//	m_LinkType	= szBuff[nPos];
//	nPos += 2;
//	m_SelAdap	= m_LinkType & 1;
//	memcpy(szTemp, &szBuff[nPos], 36);		// max leng = 36 byte
//	szTemp[36]	= 0;
//	m_szDialup	= szTemp;
//	nPos += 36;

//	// 3: Warning infor.	- 12 byte
//	m_AlTmPlan	= (szBuff[nPos  ] << 8) | szBuff[nPos+1];
//	m_AlTmShip	= (szBuff[nPos+2] << 8) | szBuff[nPos+3];
//	nPos += 4;

//	m_AlRgPlan	= (szBuff[nPos  ] << 8) | szBuff[nPos+1];
//	m_AlRgShip	= (szBuff[nPos+2] << 8) | szBuff[nPos+3];
//	nPos += 4;

//	m_AlTrPlan	= szBuff[nPos  ];
//	m_AlTrShip	= szBuff[nPos+1];
//	m_AlarZone	= szBuff[nPos+2];
//	nPos += 4;

//	// 4: Replay type	- 4 byte
//	m_ReplType	= szBuff[nPos  ];
//	m_ReplIden	= szBuff[nPos+1];
//	m_RecdTime	=(szBuff[nPos+2] << 8) | szBuff[nPos+3];
//	nPos += 4;

//	// 5: ComInfo		- 6 byte
//    for (int i = 0; i < 16; i++)
//    {
//		m_ComInfo[i].m_NumbPort	= i+1;
//		m_ComInfo[i].m_IdBaudRat= szBuff[nPos+1];
//		m_ComInfo[i].m_IdBitData= szBuff[nPos+2];
//		m_ComInfo[i].m_IdBitStop= szBuff[nPos+3];
//		m_ComInfo[i].m_IdParity	= szBuff[nPos+4];
//		m_ComInfo[i].m_IdFlwCtrl= szBuff[nPos+5];
//		nPos += 6;
//    }

//	// Connect to ARPA/AIS
//	m_ComRada	= szBuff[nPos++];
//	m_ComAIS	= szBuff[nPos++];
	
//    // Tọa độ Radar
//	m_RadaPnt.m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | szBuff[nPos+2];
//	m_RadaPnt.m_Long= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | szBuff[nPos+5];
//	nPos += 6;

//	// 6: Connect Radar by UDP
//	memcpy(m_HostRada.m_LocAddr, &szBuff[nPos], 4);
//	nPos += 4;

//	m_HostRada.m_LocPort = (szBuff[nPos  ] << 8) | szBuff[nPos+1];
//	nPos += 2;

//	m_HostRada.m_RmtPort = (szBuff[nPos  ] << 8) | szBuff[nPos+1];
//	nPos += 2;

//	return true;
//}

//-----------------------------------------------------------------------------
// Save file Info
//-----------------------------------------------------------------------------////
//bool C2_Local::SaveFile()
//{
//	CFile		hFile;
//	CFileStatus	hStat;			// handle of file status
//	QString		szPath;			//

//	szPath = m_szRoot + _T("\\") + m_szFile;

//	// Clear the Read Only
//	if (CFile::GetStatus(szPath, hStat))
//	{
//		hStat.m_attribute = 0x00;
//		CFile::SetStatus(szPath, hStat);
//	}

//	if (!hFile.Open(szPath, CFile::modeCreate | CFile::modeWrite))
//		return false;

//	// Read from file
//	BYTE	szBuff[512];		// max = 512 byte
//	int		nPos;

//	memset(szBuff, 0, 512);

//	// Header: 16 byte
//	nPos = 0;
//	memset(&szBuff[nPos], 0xFF, 8);
//	nPos += 8;
//	nPos += 3;	// CAT - LEN
//	memset(&szBuff[nPos], 0x00, 5);
//	nPos += 5;

//	// 1: Save password:	- 32
//	::ConvStrBuff(m_szPwdTec, &szBuff[nPos], 16);	// Technical
//	nPos += 16;

//	::ConvStrBuff(m_szPwdSup, &szBuff[nPos], 16);	// Supervisor
//	nPos += 16;

//	// 2: IP address of Server	- 40
//	memcpy(&szBuff[nPos], m_HostServ.m_LocAddr, 4);
//	nPos += 4;
//	memcpy(&szBuff[nPos], m_HostServ.m_RmtAddr, 4);
//	nPos += 4;

//	::ConvStrBuff(m_szAccoun, &szBuff[nPos], 16);	// Account
//	nPos += 16;

//	::ConvStrBuff(m_szPasswd, &szBuff[nPos], 16);	// Password
//	nPos += 16;

//	// 3: Link dial-up	- 38
//	szBuff[nPos  ]	= m_LinkType;
//	nPos += 2;

//	ConvStrBuff(m_szDialup, &szBuff[nPos], 36);
//	nPos += 36;

//	// 3: Warning infor.	- 12
//	szBuff[nPos++]	= BYTE(m_AlTmPlan >> 8);
//	szBuff[nPos++]	= BYTE(m_AlTmPlan	  );
//	szBuff[nPos++]	= BYTE(m_AlTmShip >> 8);
//	szBuff[nPos++]	= BYTE(m_AlTmShip	  );

//	szBuff[nPos++]	= BYTE(m_AlRgPlan >> 8);
//	szBuff[nPos++]	= BYTE(m_AlRgPlan	  );
//	szBuff[nPos++]	= BYTE(m_AlRgShip >> 8);
//	szBuff[nPos++]	= BYTE(m_AlRgShip	  );

//	szBuff[nPos++]	= m_AlTrPlan;
//	szBuff[nPos++]	= m_AlTrShip;
//	szBuff[nPos++]	= m_AlarZone;
//	szBuff[nPos++]	= 0;

//	// 4: Replay type	- 4
//	szBuff[nPos++]	= m_ReplType;
//	szBuff[nPos++]	= m_ReplIden;
//	szBuff[nPos++]	= BYTE(m_RecdTime >> 8);
//	szBuff[nPos++]	= BYTE(m_RecdTime	  );

//	// 5: ComInfo		- 6 byte
//	for (int i = 0; i < 16; i++)
//	{
//		szBuff[nPos  ]	= m_ComInfo[i].m_NumbPort ;
//		szBuff[nPos+1]	= m_ComInfo[i].m_IdBaudRat;
//		szBuff[nPos+2]	= m_ComInfo[i].m_IdBitData;
//		szBuff[nPos+3]	= m_ComInfo[i].m_IdBitStop;
//		szBuff[nPos+4]	= m_ComInfo[i].m_IdParity ;
//		szBuff[nPos+5]	= m_ComInfo[i].m_IdFlwCtrl;
//		nPos += 6;
//	}

//	// Connect to ARPA/AIS - User ports
//	szBuff[nPos++] = m_ComRada;
//	szBuff[nPos++] = m_ComAIS ;

//	szBuff[nPos  ] = BYTE(m_RadaPnt.m_Lat  >> 16);
//	szBuff[nPos+1] = BYTE(m_RadaPnt.m_Lat  >>  8);
//	szBuff[nPos+2] = BYTE(m_RadaPnt.m_Lat		);
//	szBuff[nPos+3] = BYTE(m_RadaPnt.m_Long >> 16);
//	szBuff[nPos+4] = BYTE(m_RadaPnt.m_Long >>  8);
//	szBuff[nPos+5] = BYTE(m_RadaPnt.m_Long		);
//	nPos += 6;

//	//6: Connect Radar by UDP
//	memcpy(&szBuff[nPos], m_HostRada.m_LocAddr, 4);
//	nPos += 4;

//	szBuff[nPos  ] = BYTE(m_HostRada.m_LocPort >> 8);
//	szBuff[nPos+1] = BYTE(m_HostRada.m_LocPort     );
//	nPos += 2;

//	szBuff[nPos  ] = BYTE(m_HostRada.m_RmtPort >> 8);
//	szBuff[nPos+1] = BYTE(m_HostRada.m_RmtPort     );
//	nPos += 2;

//	::MakeStrCode(&szBuff[32], 512-32);

//	szBuff[ 8] = 5;					// CAT = 5 Area
//	szBuff[ 9] = BYTE (nPos >> 8);	// LEN
//	szBuff[10] = BYTE (nPos     );

//	hFile.Write(szBuff, nPos);
//	hFile.Close();

//	return true;
//}

//-----------------------------------------------------------------------------
//	Data from ComPort: ($--TTM,xx,x.x,x.x,a,x.x,x.x,a,x.x,x.x,a,c--c,a,a*hh)
//  Hàm GetTrackRada để lọc tham số quỹ đạo mục tiêu của rada ARPA
//-----------------------------------------------------------------------------
//int C2_Local::GetTrackRada(BYTE *szBuff, int nLeng, CC_Track* pCTrack)
//{
//	C2_Track *	pTrack;		// new track
//	int			nPos;
//	int			nRec;		// length of record

//	nPos = 0;
//	while (nPos < (nLeng-12))			// 11 = min of record $RATTM/$RAOSD/$RARSD
//	{
//		if (szBuff[nPos] != '$')
//		{
//			nPos ++;
//			continue;
//		}
		
//		pTrack = new C2_Track();
//		if (!pTrack)
//			return 0;

//		if (!GetTrackRada(&szBuff[nPos], nLeng - nPos, pTrack, nRec))
//			delete pTrack;
//		else
//			pCTrack->AddTail(pTrack);
	
//		nPos += nRec;
//	}
//	return pCTrack->Count();
//}

// Get track from message - format:($--TTM ....<CR><LF>)
int C2_Local::GetTrackRada(BYTE *szBuff, int nLeng, C2_Track* pTrack, int &nRec)
{	
	// Checksum field test 
/*	if (!IsCheckSum(szBuff, nLeng))
		return 0;	*/

    QString szStr;
	int		nPos = 0;		// in buffer
	int		nTmp;
	int		bRead;			// is get record

	// 0: Get header : $RATTM
	szStr = "";
	while (nPos < nLeng)
	{ 
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;

    if ((szStr.contains(("$RATTM")) != 0) && (szStr.contains(("$RAOSD")) != 0) &&
        (szStr.contains(("$RARSD")) != 0))		// && (szStr.contains(("$GPGGA")) != 0) && (szStr.contains(("$GPZDA")) != 0) && (szStr.contains(("$GPVTG")) != 0) &&	(szStr.contains(("$GPDTM")) != 0))
	{
		nRec = nPos;
		return 0;
	}                                

    if (szStr.contains(("$RATTM")) == 0)
	{
		bRead = GetTrkTTM(&szBuff[nPos], nLeng - nPos, pTrack, nTmp);
		nRec  = nPos + nTmp;
		return (bRead != 0)? 1:0;
	}

    if (szStr.contains(("$RAOSD")) == 0)
	{
		bRead = GetTrkOSD(&szBuff[nPos], nLeng - nPos, pTrack, nTmp);
		nRec  = nPos + nTmp;
		return 0;			// not track
	}	

    if (szStr.contains(("$RARSD")) == 0)
	{
		bRead = GetTrkRSD(&szBuff[nPos], nLeng - nPos, pTrack, nTmp);
		nRec  = nPos + nTmp;
		return 0;
	}

    //if (szStr.contains(("$GPGGA")) == 0)
	//	GetTrkGGA(szBuff, nLeng, pTrack, nPos);

    //if (szStr.contains(("$GPZDA")) == 0)
	//	GetTimeZDA(szBuff, nLeng, pTrack, nPos); 

    //if (szStr.contains(("$GPVTG")) == 0)
	//	GetTrkVTG(szBuff, nLeng, pTrack, nPos);

    //if (szStr.contains(("$GPDTM")) == 0)
	//	GetTrkDTM(szBuff, nLeng, pTrack, nPos); 
  
	return 0;
}
//-----------------------------------------------------------------------------
// OnLinkBuff  AIS
//-----------------------------------------------------------------------------
int C2_Local::OnLinkBuff(BYTE *szBuff, int nLeng, int &nRec)
{
    QString  szStr;
	int      nPos;
	int      nEndOfData;
	int      nNumFrag, nCurFrag;
	int      nComm = 0;                  // Số dấu phẩy ','

	// 0: Get header : !AIVDM and !AIVDO
	szStr  = "";
	nPos   = 0 ;
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nComm++;
	nPos ++;

    if ((szStr.contains(("!AIVDM")) != 0) && (szStr.contains(("!AIVDO")) != 0))	 // Not AIVDM or !AIVDO
	{
		nRec = nPos; 
		return 0;           
	}
	//1: Count of fragment
	nNumFrag  = szBuff[nPos++] - 48;   // Tổng số mảnh  

	if ((nNumFrag <= 0) || (nNumFrag > 9))		 // ( 0< nNumFrag <9 _Add Tien 23-7-2015)
	{
		nRec = nPos; 
		return 0;					  
	}
	
	m_NumFrag = nNumFrag; 
	nComm++;
	nPos ++;

	nCurFrag  = szBuff[nPos++] - 48;    // Số thứ tự mảnh

	if ((nCurFrag <= 0)	|| (nCurFrag > 9))				// ( 0< nNumFrag <9 _Add Tien)
	{	
		nRec = nPos; 
		return 0;
	}

	nComm++;
	nPos ++;

	// Find the begin of data payload
	while (nComm < 5)
	{
		while ((nPos < nLeng) && (szBuff[nPos] != ','))
			nPos++;
	
		nPos ++;
		nComm++;
	}

 	if (nPos > nLeng)			// Test again 
		return 2;				// Error all of buffer

	// Find the end of data payload
	nEndOfData = nPos;
	while ((nEndOfData < nLeng) && (szBuff[nEndOfData] != ','))
		nEndOfData ++;

	nRec = nEndOfData;		

	if (m_NumFrag > 1)								// Tổng số mảnh nhiều hơn 1 (n packet) 	  
	{
		while (nCurFrag < m_NumFrag)				// packet 1,2,..(n-1) _Add by Tien 23-7-2015 (1-9)
		{
			if (nCurFrag == 1)
			{
				memcpy(m_Buff, &szBuff[nPos], nEndOfData - nPos);
				m_Leng = nEndOfData - nPos;
				m_preFrag = 1; 
			}
			else
			{	
				if (nCurFrag - 1 != m_preFrag)		// Oder packet not correctly !AIVDM,2,1,...!AIVDM,1,1,...!AIVDM,2,2,...
					return 0; 

				memcpy(&m_Buff[m_Leng], &szBuff[nPos], nEndOfData - nPos); 
				m_Leng		+= nEndOfData - nPos; 
				m_preFrag	+= 1; 				 
			}
			//nRec = nEndOfData
			return 0;
		}											// packet n 
			if (nCurFrag -1 != m_preFrag)			// Oder packet not correctly !AIVDM,2,1,...!AIVDM,1,1,...!AIVDM,2,2,...
				return 0; 
			memcpy(&m_Buff[m_Leng], &szBuff[nPos], nEndOfData - nPos); 
			m_Leng		+= nEndOfData - nPos; 
			m_preFrag	+= 1;
			//nRec = nEndOfData; 
			return 1;									
	}
	else                        // Only 1 fragment
	{
		if (m_preFrag != 0)
			memset(m_Buff, 0, 2048);		// delete data of packet previous 
		memcpy(m_Buff, &szBuff[nPos], nEndOfData - nPos);  
		m_Leng = nEndOfData - nPos;
		//nRec   = nEndOfData; 
	}
	
	//nRec = nEndOfData; 
	return 1;
}

//-----------------------------------------------------------------------------
//	Track from AIS: 
//	IdSrc | IdMsg | Class | MMSI  | IMO   | IdNati| VesTyp| DimA  | DimB  | Dim C | 
//	DimD  | Draug | ROT   | Speed | Head  | Long  | Lati  | NavSt | HeadTr| CallSg|
//	Name  | ETArr |	Desti | TimeRx| 
//-----------------------------------------------------------------------------
//int C2_Local::GetTrackAIS(BYTE *szBuff, int nLeng, CC_Track* pCTrack)
//{
//	C2_Track *	pTrack;
//	int			nPos, nRec, nRec1;

//	nPos = 0;
//	while (nPos < (nLeng - 12))		// 12 = min of AIS record: !AIDVO / !AIDVM
//	{
//		if (szBuff[nPos] != '!')	// Find char '!' of string
//		{
//			nPos++;
//			continue;
//		}

//		pTrack = new C2_Track();
//		if (!pTrack)
//			return 0;

//		// connect packet & get full packet
//		if (!OnLinkBuff(&szBuff[nPos], nLeng - nPos, nRec))
//		{
//			nPos+= nRec;
//			continue;
//		}

//		if (!GetTrackAIS(m_Buff, m_Leng, pTrack, nRec1))		// nRec?
//			delete pTrack;
//		else
//			pCTrack->AddTail(pTrack);

//		memset(m_Buff,0,2048);		// delete data buff
//		m_preFrag = 0;				// delete value of Flag

//		//nRec = nLeng;				// Tam dung chỉ get 1 goi
//		nPos += nRec;
//	}
//	return pCTrack->Count();
//}

int C2_Local::GetTrackAIS(BYTE *szBuff, int nLeng, C2_Track* pTrack, int &nRec)
{
	int     nPos  = 0;			// Read position in buffer  
	int		nTmp;
	BYTE    nMssg = 0;			// Type of AIS message
	int		bRead;
 
	// 1: Message ID: 1-2 byte (1,2,3,4,5,18,19)
	nMssg = Decode(szBuff[nPos++]);	 
	nRec  = 1;
	switch (nMssg)
	{
		case  1:	// Position Report Class A
		case  2:
		case  3: 
			bRead = GetTrkClassA(szBuff, nPos, pTrack, nTmp); break;

		case  4:	// Base station report
			bRead = GetTrkStatio(szBuff, nPos, pTrack, nTmp); break; 

		case  5:	// Static and voyage data
			bRead = GetTrkStatic(szBuff, nPos, pTrack, nTmp); break;

		case 18:	// Position Report Standard Class B 
			bRead = GetTrkClassB(szBuff, nPos, pTrack, nTmp); break; 

		case 19:	// Extended Class B CS Position Report
			bRead = GetTrkClasEB(szBuff, nPos, pTrack, nTmp); break; 

		case 21:	// Aid - to - Navigation Report
			bRead = GetTrkNaviga(szBuff, nPos, pTrack, nTmp); break;

		case 24:	// Type = 24 Static data report
			bRead = GetTrkType24(szBuff, nPos, pTrack, nTmp); break;

		default:
			return 0;
	}

	nRec += nTmp;
	pTrack->m_TypAIS = nMssg;

	return bRead? 1:0;
}

BYTE C2_Local::Decode(BYTE nBuff)
{
	if (nBuff < 48)					// not necessary 
		return nBuff;

	if ((nBuff - 48) > 40)
		return (nBuff - 56);	

	return (nBuff - 48);
}

int	C2_Local::GetTrkClassA(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString szNum;
	int     nLen, nOld;

	nOld = nPos;		// Get begin of buffer

	// 1: Type AIS
	pTrack->m_Fspec[0] |= FRN_1;
	pTrack->m_Fspec[2] |= FRN_15;

	// 2: Repeat indicator: bit 6, 7
	int		rep_ind = (Decode(szBuff[nPos]) & 0x30) >> 4;		// >> 4 bit	_Add by Tien 

	// 3: MMSI: bit 8-37
	long	nMMSI;
	nMMSI = ((Decode(szBuff[nPos  ]) & 0x0F) << 26) | ((Decode(szBuff[nPos+1]) & 0x3F) <<20) |
		    ((Decode(szBuff[nPos+2]) & 0x3F) << 14) | ((Decode(szBuff[nPos+3]) & 0x3F) << 8) |
		    ((Decode(szBuff[nPos+4]) & 0x3F) <<  2) | ((Decode(szBuff[nPos+5]) & 0x30) >> 4) ;
	nPos += 5;

    szNum = QString::number( nMMSI);
    nLen = szNum.size();

	if ((nLen < 4) || (nLen > 9))
		return 0;
    memcpy(pTrack->m_MMSI,szNum.toAscii().data(), 9);
//	::ConvStrBuff(szNum, pTrack->m_MMSI, 9);
	pTrack->m_Fspec[2] |= FRN_16;	

	if      (nLen == 9)
		pTrack->m_Type = 0x12;      // Auto + Vessel
	else if (nLen == 7)
		pTrack->m_Type = 0x13;      // Auto + Terrain
	else
		pTrack->m_Type = 0x10;      // Auto + Default

	// 4: Navigator status: bit 38 - 41
	BYTE	nNaviSt = Decode(szBuff[nPos++]) & 0x0F;
	pTrack->m_NaviSt	= ConvNaviStat(nNaviSt); 
	pTrack->m_Fspec[1] |= FRN_12;

	// 5: Rate of turn: bit 42 - 49: 8 bit	_Repaired by Tien 8/8/2015
	int		nROT;
	nROT = ((Decode(szBuff[nPos]) & 0x3F) << 2)|
		   ((Decode(szBuff[nPos+1]) & 0x30)>> 4);
	
	if ((nROT >> 7) & 0x01)						// Sign bit == 1	
		nROT = - (((~nROT) & 0x000000FF) + 1);	// convert 2's comple to decimal of 8 bit	
	
	if (nROT == -128)
		pTrack->m_ROT = 0;									// No information 
	else
	if (nROT == 127)
		pTrack->m_ROT = 10;									// Turning right 
	else
	if (nROT == -127)
		pTrack->m_ROT = -10;								// Turning left 
	else
	if ((0<= nROT)&& (nROT <= 126))
        pTrack->m_ROT = int ((nROT/4.733)*(nROT/4.733));	// Turning right
	else
        pTrack->m_ROT = - int((nROT/4.733)*(nROT/4.733));	// Turning left
		
	pTrack->m_Fspec[2] |= FRN_18;
	nPos ++;

	// 6: Speed over ground: 50 - 59
	pTrack->m_Speed = ((Decode(szBuff[nPos]) & 0x0F) << 6) | (Decode(szBuff[nPos+1]) & 0x3F); 
	pTrack->m_Fspec[1] |= FRN_9;
	nPos += 2;

	// 7: Position accuracy: bit 60
	bool bPos = ((Decode(szBuff[nPos]) & 0x20) >> 5) & 1;
	
	// 8: Long: 61 - 88: 28 bit type 2's complement 
	long nLong; 
	long nTmp ;	 // Temp 

	nLong = ((Decode(szBuff[nPos  ]) & 0x1F) << 23) | ((Decode(szBuff[nPos+1]) & 0x3F)<<17) |
				 ((Decode(szBuff[nPos+2]) & 0x3F) << 11) | ((Decode(szBuff[nPos+3]) & 0x3F)<< 5) |
				  (Decode(szBuff[nPos+4]) >> 1) & 0x1F;	

	if ((nLong >> 27) & 0x01)							// Nagative number
		nTmp = -(((~nLong)& 0x0FFFFFFF) + 1 );
	else
		nTmp = nLong; 	

	pTrack->m_Long = long(nTmp / 421875.0 * (1 << 15));       // Minute

	pTrack->m_Fspec[1] |= FRN_8;
	nPos += 4; 

	// 9: Lat: 89 - 115: 27 bit represent by 2's complement
	long	nLat;
	nLat = ((Decode(szBuff[nPos  ]) & 1   ) << 26) | ((Decode(szBuff[nPos+1]) & 0x3F)<<20) |
		   ((Decode(szBuff[nPos+2]) & 0x3F) << 14) | ((Decode(szBuff[nPos+3]) & 0x3F)<< 8) |
	       ((Decode(szBuff[nPos+4]) & 0x3F) <<  2) | ((Decode(szBuff[nPos+5]) & 0x30)>> 4); 

	if ((nLat >> 26) & 0x01)							// Nagative 
		nTmp = -(((~nLat) & 0x07FFFFFF) + 1);
	else
		nTmp = nLat; 

	pTrack->m_Lat  =  long(nTmp / 421875.0 * (1 << 15));        // degree//* bit23/180;

	nPos += 5;

	// 10: Course over ground: 116 - 127 :10
	long	nCOG;
	nCOG = ((Decode(szBuff[nPos  ]) & 0x0F) << 8) | ((Decode(szBuff[nPos+1]) & 0x3F) << 2)|
	       ((Decode(szBuff[nPos+2]) & 0x03) >> 4);
	nPos += 2;
	pTrack->m_Head = UINT(nCOG * (1<<12)/225);	// bit15/1800);

	// 11: true heading: 128 - 136
	int		nHeadTr;
	nHeadTr = ((Decode(szBuff[nPos ]) & 0x0F) << 5) | ((Decode(szBuff[nPos+1]) & 0x3E)/2);
	pTrack->m_HeadTr = nHeadTr * bit15/180;
	pTrack->m_Fspec[2] |= FRN_19;
	nPos ++;

	//12: Time Stamp: 137 - 142
	BYTE	nTimeStp;
	nTimeStp = ((Decode(szBuff[nPos]) & 1) << 5) | ((Decode(szBuff[nPos+1]) & 0x3E )/2); 
	nPos ++;

	// 13: Maneuver indicator: 143 -144
	BYTE	nManInd;
	nManInd  = ((Decode(szBuff[nPos]) & 1)*2) | ((Decode(szBuff[nPos+1]) & 0x20)>>5);
	nPos ++;

	// 14: Spare: 145 - 147
	BYTE	nSpare;
	nSpare = (Decode(szBuff[nPos]) & 0x1C) >> 2;

	// 15:RAIM flag: bit 148
	bool	bRaimFl;
	bRaimFl = ((Decode(szBuff[nPos]) & 0x02) >> 1) & 1;

	// 16: Radio status: 149 - 167	: SOTDMA communication state 
	long	nRadiSt;
	nRadiSt = ((Decode(szBuff[nPos]  ) & 0x01) << 18) |((Decode(szBuff[nPos+1]) & 0x3F) << 12)|
			  ((Decode(szBuff[nPos+2]) & 0x3F) <<  6) | (Decode(szBuff[nPos+3]) & 0x3F);
	nPos += 3;

	nRec = nPos - nOld;	
	return 1;
}

//-----------------------------------------------------------------------------
int C2_Local::GetTrkStatio(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
	int     nLen;
	int		nOld = nPos;

	// 1: TypeAIS
	pTrack->m_Fspec[0] |= FRN_1;
	pTrack->m_Fspec[2] |= FRN_15;

	// 2: Repeat indicator: bit 6, 7
	int rep_ind;
	rep_ind = (Decode(szBuff[nPos]) & 0x30) >> 4;	//_Add by Tien 25-7-2015

	// 3: MMSI: bit 8-37
	long nMMSI;
	nMMSI = ((Decode(szBuff[nPos  ]) & 0x0F) << 26)|((Decode(szBuff[nPos+1]) & 0x3F) << 20)|
		    ((Decode(szBuff[nPos+2]) & 0x3F) << 14)|((Decode(szBuff[nPos+3]) & 0x3F) << 8) |
		    ((Decode(szBuff[nPos+4]) & 0x3F) <<  2)|((Decode(szBuff[nPos+5]) & 0x30) >> 4);
	nPos += 5;

    szNum = QString::number( nMMSI);
    nLen = szNum.size();

	if ((nLen < 4)||(nLen > 9))
		return 0;
    memcpy(pTrack->m_MMSI,szNum.toAscii().data(), 9);
//	::ConvStrBuff(szNum, pTrack->m_MMSI, 9);
	pTrack->m_Fspec[2] |= FRN_16;

	if		(nLen == 9)
		pTrack->m_Type	= 0x12;			// Auto + Vessel
	else if (nLen == 7)
		pTrack->m_Type	= 0x13;			// Auto + Terrain
	else
		pTrack->m_Type	= 0x10;			// Auto + Default	

	// 4: Year (UTC): bit 38-51
	int nYear;
	nYear = ((Decode(szBuff[nPos]) & 0x0F)<< 10)|((Decode(szBuff[nPos+1]) & 0x3F)<< 4)|
		    ((Decode(szBuff[nPos+2]) >> 2)& 0x0F);
	nPos += 2;

	// 5: Month(UTC): bit 52-55
	int nMonth;
	nMonth = ((Decode(szBuff[nPos]) & 0x03) << 2)|((Decode(szBuff[nPos+1]) & 0x30) >> 4); 
	nPos ++;

	// 6: Day (UTC): bit 56-60
	int nDay;
	nDay = ((Decode(szBuff[nPos]) & 0x0F) << 1)|((Decode(szBuff[nPos+1]) & 0x02) >> 5); 
	nPos ++;

	// 7: Hour(UTC): bit 61-65
	int nHour;
	nHour = Decode(szBuff[nPos]) & 0x1F;
	nPos++;

	// 8: Minute(UTC): Bit 66-71
	int nMin;
	nMin = Decode(szBuff[nPos]) & 0x3F;
	nPos++;

	// 9: Second (UTC): 72-77
	int nSecond;
	nSecond = Decode(szBuff[nPos]) & 0x3F;
	nPos++;

	// 10: fix quality: bit 78
	bool bQual = (Decode(szBuff[nPos]) >> 5) & 1;

	// 11: Longitude:bit 79-106: 28 bit
	long nLong;
	long nTmp ;

	nLong = ((Decode(szBuff[nPos  ]) & 0x1F) << 23) | ((Decode(szBuff[nPos+1]) & 0x3F) <<17)  |
		         ((Decode(szBuff[nPos+2]) & 0x3F) << 11) | ((Decode(szBuff[nPos+3]) & 0x3F) << 5)|
		         ((Decode(szBuff[nPos+4]) & 0x3E)/2);

	if ((nLong >> 27) & 0x01)		// Nagative number
		nTmp = -(((~nLong)& 0x0FFFFFFF) + 1 );
	else
		nTmp = nLong; 	
	
	pTrack->m_Long = long(nTmp / 421875.0 * (1 << 15));         // minute
	pTrack->m_Fspec[1] |= FRN_8;
	nPos += 4; 

	//12: Latitude: bit 107-133
	long	nLat;
	nLat = ((Decode(szBuff[nPos  ]) & 1   ) << 26) | ((Decode(szBuff[nPos+1]) & 0x3F) << 20) |
		   ((Decode(szBuff[nPos+2]) & 0x3F) << 14) | ((Decode(szBuff[nPos+3]) & 0x3F) << 8 ) |
	       ((Decode(szBuff[nPos+4]) & 0x3F) <<  2) | ((Decode(szBuff[nPos+5]) & 0x30) >> 4);

	if ((nLat >> 26) & 0x01)		// Nagative 
		nTmp = -(((~nLat) & 0x07FFFFFF) + 1);
	else
		nTmp = nLat;

	pTrack->m_Lat  =  long(nTmp / 421875.0 * (1 << 15));        // degree//* bit23/180; 
	nPos += 5;

	// 13: Type of EPFD:bit  134-137
	BYTE m_EPFD;
	m_EPFD = Decode(szBuff[nPos]) & 0x0F;
	nPos ++;

	// 14: Spare :bit 138-147
	BYTE m_Spare;
	m_Spare = ((Decode(szBuff[nPos])& 0x3F) << 4) | ((Decode(szBuff[nPos+1])>>2)& 0x0F);
	nPos++;

	// 15:RAIM flag: bit 148
	bool bRAIM = (Decode(szBuff[nPos]) >> 1) & 1;

	// 16: SOTDMA state: 149- 167
	long nRadiSt;
	nRadiSt = ((Decode(szBuff[nPos  ]) & 0x01) << 18) | ((Decode(szBuff[nPos+1]) & 0x3F) << 12)|
		      ((Decode(szBuff[nPos+2]) & 0x3F) <<  6) |  (Decode(szBuff[nPos+3]) & 0x3F);
	nPos += 3;	

	nRec = nPos - nOld;
	return 1;
}

int C2_Local::GetTrkStatic(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
	int     nLen;
	int		nOld = nPos;

	//1: Type AIS
	pTrack->m_Fspec[0]   |= FRN_1;
	pTrack->m_Fspec[2]   |= FRN_15;
    
	//2: Repeat indicator: bit 6,7
	int rep_ind;
	rep_ind = (Decode(szBuff[nPos]) & 0x30) >> 4;

	//3: MMSI: bit 8 - 37
	long nMMSI;
	nMMSI = ((Decode(szBuff[nPos]) & 0x0F)   << 26)| ((Decode(szBuff[nPos+1])&0x3F)<<20)|
		    ((Decode(szBuff[nPos+2]) & 0x3F) << 14)| ((Decode(szBuff[nPos+3])&0X3F)<<8) |
			((Decode(szBuff[nPos+4]) & 0X3F) << 2) | ((Decode(szBuff[nPos+5])&0x30)>>4)  ;
	nPos += 5;

    szNum = QString::number( nMMSI);
    nLen = szNum.size();

    if ((nLen < 4)||(nLen > 9))
        return 0;

    memcpy( pTrack->m_MMSI,szNum.toAscii().data(), 9);
	pTrack->m_Fspec[2] |= FRN_16;

	if		(nLen == 9)
		pTrack->m_Type	= 0x12;			// Auto + Vessel
	else if (nLen == 7)
		pTrack->m_Type	= 0x13;			// Auto + Terrain
	else
		pTrack->m_Type	= 0x10;			// Auto + Default

	//4:AIS Version bit 38 - 39
	BYTE nVersion = (Decode(szBuff[nPos])>> 2) & 0x03;

	//5: IMO number bit 40 -69
	long nIMO;
	nIMO = ((Decode(szBuff[nPos  ])& 0x03) << 28)|((Decode(szBuff[nPos+1])& 0x3F) << 22)|
		   ((Decode(szBuff[nPos+2])& 0x3F) << 16)|((Decode(szBuff[nPos+3])& 0x3F) << 10)|
		   ((Decode(szBuff[nPos+4])& 0x3F) << 4) |((Decode(szBuff[nPos+5])>> 2) & 0x0F) ;
	nPos += 5;
	 
	// Check value of IMO
	if ((nIMO < 1000000) || (nIMO > 9999999))	// IMO not used of _Add by Tien 
		return 0; 	
	//if (szNum.GetLength() > 7)					// not correctly 
	//	return 0;
    szNum = QString::number( nIMO);//szNum.Format(_T("%d"), nIMO);//qt compatible add by Phuong
    //::ConvStrBuff(szNum, pTrack->m_IMO, 7);
    memcpy( pTrack->m_IMO,szNum.toAscii().data(), 7);//qt compatible add by Phuong
	pTrack->m_Fspec[2]	  |= FRN_20;
	
	//6: Call Sign bit 70 - 111	  : Hô hiệu
	char	sCallsign[8];
	sCallsign[7] = 0x00 ;	
	char ch;
	for (int j = 0; j < 7; j++)
	{		
		ch = (((Decode(szBuff[nPos + j  ])& 0x03) << 4 )|			// Repair by Tien 26-7-2015
			 ((Decode(szBuff[nPos + j + 1]) >> 2) & 0x0F ));

		if (ch == 0)
		{
         sCallsign[j] = 0x00;
		 break;
		}
		if (ch < 32)    
			ch += 64;
		sCallsign[j] = ch;
	}
	pTrack->m_szClSg     = sCallsign;
//	pTrack->m_szClSg.Trim();					// delete space
	pTrack->m_Fspec[3]	|= FRN_22;
	nPos += 7;

	//7: Vessel Name : bit 112-231	
	char	szName[21];	
	szName[20] = 0x00;
	for (int j = 0; j < 20; j++)
	{
		ch = (((Decode(szBuff[nPos + j    ])& 0x03) << 4 )|			// Repair by Tien 26-7-2015
			((Decode(szBuff[nPos + j + 1]) >> 2) & 0x0F ));

		if (ch == 0)	               // '@' - finish
		{
			szName[j] = 0x00;
			break;
		}
		if (ch < 32)
			ch += 64;
		szName[j] = ch;
	}
	pTrack->m_szName = szName;
//	pTrack->m_szName.Trim();					// delete space
	pTrack->m_Fspec[3]	|= FRN_23;
	nPos += 20;

	//8: Ship Type: bit 232-239 
	BYTE nType;
	nType = ((Decode(szBuff[nPos])& 0x03) << 6) | (Decode(szBuff[nPos + 1])& 0x3F);
	pTrack->m_SbType  = ConvShipType(nType);
	pTrack->m_Fspec[1]	|= FRN_11; 
	nPos += 2;

	//9: Dimension to Bow :bit 240-248     
	int		nDimA, nDimB;
	BYTE	nDimC, nDimD;

	nDimA = ((Decode(szBuff[nPos  ]) & 0x3F)<<3) |
		    ((Decode(szBuff[nPos+1]) >> 3) & 0x07);
	nPos ++;

	//10: Dimension to Stern : bit 249-257
	nDimB = ((Decode(szBuff[nPos ]) & 0x07) << 6) | (Decode(szBuff[nPos+1]) & 0x3F);
	pTrack->m_Length	= (nDimA + nDimB) * 10;	  // [0.1m]
	pTrack->m_Fspec[2]	|= FRN_21;                // Chiều dài của tàu 
	nPos += 2;

	//11: Dimension to Port:bit 258-263
	nDimC = Decode(szBuff[nPos]) & 0x3F;
	nPos ++;

	//12: Dimension to Starboard: bit 264-269
	nDimD = Decode(szBuff[nPos])&0x3F;
	pTrack->m_Width = (nDimC + nDimD) * 5;	      // [0.2m]
	pTrack->m_Fspec[2]	|= FRN_21;                // Chiều rộng của tàu
	nPos ++;

	//14: ETA month (UTC)  :bit 274 - 277
	BYTE   nMonth, nDay;
	BYTE   nHour, nMin;

	nMonth = ((Decode(szBuff[nPos])& 0x03)<<2) | ((Decode(szBuff[nPos+1])>>4) & 0x03);
	nPos ++;

	//15: ETA day (UTC)  : bit 278- 282

	nDay   = ((Decode(szBuff[nPos])& 0x0F)<<1) | ((Decode(szBuff[nPos+1])>>5)&1);
	nPos ++;

	//16: ETA hour (UTC) : bit 283-287

	nHour  = Decode(szBuff[nPos])& 0x1F;
	nPos ++;

	//17: ETA minute (UTC) : bit 288 - 293

	nMin   = Decode(szBuff[nPos])& 0x3F;
	nPos ++;

	if (nMin > 59)
	{
		nMin -= 60;
		nHour ++;
	}
	if (nHour > 23)
	{
		nHour = 24;
		nDay ++;
	}

	pTrack->m_TimEta[0] = nHour;
	pTrack->m_TimEta[1] = nMin;
	pTrack->m_TimEta[2] = nDay;
	pTrack->m_TimEta[3] = nMonth;

	pTrack->m_Fspec[3]	|= FRN_25;

	//18: Draught : bit 294-301
	BYTE nDraught;
	nDraught = ((Decode(szBuff[nPos]) & 0x3F) << 2) | ((Decode(szBuff[nPos+1]) >> 4) & 0x03);
	pTrack->m_Draugh    = nDraught;	   //[0.1m]
	pTrack->m_Fspec[2] |= FRN_21;
	nPos ++;

	//19: Destination : bit 302 - 421 (120bit)
	char	szDes[21];	
	szDes[20] = 0x00;
	for (int j = 0; j < 20; j++)
	{
		ch = (((Decode(szBuff[nPos + j    ]) & 0x0F) << 2)|
			  ((Decode(szBuff[nPos + j + 1]) & 0x30) >> 4));
		if (ch == 0)	             // '@' - finish
		{
			szDes[j] = 0x00;
			break;
		}
		if (ch < 32)
			ch += 64;
		szDes[j] = ch;
	}	 
	pTrack->m_szDest = szDes;
//	pTrack->m_szDest.Trim();				// delete space
	pTrack->m_Fspec[3]	|= FRN_24;
	nPos += 20;
		
	nRec = nPos - nOld;
	return 1;
}

int C2_Local::GetTrkClassB(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
	int		nOld = nPos;

	// 1: TypeAIS
	pTrack->m_Fspec[0] |= FRN_1;
	pTrack->m_Fspec[2] |= FRN_15;

	// 2: Repeat indicator: bit 6, 7
	int rep_ind;
	rep_ind = (Decode(szBuff[nPos]) & 0x30) >> 4;	// Modify by Tien 26-7-2015

	// 3: MMSI: bit 8-37
	long nMMSI;
	nMMSI = ((Decode(szBuff[nPos])   & 0x0F)<<26)|((Decode(szBuff[nPos+1]) & 0x3F)<<20)|
		    ((Decode(szBuff[nPos+2]) & 0x3F)<<14)|((Decode(szBuff[nPos+3]) & 0x3F)<<8) |
		    ((Decode(szBuff[nPos+4]) & 0x3F)<< 2)|((Decode(szBuff[nPos+5]) & 0x30)>> 4) ;
	nPos += 5;

    szNum=QString::number( nMMSI);
    if((szNum.size() < 4) || (szNum.size() > 9))
        return 0;

    memcpy(pTrack->m_MMSI,szNum.toAscii().data(), 9);
	pTrack->m_Fspec[2] |= FRN_16;	

	// 4: Regional reserved: 38-45: not used	
	nPos++;

	// 5: Speed over ground: 46 - 55
	pTrack->m_Speed = ((Decode(szBuff[nPos  ]) & 0x03) << 8) | ((Decode(szBuff[nPos+1]) & 0x3F) <<2)|
		              ((Decode(szBuff[nPos+2]) >> 4) & 0x03);
	pTrack->m_Fspec[1]	   |= FRN_9;
	nPos += 2;

	// 6: position accuracy: bit 56
	bool bPos = ((Decode(szBuff[nPos]) & 0x08) >> 3) & 1;	// Modify by Tien 26/7/2015

	// 8: Long: 57 - 84: 28 bit
	long nLong, nTmp;
	nLong = ((Decode(szBuff[nPos  ]) & 0x07) << 25)|((Decode(szBuff[nPos+1]) & 0x3F) <<19)|
			((Decode(szBuff[nPos+2]) & 0x3F) << 13)|((Decode(szBuff[nPos+3]) & 0x3F) << 7)|
			((Decode(szBuff[nPos+4]) & 0x3F) <<  1)|((Decode(szBuff[nPos+5]) >> 5) & 1);

	if ((nLong >> 27) & 0x01)		// Nagative number
		nTmp = -(((~nLong)& 0x0FFFFFFF) + 1 );
	else
		nTmp = nLong; 

	pTrack->m_Long = long(nTmp / 421875.0 * (1 << 15));     // degree = 600000*bit23/180;
	pTrack->m_Fspec[1]   |=FRN_8;
	nPos += 5; 

	// 9: Lat: 85 - 111
	long nLat;
	nLat = ((Decode(szBuff[nPos])   & 0x1F) << 22)|((Decode(szBuff[nPos+1]) & 0x3F) << 16)|
		   ((Decode(szBuff[nPos+2]) & 0x3F) << 10)|((Decode(szBuff[nPos+3]) & 0x3F) << 4 )|
	       ((Decode(szBuff[nPos+4]) >> 2)& 0x0F);

	if ((nLat >> 26) & 0x01)		// Nagative 
		nTmp = -(((~nLat) & 0x07FFFFFF) + 1);
	else
		nTmp = nLat; 

	pTrack->m_Lat  = long(nTmp / 421875.0 * (1 << 15));		// degree
	nPos += 4;

	// 10: Course over ground: 112 - 123
	int nCOG;
	nCOG = ((Decode(szBuff[nPos  ]) & 0x03) << 10) | ((Decode(szBuff[nPos+1]) & 0x3F) << 4)|
	       ((Decode(szBuff[nPos+2]) >> 2 ) & 0x0F);
	nPos += 2;

	// 11: True heading: 124 - 132
	int nHeadTr;
	nHeadTr = ((Decode(szBuff[nPos]) & 0x03) << 7)|((Decode(szBuff[nPos+1]) & 0x3F)<<1)|
		      ((Decode(szBuff[nPos+2]) >> 5) & 1) ;
	pTrack->m_HeadTr = nHeadTr * bit15/180;
	pTrack->m_Fspec[2]  |= FRN_19;
	nPos += 2;

	//12: Time Stamp: 133 - 138
	BYTE m_TimeStp;
	m_TimeStp = ((Decode(szBuff[nPos]) & 0x1F) << 1) |((Decode(szBuff[nPos+1]) >> 5 ) & 1);
	nPos ++;

	// 13: Regional reserved: 139 - 140: uninterpreted
	
	// 14: CS Unit: bit 141

	//... More fields not used 
	nRec = nPos - nOld;
	return 1;
}
int C2_Local::GetTrkClasEB(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
	int		nOld = nPos;

	// 1: TypeAIS
	pTrack->m_Fspec[0] |= FRN_1;
	pTrack->m_Fspec[2] |= FRN_15;

	// 2: Repeat indicator: bit 6, 7
	int rep_ind;
	rep_ind = (Decode(szBuff[nPos]) & 0x30) >> 4;

	// 3: MMSI: bit 8-37
	long nMMSI;
	nMMSI = ((Decode(szBuff[nPos  ]) & 0x0F)<<26)|((Decode(szBuff[nPos+1]) & 0x3F)<<20)|
		    ((Decode(szBuff[nPos+2]) & 0x3F)<<14)|((Decode(szBuff[nPos+3]) & 0x3F)<<8)|
		    ((Decode(szBuff[nPos+4]) & 0x3F)<< 2)|((Decode(szBuff[nPos+5]) & 0x30)>>4) ;
	nPos += 5;

    szNum = QString::number(nMMSI);
    if((szNum.size() < 4) || (szNum.size() > 9))
        return 0;

    memcpy( pTrack->m_MMSI,szNum.toAscii().data(), 9);
	
	pTrack->m_Fspec[2] |= FRN_16;	

	// 4: Regional reserved: 38-45: not used	
	nPos++;

	// 5: Speed over ground: 46 - 55
	pTrack->m_Speed = (((Decode(szBuff[nPos  ]) & 0x03) << 8)|((Decode(szBuff[nPos+1]) & 0x3F) <<2)|
		               ((Decode(szBuff[nPos+2]) >>4   ) & 0x03));
	pTrack->m_Fspec[1]     |= FRN_9;
	nPos += 2;

	// 6: position accuracy: bit 56
	 bool bPos = ((Decode(szBuff[nPos]) & 0x08) >> 3) & 1;	// Modify by Tien 26/7/2015

	// 8: Long: 57 - 84
	long nLong, nTmp; 
	nLong = ((Decode(szBuff[nPos  ]) & 0x07) << 25)|((Decode(szBuff[nPos+1])& 0x3F)<< 19)|
			((Decode(szBuff[nPos+2]) & 0x3F) << 13)|((Decode(szBuff[nPos+3])& 0x3F)<<  7)|
			((Decode(szBuff[nPos+4]) & 0x3F) <<  1)|((Decode(szBuff[nPos+5]) >> 5) &   1);	

	if ((nLong >> 27) & 0x01)		// Nagative number
		nTmp = -(((~nLong)& 0x0FFFFFFF) + 1 );
	else
		nTmp = nLong; 
	pTrack->m_Long = long(nTmp / 421875.0 * (1 << 15));
	nPos += 5; 

	// 9: Lat: 85 - 111
	long nLat;
	nLat = ((Decode(szBuff[nPos  ]) & 0x1F) << 22)|((Decode(szBuff[nPos+1]) & 0x3F) << 16)|
		   ((Decode(szBuff[nPos+2]) & 0x3F) << 10)|((Decode(szBuff[nPos+3]) & 0x3F) << 4)|
	       ((Decode(szBuff[nPos+4]) >> 2)& 0x0F);

	if ((nLat >> 26) & 0x01)		// Nagative 
		nTmp = -(((~nLat) & 0x07FFFFFF) + 1);
	else
		nTmp = nLat; 

	pTrack->m_Lat	= long(nTmp / 421875.0 * (1 << 15));		// degree//* bit23/180;
	nPos += 4;

	// 10: Course over ground: 112 - 123
	long	nCOG;
	nCOG = ((Decode(szBuff[nPos  ]) & 0x03) << 10) | ((Decode(szBuff[nPos+1]) & 0x3F) << 4)|
		   ((Decode(szBuff[nPos+2]) >> 2 ) & 0x0F);
	nPos += 2;
	pTrack->m_Head = UINT(nCOG * (1<<12)/225);	// bit15/1800

	// 11: true heading: 124 - 132
	int		nHeadTr;
	nHeadTr = ((Decode(szBuff[nPos  ]) &  0x03) << 7)|((Decode(szBuff[nPos+1]) & 0x3F)<<1)|
			  ((Decode(szBuff[nPos+2]) >> 5   ) & 1) ;
	pTrack->m_HeadTr = nHeadTr * bit15/180;
	pTrack->m_Fspec[2]   |= FRN_19;
	nPos += 2;
	//12: Time Stamp: 133 - 138
	BYTE m_TimeStp;
	m_TimeStp = ((Decode(szBuff[nPos]) & 0x1F) << 1) |((Decode(szBuff[nPos+1]) >> 5 ) & 1);
	nPos ++;

	//... More			??? can decode some field 
	nRec = nPos - nOld;
	return 1;
}

//-----------------------------------------------------------------------------
int C2_Local::GetTrkNaviga(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString szNum;
	char    szStr[24];
	char    ch;
	int     nRep, j, nLen;
	long    nLong, nLat, nTmp;
	BYTE    nType;
	int		nOld = nPos;

	// 1: Type AIS
	pTrack->m_Fspec[2]  |= FRN_15;

	// 2: Repeat indicator: bit 6,7
	nRep = Decode(szBuff[nPos] & 0x30) >> 4;	

	// 3: MMSI: bit 8 - 37
	nLong = ((Decode(szBuff[nPos  ]) & 0x0F) << 26) | ((Decode(szBuff[nPos+1])& 0x3F)<<20) |
			((Decode(szBuff[nPos+2]) & 0x3F) << 14) | ((Decode(szBuff[nPos+3])& 0X3F)<< 8) |
			((Decode(szBuff[nPos+4]) & 0X3F) <<  2) | ((Decode(szBuff[nPos+5])& 0x30)>> 4);
	nPos += 5;

    szNum = QString::number( nLong);
    nLen = szNum.size();

    if ((nLen < 4)||(nLen > 9))
        return 0;
    memcpy( pTrack->m_MMSI,szNum.toAscii().data(), 9);
	pTrack->m_Fspec[2] |= FRN_16;

	if      (nLen == 9)
		pTrack->m_Type = 0x12;      // Auto + Vessel
	else if (nLen == 7)
		pTrack->m_Type = 0x13;      // Auto + Terrain
	else
		pTrack->m_Type = 0x10;      // Auto + default;

	// 4: Aid type: 38 - 42
	nType = ((Decode(szBuff[nPos]) & 0x0F) << 1) | ((Decode(szBuff[nPos+1]) & 0x20) >> 5);
	pTrack->m_SbType = ConvNaviStat(nType);
	nPos += 1;			// Add by Tien 26-7-2015

	// 5: Name: 43 -162: 120 bit
	szStr[20] = 0x00;
	for (j = 0; j < 20; j++)
	{
		ch = ((Decode(szBuff[nPos+j  ]) & 0x1F) << 1) |
			 ((Decode(szBuff[nPos+j+1]) & 0x20) >> 5);

		if (ch == 0)     // '@' - finish
		{
			szStr[j] = 0x00;
			break;
		}
		if (ch < 32)
			ch += 64;
		szStr[j] = ch;
	}
	pTrack->m_szName	= szStr;
//	pTrack->m_szName.Trim();				// delete space
	pTrack->m_Fspec[3] |= FRN_23;
	nPos += 20;

	// 6: Position accuracy: 163
	bool bAccur;

	bAccur = (Decode(szBuff[nPos]) >> 4) & 1;

	// 7: Long: 164-191: 28		
	nLong = ((Decode(szBuff[nPos	]) & 0x0F) << 24) | ((Decode(szBuff[nPos + 1]) & 0x3F) << 18)|
			((Decode(szBuff[nPos + 2]) & 0x3F) << 12) | ((Decode(szBuff[nPos + 3]) & 0x3F) << 6 )|
			((Decode(szBuff[nPos + 4]) & 0x3F)); 

	if ((nLong >> 27) & 0x01)		// Nagative number
		nTmp = -(((~nLong)& 0x0FFFFFFF) + 1 );
	else
		nTmp = nLong;

	pTrack->m_Long		= long(nTmp / 421875.0 * (1 << 15));
	pTrack->m_Fspec[1] |= FRN_8;
	nPos += 4;

	// 8: Lat: 192 - 218: 27		
	nLat = ((Decode(szBuff[nPos + 1])& 0x3F) << 21)|((Decode(szBuff[nPos + 2])& 0x3F) << 15) |
			((Decode(szBuff[nPos + 3])& 0x3F) << 11)|((Decode(szBuff[nPos + 4])& 0x3F) << 6 ) |
			((Decode(szBuff[nPos + 5])& 0x38) >> 3);

	if ((nLat >> 26) & 0x01)		// Nagative 
		nTmp = -(((~nLat) & 0x07FFFFFF) + 1);
	else
		nTmp = nLat;

	pTrack->m_Lat = long(nTmp / 421875.0 * (1 << 15));
	nPos += 5;

	int nDimA, nDimB, nDimC, nDimD;

	// 9: Dimension to bow: 219 - 227	
	nDimA = ((Decode(szBuff[nPos]) & 0x07) << 6)|(Decode(szBuff[nPos+1]));
	nPos += 2;

	// 10: Dimension to Stern: 228 - 236
	nDimB = ((Decode(szBuff[nPos]) & 0x3F) << 3)|((Decode(szBuff[nPos+1]) & 0x38) >> 3);
	nPos ++;

	pTrack->m_Length	= (nDimA + nDimB) * 10;	// [0.1m]
	pTrack->m_Fspec[2] |= FRN_21;

	// 11: Dimension to Port: 237 - 242
	nDimC = ((Decode(szBuff[nPos]) & 0x07) << 3)|((Decode(szBuff[nPos+1]) & 0x38) >> 3);
	nPos ++;

	// 12: Dimension to Starboard: 243 - 248
	nDimD = ((Decode(szBuff[nPos]) & 0x07) << 3)|((Decode(szBuff[nPos+1]) & 0x38) >> 3);
	nPos ++;

	pTrack->m_Width		= (nDimC + nDimD) * 5;	// [0.2m]
	pTrack->m_Fspec[2] |= FRN_21;

	// More
	nRec = nPos - nOld;
	return 1;
}

//-----------------------------------------------------------------------------
int C2_Local::GetTrkType24(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
	int		nRep, nLen;
	long	nLong;
	int		nOld = nPos;

	// 1: Type AIS	
	pTrack->m_Fspec[2]   |= FRN_15;

	// 2: Repeat indicator: bit 6,7	
	nRep = (Decode(szBuff[nPos]) & 0x30) >> 4;		// Add by Tien

	// 3: MMSI: bit 8 - 37
	nLong = ((Decode(szBuff[nPos  ]) & 0x0F) << 26) | ((Decode(szBuff[nPos+1])& 0x3F)<<20) |
			((Decode(szBuff[nPos+2]) & 0x3F) << 14) | ((Decode(szBuff[nPos+3])& 0X3F)<< 8) |
			((Decode(szBuff[nPos+4]) & 0X3F) <<  2) | ((Decode(szBuff[nPos+5])& 0x30)>> 4);
	nPos += 5;

    szNum = QString::number( nLong);
    nLen = szNum.size();		//5.12

    if ((nLen < 4)||(nLen > 9))
        return 0;

    memcpy( pTrack->m_MMSI,szNum.toAscii().data(),  9);
	pTrack->m_Fspec[2] |= FRN_16;

	if		(nLen == 9)
		pTrack->m_Type	= 0x12;			// Auto + Vessel
	else if (nLen == 7)
		pTrack->m_Type	= 0x13;			// Auto + Terrain
	else
		pTrack->m_Type	= 0x10;			// Auto + Default	

	// 4: Part number bit 38 - 39
	BYTE	nPart = (Decode(szBuff[nPos])>> 2) & 0x03;
	int		nTmp  = 0;

	nRec = nPos - nOld;
	if (nPart == 0)	// Part A		
	{	
		if (!GetTrkTyp24A(szBuff, nPos, pTrack, nTmp))
			return 0;
	}
	else
		if (!GetTrkTyp24B(szBuff, nPos, pTrack, nTmp))
			return 0;

	nRec += nTmp;
	return 1;
}

//-----------------------------------------------------------------------------
int C2_Local::GetTrkTyp24A(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{	
	char	szStr[24];
	char	ch;
	int		j;	
	int		nOld = nPos;

	// 5: Vessel name: 40 -159
	szStr[20] = 0x00;
	for (j = 0; j < 20; j++)
	{
		ch = (((Decode(szBuff[nPos+j  ]) << 4) & 0x30) | 
			 ((Decode(szBuff[nPos+j+1]) >> 2) & 0x0F));
		if (ch == 0)	// '@' - finish
		{
			szStr[j] = 0x00;
			break;
		}
			if (ch < 32)
				ch += 64;
			szStr[j] = ch;
	}
	pTrack->m_szName	= szStr;
//	pTrack->m_szName.Trim();					// delete space
	pTrack->m_Fspec[3] |= FRN_23;
	nPos += 20;

	// 6: Spare: 160 - 167: not use
	nPos +=2;

	nRec = nPos - nOld;
	return 1;
}

//-----------------------------------------------------------------------------
int C2_Local::GetTrkTyp24B(BYTE* szBuff, int nPos, C2_Track* pTrack, int &nRec)
{
    QString	szNum;
    char	szStr[24];
    char	ch;
    int		j;
    //long	nLong;
    int		nOld = nPos;

    //5: ship type: 40-47
    BYTE	nType;

    nType = ((Decode(szBuff[nPos])& 0x03) << 6) | (Decode(szBuff[nPos + 1])& 0x3F);
    pTrack->m_SbType	= ConvShipType(nType);
    pTrack->m_Fspec[1] |= FRN_11;
    nPos += 2;

    // 6: Vendor ID: 48 - 90: 42 bit
    QString szVendorId;

    szStr[7] = 0x00;
    for (j = 0; j < 7; j++)
    {
        ch = Decode(szBuff[nPos+j]);
        if (ch == 0)	// '@' - finish
        {
            szStr[j] = 0x00;
            break;
        }
            if (ch < 32)
                ch += 64;
            szStr[j] = ch;
    }
    szVendorId = szStr;
//    szVendorId.Trim();					// delete space
    nPos += 7;
    // 7: CallSign: 90 - 131: 42bits
    szStr[7] = 0x00;
    for (j = 0; j < 7; j++)
    {
        ch = Decode(szBuff[nPos+j]);
        if (ch == 0)	// '@' - finish
        {
            szStr[j] = 0x00;
            break;
        }
        if (ch < 32)
            ch += 64;
        szStr[j] = ch;
    }
    pTrack->m_szClSg	= szStr;
//    pTrack->m_szClSg.Trim();						// delete space
    pTrack->m_Fspec[3] |= FRN_22;
    nPos += 7;

    // 8: Dimension to Bow: 132 - 140

    int		nDimA, nDimB;
    BYTE	nDimC, nDimD;

    nDimA = ((Decode(szBuff[nPos  ])& 0x3F) << 3) |
        ((Decode(szBuff[nPos+1])  >> 3) & 0x07);
    nPos ++;

    // 9: Dimension to Stern : 141 - 149
    nDimB = ((Decode(szBuff[nPos  ])& 0x07)<<6) |
        (Decode(szBuff[nPos+1])& 0x3F);
    nPos += 2;

    pTrack->m_Length	= (nDimA + nDimB) * 10;	// [0.1m]
    pTrack->m_Fspec[2] |= FRN_21;

    // 10: Dimension to Port: 150 - 155
    nDimC = Decode(szBuff[nPos++]) & 0x3F;

    // 11: Dimension to Starboard: 156 - 161
    nDimD = Decode(szBuff[nPos++])& 0x3F;

    pTrack->m_Width		= (nDimC + nDimD) * 5;	// [0.2m]
    pTrack->m_Fspec[2] |= FRN_21;

    nRec = nPos - nOld;
    return 1;
}
//-----------------------------------------------------------------------------
BYTE C2_Local::ConvShipType(BYTE nType)
{
	BYTE	nTyp3x[10] = {30,31,31,33,34,35,36,37,14,14};
	BYTE	nTyp5x[10] = {50,51,52,53,54,55,56,57,14,59};
	BYTE	nTyp7x[10] = {70,72,72,72,74,70,70,70,70,70};

	if		(nType == 0)		// Not defined
		return 0;
	else if	(nType < 20)		// Reserved
		return 14;
	else if (nType < 30)		// Wing on ground
		return 20;
	else if (nType < 40)
		return nTyp3x[nType-30];
	else if (nType < 50)
		return 40;
	else if (nType < 60)
		return nTyp5x[nType-50];
	else if (nType < 70)
		return 60;
	else if (nType < 80)
		return 70;
	else if (nType < 90)
		return 80;
	else
		return 14;
}
//-----------------------------------------------------------------------------
BYTE C2_Local::ConvNaviStat(BYTE nIdNav)
{
	BYTE	nNaviSt[15] = {2, 1, 4, 3, 0, 5, 6, 7, 8, 0, 0, 0, 0, 0, 12};		// 9,10,11,12,13 undefined => 0

	return (nIdNav < 15)? nNaviSt[nIdNav] : 0;	
}
//-----------------------------------------------------------------------------
// 2012-12-04 09:20:33.062
//-----------------------------------------------------------------------------
bool C2_Local::ConvEstaEcom (char *szBuff, int nLeng, C2_Track* pTrack)
{
	if (nLeng < 20)
		return false;

	char	szTmp[16];
	int		nTemp;
	int		nPos = 0;

	// 1: Date
	memcpy(szTmp, &szBuff[nPos], 4);		// Year
	szTmp[4] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_TimEta[4] = BYTE(nTemp >> 8);
	pTrack->m_TimEta[5] = BYTE(nTemp     );
	nPos += 5;

	memcpy(szTmp, &szBuff[nPos], 2);		// Mon
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_TimEta[3] = BYTE(nTemp);
	nPos += 3;

	memcpy(szTmp, &szBuff[nPos], 2);		// Day
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_TimEta[2] = BYTE(nTemp);
	nPos += 3;

	// 2: Time
	memcpy(szTmp, &szBuff[nPos], 2);		// Hour
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_TimEta[0] = BYTE(nTemp);
	nPos += 3;

	memcpy(szTmp, &szBuff[nPos], 2);		// Min
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_TimEta[1] = BYTE(nTemp);
	nPos += 3;

/*	memcpy(szTmp, &szBuff[nPos], 2);		// Sec
	szTmp[2] = 0x00;
	pTrack->m_TimEta   += atoi(szTmp);
	nPos += 3;

	pTrack->m_TimEta   *= 128; */
	return true;
}

// 20121204092209
bool C2_Local::ConvTimeEcom(char *szBuff, int nLeng, C2_Track* pTrack)
{
	if (nLeng < 14)
		return false;

	char	szTmp[16];
	int		nTemp;
	int		nPos = 0;

	// 1: Date
	memcpy(szTmp, &szBuff[nPos], 4);		// Year
	szTmp[4] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_DayLst[2] = BYTE(nTemp >> 8);
	pTrack->m_DayLst[3] = BYTE(nTemp     );
	nPos += 4;

	memcpy(szTmp, &szBuff[nPos], 2);		// Mon
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_DayLst[1] = BYTE(nTemp);
	nPos += 2;

	memcpy(szTmp, &szBuff[nPos], 2);		// Day
	szTmp[2] = 0x00;
	nTemp = atoi(szTmp);
	pTrack->m_DayLst[0] = BYTE(nTemp);
	nPos += 2;

	// 2: Time
	memcpy(szTmp, &szBuff[nPos], 2);		// Hour
	szTmp[2] = 0x00;
	pTrack->m_TimLst	= atoi(szTmp)*3600;
	nPos += 2;

	memcpy(szTmp, &szBuff[nPos], 2);		// Min
	szTmp[2] = 0x00;
	pTrack->m_TimLst   += atoi(szTmp)*60;
	nPos += 2;

	memcpy(szTmp, &szBuff[nPos], 2);		// Sec
	szTmp[2] = 0x00;
	pTrack->m_TimLst   += atoi(szTmp);
	nPos += 2;

	pTrack->m_Time		= pTrack->m_TimLst*128;
	return true;
}
// ------------------------------------------------------------------
// Check Field to test packet is valid or not valid 
//-------------------------------------------------------------------
bool C2_Local::IsCheckSum(BYTE * szBuff, int nLeng)
{
	int nPos = 0; 
	char szTmp  ; 
	int nCheckSum = 0;				// CheckSum in packet
	int nCheckS_Trans = 0;			// CheckSum is transmitted 

	while (nPos<nLeng ) 
	{
		szTmp = szBuff[nPos ++];		

		switch (szTmp)
		{
		case '$':
			break;

		case '*':						// if not '*' or (nCheckS_Trans ! nCheckSum)=> Not valid
			nCheckS_Trans = (szBuff[nPos] - 0x30)*16 + (szBuff[nPos + 1] - 0x30) ; 
			return	(nCheckS_Trans == nCheckSum) ? 1 : 0; 	

		default:
			nCheckSum ^= szTmp; 
			break;
		}
	}
	return 0; 
}
//----------------------------------------------------------------------------------
// Get Info of $RATTM from RADADK
//----------------------------------------------------------------------------------
int C2_Local::GetTrkTTM(BYTE *szBuff, int nLeng, C2_Track* pTrack, int &nRec)
{
    QString		szStr;
	int			nTmp ; 
	int			nPos = 0;

	// 1: Track/ target number 00 - 99                 // Số hiệu mục tiêu
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    pTrack->m_TRN = (szStr[0].toAscii() - 0x30)*10 + (szStr[1].toAscii() - 0x30);
	pTrack->m_Fspec[0] |= FRN_4;

	//2: Target Distance: X.x nm
	int		nRge;			// 0.001nm
	UINT	nAzi;			// 1/2^16

	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos]==',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}
    nRge = int((szStr.toInt()) * 1000);	// m

	// 3: Bearing /Head: X.x degree
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    nAzi = UINT((szStr.toInt()) * (1<<13)/45);			// _wtof convert character string to a duble-precision.
	m_RadaPnt.ConvPolToWGS(pTrack, nRge, nAzi);		// Convert Polar(Range, Azimuth) to WGS(Lat, Long)
	pTrack->m_Fspec[1]	|= FRN_8;

	// 4: Unit of bearing: T/R (True/Relative)
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		nPos++;
	}
	nPos++;
	
	// 5: Track speed: X.x knot
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos]==',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    pTrack->m_Speed = UINT(szStr.toInt()*10);			// 0.1nm/hour
	pTrack->m_Fspec[1]	|= FRN_9;

	//6: Target Course: X.x degree
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos]==',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    pTrack->m_Head = int(szStr.toInt()* (1<<13)/ 45);			// 2^16/360
	pTrack->m_Fspec[1]	|= FRN_9;

	// 7: Unit of Course: T/R (True/Relative)
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		nPos++;
	}
	nPos++;

	// 8: Distance of CPA: X.x nm
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    nTmp = int(szStr.toInt());

	// 9: Time until CPA: minute,  '-' = move away
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

    nTmp = int(szStr.toInt());

	// 10: Units of Speed/Distance: K/N/S
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		nPos++;
	}
	nPos++;

	// 11: Target name
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
	if (nPos > nLeng)
	{
		nRec = nPos;
		return 0;
	}

	pTrack->m_szName = szStr;
	pTrack->m_Fspec[3]	|= FRN_23;
	
	// 12: Target status: L/Q/T (Lost, Query, Tracking)
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;

	if		(szStr[0] == 'L')		// Lost ?
		pTrack->m_StMain = 0;
	else if (szStr[0] == 'Q')		// Query
		pTrack->m_StMain = 0;
	else if (szStr[0] == 'T')		// Tracking
		pTrack->m_StMain = 1;

	pTrack->m_Fspec[0]	|= FRN_6;

	// 13: Reference target = R
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;

	// 14: Time of data (hhmmss.ss)
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;

	// 15: Type of target acquisition A / M (Automatic / Manual)
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == '*')				//Modify by Tien 
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;

	// 16: Checksum
	szStr = "";
	for (int i = 0; i < 2; i++)					// checksum: hh _modify by Tien
		szStr += szBuff[nPos++];

	nRec = nPos;
	return 1;
}

//----------------------------------------------------------------------------------
// Get Info of $RAOSD from RADADK- Own Ship Data 
//----------------------------------------------------------------------------------
int C2_Local::GetTrkOSD(BYTE *szBuff, int nLeng, C2_Track* pTrack, int &nRec)
{
	// II:  Process packet 
    QString		szStr;
	int			nTmp ; 
	int			nPos = 0;

	// 1: Heading, degrees True
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++]; 
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 2: Heading status: A = Valid, V = invalid
	nPos += 2; 

	// 3: Vessel Course, degrees True
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 4: Course Reference B/M/W/R/P
	nPos += 2; 

	// 5: Vessel speed 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = int(szStr.toInt());

	// 6: Speed reference, B/M/W/R/P
	nPos += 2; 

	// 7: Vessel set, degree True
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos ++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 8: Vessel drift(speed)
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
    nTmp = int(szStr.toInt());

	// 9: Speed units, K/N/S
	nPos += 2;
	nRec = nPos;

	return 1;
}

//----------------------------------------------------------------------------------
// Get Info of $RARSD from RADADK - Radar System Data 
//----------------------------------------------------------------------------------
int C2_Local::GetTrkRSD(BYTE *szBuff, int nLeng, C2_Track* pTrack, int &nRec)
{
	// II:  Process packet 
    QString		szStr;
	int			nTmp ; 
	int			nPos = 0;

	// 1: Origin 1 range, from own ship 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = int(szStr.toInt());

	// 2: Origin1 bearing, degrees from 0
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 3: Variable Range Marker 1(VRM1), range 
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 4: Bearing Line 1(EBL1), degrees from 0
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 5: Origin 2 range 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 6: Origin 2 bearing
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 7: VRM2, range 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 8: EBL2, degrees
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 9: Cursor range, from own ship 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 10: Cursor bearing, degrees clockwise from 0
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 11: Range scale in use 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nTmp  = int(szStr.toInt());

	// 12: Range units, K/N/S
	nPos += 2; 

	// 13: Display rotation 
	nPos += 2; 

	nRec = nPos;
	return 1;
}

//----------------------------------------------------------------------------------
// Get Info of $GPGGA from RADADK -Global Possitioning system fix data  
//----------------------------------------------------------------------------------
void C2_Local::GetTrkGGA(BYTE *szBuff, int nLeng, C2_Track* pTrack, int nPos)
{
	// II: Process packet data 
    QString szStr;
	int		nTmp, i; 

	// 1: UTC of position
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 

	BYTE	nHour, nMinute;
	float	nSecond; 
    QString nTmpSecond;

    nHour   = (szStr[0].toAscii() - 0x30)*10 + (szStr[1].toAscii() - 0x30);
    nMinute = (szStr[2].toAscii() - 0x30)*10 + (szStr[3].toAscii() - 0x30);
//	nSecond = _wtof(&szStr[4]);		-error

    for (i = 4; i < szStr.size(); i++)
		nTmpSecond += szStr[i];

    nSecond = float(nTmpSecond.toFloat());

	// 2: Latitude - N/S
	// 2a: Latitude
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
	int		nDeg; 
	float	nMin;
    QString szMinTmp;

    nDeg = (szStr[0].toAscii() - 0x30)*10 + (szStr[1].toAscii() -0x30);
    for (i = 2; i < szStr.size(); i++)
        szMinTmp += szStr[i].toAscii();

    nMin = float((szMinTmp.toFloat()));
	// 2b: N/S
	nPos += 2; 

	// 3: Longtitude - E/W
	// 3a: Longtitude
	szStr = ""; 
	szMinTmp = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++;
    nDeg = (szStr[0].toAscii() - 0x30)*100 + (szStr[1].toAscii() - 0x30)*10 + (szStr[2].toAscii() - 0x30);
	//	nMin = _wtof(&szStr[3]);	error
    for (i = 3; i< szStr.size(); i++)
        szMinTmp += szStr[i].toAscii();

    nMin = float((szMinTmp.toFloat()));

	//3b: E/W
	nPos += 2; 

	// 4: GPS Quality indecator 
	//szStr = ""; 
	char cQual;			// Quality
	cQual = szBuff[nPos++];
	nPos ++;
	nTmp = (cQual - 0x30); 

	// 5: Number of satellites in use, 00-12, may be differient from the number in view
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = (szStr[0].toAscii() - 0x30)*10 + (szStr[1].toAscii()-0x30);

	// 6: Horizontal dilution of precision  x.x
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 7: Altitude re: mean-sea-level (geoid), metters
	// 7a: Altitude 
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++]; 
	}
	nPos ++; 
    nTmp = int(szStr.toInt());
	//7b: 
	nPos +=2; 
	// 8: Geoidal separation, meters
	// 8a: Geoidal 
	szStr =""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());
	// 8b: 
	nPos += 2; 

	// 9: Age of differential GPS data x.x
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = int(szStr.toInt());

	// 10: Differential reference station ID, 0000-1023 xxxx
	szStr =""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == '*')
			break; 
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = szStr.toInt();

}
//------------------------------------------------------------------------------------------------------
// Process packet of GPZDA 
//------------------------------------------------------------------------------------------------------
void C2_Local::GetTimeZDA(BYTE * szBuff, int nLeng, C2_Track* pTrack, int nPos)
{
	// II: Process packet
    QString szStr;
	int		nTmp, i; 

	// 1: Get Time: UTC
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 

	BYTE	nHour, nMinute;
	float	nSecond; 
    QString nTmpSecond;

    nHour   = (szStr[0].toAscii() - 0x30)*10 + (szStr[1].toAscii()  - 0x30);
    nMinute = (szStr[2].toAscii()  - 0x30)*10 + (szStr[3].toAscii()  - 0x30);
	//	nSecond = _wtof(&szStr[4]);		-error

    for (i = 4; i < szStr.size(); i++)
		nTmpSecond += szStr[i];

    nSecond = float(nTmpSecond.toFloat());

	// 2: Get Day: 00-31 
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = szStr.toInt();

	// 3: Get Month: 01- 12
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = szStr.toInt();

	// 4: Get Year: xxxx
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos ++; 
    nTmp = szStr.toInt();

	// 5: Get local zone hour:00:(+)(-) 13 hrs
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 


	// 6: Local zone minutes, 00 to +59
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == '*')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
}
//---------------------------------------------------------------------------------------------------------------
//	Process VTG - Track made good and Ground speed 
//---------------------------------------------------------------------------------------------------------------
void C2_Local::GetTrkVTG(BYTE * szBuff, int nLeng, C2_Track* pTrack, int nPos)
{
	//II: Process packet data
    QString szStr;
	int nTmp; 

	// 1: Track degrees
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = szStr.toInt();

	// 2: T: true
	nPos += 2; 

	// 3: Track degrees 
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = szStr.toInt();

	// 4: M: Magnetic 
	nPos += 2; 
	// 5: Speed Knots
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = szStr.toInt();

	// 6: N:Knots
	nPos += 2; 

	// 7: Speed kilometters per hour
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = szStr.toInt();

	// 8: K: Kilomet per hour
	nPos += 2; 

	// 9: FAA mode indicator 
	nPos += 2; 
}
//---------------------------------------------------------------------------------------------------------------
//	Process DTM- Datum Reference 
//---------------------------------------------------------------------------------------------------------------
void C2_Local::GetTrkDTM(BYTE * szBuff, int nLeng, C2_Track* pTrack, int nPos)
{
	// II: Process packet data 
    QString szStr;
	int		nTmp ; 

	// 1: Local datum code
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 

	// 2: Local datum subdivision code
	nPos += 2; 

	// 3: Lat offset, minutes, N/S
	// 3a: 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
    nTmp = int(szStr.toInt());

	// 3b: 
	nPos += 2; 

	// 4: Lon offset, minutes, E/W
	// 4a:
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++;
    nTmp = int(szStr.toInt());

	// 4b:
	nPos += 2; 

	// 5: Altitude offset, meters 
	szStr = ""; 
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == ',')
			break;
		szStr += szBuff[nPos++];
	}
	nPos++; 
    nTmp = int(szStr.toInt());

	// 6: Reference datum code 
	szStr = "";
	while (nPos < nLeng)
	{
		if (szBuff[nPos] == '*')
			break;
		szStr+= szBuff[nPos++];
	}
	nPos++; 
}
