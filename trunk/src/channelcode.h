/*
 * channelcode.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#pragma once


#include "object.h"


class Randomizer: public virtual Object
{
	private:
		u8 RandOrArray[204 * 8] ;
		u32 Period;
		u32 period_idx;

	public:
		Randomizer()
		{
			SetObjName("Randomizer");
		};
		virtual ~Randomizer() {};
		bool InitRandomization(u8 PktSize);
		bool  Randomization(u8 *pPkt);
};

class FECCoder: public virtual Object
{
	private:
#define GF_ADD(a,b) a^b
#define GF_MUL(a,b) mul2(a,b)

		static u8 GF_MUL_RESULT[];
		static u8 g[];

	public:
		FECCoder()
		{
			SetObjName("FECCoder");
		};
		virtual ~FECCoder() {};

		u8 mul2(u8 a, u8 b);
		int rs_encode(u8 *pPkt);

};

class InterLeaver: public virtual Object
{
	private:
		u8 Fifo[12][204];
		u8 FifoIdx;
	public:
		InterLeaver()
		{
			SetObjName("InterLeaver");
		};
		virtual ~InterLeaver() {};

		bool InitInterLeaver();
		bool InterLeave(u8 *pPkt, u8 *pOutPut);
		bool deInterLeave(u8 *pPkt, u8 *pOutPut);


};


class ChannelCode: public Randomizer, public InterLeaver, public FECCoder
{

	public:
		ChannelCode();

		virtual ~ChannelCode();

		bool CCInit();
		bool CCEncoder(u8 *pInput, u8 *pOutput);

};


