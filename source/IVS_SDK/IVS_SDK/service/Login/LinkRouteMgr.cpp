/********************************************************************
 filename    :    LinkRouteMgr.cpp
 author      :    s00191067
 created     :    2012/11/26
 description :    ����Link·�ɹ�����;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/26 ��ʼ�汾
*********************************************************************/
#include "LinkRouteMgr.h"
#include "IVSCommon.h"
#include "bp_log.h"
#include "LockGuard.h"
#include "IVS_Trace.h"
using namespace BP;

CLinkRouteMgr::CLinkRouteMgr(void)
{
    m_pMutexLinkRouteMap = VOS_CreateMutex();
    m_LinkRouteMap.clear();
}

CLinkRouteMgr::~CLinkRouteMgr(void)
{
    try
    {
        // �ͷ�map��List�ڴ棬����map
        ClearAllLink();
        // ������
        if (NULL != m_pMutexLinkRouteMap)
        {
            VOS_DestroyMutex(m_pMutexLinkRouteMap);
            m_pMutexLinkRouteMap = NULL;
        }
    }
    catch (...)
    {
    }
}

// ���һ��������Ϣ
int CLinkRouteMgr::AddLinkInfo(int iSessionID, const CLinkInfo& oLinkIDInfo)
{
    IVS_DEBUG_TRACE("iSessionID:%d", iSessionID);
    CLinkInfo* poLinkIdInfo = IVS_NEW(poLinkIdInfo);
    if (NULL == poLinkIdInfo)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Add LinkInfo failed", "alloc Memory for pstLinkIdInfo error");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    // ����Link����
	*poLinkIdInfo = oLinkIDInfo;
	LINKID_INFO_LIST* pLinkInfoList = NULL;
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);

    // map��û��iSessionID��Ӧ��List������List�����LinkInfo
	if (linkRouteMapIter == m_LinkRouteMap.end())
	{
		pLinkInfoList = IVS_NEW(pLinkInfoList);
        if (NULL == pLinkInfoList)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Add LinkInfo failed", "alloc Memory for pLinkInfoList error");
            IVS_DELETE(poLinkIdInfo);
            return IVS_ALLOC_MEMORY_ERROR;
        }
		pLinkInfoList->push_back(poLinkIdInfo);
		(void)m_LinkRouteMap.insert(make_pair(iSessionID, pLinkInfoList));
		return IVS_SUCCEED;
	}

    // map�д���iSessionID��Ӧ��List��ֱ����List���
	pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Add LinkInfo failed", "pLinkInfoList is null");
        IVS_DELETE(poLinkIdInfo);
        return IVS_OPERATE_MEMORY_ERROR;
    }

	pLinkInfoList->push_back(poLinkIdInfo);
	return IVS_SUCCEED;
}

// ����LinkInfo, ���в����ڣ���Ҫ�¼ӣ�����Ѿ����ڣ������״̬
int CLinkRouteMgr::UpdateLinkInfo(int iSessionID, const CLinkInfo& oLinkIDInfo, bool bInsertOnly)
{

    CLinkInfo* poLinkIdInfo = IVS_NEW(poLinkIdInfo);
    if (NULL == poLinkIdInfo)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Add LinkInfo failed", "alloc Memory for pstLinkIdInfo error");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    LINKID_INFO_LIST* pLinkInfoList = NULL;
   	*poLinkIdInfo = oLinkIDInfo;
    CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    // map��û��iSessionID��Ӧ��List������List�����LinkInfo
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        pLinkInfoList = IVS_NEW(pLinkInfoList);
        if (NULL == pLinkInfoList)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Add LinkInfo failed", "alloc Memory for pLinkInfoList error");
            IVS_DELETE(poLinkIdInfo);
            return IVS_ALLOC_MEMORY_ERROR;
        }
        pLinkInfoList->push_back(poLinkIdInfo);
        (void)m_LinkRouteMap.insert(make_pair(iSessionID, pLinkInfoList));
        return IVS_SUCCEED;
    }

    // map�д���iSessionID��Ӧ��List��ֱ����List���
    pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Add LinkInfo failed", "pLinkInfoList is null");
        IVS_DELETE(poLinkIdInfo);
        return IVS_OPERATE_MEMORY_ERROR;
    }

    LINKID_INFO_LIST_ITER itor = pLinkInfoList->begin();
	LINKID_INFO_LIST_ITER itorEnd = pLinkInfoList->end();
    for (; itorEnd != itor; itor++)
    {
        if (0 ==  strncmp(oLinkIDInfo.GetStrDomainCode().c_str(), (*itor)->GetStrDomainCode().c_str(), IVS_DOMAIN_CODE_LEN) 
            && (*itor)->GetNetElemType() == oLinkIDInfo.GetNetElemType())
        {
            // ���cache�д����������ͬ����IP��һ����item������Ҫ����ˢ��״̬��close or enable��
            if (bInsertOnly && 0 == strncmp(oLinkIDInfo.GetStrDomainIP().c_str(), (*itor)->GetStrDomainIP().c_str(), IVS_IP_LEN))
            {
                // ��ǰֻ������룬����������
				//add by zwx211831 �ͷ��ڴ棬�����ڴ�й¶��Date�� 20140520
				IVS_DELETE(poLinkIdInfo);
                return IVS_SUCCEED;
            }
            
            (*itor)->Init(poLinkIdInfo->GetNetElemType(), oLinkIDInfo.GetLinkStatus(), 
                oLinkIDInfo.GetStrLinkId(), oLinkIDInfo.GetStrDomainCode(), oLinkIDInfo.GetStrDomainIP(), oLinkIDInfo.GetStrNVRCode());
            IVS_DELETE(poLinkIdInfo);
            break;
        }
    }

    if (pLinkInfoList->end() == itor)
    {
        pLinkInfoList->push_back(poLinkIdInfo);
    }
    return IVS_SUCCEED;
}


// ɾ��ָ����һ������
int CLinkRouteMgr::RemoveLinkInfo(int iSessionID, const std::string& strLinkId)
{
    IVS_DEBUG_TRACE("iSessionID:%d, LinkId:%s", iSessionID, strLinkId.c_str());
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
	if (linkRouteMapIter == m_LinkRouteMap.end())
	{
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Remove LinkInfo failed", "can't find LINKID_INFO_LIST by iSessionID");
		return IVS_FAIL;
	}

	LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
	if (NULL == pLinkInfoList)
	{
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Remove LinkInfo failed", "pLinkInfoList is null");
        return IVS_FAIL;
	}

	LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
	for (; linkInfoListIter != linkInfoListIterEnd;)
	{
        CLinkInfo* poLinkInfo = *linkInfoListIter;
        if (NULL == poLinkInfo)
        {
            linkInfoListIter++;
            continue;
        }

		if (0 == strLinkId.compare(poLinkInfo->GetStrLinkId()))
		{
			CNetFactory::Instance().DeleteClientLink(poLinkInfo->GetStrLinkId(), poLinkInfo->GetNetElemType());
			IVS_DELETE(poLinkInfo);
			pLinkInfoList->erase(linkInfoListIter++);
            continue;
		}
         linkInfoListIter++; //lint !e539
	}
    return IVS_SUCCEED;
}



// ɾ��ָ���û�����������
void CLinkRouteMgr::DeleteLinkList(int iSessionID)
{
    IVS_DEBUG_TRACE("iSessionID:%d", iSessionID);
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
	if (linkRouteMapIter == m_LinkRouteMap.end())
	{
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Delete LinkList failed", "can't find LINKID_INFO_LIST by iSessionID");
		return;
	}
	LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Delete LinkList failed", "pLinkInfoList is null");
        return;
    }
	// �ͷ�list�е�����linkinfo�ڴ�
	LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
	for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
	{
		CLinkInfo* poLinkInfoTmp = *linkInfoListIter;
        if (NULL != poLinkInfoTmp)
        {
            CNetFactory::Instance().DeleteClientLink(poLinkInfoTmp->GetStrLinkId(), poLinkInfoTmp->GetNetElemType());
            IVS_DELETE(poLinkInfoTmp);
        }
	}
    pLinkInfoList->clear();
	// �ͷ�list
	IVS_DELETE(pLinkInfoList);
	// map��ɾ��list
	m_LinkRouteMap.erase(linkRouteMapIter);
}

// �������Ӳ���iSessionID
int CLinkRouteMgr::FindSessionIDByLinkID(const std::string& strLinkId)
{
    IVS_DEBUG_TRACE("strLinkId:%s", strLinkId.c_str());
	if (strLinkId.empty())
	{
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Find SessionID By LinkID failed", "strLinkId is empty");
		return SDK_SESSIONID_INIT;
	}
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.begin();
    LINK_ROUTE_MAP_ITER linkRouteMapIterEnd = m_LinkRouteMap.end();
	for (; linkRouteMapIter != linkRouteMapIterEnd; linkRouteMapIter++)
	{
		LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
		if (NULL == pLinkInfoList)
		{
			continue;
		}
		LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
        LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
		for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
		{
            CLinkInfo* poLinkIdInfo = *linkInfoListIter;
            if (NULL == poLinkIdInfo)
            {
                continue;
            }
			if (0 == strLinkId.compare(poLinkIdInfo->GetStrLinkId()))
			{
                int iSessionID = linkRouteMapIter->first;
				return iSessionID;
			}
		}
	}
    BP_RUN_LOG_ERR(IVS_FAIL, "Find SessionID By LinkID failed", "can't Find SessionID By LinkID");
	return SDK_SESSIONID_INIT;
}

// ������������
bool CLinkRouteMgr::FindLinkIDInfo(const std::string& strLinkId, CLinkInfo &oLinkIDInfo)
{
    IVS_DEBUG_TRACE("strLinkId:%s", strLinkId.c_str());
	if (strLinkId.empty())
	{
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Find LinkID Info failed", "strLinkId is empty");
		return false;
	}
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkIter = m_LinkRouteMap.begin();
    LINK_ROUTE_MAP_ITER linkRouteMapIterEnd = m_LinkRouteMap.end();
	for (; linkIter != linkRouteMapIterEnd; linkIter++)
	{
		LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkIter->second); //lint !e612
		if (NULL == pLinkInfoList)
		{
			continue;
		}
		LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
        LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
		for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
		{
            CLinkInfo* poLinkIdInfo = *linkInfoListIter;
            if (NULL == poLinkIdInfo)
            {
                continue;
            }

			if (0 == strLinkId.compare(poLinkIdInfo->GetStrLinkId()))
			{
				oLinkIDInfo = *poLinkIdInfo;
				return true;
			}
		}
	}
    BP_RUN_LOG_ERR(IVS_FAIL,"Find LinkID Info failed", "can't find stLinkIDInfo");
	return false;
}

// ����linkid ����link״̬
void CLinkRouteMgr::SetLinkIDStatus(const std::string& strLinkId, int iLinkStatus)
{
    IVS_DEBUG_TRACE("strLinkId:%s, iLinkStatus:%d", strLinkId.c_str(), iLinkStatus);
	if (strLinkId.empty() || iLinkStatus < LINKID_STATUS_OPENNING || iLinkStatus > LINKID_STATUS_CLOSED)
	{
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Set LinkID Status failed", "strLinkId is empty or iLinkStatus error");
		return;
	}
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteIterTemp = m_LinkRouteMap.begin();
    LINK_ROUTE_MAP_ITER linkRouteMapIterEnd = m_LinkRouteMap.end();
	for (; linkRouteIterTemp != linkRouteMapIterEnd; linkRouteIterTemp++)
	{
		LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteIterTemp->second); //lint !e612
		if (NULL == pLinkInfoList)
		{
			continue;
		}
		LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
        LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
		for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
		{
            CLinkInfo* poLinkIdInfo = *linkInfoListIter;
            if (NULL == poLinkIdInfo)
            {
                continue;
            }
			if (0 == strLinkId.compare(poLinkIdInfo->GetStrLinkId()))
			{
				poLinkIdInfo->SetLinkStatus(iLinkStatus);
				return;
			}
		}
	}
    BP_RUN_LOG_ERR(IVS_FAIL,"Set LinkID Status failed", "can't find stLinkIDInfo");
}

// ����linkID,linkID״̬(�ص�¼�ɹ��������LinkInfo��LinkID�Լ�iLinkStatus)
void CLinkRouteMgr::SetLinkIDByDomainCode(int iSessionID, int iNetElemType, const std::string& strDomainCode, const std::string& strLinkID, int iLinkStatus)
{
    IVS_DEBUG_TRACE("iSessionID:%d, strDomainCode:%s, strLinkID:%s, iLinkStatus:%d", iSessionID, strDomainCode.c_str(), strLinkID.c_str(), iLinkStatus);
    if (iLinkStatus < LINKID_STATUS_OPENNING || iLinkStatus > LINKID_STATUS_CLOSED)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Set SmuLinkID By DomainCode failed", "iLinkStatus error");
        return;
    }
    if (strDomainCode.empty() || strLinkID.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Set SmuLinkID By DomainCode failed", "strDomainCode or strLinkId is empty");
        return;
    }
    CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Set SmuLinkID By DomainCode failed", "can't find linkRoute in m_LinkRouteMap");
        return;
    }
    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Set SmuLinkID By DomainCode failed", "pLinkInfoList is null");
        return;
    }
    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
    for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
    {
        CLinkInfo* poLinkIdInfo = *linkInfoListIter;
        if (NULL == poLinkIdInfo)
        {
            continue;
        }
        if (iNetElemType == poLinkIdInfo->GetNetElemType() 
            && 0 == poLinkIdInfo->GetStrDomainCode().compare(strDomainCode))
        {
            poLinkIdInfo->SetStrLinkId(strLinkID);
            poLinkIdInfo->SetLinkStatus(iLinkStatus);
            return;
        }
    }
}

// ���±���CODEΪstrDomainCode(�û���½ʱ�����һ����CODEΪ��ʼ��DOMAIN_CODE_INIT��LinkID)
void CLinkRouteMgr::UpdateLocalDomainCode(int iSessionID, const std::string& strDomainCode)
{
    IVS_DEBUG_TRACE("iSessionID:%d, strDomainCode:%s", iSessionID, strDomainCode.c_str());
    if (strDomainCode.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Update Local DomainCode failed", "strLinkId is empty");
        return;
    }
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Update Local DomainCode failed", "can't find linkRoute , m_LinkRouteMap size:%d", m_LinkRouteMap.size());
        return;
    }
    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
		BP_RUN_LOG_ERR(IVS_FAIL,"Update Local DomainCode failed", "pLinkInfoList is null");
        return;
    }
    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
    for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
    {
        CLinkInfo* poLinkIdInfo = *linkInfoListIter;
        if (NULL == poLinkIdInfo)
        {
            continue;
        }
        if ( 0 == poLinkIdInfo->GetStrDomainCode().compare(LOCAL_DOMAIN_CODE_INIT))
        {
            poLinkIdInfo->SetStrDomainCode(strDomainCode);
            return;
        }
    }
    BP_RUN_LOG_INF("Update Local DomainCode failed", "pLinkInfoList size:%d", pLinkInfoList->size());
}

//����iSessionID����������SMU��LinkIDInfo��Ϣ
bool CLinkRouteMgr::GetSmuLinkIDInfoByDomainCode(int iSessionID, const std::string &strDomaiCode, CLinkInfo &oLinkIDInfo)
{
    return GetLinkIDInfoByDomainCode(iSessionID, NET_ELE_SMU_NSS, strDomaiCode, oLinkIDInfo);
}

// ����iSessionId��������NVRCode ��ѯlinkIDInfo
bool CLinkRouteMgr::GetScuLinkByDomainCodeAndNVRCode(int iSessionID, const std::string &strDomaiCode, const std::string &strNVRCode, CLinkInfo &oLinkIDInfo)
{
    IVS_DEBUG_TRACE("iSessionID��%d, strDomaiCode:%s, strNVRCode:%s", iSessionID, strDomaiCode.c_str(), strNVRCode.c_str());
    if (strDomaiCode.empty() || strNVRCode.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Get ScuLink By DomainCodeAndNVRCode failed", "strLinkId or strNVRCode is empty");
        return false;
    }
    // ����������iSessionID�����û���LinkList
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get ScuLink By DomainCodeAndNVRCode failed", "can't find linkRoute in m_LinkRouteMap");
        return false;
    }

    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get ScuLink By DomainCodeAndNVRCode failed", "pLinkInfoList is null");
        return false;
    }

    // ����iNetElemType��strDomaiCode����LinkInfo
    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
    for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
    {
        CLinkInfo* poLinkIdInfo = *linkInfoListIter;
        if (NULL == poLinkIdInfo)
        {
            continue;
        }

        if (NET_ELE_SCU_NSS == poLinkIdInfo->GetNetElemType() 
            && 0 == strDomaiCode.compare(poLinkIdInfo->GetStrDomainCode()) 
            && 0 == strNVRCode.compare(poLinkIdInfo->GetStrNVRCode()) )
        {
            oLinkIDInfo = *poLinkIdInfo;
            return true;
        }
    }
    BP_RUN_LOG_ERR(IVS_FAIL,"Get ScuLink By DomainCodeAndNVRCode failed", "can't find ScuLinkID");
    return false;
}

// ����������ѯ��������
bool CLinkRouteMgr::GetLinkIDInfoByDomainCode(int iSessionID, int iNetElemType, const std::string &strDomaiCode, CLinkInfo &oLinkIDInfo)
{
    IVS_DEBUG_TRACE("iSessionID��%d, iNetElemType: %d, strDomaiCode:%s", iSessionID, iNetElemType, strDomaiCode.c_str());
    if (strDomaiCode.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Get LinkIDInfo By DomainCode failed", "strLinkId is empty");
        return false;
    }

    // ����������iSessionID�����û���LinkList
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get LinkIDInfo By DomainCode failed", "can't find linkRoute in m_LinkRouteMap");
        return false;
    }

    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get LinkIDInfo By DomainCode failed", "pLinkInfoList is null");
        return false;
    }

    // ����iNetElemType��strDomaiCode����LinkInfo
    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
    for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
    {
        CLinkInfo* poLinkIdInfo = *linkInfoListIter;
        if (NULL == poLinkIdInfo)
        {
            continue;
        }

        if (iNetElemType == poLinkIdInfo->GetNetElemType() && 0 == strDomaiCode.compare(poLinkIdInfo->GetStrDomainCode()) )
        {
			oLinkIDInfo = *poLinkIdInfo;
            return true;
        }
    }
    BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get LinkIDInfo By DomainCode failed", "can't find pstLinkIdInfo in pLinkInfoList");
    return false;
}


//�ж�������Ƿ���Ч
IVS_INT32 CLinkRouteMgr::DecideDomainCode(int iSessionID, int iNetElemType, const std::string &strDomaiCode)
{
	IVS_DEBUG_TRACE("iSessionID��%d, iNetElemType: %d, strDomaiCode:%s", iSessionID, iNetElemType, strDomaiCode.c_str());
	if (strDomaiCode.empty())
	{
		BP_RUN_LOG_ERR(IVS_CODE_INVALID, "DomainCode is empty", "NA");
		return IVS_CODE_INVALID;
	}

	// ����������iSessionID�����û���LinkList
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
	if (linkRouteMapIter == m_LinkRouteMap.end())
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"Decide DomainCode failed", "SessionID is Invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}

	LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
	if (NULL == pLinkInfoList)
	{
		BP_RUN_LOG_ERR(IVS_SMU_USER_OFF_LINE,"Decide DomainCode failed", "pLinkInfoList is null");
		return IVS_SMU_USER_OFF_LINE;
	}

	// ����iNetElemType��strDomaiCode����LinkInfo
	LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
	LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
	for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
	{
		CLinkInfo* poLinkIdInfo = *linkInfoListIter;
		if (NULL == poLinkIdInfo)
		{
			continue;
		}

		if (iNetElemType == poLinkIdInfo->GetNetElemType() && 0 == strDomaiCode.compare(poLinkIdInfo->GetStrDomainCode()) )
		{
			return IVS_SUCCEED;
		}
	}
	BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"DomainCode not Exist", "NA");
	return IVS_SDK_ERR_DOMAINCODE_NOT_EXIST;
}

// ɾ����ָ�����ͣ���������������
void CLinkRouteMgr::DeleteLinkExceptLocalDomain(int iSessionID, int iNetElemType, const std::string &strDomaiCode)
{
    IVS_DEBUG_TRACE("iSessionID:%d, iNetElemType:%d, strDomaiCode:%d", iSessionID, iNetElemType, strDomaiCode.c_str());
    CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
    LINK_ROUTE_MAP_ITER linkRouteMapIter = m_LinkRouteMap.find(iSessionID);
    if (linkRouteMapIter == m_LinkRouteMap.end())
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Delete LinkList ExceptLocalDomain failed", "can't find LINKID_INFO_LIST by iSessionID");
        return;
    }
    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteMapIter->second); //lint !e612
    if (NULL == pLinkInfoList)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Delete LinkList ExceptLocalDomain failed", "pLinkInfoList is null");
        return;
    }
    // �ͷ�list�е�����linkinfo�ڴ�
    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
    for (; linkInfoListIter != linkInfoListIterEnd;)
    {
        CLinkInfo* poLinkInfo = *linkInfoListIter;
        if (NULL == poLinkInfo)
        {
            linkInfoListIter++;
            continue;
        }
        // ɾ����������,����SCU����
        if (0 != strDomaiCode.compare(poLinkInfo->GetStrDomainCode()) || NET_ELE_SCU_NSS == poLinkInfo->GetNetElemType())
        {
            CNetFactory::Instance().DeleteClientLink(poLinkInfo->GetStrLinkId(), poLinkInfo->GetNetElemType());
            IVS_DELETE(poLinkInfo);
            pLinkInfoList->erase(linkInfoListIter++);
            continue;
        }
        // OMU���ӣ���ɾ��TCP����
        if (NET_ELE_OMU_NSS == poLinkInfo->GetNetElemType())
        {
            CNetFactory::Instance().DeleteClientLink(poLinkInfo->GetStrLinkId(), poLinkInfo->GetNetElemType());
        }
        linkInfoListIter++; //lint !e539
    }
}

// �ͷ�����List���ڴ棬�ͷ�List�����map
void CLinkRouteMgr::ClearAllLink()
{
    // ����
	CLockGuard cLinkRouteMapLock(m_pMutexLinkRouteMap);
	LINK_ROUTE_MAP_ITER linkRouteIter = m_LinkRouteMap.begin();
	LINK_ROUTE_MAP_ITER linkRouteMapIterEnd = m_LinkRouteMap.end();
	for (;linkRouteIter != linkRouteMapIterEnd; linkRouteIter++)
	{
	    LINKID_INFO_LIST* pLinkInfoList = dynamic_cast<LINKID_INFO_LIST*>(linkRouteIter->second); //lint !e612
	    if (NULL == pLinkInfoList)
	    {
		    continue;
	    }
	    // �ͷ�list�е�����linkinfo�ڴ�
	    LINKID_INFO_LIST_ITER linkInfoListIter = pLinkInfoList->begin();
	    LINKID_INFO_LIST_ITER linkInfoListIterEnd = pLinkInfoList->end();
	    for (; linkInfoListIter != linkInfoListIterEnd; linkInfoListIter++)
	    {
            CLinkInfo* poLinkInfoTmp = *linkInfoListIter;
            if (NULL != poLinkInfoTmp)
            {
                CNetFactory::Instance().DeleteClientLink(poLinkInfoTmp->GetStrLinkId(), poLinkInfoTmp->GetNetElemType());
                IVS_DELETE(poLinkInfoTmp);
            }
	    }
	    pLinkInfoList->clear();
	    // �ͷ�list
	    IVS_DELETE(pLinkInfoList);
	}
    // ���map������list
    m_LinkRouteMap.clear();
}

