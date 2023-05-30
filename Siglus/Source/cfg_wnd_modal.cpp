#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_icon.h"
#include	"dialog/cfg_wnd_modal.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F���[�_��
// ================================================================

// �E�B���h�E�v���V�[�W��
LRESULT C_cfg_wnd_modal::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_TIMER:			if (!on_timer(wp, lp))		return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
		case WM_MOUSEWHEEL:		if (!on_mousewheel(wp, lp))	return FALSE;	break;
		case WM_HSCROLL:		if (!on_h_scroll(wp, lp))	return FALSE;	break;
		case WM_VSCROLL:		if (!on_v_scroll(wp, lp))	return FALSE;	break;
		case WM_CTLCOLORSTATIC:
			{
				LRESULT res;
				if (res = on_ctlcolorstatic(wp, lp))	{	return res;	}
			}
			break;
	}
	return C_modal_dialog::window_proc(msg, wp, lp);
}

// ������
bool C_cfg_wnd_modal::on_init()
{
	// �A�C�R����ݒ�
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	// �T�C�Y�O���b�v
	statusbar.create_sizegrip(m_h_wnd);

	// ������
	on_init_func();

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();

	// �^�C�}�[�J�n
	SetTimer(m_h_wnd, CFG_WND_MODAL_TIMER_ID, CFG_WND_MODAL_TIMER_TIME, NULL);

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_cfg_wnd_modal::on_close()
{
	on_close_func();

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �^�C�}�[
bool C_cfg_wnd_modal::on_timer(WPARAM wp, LPARAM lp)
{
	on_timer_func(wp, lp);
	if (!is_key_down(VK_LBUTTON))	{
		rep_window_pos_outside_desktop();
	}
	return true;
}

// ****************************************************************
// �R���t�B�O�E�B���h�E�F���[�_���F�I�[�v�����[�_��
// ================================================================
void C_cfg_wnd_modal::open_modal(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	open_modal_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
}

bool C_cfg_wnd_modal::open_modal_result_bool(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	bool res = open_modal_result_bool_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
	return res;
}

int C_cfg_wnd_modal::open_modal_result_int(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	int res = open_modal_result_int_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
	return res;
}

// ****************************************************************
// �R���t�B�O�E�B���h�E�F���[�_���F�I�[�v�����[�_���t�@���N�V�����i���z�֐��j
// ================================================================
void C_cfg_wnd_modal::open_modal_func(HWND parent_h_wnd)
{
	// NOP
}

bool C_cfg_wnd_modal::open_modal_result_bool_func(HWND parent_h_wnd)
{
	// NOP
	return false;
}

int C_cfg_wnd_modal::open_modal_result_int_func(HWND parent_h_wnd)
{
	// NOP
	return -1;
}

