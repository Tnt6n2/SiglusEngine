#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_volume.h"
#include	"dialog/cfg_wnd_solo_volume_bgmfade.h"
#include	"dialog/cfg_wnd_modal_volume_bgmfade.h"
#include	"dialog/cfg_wnd_common.h"
#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�{�����[���O���[�v�̉������擾
// ================================================================

int Gf_get_volume_group_num()
{
	int group_num = 1;		// �S�̃{�����[���͕K������

	if (Gp_ini->config.exist_bgm)
	{
		group_num++;
	}
	if (Gp_ini->config.exist_koe)
	{
		group_num++;
	}
	if (Gp_ini->config.exist_pcm)
	{
		group_num++;
	}
	if (Gp_ini->config.exist_se)
	{
		group_num++;
	}
	if (Gp_ini->config.exist_mov)
	{
		group_num++;
	}

	return group_num;
}

int Gf_get_volume_group_width()
{
	int volume_group_num = Gf_get_volume_group_num();
	int slider_width = DLG_CM_SLIDER_WIDTH;
	int each_group_width = slider_width + DLG_CM_GROUP_PDG_X;		// ����F�l�߂܂���
	int volume_contents_width = each_group_width * volume_group_num + (DLG_CM_SPACE / 2) * (volume_group_num - 1);
	int volume_group_width = DLG_CM_GROUP_PDG_X + volume_contents_width + DLG_CM_GROUP_PDG_X;

	return volume_group_width;
}

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F����
// ================================================================

// �A�C�e�����\�z
void Ccfg_wnd_func_volume::setup_dialog()
{
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;

	int volume_group_width, volume_group_height, volume_contents_width;
	int bgmfade_group_width;
	int each_group_width, each_group_height;
	int slider_x, slider_y, slider_width, slider_height;
	int check_x, check_y, check_width, check_height;

	int p_x = 0, p_y = 0;

	// �_�C�A���O�^�C�g��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_VOLUME_DIALOG_DIALOG_TITLE);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		p_dlg->set_text(LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE);
	}

	// �t�H���g
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// �O���[�v��
	int volume_group_num = Gf_get_volume_group_num();

	// ����
	slider_width = DLG_CM_SLIDER_WIDTH;
	each_group_width = slider_width + DLG_CM_GROUP_PDG_X;		// ����F�l�߂܂���
	volume_contents_width = each_group_width * volume_group_num + (DLG_CM_SPACE / 2) * (volume_group_num - 1);

	// �O���[�v�̉������v�Z
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		// �O���[�v�����Ȃ̂ł��̂܂�
		volume_group_width = volume_contents_width;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// �R���e���c�{�p�f�B���O
		volume_group_width = DLG_CM_GROUP_PDG_X + volume_contents_width + DLG_CM_GROUP_PDG_X;	// 12 + 240 + 12 = 264
	}

	// �a�f�l�t�F�[�h�O���[�v�̉���
	bgmfade_group_width = Gf_get_bgmfade_group_width();

	// ���ʃO���[�v�̏c���͌Œ�
	volume_group_height = 240;

	// �P�̃O���[�v�̍������v�Z
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		// �O���[�v�����Ȃ̂ł��̂܂�
		each_group_height = volume_group_height;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		// �O���[�v��������
		each_group_height = volume_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// �O���[�v�{�u�����ݒ�ɖ߂��v�{�^����������
		each_group_height = volume_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
	}

	// �S�̋�`
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		dialog_width = DLG_CM_SOLO_PDG_X + volume_group_width + DLG_CM_SOLO_PDG_X;										// 12 + 240 + 12 = 264
		dialog_height = DLG_CM_SOLO_PDG_T + volume_group_height + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_SOLO_PDG_B;		// 12 + 240 + 6 + 24 + 36 = 318
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		dialog_width = DLG_CM_SOLO_PDG_X + volume_group_width + DLG_CM_SPACE * 2 + bgmfade_group_width + DLG_CM_SOLO_PDG_X;	// 12 + 264 + 12 + 120 + 12 = 420
		dialog_height = DLG_CM_SOLO_PDG_T + volume_group_height + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_SOLO_PDG_B;			// 12 + 240 + 6 + 24 + 36 = 318
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		dialog_width = DLG_CM_BASE_PDG_X + volume_group_width + DLG_CM_SPACE * 2 + bgmfade_group_width + DLG_CM_BASE_PDG_X;	// 12 + 264 + 12 + 120 + 12 = 420
		dialog_height = DLG_CM_BASE_PDG_T + volume_group_height + DLG_CM_BASE_PDG_B;										// 12 + 240 + 12 = 264
		dialog_pdg_x = DLG_CM_BASE_PDG_X;
		dialog_pdg_t = DLG_CM_BASE_PDG_T;
		dialog_pdg_b = DLG_CM_BASE_PDG_B;
	}

	// �X���C�_�[
	slider_x = DLG_CM_GROUP_PDG_X / 2;
	slider_y = DLG_CM_GROUP_PDG_T;
	slider_height = each_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_GROUP_PDG_B) - DLG_CM_CHKBOX_H - DLG_CM_SPACE;

	// �`�F�b�N�{�b�N�X
	check_width = DLG_CM_CHKBOX_ADD_WIDTH + font_get_width(p_dlg->get_handle(), LCL_STR_VOLUME_DIALOG_STC_PLAY, font);
	check_height = DLG_CM_CHKBOX_H;
	check_x = (each_group_width - check_width) / 2;
	check_y = slider_y + slider_height + DLG_CM_SPACE;

	// ����{�^���Ə����ݒ�ɖ߂��{�^���i�_�C�A���O��j
	int close_btn_x = dialog_width - dialog_pdg_x - DLG_CM_BTN_CLOSE_W;
	int close_btn_y = dialog_height - dialog_pdg_b - DLG_CM_BTN_CLOSE_H;
	int init_btn_x = dialog_width - dialog_pdg_x - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W;
	int init_btn_y = dialog_height - dialog_pdg_b - DLG_CM_BTN_INIT_H;

	if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// ����F�x�[�X�
		init_btn_x = (volume_group_width - DLG_CM_BTN_INIT_W) / 2;
		init_btn_y = each_group_height + DLG_CM_SPACE;
	}

	// �R���g���[����z�u
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.grp_volume.create(_T(""), _T(""), base_x, base_y, volume_group_width, volume_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_volume.set_text(LCL_STR_VOLUME_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume, 4);
		SendMessage(ci.grp_volume.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		base_x += DLG_CM_GROUP_PDG_X;
		base_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	p_x = base_x;

	ci.grp_volume_all.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
	ci.grp_volume_all.set_text(LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE);
	p_dlg->add_child(&ci.grp_volume_all, 4);
	SendMessage(ci.grp_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	ci.sld_volume_all.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
	ci.sld_volume_all.set_slider_range(0, 255);
	p_dlg->add_child(&ci.sld_volume_all, 4);
	SendMessage(ci.sld_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	ci.chk_volume_all.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_volume_all.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
	p_dlg->add_child(&ci.chk_volume_all, 1);
	SendMessage(ci.chk_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

	if (Gp_ini->config.exist_bgm)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_bgm.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_bgm.set_text(LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_bgm, 4);
		SendMessage(ci.grp_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.sld_volume_bgm.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_bgm.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_bgm, 4);
		SendMessage(ci.sld_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.chk_volume_bgm.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_bgm.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_bgm, 1);
		SendMessage(ci.chk_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	if (Gp_ini->config.exist_koe)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_koe.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_koe.set_text(LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_koe, 4);
		SendMessage(ci.grp_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.sld_volume_koe.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_koe.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_koe, 4);
		SendMessage(ci.sld_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.chk_volume_koe.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_koe.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_koe, 1);
		SendMessage(ci.chk_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	if (Gp_ini->config.exist_pcm)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_pcm.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_pcm.set_text(LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_pcm, 4);
		SendMessage(ci.grp_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.sld_volume_pcm.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_pcm.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_pcm, 4);
		SendMessage(ci.sld_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.chk_volume_pcm.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_pcm.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_pcm, 1);
		SendMessage(ci.chk_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	if (Gp_ini->config.exist_se)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_se.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_se.set_text(LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_se, 4);
		SendMessage(ci.grp_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.sld_volume_se.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_se.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_se, 4);
		SendMessage(ci.sld_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.chk_volume_se.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_se.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_se, 1);
		SendMessage(ci.chk_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	if (Gp_ini->config.exist_mov)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_mov.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_mov.set_text(LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_mov, 4);
		SendMessage(ci.grp_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.sld_volume_mov.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_mov.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_mov, 4);
		SendMessage(ci.sld_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.chk_volume_mov.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_mov.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_mov, 1);
		SendMessage(ci.chk_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}

	// ���ʂ̃t�b�^�{�^��
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		ci.btn_volume_close.create(_T(""), _T(""), close_btn_x, close_btn_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_close, 3);
		SendMessage(ci.btn_volume_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�

		ci.btn_volume_init.create(_T(""), _T(""), init_btn_x, init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_init, 3);
		SendMessage(ci.btn_volume_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.btn_volume_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_init, 1);
		SendMessage(ci.btn_volume_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// �O���[�v�̃t�H���g�ݒ�
	}

	// �_�C�A���O���Ĕz�u
	refresh_dialog_func(dialog_width, dialog_height);
}

// ������
void Ccfg_wnd_func_volume::on_init_funcfunc()
{
	// �_�C�A���O���\�z
	setup_dialog();

/*
	// �A�C�e���o�^
	p_dlg->bind_control(&ci.grp_volume, IDC_GRP_VOLUME, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_all, IDC_GRP_VOLUME_ALL, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_bgm, IDC_GRP_VOLUME_BGM, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_koe, IDC_GRP_VOLUME_KOE, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_pcm, IDC_GRP_VOLUME_PCM, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_se, IDC_GRP_VOLUME_SE, _T(""), 4);
	p_dlg->bind_control(&ci.grp_volume_mov, IDC_GRP_VOLUME_MOV, _T(""), 4);

	p_dlg->bind_control(&ci.sld_volume_all, IDC_SLD_VOLUME_ALL, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_all, IDC_CHK_VOLUME_ALL, _T(""), 1);
	p_dlg->bind_control(&ci.sld_volume_bgm, IDC_SLD_VOLUME_BGM, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_bgm, IDC_CHK_VOLUME_BGM, _T(""), 1);
	p_dlg->bind_control(&ci.sld_volume_koe, IDC_SLD_VOLUME_KOE, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_koe, IDC_CHK_VOLUME_KOE, _T(""), 1);
	p_dlg->bind_control(&ci.sld_volume_pcm, IDC_SLD_VOLUME_PCM, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_pcm, IDC_CHK_VOLUME_PCM, _T(""), 1);
	p_dlg->bind_control(&ci.sld_volume_se, IDC_SLD_VOLUME_SE, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_se, IDC_CHK_VOLUME_SE, _T(""), 1);
	p_dlg->bind_control(&ci.sld_volume_mov, IDC_SLD_VOLUME_MOV, _T(""), 4);
	p_dlg->bind_control(&ci.chk_volume_mov, IDC_CHK_VOLUME_MOV, _T(""), 1);

	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		p_dlg->bind_control(&ci.btn_volume_init, IDC_BTN_VOLUME_INIT, _T(""), 3);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_MODAL_VOLUME_BGMFADE)	{		// ���ʁF�������{�^���̍��W�������[�h���Ⴂ�܂�
		p_dlg->bind_control(&ci.btn_volume_init, IDC_BTN_VOLUME_INIT, _T(""), 3);
	}
	else	{
		p_dlg->bind_control(&ci.btn_volume_init, IDC_BTN_VOLUME_INIT, _T(""), 1);
	}

	p_dlg->bind_control(&ci.btn_volume_close, IDC_BTN_VOLUME_CLOSE, _T(""), 3);

	// �g�p���Ȃ����ڂ��폜���A���ڂ��l�߂�
	struct item_struct
	{
		bool exist;
		C_control* ctrl_grp;
		C_control* ctrl_sld;
		C_control* ctrl_chk;
		C_rect rect;
	};
	ARRAY<item_struct>	item(5);
	item[0].exist = Gp_ini->config.exist_bgm;
	item[1].exist = Gp_ini->config.exist_koe;
	item[2].exist = Gp_ini->config.exist_pcm;
	item[3].exist = Gp_ini->config.exist_se;
	item[4].exist = Gp_ini->config.exist_mov;
	item[0].ctrl_grp = &ci.grp_volume_bgm;
	item[1].ctrl_grp = &ci.grp_volume_koe;
	item[2].ctrl_grp = &ci.grp_volume_pcm;
	item[3].ctrl_grp = &ci.grp_volume_se;
	item[4].ctrl_grp = &ci.grp_volume_mov;
	item[0].ctrl_sld = &ci.sld_volume_bgm;
	item[1].ctrl_sld = &ci.sld_volume_koe;
	item[2].ctrl_sld = &ci.sld_volume_pcm;
	item[3].ctrl_sld = &ci.sld_volume_se;
	item[4].ctrl_sld = &ci.sld_volume_mov;
	item[0].ctrl_chk = &ci.chk_volume_bgm;
	item[1].ctrl_chk = &ci.chk_volume_koe;
	item[2].ctrl_chk = &ci.chk_volume_pcm;
	item[3].ctrl_chk = &ci.chk_volume_se;
	item[4].ctrl_chk = &ci.chk_volume_mov;
	if (full_mode_flag)	{		// �f�o�b�O�p�t�����[�h
		for (int i = 0; i < (int)item.size(); i++)	{
			item[i].exist = true;
		}
	}
	for (int i = 0; i < (int)item.size(); i++)	{
		item[i].rect = item[i].ctrl_grp->get_rltv_rect();
	}
	int x_rep = 0;
	for (int i = 0; i < (int)item.size(); i++)	{
		if (item[i].exist)	{
			item[i].ctrl_grp->add_rltv_pos_with_def_rect(-x_rep, 0);
			item[i].ctrl_sld->add_rltv_pos_with_def_rect(-x_rep, 0);
			item[i].ctrl_chk->add_rltv_pos_with_def_rect(-x_rep, 0);
		}
		else	{
			item[i].ctrl_grp->set_show(false);		// ����
			item[i].ctrl_sld->set_show(false);		// ����
			item[i].ctrl_chk->set_show(false);		// ����
			if (i < ((int)item.size() - 1))	{	x_rep += item[i + 1].rect.left - item[i].rect.left;	}	// ���̍��ڂƂ̋���
			else	{
				if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
					x_rep += item[i].rect.width() + 9;	// �Ō�̍��ڂ͕��i + 9 = �������j
				}
				else	{
					x_rep += item[i].rect.width() + 11;	// �Ō�̍��ڂ͕��i + 11 = �������j
				}
			}
		}
	}

	// ���ʃO���[�v�̕��𒲐�����
	ci.grp_volume.add_window_size_with_def_size(-x_rep, 0);

	// �������{�^���̍��W�𒲐�����
	if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)	{
		int btn_width = ci.btn_volume_init.get_client_rect().width();
		int grp_width = ci.grp_volume.get_client_rect().width();
		int btn_y = ci.btn_volume_init.get_rltv_rect().top;
		int grp_x = ci.grp_volume.get_rltv_rect().left;
		ci.btn_volume_init.set_rltv_pos_with_def_rect(C_point(grp_x + ((grp_width - btn_width) / 2), btn_y));
	}
	else	{
		rep_window_size_func(x_rep);
	}

	// �a�f�l�t�F�[�h�̍��W�����p�ɋL��
	bgmfade_item_x_rep = x_rep;

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_volume_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_volume_all.set_slider_range(0, 255);
	ci.sld_volume_bgm.set_slider_range(0, 255);
	ci.sld_volume_koe.set_slider_range(0, 255);
	ci.sld_volume_pcm.set_slider_range(0, 255);
	ci.sld_volume_se.set_slider_range(0, 255);
	ci.sld_volume_mov.set_slider_range(0, 255);
	*/

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void Ccfg_wnd_func_volume::on_close_funcfunc()
{
}

// �R�}���h
bool Ccfg_wnd_func_volume::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.chk_volume_all.get_handle())
	{
		Gp_config->play_all_sound_check = ci.chk_volume_all.get_check();
		return true;
	}
	else if ((HWND)lp == ci.chk_volume_bgm.get_handle())
	{
		Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] = ci.chk_volume_bgm.get_check();
		return true;
	}
	else if ((HWND)lp == ci.chk_volume_koe.get_handle())
	{
		Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] = ci.chk_volume_koe.get_check();
		return true;
	}
	else if ((HWND)lp == ci.chk_volume_pcm.get_handle())
	{
		Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] = ci.chk_volume_pcm.get_check();
		return true;
	}
	else if ((HWND)lp == ci.chk_volume_se.get_handle())
	{
		Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] = ci.chk_volume_se.get_check();
		return true;
	}
	else if ((HWND)lp == ci.chk_volume_mov.get_handle())
	{
		Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] = ci.chk_volume_mov.get_check();
		return true;
	}
	else if ((HWND)lp == ci.btn_volume_init.get_handle())
	{
		init_state();

		if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)			// ���ʁF�a�f�l�t�F�[�h�����������܂�
		{
			if (modal_mode_flag)
			{
				((Ccfg_wnd_modal_volume_bgmfade *)p_dlg)->cfg_wnd_func_bgmfade.init_state();
			}
			else
			{
				((Ccfg_wnd_solo_volume_bgmfade *)p_dlg)->cfg_wnd_func_bgmfade.init_state();
			}
		}
		return true;
	}
	else if ((HWND)lp == ci.btn_volume_close.get_handle())
	{
		p_dlg->close();
		return true;
	}

	return false;
}

// �ʒm
bool Ccfg_wnd_func_volume::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool Ccfg_wnd_func_volume::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool Ccfg_wnd_func_volume::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �u�X�N���[��
bool Ccfg_wnd_func_volume::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_volume_all.get_handle())	{
		Gp_config->all_sound_user_volume = 255 - ci.sld_volume_all.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_volume_bgm.get_handle())	{
		Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] = 255 - ci.sld_volume_bgm.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_volume_koe.get_handle())	{
		Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] = 255 - ci.sld_volume_koe.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_volume_pcm.get_handle())	{
		Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] = 255 - ci.sld_volume_pcm.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_volume_se.get_handle())	{
		Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] = 255 - ci.sld_volume_se.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_volume_mov.get_handle())	{
		Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] = 255 - ci.sld_volume_mov.get_slider_pos();
		return true;
	}

	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT Ccfg_wnd_func_volume::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void Ccfg_wnd_func_volume::frame_funcfunc()
{
	if (!h_wnd)
		return;

	// �O���ŕύX���ꂽ
	if (Gp_config->all_sound_user_volume != (255 - ci.sld_volume_all.get_slider_pos()))	{
		ci.sld_volume_all.set_slider_pos(255 - Gp_config->all_sound_user_volume);
	}
	if (Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] != (255 - ci.sld_volume_bgm.get_slider_pos()))	{
		ci.sld_volume_bgm.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM]);
	}
	if (Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] != (255 - ci.sld_volume_koe.get_slider_pos()))	{
		ci.sld_volume_koe.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE]);
	}
	if (Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] != (255 - ci.sld_volume_pcm.get_slider_pos()))	{
		ci.sld_volume_pcm.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM]);
	}
	if (Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] != (255 - ci.sld_volume_se.get_slider_pos()))	{
		ci.sld_volume_se.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE]);
	}
	if (Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] != (255 - ci.sld_volume_mov.get_slider_pos()))	{
		ci.sld_volume_mov.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV]);
	}

	if (Gp_config->play_all_sound_check != ci.chk_volume_all.get_check())	{
		ci.chk_volume_all.set_check(Gp_config->play_all_sound_check);
	}
	if (Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] != ci.chk_volume_bgm.get_check())	{
		ci.chk_volume_bgm.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM]);
	}
	if (Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] != ci.chk_volume_koe.get_check())	{
		ci.chk_volume_koe.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE]);
	}
	if (Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] != ci.chk_volume_pcm.get_check())	{
		ci.chk_volume_pcm.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM]);
	}
	if (Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] != ci.chk_volume_se.get_check())	{
		ci.chk_volume_se.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE]);
	}
	if (Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] != ci.chk_volume_mov.get_check())	{
		ci.chk_volume_mov.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV]);
	}
}

// ������Ԃɖ߂�
void Ccfg_wnd_func_volume::init_state()
{
	tnm_init_config_volume();
	update_dialog();
}

// �_�C�A���O�X�V
void Ccfg_wnd_func_volume::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_volume_all.set_slider_pos(255 - Gp_config->all_sound_user_volume);
	ci.sld_volume_bgm.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM]);
	ci.sld_volume_koe.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE]);
	ci.sld_volume_pcm.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM]);
	ci.sld_volume_se.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE]);
	ci.sld_volume_mov.set_slider_pos(255 - Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV]);
	ci.chk_volume_all.set_check(Gp_config->play_all_sound_check);
	ci.chk_volume_bgm.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM]);
	ci.chk_volume_koe.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE]);
	ci.chk_volume_pcm.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM]);
	ci.chk_volume_se.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE]);
	ci.chk_volume_mov.set_check(Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV]);
}

// ****************************************************************
// �u�E�B���h�E�T�C�Y�v�Ɓu�������{�^���v�Ɓu����{�^���v�𒲐�����
// ================================================================
void Ccfg_wnd_func_volume::rep_window_size_func(int x_rep)
{
	ci.btn_volume_init.add_rltv_pos_with_def_rect(x_rep, 0);	// �������{�^��
	ci.btn_volume_close.add_rltv_pos_with_def_rect(x_rep, 0);	// ����{�^��
	p_dlg->add_window_size_with_def_size(x_rep, 0);				// �E�B���h�E�T�C�Y
	p_dlg->send_wm_size();										// ������d�v�I�i�N�����A�E�B���h�E�����T�C�Y���Ȃ������ꍇ�A�A�C�e���̍��W�������̂ŋ����I�Ƀ��T�C�Y������j
}
