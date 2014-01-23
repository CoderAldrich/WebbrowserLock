#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.SocketControl\\Include\\Frame\\netserver\\NetServer.h>
#include <Function.Internal.RegistryControl\RegistryControlTool.h>
#include <Function.Internal.FileControl\FileControlTool.h>

#include "CommandServicesObj.h"
#include <shlwapi.h>

#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib")
#pragma comment(lib ,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")

BYTE CCommandServicesObj::m_btCommandBag[MAX_BAG_LEN] = {0x13,0x60,0x05,0x73,0x40};

CCommandServicesObj::CCommandServicesObj(void)
{
	SetIOThreadCount(1);//��������IO�߳�����
	SetWorkThreadCount(255);//���ù����߳���
	Listen(9080);
	if(!m_cRegistryObj.Open(REGISTRY_ROOT))
	{
		Out(Dbg,_T("Resource Services Object Initialization Registry[%s] Failed."),REGISTRY_ROOT);
		ExitProcess(0);
	}
}


CCommandServicesObj::~CCommandServicesObj(void)
{
}

PVOID CCommandServicesObj::Main(PVOID pParam)
{
	while ( IsOk() )
	{
		//ִ��ҵ��
		Sleep( 1000 );
	}
	return NULL;
}


/**
* �������¼��ص�����
* 
* ������ʵ�־�������ҵ����
* 
*/
void CCommandServicesObj::OnConnect(mdk::NetHost& host)
{
	Debug(Dbg,_T( "new client(%d) connect in\n"),host.ID());
}

/**
* ���ӹر��¼����ص�����
* 
* ������ʵ�־���Ͽ�����ҵ����
* 
*/
void CCommandServicesObj::OnCloseConnect(mdk::NetHost &host)
{
	Debug(Dbg,_T("client(%d) close connect\n"),host.ID());
}


bool CCommandServicesObj::FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost)
{
	DebugA(OutputsA,("[CCommandServicesObj]Format Command Info[%s] Start."),lpFileName);
	bool bRet = FALSE;
	CHAR szFileName[MAX_PATH] = {0};
	RESOURCEREQUIT rResourceRequit;
	memcpy_s(rResourceRequit.btResourceBag,MAX_BAG_LEN,m_btCommandBag,MAX_BAG_LEN);


#ifdef UNICODE
	BOOL bUnicode = FALSE;
	WideCharToMultiByte( CP_ACP, 0, lpFileName, _tcslen(lpFileName), szFileName, MAX_PATH,NULL,&bUnicode);  
#else
	_tcscpy_s(szFileName,MAX_PATH,lpFileName);
#endif

	PathRemoveExtensionA(szFileName);
	strcpy_s(rResourceRequit.szResourceID,szFileName);

	DebugA(OutputsA,("[CCommandServicesObj]Send Resource Requit Info:[%s] Len:[%d] Start."),rResourceRequit.szResourceID,sizeof(RESOURCEREQUIT));
	bRet = nClientHost.Send((PUCHAR)&rResourceRequit,sizeof(RESOURCEREQUIT));

	Debugend(bRet,_T("[CCommandServicesObj]Format Command Info Stop[%d]."),bRet);
	return bRet;
}


bool CCommandServicesObj::UnInitCommand(PRESOURCESENDINFO pResourceObj)
{
	return true;
}

bool CCommandServicesObj::InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj)
{
	Out(Dbg,_T("[CCommandServicesObj]Init Command Starting."));
	bool bRet = TRUE;


	// ����CLSID
	if (!stricmp(pCommandRequit->szCommandID,COMMAND_SERVICES_CLSID))
	{
		if(m_cRegistryObj.Read(REGISTRY_DATADIRECTORY,m_szDataDirectory,MAX_PATH))
		{
			Out(Dbg,_T("[CCommandServicesObj]Find Files [%s] Starting."),m_szDataDirectory);
			bool cancel = false;

			PathAddBackslash(m_szDataDirectory);
			PathAppend(m_szDataDirectory,_T("*.cfg"));

			WIN32_FIND_DATA fd;
			HANDLE hfind = ::FindFirstFile(m_szDataDirectory, &fd);
			BOOL loop = (hfind != INVALID_HANDLE_VALUE);

			for (; loop && !cancel; loop = ::FindNextFile(hfind, &fd))
			{
				if (fd.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//Ŀ¼
					Out(Dbg,_T("[CCommandServicesObj]Find the Directory [%s]."),fd.cFileName);
				}
				else
				{
					//�ļ�
					Out(Dbg,_T("[CCommandServicesObj]Find the Files [%s]."),fd.cFileName);
					FormatCommandInfo(fd.cFileName,host);
				}
			}

			::FindClose(hfind);

			Out(Dbg,_T("[CCommandServicesObj]Find Files [%s] Successed."),m_szDataDirectory);
		}
		else
		{
			Out(Dbg,_T("[CCommandServicesObj]Registry Read [%s] Failed."),REGISTRY_DATADIRECTORY);
		}
	}else
	{
		Out(Dbg,_T("[CCommandServicesObj]Init Command Failed."));
		return false;
	}


	Out(Dbg,_T("[CCommandServicesObj]Init Command Successed."));
	return true;
}

void CCommandServicesObj::DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit)
{
	Out(Dbg,_T("Dispatch Command Starting."));

	if (0 == stricmp(pResourceRequit->szCommandID,REQUIT_COMMAND_QUERT_CONFIG))
	{
	}else if (0 == stricmp(pResourceRequit->szCommandID,"{000000-0000-0000-0000-00000000000}"))
	{
	}
	RESOURCESENDINFO rResourceObj;

	if(InitCommand(host,pResourceRequit,&rResourceObj))
		UnInitCommand(&rResourceObj);
	Out(Dbg,_T("Dispatch Command Stoping."));
}

void CCommandServicesObj::OnMsg(mdk::NetHost &host)
{
	/*
	��ȡ���ݳ��ȣ����Ȳ���2byteֱ�ӷ��أ��ȴ��´����ݵ���ʱ�ٶ�ȡ
	ֻ��ȡ2byte��false��ʾ��������ȡ�������ݴӻ�����ɾ�����´λ��ǿ��Զ���
	*/
	COMMANDREQUIT rCommandRequit;

	if ( !host.Recv((PUCHAR)&rCommandRequit, sizeof(COMMANDREQUIT),false)) return;

	if ( !host.Recv((PUCHAR)&rCommandRequit, sizeof(COMMANDREQUIT))) return;//�����Ķ��������ӽ��ջ�����ɾ�������Բ����ܳ��Ȳ�������ʹ�����Ѿ��Ͽ���Ҳ���Զ�������

	OutA(OutputsA,"Command Services Object On Message [%s].",rCommandRequit.szCommandID);

	return DispatchCommand(host,&rCommandRequit);
}
