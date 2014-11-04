#ifndef _IVS_BP_SESSION_MGR_H
#define _IVS_BP_SESSION_MGR_H

#include <map>
#include "bp_stdint.h"
#include "ISession.h"
#include "bp_def.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN // namespace begin
class EXPORT_DLL CSessionMgr {
public:
    CSessionMgr();
    ~CSessionMgr();

    /**************************************************************************
    * name       : AddSession
    * description: ���ỰID�ͻỰʵ����Ψһ�󶨣�����Ự������
    * input      : 
    *               strSessionId �ỰID
    *               pSession     �Ựʵ��
    * return     : 
    *               OK �ɹ� FAIL ʧ��
    * remark     : NA
    **************************************************************************/
    int AddSession(const char *strSessionId, ISession* pSession);

    /**************************************************************************
    * name       : CheckTimeout
    * description: ������лỰ�Ƿ�ʱ
    * input      : NA
    * return     : NA
    * remark     : ��ʱ�ļ�ⷽ���ͳ�ʱ��Ķ������ɸ��Ự��������ָ��
    **************************************************************************/
    int CheckTimeout();

    /**************************************************************************
    * name       : GetSession
    * description: ��ȡ�Ựʵ��
    * input      : 
    *               strSessionId �ỰID
    * return     : ISession* �ỰID�Ķ�Ӧʵ��
    * remark     : NA
    **************************************************************************/
    ISession* GetSession(const char *strSessionId);

    /**************************************************************************
    * name       : RemoveSession
    * description: �Ƴ��Ựʵ��
    * input      : 
    *               strSessionId �ỰID
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int RemoveSession(const char *strSessionId);

	/**************************************************************************
    * name       : RoutingAll
    * description: 
    * input      : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int RoutingAll();

	/**************************************************************************
    * name       : SetSessionMapMaxSize
    * description: ���ûỰmap���������
    * input      : ulSessionMapMaxSize �������
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetSessionMapMaxSize(uint32_t uiSessionMapMaxSize) {m_uiSessionMapMaxSize =uiSessionMapMaxSize;};

	/**************************************************************************
    * name       : GetSessionMapMaxSize
    * description: ��ȡ�Ựmap���������
    * input      : NA 
    * return     : �������
    * remark     : NA
    **************************************************************************/
	uint32_t GetSessionMapMaxSize(){return m_uiSessionMapMaxSize;};


protected:
    typedef std::map<const std::string, ISession*> CSessionMap;
    typedef CSessionMap::iterator SessionIter;

    CSessionMap m_SessionMap;

	uint32_t m_uiSessionMapMaxSize;    // �Ựmap�Ĵ�С���������������ӻỰ
	ACE_Thread_Mutex                    m_mutex;  // �̸߳����б���;
    time_t m_lastPrintTime;
};

NAMESPACE_BP_END   // namespace end
#endif //_IVS_BP_SESSION_MGR_H
