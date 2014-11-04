/********************************************************************
filename    :    bp_mutex.h
author      :    s90003596
created     :    2012.9.18
description :    �������ֻ࣬�ǽ�ACE��ϵ���ඨ�������
                 IVSӦ�ó���ʹ�ò�ֱ��ʹ����ЩACE�ĺ��������࣬����ʹ���������
                 �Ա�����Ҫ�滻ACE�ĳ����£����Ըɾ��������滻ACE��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.9.18 ��ʼ�汾
*********************************************************************/


#ifndef _bp_mutex_h
#define _bp_mutex_h

#include <ace/Global_Macros.h>
#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Recursive_Thread_Mutex.h>

typedef ACE_Thread_Mutex BP_Thread_Mutex;
typedef ACE_Recursive_Thread_Mutex BP_Recursive_Thread_Mutex;
typedef ACE_Null_Mutex BP_Null_Mutex;
typedef ACE_RW_Thread_Mutex BP_RW_Thread_Mutex;

#define BP_GUARD_RETURN ACE_GUARD_RETURN
#define BP_GUARD ACE_GUARD

#endif //_bp_mutex_h

