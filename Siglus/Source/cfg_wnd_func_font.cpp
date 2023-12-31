#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_moji_manager.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_font.h"
#include	"dialog/cfg_wnd_common.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_font.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：フォント
// ================================================================

// アイテムを構築
void C_cfg_wnd_func_font::setup_dialog()
{
	int p_x, p_y;

	bool font_msgspd = (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD || func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD);

	// ダイアログタイトル
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_FONT_DIALOG_DIALOG_TITLE);
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		p_dlg->set_text(LCL_STR_FONT_MSGSPD_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// 全パターンに置いて、高さは可変です
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;
	int base_width, base_height;
	int group_width, group_height;
	int contents_width, contents_height;

	// マージン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO || func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		dialog_pdg_x = DLG_CM_SOLO_PDG_X;
		dialog_pdg_t = DLG_CM_SOLO_PDG_T;
		dialog_pdg_b = DLG_CM_SOLO_PDG_B;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_BASE || func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD)
	{
		dialog_pdg_x = DLG_CM_BASE_PDG_X;
		dialog_pdg_t = DLG_CM_BASE_PDG_T;
		dialog_pdg_b = DLG_CM_BASE_PDG_B;
	}

	// 幅は固定
	dialog_width = 480;
	base_width = dialog_width - DLG_CM_SOLO_PDG_X * 2;
	group_width = base_width;
	contents_width = group_width - (font_msgspd ? DLG_CM_GROUP_PDG_X * 2 : 0);

	// ベース位置
	int base_x = dialog_pdg_x;
	int base_y = dialog_pdg_t;
	int group_x = base_x;
	int group_y = base_y;
	int contents_x = base_x;
	int contents_y = base_y;

	// グループ
	if (font_msgspd)
	{
		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;

		// 大きさは仮
		ci.grp_font.create(_T(""), _T(""), group_x, group_y, 0, 0, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_font.set_text(LCL_STR_FONT_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_font, 5);
		SendMessage(ci.grp_font.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// コンテンツを配置
	p_x = contents_x;
	p_y = contents_y;

	int list_height = (font_msgspd) ? 48 : 216;
	int language_offset_height = 0;			// 言語によって高さを変動させる一時変数

	ci.lst_font.create(_T(""), _T(""), p_x, p_y, contents_width, list_height, false, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.lst_font, 5);
	SendMessage(ci.lst_font.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += list_height + DLG_CM_SPACE * 2;

	ci.chk_proportional.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_proportional.set_text(LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF);
	p_dlg->add_child(&ci.chk_proportional, 1);
	SendMessage(ci.chk_proportional.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_CHKBOX_H;

	// フランス語とインドネシア語は次の項目で２行分の高さを確保する
#if defined(__SIGLUS_FR) || defined(__SIGLUS_ID)
	language_offset_height = DLG_CM_CHKBOX_H;
#endif

	ci.stc_proportional.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_CHKBOX_H + language_offset_height, false, WS_CHILD | WS_VISIBLE, 0, false, false, h_wnd);
	ci.stc_proportional.set_text(LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC);
	p_dlg->add_child(&ci.stc_proportional, 1);
	SendMessage(ci.stc_proportional.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_CHKBOX_H + language_offset_height + DLG_CM_SPACE;

	// 文字装飾
	{
		int width;

		width = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_BOLD, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.chk_font_futoku.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_font_futoku.set_text(LCL_STR_FONT_DIALOG_STC_BOLD);
		p_dlg->add_child(&ci.chk_font_futoku, 1);
		SendMessage(ci.chk_font_futoku.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		if (wide_language_flag)
		{
			p_x = contents_x;
			p_y += DLG_CM_CHKBOX_H + DLG_CM_SPACE;
		}
		else
		{
			p_x += width + DLG_CM_SPACE;
		}

		width = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_OFF, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_none.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_none.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_OFF);
		p_dlg->add_child(&ci.rad_font_shadow_none, 1);
		SendMessage(ci.rad_font_shadow_none.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += width + DLG_CM_SPACE;

		width = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_shadow.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_shadow.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_ON);
		p_dlg->add_child(&ci.rad_font_shadow_shadow, 1);
		SendMessage(ci.rad_font_shadow_shadow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		if (wide_language_flag)
		{
			p_x = contents_x;
			p_y += DLG_CM_CHKBOX_H + DLG_CM_SPACE;
		}
		else
		{
			p_x += width + DLG_CM_SPACE;
		}

		width = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_BORDER_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_fuchidoru.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_fuchidoru.set_text(LCL_STR_FONT_DIALOG_STC_BORDER_ON);
		p_dlg->add_child(&ci.rad_font_shadow_fuchidoru, 1);
		SendMessage(ci.rad_font_shadow_fuchidoru.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += width + DLG_CM_SPACE;

		width = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_fuchidoru_shadow.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_fuchidoru_shadow.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON);
		p_dlg->add_child(&ci.rad_font_shadow_fuchidoru_shadow, 1);
		SendMessage(ci.rad_font_shadow_fuchidoru_shadow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		// 初期化ボタン
		if (font_msgspd)
		{
			ci.btn_font_init.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_font_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
			p_dlg->add_child(&ci.btn_font_init, 3);
			SendMessage(ci.btn_font_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

			p_y += DLG_CM_BTN_INIT_H;
		}
		else
		{
			p_y += DLG_CM_RADIO_H;
		}
	}

	p_x = contents_x;

	// フォントのみの場合はここでサンプル表示
	if (!font_msgspd)
	{
		p_y += DLG_CM_SPACE;

		TSTR text = Gp_ini->config.font_sample_str_short;

		ci.stc_font_sample.create(_T(""), _T(""), p_x, p_y, contents_width, 32, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_STATICEDGE, false, false, h_wnd);
		ci.stc_font_sample.set_text(text);
		p_dlg->add_child(&ci.stc_font_sample, 1);
		SendMessage(ci.stc_font_sample.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += 32;
	}

	// コンテンツの高さが決定
	contents_height = p_y - contents_y;

	// グループ下
	if (font_msgspd)
	{
		p_y += DLG_CM_GROUP_PDG_B;
	}

	// グループの高さが決定
	group_height = p_y - group_y;

	// グループの大きさを設定
	ci.grp_font.set_window_size_with_def_size(group_width, group_height);

	// スペース
	p_y += DLG_CM_SPACE * 2;

	// メッセージ速度ダイアログ
	if (font_msgspd)
	{
		int msgspd_group_height = 160;

		p_y += msgspd_group_height;
	}

	// フッタボタン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_y += DLG_CM_SPACE;

		ci.btn_font_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_font_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_font_close, 3);
		SendMessage(ci.btn_font_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_font_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_font_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_font_init, 3);
		SendMessage(ci.btn_font_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_CLOSE_H;
	}
	else if (func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		p_y += DLG_CM_SPACE;

		ci.btn_font_close.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_font_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_font_close, 3);
		SendMessage(ci.btn_font_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_CLOSE_H;
	}

	// ベースの高さが決定
	base_height = p_y - base_y;

	// ダイアログの高さが決定
	dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;

	// ダイアログを再配置
	refresh_dialog_func(dialog_width, dialog_height);

	/*
	// 固定サイズ
	int dialog_width = 480;
	int dialog_height = 400;

	// 全体グループ
	int group_width = dialog_width - DLG_CM_SOLO_PDG_X * 2;
	int group_height = dialog_height - (DLG_CM_SOLO_PDG_T + DLG_CM_SOLO_PDG_B);

	// 合同の場合は固定
	if (font_msgspd)
	{
		group_height = 200;
	}

	// コンテンツ
	int contents_width = group_width - (font_msgspd ? DLG_CM_GROUP_PDG_X * 2 : 0);
	int contents_height = group_height - (font_msgspd ? DLG_CM_GROUP_PDG_T + DLG_CM_SPACE + DLG_CM_GROUP_PDG_B : 0);

	// 閉じるボタンと初期設定に戻すボタン
	int close_btn_x = dialog_width - DLG_CM_SOLO_PDG_X - DLG_CM_BTN_CLOSE_W;
	int close_btn_y = dialog_height - DLG_CM_SOLO_PDG_B - DLG_CM_BTN_CLOSE_H;
	int init_btn_x = dialog_width - DLG_CM_SOLO_PDG_X - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W;
	int init_btn_y = dialog_height - DLG_CM_SOLO_PDG_B - DLG_CM_BTN_INIT_H;

	// コントロールを配置
	int base_x = DLG_CM_SOLO_PDG_X;
	int base_y = DLG_CM_SOLO_PDG_T;
	int contents_x = base_x;
	int contents_y = base_y;

	if (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD || func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD)
	{
		ci.grp_font.create(_T(""), _T(""), base_x, base_y, group_width, group_height, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_font.set_text(LCL_STR_FONT_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_font, 5);
		SendMessage(ci.grp_font.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	p_x = contents_x;
	p_y = contents_y;
	
	int list_height = (func_mod == CFG_WND_FUNC_MOD_SOLO) ? 216 : 48;

	ci.lst_font.create(_T(""), _T(""), p_x, p_y, contents_width, list_height, false, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.lst_font, 5);
	SendMessage(ci.lst_font.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += list_height + DLG_CM_SPACE * 2;

	ci.chk_proportional.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	ci.chk_proportional.set_text(LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_ONOFF);
	p_dlg->add_child(&ci.chk_proportional, 1);
	SendMessage(ci.chk_proportional.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_CHKBOX_H;

	ci.stc_proportional.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE, 0, false, false, h_wnd);
	ci.stc_proportional.set_text(LCL_STR_FONT_DIALOG_STC_PROPORTIONAL_DESC);
	p_dlg->add_child(&ci.stc_proportional, 1);
	SendMessage(ci.stc_proportional.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_CHKBOX_H + DLG_CM_SPACE;

	{
		int w;
		int check_base_x = p_x;

		w = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_BOLD, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.chk_font_futoku.create(_T(""), _T(""), p_x, p_y, w, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_font_futoku.set_text(LCL_STR_FONT_DIALOG_STC_BOLD);
		p_dlg->add_child(&ci.chk_font_futoku, 1);
		SendMessage(ci.chk_font_futoku.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += w + DLG_CM_SPACE;

		w = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_OFF, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_none.create(_T(""), _T(""), p_x, p_y, w, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_none.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_OFF);
		p_dlg->add_child(&ci.rad_font_shadow_none, 1);
		SendMessage(ci.rad_font_shadow_none.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += w + DLG_CM_SPACE;

		w = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_shadow.create(_T(""), _T(""), p_x, p_y, w, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_shadow.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_ON);
		p_dlg->add_child(&ci.rad_font_shadow_shadow, 1);
		SendMessage(ci.rad_font_shadow_shadow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += w + DLG_CM_SPACE;

		w = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_BORDER_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_fuchidoru.create(_T(""), _T(""), p_x, p_y, w, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_fuchidoru.set_text(LCL_STR_FONT_DIALOG_STC_BORDER_ON);
		p_dlg->add_child(&ci.rad_font_shadow_fuchidoru, 1);
		SendMessage(ci.rad_font_shadow_fuchidoru.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_x += w + DLG_CM_SPACE;

		w = font_get_width(p_dlg->get_handle(), LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.rad_font_shadow_fuchidoru_shadow.create(_T(""), _T(""), p_x, p_y, w, DLG_CM_RADIO_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
		ci.rad_font_shadow_fuchidoru_shadow.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON);
		p_dlg->add_child(&ci.rad_font_shadow_fuchidoru_shadow, 1);
		SendMessage(ci.rad_font_shadow_fuchidoru_shadow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	p_x = contents_x;

	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_y += DLG_CM_CHKBOX_H + DLG_CM_SPACE;

		ci.stc_font_sample.create(_T(""), _T(""), p_x, p_y, contents_width, 32, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_STATICEDGE, false, false, h_wnd);
		ci.stc_font_sample.set_text(LCL_STR_FONT_DIALOG_STC_SHADOW_BORDER_ON);
		p_dlg->add_child(&ci.stc_font_sample, 1);
		SendMessage(ci.stc_font_sample.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// 共通のフッタボタン
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		ci.btn_font_close.create(_T(""), _T(""), close_btn_x, close_btn_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_font_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_font_close, 3);
		SendMessage(ci.btn_font_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_font_init.create(_T(""), _T(""), init_btn_x, init_btn_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_font_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_font_init, 3);
		SendMessage(ci.btn_font_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	}

	// ダイアログを再配置
	refresh_dialog_func(dialog_width, dialog_height);
	*/
}

// 初期化
void C_cfg_wnd_func_font::on_init_funcfunc()
{
	work.frame_first_flag = true;
	work.now_text.clear();

	// 作業用フォント初期化
	init_work_font();

	// ダイアログを構築
	setup_dialog();

	// アイテム登録
//	p_dlg->bind_control(&ci.grp_font, IDC_GRP_FONT, _T(""), 5);
//	p_dlg->bind_control(&ci.lst_font, IDC_LST_FONT, _T(""), 5);
//	p_dlg->bind_control(&ci.chk_proportional, IDC_CHK_PROPORTIONAL, _T(""), 1);
//	p_dlg->bind_control(&ci.stc_proportional, IDC_STC_PROPORTIONAL, _T(""), 1);
//	p_dlg->bind_control(&ci.chk_font_futoku, IDC_CHK_FONT_FUTOKU, _T(""), 1);
//	p_dlg->bind_control(&ci.rad_font_shadow_none, IDC_RAD_FONT_SHADOW_NONE, _T(""), 1);
//	p_dlg->bind_control(&ci.rad_font_shadow_shadow, IDC_RAD_FONT_SHADOW_SHADOW, _T(""), 1);
//	p_dlg->bind_control(&ci.rad_font_shadow_fuchidoru, IDC_RAD_FONT_SHADOW_FUTIDORU, _T(""), 1);
//	p_dlg->bind_control(&ci.rad_font_shadow_fuchidoru_shadow, IDC_RAD_FONT_SHADOW_FUTIDORU_SHADOW, _T(""), 1);
//	p_dlg->bind_control(&ci.btn_font_init, IDC_BTN_FONT_INIT, _T(""), 3);
//	p_dlg->bind_control(&ci.stc_font_sample, IDC_STC_FONT_SAMPLE, _T(""), 2);
//	p_dlg->bind_control(&ci.btn_font_close, IDC_BTN_FONT_CLOSE, _T(""), 3);

	// アイテムの設定
//	if (modal_mode_flag)	{ ci.btn_font_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// フォントリスト作成
	create_font_list();

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void C_cfg_wnd_func_font::on_close_funcfunc()
{
	// 作業用フォント開放
	free_work_font();

	// フォントリスト開放
	work.fnlst.free_font_list();
}

// コマンド
bool C_cfg_wnd_func_font::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.lst_font.get_handle())
	{
		switch (HIWORD(wp))
		{
		case LBN_SELCHANGE:
			get_font_list();
			update_font();
			break;
		}
		return true;
	}
	else if ((HWND)lp == ci.chk_proportional.get_handle())
	{
		Gp_config->font_proportional = ci.chk_proportional.get_check();

		create_font_list();

		return true;
	}
	else if ((HWND)lp == ci.chk_font_futoku.get_handle())
	{
		if (Gp_config->font_futoku != ci.chk_font_futoku.get_check())
		{
			Gp_config->font_futoku = ci.chk_font_futoku.get_check();

			update_font();
		}
		return true;
	}
	else if ((HWND)lp == ci.rad_font_shadow_none.get_handle())
	{
		if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_NONE)
		{
			Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_NONE;

			update_font();
		}
		return true;
	}
	else if ((HWND)lp == ci.rad_font_shadow_shadow.get_handle())
	{
		if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_SHADOW)
		{
			Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_SHADOW;

			update_font();
		}
		return true;
	}
	else if ((HWND)lp == ci.rad_font_shadow_fuchidoru.get_handle())
	{
		if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_FUTIDORU)
		{
			Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU;

			update_font();
		}
		return true;
	}
	else if ((HWND)lp == ci.rad_font_shadow_fuchidoru_shadow.get_handle())
	{
		if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
		{
			Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW;

			update_font();
		}
		return true;
	}
	else if ((HWND)lp == ci.btn_font_init.get_handle())
	{
		init_state();

		return true;
	}
	else if ((HWND)lp == ci.btn_font_close.get_handle())
	{
		p_dlg->close();

		return true;
	}

	/*
	switch (LOWORD(wp))	{
		case IDC_LST_FONT:
			switch (HIWORD(wp))	{
				case LBN_SELCHANGE:
					get_font_list();
					update_font();
					break;
			}
			return true;

		case IDC_CHK_PROPORTIONAL:
			Gp_config->font_proportional = ci.chk_proportional.get_check();
			{
				// フォントリストを作成
				create_font_list();
			}

		case IDC_CHK_FONT_FUTOKU:
			if (Gp_config->font_futoku != ci.chk_font_futoku.get_check())	{
				Gp_config->font_futoku = ci.chk_font_futoku.get_check();
				update_font();
			}
			return true;

		case IDC_RAD_FONT_SHADOW_NONE:
			if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_NONE)	{
				Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_NONE;
				update_font();
			}
			return true;

		case IDC_RAD_FONT_SHADOW_SHADOW:
			if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_SHADOW)	{
				Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_SHADOW;
				update_font();
			}
			return true;

		case IDC_RAD_FONT_SHADOW_FUTIDORU:
			if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_FUTIDORU)	{
				Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU;
				update_font();
			}
			return true;

		case IDC_RAD_FONT_SHADOW_FUTIDORU_SHADOW:
			if (Gp_config->font_shadow != TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)	{
				Gp_config->font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW;
				update_font();
			}
			return true;

		case IDC_BTN_FONT_INIT:
			init_state();
			return true;

		case IDC_BTN_FONT_CLOSE:
			p_dlg->close();
			return true;
	}
	*/

	return false;
}

// 通知
bool C_cfg_wnd_func_font::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// マウスホイール
bool C_cfg_wnd_func_font::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// Ｈスクロール
bool C_cfg_wnd_func_font::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// Ｖスクロール
bool C_cfg_wnd_func_font::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func_font::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if (ci.stc_font_sample.get_handle() == (HWND)lp)
	{
		SetBkMode((HDC)wp, TRANSPARENT);	// 背景色モードを透明にする
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	return NULL;
}

// フレーム
void C_cfg_wnd_func_font::frame_funcfunc()
{
	if (!h_wnd)
		return;

	// 初回のみフォントリストを更新
	if (work.frame_first_flag)	{
		update_font_list();
	}

	// 作業用フォント変更
	if (work_font.now_font_name != Gp_config->font_name)	{
		load_work_font(Gp_config->font_name);
		work.now_text.clear();
	}

	// サンプルテキスト更新
	TSTR disp_text = Gp_ini->config.font_sample_str_short;
	if (work.now_text != disp_text)	{	// 表示する文字が変わった時のみ再描画する
		ci.stc_font_sample.set_text(disp_text);
		work.now_text = disp_text;
	}

	// 外部で変更された
	if (Gp_config->font_proportional != ci.chk_proportional.get_check())	{
		ci.chk_proportional.set_check(Gp_config->font_proportional);

		// フォントリストを作成
		create_font_list();
	}
	if (Gp_config->font_name != work.cur_font_name)	{
		update_font_list();
	}
	if (Gp_config->font_futoku != ci.chk_font_futoku.get_check())	{
		ci.chk_font_futoku.set_check(Gp_config->font_futoku);
	}
	if (Gp_config->font_shadow != get_radio_shadow())	{
		set_radio_shadow();
	}

	// 親を再描画する（これがないとうまく表示されない…）
	if (work.frame_first_flag)	{
		::InvalidateRect(h_wnd, NULL, FALSE);
	}

	work.frame_first_flag = false;
}

// 初期状態に戻す
void C_cfg_wnd_func_font::init_state()
{
	// 現在の状態がデフォルトの状態と違う場合はフォントも更新
	bool change_font = (
		Gp_config->font_name != Gp_ini->config.font_name ||
		Gp_config->font_futoku != Gp_ini->config.font_futoku ||
		Gp_config->font_shadow != Gp_ini->config.font_shadow
	) ? true : false;
	tnm_init_config_font();
	update_dialog();
	if (change_font)	{	update_font();	}
	update_font_list();
}

// ダイアログ更新
void C_cfg_wnd_func_font::update_dialog()
{
	if (!h_wnd)
		return;

	// 「文字を太くする」を更新
	ci.chk_font_futoku.set_check(Gp_config->font_futoku);

	// 文字の影タイプを更新
	set_radio_shadow();

	// WM_SIZE を送る
	C_rect rect;
	GetClientRect(h_wnd, &rect);
	SendMessage(h_wnd, WM_SIZE, NULL, MAKELPARAM(rect.width(), rect.height()));
}

// ****************************************************************
// 影のラジオボタンを設定
// ================================================================
void C_cfg_wnd_func_font::set_radio_shadow()
{
	if (!h_wnd)
		return;

	ci.rad_font_shadow_none.set_check(false);
	ci.rad_font_shadow_shadow.set_check(false);
	ci.rad_font_shadow_fuchidoru.set_check(false);
	ci.rad_font_shadow_fuchidoru_shadow.set_check(false);
	switch (Gp_config->font_shadow)	{
		case TNM_FONT_SHADOW_MODE_NONE:				ci.rad_font_shadow_none.set_check(true);				break;
		case TNM_FONT_SHADOW_MODE_SHADOW:			ci.rad_font_shadow_shadow.set_check(true);				break;
		case TNM_FONT_SHADOW_MODE_FUTIDORU:			ci.rad_font_shadow_fuchidoru.set_check(true);			break;
		case TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW:	ci.rad_font_shadow_fuchidoru_shadow.set_check(true);	break;
	}
}

// ****************************************************************
// 影のラジオボタンを取得
// ================================================================
int C_cfg_wnd_func_font::get_radio_shadow()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_font_shadow_none.get_check())					{ return TNM_FONT_SHADOW_MODE_NONE; }
	else if (ci.rad_font_shadow_shadow.get_check())				{ return TNM_FONT_SHADOW_MODE_SHADOW; }
	else if (ci.rad_font_shadow_fuchidoru.get_check())			{ return TNM_FONT_SHADOW_MODE_FUTIDORU; }
	else if (ci.rad_font_shadow_fuchidoru_shadow.get_check())	{ return TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW; }
	return -1;
}

// ****************************************************************
// フォントリストを作成
// ================================================================
void C_cfg_wnd_func_font::create_font_list()
{
	// フォントリストをクリア
	ci.lst_font.clear();

	// 日本語のみフォントを絞る（それ以外は分からないので全部出します…）
#if defined(__SIGLUS_JA)
	BYTE char_set = SHIFTJIS_CHARSET;
#else
	BYTE char_set = ANSI_CHARSET;
#endif

	// フォントリスト作成
	if (work.fnlst.make_font_list(h_wnd, false, !Gp_config->font_proportional, char_set, Gp_ini->tateyoko_mode == 1 ? FONT_YOKOTATE_MODE_TATE_ONLY : FONT_YOKOTATE_MODE_YOKO_ONLY))	{
		ARRAY<ENUMLOGFONT>::iterator itr = work.fnlst.m_font_list.begin();
		for (int i = 0; i < (int)work.fnlst.m_font_list.size(); i++)	{
			ci.lst_font.add_item(TSTR(itr->elfLogFont.lfFaceName));
			itr++;
		}
	}

	// フォントリスト更新
	update_font_list();
}

// ****************************************************************
// フォントを更新
// ================================================================
void C_cfg_wnd_func_font::update_font()
{
	//G_moji_manager.clear();

	tnm_update_font(false);
}

// ****************************************************************
// フォントリストを更新
// ================================================================
void C_cfg_wnd_func_font::update_font_list()
{
	if (!h_wnd)
		return;

	work.cur_font_name = Gp_config->font_name;	// フォント名記憶
	set_font_list();
}

// ****************************************************************
// リストを現在のフォントに設定
// ================================================================
void C_cfg_wnd_func_font::set_font_list()
{
	if (!h_wnd)
		return;

	if (work.fnlst.m_font_list.empty())
		return;

	ARRAY<ENUMLOGFONT>::iterator itr = work.fnlst.m_font_list.begin();
	for (int i = 0; i < (int)work.fnlst.m_font_list.size(); i++)	{
		if (itr->elfLogFont.lfFaceName == Gp_config->font_name)	{
			ci.lst_font.set_selected_item_no(i);						// 現在のフォントを選択する
			return;
		}
		itr++;
	}
}

// ****************************************************************
// リストからフォント取得
// ================================================================
void C_cfg_wnd_func_font::get_font_list()
{
	if (!h_wnd)	{	return;	}
	if (work.fnlst.m_font_list.empty())	{ return; }
	int list_no = ci.lst_font.get_selected_item_no();
	if (!(0 <= list_no && list_no < (int)work.fnlst.m_font_list.size()))	{ return; }
	ARRAY<ENUMLOGFONT>::iterator itr = work.fnlst.m_font_list.begin() + list_no;
	Gp_config->font_name = itr->elfLogFont.lfFaceName;
}

// ****************************************************************
// 作業用フォント
// ================================================================

// 初期化
void C_cfg_wnd_func_font::init_work_font()
{
	work_font.original_font = NULL;
	work_font.now_font = NULL;
	work_font.now_font_name.clear();
}

// 開放
void C_cfg_wnd_func_font::free_work_font()
{
	// 作業用フォント開放
	if (work_font.now_font)	{
		DeleteObject(work_font.now_font);
		work_font.now_font = NULL;
	}
	work_font.now_font_name.clear();
}

// 読み込み
void C_cfg_wnd_func_font::load_work_font(CTSTR& font_name)
{
	work_font.now_font_name = font_name;

	// フォント情報を取得
	ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(Gp_config->font_name));
	if (itr != Gp_font_name_list->m_font_list.end())	{

		work_font.now_font = CreateFont(
			24, 0, 0, 0, FW_REGULAR, 0, 0, 0, itr->elfLogFont.lfCharSet,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, work_font.now_font_name.c_str());
		HDC hdc = GetDC(ci.stc_font_sample.get_handle());
		HFONT kill_font = (HFONT)SelectObject(hdc, work_font.now_font);
		if (work_font.original_font)	{ DeleteObject(kill_font); }
		else							{ work_font.original_font = kill_font; }	// 最初のオリジナルフォントは開放しない
		ReleaseDC(ci.stc_font_sample.get_handle(), hdc);
		SendMessage(ci.stc_font_sample.get_handle(), WM_SETFONT, (WPARAM)work_font.now_font, (LPARAM)MAKELPARAM(TRUE, 0));
	}
}

