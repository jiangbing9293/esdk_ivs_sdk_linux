#ifndef _tcp_server_h
#define _tcp_server_h

#include <map>
#include "net_element.h"
#include "ILink.h"
#include "openssl/ssl.h"

NAMESPACE_BP_BEGIN

#define INVALIDIP_FOREVER -1
typedef struct invalidIPInfo
{
	time_t        startTime;
	long lTimeOut;
}INVALID_IP_INFO;

// TCP��������Ԫ
class EXPORT_DLL CTCPServer : public INetElement
{
public:
    CTCPServer(void);
    virtual ~CTCPServer(void);

    // �����
    virtual int HandleCmd(CCmd* pCmd);

    // �г̴���
    virtual uint32_t Routing(void);    

    // ������Ԫ
    virtual bool Startup(void);

    // ֹͣ��Ԫ
    virtual void Setdown(void);

    // �ж��Ƿ�ͻ���
    virtual bool IsClient(void);

    // ע���������¼���IO
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet);

	void SetValidatedList(const std::list<std::string>& ValidateIpList);

	//�������������IP
	void AddValidatedLink(const std::string & validateIp);

	//ɾ���������е�IP
	void DelValidatedLink(const std::string & validateIp);

	//�������������IP
	void AddInvalidIP(const std::string & strInvalidIp, long lTimeOut);

	//ɾ���������е�IP
	void DelInValidIP(const std::string & strInvalidIp);

	//����epoll�Ľ��
	void SetEpollResult(int iEvent);

private:
    // �������������Ӵ���
    uint32_t DoAccept(void);	

	//����Ƿ��ڰ�������
	bool CheckIPValidated(const char* pszClientIP);

	//����Ƿ��ں�������
	bool CheckIPInvalid(std::string strClientIP);

private:
    ACE_SOCK_Acceptor m_sockAcceptor;   // ����������������

	//ip������
	std::list<std::string> m_ValidatedIPList;

	//ip������
	typedef std::map<const std::string, INVALID_IP_INFO*> InvalidIpInfoMap;
	typedef InvalidIpInfoMap::iterator InvalidIpInfoMapIter;

	InvalidIpInfoMap m_invalidIpInfoMap;

    bool InitServerSSLCtx();
    SSL * CreateServerSSL(ACE_HANDLE aAceHandle);
    bool ServerSSLConnect(ILink *& pLink);

    // SSLЭ��server����
    SSL_METHOD * m_pServerSSLMethod;    // SSLЭ����ʹ�õļӽ��ܡ�ժҪ����ȷ���
    SSL_CTX    * m_pServerSSLCtx;       // SSL��������Ϣ
    bool         m_bInitServerSSLCtx;   // �Ƿ�ɹ���ʼ��SSL������
    bool         m_bHasTryToInitCtx;    // �Ƿ��Թ�ȥ��ʼ��SSL������

	EPOLL_DATA   m_stEpollData;         // ����epoll������
	bool         m_bIsReadable;         // socket�Ƿ��пɶ�����
	bool         m_bIsException;        // socket�Ƿ����쳣����
};

NAMESPACE_BP_END

#endif 
