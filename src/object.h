/*
 * object.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#pragma once


#include "typedefine.h"



#define OBJ_NAME_LENTH		32

class Object
{
	private:
		static s32     ObjCnt;
		static Object *Head;
		Object *pNext;
		s8 Name[OBJ_NAME_LENTH];
	public:
		Object();

		virtual ~Object();

		void SetObjName(const s8 *nn);
		static bool DumpObjInfo();
		static bool FreeAllObj();

};


