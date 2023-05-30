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
// コンフィグウィンドウ：ファンクション：文字速度
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


// アイテムを構築
void C_cfg_wnd_func_msgspd::setup_dialog()
{
	int p_x, p_y, width;

	bool font_msgspd = (func_mod == CFG_WND_FUNC_MOD_BASE_FONT_MSGSPD || func_mod == CFG_WND_FUNC_MOD_SOLO_FONT_MSGSPD);

	// ダイアログタイトル
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		p_dlg->set_text(LCL_STR_MSGSPD_DIALOG_DIALOG_TITLE);
	}

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// 全パターンに置いて、高さは可変です
	int dialog_width, dialog_height;
	int dialog_pdg_x, dialog_pdg_t, dialog_pdg_b;
	int base_width, base_height;
	int group_width, group_height, group_align_rep;
	int contents_width, contents_height;

	// フォントグループの高さ
	int font_group_height = 160;

	if (wide_language_flag)
	{
		font_group_height += 44;
	}

	// マージン
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
		ci.grp_msgspd.create(_T(""), _T(""), group_x, group_y, 0, 0, false, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, false, false, h_wnd);
		ci.grp_msgspd.set_text(LCL_STR_MSGSPD_DIALOG_GROUP_TITLE);
		p_dlg->add_child(&ci.grp_msgspd, font_msgspd ? 2 : 5);
		SendMessage(ci.grp_msgspd.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));		// グループのフォント設定

		contents_x += DLG_CM_GROUP_PDG_X;
		contents_y += DLG_CM_GROUP_PDG_T + DLG_CM_SPACE;
	}

	// コンテンツを配置
	p_x = contents_x;
	p_y = contents_y;

	width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_SLOW, font);

	ci.stc_msgspd_slow.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_STC_H, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, 0, false, false, h_wnd);
	ci.stc_msgspd_slow.set_text(LCL_STR_MSGSPD_DIALOG_STC_SLOW);
	p_dlg->add_child(&ci.stc_msgspd_slow, font_msgspd ? 1 : 7);
	SendMessage(ci.stc_msgspd_slow.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	
	width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_FAST, font);

	ci.stc_msgspd_fast.create(_T(""), _T(""), contents_x + contents_width - width, p_y, width, DLG_CM_STC_H, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_RIGHT, false, false, h_wnd);
	ci.stc_msgspd_fast.set_text(LCL_STR_MSGSPD_DIALOG_STC_FAST);
	p_dlg->add_child(&ci.stc_msgspd_fast, font_msgspd ? 3 : 9);
	SendMessage(ci.stc_msgspd_fast.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += DLG_CM_STC_H + DLG_CM_SPACE;

	ci.sld_msgspd.create(_T(""), _T(""), p_x, p_y, contents_width, DLG_CM_SLIDER_HEIGHT, false, WS_CHILD | WS_VISIBLE | TBS_TOP, 0, false, false, h_wnd);
	ci.sld_msgspd.set_slider_range(TNM_MESSAGE_SPEED_MIN, TNM_MESSAGE_SPEED_MAX);
	p_dlg->add_child(&ci.sld_msgspd, font_msgspd ? 2 : 8);
	SendMessage(ci.sld_msgspd.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));			// グループのフォント設定

	p_y += DLG_CM_SLIDER_HEIGHT + DLG_CM_SPACE;

	// サンプル表示
	ci.stc_msgspd_sample.create(_T(""), _T(""), p_x, p_y, contents_width, 32, false, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, WS_EX_STATICEDGE, false, false, h_wnd);
	ci.stc_msgspd_sample.set_text(get_sample_text());
	p_dlg->add_child(&ci.stc_msgspd_sample, font_msgspd ? 2 : 8);
	SendMessage(ci.stc_msgspd_sample.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

	p_y += 32;

	p_y += DLG_CM_SPACE * 2;

	// ノーウェイト
	{
		int width = font_get_width(p_dlg->get_handle(), LCL_STR_MSGSPD_DIALOG_STC_NOWAIT, font) + DLG_CM_CHKBOX_ADD_WIDTH;

		ci.chk_msgspd_nowait.create(_T(""), _T(""), p_x, p_y, width, DLG_CM_CHKBOX_H, false, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		ci.chk_msgspd_nowait.set_text(LCL_STR_MSGSPD_DIALOG_STC_NOWAIT);
		p_dlg->add_child(&ci.chk_msgspd_nowait, 1);
		SendMessage(ci.chk_msgspd_nowait.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		// 初期化ボタン
		if (font_msgspd)
		{
			ci.btn_msgspd_init.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_init, 3);
			SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

			p_y += DLG_CM_BTN_INIT_H;
		}
		else if (func_mod == CFG_WND_FUNC_MOD_SOLO)
		{
			ci.btn_msgspd_close.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_close, 3);
			SendMessage(ci.btn_msgspd_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

			ci.btn_msgspd_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
			ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
			p_dlg->add_child(&ci.btn_msgspd_init, 3);
			SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

			p_y += DLG_CM_BTN_CLOSE_H;
		}
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
	ci.grp_msgspd.set_window_size_with_def_size(group_width, group_height);

	// 閉じるボタン
	/*
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		ci.btn_msgspd_close.create(_T(""), _T(""), contents_x + contents_width - DLG_CM_BTN_CLOSE_W, p_y, DLG_CM_BTN_CLOSE_W, DLG_CM_BTN_CLOSE_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_msgspd_close.set_text(modal_mode_flag ? LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR : LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR);
		p_dlg->add_child(&ci.btn_msgspd_close, 3);
		SendMessage(ci.btn_msgspd_close.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		ci.btn_msgspd_init.create(_T(""), _T(""), base_x + base_width - DLG_CM_BTN_CLOSE_W - DLG_CM_SPACE - DLG_CM_BTN_INIT_W, p_y, DLG_CM_BTN_INIT_W, DLG_CM_BTN_INIT_H, false, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, false, false, h_wnd);
		ci.btn_msgspd_init.set_text(LCL_STR_MODAL_DIALOG_INIT_BTN_STR);
		p_dlg->add_child(&ci.btn_msgspd_init, 3);
		SendMessage(ci.btn_msgspd_init.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定

		p_y += DLG_CM_BTN_CLOSE_H;
	}
	*/


	// ベースの高さが決定
	base_height = p_y - base_y;

	// ダイアログの高さが決定
	dialog_height = base_height + dialog_pdg_t + dialog_pdg_b;

	// ダイアログを再配置
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)
	{
		refresh_dialog_func(dialog_width, dialog_height);
	}
}

// 初期化
void C_cfg_wnd_func_msgspd::on_init_funcfunc()
{
	work.now_time = 0;
	work.now_text.clear();

	// 作業用フォント初期化
	init_work_font();

	// ダイアログを構築
	setup_dialog();

	/*
	// アイテム登録
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

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_msgspd_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_msgspd.set_slider_range(TNM_MESSAGE_SPEED_MIN, TNM_MESSAGE_SPEED_MAX);
	*/

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void C_cfg_wnd_func_msgspd::on_close_funcfunc()
{
	// 作業用フォント開放
	free_work_font();
}

// コマンド
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

// 通知
bool C_cfg_wnd_func_msgspd::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool C_cfg_wnd_func_msgspd::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
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

// Ｖスクロール
bool C_cfg_wnd_func_msgspd::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func_msgspd::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if (ci.stc_msgspd_sample.get_handle() == (HWND)lp)
	{
		SetBkMode((HDC)wp, TRANSPARENT);	// 背景色モードを透明にする
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	return NULL;
}

// フレーム
void C_cfg_wnd_func_msgspd::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 作業用フォント変更
	if (work_font.now_font_name != Gp_config->font_name)	{
		load_work_font(Gp_config->font_name);
		work.now_text.clear();
	}

	// サンプルテキスト更新
	TSTR smp_text = get_sample_text();
	if (smp_text.empty())
	{
		ci.stc_msgspd_sample.set_text(_T(""));		// サンプルテキストが無いので文字を消す
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
			int total_time = ((int)smp_text.size() * Gp_config->message_speed) + 500;		// 500 = 待ち時間
			work.now_time = (work.now_time + frame_time_past) % total_time;
			int disp_cnt = (work.now_time / Gp_config->message_speed) + 1;
			disp_text = smp_text.substr(0, disp_cnt);
		}

		// 表示する文字が変わった時のみ再描画する
		if (work.now_text != disp_text)
		{
			ci.stc_msgspd_sample.set_text(disp_text);
			work.now_text = disp_text;
		}
	}

	// 外部で変更された
	if (Gp_config->message_speed != TNM_MESSAGE_SPEED_MAX - ci.sld_msgspd.get_slider_pos())
	{
		ci.sld_msgspd.set_slider_pos(TNM_MESSAGE_SPEED_MAX - Gp_config->message_speed);
	}
	if (Gp_config->message_speed_nowait != ci.chk_msgspd_nowait.get_check())
	{
		ci.chk_msgspd_nowait.set_check(Gp_config->message_speed_nowait);
	}
}

// 初期状態に戻す
void C_cfg_wnd_func_msgspd::init_state()
{
	tnm_init_config_message_speed();
	update_dialog();
}

// ダイアログ更新
void C_cfg_wnd_func_msgspd::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_msgspd.set_slider_pos(TNM_MESSAGE_SPEED_MAX - Gp_config->message_speed);
	ci.chk_msgspd_nowait.set_check(Gp_config->message_speed_nowait);
}

// ****************************************************************
// 作業用フォント
// ================================================================

// 初期化
void C_cfg_wnd_func_msgspd::init_work_font()
{
	work_font.original_font = NULL;
	work_font.now_font = NULL;
	work_font.now_font_name.clear();
}

// 開放
void C_cfg_wnd_func_msgspd::free_work_font()
{
	// 作業用フォント開放
	if (work_font.now_font)	{
		DeleteObject(work_font.now_font);
		work_font.now_font = NULL;
	}
	work_font.now_font_name.clear();
}

// 読み込み
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
	else							{	work_font.original_font = kill_font;	}	// 最初のオリジナルフォントは開放しない
	ReleaseDC(ci.stc_msgspd_sample.get_handle(), hdc);
	SendMessage(ci.stc_msgspd_sample.get_handle(), WM_SETFONT, (WPARAM)work_font.now_font, (LPARAM)MAKELPARAM(TRUE, 0));
}

