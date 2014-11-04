/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspPlayMessage.cpp
  ��    �� : y00182336
  �������� : RTSP Play��Ϣ
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include <sstream>
#include "Rtsp_Log.h"
#include "RtspPlayMessage.h"
#include "RtspRetCode.h"
using namespace std;


/***************************************************************************
 �� �� �� : CRtspPlayMessage
 �������� : ���캯��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspPlayMessage::CRtspPlayMessage()
{
    m_unRtspMethod  = RTSP_METHOD_PLAY;
    m_dScale       = 0;
    memset (&m_stRange, 0x0, sizeof(MEDIA_RANGE_S));
}

/***************************************************************************
 �� �� �� : ~CRtspPlayMessage
 �������� : ��������
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspPlayMessage::~CRtspPlayMessage()
{
    
}


/***************************************************************************
 �� �� �� : encodeMessage
 �������� : ��װRTSP Play��Ϣ
 ������� : ��
 ������� : strMessage              ��װ����ַ���
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspPlayMessage::encodeMessage(std::string &strMessage)
{
    strMessage.clear();
    // ��װRTSP URL
    if (RET_OK != encodeUrl(strMessage))
    {
        IVS_RUN_LOG_CRI("encode rtsp setup message url fail.");
        return RET_FAIL;
    }
    // Scale: 0.0
    stringstream   strValue;
    strValue<<m_dScale;
    strMessage += RTSP_TOKEN_STR_SCALE;
    strMessage += strValue.str();
    strMessage += RTSP_END_TAG;
    // Range: npt=0.000-
    encodeRangeField(strMessage);
    // CSeq:
    encodeCSeq(strMessage);
    strMessage += RTSP_END_TAG;
    return RET_OK;
}

/***************************************************************************
 �� �� �� : setRang
 �������� : ���ò���ʱ���
 ������� : strRange                ����ʱ���ַ���
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspPlayMessage::setRang(const MEDIA_RANGE_S* stRange)
{
    if (NULL == stRange)
    {
        // ʵʱ���ģʽ��ȫ��Ϊ0
        m_stRange.enRangeType       = (MEDIA_RANGE_TYPE_E)0;
        m_stRange.MediaBeginOffset  = 0;
        m_stRange.MediaEndOffset    = 0;
    }
    else
    {
        m_stRange.enRangeType       = stRange->enRangeType;
        m_stRange.MediaBeginOffset  = stRange->MediaBeginOffset;
        m_stRange.MediaEndOffset    = stRange->MediaEndOffset;
    }
    return;
}


/***************************************************************************
 �� �� �� : setScale
 �������� : ���ò�������
 ������� : dScale                  ���������ַ���
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspPlayMessage::setScale(const double dScale)
{
    m_dScale = dScale;
    return;
}



/***************************************************************************
 �� �� �� : encodeRangeField
 �������� : ��װ����ʱ���
 ������� : ��
 ������� : strMessage                  ��װ����ַ���
 �� �� ֵ : ��
***************************************************************************/
void CRtspPlayMessage::encodeRangeField(std::string &strMessage)
{
    char strTime[32] = { 0 };
    if (RANGE_TYPE_UTC == m_stRange.enRangeType)
    {
        // clockʱ��
        if ((OFFSET_CUR == m_stRange.MediaBeginOffset)
            || (OFFSET_END == m_stRange.MediaBeginOffset))
        {
            // ��ǰ�㣬ֱ�ӷ��أ�û��range�ֶ�
            return;
        }
        else if(OFFSET_BEGIN == m_stRange.MediaBeginOffset)
        {
            // ��ʼ�㣬Ϊ0
            m_stRange.MediaBeginOffset = 0;
        }
        strMessage += RTSP_TOKEN_STR_RANGE;
        time_t rangeTime = (time_t) m_stRange.MediaBeginOffset;
        struct tm tmv;

        (void)ACE_OS::localtime_r(&rangeTime, &tmv);
        (void)ACE_OS::snprintf(strTime, 32, "%04d%02d%02dT%02d%02d%02dZ", tmv.tm_year
            + 1900, tmv.tm_mon + 1, tmv.tm_mday, tmv.tm_hour, tmv.tm_min,
            tmv.tm_sec);

        strMessage += RTSP_RANGE_CLOCK;
        strMessage += strTime;
        strMessage += SIGN_H_LINE;

        if (OFFSET_END != m_stRange.MediaEndOffset)
        {
            rangeTime = (time_t) m_stRange.MediaEndOffset;
            (void) ACE_OS::localtime_r(&rangeTime, &tmv);
            (void) ACE_OS::snprintf(strTime, 32, "%04d%02d%02dT%02d%02d%02dZ",
                tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
                tmv.tm_hour, tmv.tm_min, tmv.tm_sec);

            strMessage += strTime;
        }

        strMessage += RTSP_END_TAG;
        return;
    }

    if ((OFFSET_CUR == m_stRange.MediaBeginOffset)
        || (OFFSET_END == m_stRange.MediaBeginOffset))
    {
        // ��ֱ�ӷ��أ�û��range�ֶ�
        return;
    }
    else
    {
        strMessage += RTSP_TOKEN_STR_RANGE;
        // nptʱ��
        strMessage += RTSP_RANGE_NPT;
        (void) ACE_OS::snprintf(strTime, 32, "%u", m_stRange.MediaBeginOffset);
        strMessage += strTime;
    }
    strMessage += SIGN_H_LINE;

    if (OFFSET_END != m_stRange.MediaEndOffset)
    {
        (void) ACE_OS::snprintf(strTime, 32, "%u", m_stRange.MediaEndOffset);
        strMessage += strTime;
    }
    strMessage += RTSP_END_TAG;

    return;
}

