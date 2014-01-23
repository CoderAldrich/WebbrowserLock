#ifndef SYSTEM_PLUGIN_SYSTEMINFO_H
#define SYSTEM_PLUGIN_SYSTEMINFO_H

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�


#include <Function.Internal.CommonControl/CommonControlTool.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

const char* const ClsidSystemInfo = "88D7346A-3A9A-4DF6-BD88-F0DD3610D3CE";

#define MAX_BUFFER_INFO_SIZE							1024


//////////////////////////////////////////////////////////////////////////
// API����
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

class CSystemInfo
	: public IPlugin, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CSystemInfo)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
	CSystemInfo();
	virtual ~CSystemInfo();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� InitlizationRemove()
	//  Return: �ɹ�True,����False

	virtual bool Initialization(PVOID pData);
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain: ͨ����������ȡ����ID GetProcessIDForProcessName(LPTSTR lpProcessName)
	//  Parameter: lpProcessName ������
	//  Return: �ɹ����ؽ���Id��ʧ�ܷ���0

	static DWORD GetProcessIDForProcessName(LPCTSTR lpProcessName);
private:
	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��

	void CheckOperatingSystemVersion(__out LPTSTR lpSystemVersionBuffer,size_t iBufferLen) const;

	void CheckSystemInfo(__out LPTSTR lpSystemInfoBuffer,size_t iBufferLen) const;

	void CheckCPUInfo(__out LPTSTR lpCPUInfoBuffer,size_t iBufferLen) const;

	void CheckMemoryInfo(__out LPTSTR lpSystemVersionBuffer,size_t iBufferLen) const;

	void CheckDriveInfo(__out LPTSTR lpDriveInfoBuffer,size_t iBufferLen) const;

	void CheckNetworkInfo(__out LPTSTR lpNetworkInfoBuffer,size_t iBufferLen) const;

	void CheckEnvironment(__out LPTSTR lpEnvironmentBuffer,size_t iBufferLen) const;

	void CheckIEVersion(__out LPTSTR lpIEVersionBuffer,size_t iBufferLen) const;
	void CheckDisplayCardInfo(__out LPTSTR lpIEVersionBuffer,size_t iBufferLen) const;

	//////////////////////////////////////////////////////////////////////////
	//

	BOOL GetProcessPath(IN DWORD dwProcessID,IN LPWSTR lpszBuffer,IN int nSize,IN BOOL bFullPath) const;


private:
	LPTSTR m_lpInfoBuffer;
};

#endif