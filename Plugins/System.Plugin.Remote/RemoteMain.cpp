#include <UtilFunc/PluginInc.h>
#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>

#include "RemoteTool.h"

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(ClsidRemote, CRemote)
XEND_DEFINE_MODULE()

// �иú���ʱ���ڲ������ʱ�ɲ�����������ã������ؼ���ʼ������
OUTAPI bool xPluginOnLoad()
{
	return true;
}

// �иú���ʱ���ڲ��ж��ǰ�ɲ�����������ã������ͷ���Դ����
OUTAPI void xPluginOnUnload()
{
}