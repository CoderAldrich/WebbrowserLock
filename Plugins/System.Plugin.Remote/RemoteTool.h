#ifndef SYSTEM_PLUGIN_REMOTE_H
#define SYSTEM_PLUGIN_REMOTE_H

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.ShareMemory/ShareMemoty.h>
#include <Function.Internal.FileControl/FileControlTool.h>
#include <Function.Internal.SystemControl/SystemControlTool.h>

const char* const ClsidRemote = "4C425EF0-3732-4C51-B2F4-32D7CE3E6F33";


//////////////////////////////////////////////////////////////////////////
// API����
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

class CRemote
    : public IPlugin, CCommonControl
{
    X3BEGIN_CLASS_DECLARE(CRemote)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
    X3END_CLASS_DECLARE()
	//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
    CRemote();
    virtual ~CRemote();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� InitlizationRemove()
	//  Return: �ɹ�True,����False

	virtual bool (Initialization)(PVOID pData);
private:
	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��

	void SetAccessRights();

private:
	CFileControlTool m_cFileControlTool;
	CSystemControlTool m_cSystemControlTool;
};

#endif