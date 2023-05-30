#include	"pch.h"
#include	"dialog/cfg_wnd_solo_msgspd.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ソロ：文字速度
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_solo_msgspd")

// 初期化
void C_cfg_wnd_solo_msgspd::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_SOLO_COMMON);
}

// 初期化
bool C_cfg_wnd_solo_msgspd::on_init_func()
{
	cfg_wnd_func_msgspd.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, false);

	return true;
}

// 閉じる
bool C_cfg_wnd_solo_msgspd::on_close_func()
{
	cfg_wnd_func_msgspd.on_close_func();

	return true;
}

// コマンド
bool C_cfg_wnd_solo_msgspd::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_msgspd.on_command_func(wp, lp))	{	return true;	}

	return true;
}

// 通知
bool C_cfg_wnd_solo_msgspd::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_msgspd.on_notify_func(wp, lp))		{	return true;	}

	return true;
}

// マウスホイール
bool C_cfg_wnd_solo_msgspd::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_msgspd.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool C_cfg_wnd_solo_msgspd::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_msgspd.on_h_scroll_func(wp, lp))	{	return true;	}

	return true;
}

// Ｖスクロール
bool C_cfg_wnd_solo_msgspd::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_msgspd.on_v_scroll_func(wp, lp))	{	return true;	}

	return true;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_solo_msgspd::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_msgspd.on_ctlcolorstatic_func(wp, lp))	{	return res;	}

	return NULL;
}

// フレーム
void C_cfg_wnd_solo_msgspd::frame_func()
{
	if (!m_h_wnd)	{	return;	}

	cfg_wnd_func_msgspd.frame_func();
}

