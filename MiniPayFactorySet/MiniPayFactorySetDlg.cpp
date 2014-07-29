
// MiniPayFactorySetDlg.cpp : ʵ���ļ�
//
#pragma warning (disable : 4996) 
#include "stdafx.h"
#include "MiniPayFactorySet.h"
#include "MiniPayFactorySetDlg.h"
#include "afxdialogex.h"
#include "tool.h"
#include "PCSC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MiniPayInfo minpayinfo;
IniFileType g_initinfo[]=
{
	{"Union_TerminalStatus", "value","selected",FALSE},
	{"Union_TerminalID", "value","selected", FALSE},
	{"Union_FirmwareVersion", "value","selected", FALSE},
	{"Union_PublicKeyVersion", "value","selected", FALSE},
	{"Union_CardPAN", "value", "selected",FALSE},
	{"Union_ReverseFlag", "value","selected", FALSE},
	{"Union_Label", "value", "selected",FALSE},
	{"Union_Issuer", "value","selected", FALSE},
	{"Union_PinCertSerialNumber", "value","selected", FALSE},
	{"Union_RACertType", "value","selected", FALSE},
	{_T(""), _T(""),_T(""), FALSE}

};

DEVINFO_Ver_1_0 devinfo;
IniFileType g_devinfo[]=
{
	{"Version", "value","selected",FALSE},
	{ "Manufactuer", "value","selected", FALSE},
	{ "Issuer", "value","selected", FALSE},
	{"Label", "value","selected", FALSE},
	{"SerialNumber", "value", "selected",FALSE},
	{"HWVersion", "value","selected", FALSE},
	{ "FirmwareVersion", "value", "selected",FALSE},
	{"AlgSymCap", "value","selected", FALSE},
	{"AlgAsymCap", "value","selected", FALSE},
	{"AlgHashCap", "value","selected", FALSE},
	{"DevAuthAlgId", "value","selected",FALSE},
	{ "TotalSpace", "value","selected", FALSE},
	{ "FreeSpace", "value","selected", FALSE},
	{"FactotySetting", "value","selected", FALSE},
	{"ProductionDate", "value", "selected",FALSE},
	{"TerminalState", "value","selected", FALSE},
	{"TerminalVersion", "value", "selected",FALSE},
	{"BootLoaderVersion", "value","selected", FALSE},
	{_T(""), _T(""),_T(""), FALSE}

};

READ_LIST g_Read_List[]=
{
	{"�ն��豸״̬",1},
	{"�ն��豸��Ϣ",2},
	{NULL, 0}
};



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMiniPayFactorySetDlg �Ի���



CMiniPayFactorySetDlg::CMiniPayFactorySetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMiniPayFactorySetDlg::IDD, pParent)
	, m_strID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniPayFactorySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDV_MaxChars(pDX, m_strID, 8);
	DDX_Control(pDX, IDC_COMBO1, m_READLIST);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
}

BEGIN_MESSAGE_MAP(CMiniPayFactorySetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &CMiniPayFactorySetDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON1, &CMiniPayFactorySetDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMiniPayFactorySetDlg ��Ϣ�������

BOOL CMiniPayFactorySetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��ʼ���ṹ������

	memset(&minpayinfo, '\0', sizeof(minpayinfo));
	memset(&devinfo, '\0', sizeof(devinfo));
	GetIniUnionInfo();
	GetIniDevInfo();

	m_READLIST.Clear();
	int i;
	for(i = 0;;i++)
	{
		if(g_Read_List[i].nReadID == 0)
		{
			break;
		}
		m_READLIST.InsertString(i, g_Read_List[i].strReadName);
	}

      m_READLIST.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMiniPayFactorySetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMiniPayFactorySetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMiniPayFactorySetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMiniPayFactorySetDlg::ConnectDevice()
{
	BOOL bResult = FALSE;
	DWORD dwResult = 0;
	BYTE bATR[64] = {0};		
	unsigned char ucVD10Name[20] = {0};
	unsigned short usReaderLen = 0;
	int i = 0;

	//	usReaderLen = sizeof("Vanstone VD10") - 1;
	//	memcpy(ucVD10Name,"Vanstone VD10",usReaderLen);

	memcpy(ucVD10Name, ucDevName, 20);
	usReaderLen =  strlen((char *)ucVD10Name);
	int devIndex = 0;

	dwResult = SCard_UpdateDevice();
	for (i = 0;i < 20 ;i ++)
	{
		if (memcmp(ucVD10Name,ReaderName[i],usReaderLen) == 0)
		{
			devIndex = i;
			break;
		}

	}

	if (i >= 20)
	{
		MessageBox("�Ҳ��������ļ��е��豸��!");
		bResult = FALSE;
		return bResult;
	}

	dwResult = SCard_Connect((LPCTSTR)ReaderName[devIndex],bATR);
	if (SCARD_S_SUCCESS == dwResult)
	{	

		bResult = TRUE;
	}
	else
	{
		MessageBox("Connect Error!");
		bResult = FALSE;
		return bResult;
	}
	return bResult;
}

void CMiniPayFactorySetDlg::GetIniUnionInfo()
{
	CString dir="";
	::GetCurrentDirectory(MAX_PATH,dir.GetBuffer(MAX_PATH));
	dir.ReleaseBuffer();
	dir+="\\MiniPayConfig.ini";  // ·��
   
	CString devicename;
	GetPrivateProfileString("DeviceName","value","",devicename.GetBuffer(MAX_PATH),MAX_PATH,dir);  
	devicename.ReleaseBuffer();
	memset(ucDevName, 0x00, 20);  //�豸����
	memcpy(ucDevName, devicename, devicename.GetLength());

	CString str;
	int i = 0;
	for(i = 0;; i++)
	{
		str = "";
		if(g_initinfo[i].strApp[0] == '\0')
		{
			break;
		}

		GetPrivateProfileString((LPCSTR)g_initinfo[i].strApp,(LPCSTR)g_initinfo[i].strKey1,"",str.GetBuffer(MAX_PATH),MAX_PATH,dir); 
		str.ReleaseBuffer();
		m_strUnionMinipay.Add(str);
	}


	for(i = 0;; i++)
	{
		str = "";
		if(g_initinfo[i].strApp[0] == '\0')
		{
			break;
		}

		GetPrivateProfileString((LPCSTR)g_initinfo[i].strApp,(LPCSTR)g_initinfo[i].strKey2,"",str.GetBuffer(MAX_PATH),MAX_PATH,dir); 
		str.ReleaseBuffer();
		if(str == _T("TRUE"))
		{
			g_initinfo[i].bSelected = TRUE;
		}else
		{
			g_initinfo[i].bSelected = FALSE;
		}

	}



}




BOOL CMiniPayFactorySetDlg::SetMiniPay()
{

	BOOL    bResult = TRUE;
	BYTE    pbSendBuffer[1025]={0};
	BYTE    pbRecvBuffer[1025]={0};
	unsigned short dwSlen =256;
	unsigned short dwRlen =256;
	DWORD   dwRet = 256;
	unsigned short usTemplen = 0;
	int i;

	if(!ConnectDevice())
	{
		return FALSE;
	}

	pbSendBuffer[CLA]=CLA_SYSTME_CMD_NONE;//FF     
	pbSendBuffer[INS]=INS_SYSTME_CMD;//70
	pbSendBuffer[P1]=P1_SYSTEM_CMD;//25
	pbSendBuffer[P2]=P2_SYSTEM_CMD;//7B
	pbSendBuffer[Lc]=LC_SYSTEM_CMD;//LC
	pbSendBuffer[DataAddr+CLA]=Data_CLA;//CLA
	pbSendBuffer[DataAddr+INS]=Data_INS_SetTerminalInfo;//INS
	pbSendBuffer[DataAddr+P1]=Data_P1_MiniPay;//P1
	pbSendBuffer[DataAddr+P2]=Data_P2;//P2     ����ָ���ֲ�����
	pbSendBuffer[DataAddr+Lc]=Data_Lc;//LC


	int nCount = m_strUnionMinipay.GetSize();
	int iSelected[max_len];
	for(i = 0; i < max_len; i++)
		iSelected[i] = -1;

	for(i = 0; i < nCount; i++)
	{
		if(g_initinfo[i].bSelected)
		{
			iSelected[i] = i;
		}
	}

	for(i = 0; i < nCount; i++)
	{

		usTemplen = 0;
		memset(pbSendBuffer+Data_Offset, 0x00,sizeof(pbSendBuffer)-Data_Offset);  //���

		switch(iSelected[i])
		{
		case 0:
			pbSendBuffer[DataAddr+P2] = P2_TerminalStatus;

         	HexCharsToStdChars((PBYTE)m_strUnionMinipay[0].GetBuffer(m_strUnionMinipay[0].GetLength()), &(minpayinfo.TerminalStatus), m_strUnionMinipay[0].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(minpayinfo.TerminalStatus), sizeof(minpayinfo.TerminalStatus));
			usTemplen += sizeof(minpayinfo.TerminalStatus);
			break;

		case 1:
			{
				//23���ֽ�(ASCII)���㸶�ն�ID = ��������(8) + ���̱���(3 ) + �������� (4)+ �ն˱�ʶ��(8)
				pbSendBuffer[DataAddr+P2] = P2_TerminalID;

				CString strTerminalID, strMiniPayID;
				UpdateData(TRUE);
				GetDlgItemText(IDC_EDIT1,strTerminalID);   //8�ֽڵ��ն˱�ʶ��
				UpdateData(FALSE);

				SYSTEMTIME systemtime;   
				GetLocalTime(&systemtime);
				CString ProductionDate,year,month;
				year.Format(_T("%04d"),systemtime.wYear);
				month.Format(_T("%02d"),systemtime.wMonth);
				ProductionDate =year.Right(2)+month;     //4�ֽ��ն���������

				//m_strUnionMiniPay[1]= ��������(8)+ ���̱���(3)
				strMiniPayID = m_strUnionMinipay[1] + ProductionDate; 
				strMiniPayID += strTerminalID;          //׷��8�ֽ��ն˱�ʶ��
				memcpy(minpayinfo.TerminalID, strMiniPayID, strMiniPayID.GetLength());

				memcpy(pbSendBuffer+Data_Offset, minpayinfo.TerminalID, sizeof(minpayinfo.TerminalID));
				usTemplen += sizeof(minpayinfo.TerminalID);
				break;

			}
	
     	case 2:
			pbSendBuffer[DataAddr+P2] = P2_FirmwareVersion;

			memcpy( minpayinfo.FirmwareVersion,m_strUnionMinipay[2],m_strUnionMinipay[2].GetLength());
			memcpy(pbSendBuffer+Data_Offset, minpayinfo.FirmwareVersion, sizeof(minpayinfo.FirmwareVersion));
			usTemplen += sizeof(minpayinfo.FirmwareVersion);
			break;

		case 3:
			{
				pbSendBuffer[DataAddr+P2] = P2_PublicKeyVersion;

				memcpy( minpayinfo.PublicKeyVersion,m_strUnionMinipay[3],m_strUnionMinipay[3].GetLength());
				memcpy(pbSendBuffer+Data_Offset, minpayinfo.PublicKeyVersion, sizeof(minpayinfo.PublicKeyVersion));
				usTemplen += sizeof(minpayinfo.PublicKeyVersion);
				break;
			}
		

		case 4:
			pbSendBuffer[DataAddr+P2] = P2_CardPAN;

			memcpy( minpayinfo.CardPAN,m_strUnionMinipay[4],m_strUnionMinipay[4].GetLength());
			memcpy(pbSendBuffer+Data_Offset, minpayinfo.CardPAN, sizeof(minpayinfo.CardPAN));
			usTemplen += sizeof(minpayinfo.CardPAN);
			break;

		case 5:
			pbSendBuffer[DataAddr+P2] = P2_ReverseFlag;

			HexCharsToStdChars((PBYTE)m_strUnionMinipay[5].GetBuffer(m_strUnionMinipay[5].GetLength()), &(minpayinfo.ReverseFlag), m_strUnionMinipay[5].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(minpayinfo.ReverseFlag), sizeof(minpayinfo.ReverseFlag));
			usTemplen += sizeof(minpayinfo.ReverseFlag);
			break;
		case 6:
			pbSendBuffer[DataAddr+P2] = P2_Label;

			memcpy( minpayinfo.Label,m_strUnionMinipay[6],m_strUnionMinipay[6].GetLength());
			memcpy(pbSendBuffer+Data_Offset, minpayinfo.Label, sizeof(minpayinfo.Label));
			usTemplen += sizeof(minpayinfo.Label);
			break;
		case 7:
			pbSendBuffer[DataAddr+P2] = P2_Issuer;

			memcpy(minpayinfo.Issuer,m_strUnionMinipay[7],m_strUnionMinipay[7].GetLength());
			memcpy(pbSendBuffer+Data_Offset, minpayinfo.Issuer, sizeof(minpayinfo.Issuer));
			usTemplen += sizeof(minpayinfo.Issuer);
			break;
	    case 8:
			pbSendBuffer[DataAddr+P2] = P2_PinCertSerialNumber;

			HexCharsToStdChars((PBYTE)m_strUnionMinipay[8].GetBuffer(m_strUnionMinipay[8].GetLength()), minpayinfo.PinCertSerialNumber, m_strUnionMinipay[8].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, minpayinfo.PinCertSerialNumber, sizeof(minpayinfo.PinCertSerialNumber));
			usTemplen += sizeof(minpayinfo.PinCertSerialNumber);
			break;

		case 9:
			pbSendBuffer[DataAddr+P2] = P2_RACertType;

			HexCharsToStdChars((PBYTE)m_strUnionMinipay[9].GetBuffer(m_strUnionMinipay[9].GetLength()), &(minpayinfo.RACertType), m_strUnionMinipay[9].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(minpayinfo.RACertType), sizeof(minpayinfo.RACertType));
			usTemplen += sizeof(minpayinfo.RACertType);
			break;
		default:
			continue;
		}

		pbSendBuffer[Lc] = (BYTE)usTemplen + 5;
		pbSendBuffer[DataAddr+Lc] = (BYTE)usTemplen;
		dwSlen = Data_Offset + usTemplen;
		dwRlen = 255;

		
		dwRet = SCard_ApduExchang(pbSendBuffer, dwSlen, pbRecvBuffer, &dwRlen);
		if((dwRet != 0 )|(0x90 != pbRecvBuffer[dwRlen - 2]) | (0x00 != pbRecvBuffer[dwRlen - 1]))
		{
			AfxMessageBox(m_strUnionMinipay[i]+"���ó���");
			bResult = FALSE;
			return bResult;
		}
	}

	SCard_DisConnect();

	return bResult;
}



BOOL CMiniPayFactorySetDlg::SetManufactureInfo()
{
	BOOL    bResult = TRUE;
	BYTE    pbSendBuffer[1025]={0};
	BYTE    pbRecvBuffer[1025]={0};
	unsigned short dwSlen =256;
	unsigned short dwRlen =256;
	DWORD   dwRet = 256;
	unsigned short usTemplen = 0;
	int i;

	if(!ConnectDevice())
	{
		return FALSE;
	}

	pbSendBuffer[CLA]=CLA_SYSTME_CMD_NONE;//FF     
	pbSendBuffer[INS]=INS_SYSTME_CMD;//70
	pbSendBuffer[P1]=P1_SYSTEM_CMD;//25
	pbSendBuffer[P2]=P2_SYSTEM_CMD;//7B
	pbSendBuffer[Lc]=LC_SYSTEM_CMD;//LC
	pbSendBuffer[DataAddr+CLA]=Data_CLA;//CLA
	pbSendBuffer[DataAddr+INS]=Data_INS_SetTerminalInfo;//INS
	pbSendBuffer[DataAddr+P1]=Data_P1_DEV;//P1
	pbSendBuffer[DataAddr+P2]=Data_P2;//P2     ����ָ���ֲ�����
	pbSendBuffer[DataAddr+Lc]=Data_Lc;//LC


	int nCount = m_strDev.GetSize();
	int iSelected[max_len];
	for(i = 0; i < max_len; i++)
		iSelected[i] = -1;

	for(i = 0; i < nCount; i++)
	{
		if(g_devinfo[i].bSelected)
		{
			iSelected[i] = i;
		}
	}

	for(i = 0; i < nCount; i++)
	{

		usTemplen = 0;
		memset(pbSendBuffer+Data_Offset, 0x00,sizeof(pbSendBuffer)-Data_Offset);  //���

		switch(iSelected[i])
		{
		case 0:
			pbSendBuffer[DataAddr+P2] = DEV_P2_Version;

			HexCharsToStdChars((PBYTE)m_strDev[0].GetBuffer(m_strDev[0].GetLength()), &(devinfo.Version.major), m_strDev[0].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.Version), sizeof(devinfo.Version));
			usTemplen += sizeof(devinfo.Version);
			break;

		case 1:
			pbSendBuffer[DataAddr+P2] = DEV_P2_Manufacturer;

			memcpy( devinfo.Manufacturer,m_strDev[1],m_strDev[1].GetLength());
			memcpy(pbSendBuffer+Data_Offset, devinfo.Manufacturer, strlen(devinfo.Manufacturer)+1);
			usTemplen += strlen(devinfo.Manufacturer)+1;
			break;

		case 2:
			pbSendBuffer[DataAddr+P2] = DEV_P2_Issuer;

			memcpy( devinfo.Issuer,m_strDev[2],m_strDev[2].GetLength());
			memcpy(pbSendBuffer+Data_Offset, devinfo.Issuer, strlen(devinfo.Issuer)+1);
			usTemplen += strlen(devinfo.Issuer)+1;
			break;

		case 3:
			pbSendBuffer[DataAddr+P2] = DEV_P2_Label;

			memcpy(devinfo.Label, m_strDev[3],m_strDev[3].GetLength());
			memcpy(pbSendBuffer+Data_Offset, devinfo.Label, strlen(devinfo.Label)+1);
			usTemplen += strlen(devinfo.Label)+1;
			break;

		case 4:
		 {	pbSendBuffer[DataAddr+P2] = DEV_P2_SerialNumber;

			// ���к� = ���㸶�ն�ID���8�ֽڵ��ն˱�ʶ��
			CString strSerialNumber;
			UpdateData(TRUE);
			GetDlgItemText(IDC_EDIT1,strSerialNumber);
			UpdateData(FALSE);

			memcpy( devinfo.SerialNumber,strSerialNumber,strSerialNumber.GetLength());
			memcpy(pbSendBuffer+Data_Offset, devinfo.SerialNumber, strlen(devinfo.SerialNumber)+1);
			usTemplen += strlen(devinfo.SerialNumber)+1;
			break;
		 }
		case 5:
			pbSendBuffer[DataAddr+P2] = DEV_P2_HWVersion;

			HexCharsToStdChars((PBYTE)m_strDev[5].GetBuffer(m_strDev[5].GetLength()), &(devinfo.HWVersion.major), m_strDev[5].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.HWVersion), sizeof(devinfo.HWVersion));
			usTemplen += sizeof(devinfo.HWVersion);
			break;
		case 6:
			pbSendBuffer[DataAddr+P2] = DEV_P2_FirmVersion;

			HexCharsToStdChars((PBYTE)m_strDev[6].GetBuffer(m_strDev[6].GetLength()), &(devinfo.FirmwareVersion.major), m_strDev[6].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.FirmwareVersion), sizeof(devinfo.FirmwareVersion));
			usTemplen += sizeof(devinfo.FirmwareVersion);
			break;
		case 7:
			pbSendBuffer[DataAddr+P2] = DEV_P2_AlgSymCap;

			HexCharsToStdChars((PBYTE)m_strDev[7].GetBuffer(m_strDev[7].GetLength()), devinfo.AlgSymCap, m_strDev[7].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, devinfo.AlgSymCap, sizeof(devinfo.AlgSymCap));
			usTemplen += sizeof(devinfo.AlgSymCap);
			break;
		case 8:
			pbSendBuffer[DataAddr+P2] = DEV_P2_AlgAsymCap;

			HexCharsToStdChars((PBYTE)m_strDev[8].GetBuffer(m_strDev[8].GetLength()), devinfo.AlgAsymCap, m_strDev[8].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, devinfo.AlgAsymCap, sizeof(devinfo.AlgAsymCap));
			usTemplen += sizeof(devinfo.AlgAsymCap);
			break;

		case 9:
			pbSendBuffer[DataAddr+P2] = DEV_P2_AlgHashCap;

			HexCharsToStdChars((PBYTE)m_strDev[9].GetBuffer(m_strDev[9].GetLength()), devinfo.AlgHashCap, m_strDev[9].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, devinfo.AlgHashCap, sizeof(devinfo.AlgHashCap));
			usTemplen += sizeof(devinfo.AlgHashCap);
			break;
		case 10:
			pbSendBuffer[DataAddr+P2] = DEV_P2_DevAuthAlgId;

			HexCharsToStdChars((PBYTE)m_strDev[10].GetBuffer(m_strDev[10].GetLength()),devinfo.DevAuthAlgId, m_strDev[10].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, devinfo.DevAuthAlgId, sizeof(devinfo.DevAuthAlgId));
			usTemplen += sizeof(devinfo.DevAuthAlgId);
			break;

		case 11:
			pbSendBuffer[DataAddr+P2] = DEV_P2_TotalSpace;

			HexCharsToStdChars((PBYTE)m_strDev[11].GetBuffer(m_strDev[11].GetLength()), devinfo.TotalSpace, m_strDev[11].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, devinfo.TotalSpace, sizeof(devinfo.TotalSpace));
			usTemplen += sizeof(devinfo.TotalSpace);
			break;
		case 12:
			pbSendBuffer[DataAddr+P2] = DEV_P2_FreeSpace;

			HexCharsToStdChars((PBYTE)m_strDev[12].GetBuffer(m_strDev[12].GetLength()),devinfo.FreeSpace, m_strDev[12].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset,devinfo.FreeSpace, sizeof(devinfo.FreeSpace));
			usTemplen += sizeof(devinfo.FreeSpace);
			break;

		case 13:
			pbSendBuffer[DataAddr+P2] = DEV_P2_FactorySetting;

			HexCharsToStdChars((PBYTE)m_strDev[13].GetBuffer(m_strDev[13].GetLength()), &(devinfo.FactotySetting), m_strDev[13].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.FactotySetting), sizeof(devinfo.FactotySetting));
			usTemplen += sizeof(devinfo.FactotySetting);
			break;

		case 14:
			pbSendBuffer[DataAddr+P2] = DEV_P2_ProductionDate;

			HexCharsToStdChars((PBYTE)m_strDev[14].GetBuffer(m_strDev[14].GetLength()), devinfo.ProductionDate, m_strDev[14].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.ProductionDate), sizeof(devinfo.ProductionDate));
			usTemplen += sizeof(devinfo.ProductionDate);
			break;

		case 15:
			pbSendBuffer[DataAddr+P2] = DEV_P2_TerminalState;

			HexCharsToStdChars((PBYTE)m_strDev[15].GetBuffer(m_strDev[15].GetLength()), &(devinfo.TerminalState), m_strDev[15].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.TerminalState), sizeof(devinfo.TerminalState));
			usTemplen += sizeof(devinfo.TerminalState);
			break;

		case 16:
			pbSendBuffer[DataAddr+P2] = DEV_P2_TerminalVersion;

			HexCharsToStdChars((PBYTE)m_strDev[16].GetBuffer(m_strDev[16].GetLength()), &(devinfo.TerminalVersion.major), m_strDev[16].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.TerminalVersion), sizeof(devinfo.TerminalVersion));
			usTemplen += sizeof(devinfo.TerminalVersion);
			break;

		case 17:
			pbSendBuffer[DataAddr+P2] =  DEV_P2_BootLoaderVersion;

			HexCharsToStdChars((PBYTE)m_strDev[17].GetBuffer(m_strDev[17].GetLength()), &(devinfo.BootLoadVersion.major), m_strDev[17].GetLength()/2);
			memcpy(pbSendBuffer+Data_Offset, &(devinfo.BootLoadVersion), sizeof(devinfo.BootLoadVersion));
			usTemplen += sizeof(devinfo.BootLoadVersion);
			break;

		default:
			continue;
		}

		pbSendBuffer[Lc] = (BYTE)usTemplen + 5;
		pbSendBuffer[DataAddr+Lc] = (BYTE)usTemplen;
		dwSlen = Data_Offset + usTemplen;
		dwRlen = 255;


		dwRet = SCard_ApduExchang(pbSendBuffer, dwSlen, pbRecvBuffer, &dwRlen);
		if((dwRet != 0 )|(0x90 != pbRecvBuffer[dwRlen - 2]) | (0x00 != pbRecvBuffer[dwRlen - 1]))
		{
			AfxMessageBox(m_strDev[i]+"���ó���");
			bResult = FALSE;
			return bResult;
		}
	}

	SCard_DisConnect();
	return bResult;
}



void CMiniPayFactorySetDlg::GetIniDevInfo()
{
	CString dir="";
	::GetCurrentDirectory(MAX_PATH,dir.GetBuffer(MAX_PATH));
	dir.ReleaseBuffer();
	dir+="\\MiniPayConfig.ini";  // ·��


	CString str;
	int i = 0;
	for(i = 0;; i++)
	{
		str = "";
		if(g_devinfo[i].strApp[0] == '\0')
		{
			break;
		}

		GetPrivateProfileString((LPCSTR)g_devinfo[i].strApp,(LPCSTR)g_devinfo[i].strKey1,"",str.GetBuffer(MAX_PATH),MAX_PATH,dir); 
		str.ReleaseBuffer();
		m_strDev.Add(str);
	}


	for(i = 0;; i++)
	{
		str = "";
		if(g_devinfo[i].strApp[0] == '\0')
		{
			break;
		}

		GetPrivateProfileString((LPCSTR)g_devinfo[i].strApp,(LPCSTR)g_devinfo[i].strKey2,"",str.GetBuffer(MAX_PATH),MAX_PATH,dir); 
		str.ReleaseBuffer();
		if(str == _T("TRUE"))
		{
			g_devinfo[i].bSelected = TRUE;
		}else
		{
			g_devinfo[i].bSelected = FALSE;
		}

	}
}

BOOL CMiniPayFactorySetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{  
	
	CString str;
	str = "";
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT1,str);
	UpdateData(FALSE);

		if(str.GetLength() == 8)
		{   
		    SetDlgItemText(IDC_STATIC_RESULT,"...");
			 if(!SetMiniPay())
			 {
				 SetDlgItemText(IDC_STATIC_RESULT,"����");
				 SetDlgItemText(IDC_EDIT1, "");
				 return TRUE;
				
			 }
			 if(!SetManufactureInfo())
			 {
				 SetDlgItemText(IDC_STATIC_RESULT,"����");
				 	SetDlgItemText(IDC_EDIT1, "");
				 return TRUE;
			
			 }

			SetDlgItemText(IDC_STATIC_RESULT,"������ϣ�");
			SetDlgItemText(IDC_EDIT1, "");
	

		}
		else
		{
			MessageBox("��������кų��Ȳ�����11,����������");
			SetDlgItemText(IDC_EDIT1,""); 
		
		}

		return TRUE;
	}
    
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMiniPayFactorySetDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(!ReadMiniPayInfo())
	{
		MessageBox("��ȡ���㸶�ն���Ϣ����");
		return;
	}


}


BOOL CMiniPayFactorySetDlg::ReadMiniPayInfo()
{

	BOOL    bResult = TRUE;
	BYTE    pbSendBuffer[1025]={0};
	BYTE    pbRecvBuffer[1025]={0};
	unsigned short dwSlen =256;
	unsigned short dwRlen =256;
	DWORD   dwRet = 256;

	if(!ConnectDevice())
	{
		return FALSE;
	}

	pbSendBuffer[CLA]=0x7E;   //CLA
	pbSendBuffer[INS]=0x10;//INS
	pbSendBuffer[P1]= 0x00;//P1
	pbSendBuffer[P2]= 0x00;//P2
	pbSendBuffer[Lc]= 0x00;//Le

	int nID = g_Read_List[m_READLIST.GetCurSel()].nReadID;

	switch(nID)
	{
	case 1:
		pbSendBuffer[P2]= 0x00;//P2
		pbSendBuffer[Lc]= 0x01;//Le
		break;
	case 2:
		pbSendBuffer[P2]= 0x01;//P2
		pbSendBuffer[Lc]= 0x00;//Le
		break;
	default:
		return FALSE;
	}

	dwSlen = 5;
	dwRlen = 255;

	dwRet = SCard_ApduExchang(pbSendBuffer, dwSlen, pbRecvBuffer, &dwRlen);
	if((dwRet != 0 )|(0x90 != pbRecvBuffer[dwRlen - 2]) | (0x00 != pbRecvBuffer[dwRlen - 1]))
	{
		AfxMessageBox("�����");
		bResult = FALSE;
		return bResult;
	}

   m_edit.SetWindowText("");
   UpdateData(TRUE);
   if(nID == 1)
   {  
	   BYTE bStatus[1];
	   memcpy(bStatus, pbRecvBuffer,dwRlen -2);
	   if(bStatus[0]& 0x01)
	   {
		   m_edit.ReplaceSel("1- : �������ն�����\r\n");
	   }else
	   {
		    m_edit.ReplaceSel("1- : δ�����ն�����\r\n");
	   }

	   if(bStatus[0]& 0x02)
	   {
		   m_edit.ReplaceSel("2- : �Ѱ�װCA��֤��\r\n");
	   }else
	   {
		   m_edit.ReplaceSel("2- : δ��װCA��֤��\r\n");
	   }

	   if(bStatus[0]& 0x08)
	   {
		    m_edit.ReplaceSel("3- : �Ѱ�װPIN����֤��\r\n");
	   }else
	   {
		    m_edit.ReplaceSel("3- : δ��װPIN����֤��\r\n");
	   }

	   if(bStatus[0]& 0x10)
	   {
		    m_edit.ReplaceSel("4- : �Ѱ�װ�ն�֤��\r\n");

	   }else
	   {
		    m_edit.ReplaceSel("4- �� δ��װ�ն�֤��\r\n");
	   }

	   if( (bStatus[0]& 0x60) == 0x00)
	   {
		    m_edit.ReplaceSel("5- : �ն�״̬�ϵ�\r\n");

	   }else if( (bStatus[0]& 0x60) == 0x40)
	   {
		    m_edit.ReplaceSel("5- : �Ѱ�װCA��֤��\r\n");
	   }

   }
   else
   {
	   BYTE bTempData[1024] = {0};
	   memcpy(bTempData, pbRecvBuffer,dwRlen -2);
	   
	   int len = dwRlen-2;
	   ParseTLVData(bTempData, len);


   }

   UpdateData(FALSE);
   SCard_DisConnect();

return TRUE;
}

void  CMiniPayFactorySetDlg::ParseTLVData(unsigned char* Data, unsigned int Datalen)
{
	unsigned int currentTLVIndex = 0;
	unsigned int valuesize = 0;
	char ch_Value[1024] = {0};

	while (currentTLVIndex < Datalen)
	{  
		CString str = "";
		BYTE bTag[]={0};
		BYTE bLength[]= {0};
		memcpy(bTag,Data+currentTLVIndex, 1);        //TLV��Tag
		memcpy(bLength, Data+currentTLVIndex+1, 1);  //TLV��Length
		valuesize = bLength[0];

		memset(ch_Value, 0x00, 1024);
		memcpy(ch_Value, Data+TLV_ValueOffset+currentTLVIndex, valuesize);  //TLV��VALUE
		str = (CString)ch_Value;
		str = str + "\r\n";

		switch(bTag[0])
		{
		case 0x01:
      		str = "1-���㸶�ն�ID:    " + str;
			break;

		case 0x02:
			str = "2-���㸶�ն˰汾��:    " + str;		
			break;

		case 0x03:
		    str = "3-���㸶�ն˹�Կ�汾��:   "+ str;
			break;

		case 0x04:
			str = "4-IC������:   " + str;
			break;

		case 0x05:
			{
				memset(ch_Value, 0x00, sizeof(ch_Value));
				for(int index = 0; index < valuesize; index++)
				{
					sprintf(ch_Value, "%s%02X", ch_Value,Data[TLV_ValueOffset+currentTLVIndex+index]);
				}
			    str =(CString) ch_Value+"\r\n";
				str = "5-������ʶ:   "+str;
				break;
			}

		case 0x06:
             str = "6-�ն��ͺ�:   " + str;
			break;

		case 0x07:
		    str = "7-�ն˷��л���:   " + str;
			break;


		case 0x08:
			memset(ch_Value, 0x00, 1024);
			for(int index = 0; index < valuesize; index++)
			{
				sprintf(ch_Value, "%s%02X", ch_Value,Data[TLV_ValueOffset+currentTLVIndex+index]);
			}
			str =(CString)ch_Value+"\r\n";
			str = "8-Pin֤�����к�:   "+str;
			break;

		case 0x09:
			memset(ch_Value, 0x00, 1024);
			for(int index = 0; index < valuesize; index++)
			{
				sprintf(ch_Value, "%s%02X", ch_Value,Data[TLV_ValueOffset+currentTLVIndex+index]);
			}
			str =(CString) ch_Value+"\r\n";
			str = "9-RA֤������:   " + str;
			break;

		case 0x84:
			memset(ch_Value, 0x00, 1024);
			for(int index = 0; index < valuesize; index++)
			{
				sprintf(ch_Value, "%s%02X", ch_Value,Data[TLV_ValueOffset+currentTLVIndex+index]);
			}
			str =(CString) ch_Value;
			str = " ר���ļ�����:   " + str;
			break;
		default:
			break;
		}
		m_edit.ReplaceSel(str);
		currentTLVIndex = currentTLVIndex + TLV_ValueOffset + valuesize;
	
     }
}

void CMiniPayFactorySetDlg::Parse(TLVEntity *tlvEntity, unsigned int entityLength, unsigned char *buffer, unsigned int& bufferLength)
{
	int currentIndex = 0;
	int currentTLVIndex = 0;
	unsigned long valueSize = 0;

	while (currentTLVIndex < entityLength)
	{
		valueSize = 0;
		TLVEntity entity = tlvEntity[currentTLVIndex];
		
		memcpy(buffer + currentIndex, entity.Tag, entity.TagSize);     // ����Tag
		currentIndex += entity.TagSize;

		for(int index = 0; index < entity.LengthSize; index++)
		{
			valueSize += entity.Length[index]<<(index * 8);        //����length����
		}

	    if(valueSize > 127)
		{
			buffer[currentIndex] = 0x80 | entity.LengthSize;
			currentIndex += 1;
		}

		memcpy(buffer + currentIndex, entity.Length, entity.LengthSize);
		currentIndex += entity.LengthSize;
		//�ж��Ƿ������Ƕ��TLV
		if(entity.Sub_TLVEntity == NULL)
		{
			memcpy(buffer + currentIndex, entity.Value, valueSize);
			currentIndex += valueSize;
		}
		else
		{
			unsigned int oLength;
			Parse(entity.Sub_TLVEntity, 1, buffer + currentIndex, oLength);
			currentIndex += oLength;
		}

		currentTLVIndex++;
	}

	buffer[currentIndex] = 0;
	bufferLength = currentIndex;

}



void CMiniPayFactorySetDlg::Construct(unsigned char *buffer, unsigned int buffferLength, TLVEntity* tlvEntity, unsigned int& entityLength, unsigned int status = 0)
{
   int currentTLVIndex = 0;
   int currentIndex = 0;
   int currentStatus = 'T';  // ״̬�ַ�
   unsigned long valueSize = 0;


while (currentIndex < buffferLength)
{
switch (currentStatus)
{
case 'T':
	valueSize = 0;

   if((status == 1 && buffer[currentIndex] & 0x20) != 0x20)
   {
	   tlvEntity[currentIndex].Sub_TLVEntity = NULL;   //��һ����ʱ����Tag�ÿ�
	   //�ж��Ƿ�Ϊ���ֽ�Tag
	   if((buffer[currentIndex]& 0x1f) == 0x1f)
	   {
		    int endTagIndex = currentIndex;
			while ((buffer[++endTagIndex]& 0x80) == 0x80);  //�жϵڶ����ֽڵ����λ�Ƿ�Ϊ1
            int tagSize = endTagIndex - currentIndex + 1;   //����Tag���������ֽ�

			tlvEntity[currentTLVIndex].Tag = new unsigned char[tagSize];
			memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, tagSize);
			tlvEntity[currentIndex].TagSize = tagSize;
			currentIndex += tagSize;
	   }else
	   {
		   tlvEntity[currentTLVIndex].Tag = new unsigned char[1];
		   memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, 1);
		   tlvEntity[currentTLVIndex].Tag[1] = 0;

		   tlvEntity[currentTLVIndex].TagSize = 1;
		   currentIndex += 1;
	   }
  }
   else
   {
	   // �ж��Ƿ���ֽ�Tag
	   if((buffer[currentIndex] & 0x1f) == 0x1f)
	   {
		   int endTagIndex = currentIndex;
		   while ((buffer[++endTagIndex]& 0x80) == 0x80); //�жϵڶ����ֽڵ����λ�Ƿ�Ϊ1
		   int tagSize = endTagIndex - currentIndex + 1;  //����Tag���������ֽ�
           
		   tlvEntity[currentIndex].Tag = new unsigned char[tagSize];
		   memcpy(TLVEntity[currentTLVIndex].Tag, buffer + currentIndex, tagSize);
		   tlvEntity[currentIndex].Tag[tagSize] = 0;

		   tlvEntity[currentTLVIndex].TagSize = tagSize;
		   currentIndex += tagSize;
	   }
	   else
	   {
		  tlvEntity[currentTLVIndex].Tag = new unsigned char[1];
		  memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, 1);
		  tlvEntity[currentTLVIndex].Tag[1] = 0;
		  tlvEntity[currentTLVIndex].TagSize = 1;
		  currentIndex += 1;
	   }

	   // ����SubTag
	   int subLength = 0;
	   unsigned char *temp;
	   if((buffer[currentIndex] & 0x80) == 0x80)
	   {
		   for(int index = 0; index < 2; index++)
		   {
			   subLength += buffer[currentIndex + 1 + index]<<(index * 8); //����Length��ĳ���
		   }

		   temp = new unsigned char[subLength];
		   memcpy(temp, buffer + currentIndex + 3, subLength);
	   }
	   else
	   {
		   subLength = buffer[currentIndex];
		   temp = new unsigned char[subLength];
		   memcpy(temp, buffer + currentIndex + 1, subLength);
	   }
	   temp[subLength] = 0;
	   unsigned int oLength;
	   tlvEntity[currentTLVIndex].Sub_TLVEntity = new TLVEntity[1];
	   Construct(temp, subLength, tlvEntity[currentTLVIndex].Sub_TLVEntity,, oLength);
   }
   
   currentStatus = 'L';
   break;
case 'L':
	// �жϳ����ֽڵ����λ�Ƿ�Ϊ1,���ΪΪ1������ֽ�Ϊ������չ�ֽڣ�����һ���ֽڿ�ʼ��������
	if((buffer[currentIndex] & 0x80) != 0x80)
	{
       tlvEntity[currentTLVIndex].Length = new unsigned char[1];
	   memcpy(tlvEntity[currentTLVIndex].Length, buffer + currentIndex, 1);
	   tlvEntity[currentTLVIndex].Length[1] = 0;
	   tlvEntity[currentTLVIndex].LengthSize = 1;

	   valueSize = tlvEntity[currentTLVIndex].Length(0);
	   currentIndex += 1;
	}
	else
	{
		//Ϊ1�����
		unsigned int lengthSize = buffer[currentIndex] & 0x7f;
		currentIndex += 1; //����һ���ֽڿ�ʼ��Length��
		for(int index = 0; index < lengthSize; index++)
		{
			valueSize += buffer[currentIndex + index]<<(index * 8); //����Length��ĳ���

		}

		tlvEntity[currentTLVIndex].Length = new unsigned char[lengthSize];
		memcpy(tlvEntity[currentTLVIndex].Length, buffer + currentIndex, lengthSize);
		tlvEntity[currentTLVIndex].Length[lengthSize] = 0;

		tlvEntity[currentTLVIndex].LengthSize = lengthSize;
		currentIndex += lengthSize;
	}
    
	currentStatus = 'V';
	break;

case 'V':
    tlvEntity[currentIndex].Value = new unsigned char[valueSize];
	memcpy(tlvEntity[currentTLVIndex].Value buffer + currentIndex, valueSize);
	tlvEntity[currentTLVIndex].Value[valueSize] = 0;

	currentIndex += valueSize;
	//������һ��TLV����ѭ��
	currentTLVIndex += 1;
	currentStatus = 'T';
	break;
default:
	return;
}

}

  entityLength = currentTLVIndex;
}
