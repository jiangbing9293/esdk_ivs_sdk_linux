/********************************************************************
filename    :    bp_load_net_service.h
author      :    w90004666
created     :    2012/9/20
description :	 ������Ԫ���ã���������ԪͨѶ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_LOAD_NET_SERVICE_H
#define BP_LOAD_NET_SERVICE_H

#include "ivs_xml.h"
#include "Thread.h"
#include "bp_namespace.h"
#include "bp_stdint.h"
#include "NetElementMgr.h"
#include "bp_net_config.h"
#include "bp_config_alarmer.h"

#include <map>
#include <list>
#include <string>

NAMESPACE_BP_BEGIN // namespace begin

class CLoadNetServcieByConfig : private CConfigAlarmer
{
public:
	CLoadNetServcieByConfig(void);
	virtual ~CLoadNetServcieByConfig(void) throw();

	/**************************************************************************
	 * name       : LoadService
	 * description: �������ļ��м�����ԪͨѶ����
	 * input      : const std::string & strConfigPath
	 * output     : NA
	 * return     : bool
	 * remark     :               
	 **************************************************************************/
	bool LoadService(const std::string& strConfigPath);

	bool CreateLinkCreater(CNetElementConfig* pElementConfig, ILinkCreater*& pLinkCreater, bool bUseDefDllName = false);

	bool CreateNetElement(CNetElementConfig* pElementConfig, CNetElementMgr* pMgr, INetElement*& pNetElement);

private:
	//��xml�м���������Ϣ
	bool LoadConfig(const std::string& strConfigPath);

	bool LoadAllThreadConfig(void);

	bool LoadAllNetElementConfig(void);

	bool LoadAllProtocolDllConfig(void);

	bool LoadNetElementListByThread(CCommuThreadConfig& ThreadConfig);

	bool LoadProtocolByNetElement(CNetProtocolConfig& ProtoclConfig);

	void LoadProtocolInfoByNetElement(CNetProtocolConfig& ProtoclConfig);

	bool LoadClientListByNetElement(std::list<CNetClientConfig>& ClientConfigList);

	bool LoadClientByNetElement(CNetClientConfig& ClientConfig);

	bool LoadServerByNetElement(CNetServerConfig& ServerConfig);

	bool LoadClientIPListByServer(CNetServerConfig& ServerConfig);

	//���ݻ����е�������Ϣ��������ԪͨѶ
	bool StartService(void);

	bool StartSingleThread(CCommuThreadConfig& ThreadConfig);

	bool AddNetElementToMgr(const std::string& strElementName, CNetElementMgr* pNetManager, CThread* pThread);

	bool CreateClientLink(CNetElementConfig* pElementConfig, INetElement* pNetElement);

	//������Ԫ���ƻ�ȡ��Ӧ����Ԫ���ö���
	CNetElementConfig* GetNetElementConfigByName(const std::string& strName);

private:
    bool                           m_bAlreadyLoad;          // �Ƿ��Ѽ���
    CBB::CXml                      m_NetXml;                // �����ļ�����
    std::list<CCommuThreadConfig>  m_ThreadConfigList;      // �߳����ñ�
    CProtocolDllConfigMap          m_ProtocolDllConfigMap;  // Э��ջ���ñ�
    CNetElementConfigMap           m_NetElementConfigMap;   // ��Ԫ���ñ�
	unsigned int                   m_uiThreadTimeout;       // �̳߳�ʱʱ��
};

NAMESPACE_BP_END   // namespace end
#endif //BP_LOAD_NET_SERVICE_H
