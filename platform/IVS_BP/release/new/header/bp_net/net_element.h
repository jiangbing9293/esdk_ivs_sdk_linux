/********************************************************************
filename    :    net_element.h
author      :    j00213308
created     :    2012/04/07
description :    ��Ԫ������Ԫ���ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 ��ʼ�汾
*********************************************************************/
#ifndef _net_element_h
#define _net_element_h

#include <map>
#include "ace_header.h"
#include "bp_def.h"
#include <list>

#ifndef WIN32
//Linux��socketͷ�ļ�
#include <sys/epoll.h>
#else
#define EPOLLIN 0
#define EPOLLOUT 0
#endif

NAMESPACE_BP_BEGIN        // IVS NAMESPACE

// ����Ԥ����
class CCmd;
class ILink;
class ILinkCreater;
class CNetElementMgr;

// �����ض���
typedef std::map<std::string, ILink*>            CLinkMap;          // Link������ACE socket�����Ӧmap
typedef CLinkMap::iterator  LinkMapIterator;
typedef std::map<string, ILink*>                 CTransLinkMap;     // Link����������ID(string) map
typedef CTransLinkMap::iterator       TransLinkMapIterator;
typedef ACE_Recursive_Thread_Mutex               RecursiveMutex;

//�����windows������յĽṹ��
#ifdef WIN32
struct epoll_event
{
};
#endif

//��Ԫ��������IPV6��IPV4
#define IPV6_MIN_NET_ELEMENT_TYPE 60000
#define IPV4_MIN_NET_ELEMENT_TYPE 40000



// ��Ԫ�ӿ�
class EXPORT_DLL INetElement
{
public:
    INetElement(void);
    virtual ~INetElement(void);

    /**************************************************************************
    * name       : HandleCmd
    * description: ��Ϣ�����
    * input      : pCmd : ���������Ϣ����
    * output     : NA
    * return     : 0 - ��Ϣ�����Ѵ���, -1 - ��Ϣ������δ����
    * remark     : ���Ѵ�����Ϣ��������������������ģ����������
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: �г̴���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void) = 0;

    /**************************************************************************
    * name       : Startup
    * description: ������Ԫ
    * input      : NA
    * output     : NA
    * return     : true - �ɹ�, false - ʧ��
    * remark     : 1. ʹ����Ԫǰ�����ô˺�������ʾ��Ԫ������
    *              2. ����TCP�ͻ�����Ԫ,�ط���true
    *              3. ����TCP��������Ԫ,��򿪼���
    *              4. ����UDP��Ԫ,�ط���true
    **************************************************************************/
    virtual bool Startup(void);

    /**************************************************************************
    * name       : Setdown
    * description: ֹͣ��Ԫ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. ֹͣ��Ԫ��ͬʱ�ر�������������
    *              2. ����TCP�ͻ�����Ԫ,�ر�������������
    *              3. ����TCP��������Ԫ,�ر������������ӣ���ֹͣ����
    *              4. ����UDP��Ԫ,�ر���������UDP�׽���
    **************************************************************************/
    virtual void Setdown(void);

    // ��Ԫ�Ƿ��ǿͻ���
    virtual bool IsClient(void) = 0;
    
    // ע��Select�����
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet) = 0;

    // ������Ƿ�ɶ�
    bool PollReadEvent(ACE_HANDLE handle);

    // ������Ƿ��д
    bool PollWriteEvent(ACE_HANDLE handle);

    // ������Ƿ����쳣
    bool PollExpEvent(ACE_HANDLE handle);

    // ע��̶����Ӿ����select�����
    void RegisterLinkIdHandle();
	

    // ע�ᶯ̬���������Ӿ����select�����
    void RegisterTransLinkIdHandle();

    // ��ȡ���Ӷ���
    ILink* GetLink(const std::string& strLinkId);

    // ��ȡ���Ӷ��󡣷���IP�˿���ͬ������
    ILink* GetLinkByPeerAddr(const ACE_INET_Addr* peerAddr);

    // ����LinkId ���Link����
    bool AddLink(const std::string& strLinkId, ILink* pLink);

    // ����LinkId ɾ��link����
    ILink* RmvLink(const std::string& strLinkId);

	int UpdateLink(const std::string& strOldLinkId, const std::string& strNewLinkId);

    // ��������ID�����Link����(��̬)
    bool AddLinkByTransId(const string& strTransId, ILink* pLink);

    // ��������ID  ɾ��Link����
    ILink* RmvLinkByTransId(const string& strTransId);

    // ������Ԫ����
    void SetType(int iType);

    // ������ԪKeepAlive����
    void SetKeepAlive(bool bKeepAlive);

    // ��ȡ��ԪKeepAlive����
    bool GetKeepAlive(void);

    // ������Ԫ�����Ӵ�����
    ILinkCreater* GetLinkCreater(void);

    // ������Ԫ�����Ӵ�����
    void SetLinkCreater(ILinkCreater* plinkCreater);

    // ������Ԫ���ص�ַ
    void SetLocalAddr(const ACE_INET_Addr & addr);
    
    // ������Ԫ�Զ˵�ַ
    void SetPeerAddr(const ACE_INET_Addr & addr);

    // ������Ԫ����ʵ����ַ
    void SetElementManger(CNetElementMgr* pMgr);

    // ��ȡ��Ԫ����
    int  GetType(void) const;

    // ��ȡ��Ԫ���ص�ַ
    const ACE_INET_Addr& GetLocalAddr(void) const;

    // ��ȡ��Ԫ�Զ˵�ַ
    const ACE_INET_Addr& GetPeerAddr(void) const;

	// ��ȡ������Ԫ��Ӧ��LinkID
	const std::string& GetLocalLinkId(void);

	// ���ñ�����Ԫ��Ӧ��LinkID
	void SetLocalLinkId(const std::string& strLocalLinkId);

    // ��֪�Ƿ��贴����ȫ�׽���
    bool GetSecuritySocket(void) const;

    // ��֪SSL����˵�˽Կ�ļ���key(���ܹ���)
    const std::string & GetPriPemFileKey(void) const;

	// ��ȡepoll��FD
	int GetEpollFD(void);

protected:
    // ������ָ��
    int HandleSendCmd(CCmd* pCmd);

    // ����������ָ��
    int HandleCreateLinkCmd(CCmd* pCmd);

    // ����������ָ��
    int HandleDestroyLinkCmd(const CCmd* pCmd);

	// ����ͬ����������ָ��
	int HandleCreateLinkSyncCmd(CCmd* pCmd);

	// ������������ָ��
	int HandleDestroyLinkSyncCmd(CCmd* pCmd);

    // ��������
    ILink* CreateLink(void);

    // �����������ӵ�routing
    uint32_t DoLinksRouting(void);

    uint32_t DoLinkIdRouting(void);

    uint32_t DoTransLinkIdRouting(void);

protected:
    CLinkMap        m_mapLink;           // ���泤���Ӷ���
    CTransLinkMap   m_mapTransLink;      // ��transaction�������
    
    ACE_Time_Value  m_timeout;           // ��Ԫ��ʱ�¼�
    ACE_INET_Addr   m_localAddr;         // ��Ԫ���ص�ַ
    ACE_INET_Addr   m_peerAddr;          // ��ԪԶ�˵�ַ
    RecursiveMutex  m_lock;              // ͬ����
    ACE_Handle_Set* m_pRdHandleSet;      // �������input�¼�
    ACE_Handle_Set* m_pWtHandleSet;      // �������output�¼�
    ACE_Handle_Set* m_pExHandleSet;      // �������exception�¼�


    CNetElementMgr* m_pElementMgr;       // ��Ԫ����������
    ILinkCreater*   m_pLinkCreater;      // ���Ӵ�����

    int             m_iType;             // ��Ԫ����
    bool            m_bKeepAlive;        // ���ӷ�ʽ����������)

    std::string     m_strLocalLinkId;   // ������Ԫ��id���Զ�ʹ�ã�ֻ�е���ԪΪServerʱ������ֶβ�����Ч�ģ�����	
};


NAMESPACE_BP_END

#endif //
