/*
 * framestructure.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#pragma once


#include "object.h"



class framestructure: public virtual Object
{
	private:
	public:
		framestructure();

		virtual ~framestructure();

};

class BBFrame: public framestructure
{
		static u8 crc8table[];

		u32 MATYPE;
		u32 UPL;
		u32 DFL;
		u32 SYNC;
		u32 SYNCD;
		u8 LastCrc8;
		u32 FrameLenth;
		u32 BufferLevel;
		u8 FrameBuff[10240];
		u8 RandOrArray[16008] ;

	public:
		BBFrame() ;
		~BBFrame() {};

		void BBFrameInit();

		/* crc8 function*/
		u8 CRC8(u8 IN, u8 crc);
		u8 CalCRC8(u8 *IN, u32 len);

		/*BB head function */
		void HeadGen(void);


		/* buffer managment function*/
		bool AddPkt(u8 *pPkt);
		bool FrameFinish(void);
		bool  BBRandomization(void);
		bool  InitBBRandomization(void);

};

