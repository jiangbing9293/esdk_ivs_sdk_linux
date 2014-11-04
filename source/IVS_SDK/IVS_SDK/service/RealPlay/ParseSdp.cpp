/*******************************************************************************
//  ��Ȩ����    2012 ��Ϊ�������޹�˾
//  �� �� ����  IVS_MLIB_Player
//  �� �� ����  IVS_Sdp.cpp
//  �ļ�˵����  SDP��������
//  ��      ���� eIVS V100R001C30
//  ��      �ߣ� yangjunhai/y00182336
//  �������ڣ�2012/08/14
//
//  �޸ı�ʶ��
//  �޸�˵����
*******************************************************************************/
#include "ParseSdp.h"
#include "bp_log.h"
#include "ivs_error.h"

/***************************************************************************
 �� �� �� : parseSdp
 �������� : ����SDP
 ������� : pSdpMsg                 ���������ַ���
 ������� : pMediaKeyInfo           ý����Կ��Ϣ
			pMediaInfo              ��ȡ����ý����������������SDK���·��
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
 SDP��ʽ���£�
 m=video 51372  RTP/AVP/TCP 31
 a=crypto:1 HW_IVS_IPC_ENCRYPT_SIMPLE StartTime:YYYYMMddHHmmss StopTime:YYYYMMddHHmms
 m=audio 49170  RTP/AVP/TCP 0
 a=crypto:1 HW_IVS_IPC_ENCRYPT_SIMPLE StartTime:YYYYMMddHHmmss StopTime:YYYYMMddHHmms
***************************************************************************/
IVS_INT32 parseSdp(const char* pSdp, MEDIA_KEY_INFO* pMediaKeyInfo,
							MEDIA_INFO_SDP* pMediaInfo,STORAGESECRET_INFO* pStorageSecretInfo)
{
	if (NULL == pMediaKeyInfo || NULL == pMediaInfo || NULL == pStorageSecretInfo)
	{
		return IVS_FAIL;
	}
    std::string strSdp(pSdp);
    std::string strBuff = "";
    unsigned int nNextStart = 0;
    bool bRet = getNextLine(strSdp, nNextStart, strBuff);
    int nRet = IVS_SUCCEED;
    IVS_ULONG ulKeyType = KEY_TYPE_VIDEO;
    char* pTmp  = NULL;
  //  bool bFirst = true;

    while (bRet && (IVS_SUCCEED == nRet))
    {
        pTmp = const_cast <char*> (strBuff.c_str());
        JUMP_SPACE(pTmp);

        //ȫ�����ǿո񣬽�����һ��
        if ('\0' == *pTmp)
        {
            bRet = getNextLine(strSdp, nNextStart, strBuff);
            continue;
        }

        //��¼��һ����Ϊ�յ��ַ�
        char cCode = *pTmp;
        pTmp++;
        JUMP_SPACE(pTmp);
        if ('=' == *pTmp)
        {
            pTmp++;
            JUMP_SPACE(pTmp);

            //if (('v' != tolower(cCode)) && bFirst)
            //{
            //    return IVS_FAIL;
            //}

            switch (tolower(cCode))
            {
            case 'o':
                nRet = parseStreamID(pTmp, pMediaInfo->SessionId);
                break;
            case 'u':
                strncpy(pMediaInfo->SessionUrl, pTmp, strlen(pTmp));
                break;
            case 'm':

                // ������ý����Ϣ
                nRet = parseMediaInfo(pTmp, &ulKeyType, &pMediaInfo->AudioDecoderType,
                                      &pMediaInfo->VideoDecoderType,
                                      &pMediaInfo->port);
                break;
            case 'a':
            {
                if (0 == strncmp(pTmp, SDP_ATTRIBUTES_CRYPTOSTORAGE, strlen(SDP_ATTRIBUTES_CRYPTOSTORAGE)))
                {
                    (void)parseStorageSecretKey(pTmp, pStorageSecretInfo);
                    break;
                }

                if (0 == strncmp(pTmp, SDP_ATTRIBUTES_CRYPTO, strlen(SDP_ATTRIBUTES_CRYPTO)))
                {
                    nRet  = parseManuType(pTmp, &pMediaInfo->ManuType);
                    nRet += parseTransSecretKey(pTmp, ulKeyType, pMediaKeyInfo);
                    break;
                }

                if (0 == strncmp(pTmp, SDP_CER_INDEX, strlen(SDP_CER_INDEX)))
                {
                    nRet = parseCerIndex(pTmp, pStorageSecretInfo);
                    break;
                }
				if (0 == strncmp(pTmp, SDP_RTP_MAP, strlen(SDP_RTP_MAP)))
				{
					//������Ƶ������
					nRet = parseRtpMap(pTmp, pMediaKeyInfo);
					break;
				}

                break;
            }
            case 'b':
            {
                break;
            }
            default:
                break;
            }

            if (IVS_SUCCEED != nRet)
            {
                return IVS_FAIL;
            }

          //  bFirst = false;
        }
        else
        {
            return IVS_FAIL;
        }

        // ������һ�еĲ���
        bRet = getNextLine(strSdp, nNextStart, strBuff);
    }

    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : getNextLine
 �������� : �з�SDP��Ϣ
 ������� : strSdpMsg                   ���������ַ���
            nNextStart                  ƫ��ֵ
 ������� : strBuff                     ��ȡ������һ��SDP�ַ���
 �� �� ֵ : true �ɹ�           fail ʧ��
***************************************************************************/
bool getNextLine(const std::string &strSdpMsg,
                 unsigned int &     nNextStart,
                 std::string  &     strBuff)
{
    unsigned int nLen = 0;
	unsigned int Sdplen = strSdpMsg.length();
	if (nNextStart >= Sdplen)
	{
		return false;
	}
    const char *pszStart = strSdpMsg.c_str() + nNextStart;
    while (('\0' != *pszStart) && ('\n' != *pszStart) && ('\r' != *pszStart))
    {
        pszStart++;
        nLen++;
    }
    strBuff = strSdpMsg.substr(nNextStart, nLen);
    nNextStart += nLen;
    nNextStart++;
    return true;
}

/***************************************************************************
 �� �� �� : parseStreamID
 �������� : ����SessionID
 ������� : pSdpMsg                     ���������ַ���
 ������� : pSessionId                  ��ȡ����SessionID
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseStreamID(const char* pSdpMsg, char* pSessionId)
{
    if ((NULL == pSdpMsg) || (NULL == pSessionId))
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Parse Stream ID", "parse streamID fail.");
        return IVS_FAIL;
    }

    std::string strTmp(pSdpMsg);

    //std::string::size_type Pos = ;
    std::string::size_type npos = strTmp.find(" ");
    if (std::string::npos == npos)
    {
        return IVS_FAIL;
    }

    strTmp = strTmp.substr(npos + 1);

    //  IVS_LOG(LOG_INFO, "strTmp=%s", strTmp.c_str());
    npos = strTmp.find(" ");
    if (std::string::npos == npos)
    {
        return IVS_FAIL;
    }

    std::string strValue = "";
    strValue = strTmp.substr(0, npos);

    //(LOG_INFO, "strValue=%s", strValue.c_str());
    strncpy(pSessionId, strValue.c_str(), strValue.length());

    //IVS_LOG(LOG_INFO, "streamID=%s", pSessionId);
    return IVS_SUCCEED;
} //lint !e818

/***************************************************************************
 �� �� �� : parseApplications
 �������� : ����ý�����ͣ���ȡTCP�˿ں�
 ������� : pAppliMsg                   ���������ַ���
 ������� : pPort                       ��ȡ����TCP�˿ں�
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseApplications(const char* pTmp, unsigned long* pPort)
{
    if ((NULL == pTmp) || (NULL == pPort))
    {
        return IVS_FAIL;
    }

    std::string strTmp(pTmp);
    std::string::size_type Pos = strTmp.find(" ");
    if (std::string::npos == Pos)
    {
        return IVS_FAIL;
    }

    strTmp = strTmp.substr(Pos + 1);
    Pos = strTmp.find("TCP");
    if (std::string::npos == Pos)
    {
        return IVS_FAIL;
    }

    std::string strValue = "";
    strValue = strTmp.substr(0, Pos);
    *pPort = (unsigned long)atoi(strValue.c_str());
    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : parseDecoderType
 �������� : ������������
 ������� : pTmpMsg                     ���������ַ���
 ������� : pDecoderType                ��ȡ���ı�������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseDecoderType(const char* pTmp, IVS_ULONG* pDecoderType)
{
    if ((NULL == pTmp) || (NULL == pDecoderType))
    {
        return IVS_FAIL;
    }

    std::string strTmp(pTmp);
    std::string::size_type pos = strTmp.rfind(" ");
    if (std::string::npos == pos)
    {
        return IVS_FAIL;
    }

    std::string strValue = strTmp.substr(pos + 1);
    *pDecoderType = (IVS_ULONG)atoi(strValue.c_str());
    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : parseManuType
 �������� : ������������
 ������� : pTmpMsg                     ���������ַ���
 ������� : pManuType                   ��ȡ���ĳ�������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseManuType(char* pTmp, IVS_ULONG* ulManuType)
{
    if ((NULL == pTmp) || (NULL == ulManuType))
    {
        return IVS_FAIL;
    }

    std::string strTmp(pTmp);
    std::string::size_type pos = strTmp.find(SDP_ATTRIBUTES_DECODER_TYPE);
    if (std::string::npos == pos)
    {
        return IVS_SUCCEED;
    }

    strTmp = strTmp.substr(pos);
    *ulManuType = (IVS_ULONG)atoi(strTmp.c_str());
    return IVS_SUCCEED;
} //lint !e818

/***************************************************************************
 �� �� �� : parseRtpMap
 �������� : ��������Ƶ����
 ������� : pTmp                     ���������ַ���
 ������� : pMediaKeyInfo            ��ȡ������Ƶ����Ƶ������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseRtpMap(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo)
{
	if (NULL == pTmp || NULL == pMediaKeyInfo)
	{
		return IVS_FAIL;
	}

	/*
	 * decode rtp payload value, add by w00210470
	 */
	unsigned int PtVal = 0;
	const char *pVal = strchr(pTmp, ':');
	if (NULL != pVal)
	{
		pVal++;
		PtVal = atoi(pVal);//lint !e732
	}

	std::string strTmp(pTmp);
	std::string::size_type pEndPos = strTmp.find(SDP_VIDEO_H264_TYPE);
	do 
	{
		//H264
		if (std::string::npos != pEndPos)
		{
			pMediaKeyInfo->enVideoDecoderType = VIDEO_DEC_H264;
			pMediaKeyInfo->uPTVideo = PtVal;
			break;
		}
		//Mjpeg
		pEndPos = strTmp.find(SDP_VIDEO_MJPEG_TYPE);
		if (std::string::npos != pEndPos)
		{
			pMediaKeyInfo->enVideoDecoderType = VIDEO_DEC_MJPEG ;
			pMediaKeyInfo->uPTVideo = PtVal;
			break;
		}
		//PCMU
		pEndPos = strTmp.find(SDP_AUDIO_PCMU_TYPE);
		if (std::string::npos != pEndPos)
		{
			pMediaKeyInfo->enAudioDecoderType = AUDIO_DEC_G711U;
			pMediaKeyInfo->uPTAudio = PtVal;
			break;
		}
		//PCMA
		pEndPos = strTmp.find(SDP_AUDIO_PCMA_TYPE);
		if (std::string::npos != pEndPos)
		{
			pMediaKeyInfo->enAudioDecoderType = AUDIO_DEC_G711A;
			pMediaKeyInfo->uPTAudio = PtVal;
			break;
		} 
	} while (false); //lint !e717
	return IVS_SUCCEED;
}


/***************************************************************************
 �� �� �� : parseAttributes
 �������� : ������������������Կ
 ������� : pTmpMsg                     ���������ַ���
            ulKeyType                   ý������
������� : pMediaKeyInfo           ý����Կ��Ϣ
�� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

			�ַ������ͣ�
 //a=crypto:1 HW_IVS_IPC_ENCRYPT_SIMPLE inline:d0RmdmcmVCspeEc3QGZiNWpVLFJhQX1cfHAwJSoj
***************************************************************************/

//a=crypto:1 HW_IVS_IPC_ENCRYPT_SIMPLE inline:d0RmdmcmVCspeEc3QGZiNWpVLFJhQX1cfHAwJSoj
int parseTransSecretKey(const char* pTmp, IVS_ULONG ulKeyType, MEDIA_KEY_INFO* pMediaKeyInfo)
{
    if (NULL == pTmp || NULL == pMediaKeyInfo)
    {
        return IVS_FAIL;
    }
    std::string strTmp(pTmp);
	//������--����ͷ
	std::string::size_type pEndPos = strTmp.find(IVS_SECRET_KEY_TYPE_SIMPLE_HEADER_PLAIN);
	int iSecretType = HW_NO_DEC_TYPE;
	do 
	{
		if (std::string::npos != pEndPos)
		{
			iSecretType = HW_IPC_ENCRYPT_SIMPLE_HEADER;
			break;
		}
		//������--����ͷ
		pEndPos = strTmp.find(IVS_SECRET_KEY_TYPE_SIMPLE);
		if (std::string::npos != pEndPos)
		{
			iSecretType = HW_IPC_ENCRYPT_SIMPLE ;
			break;
		} 

		//AES-256���ּ���
		pEndPos = strTmp.find(IVS_SECRET_KEY_TYPE_AES256_PARTIAL);
		if (std::string::npos != pEndPos)
		{
			iSecretType = HW_IPC_ENCRYPT_AES256_PARTIAL;
			break;
		}

		//AES256(����ͷ)
		pEndPos = strTmp.find(IVS_SECRET_KEY_TYPE_AES256);
		if (std::string::npos != pEndPos)
		{
			iSecretType = HW_IPC_ENCRYPT_AES256;
			break;
		} 
		return IVS_SUCCEED;
	} while (false); //lint !e717
	//û���ҵ�crypto,����ʧ��
    std::string::size_type pStartPos = strTmp.find(SDP_ATTRIBUTES_CRYPTO);
    if (std::string::npos == pStartPos)
    {
        return IVS_FAIL;
    }

    //IVS_ULONG* pKeyType = NULL;
    //char* pSecretKey = NULL;
    if (KEY_TYPE_AUDIO == ulKeyType)
    {
		pMediaKeyInfo->iAudioKeyType = iSecretType;
    }
    else if (KEY_TYPE_VIDEO == ulKeyType)
    {
		pMediaKeyInfo->iVideoKeyType = iSecretType;
    }
	//(void)parseStarTimeAndEndTime(pTmp,pMediaKeyInfo);
	(void)parseSessionID(pTmp,pMediaKeyInfo);
    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : parseStorageSecretKey
 �������� : �����洢������Կ
 ������� : pTmpMsg                     ���������ַ���
 ������� : pStorageSecretInfo          ��ȡ���洢������Ϣ
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

 �ַ������ͣ�
 a=cryptostorage:1 ASYM_RSA_2048_AES_CM_256 inline:����������Key������
 b=cerindex:֤��������
***************************************************************************/
int parseStorageSecretKey(const char* pTmp, STORAGESECRET_INFO* pStorageSecretInfo)
{
	CHECK_POINTER(pTmp, IVS_FAIL);
	CHECK_POINTER(pStorageSecretInfo, IVS_FAIL);

    std::string strTmp(pTmp);

    std::string::size_type pStartPos = strTmp.find(SDP_ATTRIBUTES_CRYPTOSTORAGE);

    if (std::string::npos == pStartPos)
    {
        return IVS_FAIL;
    }

    std::string::size_type pEndPos = strTmp.find(IVS_SECRET_STORAGE_KEY_TYPE);
    if (std::string::npos == pEndPos)
    {
        return IVS_SUCCEED;
    }

    std::string strValue = "";
    strValue = strTmp.substr((pStartPos + strlen(SDP_ATTRIBUTES_CRYPTOSTORAGE)), (pEndPos - 1));

    pStorageSecretInfo->keyTpye = (IVS_ULONG)atoi(strValue.c_str()); //lint !e613

    pStartPos = strTmp.find(SDP_ATTRIBUTES_INLINE);
    if (std::string::npos == pStartPos)
    {
        return IVS_FAIL;
    }

    strValue.clear();
    strValue = strTmp.substr(pStartPos + strlen(SDP_ATTRIBUTES_INLINE));
    strncpy(pStorageSecretInfo->keyValue, strValue.c_str(), strValue.length());

    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : parseCerIndex
 �������� : �����洢����֤���
 ������� : pTmpMsg                     ���������ַ���
 ������� : pStorageSecretInfo          ��ȡ���洢������Ϣ
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

 �ַ������ͣ�
 b=cerindex:֤��������
***************************************************************************/
int parseCerIndex(const char* pTmp, STORAGESECRET_INFO* pStorageSecretInfo)
{
    std::string strTmp(pTmp);

    std::string::size_type pStartPos = strTmp.find(SDP_CER_INDEX);

    if (std::string::npos == pStartPos)
    {
        return IVS_FAIL;
    }

    std::string strValue = "";
    strValue = strTmp.substr((pStartPos + strlen(SDP_CER_INDEX)), strTmp.length());

    pStorageSecretInfo->cerIndex = (IVS_ULONG)atoi(strValue.c_str()); //lint !e613

    return IVS_SUCCEED;
}

/***************************************************************************
 �� �� �� : parseMediaInfo
 �������� : ����ý����Ϣ
 ������� : pTmpMsg                     ���������ַ���
            ulKeyType                   ý������
 ������� : pAudioDecoderType           ��ȡ������Ƶ��������
            pVideoDecoderType           ��ȡ������Ƶ��������
            pPort                       ��ȡ���Ķ˿ں�
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseMediaInfo(char* pTmp, IVS_ULONG* pKeyType, IVS_ULONG* pAudioDecoderType,
                   IVS_ULONG* pVideoDecoderType, IVS_ULONG* pPort)
{
	CHECK_POINTER(pTmp, IVS_FAIL);
	CHECK_POINTER(pKeyType, IVS_FAIL);
	CHECK_POINTER(pAudioDecoderType, IVS_FAIL);
	CHECK_POINTER(pVideoDecoderType, IVS_FAIL);
	CHECK_POINTER(pPort, IVS_FAIL);

    int nRet = 0;

    if (0 == strncmp(pTmp, SDP_MEDIA_TYPE_AUDIO, strlen(SDP_MEDIA_TYPE_AUDIO)))
    {
        *pKeyType = KEY_TYPE_AUDIO;
        nRet = parseDecoderType(pTmp, pAudioDecoderType);
    }
    else if (0 == strncmp(pTmp, SDP_MEDIA_TYPE_VIDEO, strlen(SDP_MEDIA_TYPE_VIDEO)))
    {
        *pKeyType = KEY_TYPE_VIDEO;
        nRet = parseDecoderType(pTmp, pVideoDecoderType);
    }
    else if (0 == strncmp(pTmp, SDP_MEDIA_SYPE_APPLICATION, strlen(SDP_MEDIA_SYPE_APPLICATION)))
    {
        nRet = parseApplications(pTmp, pPort);
    }

    return nRet; //lint !e438 !e550 !e818
} //lint !e438 !e550 !e818

/***************************************************************************
 �� �� �� : parseStarTimeAndEndTime
 �������� : ����ý��ý�忪ʼ����ʱ��
 ������� : pTmp                    ���������ַ���
 ������� : pMediaKeyInfo           ý����Կ��Ϣ

 �� �� ֵ : IVS_SUCCEED-�ɹ�        ����-ʧ��
***************************************************************************/
int parseStarTimeAndEndTime(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo)
{
	if (NULL == pMediaKeyInfo)
	{
		return IVS_FAIL;
	}
	std::string strTmp(pTmp);
	std::string::size_type pStartPos = strTmp.find(SDP_MEDIA_STAR_TIME);
	if (std::string::npos == pStartPos)
	{
		return IVS_FAIL;
	}
	std::string strValue = "";
	strValue = strTmp.substr((pStartPos + strlen(SDP_MEDIA_STAR_TIME)), SDP_TIME_LEN);
	//��ȡstarTime
#ifdef WIN32
	memcpy_s(pMediaKeyInfo->szStarTime,SDP_TIME_LEN,strValue.c_str(), SDP_TIME_LEN);
#else
	memcpy(pMediaKeyInfo->szStarTime,strValue.c_str(),SDP_TIME_LEN);
#endif
	//׼����ȡendTime
	strValue.clear();
	pStartPos = strTmp.find(SDP_MEDIA_END_TIME);
	if (std::string::npos == pStartPos)
	{
		return IVS_FAIL;
	}
	strValue = strTmp.substr((pStartPos + strlen(SDP_MEDIA_END_TIME)), SDP_TIME_LEN);
#ifdef WIN32
	memcpy_s(pMediaKeyInfo->szEndTime,SDP_TIME_LEN,strValue.c_str(), SDP_TIME_LEN);
#else
	memcpy(pMediaKeyInfo->szEndTime,strValue.c_str(),SDP_TIME_LEN);
#endif
	return IVS_SUCCEED;
}

int parseSessionID(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo)
{
	if (NULL == pMediaKeyInfo)
	{
		return IVS_FAIL;
	}
	std::string strTmp(pTmp);
	std::string::size_type pStartPos = strTmp.find(SDP_SESSION_ID);
	if (std::string::npos == pStartPos)
	{
		return IVS_FAIL;
	}
	unsigned int uiStrSize = strTmp.size();
	unsigned int uiStartIdex = pStartPos + strlen(SDP_SESSION_ID);
	std::string strValue = "";
	strValue = strTmp.substr(uiStartIdex, uiStrSize - uiStartIdex);
	memset(&pMediaKeyInfo->szSessionID,0,SESSION_ID_LEN);//lint !e545
#ifdef WIN32
	memcpy_s(pMediaKeyInfo->szSessionID,SESSION_ID_LEN,strValue.c_str(), strValue.length());
#else
	memcpy(pMediaKeyInfo->szSessionID,strValue.c_str(),(SESSION_ID_LEN<strValue.length())?SESSION_ID_LEN:strValue.length());
#endif
	return IVS_SUCCEED;
}
