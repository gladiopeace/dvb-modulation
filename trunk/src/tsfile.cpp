/*
 * tsfile.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */


#include "tsfile.h"


CTSFile::CTSFile(const s8 *pFileName): CFile(pFileName)
{
	m_FileState 	= TSFileUnknow;

	m_FileSize 	= 0;
	m_nPacketSize = 0;
	m_PacketCount = 0;
	m_Offset		=	0;
	SetObjName("CTSFile");

}

CTSFile::~CTSFile(void)
{
}

bool CTSFile::OpenAndAnalyse()
{
	if(false == Open(CFile::OPEN_BIN_READ_AND_WRITE))
	{
		if(false == Open(CFile::OPEN_BIN_READ_ONLY))
		{
			m_FileState = TSFileOpenFailed;
			return false;
		}
	}

	GetLenth(m_FileSize);

	if(false == Analyse())
	{
		m_FileState = TSFileAnalyseFailed;
		return false;
	}
	else
	{
		m_FileState = TSFileOK;
		return true;
	}
}

void CTSFile::OpenForWrite()
{
	if(false == Open(CFile::OPEN_BIN_WRITE_ONLY))
	{
		m_FileState = TSFileOpenFailed;
	}
	else
		m_FileState = TSFileOK;
}

bool CTSFile::Analyse(void)
{
	u32 	TestTime;
	bool Ret	= false;
	u8 data;
	u32 os = 0;

	m_Offset = 0;
	for(int i = 0; i < 204 ; i++, os++)
	{
		GetByte(os, data);
		if(data == SYN_BYTE)
		{
			/*	try 188 packet size	*/
			for(TestTime = 1; TestTime < PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				GetByte(os + 188 * TestTime, data);
				if(data != SYN_BYTE)
					break;
			}
			if(TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
			{
				Ret = true;
				m_nPacketSize = 188;
				break;
			}
			/*	try 204 packet size	*/
			for(TestTime = 1; TestTime < PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				GetByte(os + 204 * TestTime, data);
				if(data != SYN_BYTE)
					break;
			}
			if(TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
			{
				Ret = true;
				m_nPacketSize = 204;
				break;
			}
		}
	}
	if(Ret)
	{
		m_PacketCount = (u32)((m_FileSize - os) / m_nPacketSize) ;
		m_Offset = os;
	}
	return Ret;
}


bool CTSFile::TraceTsfileState()
{

	INF("File : %s " , m_FileName);
	INF("Lenth : %d " , m_FileSize);
	INF("Packet size : %d " , m_nPacketSize);
	INF("Packet count : %d " , m_PacketCount);
	INF("start_offset : %d " , m_Offset);
	INF("end_offset : %d " , (m_FileSize - (m_nPacketSize * m_PacketCount) - m_Offset));

	switch(m_FileState)
	{
		default:
		case TSFileUnknow:
			ERR("unknow error");
			break;
		case TSFileOpenFailed:
			ERR("file open failed");
			break;
		case TSFileAnalyseFailed:
			ERR("file analyse failed");
			break;
		case TSFileOK:
			INF("ts file ok! ");
			return true;
	}
	return false	;
}

u8 	CTSFile::GetPacketSize()
{
	return m_nPacketSize;
}

u32 	CTSFile::GetPacketCount()
{
	return m_PacketCount;
}

u16 CTSFile::ReadPktPID(u32 PacketIndex)
{
	u8 data1, data2;
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/

	GetByte(m_Offset + (PacketIndex * m_nPacketSize) + 1, data1);
	GetByte(m_Offset + (PacketIndex * m_nPacketSize) + 2, data2);

	return (((data1) & 0x1F) * 256 + data2);
}

void CTSFile::ReadPacket(u8 *pBuffer, u32 PacketIndex)
{
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/
	Read(pBuffer, m_nPacketSize , m_Offset + PacketIndex * m_nPacketSize);
}



bool CTSFile::IsValid(void)
{
	if(m_FileState == TSFileOK)
	{
		/*if((m_nPacketSize == 0)||(m_PacketCount == 0))
		{
			if(AnalyseAttribute())
			{
				m_FileState = TSFileReady;
				return true;
			}
			else
			{
				m_FileState = TSFileError;
				return false;
			}
		}*/
		return true;
	}
	else
		return false;
}



