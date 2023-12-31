#include	"pch.h"
#include	"dialog/cfg_wnd_base_volume_bgmfade.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ベース：音量（音量、ＢＧＭフェード）
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_base_volume_bgmfade")

// 初期化
void Ccfg_wnd_base_volume_bgmfade::init()
{
	regist(REGIST_WINDOW_NAME, true, false, LCL_IDD_CFG_WIN_BASE_COMMON);
}

// 初期化
bool Ccfg_wnd_base_volume_bgmfade::on_init_func()
{
	cfg_wnd_func_volume.on_init_func(this, CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE, true, cfg_wnd_full_mode_flag, false);
	cfg_wnd_func_bgmfade.on_init_func(this, CFG_WND_FUNC_MOD_BASE_VOLUME_BGMFADE, true, cfg_wnd_full_mode_flag, false);

	return true;
}

// 閉じる
bool Ccfg_wnd_base_volume_bgmfade::on_close_func()
{
	cfg_wnd_func_volume.on_close_func();
	cfg_wnd_func_bgmfade.on_close_func();
	return true;
}

// コマンド
bool Ccfg_wnd_base_volume_bgmfade::on_command(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_command_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_command_func(wp, lp))	{	return true;	}
	return true;
}

// 通知
bool Ccfg_wnd_base_volume_bgmfade::on_notify(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_notify_func(wp, lp))		{	return true;	}
	if (cfg_wnd_func_bgmfade.on_notify_func(wp, lp))	{	return true;	}
	return true;
}

// マウスホイール
bool Ccfg_wnd_base_volume_bgmfade::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_mousewheel_func(wp, lp))		{	return false;	}
	if (cfg_wnd_func_bgmfade.on_mousewheel_func(wp, lp))	{	return false;	}
	return true;
}

// Ｈスクロール
bool Ccfg_wnd_base_volume_bgmfade::on_h_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_h_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_h_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// Ｖスクロール
bool Ccfg_wnd_base_volume_bgmfade::on_v_scroll(WPARAM wp, LPARAM lp)
{
	if (cfg_wnd_func_volume.on_v_scroll_func(wp, lp))	{	return true;	}
	if (cfg_wnd_func_bgmfade.on_v_scroll_func(wp, lp))	{	return true;	}
	return true;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_base_volume_bgmfade::on_ctlcolorstatic(WPARAM wp, LPARAM lp)
{
	LRESULT res;
	if (res = cfg_wnd_func_volume.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	if (res = cfg_wnd_func_bgmfade.on_ctlcolorstatic_func(wp, lp))	{	return res;	}
	return NULL;
}

// フレーム
void Ccfg_wnd_base_volume_bgmfade::frame_func()
{
	if (!m_h_wnd)	{	return;	}
	cfg_wnd_func_volume.frame_func();
	cfg_wnd_func_bgmfade.frame_func();
}

