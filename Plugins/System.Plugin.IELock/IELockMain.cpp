#include "IELockTool.h"

#include <UtilFunc/PluginInc.h>
#include <Module/XModuleMacro.h>
#include <Module/XModuleImpl.h>

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(ClsidIELock, CIELock)
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