#pragma once

#include <Aclapi.h>


//
// ����ĳ���˻���ĳ���ļ�(��)�����в���Ȩ��
// pszPath: �ļ�(��)·��
// pszAccount: �˻�����
// AccessPermissions��Ȩ������
// ͨ�ö���GENERIC_READ��
// ͨ��д��GENERIC_WRITE��
// ͨ��ִ�У�GENERIC_EXECUTE��
// ͨ�����У�GENERIC_ALL��
// ע�������(KEY_ALL_ACCESS)
// ע���ִ�У�KEY_EXECUTE��
// ע���д��KEY_WRITE��
// ע������KEY_READ��
//
BOOL  EnableAccountPrivilege (LPCTSTR pszPath, LPCTSTR pszAccount ,DWORD AccessPermissions = GENERIC_READ | GENERIC_EXECUTE ,ACCESS_MODE AccessMode = DENY_ACCESS ,SE_OBJECT_TYPE dwType = SE_FILE_OBJECT);
