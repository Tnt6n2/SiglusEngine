#include	"pch.h"
#include	"dialog/sys_wnd_solo_save.h"
#include	"localize/localize.h"

#include	"../resource.h"

// ****************************************************************
// �V�X�e���E�B���h�E�F�\���F�Z�[�u
// ================================================================

#define		REGIST_WINDOW_NAME		_T("sys_wnd_solo_save")

// ������
void C_sys_wnd_solo_save::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_SYS_WIN_SOLO_SAVELOAD);
}

// ������
bool C_sys_wnd_solo_save::on_init_func()
{
	sys_wnd_func_saveload.on_init_func(this, SYS_WND_FUNC_MOD_SOLO_SAVE, false, false, false);
	return true;
}

// ����
bool C_sys_wnd_solo_save::on_close_func()
{
	sys_wnd_func_saveload.on_close_func();
	return true;
}

// �R�}���h
bool C_sys_wnd_solo_save::on_command(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// �ʒm
bool C_sys_wnd_solo_save::on_notify(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// �}�E�X�z�C�[��
bool C_sys_wnd_solo_save::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// �g�X�N���[��
bool C_sys_wnd_solo_save::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �u�X�N���[��
bool C_sys_wnd_solo_save::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_sys_wnd_solo_save::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = sys_wnd_func_saveload.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// �t���[��
void C_sys_wnd_solo_save::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	sys_wnd_func_saveload.frame_func();
}

