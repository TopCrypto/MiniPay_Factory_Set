#ifndef  __SMARTCARD_H__
#define  __SMARTCARD_H__

#include <Winscard.h>
#define SEND_TYPE_A	0
#define SEND_TYPE_B	1

extern char ReaderName[20][50];


DWORD LCardConnectCard(LPCTSTR  pbReaderName,SCARDHANDLE *hScard,
					   DWORD *pdwActiveProtocol,SCARDCONTEXT *phContext,
					   BYTE *bATR);
DWORD LCardSendCommand(SCARDHANDLE  hScard, BYTE bSendType,
					   PBYTE pbCmd, DWORD dwCmdSize,
					   PBYTE pbRec, DWORD *dwRecLen);
DWORD LCardGetData(SCARDHANDLE  hScard,BYTE bSendType,
				   PBYTE pbCmd, DWORD dwCmdSize,
				   PBYTE pbResponse, PDWORD pdwResponseSize);
DWORD LCardDisConnectCard(SCARDHANDLE hScard);
DWORD UpdateDevice();

#endif