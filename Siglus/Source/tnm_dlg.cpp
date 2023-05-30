#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dlg.h"
#include	"data/tnm_icon.h"

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
void C_tnm_dlg::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	C_dialog::window_proc_pre(msg, wp, lp);

	switch (msg)	{

		case WM_INITDIALOG:
			// アイコンを設定
			set_icon(Gp_icon->get_large_icon());
			set_small_icon(Gp_icon->get_small_icon());
			break;
	}
}

LRESULT C_tnm_dlg::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// ラジオボタンのフォーカスを外す為の処理
		case WM_ACTIVATE:
			if (LOWORD(wp) == WA_INACTIVE)	{
				kill_focus_for_radio_item();
			}
			break;
	}
	return C_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// ラジオボタンからフォーカスを外す
// ================================================================
bool C_tnm_dlg::kill_focus_for_radio_item()
{
	HWND focus_h_wnd = GetFocus();
	CHILDITR end = get_child_list_end();
	C_control *ctrl;
	DWORD style;
	for (CHILDITR itr = get_child_list_begin(); itr != end; ++itr)	{
		ctrl = (C_control *)(*itr);
		if (focus_h_wnd == ctrl->get_handle())	{
			if (ctrl->get_class() == WC_BUTTON)	{
				style = (DWORD)ctrl->get_style() & BS_TYPEMASK;
				if (style == BS_RADIOBUTTON || style == BS_AUTORADIOBUTTON)	{
					send_message(WM_NEXTDLGCTL, (WPARAM)m_h_wnd, TRUE);		// m_h_wnd はダイアログのハンドルなので、本来渡すべきはコントロールのハンドルです。
					return true;
				}
			}
		}
	}
	return false;
}

// ****************************************************************
// 登録
// ================================================================
void C_tnm_dlg::regist(CTSTR window_name, bool limit_min, bool attach, int dialog_id)
{
	regist_state.window_name = window_name;
	regist_state.limit_min = limit_min;
	regist_state.attach = attach;
	regist_state.dialog_id = dialog_id;
}

// ****************************************************************
// 初期オープン判定
// ================================================================
bool C_tnm_dlg::initial_open_check()
{
	if (regist_state.window_name.empty())											{	return false;	}
	if (C_ini_file::body().load_bool(_T("open"), false, regist_state.window_name))	{	return true;	}
	return false;
}

// ****************************************************************
// 初期オープン
// ================================================================
//		アプリケーション起動時に呼ばれる
//		前回終了時に開いていた場合は開く
// ================================================================
void C_tnm_dlg::initial_open()
{
	if (!initial_open_check())	{	return;	}
	open();
}

// ****************************************************************
// 開閉状態を ini に保存
// ================================================================
void C_tnm_dlg::save_ini_open_state()
{
	if (regist_state.window_name.empty())	{	return;	}
	C_ini_file::body().save_bool(_T("open"), m_h_wnd ? true : false, regist_state.window_name);
}

// ****************************************************************
// ****************************************************************
// ****************************************************************
// 以下は、ヴァーチャル関数
// ================================================================
// ================================================================
// ================================================================

// ****************************************************************
// 解放
// ================================================================
//		アプリケーション終了時に呼ばれる
//		開閉状態を保存してダイアログを閉じる
// ================================================================
void C_tnm_dlg::free()
{
	save_ini_open_state();	// 開閉状態を ini に保存しておく
	close();				// ダイアログを閉じる
}

// ****************************************************************
// 開く（作成）
// ================================================================
void C_tnm_dlg::open(HWND parent_h_wnd)
{
	if (m_h_wnd)					{	return;	}
	if (regist_state.dialog_id < 0)	{	return;	}

	// ダイアログを作成する
	create(regist_state.dialog_id, regist_state.window_name, regist_state.limit_min, regist_state.attach, parent_h_wnd);
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_tnm_dlg::load_state()
{
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_tnm_dlg::save_state()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_tnm_dlg::frame()
{
}

