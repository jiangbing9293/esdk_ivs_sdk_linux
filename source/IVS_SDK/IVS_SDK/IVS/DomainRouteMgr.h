/********************************************************************
 filename    :    DomainRouteMgr.h
 author      :    chenxianxiao 00206592
 created     :    2013/01/17
 description :    ��·�ɹ���
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/17 ��ʼ�汾
*********************************************************************/
#ifndef __DOMAIN_ROUTE_MGR__
#define __DOMAIN_ROUTE_MGR__

#include "ivs_error.h"
#include "hwsdk.h"
#include "vos.h"
// #include <vector>
// #include <list>
// #include <map>
#include "ivs_xml.h"
#include "SDKDef.h"

//��·��map
typedef std::map<std::string, IVS_DOMAIN_ROUTE*> DOMAIN_CODE_LIST_MAP;
typedef DOMAIN_CODE_LIST_MAP::iterator DOMAIN_CODE_LIST_MAP_ITER;

typedef std::list<IVS_DOMAIN_ROUTE*> DOMAIN_CODE_LIST;
typedef DOMAIN_CODE_LIST::iterator DOMAIN_CODE_LIST_ITER;

//������Map
typedef std::map<std::string, IVS_DOMAIN_ROUTE*>PROXY_DOMAIN_CODEL_MAP;
typedef PROXY_DOMAIN_CODEL_MAP::iterator PROXY_DOMAIN_CODEL_MAP_ITER;

typedef std::list<std::string> SUPER_DOMAIN_CODE_LIST;
typedef SUPER_DOMAIN_CODE_LIST::iterator SUPER_DOMAIN_CODE_LIST_ITER;

typedef std::vector<std::string> DOMAIN_CODE_VEC;

class CUserMgr;  //lint !e763 !e830
class CDomainRouteMgr
{
public:
	CDomainRouteMgr(void);
	~CDomainRouteMgr(void);

	/******************************************************************
	 function   : SetUserMgr
	 description: �����û�ָ��
	 input      : CUserMgr * pUserMgr
	 output     : NA
	 return     : void
	*******************************************************************/
	void SetUserMgr(CUserMgr *pUserMgr);
public:
	/******************************************************************
    function : GetDomainRoute
    description: ��ѯ��·��
    input : IVS_UINT32 uiBufferSize
    output : IVS_DOMAIN_ROUTE_LIST * pDomainRouteList
    return : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetDomainRoute(IVS_DOMAIN_ROUTE_LIST* pDomainRouteList, IVS_UINT32 uiBufferSize );

	/******************************************************************
    function : ModifyDomainRoute
    description: �޸���·�ɱ�
    input : IVS_DOMAIN_ROUTE pDomainRoute
    output : NA
    return : IVS_INT32
    *******************************************************************/
	IVS_INT32 ModifyDomainRoute(IVS_DOMAIN_ROUTE* pDomainRoute);

	/******************************************************************
    function : GetDomainRoute
    description: ˢ��Map��Ϣ������GetDomainRoute���������践��ֵ
    input : NA
    output : NA
    return : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetDomainRoute();
	
	/******************************************************************
    function : GetExDomainList
    description: ��ѯ�ⲿ���б�
	input  : pQueryInfo ͨ�ò�ѯ
    output : pRspXml ��ѯ���
    return : IVS_INT32
    *******************************************************************/
	IVS_INT32 GetExDomainList(const IVS_CHAR* pQueryInfo, IVS_CHAR** pRspXml);

	/******************************************************************
    function : SetShareCatalog
    description:  ���ù���Ŀ¼
    input : pShareInfo ��ϢXML
    output : NA
    return : IVS_INT32
    *******************************************************************/
	IVS_INT32 SetShareCatalog(const IVS_CHAR* pShareInfo);

	/******************************************************************
    function : GetShareCatalogList
    description: ��ѯĿ¼������Ϣ�б�
    input : pQueryInfo ��ѯ��ϢXML
    output : pRspXml ��ѯ���
    return : IVS_INT32
    *******************************************************************/
	IVS_INT32 GetShareCatalogList(const IVS_CHAR* pQueryInfo,IVS_CHAR** pRspXml);


    /******************************************************************
    function : GetLocalDomainCode
    description: ��ȡ��¼SMU����CODE
    input : NA
    output : strLocalDomainCode ��¼��SMU����CODE
    return : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetLocalDomainCode(std::string& strLocalDomainCode);

    /******************************************************************
    function : GetDomainRouteStruct
    description: ����map���ṹ��
    input : IVS_UINT32 uiBufferSize
    output : IVS_DOMAIN_ROUTE_LIST * pDomainRouteList
    return : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetDomainRouteStruct(IVS_DOMAIN_ROUTE_LIST* pDomainRouteList, IVS_UINT32 uiBufferSize );

    /******************************************************************
     function   : ClearDomainRouteMap
     description: ɾ����·��
     output     : NA
     return     : IVS_VOID
    *******************************************************************/
    IVS_VOID ClearDomainRouteMap();

	/******************************************************************
	 function   : ClearProxyDomainRouteMap
	 description: ɾ��������·��
	 output     : NA
	 return     : IVS_VOID
	*******************************************************************/
	IVS_VOID ClearProxyDomainRouteMap();

	/******************************************************************
	 function   : GeDomainRouteParseXML
	 description:  ������ȡ��·����Ӧ
	 input      : IVS_DOMAIN_ROUTE_LIST * pDomainRouteList
	 input      : CXml & xmlRsp
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GeDomainRouteParseXML( IVS_DOMAIN_ROUTE_LIST* pDomainRouteList, CXml &xmlRsp);

    /******************************************************************
     function   : FindProxyDomainCode
     description: ����Ŀ���������������·�ɱ��ҳ���Ӧ�Ĵ�����;
     input      : const std::string & strDestDomainCode Ŀ����������;
     output     : std::string & strProxyDomainCode ��Ӧ�Ĵ�������룬��������ڣ���Ϊ��;
     return     : bool true ���ڴ����� false�������ڴ�����;
    *******************************************************************/
    bool FindProxyDomainCode(const std::string& strDestDomainCode, std::string& strProxyDomainCode);

    /******************************************************************
     function   : GetDomainRoutebyCode
     description: ����������ȡ��·����Ϣ;
     input      : const std::string & strDomainCode     �����;
     output     : IVS_DOMAIN_ROUTE & stDomainRoute      ��·����Ϣ;
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetDomainRoutebyCode(const std::string& strDomainCode, IVS_DOMAIN_ROUTE& stDomainRoute);

	/******************************************************************
     function   : GetDomainRouteByIP
     description: ����IP��ȡ��·����Ϣ;
     input      : const std::string& strIP				IP;
     output     : IVS_DOMAIN_ROUTE & stDomainRoute      ��·����Ϣ;
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetDomainRouteByIP(const std::string& strIP, IVS_DOMAIN_ROUTE& stDomainRoute);

	/******************************************************************
     function   : IsInDomainTree
     description: �ж����Ƿ��ڵ�½���¼���
     input      : strDomainCode 
     output     : 
     return     : bool true�ɹ� falseʧ��
    *******************************************************************/
	bool IsInDomainTree(const std::string& strDomainCode);
	
	/******************************************************************
     function   : GetSameSuperDomainCode
     description: ��ȡ����ĸ���ڵ�
     input      : strDomainCode1 ��Code1
     input      : strDomainCode2 ��Code2
     output     : strSuperDomainCode ������Ϣ
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 GetSameSuperDomainCode(const std::string& strDomainCode1,const std::string& strDomainCode2,
		std::string& strSuperDomainCode);

	/******************************************************************
     function   : GetAllDomainCode
     description: ��ȡ���е�ǰ������Code
     output     : stDomainCodeVec ���е���code
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 GetAllDomainCode(DOMAIN_CODE_VEC& stDomainCodeVec );
private:
    /******************************************************************
     function   : ModifyProxyDomainRoute
     description: ����Ŀ����ʹ������ӳ���;
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
	IVS_INT32 ModifyProxyDomainRoute(IVS_DOMAIN_ROUTE* pDomainRoute);

	/******************************************************************
     function   : SendCmdAndGetInfo
	 description: ����ָ���ȡ������Ϣ
	 output     : NA
     return     : IVS_INT32
    *******************************************************************/
	IVS_INT32 SendCmdAndGetInfo(CXml& xmlRsp) const;

	/******************************************************************
	 function   : GetDomainRouteToListParseXML
	 description: ������·�ɵ�List
	 input      : DOMAIN_CODE_LIST & codeInfoList
	 input      : CXml & xmlRsp
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetDomainRouteToListParseXML(DOMAIN_CODE_LIST& codeInfoList, CXml &xmlRsp);

	/******************************************************************
	 function   : GetSuperDomainCodeList
	 description: ��ȡstrDomainCode��������ϼ�����Ϣ
	 input      : strDomainCode ��Code
	 output     : superDomainCodeList ����List
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetSuperDomainCodeList(SUPER_DOMAIN_CODE_LIST& superDomainCodeList, const std::string& strDomainCode);

	/******************************************************************
	 function   : FindSameSuperDomainCode(�����߱�֤��list1��size���ڵ���list2)
	 description: ��ѯ��һ����ͬ�������Code
	 input      : superDomainCodeList1 ����List1
	 input      : superDomainCodeList2 ����List2
	 output     : strSuperDomainCode ��ͬ����
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 FindSameSuperDomainCode(SUPER_DOMAIN_CODE_LIST& superDomainCodeList1,SUPER_DOMAIN_CODE_LIST& superDomainCodeList2,
										std::string& strSuperDomainCode);

	//����������XML�Ļ���
	IVS_INT32 SplitDevDomainCode(std::string& strBefore, std::string& strAfter);
	//�ϲ��豸�����Լ������
	IVS_INT32 ComplicationDevDomainCode(std::string& strBefore, std::string& strAfter);


private:
	CUserMgr *m_pUserMgr;                               // �û�����ָ��
	DOMAIN_CODE_LIST_MAP        m_domainCodeMap;        // �������·����Ϣ
	PROXY_DOMAIN_CODEL_MAP      m_proxyDomainCodeMap;   // ����������·����Ϣ
};

#endif // __DOMAIN_ROUTE_MGR__
