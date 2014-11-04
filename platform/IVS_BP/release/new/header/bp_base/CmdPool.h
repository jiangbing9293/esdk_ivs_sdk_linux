/********************************************************************
filename    :    CmdPool.h
author      :    j00213308
created     :    2012/05/07
description :    ��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/05/07 ��ʼ�汾
*********************************************************************/
#ifndef BP_CMD_POOL_H
#define BP_CMD_POOL_H

#include <map>
#include <list>
#include "ace_header.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN

#define BP_CMD_HIGH_WATER_MARK			40960			// �ظ�ˮλ��ʾ
#define BP_CMD_LOW_WATER_MARK			4096			// �ص�ˮλ��ʾ
#define BP_CMD_POOL_GROW_SIZE			50				// ����������
#define BP_CMD_BUFF_LEN				4096			// ���ڶ������泤��
#define BP_CMD_POOL_INIT_SIZE			500				// �س�ʼ��С

class CCmd;

typedef std::list<CCmd*>					ListCmd;
typedef std::list<CCmd*>::iterator			ListCmdIter;
typedef std::map<CCmd*, CCmd*>				MapCmd;
typedef std::map<CCmd*, CCmd*>::iterator	MapCmdIter;
typedef ACE_Recursive_Thread_Mutex	MutexThread;

// ��������
class CCmdPool
{
public:
	static CCmdPool* GetInstance(void);
	~CCmdPool(void);

	/**************************************************************************
	* name       : Init
	* description: ��ʼ����������ڴ��
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Init(void);

	/**************************************************************************
	* name       : Fnit
	* description: ������������ڴ��
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Fnit(void);

	/**************************************************************************
	* name       : Alloc
	* description: �Ӷ���ػ�ȡ�������
	* input      : uiBuffLen: ��������ڻ����С
	* output     : NA
	* return     : �������ָ��
	* remark     : NA
	**************************************************************************/
	CCmd* Alloc(unsigned int uiBuffLen);

	/**************************************************************************
	* name       : Release
	* description: �ͷ��������
	* input      : pCmd: ���ͷŵ��������ָ��
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Release(CCmd* pCmd);

private:
	CCmdPool(void);
	// ���������
	void Grow(unsigned int uiNum);

private:
	static CCmdPool*    m_pInstance;		// ��ָ��
	static MutexThread  m_mutex;			// ����

	unsigned int m_uiHighWaterMark;			// �������ظ�ˮλ��ʶ
	unsigned int m_uiLowWaterMark;			// �������ص�ˮλ��ʶ
	unsigned int m_uiGrowSize;				// �ص���������
	unsigned int m_uiBuffLen;				// ��������ڻ����С

	ListCmd		 m_freeCmdList;				// ���õ��������
	MapCmd		 m_usedCmdList;				// ��ʹ�õ��������
};

NAMESPACE_BP_END

#endif
