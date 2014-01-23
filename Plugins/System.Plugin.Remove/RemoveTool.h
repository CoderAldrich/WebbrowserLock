#ifndef SYSTEM_PLUGIN_REMOVE_H
#define SYSTEM_PLUGIN_REMOVE_H


#include "resource.h"

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <tchar.h>
#include <UtilFunc/PluginInc.h>

#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include <Function.Internal.FileControl\\FileControlTool.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>

#include <Function.Internal.RegistryControl\\RegistryControlTool.h>

#include <Function.Internal.ResourceControl\\ResourceControlTool.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

const char* const ClsidRemove = "A93EF1C0-CD72-4CF0-B321-1596CA24F083";

#define DEFAULT_HIJACK_HOME		_T("http://www.baidu.com/?")


//////////////////////////////////////////////////////////////////////////
// API����

//////////////////////////////////////////////////////////////////////////
//  Ĭ�ϻص�����

bool CallbackFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool CallbackFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool DefaultDenyAccess(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);

//////////////////////////////////////////////////////////////////////////
//  ���ӻص�����

bool XyExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool HxyktExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool HxyktExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool For58ExecuteFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool For58ExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool IntelligentFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool IntelligentFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool MzdFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool MzdExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID);
bool YyFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);
bool PubwinExecuteFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID);
bool CultureFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pVoid);


class CRemove
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CRemove)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
    CRemove();
    virtual ~CRemove();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� Initialization()
	//  Return: �ɹ�True,����False

	virtual bool Initialization(PVOID pData);

protected:
	bool FilesWhetherExecutable(LPCTSTR lpFilePath,LPCTSTR lpFileName = NULL);

};

#endif