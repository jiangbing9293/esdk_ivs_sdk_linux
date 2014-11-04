/********************************************************************
	filename	: 	ParaCheck.h
	author		:	z00201790
	created		:	2013/01/17
	description	:	�Բ�������У�����
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/17 ��ʼ�汾
*********************************************************************/

#ifndef __PARACHECK_H__
#define __PARACHECK_H__

#include "hwsdk.h"
class CParaCheck
{
public:

    /***********************************************************************************
    * name       : CheckIsIvsBool
    * description: �������Ĳ����Ƿ�Ϊ0/1�͵�IVS_BOOL
    * input      : bIvsBool:
    * output     : NA
    * return     : ����ڷ����ڷ���IVS_SUCCEED,ʧ�ܷ���IVS_PARA_INVALID
    * remark     : NA
    ***********************************************************************************/
    static IVS_INT32 CheckIsIvsBool(IVS_BOOL bIvsBool);
};

#endif

