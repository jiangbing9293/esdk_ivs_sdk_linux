/********************************************************************
 filename    :    LinkRouteMgr.h
 author      :    s00191067
 created     :    2012/11/26
 description :    ����Link·�ɹ�����;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/26 ��ʼ�汾
*********************************************************************/
#ifndef LINK_ROUTE_MGR
#define LINK_ROUTE_MGR
#include "hwsdk.h"
#include "ivs_error.h"
#include "SDKDef.h"
#include "vos.h"
#include "bp_net_factory.h"

// ����״̬
typedef enum LINKID_STATUS
{
    LINKID_STATUS_OPENNING = 0,     // �½���
    LINKID_STATUS_ENABLED,               // ���ã������ߵ�δ����
    LINKID_STATUS_OPENED,                // ����  
    LINKID_STATUS_CLOSED                 // �Ͽ�
}EM_LINKID_STATUS;//lint !e612

// ������Ϣ��
class CLinkInfo
{
public:
    CLinkInfo()
        : m_iNetElemType(NET_ELE_SMU_NSS)
        , m_iLinkStatus(LINKID_STATUS_OPENED)
    {
    };
    ~CLinkInfo()
    {
    };
    void Init(int iNetElemType, int iLinkStatus, const std::string& strLinkId, const std::string& strDomainCode, const std::string& strDomainIP, const std::string& strNVRCode)
    {
        this->m_iNetElemType = iNetElemType;
        this->m_iLinkStatus = iLinkStatus;
        this->m_strLinkId = strLinkId;
        this->m_strDomainCode = strDomainCode;
        this->m_strNVRCode = strNVRCode;
        this->m_strDomainIP = strDomainIP;
    };
	CLinkInfo& operator = (const CLinkInfo& other)
	{
		if (this == &other)
		{
			return *this;
		}
		this->m_iNetElemType = other.m_iNetElemType;
		this->m_iLinkStatus = other.m_iLinkStatus;
		this->m_strLinkId = other.m_strLinkId;
		this->m_strDomainCode = other.m_strDomainCode;
		this->m_strNVRCode = other.m_strNVRCode;
        this->m_strDomainIP = other.m_strDomainIP;
        return *this;
	};
    // ��ȡ��Ԫ����
    int GetNetElemType() const { return m_iNetElemType; };
    // ������Ԫ����
    void SetNetElemType(int iNetElemType) { m_iNetElemType = iNetElemType; };
    // ��ȡ����״̬
    int GetLinkStatus() const { return m_iLinkStatus; };
    // ��������״̬
    void SetLinkStatus(int iLinkStatus) { m_iLinkStatus = iLinkStatus; };
    // ��ȡlinkId
    std::string GetStrLinkId() const { return m_strLinkId; }; //lint !e1036 !e1536
    // ����linkId
    void SetStrLinkId(const std::string& strLinkID) { m_strLinkId = strLinkID; };
    // ��ȡ�����
    std::string GetStrDomainCode() const { return m_strDomainCode; }; //lint !e1036 !e1536
    // ���������
    void SetStrDomainCode(const std::string& strDomainCode) { m_strDomainCode = strDomainCode; };

    std::string GetStrDomainIP() const { return m_strDomainIP; };

    void SetStrDomainIP(const std::string& strDomainIP) {m_strDomainIP = strDomainIP; };

    // ��ȡNVR����
    std::string GetStrNVRCode() const { return m_strNVRCode; }; //lint !e1036 !e1536
    // ����NVR����
    void SetStrNVRCode(const std::string& strNVRCode) { m_strNVRCode = strNVRCode; };



private:
    int m_iNetElemType;      // ��Ԫ����,�ο�bp_def.h.ȡֵ��Χ��20105:NET_ELE_SMU_NSS 10105:NET_ELE_SCU_NSS 60105:NET_ELE_OMU_NSS
    int m_iLinkStatus;       // ����״̬ ȡֵ��Χ�ο�LINKID_STATUS: 0-�½�����δ�յ��첽�Ľ����ɹ���Ϣ��1-���ӣ�2-�Ͽ�
    std::string m_strLinkId;      // linkId
    std::string m_strDomainCode;  // �����
    std::string m_strDomainIP;  // ����룬����Ӧ��ҵ������У��¼������IP������벻�䣩
    std::string m_strNVRCode;

};

typedef std::list<CLinkInfo*> LINKID_INFO_LIST;
typedef LINKID_INFO_LIST::iterator LINKID_INFO_LIST_ITER;

typedef std::map<int, LINKID_INFO_LIST*> LINK_ROUTE_MAP;
typedef LINK_ROUTE_MAP::iterator LINK_ROUTE_MAP_ITER;

class CLinkRouteMgr
{
private:
    CLinkRouteMgr(void);

public:
    ~CLinkRouteMgr(void);

    static CLinkRouteMgr& instance()
    {
        static CLinkRouteMgr linkRouteMgr;
        return linkRouteMgr;
    }

	/******************************************************************
	 function   : AddLinkInfo
	 description: ��list�����һ��link( ��iSessionID��ӦList���ȴ���List �����link)
	 input      : int iSessionID ��ʶΨһ�û�
	 input      : const CLinkInfo & oLinkIDInfo ����ӵ�Link��Ϣ
	 output     : NA
	 return     : int �ɹ�����0��ʧ�ܷ���1
	*******************************************************************/
	int AddLinkInfo(int iSessionID, const CLinkInfo& oLinkIDInfo);
	
	/******************************************************************
	 function   : UpdateLinkInfo
	 description: ����LinkInfo
	 input      : int iSessionID ��ʶΨһ�û�
	 input      : const CLinkInfo& oLinkIDInfo �����µ�Link��Ϣ
	 input		��const CLinkInfo& oLinkIDInfo �Ƿ���Ҫ����
	 output     : NA
	 return     : int �ɹ�����0��ʧ�ܷ���1
	*******************************************************************/
    int UpdateLinkInfo(int iSessionID, const CLinkInfo& oLinkIDInfo, bool bInsertOnly = false);

	/******************************************************************
	 function   : RemoveLinkInfo
	 description: ��list��ɾ��һ��link
	 input      : int iSessionID ��ʶΨһ�û�
	 input      : const std::string & strLinkId ��ʶΨһ����
	 output     : NA
	 return     : int �ɹ�����0��ʧ�ܷ���1
	*******************************************************************/
	int RemoveLinkInfo(int iSessionID, const std::string& strLinkId);
	
	/******************************************************************
	 function   : DeleteLinkList
	 description: ɾ��һ���û���link List
	 input      : int iSessionID  ��ʶΨһ�û�
	 output     : NA
	 return     : void
	*******************************************************************/
	void DeleteLinkList(int iSessionID);
	
	/******************************************************************
	 function   : FindSessionIDByLinkID
	 description:  ����linkid ����session id
	 input      : const std::string & strLinkId ��ʶΨһ����
	 output     : NA
	 return     : int ��ʶΨһ�û�
	*******************************************************************/
	int FindSessionIDByLinkID(const std::string& strLinkId);
	
	/******************************************************************
	 function   : FindLinkIDInfo
	 description:  ����linkID����link info
	 input      : const std::string & strLinkId ��ʶΨһ����
	 input      : CLinkInfo & oLinkIDInfo ������Ϣ
	 output     : NA
	 return     : bool ��ѯ������true��δ�ҵ�����false
	*******************************************************************/
	bool FindLinkIDInfo(const std::string& strLinkId, CLinkInfo &oLinkIDInfo);
	
    /******************************************************************
     function   : SetLinkIDStatus
     description: ����linkid ����link״̬
     input      : const std::string & strLinkId ��ʶΨһ����
     input      : int iLinkStatus ����״̬
     output     : NA
     return     : void
    *******************************************************************/
    void SetLinkIDStatus(const std::string& strLinkId, int iLinkStatus);

    /******************************************************************
     function   : SetSmuLinkIDByDomainCode
     description: ������������linkID��Ϣ
     input      : int iSessionID
     input      : const std::string & strDomainCode
     input      : const std::string & strLinkID
     input      : int iLinkStatus
     output     : NA
     return     : void
    *******************************************************************/
    void SetLinkIDByDomainCode(int iSessionID, int iNetElemType, const std::string& strDomainCode, const std::string& strLinkID, int iLinkStatus);

    /******************************************************************
     function   : UpdateLocalDomainCode
     description: ���±���SMU��CODE
     input      : int iSessionID ��ʶΨһ�û�
     input      : const std::string & strDomainCode �������
     output     : NA
     return     : void
    *******************************************************************/
    void UpdateLocalDomainCode(int iSessionID, const std::string& strDomainCode);

    /******************************************************************
     function   : GetSmuLinkIDInfoByDomainCode
     description: ����iSessionId�� ����� ��ѯlinkIDInfo
     input      : int iSessionID
     input      : const std::string & strDomaiCode �����
     output      : CLinkInfo & oLinkIDInfo ������Ϣ
     return     : bool ��ѯ������true��δ�鵽����false
    *******************************************************************/
    bool GetSmuLinkIDInfoByDomainCode(int iSessionID, const std::string &strDomaiCode, CLinkInfo &oLinkIDInfo);

    /******************************************************************
     function   : GetScuLinkByDomainCodeAndNVRCode
     description: ����iSessionId��������NVRCode ��ѯlinkIDInfo
     input      : int iSessionID ��ʶΨһ�û�
     input      : const std::string & strDomaiCode �����
     input      : const std::string & strNVRCode NVR����
     output      : CLinkInfo & oLinkIDInfo ������Ϣ
     return     : bool ��ѯ������true��δ�鵽����false
    *******************************************************************/
    bool GetScuLinkByDomainCodeAndNVRCode(int iSessionID, const std::string &strDomaiCode, const std::string &strNVRCode, CLinkInfo &oLinkIDInfo);

    /******************************************************************
     function   : GetLinkIDInfoByDomainCode
     description: ����iSessionId����Ԫ����iNetElemType�� ����� ��ѯlinkIDInfo
     input      : int iSessionID  ��ʶΨһ�û�
     input      : int iNetElemType ��Ԫ����
     input      : const std::string & strDomaiCode �����
     output      : CLinkInfo & oLinkIDInfo  ������Ϣ
     return     : bool ��ѯ������true��δ�鵽����false
    *******************************************************************/
    bool GetLinkIDInfoByDomainCode(int iSessionID, int iNetElemType, const std::string &strDomaiCode, CLinkInfo &oLinkIDInfo);

	/******************************************************************
     function		: DecideDomainCode
     description	: ����iSessionId����Ԫ����iNetElemType�ж�������Ƿ���Ч
     input				: int iSessionID  ��ʶΨһ�û�
     input				: int iNetElemType ��Ԫ����
     input				: const std::string & strDomaiCode �����
     output			: NA
     return			: �ɹ�����IVS_SUCCESS��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 DecideDomainCode(int iSessionID, int iNetElemType, const std::string &strDomaiCode);

    /******************************************************************
     function   : DeleteLinkExceptLocalDomain
     description: ɾ����ָ�������������
     input      : int iSessionID
     input      : int iNetElemType
     input      : const std::string & strDomaiCode
     output     : NA
     return     : void
    *******************************************************************/
    void DeleteLinkExceptLocalDomain(int iSessionID, int iNetElemType, const std::string &strDomaiCode);

	/******************************************************************
	 function   : ClearAllLink
	 description: ��������û������ӣ����ͷ��ڴ�
	 output     : NA
	 return     : void
	*******************************************************************/
	void ClearAllLink();

//private:
    // ���LinkIDInfo�ṹ��
    //void FillStLinkIDInfo(const LINKID_INFO* pstLinkIdInfo, LINKID_INFO &stLinkIDInfo) const;

    // ���LinkIDInfoָ������
    //void FillPointerStLinkIDInfo(LINKID_INFO *&pstLinkIdInfo, const LINKID_INFO &stLinkIDInfo) const;
private:
	LINK_ROUTE_MAP                         m_LinkRouteMap;            // ����������Ϣ
    VOS_Mutex*                             m_pMutexLinkRouteMap;      // ������
};
#endif //LINK_ROUTE_MGR
