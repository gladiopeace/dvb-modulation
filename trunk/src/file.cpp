/*
 * file.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "file.h"
#include <string.h>


CFile::CFile(const s8 *lpszPathName)
{
	m_FileWritable = true;
	pFile = NULL;
	strcpy(m_FileName, lpszPathName);
	SetObjName("CFile");
}


CFile::~CFile(void)
{
	Close();
}


bool CFile::Close(void)
{
	if(pFile != NULL)
	{
		fclose(pFile);
		pFile = NULL;
		return true;
	}
	return false;
}
/*
标志 含义
r, rb 以只读方式打开
w, wb 以只写方式打开。如果文件不存在，则创建该文件，否则文件被截断
a, ab 以追加方式打开。如果文件不存在，则创建该文件
r+, r+b, rb+ 以读写方式打开
w+, w+b, wh+ 以读写方式打开。如果文件不存在时，创建新文件，否则文件被截断
a+, a+b, ab+ 以读和追加方式打开。如果文件不存在，创建新文件
*/
bool CFile::Open(FileOpenFlag flag)
{

	if(NULL != pFile)
	{
		Close();
	}
	switch(flag)
	{
		case OPEN_TEXT_READ_ONLY:
			pFile = fopen((const char *)m_FileName, "r");
			m_FileWritable = false;
			break;
		case OPEN_TEXT_WRITE_ONLY:
			pFile = fopen((const char *)m_FileName, "w");
			m_FileWritable = true;
			break;
		case OPEN_TEXT_READ_AND_WRITE:
			pFile = fopen((const char *)m_FileName, "r+");
			m_FileWritable = true;
			break;
		case OPEN_BIN_READ_ONLY:
			pFile = fopen((const char *)m_FileName, "rb");
			m_FileWritable = false;
			break;
		case OPEN_BIN_WRITE_ONLY:
			pFile = fopen((const char *)m_FileName, "wb");
			m_FileWritable = true;
			break;
		case OPEN_BIN_READ_AND_WRITE:
			pFile = fopen((const char *)m_FileName, "rb+");
			m_FileWritable = true;
			break;
		default:
			ERR("open flag unknow.");
			break;
	}

	if(NULL == pFile)
	{
		return false;
	}

	return true;
}


bool CFile::GetLenth(u32 &lenth)
{
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	lenth = ftell(pFile);
	return true;
}


bool CFile::GetByte(u32 OffSet, u8 &data)
{
	if(NULL == pFile)
	{
		return false;
	}
	fseek(pFile, OffSet, 0);
	data =  fgetc(pFile);

	return true;
}

bool CFile::PutByte(u32 OffSet, u8 data)
{
	if((NULL == pFile) || (false == m_FileWritable))
	{
		return false;
	}

	fseek(pFile, OffSet, 0);
	fputc(data, pFile);

	return true;
}

bool CFile::SeekTo(u32 OffSet)
{
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, OffSet, 0);

	return true;
}

bool CFile::ReachEnd()
{
	if(NULL == pFile)
	{
		return false;
	}
	return feof(pFile);
}

u32 CFile::Read(u8 *pBuff, u32 lenth, s32 OffSet)
{
	u32 readsize = 0;

	if(NULL == pFile)
	{
		return false;
	}
	if(OffSet >= 0)
	{
		fseek(pFile, OffSet, 0);
	}
	readsize =  fread(pBuff, 1,  lenth,  pFile);

	return readsize;
}

bool CFile::Write(u8 *pBuff, u32 lenth, s32 OffSet)
{
	u32 writesize;

	if((NULL == pFile) || (false == m_FileWritable))
	{
		return false;
	}
	if(OffSet >= 0)
	{
		fseek(pFile, OffSet, 0);
	}
	writesize = fwrite(pBuff, 1, lenth, pFile);

	return true;
}



