#pragma once

//////////////////////////////////////////////////////////////////////////
// ���ñ����ļ�

#include <Windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
// ���ñ�����ļ�
#pragma comment(lib,"shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

#define MAX_MAC_ADDRESS_LEN										25
#define MAX_IP4_ADDRESS_LEN										16
#define MAX_IP6_ADDRESS_LEN										32

#define DEFAULT_OBJECT_FILE_TEMP_EXTENSION						".Down"
#define DEFAULT_OBJECT_FILE_SAVE_EXTENSION						".exe"

#define DEFAULT_CONFIG_FILE_TEMP_EXTENSION						".Load"
#define DEFAULT_CONFIG_FILE_SAVE_EXTENSION						".Cfg"

//////////////////////////////////////////////////////////////////////////
// �������ݽṹ

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include "..\\System.Common\CommonTool.h"

#include "..\Function.Internal.Registry\Registry.h"
//#pragma comment(lib,"..\\Debug\\Function.Internal.Registry.lib")


#include "..\\Function.Internal.FileControl\\FileControlTool.h"
//#pragma comment(lib,"..\\Debug\\Function.Internal.FileControl.lib")

#include "..\\Function.Internal.ZipControl\\ZipControl.h"

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include "HttpObj.h"
#include "PluginManagerObj.h"


EXTERN_C FUNCTION_API STATUS Initialization(LPCSTR lpHost,LPCSTR lpUserID,LPCSTR lpDirectory,LPCSTR lpServerKey);