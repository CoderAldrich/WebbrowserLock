#ifndef SYSTEM_PLUGIN_IELOCK_H
#define SYSTEM_PLUGIN_IELOCK_H


#include "resource.h"

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�
#ifdef _USRDLL
#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <tchar.h>
#include <UtilFunc/PluginInc.h>

#include <Interfaces/PluginsInterface.h>
#endif

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include <Function.Internal.CommonControl/CommonControlTool.h>

#include <Function.Internal.HookControl/HookControlTool.h>

#include <Function.Internal.ShareMemory/ShareMemoryTool.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

const char* const ClsidIELock = "6A901B14-66EB-4C36-996B-5677DF20E077";


//////////////////////////////////////////////////////////////////////////
// API����

typedef  NTSTATUS (WINAPI * PNTDEVICEIOCONTROLFILE) (HANDLE FileHandle,HANDLE Event OPTIONAL,PVOID ApcRoutine OPTIONAL,PVOID ApcContext OPTIONAL,PVOID IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer OPTIONAL,ULONG InputBufferLength,PVOID OutputBuffer OPTIONAL,ULONG OutputBufferLength);


//////////////////////////////////////////////////////////////////////////
//  Ĭ�ϻص�����

bool CallbackFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool CallbackFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool DefaultDenyAccess(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);

//////////////////////////////////////////////////////////////////////////
//  ���ӻص�����

bool HxyktExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool HxyktExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool MzdFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool YyFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);

//////////////////////////////////////////////////////////////////////////
//	BHO
STDAPI DllRegisterServer(void);	// DllRegisterServer - ��ϵͳע���������
STDAPI DllUnregisterServer(void);	// DllUnregisterServer - ��ϵͳע������Ƴ��
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine);	// DllInstall - ���û��ͼ������ϵͳע�������һ���/�Ƴ��

class CIELock
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CIELock)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
    CIELock();
    virtual ~CIELock();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� Initialization()
	//  Return: �ɹ�True,����False

	virtual bool Initialization(PVOID pData);

private:

	static NTSTATUS FakeNtDeviceIoControlFile(HANDLE FileHandle,HANDLE Event OPTIONAL,PVOID ApcRoutine OPTIONAL,PVOID ApcContext OPTIONAL,PVOID IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer OPTIONAL,ULONG InputBufferLength,PVOID OutputBuffer OPTIONAL,ULONG OutputBufferLength);


private:
	static PNTDEVICEIOCONTROLFILE m_pfnNtDeviceIoControlFile;

};

#endif