/********************************************************************
filename    :    Rtp.h
author      :    s00191067
created     :    2012/11/16
description :    rtp����;
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/16 ��ʼ�汾
*********************************************************************/
#ifndef RTP_H
#define RTP_H

#pragma pack(push, 1)

///////////////////////////////////////////////////////////////////////////////
/// @namespace RTP
/// @brief RTPר�������ռ�
///
/// RTP�ṹ��ͳ�������
///////////////////////////////////////////////////////////////////////////////
namespace RTP
{
    const unsigned char VERSION     = 2;    //RTP��ǰ�汾��
    const unsigned char PT_RTCP_RR  = 201;  //RTCP��RR��PT

    struct FIXED_HEADER
    {
        // byte 0
        unsigned char csrc_len: 4;  // expect 0
        unsigned char extension:1;  // expect 0
        unsigned char padding:  1;  // expect 0
        unsigned char version:  2;  // expect 2
        // byte 1
        unsigned char payload:  7;  // 
        unsigned char marker:   1;  // expect 1
        // bytes 2, 3
        unsigned short seq_no;            
        // bytes 4-7
        unsigned  long timestamp;        
        // bytes 8-11
        unsigned long ssrc;         // stream number is used here
    };

    struct RTP_EXT_DATA
    {
        // bytes 0,1
        unsigned short usMultiLayerCodeType;     //����������MULTI_LAYER_CODE_TYPE
        //bytes 2,3
        unsigned short usWaterMarkType;          //����ˮӡ����WATER_MARK_TYPE
        //bytes 4,5,6,7
        unsigned long ulWaterMarkValue;          //����ˮӡֵ
    };

    typedef enum  MULTI_LAYER_CODE_TYPE
    {
        MULTI_LAYER_CODE_0 = 0,         //����������0
        MULTI_LAYER_CODE_1 = 1,         //����������1
        MULTI_LAYER_CODE_2 = 2,         //����������2
        MULTI_LAYER_CODE_NULL = 3,      //��ʹ�ö�����
    }MULTI_LAYER_CODE_TYPE_E;

    typedef enum  WATER_MARK_TYPE
    {
        WATER_MARK = 0,                 //ʹ������ˮӡ
        WATER_MARK_NULL = 1,            //��ʹ������ˮӡ
    }WATER_MARK_TYPE_E;

    struct NALU_HEADER  // 1 BYTES
    {
        // byte 0
        unsigned char TYPE: 5;
        unsigned char NRI:  2;
        unsigned char F:    1;    
    }; 

    struct FU_INDICATOR // 1 BYTES
    {
        // byte 0
        unsigned char TYPE: 5;
        unsigned char NRI:  2; 
        unsigned char F:    1;
    };

    struct FU_HEADER    // 1 BYTES
    {
        // byte 0
        unsigned char TYPE: 5;
        unsigned char R:    1;
        unsigned char E:    1;
        unsigned char S:    1;    
    };

    struct JPEG_HEADER 
    {
        unsigned int tspec:8;  //type-specific field
        unsigned int off:24;   //fragment byte offset
        unsigned char type;    //id of jpeg decoder params
        unsigned char q;       //quantization factor (or table id)
        unsigned char width;   //frame width in 8 pixel blocks
        unsigned char height;  //frame height in 8 pixel blocks
    };

    struct JPEG_HEADER_RST
    {
        unsigned short dri;
        unsigned int f:1;
        unsigned int l:1;
        unsigned int count:14;
    };

    struct JPEG_HEADER_QTABLE 
    {
        unsigned char  mbz;
        unsigned char  precision;
        unsigned short length;
    };

    struct RTCP_HEADER
    {
        // byte 0
        unsigned char   reportCount:  5;
        unsigned char   padding:      1;       // expect 0
        unsigned char   version:      2;       // expect 2
        // byte 1
        unsigned char   payload;
        // bytes 2, 3
        unsigned short  length;
        // bytes 4, 7
        unsigned long   ssrc;
    };

    // ����RR�ṹ
    typedef struct _stRTCP_RR_PACKET
    {
        RTCP_HEADER       stHeader;
        unsigned int      unReporterSSRC;
        unsigned int      unReporteeSSRC;
        unsigned int      unLossPercent : 8;    // �����ʣ�����ȡ��
        unsigned int      unLostCount   : 24;
        unsigned int      unMaxReceivedSeq;
        unsigned int      unJitter;
        unsigned int      unLastSRTimestamp;
        unsigned int      unDelayFromLastSR;
    }RTCP_RR_PACKET;

    // ����SDES�ṹ
    enum _enSDES_TYPE
    {
        SDES_TYPE_END           = 0,            // ��������
        SDES_TYPE_CNAME         = 1             // CNAME
    };
    typedef struct _stRTCP_SDES_PACKET
    {
        RTCP_HEADER        stHeader;
        unsigned int       unReporterSSRC;
    }RTCP_SDES_PACKET;

    struct INTERLEAVED_HEAD
    {
        char tag;
        char channel;
        unsigned short usLen;
    };

    typedef enum
    {
        H264_NALU_TYPE_UNDEFINED	= 0,
        H264_NALU_TYPE_STAP_A		= 24,
        H264_NALU_TYPE_STAP_B		= 25,
        H264_NALU_TYPE_MTAP16		= 26,
        H264_NALU_TYPE_MTAP24		= 27,
        H264_NALU_TYPE_FU_A			= 28,
        H264_NALU_TYPE_FU_B			= 29,
        H264_NALU_TYPE_END	
    }H264_NALU_TYPE;//lint !e751

    typedef enum
    {
        INTERLEAVED_TYPE_RTP_VIDEO   = 0,  //RTP��Ƶ
        INTERLEAVED_TYPE_RTCP_VIDEO  = 1,  //RTCP��Ƶ
        INTERLEAVED_TYPE_RTP_AUDIO   = 2,  //RTP��Ƶ
        INTERLEAVED_TYPE_RTCP_AUDIO  = 3   //RTCP��Ƶ
    }INTERLEAVED_CHANNEL_TYPE;//lint !e751

    //enum PAY_LOAD_TYPE
    //{
    //    PAY_LOAD_TYPE_PCMU  = 0,    ///< G711��u��
    //    PAY_LOAD_TYPE_G723  = 4,    ///< G723
    //    PAY_LOAD_TYPE_PCMA  = 8,    ///< G711��a��
    //    PAY_LOAD_TYPE_G722  = 9,    ///< G722
    //    PAY_LOAD_TYPE_MJPEG = 26,   ///< MJPEG
    //    PAY_LOAD_TYPE_AMR_MB= 97,   ///< AMR_NB
    //    PAY_LOAD_TYPE_MPEG4 = 98,   ///< MPEG4
    //    PAY_LOAD_TYPE_H264  = 99,   ///< H264
    //    PAY_LOAD_TYPE_AVS   = 100,  ///< AVS
    //    PAY_LOAD_TYPE_MP3   = 101,  ///< MP3
    //    PAY_LOAD_TYPE_3GPP  = 102  ///< 3GPP
    //};

}

#pragma pack(pop)
#endif //RTP_H

