#include "CloseLinkCount.h"
#include "IVSCommon.h"
#include "bp_log.h"
#include "bp_def.h"
#include "UserMgr.h"

CCloseLinkCount::CCloseLinkCount(void)
	: m_pUserMgr(NULL)
{
	m_pMutexLock = VOS_CreateMutex();
}


CCloseLinkCount::~CCloseLinkCount(void)
{
	try
	{
		CBusinessResume* pBusinessResume = NULL;
        (void)VOS_MutexLock(m_pMutexLock);
		BUSINESSRESUME_LIST_ITER InfoIter = m_listBusinessResume.begin();
		BUSINESSRESUME_LIST_ITER InfoIterEnd = m_listBusinessResume.end();
		for (; InfoIter != InfoIterEnd;)
		{
			//����ҵ�
			pBusinessResume = (CBusinessResume*)(*InfoIter);
            if (NULL == pBusinessResume)
            {
                InfoIter++;
                continue;
            }
			m_listBusinessResume.erase(InfoIter++);
			IVS_DELETE(pBusinessResume);
		}
        (void)VOS_MutexUnlock(m_pMutexLock);
		(void)VOS_DestroyMutex(m_pMutexLock);
		m_pMutexLock = NULL;
		m_pUserMgr = NULL;
	}
	catch(...)
	{
		;
	}
}

void CCloseLinkCount::SetUserMgr( CUserMgr *pUserMgr)
{
	m_pUserMgr = pUserMgr;
}

void CCloseLinkCount::onTrigger( void* /*pArg*/, ULONGLONG /*ullScales*/, TriggerStyle /*style*/ )
{
    CHECK_POINTER_VOID(m_pUserMgr);
	CLockGuard oCloseLinklock(m_pMutexLock);
    if (m_listBusinessResume.empty())
    {
        return;
    }
	CBusinessResume* pBusinessResume = NULL;
	BUSINESSRESUME_LIST_ITER InfoIter = m_listBusinessResume.begin();
	BUSINESSRESUME_LIST_ITER InfoIterEnd = m_listBusinessResume.end();
	for (; InfoIter != InfoIterEnd;)
	{
		//����ҵ�
		pBusinessResume = (CBusinessResume*)(*InfoIter);
        if (NULL == pBusinessResume)
        {
            InfoIter++;
            continue;
        }
		//CHECK_POINTER_VOID(pBusinessResume);
		if (pBusinessResume->GetCount()>=10)
		{
			//�������10�������,ɾ��
			BP_RUN_LOG_INF("CCloseLinkCount::AddLinkIdToBusinessResumeList add linkID for relink","%s",pBusinessResume->GetLinkID().c_str());
            (void)CNetFactory::Instance().DeleteClientLink(pBusinessResume->GetLinkID(), NET_ELE_SMU_NSS);
            
            // ��һ������ʧ����Ϣ
            EventCallBack fnCallBack = m_pUserMgr->GetEventCallBack();
			if (NULL != fnCallBack)
			{
				fnCallBack(IVS_EVENT_KEEP_ALIVE_FAILED, NULL, 1, m_pUserMgr->GetEventUserData());
			}
			m_pUserMgr->AddLinkIdToBusinessResumeList(pBusinessResume->GetLinkID(),
			pBusinessResume->GetNetElemType(),pBusinessResume->GetIP(),
			pBusinessResume->GetPort(),TYPE_ADD_RELINK);
			//���������Ѿ��ƶ�������Relink�У�ɾ����¼
            m_listBusinessResume.erase(InfoIter++);
			IVS_DELETE(pBusinessResume);
            continue;
			//(void)RemoveLinkIdFromBusinessList(pBusinessResume->GetLinkID());
		}

		//������1
        int iCount = pBusinessResume->GetCount() + 1;
		pBusinessResume->SetCount(iCount);

        InfoIter++;
	}
}

int CCloseLinkCount::AddLinkIdToBusinessList(const std::string& strLinkID,int iNetElemType,const std::string& strIP,int iPort)
{
	BP_RUN_LOG_INF("CCloseLinkCount::AddLinkIdToBusinessList","%s",strLinkID.c_str());
	bool bRet = false;
	CLockGuard oCloseLinklock(m_pMutexLock);
	CBusinessResume* pBusinessResume = NULL;
	BUSINESSRESUME_LIST_ITER InfoIter = m_listBusinessResume.begin();
	BUSINESSRESUME_LIST_ITER InfoIterEnd = m_listBusinessResume.end();
	for (; InfoIter != InfoIterEnd; InfoIter++)
	{
		//����ҵ�
		pBusinessResume = (CBusinessResume*)(*InfoIter);
        if (NULL == pBusinessResume)
        {
            continue;
        }
		//CHECK_POINTER(pBusinessResume,IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
		if (0 == pBusinessResume->GetLinkID().compare(strLinkID))
		{
			bRet = true;

			//����ҵ��������ü�ʱ
			pBusinessResume->SetCount(0);
			break;
		}
	}

	if (bRet)
	{
		//�ҵ���
		BP_RUN_LOG_INF("Add LinkId","find ok");
		return IVS_SUCCEED;
	}

	//û�ҵ���һ�ξͲ���
	CBusinessResume* pNewBusinessResume = IVS_NEW(pNewBusinessResume);
    pNewBusinessResume->Init(iNetElemType,iPort,0,strLinkID,strIP);
// 	pNewBusinessResume->strLinkID = strLinkID;
// 	pNewBusinessResume->iNetElemType = iNetElemType;
// 	pNewBusinessResume->strIP = strIP;
// 	pNewBusinessResume->iPort = iPort;
// 	pNewBusinessResume->iCount = 0;
	m_listBusinessResume.push_back(pNewBusinessResume);
	BP_RUN_LOG_INF("Add LinkId","insert linkID to list");
	return IVS_SUCCEED;
}

int CCloseLinkCount::RemoveLinkIdFromBusinessList(const std::string& strLinkID)
{
	BP_RUN_LOG_INF("RemoveLinkId","%s",strLinkID.c_str());
	bool bRet = false;
	CLockGuard oCloseLinklock(m_pMutexLock);
	CBusinessResume* pResume = NULL;
	BUSINESSRESUME_LIST_ITER InfoIter = m_listBusinessResume.begin();
	BUSINESSRESUME_LIST_ITER InfoIterEnd = m_listBusinessResume.end();
	for (; InfoIter != InfoIterEnd; InfoIter++)
	{
		//����ҵ�
		pResume = dynamic_cast<CBusinessResume*>(*InfoIter);
        if (NULL == pResume)
        {
            continue;
        }
		//CHECK_POINTER(pBusinessResume,IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
		if (0 == pResume->GetLinkID().compare(strLinkID))
		{
			bRet = true;

			//����ҵ���ɾ��
			m_listBusinessResume.erase(InfoIter);
			IVS_DELETE(pResume);
			break;
		}
	}

	if (bRet)
	{
		//�ҵ���
		BP_RUN_LOG_INF("RemoveLinkId","Find ok");
		return IVS_SUCCEED;
	}
	else
	{
		BP_RUN_LOG_INF("RemoveLinkId","Find fail");
		return IVS_FAIL;
	}
}
