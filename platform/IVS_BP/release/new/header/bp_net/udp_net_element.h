#ifndef _udp_net_element_h
#define _udp_net_element_h

#include "net_element.h"

NAMESPACE_BP_BEGIN

// UDPͨѶ��Ԫ��
class EXPORT_DLL CUDPElement : public INetElement
{
public:
    CUDPElement(void);
    virtual ~CUDPElement(void);

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

#endif 
