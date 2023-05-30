#include	"pch.h"
#include	"dialog/cfg_wnd.h"
#include	"dialog/cfg_wnd_config_base_full_mode.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ベースフルモード
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_base_full_mode")

// 初期化
void C_cfg_wnd_config_base_full_mode::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_BASE);
	cfg_wnd_full_mode_flag = true;		// ★ true はデバッグ用の全環境設定モードです。
	init_child_window();
}

