/********************************************************************
	filename	: 	NSSOperator.h
	author		:	z00193167
	created		:	2012/11/05	
	description	:	NSS������
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/11/05 ��ʼ�汾
*********************************************************************/
#ifndef NSS_OPERATOR_H
#define NSS_OPERATOR_H
// #include <map>
#include "bp_log.h"
#include "SDKMainCtrl.h"
#include "bp_net_factory.h"
#include "bp_dispatcher.h"
#include "MKTransID.h"
#include "NSSP.h"
#include "Cmd.h"
#include "vos.h"
#include "ivs_xml.h"
#include "hwsdk.h"
#include "SDKDef.h"

// TLV�Ľṹ��
#pragma pack(push, 1)
typedef struct NssTLVMsg
{
	uint16_t usiTag;//lint !e601 !e10
	uint32_t uiLength;//lint !e601 !e10
	char* pszValue;
}TNssTLVMsg;
#pragma pack(pop)

class CNSSOperator
{
public: 
	static CNSSOperator& instance();

public:
	// �ֶ���������
	int CreateClientLink(std::string& strNetElementID, int iNetElementType, const char* pszPeerIP, int iPeerPort, const char* pszLocalIP);

    // �ֶ��������ӣ�ͬ����
    int CreateSyncClientLink(int iNetElementType, const char* pszPeerIP, int iPeerPort, std::string& strNetElementID)const;

	// ͬ�����ͷ�������,�ڴ���Ҫ���ⲿ�ͷŵ�
	CCmd* SendSyncCmd(CCmd* pCmd, uint32_t uiTimeout = 35, IVS_BOOL bTimeout = 0) const; 

	// ͬ�����������Map����Ӧcmd��Ҫ�ͷ��ڴ�
	IVS_INT32 SendSyncCmd(CmdMap& cmdMap, uint32_t uiTimeout = 35) const; 

	// ȡ��cmd�е�xml,�����ؽ����
	char* ParseCmd2XML(CCmd* pCmd, int& iRspCode) const;

	// ֻ��NSSͷ�Ĳ��������������һ�������
	int ParseCmd2NSS(CCmd* pCmd);

	// ��ȡCmd�е�����ָ��
	char* ParseCmd2Data(CCmd* pCmd, int& Len);

	// ƴװ���͵�SMU��CMD����
	CCmd* BuildSMUCmd(unsigned short iReqID,const char* pData, int length = 0);
	// ƴװ���͵�SCU��CMD����
	CCmd* BuildSCUCmd(unsigned short iReqID,const char* pData);

    // ƴװ���͵�SMU��CMD����
    CCmd* BuildSMUCmd(unsigned short iReqID,const char* pData, const string& sLinkId);
    // ƴװ���͵�SCU��CMD����
    CCmd* BuildSCUCmd(unsigned short iReqID,const char* pData, const string& sLinkId);
    // ƴװ���͵�OMU��CMD����
    CCmd* BuildOMUCmd(unsigned short iReqID,const char* pData, const string& sLinkId);

	// ���õ�ģ����linkid�����������ӽ������������Ϣ��������ӽ���
	void SetSMULinkID(std::string linkid){ m_strSMULinkID = linkid; }; //lint !e1746 ����Ҫ
	void SetSCULinkID(std::string linkid) { m_strSCULinkID = linkid; } //lint !e1746 ����Ҫ
	void SetOMULinkID(std::string linkid) { m_strOMULinkID = linkid; } //lint !e1746 ����Ҫ
    //��ȡ��ģ��linkId���ر�������Ҫ
    std::string GetSMULinkID() const{return m_strSMULinkID;}; //lint !e1036
    std::string GetSCULinkID() const { return m_strSCULinkID; }//lint !e1036
    std::string GetOMULinkID() const { return m_strOMULinkID; }//lint !e1036

	// ����һ��TLV�������׵�ַ��ת����TLV�ṹ�壬�����������ֽ���ת��
	void ParseData2TLV(const void* pData, TNssTLVMsg& tlvMsg)const;

	IVS_INT32 AddLoginIdAndSendCmd(unsigned short iReqID, const IVS_CHAR* pReqXml) const;
	IVS_INT32 AddLoginIdAndSendCmd(unsigned short iReqID, const IVS_CHAR* pReqXml, IVS_CHAR*& pRsqXml) const;

	// ����cmd����,�ڲ�ΪXML
	CCmd* BuildCmd(const std::string& strLinkID, int iNetElementType,unsigned short iReqID,const char* pData) const;
	
    // ����cmd����,�ڲ�ΪXML,ָ������ID
    CCmd* BuildCmd(const std::string& strLinkID, int iNetElementType,unsigned short iReqID,const char* pData, const std::string& strTransID) const;
    // ������ֱ��ƴ��CMD
	CCmd* BuildCmd(const std::string& strLinkID, int iNetElementType, unsigned short iReqID, const char* pData, int length) const;
	//����BP��������CMD��ֵ
	int ConfCmd(CCmd* pCmd,unsigned short iReqID,const char* pData, const std::string& strTransID)const;
private: 
	CNSSOperator(void){;}
	~CNSSOperator(void){;} 
	CNSSOperator(const CNSSOperator&); 
	CNSSOperator& operator = (const CNSSOperator&);          

private:
	std::string m_strSMULinkID;
	std::string m_strSCULinkID;
	std::string m_strOMULinkID;

};
#endif //NSS_OPERATOR_H
