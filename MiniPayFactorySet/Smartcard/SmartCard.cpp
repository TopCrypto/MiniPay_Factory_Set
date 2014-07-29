#include "stdafx.h"

#include <Winscard.h>
#include "SmartCard.h"
#pragma comment(lib, "Winscard.lib")


SCARD_READERSTATE ReaderState;
SCARDCONTEXT g_sc1;
char ReaderName[20][50];
//extern SCARDHANDLE S_CardHandle;

//DWORD UpdateDevice(char *pReaderName) 
DWORD UpdateDevice() 
{
	// TODO: Add your control notification handler code here
	DWORD		lReturn = 0;
	LPTSTR		pmszReaders = NULL, pReader=NULL;
	DWORD		cch = SCARD_AUTOALLOCATE;
	unsigned short i = 0,j = 0;
	CString		str;
	unsigned char   readercount = 0;		
	SCARDCONTEXT	g_Context = 0;

	lReturn = SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&g_Context);

	lReturn=SCardListReaders(g_Context, NULL, (LPTSTR)&pmszReaders, &cch);


	if (SCARD_S_SUCCESS == lReturn)
	{
		readercount=0x00;
		for(i=0;i<20;i++)
		for(j=0;j<100;j++)
		ReaderName[i][j]=0x00;		
		pReader = pmszReaders;
		while ( '\0' != *pReader )
		{
			str.Format("%s",pReader);			
			memcpy(ReaderName[readercount], pReader, strlen(pReader));	//save reader names 
			// Advance to the next value.
			readercount++;
			pReader = pReader +  strlen(pReader) + 1;
		}
		lReturn = SCardFreeMemory(g_Context,pmszReaders );
	}
	return lReturn;
}


DWORD LCardConnectCard(LPCTSTR  pbReaderName,SCARDHANDLE *hScard,
					   DWORD *pdwActiveProtocol,SCARDCONTEXT *phContext,
					   BYTE *bATR)
{
	DWORD dwRet = SCARD_S_SUCCESS;
	//DWORD dwAP;
	// Establish the context.
	dwRet = SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&g_sc1);
	if ( dwRet != SCARD_S_SUCCESS  )
	{
		return dwRet;
	}
	else
	{
		*phContext= g_sc1;

		//Connect Card
		dwRet = SCardConnect(g_sc1, pbReaderName, 
			SCARD_SHARE_SHARED,SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
			hScard,
			pdwActiveProtocol);
	}

	//S_CardHandle = (SCARDHANDLE) *hScard;

	// get ATR
	CString str;
	BYTE bAtr[64] = {0};
	
	ReaderState.szReader = pbReaderName;
	ReaderState.pvUserData = NULL;
	ReaderState.dwEventState = SCARD_STATE_CHANGED;

	dwRet=SCardGetStatusChange(
		g_sc1,
		1000,
		&ReaderState,
		1);
	if(dwRet==SCARD_S_SUCCESS)
	{
// 		CHAR Hex[3];
// 		UINT i;
// 
// 		memset(bAtr,0,sizeof(bAtr));
// 		for(i=0;i<ReaderState.cbAtr;i++)
// 		{
// 			_itoa(ReaderState.rgbAtr[i],Hex,16);
// 			if(lstrlen(Hex)==1) 
// 			{
// 				Hex[1]=Hex[0];
// 				Hex[0]='0'; 
// 				Hex[2]=0;
// 			}
// 			strcat((char *)bAtr,Hex);
// 			strcat((char *)bAtr," ");
// 		}

	//	memcpy(bATR,bAtr,strlen((char *)bAtr));
		memcpy(bATR, ReaderState.rgbAtr, ReaderState.cbAtr);

	}
	return dwRet;
}

DWORD LCardSendCommand(SCARDHANDLE  hScard,BYTE bSendType,
					   PBYTE pbCmd, DWORD dwCmdSize,
					   PBYTE pbRec, DWORD *dwRecLen)
{
	DWORD             dwReturn = 0;	
	if(SCARD_PROTOCOL_T0 == bSendType)
	{

		dwReturn = SCardTransmit(hScard, 
			SCARD_PCI_T0,
			pbCmd, 
			dwCmdSize, 
			NULL, 
			pbRec, 
			dwRecLen);
	}
	else if (SCARD_PROTOCOL_T1 == bSendType)
	{
		dwReturn = SCardTransmit(hScard, 
			SCARD_PCI_T1,
			pbCmd, 
			dwCmdSize, 
			NULL, 
			pbRec, 
			dwRecLen);
	}	
	return dwReturn;
}


DWORD LCardGetData(SCARDHANDLE  hScard,BYTE bSendType,
				   PBYTE pbCmd, DWORD dwCmdSize,
				   PBYTE pbResponse,PDWORD pdwResponseSize)
{

	DWORD dwReturn;
	//	SCARD_IO_REQUEST  ioSendPci = {1, sizeof(SCARD_IO_REQUEST)};
	BYTE			recvbuf[256];
	DWORD			recvlen = sizeof(recvbuf);
	BYTE            SW1, SW2;
	DWORD			dwDataSize = 0;

	__try
	{
		if(!bSendType)
		{
			dwReturn = SCardTransmit(hScard, 
				SCARD_PCI_T0, 
				pbCmd, 
				dwCmdSize, 
				NULL, 
				recvbuf, 
				&recvlen);
		}
		else
		{
			dwReturn = SCardTransmit(hScard, 
				SCARD_PCI_T1, 
				pbCmd, 
				dwCmdSize, 
				NULL, 
				recvbuf, 
				&recvlen);
		}
		if ( dwReturn != SCARD_S_SUCCESS )
		{
			return dwReturn;
		}
		SW1 = recvbuf[recvlen-2];
		SW2 = recvbuf[recvlen-1];

		if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
		{
			dwDataSize = recvlen-2;
			memcpy(pbResponse, recvbuf, dwDataSize);
		}
		else
		{
			dwReturn = SCARD_F_UNKNOWN_ERROR;
			return dwReturn;
		}
		*pdwResponseSize = dwDataSize;
	}
	__finally
	{
	}

	return dwReturn;
}

DWORD LCardDisConnectCard(SCARDHANDLE hScard)
{
	DWORD dwRet = SCARD_S_SUCCESS;

	dwRet = SCardDisconnect(hScard, SCARD_LEAVE_CARD);
	if(dwRet != SCARD_S_SUCCESS)
	{
		return dwRet;
	}
	if(g_sc1)
	{
		// Free the context.
		dwRet=SCardReleaseContext(g_sc1);
		if(dwRet != SCARD_S_SUCCESS)
		{
			return dwRet;
		}
	}
	return dwRet;
}
