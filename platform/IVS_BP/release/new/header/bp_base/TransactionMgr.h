/********************************************************************
filename    :    TransactionMgr.h
author      :    s00191067
created     :    2012/03/27
description :    ���������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef TRANSACTION_MGR_H
#define TRANSACTION_MGR_H
#include <map>
#include <string>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"
#include "Cmd.h"

NAMESPACE_BP_BEGIN // namespace begin

const int TransactionTimeoutDef = 30;

typedef struct TrasactionStaticAttr
{
	std::string     szTransID;
	ACE_thread_t    iThreadID;
	std::string     szNetLinkID;
	uint32_t        uiServiceID;
	bool            bIsSync;
	TrasactionStaticAttr()
	{
		szTransID = "";
		iThreadID = 0;
		szNetLinkID = "";
		uiServiceID = 0;
		bIsSync = false;
	}
} TRANSACTION_STATIC_ATTR;

class EXPORT_DLL ITransaction
{
public:
    ITransaction(void);
    virtual ~ITransaction(void);
    // ��ȡTransID
    const std::string& GetTransID();

    // ����TransID
    void SetTransID(const std::string& strTransID);

    // ��ȡThreadID
    ACE_thread_t GetThreadId();

    // ����ThreadID
    void SetThreadID(const ACE_thread_t& threadID);

	// ��ȡLinkID
	const std::string& GetNetLinkID();

	// ����NetLinkID
	void SetNetLinkID(const std::string& strLinkID);

    // ��ȡServiceID
    uint32_t GetServiceID();

	// ��ȡServiceID
	void SetServiceID(uint32_t iServiceID);

	void SetTimeout(uint32_t uiTimeout);
	void UpdateTime();
	bool IsTimeout(time_t tCurTime);

    // ����Ϊͬ������
    void setSync(bool bIsSync);

	// �ж��Ƿ�Ϊͬ������
	bool isSync();

	//��ʼ��ͬ������
	void initSync(void);
    
	//������Ӧ��Ϣ
	void setCmd(CCmd* pCmd);

	//��ȡ��Ӧ��Ϣ
	CCmd* getCmd();

	ACE_Auto_Event* getSyncLock();

protected:
    std::string     m_strTransID;
    ACE_thread_t    m_threadID;
	std::string     m_netLinkID;
    uint32_t        m_uiServiceID;
	uint32_t        m_uiTimeout;
	time_t          m_tUpdateTime;
	bool            m_isSync;  // �Ƿ�ͬ������
	ACE_Auto_Event* m_pWait;  //ͬ��������
	CCmd* m_pCmd;              //ͬ����Ӧ��Ϣ
};


class EXPORT_DLL CBPTransaction : public ITransaction
{
public:
    CBPTransaction(void);
    virtual ~CBPTransaction(void);
    static CBPTransaction* NewInstance();
};

class EXPORT_DLL CBPSyncTransaction : public ITransaction
{
public:
	CBPSyncTransaction(void);
	virtual ~CBPSyncTransaction(void);
	static CBPSyncTransaction* NewInstance();
};


const time_t TRANSACTION_TIMEOUT_CHECK_INTERVAL = 10;

class EXPORT_DLL CTransactionMgr
{
public:
    ~CTransactionMgr(void);

    static CTransactionMgr& instance();

    void close();

    /**************************************************************************
    * name       : AddTransaction
    * description: ��������
    * input      : strTransationID ����ID pTransation �������
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int AddTransaction(const std::string& strTransationID, ITransaction* pTransation);

    /**************************************************************************
    * name       : GetThreadID
    * description: ��������ID��ȡ�߳�ID
    * input      : strTransationID ����ID
    * output     : NA
    * return     : ACE_thread_t �߳�ID
    * remark     : NA
    **************************************************************************/
    ACE_thread_t GetThreadID(const std::string& strTransationID);


	/**************************************************************************
    * name       : GetThreadID
    * description: ��������ID��ȡ�߳�ID,���ٱ���map�Ĵ���
    * input      : strTransationID ����ID, ���ص��Ƿ���ͬ������
    * output     : NA
    * return     : ACE_thread_t �߳�ID
    * remark     : NA
    **************************************************************************/
	void GetTransActionAttr(const std::string& strTransationID, TRANSACTION_STATIC_ATTR& stTransactionAttr);


    /**************************************************************************
    * name       : GetServiceID
    * description: ��������ID��ȡServiceID
    * input      : strTransationID ����ID
    * output     : NA
    * return     : ����ID
    * remark     : NA
    **************************************************************************/
    uint32_t GetServiceID(const std::string& strTransationID);

    /**************************************************************************
    * name       : RemoveTransaction
    * description: ɾ������
    * input      : strTransationID ����ID
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void RemoveTransaction(const std::string& strTransationID);

    /**************************************************************************
    * name       : GetLinkID
    * description: �������ID
    * input      : strTransationID ����ID
    * output     : NA
    * return     : ����ID
    * remark     : NA
    **************************************************************************/
	const std::string& GetLinkID(const std::string& strTransationID);

    /**************************************************************************
    * name       : Begin
    * description: ��ʼ������
    * input      : pszTransID: ����id
	*              reqID     : ����id
	*              netType   : ��Ԫ����
	*              strLinkId  : ���Ӻ�
	*              pPkgData  : ����
	*              iDataLen  : ���ݳ���
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
	int Begin(const char* pszTransID, uint32_t reqID, int netType, const std::string& strLinkId, const char* pPkgData, unsigned int iDataLen);

    /**************************************************************************
    * name       : End
    * description: �ر�������
    * input      : pszTransID: ����id
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
	int End(const char* pszTransID);
	
	/**************************************************************************
    * name       : getTransactionSyncLock
    * description: ��ȡ����ͬ����
    * input      : pszTransID: ����id
    * output     : NA
    * return     : ͬ��������
    * remark     : NA
    **************************************************************************/
	ACE_Auto_Event* getTransactionSyncLock(const std::string& strTransationID);

	/**************************************************************************
    * name       : setSyncTransactionResp
    * description: ����ͬ���������Ӧ��Ϣ
    * input      : pszTransID: ����id, ��Ӧ��Ϣָ��
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
	int setSyncTransactionResp(const std::string& strTransationID, CCmd* pCmd);

   /**************************************************************************
    * name       : getSyncTransactionResp
    * description: ��ȡͬ���������Ӧ��Ϣ
    * input      : pszTransID: ����id, ��Ӧ��Ϣָ��
    * output     : NA
    * return     : CCmd��ָ��
    * remark     : NA
    **************************************************************************/
	CCmd* getSyncTransactionResp(const std::string& strTransationID);

    /**************************************************************************
    * name       : ifSyncTransaction
    * description: �ж��Ƿ���ͬ������
    * input      : pszTransID: ����id
    * output     : NA
    * return     : true - ��ͬ������false - ���첽����
    * remark     : NA
    **************************************************************************/
	bool ifSync(const std::string& strTransationID);

	/**************************************************************************
    * name       : GetTransactionCount
    * description: ��ȡtransaction�ĵ�ǰ����
    * input      : NA
    * output     : NA
    * return     : transaction�ĵ�ǰ����
    * remark     : NA
    **************************************************************************/
	uint32_t GetTransactionCount();
	
   /**************************************************************************
    * name       : CheckTimeout
    * description: �������Transaction�Ƿ�ʱ, ��ʱ�������
    * input      : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void CheckTimeout(time_t tCurrentTime);
	/**************************************************************************
    * name       : SetTranMapMaxSize
    * description: ����transaction map���������
    * input      : uiTranMapMaxSize transaction map���������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetTranMapMaxSize(uint32_t uiTranMapMaxSize){m_uiTranMapMaxSize = uiTranMapMaxSize;};

	/**************************************************************************
    * name       : GetTranMapMaxSize
    * description: ��ȡtransaction map���������
    * input      : NA
    * output     : NA
    * return     : transaction map���������
    * remark     : NA
    **************************************************************************/
	uint32_t GetTranMapMaxSize(){return m_uiTranMapMaxSize;};

private:
    CTransactionMgr(void);
	// ����TransID��ȡ����
	ITransaction* GetTransaction(const std::string& strTransationID);

private:
    typedef std::map<std::string, ITransaction*>     CTransactionMap;
    typedef CTransactionMap::iterator                TransactionMapIter;

    CTransactionMap m_mapTransaction;
    ACE_Thread_Mutex                    m_mutex;

	uint32_t m_uiTranMapMaxSize;
};

NAMESPACE_BP_END   // namespace end
#endif //TRANSACTION_MGR_H
