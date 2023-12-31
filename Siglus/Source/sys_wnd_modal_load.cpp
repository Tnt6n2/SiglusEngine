#include	"pch.h"
#include	"dialog/sys_wnd_modal_load.h"
#include	"localize/localize.h"

#include	"../resource.h"

// ****************************************************************
// システムウィンドウ：モーダル：ロード
// ================================================================

#define		REGIST_WINDOW_NAME		_T("sys_wnd_modal_load")

// オープンモーダルファンクション
int C_sys_wnd_modal_load::open_modal_result_int_func(HWND parent_h_wnd)
{
	create(LCL_IDD_SYS_WIN_SOLO_SAVELOAD, REGIST_WINDOW_NAME, true, parent_h_wnd);
	return sys_wnd_func_saveload.select_return_value;
}

// 初期化
bool C_sys_wnd_modal_load::on_init_func()
{
	sys_wnd_func_saveload.on_init_func(this, SYS_WND_FUNC_MOD_SOLO_LOAD, false, false, true);
	return true;
}

// 閉じる
bool C_sys_wnd_modal_load::on_close_func()
{
	sys_wnd_func_saveload.on_close_func();
	return true;
}

// コマンド
bool C_sys_wnd_modal_load::on_command(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// タイマー
bool C_sys_wnd_modal_load::on_timer_func(WPARAM wp, LPARAM lp)
{
	sys_wnd_func_saveload.frame_func();
	return true;
}

// 通知
bool C_sys_wnd_modal_load::on_notify(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// マウスホイール
bool C_sys_wnd_modal_load::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool C_sys_wnd_modal_load::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool C_sys_wnd_modal_load::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT C_sys_wnd_modal_load::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = sys_wnd_func_saveload.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

