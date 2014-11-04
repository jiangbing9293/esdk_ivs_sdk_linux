/********************************************************************
filename    :    LoadServiceByConfig.h
author      :    z00189721
created     :    2012/09/20
description :    ���߳̿���ģ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/09/20 ��ʼ�汾
*********************************************************************/
#ifndef _LOAD_SERVICE_BY_CONFIG_H_
#define _LOAD_SERVICE_BY_CONFIG_H_

#include "bp_config_alarmer.h"
#include "IService.h"
#include "ivs_xml.h"
#include <map>

NAMESPACE_BP_BEGIN // namespace begin

// ����service����������
typedef IService* (*CREATE_SERVICE)();

typedef std::map<int, std::string>           CMD_MAP;
typedef std::map<int, std::string>::iterator CMD_MAP_ITER;

class CServiceInfo
{
public:
	CServiceInfo(void);
	virtual ~CServiceInfo(void);

public:
	/**************************************************************************
    * name       : SetServiceName
    * description: ���÷�����
    * input      : strServiceName ������
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetServiceName(const std::string &strServiceName);

	/**************************************************************************
    * name       : SetCommandID
    * description: ������ϢID����Ϣ��
    * input      : iCommandID      ��ϢID
	               strCommandName  ��Ϣ��
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetCommandID(int iCommandID, const std::string &strCommandName);

	/**************************************************************************
    * name       : SetDllName
    * description: ���÷������
    * input      : strDllName  �������
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetDllName(const std::string &strDllName);

	/**************************************************************************
    * name       : SetDllName
    * description: �����������
    * input      : NA
    * output     : NA
    * return     : �������
    * remark     : NA    
    **************************************************************************/
	IService* CreateBusinessService();

	/**************************************************************************
    * name       : RegCapability
    * description: ע����Ϣ����
    * input      : uiServiceID ����ID
	               iThreadID   �߳�ID
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int RegCapability(uint32_t uiServiceID, ACE_thread_t iThreadID);

    void SetResendIntervalOffset(uint32_t uiResendIntervalOffset);

    uint32_t GetResendIntervalOffset() const;

    void SetResendTimes(uint32_t uiResendTimes);

    uint32_t GetResendTimes() const;

private:
	std::string m_strServiceName;  //������
	std::string m_strDllName;      //�������
	CMD_MAP     m_mapCommand;      //����֧�ֵ���Ϣcommand����Ϊ���̷߳��ʸ�map�����Բ���Ҫ��������
    uint32_t    m_uiResendIntervalOffset;
    uint32_t    m_uiResendTimes;
};

typedef std::map<std::string, CServiceInfo*>           SERVICE_MAP;
typedef std::map<std::string, CServiceInfo*>::iterator SERVICE_MAP_ITER;

class CLoadServiceByConfig : private CConfigAlarmer
{
public:
	CLoadServiceByConfig(void);
	virtual ~CLoadServiceByConfig(void);

public:
	/**************************************************************************
    * name       : Init
    * description: ��ʼ��
    * input      : strFileName �����ļ���
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int Init(const std::string &strFileName);

	/**************************************************************************
    * name       : CreateAllService
    * description: �������еķ���
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int CreateAllService();	

	/**************************************************************************
    * name       : Release
    * description: �ͷ���Դ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void Release();

    /**************************************************************************
    * name       : GetNextServiceID
    * description: ��ȡ��һ������ID
    * input      : NA
    * output     : NA
    * return     : ���ط���ID
    * remark     : NA    
    **************************************************************************/
    int GetNextServiceID();
private:
	/**************************************************************************
    * name       : GetAllService
    * description: ��ȡ���еķ�����Ϣ
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int GetAllService();

	/**************************************************************************
    * name       : GetThreadNum
    * description: ��ȡ�߳���
    * input      : NA
    * output     : NA
    * return     : �߳���
    * remark     : NA    
    **************************************************************************/
	int GetThreadNum();

	/**************************************************************************
    * name       : GetThreadName
    * description: ��ȡ�߳�����
    * input      : NA
    * output     : NA
    * return     : �߳�����
    * remark     : NA    
    **************************************************************************/
	std::string GetThreadName();

	/**************************************************************************
    * name       : GetThreadConnectDb
    * description: ��ȡ�߳��Ƿ񴴽����ݿ�����
    * input      : NA
    * output     : NA
    * return     : 1 - ��Ҫ�������ݿ����ӣ� 0 - ����Ҫ�������ݿ�����
    * remark     : NA    
    **************************************************************************/
	int GetThreadConnectDb();
	
	/**************************************************************************
    * name       : GetCmdQueueLength
    * description: ��ȡ�߳���Ϣ���г���
    * input      : NA
    * output     : NA
    * return     : ��Ϣ���г���
    * remark     : NA    
    **************************************************************************/
	int GetCmdQueueLength();

	/**************************************************************************
    * name       : GetCommand
    * description: ��ȡcommand��Ϣ
    * input      : pServiceInfo �� ������Ϣ����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int GetCommand(CServiceInfo *&pServiceInfo);

	/**************************************************************************
    * name       : CreateBusinessService
    * description: �����̺߳ͷ���
    * input      : iThreadNum ��           �߳���
	*              strThreadName��         �߳�����
	*              bThreadConnectDatabase  �߳��Ƿ��������ݿ�
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
	int CreateBusinessService(int iThreadNum, const std::string &strThreadName, bool bThreadConnectDatabase, int iCmdQueueLength);

private:
	CBB::CXml             m_ServiceXML;     // XML����
	bool                  m_bInit;          // ��ʼ�����
	int                   m_iServiceID;     // ����ID
	SERVICE_MAP           m_mapService;     // ����map��
	unsigned int          m_uiThreadTimeout;// �̳߳�ʱʱ��
};

NAMESPACE_BP_END   // namespace end

#endif //_LOAD_SERVICE_BY_CONFIG_H_
