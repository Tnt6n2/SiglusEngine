#include	"pch.h"

#include	"dialog/cfg_wnd_func.h"

#include	"../resource.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション
// ================================================================

// 初期化
void C_cfg_wnd_func::on_init_func(C_dialog* p_dlg_, int func_mod_, bool is_base_mode, bool is_full_mode, bool is_modal_mode)
{
	p_dlg = p_dlg_;
	h_wnd = p_dlg->get_handle();
	func_mod = func_mod_;
	base_mode_flag = is_base_mode;
	full_mode_flag = is_full_mode;
	modal_mode_flag = is_modal_mode;
	frame_start_time = ::timeGetTime();
	frame_time_past = 0;

	// 言語補正
#if defined(__SIGLUS_JA) || defined(__SIGLUS_ZH) || defined(__SIGLUS_ZHTW)
	wide_language_flag = false;
#else
	wide_language_flag = true;
#endif

	// フランス、インドネシア語の場合は音声の設定ウィンドウをワイド化する
#if defined(__SIGLUS_FR) || defined(__SIGLUS_ID)
	wide_koe_wnd_flag = true;
#else
	wide_koe_wnd_flag = false;
#endif

	on_init_funcfunc();
}

// 閉じる
void C_cfg_wnd_func::on_close_func()
{
	on_close_funcfunc();
	p_dlg = NULL;
	h_wnd = NULL;
}

// コマンド
bool C_cfg_wnd_func::on_command_func(WPARAM wp, LPARAM lp)
{
	return on_command_funcfunc(wp, lp);
}

// 通知
bool C_cfg_wnd_func::on_notify_func(WPARAM wp, LPARAM lp)
{
	return on_notify_funcfunc(wp, lp);
}

// マウスホイール
bool C_cfg_wnd_func::on_mousewheel_func(WPARAM wp, LPARAM lp)
{
	return on_mousewheel_funcfunc(wp, lp);
}

// Ｈスクロール
bool C_cfg_wnd_func::on_h_scroll_func(WPARAM wp, LPARAM lp)
{
	return on_h_scroll_funcfunc(wp, lp);
}

// Ｖスクロール
bool C_cfg_wnd_func::on_v_scroll_func(WPARAM wp, LPARAM lp)
{
	return on_v_scroll_funcfunc(wp, lp);
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func::on_ctlcolorstatic_func(WPARAM wp, LPARAM lp)
{
	return on_ctlcolorstatic_funcfunc(wp, lp);
}

// フレーム
void C_cfg_wnd_func::frame_func()
{
	if (!h_wnd)	{	return;	}
	int now_time = ::timeGetTime();
	frame_time_past = now_time - frame_start_time;
	frame_start_time = now_time;
	frame_funcfunc();
}

void C_cfg_wnd_func::refresh_dialog_func(int dialog_width, int dialog_height)
{
	p_dlg->set_def_client_rect(C_rect(0, 0, dialog_width, dialog_height));

	if (!base_mode_flag)
	{
		C_rect rect(0, 0, dialog_width, dialog_height);
		::AdjustWindowRectEx(&rect, p_dlg->get_style(), FALSE, p_dlg->get_style_ex());
		p_dlg->set_def_window_rect(rect);
		p_dlg->set_window_size(rect.size());
		p_dlg->set_pos_my_desktop_center();
	}
}

// ****************************************************************
// コンフィグウィンドウ：ファンクション：仮想関数
// ================================================================

// 初期化
void C_cfg_wnd_func::on_init_funcfunc()
{
	// NOP
}

// 閉じる
void C_cfg_wnd_func::on_close_funcfunc()
{
	// NOP
}

// コマンド
bool C_cfg_wnd_func::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// 通知
bool C_cfg_wnd_func::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// マウスホイール
bool C_cfg_wnd_func::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// Ｈスクロール
bool C_cfg_wnd_func::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// Ｖスクロール
bool C_cfg_wnd_func::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	return false;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
	return NULL;
}

// フレーム
void C_cfg_wnd_func::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}
}
