#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_wipe.h"

#include	"element/elm_stage.h"
#include	"element/elm_editbox.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"

#include	"engine/eng_input.h"
#include	"engine/eng_message.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_editbox.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_dialog.h"

#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// 前方宣言
// ================================================================
bool	tnm_message_key_wait_proc_sub();
bool	tnm_message_key_wait_proc_sub_sub(C_elm_mwnd* p_mwnd);

// ****************************************************************
// プロセス処理：ゲーム終了時のワイプを開始する
// ================================================================
bool tnm_game_end_wipe_proc()
{
	// 次のプロセスへ
	tnm_pop_proc();

	// エディットボックスを全破壊する
	for (int i = 0; i < Gp_editbox_list->get_size(); i++)
		(*Gp_editbox_list)[i].destroy();

	// 音を止める
	Gp_sound->m_bgm.stop(Gp_ini->load_wipe_time);
	Gp_sound->m_koe.stop();
	Gp_sound->m_pcm.stop();
	for (int i = 0; i < Gp_sound->m_pcmch_list.get_size(); i++)
		Gp_sound->m_pcmch_list[i].stop();

	// ゲーム終了時のワイプ開始
	int option[TNM_WIPE_OPTION_MAX] = {0};
	Gp_wipe->start(S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX), TNM_WIPE_RANGE_SYSTEM_OUT, 1, Gp_ini->load_wipe_type, Gp_ini->load_wipe_time, 0, 0, option, true, 0, false, false);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：ゲーム開始時のワイプを開始する
// ================================================================
bool tnm_game_start_wipe_proc()
{
	// 次のプロセスへ
	tnm_pop_proc();

	// ゲーム開始時のワイプ開始
	int option[TNM_WIPE_OPTION_MAX] = {0};
	Gp_wipe->start(S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX), TNM_WIPE_RANGE_SYSTEM_IN, 1, Gp_ini->load_wipe_type, Gp_ini->load_wipe_time, 0, 0, option, true, 0, false, false);

	// システムワイプフラグを降ろす
	Gp_global->system_wipe_flag = false;

	// ※注意
	// ロードした時点で、ネクストステージは黒のはずです。
	// 上のワイプでは、フロント→ネクストのコピーを行っていないので、必ず黒からのワイプになります。
	// なので、システムワイプフラグはここで降ろしても大丈夫なのです。

	// ネクストステージを保存するなら、「システムワイプフラグを降ろす」を予約し、
	// ワイプ後にシステムワイプフラグを降ろさなければなりません。

	return true;
}

// ****************************************************************
// プロセス処理：ゲームタイマー開始
// ================================================================
bool tnm_game_timer_start_proc()
{
	// 次のプロセスへ
	tnm_pop_proc();

	// ★ゲームタイマー開始
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// プロセス処理：メニューに戻る
// ================================================================
bool tnm_return_to_menu_proc()
{
	// メニューに戻る
	if (Gp_local->cur_proc.option == 1)
	{
		// メッセ時バックは初期化しないフラグ
		Gp_global->reinit_msgbk_except_flag = true;
		tnm_scene_proc_restart_from_menu_scene();
		Gp_global->reinit_msgbk_except_flag = false;
	}
	else
	{
		tnm_scene_proc_restart_from_menu_scene();
	}

	// 「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// システムワイプフラグを降ろす
	Gp_global->system_wipe_flag = false;

	// ※注意
	// 「メニューに戻る」は「ロード」と違い、「ゲーム開始時のワイプ」がありません。
	// なので、ここでシステムワイプフラグを降ろしておく必要があります。

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：ロード
// ================================================================
bool tnm_load_proc()
{
	// ロード
	tnm_saveload_proc_load(Gp_local->cur_proc.option);

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：簡易ロード（デバッグ用）
// ================================================================
bool tnm_easy_load_proc()
{
	// もうロード処理は終わってる
	// ここでは後始末を行う

	// 次のプロセスへ
	tnm_pop_proc();

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：クイックロード
// ================================================================
bool tnm_quick_load_proc()
{
	// クイックロード
	tnm_saveload_proc_quick_load(Gp_local->cur_proc.option);

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：エンドロード
// ================================================================
bool tnm_end_load_proc()
{
	// エンドロード
	tnm_saveload_proc_end_load();

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：インナーロード
// ================================================================
bool tnm_inner_load_proc()
{
	// インナーロード
	tnm_saveload_proc_load_inner_save(Gp_local->cur_proc.option);

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：バックログロード
// ================================================================
bool tnm_backlog_load_proc()
{
	// バックログロード
	tnm_saveload_proc_backlog_load(Gp_local->cur_proc.option_time_id);

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：前の選択肢に戻る
// ================================================================
bool tnm_return_to_sel_proc()
{
	// 前の選択肢に戻る
	tnm_saveload_proc_return_to_sel();

	// ２．「ゲームタイマー開始」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// １．「ゲーム開始時のワイプ」を予約
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// システムワイプフラグを立てる
	Gp_global->system_wipe_flag = true;

	// １回だけフレームアクションを行う
	Gp_global->do_frame_action_flag = true;

	// ロードアフターコールを行う
	Gp_global->do_load_after_call_flag = true;

	// １回描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：タイムウェイト
// ================================================================
//		時間経過を待ちます。
//		timewait コマンドで発生します。
// ================================================================
bool tnm_time_wait_proc()
{
	if (false);

	// スキップ中は次のプロセスへ
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 時間が来たら次のプロセスへ
	else if (Gp_timer->local_game_time - Gp_local->cur_proc.option >= 0)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(-1);	// 結果：キャンセルキー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：キーウェイト
// ================================================================
//		キー入力を待ちます。すっ飛ばし可能です。
//		keywait で発生します。
// ================================================================
bool tnm_key_wait_proc()
{
	if (false);

	// スキップ中は次のプロセスへ
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キーを押したら次のプロセスへ
	else if (tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：カウンタウェイト
// ================================================================
//		カウンタが終了するのを待ちます。
//		counter[0].wait コマンドで発生します。
// ================================================================
bool tnm_counter_wait_proc()
{
	C_elm_counter* p_counter = (C_elm_counter *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// カウンタがなければ次のプロセスへ
	if (!p_counter)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// カウントが終了したら次のプロセスへ
	else if (p_counter->get_count() - Gp_local->cur_proc.option >= 0)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：メッセージウィンドウ開きアニメ待ち
// ================================================================
//		メッセージウィンドウの開きアニメが終わるのを待ちます。
//		open_wait コマンドなどで発生します。
// ================================================================
bool tnm_mwnd_open_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// メッセージウィンドウがなければ次のプロセスへ
	if (!p_mwnd)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中はアニメを終わらせる
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_mwnd->end_open_anime();
	}

	// アニメが終了したら次のプロセスへ
	if (!p_mwnd->is_open_anime_doing())	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：メッセージウィンドウ閉じアニメ待ち
// ================================================================
//		メッセージウィンドウの閉じアニメが終わるのを待ちます。
//		close_wait コマンドなどで発生します。
// ================================================================
bool tnm_mwnd_close_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// メッセージウィンドウがなければ次のプロセスへ
	if (!p_mwnd)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中はアニメを終わらせる
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_mwnd->end_close_anime();
	}

	// アニメが終了したら次のプロセスへ
	if (!p_mwnd->is_close_anime_doing())	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

bool tnm_mwnd_close_wait_all_proc()
{
	bool not_close_exist = false;

	// すべてのウィンドウに対して判定
	int stage_cnt = Gp_stage_list->get_size();
	for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
		int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
		for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			// スキップ中はアニメを終わらせる
			if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
				p_mwnd->end_close_anime();
			}

			// アニメが終了しているかどうか判定
			if (p_mwnd->is_close_anime_doing())	{
				not_close_exist = true;
			}
		}
	}

	// 全てのウィンドウのアニメが終わっていれば次のプロセスへ
	if (!not_close_exist)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：メッセージウェイト
// ================================================================
//		メッセージの表示終了を待つプロセスです。
//		msg_wait, pp, r コマンドなどで発生します。
// ================================================================
bool tnm_message_wait_proc()
{
	int stage_cnt = Gp_stage_list->get_size();

	bool not_disp_check = false;		// メッセージ表示中かどうかを判定

	// すべてのウィンドウに対して判定
	for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
		int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
		for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			if (p_mwnd->check_not_appear_moji())	{
				not_disp_check = true;		// メッセージ表示中が見つかった！
				break;
			}
		}
	}

	// クリックで文章を一括表示する
	if (!Gp_local->pod.not_skip_msg_by_click)	{
		if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

			// クリックで文章を一括表示したよフラグ
			Gp_global->msg_wait_skip_by_click = true;
		}
	}

	// クリックや早送り時は文章を一括表示
	if (not_disp_check && (Gp_global->msg_wait_skip_by_click || tnm_is_skipping_msg()))	{
		not_disp_check = false;
		
		// すべてのウィンドウに対して判定
		for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
			int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
			for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
				C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

				// クリックしたのですべての文字を表示
				p_mwnd->jump_to_msg_wait_by_nowait();

				// まだ非表示の文字があるか判定（オーバーフロー時やメッセージウィンドウアニメ中の場合はここを通るのです）
				if (p_mwnd->check_not_appear_moji())	{
					not_disp_check = true;
				}
			}
		}
	}

	// 処理中のウィンドウが存在しない場合は次のプロセスへ
	// ここの判定に早送り中かどうかを入れてはいけません。（オーバーフロー時は２度dispをかけないといけないのです）
	if (!not_disp_check)	{

		// すべてのウィンドウに対して処理
		for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
			int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
			for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
				C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

				// キー待ちカーソルのアニメを停止する
				p_mwnd->set_key_icon_appear(false);
			}
		}

		// 次のプロセスへ
		tnm_pop_proc();

		return true;
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：メッセージキーウェイト
// ================================================================
//		メッセージの表示が終了後のキー待ちです。
//		キー待ちアイコンが動きます。
//		pp, r コマンドなどで発生します。
// ================================================================
bool tnm_message_key_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// メッセージウィンドウがなければ次のプロセスへ
	if (!p_mwnd)	{

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップトリガーはもうオフにする
	Gp_local->pod.skip_trigger = false;

	// 文章を進めるかどうかを判定
	if (tnm_message_key_wait_proc_sub())	{

		// キー待ちカーソルのアニメを停止する
		p_mwnd->set_key_icon_appear(false);

		// 進めたら次のプロセスへ
		tnm_pop_proc();

#if 1
		// 描画に行くか？（高速化のため、数回に１回しか描画しないのです。）

		if (Gp_global->wait_display_vsync_total)	{	// 垂直同期を待つ場合
			Gp_global->disp_because_msg_wait_cnt ++;

			if (::timeGetTime() - Gp_timer->real_time > 100)	{		// 500ms 以上経っていたら強制的に０に戻して描画する
				Gp_global->disp_because_msg_wait_cnt = 0;
				Gp_global->disp_because_msg_wait_cnt_max = 0;

				return false;		// ★処理を１度ぬけて描画に行く
			}
			else if (Gp_global->disp_because_msg_wait_cnt >= Gp_global->disp_because_msg_wait_cnt_max / 100)	{
				Gp_global->disp_because_msg_wait_cnt = 0;

				// 30 fps 付近を保つ
				if (false);
				else if (Gp_global->frame_rate_100msec_total < 30)	{
					Gp_global->disp_because_msg_wait_cnt_max -= (30 - Gp_global->frame_rate_100msec_total) * 10;	// 遅いので描画回数を減らしてみる
					if (Gp_global->disp_because_msg_wait_cnt_max < 0)
						Gp_global->disp_because_msg_wait_cnt_max = 0;
				}
				else	{
					Gp_global->disp_because_msg_wait_cnt_max += (Gp_global->frame_rate_100msec_total - 29);	// 速いので描画回数を増やしてみる
					if (Gp_global->disp_because_msg_wait_cnt_max > 3000)
						Gp_global->disp_because_msg_wait_cnt_max = 3000;	// 最大で３０メッセージまで飛ばす
				}

				return false;		// ★処理を１度ぬけて描画に行く
			}
			else	{
				return true;		// 次のプロセスへ
			}
		}
		else	{
			return false;		// ★処理を１度ぬけて描画に行く
		}
#endif

		return true;	// 次のプロセスへ
	}
	else	{

		// キー待ちカーソルのアニメを開始する
		p_mwnd->set_key_icon_appear(true);
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：メッセージキーウェイト（サブ）
// ================================================================
//		以下の２つの共通処理部分です。
//		・メッセージの表示が終了後のキー待ち
//		・オーバーフロー中のキー待ち
// ================================================================
//		戻り値：文章を次に進める場合は true
// ================================================================
bool tnm_message_key_wait_proc_sub()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);
	if (!p_mwnd)
		return true;	// 文章を進める

	// 次に進めるかどうかを判定
	if (!tnm_message_key_wait_proc_sub_sub(p_mwnd))
		return false;	// 文章を進めない

	// -- 次に進める場合、各種カウントをリセットする

	// オートモードカウントを終了
	Gp_global->is_auto_mode_count_start = false;

	// 声の再生が終わってからの待ち時間カウント終了
	Gp_global->is_koe_only_count_start = false;

	// ここまでオートモードを進めた
	p_mwnd->set_auto_mode_end_moji_cnt();	// ▲▲▲マルチメッセージ

	// メッセージ速度計算をリセット
	p_mwnd->set_progress_start_flag(false);

	// 文章を次に進める
	return true;
}

bool tnm_message_key_wait_proc_sub_sub(C_elm_mwnd* p_mwnd)
{
	// マウスを動かせばオートモードカウントをリセット（アクティブ時のみ）
	if (Gp_global->active_flag && Gp_cur_input->mouse.pos != Gp_last_input->mouse.pos)	{
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// 実時刻
	}
	// 声の再生中はオートモードカウントをリセット
	if (Gp_sound->m_koe.is_playing())	{
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// 実時刻
	}
	// オートモードカウントを開始していなければ開始する
	if (!Gp_global->is_auto_mode_count_start)	{
		Gp_global->is_auto_mode_count_start = true;						// オートモードカウントを開始
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// 実時刻
	}
	// メッセージウィンドウを消している間は進まない
	if (tnm_syscom_hide_mwnd_get_onoff_flag())	{

		// 文章を進めない
		return false;
	}

	// 早送り中ならメッセージを進める
	if (tnm_is_skipping_msg())	{

		// 文章を次に進める
		return true;
	}

	// キーボードの決定キーでメッセージを進める
	if (tnm_input_use_key_down_up(VK_EX_KEYBOARD_DECIDE))	{

		// キーボードで進んだ場合のみはマウスカーソルを消す
		Gp_global->auto_mode_cursor_off = true;

		// 文章を次に進める
		return true;
	}

	// キーボード以外の決定キー、またはホイールで次に進む
	if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

		// 文章を次に進める
		return true;
	}

	// 「音声のみ」でメッセージが表示されていない場合は自動で次に進む
	if (Gp_config->koe_mode == TNM_KOE_MODE_KOEONLY && p_mwnd->get_koe_no() >= 0 && p_mwnd->get_koe_play_flag())	{

		// 声の再生が終われば次に進む
		if (!Gp_sound->m_koe.is_playing())	{

			// 声の再生が終わってからの待ち時間カウントが開始していなければ開始する
			if (!Gp_global->is_koe_only_count_start)	{
				Gp_global->is_koe_only_count_start = true;					// 声の再生が終わってからの待ち時間カウント開始
				Gp_global->koe_only_count_start_time = Gp_timer->real_time;	// 実時刻
			}

			// 声の再生が終わってからの待ち時間が終われば次に進む
			int wait_time = TNM_KOE_ONLY_WAIT;
			if (Gp_timer->real_time - Gp_global->koe_only_count_start_time > wait_time)	{

				// 文章を次に進める
				return true;
			}
		}
	}

	// オートモード中の場合は時間が経てば次へ進む
	if (tnm_is_auto_mode())	{

		// 声がある場合は声が終われば次へ進む
		if (p_mwnd->get_koe_no() >= 0 && p_mwnd->get_koe_play_flag() && !p_mwnd->get_koe_no_auto_mode_flag())	{

			// 声の再生が終われば次に進む
			if (!Gp_sound->m_koe.is_playing())	{

				// 声の再生が終わってからの待ち時間カウントが開始していなければ開始する
				if (!Gp_global->is_koe_only_count_start)	{
					Gp_global->is_koe_only_count_start = true;					// 声の再生が終わってからの待ち時間カウント開始
					Gp_global->koe_only_count_start_time = Gp_timer->real_time;	// 実時刻
				}

				// 声の再生が終わってからの待ち時間が終われば次に進む
				int wait_time = Gp_config->auto_mode_min_wait;
				if (Gp_timer->real_time - Gp_global->koe_only_count_start_time > wait_time)	{

					// オートモードで進んだのでマウスカーソルを消す
					Gp_global->auto_mode_cursor_off = true;

					// 文章を次に進める
					return true;
				}
			}
		}

		// 声がない場合はオートモードで計算
		else	{

			int wait_time = 0;

			// オートモード用の文字数が個別に設定されている場合
			if (Gp_local->pod.auto_mode_moji_cnt > 0)	{

				int moji_cnt = Gp_local->pod.auto_mode_moji_cnt;
				wait_time = tnm_get_auto_mode_moji_wait() * moji_cnt + tnm_get_auto_mode_min_wait();
			}
			else	{

				// 待つべき文字数を求めて時間を計算する
				int moji_cnt = p_mwnd->get_disp_moji_cnt() - p_mwnd->get_auto_mode_end_moji_cnt();
				wait_time = tnm_get_auto_mode_moji_wait() * moji_cnt + tnm_get_auto_mode_min_wait();
			}

			if (Gp_timer->real_time - Gp_global->auto_mode_count_start_time > wait_time)	{
				// ↑ここの不等号を '>=' にしていはいけません。
				// なぜなら、０秒に設定したときに、「声を再生中だからタイマーをリセット」という処理中にも関わらず
				// オートモードで文章を進めてしまうからです。

				// オートモードで進んだのでマウスカーソルを消す
				Gp_global->auto_mode_cursor_off = true;

				// 文章を次に進める
				return true;
			}
		}
	}

	// 文章を進めない
	return false;
}

// ****************************************************************
// プロセス処理：ＢＧＭウェイト
// ================================================================
bool tnm_bgm_wait_proc()
{
	C_elm_bgm* p_bgm = (C_elm_bgm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ＢＧＭがなければ次のプロセスへ
	if (!p_bgm)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// ＢＧＭが終わってたら次のプロセスへ
	if (!p_bgm->is_playing())	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（ＢＧＭは止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（ＢＧＭは止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：ＢＧＭフェードアウトウェイト
// ================================================================
bool tnm_bgm_fade_out_wait_proc()
{
	C_elm_bgm* p_bgm = (C_elm_bgm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ＢＧＭがなければ次のプロセスへ
	if (!p_bgm)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// ＢＧＭのフェードアウトが終わってたら次のプロセスへ
	if (!p_bgm->is_fade_out_doing())	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（ＢＧＭは止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（ＢＧＭは止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：声ウェイト
// ================================================================
bool tnm_koe_wait_proc()
{
	C_elm_koe* p_koe = &Gp_sound->m_koe;

	// 声がなければ次のプロセスへ
	if (!p_koe)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 声が終わってたら次のプロセスへ
	if (!p_koe->is_playing())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（声は止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（声は止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：効果音ウェイト
// ================================================================
bool tnm_pcm_wait_proc()
{
	C_elm_pcm* p_pcm = (C_elm_pcm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// 効果音がなければ次のプロセスへ
	if (!p_pcm)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 効果音が終わってたら次のプロセスへ
	if (!p_pcm->is_playing())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（効果音は止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（効果音は止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：効果音チャンネルウェイト
// ================================================================
bool tnm_pcmch_wait_proc()
{
	C_elm_pcmch* p_pcmch = (C_elm_pcmch *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// 効果音がなければ次のプロセスへ
	if (!p_pcmch)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 効果音が終わってたら次のプロセスへ
	if (!p_pcmch->is_playing())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（効果音は止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（効果音は止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：効果音フェードアウトウェイト
// ================================================================
bool tnm_pcmch_fade_out_wait_proc()
{
	C_elm_pcmch* p_pcmch = (C_elm_pcmch *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// 効果音がなければ次のプロセスへ
	if (!p_pcmch)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 効果音のフェードアウトが終わってたら次のプロセスへ
	if (!p_pcmch->is_fade_out_doing())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（効果音は止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（効果音は止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：効果音イベントウェイト
// ================================================================
bool tnm_pcm_event_wait_proc()
{
	C_elm_pcm_event* p_pcm_event = (C_elm_pcm_event *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// 効果音イベントがなければ次のプロセスへ
	if (!p_pcm_event)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// 効果音イベントが終わってたら次のプロセスへ
	if (!p_pcm_event->check())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ（効果音は止めない）
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたら次のプロセスへ（イベントは止めない）
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：ムービーウェイト
// ================================================================
bool tnm_mov_wait_proc()
{
	// ムービーは１つしかない
	C_elm_mov* p_mov = &Gp_sound->m_mov;

	// ムービーがなければ次のプロセスへ
	if (!p_mov)	{

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// ムービーが終わってたら次のプロセスへ
	if (!p_mov->is_playing())	{

		// ムービーを閉じる
		p_mov->close();

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		// 次のプロセスへ
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、決定キーが押されたら次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ↑ここの down_up は重要です。
		// up だけだと、Alt+Enter 時に、Enter の up でムービーが終わってしまいます。

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		// ムービーを閉じる
		p_mov->close();

		// 次のプロセスへ
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キャンセルキーが押されたら次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// ↑ここの down_up は重要です。
		// up だけだと、Alt+Enter 時に、Enter の up でムービーが終わってしまいます。

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(-1);	// 結果：キャンセルキー
		}

		// ムービーを閉じる
		p_mov->close();

		// 次のプロセスへ
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// デバッグ時、'M' キーが押されたら次のプロセスへ
	if (Gp_global->debug_flag && tnm_input_use_key_down_up(_T('M')))	{

		// ↑ここの down_up は重要です。
		// up だけだと、Alt+Enter 時に、Enter の up でムービーが終わってしまいます。

		// キー待ちなら結果を返す
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		// ムービーを閉じる
		p_mov->close();

		// 次のプロセスへ
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// このプロセスを続ける
	return false;
}

// ****************************************************************
// プロセス処理：ワイプ待ち
// ================================================================
//		ワイプが終了するのを待ちます。
// ================================================================
bool tnm_wipe_wait_proc()
{
	if (false);

	// スキップ中は次のプロセスへ
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		Gp_wipe->end();		// ワイプを終了する
		tnm_pop_proc();
	}

	// ワイプが終了していたら次のプロセスへ
	else if (Gp_wipe->m_type == -1)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		Gp_wipe->end();		// ワイプを終了する
		tnm_pop_proc();
	}

	// キー待ちの場合、キーが押されたらワイプを終了して次のプロセスへ
	else if (Gp_wipe->is_wipe_doing() && Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		Gp_wipe->end();		// ワイプを終了する
		tnm_pop_proc();
	}

	// ★必ず処理を１度ぬけて描画に行く！
	return false;
}

// ****************************************************************
// プロセス処理：シェイクウェイト
// ================================================================
//		シェイクが終了するのを待ちます。
// ================================================================
bool tnm_shake_wait_proc()
{
	// シェイクは１つしかない
	S_tnm_shake* p_shake = &Gp_screen->shake();

	// シェイクが終了していたら次のプロセスへ
	if (p_shake->m_no == -1)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_shake->end();
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらシェイクを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_shake->end();
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：クエイクウェイト
// ================================================================
//		クエイクが終了するのを待ちます。
// ================================================================
bool tnm_quake_wait_proc()
{
	// クエイクを取得
	C_elm_quake* p_quake = (C_elm_quake *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// クエイクがなければ次のプロセスへ
	if (!p_quake)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// クエイクが終了していたら次のプロセスへ
	if (p_quake->get_type() == TNM_QUAKE_TYPE_NONE)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// スキップ中は次のプロセスへ
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_quake->end(0);
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらシェイクを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_quake->end(0);
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：イベントウェイト
// ================================================================
//		イベントが終了するのを待ちます。
// ================================================================
bool tnm_event_wait_proc()
{
	C_elm_int_event* p_event = (C_elm_int_event *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// イベントが見つからなかったら次のプロセスへ
	if (!p_event)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}
	// イベントが終了していたら次のプロセスへ
	if (!p_event->check_event())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらシェイクを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		p_event->end_event();
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：全体イベントウェイト
// ================================================================
//		全体イベントが終了するのを待ちます。
// ================================================================
bool tnm_all_event_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// イベントが見つからなかったら次のプロセスへ
	if (!p_object)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// イベントが終了していたら次のプロセスへ
	if (!p_object->check_all_event())	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらシェイクを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_object->end_all_event();
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：オブジェクトムービーウェイト
// ================================================================
//		オブジェクトムービーが終了するのを待ちます。
// ================================================================
bool tnm_obj_mov_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// オブジェクトが見つからなかったら次のプロセスへ
	if (!p_object)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// ムービーが終了していたら次のプロセスへ
	if (!p_object->check_movie())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらムービーを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		p_object->init_type(true);
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：オブジェクト E-mote ウェイト
// ================================================================
//		オブジェクト E-mote が終了するのを待ちます。
// ================================================================
bool tnm_obj_emote_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// オブジェクトが見つからなかったら次のプロセスへ
	if (!p_object)	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// ムービーが終了していたら次のプロセスへ
	if (!p_object->emote_check_playing())	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// 結果：タイムオーバー
		}

		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キー待ちの場合、キーが押されたらアニメを終了して次のプロセスへ
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// 結果を返す
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// 結果：決定キー
		}

		p_object->emote_pass();
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：選択肢
// ================================================================
bool tnm_sel_proc()
{
	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：ボタン選択肢
// ================================================================
bool tnm_sel_btn_proc()
{
	// フロントのボタン選択肢を監視する
	C_elm_btn_select* p_sel = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_btn_sel;

	// 選択が終わっていれば次のプロセスへ
	if (!p_sel->is_processing())	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：ボタンオブジェクト選択肢
// ================================================================
bool tnm_sel_btn_obj_proc()
{
	// ターゲットのグループを取得
	C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// グループが見つからなかったら次のプロセスへ
	if (!p_group)	{
		tnm_pop_proc();
		return true;	// 次のプロセスへ
	}

	// キャンセル可能な場合
	if (p_group->get_cancel_flag() && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// 効果音
		tnm_play_se(p_group->get_cancel_se_no());
		if (p_group->get_wait_flag())	{
			p_group->end();				// 選択終了
			tnm_stack_push_int(-1);		// 結果をスタックに積む
			tnm_pop_proc();				// 次のプロセスへ
			return true;
		}
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：セーブダイアログを開く
// ================================================================
bool tnm_save_dialog_proc()
{
	// セーブダイアログを開く（キャプチャーは行わない）
	tnm_open_save_dialog(false);

	// 次のプロセスへ
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// プロセス処理：エンドセーブ（２段階目）
// ================================================================
bool tnm_end_save_proc()
{
	// エンドセーブ２段階目（キャプチャーを行わない）
	tnm_syscom_end_save_period_2(false);

	// 次のプロセスへ
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// プロセス処理：ゲーム終了
// ================================================================
bool tnm_end_game_proc()
{
	// 終わり
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;

	// 次のプロセスへ
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// プロセス処理：開始時の警告画面
// ================================================================
bool tnm_start_warning_proc()
{
	static TCHAR* warning_file_01 = _T("___SYSEVE_WARNING");
	static TCHAR* warning_file_02 = _T("___SYSEVE_BLACK");

	if (false);
	else if (Gp_local->cur_proc.option == 0)	{

		// 次のステートへ
		Gp_local->cur_proc.option = 1;

		// 警告画像がなければプロセスを抜ける（エラーを出さない）
		if (!tnm_check_pct(warning_file_01, false))	{
			tnm_pop_proc();
			return true;
		}

		// 各種操作を禁止
		Gp_local->pod.ctrl_disable = true;
		Gp_local->pod.syscom_menu_disable = true;
		Gp_local->pod.hide_mwnd_disable = true;
		Gp_local->pod.msg_back_disable = true;
		Gp_local->pod.shortcut_disable = true;

		// タイムウェイト
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT;
		proc.option = Gp_timer->local_game_time + 1000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 1)	{

		// 次のステートへ
		Gp_local->cur_proc.option = 2;

		// 警告用のオブジェクトを準備
		(*Gp_stage_list)[TNM_STAGE_BACK].m_object_list[0].create_pct(warning_file_01);
		(*Gp_stage_list)[TNM_STAGE_BACK].m_object_list[0].set_disp(1);

		// ワイプ
		int option[TNM_WIPE_OPTION_MAX] = {0};
		Gp_wipe->start(S_tnm_sorter(0, INT_MIN), S_tnm_sorter(0, INT_MAX), TNM_WIPE_RANGE_NORMAL, false
			, 0, 1000, 0, 0, option, true, 0, false, true);
	}
	else if (Gp_local->cur_proc.option == 2)	{

		// 次のステートへ
		Gp_local->cur_proc.option = 3;

		// 次の画像がなければ最終ステートへ
		if (!tnm_check_pct(warning_file_02, false))	{
			Gp_local->cur_proc.option = 99;
		}

		// タイムウェイト
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT; 
		proc.option = Gp_timer->local_game_time + 3000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 3)	{

		// 次のステートへ
		Gp_local->cur_proc.option = 4;

		// ワイプ
		int option[TNM_WIPE_OPTION_MAX] = {0};
		Gp_wipe->start(S_tnm_sorter(0, INT_MIN), S_tnm_sorter(0, INT_MAX), TNM_WIPE_RANGE_NORMAL, false
			, 0, 1000, 0, 0, option, true, 0, false, true);
	}
	else if (Gp_local->cur_proc.option == 4)	{

		// 最終のステートへ
		Gp_local->cur_proc.option = 99;

		// タイムウェイト
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT; 
		proc.option = Gp_timer->local_game_time + 1000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 99)	{

		// 各種操作を許可
		Gp_local->pod.ctrl_disable = false;
		Gp_local->pod.syscom_menu_disable = false;
		Gp_local->pod.hide_mwnd_disable = false;
		Gp_local->pod.msg_back_disable = false;
		Gp_local->pod.shortcut_disable = false;

		// 次のプロセスへ
		tnm_pop_proc();
		return true;
	}

	return false;	// このプロセスを続ける
}

// ****************************************************************
// プロセス処理：ツイート用のキャプチャーの次の処理
// ================================================================
bool tnm_tweet_capture_after_proc()
{
	// Twitter ダイアログを開く
	tnm_dlg_open_twitter_dialog();

	// 次のプロセスへ
	tnm_pop_proc();
	return true;
}
