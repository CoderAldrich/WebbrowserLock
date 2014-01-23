#ifndef SYSTEM_PLUGIN_SECURITY_H
#define SYSTEM_PLUGIN_SECURITY_H

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include <Function.Internal.RegistryControl/RegistryControlTool.h>
#include <Function.Internal.FileControl/FileControlTool.h>
#include <Function.Internal.ShareMemory/ShareMemoryTool.h>
#include <Function.Internal.CommonControl/CommonControlTool.h>

const char* const ClsidSecurity = "72306C77-84AF-4627-B2CD-1901546344BD";

//////////////////////////////////////////////////////////////////////////
// �궨��

#define PLUGIN_SECURITY_ALL							0xFFFFFFFF
#define PLUGIN_SECURITY_BHO							0x00000001
#define PLUGIN_SECURITY_ARP							0x00000002
#define PLUGIN_SECURITY_TIME						0x00000004

//////////////////////////////////////////////////////////////////////////
// API����
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);



class CSecurity
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CSecurity)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
    CSecurity();
    virtual ~CSecurity();

private:
#define ADDR_ANY                INADDR_ANY

public:
	typedef enum  _ENUM_ARP_TYPE
	{
		Other = 1,
		Invalid,
		Dynamic,
		Static,
	}ENUM_ARP_TYPE, * PENUM_ARP_TYPE;

	static LPCWSTR ENUM_ARP_TYPE_STRING[];

	//////////////////////////////////////////////////////////////////////////
	// Security Data

	// BHO
	typedef struct _DATA_BHO
	{
		WCHAR CLSID[MAX_CLSID_LEN+1];
	}DATA_BHO, * PDATA_BHO;
	typedef struct _DATA_SECURITY_BHO
	{
		bool Type;
		ULONG ArraySize;
		ULONG ArrayOffset;
		PDATA_BHO BHO;
	}DATA_SECURITY_BHO, * PDATA_SECURITY_BHO;

	// ARP
	typedef struct _DATA_ARP
	{
		ULONG Sleep;
		WCHAR BindIP[MAX_IP4_ADDRESS_LEN];
		WCHAR BindMAC[MAX_MAC_ADDRESS_LEN];
		ENUM_ARP_TYPE Type;
	}DATA_ARP, * PDATA_ARP;

	typedef struct _DATA_SECURITY_ARP
	{
		ULONG ArraySize;
		PDATA_ARP ARP;
		_DATA_SECURITY_ARP()
		{
			ArraySize = 0;
			ARP = NULL;
		}
	}DATA_SECURITY_ARP, * PDATA_SECURITY_ARP;

	typedef struct _DATA_SECURITY_TIME
	{
		LONGLONG Time;
		_DATA_SECURITY_TIME()
		{
			Time = 0;
		}
	}DATA_SECURITY_TIME, * PDATA_SECURITY_TIME;

	typedef struct _DATA_SECURITY
	{
		LONG Switch;
		DATA_SECURITY_ARP ARP;
		DATA_SECURITY_BHO BHO;
		DATA_SECURITY_TIME TIME;
		_DATA_SECURITY()
		{
			Switch = 0;
		}
	}DATA_SECURITY, * PDATA_SECURITY;

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� InitlizationRemove()
	//  Return: �ɹ�True,����False

	virtual bool Initialization(PVOID pData);



	static CSecurity::ENUM_ARP_TYPE GetArpType(const char * szType,CSecurity::ENUM_ARP_TYPE DefType = CSecurity::Static)
	{
		if (NULL == szType)
		{
			return  DefType;
		}
		switch (szType[0])
		{
		case 'S':
			return  CSecurity::Static;
		case 's':
			return  CSecurity::Static;
		case 'D':
			return  CSecurity::Dynamic;
		case 'd':
			return  CSecurity::Dynamic;
		case 'I':
			return  CSecurity::Invalid;
		case 'i':
			return  CSecurity::Invalid;
		case 'O':
			return  CSecurity::Other;
		case 'o':
			return  CSecurity::Other;
		default:
			return DefType;
		}
		return DefType;
	}

	static bool GetBhoListType(const char * szType,bool DefType = true)
	{
		switch (szType[0])
		{
		case  0 :
			return  DefType;
		case 'U':
			return  true;
		case 'u':
			return  true;
		case 'T':
			return  false;
		case 't':
			return  false;
		default:
			return DefType;
		}
		return DefType;
	}
private:
	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ͬ��ϵͳʱ��
	//	Parameter:
	//		lTime	��Ҫ�޸ĵ�ʱ���Ӧ��ʱ���
	//	Return: �ɹ�����True,ʧ�ܷ���False;

	bool SynchronizeSystemTime(DATA_SECURITY_TIME datTimeData,bool bInstall = true);
	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ��ARP
	//	Parameter:
	//		lTime	��Ҫ�޸ĵ�ʱ���Ӧ��ʱ���
	//	Return: �ɹ�����True,ʧ�ܷ���False;

	bool ArpTatleBinding(DATA_SECURITY_ARP dsaArpData,bool bInstall = true);
	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ��ARP
	//	Parameter:
	//		lTime	��Ҫ�޸ĵ�ʱ���Ӧ��ʱ���
	//	Return: �ɹ�����True,ʧ�ܷ���False;

	bool SecurityMonitorForIE(DATA_SECURITY_BHO dsaArpData,bool bInstall = true);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ��Ӿ�̬ARP��
	//	Parameter:
	//		pszBindIP	��Ҫ�󶨵�IP��ַ
	//		pszBindMAC	��Ҫ�󶨵�MAC��ַ
	//		uType		���ΰ󶨵����(1 = ������Ŀ,2 = ��Ч��Ŀ,3 = ��̬��Ŀ,4 = ��̬��Ŀ)
	//	Return: �ɹ�����True,ʧ�ܷ���False;

	static bool CreateArpTatle(LPCTSTR pszBindIP,LPCTSTR pszBindMAC,ENUM_ARP_TYPE uType = Dynamic);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ARP��ִ���߳�
	//	Parameter:
	//		pszBindIP	��Ҫ�󶨵�IP��ַ

	static DWORD WINAPI ExecuteArpBinding(PDATA_SECURITY_ARP pArpData);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain: ��ÿ���
	//	Patameter:
	//		uFlags	��Ŀ
	//	Return:�򿪷���True,�رշ���False.

	bool GetSecuritySwitch(ULONG uFlags);

private:
	static PLUGIN_INIT_DATA m_PluginInitData;
	CSecurity::DATA_SECURITY m_SecurityData;
	CShareMemory<CSecurity::DATA_SECURITY_BHO> m_ShareMemoryBHO;

protected:
	CFileControlTool m_cFileControlTool;
};

#endif