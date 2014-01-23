#pragma once

//////////////////////////////////////////////////////////////////////////
// ���ñ����ļ�

#include <Windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
// ���ñ�����ļ�

#pragma comment(lib,"shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

//////////////////////////////////////////////////////////////////////////
// �������ݽṹ

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�
#include <Function.Internal.CommonControl/CommonControlTool.h>
#include "..\\Function.Internal.Registry\Registry.h"
#include "..\\Function.Internal.FileControl\FileControlTool.h"
#include "..\\Function.Internal.Socket\Include\Frame\netserver\NetServer.h"

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include "..\\Common\\Socket\\Client\\ResourceClientObj.h"

//////////////////////////////////////////////////////////////////////////
// �������

// �����з�����

EXTERN_C FUNCTION_API void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);

EXTERN_C FUNCTION_API bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);

EXTERN_C FUNCTION_API bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

EXTERN_C FUNCTION_API bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);

// ��Դ������

EXTERN_C FUNCTION_API void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);

EXTERN_C FUNCTION_API bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

EXTERN_C FUNCTION_API bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

EXTERN_C FUNCTION_API void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);
