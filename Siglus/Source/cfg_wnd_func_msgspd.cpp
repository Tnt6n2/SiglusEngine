#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_msgspd.h"
#include	"dialog/cfg_wnd_common.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�������x
// ================================================================

TSTR C_cfg_wnd_func_msgspd::get_sample_text()
{
	TSTR text;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		text = Gp_ini->config.font_sample_str_short;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		text = Gp_ini->config.font_sample_str_short;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD)
	{
		text = Gp_ini->config.font_sample_str_long;
	}

	return text;
}


// �A�C�e�����\�z
void C_cfg_wnd_func_msgspd::setup_dialog()
{
	int p_x, p_y, width;

	bool font_msgspd = (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD || func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD);

	// �_�C�A���O�^�C�g��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE);
	}

	// �t�H���g
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// �S�p�^�[���ɒu���āA�����͉ςł�
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;
	int base_width, base_height;
	int group_width, group_height, group_align_rep;
	int contents_width, contents_height;

	// �t�H���g�O���[�v�̍���
	int font_group_height = 160;

	if (wide_language_flag)
	{
		font_group_height += 44;
	}

	// �}�[�W��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
		group_align_rep = 0;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T + DLG_CM_SPACE + font_group_height;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
		group_align_rep = 0;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD)
	{
		dialog_pdg_x = DLG_CM_BASE_PDG_X;
		dialog_pdg_t = DLG_CM_BASE_PDG_T + DLG_CM_SPACE + font_group_height;
		dialog_pdg_b = DLG_CM_BASE_PDG_B;
		group_align_rep = 0;
	}

	// ���͌Œ�
	dialog_width = 480;
	base_width = dialog_width - DLG_CM_SOLO_PDG_X * 2;
	group_width = base_width;
	contents_width = group_width - (font_msgspd ? DLG_CM_GROUP_PDG_X * 2 : 0);

	// �x�[�X�ʒu
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;
	int group_x = base_x;
	int group_y = base_y;
	int contents_x = base_x;
	int contents_y = base_y;

	// �O���[�v
	if (font_msgspd)
	{
		ci.grp_msgspd.create(_T(""), _T(""), group_x, group_y, 0, 0, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_msgspd.set_text(LCL_STR_MSGSPD_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_msgspd, font_msgspd ? 2 : 5);
		SendMessage(ci.grp_msgspd.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));		// �O���[�v�̃t�H���g�ݒ�

		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	// �R���e���c��z�u
	p_x = contents_x;
	p_y = contents_y;

	width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_SLOW, font);

	ci.stc_msgspd_slow.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_STC_H, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, 0, false, false, h_wnd);
	ci.stc_msgspd_slow.set_text(LCL_STR_MSGSPD_DIALOG_STC_SLOW);
	p_dlg->add_child(&ci.stc_msgspd_slow, font_msgspd ? 1 : 7);
	SendMessage(ci.stc_msgspd_slow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	
	width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_FAST, font);

	ci.stc_msgspd_fast.create(_T(""), _T(""), contents_x + contents_width - width, p_y, width, DLG_CM_STC_H, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_RIGHT, false, false, h_wnd);
	ci.stc_msgspd_fast.set_text(LCL_STR_MSGSPD_DIALOG_STC_FAST);
	p_dlg->add_child(&ci.stc_msgspd_fast, font_msgspd ? 3 : 9);
	SendMessage(ci.stc_msgspd_fast.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	p_y += DLG_CM_STC_H + DLG_CM_SPACE;

	ci.sld_msgspd.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_SLIDER_HEIGHT, false, WS_CHILD | WS_VISIBLE | TBS_TOP, 0, false, false, h_wnd);
	ci.sld_msgspd.set_slider_range(TNM_MESSAGE_SPEED_MIN, TNM_MESSAGE_SPEED_MAX);
	p_dlg->add_child(&ci.sld_msgspd, font_msgspd ? 2 : 8);
	SendMessage(ci.sld_msgspd.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));			// �O���[�v�̃t�H���g�ݒ�

	p_y += DLG_CM_SLIDER_HEIGHT + DLG_CM_SPACE;

	// �T���v���\��
	ci.stc_msgspd_sample.create(_T(""), _T(""), p_x, p_y, contents_width, 32, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_STATICEDGE, false, false, h_wnd);
	ci.stc_msgspd_sample.set_text(get_sample_text());
	p_dlg->add_child(&ci.stc_msgspd_sample, font_msgspd ? 2 : 8);
	SendMessage(ci.stc_msgspd_sample.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	p_y += 32;

	p_y += DLG_CM_SPACE * 2;

	// �m�[�E�F�C�g
	{
		int width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_NOWAIT, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.chk_msgspd_nowait.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_msgspd_nowait.set_text(LCL_STR_MSGSPD_DIALOG_STC_NOWAIT);
		p_dlg->add_child(&ci.chk_msgspd_nowait, 1);
		SendMessage(ci.chk_msgspd_nowait.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		// �������{�^��
		if (font_msgspd)
		{
			ci.btn_msgspd_init.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_init, 3);
			SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

			p_y += DLG_CM_BTN_INIT_H;
		}
		else if (func_mod == CFG_WND_FUNC_MOD_SOLO)
		{
			ci.btn_msgspd_close.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_close, 3);
			SendMessage(ci.btn_msgspd_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

			ci.btn_msgspd_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_init, 3);
			SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

			p_y += DLG_CM_BTN_CLOSE_H;
		}
	}

	// �R���e���c�̍���������
	contents_height = p_y - contents_y;

	// �O���[�v��
	if (font_msgspd)
	{
		p_y += DLG_CM_GROUP_PDG_B;
	}

	// �O���[�v�̍���������
	group_height = p_y - group_y;

	// �O���[�v�̑傫����ݒ�
	ci.grp_msgspd.set_window_size_with_def_size(group_width, group_height);

	// ����{�^��
	/*
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		ci.btn_msgspd_close.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_msgspd_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_msgspd_close, 3);
		SendMessage(ci.btn_msgspd_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.btn_msgspd_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_msgspd_init, 3);
		SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		p_y += DLG_CM_BTN_CLOSE_H;
	}
	*/


	// �x�[�X�̍���������
	base_height = p_y - base_y;

	// �_�C�A���O�̍���������
	dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;

	// �_�C�A���O���Ĕz�u
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		refresh_dialog_func(dialog_width, dialog_height);
	}
}

// ������
void C_cfg_wnd_func_msgspd::on_init_funcfunc()
{
	work.now_time = 0;
	work.now_text.clear();

	// ��Ɨp�t�H���g������
	init_work_font();

	// �_�C�A���O���\�z
	setup_dialog();

	/*
	// �A�C�e���o�^
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		p_dlg->bind_control(&ci.grp_msgspd, IDC_GRP_MSGSPD, _T(""), 5);

		p_dlg->bind_control(&ci.sld_msgspd, IDC_SLD_MSGSPD, _T(""), 8);
		p_dlg->bind_control(&ci.stc_msgspd_sample, IDC_STC_MSGSPD_SAMPLE, _T(""), 8);
		p_dlg->bind_control(&ci.stc_msgspd_slow, IDC_STC_MSGSPD_SLOW, _T(""), 7);
		p_dlg->bind_control(&ci.stc_msgspd_fast, IDC_STC_MSGSPD_FAST, _T(""), 9);
		p_dlg->bind_control(&ci.chk_msgspd_nowait, IDC_CHK_MSGSPD_NOWAIT, _T(""), 7);
		p_dlg->bind_control(&ci.btn_msgspd_init, IDC_BTN_MSGSPD_INIT, _T(""), 3);
		p_dlg->bind_control(&ci.btn_msgspd_close, IDC_BTN_MSGSPD_CLOSE, _T(""), 3);
	}
	else	{
		p_dlg->bind_control(&ci.grp_msgspd, IDC_GRP_MSGSPD, _T(""), 2);

		p_dlg->bind_control(&ci.sld_msgspd, IDC_SLD_MSGSPD, _T(""), 2);
		p_dlg->bind_control(&ci.stc_msgspd_sample, IDC_STC_MSGSPD_SAMPLE, _T(""), 2);
		p_dlg->bind_control(&ci.stc_msgspd_slow, IDC_STC_MSGSPD_SLOW, _T(""), 1);
		p_dlg->bind_control(&ci.stc_msgspd_fast, IDC_STC_MSGSPD_FAST, _T(""), 3);
		p_dlg->bind_control(&ci.chk_msgspd_nowait, IDC_CHK_MSGSPD_NOWAIT, _T(""), 1);
		p_dlg->bind_control(&ci.btn_msgspd_init, IDC_BTN_MSGSPD_INIT, _T(""), 3);
		p_dlg->bind_control(&ci.btn_msgspd_close, IDC_BTN_MSGSPD_CLOSE, _T(""), 3);
	}

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_msgspd_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_msgspd.set_slider_range(TNM_MESSAGE_SPEED_MIN, TNM_MESSAGE_SPEED_MAX);
	*/

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void C_cfg_wnd_func_msgspd::on_close_funcfunc()
{
	// ��Ɨp�t�H���g�J��
	free_work_font();
}

// �R�}���h
bool C_cfg_wnd_func_msgspd::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.chk_msgspd_nowait.get_handle())
	{
		Gp_config->message_speed_nowait = ci.chk_msgspd_nowait.get_check();
		return true;
	}
	else if ((HWND)lp == ci.btn_msgspd_init.get_handle())
	{
		init_state();
		return true;
	}
	else if ((HWND)lp == ci.btn_msgspd_close.get_handle())
	{
		p_dlg->close();
		return true;
	}

	/*
	switch (LOWORD(wp))	{
		case IDC_CHK_MSGSPD_NOWAIT:
			Gp_config->message_speed_nowait = ci.chk_msgspd_nowait.get_check();
			return true;

		case IDC_BTN_MSGSPD_INIT:
			init_state();
			return true;

		case IDC_BTN_MSGSPD_CLOSE:
			p_dlg->close();
			return true;
	}
	*/

	return false;
}

// �ʒm
bool C_cfg_wnd_func_msgspd::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_func_msgspd::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool C_cfg_wnd_func_msgspd::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_msgspd.get_handle())
	{
		Gp_config->message_speed = TNM_MESSAGE_SPEED_MAX - ci.sld_msgspd.get_slider_pos();
		return true;
	}
	return false;
}

// �u�X�N���[��
bool C_cfg_wnd_func_msgspd::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_cfg_wnd_func_msgspd::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if (ci.stc_msgspd_sample.get_handle() == (HWND)lp)
	{
		SetBkMode((HDC)wp, TRANSPARENT);	// �w�i�F���[�h�𓧖��ɂ���
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	return NULL;
}

// �t���[��
void C_cfg_wnd_func_msgspd::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// ��Ɨp�t�H���g�ύX
	if (work_font.now_font_name != Gp_config->font_name)	{
		load_work_font(Gp_config->font_name);
		work.now_text.clear();
	}

	// �T���v���e�L�X�g�X�V
	TSTR smp_text = get_sample_text();
	if (smp_text.empty())
	{
		ci.stc_msgspd_sample.set_text(_T(""));		// �T���v���e�L�X�g�������̂ŕ���������
	}
	else
	{
		TSTR disp_text;
		if (Gp_config->message_speed_nowait || Gp_config->message_speed <= 0)
		{
			disp_text = smp_text;
		}
		else
		{
			int total_time = ((int)smp_text.size() * Gp_config->message_speed) + 500;		// 500 = �҂�����
			work.now_time = (work.now_time + frame_time_past) % total_time;
			int disp_cnt = (work.now_time / Gp_config->message_speed) + 1;
			disp_text = smp_text.substr(0, disp_cnt);
		}

		// �\�����镶�����ς�������̂ݍĕ`�悷��
		if (work.now_text != disp_text)
		{
			ci.stc_msgspd_sample.set_text(disp_text);
			work.now_text = disp_text;
		}
	}

	// �O���ŕύX���ꂽ
	if (Gp_config->message_speed != TNM_MESSAGE_SPEED_MAX - ci.sld_msgspd.get_slider_pos())
	{
		ci.sld_msgspd.set_slider_pos(TNM_MESSAGE_SPEED_MAX - Gp_config->message_speed);
	}
	if (Gp_config->message_speed_nowait != ci.chk_msgspd_nowait.get_check())
	{
		ci.chk_msgspd_nowait.set_check(Gp_config->message_speed_nowait);
	}
}

// ������Ԃɖ߂�
void C_cfg_wnd_func_msgspd::init_state()
{
	tnm_init_config_message_speed();
	update_dialog();
}

// �_�C�A���O�X�V
void C_cfg_wnd_func_msgspd::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_msgspd.set_slider_pos(TNM_MESSAGE_SPEED_MAX - Gp_config->message_speed);
	ci.chk_msgspd_nowait.set_check(Gp_config->message_speed_nowait);
}

// ****************************************************************
// ��Ɨp�t�H���g
// ================================================================

// ������
void C_cfg_wnd_func_msgspd::init_work_font()
{
	work_font.original_font = NULL;
	work_font.now_font = NULL;
	work_font.now_font_name.clear();
}

// �J��
void C_cfg_wnd_func_msgspd::free_work_font()
{
	// ��Ɨp�t�H���g�J��
	if (work_font.now_font)	{
		DeleteObject(work_font.now_font);
		work_font.now_font = NULL;
	}
	work_font.now_font_name.clear();
}

// �ǂݍ���
void C_cfg_wnd_func_msgspd::load_work_font(CTSTR& font_name)
{
	work_font.now_font_name = font_name;

	work_font.now_font = CreateFont(
			24, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, work_font.now_font_name.c_str());
	HDC hdc = GetDC(ci.stc_msgspd_sample.get_handle());
	HFONT kill_font = (HFONT)SelectObject(hdc, work_font.now_font);
	if (work_font.original_font)	{	DeleteObject(kill_font);				}
	else							{	work_font.original_font = kill_font;	}	// �ŏ��̃I���W�i���t�H���g�͊J�����Ȃ�
	ReleaseDC(ci.stc_msgspd_sample.get_handle(), hdc);
	SendMessage(ci.stc_msgspd_sample.get_handle(), WM_SETFONT, (WPARAM)work_font.now_font, (LPARAM)MAKELPARAM(TRUE, 0));
}

