#include "CommandTool.h"

//////////////////////////////////////////////////////////////////////////
// �����ڲ�ͷ�ļ�
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// ���ÿ��ļ�
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.XmlControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ShareMemory.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.RegistryControl.lib" )
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.ResourceControl.lib" )


int APIENTRY _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, __in int nShowCmd ) //(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	CCommandObj cCommandObj(_T("System.Client"));

	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��ȫ������

	cCommandObj.InitializationConfig(DEFAULT_CONFIG_FILE_PATH);

	//////////////////////////////////////////////////////////////////////////
	// ���ع���ģ��

	//cCommandObj.Download(lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	// ִ�й���ģ��

	if (cCommandObj.Execute(lpCmdLine))
	{
		while (true)
			Sleep(100000);
	}

	return 0;
}



