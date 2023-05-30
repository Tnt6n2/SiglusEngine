#include	"pch.h"
#include	"dialog/cfg_wnd_base_automode.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X�F�I�[�g���[�h
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_base_automode")

// ������
void Ccfg_wnd_base_automode::init()
{
	regist(REGIST_WINDOW_NAME, true, false, LCL_IDD_CFG_WIN_BASE_COMMON);
}

// ������
bool Ccfg_wnd_base_automode::on_init_func()
{
	cfg_wnd_func_automode.on_init_func(this, CFG_WND_FUNC_MOD_BASE, true, cfg_wnd_full_mode_flag, false);
	return true;
}

// ����
bool Ccfg_wnd_base_automode::on_close_func()
{
	cfg_wnd_func_automode.on_close_func();
	return true;
}

// �R�}���h
bool Ccfg_wnd_base_automode::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_automode.on_command_func(wp, lp))		{	return true;	}
	return true;
}

// �ʒm
bool Ccfg_wnd_base_automode::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_automode.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_base_automode::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_automode.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// �g�X�N���[��
bool Ccfg_wnd_base_automode::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_automode.on_h_scroll_func(wp, lp))		{	return true;	}
	return true;
}

// �u�X�N���[��
bool Ccfg_wnd_base_automode::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_automode.on_v_scroll_func(wp, lp))		{	return true;	}
	return true;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_base_automode::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_automode.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// �t���[��
void Ccfg_wnd_base_automode::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	cfg_wnd_func_automode.frame_func();
}

