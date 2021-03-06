//=============================================================================
//
// CLocal.h: interface for the C2_Local class.
// File name	: CLocal.h - header file for the system information
//=============================================================================

#if !defined(AFX_C2LOCAL_H_)
#define AFX_C2LOCAL_H_

//#pragma once
#include "CTrack.h"
#include "CDefine.h"
//#include "CSerial.h"
//#include "CNation.h"
//#include "CLogfile.h"


//-----------------------------------------------------------------------------
//	Host site: Server/Source
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//	All of system information
//-----------------------------------------------------------------------------
class C2_Local  
{
public:
//	CString		m_szRoot;			// Root path of App.exe file
//	CString		m_szFile;			// Name of information file
//	CString		m_szNamePC;			// Name of BaTe computer
//	CString		m_szAccoun;
//	CString		m_szPasswd;

//	C2_Host		m_HostServ;			// Server of system! -> SeBa
//	C2_Host     m_HostRada;         // Address of Rada/ARPA
//	BYTE		m_ComRada;		    // Serial port of Radar
//	BYTE		m_ComAIS ;		    // Serial port of AIS

//	BYTE		m_LinkType;			// b0=(0:Ether, 1:Modem); b4:Auto dial
//	CString		m_szDialup;			// Is check name of dial entry
//	bool		m_IsCheck ;			// Is auto dial-up
//	BYTE		m_SelAdap ;			// Temple in session

//	BYTE		m_UserRole;			// User role in Server: 0=Oper, 1:Tech, 2:Root
//	CString		m_szPwdTec;			// Password of Technical
//	CString		m_szPwdSup;			// Password of Supervisor

//	int			m_AlTmPlan;			// Min time for Air alarm  (minute)
//	int			m_AlTmShip;			// Min time for Ship alarm (minute)
//	int			m_AlRgPlan;			// Min range for Air alarm (miles)
//	int			m_AlRgShip;			// Min range for Ship alarm(miles)
//	BYTE		m_AlTrPlan;			// Type of track to alarm  (bits )
//	BYTE		m_AlTrShip;
//	BYTE		m_AlarZone;

//	BYTE		m_ReplType;			// Replay type
//	BYTE		m_ReplIden;
//	int			m_ReplTAN ;
//	int			m_RecdTime;			// Record time [min]

//	// Port Com parameters
//	BYTE		m_ComStat[16];	    // Max = 16 port status
//	C2_ComInfo	m_ComInfo[16];      // Information of all comports
	C2_Point	m_RadaPnt;			// Position of radar

	BYTE        m_preFrag;          // the previous fragment number
	int         m_NumFrag;          // total fragments
	BYTE        m_Buff[2048];       // Save temporary fragment
	int         m_Leng;				// length of fragment buffer

public:
	C2_Local();
	~C2_Local();

//	int			GetAddrLocal(BYTE, CString *);
//	CString		GetNameHost	();
//	long		GetTimeUTC	();

//	bool		CheckPwd	(BYTE, CString);		// Type, Password
//	void		ChangePwd	(BYTE, CString);
//	bool		LoadFile	();
//	bool		SaveFile	();

public:
//	int			GetTrackRada(BYTE*, int, CC_Track*);
	int			GetTrackRada(BYTE*, int, C2_Track*, int&);
//	int			GetTrackAIS (BYTE*, int, CC_Track*);
	int			GetTrackAIS (BYTE*, int, C2_Track*, int&);
	int         OnLinkBuff  (BYTE*, int, int &);		

private:
	BYTE		Decode		(BYTE);						// AIS coding
	BYTE		ConvShipType(BYTE);						// AIS - Sub type
	BYTE		ConvNaviStat(BYTE);						// AIS - Navi status
	bool		ConvEstaEcom(char*, int, C2_Track*);	// ELCOM AIS
	bool		ConvTimeEcom(char*, int, C2_Track*);

	int			GetTrkClassA(BYTE*, int, C2_Track*, int&);	// type = 1, 2, 3
	int			GetTrkClassB(BYTE*, int, C2_Track*, int&);	// type = 18
	int			GetTrkClasEB(BYTE*, int, C2_Track*, int&);	// type = 19
	int			GetTrkStatio(BYTE*, int, C2_Track*, int&);	// type = 4
	int         GetTrkStatic(BYTE*, int, C2_Track*, int&);  // type = 5 Static and Voyage related data
	int			GetTrkNaviga(BYTE*, int, C2_Track*, int&);	// type = 21 Aid - to - Navigation Report
	int         GetTrkType24(BYTE*, int, C2_Track*, int&);  // type = 24 Static data report
	int			GetTrkTyp24A(BYTE*, int, C2_Track*, int&);	// type 24 PartA
    int			GetTrkTyp24B(BYTE*, int, C2_Track*, int&);	// type 24 PartB

	// Function of data packet from ARPA 
	bool		IsCheckSum	(BYTE*, int);					// Test CheckSum 
	int 		GetTrkTTM	(BYTE*, int, C2_Track*, int&);	// Get Info of $RATTM
	int 		GetTrkOSD	(BYTE*, int, C2_Track*, int&);	// --  $RAOSD
	int 		GetTrkRSD	(BYTE*, int, C2_Track*, int&);	// --  $RARSD

	void		GetTrkGGA	(BYTE*, int, C2_Track*, int);	// --  $GPGGA
	void		GetTimeZDA	(BYTE*, int, C2_Track*, int);	// --  $GPZDA
	void		GetTrkVTG	(BYTE*, int, C2_Track*, int);	// --  $GPVTG
	void		GetTrkDTM	(BYTE*, int, C2_Track*, int);	// --  $GPDTM

};

//-----------------------------------------------------------------------------
#endif // !defined(AFX_C2LOCAL_H_)
