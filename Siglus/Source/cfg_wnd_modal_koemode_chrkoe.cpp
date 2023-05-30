#include	"pch.h"
#include	"dialog/cfg_wnd_modal_koemode_chrkoe.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X�F�����i�������[�h�A�L�����N�^�[�����j
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_modal_koemode_chrkoe")

// �I�[�v�����[�_���t�@���N�V����
void Ccfg_wnd_modal_koemode_chrkoe::open_modal_func(HWND parent_h_wnd)
{
	create(LCL_IDD_CFG_WIN_SOLO_COMMON, REGIST_WINDOW_NAME, true, parent_h_wnd);
}

// ������
bool Ccfg_wnd_modal_koemode_chrkoe::on_init_func()
{
	cfg_wnd_func_koemode.on_init_func(this, CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE, false, false, true);
	cfg_wnd_func_chrkoe.on_init_func(this, CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE, false, false, true);
	return true;
}

// ����
bool Ccfg_wnd_modal_koemode_chrkoe::on_close_func()
{
	cfg_wnd_func_koemode.on_close_func();
	cfg_wnd_func_chrkoe.on_close_func();
	return true;
}

// �R�}���h
bool Ccfg_wnd_modal_koemode_chrkoe::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_koemode.on_command_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_chrkoe.on_command_func(wp, lp))		{	return true;	}
	return true;
}

// �^�C�}�[
bool Ccfg_wnd_modal_koemode_chrkoe::on_timer_func(WPARAM wp, LPARAM lp)
{
	cfg_wnd_func_koemode.frame_func();
	cfg_wnd_func_chrkoe.frame_func();
	return true;
}

// �ʒm
bool Ccfg_wnd_modal_koemode_chrkoe::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_koemode.on_notify_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_chrkoe.on_notify_func(wp, lp))		{	return true;	}
	return true;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_modal_koemode_chrkoe::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_koemode.on_mousewheel_func(wp, lp))	{	return false;	}
	if (cfg_wnd_func_chrkoe.on_mousewheel_func(wp, lp))		{	return false;	}
	return true;
}

// �g�X�N���[��
bool Ccfg_wnd_modal_koemode_chrkoe::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_koemode.on_h_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_chrkoe.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �u�X�N���[��
bool Ccfg_wnd_modal_koemode_chrkoe::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_koemode.on_v_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_chrkoe.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_modal_koemode_chrkoe::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_koemode.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	if (res = cfg_wnd_func_chrkoe.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

