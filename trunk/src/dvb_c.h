/*
 * dvb_c.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */



#pragma once


#include "channelcode.h"
#include "framestructure.h"
#include "modulation.h"




class dvb_c: public ChannelCode, public framestructure,public modulation
{
	u8 * pBuff;

public:
	dvb_c();
	
	virtual ~dvb_c();
	bool  DvbCInit(IQSignalCallback cb, u32 Data);
};


