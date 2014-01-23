#include <UtilFunc/PluginInc.h>
#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>

#include "RemoveTool.h"

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(ClsidRemove, CRemove)
XEND_DEFINE_MODULE()

OUTAPI void Go()
{
	Cx_Interface<IPlugin> pPluginAddress(ClsidRemove);
	pPluginAddress->Initialization(NULL);
}


// �иú���ʱ���ڲ������ʱ�ɲ�����������ã������ؼ���ʼ������
OUTAPI bool xPluginOnLoad()
{
	return true;
}

// �иú���ʱ���ڲ��ж��ǰ�ɲ�����������ã������ͷ���Դ����
OUTAPI void xPluginOnUnload()
{
}