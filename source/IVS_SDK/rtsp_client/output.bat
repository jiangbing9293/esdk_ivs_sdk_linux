echo off
set GetType=%~1
@echo on
@echo ----------------------------��ʼ���������ļ�-----------------------------
@echo off
if /i "%GetType%"=="0" (
   	xcopy /y /i /r /s "..\build\lib\rtsp_client.lib"		"out\lib\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.lib"		"out\lib\debug\"
	xcopy /y /i /r /s "..\build\lib\rtsp_client.dll"		"out\out\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.dll"		"out\out\debug\"
	xcopy /y /i /r /s "..\build\lib\rtsp_client.pdb"		"out\pdb\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.pdb"		"out\pdb\debug\"
	xcopy /y /i /r /s "header\rtsp_client_api.h"		"out\inc\"
	xcopy /y /i /r /s "header\rtsp_client_datatype.h"		"out\inc\"
) else (
   	xcopy /y /i /r /s "..\build\lib\rtsp_client.lib"		"out\lib\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.lib"		"out\lib\debug\"
	xcopy /y /i /r /s "..\build\lib\rtsp_client.dll"		"out\out\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.dll"		"out\out\debug\"
	xcopy /y /i /r /s "..\build\lib\rtsp_client.pdb"		"out\pdb\release\"
	xcopy /y /i /r /s "..\build\lib\rtsp_clientd.pdb"		"out\pdb\debug\"
	xcopy /y /i /r /s "header\rtsp_client_api.h"		"out\inc\"
	xcopy /y /i /r /s "header\rtsp_client_datatype.h"		"out\inc\"
)
@echo on
@echo ----------------------------�����ļ���������---------------------------------
@echo off


