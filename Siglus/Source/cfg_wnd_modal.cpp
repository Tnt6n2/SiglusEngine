#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_icon.h"
#include	"dialog/cfg_wnd_modal.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// コンフィグウィンドウ：モーダル
// ================================================================

// ウィンドウプロシージャ
LRESULT C_cfg_wnd_modal::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_TIMER:			if (!on_timer(wp, lp))		return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
		case WM_MOUSEWHEEL:		if (!on_mousewheel(wp, lp))	return FALSE;	break;
		case WM_HSCROLL:		if (!on_h_scroll(wp, lp))	return FALSE;	break;
		case WM_VSCROLL:		if (!on_v_scroll(wp, lp))	return FALSE;	break;
		case WM_CTLCOLORSTATIC:
			{
				LRESULT res;
				if (res = on_ctlcolorstatic(wp, lp))	{	return res;	}
			}
			break;
	}
	return C_modal_dialog::window_proc(msg, wp, lp);
}

// 初期化
bool C_cfg_wnd_modal::on_init()
{
	// アイコンを設定
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	// サイズグリップ
	statusbar.create_sizegrip(m_h_wnd);

	// 初期化
	on_init_func();

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();

	// タイマー開始
	SetTimer(m_h_wnd, CFG_WND_MODAL_TIMER_ID, CFG_WND_MODAL_TIMER_TIME, NULL);

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_cfg_wnd_modal::on_close()
{
	on_close_func();

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// タイマー
bool C_cfg_wnd_modal::on_timer(WPARAM wp, LPARAM lp)
{
	on_timer_func(wp, lp);
	if (!is_key_down(VK_LBUTTON))	{
		rep_window_pos_outside_desktop();
	}
	return true;
}

// ****************************************************************
// コンフィグウィンドウ：モーダル：オープンモーダル
// ================================================================
void C_cfg_wnd_modal::open_modal(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	open_modal_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
}

bool C_cfg_wnd_modal::open_modal_result_bool(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	bool res = open_modal_result_bool_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
	return res;
}

int C_cfg_wnd_modal::open_modal_result_int(HWND parent_h_wnd)
{
	tnm_dlg_set_enable(false);
	int res = open_modal_result_int_func(parent_h_wnd);
	tnm_dlg_set_enable(true);
	return res;
}

// ****************************************************************
// コンフィグウィンドウ：モーダル：オープンモーダルファンクション（仮想関数）
// ================================================================
void C_cfg_wnd_modal::open_modal_func(HWND parent_h_wnd)
{
	// NOP
}

bool C_cfg_wnd_modal::open_modal_result_bool_func(HWND parent_h_wnd)
{
	// NOP
	return false;
}

int C_cfg_wnd_modal::open_modal_result_int_func(HWND parent_h_wnd)
{
	// NOP
	return -1;
}

