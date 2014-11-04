/********************************************************************
filename    :     bp_net_factory
author      :     w90004666
created     :     2012/09/20
description :     ��̬��������������
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_NET_FACTORY_H_
#define BP_NET_FACTORY_H_
#include "bp_namespace.h"
#include "bp_def.h"
#include "bp_net_config.h"
#include "NetElementMgr.h"
#include "tcp_server.h"


NAMESPACE_BP_BEGIN

#pragma  pack(push)
#pragma  pack(1)
    typedef struct stCreateClientInfo
{
    stCreateClientInfo(void)
    {
        memset(NetElementID, 0, sizeof(NetElementID));
        NetElementType = 0;
        memset(PeerIP, 0, sizeof(PeerIP));
        PeerPort  = 0;
        memset(LocalIP, 0, sizeof(LocalIP));
        NeedSwitchId = true;
        uiTimeout = 30;
    }

    char        NetElementID[NET_ELEMENT_ID_LEGTH];      // ��ģ���ģ��ID��NSSЭ����Ԫ��SDK���⣩����ָ��
    int         NetElementType;                          // ��Ԫ���ͣ�����
    char        PeerIP[IP_LENGTH];                       // Զ�˷�����IP
    int         PeerPort;                                // Զ�˷�����PORT
    char        LocalIP[IP_LENGTH];                      // ����IP������ΪNULL
    bool        NeedSwitchId;                            // �Ƿ��öԶ�ID���±��������е�����ID��Ĭ��Ϊtrue��SDK��Ҫָ��Ϊfalse
    uint32_t    uiTimeout;                               // �������ӳ�ʱʱ�䣬��λΪ�룬Ĭ��Ϊ30��, ��ȡֵΪ0�����ʾ�޳�ʱʱ������
} CREATE_CLIENT_INFO;

#pragma pack(pop)

class EXPORT_DLL CNetFactory
{
public:
	virtual ~CNetFactory(void) throw();
	
	/**************************************************************************
	 * name       : Instance
	 * description: �����ӿ�
	 * output     : 
	 * return     : CNetFactory&
	 * remark     :               
	 **************************************************************************/
	static CNetFactory& Instance();

	/**************************************************************************
	 * name       : CreateClientLink
	 * description: ����ĳ���Ѵ�������Ԫ����̬��������
	 * input      : std::string & strNetElementID����ģ���ģ��ID��NSSЭ����Ԫ��SDK���⣩����ָ��
	 * input      : int iNetElementType����Ԫ���ͣ�����
	 * input      : const char * pszPeerIP���Զ�IP
	 * input      : int iPeerPort���Զ˶˿�
	 * input      : const char * pszLocalIP������IP������ΪNULL
     * input      : bool bNeedSwitchId���Ƿ��öԶ�ID���±��������е�����ID��Ĭ��Ϊtrue��SDK��Ҫָ��Ϊfalse
     * input      : uint32_t uiTimeout���������ӳ�ʱʱ�䣬��λΪ�룬Ĭ��Ϊ0, ��ʾ�޳�ʱʱ������
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int CreateClientLink(std::string& strNetElementID, int iNetElementType, const char* pszPeerIP, int iPeerPort, const char* pszLocalIP, bool bNeedSwitchId = true,
                         uint32_t uiTimeout = 0);

    /**************************************************************************
	 * name       : CreateClientLinkSyn
	 * description: ����ͬ����ʽ��̬��������
	 * input      : CREATE_CLIENT_INFO& createClientInfo���������Ӳ�����Ϣ
	 * output     :
	 * return     : int
     * remark     : ͬ��������̬����ʱ��uiTimeout����ֵ������Ϊ0
     **************************************************************************/
    int CreateClientLinkSyn(CREATE_CLIENT_INFO& createClientInfo);

	/**************************************************************************
	 * name       : DeleteClientLink
	 * description: ������Ԫ���ͺ�����IDɾ��ָ������
	 * input      : const std::string & strNetElementID
	 * input      : iNetElementType
	 * output     : 
	 * return     : int
	 * remark     : 
	 **************************************************************************/
	int DeleteClientLink(const std::string& strNetElementID, int iNetElementType);

    int DeleteClientLinkByAddr(const char* pszPeerIP, int iPeerPort, int iNetElementType);
    
	/**************************************************************************
	 * name       : CreateElement
	 * description: ������Ԫ�Ļ�����Ϣ������Ԫ��ֻ����������Ԫ��û�м��ص�ͨ���߳�������
					ע�⣺
					1����Բ�ͬClient��Server�����Ԫ����Ҫ�ṩ����Ԫ������Ϣ��ͬ����Ϣ�ĸ�ʽ����Χ��ο���Ԫ�����ļ�net.xml
					   Client����Ҫ�ṩElementName��ElementType��Protocol��ServiceType������Ҫ�ṩClientList
					   Server������������Ϣ������ҪListenPort��LocalIP��ElementID�����Բ��ṩ��ȨIP�б�

	 * input      : CNetElementConfig & ElementConfig
	 * input      : INetElement * & pNetElement
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int CreateElement(CNetElementConfig& ElementConfig, INetElement*& pNetElement);

	/**************************************************************************
	 * name       : AddValidatedLink
	 * description: ��̬������Ϊ����˵���Ԫ�ĺϷ�����IP�б�
	 * input      : std::string& strPeerIP
	 *input	   : int netElemType ��Ԫ����
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int AddValidatedLink(const std::string& strPeerIP, int netElemType);

	/**************************************************************************
	 * name       : DelValidatedLink
	 * description: ��̬ɾ����Ϊ����˵���Ԫ�ĺϷ�����IP�б�
	 * input      : std::string& strPeerIP
	 *input	   : int netElemType ��Ԫ����
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int DelValidatedLink(const std::string& strPeerIP, int netElemType);

	/**************************************************************************
	 * name       : AddInvalidIP
	 * description: ��̬������Ϊ����˵���Ԫ�ķǷ�IP�б���������
	 * input      : strPeerIP   IP��ַ
	 *				netElemType ��Ԫ����
	 *              lTimeOut    �����������ʱ�䣬-1Ϊ���ã���λΪs
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int AddInvalidIP(const std::string& strPeerIP, int netElemType, long lTimeOut);

		/**************************************************************************
	 * name       : DelValidatedLink
	 * description: ��̬ɾ����Ϊ����˵���Ԫ�ķǷ�IP�б���������
	 * input      : strPeerIP   IP��ַ
	 *              netElemType ��Ԫ����
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int DelInValidIP(const std::string& strPeerIP, int netElemType);
private:
	CNetFactory(void);

	CTCPServer * GetTCPServerByNetElementType(int netElemType);

private:
	static CNetFactory m_Instance;
};

NAMESPACE_BP_END

#endif /* BP_NET_FACTORY_H_ */

