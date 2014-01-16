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
	pBuff = NULL;
}


dvb_c::~dvb_c()
{
	//DBG("Free ~dvb_c");
}


bool  dvb_c::DvbCInit(IQSignalCallback cb, u32 Data)
{
	CCInit();
	MdInit(cb, Data);
}



