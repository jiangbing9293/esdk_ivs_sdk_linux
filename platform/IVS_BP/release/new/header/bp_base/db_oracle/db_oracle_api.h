/********************************************************************
filename    :    db_oralce_api.h
author       :    z00193167
created      :    2012/04/16
description :    
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history       :    2012/04/16 ��ʼ�汾
*********************************************************************/
#ifndef BP_DB_ORACLE_API_H
#define BP_DB_ORACLE_API_H


#include "DBInterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**********************************************************************
	* name			: CreateDBConnect
	* description	: 
	* input			: NA
	* output     	: NA
	* return     	: IDBConnect*
	* remark     	: ����һ������   
	*********************************************************************/
	DLLEXPORT IDBConnect* CreateDBConnect();

	/**********************************************************************
	* name			: InitDBContext
	* description	: 
	* input			: NA
	* output     	: NA
	* return     	: void
	* remark     	: ��ʼ�����ݿ⻷��  
	*********************************************************************/
	DLLEXPORT void InitDBContext(DBExceptionCallBack pFuncExcptionCallBack);
#ifdef __cplusplus
}
#endif


#endif // BP_DB_ORACLE_API_H
