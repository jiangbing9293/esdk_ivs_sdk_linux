/********************************************************************
filename    :    DumpProcess.h 
author      :    w00210470
created     :    2012/05/07
description :	 ����CoreDump�ļ� 
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/05/07 ��ʼ�汾
*********************************************************************/
#ifndef  DUMP_PROCESS_H
#define  DUMP_PROCESS_H

#include "bp_def.h"


extern "C" void EXPORT_DLL InitExceptionFilter();


void DumpProcess();

#endif  // end of  DUMP_PROCESS_H
