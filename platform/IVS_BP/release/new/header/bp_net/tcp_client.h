#ifndef _tcp_client_h
#define _tcp_client_h


#include "net_element.h"


NAMESPACE_BP_BEGIN

// TCPͨѶ��Ԫ��
class EXPORT_DLL CTCPClient : public INetElement
{
public:
    CTCPClient(void);
    virtual ~CTCPClient(void);

    // �����
    virtual int HandleCmd(CCmd* pCmd);

    // �г̴���
    virtual uint32_t Routing(void);

    // �ж��Ƿ�ͻ���
    virtual bool IsClient(void);

    // ע���������¼���IO
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet);
};


NAMESPACE_BP_END

#endif //
