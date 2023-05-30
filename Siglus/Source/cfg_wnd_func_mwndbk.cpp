#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_mwndbk.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：ウィンドウ背景色
// ================================================================

// 初期化
void Ccfg_wnd_func_mwndbk::on_init_funcfunc()
{
	// アイテム登録
	p_dlg->bind_control(&ci.grp_mwndbk, IDC_GRP_MWNDBK, _T(""), 5);

	p_dlg->bind_control(&ci.stc_mwndbk_r, IDC_STC_MWNDBK_R, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_r, IDC_SLD_MWNDBK_R, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_g, IDC_STC_MWNDBK_G, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_g, IDC_SLD_MWNDBK_G, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_b, IDC_STC_MWNDBK_B, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_b, IDC_SLD_MWNDBK_B, _T(""), 8);
	p_dlg->bind_control(&ci.stc_mwndbk_tr, IDC_STC_MWNDBK_TR, _T(""), 7);
	p_dlg->bind_control(&ci.sld_mwndbk_tr, IDC_SLD_MWNDBK_TR, _T(""), 8);
	p_dlg->bind_control(&ci.btn_mwndbk_init, IDC_BTN_MWNDBK_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.btn_mwndbk_close, IDC_BTN_MWNDBK_CLOSE, _T(""), 3);

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_mwndbk_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_mwndbk_r.set_slider_range(0, 255);
	ci.sld_mwndbk_g.set_slider_range(0, 255);
	ci.sld_mwndbk_b.set_slider_range(0, 255);
	ci.sld_mwndbk_tr.set_slider_range(0, 255);

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_mwndbk::on_close_funcfunc()
{
}

// コマンド
bool Ccfg_wnd_func_mwndbk::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_MWNDBK_INIT:
			init_state();
			return true;

		case IDC_BTN_MWNDBK_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// 通知
bool Ccfg_wnd_func_mwndbk::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_mwndbk::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_mwndbk::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_mwndbk_r.get_handle())	{
		Gp_config->filter_color.r = ci.sld_mwndbk_r.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_g.get_handle())	{
		Gp_config->filter_color.g = ci.sld_mwndbk_g.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_b.get_handle())	{
		Gp_config->filter_color.b = ci.sld_mwndbk_b.get_slider_pos();
		return true;
	}
	else if ((HWND)lp == ci.sld_mwndbk_tr.get_handle())	{
		Gp_config->filter_color.a = 255 - ci.sld_mwndbk_tr.get_slider_pos();
		return true;
	}
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_mwndbk::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_mwndbk::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_mwndbk::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (Gp_config->filter_color.r != ci.sld_mwndbk_r.get_slider_pos())	{
		ci.sld_mwndbk_r.set_slider_pos(Gp_config->filter_color.r);
	}
	if (Gp_config->filter_color.g != ci.sld_mwndbk_g.get_slider_pos())	{
		ci.sld_mwndbk_g.set_slider_pos(Gp_config->filter_color.g);
	}
	if (Gp_config->filter_color.b != ci.sld_mwndbk_b.get_slider_pos())	{
		ci.sld_mwndbk_b.set_slider_pos(Gp_config->filter_color.b);
	}
	if (Gp_config->filter_color.a != 255 - ci.sld_mwndbk_tr.get_slider_pos())	{
		ci.sld_mwndbk_tr.set_slider_pos(255 - Gp_config->filter_color.a);
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_mwndbk::init_state()
{
	tnm_init_config_filter();
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_mwndbk::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.sld_mwndbk_r.set_slider_pos(Gp_config->filter_color.r);
	ci.sld_mwndbk_g.set_slider_pos(Gp_config->filter_color.g);
	ci.sld_mwndbk_b.set_slider_pos(Gp_config->filter_color.b);
	ci.sld_mwndbk_tr.set_slider_pos(255 - Gp_config->filter_color.a);
}

