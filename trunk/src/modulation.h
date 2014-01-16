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



enum DvbMdlType_e
{
	QPSK,
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
		bool Convert(u8 *pPkt);
		bool GetSymbol(DvbMdlType_e tp, u8 &A, u8 &B, u8 &Qbits);


};

enum PunctureCodeRate
{
	CODE_RATE_1_2,
	CODE_RATE_2_3,
	CODE_RATE_3_4,
	CODE_RATE_5_6,
	CODE_RATE_7_8
};
class convolutional: public virtual Object
{

	private:
		u8 ByteBuff[204];
		u8 ByteCout;
		u32 G12BuffBit;
		u8 IQBuff[32];
		u8 IQBIdx;
		u8 IQBMax;
		u8 PunctureMax;
		u8 PuncIdx;
		u8 PunBool[16];
	public:
		convolutional()
		{
			SetObjName("convolutional");
			ByteCout = 0;
			G12BuffBit = 0 ;
			IQBIdx = 0;
			IQBMax = 0;
			PunctureMax = 0;
			PuncIdx = 0;
		};
		virtual ~convolutional() {};
		bool CvlInit(PunctureCodeRate cr);
		bool Convert(u8 *pPkt);
		bool GetSymbol(u8 &I, u8 &Q);


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
		static IQSigal QPSKMap[4];
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

		IQSigal MapQamIQ(DvbMdlType_e tp, u8 quadrant, u8 rotation);

};

typedef bool (* IQSignalCallback)(u32 CbData , IQSigal sig);


class ModulatorInitParam: public virtual Object
{
	public:
		DvbMdlType_e tp;
		IQSignalCallback cb;
		u32 Data;
		PunctureCodeRate cr;
};

