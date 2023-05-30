#include	"pch.h"

#include	"dialog/cfg_wnd_modal_font.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：モーダル：フォント
// ================================================================

#define		REGIST_WINDOW_NAME				_T("cfg_wnd_modal_font")

// オープンモーダルファンクション
void C_cfg_wnd_modal_font::open_modal_func(HWND parent_h_wnd)
{
	create(LCL_IDD_CFG_WIN_SOLO_COMMON, REGIST_WINDOW_NAME, true, parent_h_wnd);
}

// 初期化
bool C_cfg_wnd_modal_font::on_init_func()
{
	cfg_wnd_func_font.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, true);

	return true;
}

// 閉じる
bool C_cfg_wnd_modal_font::on_close_func()
{
	cfg_wnd_func_font.on_close_func();

	return true;
}

// コマンド
bool C_cfg_wnd_modal_font::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_command_func(wp, lp))	{	return true;	}

	return true;
}

// タイマー
bool C_cfg_wnd_modal_font::on_timer_func(WPARAM wp, LPARAM lp)
{
	cfg_wnd_func_font.frame_func();

	return true;
}

// 通知
bool C_cfg_wnd_modal_font::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_notify_func(wp, lp))	{	return true;	}

	return true;
}

// マウスホイール
bool C_cfg_wnd_modal_font::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_mousewheel_func(wp, lp))	{	return false;	}

	return true;
}

// Ｈスクロール
bool C_cfg_wnd_modal_font::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_h_scroll_func(wp, lp))		{	return true;	}

	return true;
}

// Ｖスクロール
bool C_cfg_wnd_modal_font::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_font.on_v_scroll_func(wp, lp))		{	return true;	}

	return true;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_modal_font::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;

	if (res = cfg_wnd_func_font.on_ctlcolorstatic_func(wp, lp))		{	return res;	}

	return NULL;
}

