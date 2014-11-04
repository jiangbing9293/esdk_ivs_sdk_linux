/********************************************************************
filename    :    bp_net_config.h
author      :    w90004666
created     :    2012/9/20
description :	 ��Ԫ������Ϣ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_NET_CONFIG_H
#define BP_NET_CONFIG_H
#include <list>
#include <map>
#include "Thread.h"
#include "bp_namespace.h"
#include "bp_stdint.h"


NAMESPACE_BP_BEGIN // namespace begin

class CProtocolDllConfig;
class CNetElementConfig;

typedef std::map<std::string, CProtocolDllConfig>  CProtocolDllConfigMap; 
typedef std::map<std::string, CNetElementConfig> CNetElementConfigMap; 

class EXPORT_DLL CProtocolDllConfig
{
public:
	CProtocolDllConfig(){};
	virtual ~CProtocolDllConfig(){};

	const std::string& GetProtocolName(void) { return this->m_strProtocolName; };
	void SetProtocolName(const std::string& strName) { this->m_strProtocolName = strName; };

	const std::string& GetDllName(void) { return this->m_strDllName; };
	void SetDllName(const std::string& strName) { this->m_strDllName = strName; };

private:
	std::string m_strProtocolName;
	std::string m_strDllName;
};

class EXPORT_DLL CNetClientConfig
{
public:
	CNetClientConfig() : m_uPeerPort ( 0 ), m_uLocalPort ( 0 ) {};
	virtual ~CNetClientConfig(){};

	const std::string& GetPeerIP(void) { return this->m_PeerIP; };
	void SetPeerIP(const std::string& strName) { this->m_PeerIP = strName; };

	const std::string& GetLocalIP(void) { return this->m_LocalIP; };
	void SetLocalIP(const std::string& strName) { this->m_LocalIP = strName; };

	uint32_t GetPeerPort(void) { return this->m_uPeerPort; };
	void SetPeerPort(uint32_t uPort) { this->m_uPeerPort = uPort; };

	uint32_t GetLocalPort(void) { return this->m_uLocalPort; };
	void SetLocalPort(uint32_t uPort) { this->m_uLocalPort = uPort; };

	const std::string& GetNetElementID(void) { return this->m_strNetElementID; };
	void SetNetElementID(const std::string& strID) { this->m_strNetElementID = strID; };

private:
	std::string m_PeerIP;						//�Զ�IP
	uint32_t m_uPeerPort;						//�Զ�Port
	std::string m_LocalIP;						//����IP
	uint32_t m_uLocalPort;						//����Port
	std::string m_strNetElementID;				//������ԪID�����Զ�ʹ��
};

class EXPORT_DLL CNetServerConfig
{
public:
    CNetServerConfig() : m_uListenPort ( 0 ) {};
    virtual ~CNetServerConfig(){};

    const std::string& GetLocalIP(void) { return this->m_LocalIP; };
    void SetLocalIP(const std::string& strName) { this->m_LocalIP = strName; };

    uint32_t GetListenPort(void) { return this->m_uListenPort; };
    void SetListenPort(uint32_t uPort) { this->m_uListenPort = uPort; };

    const std::string& GetNetElementID(void) { return this->m_strNetElementID; };
    void SetNetElementID(const std::string& strID) { this->m_strNetElementID = strID; };

    void AddClientIP(const std::string& strIP);

    const std::list<std::string>& GetClientIPList() { return this->m_ClientIPList; };

private:
    std::string m_LocalIP;						//����IP
    uint32_t m_uListenPort;						//��������Port
    std::string m_strNetElementID;				//������ԪID�����Զ�ʹ��
    std::list<std::string> m_ClientIPList;		//�������ӵĿͻ����б�
};

class EXPORT_DLL CNetProtocolConfig
{
public:
    CNetProtocolConfig() : m_eTransProtocol ( LTYPE_TCP ), m_uConnectionType ( 0 ),
        m_uHeartbeatInterval ( 10 ), m_uHeartbeatTimeout ( 30 ), m_bSecuritySocket ( false ) {};
    virtual ~CNetProtocolConfig(){};

    const std::string& GetProtocolName(void) { return this->m_strProtocolName; };
    void SetProtocolName(const std::string& strName) { this->m_strProtocolName = strName; };

    LINK_TYPE GetTransProtocol(void) { return this->m_eTransProtocol; };
    void SetTransProtocol(const std::string& strProtocol);

    uint32_t GetConnectionType(void) { return this->m_uConnectionType; };
    void SetConnectionType(uint32_t uType);

    uint32_t GetHeartbeatInterval(void) { return this->m_uHeartbeatInterval; };
    void SetHeartbeatInterval(uint32_t uInterval);

    uint32_t GetHeartbeatTimeout(void) { return this->m_uHeartbeatTimeout; };
    void SetHeartbeatTimeout(uint32_t uTimeout);

    bool GetSecuritySocket(void) const { return this->m_bSecuritySocket; }
    void SetSecuritySocket(bool bSecuritySocket);

    const std::string & GetPriPemFileKey(void) const { return this->m_strPriPemFileKey; }
    void SetPriPemFileKey(const std::string & strPriPemFileKey);

private:
    std::string m_strProtocolName;				//Э������
    LINK_TYPE m_eTransProtocol;				//����Э��
    uint32_t m_uConnectionType;					//��������
    uint32_t m_uHeartbeatInterval;				//���������
    uint32_t m_uHeartbeatTimeout;				//������ʱʱ��

    bool         m_bSecuritySocket;             // �Ƿ���Ҫʹ��SSLЭ�鴫������(ֻ֧��TCP)
    std::string  m_strPriPemFileKey;            // SSL����˵�˽Կ�ļ���key(���ܹ���)
};

class EXPORT_DLL CNetElementConfig
{
public:
    CNetElementConfig() : m_bServiceType ( false ), m_uElementType( 0 ) {};
    virtual ~CNetElementConfig(){};

    const std::string& GetElementName(void) { return this->m_strElementName; };
    void SetElementName(const std::string& strName) { this->m_strElementName = strName; };

    uint32_t GetElementType(void) { return this->m_uElementType; };
    void SetElementType(uint32_t uType) { this->m_uElementType = uType; };

    bool GetServiceType(void) { return this->m_bServiceType; };
    void SetServiceType(const std::string& strType);

    CNetProtocolConfig& GetNetProtocolConfig(void) { return this->m_Protocol; };

    CNetServerConfig& GetNetServerConfig(void) { return this->m_ServerConfig; };

    std::list<CNetClientConfig>& GetNetClientConfigList(void) { return this->m_ClientConfigList; };

private:
    std::string m_strElementName;               //��Ԫ����
    uint32_t m_uElementType;                    //��Ԫ����

    CNetProtocolConfig m_Protocol;              //Э������

    bool m_bServiceType;                        //�Ƿ���Server

    CNetServerConfig m_ServerConfig;            //����Ϊ������ʱ������������

    std::list<CNetClientConfig> m_ClientConfigList;			//����Ϊ�ͻ���ʱ���ͻ������ñ�
};


/**
 * @class ͨ���߳�������
 * @brief �����net.xml�ж�ȡ��ͨ���߳����ò��������಻���𴴽��̡߳�
 */
class EXPORT_DLL CCommuThreadConfig
{
public:
	CCommuThreadConfig() : m_bConnectDb ( false ), m_uiCmdQueueLength( DEFAULT_CMD_QUEUE_LENGTH * 10 ){};
	virtual ~CCommuThreadConfig(){};

	/**************************************************************************
    * name       : IsConnectDatabase
    * description: �жϴ��߳��Ƿ���Ҫ�������ݿ�
    * input      : NA 
    * output     : NA
    * return     : true - ��Ҫ�������ݿ�, false - ����Ҫ�������ݿ�
    * remark     : NA    
    **************************************************************************/
    bool IsConnectDatabase() const { return this->m_bConnectDb; };
    void SetConnectDatabase(const bool bConnectDb) { this->m_bConnectDb = bConnectDb; };

    const std::string& GetThreadName(void) { return this->m_strThreadName; };
    void SetThreadName(const std::string& strName) { this->m_strThreadName = strName; };

    unsigned int GetCmdQueueLength(){return m_uiCmdQueueLength;};
    void SetCmdQueueLength(unsigned int uiCmdQueueLength){m_uiCmdQueueLength = uiCmdQueueLength;};

    std::list<std::string>& GetNetElementList(void) { return this->m_NetElementList; };

    void AddNetElement(const std::string& strName);

private:
    bool m_bConnectDb;                              //�Ƿ񴴽����ݿ�����
    std::string m_strThreadName;                    //�߳�����
    unsigned int m_uiCmdQueueLength;                //�߳���Ϣ���г���
    std::list<std::string> m_NetElementList;        //���̰߳󶨵���Ԫ�б�
};

NAMESPACE_BP_END   // namespace end
#endif //BP_NET_CONFIG_H
