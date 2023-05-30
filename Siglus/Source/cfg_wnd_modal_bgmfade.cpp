#include	"pch.h"
#include	"dialog/cfg_wnd_modal_bgmfade.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：モーダル：ＢＧＭフェード
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_modal_bgmfade")

// オープンモーダルファンクション
void Ccfg_wnd_modal_bgmfade::open_modal_func(HWND parent_h_wnd)
{
	create(LCL_IDD_CFG_WIN_SOLO_COMMON, REGIST_WINDOW_NAME, true, parent_h_wnd);
}

// 初期化
bool Ccfg_wnd_modal_bgmfade::on_init_func()
{
	cfg_wnd_func_bgmfade.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, true);
	return true;
}

// 閉じる
bool Ccfg_wnd_modal_bgmfade::on_close_func()
{
	cfg_wnd_func_bgmfade.on_close_func();
	return true;
}

// コマンド
bool Ccfg_wnd_modal_bgmfade::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_bgmfade.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// タイマー
bool Ccfg_wnd_modal_bgmfade::on_timer_func(WPARAM wp, LPARAM lp)
{
	cfg_wnd_func_bgmfade.frame_func();
	return true;
}

// 通知
bool Ccfg_wnd_modal_bgmfade::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_bgmfade.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// マウスホイール
bool Ccfg_wnd_modal_bgmfade::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_bgmfade.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool Ccfg_wnd_modal_bgmfade::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_bgmfade.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool Ccfg_wnd_modal_bgmfade::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_bgmfade.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_modal_bgmfade::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_bgmfade.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

