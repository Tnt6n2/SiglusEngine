#include	"pch.h"
#include	"dialog/cfg_wnd_modal_mwndbk.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：モーダル：ウィンドウ背景色
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_modal_mwndbk")

// オープンモーダルファンクション
void Ccfg_wnd_modal_mwndbk::open_modal_func(HWND parent_h_wnd)
{
	create(LCL_IDD_CFG_WIN_SOLO_MWNDBK, REGIST_WINDOW_NAME, true, parent_h_wnd);
}

// 初期化
bool Ccfg_wnd_modal_mwndbk::on_init_func()
{
	cfg_wnd_func_mwndbk.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, true);
	return true;
}

// 閉じる
bool Ccfg_wnd_modal_mwndbk::on_close_func()
{
	cfg_wnd_func_mwndbk.on_close_func();
	return true;
}

// コマンド
bool Ccfg_wnd_modal_mwndbk::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_mwndbk.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// タイマー
bool Ccfg_wnd_modal_mwndbk::on_timer_func(WPARAM wp, LPARAM lp)
{
	cfg_wnd_func_mwndbk.frame_func();
	return true;
}

// 通知
bool Ccfg_wnd_modal_mwndbk::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_mwndbk.on_notify_func(wp, lp))		{	return true;	}
	return true;
}

// マウスホイール
bool Ccfg_wnd_modal_mwndbk::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_mwndbk.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool Ccfg_wnd_modal_mwndbk::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_mwndbk.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool Ccfg_wnd_modal_mwndbk::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_mwndbk.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_modal_mwndbk::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_mwndbk.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

