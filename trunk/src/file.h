/*
 * file.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */


#ifndef FILE_H
#define FILE_H

#include "object.h"



class CFile : public Object
{

	public:

		enum FileOpenFlag
		{
			OPEN_TEXT_READ_ONLY,
			OPEN_TEXT_WRITE_ONLY,
			OPEN_TEXT_READ_AND_WRITE,
			OPEN_BIN_READ_ONLY,
			OPEN_BIN_WRITE_ONLY,
			OPEN_BIN_READ_AND_WRITE
		};

		FILE            *pFile;
		s8  		  m_FileName[256];

		bool			m_FileWritable;

	public:
		CFile(const s8 *lpszPathName);
		~CFile(void);

		bool 		Close(void);
		bool 		Open(FileOpenFlag flag);

		bool 		GetLenth(u32 &lenth);
		s8 		*GetFileName()
		{
			return m_FileName;
		};
		bool 		GetByte(u32 OffSet, u8 &data);
		bool 		PutByte(u32 OffSet, u8 data);
		bool             SeekTo(u32 OffSet);
		bool            ReachEnd();
		u32  		Read(u8 *pBuff, u32 lenth, s32 OffSet = -1);
		bool 		Write(u8 *pBuff, u32 lenth, s32 OffSet = -1);

};


#endif
