#ifndef __DOWMLOAD_MGR_H__
#define __DOWMLOAD_MGR_H__

#include "Download.h"
#include "UserInfo.h"
#include "hwsdk.h"
#include "SDKDef.h"

class CDownloadMgr
{
public:
	CDownloadMgr(void);
	~CDownloadMgr(void);

public:
	void SetUserMgr(CUserMgr* pUserMgr);
	
	int StartPlatformDownLoad(
		    const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		    const char* pCameraCode,
		    const IVS_DOWNLOAD_PARAM* pDownloadParam,
		    IVS_ULONG* pHandle
		  );

	int StartPlatformDownLoadRawCB(
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pNVRCode,
		const char* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam,
		DownloadCallBackRaw pFPlatformDownLoadRawCB,
		void* pUserData,
		IVS_ULONG* pHandle);
	
	int StopPlatformDownLoad(IVS_ULONG ulHandle);

    // ��������¼������
    int StartDisasterRecoveryDownload(const char* pCameraCode, 
        const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pHandle);
    // ֹͣ����¼������
    int StopDisasterRecoveryDownload(IVS_ULONG ulHandle);

	int DownLoadPause(IVS_ULONG ulHandle);
	int DownLoadResume(IVS_ULONG ulHandle);

	int SetDownLoadSpeed(IVS_ULONG ulHandle, float fSpeed);
	int GetDownLoadInfo(IVS_ULONG ulHandle, IVS_DOWNLOAD_INFO* pInfo);

	int StartPUDownLoad(
			const char* pCameraCode,
			const IVS_DOWNLOAD_PARAM* pDownloadParam,
			IVS_ULONG  *pHandle
		);
	int StartPUDownLoadRawCB(
		const char* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam,
		DownloadCallBackRaw pFPUDownLoadRawCB,
		void* pUserData,
		IVS_ULONG  *pHandle
		);

	int StopPUDownLoad(IVS_ULONG ulHandle);

	CDownload* GetFreeDownLoad(IVS_ULONG& ulHandle);
    CDownload* GetDownLoad(IVS_ULONG ulHandle);

	// ����rtsp handle��ȡ���ؾ��
    IVS_ULONG  GetDownloadHandlebyRtspHandle(int iRtspHandle);

    // ����netSourceHandle��ȡ���ؾ��
    IVS_ULONG GetDownloadHandleByNetSourceChannel(unsigned long ulNetSourceChannel);

	// ���ݲ���ͨ���Ż�ȡ���ؾ��
    IVS_ULONG GetDownloadHandleByPlayChannel(unsigned long ulPlayChannel);


    // ��ȡǰ��¼���ļ���
	int GetPUDownLoadFileName(const char* pCameraCode,
                                              const IVS_TIME_SPAN* pTimeSpan,
                                              IVS_RECORD_INFO &stRecordInfo);

private:
	CUserMgr *m_pUserMgr;
	typedef std::map<unsigned long, CDownload*> HandleDownLoadMap;
	typedef HandleDownLoadMap::iterator HandleDownLoadIter;
	
	HandleDownLoadMap m_HandleDownLoadMap;
	VOS_Mutex* m_pHandleDownLoadMutex;	
};

#endif
