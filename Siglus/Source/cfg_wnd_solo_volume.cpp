#include	"pch.h"
#include	"dialog/cfg_wnd_solo_volume.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ソロ：音量
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_solo_volume")

// 初期化
void Ccfg_wnd_solo_volume::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_SOLO_COMMON);
}

// 初期化
bool Ccfg_wnd_solo_volume::on_init_func()
{
	cfg_wnd_func_volume.on_init_func(this, CFG_WND_FUNC_MOD_SOLO, false, false, false);

	return true;
}

// 閉じる
bool Ccfg_wnd_solo_volume::on_close_func()
{
	cfg_wnd_func_volume.on_close_func();
	return true;
}

// コマンド
bool Ccfg_wnd_solo_volume::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// 通知
bool Ccfg_wnd_solo_volume::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_notify_func(wp, lp))		{	return true;	}
	return true;
}

// マウスホイール
bool Ccfg_wnd_solo_volume::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool Ccfg_wnd_solo_volume::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool Ccfg_wnd_solo_volume::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_solo_volume::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_volume.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// フレーム
void Ccfg_wnd_solo_volume::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	cfg_wnd_func_volume.frame_func();
}

