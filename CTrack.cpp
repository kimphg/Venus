//=============================================================================
//	File name: CTrack.h	
//	Object: the Track class
//	Compiler: Microsoft Visual C++ 6.0

//	Update:	10-10-2003
//	Copyright by Dang Quang Hieu - AIC Corp.
//=============================================================================

//#include "StdAfx.h"
#include "CTrack.h"



//=============================================================================
// Class Track
//=============================================================================
C2_Track::C2_Track()
{
	memset(m_Fspec, 0, 5);
	m_Sac		= 0;			// Source type
	m_Sic		= 0;
	m_Comm		= COMM_NON;

	m_TRN		= 0;
	m_StMain	= 0x01;
	m_StExt1	= 0x00;
	m_Lat		= long( 20.827 * (1<<21)/45);
	m_Long		= long(106.72  * (1<<21)/45);
	m_Time		= 0;
	m_Speed		= 0;			// [0.1nm/h]
	m_Head		= 0;			// [360/2^16]
	m_Alti		= 0;
	m_TmInit	= 0;			// Init of track [1/128s]	

	m_Iden		= 0;			// Iden | Mission
	m_SbType	= 0;			// Sub type
	m_NaviSt	= 0;
	m_TQ		= 0;			// [0..9]
	m_szLabe	= "";			// Track label
	m_szNote	= "";			// Track note

	m_TN		= 0;			// PKKQ
	m_Mode1		= 0;
	m_Mode2		= 0;
	m_Mode3A	= 0;

	// Data from AIS:
	m_TypMsg	= 0 ;
	memset(m_MMSI, 0, 9);		// MMSI number
	memset(m_IMO , 0, 7);		// IMO number
	m_IdNati	= 0;
	m_szNati	= "";			// national of ship
	m_ROT		= 0 ;			// [1deg/min]
	m_Length	= 0 ;			// [0.1m]
	m_Width		= 0 ;			// [0.2m]
	m_Draugh	= 0 ;			// [0.1m]
	m_szClSg	= "";			// Call sign of track
	m_szDest	= "";			// Destination

	// Addition information
	m_TmUpd		= 0;			// Tactical update time [1/128s]
	m_TmHist	= 180;			// Make history [120s]
	m_LvWarn	= 0;
	m_StWarn	= 0x00;			// No Warn signs
	m_bBlink	= 0x00;			// Not blink
	m_IsView	= true;
	m_OnHist	= false;

	m_IsHisOn	= false;
	m_IsPrdMov	= false;		// Is predict of moving
	m_IsPrdCPA	= false;		// Is predict of Closest Point Aproach
	m_IsPrdCTS	= false;		// Is predict of Course To Steer

	m_TimeMove	= 1800;			// [sec]
	m_TypeCTS	= 1;			// 1 = CTS, 2 = CTT

	m_TmHist	= 240;			// Make history 
	m_TmSave	= 1200;			// Is save full
}

C2_Track::~C2_Track()
{
	Clear();
}
void C2_Track::SetNewTRN(int nTRN)
{
	m_TRN		= nTRN;
	m_Fspec[0] |= FRN_4;
}

void C2_Track::OnInitSimu(BYTE nType)
{
	if (nType == 1)		// Aircraft		
	{
		m_Fspec[0]	= 0xBD;			// 1011 1101: Src, SID, TRN, Tim, Sta, 
		m_Fspec[1]	= 0xFD;			// 1111 1101: Pos, Hd/Sp, Alti, Type, Iden, TQ
		m_Fspec[2]	= 0x01;			// 0000 0001:    
		m_Fspec[3]	= 0xCF;			// 1100 1111: Clsg, Name, Init, Labl, Note
		m_Fspec[4]	= 0xC0;			// 1100 0000: Mode, TN 
	}
	else
	if (nType == 2)		// Surface
	{
		m_Fspec[0]	= 0xBD;			// 1011 1101: Src, SID, TRN, Tim, Sta
		m_Fspec[1]	= 0xDF;			// 1101 1111: Pos, Hd/Sp, Type, Iden, TQ, Nati
		m_Fspec[2]	= 0xFF;			// 1111 1111: AISt, MMSI, Nati, ROT, TrHd, IMO, Size,   
		m_Fspec[3]	= 0xFE;			// 1111 1110: Clsg, Name, Dest, ETA, Init, Labl, Note
		m_Fspec[4]	= 0x02;			// 0000 0000: none
	}
	else
	if (nType == 3)		// Terrain
	{
		m_Fspec[0]	= 0xBD;			// 1011 1101: Src, SID, TRN, Tim, Sta
		m_Fspec[1]	= 0xDF;			// 1101 1111: Pos, Hd/Sp, Type, Iden, TQ, Nati
		m_Fspec[2]	= 0xE5;			// 1110 0101: AISt, MMSI, Nati, IMO,   
		m_Fspec[3]	= 0xFE;			// 1111 1110: Clsg, Name, Dest, ETA, Init, Labl, Note
		m_Fspec[4]	= 0x02;			// 0000 0010: none
	}

	m_HeadTr	= m_Head;
}

//-----------------------------------------------------------------------------
//	Function: Update a track from SYSTEM
//-----------------------------------------------------------------------------
void C2_Track::Update(C2_Track *pTrack)
{
	C2_Track::Update(pTrack, 0);
}

void C2_Track::Update(C2_Track *pTrack, BYTE nSic)
{		
	// BYTE 1:
	if (pTrack->m_Fspec[0] & FRN_1)
	{
		m_Sac		= pTrack->m_Sac	;		// System Area Code
		m_Sic		= pTrack->m_Sic	;		// System Iden Code
		m_Fspec[0] |= FRN_1;
	}
	
	if (pTrack->m_Fspec[0] & FRN_3)
	{
		m_TypMsg	= pTrack->m_TypSer;
		m_Fspec[0] |= FRN_3;
	}

	if (pTrack->m_Fspec[0] & FRN_4)         // Target number, 00 to 99
	{
		m_TRN		= pTrack->m_TRN;
		m_Fspec[0] |= FRN_4;
	}

	if (pTrack->m_Fspec[0] & FRN_5)
	{
		m_Time		= pTrack->m_Time;
		m_Fspec[0] |= FRN_5;
	}

	// FRN 6: Status	- 2 byte
	if (pTrack->m_Fspec[0] & FRN_6)
	{
		m_StMain	= pTrack->m_StMain;
		if (m_StMain & FX)
		m_StExt1	= pTrack->m_StExt1;
		m_Fspec[0] |= FRN_6;
	}

	// BYTE #2
	if (pTrack->m_Fspec[1] & FRN_8)        // Toa do muc tieu
	{
		m_Lat		= pTrack->m_Lat	;
		m_Long		= pTrack->m_Long;
		m_Lat0		= pTrack->m_Lat ;
		m_Long0		= pTrack->m_Long;		
		m_Fspec[1] |= FRN_8;
	}

	if (pTrack->m_Fspec[1] & FRN_9)       // Target speed
	{
		m_Speed		= pTrack->m_Speed;
		m_Head		= pTrack->m_Head ;
		m_Fspec[1] |= FRN_9;
	}

	if (pTrack->m_Fspec[1] & FRN_10)
	{
		m_Alti		= pTrack->m_Alti;
		m_Fspec[1] |= FRN_10;
	}

	if (pTrack->m_Fspec[1] & FRN_11)
	{
		m_Type		= pTrack->m_Type;
		m_SbType	= pTrack->m_SbType;
		m_Fspec[1] |= FRN_11;
	}

	if (pTrack->m_Fspec[1] & FRN_12)
	{
		m_Iden		= pTrack->m_Iden;
		m_NaviSt	= pTrack->m_NaviSt;
		m_Fspec[1] |= FRN_12;
	}

	if (pTrack->m_Fspec[1] & FRN_13)
	{
		m_TQ		= pTrack->m_TQ;
		m_Fspec[1] |= FRN_13;
	}

	if (pTrack->m_Fspec[1] & FRN_14)
	{
		m_IdNati	= pTrack->m_IdNati;
		m_Fspec[1] |= FRN_14;
	}

	// BYTE #3
	if (pTrack->m_Fspec[2] & FRN_15)
	{
		m_TypMsg	= pTrack->m_TypMsg;
		m_Fspec[2] |= FRN_15;
	}

	if (pTrack->m_Fspec[2] & FRN_16)
	{
		memcpy(m_MMSI, pTrack->m_MMSI, 9);
		m_Fspec[2] |= FRN_16;
	}

	if (pTrack->m_Fspec[2] & FRN_17)
	{
		m_szNati	= pTrack->m_szNati;
		m_Fspec[2] |= FRN_17;
	}

	if (pTrack->m_Fspec[2] & FRN_18)
	{
		m_ROT		= pTrack->m_ROT;
		m_Fspec[2] |= FRN_18;
	}

	if (pTrack->m_Fspec[2] & FRN_19)
	{
		m_HeadTr	= pTrack->m_HeadTr;
		m_Fspec[2] |= FRN_19;
	}

	if (pTrack->m_Fspec[2] & FRN_20)
	{
		memcpy(m_IMO, pTrack->m_IMO, 7);
		m_Fspec[2] |= FRN_20;
	}

	if (pTrack->m_Fspec[2] & FRN_21)
	{
		m_Length	= pTrack->m_Length;
		m_Width		= pTrack->m_Width ;
		m_Draugh	= pTrack->m_Draugh;
		m_Fspec[2] |= FRN_21;
	}

	// BYTE #4
	if (pTrack->m_Fspec[3] & FRN_22)
	{
		m_szClSg	= pTrack->m_szClSg;
		m_Fspec[3] |= FRN_22;
	}

	if (pTrack->m_Fspec[3] & FRN_23)
	{
		m_szName	= pTrack->m_szName;
		m_Fspec[3] |= FRN_23;
	}

	if (pTrack->m_Fspec[3] & FRN_24)
	{
		m_szDest	= pTrack->m_szDest;
		m_Fspec[3] |= FRN_24;
	}

	if (pTrack->m_Fspec[3] & FRN_25)
	{
		memcpy(m_EsTime, pTrack->m_EsTime, 6);
		m_Fspec[3] |= FRN_25;
	}

	if (pTrack->m_Fspec[3] & FRN_26)
	{
		m_TmInit	= pTrack->m_TmInit;
		m_Fspec[3] |= FRN_26;
	}

	if (pTrack->m_Fspec[3] & FRN_27)
	{
		m_szLabe	= pTrack->m_szLabe;
		m_Fspec[3] |= FRN_27;
	}

	if (pTrack->m_Fspec[3] & FRN_28)
	{
		m_szNote	= pTrack->m_szNote;
		m_Fspec[3] |= FRN_28;
	}

	// BYTE #5:
	if (pTrack->m_Fspec[4] & FRN_29)
	{
		m_Mode1		= pTrack->m_Mode1;
		m_Mode2		= pTrack->m_Mode2;
		m_Mode3A	= pTrack->m_Mode3A;
		m_Fspec[4] |= FRN_29;
	}

	if (pTrack->m_Fspec[4] & FRN_30)
	{
		m_TN		= pTrack->m_TN;
		m_Fspec[4] |= FRN_30;
	}

	// User addition		
	m_TmUpd		= 0;

	if (m_StWarn & 0x80)	// IsLost or IsDrop
	{
		m_StWarn &= 0x7F;	// b7 = 0
		m_bBlink &= 0x7F;	// Not blink
	}
}

//-----------------------------------------------------------------------------
//	Check same MMSI
bool C2_Track::CheckMMSI(BYTE *nMMSI)
{
	for (int i = 0; i < 9; i++)
	if ((m_MMSI[i] != 0) &&	(m_MMSI[i] != nMMSI[i]))
		return false;

	return true;
}
// 0:=, 1:>, 2:<
int C2_Track::CompaMMSI(BYTE *nMMSI)
{
	for (int i = 0; i < 9; i++)
		if (m_MMSI[i] > nMMSI[i])
			return 1;
		else
			if (m_MMSI[i] < nMMSI[i])
				return 2;	

	return 0;
}
//-----------------------------------------------------------------------------
bool C2_Track::CheckIMO (BYTE *nIMO)
{
	for (int i = 0; i < 7; i++)
	if ((m_IMO[i] != 0) && (m_IMO[i] != nIMO[i]))
		return false;

	return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Function: Get string Speed [0.1nm/h]
//-----------------------------------------------------------------------------

int C2_Track::GetIntSpeed()
{
	long	nTmp;
	int		nKm, nDm;

	if ((m_Type & 0x0F) == 1)		// Aircraft
		nTmp = long(m_Speed * 1.852);
	else
		nTmp = long(m_Speed * 1.0  );

	nKm	 = nTmp/10;	
	nDm	 = nTmp%10;
	if (nDm > 5)nKm ++;

	return nKm;
}

int C2_Track::GetIntHead()
{
	return C2_Place::GetIntAzimu(m_Head);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Get string/value of Type/Iden/Navi,...
//-----------------------------------------------------------------------------
//QString C2_Track::GetStrClass(BYTE nId)
//{
//    QString szType[4] = {(""), ("May bay"),	("Tau bien"),("Đia vat")};

//    return (nId < 4)? szType[nId] : ("...");
//}

//QString C2_Track::GetStrTypAir(BYTE nId, BYTE &nType)
//{
//	_TCHAR *szType[16] = {
//		_T("Không rõ"),		_T("Ném bom"),		_T("Cường kích"),	_T("Tiêm kích"),
//		_T("Trực thăng"),	_T("Vận tải"),		_T("Tiếp dầu"),		_T("Trinh sát"),
//		_T("Gây nhiễu"), 	_T("Huấn luyện"),	_T("Khí cầu"),		_T("Cảnh giới"),
//		_T("TL h.trình"),	_T("TL đ.đạo"),		_T("Hàng không"),	_T("UFO") };

//	BYTE	nIndex[16] = { 0, 1, 2, 3, 4, 5, 6, 7,
//						   8, 9,10,11,12,13,14,15};

//	nType = (nId < 16)? nIndex[nId] : 0;
//	return (nId < 16)? szType[nId] : _T("...");
//}

//QString C2_Track::GetStrTypCrf(BYTE nId, BYTE &nType)
//{
//	_TCHAR *szType[3] = {_T("Máy bay"), _T("T.thăng"), _T("T.thám")};
//	BYTE	nIndex[3] = {1, 8, 13};

//	nType = (nId < 3)? nIndex[nId] : 0;
//	return (nId < 3)? szType[nId] : _T("...");
//}

//BYTE C2_Track::FindIdTypAir(BYTE nType)
//{
//	BYTE	nIndex[16] = {0, 1, 2, 3, 4, 5, 6, 7,
//						  8, 9,10,11,12,13,14,15};

//	for (BYTE nId = 0; nId < 16; nId ++)
//	if (nType == nIndex[nId])
//		return nId;

//	return 0;
//}

//BYTE C2_Track::FindIdTypCrf(BYTE nType)
//{
//	BYTE	nIndex[3] = {1, 8, 13};

//	for (BYTE nId = 0; nId < 3; nId ++)
//	if (nIndex[nId] == nType)
//		return nId;

//	return 0;
//}

//QString C2_Track::GetStrTypSuf(BYTE nId, BYTE &nType)
//{
//	_TCHAR *szType[28] = {
//		_T("Tàu biển"),		_T("Dân sự"),		_T("Cánh ngầm"),	_T("Tàu cá"),
//		_T("Được kéo"),		_T("Nạo vét"),		_T("Phà biển"),		_T("Quân sự"),
//		_T("Th. buồm"),		_T("Du thuyền"),	_T("Xg. máy"),		_T("Môtô nước"),
//		_T("Tàu c.tốc"),	_T("Hoa tiêu"),		_T("Cứu hộ"),		_T("Tàu kéo"),
//		_T("Tiếp liệu"),	_T("C.sát biển"),	_T("Hải giám"),		_T("Kiểm ngư"),
//		_T("Tàu ngầm"),		_T("Hàng q.sự"),	_T("Chở khách"),	_T("Tàu buôn"),
//		_T("Nguy hiểm"),	_T("Containơ"),		_T("Tàu dầu"),		_T("Phao biển")};
//	BYTE	nIndex[28] = {	14, 15, 20, 30, 31, 33, 34, 35,
//		 					36, 37, 38, 39, 40, 50, 51, 52,
//							53, 55, 56, 57, 58, 59, 60, 70,
//							72, 74, 80, 85 };

//	nType = (nId < 28)? nIndex[nId] : 0;
//	return (nId < 28)? szType[nId] : _T("...");
//}

//BYTE C2_Track::FindIdTypSuf(BYTE nType)
//{
//	BYTE	nIndex[28] = {	14, 15, 20, 30, 31, 33, 34, 35,
//							36, 37, 38, 39, 40, 50, 51, 52,
//							53, 55, 56, 57, 58, 59, 60, 70,
//							72, 74, 80, 85 };

//	for (BYTE nId = 0; nId < 28; nId ++)
//	if (nType == nIndex[nId])
//		return nId;

//	return 0;
//}

//QString C2_Track::GetStrTypTer(BYTE nId, BYTE &nType)
//{
//	_TCHAR *szType[3] = {_T("Địa vật"),	_T("Tr.thăng"),	_T("Cứ điểm")};
//	BYTE	nIndex[3] = {90, 92, 96};

//	nType = (nId < 3)? nIndex[nId] : 0;
//	return (nId < 3)? szType[nId] : _T("...");
//}

//BYTE C2_Track::FindIdTypTer(BYTE nType)
//{
//	BYTE	nIndex[3] = {90, 92, 96};

//	for (BYTE nId = 0; nId < 3; nId ++)
//	if (nType == nIndex[nId])
//		return nId;

//	return 0;
//}

//QString C2_Track::GetStrTypShp(BYTE nId)
//{
//	_TCHAR* szType[27] = {_T("Chưa x.đ")    , _T("N/A")       , _T("Dành riêng")  , _T("Cánh ngầm")  , _T("Du thuyền"),
//		                  _T("HSC")         , _T("Hoa tiêu")  , _T("T.kiếm C.nạn"), _T("Tàu kéo")    , _T("Tiếp liệu"),
//	                      _T("Tàu m.trường"), _T("Thực thiPL"), _T("Dự phòng 56") , _T("Dự phòng 57"), _T("Vận tải Ytế"),
//						  _T("Ship accord") , _T("Tàu Khách") , _T("Tàu Hàng")    , _T("Chở dầu")    , _T("Tàu khác"),
//						  _T("Tàu cá")      ,_T("Towing 1")   , _T("Towing 2")    , _T("Tàu nạo vét"), _T("Tàu lặn"),
//						  _T("Tàu Q.Sự")    , _T("Thuyền Buồm")};
//	return (nId < 28)? szType[nId] : _T("...");
//}

//-----------------------------------------------------------------------------
//QString C2_Track::GetStrIden(BYTE nId)
//{
//	_TCHAR *szType[8] = {
//		_T("Chưa x.đ"),	_T("Không rõ"),	_T("Kẻ địch"),	_T("Nghi ngờ"),
//		_T("Quân sự"),	_T("Quốc tế"),	_T("Nội địa"),	_T("Quá cảnh") };
	
//	return (nId < 8)? szType[nId] : _T("...");
//}

//QString C2_Track::GetStrNaviSt(BYTE nId)
//{
//	_TCHAR *szType[13] = {	_T("Chưa x.đ"),
//		_T("Thả neo"),	_T("Đang đi"),	_T("Đi chậm"),	_T("Mất k.soát"),
//		_T("Neo dừng"),	_T("Mắc cạn"),	_T("Đánh cá"), 	_T("Thả trôi"),
//		_T("Khoan dầu"),_T("Tuần tiễu"),_T("Chặn bắt"),	_T("Cứu hộ") };

//	return (nId < 13)? szType[nId] : _T("...");
//}

//-----------------------------------------------------------------------------
// Region & Nation

//QString C2_Track::GetRegion(BYTE nId)
//{
//	_TCHAR* szRegion[10] = {_T("'Ship group, coast station, coast station group"),
//		_T("Not used - InmarsatA"), _T("Europe"), _T("North and Central America, Caribbean"),
//	_T("Asia"), _T("Oceana"), _T("Africa"), _T("America"), _T("Regional"), _T("National")};

//	return (nId < 10)? szRegion[nId]: _T("Chưa x.đ");
//}

//-----------------------------------------------------------------------------
//	Estimate Time/Date
//QString C2_Track::GetEstTime()
//{
//    QString	szStr;

//	szStr.Format(_T("%02d:%02d"), m_EsTime[0], m_EsTime[1]);
//	return szStr;
//}

//QString C2_Track::GetEstDate()
//{
//    QString	szStr;

//	szStr.Format(_T("%02d-%02d-%04d"), m_EsTime[2], m_EsTime[3],
//									  (m_EsTime[4] << 8) | m_EsTime[5]);
//	return szStr;
//}

//-----------------------------------------------------------------------------
void C2_Track::PredicPos()
{
	m_Lat	= m_Lat0 ;
	m_Long	= m_Long0;
	ConvPolToWGS(this, m_Speed * m_TmUpd / 36, m_Head);	// 36 = 1000 x 0.1nm / 3600s
}


//-----------------------------------------------------------------------------
//	Function: Update history of track: 1s
//-----------------------------------------------------------------------------
//bool C2_Track::MakeHist()
//{
//	BYTE	nType = m_Type & 0x03;

//	if (((nType == 1) && (m_TmHist <  30)) ||	// AirPlane  30s = 8.0km
//		((nType == 2) && (m_TmHist < 180)) ||	// Surface	120
//		((nType == 3) && (m_TmHist < 600)))		// SpePoint	120s = 1.2km
//	{
//		m_TmHist ++;
//		return false;
//	}

//	C2_Point *pPoint;

//	pPoint = GetHead();
//	if (pPoint)
//	if ((pPoint->m_Lat  == m_Lat ) &&
//		(pPoint->m_Long == m_Long))
//	{
//		m_TmHist --;
//		return false;
//	}

//	pPoint = new C2_Point();
//	if (!pPoint)
//		return false;

//	pPoint->m_Lat = m_Lat ;
//	pPoint->m_Long= m_Long;
//    m_ListHist.push_front(pPoint);

//	m_TmHist = 0;
//	return true;
//}

//-----------------------------------------------------------------------------
//	Clear/Count/Empty: HISTORY
//-----------------------------------------------------------------------------
void C2_Track::Clear()
{


    m_ListHist.clear();
}

int  C2_Track::Count()
{
    return m_ListHist.size();
}

bool C2_Track::IsEmpty()
{	
    return (m_ListHist.isEmpty());
}


//void C2_Track::CopyHist(C2_Track *pTrack)
//{
//	Clear();

//	C2_Point *	pPoint;
//	C2_Point *	pHist ;
//	POSITION	nPos;

//	nPos = pTrack->GetPosH();
//	while (nPos)
//	{
//		pPoint = pTrack->GetNext(nPos);

//		pHist  = new C2_Point();
//		if (pHist)
//		{
//			pHist->Update(pPoint);
//			push_back(pHist);
//		}
//	}
//}
