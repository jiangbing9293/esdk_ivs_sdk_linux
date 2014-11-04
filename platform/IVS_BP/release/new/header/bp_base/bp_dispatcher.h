/********************************************************************
filename    :     bp_dispatcher
author      :     s69859
created     :     2012/09/20
description :     �ַ���������
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_DISPATCHER_HREAD_H_
#define BP_DISPATCHER_HREAD_H_

#include "bp_namespace.h"
#include "Cmd.h"
#include "bp_def.h"
#include "Thread.h"
#include "TransactionMgr.h"
#include "bp_map.h"

NAMESPACE_BP_BEGIN // namespace begin

typedef std::map<string,CCmd *> CmdMap;
typedef CmdMap::iterator        CmdMapIterator;

class EXPORT_DLL CDispatcher
{
public:
	virtual ~CDispatcher(void);
	
	static CDispatcher& instance();

    /**
     * �첽��Ϣ����
	 * bWithTransation �Ƿ���Ҫ����Transaction��
	 * uiTimeout �� �����Ҫ����Transation����transaction�ĳ�ʱʱ��;
     * �ɹ�����0, ʧ�ܷ��ش�����.
     */
	int dispatcherMsgIn(CCmd* &pCmd, bool bWithTransation, uint32_t uiTimeout, bool bIsBetweenService = false);
	// �첽�ַ�ֻ������service����Ҫ����������Ϣ����ͬһ��service��IDΪuiServiceID�����ײ㽫����һ��Transaction������transid��serviceid�Ĺ�����ϵ;
	int dispatcherInWithTransaction(int iNetElemType, const char* pszNetLinkID, 
		uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = TransactionTimeoutDef);

	// ���轫������Ϣ��������ǰ��service
	int dispatcherInWithoutTransaction(int iNetElemType, const char* pszNetLinkID, 
		uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock);

	 // bWithTransation �Ƿ���Ҫ����Transaction;
	 // uiTimeout �� �����Ҫ����Transation����transaction�ĳ�ʱʱ��;	
	int dispatcherMsgOut(CCmd* &pCmd, bool bWithTransation, uint32_t uiTimeout);   //�첽��Ϣ����
	
	// �첽���ͣ���Ҫ����������Ϣ����ͬһ��service��IDΪuiServiceID�����ײ㽫����һ��Transaction������transid��serviceid�Ĺ�����ϵ;
	int dispatcherOutWithTransaction(uint32_t uiServiceID, int iNetElemType, const char* pszNetLinkID, 
		const char* pszTransID, uint32_t uiReqID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = TransactionTimeoutDef, bool bCompress = false);   //�첽��Ϣ����

	// �첽���ͣ����轫������Ϣ��������ǰ��service
	int dispatcherOutWithoutTransaction(int iNetElemType, const char* pszNetLinkID, const char* pszTransID, 
		uint32_t uiReqID, ACE_Message_Block* &pMsgBlock, bool bCompress = false);   //�첽��Ϣ����

	CCmd* dispatcherSyncMsgOut(CCmd*& pCmd, uint32_t uiTimeout = 5);  //ͬ����Ϣ���ͣ�����ֵΪͬ����Ϣ�ķ���
	CCmd* dispatcherSyncMsgOut(uint32_t uiServiceID, int iNetElemType, const char* pszNetLinkID, 
		const char* pszTransID, uint32_t uiReqID, ACE_Message_Block*& pMsgBlock, bool bCompress = false, uint32_t uiTimeout = 5);   //ͬ����Ϣ���ͣ�����ֵΪͬ����Ϣ�ķ���

	CCmd* dispatcherSyncMsgIn(CCmd*& pCmd, uint32_t uiTimeout = 5);  //ͬ�����ڷ�����Ϣ
	CCmd* dispatcherSyncMsgIn(uint32_t uiServiceID, uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = 5);

    /**
     * ͬһ������һ������������һ������������
     * �ɹ�����0, ʧ�ܷ��ش�����.
     */
	int dispatcherMsgBetweenService(uint32_t uiServiceID, uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock);

	int dispatcherSyncMsgOutBatch(CmdMap &cmdMap, uint32_t uiTimeout);
protected:
    /**
     * ����������������Ϣ.
     * �ɹ�����0, ʧ�ܷ��ش�����.
     */
    int NewTransaction(const std::string& strTransID, 
		const std::string& strNetLinkID, uint32_t uiServiceID, const ACE_thread_t& threadID, uint32_t uiTimeout);

    /**
     * ����ͬ��������Ϣ����Ӧ��Ϣ
     * �ɹ�����0, ʧ�ܷ��ش�����.
     */
    int HandleSyncRespond(CCmd& cmd);
private:
	CDispatcher(void);

    static CDispatcher oDispatcher;
};
NAMESPACE_BP_END   // namespace end

#endif /* BP_DISPATCHER_HREAD_H_ */

