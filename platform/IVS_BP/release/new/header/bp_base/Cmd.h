/********************************************************************
filename    :    Cmd.h
author      :    s00191067
created     :    2012/03/27
description :    �����װ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_CMD_H
#define BP_CMD_H

#include <string>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"

NAMESPACE_BP_BEGIN // namespace begin

#pragma  pack(push)
#pragma  pack(1)
typedef struct stNetElementInfo
{
	stNetElementInfo(void)
	{
		memset(PeerIP, 0, sizeof(PeerIP));
		memset(LocalIP, 0, sizeof(LocalIP));
		PeerPort  = 0;
		LocalPort = 0;
		NeedSwitchId = true;
        uiTimeout = 0;
        bAsyncCreateLink = true;
	}

	char   PeerIP[IP_LENGTH];			// Զ�˷�����IP 
	char   LocalIP[IP_LENGTH];			// ����IP
	unsigned short PeerPort;    // Զ�˷�����PORT
	unsigned short LocalPort;   // ����PORT
	bool NeedSwitchId;			// �Ƿ��öԶ�ID���±��������е�����ID
    uint32_t uiTimeout;         // �������ӳ�ʱʱ�䣬��λΪ��
    bool   bAsyncCreateLink;        // �Ƿ��첽��ʽ�������ӣ�Ĭ��Ϊtrue����Ϊfalse�����ʾ����ͬ����ʽ��������
} NET_ELEMENT_CREATE_INFO;

#pragma pack(pop)

class CCmd
{
public:
    CCmd(void)
		: m_bCompress(false)
        , m_uiReqID(0)
        , m_threadID(0)
        , m_uiServiceID(0)
        , m_enCmdType(CMD_UNDEF)
        , m_enNetElemType(NET_ELE_SCU_NONE)
        , m_strNetLinkID("")
        , m_strTransID("")
        , m_pMsgBlock(NULL)
    {
    }

    ~CCmd(void)
    {
        // ��������Ϣ������ TODO��ȷ���Ƿ�����ʵ��
        ReleaseMsgBlock();
    }

	void Compress(bool bCompress)
	{
		this->m_bCompress = bCompress;
	}

	bool Compress()
	{
		return this->m_bCompress;
	}

    uint32_t GetReqID() const{ return m_uiReqID; }

    void SetReqID( uint32_t uiReqID ){ m_uiReqID = uiReqID; }

    uint32_t GetServiceID() const{ return m_uiServiceID; }

    void SetServiceID( uint32_t uiServiceID ){ m_uiServiceID = uiServiceID; }

    ACE_thread_t GetThreadID() const{ return m_threadID; }

    void SetThreadID( const ACE_thread_t& threadID ){ m_threadID = threadID; }

    CMD_TYPE GetCmdType() const{ return m_enCmdType; }

    void SetCmdType( CMD_TYPE enCmdType ){ m_enCmdType = enCmdType; }
    
    int GetNetElemType() const{ return m_enNetElemType; }

    void SetNetElemType( int enNetElemType ){ m_enNetElemType = enNetElemType; }

    const std::string& GetNetLinkID() const{ return m_strNetLinkID; }

    void SetNetLinkID(const std::string& strNetLinkID ){ m_strNetLinkID = strNetLinkID; }

    const std::string& GetTransID() const { return m_strTransID; }

    void SetTransID( const std::string& strTransID ) { m_strTransID = strTransID; }

    ACE_Message_Block* GetMsgBlock() { return m_pMsgBlock; }

    void SetMsgBlock(ACE_Message_Block* pMsgBlock) { m_pMsgBlock = pMsgBlock; }

	//���¿�¡һ�ݳ���
	CCmd* NewClone()
	{
		CCmd* pCmdClone = NULL;
		HW_NEW(pCmdClone, CCmd);
		if(NULL == pCmdClone)
		{
			return NULL;
		}

        pCmdClone->m_uiReqID = m_uiReqID;
        pCmdClone->m_threadID = m_threadID;
        pCmdClone->m_uiServiceID = m_uiServiceID;
        pCmdClone->m_enCmdType = m_enCmdType;
        pCmdClone->m_enNetElemType = m_enNetElemType;
        pCmdClone->m_strNetLinkID = m_strNetLinkID;
        pCmdClone->m_strTransID = m_strTransID;

        if (NULL != m_pMsgBlock)
        {
			ACE_Message_Block* pMsgBlock = NULL;
			HW_NEW(pMsgBlock, ACE_Message_Block(m_pMsgBlock->total_size()));
			if(NULL == pMsgBlock)
			{
				HW_DELETE(pCmdClone);
				return NULL;
			}
			pMsgBlock->copy(m_pMsgBlock->base(), m_pMsgBlock->total_size());
			pCmdClone->m_pMsgBlock = pMsgBlock;
		}

		return pCmdClone;
	}

    void CopyMsgBlockData(const char* pszMsgBlockData,unsigned int uiMsgLength)
    {
        CHECK_POINTER_VOID(pszMsgBlockData);

        // �����ж�ԭ���Ƿ�������
        ReleaseMsgBlock();
        HW_NEW(m_pMsgBlock, ACE_Message_Block(uiMsgLength));
        CHECK_POINTER_VOID(m_pMsgBlock);
        m_pMsgBlock->copy(pszMsgBlockData, uiMsgLength);
    }

	bool SaveData(const char* pData, unsigned int uiDataLen)
	{
		bool bRet = false;

		do 
		{
			if ((NULL == m_pMsgBlock) || (NULL == pData) || (0 == uiDataLen) || (m_pMsgBlock->capacity() < uiDataLen))
			{
				break;
			}

			m_pMsgBlock->copy(pData, uiDataLen);
			bRet = true;

		} while ( false );

		return bRet;
	}

    static CCmd* NewInstance()
	{
		CCmd* cmd = NULL;
		HW_NEW(cmd, CCmd);
		return cmd;
	}

    /**************************************************************************
    * name       : PopMsgBlock
    * description: ����MsgBlock
    * input      : NA
    * output     : NA
    * return     : ����MsgBlockָ�룬�����ɵ��÷�������ڴ�
    * remark     : NA
    **************************************************************************/
    ACE_Message_Block* PopMsgBlock()
    {
        ACE_Message_Block* pMsgBlock = m_pMsgBlock;
        m_pMsgBlock = NULL;
        return pMsgBlock;
    }

private:
    CCmd(const CCmd& cmd)
        : m_uiReqID(cmd.GetReqID())
        , m_threadID(cmd.GetThreadID())
        , m_uiServiceID(cmd.GetServiceID())
        , m_enCmdType(cmd.GetCmdType())
        , m_enNetElemType(cmd.GetNetElemType())
        , m_strNetLinkID(cmd.GetNetLinkID())
        , m_strTransID(cmd.GetTransID())
    {
        CCmd* pCmd = const_cast<CCmd*>(&cmd);
        if (NULL != pCmd)
        {
            CopyMsgBlockData(pCmd->GetMsgBlock()->rd_ptr(), pCmd->GetMsgBlock()->length());
        } 
    }

    CCmd& operator = (const CCmd& cmd)
    {
        this->m_uiReqID = cmd.GetReqID();
        this->m_threadID = cmd.GetThreadID();
        this->m_uiServiceID = cmd.GetServiceID();
        this->m_enCmdType = cmd.GetCmdType();
        this->m_enNetElemType = cmd.GetNetElemType();
        this->m_strNetLinkID = cmd.GetNetLinkID();
        this->m_strTransID = cmd.GetTransID();

        if (NULL != m_pMsgBlock)
        {
            CCmd* pCmd = const_cast<CCmd*>(&cmd);
            if (NULL != pCmd)
            {
                CopyMsgBlockData(pCmd->GetMsgBlock()->rd_ptr(), pCmd->GetMsgBlock()->length());
            }      
        }
        return *this;
    }
    /**************************************************************************
    * name       : ReleaseMsgBlock
    * description: �ͷ����ݿ�
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/
    void ReleaseMsgBlock()
    {
        if (NULL != this->m_pMsgBlock)
        {
            if (m_pMsgBlock->reference_count() > 1)
            {
                m_pMsgBlock->release();
                //return;
            }

            // û������ʹ�ø���Ϣ����
            m_pMsgBlock->reset();
            HW_DELETE(m_pMsgBlock);
        }
    }

private:
	bool m_bCompress;   //true: ѹ����Ϣ, false: ��ѹ����Ϣ
    uint32_t                m_uiReqID;          // ��������
	ACE_thread_t            m_threadID;         // �����������ڵ��߳�ID
    uint32_t                m_uiServiceID;      // ��������ID���߳�·�ɱ��
    CMD_TYPE                m_enCmdType;        // ��������(��Ԫͨ�ű���)
    int           m_enNetElemType;    // ���ⷢ��ʱָ����Ԫ����(��Ԫ��������)
    std::string             m_strNetLinkID;     // ���ⷢ��ʱָ����Ԫ����ID(��Ԫͨ�ű���)
    std::string             m_strTransID;       // ����ID
    ACE_Message_Block*      m_pMsgBlock;        // ��Ϣ������  
};

NAMESPACE_BP_END   // namespace end
#endif //BP_CMD_H
