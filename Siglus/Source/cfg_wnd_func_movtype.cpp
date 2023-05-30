#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_movtype.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F���[�r�[�̍Đ����@
// ================================================================

// ������
void Ccfg_wnd_func_movtype::on_init_funcfunc()
{
	support_return_value = true;

	// �A�C�e���o�^
	p_dlg->bind_control(&ci.stc_movtype_mess00, IDC_STC_MOVTYPE_MESS00, _T(""), 7);
	p_dlg->bind_control(&ci.stc_movtype_mess01, IDC_STC_MOVTYPE_MESS01, _T(""), 7);
	p_dlg->bind_control(&ci.rad_movtype_mci, IDC_RAD_MOVTYPE_MCI, _T(""), 7);
	p_dlg->bind_control(&ci.rad_movtype_wmp, IDC_RAD_MOVTYPE_WMP, _T(""), 7);
	p_dlg->bind_control(&ci.btn_movtype_close, IDC_BTN_MOVTYPE_CLOSE, _T(""), 3);
	p_dlg->bind_control(&ci.btn_movtype_deside, IDC_BTN_MOVTYPE_DESIDE, _T(""), 3);
	p_dlg->bind_control(&ci.btn_movtype_cancel, IDC_BTN_MOVTYPE_CANCEL, _T(""), 3);

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_movtype_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void Ccfg_wnd_func_movtype::on_close_funcfunc()
{
}

// �R�}���h
bool Ccfg_wnd_func_movtype::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_RAD_MOVTYPE_MCI:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_MCI;
			return true;

		case IDC_RAD_MOVTYPE_WMP:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
			return true;

//		case IDC_BTN_MOVTYPE_INIT:
//			init_state();
//			return true;

		case IDC_BTN_MOVTYPE_CLOSE:
			p_dlg->close();
			return true;

		case IDC_BTN_MOVTYPE_DESIDE:
			p_dlg->close();
			return true;

		case IDC_BTN_MOVTYPE_CANCEL:
			support_return_value = false;
			p_dlg->close();
			return true;
	}
	return false;
}

// �ʒm
bool Ccfg_wnd_func_movtype::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_func_movtype::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool Ccfg_wnd_func_movtype::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �u�X�N���[��
bool Ccfg_wnd_func_movtype::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_func_movtype::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void Ccfg_wnd_func_movtype::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// �O���ŕύX���ꂽ
	if (Gp_system_config->movie_type != get_radio_movtype())	{
		set_radio_movtype();
	}
}

// ������Ԃɖ߂�
void Ccfg_wnd_func_movtype::init_state()
{
	update_dialog();
}

// �_�C�A���O�X�V
void Ccfg_wnd_func_movtype::update_dialog()
{
	if (!h_wnd)	{	return;	}
	set_radio_movtype();
}

// ****************************************************************
// ���[�r�[�̍Đ����@�̃��W�I�{�^����ݒ�
// ================================================================
void Ccfg_wnd_func_movtype::set_radio_movtype()
{
	if (!h_wnd)	{	return;	}
	ci.rad_movtype_mci.set_check(false);
	ci.rad_movtype_wmp.set_check(false);
	switch (Gp_system_config->movie_type)	{
		case TNM_MOVIE_PLAY_TYPE_MCI:	ci.rad_movtype_mci.set_check(true);	break;
		case TNM_MOVIE_PLAY_TYPE_WMP:	ci.rad_movtype_wmp.set_check(true);	break;
	}
}

// ****************************************************************
// ���[�r�[�̍Đ����@�̃��W�I�{�^�����擾
// ================================================================
int Ccfg_wnd_func_movtype::get_radio_movtype()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_movtype_mci.get_check())			{	return TNM_MOVIE_PLAY_TYPE_MCI;	}
	else if (ci.rad_movtype_wmp.get_check())	{	return TNM_MOVIE_PLAY_TYPE_WMP;	}
	return -1;
}


