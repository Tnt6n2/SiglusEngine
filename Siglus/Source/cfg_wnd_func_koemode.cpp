#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_koemode.h"
#include	"dialog/cfg_wnd_common.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：音声モード
// ================================================================

// アイテムを構築
void Ccfg_wnd_func_koemode::setup_dialog()
{
	bool group_flag = (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_KOEMODE_CHRKOE || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE);
	bool koemod_chrkoe_flag = (func_mod == CFG_WND_FUNC_MOD_BASE_KOEMODE_CHRKOE || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE);

	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;
	int base_width, base_height;
	int group_width, group_height;
	int contents_width, contents_height;
	int p_x, p_y, width;

	// ダイアログタイトル
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_KOEMOD_DIALOG_DIALOG_TITLE);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
		p_dlg->set_text(LCL_STR_KOEMOD_CHRKOE_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// キャラクター音声グループの幅
	int chrkoe_group_width = 288;

	if (wide_koe_wnd_flag)
	{
		chrkoe_group_width += 160;
	}

	// マージン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_KOEMODE_CHRKOE)
	{
		dialog_pdg_x = DLG_CM_BASE_PDG_X;
		dialog_pdg_t = DLG_CM_BASE_PDG_T;
		dialog_pdg_b = DLG_CM_BASE_PDG_B;
	}

	// 幅は固定です。
	group_width = (func_mod == CFG_WND_FUNC_MOD_SOLO ? 188 : 160);
	if (wide_koe_wnd_flag)
	{
		group_width += 100;
	}
	base_width = group_width + (koemod_chrkoe_flag ? DLG_CM_SPACE + chrkoe_group_width : 0);
	dialog_width = base_width + DLG_CM_SOLO_PDG_X * 2;
	contents_width = group_width - (group_flag ? DLG_CM_GROUP_PDG_X * 2 : 0);

	// グループの高さを固定します。（下で変更される場合もあります。）
	group_height = 160;
	contents_height = group_height - (group_flag ? DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B : 0);

	// ベース位置
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;
	int group_x = base_x;
	int group_y = base_y;
	int contents_x = base_x;
	int contents_y = base_y;

	// グループ
	if (group_flag)
	{
		ci.grp_koemode.create(_T(""), _T(""), group_x, group_y, 0, 0, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_koemode.set_text(LCL_STR_KOEMOD_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_koemode, group_flag ? 4 : 5);
		SendMessage(ci.grp_koemode.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	// コンテンツを配置
	p_x = contents_x;
	p_y = contents_y;

	// チェック
	width = font_get_width(p_dlg->get_handle(), LCL_STR_KOEMOD_DIALOG_KOE_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

	ci.rad_koemode_normal.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
	ci.rad_koemode_normal.set_text(LCL_STR_KOEMOD_DIALOG_KOE_ON);
	p_dlg->add_child(&ci.rad_koemode_normal, 7);
	SendMessage(ci.rad_koemode_normal.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_RADIO_H + DLG_CM_SPACE;

	width = font_get_width(p_dlg->get_handle(), LCL_STR_KOEMOD_DIALOG_KOE_OFF, font) + DLG_CM_CHKBOX_ADD_WIDTH;

	ci.rad_koemode_textonly.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
	ci.rad_koemode_textonly.set_text(LCL_STR_KOEMOD_DIALOG_KOE_OFF);
	p_dlg->add_child(&ci.rad_koemode_textonly, 7);
	SendMessage(ci.rad_koemode_textonly.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_RADIO_H + DLG_CM_SPACE;

	width = font_get_width(p_dlg->get_handle(), LCL_STR_KOEMOD_DIALOG_KOE_ONLY, font) + DLG_CM_CHKBOX_ADD_WIDTH;

	ci.rad_koemode_koeonly.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
	ci.rad_koemode_koeonly.set_text(LCL_STR_KOEMOD_DIALOG_KOE_ONLY);
	p_dlg->add_child(&ci.rad_koemode_koeonly, 7);
	SendMessage(ci.rad_koemode_koeonly.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_RADIO_H;

	// 統合ダイアログの場合
	if (func_mod == CFG_WND_FUNC_MOD_BASE_KOEMODE_CHRKOE || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
		// 高さは固定になる

		// コンテンツ下端へ
		p_y = contents_y + contents_height;

		// 初期化ボタン
		ci.btn_koemode_init.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W, p_y - DLG_CM_BTN_INIT_H, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_koemode_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_koemode_init, 1);
		SendMessage(ci.btn_koemode_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		// グループ下端へ
		p_y = group_y + group_height;

		// グループの大きさを設定
		if (group_flag)
		{
			ci.grp_koemode.set_window_size_with_def_size(group_width, group_height);
		}

		// ソロの場合
		if (func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
		{
			p_y += DLG_CM_SPACE;

			// 閉じるボタン
			ci.btn_koemode_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_koemode_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
			p_dlg->add_child(&ci.btn_koemode_close, 3);
			SendMessage(ci.btn_koemode_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

			p_y += DLG_CM_BTN_INIT_H;
		}

		// ベースとダイアログ高さが決定
		base_height = p_y - base_y;
		dialog_height = base_height + (DLG_CM_SOLO_PDG_T + DLG_CM_SOLO_PDG_B);

		// ダイアログを再配置
		refresh_dialog_func(dialog_width, dialog_height);
	}

	// 単一ダイアログの場合
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		// 高さは可変になる

		// グループの高さが決定
		group_height = p_y - group_y;

		// スペース
		p_y += DLG_CM_SPACE * 2;

		// フッタ
		ci.btn_koemode_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_koemode_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_koemode_close, 3);
		SendMessage(ci.btn_koemode_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_koemode_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_koemode_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_koemode_init, 3);
		SendMessage(ci.btn_koemode_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_INIT_H;

		// ベースの高さが決定
		base_height = p_y - base_y;

		// ダイアログの高さが決定
		dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;

		// ダイアログを再配置
		refresh_dialog_func(dialog_width, dialog_height);
	}


	/*
	// フッタボタン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_y += DLG_CM_SPACE;

	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
		p_y += DLG_CM_SPACE;

		ci.btn_koemode_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_koemode_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_koemode_close, 3);
		SendMessage(ci.btn_koemode_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_CLOSE_H;
	}

	// ソロの場合
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
	}
	*/

}

// 初期化
void Ccfg_wnd_func_koemode::on_init_funcfunc()
{
	// ダイアログを構築
	setup_dialog();

	/*
	// アイテム登録
	p_dlg->bind_control(&ci.grp_koemode, IDC_GRP_KOEMODE, _T(""), 4);

	p_dlg->bind_control(&ci.rad_koemode_normal, IDC_RAD_KOEMODE_NORMAL, _T(""), 7);
	p_dlg->bind_control(&ci.rad_koemode_textonly, IDC_RAD_KOEMODE_TEXTONLY, _T(""), 7);
	p_dlg->bind_control(&ci.rad_koemode_koeonly, IDC_RAD_KOEMODE_KOEONLY, _T(""), 7);

	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		p_dlg->bind_control(&ci.btn_koemode_init, IDC_BTN_KOEMODE_INIT, _T(""), 3);
	}
	else	{
		p_dlg->bind_control(&ci.btn_koemode_init, IDC_BTN_KOEMODE_INIT, _T(""), 1);
	}

	p_dlg->bind_control(&ci.btn_koemode_close, IDC_BTN_KOEMODE_CLOSE, _T(""), 3);

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_koemode_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	*/

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_koemode::on_close_funcfunc()
{
}

// コマンド
bool Ccfg_wnd_func_koemode::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.rad_koemode_normal.get_handle())
	{
		Gp_config->koe_mode = TNM_KOE_MODE_NORMAL;
		return true;
	}
	else if ((HWND)lp == ci.rad_koemode_textonly.get_handle())
	{
		Gp_config->koe_mode = TNM_KOE_MODE_TEXTONLY;
		return true;
	}
	else if ((HWND)lp == ci.rad_koemode_koeonly.get_handle())
	{
		Gp_config->koe_mode = TNM_KOE_MODE_KOEONLY;
		return true;
	}
	else if ((HWND)lp == ci.btn_koemode_init.get_handle())
	{
		init_state();
		return true;
	}
	else if ((HWND)lp == ci.btn_koemode_close.get_handle())
	{
		p_dlg->close();
		return true;
	}

	/*
	switch (LOWORD(wp))	{
		case IDC_RAD_KOEMODE_NORMAL:
			Gp_config->koe_mode = TNM_KOE_MODE_NORMAL;
			return true;

		case IDC_RAD_KOEMODE_TEXTONLY:
			Gp_config->koe_mode = TNM_KOE_MODE_TEXTONLY;
			return true;

		case IDC_RAD_KOEMODE_KOEONLY:
			Gp_config->koe_mode = TNM_KOE_MODE_KOEONLY;
			return true;

		case IDC_BTN_KOEMODE_INIT:
			init_state();
			return true;

		case IDC_BTN_KOEMODE_CLOSE:
			p_dlg->close();
			return true;
	}
	*/

	return false;
}

// 通知
bool Ccfg_wnd_func_koemode::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_koemode::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_koemode::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_koemode::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_koemode::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_koemode::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (Gp_config->koe_mode != get_radio_koemode())	{
		set_radio_koemode();
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_koemode::init_state()
{
	tnm_init_config_koe_mode();
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_koemode::update_dialog()
{
	if (!h_wnd)	{	return;	}
	set_radio_koemode();
}

// ****************************************************************
// 声モードのラジオボタンを設定
// ================================================================
void Ccfg_wnd_func_koemode::set_radio_koemode()
{
	if (!h_wnd)	{	return;	}
	ci.rad_koemode_normal.set_check(false);
	ci.rad_koemode_textonly.set_check(false);
	ci.rad_koemode_koeonly.set_check(false);
	switch (Gp_config->koe_mode)	{
		case TNM_KOE_MODE_NORMAL:		ci.rad_koemode_normal.set_check(true);		break;
		case TNM_KOE_MODE_TEXTONLY:		ci.rad_koemode_textonly.set_check(true);	break;
		case TNM_KOE_MODE_KOEONLY:		ci.rad_koemode_koeonly.set_check(true);		break;
	}
}

// ****************************************************************
// 声モードのラジオボタンを取得
// ================================================================
int Ccfg_wnd_func_koemode::get_radio_koemode()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_koemode_normal.get_check())			{	return TNM_KOE_MODE_NORMAL;		}
	else if (ci.rad_koemode_textonly.get_check())	{	return TNM_KOE_MODE_TEXTONLY;	}
	else if (ci.rad_koemode_koeonly.get_check())	{	return TNM_KOE_MODE_KOEONLY;	}
	return -1;
}

