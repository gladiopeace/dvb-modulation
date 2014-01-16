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






class dvb_s: public ChannelCode, public framestructure, \
	public convolutional, public diffencoder, public mapping
{
	private:
		u32 CbData;
		IQSignalCallback SigCb;
		u8 buff2[204];
		DvbMdlType_e type;

	public:
		dvb_s();

		virtual ~dvb_s();
		bool  DvbSInit(ModulatorInitParam & param);
		bool Modulate(u8 *pPkt);

};


