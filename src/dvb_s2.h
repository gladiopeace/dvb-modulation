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






class dvb_s2: public BBFrame
{
	private:
		u32 CbData;
		IQSignalCallback SigCb;
		u8 buff2[204];
		DvbMdlType_e type;

	public:
		dvb_s2();

		virtual ~dvb_s2();
		bool  DvbS2Init(ModulatorInitParam &param);
		bool Modulate(u8 *pPkt);

};


