#include "CommandTool.h"
#include "Dump.h"
//////////////////////////////////////////////////////////////////////////
// �����ڲ�ͷ�ļ�
#include "CommandObj.h"


//////////////////////////////////////////////////////////////////////////
// ���ÿ��ļ�
#pragma comment ( lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib" )

#ifdef DEBUG
int APIENTRY _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, __in int nShowCmd ) //(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
#else
int main()
{
	LPTSTR lpCmdLine = GetCommandLine();
#endif
	LoadLibrary(_T("System.Security.dll"));
	DeclareDumpFile(); 
	CCommandObj cCommandObj(_T("System.Client"));

	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��ȫ������

	//cCommandObj.InitializationConfig(DEFAULT_CONFIG_FILE_PATH);

	//////////////////////////////////////////////////////////////////////////
	// ���ع���ģ��

	//bRet = cCommandObj.Download(lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	// ִ�й���ģ��

	return cCommandObj.Execute(lpCmdLine);
}



