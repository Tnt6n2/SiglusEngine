#pragma		once
#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_mod.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X
// ================================================================
class C_cfg_wnd_base : public C_cfg_wnd_solo
{
public:
	void		frame();				// �t���[��

private:

	bool		on_init();				// ������
	bool		on_close();				// ����
};

