
// MiniPayFactorySetDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define CLA   0
#define INS   1
#define P1    2
#define P2    3
#define Lc    4
#define Le    4
#define DataAddr 5


#define CLA_SYSTME_CMD_NONE                0xFF
#define INS_SYSTME_CMD                     0x70
#define P1_SYSTEM_CMD                      0x25
#define P2_SYSTEM_CMD                      0x7B
#define LC_SYSTEM_CMD                      0x00




#define Data_CLA                            0xFE
#define Data_INS_SetTerminalInfo            0x11
#define Data_P1_MiniPay                     0x02 
#define Data_P1_DEV                         0x00
#define Data_P2                             0x00
#define Data_Lc                             0x00
#define Data_Offset                         0x0A



#define  P2_TerminalStatus                  0x00
#define  P2_TerminalID                      0x01
#define  P2_FirmwareVersion                 0x02
#define  P2_PublicKeyVersion                0x03
#define  P2_CardPAN                         0x04
#define  P2_ReverseFlag                     0x05
#define  P2_Label                           0x06
#define  P2_Issuer                          0x07
#define  P2_PinCertSerialNumber             0x08
#define  P2_RACertType                      0x09


#define  DEV_P2_Version                     0x00
#define  DEV_P2_Manufacturer                0x01
#define  DEV_P2_Issuer                      0x02
#define  DEV_P2_Label                       0x03
#define  DEV_P2_SerialNumber                0x04
#define  DEV_P2_HWVersion                   0x05
#define  DEV_P2_FirmVersion                 0x06
#define  DEV_P2_AlgSymCap                   0x07
#define  DEV_P2_AlgAsymCap                  0x08
#define  DEV_P2_AlgHashCap                  0x09
#define  DEV_P2_DevAuthAlgId                0x0A
#define  DEV_P2_TotalSpace                  0x0B
#define  DEV_P2_FreeSpace                   0x0C
#define  DEV_P2_FactorySetting              0x0D
#define  DEV_P2_ProductionDate              0x0E
#define  DEV_P2_TerminalState               0x0F
#define  DEV_P2_TerminalVersion             0x10
#define  DEV_P2_BootLoaderVersion           0x12


#define  max_len                            50   //������õ���Ϣ��
#define  TLV_ValueOffset                    2   

typedef  struct _readlist
{
	char strReadName[128];  // �ն���Ϣ����
	int  nReadID;           // ���
}READ_LIST, *READ_LIST_PTR;


typedef struct  Struct_MiniPayInfo
{
	BYTE TerminalStatus;
	CHAR TerminalID[23]; 
	CHAR FirmwareVersion[11];
	CHAR PublicKeyVersion[2];
	CHAR CardPAN[19];
	BYTE ReverseFlag;
	CHAR Label[16];
	CHAR Issuer[8];
	BYTE PinCertSerialNumber[32];
	BYTE RACertType;
}MiniPayInfo, *PMinPayInfo;

typedef struct Struct_IniFileType
{
	CHAR strApp[128];
	CHAR strKey1[10];
	CHAR strKey2[10];
	BOOL bSelected;
}IniFileType;


typedef struct Struct_Version
{
	BYTE major;  //���汾��
	BYTE minor;  //�ΰ汾��

}VERSION;

typedef struct Struct_Manufacturer
{
	VERSION     Version;
	CHAR		Manufacturer[64];
	CHAR		Issuer[64];
	CHAR		Label[32];
	CHAR		SerialNumber[32];
	VERSION		HWVersion;
	VERSION		FirmwareVersion;
	BYTE		AlgSymCap[4];
	BYTE		AlgAsymCap[4];
	BYTE		AlgHashCap[4];
	BYTE		DevAuthAlgId[4];
	BYTE		TotalSpace[4];
	BYTE		FreeSpace[4];
	BYTE		FactotySetting;
	BYTE		ProductionDate[4];
	BYTE        TerminalState;
	VERSION		TerminalVersion;
	VERSION		BootLoadVersion;

}DEVINFO_Ver_1_0, *PDEVINFO_Ver_1_0;

//TLV�ṹ��
typedef struct Struct_TLVEntity{
	unsigned char *Tag;    //���
	unsigned char *Length; //���ݳ���
	unsigned char *Value;  //����
	unsigned int TagSize;  //���ռ���ֽ���
	unsigned  int LengthSize;  //���ݳ���
	TLVEntity* Sub_TLVEntity;  //��Ƕ��TLVʵ��
}TLVEntity, *PTLVEntity;






// CMiniPayFactorySetDlg �Ի���
class CMiniPayFactorySetDlg : public CDialogEx
{
// ����
public:
	CMiniPayFactorySetDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CStringArray m_strUnionMinipay;
	CStringArray m_strDev;
	BYTE ucDevName[20];


// �Ի�������
	enum { IDD = IDD_MINIPAYFACTORYSET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	void GetIniUnionInfo();
	void GetIniDevInfo();
	BOOL SetMiniPay();
	BOOL ReadMiniPayInfo();
	BOOL SetManufactureInfo();
	BOOL ConnectDevice();
	void ParseTLVData(unsigned char* Data, unsigned int len);
	void Parse(TLVEntity *tlvEntity, unsigned int entityLength, unsigned char *buffer, unsigned int& bufferLength);
	void Construct(unsigned char *buffer, unsigned int buffferLength, TLVEntity* tlvEntity, unsigned int& entityLength, unsigned int status = 0);


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strID;
	afx_msg void OnBnClickedButton1();
	CComboBox m_READLIST;
	CEdit m_edit;
};
