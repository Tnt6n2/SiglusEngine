#include	"pch.h"
#include	"dialog/sys_wnd_solo_load.h"
#include	"localize/localize.h"

#include	"../resource.h"

// ****************************************************************
// システムウィンドウ：ソロ：ロード
// ================================================================

#define		REGIST_WINDOW_NAME		_T("sys_wnd_solo_load")

// 初期化
void C_sys_wnd_solo_load::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_SYS_WIN_SOLO_SAVELOAD);
}

// 初期化
bool C_sys_wnd_solo_load::on_init_func()
{
	sys_wnd_func_saveload.on_init_func(this, SYS_WND_FUNC_MOD_SOLO_LOAD, false, false, false);
	return true;
}

// 閉じる
bool C_sys_wnd_solo_load::on_close_func()
{
	sys_wnd_func_saveload.on_close_func();
	return true;
}

// コマンド
bool C_sys_wnd_solo_load::on_command(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// 通知
bool C_sys_wnd_solo_load::on_notify(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// マウスホイール
bool C_sys_wnd_solo_load::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool C_sys_wnd_solo_load::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool C_sys_wnd_solo_load::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (sys_wnd_func_saveload.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT C_sys_wnd_solo_load::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = sys_wnd_func_saveload.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// フレーム
void C_sys_wnd_solo_load::frame_func()
{
	if (!m_h_wnd)
		return;

	sys_wnd_func_saveload.frame_func();
}

