/*
 * dvb_c.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "dvb_c.h"


dvb_c::dvb_c()
{
	SetObjName("dvb_c");
	SigCb = NULL;

}


dvb_c::~dvb_c()
{
	//DBG("Free ~dvb_c");
}


bool  dvb_c::DvbCInit(ModulatorInitParam & param)
{
	CCInit();
	SigCb = param.cb;
	CbData = param.Data;
	type = param.tp;

}


bool dvb_c::Modulate(u8 *pPkt)
{
	if(NULL != SigCb)
	{
		u8 I, Q, quad;
		u8 A,  B, Qbits;
		IQSigal sgl;

		CCEncoder(pPkt, buff2);
		Convert(buff2);
		while(true == GetSymbol(type, A, B, Qbits))
		{
			EncoderIQ(A, B, &I, &Q);
			quad = (I << 1 | Q);
			sgl = MapQamIQ(type, quad, Qbits);

			SigCb(CbData, sgl);
		}
	}
	else
	{
		ERR("with out callback.");
	}
	return true;
}

