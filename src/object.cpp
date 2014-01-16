/*
 * object.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "object.h"


s32    Object::ObjCnt = 0;
Object     *Object::Head = NULL;

Object::Object()
{
	ObjCnt++;
	pNext = NULL;
	SetObjName("Obj");
	pNext = Head;
	Head = this;
}

/*
*delete me and my children but not brothers
*/
Object::~Object()
{
	Object *tmp = Head;
	ObjCnt--;

	if(Head == this)
	{
		Head = Head->pNext;
	}
	else
	{
		while((NULL != tmp->pNext) && (tmp->pNext != this))
			tmp = tmp->pNext;

		if(NULL == tmp->pNext)
		{
			ERR("obsult?");
		}
		else
		{
			tmp->pNext = pNext;
		}
	}
}

void Object::SetObjName(const s8 *nn)
{
	strcpy(Name, nn);
}

bool Object::DumpObjInfo()
{
	Object *tmp = Head;

	INF(" ***************** ");

	INF(" ObjCnt %d ", ObjCnt);
	while(NULL != tmp)
	{
		INF("Obj : %s", tmp->Name);
		tmp = tmp->pNext;
	}
	INF(" $$$$$$$$$$$$$$$$$$$ ");

	return true;
}

bool Object::FreeAllObj()
{
	Object *tmp2, *tmp ;
	tmp = Head;

	while(NULL != tmp)
	{
		tmp2 = tmp;
		tmp = tmp->pNext;
		delete tmp2;
	}
	Head = NULL;
	
	return true;
}

