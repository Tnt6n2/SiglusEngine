#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_reg_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_namae.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_call.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// ゲーム名を取得
// ================================================================
TSTR tnm_get_game_name()
{
	return Gp_ini->game_name;
}

// ****************************************************************
// シーン番号からシーン名を取得
// ================================================================
TSTR tnm_get_scn_name(int scn_no)
{
	return Gp_lexer->get_scn_name(scn_no);
}

// ****************************************************************
// シーン番号からフルパス名を取得
// ================================================================
TSTR tnm_get_scn_full_path_name(int scn_no)
{
	return Gp_config->ss_path + _T("\\") + Gp_lexer->get_scn_name(scn_no) + _T(".ss");
}

// ****************************************************************
// シーンタイトルを取得
// ================================================================
TSTR tnm_get_scene_title()
{
	return Gp_local->scene_title;
}

// ****************************************************************
// フルタイトルを取得
// ================================================================
TSTR tnm_get_full_title()
{
	// デバッグ中は色々な情報を添える
	if (Gp_global->debug_flag)	{

		TSTR info_str;

		// 簡易シーンパック
		if (!Gp_lexer->is_exist_org_source())	{
			info_str += _T("[★無圧縮★]");
		}
		// エフェクトをソースから作成した
		if (Gp_global->shader_from_source)	{
			info_str += _T("[★ｴﾌｪｸﾄ仮★]");
		}

		// ゲーム名
		info_str += Gp_ini->game_name;

		// ブレーク中
		if (Gp_global->is_breaking)	{
			info_str += _T(" [ブレーク中]");
		}

		// シーン名
		int scn_no = Gp_lexer->get_cur_scn_no();
		if (scn_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			int line_no = Gp_lexer->get_cur_line_no();
			info_str += _T(" ") + Gp_local->scene_title + _T(" / scene=") + scn_name + _T("(") + tostr(line_no) + _T(")");
		}

		// 重要な情報
		info_str += _T(" rate=" + tostr(Gp_global->frame_rate_100msec_total)
			+ _T(", cstack=") + tostr(Gp_call_list->get_call_cnt()) + _T("(max=") + tostr(Gp_call_list->get_size()) + _T(")")
			+ _T(", pgstack=") + tostr(Gp_stack->int_now_cnt + (int)Gp_stack->str_stack.size()));

		// 早送りおよびオートモード
		if (tnm_is_skipping())	{
			info_str += _T("[早=") + tostr(Gp_global->disp_because_msg_wait_cnt_max / 100) + _T("]");
		}
		if (tnm_is_auto_mode())	{
			info_str += _T("[自]");
		}

		// キャプチャーのあるなし
		if (Gp_global->capture_prior)	{
			info_str += _T("★ｷｬﾌﾟﾁｬｰ") + tostr(Gp_global->capture_prior);
		}

		return info_str;
	}
	else	{

		TSTR info_str;

		// ゲーム名
		info_str += Gp_ini->game_name;

		// シーン名
		info_str += _T(" ") + Gp_local->scene_title;

		return info_str;
	}
}

// ****************************************************************
// ウィンドウ名を取得
// ================================================================
TSTR tnm_get_window_name()
{
	TSTR window_name = _T("siglus_engine_main_window_") + Gp_ini->game_id + _T("_") + Gp_reg_ini->reg_name;
	if (Gp_cmd_line->multi_user_id_set)
	{
		window_name += _T("_") + Gp_cmd_line->multi_user_id;
	}

	return window_name;
}

// ****************************************************************
// ＥＸシーン：ＥＸコール中
// ================================================================
bool tnm_excall_is_excall()
{
	return Gp_global->ex_call_flag;
}

// ****************************************************************
// ＥＸシーン：メッセージバック中
// ================================================================
bool tnm_excall_is_msg_back()
{
	return tnm_msg_back_is_open();
}

// ****************************************************************
// ＥＸシーン：メッセージウィンドウ消去中
// ================================================================
bool tnm_excall_is_hide_mwnd()
{
	return tnm_syscom_hide_mwnd_get_onoff_flag();
}

// ****************************************************************
// 早送り中かを判定
// ================================================================
bool tnm_is_skipping()
{
	// 入力による「Ctrl スキップ」
	// システムコマンドメニューの「既読早送り」
	// スクリプトの「Ctrl 禁止」
	// スクリプトの「早送り禁止」
	return 
		tnm_excall_is_msg_back() ? false :				// メッセージバック中は早送りできない
		Gp_local->pod.ctrl_disable ? false :			// Ctrl 禁止ならもう絶対早送りでない
		Gp_global->ctrl_skip_flag ? true :				// Ctrl を押していたら早送り
		Gp_local->pod.skip_disable ? false :			// 早送り禁止なら早送りできない
		Gp_global->skip_because_skip_trigger ? true:	// スキップトリガーによるスキップ
		Gp_syscom->read_skip_flag;						// 早送りフラグ
}

// ****************************************************************
// 早送り中かを判定（スキップトリガーを除く）
// ================================================================
bool tnm_is_skipping_without_skip_trigger()
{
	// 入力による「Ctrl スキップ」
	// システムコマンドメニューの「既読早送り」
	// スクリプトの「Ctrl 禁止」
	// スクリプトの「早送り禁止」
	return 
		tnm_excall_is_msg_back() ? false :				// メッセージバック中は早送りできない
		Gp_local->pod.ctrl_disable ? false :			// Ctrl 禁止ならもう絶対早送りでない
		Gp_global->ctrl_skip_flag ? true :				// Ctrl を押していたら早送り
		Gp_local->pod.skip_disable ? false :			// 早送り禁止なら早送りできない
		Gp_syscom->read_skip_flag;						// 早送りフラグ
}

// ****************************************************************
// メッセージ早送り中かを判定
// ================================================================
bool tnm_is_skipping_msg()
{
	// 入力による「Ctrl スキップ」
	// システムコマンドメニューの「既読早送り」
	// スクリプトの「Ctrl 禁止」
	// スクリプトの「早送り禁止」
	return 
		tnm_excall_is_msg_back() ? false :				// メッセージバック中は早送りできない
		Gp_local->pod.ctrl_disable ? false :			// Ctrl 禁止ならもう絶対早送りでない
		Gp_global->ctrl_skip_flag ? true :				// Ctrl を押していたら早送り
		Gp_global->cs_skip_flag ? true :				// Ctrl+Shift を押していたら早送り
		Gp_local->pod.skip_disable ? false :			// 早送り禁止なら早送りできない
		Gp_global->skip_because_skip_trigger ? true:	// スキップトリガーによるスキップ
		Gp_syscom->read_skip_flag;						// 早送りフラグ
}

// ****************************************************************
// 未読を早送りするかを判定
// ================================================================
bool tnm_is_skip_unread_msg()
{
	return
		Gp_local->pod.skip_unread_message ? true :		// 一時的に未読でも早送りする
		Gp_config->system.skip_unread_message_flag;		// 環境設定
}

// ****************************************************************
// オートモードかを判定
// ================================================================
bool tnm_is_auto_mode()
{
	// 環境設定の「オードモード」
	// スクリプトの「一時的にオートモード」
	return Gp_local->pod.auto_mode_flag || Gp_config->auto_mode_onoff;
}

// ****************************************************************
// オートモードの文字待ち時間を取得
// ================================================================
int tnm_get_auto_mode_moji_wait()
{
	// スクリプトのオートモード
	if (Gp_local->pod.auto_mode_flag)	{
		if (Gp_local->pod.auto_mode_moji_wait >= 0)	{
			return Gp_local->pod.auto_mode_moji_wait;
		}
		else	{
			return Gp_config->auto_mode_moji_wait;
		}
	}
	// コンフィグのオートモード
	else	{
		return Gp_config->auto_mode_moji_wait;
	}
}

// ****************************************************************
// オートモードの最小待ち時間を取得
// ================================================================
int tnm_get_auto_mode_min_wait()
{
	// スクリプトのオートモード
	if (Gp_local->pod.auto_mode_flag)	{
		if (Gp_local->pod.auto_mode_min_wait >= 0)	{
			return Gp_local->pod.auto_mode_min_wait;
		}
		else	{
			return Gp_config->auto_mode_min_wait;
		}
	}
	// コンフィグのオートモード
	else	{
		return Gp_config->auto_mode_min_wait;
	}
}

// ****************************************************************
// 自動でマウスカーソルを隠すかを判定
// ================================================================
bool tnm_is_mouse_cursor_hide()
{
	// 環境設定の「自動でマウスカーソルを隠す」
	// スクリプトの「自動でマウスカーソルを隠すを禁止」
	return
		Gp_local->pod.mouse_cursor_hide_onoff == 1 ? true :
		Gp_local->pod.mouse_cursor_hide_onoff == 0 ? false :
		Gp_config->mouse_cursor_hide_onoff;
}

// ****************************************************************
// 自動でマウスカーソルを隠す時間
// ================================================================
int tnm_get_mouse_cursor_hide_time()
{
	// 環境設定の「自動でマウスカーソルを隠す」
	// スクリプトの「自動でマウスカーソルを隠すを禁止」
	if (Gp_local->pod.mouse_cursor_hide_time != -1)
		return Gp_local->pod.mouse_cursor_hide_time;

	return Gp_config->mouse_cursor_hide_time;
}

// ****************************************************************
// メッセージウィンドウのアニメを行うか判定
// ================================================================
bool tnm_is_mwnd_anime_use()
{
	// スクリプトの「一時的にウィンドウアニメを行う」
	// スクリプトの「一時的にウィンドウアニメを行わない」
	// 環境設定の「ウィンドウアニメを行わない」
	return 
		Gp_local->pod.mwnd_anime_on_flag ? true :
		Gp_local->pod.mwnd_anime_off_flag ? false :
		!Gp_config->system.no_mwnd_anime_flag;
}

// ****************************************************************
// 声を止めるかを判定
// ================================================================
bool tnm_is_stop_koe()
{
	// スクリプトの「一時的に声を止める」
	// スクリプトの「一時的に声を止めない」
	// 環境設定の「声を止めない」
	return 
		Gp_local->pod.koe_dont_stop_on_flag ? false :
		Gp_local->pod.koe_dont_stop_off_flag ? true :
		!Gp_config->system.koe_dont_stop_flag;
}

// ****************************************************************
// メッセージウィンドウを隠すかを判定
// ================================================================
bool tnm_is_hide_mwnd()
{
	// システムコマンド：「メッセージウィンドウを隠す」がオンの場合はメッセージウィンドウを隠す
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return true;

	// ＥＸコール：「メッセージバック」中はメッセージウィンドウを隠す
	if (tnm_excall_is_msg_back())
		return true;

	// ローカルデータ：「メッセージウィンドウを表示しない」の場合は隠す
	if (Gp_local->pod.mwnd_disp_off_flag)
		return true;

	return false;
}

// ****************************************************************
// カーソルを表示するかを判定
// ================================================================
bool tnm_is_cursor_disp()
{
	// ＥＸコール中はカーソルを表示する
	if (tnm_excall_is_excall())
		return true;

	// メッセージバック中はカーソルを表示する
	if (tnm_excall_is_msg_back())
		return true;

	// オートモード中によりカーソルを隠す
	if (Gp_global->auto_mode_cursor_off)
		return false;

	// 自動でカーソルを消す機能
	if (tnm_is_mouse_cursor_hide())	{
		if (Gp_global->mouse_cursor_hide_time >= tnm_get_mouse_cursor_hide_time())	{
			return false;
		}
	}

	// シーンデータ：カーソルを隠す
	if (Gp_local->pod.cursor_disp_off)
		return false;

	return true;
}

// ****************************************************************
// 声を再生するかどうかを判定
// ================================================================
bool tnm_is_koe_play(int chara_no)
{
	// スキップ中は声を再生しない（スキップトリガーは除く）
	if (tnm_is_skipping_without_skip_trigger())
		return false;

	// 音全体のチェックが入っていないなら再生しない
	if (!Gp_config->play_all_sound_check)
		return false;

	// 音声のチェックが入っていないなら再生しない
	if (!Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE])
		return false;

	// 「音声なし」なら声を再生しない
	if (Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY)
		return false;

	// キャラ番号がマイナスなら再生する
	if (chara_no < 0)
		return true;

	// キャラ番号に対応する登録番号を求める
	for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
		for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
			if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
				// この登録番号を再生する？
				if (i < (int)Gp_config->chrkoe.size() && !Gp_config->chrkoe[i].onoff)
					return false;
			}
		}
	}

	// 再生する
	return true;
}

// ****************************************************************
// キャラクター音声オンオフを取得
// ================================================================
bool tnm_get_chrkoe_onoff(int chara_no)
{
	// キャラ番号がマイナスなら再生する
	if (chara_no < 0)
		return true;

	// キャラ番号に対応する登録番号を求める
	for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
		for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
			if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
				// この登録番号を再生する？
				if (i < (int)Gp_config->chrkoe.size() && !Gp_config->chrkoe[i].onoff)
					return false;
			}
		}
	}

	return true;
}

// ****************************************************************
// キャラクター音声ボリュームを取得
// ================================================================
int tnm_get_chrkoe_volume(int chara_no)
{
	int volume = 255;

	// キャラ番号に対応する登録番号を求める
	if (chara_no >= 0)	{
		for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
			for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
				if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
					volume = volume * Gp_config->chrkoe[i].volume / 255;
				}
			}
		}
	}

	return volume;
}

// ****************************************************************
// キャラクター音声の名前の伏字を公開するかどうか判定（公開フラグをＯＮにします）
// ================================================================
void tnm_is_chrkoe_name_check(CTSTR& name_str)
{
	if (Gp_chrkoe->empty())
		return;

	for (int i = 0; i < (int)Gp_chrkoe->size(); i++)	{
		if (!(*Gp_chrkoe)[i].look_flag && Gp_ini->config.chrkoe[i].check_mode == TNM_CHRKOE_CHECK_MODE_AUTO)	{		// まだ伏字状態で、モードが TNM_CHRKOE_CHECK_MODE_AUTO の場合
			if (Gp_ini->config.chrkoe[i].name_str == name_str || Gp_ini->config.chrkoe[i].check_name_str == name_str)	{
				(*Gp_chrkoe)[i].look_flag = true;
			}
		}
	}
}

// ****************************************************************
// 既読早送りを更新
// ================================================================
void tnm_update_read_skip()
{
	int rf_scn_no = Gp_local->pod.cur_read_flag_scn_no;
	int rf_flag_no = Gp_local->pod.cur_read_flag_flag_no;

	// 既読早送りが可能かどうかを判定する
	if (false);
	else if (rf_scn_no < 0 || (int)Gp_read_flag->size() <= rf_scn_no)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else if (rf_flag_no < 0 || (int)(*Gp_read_flag)[rf_scn_no].size() <= rf_flag_no)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else if ((*Gp_read_flag)[rf_scn_no][rf_flag_no] == 0)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else	{
		Gp_global->read_skip_enable_flag = true;
	}

	// 早送りが不可能な場合は早送りを止める
	if (!Gp_global->read_skip_enable_flag && !tnm_is_skip_unread_msg())	{
		tnm_syscom_read_skip_set_onoff_flag(false);
		Gp_global->cant_auto_skip_before_click = true;	// クリックするまで自動早送りを禁止する
	}
}

// ****************************************************************
// 自動早送りを開始する
// ================================================================
void tnm_update_start_skip()
{
	// 「クリックするまで自動早送りを禁止する」フラグが立っていれば何もしない
	if (Gp_global->cant_auto_skip_before_click)
		return;

	// 自動早送りがＯＮの場合
	if (tnm_syscom_auto_skip_get_onoff_flag())	{
		// 既読もしくは、未読スキップがＯＮの場合
		if (Gp_global->read_skip_enable_flag || tnm_is_skip_unread_msg())	{
			// 早送りを開始
			tnm_syscom_read_skip_set_onoff_flag(true);
		}
	}
}

// ****************************************************************
// メッセージスピードを取得する
// ================================================================
int tnm_get_total_msg_speed()
{
	// シーンでノーウェイトが指定してあればノーウェイト
	if (Gp_local->pod.msg_nowait)
		return 0;

	// シーンで速度が指定してあればその値
	if (Gp_local->pod.msg_speed != -1)
		return Gp_local->pod.msg_speed;

	// 環境設定でノーウェイトなら０秒
	if (Gp_config->message_speed_nowait)
		return 0;

	// 環境設定でノーウェイトでないなら環境設定の値
	return Gp_config->message_speed;
}



