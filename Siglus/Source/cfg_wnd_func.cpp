#include	"pch.h"

#include	"dialog/cfg_wnd_func.h"

#include	"../resource.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V����
// ================================================================

// ������
void C_cfg_wnd_func::on_init_func(C_dialog* p_dlg_, int func_mod_, bool is_base_mode, bool is_full_mode, bool is_modal_mode)
{
	p_dlg = p_dlg_;
	h_wnd = p_dlg->get_handle();
	func_mod = func_mod_;
	base_mode_flag = is_base_mode;
	full_mode_flag = is_full_mode;
	modal_mode_flag = is_modal_mode;
	frame_start_time = ::timeGetTime();
	frame_time_past = 0;

	// ����␳
#if defined(__SIGLUS_JA) || defined(__SIGLUS_ZH) || defined(__SIGLUS_ZHTW)
	wide_language_flag = false;
#else
	wide_language_flag = true;
#endif

	// �t�����X�A�C���h�l�V�A��̏ꍇ�͉����̐ݒ�E�B���h�E�����C�h������
#if defined(__SIGLUS_FR) || defined(__SIGLUS_ID)
	wide_koe_wnd_flag = true;
#else
	wide_koe_wnd_flag = false;
#endif

	on_init_funcfunc();
}

// ����
void C_cfg_wnd_func::on_close_func()
{
	on_close_funcfunc();
	p_dlg = NULL;
	h_wnd = NULL;
}

// �R�}���h
bool C_cfg_wnd_func::on_command_func(WPARAM wp, LPARAM lp)
{
	return on_command_funcfunc(wp, lp);
}

// �ʒm
bool C_cfg_wnd_func::on_notify_func(WPARAM wp, LPARAM lp)
{
	return on_notify_funcfunc(wp, lp);
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_func::on_mousewheel_func(WPARAM wp, LPARAM lp)
{
	return on_mousewheel_funcfunc(wp, lp);
}

// �g�X�N���[��
bool C_cfg_wnd_func::on_h_scroll_func(WPARAM wp, LPARAM lp)
{
	return on_h_scroll_funcfunc(wp, lp);
}

// �u�X�N���[��
bool C_cfg_wnd_func::on_v_scroll_func(WPARAM wp, LPARAM lp)
{
	return on_v_scroll_funcfunc(wp, lp);
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_cfg_wnd_func::on_ctlcolorstatic_func(WPARAM wp, LPARAM lp)
{
	return on_ctlcolorstatic_funcfunc(wp, lp);
}

// �t���[��
void C_cfg_wnd_func::frame_func()
{
	if (!h_wnd)	{	return;	}
	int now_time = ::timeGetTime();
	frame_time_past = now_time - frame_start_time;
	frame_start_time = now_time;
	frame_funcfunc();
}

void C_cfg_wnd_func::refresh_dialog_func(int dialog_width, int dialog_height)
{
	p_dlg->set_def_client_rect(C_rect(0, 0, dialog_width, dialog_height));

	if (!base_mode_flag)
	{
		C_rect rect(0, 0, dialog_width, dialog_height);
		::AdjustWindowRectEx(&rect, p_dlg->get_style(), FALSE, p_dlg->get_style_ex());
		p_dlg->set_def_window_rect(rect);
		p_dlg->set_window_size(rect.size());
		p_dlg->set_pos_my_desktop_center();
	}
}

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F���z�֐�
// ================================================================

// ������
void C_cfg_wnd_func::on_init_funcfunc()
{
	// NOP
}

// ����
void C_cfg_wnd_func::on_close_funcfunc()
{
	// NOP
}

// �R�}���h
bool C_cfg_wnd_func::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// �ʒm
bool C_cfg_wnd_func::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_func::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// �g�X�N���[��
bool C_cfg_wnd_func::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// �u�X�N���[��
bool C_cfg_wnd_func::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_cfg_wnd_func::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	return NULL;
}

// �t���[��
void C_cfg_wnd_func::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}
}
