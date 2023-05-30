#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_namae.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_chrkoe.h"
#include	"dialog/cfg_wnd_common.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：キャラクター音声
// ================================================================

#define		CHARAITEM_HEIGHT				26
#define		CHARAITEM_BASE_X				8
#define		CHARAITEM_BASE_Y				1
#define		CHARAITEM_SPACE_X				4
#define		CHARAITEM_SPACE_Y				1
#define		CHARAITEM_NEXT					(CHARAITEM_HEIGHT + CHARAITEM_SPACE_Y)

#define		CHARAITEM_CHECKBOX_HEIGHT		14
#define		CHARAITEM_CHECKBOX_WIDTH_ADD	20
#define		CHARAITEM_SLIDER_HEIGHT			CHARAITEM_HEIGHT

// アイテムを構築
void Ccfg_wnd_func_chrkoe::setup_dialog()
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
		p_dlg->set_text(LCL_STR_CHRKOE_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// 音声モードグループの幅
	int koemod_group_width = 160;
	if (wide_koe_wnd_flag)
	{
		koemod_group_width += 100;
	}

	// マージン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_KOEMODE_CHRKOE)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X + (koemod_chrkoe_flag ? koemod_group_width + DLG_CM_SPACE : 0);
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_KOEMODE_CHRKOE)
	{
		dialog_pdg_x = DLG_CM_BASE_PDG_X + (koemod_chrkoe_flag ? koemod_group_width + DLG_CM_SPACE : 0);
		dialog_pdg_t = DLG_CM_BASE_PDG_T;
		dialog_pdg_b = DLG_CM_BASE_PDG_B;
	}

	// グループの幅は固定です。
	group_width = (func_mod == CFG_WND_FUNC_MOD_SOLO ? 384 : 288);
	if (wide_koe_wnd_flag)
	{
		group_width += 160;
	}
	base_width = group_width + (koemod_chrkoe_flag ? DLG_CM_SPACE + koemod_group_width : 0);
	dialog_width = base_width + DLG_CM_SOLO_PDG_X * 2;
	contents_width = group_width - (group_flag ? DLG_CM_GROUP_PDG_X * 2 : 0);

	// グループの高さは固定です。
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
		ci.grp_chrkoe.create(_T(""), _T(""), group_x, group_y, 0, 0, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_chrkoe.set_text(LCL_STR_CHRKOE_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_chrkoe, 5);
		SendMessage(ci.grp_chrkoe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	// コンテンツを配置
	p_x = contents_x;
	p_y = contents_y;

	// サブリスト
	ci.lst_chrkoe.create(_T(""), _T(""), p_x, p_y, contents_width, contents_height, false, WS_CHILD | WS_VISIBLE | SS_SUNKEN, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.lst_chrkoe, 5);
	SendMessage(ci.lst_chrkoe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	ci.scr_chrkoe.create(_T(""), _T(""), p_x + contents_width - DLG_CM_SCROLL_WIDTH - 1, p_y + 1, DLG_CM_SCROLL_WIDTH, contents_height - 2, false, WS_CHILD | WS_VISIBLE | SBS_VERT, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.scr_chrkoe, 6);
	SendMessage(ci.scr_chrkoe.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	// キャラクター作成
	ci.lst_chrkoe.p_parent_class = this;
	work.character_list_scrool_pos = -999;
	work.character_list_width = -999;
	work.character_cnt = 0;
	C_tnm_chrkoe* p_chrkoe = NULL;
	if (!Gp_chrkoe->empty())
	{
		for (int i = 0; i < (int)Gp_chrkoe->size(); i++)
		{
			p_chrkoe = &(*Gp_chrkoe)[i];
			if (!p_chrkoe->name_str.empty())	{ work.character_cnt++; }
		}
	}
	if (work.character_cnt > 0)
	{
		work.character.resize(work.character_cnt);
		int chara_index = 0;
		for (int i = 0; i < (int)Gp_chrkoe->size(); i++)
		{
			p_chrkoe = &(*Gp_chrkoe)[i];
			if (!p_chrkoe->name_str.empty())	{ work.character[chara_index++].ini_no = i; }
		}

		work.character_chkbox_max_width = 0;
		for (int i = 0; i < work.character_cnt; i++)
		{
			p_chrkoe = &(*Gp_chrkoe)[work.character[i].ini_no];

			// チェックボックス作成
			work.character[i].item_chk.create(_T(""), _T(""), 0, 0, 10, 10, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_AUTOCHECKBOX | BS_LEFTTEXT, WS_EX_RIGHT, false, false, ci.lst_chrkoe.get_handle());
			SendMessage(work.character[i].item_chk.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
			if (p_chrkoe->look_flag)
			{
				work.character[i].item_chk.set_text(p_chrkoe->name_str);
			}
			else
			{
				work.character[i].item_chk.set_text(Gp_ini->config.chrkoe_not_look_name_str);
			}
			work.character[i].item_chk.p_parent_class = this;

			// 項目文字列の幅を取得
			width = work.character[i].item_chk.get_adjust_width_from_text(CHARAITEM_CHECKBOX_WIDTH_ADD);
			if (work.character_chkbox_max_width < width)	{ work.character_chkbox_max_width = width; }

			// スライダー作成
			work.character[i].item_sld.create(_T(""), _T(""), 0, 0, 10, CHARAITEM_SLIDER_HEIGHT, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | TBS_HORZ | TBS_BOTH | TBS_NOTICKS, 0, false, false, ci.lst_chrkoe.get_handle());
			work.character[i].item_sld.p_parent_class = this;
			work.character[i].item_sld.set_slider_range(0, 255);
		}
	}
	else	{ ci.scr_chrkoe.set_show(false); }	// スクロールバーを消す

	resize_character_list();
	ci.scr_chrkoe.set_scroll_pos(0);


	if (group_flag)
	{
		// リストの高さ
		int list_height = contents_height - DLG_CM_BTN_INIT_H - DLG_CM_SPACE;

		// リストの大きさを設定
		ci.lst_chrkoe.set_window_size_with_def_size(contents_width, list_height);
		ci.scr_chrkoe.set_window_size_with_def_size(DLG_CM_SCROLL_WIDTH, list_height);

		// コンテンツ下端へ
		p_y = contents_y + contents_height;

		// 初期化ボタン
		if (wide_koe_wnd_flag)
		{
			width = 152;
		}
		else
		{
			width = 72;
		}

		ci.btn_chrkoe_init.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W, p_y - DLG_CM_BTN_INIT_H, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_chrkoe_init, 3);
		SendMessage(ci.btn_chrkoe_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_chrkoe_all_off.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W - DLG_CM_SPACE - width, p_y - DLG_CM_BTN_INIT_H, width, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_all_off.set_text(LCL_STR_CHRKOE_DIALOG_ALL_OFF);
		p_dlg->add_child(&ci.btn_chrkoe_all_off, 3);
		SendMessage(ci.btn_chrkoe_all_off.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_chrkoe_all_on.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W - DLG_CM_SPACE - width - DLG_CM_SPACE - width, p_y - DLG_CM_BTN_INIT_H, width, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_all_on.set_text(LCL_STR_CHRKOE_DIALOG_ALL_ON);
		p_dlg->add_child(&ci.btn_chrkoe_all_on, 3);
		SendMessage(ci.btn_chrkoe_all_on.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		// グループ下端へ
		p_y = group_y + group_height;

		// グループの大きさを設定
		ci.grp_chrkoe.set_window_size_with_def_size(group_width, group_height);

		// ベースの高さが決定
		base_height = p_y - base_y;

		// ダイアログの高さが決定
		dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;
	}
	else
	{
		// リストの高さ
		int list_height = contents_height;

		// リストの大きさを設定
		ci.lst_chrkoe.set_window_size_with_def_size(contents_width, list_height);
		ci.scr_chrkoe.set_window_size_with_def_size(DLG_CM_SCROLL_WIDTH, list_height - 2);

		// グループ下端へ
		p_y = group_y + group_height;

		// フッタ
		p_y += DLG_CM_SPACE * 2;

		if (wide_koe_wnd_flag)
		{
			width = 152;
		}
		else
		{
			width = 72;
		}

		ci.btn_chrkoe_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_chrkoe_close, 3);
		SendMessage(ci.btn_chrkoe_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_chrkoe_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_chrkoe_init, 3);
		SendMessage(ci.btn_chrkoe_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_chrkoe_all_off.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W - DLG_CM_SPACE - width, p_y, width, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_all_off.set_text(LCL_STR_CHRKOE_DIALOG_ALL_OFF);
		p_dlg->add_child(&ci.btn_chrkoe_all_off, 3);
		SendMessage(ci.btn_chrkoe_all_off.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_chrkoe_all_on.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W - DLG_CM_SPACE - width - DLG_CM_SPACE - width, p_y, width, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_chrkoe_all_on.set_text(LCL_STR_CHRKOE_DIALOG_ALL_ON);
		p_dlg->add_child(&ci.btn_chrkoe_all_on, 3);
		SendMessage(ci.btn_chrkoe_all_on.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_INIT_H;

		// ベースの高さが決定
		base_height = p_y - base_y;

		// ダイアログの高さが決定
		dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;
	}

	// ダイアログを再配置
	if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		refresh_dialog_func(dialog_width, dialog_height);
	}
}

// 初期化
void Ccfg_wnd_func_chrkoe::on_init_funcfunc()
{
	// ダイアログを構築
	setup_dialog();

	/*
	// アイテム登録
	p_dlg->bind_control(&ci.grp_chrkoe, IDC_GRP_CHRKOE, _T(""), 5);

	p_dlg->bind_control(&ci.lst_chrkoe, IDC_LST_CHRKOE, _T(""), 5);
	p_dlg->bind_control(&ci.scr_chrkoe, IDC_SCR_CHRKOE, _T(""), 6);
	p_dlg->bind_control(&ci.btn_chrkoe_all_on, IDC_BTN_CHRKOE_ALL_ON, _T(""), 3);
	p_dlg->bind_control(&ci.btn_chrkoe_all_off, IDC_BTN_CHRKOE_ALL_OFF, _T(""), 3);
	p_dlg->bind_control(&ci.btn_chrkoe_init, IDC_BTN_CHRKOE_INIT, _T(""), 3);

	p_dlg->bind_control(&ci.btn_chrkoe_close, IDC_BTN_CHRKOE_CLOSE, _T(""), 3);

	// ▲▲▲コンフィグのキャラクター音声（ソロ版の場合、キャラクター項目数に応じて、初期のウィンドウサイズを調整するべきか？）

	// キャラクター作成
	ci.lst_chrkoe.p_parent_class = this;
	work.character_list_scrool_pos = -999;
	work.character_list_width = -999;
	work.character_cnt = 0;
	C_tnm_chrkoe* p_chrkoe = NULL;
	if (!Gp_chrkoe->empty())	{
		for (int i = 0; i < (int)Gp_chrkoe->size(); i++)	{
			p_chrkoe = &(*Gp_chrkoe)[i];
			if (!p_chrkoe->name_str.empty())	{	work.character_cnt++;	}
		}
	}
	if (work.character_cnt > 0)	{
		work.character.resize(work.character_cnt);
		int chara_index = 0;
		for (int i = 0; i < (int)Gp_chrkoe->size(); i++)	{
			p_chrkoe = &(*Gp_chrkoe)[i];
			if (!p_chrkoe->name_str.empty())	{	work.character[chara_index++].ini_no = i;	}
		}
		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		int width;
		work.character_chkbox_max_width = 0;
		for (int i = 0; i < work.character_cnt; i++)	{
			p_chrkoe = &(*Gp_chrkoe)[work.character[i].ini_no];

			// チェックボックス作成
			work.character[i].item_chk.create(_T(""), _T(""), 0, 0, 10, 10, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_AUTOCHECKBOX | BS_LEFTTEXT, WS_EX_RIGHT, false, false, ci.lst_chrkoe.get_handle());
			SendMessage(work.character[i].item_chk.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
			if (p_chrkoe->look_flag)	{
				work.character[i].item_chk.set_text(p_chrkoe->name_str);
			}
			else	{
				work.character[i].item_chk.set_text(Gp_ini->config.chrkoe_not_look_name_str);
			}
			work.character[i].item_chk.p_parent_class = this;

			// 項目文字列の幅を取得
			width = work.character[i].item_chk.get_adjust_width_from_text(CHARAITEM_CHECKBOX_WIDTH_ADD);
			if (work.character_chkbox_max_width < width)	{	work.character_chkbox_max_width = width;	}

			// スライダー作成
			work.character[i].item_sld.create(_T(""), _T(""), 0, 0, 10, CHARAITEM_SLIDER_HEIGHT, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | TBS_HORZ | TBS_BOTH | TBS_NOTICKS, 0, false, false, ci.lst_chrkoe.get_handle());
			work.character[i].item_sld.p_parent_class = this;
			work.character[i].item_sld.set_slider_range(0, 255);
		}
	}
	else	{	ci.scr_chrkoe.set_show(false);	}	// スクロールバーを消す

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_chrkoe_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	resize_character_list();
	ci.scr_chrkoe.set_scroll_pos(0);
	*/

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_chrkoe::on_close_funcfunc()
{
	// キャラクター開放
	work.character.clear();
}

// コマンド
bool Ccfg_wnd_func_chrkoe::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.btn_chrkoe_all_on.get_handle())
	{
		if (work.character_cnt > 0)
		{
			S_tnm_config_chrkoe* p_chrkoe;
			for (int i = 0; i < work.character_cnt; i++)
			{
				p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
				p_chrkoe->onoff = true;
			}
			update_dialog();
		}
		return true;
	}
	else if ((HWND)lp == ci.btn_chrkoe_all_off.get_handle())
	{
		if (work.character_cnt > 0)
		{
			S_tnm_config_chrkoe* p_chrkoe;
			for (int i = 0; i < work.character_cnt; i++)
			{
				p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
				p_chrkoe->onoff = false;
			}
			update_dialog();
		}
		return true;
	}
	else if ((HWND)lp == ci.btn_chrkoe_init.get_handle())
	{
		init_state();
		return true;
	}
	else if ((HWND)lp == ci.btn_chrkoe_close.get_handle())
	{
		p_dlg->close();
		return true;
	}

	/*
	switch (LOWORD(wp))	{
		case IDC_BTN_CHRKOE_ALL_ON:
			if (work.character_cnt > 0)	{
				S_tnm_config_chrkoe* p_chrkoe;
				for (int i = 0; i < work.character_cnt; i++)	{
					p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
					p_chrkoe->onoff = true;
				}
				update_dialog();
			}
			return true;

		case IDC_BTN_CHRKOE_ALL_OFF:
			if (work.character_cnt > 0)	{
				S_tnm_config_chrkoe* p_chrkoe;
				for (int i = 0; i < work.character_cnt; i++)	{
					p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
					p_chrkoe->onoff = false;
				}
				update_dialog();
			}
			return true;

		case IDC_BTN_CHRKOE_INIT:
			init_state();
			return true;

		case IDC_BTN_CHRKOE_CLOSE:
			p_dlg->close();
			return true;
	}
	*/

	return false;
}

// 通知
bool Ccfg_wnd_func_chrkoe::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_chrkoe::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	on_mousewheel_character_list(wp, lp);	return true;
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_chrkoe::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_chrkoe::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.scr_chrkoe.get_handle())	{
		ci.scr_chrkoe.scroll_natural(wp);
		update_character_list();
		return true;
	}
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_chrkoe::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_chrkoe::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (work.character_cnt > 0)	{
		bool name_change = false;
		S_tnm_config_chrkoe* p_config;
		C_tnm_chrkoe* p_chrkoe;
		for (int i = 0; i < work.character_cnt; i++)	{
			WORK::CHARACTER_STRUCT* p_work = &work.character[i];
			p_config = &Gp_config->chrkoe[work.character[i].ini_no];
			if (p_config->onoff != work.character[i].item_chk.get_check())	{
				work.character[i].item_chk.set_check(p_config->onoff);
			}
			if (p_config->volume != work.character[i].item_sld.get_slider_pos())	{
				work.character[i].item_sld.set_slider_pos(p_config->volume);
			}
			// 名前が変更された
			p_chrkoe = &(*Gp_chrkoe)[work.character[i].ini_no];
			if (p_chrkoe->look_flag)	{
				if (p_chrkoe->name_str != work.character[i].item_chk.get_text())	{
					work.character[i].item_chk.set_text(p_chrkoe->name_str);
					name_change = true;
				}
			}
			else	{
				if (Gp_ini->config.chrkoe_not_look_name_str != work.character[i].item_chk.get_text())	{
					work.character[i].item_chk.set_text(Gp_ini->config.chrkoe_not_look_name_str);
					name_change = true;
				}
			}
		}
		// 名前が変更された
		if (name_change)	{
			int width;
			work.character_list_width = -999;
			for (int i = 0; i < work.character_cnt; i++)	{
				width = work.character[i].item_chk.get_adjust_width_from_text(CHARAITEM_CHECKBOX_WIDTH_ADD);
				if (work.character_chkbox_max_width < width)	{	work.character_chkbox_max_width = width;	}
			}
			update_character_list();
			ci.scr_chrkoe.invalidate_rect(NULL, false);
		}
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_chrkoe::init_state()
{
	tnm_init_config_chrkoe();
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_chrkoe::update_dialog()
{
	if (!h_wnd)	{	return;	}

	if (work.character_cnt <= 0)	{	return;	}
	S_tnm_config_chrkoe* p_chrkoe;
	for (int i = 0; i < work.character_cnt; i++)	{
		p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
		work.character[i].item_chk.set_check(p_chrkoe->onoff);
		work.character[i].item_sld.set_slider_pos(p_chrkoe->volume);
	}
}

// ****************************************************************
// スクロールバーにフォーカスを合わせる
// ================================================================
void Ccfg_wnd_func_chrkoe::focus_scrollbar()
{
	ci.scr_chrkoe.set_focus();
}

// ****************************************************************
// キャラクターリストの更新
// ================================================================
void Ccfg_wnd_func_chrkoe::update_character_list()
{
	if (work.character_cnt <= 0)	{	return;	}
	int pos = ci.scr_chrkoe.get_scroll_pos();
	int list_width = ci.lst_chrkoe.get_client_rect().width() - ((ci.scr_chrkoe.get_show()) ? ci.scr_chrkoe.get_client_rect().width() : 0);
	if (pos == work.character_list_scrool_pos && list_width == work.character_list_width)	{	return;	}
	int item_pos_x = CHARAITEM_BASE_X;
	int item_pos_y = CHARAITEM_BASE_Y - (pos * CHARAITEM_NEXT);
	int y_alpha;

	int slider_x = item_pos_x + work.character_chkbox_max_width + CHARAITEM_SPACE_X;
	int slider_width = list_width - (slider_x + CHARAITEM_SPACE_X);

	for (int i = 0; i < work.character_cnt; i++)	{

		// チェックボックスの座標とサイズを設定
		y_alpha = (CHARAITEM_HEIGHT - CHARAITEM_CHECKBOX_HEIGHT) / 2;
		work.character[i].item_chk.set_window_pos(C_point(item_pos_x, item_pos_y + y_alpha));
		work.character[i].item_chk.set_window_size(C_size(work.character_chkbox_max_width, CHARAITEM_CHECKBOX_HEIGHT));

		// スライダーの座標とサイズを設定
		work.character[i].item_sld.set_window_pos(C_point(slider_x, item_pos_y));
		work.character[i].item_sld.set_window_size(C_size(slider_width, CHARAITEM_SLIDER_HEIGHT));

		item_pos_y += CHARAITEM_NEXT;
	}
	work.character_list_scrool_pos = pos;
	work.character_list_width = list_width;
}

// ****************************************************************
// キャラクターリストのリサイズ
// ================================================================
void Ccfg_wnd_func_chrkoe::resize_character_list()
{
	if (work.character_cnt <= 0)	{	return;	}
	int area_cnt = (ci.lst_chrkoe.get_client_rect().height() - CHARAITEM_BASE_Y) / CHARAITEM_NEXT;
	ci.scr_chrkoe.set_scroll_info(0, work.character_cnt, area_cnt + 1);
	ci.scr_chrkoe.set_show((area_cnt < work.character_cnt) ? true : false);
	update_character_list();
}

// ****************************************************************
// キャラクターリストのマウスホイール
// ================================================================
void Ccfg_wnd_func_chrkoe::on_mousewheel_character_list(WPARAM wp, LPARAM lp)
{
	if (work.character_cnt <= 0)	{	return;	}
	int delta = GET_WHEEL_DELTA_WPARAM(wp);
	if (delta == 0)	{	return;	}
	int pos = ci.scr_chrkoe.get_scroll_pos();
	if (delta > 0)	{	pos -= 1;	}
	else			{	pos += 1;	}
	ci.scr_chrkoe.set_scroll_pos(pos);
	update_character_list();
}

// ****************************************************************
// キャラクターリストのコマンド
// ================================================================
bool Ccfg_wnd_func_chrkoe::on_command_character_list(WPARAM wp, LPARAM lp)
{
	if (work.character_cnt <= 0)	{	return false;	}
	S_tnm_config_chrkoe* p_chrkoe;
	for (int i = 0; i < work.character_cnt; i++)	{
		if ((HWND)lp == work.character[i].item_chk.get_handle())	{
			p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
			p_chrkoe->onoff = work.character[i].item_chk.get_check();
			return true;
		}
	}

	return false;
}

// ****************************************************************
// キャラクターリストのＨスクロール
// ================================================================
bool Ccfg_wnd_func_chrkoe::on_h_scroll_character_list(WPARAM wp, LPARAM lp)
{
	if (work.character_cnt <= 0)	{	return false;	}
	S_tnm_config_chrkoe* p_chrkoe;
	for (int i = 0; i < work.character_cnt; i++)	{
		if ((HWND)lp == work.character[i].item_sld.get_handle())	{
			p_chrkoe = &Gp_config->chrkoe[work.character[i].ini_no];
			p_chrkoe->volume = work.character[i].item_sld.get_slider_pos();
			return true;
		}
	}

	return false;
}

// ****************************************************************
// キャラクターリスト：ウィンドウプロシージャ
// ================================================================
LRESULT Ccfg_wnd_func_chrkoe::Cc_chalst::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_COMMAND:	p_parent_class->on_command_character_list(wp, lp);	break;
		case WM_HSCROLL:	p_parent_class->on_h_scroll_character_list(wp, lp);	break;
		case WM_SIZE:		p_parent_class->resize_character_list();			break;
	}
	return Cc_static::window_proc(msg, wp, lp);
}

// ****************************************************************
// キャラクターチェックボックス：ウィンドウプロシージャ
// ================================================================
LRESULT Ccfg_wnd_func_chrkoe::Cc_character_chk::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
//	switch (msg)	{
//	}
	return Cc_check::window_proc(msg, wp, lp);
}

// ****************************************************************
// キャラクタースライダー：ウィンドウプロシージャ
// ================================================================
LRESULT Ccfg_wnd_func_chrkoe::Cc_character_sld::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SETFOCUS:	p_parent_class->focus_scrollbar();	break;
	}
	return Cc_slider::window_proc(msg, wp, lp);
}

