#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_cache.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/eng_etc.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_dialog.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_error.h"
#include	"dialog/sys_wnd_modal_save.h"
#include	"dialog/sys_wnd_modal_load.h"

// ****************************************************************
// システムコマンド：ブレーク／ブレーク解除
// ================================================================
bool tnm_syscom_change_break()
{
	Gp_global->is_breaking = !Gp_global->is_breaking;

	// ブレークした場合、トレースウィンドウを自動的に開く
	if (Gp_global->is_breaking)	{
		tnm_dlg_open_trace_dialog();

		// デバッグ設定がされてない場合はさらに設定ウィンドウを開く
		if (Gp_config->ss_path.empty())
			tnm_dlg_open_debug_setting_dialog();
	}

	return true;
}

// ****************************************************************
// システムコマンド：ステップ実行
// ================================================================
bool tnm_syscom_do_one_step()
{
	Gp_global->break_step_flag = true;

	return true;
}

// ****************************************************************
// システムコマンド：最初からはじめる
// ================================================================
bool tnm_syscom_restart_from_start()
{
	int res = tnm_game_warning_box(Gp_ini->config.warninginfo.restart_warning_str);
	if (res == IDNO)
		return false;

	// ★グローバルデータをセーブする
	tnm_save_global_on_file();

	// 最初からはじめる
	tnm_scene_proc_restart_from_start_scene();

	// ★ゲームタイマー開始
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// システムコマンド：シーンからはじめる
// ================================================================
bool tnm_syscom_restart_from_scene(int scn_no, int z_no)
{
	int res = tnm_game_warning_box(Gp_ini->config.warninginfo.scenestart_warning_str);
	if (res == IDNO)
		return false;

	// ★グローバルデータをセーブする
	tnm_save_global_on_file();

	// リスタート
	tnm_scene_proc_restart(scn_no, z_no);

	// ★ゲームタイマー開始
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// システムコマンド：メニューを更新
// ================================================================
void tnm_syscom_update_menu()
{
	// 既読早送りが使えない場合、自動的に OFF にします。
	if (!tnm_syscom_read_skip_is_enable())
		Gp_syscom->read_skip_flag = false;
}

// ****************************************************************
// システムコマンド：システムコマンドのフラグを初期化
// ================================================================
void tnm_syscom_init_syscom_flag()
{
	Gp_syscom->read_skip_flag = false;
	Gp_syscom->auto_skip_flag = false;
	Gp_syscom->hide_mwnd_flag = false;
	Gp_syscom->msg_back_flag = false;

	// 存在の有無
	Gp_syscom->syscommenu.read_skip.exist = Gp_ini->syscommenu.read_skip.exist;
	Gp_syscom->syscommenu.unread_skip.exist = Gp_ini->syscommenu.unread_skip.exist;
	Gp_syscom->syscommenu.auto_skip.exist = Gp_ini->syscommenu.auto_skip.exist;
	Gp_syscom->syscommenu.auto_mode.exist = Gp_ini->syscommenu.auto_mode.exist;
	Gp_syscom->syscommenu.hide_mwnd.exist = Gp_ini->syscommenu.hide_mwnd.exist;
	Gp_syscom->syscommenu.msg_back.exist = Gp_ini->syscommenu.msg_back.exist;
	Gp_syscom->syscommenu.save.exist = Gp_ini->syscommenu.save.exist;
	Gp_syscom->syscommenu.load.exist = Gp_ini->syscommenu.load.exist;
	Gp_syscom->syscommenu.return_to_sel.exist = Gp_ini->syscommenu.return_to_sel.exist;
	Gp_syscom->syscommenu.config.exist = Gp_ini->syscommenu.config.exist;
	Gp_syscom->syscommenu.manual.exist = Gp_ini->syscommenu.manual.exist;
	Gp_syscom->syscommenu.version.exist = Gp_ini->syscommenu.version.exist;
	Gp_syscom->syscommenu.return_to_menu.exist = Gp_ini->syscommenu.return_to_menu.exist;
	Gp_syscom->syscommenu.game_end.exist = Gp_ini->syscommenu.game_end.exist;
	Gp_syscom->syscommenu.cancel.exist = Gp_ini->syscommenu.cancel.exist;

	// 許可／禁止
	Gp_syscom->syscommenu.read_skip.enable = Gp_ini->syscommenu.read_skip.enable;
	Gp_syscom->syscommenu.unread_skip.enable = Gp_ini->syscommenu.unread_skip.enable;
	Gp_syscom->syscommenu.auto_skip.enable = Gp_ini->syscommenu.auto_skip.enable;
	Gp_syscom->syscommenu.auto_mode.enable = Gp_ini->syscommenu.auto_mode.enable;
	Gp_syscom->syscommenu.hide_mwnd.enable = Gp_ini->syscommenu.hide_mwnd.enable;
	Gp_syscom->syscommenu.msg_back.enable = Gp_ini->syscommenu.msg_back.enable;;
	Gp_syscom->syscommenu.save.enable = Gp_ini->syscommenu.save.enable;
	Gp_syscom->syscommenu.load.enable = Gp_ini->syscommenu.load.enable;
	Gp_syscom->syscommenu.return_to_sel.enable = Gp_ini->syscommenu.return_to_sel.enable;
	Gp_syscom->syscommenu.config.enable = Gp_ini->syscommenu.config.enable;
	Gp_syscom->syscommenu.manual.enable = Gp_ini->syscommenu.manual.enable;
	Gp_syscom->syscommenu.version.enable = Gp_ini->syscommenu.version.enable;
	Gp_syscom->syscommenu.return_to_menu.enable = Gp_ini->syscommenu.return_to_menu.enable;
	Gp_syscom->syscommenu.game_end.enable = Gp_ini->syscommenu.game_end.enable;
	Gp_syscom->syscommenu.cancel.enable = Gp_ini->syscommenu.cancel.enable;

	// ローカル汎用スイッチ
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < TNM_LOCAL_EXTRA_SWITCH_CNT; i++)	{
			Gp_syscom->syscommenu.local_extra_switch[i].exist = Gp_ini->syscommenu.local_extra_switch[i].exist;
			Gp_syscom->syscommenu.local_extra_switch[i].enable = Gp_ini->syscommenu.local_extra_switch[i].enable;
			Gp_syscom->syscommenu.local_extra_switch[i].onoff = Gp_ini->syscommenu.local_extra_switch[i].onoff;
		}
	}

	// ローカル汎用モード
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		for (int i = 0; i < TNM_LOCAL_EXTRA_MODE_CNT; i++)	{
			Gp_syscom->syscommenu.local_extra_mode[i].exist = Gp_ini->syscommenu.local_extra_mode[i].exist;
			Gp_syscom->syscommenu.local_extra_mode[i].enable = Gp_ini->syscommenu.local_extra_mode[i].enable;
			Gp_syscom->syscommenu.local_extra_mode[i].mode = Gp_ini->syscommenu.local_extra_mode[i].mode;
		}
	}

	// メニューを更新
	tnm_syscom_update_menu();
}

// ****************************************************************
// システムコマンドを開く
// ================================================================
bool tnm_syscom_open()
{
	// システムコマンドシーンが指定されていない場合
	if (Gp_ini->cancel_scene_name.empty())	{

		// システムコマンドメニューを開く
		if (!tnm_rmenu_open())
			return false;
	}
	else	{

		// システムコマンドシーンを呼ぶ
		if (!tnm_scene_proc_call_syscom_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// システムコマンド：既読早送りの ON/OFF を設定
// ================================================================
void tnm_syscom_read_skip_set_onoff_flag(bool onoff)
{
	Gp_syscom->read_skip_flag = onoff;

	// メニューを更新
	tnm_syscom_update_menu();
}

// ****************************************************************
// システムコマンド：既読早送りの ON/OFF を取得
// ================================================================
bool tnm_syscom_read_skip_get_onoff_flag()
{
	return Gp_syscom->read_skip_flag;
}

// ****************************************************************
// システムコマンド：既読早送りの許可／不許可を設定
// ================================================================
void tnm_syscom_read_skip_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.read_skip.enable = enable;

	// メニューを更新
	tnm_syscom_update_menu();
}

// ****************************************************************
// システムコマンド：既読早送りの許可／不許可を取得
// ================================================================
bool tnm_syscom_read_skip_get_enable_flag()
{
	return Gp_syscom->syscommenu.read_skip.enable;
}

// ****************************************************************
// システムコマンド：既読早送りの表示／非表示を設定
// ================================================================
void tnm_syscom_read_skip_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.read_skip.exist = exist;

	// メニューを更新
	tnm_syscom_update_menu();
}

// ****************************************************************
// システムコマンド：既読早送りの表示／非表示を取得
// ================================================================
bool tnm_syscom_read_skip_get_exist_flag()
{
	return Gp_syscom->syscommenu.read_skip.exist;
}

// ****************************************************************
// システムコマンド：既読早送りが使用可能かを取得
// ================================================================
bool tnm_syscom_read_skip_is_enable()
{
	// 未読スキップがＯＮの場合
	if (tnm_is_skip_unread_msg())	{
		return Gp_syscom->syscommenu.read_skip.enable && Gp_syscom->syscommenu.read_skip.exist;
	}
	else	{
		return Gp_syscom->syscommenu.read_skip.enable && Gp_syscom->syscommenu.read_skip.exist && Gp_global->read_skip_enable_flag;
	}
}

// ****************************************************************
// システムコマンド：自動早送りの ON/OFF を設定
// ================================================================
void tnm_syscom_auto_skip_set_onoff_flag(bool onoff)
{
	Gp_syscom->auto_skip_flag = onoff;
}

// ****************************************************************
// システムコマンド：自動早送りの ON/OFF を取得
// ================================================================
bool tnm_syscom_auto_skip_get_onoff_flag()
{
	return Gp_syscom->auto_skip_flag;
}

// ****************************************************************
// システムコマンド：自動早送りの許可／不許可を設定
// ================================================================
void tnm_syscom_auto_skip_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.auto_skip.enable = enable;
}

// ****************************************************************
// システムコマンド：自動早送りの許可／不許可を取得
// ================================================================
bool tnm_syscom_auto_skip_get_enable_flag()
{
	return Gp_syscom->syscommenu.auto_skip.enable;
}

// ****************************************************************
// システムコマンド：自動早送りの表示／非表示を設定
// ================================================================
void tnm_syscom_auto_skip_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.auto_skip.exist = exist;
}

// ****************************************************************
// システムコマンド：自動早送りの表示／非表示を取得
// ================================================================
bool tnm_syscom_auto_skip_get_exist_flag()
{
	return Gp_syscom->syscommenu.auto_skip.exist;
}

// ****************************************************************
// システムコマンド：自動早送りが使用可能かを取得
// ================================================================
bool tnm_syscom_auto_skip_is_enable()
{
	return Gp_syscom->syscommenu.auto_skip.enable && Gp_syscom->syscommenu.auto_skip.exist;
}

// ****************************************************************
// システムコマンド：オートモードの ON/OFF を設定
// ================================================================
void tnm_syscom_auto_mode_set_onoff_flag(bool onoff)
{
	Gp_config->auto_mode_onoff = onoff;
}

// ****************************************************************
// システムコマンド：オートモードの ON/OFF を取得
// ================================================================
bool tnm_syscom_auto_mode_get_onoff_flag()
{
	return Gp_config->auto_mode_onoff;
}

// ****************************************************************
// システムコマンド：オートモードの許可／不許可を設定
// ================================================================
void tnm_syscom_auto_mode_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.auto_mode.enable = enable;
}

// ****************************************************************
// システムコマンド：オートモードの許可／不許可を取得
// ================================================================
bool tnm_syscom_auto_mode_get_enable_flag()
{
	return Gp_syscom->syscommenu.auto_mode.enable;
}

// ****************************************************************
// システムコマンド：オートモードの表示／非表示を設定
// ================================================================
void tnm_syscom_auto_mode_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.auto_mode.exist = exist;
}

// ****************************************************************
// システムコマンド：オートモードの表示／非表示を取得
// ================================================================
bool tnm_syscom_auto_mode_get_exist_flag()
{
	return Gp_syscom->syscommenu.auto_mode.exist;
}

// ****************************************************************
// システムコマンド：オートモードが使用可能かを取得
// ================================================================
bool tnm_syscom_auto_mode_is_enable()
{
	return Gp_syscom->syscommenu.auto_mode.enable && Gp_syscom->syscommenu.auto_mode.exist;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの ON/OFF を設定
// ================================================================
void tnm_syscom_hide_mwnd_set_onoff_flag(bool onoff)
{
	Gp_syscom->hide_mwnd_flag = onoff;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの ON/OFF を取得
// ================================================================
bool tnm_syscom_hide_mwnd_get_onoff_flag()
{
	return Gp_syscom->hide_mwnd_flag;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの許可／不許可を設定
// ================================================================
void tnm_syscom_hide_mwnd_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.hide_mwnd.enable = enable;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの許可／不許可を取得
// ================================================================
bool tnm_syscom_hide_mwnd_get_enable_flag()
{
	return Gp_syscom->syscommenu.hide_mwnd.enable;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの表示／非表示を設定
// ================================================================
void tnm_syscom_hide_mwnd_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.hide_mwnd.exist = exist;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるの表示／非表示を取得
// ================================================================
bool tnm_syscom_hide_mwnd_get_exist_flag()
{
	return Gp_syscom->syscommenu.hide_mwnd.exist;
}

// ****************************************************************
// システムコマンド：メッセージウィンドウを閉じるが使用可能かを取得
// ================================================================
bool tnm_syscom_hide_mwnd_is_enable()
{
	return Gp_syscom->syscommenu.hide_mwnd.enable && Gp_syscom->syscommenu.hide_mwnd.exist && !Gp_local->pod.hide_mwnd_disable;
}

// ****************************************************************
// システムコマンド：メッセージバックを開く
// ================================================================
void tnm_syscom_open_msg_back()
{
	if (!tnm_msg_back_is_enable())
		return;

	// 早送りを止める
	tnm_syscom_read_skip_set_onoff_flag(false);

	// メッセージバックを開く
	tnm_msg_back_open();

	// メッセージバックプロセスへ
	tnm_push_proc(TNM_PROC_TYPE_MSG_BACK);
}

// ****************************************************************
// システムコマンド：メッセージバックを閉じる
// ================================================================
void tnm_syscom_close_msg_back()
{
	// メッセージバックが開いている場合のみ
	if (tnm_msg_back_is_open())	{

		// メッセージバックを閉じる
		tnm_msg_back_close();

		// 次のプロセスへ
		tnm_pop_proc();
	}
}

// ****************************************************************
// システムコマンド：メッセージバックの許可／不許可を設定
// ================================================================
void tnm_syscom_msg_back_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.msg_back.enable = enable;
}

// ****************************************************************
// システムコマンド：メッセージバックの許可／不許可を取得
// ================================================================
bool tnm_syscom_msg_back_get_enable_flag()
{
	return Gp_syscom->syscommenu.msg_back.enable;
}

// ****************************************************************
// システムコマンド：メッセージバックの表示／非表示を設定
// ================================================================
void tnm_syscom_msg_back_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.msg_back.exist = exist;
}

// ****************************************************************
// システムコマンド：メッセージバックの表示／非表示を取得
// ================================================================
bool tnm_syscom_msg_back_get_exist_flag()
{
	return Gp_syscom->syscommenu.msg_back.exist;
}

// ****************************************************************
// システムコマンド：メッセージバックが使用可能かを取得
// ================================================================
bool tnm_syscom_msg_back_is_enable()
{
	return Gp_syscom->syscommenu.msg_back.enable && Gp_syscom->syscommenu.msg_back.exist && tnm_msg_back_is_exist_msg() && !Gp_local->pod.msg_back_disable;
}

// ****************************************************************
// システム：セーブダイアログを開いてセーブする
// ================================================================
bool tnm_open_save_dialog(bool capture_flag)
{
	// キャプチャーする場合
	if (capture_flag && Gp_ini->use_save_thumb)	{

		// ２．セーブダイアログを呼び出す
		tnm_push_proc(TNM_PROC_TYPE_SAVE_DIALOG);	// セーブダイアログ

		// １．キャプチャーを行う
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_SAVE, true);
	}
	else	{

		// ダイアログを開く
		C_sys_wnd_modal_save sys_wnd_modal_save;
		int res = sys_wnd_modal_save.open_modal_result_int();		// res : データ番号（-1 = キャンセル）
		if (res == -1)
			return false;

		// ダイアログで選択した場合は syscom enable チェックを行わずに必ずセーブ
		if (!tnm_syscom_save(res, true, true))
			return false;
	}

	return true;
}

// ****************************************************************
// システムコマンド：セーブを開く
// ================================================================
bool tnm_syscom_open_save(bool dialog_flag, bool capture_flag, bool enable_check)
{
	// セーブできるかを判定
	if (!tnm_is_save_enable())
		return true;

	// セーブシーンが指定されていない場合
	if (dialog_flag || Gp_ini->save_scene_name.empty())	{

		// セーブダイアログを開く
		if (!tnm_open_save_dialog(capture_flag))
			return false;
	}
	else	{
 
		// セーブシーンを呼ぶ
		if (!tnm_scene_proc_call_save_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// システムコマンド：セーブを開くの許可／不許可を設定
// ================================================================
void tnm_syscom_open_save_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.save.enable = enable;
}

// ****************************************************************
// システムコマンド：セーブを開くの許可／不許可を取得
// ================================================================
bool tnm_syscom_open_save_get_enable_flag()
{
	return Gp_syscom->syscommenu.save.enable;
}

// ****************************************************************
// システムコマンド：セーブを開くの表示／非表示を設定
// ================================================================
void tnm_syscom_open_save_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.save.exist = exist;
}

// ****************************************************************
// システムコマンド：セーブを開くの表示／非表示を取得
// ================================================================
bool tnm_syscom_open_save_get_exist_flag()
{
	return Gp_syscom->syscommenu.save.exist;
}

// ****************************************************************
// システムコマンド：セーブを開くが可能かを判定
// ================================================================
bool tnm_syscom_open_save_is_enable()
{
	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable();
}

// ****************************************************************
// システム：ロードダイアログを開いてロードする
// ================================================================
int tnm_open_load_dialog(bool is_execute)
{
	// ダイアログを開く
	C_sys_wnd_modal_load sys_wnd_modal_load;
	int res = sys_wnd_modal_load.open_modal_result_int();		// res : データ番号（-1 = キャンセル）
	if (res == -1)
		return -1;

	// ロード
	if (is_execute)	{

		// ダイアログで選択した場合は syscom enable チェックを行わずに必ずロード
		if (!tnm_syscom_load(res, true, true, true))
			return -2;		// ロードできなかった場合は -2 を戻す
	}

	return res;
}

// ****************************************************************
// システムコマンド：ロードを開く
// ================================================================
bool tnm_syscom_open_load(bool dialog_flag, bool enable_check)
{
	// ロードできるかを判定
	if (!tnm_is_load_enable())
		return true;

	// ロードシーンが指定されていない場合
	if (dialog_flag || Gp_ini->load_scene_name.empty())	{

		// ロードダイアログを開く
		if (tnm_open_load_dialog(true) < 0)
			return false;
	}
	else	{
 
		// ロードシーンを呼ぶ
		if (!tnm_scene_proc_call_load_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// システムコマンド：ロードを開くの許可／不許可を設定
// ================================================================
void tnm_syscom_open_load_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.load.enable = enable;
}

// ****************************************************************
// システムコマンド：ロードを開くの許可／不許可を取得
// ================================================================
bool tnm_syscom_open_load_get_enable_flag()
{
	return Gp_syscom->syscommenu.load.enable;
}

// ****************************************************************
// システムコマンド：ロードを開くの表示／非表示を設定
// ================================================================
void tnm_syscom_open_load_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.load.exist = exist;
}

// ****************************************************************
// システムコマンド：ロードを開くの表示／非表示を取得
// ================================================================
bool tnm_syscom_open_load_get_exist_flag()
{
	return Gp_syscom->syscommenu.load.exist;
}

// ****************************************************************
// システムコマンド：ロードを開くが可能かを判定
// ================================================================
bool tnm_syscom_open_load_is_enable()
{
	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable();
}

// ****************************************************************
// システムコマンド：セーブ
// ================================================================
bool tnm_syscom_save(int save_no, bool warning, bool se_play)
{
	// セーブできないなら失敗
	if (!tnm_is_save_enable(save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// ファイルの存在確認
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// 上書き確認
			int res = tnm_game_warning_box(Gp_ini->config.save_info.warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// セーブ（番号指定）
	if (!tnm_saveload_proc_save(save_no))
		return false;

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// システムコマンド：セーブが可能かを判定
// ================================================================
bool tnm_syscom_save_is_enable(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt <= save_no)
		return false;

	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(save_no);
}

// ****************************************************************
// システムコマンド：ロード
// ================================================================
bool tnm_syscom_load(int save_no, bool warning, bool se_play, bool fade_out)
{
	// ロードできないなら失敗
	if (!tnm_is_load_enable(save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ★ゲームタイマー停止（ロード）
	Gp_global->game_timer_move_flag = false;

	// 「ロード」を予約
	tnm_push_proc(TNM_PROC_TYPE_LOAD, save_no);

	// フェードアウト
	if (fade_out)	{

		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// システムコマンド：ロードが可能かを判定
// ================================================================
bool tnm_syscom_load_is_enable(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt <= save_no)
		return false;

	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(save_no);
}

// ****************************************************************
// システムコマンド：クイックセーブ
// ================================================================
bool tnm_syscom_quick_save(int quick_save_no, bool warning, bool se_play)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// セーブできないなら失敗
	if (!tnm_is_save_enable(Gp_ini->save_cnt + quick_save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// ファイルの存在確認
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// 上書き確認
			int res = tnm_game_warning_box(Gp_ini->config.save_info.quick_warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// クイックセーブ
	if (!tnm_saveload_proc_quick_save(quick_save_no))
		return false;

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// システムコマンド：クイックセーブが可能かを判定
// ================================================================
bool tnm_syscom_quick_save_is_enable(int quick_save_no)
{
	// 範囲チェック
	if (quick_save_no < 0 || Gp_ini->quick_save_cnt <= quick_save_no)
		return false;

	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(Gp_ini->save_cnt + quick_save_no);
}

// ****************************************************************
// システムコマンド：クイックロード
// ================================================================
bool tnm_syscom_quick_load(int quick_save_no, bool warning, bool se_play, bool fade_out)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// ロードできないなら失敗
	if (!tnm_is_load_enable(save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.quick_warning_str);
		if (res == IDNO)
			return false;
	}

	// ★ゲームタイマー停止（クイックロード）
	Gp_global->game_timer_move_flag = false;

	// 「クイックロード」を予約
	tnm_push_proc(TNM_PROC_TYPE_QUICK_LOAD, quick_save_no);

	// フェードアウト
	if (fade_out)	{

		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// システムコマンド：クイックロードが可能かを判定
// ================================================================
bool tnm_syscom_quick_load_is_enable(int quick_save_no)
{
	// 範囲チェック
	if (quick_save_no < 0 || Gp_ini->quick_save_cnt <= quick_save_no)
		return false;

	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(Gp_ini->save_cnt + quick_save_no);
}

// ****************************************************************
// システムコマンド：エンドセーブ
// ================================================================
bool tnm_syscom_end_save_period_2(bool capture_flag)
{
	// キャプチャーする場合
	if (capture_flag && Gp_ini->use_save_thumb)	{

		// ２．エンドセーブ
		tnm_push_proc(TNM_PROC_TYPE_END_SAVE);		// エンドセーブ

		// １．キャプチャーを行う
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_END, true);
	}
	else	{

		// エンドセーブを行う
		if (!tnm_saveload_proc_end_save())
			return false;
	}

	return true;
}

bool tnm_syscom_end_save(bool warning, bool se_play)
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// セーブできないなら失敗
	if (!tnm_is_save_enable(save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// ファイルの存在確認
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// 上書き確認
			int res = tnm_game_warning_box(Gp_ini->config.save_info.warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// エンドセーブ（２段階目）
	if (!tnm_syscom_end_save_period_2(true))
		return false;

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// システムコマンド：エンドセーブが可能かを判定
// ================================================================
bool tnm_syscom_end_save_is_enable()
{
	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(Gp_ini->save_cnt + Gp_ini->quick_save_cnt);
}

// ****************************************************************
// システムコマンド：エンドロード
// ================================================================
bool tnm_syscom_end_load(bool warning, bool se_play, bool fade_out)
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// ロードできないなら失敗
	if (!tnm_is_load_enable(save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ★ゲームタイマー停止（エンドロード）
	Gp_global->game_timer_move_flag = false;

	// 「エンドロード」を予約
	tnm_push_proc(TNM_PROC_TYPE_END_LOAD);

	// フェードアウト
	if (fade_out)	{

		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// システムコマンド：インナーロード
// ================================================================
bool tnm_syscom_inner_load(int inner_save_no, bool warning, bool se_play, bool fade_out)
{
	// ロードできないなら失敗
	if (!tnm_savedata_system_check_inner_save(inner_save_no))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ★ゲームタイマー停止（インナーロード）
	Gp_global->game_timer_move_flag = false;

	// 「インナーロード」を予約
	tnm_push_proc(TNM_PROC_TYPE_INNER_LOAD, inner_save_no);

	// フェードアウト
	if (fade_out)	{

		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// システムコマンド：メッセージバックロード
// ================================================================
bool tnm_syscom_msgbk_load(S_tid save_id, bool warning, bool se_play, bool fade_out)
{
	// ロードできないなら失敗
	if (!tnm_check_backlog_save(save_id))
		return false;

	// 確認メッセージ
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.msgbk_warning_str);
		if (res == IDNO)
			return false;
	}

	// ★ゲームタイマー停止（バックログロード）
	Gp_global->game_timer_move_flag = false;

	// 「バックログロード」を予約
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BACKLOG_LOAD;
	proc.option_time_id = save_id;
	tnm_push_proc(proc);

	// フェードアウト
	if (fade_out)
	{
		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// システムコマンド：エンドロードが可能かを判定
// ================================================================
bool tnm_syscom_end_load_is_enable()
{
	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(Gp_ini->save_cnt + Gp_ini->quick_save_cnt);
}

// ****************************************************************
// システムコマンド：前の選択肢に戻る
// ================================================================
void tnm_syscom_return_to_sel(bool warning, bool se_play, bool fade_out)
{
	// 確認メッセージ
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.returnsel_warning_str);
		if (res == IDNO)
			return;
	}

	// ★ゲームタイマー停止（前の選択肢に戻る）
	Gp_global->game_timer_move_flag = false;

	// 「前の選択肢に戻る」を予約
	tnm_push_proc(TNM_PROC_TYPE_RETURN_TO_SEL);

	// フェードアウト
	if (fade_out)
	{
		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_PREV_SEL);
}

// ****************************************************************
// システムコマンド：前の選択肢に戻るの許可／不許可を設定
// ================================================================
void tnm_syscom_return_to_sel_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.return_to_sel.enable = enable;
}

// ****************************************************************
// システムコマンド：前の選択肢に戻るの許可／不許可を取得
// ================================================================
bool tnm_syscom_return_to_sel_get_enable_flag()
{
	return Gp_syscom->syscommenu.return_to_sel.enable;
}

// ****************************************************************
// システムコマンド：前の選択肢に戻るの表示／非表示を設定
// ================================================================
void tnm_syscom_return_to_sel_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.return_to_sel.exist = exist;
}

// ****************************************************************
// システムコマンド：前の選択肢に戻るの表示／非表示を取得
// ================================================================
bool tnm_syscom_return_to_sel_get_exist_flag()
{
	return Gp_syscom->syscommenu.return_to_sel.exist;
}

// ****************************************************************
// システムコマンド：前の選択肢に戻るが可能かを判定
// ================================================================
bool tnm_syscom_return_to_sel_is_enable()
{
	return Gp_syscom->syscommenu.return_to_sel.enable && Gp_syscom->syscommenu.return_to_sel.exist && tnm_is_exist_sel_save();
}

// ****************************************************************
// システムコマンド：メニューに戻る
// ================================================================
void tnm_syscom_return_to_menu(bool warning, bool se_play, bool fade_out, bool leave_msgbk)
{
	// 確認メッセージ
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.returnmenu_warning_str);
		if (res == IDNO)
			return;
	}

	// ★グローバルデータをセーブする
	tnm_save_global_on_file();

	// ★ゲームタイマー停止（メニューに戻る）
	Gp_global->game_timer_move_flag = false;

	// 「メニューに戻る」を予約
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_RETURN_TO_MENU;
	proc.option = (leave_msgbk ? 1 : 0);
	tnm_push_proc(proc);

	// フェードアウト
	if (fade_out)
	{
		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// 「描画」を予約
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// システム音を鳴らす
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_MENU);
}

// ****************************************************************
// システムコマンド：メニューに戻るの許可／不許可を設定
// ================================================================
void tnm_syscom_return_to_menu_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.return_to_menu.enable = enable;
}

// ****************************************************************
// システムコマンド：メニューに戻るの許可／不許可を取得
// ================================================================
bool tnm_syscom_return_to_menu_get_enable_flag()
{
	return Gp_syscom->syscommenu.return_to_menu.enable;
}

// ****************************************************************
// システムコマンド：メニューに戻るの表示／非表示を設定
// ================================================================
void tnm_syscom_return_to_menu_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.return_to_menu.exist = exist;
}

// ****************************************************************
// システムコマンド：メニューに戻るの表示／非表示を取得
// ================================================================
bool tnm_syscom_return_to_menu_get_exist_flag()
{
	return Gp_syscom->syscommenu.return_to_menu.exist;
}

// ****************************************************************
// システムコマンド：メニューに戻るが可能かを判定
// ================================================================
bool tnm_syscom_return_to_menu_is_enable()
{
	return Gp_syscom->syscommenu.return_to_menu.enable && Gp_syscom->syscommenu.return_to_menu.exist;
}

// ****************************************************************
// ウィンドウを閉じる
// ================================================================
void tnm_syscom_close_window()
{
	Gp_wnd->close();
}

// ****************************************************************
// システムコマンド：ゲームを終了する
// ================================================================
void tnm_syscom_end_game(bool warning, bool se_play, bool fade_out)
{
	// 確認メッセージ
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.gameend_warning_str);
		if (res == IDNO)
			return;
	}

	// ★ゲームタイマー停止（ゲームを終了する）
	Gp_global->game_timer_move_flag = false;

	// 「描画」を予約
	tnm_push_proc(TNM_PROC_TYPE_DISP);

	// 「ゲームを終了する」を予約
	tnm_push_proc(TNM_PROC_TYPE_END_GAME);

	// フェードアウト
	if (fade_out)
	{
		// 「ゲーム終了時のワイプ」を予約
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
	}

	// エンドセーブ（キャプチャーのための描画が入る可能性があります）
	tnm_syscom_end_save(false, false);		// ★★★エンドセーブ（syscom）
}

// ****************************************************************
// システムコマンド：ゲームを終了の許可／不許可を設定
// ================================================================
void tnm_syscom_end_game_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.game_end.enable = enable;
}

// ****************************************************************
// システムコマンド：ゲームを終了の許可／不許可を取得
// ================================================================
bool tnm_syscom_end_game_get_enable_flag()
{
	return Gp_syscom->syscommenu.game_end.enable;
}

// ****************************************************************
// システムコマンド：ゲームを終了の表示／非表示を設定
// ================================================================
void tnm_syscom_end_game_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.game_end.exist = exist;
}

// ****************************************************************
// システムコマンド：ゲームを終了の表示／非表示を取得
// ================================================================
bool tnm_syscom_end_game_get_exist_flag()
{
	return Gp_syscom->syscommenu.game_end.exist;
}

// ****************************************************************
// システムコマンド：ゲームを終了するが可能かを判定
// ================================================================
bool tnm_syscom_end_game_is_enable()
{
	return Gp_syscom->syscommenu.game_end.enable && Gp_syscom->syscommenu.game_end.exist;
}

// ****************************************************************
// システムコマンド：声を再生
// ================================================================
void tnm_syscom_koe_play()
{
	// 声のリプレイはメッセージバック扱い
	int jitan_rate = Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL;
	tnm_play_koe(Gp_local->pod.cur_koe_no, Gp_local->pod.cur_chr_no, jitan_rate, 0, false, false, false, true, false);
}

// ****************************************************************
// システムコマンド：声を再生が可能かを判定
// ================================================================
bool tnm_syscom_koe_play_is_enable()
{
	return Gp_local->pod.cur_koe_no >= 0;
}

// ****************************************************************
// システムコマンド：環境設定を開く
// ================================================================
void tnm_syscom_open_config()
{
	// 環境設定シーンが指定されていない場合
	if (Gp_ini->config_scene_name.empty())
	{
		// 環境設定ダイアログを開く
		tnm_dlg_open_config_dialog();
	}
	else
	{
		// 環境設定シーンを呼ぶ
		if (!tnm_scene_proc_call_config_scene())
			return;
	}
}

// ****************************************************************
// システムコマンド：環境設定を開くの許可／不許可を設定
// ================================================================
void tnm_syscom_open_config_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.config.enable = enable;
}

// ****************************************************************
// システムコマンド：環境設定を開くの許可／不許可を取得
// ================================================================
bool tnm_syscom_open_config_get_enable_flag()
{
	return Gp_syscom->syscommenu.config.enable;
}

// ****************************************************************
// システムコマンド：環境設定を開くの表示／非表示を設定
// ================================================================
void tnm_syscom_open_config_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.config.exist = exist;
}

// ****************************************************************
// システムコマンド：環境設定を開くの表示／非表示を取得
// ================================================================
bool tnm_syscom_open_config_get_exist_flag()
{
	return Gp_syscom->syscommenu.config.exist;
}

// ****************************************************************
// システムコマンド：環境設定を開くが可能かを判定
// ================================================================
bool tnm_syscom_open_config_is_enable()
{
	return Gp_syscom->syscommenu.config.enable && Gp_syscom->syscommenu.config.exist;
}

// ****************************************************************
// システムコマンド：マニュアルを開くの許可／不許可を設定
// ================================================================
void tnm_syscom_manual_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.manual.enable = enable;
}

// ****************************************************************
// システムコマンド：マニュアルを開くの許可／不許可を取得
// ================================================================
bool tnm_syscom_manual_get_enable_flag()
{
	return Gp_syscom->syscommenu.manual.enable;
}

// ****************************************************************
// システムコマンド：マニュアルを開くの表示／非表示を設定
// ================================================================
void tnm_syscom_manual_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.manual.exist = exist;
}

// ****************************************************************
// システムコマンド：マニュアルを開くの表示／非表示を取得
// ================================================================
bool tnm_syscom_manual_get_exist_flag()
{
	return Gp_syscom->syscommenu.manual.exist;
}

// ****************************************************************
// システムコマンド：バージョン情報を開くの許可／不許可を設定
// ================================================================
void tnm_syscom_version_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.version.enable = enable;
}

// ****************************************************************
// システムコマンド：バージョン情報を開くの許可／不許可を取得
// ================================================================
bool tnm_syscom_version_get_enable_flag()
{
	return Gp_syscom->syscommenu.version.enable;
}

// ****************************************************************
// システムコマンド：バージョン情報を開くの表示／非表示を設定
// ================================================================
void tnm_syscom_version_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.version.exist = exist;
}

// ****************************************************************
// システムコマンド：バージョン情報を開くの表示／非表示を取得
// ================================================================
bool tnm_syscom_version_get_exist_flag()
{
	return Gp_syscom->syscommenu.version.exist;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの ON/OFF を設定
// ================================================================
void tnm_syscom_local_extra_switch_set_onoff_flag(int switch_no, bool onoff)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff = onoff;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの ON/OFF を取得
// ================================================================
bool tnm_syscom_local_extra_switch_get_onoff_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの ON/OFF を反転
// ================================================================
void tnm_syscom_local_extra_switch_turn_onoff_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff = !Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの許可／不許可を設定
// ================================================================
void tnm_syscom_local_extra_switch_set_enable_flag(int switch_no, bool enable)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].enable = enable;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの許可／不許可を取得
// ================================================================
bool tnm_syscom_local_extra_switch_get_enable_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].enable;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの表示／非表示を設定
// ================================================================
void tnm_syscom_local_extra_switch_set_exist_flag(int switch_no, bool exist)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].exist = exist;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチの表示／非表示を取得
// ================================================================
bool tnm_syscom_local_extra_switch_get_exist_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].exist;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用スイッチが使えるか判定
// ================================================================
bool tnm_syscom_local_extra_switch_is_enable(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].enable && Gp_syscom->syscommenu.local_extra_switch[switch_no].exist;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの値を設定
// ================================================================
void tnm_syscom_local_extra_mode_set_value(int mode_no, int value)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode = value;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの値を取得
// ================================================================
int tnm_syscom_local_extra_mode_get_value(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].mode;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの値を進める
// ================================================================
void tnm_syscom_local_extra_mode_increment(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)	{
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode ++;
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode %= Gp_ini->syscommenu.local_extra_mode[mode_no].item_cnt;
	}
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの許可／不許可を設定
// ================================================================
void tnm_syscom_local_extra_mode_set_enable_flag(int mode_no, bool enable)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].enable = enable;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの許可／不許可を取得
// ================================================================
bool tnm_syscom_local_extra_mode_get_enable_flag(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].enable;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの表示／非表示を設定
// ================================================================
void tnm_syscom_local_extra_mode_set_exist_flag(int mode_no, bool exist)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].exist = exist;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードの表示／非表示を取得
// ================================================================
bool tnm_syscom_local_extra_mode_get_exist_flag(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].exist;

	return false;
}

// ****************************************************************
// システムコマンド：ローカル汎用モードが使えるか判定
// ================================================================
bool tnm_syscom_local_extra_mode_is_enable(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].enable && Gp_syscom->syscommenu.local_extra_mode[mode_no].exist;

	return false;
}

// ****************************************************************
// システムコマンド：キャンセルの許可／不許可を設定
// ================================================================
void tnm_syscom_cancel_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.cancel.enable = enable;
}

// ****************************************************************
// システムコマンド：キャンセルの許可／不許可を取得
// ================================================================
bool tnm_syscom_cancel_get_enable_flag()
{
	return Gp_syscom->syscommenu.cancel.enable;
}

// ****************************************************************
// システムコマンド：キャンセルの表示／非表示を設定
// ================================================================
void tnm_syscom_cancel_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.cancel.exist = exist;
}

// ****************************************************************
// システムコマンド：キャンセルの表示／非表示を取得
// ================================================================
bool tnm_syscom_cancel_get_exist_flag()
{
	return Gp_syscom->syscommenu.cancel.exist;
}

