/********************************************************************
filename    :    ivs_compression.h
author      :    yWX150774
created     :    2012/12/26
description :    ѹ������ѹ���ļ��ӿڷ�װ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/26 ��ʼ�汾
*********************************************************************/

#ifndef CBB_COMPRESSION_H
#define CBB_COMPRESSION_H


#include "ivs_namespace.h"

#include "zlib.h"

NAMESPACE_CBB_BEGIN

/*********************************************
 * ѹ������ѹ���ļ���, ��ʱֻ�ṩzip��ʽ
*********************************************/
class EXPORT_DLL CCompression
{
public:
    enum enCompressionLevel
    {
        ZIP_COMPRESS_NO      = Z_NO_COMPRESSION,     // ��ѹ��
        ZIP_COMPRESS_FAST    = Z_BEST_SPEED,         // ���
        ZIP_COMPRESS_BEST    = Z_BEST_COMPRESSION,   // ����
        ZIP_COMPRESS_DEFAULT = Z_DEFAULT_COMPRESSION // Ĭ��
    };

public:
    /*****************************************************************
     * zip��ʽѹ���ļ�
     * pszSrcFilename: ��ѹ���ļ���
     * pszDstFilename: ѹ������ļ��� (ΪNULL���Զ������ļ���)
     * enZipLevel    : ѹ���ȼ�
     * ����ֵ         : ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool Zip(
        const char * pszSrcFilename, 
        const char * pszDstFilename, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip��ʽ��ѹ���ļ�
     * pszSrcFilename: ����ѹ���ļ���
     * pszDstFilename: ��ѹ������ļ��� (ΪNULLʱ, ֱ�ӷ���ʧ��)
     * ����ֵ         : ��ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool UnZip(
        const char * pszSrcFilename, 
        const char * pszDstFilename
    );

    /*****************************************************************
     * zip��ʽѹ���ļ�
     * pSrcFile  : ��ѹ���ļ����    (�����������ڴ��ͷ�)
     * pDstFile  : ѹ������ļ����   (�����������ڴ��ͷ�)
     * enZipLevel: ѹ���ȼ�
     * ����ֵ     : ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool Zip(
        FILE * pSrcFile, 
        FILE * pDstFile, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip��ʽ��ѹ���ļ�
     * pSrcFile: ����ѹ���ļ����    (�����������ڴ��ͷ�)
     * pDstFile: ��ѹ������ļ����   (�����������ڴ��ͷ�)
     * ����ֵ   : ��ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool UnZip(
        FILE * pSrcFile, 
        FILE * pDstFile
    );

    /*****************************************************************
     * ��֪zip��ʽѹ��������ݻ��泤��
     * pszSrcData      : ��ѹ��������
     * uiSrcDataLength : ��ѹ�������ݵĴ�С
     * uiNeedBufferSize: �����������Ļ����С (�������)
     * enZipLevel      : ѹ���ȼ�
     * ����ֵ           : ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool GetZipBufferLength(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned int & uiNeedBufferSize, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * ��֪zip��ʽ��ѹ��������ݻ��泤��
     * pszSrcData      : ����ѹ��������
     * uiSrcDataLength : ����ѹ�������ݵĴ�С
     * uiNeedBufferSize: �����������Ļ����С (�������)
     * ����ֵ           : ��ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool GetUnZipBufferLength(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned int & uiNeedBufferSize
    );

    /*****************************************************************
     * zip��ʽѹ������
     * pszSrcData      : ��ѹ��������
     * uiSrcDataLength : ��ѹ�������ݵĴ�С
     * pszDstData      : �������   (�������)
     * uiDstDataLength : �������ʱ: ��������С; �������ʱ: ������ݵĴ�С
     * enZipLevel      : ѹ���ȼ�
     * ����ֵ           : ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool Zip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip��ʽ��ѹ������
     * pszSrcData      : ����ѹ��������
     * uiSrcDataLength : ����ѹ�������ݵĴ�С
     * pszDstData      : �������   (�������)
     * uiDstDataLength : �������ʱ: ��������С, �������ʱ: ������ݵĴ�С
     * ����ֵ           : ��ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool UnZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength
    );

private:
    /*****************************************************************
     * ����zip��ʽѹ������
     * pszSrcData      : ��ѹ��������
     * uiSrcDataLength : ��ѹ�������ݵĴ�С
     * pszDstData      : �������, ΪNULLʱ, ֻ��֪�����������Ļ����С
     * uiDstDataLength : �����������Ļ����С
     * enZipLevel      : ѹ���ȼ�
     * ����ֵ           : ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool TryZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * ����zip��ʽ��ѹ������
     * pszSrcData      : ����ѹ��������
     * uiSrcDataLength : ����ѹ�������ݵĴ�С
     * pszDstData      : �������, ΪNULLʱ, ֻ��֪�����������Ļ����С
     * uiDstDataLength : �����������Ļ����С
     * ����ֵ           : ��ѹ���Ƿ�ɹ�
     ****************************************************************/
    static bool TryUnZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength
    );

private:
    static const unsigned int ZIP_COMPRESSION_MAX_SIZE; // ÿ��zipѹ���ļ�������С
};

NAMESPACE_CBB_END


#endif // CBB_COMPRESSION_H
