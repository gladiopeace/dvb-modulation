/*
 * dvb_c.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "dvb_s2.h"


dvb_s2::dvb_s2()
{
	SetObjName("dvb_s2");
	SigCb = NULL;

}


dvb_s2::~dvb_s2()
{
	//DBG("Free ~dvb_c");
}


bool  dvb_s2::DvbS2Init(ModulatorInitParam &param)
{

	BBFrameInit();
}


bool dvb_s2::Modulate(u8 *pPkt)
{
	if(NULL != SigCb)
	{
		u8 I, Q, quad;
		IQSigal sgl;

		if(true == AddPkt(pPkt))
		{
			HeadGen();
			BBRandomization();
			/*FEC encode*/
			/*mapping 	*/
			/* PL Frame encode*/
			/*modulation*/
			FrameFinish();
		}
	}
	else
	{
		ERR("with out callback.");
	}
	return true;
}

