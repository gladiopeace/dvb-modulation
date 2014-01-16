/*
 * tsfile.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */


#ifndef TSFILE_H
#define TSFILE_H

#include "file.h"


class CTSFile : public CFile
{
	public:
#define PACKETS_NUMBER_TO_TRY_PACKET_SIZE	20
#define SYN_BYTE			0x47


		enum TSFileState
		{
			TSFileUnknow,
			TSFileOpenFailed,
			TSFileAnalyseFailed,
			TSFileOK,

		};

		//#define SOFTSIZE	2000

		TSFileState	m_FileState;

		u32			m_FileSize;
		u8 			m_nPacketSize;
		u32 			m_PacketCount;
		u32 	 		m_Offset;


	public:
		CTSFile(const s8 *pFileName);
		//CTSFile(u8	*	FileData, u32		FileLength);
		~CTSFile(void);
		bool		    OpenAndAnalyse();
		bool 		Analyse(void);
		void            OpenForWrite();


		bool 		TraceTsfileState();

		u8 			GetPacketSize();
		u32 			GetPacketCount();
		void 		ReadPacket(u8 *pBuffer, u32 PacketIndex);
		u16 			ReadPktPID(u32 PacketIndex);
		bool 		IsValid(void);

};


#endif
