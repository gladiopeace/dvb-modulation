/*
 * framestructure.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "framestructure.h"


framestructure::framestructure()
{
	SetObjName("framestructure");
}


framestructure::~framestructure()
{

}


u8 BBFrame::crc8table[] =
{
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

BBFrame::BBFrame()
{
	SetObjName("BBFrame");
}

void BBFrame::BBFrameInit()
{
	MATYPE = 65500;
	UPL = 188 * 8;
	SYNC = 71;
	SYNCD = 0xFFFFFFFF; /* need update*/
	FrameLenth = 16008;
	DFL = FrameLenth - 80;
	BufferLevel = 10;
	LastCrc8 = 0x47;
	InitBBRandomization();
}

u8 BBFrame::CRC8(u8 IN, u8 crc)
{

	IN ^= crc;
	crc = crc8table[IN];

	return  crc;

}

u8 BBFrame::CalCRC8(u8 *IN, u32 len)
{

	u32 idx = 0 ;
	u8 crc = 0;

	for(idx = 0; idx < len; idx++)
	{
		crc = CRC8(IN[idx], crc);
	}
	return crc;

}

void BBFrame::HeadGen(void)
{
	FrameBuff[0] = (MATYPE >> 8) & 0xFF;
	FrameBuff[1] = (MATYPE) & 0xFF;
	FrameBuff[2] = (UPL >> 8) & 0xFF;
	FrameBuff[3] = (UPL) & 0xFF;
	FrameBuff[4] = (DFL >> 8) & 0xFF;
	FrameBuff[5] = (DFL) & 0xFF;
	FrameBuff[6] =  SYNC & 0xFF;
	FrameBuff[7] = (SYNCD >> 8) & 0xFF;
	FrameBuff[8] = (SYNCD) & 0xFF;

	FrameBuff[9] = CalCRC8(FrameBuff, 9)  ;
}



bool  BBFrame::InitBBRandomization(void)
{
	u16 PRBS_reg = 0x4a80; // 100101010000000
	u32 ByteIdx, BitIdx;
	u8 CurBit;

	memset(RandOrArray, 0 , 16008);

	ByteIdx = 0;// start form 2ed byte.
	while(ByteIdx  <  16008)
	{
		BitIdx = 0;
		while(BitIdx < 8)
		{
			CurBit = (PRBS_reg & 0x1) ^ ((PRBS_reg & 0x2) >> 1);
			RandOrArray[ByteIdx] = (RandOrArray[ByteIdx] << 1) | CurBit;
			PRBS_reg = (PRBS_reg >> 1) | (CurBit << 14);
			//DBG("byteidx %d, byte 0x%x,bitidx %d, curbit %d, cur PRBS_reg 0x%04x.",
			//ByteIdx, RandOrArray[ByteIdx], BitIdx, CurBit, PRBS_reg);
			BitIdx++;
		}
		ByteIdx ++ ;
	}

	return true;

}


bool  BBFrame::BBRandomization(void)
{
	u32 ByteIdx = 0;

	while(ByteIdx < 16008)
	{
		FrameBuff[ByteIdx] = FrameBuff[ByteIdx] ^ RandOrArray[ByteIdx];
		ByteIdx++;
	}
	return true;
}


bool BBFrame::AddPkt(u8 *pPkt)
{
	pPkt[0] = LastCrc8;
	LastCrc8 = CalCRC8(pPkt , 188);
	memcpy(FrameBuff + BufferLevel, pPkt, 188);
	if(0xFFFFFFFF == SYNCD)
	{
		SYNCD = (8 * (BufferLevel - 10));
	}
	BufferLevel += 188;
	if((BufferLevel * 8) >= FrameLenth)
		return true;
	else
		return false;
}
bool BBFrame::FrameFinish(void)
{
	memcpy(FrameBuff + 10, FrameBuff + (FrameLenth / 8), (FrameLenth / 8) - 10);

	BufferLevel -= (FrameLenth / 8) - 10;
	SYNCD = 0xFFFFFFFF;
}




