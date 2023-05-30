#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_movtype.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：ムービーの再生方法
// ================================================================

// 初期化
void Ccfg_wnd_func_movtype::on_init_funcfunc()
{
	support_return_value = true;

	// アイテム登録
	p_dlg->bind_control(&ci.stc_movtype_mess00, IDC_STC_MOVTYPE_MESS00, _T(""), 7);
	p_dlg->bind_control(&ci.stc_movtype_mess01, IDC_STC_MOVTYPE_MESS01, _T(""), 7);
	p_dlg->bind_control(&ci.rad_movtype_mci, IDC_RAD_MOVTYPE_MCI, _T(""), 7);
	p_dlg->bind_control(&ci.rad_movtype_wmp, IDC_RAD_MOVTYPE_WMP, _T(""), 7);
	p_dlg->bind_control(&ci.btn_movtype_close, IDC_BTN_MOVTYPE_CLOSE, _T(""), 3);
	p_dlg->bind_control(&ci.btn_movtype_deside, IDC_BTN_MOVTYPE_DESIDE, _T(""), 3);
	p_dlg->bind_control(&ci.btn_movtype_cancel, IDC_BTN_MOVTYPE_CANCEL, _T(""), 3);

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_movtype_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_movtype::on_close_funcfunc()
{
}

// コマンド
bool Ccfg_wnd_func_movtype::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_RAD_MOVTYPE_MCI:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_MCI;
			return true;

		case IDC_RAD_MOVTYPE_WMP:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
			return true;

//		case IDC_BTN_MOVTYPE_INIT:
//			init_state();
//			return true;

		case IDC_BTN_MOVTYPE_CLOSE:
			p_dlg->close();
			return true;

		case IDC_BTN_MOVTYPE_DESIDE:
			p_dlg->close();
			return true;

		case IDC_BTN_MOVTYPE_CANCEL:
			support_return_value = false;
			p_dlg->close();
			return true;
	}
	return false;
}

// 通知
bool Ccfg_wnd_func_movtype::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_movtype::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_movtype::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_movtype::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_movtype::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_movtype::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (Gp_system_config->movie_type != get_radio_movtype())	{
		set_radio_movtype();
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_movtype::init_state()
{
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_movtype::update_dialog()
{
	if (!h_wnd)	{	return;	}
	set_radio_movtype();
}

// ****************************************************************
// ムービーの再生方法のラジオボタンを設定
// ================================================================
void Ccfg_wnd_func_movtype::set_radio_movtype()
{
	if (!h_wnd)	{	return;	}
	ci.rad_movtype_mci.set_check(false);
	ci.rad_movtype_wmp.set_check(false);
	switch (Gp_system_config->movie_type)	{
		case TNM_MOVIE_PLAY_TYPE_MCI:	ci.rad_movtype_mci.set_check(true);	break;
		case TNM_MOVIE_PLAY_TYPE_WMP:	ci.rad_movtype_wmp.set_check(true);	break;
	}
}

// ****************************************************************
// ムービーの再生方法のラジオボタンを取得
// ================================================================
int Ccfg_wnd_func_movtype::get_radio_movtype()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_movtype_mci.get_check())			{	return TNM_MOVIE_PLAY_TYPE_MCI;	}
	else if (ci.rad_movtype_wmp.get_check())	{	return TNM_MOVIE_PLAY_TYPE_WMP;	}
	return -1;
}


