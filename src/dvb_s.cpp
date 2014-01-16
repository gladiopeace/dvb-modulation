/*
 * dvb_c.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "dvb_s.h"


dvb_s::dvb_s()
{
	SetObjName("dvb_s");
	SigCb = NULL;

}


dvb_s::~dvb_s()
{
	//DBG("Free ~dvb_c");
}


bool  dvb_s::DvbSInit(ModulatorInitParam &param)
{
	CCInit();
	SigCb = param.cb;
	CbData = param.Data;
	type = param.tp;
	CvlInit(param.cr);

}


bool dvb_s::Modulate(u8 *pPkt)
{
	if(NULL != SigCb)
	{
		u8 I, Q, quad;
		IQSigal sgl;

		CCEncoder(pPkt, buff2);
		Convert(buff2);
		while(true == GetSymbol(I, Q))
		{
			quad = (I << 1 | Q);
			//INF("%x, %x, %x,%x, %x, %x.",I, Q, quad, buff2[0], buff2[1], buff2[203]);
			sgl = MapQamIQ(type, quad, 0);

			SigCb(CbData, sgl);
		}
	}
	else
	{
		ERR("with out callback.");
	}
	return true;
}

