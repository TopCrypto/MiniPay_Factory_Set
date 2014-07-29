#include "stdafx.h"
#include "SmartCard.h"
#include "PCSC.h"
/*****smard card ****************/
SCARDHANDLE hCard = 0;
DWORD dwProtocol = 0;
/********************************/
DWORD SCard_UpdateDevice()
{
	DWORD dwRet = 0;
	dwRet = UpdateDevice();
	return dwRet;
}

DWORD SCard_Connect(LPCTSTR  pbReaderName,BYTE *bATR)
{
	LPSCARDCONTEXT phContext = 0;
	DWORD dwRet = 0;
	SCARDCONTEXT	g_Context = 0;	
	
	dwRet = LCardConnectCard(pbReaderName,&hCard,&dwProtocol,&g_Context,bATR);	
	return dwRet;
}

DWORD SCard_DisConnect()
{
	DWORD dwRet = 0;
	dwRet = LCardDisConnectCard(hCard);
	hCard = 0;
	dwProtocol = 0;
	return dwRet;
}

DWORD SCard_ApduExchang(UCHAR *pucCommand,unsigned short usCommandLen,UCHAR *pucResond,unsigned short *pusRespondLen)
{  
	DWORD dwResult = 0;
	DWORD dwRespondLen = 512;
	BYTE SW1 = 0;
	BYTE SW2 = 0;

	dwResult = LCardSendCommand(hCard,(BYTE)dwProtocol,pucCommand,usCommandLen,pucResond,&dwRespondLen);//- Le
	if (SCARD_S_SUCCESS != dwResult)
	{
		return dwResult;
	}
	SW1 = pucResond[dwRespondLen - 2];
	SW2 = pucResond[dwRespondLen - 1];
	if (0x61 == SW1)
	{		
		BYTE ucGetRespondCommand_61[5] = "\x00\xC0\x00\x00";
		ucGetRespondCommand_61[4] = SW2; 
		dwRespondLen = 256;
		dwResult = LCardSendCommand(hCard,(BYTE)dwProtocol,ucGetRespondCommand_61,0x05,pucResond,&dwRespondLen);
		
	}
	else if (0x6C == SW1)
	{
		BYTE ucGetRespondCommand_6C[5] = "\x00\x00\x00\x00";
		memcpy(&ucGetRespondCommand_6C,pucCommand,0x04);
		ucGetRespondCommand_6C[4] = SW2; 
		dwRespondLen = 256;
		dwResult = LCardSendCommand(hCard,(BYTE)dwProtocol,ucGetRespondCommand_6C,0x05,pucResond,&dwRespondLen);
	}
	*pusRespondLen = (USHORT)dwRespondLen;
	return dwResult;
}

DWORD SCard_SendCommand(BYTE *pucCommand,unsigned short usCommandLen,BYTE *pucResond,unsigned short *pusRespondLen,BOOL bPassFlag)
{  
	DWORD dwResult = 0;
	DWORD dwRespondLen = 256;
	BYTE SW1 = 0;
	BYTE SW2 = 0;
	
	dwResult = LCardSendCommand(hCard,(unsigned char)dwProtocol,pucCommand,usCommandLen,pucResond,&dwRespondLen);
	if (SCARD_S_SUCCESS != dwResult)
	{
		return dwResult;
	}

	if (FALSE == bPassFlag)
	{
		SW1 = pucResond[dwRespondLen - 2];
		SW2 = pucResond[dwRespondLen - 1];
		if (0x61 == SW1)
		{		
			UCHAR ucGetRespondCommand_61[5] = "\x00\xC0\x00\x00";
			ucGetRespondCommand_61[4] = SW2; 
			dwRespondLen = 256;
			dwResult = LCardSendCommand(hCard,(unsigned char)dwProtocol,ucGetRespondCommand_61,0x05,pucResond,&dwRespondLen);
			
		}
		else if (0x6C == SW1)
		{
			UCHAR ucGetRespondCommand_6C[5] = "\x00\x00\x00\x00";
			memcpy(&ucGetRespondCommand_6C,pucCommand,0x04);
			ucGetRespondCommand_6C[4] = SW2; 
			dwRespondLen = 256;
			dwResult = LCardSendCommand(hCard,(unsigned char)dwProtocol,ucGetRespondCommand_6C,0x05,pucResond,&dwRespondLen);
			
		}
	}
	
	*pusRespondLen = (USHORT)dwRespondLen;
	return TRUE;
}