#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_mwndbk.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�E�B���h�E�w�i�F
// ================================================================

// ������
void Ccfg_wnd_func_mwndbk::on_init_funcfunc()
{
	// �A�C�e���o�^
	p_dlg->bind_control(&ci.grp_mwndbk, IDC_GRP_MWNDBK, _T(""), 5);

	p_dlg->bind_control(&ci.stc_mwndbk_r, IDC_STC_MWNDBK_R, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_r, IDC_SLD_MWNDBK_R, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_g, IDC_STC_MWNDBK_G, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_g, IDC_SLD_MWNDBK_G, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_b, IDC_STC_MWNDBK_B, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_b, IDC_SLD_MWNDBK_B, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_tr, IDC_STC_MWNDBK_TR, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_tr, IDC_SLD_MWNDBK_TR, _T(""), 8);
	p_dlg->bind_control(&ci.btn_mwndbk_init, IDC_BTN_MWNDBK_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.btn_mwndbk_close, IDC_BTN_MWNDBK_CLOSE, _T(""), 3);

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_mwndbk_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_mwndbk_r.set_slider_range(0, 255);
	ci.sld_mwndbk_g.set_slider_range(0, 255);
	ci.sld_mwndbk_b.set_slider_range(0, 255);
	ci.sld_mwndbk_tr.set_slider_range(0, 255);

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void Ccfg_wnd_func_mwndbk::on_close_funcfunc()
{
}

// �R�}���h
bool Ccfg_wnd_func_mwndbk::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_MWNDBK_INIT:
			init_state();
			return true;

		case IDC_BTN_MWNDBK_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// �ʒm
bool Ccfg_wnd_func_mwndbk::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_func_mwndbk::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool Ccfg_wnd_func_mwndbk::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_mwndbk_r.get_handle())	{
		Gp_config->filter_color.r = ci.sld_mwndbk_r.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_g.get_handle())	{
		Gp_config->filter_color.g = ci.sld_mwndbk_g.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_b.get_handle())	{
		Gp_config->filter_color.b = ci.sld_mwndbk_b.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_tr.get_handle())	{
		Gp_config->filter_color.a = 255 - ci.sld_mwndbk_tr.get_slider_pos();
		return true;
	}
	return false;
}

// �u�X�N���[��
bool Ccfg_wnd_func_mwndbk::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_func_mwndbk::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void Ccfg_wnd_func_mwndbk::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// �O���ŕύX���ꂽ
	if (Gp_config->filter_color.r != ci.sld_mwndbk_r.get_slider_pos())	{
		ci.sld_mwndbk_r.set_slider_pos(Gp_config->filter_color.r);
	}
	if (Gp_config->filter_color.g != ci.sld_mwndbk_g.get_slider_pos())	{
		ci.sld_mwndbk_g.set_slider_pos(Gp_config->filter_color.g);
	}
	if (Gp_config->filter_color.b != ci.sld_mwndbk_b.get_slider_pos())	{
		ci.sld_mwndbk_b.set_slider_pos(Gp_config->filter_color.b);
	}
	if (Gp_config->filter_color.a != 255 - ci.sld_mwndbk_tr.get_slider_pos())	{
		ci.sld_mwndbk_tr.set_slider_pos(255 - Gp_config->filter_color.a);
	}
}

// ������Ԃɖ߂�
void Ccfg_wnd_func_mwndbk::init_state()
{
	tnm_init_config_filter();
	update_dialog();
}

// �_�C�A���O�X�V
void Ccfg_wnd_func_mwndbk::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_mwndbk_r.set_slider_pos(Gp_config->filter_color.r);
	ci.sld_mwndbk_g.set_slider_pos(Gp_config->filter_color.g);
	ci.sld_mwndbk_b.set_slider_pos(Gp_config->filter_color.b);
	ci.sld_mwndbk_tr.set_slider_pos(255 - Gp_config->filter_color.a);
}

