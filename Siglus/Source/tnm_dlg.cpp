#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dlg.h"
#include	"data/tnm_icon.h"

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
void C_tnm_dlg::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	C_dialog::window_proc_pre(msg, wp, lp);

	switch (msg)	{

		case WM_INITDIALOG:
			// �A�C�R����ݒ�
			set_icon(Gp_icon->get_large_icon());
			set_small_icon(Gp_icon->get_small_icon());
			break;
	}
}

LRESULT C_tnm_dlg::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// ���W�I�{�^���̃t�H�[�J�X���O���ׂ̏���
		case WM_ACTIVATE:
			if (LOWORD(wp) == WA_INACTIVE)	{
				kill_focus_for_radio_item();
			}
			break;
	}
	return C_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// ���W�I�{�^������t�H�[�J�X���O��
// ================================================================
bool C_tnm_dlg::kill_focus_for_radio_item()
{
	HWND focus_h_wnd = GetFocus();
	CHILDITR end = get_child_list_end();
	C_control *ctrl;
	DWORD style;
	for (CHILDITR itr = get_child_list_begin(); itr != end; ++itr)	{
		ctrl = (C_control *)(*itr);
		if (focus_h_wnd == ctrl->get_handle())	{
			if (ctrl->get_class() == WC_BUTTON)	{
				style = (DWORD)ctrl->get_style() & BS_TYPEMASK;
				if (style == BS_RADIOBUTTON || style == BS_AUTORADIOBUTTON)	{
					send_message(WM_NEXTDLGCTL, (WPARAM)m_h_wnd, TRUE);		// m_h_wnd �̓_�C�A���O�̃n���h���Ȃ̂ŁA�{���n���ׂ��̓R���g���[���̃n���h���ł��B
					return true;
				}
			}
		}
	}
	return false;
}

// ****************************************************************
// �o�^
// ================================================================
void C_tnm_dlg::regist(CTSTR window_name, bool limit_min, bool attach, int dialog_id)
{
	regist_state.window_name = window_name;
	regist_state.limit_min = limit_min;
	regist_state.attach = attach;
	regist_state.dialog_id = dialog_id;
}

// ****************************************************************
// �����I�[�v������
// ================================================================
bool C_tnm_dlg::initial_open_check()
{
	if (regist_state.window_name.empty())											{	return false;	}
	if (C_ini_file::body().load_bool(_T("open"), false, regist_state.window_name))	{	return true;	}
	return false;
}

// ****************************************************************
// �����I�[�v��
// ================================================================
//		�A�v���P�[�V�����N�����ɌĂ΂��
//		�O��I�����ɊJ���Ă����ꍇ�͊J��
// ================================================================
void C_tnm_dlg::initial_open()
{
	if (!initial_open_check())	{	return;	}
	open();
}

// ****************************************************************
// �J��Ԃ� ini �ɕۑ�
// ================================================================
void C_tnm_dlg::save_ini_open_state()
{
	if (regist_state.window_name.empty())	{	return;	}
	C_ini_file::body().save_bool(_T("open"), m_h_wnd ? true : false, regist_state.window_name);
}

// ****************************************************************
// ****************************************************************
// ****************************************************************
// �ȉ��́A���@�[�`�����֐�
// ================================================================
// ================================================================
// ================================================================

// ****************************************************************
// ���
// ================================================================
//		�A�v���P�[�V�����I�����ɌĂ΂��
//		�J��Ԃ�ۑ����ă_�C�A���O�����
// ================================================================
void C_tnm_dlg::free()
{
	save_ini_open_state();	// �J��Ԃ� ini �ɕۑ����Ă���
	close();				// �_�C�A���O�����
}

// ****************************************************************
// �J���i�쐬�j
// ================================================================
void C_tnm_dlg::open(HWND parent_h_wnd)
{
	if (m_h_wnd)					{	return;	}
	if (regist_state.dialog_id < 0)	{	return;	}

	// �_�C�A���O���쐬����
	create(regist_state.dialog_id, regist_state.window_name, regist_state.limit_min, regist_state.attach, parent_h_wnd);
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_tnm_dlg::load_state()
{
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_tnm_dlg::save_state()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_tnm_dlg::frame()
{
}

