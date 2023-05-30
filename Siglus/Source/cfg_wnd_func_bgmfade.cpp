#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_syscom.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_bgmfade.h"
#include	"dialog/cfg_wnd_base_volume_bgmfade.h"
#include	"dialog/cfg_wnd_solo_volume_bgmfade.h"
#include	"dialog/cfg_wnd_modal_volume_bgmfade.h"
#include	"dialog/cfg_wnd_common.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�a�f�l�t�F�[�h�O���[�v�̉������擾
// ================================================================

int Gf_get_bgmfade_group_width()
{
	return 120;
}

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�a�f�l�t�F�[�h
// ================================================================

// �A�C�e�����\�z
void Ccfg_wnd_func_bgmfade::setup_dialog()
{
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;

	int group_width, group_height, contents_width, contents_height;

	int explain_width, explain_height;
	int slider_x, slider_y, slider_width, slider_height;

	int p_x = 0, p_y = 0;

	// �_�C�A���O�^�C�g��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE);
	}

	// �t�H���g
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// �P�̃_�C�A���O�͂���������B
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;

		// �_�C�A���O�̕��̓{�^��������T�C�Y
		dialog_width = DLG_CM_SOLO_PDG_X + DLG_CM_BTN_CLOSE_W + DLG_CM_SPACE + DLG_CM_BTN_INIT_W + DLG_CM_SOLO_PDG_X;

		// �_�C�A���O�̍����͌Œ�
		dialog_height = 270;

		// �O���[�v�̑傫�����v�Z
		group_width = dialog_width - DLG_CM_SOLO_PDG_X * 2;
		group_height = dialog_height - (DLG_CM_SOLO_PDG_T + DLG_CM_SOLO_PDG_B);

		// �R���e���c�̑傫�����v�Z
		contents_width = group_width;
		contents_height = group_height;

		// ����
		explain_width = contents_width;
#if defined(__SIGLUS_ID)
		explain_height = 36;		// �C���h�l�V�A��݂̂P�s�T�C�Y�𑝂₷
#else
		explain_height = 24;
#endif

		// �X���C�_�[
		slider_width = DLG_CM_SLIDER_WIDTH;
		slider_height = 128;
		slider_x = (contents_width - slider_width) / 2;
		slider_y = explain_height + DLG_CM_SPACE;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// �O���[�v�̑傫���͌Œ�
		group_width = Gf_get_bgmfade_group_width();
		group_height = 240;

		// �{�����[���O���[�v�̑傫���������Ă���
		int volume_group_width = Gf_get_volume_group_width();
		int volume_group_height = 240;

		// �_�C�A���O�̑傫���̓{�����[���_�C�A���O���Q�l��
		if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
		{
			dialog_pdg_x = DLG_CM_SOLO_PDG_X + volume_group_width + DLG_CM_SPACE * 2;
			dialog_pdg_t = DLG_CM_SOLO_PDG_T;
			dialog_pdg_b = DLG_CM_SOLO_PDG_B;
			dialog_width = DLG_CM_SOLO_PDG_X + volume_group_width + DLG_CM_SPACE * 2 + group_width + DLG_CM_SOLO_PDG_X;
			dialog_height = DLG_CM_SOLO_PDG_T + volume_group_height + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_SOLO_PDG_B;
		}
		else if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
		{
			dialog_pdg_x = DLG_CM_BASE_PDG_X + volume_group_width + DLG_CM_SPACE * 2;
			dialog_pdg_t = DLG_CM_BASE_PDG_T;
			dialog_pdg_b = DLG_CM_BASE_PDG_B;
			dialog_width = DLG_CM_BASE_PDG_X + volume_group_width + DLG_CM_SPACE * 2 + group_width + DLG_CM_BASE_PDG_X;
			dialog_height = DLG_CM_BASE_PDG_T + volume_group_height + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_BASE_PDG_B;
		}

		// �R���e���c�̑傫�����v�Z
		contents_width = group_width - DLG_CM_GROUP_PDG_X * 2;
		contents_height = group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_GROUP_PDG_B);

		// ����
		explain_width = contents_width;
#if defined(__SIGLUS_ID)
		explain_height = 60;		// �C���h�l�V�A��݂̂P�s�T�C�Y�𑝂₷
#else
		explain_height = 48;
#endif

		// �X���C�_�[
		slider_width = DLG_CM_SLIDER_WIDTH;
		slider_height = contents_height - (explain_height + DLG_CM_SPACE + DLG_CM_CHKBOX_H + DLG_CM_SPACE) - (DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
		slider_x = (contents_width - slider_width) / 2;
		slider_y = explain_height + DLG_CM_SPACE;

		if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
		{
			slider_height -= DLG_CM_SPACE + DLG_CM_BTN_H;
		}
	}

	// �L���^����
	int enable_width = DLG_CM_CHKBOX_ADD_WIDTH + font_get_width(p_dlg->get_handle(), LCL_STR_BGMFADE_DIALOG_STC_ENABLE, font);
	int enable_height = DLG_CM_CHKBOX_H;
	int enable_x = (contents_width - enable_width) / 2;
	int enable_y = slider_y + slider_height + DLG_CM_SPACE;

	// ����{�^���Ə����ݒ�ɖ߂��{�^��
	int close_btn_x = contents_width - DLG_CM_BTN_CLOSE_W;
	int close_btn_y = contents_height - DLG_CM_BTN_CLOSE_H;
	int init_btn_x = close_btn_x - DLG_CM_SPACE - DLG_CM_BTN_INIT_W;
	int init_btn_y = contents_height - DLG_CM_BTN_INIT_H;

	if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// ���ʁE�a�f�l�t�F�[�h�̓����͓���z�u�ɂȂ�܂��B
		init_btn_x = (contents_width - DLG_CM_BTN_INIT_W) / 2;
		init_btn_y = contents_height - DLG_CM_BTN_INIT_H - DLG_CM_GROUP_PDG_B;
	}

	// �R���g���[����z�u
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.grp_bgmfade.create(_T(""), _T(""), base_x, base_y, group_width, group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_bgmfade, 4);
		SendMessage(ci.grp_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		base_x += DLG_CM_GROUP_PDG_X;
		base_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	ci.stc_bgmfade.create(_T(""), _T(""), base_x, base_y, explain_width, explain_height, false, WS_CHILD | WS_VISIBLE, 0, false, false, h_wnd);
	ci.stc_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE);
	p_dlg->add_child(&ci.stc_bgmfade, 7);
	SendMessage(ci.stc_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	ci.sld_bgmfade.create(_T(""), _T(""), base_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
	ci.sld_bgmfade.set_slider_range(0, 255);
	p_dlg->add_child(&ci.sld_bgmfade, 4);
	SendMessage(ci.sld_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	ci.chk_bgmfade.create(_T(""), _T(""), base_x + enable_x, base_y + enable_y, enable_width, enable_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_STC_ENABLE);
	p_dlg->add_child(&ci.chk_bgmfade, 1);
	SendMessage(ci.chk_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	// ���ʂ̃t�b�^�{�^��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		ci.btn_bgmfade_close.create(_T(""), _T(""), base_x + close_btn_x, base_y + close_btn_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_close, 3);
		SendMessage(ci.btn_bgmfade_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.btn_bgmfade_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_init, 3);
		SendMessage(ci.btn_bgmfade_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.btn_bgmfade_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_init, 1);
		SendMessage(ci.btn_bgmfade_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}

	// �_�C�A���O���Ĕz�u
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		refresh_dialog_func(dialog_width, dialog_height);
	}
}

// ������
void Ccfg_wnd_func_bgmfade::on_init_funcfunc()
{
	// �_�C�A���O���\�z
	setup_dialog();

	/*
	// �A�C�e���o�^
	p_dlg->bind_control(&ci.grp_bgmfade, IDC_GRP_BGMFADE, _T(""), 4);

	p_dlg->bind_control(&ci.stc_bgmfade, IDC_STC_BGMFADE, _T(""), 7);
	p_dlg->bind_control(&ci.sld_bgmfade, IDC_SLD_BGMFADE, _T(""), 4);
	p_dlg->bind_control(&ci.chk_bgmfade, IDC_CHK_BGMFADE, _T(""), 1);

	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		p_dlg->bind_control(&ci.btn_bgmfade_init, IDC_BTN_BGMFADE_INIT, _T(""), 3);
	}
	else	{
		p_dlg->bind_control(&ci.btn_bgmfade_init, IDC_BTN_BGMFADE_INIT, _T(""), 1);
	}

	p_dlg->bind_control(&ci.btn_bgmfade_close, IDC_BTN_BGMFADE_CLOSE, _T(""), 3);

	// ���W����
	int x_rep = 0;
	if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)	{
		x_rep = ((Ccfg_wnd_base_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.bgmfade_item_x_rep;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)	{
		x_rep = ((Ccfg_wnd_solo_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.bgmfade_item_x_rep;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_MODAL_VOLUME_BGMFADE)	{
		x_rep = ((Ccfg_wnd_modal_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.bgmfade_item_x_rep;
	}
	ci.grp_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// �O���[�v
	ci.stc_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// ������
	ci.sld_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// �X���C�_�[
	ci.chk_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// �`�F�b�N�{�b�N�X
	ci.btn_bgmfade_init.add_rltv_pos_with_def_rect(-x_rep, 0);	// �������{�^��

	// �A�C�e������
//	if (false)	{
//		ci.grp_bgmfade.set_show(false);			// �O���[�v
//		ci.stc_bgmfade.set_show(false);			// ������
//		ci.sld_bgmfade.set_show(false);			// �X���C�_�[
//		ci.chk_bgmfade.set_show(false);			// �`�F�b�N�{�b�N�X
//		ci.btn_bgmfade_init.set_show(false);	// �������{�^��
//
//		// �E�B���h�E�T�C�Y�����i���ʂ̊֐����Ăяo���܂��j
//		if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)	{			// �a�f�l�t�F�[�h�F�A�C�e���������̃E�B���h�E�T�C�Y�����i���ʂ̊֐����Ăяo���܂��j
//			int xx_rep = ci.grp_bgmfade.get_window_rect().width() + 10;		// + 10 = ������
//			((Ccfg_wnd_solo_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.rep_window_size_func(-xx_rep);
//		}
//		else if (func_mod == CFG_WND_FUNC_MOD_MODAL_VOLUME_BGMFADE)	{		// �a�f�l�t�F�[�h�F�A�C�e���������̃E�B���h�E�T�C�Y�����i���ʂ̊֐����Ăяo���܂��j
//			int xx_rep = ci.grp_bgmfade.get_window_rect().width() + 10;		// + 10 = ������
//			((Ccfg_wnd_modal_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.rep_window_size_func(-xx_rep);
//		}
//	}

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_bgmfade_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_bgmfade.set_slider_range(0, 255);
	*/

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void Ccfg_wnd_func_bgmfade::on_close_funcfunc()
{
}

// �R�}���h
bool Ccfg_wnd_func_bgmfade::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.btn_bgmfade_init.get_handle())
	{
		init_state();
		return true;
	}
	else if ((HWND)lp == ci.btn_bgmfade_close.get_handle())
	{
		p_dlg->close();
		return true;
	}
	else if ((HWND)lp == ci.chk_bgmfade.get_handle())
	{
		Gp_config->bgmfade_use_check = ci.chk_bgmfade.get_check();
		return true;
	}

	return false;
}

// �ʒm
bool Ccfg_wnd_func_bgmfade::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_func_bgmfade::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool Ccfg_wnd_func_bgmfade::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �u�X�N���[��
bool Ccfg_wnd_func_bgmfade::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_bgmfade.get_handle())	{
		Gp_config->bgmfade_volume = 255 - ci.sld_bgmfade.get_slider_pos();
		return true;
	}
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_func_bgmfade::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	return NULL;
}

// �t���[��
void Ccfg_wnd_func_bgmfade::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// �O���ŕύX���ꂽ
	if (Gp_config->bgmfade_volume != (255 - ci.sld_bgmfade.get_slider_pos()))	{
		ci.sld_bgmfade.set_slider_pos(255 - Gp_config->bgmfade_volume);
	}
	if (Gp_config->bgmfade_use_check != ci.chk_bgmfade.get_check())	{
		ci.chk_bgmfade.set_check(Gp_config->bgmfade_use_check);
	}
}

// ������Ԃɖ߂�
void Ccfg_wnd_func_bgmfade::init_state()
{
	tnm_init_config_bgmfade();
	update_dialog();
}

// �_�C�A���O�X�V
void Ccfg_wnd_func_bgmfade::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_bgmfade.set_slider_pos(255 - Gp_config->bgmfade_volume);
	ci.chk_bgmfade.set_check(Gp_config->bgmfade_use_check);
}

