#include	"pch.h"
#include	"dialog/cfg_wnd_base.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X
// ================================================================

// ������
bool C_cfg_wnd_base::on_init()
{
	// ������
	on_init_func();

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_cfg_wnd_base::on_close()
{
	on_close_func();
	return true;
}

// �t���[��
void C_cfg_wnd_base::frame()
{
	frame_func();
}
