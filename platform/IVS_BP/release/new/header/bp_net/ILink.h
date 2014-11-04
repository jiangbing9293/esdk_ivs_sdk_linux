/********************************************************************
filename    :    ILink.h
author      :    j00213308
created     :    2012/04/06
description :    ���Ӷ���ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 ��ʼ�汾
*********************************************************************/
#ifndef ILINK_H
#define ILINK_H

#include <list>
#include "ace_header.h"
#include "NetElementMgr.h"
#include "bp_def.h"
#include "bp_stdint.h"

#include "openssl/ssl.h"

NAMESPACE_BP_BEGIN        // IVS NAMESPACE


// ����״̬
enum enLinkState
{	
	LSTATE_INIT,			// ��ʼ״̬
	LSTATE_OPENING,			// ����״̬
	LSTATE_OPEN,			// �Ѵ�״̬
	LSTATE_DELAY,			// �ӳ�״̬
	LSTATE_COMPLETE,		// ���״̬
    LSTATE_TIMEOUT,         // ��ʱ״̬���������ڶ�̬����Link��
};


// ���ͻ���
typedef struct
{
	char         * pBuffer;
	unsigned int   uiLen;
    time_t         tTime;
} SEND_BUFFER;


typedef struct 
{
	ILink * pLink;
	INetElement * pTCPServer;
}EPOLL_DATA;

typedef std::list<SEND_BUFFER* >             SendBuffList;
typedef std::list<SEND_BUFFER* >::iterator   SendBuffListIter;

#pragma pack(push, 1)

typedef struct tagLinkStatusReportInfo
{
    enLinkState LinkStatus;  //����״̬���μ�enLinkState
    char LinkId[DEVICE_CODE_LEN];
    char DevId[DEVICE_CODE_LEN];        
	char PeerIPAddr[16];
	uint32_t PeerPort;
	char LocalIPAddr[16];
	uint32_t LocalPort;
	uint32_t DevNameLen;
	char DevName[1];
}LINK_STATUS_REPORT_INFO;

#pragma pack(pop)

// Ԥ����
class CCmd;
class IProtocol;

// �������ӵĻ���
class ILink
{
public:
    ILink(void);
    virtual ~ILink(void);

    /**************************************************************************
    * name       : HandleCommand
    * description: ���ӵ������
    * input      : pCmd    : �������
    * output     : N/A
    * return     : N/A
    * remark     : N/A
    **************************************************************************/
    virtual void HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: ���д���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

    /**************************************************************************
    * name       : Open
    * description: ������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. ��ΪTCP�ͻ��ˣ�����Զ�˽�������
    *               2. ��ΪUDP�ͻ��ˣ����UDP�׽���
    **************************************************************************/
    virtual bool Open(void) = 0;

    /**************************************************************************
    * name       : Close
    * description: �ر�����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. ��ΪTCP�ͻ��ˣ�����Զ�˶Ͽ�����
    *               2. ��ΪUDP�ͻ��ˣ���ر�UDP�׽���
    **************************************************************************/
    virtual void Close(void) = 0;

    /**************************************************************************
    * name       : Send
    * description: ��������
    * input      : pBuffer : ��������
    *              uiLen   : ���ݳ���
    * output     : NA
    * return     : ������ϵ����ݳ���
    * remark     : 1. �ѷ������ݼ��뷢�ͻ������
    *              2. �����쳣�򷵻�0
    **************************************************************************/
    virtual int Send(const char* pBuffer, unsigned int uiLen, bool bIsHeartBeatMsg = false);

    /**************************************************************************
    * name       : Recv
    * description: ��������
    * input      : pBuffer : ��Ž������ݵĻ���
    *              uiLen   : ���泤��
    * output     : NA
    * return     : ���յ������ݳ���
    * remark     : 1. ���չ��̣����ӶϿ������֪ͨЭ��ջ�������ӶϿ�
    *               2. �������գ��򷵻ؽ��յ������ݳ���
    *              3. �����쳣�򷵻�0
    **************************************************************************/
    virtual int Recv(char* pBuffer, unsigned int uiLen) = 0;

    /**************************************************************************
    * name       : GetId
    * description: ��ȡLink��ID
    * input      : NA
    * output     : NA
    * return     : Link��ID
    * remark     : ��������ӽ������ٵ��ñ�����
    **************************************************************************/
    virtual const std::string& GetId(void);

    /**************************************************************************
    * name       : GetHandle
    * description: ��ȡ�׽��־��
    * input      : NA
    * output     : NA
    * return     : �׽��־��
    * remark     : ��������ӽ������ٵ��ñ�����
    **************************************************************************/
    virtual ACE_HANDLE GetHandle(void) = 0;

	// ��ȡ���ص�ַ
	virtual string GetLocalIP(void) = 0;

    // �������ӵ�Init״̬
	void HandleStateInit(void);

	// �������ӵ�Opening״̬
	void HandleStateOpening(void);

	// �������ӵ�Open״̬
	uint32_t HandleStateOpen(void);

	// �������ӵ�Delay״̬
	void HandleStateDelay(void);

	// ������¼�
	virtual void OnOpen(void) {};

	// ����ر��¼�
	virtual void OnClose(void){};

	// �����ȡ�¼�
	virtual uint32_t OnReadEvent(void);

	// ����ɷ����¼�
	virtual uint32_t OnSendEvent(void);

	// ����д�¼�
	virtual void OnWriteEvent(void){};

	// �����쳣�¼�
	virtual void OnExpEvent(void){};

	// ֪ͨ�ϲ㽨������
	bool NotifyCreateLink(void);

	// ֪ͨ�ϲ㽨������
	bool NotifyCreateLinkSync(const CCmd * pSrcCmd, bool bSuccess);

	static bool NotifyDestoryLinkSync(const CCmd* pSrcCmd, bool bSuccess);

    // ����Link �Ƿ��״̬
    inline bool IsOpen(void) { return m_iState == LSTATE_OPEN; }

    // ����Link�Ƿ�Ϊ�ͻ���
    inline bool IsClient(void) { return m_pElement->IsClient(); }

    // ����Link�Ƿ����״̬
	inline bool IsCompleted(void) { return m_iState == LSTATE_COMPLETE; }

    // �����Ƿ�̬������Link
	inline bool IsTransLink(void) { return m_bTransLink; }

    // ����LinkID
    inline void SetId(const std::string& strId) { m_strLinkId = strId; }

    // ����Link�󶨵�Э��
    inline void SetProtocol(IProtocol* pProtocol) { m_pProtocol = pProtocol; }

    // ����Link���ص�ַ
    inline void SetLocalAddr(const ACE_INET_Addr& addr) { m_localAddr.set(addr); }

    // ����Link�Զ˵�ַ
    inline void SetPeerAddr(const ACE_INET_Addr& addr) { m_peerAddr.set(addr); }

    // ���þ����
    inline void SetHandleSet(ACE_Handle_Set* pHandleSet) { m_pHandleSet = pHandleSet; }

	// ��ȡLink��������Ԫ
	inline INetElement* GetElement(void) { return m_pElement; }

	// ����Link��������Ԫ
    inline void SetElement(INetElement* pNetElement) { m_pElement = pNetElement; }

    // ����Link������ID
    inline void SetTransId(const string& strTransId) { m_strTransId = strTransId; }

    // ����Link ��̬��������
	inline void SetTransLink(bool bTransLink) { m_bTransLink = bTransLink; }

    // ��ȡLink������Э��ӿ�
    inline IProtocol* GetProtocol(void) const { return m_pProtocol; }

    // ��ȡLink�����ı��ص�ַ
    inline ACE_INET_Addr GetLocalAddr(void) const { return m_localAddr; }

    // ��ȡLink�����ĶԶ˵�ַ
    inline ACE_INET_Addr GetPeerAddr(void) const { return m_peerAddr; }

    // ��ȡLink��������Ԫ����
    inline int GetElementType(void) const { return m_pElement->GetType(); }

    // ��ȡLink����������ID
    inline string GetTransId(void) const { return m_strTransId; }

    // ��ȡLink��������ʽ�׽��֣�TCP����������
    inline ACE_SOCK_Stream& GetSockStream(void) { return m_sockStream; }

    // ��ȡLink���������ݱ��׽��֣�UDP����������
    inline ACE_SOCK_Dgram& GetSockDgram(void) { return m_sockDgram; }

    // ���� Link��ǰerrno �Ƿ�����
	inline bool IsLinkOk(int iErrNo) { return EAGAIN == iErrNo || EWOULDBLOCK == iErrNo || ETIME == iErrNo || EINTR == iErrNo; }

    // Link�Ƿ����ڴ�
	inline bool IsOpening(void) { return m_iState == LSTATE_OPENING; }

    // ��ȡ���ͻ���Ĵ�С
	inline size_t GetSendBuffListSize(void) { return m_listSendBuff.size(); }

	// ��ȡ������Ԫ��Ӧ��LinkID
	inline const std::string& GetLocalLInkId(void) { return this->m_strLocalLinkId; }

	// ���ñ�����Ԫ��Ӧ��LinkID
	inline void SetLocalLInkId(const std::string& strLocalLinkId) { this->m_strLocalLinkId = strLocalLinkId; }

	//��ѯ�Ƿ����ID����
	inline bool IsNeedSwitchId(void) { return m_bNeedSwitchId; }

	//�����Ƿ����ID����
	inline void SetNeedSwitchId(bool bNeedSwitchId) { this->m_bNeedSwitchId = bNeedSwitchId; }

    //���ð�ȫ�׽���SSL
    inline void SetSSL(SSL * pSSL) { this->m_pSSL = pSSL; }

    //���ô������ӳ�ʱʱ�䣬��λΪ��
    inline void SetTimeout(uint32_t uiTimeout) { this->m_timeout = ((LOOP_RETRY == uiTimeout)? LOOP_RETRY : ( ACE_OS::gettimeofday().sec() + uiTimeout)); }

    //�Ƿ��첽��ʽ��������
    inline bool IsAsyncCreateLink(void) { return m_bAsyncCreateLink; }

    //�����Ƿ��첽��ʽ��������
    inline void SetAsyncCreateLink(bool bAsyncCreateLink) { m_bAsyncCreateLink = bAsyncCreateLink; }

	// ��ȡepoll�����ݣ����ڷŵ�epoll��ev�epoll_wait��ʱ��������
	EPOLL_DATA* GetEpollData(){return &m_stEpollData;};

	// ����epoll�Ľ��
	void SetEpollResult(int iEvent);

    inline void sockStreamClose() { if (m_sockStream.close()!=0) { BP_RUN_LOG_WAR("tcp socket close exception.","errno=%d", errno); } }

    inline void sockDgramClose() { if (m_sockStream.close()!=0) { BP_RUN_LOG_WAR("udp socket close exception.","errno=%d", errno); } }
protected:
    // ����Linkʱ��
	void UpdateTimeStamp(void);

    // �ͷŷ��ͻ���
	void ReleaseSendBuff(void);

    // ִ�з���
	uint32_t DoSend(void);

	virtual int ExcuteSend(char* pData, unsigned int uiLen) = 0;
	
	// epoll�¼�
	void RegisterEpollAdd();
	void RegisterEpollMod(int events);
	void DeleteEpoll();

protected:
	bool			m_bNeedSwitchId;	//�Ƿ��öԶ�ID���±��������е�����ID

	bool            m_bTransLink;	    // ����ID�����ڶ�̬����link
	int             m_iState;           // ���ӵ�ǰ��״̬
	time_t          m_timeStamp;        // ����ʱ���

	//WTK�������linkId���͸��ݻ���λ���仯��32λʱΪ4�ֽڣ�64λʱΪ8�ֽڣ�����ָ��ĳ���
    std::string     m_strLinkId;        // �Զ���Ԫ���ص�id������ʹ��
    std::string     m_strLocalLinkId;   // ������Ԫ��id���Զ�ʹ��
    ACE_Handle_Set* m_pHandleSet;       // �������IO�ľ����
    ACE_SOCK_Stream m_sockStream;       // TCP������
    ACE_SOCK_Dgram  m_sockDgram;        // UDP����
    ACE_INET_Addr   m_localAddr;        // TCP,UDP�ͻ��˱��ص�ַ
	std::string     m_strLocalAddr;     // �ַ�����ʽ�����ڴ�ӡ
    ACE_INET_Addr   m_peerAddr;         // TCP,UDPԶ�˵�ַ
	std::string     m_strPeerAddr;      // �ַ�����ʽ�����ڴ�ӡ

    IProtocol*      m_pProtocol;        // Э��ջ����ָ��
    INetElement*    m_pElement;         // ����������Ԫ����
    
    string          m_strTransId;       // ���������������ID

	SendBuffList    m_listSendBuff;		// ���ͻ������
	unsigned int    m_uiSendOffset;		// �ѷ������ݳ���
	int             m_iPerPkgCnt;	    // ���η��͵����ݰ�����

    SSL           * m_pSSL;             // ��ȫ�׽��� Security Socket
    time_t          m_timeout;          // �������ӳ�ʱʱ�䣬��λΪ��
    bool            m_bAsyncCreateLink; // �Ƿ��첽��ʽ�������ӣ�Ĭ��Ϊtrue����Ϊfalse�����ʾ����ͬ����ʽ��������

	EPOLL_DATA      m_stEpollData;      // ����epoll������
	bool            m_bIsReadable;      // socket�Ƿ��пɶ�����
	bool            m_bIsWriteable;     // socket�Ƿ��п�д����
	bool            m_bIsException;     // socket�Ƿ����쳣����

    unsigned long   m_sockErrCount;       // socket���Ӵ�����������ڷ�ֹ��־��ӡ̫��
};

// ��������TCP��
class CTCPLink : public ILink
{
public:
    CTCPLink(void);
    virtual ~CTCPLink(void);

    // ���д���
    virtual uint32_t Routing(void);

    // ������
    virtual bool Open(void);

    // �ر�����
    virtual void Close(void);
   
    // ��������
    virtual int Recv(char* pBuffer, unsigned int uiLen);

	// ������¼�
	virtual void OnOpen(void);

	// ����ر��¼�	
	virtual void OnClose(void);

	// �����д�¼�
	virtual void OnWriteEvent(void);

	// �����쳣�¼�
	virtual void OnExpEvent(void);

	// ��ȡ���Ӿ��
	virtual ACE_HANDLE GetHandle(void);

    // ��ȡ����Ip��ַ
	virtual string GetLocalIP(void);

protected:
	virtual int ExcuteSend(char* pData, unsigned int uiLen);

private:
	ACE_SOCK_Connector m_sockConnector;

private:
    bool InitClientSSLCtx();
    SSL * CreateClientSSL();
    bool ClientSSLConnect();

protected:
    // SSLЭ��client����
    SSL_METHOD * m_pClientSSLMethod;    // SSLЭ����ʹ�õļӽ��ܡ�ժҪ����ȷ���
    SSL_CTX    * m_pClientSSLCtx;       // SSL��������Ϣ
    bool         m_bInitClientSSLCtx;   // �Ƿ�ɹ���ʼ��SSL������
    bool         m_bHasTryToInitCtx;    // �Ƿ��Թ�ȥ��ʼ��SSL������
};

// ��������UDP��
class CUDPLink : public ILink
{
public:
    CUDPLink(void);
    virtual ~CUDPLink(void);

    // ���д���
    virtual uint32_t Routing(void);

    // ������
    virtual bool Open(void);
    
    // �ر�����
    virtual void Close(void);

    // ��������
    virtual int Recv(char* pBuffer, unsigned int uiLen);

	// ������¼�
	virtual void OnOpen(void);

	// ����ر��¼�	
	virtual void OnClose(void);

	// ��ȡ���Ӿ��
	virtual ACE_HANDLE GetHandle(void);

    // ��ȡ���ӱ��ص�ַ
	virtual string GetLocalIP(void);

protected:
	virtual int ExcuteSend(char* pData, unsigned int uiLen);
};

NAMESPACE_BP_END

#endif // ILINK_H
