/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/
#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/
#include "user_app.h"
#include "user_cfg.h"
#include "user_bsp.h"

#include  <bsp.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <ucos_ii.h>


enum
{
	MSG_BackPic = 0,		
    MSG_TaskInfo,
    MSG_SaveSR,
    MSG_BackLight,
	MSG_BackPicPressed
};


/* ����һ�����䣬    ��ֻ��һ������ָ�룬  OSMboxCreate�����ᴴ������������Դ */
extern OS_EVENT *AppUserIFMbox;
extern OS_EVENT *AppMsgMbox;
extern OS_EVENT *COM1_Mbox;
/* ����һ���ź�����  ��ֻ��һ���ź���ָ�룬OSSEMCreate�����ᴴ������������Դ */
extern OS_EVENT *Semp;
extern OS_EVENT *AppBackpicSemp;


#endif
