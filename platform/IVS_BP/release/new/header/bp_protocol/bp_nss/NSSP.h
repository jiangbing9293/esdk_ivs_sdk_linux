/********************************************************************
filename    :    NSSP.h
author      :    w90004666
created     :    2012/9/19
description :	 NSSЭ��ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/19 ��ʼ�汾
*********************************************************************/
#ifndef NSSP_H
#define NSSP_H

#include <map>
#include <time.h>
#include "ace_header.h"
#include "IProtocol.h"
#include "bp_namespace.h"
#include "bp_stdint.h"
#include "bp_nss_def.h"

NAMESPACE_BP_BEGIN // namespace begin




class CNSSP : public IProtocol
{
public:
	// ���캯��
	CNSSP();
	// ��������
	virtual ~CNSSP();
private:
    CNSSP(const CNSSP &src);

    CNSSP& operator=(const CNSSP&);

public:
	
	/**************************************************************************
    * name       : NotifyOpen
    * description: ��Ԫ�򿪺�Open�������Գ�ʼ��һЩ��Ҫ����
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
	virtual void NotifyOpen(void);

	/**************************************************************************
    * name       : NotifyClose
    * description: ��Ԫ���ӶϿ�����һЩ��Ҫ��������
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
	virtual void NotifyClose(void);

    /**************************************************************************
    * name       : NotifyTimeout
    * description: ֪ͨЭ��ջ���ӳ�ʱ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void NotifyTimeout(void);

	/**************************************************************************
    * name       : NotifyRecv
    * description: ����Ԫ��ȡ���ݽ��д���
    * input      : void
    * output     : NA
    * return     : IVS_SUCCESS �ɹ� IVS_FAIL ʧ��
    * remark     : NA
    **************************************************************************/
	virtual int NotifyRecv(void);

	virtual void HandleCmd(CCmd* pCmd);
		
	// �г̴���
	virtual void Routing(void);

    inline void SetHeartBeat(unsigned int uiHeartBeat) { m_uiHeartBeat = uiHeartBeat; }

    inline void SetTimeout(unsigned int uiTimeout) { m_uiTimeout = uiTimeout; }

	virtual void LinkStatusReport(enum enLinkState linkState, const char* pszTransID);
private:


    typedef std::map<string,string> HTTPHeaderMap;
    typedef HTTPHeaderMap::iterator HTTPHeaderMapIter;

    /**************************************************************************
    * name       : ResetProtocol
    * description: ����Э��ջ״̬
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
    void ResetProtocol();

    /**************************************************************************
    * name       : Login
    * description: NSSP��¼����
    * input      : 
    * output     : NA
    * return     : int
    * remark     : NA
    **************************************************************************/
	int Login();

    /**************************************************************************
    * name       : ProcessLogin
    * description: ��¼Э�����
    * input      : 
    * output     : NA
    * return     : �Ƿ��ܽ�����¼Э��
    * remark     : NA
    **************************************************************************/

    int ProcessLoginReq(const char* pszMsg, uint32_t uiMsgLength);
    int ProcessLoginRsp(const char* pszMsg, uint32_t uiMsgLength);
    int ProcessHeartbeatReq(const char* pszMsg, uint32_t uiMsgLength);
	int ProcessHeartbeatRsp(const char* pszMsg, uint32_t uiMsgLength);	


    /**************************************************************************
    * name       : SendHeartbeat
    * description: ��������
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/    
	int SendHeartbeat(); 

	/**************************************************************************
    * name       : SendSystemBusyMsg
    * description: ���͸�ϵͳæ��Ϣ
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/  
	void SendSystemBusyMsg(TNssMsgHeader* nssMgsHeader);

    /**************************************************************************
    * name       : SendRequest
    * description: ����������Ϣ
    * input      : pszSendData Ҫ���͵����ݻ�����ָ��
                   uiDataLength ��Ϣ����
                   bCompress    �Ƿ�Ҫѹ����Ϣ
    * output     : NA
    * return     : ���ͳɹ�ʧ��
    * remark     : NA
    **************************************************************************/    
    int SendRequest(const char* pszSendData,unsigned int uiDataLength, bool bNeedCompress, bool bIsHeartBeatMsg = false);

    /**************************************************************************
    * name       : ProcessData
    * description: �������ݰ�
    * input      : 
    * output     : 
    * return     : ���سɹ�ʧ��
    * remark     : NA
    **************************************************************************/
	int ProcessData();

	bool SendRegistReqMsg(void);

	bool SendRegistRespMsg(const TNssMsgHeader* pReqMsg, uint32_t uRespCode);

	bool UpdateLinkId(const std::string& strLinkId);

    // ֪ͨ�ϲ㽨������(ͬ����ʽ��������)
    bool NotifyCreateLinkSync(bool bSuccess);

	void Close();

private:
    ACE_Message_Block * CreateMsgBlock(unsigned int uiLength);
    void DestroyMsgBlock(ACE_Message_Block *& pMsgBlock);
    ACE_Message_Block * MakeRespMsgBlock(const char * pszRecvData, unsigned int uiRecvLength);

	int BufPosTurnToBegin();  // �����λ
	int ProcessRecvData();
	bool UnZipMsgBlock();       // ������Ϣѹ��

private:
	std::string       m_strDevName;	  // �豸����

    std::string       m_strDevID;	  // �豸id

    bool              m_bConnnected;       // ���ӳɹ���־
    NSSP_STAT_FIAG    m_LoginFlag;         // ��¼״̬��־
	
	char *m_pBuffer;				//�������ݵ��ڴ�ָ��	
	unsigned int m_bufferSize;		//�������ݻ������Ĵ�С	
	unsigned int m_curDataLen;		//��ǰ�������ݵĴ�С(һ����Ϣ�ѽ��յ��ֽ���)
	unsigned int m_curPos;          //���ڽ�����Ϣ��ͷ��λ��

	unsigned int m_uiHeartBeat;	      // ���������ļ��
	unsigned int m_uiTimeout;		  // ��ʱʱ��

    time_t       m_HeartbeatTimestamp;// ��������ʱ���  ���������Զ�
    time_t       m_RecvHeartbeatTimestamp;// ��������ʱ���ʱ���  �Զ˷�������
	
	ACE_Message_Block * m_pMsgBlock;   // �յ���Ϣ����������
	unsigned int m_uiWaitForRecvLen;     // ���浱ǰ��Ϣ�ܳ���
};

NAMESPACE_BP_END   // namespace end

#endif // NSSP_H

