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
// コンフィグウィンドウ：ファンクション：ボリュームグループの横幅を取得
// ================================================================

int Gf_get_volume_group_num()
{
	int group_num = 1;		// 全体ボリュームは必ずある

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
	int each_group_width = slider_width + DLG_CM_GROUP_PDG_X;		// 特殊：詰めました
	int volume_contents_width = each_group_width * volume_group_num + (DLG_CM_SPACE / 2) * (volume_group_num - 1);
	int volume_group_width = DLG_CM_GROUP_PDG_X + volume_contents_width + DLG_CM_GROUP_PDG_X;

	return volume_group_width;
}

// ****************************************************************
// コンフィグウィンドウ：ファンクション：音量
// ================================================================

// アイテムを構築
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

	// ダイアログタイトル
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_VOLUME_DIALOG_DIALOG_TITLE);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		p_dlg->set_text(LCL_STR_VOLUME_BGMFADE_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// グループ数
	int volume_group_num = Gf_get_volume_group_num();

	// 横幅
	slider_width = DLG_CM_SLIDER_WIDTH;
	each_group_width = slider_width + DLG_CM_GROUP_PDG_X;		// 特殊：詰めました
	volume_contents_width = each_group_width * volume_group_num + (DLG_CM_SPACE / 2) * (volume_group_num - 1);

	// グループの横幅を計算
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		// グループ無しなのでそのまま
		volume_group_width = volume_contents_width;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// コンテンツ＋パディング
		volume_group_width = DLG_CM_GROUP_PDG_X + volume_contents_width + DLG_CM_GROUP_PDG_X;	// 12 + 240 + 12 = 264
	}

	// ＢＧＭフェードグループの横幅
	bgmfade_group_width = Gf_get_bgmfade_group_width();

	// 音量グループの縦幅は固定
	volume_group_height = 240;

	// 単体グループの高さを計算
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		// グループ無しなのでそのまま
		each_group_height = volume_group_height;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		// グループ分を引く
		each_group_height = volume_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// グループ＋「初期設定に戻す」ボタン分を引く
		each_group_height = volume_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_BTN_H + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
	}

	// 全体矩形
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

	// スライダー
	slider_x = DLG_CM_GROUP_PDG_X / 2;
	slider_y = DLG_CM_GROUP_PDG_T;
	slider_height = each_group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_GROUP_PDG_B) - DLG_CM_CHKBOX_H - DLG_CM_SPACE;

	// チェックボックス
	check_width = DLG_CM_CHKBOX_ADD_WIDTH + font_get_width(p_dlg->get_handle(), LCL_STR_VOLUME_DIALOG_STC_PLAY, font);
	check_height = DLG_CM_CHKBOX_H;
	check_x = (each_group_width - check_width) / 2;
	check_y = slider_y + slider_height + DLG_CM_SPACE;

	// 閉じるボタンと初期設定に戻すボタン（ダイアログ基準）
	int close_btn_x = dialog_width - dialog_pdg_x - DLG_CM_BTN_CLOSE_W;
	int close_btn_y = dialog_height - dialog_pdg_b - DLG_CM_BTN_CLOSE_H;
	int init_btn_x = dialog_width - dialog_pdg_x - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W;
	int init_btn_y = dialog_height - dialog_pdg_b - DLG_CM_BTN_INIT_H;

	if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// 特殊：ベース基準
		init_btn_x = (volume_group_width - DLG_CM_BTN_INIT_W) / 2;
		init_btn_y = each_group_height + DLG_CM_SPACE;
	}

	// コントロールを配置
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.grp_volume.create(_T(""), _T(""), base_x, base_y, volume_group_width, volume_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_volume.set_text(LCL_STR_VOLUME_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume, 4);
		SendMessage(ci.grp_volume.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		base_x += DLG_CM_GROUP_PDG_X;
		base_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	p_x = base_x;

	ci.grp_volume_all.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
	ci.grp_volume_all.set_text(LCL_STR_VOLUME_DIALOG_ALL_GROUP_TITLE);
	p_dlg->add_child(&ci.grp_volume_all, 4);
	SendMessage(ci.grp_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	ci.sld_volume_all.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
	ci.sld_volume_all.set_slider_range(0, 255);
	p_dlg->add_child(&ci.sld_volume_all, 4);
	SendMessage(ci.sld_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	ci.chk_volume_all.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_volume_all.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
	p_dlg->add_child(&ci.chk_volume_all, 1);
	SendMessage(ci.chk_volume_all.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	if (Gp_ini->config.exist_bgm)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_bgm.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_bgm.set_text(LCL_STR_VOLUME_DIALOG_BGM_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_bgm, 4);
		SendMessage(ci.grp_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.sld_volume_bgm.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_bgm.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_bgm, 4);
		SendMessage(ci.sld_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.chk_volume_bgm.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_bgm.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_bgm, 1);
		SendMessage(ci.chk_volume_bgm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	if (Gp_ini->config.exist_koe)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_koe.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_koe.set_text(LCL_STR_VOLUME_DIALOG_KOE_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_koe, 4);
		SendMessage(ci.grp_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.sld_volume_koe.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_koe.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_koe, 4);
		SendMessage(ci.sld_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.chk_volume_koe.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_koe.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_koe, 1);
		SendMessage(ci.chk_volume_koe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	if (Gp_ini->config.exist_pcm)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_pcm.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_pcm.set_text(LCL_STR_VOLUME_DIALOG_PCM_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_pcm, 4);
		SendMessage(ci.grp_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.sld_volume_pcm.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_pcm.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_pcm, 4);
		SendMessage(ci.sld_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.chk_volume_pcm.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_pcm.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_pcm, 1);
		SendMessage(ci.chk_volume_pcm.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	if (Gp_ini->config.exist_se)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_se.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_se.set_text(LCL_STR_VOLUME_DIALOG_SE_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_se, 4);
		SendMessage(ci.grp_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.sld_volume_se.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_se.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_se, 4);
		SendMessage(ci.sld_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.chk_volume_se.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_se.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_se, 1);
		SendMessage(ci.chk_volume_se.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	if (Gp_ini->config.exist_mov)
	{
		p_x += each_group_width + DLG_CM_SPACE / 2;

		ci.grp_volume_mov.create(_T(""), _T(""), p_x, base_y, each_group_width, each_group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER, 0, false, false, h_wnd);
		ci.grp_volume_mov.set_text(LCL_STR_VOLUME_DIALOG_MOV_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_volume_mov, 4);
		SendMessage(ci.grp_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.sld_volume_mov.create(_T(""), _T(""), p_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
		ci.sld_volume_mov.set_slider_range(0, 255);
		p_dlg->add_child(&ci.sld_volume_mov, 4);
		SendMessage(ci.sld_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.chk_volume_mov.create(_T(""), _T(""), p_x + check_x, base_y + check_y, check_width, check_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_volume_mov.set_text(LCL_STR_VOLUME_DIALOG_STC_PLAY);
		p_dlg->add_child(&ci.chk_volume_mov, 1);
		SendMessage(ci.chk_volume_mov.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// 共通のフッタボタン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)
	{
		ci.btn_volume_close.create(_T(""), _T(""), close_btn_x, close_btn_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_close, 3);
		SendMessage(ci.btn_volume_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_volume_init.create(_T(""), _T(""), init_btn_x, init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_init, 3);
		SendMessage(ci.btn_volume_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.btn_volume_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_volume_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_volume_init, 1);
		SendMessage(ci.btn_volume_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// ダイアログを再配置
	refresh_dialog_func(dialog_width, dialog_height);
}

// 初期化
void Ccfg_wnd_func_volume::on_init_funcfunc()
{
	// ダイアログを構築
	setup_dialog();

/*
	// アイテム登録
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
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_MODAL_VOLUME_BGMFADE)	{		// 音量：初期化ボタンの座標調整モードが違います
		p_dlg->bind_control(&ci.btn_volume_init, IDC_BTN_VOLUME_INIT, _T(""), 3);
	}
	else	{
		p_dlg->bind_control(&ci.btn_volume_init, IDC_BTN_VOLUME_INIT, _T(""), 1);
	}

	p_dlg->bind_control(&ci.btn_volume_close, IDC_BTN_VOLUME_CLOSE, _T(""), 3);

	// 使用しない項目を削除し、項目を詰める
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
	if (full_mode_flag)	{		// デバッグ用フルモード
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
			item[i].ctrl_grp->set_show(false);		// 消去
			item[i].ctrl_sld->set_show(false);		// 消去
			item[i].ctrl_chk->set_show(false);		// 消去
			if (i < ((int)item.size() - 1))	{	x_rep += item[i + 1].rect.left - item[i].rect.left;	}	// 次の項目との距離
			else	{
				if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
					x_rep += item[i].rect.width() + 9;	// 最後の項目は幅（ + 9 = 微調整）
				}
				else	{
					x_rep += item[i].rect.width() + 11;	// 最後の項目は幅（ + 11 = 微調整）
				}
			}
		}
	}

	// 音量グループの幅を調整する
	ci.grp_volume.add_window_size_with_def_size(-x_rep, 0);

	// 初期化ボタンの座標を調整する
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

	// ＢＧＭフェードの座標調整用に記憶
	bgmfade_item_x_rep = x_rep;

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_volume_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_volume_all.set_slider_range(0, 255);
	ci.sld_volume_bgm.set_slider_range(0, 255);
	ci.sld_volume_koe.set_slider_range(0, 255);
	ci.sld_volume_pcm.set_slider_range(0, 255);
	ci.sld_volume_se.set_slider_range(0, 255);
	ci.sld_volume_mov.set_slider_range(0, 255);
	*/

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_volume::on_close_funcfunc()
{
}

// コマンド
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

		if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)			// 音量：ＢＧＭフェードも初期化します
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

// 通知
bool Ccfg_wnd_func_volume::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_volume::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_volume::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
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

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_volume::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_volume::frame_funcfunc()
{
	if (!h_wnd)
		return;

	// 外部で変更された
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

// 初期状態に戻す
void Ccfg_wnd_func_volume::init_state()
{
	tnm_init_config_volume();
	update_dialog();
}

// ダイアログ更新
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
// 「ウィンドウサイズ」と「初期化ボタン」と「閉じるボタン」を調整する
// ================================================================
void Ccfg_wnd_func_volume::rep_window_size_func(int x_rep)
{
	ci.btn_volume_init.add_rltv_pos_with_def_rect(x_rep, 0);	// 初期化ボタン
	ci.btn_volume_close.add_rltv_pos_with_def_rect(x_rep, 0);	// 閉じるボタン
	p_dlg->add_window_size_with_def_size(x_rep, 0);				// ウィンドウサイズ
	p_dlg->send_wm_size();										// ★これ重要！（起動時、ウィンドウがリサイズしなかった場合、アイテムの座標が狂うので強制的にリサイズさせる）
}
