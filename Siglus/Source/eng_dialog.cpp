#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dlg_manager.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// 全てのラジオボタンからフォーカスを外す
// ================================================================
void tnm_dlg_kill_focus_for_all_radio()
{
	for (int i = 0; i < (int)Gp_dlg_mng->dlg_list.size(); i++)	{
		Gp_dlg_mng->dlg_list[i]->kill_focus_for_radio_item();
	}
	for (ARRAY<C_cfg_wnd_solo *>::iterator itr = Gp_dlg_mng->cfg_wnd_config_base.get_child_window_list_begin(); itr != Gp_dlg_mng->cfg_wnd_config_base.get_child_window_list_end(); ++itr)	{
		(*itr)->kill_focus_for_radio_item();
	}
	for (ARRAY<C_cfg_wnd_solo *>::iterator itr = Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_all_child_window_list_begin(); itr != Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_all_child_window_list_end(); ++itr)	{
		(*itr)->kill_focus_for_radio_item();
	}
}

// ****************************************************************
// メッセージボックス
// ================================================================
int tnm_game_message_box(CTSTR& msg, UINT type)
{
	// 全てのダイアログに対してラジオボタンのフォーカスをはずす
	// メッセージボックスが出る前に処理する必要があります。
	tnm_dlg_kill_focus_for_all_radio();

	// ダイアログを無効にしてからメッセージボックスを出します。
	tnm_dlg_set_enable(false);
	int res = message_box(msg, Gp_ini->game_name, type);
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// 確認メッセージ
// ================================================================
int tnm_game_warning_box(CTSTR& msg)
{
	// 全てのダイアログに対してラジオボタンのフォーカスをはずす
	// メッセージボックスが出る前に処理する必要があります。
	tnm_dlg_kill_focus_for_all_radio();

	// ダイアログを無効にしてからメッセージボックスを出します。
	tnm_dlg_set_enable(false);
	int res = warning_box(msg + _T("　　　"), Gp_ini->game_name, MB_YESNO);		// 全角スペースを３つ入れてウィンドウサイズを調整する
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// 情報メッセージ
// ================================================================
int tnm_game_info_box(CTSTR& msg)
{
	// 全てのダイアログに対してラジオボタンのフォーカスをはずす
	// メッセージボックスが出る前に処理する必要があります。
	tnm_dlg_kill_focus_for_all_radio();

	// ダイアログを無効にしてからメッセージボックスを出します。
	tnm_dlg_set_enable(false);
	int res = info_box(msg + _T("　　　"), Gp_ini->game_name, MB_OK);			// 全角スペースを３つ入れてウィンドウサイズを調整する
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// 全てのダイアログに対して許可／禁止を設定する
// ================================================================
void tnm_dlg_set_enable(bool enable)
{
	Gp_dlg_mng->set_enable(enable);
}

// ****************************************************************
// マウスがダイアログに乗っているかを判定する
// ================================================================
bool tnm_dlg_check_mouse_over()
{
	return Gp_dlg_mng->check_mouse_over();
}

// ****************************************************************
// 環境設定ダイアログが開いているかを判定する
// ================================================================
bool tnm_dlg_is_config_dialog_opened()
{
	return Gp_dlg_mng->cfg_wnd_config_base.get_handle() ? true : false;
}

// ****************************************************************
// 環境設定ダイアログを開く
// ================================================================
void tnm_dlg_open_config_dialog()
{
	if (!Gp_dlg_mng->cfg_wnd_config_base.get_handle())	{
		Gp_dlg_mng->cfg_wnd_config_base.open();
	}
}

// ****************************************************************
// デバッグ設定ダイアログを開く
// ================================================================
void tnm_dlg_open_debug_setting_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_debug.get_handle())	{
		Gp_dlg_mng->db_wnd_info_debug.open();
	}
}

// ****************************************************************
// トレースダイアログを開く
// ================================================================
void tnm_dlg_open_trace_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_trace.get_handle())	{
		Gp_dlg_mng->db_wnd_info_trace.open();
	}
}

// ****************************************************************
// エラー情報ダイアログを開く
// ================================================================
void tnm_dlg_open_error_info_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_error.get_handle())	{
		Gp_dlg_mng->db_wnd_info_error.open();
	}
}

// ****************************************************************
// Twitter ダイアログを開く
// ================================================================
void tnm_dlg_open_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.open();
}

// ****************************************************************
// Twitter ダイアログを閉じる
// ================================================================
void tnm_dlg_close_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.close();
}

// ****************************************************************
// Twitter ダイアログを更新する
// ================================================================
void tnm_dlg_update_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.update_state();
}

// ****************************************************************
// Twitter 投稿に成功
// ================================================================
void tnm_dlg_twitter_dialog_on_success_tweet()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.on_success_tweet();
}
