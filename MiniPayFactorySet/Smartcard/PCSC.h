#ifndef _PCSC_H

#define PCSC_SUCCESS	0
#define PCSC_DATAERR	1

extern char ReaderName[20][50];

DWORD SCard_UpdateDevice();
DWORD SCard_Connect(LPCTSTR  pbReaderName,BYTE *bATR);
DWORD SCard_DisConnect();
DWORD SCard_ApduExchang(BYTE *pcuCommand,unsigned short usCommandLen,BYTE *pucResond,unsigned short *pusRespondLen);
#define _PCSC_H
#endif
