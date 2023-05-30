#include	"pch.h"
#include	"dialog/cfg_wnd_solo.h"

// ****************************************************************
// コンフィグウィンドウ：ソロ
// ================================================================

// ウィンドウプロシージャ
LRESULT C_cfg_wnd_solo::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
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
	return C_tnm_dlg::window_proc(msg, wp, lp);
}

// 初期化
bool C_cfg_wnd_solo::on_init()
{
	// サイズグリップ
	statusbar.create_sizegrip(m_h_wnd);

	// 初期化
	on_init_func();

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_cfg_wnd_solo::on_close()
{
	on_close_func();

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// 空の関数群
bool C_cfg_wnd_solo::on_command(WPARAM wp, LPARAM lp) { return true; }
bool C_cfg_wnd_solo::on_notify(WPARAM wp, LPARAM lp) { return true; }
bool C_cfg_wnd_solo::on_mousewheel(WPARAM wp, LPARAM lp) { return true; }
bool C_cfg_wnd_solo::on_h_scroll(WPARAM wp, LPARAM lp) { return true; }
bool C_cfg_wnd_solo::on_v_scroll(WPARAM wp, LPARAM lp) { return true; }
LRESULT C_cfg_wnd_solo::on_ctlcolorstatic(WPARAM wp, LPARAM lp) { return FALSE; }

// フレーム
void C_cfg_wnd_solo::frame()
{
	if (!m_h_wnd)	{	return;	}

	frame_func();
}

void C_cfg_wnd_solo::frame_func() {}


void C_cfg_wnd_solo::refresh_dialog_func(int dialog_width, int dialog_height)
{
	set_def_client_rect(C_rect(0, 0, dialog_width, dialog_height));

	C_rect rect(0, 0, dialog_width, dialog_height);
	::AdjustWindowRectEx(&rect, get_style(), FALSE, get_style_ex());
	set_def_window_rect(rect);
	set_window_size(rect.size());
	set_pos_my_desktop_center();
}

