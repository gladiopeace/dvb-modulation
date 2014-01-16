/*
 * typedefine.h - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */


#ifndef TYPEDEFINE_H

#define TYPEDEFINE_H

#include<stdio.h>
#include<string.h>

//#define _NODEBUG
//#define _NOERROR
//#define _NOINFOM
//#define STOP_WHEN_ERROR

#ifdef _NODEBUG
#define DBG(fmt, args...)
#else
#define DBG(fmt, args...) \
        printf("<DBG:%s:%d> " fmt "\n", __FUNCTION__, __LINE__, ##args)
#endif

#ifdef _NOERROR
#define ERR(fmt, args...)
#else
#ifdef STOP_WHEN_ERROR
#define ERR(fmt, args...) ({\
                printf("<ERR:%s:%s:%d> " fmt "\n", __FILE__,__FUNCTION__, __LINE__, ##args);\
                while(1);\
                })
#else
#define ERR(fmt, args...) \
                printf("<ERR:%s:%s:%d> " fmt "\n", __FILE__,__FUNCTION__, __LINE__, ##args)
#endif
#endif

#ifdef _NOINFOM
#define INF(fmt, args...)
#else
#define INF(fmt, args...) \
        printf(fmt "\n", ##args)
#endif

#define ASSERT(x) ({ \
    if (!(x)) { \
        printf("<assertion failed :%s:%s:%d> \n", __FILE__,__FUNCTION__,  __LINE__);\
        while(1); \
    } \
})

//#define NULL                                ((void *)0)
//#define BOOL                               unsigned char
//#define TRUE                                (1)
//#define FALSE                               (0)

typedef unsigned char                           	u8;
typedef  char                           	            s8;

typedef unsigned short                          	u16;
typedef short                          	        s16;

//typedef unsigned long                            u32;
//typedef long	                           		 s32;
typedef unsigned int                            u32;
typedef int	                           		 s32;

typedef unsigned long long                           u64;
typedef long long                     		 s64;

#define TestDataLenth() { \
        printf("u8 lenth is %d . \n",sizeof(u8));\
        printf("s8 lenth is %d . \n",sizeof(s8));\
                                                            \
        printf("u16 lenth is %d . \n",sizeof(u16));\
        printf("s16 lenth is %d . \n",sizeof(s16));\
                                                            \
        printf("u32 lenth is %d . \n",sizeof(u32));\
        printf("s32 lenth is %d . \n",sizeof(s32));\
                                                            \
        printf("u64 lenth is %d . \n",sizeof(u64));\
        printf("s64 lenth is %d . \n",sizeof(s64));\
}


#endif
