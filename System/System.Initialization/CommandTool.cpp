#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// �����ڲ�ͷ�ļ�
#include "CommandObj.h"


#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>
//////////////////////////////////////////////////////////////////////////
// ���ÿ��ļ�

#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.XmlControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma  comment( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")




XBEGIN_DEFINE_MODULE()
//	XDEFINE_CLASSMAP_ENTRY(ClsidSystemInfo, CSystemInfo)
XEND_DEFINE_MODULE()


CCommandObj cCommandObj(_T("System.Initialization"));

 EXTERN_C FUNCTION_API BOOL Initialization(HINSTANCE hDllHandle)
 {
	return cCommandObj.Initialization(hDllHandle);
 }

BOOL WINAPI DllMain( HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved )
{
	//////////////////////////////////////////////////////////////////////////
	// ��ʼ������ģ��
	if (DLL_PROCESS_ATTACH == dwReason)
	{
	}

	return TRUE;
}



