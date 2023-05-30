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
// コンフィグウィンドウ：ファンクション：ＢＧＭフェードグループの横幅を取得
// ================================================================

int Gf_get_bgmfade_group_width()
{
	return 120;
}

// ****************************************************************
// コンフィグウィンドウ：ファンクション：ＢＧＭフェード
// ================================================================

// アイテムを構築
void Ccfg_wnd_func_bgmfade::setup_dialog()
{
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;

	int group_width, group_height, contents_width, contents_height;

	int explain_width, explain_height;
	int slider_x, slider_y, slider_width, slider_height;

	int p_x = 0, p_y = 0;

	// ダイアログタイトル
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_BGMFADE_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// 単体ダイアログはすごく特殊。
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;

		// ダイアログの幅はボタンが入るサイズ
		dialog_width = DLG_CM_SOLO_PDG_X + DLG_CM_BTN_CLOSE_W + DLG_CM_SPACE + DLG_CM_BTN_INIT_W + DLG_CM_SOLO_PDG_X;

		// ダイアログの高さは固定
		dialog_height = 270;

		// グループの大きさを計算
		group_width = dialog_width - DLG_CM_SOLO_PDG_X * 2;
		group_height = dialog_height - (DLG_CM_SOLO_PDG_T + DLG_CM_SOLO_PDG_B);

		// コンテンツの大きさを計算
		contents_width = group_width;
		contents_height = group_height;

		// 説明
		explain_width = contents_width;
#if defined(__SIGLUS_ID)
		explain_height = 36;		// インドネシア語のみ１行サイズを増やす
#else
		explain_height = 24;
#endif

		// スライダー
		slider_width = DLG_CM_SLIDER_WIDTH;
		slider_height = 128;
		slider_x = (contents_width - slider_width) / 2;
		slider_y = explain_height + DLG_CM_SPACE;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// グループの大きさは固定
		group_width = Gf_get_bgmfade_group_width();
		group_height = 240;

		// ボリュームグループの大きさを持ってくる
		int volume_group_width = Gf_get_volume_group_width();
		int volume_group_height = 240;

		// ダイアログの大きさはボリュームダイアログを参考に
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

		// コンテンツの大きさを計算
		contents_width = group_width - DLG_CM_GROUP_PDG_X * 2;
		contents_height = group_height - (DLG_CM_GROUP_PDG_T + DLG_CM_GROUP_PDG_B);

		// 説明
		explain_width = contents_width;
#if defined(__SIGLUS_ID)
		explain_height = 60;		// インドネシア語のみ１行サイズを増やす
#else
		explain_height = 48;
#endif

		// スライダー
		slider_width = DLG_CM_SLIDER_WIDTH;
		slider_height = contents_height - (explain_height + DLG_CM_SPACE + DLG_CM_CHKBOX_H + DLG_CM_SPACE) - (DLG_CM_SPACE + DLG_CM_GROUP_PDG_B);
		slider_x = (contents_width - slider_width) / 2;
		slider_y = explain_height + DLG_CM_SPACE;

		if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
		{
			slider_height -= DLG_CM_SPACE + DLG_CM_BTN_H;
		}
	}

	// 有効／無効
	int enable_width = DLG_CM_CHKBOX_ADD_WIDTH + font_get_width(p_dlg->get_handle(), LCL_STR_BGMFADE_DIALOG_STC_ENABLE, font);
	int enable_height = DLG_CM_CHKBOX_H;
	int enable_x = (contents_width - enable_width) / 2;
	int enable_y = slider_y + slider_height + DLG_CM_SPACE;

	// 閉じるボタンと初期設定に戻すボタン
	int close_btn_x = contents_width - DLG_CM_BTN_CLOSE_W;
	int close_btn_y = contents_height - DLG_CM_BTN_CLOSE_H;
	int init_btn_x = close_btn_x - DLG_CM_SPACE - DLG_CM_BTN_INIT_W;
	int init_btn_y = contents_height - DLG_CM_BTN_INIT_H;

	if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		// 音量・ＢＧＭフェードの統合は特殊配置になります。
		init_btn_x = (contents_width - DLG_CM_BTN_INIT_W) / 2;
		init_btn_y = contents_height - DLG_CM_BTN_INIT_H - DLG_CM_GROUP_PDG_B;
	}

	// コントロールを配置
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE || func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.grp_bgmfade.create(_T(""), _T(""), base_x, base_y, group_width, group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_bgmfade, 4);
		SendMessage(ci.grp_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		base_x += DLG_CM_GROUP_PDG_X;
		base_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	ci.stc_bgmfade.create(_T(""), _T(""), base_x, base_y, explain_width, explain_height, false, WS_CHILD | WS_VISIBLE, 0, false, false, h_wnd);
	ci.stc_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_STC_EXPLAINE);
	p_dlg->add_child(&ci.stc_bgmfade, 7);
	SendMessage(ci.stc_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	ci.sld_bgmfade.create(_T(""), _T(""), base_x + slider_x, base_y + slider_y, slider_width, slider_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 0, false, false, h_wnd);
	ci.sld_bgmfade.set_slider_range(0, 255);
	p_dlg->add_child(&ci.sld_bgmfade, 4);
	SendMessage(ci.sld_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	ci.chk_bgmfade.create(_T(""), _T(""), base_x + enable_x, base_y + enable_y, enable_width, enable_height, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_bgmfade.set_text(LCL_STR_BGMFADE_DIALOG_STC_ENABLE);
	p_dlg->add_child(&ci.chk_bgmfade, 1);
	SendMessage(ci.chk_bgmfade.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	// 共通のフッタボタン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		ci.btn_bgmfade_close.create(_T(""), _T(""), base_x + close_btn_x, base_y + close_btn_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_close, 3);
		SendMessage(ci.btn_bgmfade_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_bgmfade_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_init, 3);
		SendMessage(ci.btn_bgmfade_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE)
	{
		ci.btn_bgmfade_init.create(_T(""), _T(""), base_x + init_btn_x, base_y + init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_bgmfade_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_bgmfade_init, 1);
		SendMessage(ci.btn_bgmfade_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// ダイアログを再配置
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		refresh_dialog_func(dialog_width, dialog_height);
	}
}

// 初期化
void Ccfg_wnd_func_bgmfade::on_init_funcfunc()
{
	// ダイアログを構築
	setup_dialog();

	/*
	// アイテム登録
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

	// 座標調整
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
	ci.grp_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// グループ
	ci.stc_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// 文字列
	ci.sld_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// スライダー
	ci.chk_bgmfade.add_rltv_pos_with_def_rect(-x_rep, 0);		// チェックボックス
	ci.btn_bgmfade_init.add_rltv_pos_with_def_rect(-x_rep, 0);	// 初期化ボタン

	// アイテム消去
//	if (false)	{
//		ci.grp_bgmfade.set_show(false);			// グループ
//		ci.stc_bgmfade.set_show(false);			// 文字列
//		ci.sld_bgmfade.set_show(false);			// スライダー
//		ci.chk_bgmfade.set_show(false);			// チェックボックス
//		ci.btn_bgmfade_init.set_show(false);	// 初期化ボタン
//
//		// ウィンドウサイズ調整（音量の関数を呼び出します）
//		if (func_mod == CFG_WND_FUNC_MOD_SOLO_VOLUME_BGMFADE)	{			// ＢＧＭフェード：アイテム消去時のウィンドウサイズ調整（音量の関数を呼び出します）
//			int xx_rep = ci.grp_bgmfade.get_window_rect().width() + 10;		// + 10 = 微調整
//			((Ccfg_wnd_solo_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.rep_window_size_func(-xx_rep);
//		}
//		else if (func_mod == CFG_WND_FUNC_MOD_MODAL_VOLUME_BGMFADE)	{		// ＢＧＭフェード：アイテム消去時のウィンドウサイズ調整（音量の関数を呼び出します）
//			int xx_rep = ci.grp_bgmfade.get_window_rect().width() + 10;		// + 10 = 微調整
//			((Ccfg_wnd_modal_volume_bgmfade *)p_dlg)->cfg_wnd_func_volume.rep_window_size_func(-xx_rep);
//		}
//	}

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_bgmfade_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_bgmfade.set_slider_range(0, 255);
	*/

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_bgmfade::on_close_funcfunc()
{
}

// コマンド
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

// 通知
bool Ccfg_wnd_func_bgmfade::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_bgmfade::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_bgmfade::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_bgmfade::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_bgmfade.get_handle())	{
		Gp_config->bgmfade_volume = 255 - ci.sld_bgmfade.get_slider_pos();
		return true;
	}
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_bgmfade::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	return NULL;
}

// フレーム
void Ccfg_wnd_func_bgmfade::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (Gp_config->bgmfade_volume != (255 - ci.sld_bgmfade.get_slider_pos()))	{
		ci.sld_bgmfade.set_slider_pos(255 - Gp_config->bgmfade_volume);
	}
	if (Gp_config->bgmfade_use_check != ci.chk_bgmfade.get_check())	{
		ci.chk_bgmfade.set_check(Gp_config->bgmfade_use_check);
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_bgmfade::init_state()
{
	tnm_init_config_bgmfade();
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_bgmfade::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_bgmfade.set_slider_pos(255 - Gp_config->bgmfade_volume);
	ci.chk_bgmfade.set_check(Gp_config->bgmfade_use_check);
}

