#ifndef _SHAREMEMORYAPI_H
#define _SHAREMEMORYAPI_H

//���������ڴ���API���ṩ��ƽ̨֧��

#ifdef _WIN32
#define SM_INVALID_HANDLE NULL 
#endif

#ifdef  _LINUX
#define SM_INVALID_HANDLE -1 
#endif

#define MAX_SHAREMEMORY_64  64

//�����windows���������
#ifdef _WIN32
#define INIT_HANDLE 0xFFFFFFFFFFFFFFFF
typedef HANDLE SMHANDLE;
typedef PHANDLE PSMHANDLE;
typedef int    SMKEY;
#endif

//�����linux���������
#ifdef  _LINUX
typedef int   SMHandle;
typedef key_t SMKey;	
#endif

	//����һ��ShareMemory����ָ��
	BOOL CreateShareMemory(PSMHANDLE pHandle,SMKEY Key, rsize_t nSize);
	//��һ���Ѿ����ڵ�ShareMemory����ָ��
	BOOL OpenShareMemory(PSMHANDLE pHandle,SMKEY Key,rsize_t nSize = 1);
	//�õ�ָ����ShareMemory����ָ��ӳ��
	BOOL MapShareMemory(PVOID pShareMemory,SMHANDLE handle);
	//�ر�ָ����ShareMemory����ָ��ӳ��
	BOOL UnShareMemory(PVOID pShareMemory);
	//ע��һ��ShareMemory����ָ��
	BOOL CloseShareMemory(SMHANDLE handle);

#endif