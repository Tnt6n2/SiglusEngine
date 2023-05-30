#include	"pch.h"
#include	"dialog/cfg_wnd_solo_font.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�\���F�t�H���g
// ================================================================

#define		REGIST_WINDOW_NAME				_T("cfg_wnd_solo_font")

// ������
void C_cfg_wnd_solo_font::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_SOLO_COMMON);
}

// ������
bool C_cfg_wnd_solo_font::on_init_func()
{
	cfg_wnd_func_font.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, false);
	return true;
}

// ����
bool C_cfg_wnd_solo_font::on_close_func()
{
	cfg_wnd_func_font.on_close_func();
	return true;
}

// �R�}���h
bool C_cfg_wnd_solo_font::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// �ʒm
bool C_cfg_wnd_solo_font::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_solo_font::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// �g�X�N���[��
bool C_cfg_wnd_solo_font::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_h_scroll_func(wp, lp))		{	return true;	}
	return true;
}

// �u�X�N���[��
bool C_cfg_wnd_solo_font::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_v_scroll_func(wp, lp))		{	return true;	}
	return true;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_cfg_wnd_solo_font::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_font.on_ctlcolorstatic_func(wp, lp))		{	return res;	}
	return NULL;
}

// �t���[��
void C_cfg_wnd_solo_font::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	cfg_wnd_func_font.frame_func();
}

