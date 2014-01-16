/*
 * modulation.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#pragma once


#include "object.h"



enum DvbcMdlType_e
{
	QAM_16,
	QAM_32,
	QAM_64,
	QAM_128,
	QAM_256
};


struct IQSigal
{
	s8 I;
	s8 Q;
};

/*
struct Qam16Symbol
{
	u8 	qbits	: 2	;
	u8 	B	: 1	;
	u8 	A 	: 1	;
	//u8 	reserve	:4	;
};
struct Qam32Symbol
{
	u8 	qbits	: 3	;
	u8 	B	: 1	;
	u8 	A 	: 1	;
	//u8 	reserve	:3	;
};
struct Qam64Symbol
{
	u8 	qbits	: 4	;
	u8 	B	: 1	;
	u8 	A 	: 1	;
	//u8 	reserve	:2	;
};
struct Qam128Symbol
{
	u8 	qbits	: 5	;
	u8 	B	: 1	;
	u8 	A 	: 1	;
	//u8 	reserve	:1	;
};
struct Qam256Symbol
{
	u8 	qbits	: 6	;
	u8 	B	: 1	;
	u8 	A 	: 1	;
};
*/
/*
byte to symble mapping
*/
class conversion: public virtual Object
{
	private:
		u8 ByteBuff[204];
		u32 BitBuff;
		u8 BitCout;
		u8 ByteCout;
	public:
		conversion()
		{
			SetObjName("conversion");
			BitCout = 0;
			ByteCout = 0;
		};
		virtual ~conversion() {};
		bool convert(u8 *pPkt);
		bool GetSymbol(DvbcMdlType_e tp, u8 &A, u8 &B, u8 &Qbits);


};

class diffencoder: public virtual Object
{
	private:
		u8 Last_I;
		u8 Last_Q;
	public:
		diffencoder()
		{
			SetObjName("diffencoder");
			Last_I = 0 ;
			Last_Q = 0;
		};
		virtual ~diffencoder() {};
		bool EncoderIQ(u8 A, u8 B, u8 *I, u8   *Q);
};

class mapping: public virtual Object
{
	private:
		static IQSigal Qam16Map[4][4];
		static IQSigal Qam32Map[4][8];
		static IQSigal Qam64Map[4][16];
		static IQSigal Qam128Map[4][32];
		static IQSigal Qam256Map[4][64];
	public:
		mapping()
		{
			SetObjName("mapping");
		};
		virtual ~mapping() {};

		IQSigal MapQamIQ(DvbcMdlType_e tp, u8 quadrant, u8 rotation);

};

typedef bool (* IQSignalCallback)(u32 CbData , IQSigal sig);

class modulation: public conversion, public mapping, public diffencoder
{
	private:
		u32 CbData;
		IQSignalCallback SigCb;
	public:
		modulation()
		{
			SetObjName("modulation");
			SigCb = NULL;
		};

		virtual ~modulation() {};

		bool MdInit(IQSignalCallback cb, u32 Data);
		bool Modulate(DvbcMdlType_e tp , u8 *pPkt);
};


