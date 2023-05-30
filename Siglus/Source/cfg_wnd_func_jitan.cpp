#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_jitan.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F���Z�Đ�
// ================================================================

// ������
void Ccfg_wnd_func_jitan::on_init_funcfunc()
{
	// �A�C�e���o�^
	p_dlg->bind_control(&ci.grp_jitan, IDC_GRP_JITAN, _T(""), 5);

	p_dlg->bind_control(&ci.stc_jitan_normal, IDC_STC_JITAN_NORMAL, _T(""), 7);
	p_dlg->bind_control(&ci.stc_jitan_2bai, IDC_STC_JITAN_2BAI, _T(""), 8);
	p_dlg->bind_control(&ci.stc_jitan_3bai, IDC_STC_JITAN_3BAI, _T(""), 9);
	p_dlg->bind_control(&ci.sld_jitan, IDC_SLD_JITAN, _T(""), 8);
	p_dlg->bind_control(&ci.chk_jitan_normal, IDC_CHK_JITAN_NORMAL, _T(""), 7);
	p_dlg->bind_control(&ci.chk_jitan_automode, IDC_CHK_JITAN_AUTOMODE, _T(""), 7);
	p_dlg->bind_control(&ci.chk_jitan_msgbk, IDC_CHK_JITAN_MSGBK, _T(""), 7);
	p_dlg->bind_control(&ci.btn_jitan_init, IDC_BTN_JITAN_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.btn_jitan_close, IDC_BTN_JITAN_CLOSE, _T(""), 3);

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_jitan_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_jitan.set_slider_range(0, 8);

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void Ccfg_wnd_func_jitan::on_close_funcfunc()
{
}

// �R�}���h
bool Ccfg_wnd_func_jitan::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_JITAN_NORMAL:
			Gp_config->jitan_normal_onoff = ci.chk_jitan_normal.get_check();
			return true;

		case IDC_CHK_JITAN_AUTOMODE:
			Gp_config->jitan_auto_mode_onoff = ci.chk_jitan_automode.get_check();
			return true;

		case IDC_CHK_JITAN_MSGBK:
			Gp_config->jitan_msgbk_onoff = ci.chk_jitan_msgbk.get_check();
			return true;

		case IDC_BTN_JITAN_INIT:
			init_state();
			return true;

		case IDC_BTN_JITAN_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// �ʒm
bool Ccfg_wnd_func_jitan::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_func_jitan::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool Ccfg_wnd_func_jitan::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_jitan.get_handle())	{
		Gp_config->jitan_speed = 100 + (ci.sld_jitan.get_slider_pos() * 25);
		update_text_info();
		return true;
	}
	return false;
}

// �u�X�N���[��
bool Ccfg_wnd_func_jitan::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_func_jitan::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void Ccfg_wnd_func_jitan::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// �O���ŕύX���ꂽ
	bool update_text_info_flag = false;
	if (Gp_config->jitan_normal_onoff != ci.chk_jitan_normal.get_check())	{
		ci.chk_jitan_normal.set_check(Gp_config->jitan_normal_onoff);
	}
	if (Gp_config->jitan_auto_mode_onoff != ci.chk_jitan_automode.get_check())	{
		ci.chk_jitan_automode.set_check(Gp_config->jitan_auto_mode_onoff);
	}
	if (Gp_config->jitan_msgbk_onoff != ci.chk_jitan_msgbk.get_check())	{
		ci.chk_jitan_msgbk.set_check(Gp_config->jitan_msgbk_onoff);
	}
	if (Gp_config->jitan_speed != 100 + (ci.sld_jitan.get_slider_pos() * 25))	{
		ci.sld_jitan.set_slider_pos((Gp_config->jitan_speed - 100) / 25);
		update_text_info_flag = true;
	}
	if (update_text_info_flag)	{	update_text_info();	}
}

// ������Ԃɖ߂�
void Ccfg_wnd_func_jitan::init_state()
{
	tnm_init_config_jitan();
	update_dialog();
}

// �_�C�A���O�X�V
void Ccfg_wnd_func_jitan::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.chk_jitan_normal.set_check(Gp_config->jitan_normal_onoff);
	ci.chk_jitan_automode.set_check(Gp_config->jitan_auto_mode_onoff);
	ci.chk_jitan_msgbk.set_check(Gp_config->jitan_msgbk_onoff);
	ci.sld_jitan.set_slider_pos((Gp_config->jitan_speed - 100) / 25);
	update_text_info();
}

// ****************************************************************
// �e�L�X�g���̍X�V
// ================================================================
void Ccfg_wnd_func_jitan::update_text_info()
{
	if (!h_wnd)	{	return;	}
#if 0	// ���x�̕\���i�����g�p�j
	TSTR str;
	if (Gp_config->jitan_speed == 100)	{
		str = _T("����");
	}
	else	{
		int a = Gp_config->jitan_speed / 100;
		int b = (Gp_config->jitan_speed % 100) / 10;
		int c = Gp_config->jitan_speed % 10;
		TSTR a_str = str_to_zenkaku_ret(str_format(_T("%d"), a));
		TSTR b_str = str_to_zenkaku_ret(str_format(_T("%d"), b));
		TSTR c_str = str_to_zenkaku_ret(str_format(_T("%d"), c));
		if (b == 0)	{
			if (c == 0)	{	str = a_str + _T(" �{��");				}
			else		{	str = a_str + _T("�D") + b_str + c_str;	}
		}
		else	{
			if (c == 0)	{	str = a_str + _T("�D") + b_str;	}
			else		{	str = a_str + _T("�D") + b_str + c_str;	}
		}
		str += _T(" �{��");
	}
#endif
}

