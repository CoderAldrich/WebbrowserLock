
#include "SystemControl.h"
#include "HttpObj.h"
#include "SystemInfoCollect.h"
//#include <iptypes.h>
#include <iphlpapi.h>


CSystemInfoCollect::CSystemInfoCollect(void)
{
}


CSystemInfoCollect::~CSystemInfoCollect(void)
{
}


bool CSystemInfoCollect::GetComputerMACAddress(__out char * lpMACAddressBuffer)
{
	Out(Dbg,_T("Get Computer MAC Address Starting."));
	bool bRet = false;
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{

		bRet = true;
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		//while (pIpAdapterInfo)
		{
			Out(Outputs,_T("*****************************************************"));
			OutA(OutputsA,("�������ƣ�%s"),pIpAdapterInfo->AdapterName);
			OutA(OutputsA,"����������%s",pIpAdapterInfo->Description);
			sprintf_s(lpMACAddressBuffer,MAX_MAC_ADDRESS_LEN,"%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x", pIpAdapterInfo->Address[0],pIpAdapterInfo->Address[1],pIpAdapterInfo->Address[2],pIpAdapterInfo->Address[3],pIpAdapterInfo->Address[4],pIpAdapterInfo->Address[5],pIpAdapterInfo->Address[6],pIpAdapterInfo->Address[7]);
			OutA(OutputsA,"����MAC��ַ��%s",lpMACAddressBuffer);
			Out(Dbg,_T("����IP��ַ���£�"));
			//���������ж�IP,���ͨ��ѭ��ȥ�ж�
			IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
			do 
			{
				OutA(OutputsA,pIpAddrString->IpAddress.String);
				pIpAddrString=pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
			Out(Dbg,_T("*****************************************************"));
		}
	}
	else
	{
		Out(Dbg,_T("Get Computer MAC Address Failed."));
		Show();
		bRet = false;
	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		//delete pIpAdapterInfo;
	}
	Out(Dbg,_T("Get Computer MAC Address Successed."));
	return true;
}

bool CSystemInfoCollect::GetComputerIPAddress(__out char * lpIPAddressBuffer)
{
	Out(Dbg,_T("Get Computer IP Address Starting."));
	GETIPPAGEFORMATINFO ipFormatInfo[4];
	strcpy_s(ipFormatInfo[0].PageURL , "http://pv.sohu.com/cityjson");
	strcpy_s(ipFormatInfo[1].PageURL , "http://ip.3322.net/");
	strcpy_s(ipFormatInfo[2].PageURL , "http://apps.game.qq.com/comm-htdocs/ip/get_ip.php");

	strcpy_s(ipFormatInfo[0].PageHand , "\"cip\": \"");
	strcpy_s(ipFormatInfo[1].PageHand , "");
	strcpy_s(ipFormatInfo[2].PageHand , "\"ip_address\":\"");

	strcpy_s(ipFormatInfo[0].PageEnd , "\", \"cid\"");
	strcpy_s(ipFormatInfo[1].PageEnd , "");
	strcpy_s(ipFormatInfo[2].PageEnd , "\"}");
	for (int i = 0;i < 3;i++)
	{
		lpIPAddressBuffer[0] = '\0';
		LPCSTR lpIpPage = m_cInternetDownObj.GetPage(ipFormatInfo[i].PageURL);
		if (NULL==lpIpPage)
		{
			Out(Dbg,_T("Get Computer IP Address Failed."));
			Show();
			continue;
		}
		else
		{
			LPCSTR lpIpPageHand = strstr(lpIpPage,ipFormatInfo[i].PageHand);
			if (NULL == lpIpPageHand)
			{
				Out(Dbg,_T("Get Computer IP Address Failed."));
				Show();
				continue;
			}
			lpIpPageHand += strlen(ipFormatInfo[i].PageHand);
			LPCSTR lpIpPageEnd = strstr(lpIpPage,ipFormatInfo[i].PageEnd);

			if (lpIpPage == lpIpPageEnd)
			{
				strncpy_s(lpIPAddressBuffer,MAX_PATH,lpIpPageHand,strlen(lpIpPage));
				OutA(OutputsA,lpIPAddressBuffer);
				break;
			}
			strncpy_s(lpIPAddressBuffer,MAX_IP4_ADDRESS_LEN,lpIpPageHand,lpIpPageEnd-lpIpPageHand);

			OutA(OutputsA,lpIPAddressBuffer);
			break;
		}
	}
	Out(Dbg,_T("Get Computer IP Address Successed."));
	return true;
}