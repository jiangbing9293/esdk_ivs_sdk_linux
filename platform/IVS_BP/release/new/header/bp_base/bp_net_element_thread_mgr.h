/*
 * bp_net_element_thread_mgr.h
 *
 *  Created on: 2012-9-21
 *      Author: s69859
 */

#ifndef BP_NET_ELEMENT_THREAD_MGR_H_
#define BP_NET_ELEMENT_THREAD_MGR_H_
#include "bp_namespace.h"
#include "bp_errno.h"
#include "Thread.h"
#include "bp_mutex.h"

USING_NAMESPACE_BP

typedef std::map<int, CThread*>  NetElementThreadMap;
typedef std::map<int, CThread*>::iterator  NetElementThreadMapIter;

class EXPORT_DLL CNetElemenThreadMgr
{
public:
	~CNetElemenThreadMgr(void);

	static CNetElemenThreadMgr& instance();

	int addToMap(int iElementType, CThread* pThread);

    CThread* getFromMap(int iElementType);

    /**
     * ��һ����Ԫ���ͼ��뵽һ���߳��С����߳�ͨ������ѡ��
     * �ɹ�����0��ʧ�ܷ��ش�����
     */
	int addNetElemType(int iElementType, CThread *&pThread);

	void Close();
    bool GetMultiLinkConfig() { return m_bMultiLink; };
    void SetMultiLinkConfig(bool val) { m_bMultiLink = val; };
private:	
	CNetElemenThreadMgr(void);

private:
	static CNetElemenThreadMgr m_Instance;

	NetElementThreadMap m_NetElementThreadMap;

    //����addNetElemType
    NetElementThreadMap::const_iterator m_LastIter;

    BP_Recursive_Thread_Mutex m_Mutex;
    bool m_bMultiLink;
};



#endif /* BP_NET_ELEMENT_THREAD_MGR_H_ */
