#include	"pch.h"
#include	"ifc_eng.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_font.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_display.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/flow_script.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_input.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_font.h"
#include	"engine/eng_etc.h"
#include	"engine/ifc_steam.h"

#if __USE_EMOTE
#include	"engine/eng_emote.h"
#endif

#include	"../resource.h"

// ****************************************************************
// フレーム処理
// ================================================================
//		入力を取得
//		タイマーを更新（できるだけ早く行う。入力の処理が先に必要）
// ================================================================
//		ヒットをチェック（前回のヒット矩形と、今回の入力状態から計算）
//		ヒットイベント（当たったときに効果音を鳴らす…など）
//		各エレメントのフレーム初期化
//		スクリプト処理
//		各エレメントのフレーム処理（ここでヒット矩形を計算）
// ================================================================
bool C_tnm_eng::frame()
{
	if (!frame_local())
		return false;

	if (!frame_global())
		return false;

	return true;
}

// ****************************************************************
// フレームローカル処理
// ================================================================
//		入力を取得
//		タイマーを更新（できるだけ早く行う。入力の処理が先に必要）
// ================================================================
//		ヒットをチェック（前回のヒット矩形と、今回の入力状態から計算）
//		ヒットイベント（当たったときに効果音を鳴らす…など）
//		各エレメントのフレーム初期化
//		スクリプト処理
//		各エレメントのフレーム処理（ここでヒット矩形を計算）
// ================================================================

// 軽いフォントを使う判定
bool tnm_is_use_light_font()
{
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
		return false;	// キャプチャーを行うときは軽いフォントは使わない

	int type = Gp_local->cur_proc.type;
	if (type == TNM_PROC_TYPE_SEL || type == TNM_PROC_TYPE_SEL_BTN || type == TNM_PROC_TYPE_SEL_BTN_OBJ)
		return false;	// 選択肢中は軽いフォントは使わない

	int proc_cnt = (int)Gp_local->proc_stack.size();
	for (int i = 0; i < proc_cnt; i++)	{
		int type = Gp_local->proc_stack[i].type;
		if (type == TNM_PROC_TYPE_SEL || type == TNM_PROC_TYPE_SEL_BTN || type == TNM_PROC_TYPE_SEL_BTN_OBJ)
			return false;	// 選択肢中は軽いフォントは使わない
	}

	if (tnm_is_skipping_without_skip_trigger())
		return true;	// 早送り中は軽いフォントを使う（スキップトリガーは除く）

	return false;	// 普通は軽いフォントを使わない
}

bool C_tnm_eng::frame_local()
{
	// D3D デバイスチェック
	if (!tnm_check_device())
		return false;

	// Steam
#if __USE_STEAM
	steam_frame_proc();
#endif

	// 前回の情報を保存する
	m_global.total_game_screen_pos_last = m_global.total_game_screen_pos;
	m_global.total_game_screen_size_last = m_global.total_game_screen_size;

	// フレーム変数を初期化する
	m_global.active_flag = false;
	m_global.focused_flag = false;
	m_global.focused_flag_on_edit_box = false;
	m_global.editbox_hide_flag_total = false;
	m_global.disp_flag = true;
	m_global.present_flag = true;
	m_global.do_frame_action_flag = false;
	m_global.do_load_after_call_flag = false;
	m_global.sel_start_call_flag = false;
	m_global.msg_wait_skip_by_click = false;

	// アクティブ判定
	HWND h_active_wnd = GetActiveWindow();
	m_global.active_flag = (h_active_wnd == Gp_wnd->get_handle());

	// フォーカスウィンドウがエディットボックスかどうかを判定する
	// 「エディットボックスにフォーカスがある場合は Ctrl スキップを行わない」処理に使う
	HWND h_focused_wnd = GetFocus();
	if (h_focused_wnd)	{
		if (h_focused_wnd == Gp_wnd->get_handle())	{
			m_global.focused_flag = true;
		}
		else	{
			TCHAR class_name[256] = {0};
			GetClassName(h_focused_wnd, class_name, 256);
			if (TSTR(class_name) == WC_EDIT)	{
				m_global.focused_flag_on_edit_box = true;
			}
		}
	}

	// 解像度を変更した直後の処理
	if (m_global.change_display_mode_proc_cnt)	{
		Gp_global->alt_menu_wait_flag = true;
		m_global.change_display_mode_proc_cnt --;
		if (m_global.change_display_mode_proc_cnt == 0)	{
			tnm_reset_screen_size_mode();
		}
	}

#if _DEBUG
	// Shiftを押している間は何もしない
	if (is_key_down(VK_SHIFT))	{
		m_global.disp_flag = false;
		return true;
	}
#endif

	// ブレーク中は何もしない（ステップ実行中は処理できる）
	if (m_global.is_breaking && !m_global.break_step_flag)	{
		m_global.disp_flag = false;
		return true;
	}

	// ムービー準備中は何もしない
	if (m_sound.m_mov.is_readying())	{
		m_global.disp_flag = false;
		return true;
	}

	// 前回のマウス情報を設定しておく
	m_last_input = Gp_input->now;
	m_last_input.mouse.pos = m_cur_input.mouse.pos;		// 相対座標へ

	// 入力を更新
	Gp_input->frame();

	// １フレームにボタンを押した放したが来た場合、「放した」を強制的にキャンセル
	if (Gp_input->now.mouse.left.get_down_up_stock() == 2 && m_last_input.mouse.left.get_down_up_stock() == 0)	{
		Gp_input->now.mouse.left.unset_up();
		Gp_input->next.mouse.left.set_real_state(1);
	}

	// タイマー処理（入力を使ってるのでこの位置）
	timer_proc();

	// 今回の新規マウス情報を記憶しておく
	m_cur_input = Gp_input->now;

	// キーボードでマウスカーソル移動（ここではまだマウスは絶対座標に配置されている）
	mouse_move_proc();

	// マウス座標をクライアント座標に調整する
	Gp_cur_input->mouse.pos = Gp_wnd->screen_to_client(Gp_cur_input->mouse.pos);

	// ウィンドウサイズによってマウス座標が変更される
	Gp_cur_input->mouse.pos.x = (Gp_cur_input->mouse.pos.x - m_global.total_game_screen_pos.x) * m_global.game_screen_size.cx / m_global.total_game_screen_size.cx;
	Gp_cur_input->mouse.pos.y = (Gp_cur_input->mouse.pos.y - m_global.total_game_screen_pos.y) * m_global.game_screen_size.cy / m_global.total_game_screen_size.cy;

	// １フレームだけクリックを禁止する処理
	// コンテキストメニューを消すためのクリックはここでキャンセルされます。
	// Enter キーなどはメインウィンドウには来ないので処理する必要はありません。
	if (m_global.skip_click_1_frame)	{
		m_global.skip_click_1_frame = false;
		Gp_input->now.mouse.left.use();
		Gp_input->now.mouse.right.use();
	}

	// マウスを動かせばオートモードカウントをリセット
	if (Gp_cur_input->mouse.pos != m_last_input.mouse.pos)	{
		m_global.auto_mode_cursor_off = false;							// オートモード中に消していたカーソルを復帰させます。
	}
	// キーを押せばオートモードカウントをリセット
	//if (Gp_input->check_ex_key_down(VK_EX_DECIDE) || Gp_input->check_ex_key_down(VK_EX_CANCEL))	{
	//	m_global.auto_mode_count_start_time = m_timer.real_time;		// 実時刻
	//	m_global.auto_mode_cursor_off = false;							// オートモード中に消していたカーソルを復帰させます。
	//}
	// キーを押せば「クリックするまで自動早送りを禁止する」フラグを降ろす
	if (tnm_input_check_key_down(VK_EX_DECIDE) || tnm_input_check_key_down(VK_EX_CANCEL) || tnm_input_is_key_down(VK_CONTROL))	{
		m_global.cant_auto_skip_before_click = false;		// これで自動早送りが有効になる
	}
	// 自動でマウスカーソルを消す
	if (Gp_cur_input->mouse.pos != m_last_input.mouse.pos || tnm_input_check_key_down(VK_EX_DECIDE) || tnm_input_check_key_down(VK_EX_CANCEL))	{
		m_global.mouse_cursor_hide_time = 0;
	}
	else	{
		m_global.mouse_cursor_hide_time += Gp_timer->local_real_time_past;
	}

	// デバッグキー処理
	if (!debug_key_proc())
		return false;

	// システムキー処理
	if (!system_key_proc())
		return false;

	// ボタン処理
	if (!button_proc())
		return false;

	// キャンセルコール処理（ポップアップ時はゲーム処理じゃなくてシステム処理にするべきかも？）
	if (!cancel_call_proc())
		return false;

	// ボタンヒットテスト
	if (!button_hit_test_proc())
		return false;

	// ボタンイベント処理
	if (!button_event_proc())
		return false;

	// 左クリックで早送り停止（右クリックは右クリックメニューの呼び出しが優先です。早送りは止まりません）
	if (!m_local.pod.not_stop_skip_by_click)	{
		if (m_syscom.read_skip_flag && tnm_input_use_key_down(VK_EX_DECIDE))	{
			tnm_syscom_read_skip_set_onoff_flag(false);		// 早送りを止める
			m_global.cant_auto_skip_before_click = true;	// 「クリックするまで自動早送りを禁止する」フラグを立てる（次にアクションを起こせば自動早送りが再開される）
		}
	}

	// 早送りでないならメッセージスキップ速度を元に戻す
	if (!tnm_is_skipping())	{
		Gp_global->disp_because_msg_wait_cnt_max = 0;
	}

	// スキップトリガー
	if (!Gp_global->ex_call_flag && Gp_local->pod.skip_trigger)	{

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

		// 表示中があれば一括表示処理に進むのです。
		if (not_disp_check)	{

			// クリックを判定
			if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

				// クリックで文章を表示したよフラグ
				Gp_global->msg_wait_skip_by_click = true;
			}

			// クリックや早送り時は文章を一括表示
			if (Gp_global->msg_wait_skip_by_click)	{

				// すべてのウィンドウに対して判定
				for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
					int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
					for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
						C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

						// クリックしたのですべての文字を表示
						p_mwnd->jump_to_msg_wait_by_nowait();
					}
				}
			}
		}
		// 表示中がなければスキップトリガー発動なのです。
		else	{

			// クリックを判定（up のみ！）
			if (tnm_input_use_key_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{
				Gp_global->skip_because_skip_trigger = true;
			}
			else	{

				C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_cur_mwnd();
				if (p_mwnd)	{
				
					// キー待ちカーソルのアニメを開始する
					p_mwnd->set_key_icon_appear(true);
				}
			}
		}
	}

	// ================================================================
	// ★エレメントのフレーム初期化処理
	// ================================================================
	{
		// ステージのフレーム初期化
		int stage_cnt = m_stage_list.get_size();
		for (int i = 0; i < stage_cnt; i++)
			m_stage_list[i].frame_init();

		// メッセージバックのフレーム初期化
		m_msg_back.frame_init();

		// ＥＸコールステージのフレーム初期化
		int excall_stage_cnt = m_excall.m_stage_list.get_size();
		for (int i = 0; i < excall_stage_cnt; i++)
			m_excall.m_stage_list[i].frame_init();
	}

	// ================================================================
	// ★エレメントの時間を進める
	// ================================================================

	// エレメントの経過時間を求める
	int element_past_real_time = m_timer.local_real_time_past;
	int element_past_game_time = m_timer.local_game_time_past;
	int element_past_wipe_time = m_timer.local_wipe_time_past;
	{
		// ワイプの時間を進める
		m_wipe.update_time(element_past_wipe_time);

		// excall 以外について、時間停止中は進めない。ただしワイプだけは例外（設計ミスだな…）
		if (!m_local.pod.time_stop_flag)	{

			// カウンタの時間を進める
			if (!m_local.pod.counter_time_stop_flag)	{
				int counter_cnt = m_counter_list.get_size();
				for (int i = 0; i < counter_cnt; i++)
					m_counter_list[i].update_time(element_past_game_time, element_past_real_time);
			}

			// マスクの時間を進める
			int mask_cnt = m_mask_list.get_size();
			for (int i = 0; i < mask_cnt; i++)
				m_mask_list[i].update_time(element_past_game_time, element_past_real_time);

			// スクリーンの時間を進める
			m_screen.update_time(element_past_game_time, element_past_real_time);

			// フレームアクションの時間を進める
			if (!m_local.pod.frame_action_time_stop_flag)	{
				m_frame_action.update_time(element_past_game_time, element_past_real_time);

				// フレームアクションチャンネルの時間を進める
				int frame_action_ch_cnt = m_frame_action_ch_list.get_size();
				for (int i = 0; i < frame_action_ch_cnt; i++)
					m_frame_action_ch_list[i].update_time(element_past_game_time, element_past_real_time);
			}

			// ステージの時間を進める
			if (!m_local.pod.stage_time_stop_flag)	{
				m_stage_list[TNM_STAGE_BACK].update_time(element_past_game_time, element_past_real_time);
				m_stage_list[TNM_STAGE_FRONT].update_time(element_past_game_time, element_past_real_time);
				if (m_wipe.is_wipe_doing())	{
					m_stage_list[TNM_STAGE_NEXT].update_time(element_past_game_time, element_past_real_time);
				}
			}

			// フォグの時間を進める
			m_fog.update_time(element_past_game_time, element_past_real_time);

			// ＢＧＭの時間を進める
			m_sound.m_bgm.update_time(element_past_real_time);

			// 効果音チャンネルの時間を進める
			int pcmch_cnt = m_sound.m_pcmch_list.get_size();
			for (int i = 0; i < pcmch_cnt; i++)
				m_sound.m_pcmch_list[i].update_time(element_past_real_time);
		}

		// ＥＸコールの時間を進める
		if (m_excall.is_ready())	{

			// カウンタの時間を進める
			int excall_counter_cnt = m_excall.m_counter_list.get_size();
			for (int i = 0; i < excall_counter_cnt; i++)
				m_excall.m_counter_list[i].update_time(element_past_game_time, element_past_real_time);

			// フレームアクションの時間を進める
			m_excall.m_frame_action.update_time(element_past_game_time, element_past_real_time);

			// フレームアクションチャンネルの時間を進める
			int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size(); 
			for (int i = 0; i < excall_frame_action_ch_cnt; i++)
				m_excall.m_frame_action_ch_list[i].update_time(element_past_game_time, element_past_real_time);

			// ステージの時間を進める
			m_excall.m_stage_list[TNM_STAGE_BACK].update_time(element_past_game_time, element_past_real_time);
			m_excall.m_stage_list[TNM_STAGE_FRONT].update_time(element_past_game_time, element_past_real_time);
			if (m_wipe.is_wipe_doing())	{
				m_excall.m_stage_list[TNM_STAGE_NEXT].update_time(element_past_game_time, element_past_real_time);
			}
		}
	}

	// ================================================================
	// ★システムが使わなかった入力をスクリプトに渡す
	// ================================================================
	// スクリプトに入力を渡す
	// 2010/09/28 ここに移しました。
	// 何かこの中で入力を進めてんだけど…。
	// 毎フレーム input.next を処理してるよね。超やばいよね。１ループで disp ２回やったら入力取りこぼすよ？
	// どうしよう。
	// ================================================================
	script_input_proc();

	// ================================================================
	// ★メイン処理
	// ================================================================
	frame_main_proc();

	// ================================================================
	// ★フレームアクション処理
	// ================================================================
	frame_action_proc();

	// ================================================================
	// ★エレメントのフレーム処理
	// ================================================================
	{
		// 文字パラメータ（すっ飛ばし中の文字クオリティーを調整する）
		bool draw_futoku = m_global.total_font_futoku;
		bool draw_shadow = (m_global.total_font_shadow == TNM_FONT_SHADOW_MODE_NONE) ? false : true;

		if (tnm_is_use_light_font())
		{
			draw_futoku = false;
			draw_shadow = false;
		}

		// ワイプのフレーム処理
		m_wipe.frame();

		// マスクのフレーム処理
		int mask_cnt = m_mask_list.get_size(); 
		for (int i = 0; i < mask_cnt; i++)
			m_mask_list[i].frame();

		// スクリーンのフレーム処理
		m_screen.frame();

		// フレームアクションのフレーム処理
		m_frame_action.frame();

		// フレームアクションチャンネルのフレーム処理
		int frame_action_ch_cnt = m_frame_action_ch_list.get_size();
		for (int i = 0; i < frame_action_ch_cnt; i++)
			m_frame_action_ch_list[i].frame();

		// ステージのフレーム処理（ムービー再生中は何もしない）
		if (!m_sound.m_mov.is_playing())	{
			m_stage_list[TNM_STAGE_FRONT].frame(draw_futoku, draw_shadow, 0);
			if (m_wipe.is_wipe_doing())	{
				m_stage_list[TNM_STAGE_NEXT].frame(draw_futoku, draw_shadow, 0);
			}
		}

		// フォグのフレーム処理
		m_fog.frame();

		// 効果音イベントのフレーム処理
		int pcm_event_cnt = m_pcm_event_list.get_size();
		for (int i = 0; i < pcm_event_cnt; i++)
			m_pcm_event_list[i].frame(element_past_game_time, element_past_real_time);

		// エディットボックスのフレーム処理
		int editbox_cnt = m_editbox_list.get_size();
		for (int i = 0; i < editbox_cnt; i++)
			m_editbox_list[i].frame();

		// ＥＸコールのフレーム処理
		if (m_excall.is_ready())	{

			// フレームアクションのフレーム処理
			m_excall.m_frame_action.frame();

			// フレームアクションチャンネルのフレーム処理
			int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size();
			for (int i = 0; i < excall_frame_action_ch_cnt; i++)
				m_excall.m_frame_action_ch_list[i].frame();
		
			// ステージのフレーム処理
			m_excall.m_stage_list[TNM_STAGE_FRONT].frame(draw_futoku, draw_shadow, Gp_ini->excall_order);
			if (m_wipe.is_wipe_doing())	{
				m_excall.m_stage_list[TNM_STAGE_NEXT].frame(draw_futoku, draw_shadow, Gp_ini->excall_order);
			}
		}

		// メッセージバックのフレーム処理
		m_msg_back.frame(m_timer.local_game_time, draw_futoku, draw_shadow);
	}

	return true;
}

// ****************************************************************
// フレームグローバル処理
// ================================================================
//		ゲームタイマーが止まっていたり、解像度切り替え中だったりしても
//		常に通るフレーム処理を書きます。
// ================================================================
bool C_tnm_eng::frame_global()
{
	// マウスカーソルのフレーム処理
	tnm_cursor_frame(m_timer.real_time);

	// サウンドのフレーム処理
	m_sound.frame();

	// サウンドのボリュームを更新
	tnm_update_sound_volume(false);

	// フォントの切り替え
	tnm_update_font(false);

	// 垂直同期の切り替え処理
	bool wait_display_vsync_total = m_local.pod.wait_display_vsync_off_flag ? false : m_global.wait_display_vsync_def;
	tnm_set_wait_display_vsync(wait_display_vsync_total);

	// スクリーンサイズの切り替え処理
	screen_size_proc();

	// ビデオメモリの使用量を求める
	const C_d3d_device_state& device_state = G_d3d.device.state();
	int video_memory_default_size = device_state.vm_size;
	m_global.video_memory_rest_size = (int)G_d3d.device->GetAvailableTextureMem();
	m_global.video_memory_use_size = (video_memory_default_size - m_global.video_memory_rest_size) / 1024 / 1024;
	if (m_global.video_memory_use_size > m_global.video_memory_use_max_size)
		m_global.video_memory_use_max_size = m_global.video_memory_use_size;

	// フレームレートを計算（メッセージスキップ用）
	{
		static DWORD frame_cnt = 0;
		static DWORD last = timeGetTime();
		DWORD now = timeGetTime();
		int second_past_cnt = (now - last) / 100;

		if (second_past_cnt > 0)	{
			m_global.frame_rate_100msec[m_global.frame_rate_100msec_index] = frame_cnt / second_past_cnt;
			m_global.frame_rate_100msec_index++;
			if (m_global.frame_rate_100msec_index >= 10)	{
				m_global.frame_rate_100msec_index = 0;
			}
			m_global.frame_rate_100msec_total = 0;
			for (int i = 0; i < 10; i++)	{
				m_global.frame_rate_100msec_total += m_global.frame_rate_100msec[i];
			}
			last += second_past_cnt * 100;
			frame_cnt = 0;
		}

		frame_cnt ++;
	}

	return true;
}

// ****************************************************************
// フレームメイン処理
// ================================================================
bool C_tnm_eng::frame_main_proc()
{
	while (1)	{

		if (false);

		// スクリプト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SCRIPT)	{

			// ブレーク中でかつステップ実行フラグも立っていない場合
			if (m_global.is_breaking && !m_global.break_step_flag)	{
				m_global.disp_flag = false;	// 描画を行わない！
				break;						// 処理を抜ける
			}

			// スクリプト処理
			tnm_proc_script();
		}
		// 表示
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_DISP)	{
			tnm_pop_proc();		// 何もせずにこの処理を終了する
			break;				// 表示のために一度だけ処理を抜ける
		}
		// キャプチャーのみ
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_CAPTURE_ONLY)	{
			tnm_pop_proc();		// 何もせずにこの処理を終了する
			break;				// 表示のために一度だけ処理を抜ける
		}
		// ゲーム開始時のワイプを開始する
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_START_WIPE)	{
			if (!tnm_game_start_wipe_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ゲーム終了時のワイプを開始する
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_END_WIPE)	{
			if (!tnm_game_end_wipe_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ゲームタイマー開始
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_TIMER_START)	{
			if (!tnm_game_timer_start_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_LOAD)	{
			if (!tnm_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 簡易ロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_EASY_LOAD)	{
			if (!tnm_easy_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// クイックロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_QUICK_LOAD)	{
			if (!tnm_quick_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// エンドロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_LOAD)	{
			if (!tnm_end_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// インナーロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_INNER_LOAD)	{
			if (!tnm_inner_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// バックログロード
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BACKLOG_LOAD)	{
			if (!tnm_backlog_load_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// メニューに戻る
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_RETURN_TO_MENU)	{
			if (!tnm_return_to_menu_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 前の選択肢に戻る
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_RETURN_TO_SEL)	{
			if (!tnm_return_to_sel_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// タイムウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_TIME_WAIT)	{
			if (!tnm_time_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// キーウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_KEY_WAIT)	{
			if (!tnm_key_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// カウントウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_COUNT_WAIT)	{
			if (!tnm_counter_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// メッセージウィンドウ開きアニメ待ち
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_OPEN_WAIT)	{
			if (!tnm_mwnd_open_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// メッセージウィンドウ閉じアニメ待ち
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_CLOSE_WAIT)	{
			if (!tnm_mwnd_close_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 全てのメッセージウィンドウ閉じアニメ待ち
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_CLOSE_WAIT_ALL)	{
			if (!tnm_mwnd_close_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// メッセージウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MESSAGE_WAIT)	{
			if (!tnm_message_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// メッセージキーウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MESSAGE_KEY_WAIT)	{
			if (!tnm_message_key_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ノベルＲ送り
		//else if (m_local.cur_proc.type == TNM_PROC_TYPE_NOVEL_R)	{
		//	if (!tnm_novel_r_proc())
		//		break;		// プロセスが終わってないので抜ける
		//}
		// ＢＧＭウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BGM_WAIT)	{
			if (!tnm_bgm_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ＢＧＭフェードアウトウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BGM_FADE_OUT_WAIT)	{
			if (!tnm_bgm_fade_out_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 声ウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_KOE_WAIT)	{
			if (!tnm_koe_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 効果音ウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCM_WAIT)	{
			if (!tnm_pcm_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 効果音チャンネルウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCMCH_WAIT)	{
			if (!tnm_pcmch_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 効果音チャンネルフェードアウトウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCMCH_FADE_OUT_WAIT)	{
			if (!tnm_pcmch_fade_out_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 効果音イベントウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCM_EVENT_WAIT)	{
			if (!tnm_pcm_event_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ムービーウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MOV_WAIT)	{
			if (!tnm_mov_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ワイプウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_WIPE_WAIT)	{
			if (!tnm_wipe_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// シェイクウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SHAKE_WAIT)	{
			if (!tnm_shake_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// クエイクウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_QUAKE_WAIT)	{
			if (!tnm_quake_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// イベントウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_EVENT_WAIT)	{
			if (!tnm_event_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// 全イベントウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_ALL_EVENT_WAIT)	{
			if (!tnm_all_event_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ムービーオブジェクトウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_OBJ_MOV_WAIT)	{
			if (!tnm_obj_mov_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// E-mote オブジェクトウェイト
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_OBJ_EMOTE_WAIT)	{
			if (!tnm_obj_emote_wait_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// コマンド
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_COMMAND)	{
			C_tnm_proc proc = m_local.cur_proc;		// プロセスの情報を記憶しておいて
			tnm_pop_proc();							// プロセスを消費する
			if (!tnm_command_proc(proc.element.begin(), proc.element.end(), proc.arg_list_id, 
				proc.arg_list.get_sub().get(), 
				proc.arg_list.get_sub().get_end(), 
				NULL, FM_VOID)
				)
				break;		// プロセスが終わってないので抜ける
		}
		// 選択肢処理
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL)	{
			if (!tnm_sel_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ボタン選択肢処理
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL_BTN)	{
			if (!tnm_sel_btn_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// ボタンオブジェクト選択肢処理
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL_BTN_OBJ)	{
			if (!tnm_sel_btn_obj_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// セーブダイアログを開く
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SAVE_DIALOG)	{
			if (!tnm_save_dialog_proc())
				break;		// プロセスが終わってないので抜ける
		}
		// エンドセーブ（２段階目）
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_SAVE)	{
			if (!tnm_end_save_proc())
				break;		// プロセスが終わってないので抜ける
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_GAME)	{
			if (!tnm_end_game_proc())
				break;		// プロセスが終わってないので抜ける
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_START_WARNING)	{
			if (!tnm_start_warning_proc())
				break;		// プロセスが終わってないので抜ける
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_TWEET_CAPTURE_AFTER)	{
			if (!tnm_tweet_capture_after_proc())
				break;		// プロセスが終わってないので抜ける
		}

		// その他の処理
		else
			break;			// 処理を抜ける
	}

	return true;
}

// ****************************************************************
// タイマー処理
// ================================================================
bool C_tnm_eng::timer_proc()
{
	const int amari_unit = 16;

	// 前回の時刻を記憶
	int real_time_last = m_timer.real_time;

	// 今回の実時刻を計算する
	m_timer.real_time = ::timeGetTime();
	m_timer.real_time_past = m_timer.real_time - real_time_last;

	// 総プレイ時間を計算する
	m_timer.global_real_time += m_timer.real_time_past;

	// 経過した時間を求める
	m_timer.local_real_time_past = m_timer.real_time_past * amari_unit;		// 一旦１６倍進めておく
	m_timer.local_game_time_past = m_timer.real_time_past * amari_unit;		// 一旦１６倍進めておく
	m_timer.local_wipe_time_past = m_timer.real_time_past * amari_unit;		// 一旦１６倍進めておく

	// Ctrl キーですっ飛ばしフラグを立てる（エディットボックスがアクティブなときは早送りしない）
	m_global.ctrl_skip_flag = false;
	m_global.cs_skip_flag = false;
	if (tnm_input_is_key_down(VK_SHIFT) && tnm_input_is_key_down(VK_CONTROL) && !m_global.focused_flag_on_edit_box)	{
		m_global.cs_skip_flag = true;
	}
	else if (tnm_input_is_key_down(VK_CONTROL) && !m_global.focused_flag_on_edit_box)	{
		m_global.ctrl_skip_flag = true;
	}

	// Alt メニューを開いていた間はゲーム時間は進まない
	// ただし Ctrl 禁止中は時間を進める
	if (m_global.alt_menu_wait_flag && !Gp_local->pod.ctrl_disable)	{
		m_global.alt_menu_wait_flag = false;
		m_timer.local_game_time_past = 0;
		m_timer.local_wipe_time_past = 0;
	}

	// ゲームタイマー停止中は動かない
	if (!m_global.game_timer_move_flag)	{
		m_timer.local_real_time_past = 0;
		m_timer.local_game_time_past = 0;
	}

	// 早送り中はゲーム時間は３２倍速で進む
	if (tnm_is_skipping())	{
		m_timer.local_game_time_past *= 32;
		m_timer.local_wipe_time_past *= 32;
	}
	// デバッグキーによって実時間およびゲーム時間刻の進みは変わる
	else if (m_global.debug_flag)	{
		if (tnm_input_is_key_down(VK_HOME) && tnm_input_is_key_down(VK_END))	{
			m_timer.local_real_time_past = 0;		// Home + End: 時間は進まない
			m_timer.local_game_time_past = 0;
			m_timer.local_wipe_time_past = 0;
		}
		else	{
			if (tnm_input_is_key_down(VK_INSERT))	{	m_timer.local_real_time_past *= 4;	m_timer.local_game_time_past *= 4;	m_timer.local_wipe_time_past *= 4;	}		// Insert:	４倍速
			if (tnm_input_is_key_down(VK_DELETE))	{	m_timer.local_real_time_past *= 2;	m_timer.local_game_time_past *= 2;	m_timer.local_wipe_time_past *= 2;	}		// Delete:	２倍速
			if (tnm_input_is_key_down(VK_HOME))		{	m_timer.local_real_time_past /= 4;	m_timer.local_game_time_past /= 4;	m_timer.local_wipe_time_past /= 4;	}		// Home:	１／４倍速
			if (tnm_input_is_key_down(VK_END))		{	m_timer.local_real_time_past /= 8;	m_timer.local_game_time_past /= 8;	m_timer.local_wipe_time_past /= 8;	}		// End:		１／８倍速
		}
	}

	// １６倍進めたので１６で割る（ついでに余りを記憶しておく）
	m_timer.local_real_time_amari += m_timer.local_real_time_past % amari_unit;
	m_timer.local_real_time_past = m_timer.local_real_time_past / amari_unit;
	if (m_timer.local_real_time_amari >= amari_unit)	{
		m_timer.local_real_time_amari -= amari_unit;
		m_timer.local_real_time_past += 1;
	}
	m_timer.local_game_time_amari += m_timer.local_game_time_past % amari_unit;
	m_timer.local_game_time_past = m_timer.local_game_time_past / amari_unit;
	if (m_timer.local_game_time_amari >= amari_unit)	{
		m_timer.local_game_time_amari -= amari_unit;
		m_timer.local_game_time_past += 1;
	}
	m_timer.local_wipe_time_amari += m_timer.local_wipe_time_past % amari_unit;
	m_timer.local_wipe_time_past = m_timer.local_wipe_time_past / amari_unit;
	if (m_timer.local_wipe_time_amari >= amari_unit)	{
		m_timer.local_wipe_time_amari -= amari_unit;
		m_timer.local_wipe_time_past += 1;
	}

	// ゲーム時間を進める
	m_timer.local_real_time += m_timer.local_real_time_past;
	m_timer.local_game_time += m_timer.local_game_time_past;
	m_timer.local_wipe_time += m_timer.local_wipe_time_past;

	return true;
}

// ****************************************************************
// マウス移動処理
// ================================================================
bool C_tnm_eng::mouse_move_proc()
{
	// メインウィンドウが非アクティブなときは動かせない（ダイアログ上にある場合はアクティブなので動かせます）
	if (!m_global.active_flag)
		return true;

	// メインウィンドウにフォーカスがないときは動かせない（エディットボックス上にフォーカスがある場合は動かせません）
	if (!m_global.focused_flag)
		return true;

	// 禁止フラグが立っていれば動かせない
	if (m_local.pod.cursor_move_by_key_disable)
		return true;

	// フェッチ処理を行う場合はフェッチ処理
	if (tnm_is_fetch_proc())	{

		// フェッチ処理
		tnm_fetch_proc();
	}
	else	{

		double speed = 0.5f;
		double mouse_move_left = m_global.mouse_move_left_amari;
		double mouse_move_right = m_global.mouse_move_right_amari;
		double mouse_move_up = m_global.mouse_move_up_amari;
		double mouse_move_down = m_global.mouse_move_down_amari;
		if (tnm_input_is_key_down(VK_LEFT))	{
			mouse_move_left += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_RIGHT))	{
			mouse_move_right += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_UP))	{
			mouse_move_up += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_DOWN))	{
			mouse_move_down += (double)m_timer.real_time_past * speed;
		}
		int mouse_move_left_pixel = (int)mouse_move_left;
		int mouse_move_right_pixel = (int)mouse_move_right;
		int mouse_move_up_pixel = (int)mouse_move_up;
		int mouse_move_down_pixel = (int)mouse_move_down;
		m_global.mouse_move_left_amari = mouse_move_left - mouse_move_left_pixel;
		m_global.mouse_move_right_amari = mouse_move_right - mouse_move_right_pixel;
		m_global.mouse_move_up_amari = mouse_move_up - mouse_move_up_pixel;
		m_global.mouse_move_down_amari = mouse_move_down - mouse_move_down_pixel;
		if (mouse_move_left_pixel != 0 || mouse_move_right_pixel != 0 || mouse_move_up_pixel != 0 || mouse_move_down_pixel != 0)	{

			C_point new_pos = Gp_cur_input->mouse.pos;
			new_pos.x -= mouse_move_left_pixel;
			new_pos.x += mouse_move_right_pixel;
			new_pos.y -= mouse_move_up_pixel;
			new_pos.y += mouse_move_down_pixel;
			::SetCursorPos(new_pos.x, new_pos.y);

			// 改めて値を取得
			::GetCursorPos(&new_pos);
			Gp_cur_input->mouse.pos = new_pos;
		}
	}

	return true;
}

// ****************************************************************
// システムキー処理
// ================================================================
bool C_tnm_eng::system_key_proc()
{
	// フルスクリーン ( Alt + Enter )
	// この処理はスクリプトで禁止できない
	if (Gp_input->now.keyboard.key[VK_MENU].is_down())
	{
		if (Gp_input->now.keyboard.key[VK_RETURN].use_down_stock())
		{
			tnm_switch_screen_size_mode(false);
		}
	}

	// ゲーム処理
	if (m_global.game_timer_move_flag)
	{
		// メッセージバック
		system_key_proc_msg_back();

		// メッセージウィンドウを隠す
		system_key_proc_hide_mwnd();

		// ショートカットキー
		system_key_proc_shortcut();
	}

	return true;
}

// ****************************************************************
// システムキー処理：ショートカットキー
// ================================================================
bool C_tnm_eng::system_key_proc_shortcut()
{
	// ＥＸコール中は使えない
	if (m_global.ex_call_flag)
		return true;

	// メッセージバック中は使えない
	if (m_msg_back.is_open())
		return true;

	// ムービー再生中は使えない
	if (m_sound.m_mov.is_playing())
		return true;

	// メッセージウィンドウを隠している場合は使えない
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return true;
	
	// ショートカットが禁止されている場合は使えない
	if (m_local.pod.shortcut_disable)
		return true;

	// ショートカットキーの判定
	for (int i = 0; i < Gp_ini->shortcut_cnt; i++)
	{
		int key_code = Gp_ini->shortcut_key[i];
		if (0 <= key_code && key_code < 256)
		{
			int cond = Gp_ini->shortcut_cond[i];
			if (
				(cond == 0 && tnm_input_use_key_down(key_code)) || 
				(cond == 1 && tnm_input_use_key_down_up(key_code))
			)	{
				if (!Gp_ini->shortcut_scn[i].empty())
				{
					if (!Gp_ini->shortcut_cmd[i].empty())
					{
						tnm_scene_proc_call_user_cmd(Gp_ini->shortcut_scn[i], Gp_ini->shortcut_cmd[i], FM_VOID, true, false);
					}
					else
					{
						tnm_scene_proc_farcall(Gp_ini->shortcut_scn[i], Gp_ini->shortcut_z_no[i], FM_VOID, true, false);
					}
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// システムキー処理：メッセージバック
// ================================================================
bool C_tnm_eng::system_key_proc_msg_back()
{
	// ＥＸコール中も使える
	// ＥＸコール中に syscom.open_msg_back でメッセージバックを開くことがあるため、
	// 「PageUp ボタンでメッセージバックを開く」は禁止するべきだが、メッセージバックの操作自体は行なう必要がある

	// ムービー再生中はメッセージバックは使えない
	if (m_sound.m_mov.is_playing())
		return true;

	// ＥＸコール：メッセージウィンドウ消去中の場合は使えない
	if (tnm_excall_is_hide_mwnd())
		return true;

	// ＥＸコール：メッセージバック中の場合
	if (tnm_excall_is_msg_back() && Gp_local->cur_proc.type == TNM_PROC_TYPE_MSG_BACK)
	{
		// メッセージバックが表示されている場合
		if (!Gp_local->pod.msg_back_disp_off)
		{
			// スクロールの余波が発生しているかどうかを判定する
			if (!m_msg_back.check_aftereffects())
			{
				// PageUp キーでターゲットを移動
				if (tnm_input_is_key_down(VK_PRIOR))
				{
					m_msg_back.page_up_key_down();
				}
				// PageDown キーでターゲットを移動
				if (tnm_input_is_key_down(VK_NEXT))
				{
					m_msg_back.page_dw_key_down();
				}
				// ホイールＵＰでターゲットを移動
				if (Gp_input->now.mouse.wheel.use_up_stock())
				{
					m_msg_back.target_up_for_wheel();
				}
				// ホイールＤＯＷＮでターゲットを移動
				if (Gp_input->now.mouse.wheel.use_down_stock())
				{
					m_msg_back.target_down_for_wheel();
				}
			}

			// キャンセルキーで閉じる
			if (tnm_input_use_key_down(VK_EX_CANCEL) || tnm_input_use_key_down(VK_SPACE))
			{
				// システムコマンド：メッセージバックを閉じる
				tnm_syscom_close_msg_back();
				// オートモードカウンタをリセット
				Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// 実時刻
			}
		}
	}
	// ＥＸコール：メッセージバック中ではない場合
	else
	{
		// ＥＸコール：メッセージウィンドウ消去中の場合は使えない
		// ＥＸコール：ＥＸコール中の場合は使えない
		if (!tnm_excall_is_hide_mwnd() && !tnm_excall_is_excall())
		{
			// システムコマンド：メッセージバックが可能？
			if (tnm_syscom_msg_back_is_enable())
			{
				// PageUp キーかマウスホイールでメッセージバックを開く
				if (tnm_input_use_key_down(VK_PRIOR) || Gp_input->now.mouse.wheel.use_up_stock())
				{
					// システムコマンド：メッセージバックを開く
					tnm_syscom_open_msg_back();
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// システムキー処理：メッセージウィンドウを隠す
// ================================================================
bool C_tnm_eng::system_key_proc_hide_mwnd()
{
	// ムービー再生中は「メッセージウィンドウを隠す」は使えない
	if (m_sound.m_mov.is_playing())
		return true;

	// ＥＸコール：「ＥＸコール」中は「メッセージウィンドウを隠す」は使えない
	if (tnm_excall_is_excall())
		return true;

	// ＥＸコール：「メッセージバック」中は「メッセージウィンドウを隠す」は使えない
	if (tnm_excall_is_msg_back())
		return true;

	// ＥＸコール：「メッセージウィンドウを隠す」中の場合
	if (tnm_excall_is_hide_mwnd())
	{
		// 決定キー、キャンセルキー、スペースキーで復帰
		// ★down_up 重要。down だけだと、次の up で閉じるボタンをまた押してしまう。
		if (tnm_input_use_key_down_up(VK_EX_DECIDE) || tnm_input_use_key_down_up(VK_EX_CANCEL) || tnm_input_use_key_down_up(VK_SPACE))
		{
			// システムコマンド：「メッセージウィンドウを隠す」をオフ
			tnm_syscom_hide_mwnd_set_onoff_flag(false);
			// オートモードカウンタをリセット
			m_global.auto_mode_count_start_time = m_timer.real_time;	// 実時刻
		}
	}

	// ＥＸコール：「メッセージウィンドウを隠す」中でない場合
	else
	{
		// システムコマンド：「メッセージウィンドウを隠す」が使えるか
		if (tnm_syscom_hide_mwnd_is_enable())
		{
			// スペースキーでメッセージウィンドウを隠す
			if (tnm_input_use_key_down(VK_SPACE))
			{
				// システムコマンド：早送りを止める
				tnm_syscom_read_skip_set_onoff_flag(false);
				// システムコマンド：「メッセージウィンドウを隠す」
				tnm_syscom_hide_mwnd_set_onoff_flag(true);
			}
		}
	}

	return true;
}

// ****************************************************************
// デバッグキー処理
// ================================================================
bool C_tnm_eng::debug_key_proc()
{
	// デバッグ版のみ有効
	if (m_global.debug_flag)
	{
		// 最初からはじめる ( F1 )
		if (tnm_input_use_key_down(VK_F1))	{
			tnm_syscom_restart_from_start();
		}
		// ブレーク ( F3 )
		if (tnm_input_use_key_down(VK_F3))	{
			tnm_syscom_change_break();
		}
		// ステップ実行 ( 1 )
		if (m_global.is_breaking && tnm_input_check_key_down('1'))	{
			tnm_syscom_do_one_step();
		}
		// エラーダイアログ ( F8 )
		if (tnm_input_use_key_down(VK_F8))	{
			tnm_dlg_open_error_info_dialog();
		}
		// ss を開く ( O )
		if (tnm_input_use_key_down('O'))	{

			// シーンをエディタで開く
			tnm_open_by_editor(true);
		}
		// koe を開く ( K )
		if (tnm_input_use_key_down('K'))	{

			// 声を関連付けで開く
			tnm_open_koe(true);
		}
		// 現在のテキストをクリップボードにコピーする ( P )
		if (tnm_input_use_key_down('P'))	{

			TSTR str;
			str += _T("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			str += _T("■");

			// ゲーム名
			str += Gp_ini->game_name;

			// シーンタイトル
			if (!Gp_local->scene_title.empty())
				str += _T(" ") + Gp_local->scene_title;

			// 日付
			SYSTEMTIME system_time;
			GetLocalTime(&system_time);
			str += _T(" [") + get_sys_time_str() + _T("]");

			// メッセージバック中はメッセージバックから取得します。
			if (Gp_msg_back->is_open())	{
				str += _T("（※メッセージバック中）\n");

				// シーン名
				int scn_no = Gp_msg_back->get_scn_no();
				if (scn_no >= 0)	{
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					int line_no = Gp_msg_back->get_line_no();
					str += scn_name + _T(".ss (") + tostr(line_no) + _T("行目)\n\n");
				}

				// メッセージ
				int koe_no = Gp_msg_back->get_koe_no();				// 声
				if (koe_no >= 0)	{
					str += str_format(_T("KOE(%09d)"), koe_no);
				}
				TSTR name = Gp_msg_back->get_original_name();		// 名前
				if (!name.empty())	{
					str += _T("【") + name + _T("】\n");
				}
				TSTR msg = Gp_msg_back->get_debug_msg();			// メッセージ
				if (!msg.empty())	{
					str += msg + _T("\n");
				}
			}
			else	{
				str += _T("\n");

				// シーン名
				int scn_no = Gp_lexer->get_cur_scn_no();
				if (scn_no >= 0)	{
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					int line_no = Gp_lexer->get_cur_line_no();
					str += scn_name + _T(".ss (") + tostr(line_no) + _T("行目)\n\n");
				}

				// 現在のメッセージ
				C_elm_mwnd* p_mwnd = tnm_get_cur_mwnd();
				if (p_mwnd)	{

					int koe_no = p_mwnd->get_koe_no();				// 声
					if (koe_no >= 0)	{
						str += str_format(_T("KOE(%09d)"), koe_no);
					}
					TSTR name = p_mwnd->get_name();					// 名前
					if (!name.empty())	{
						str += _T("【") + name + _T("】\n");
					}
					TSTR msg = p_mwnd->get_debug_msg();				// メッセージ
					if (!msg.empty())	{
						str += msg + _T("\n");
					}
				}
			}

			str += _T("\n");
			::str_copy_to_clip_board_in_MB(str);
		}
	}

	return true;
}

// ****************************************************************
// ボタン処理
// ================================================================
bool C_tnm_eng::button_proc()
{
	// ボタンマネージャのフレーム初期化（ボタンの登録は、必ずここより後で行ってください！）
	m_btn_mng.frame_init();

	// ゲームが動いていない場合は処理しない
	if (!m_global.game_timer_move_flag)
		return true;

	// ステージのボタンフレーム初期化処理
	int stage_cnt = m_stage_list.get_size();
	for (int i = 0; i < stage_cnt; i++)
		m_stage_list[i].button_frame_init();

	// ＥＸコールのボタンフレーム初期化処理
	if (m_excall.is_ready())	{

		// ステージのボタンフレーム初期化処理
		int excall_stage_cnt = m_excall.m_stage_list.get_size();
		for (int i = 0; i < excall_stage_cnt; i++)
			m_excall.m_stage_list[i].button_frame_init();
	}

	// ボタン登録処理
	button_regist_proc();

	return true;
}

// ****************************************************************
// ボタン登録処理
// ================================================================
struct S_pair
{
	int		index;
	int		value;
};

bool pair_value_less_func(const S_pair& lhs, const S_pair& rhs)
{
	return lhs.value < rhs.value;
}
bool pair_value_greater_func(const S_pair& lhs, const S_pair& rhs)
{
	return lhs.value > rhs.value;
}

bool C_tnm_eng::button_regist_proc()
{
	// ★ないないのママきたモードが動くように Gameexe.ini で設定できるようにしないと…。
	// 2016/05/21 対処済み。メッセージバックより上のオブジェクトのみ当たるようにしました。

	// メッセージバック中またはＥＸコール中の場合
	if (m_msg_back.is_open() || m_global.ex_call_flag)
	{
		// メッセージバックが開いている
		if (m_msg_back.is_open() && !Gp_local->pod.msg_back_disp_off)
		{
			// メッセージバックのボタン登録処理
			m_msg_back.regist_button(m_excall.is_ready() ? Gp_ini->excall_order : INT_MIN);
		}
		// ＥＸステージが準備されている
		if (m_excall.is_ready())
		{
			// ステージのボタン登録処理
			m_excall.m_stage_list[TNM_STAGE_FRONT].regist_button(m_msg_back.is_open() ? Gp_ini->msg_back_order : INT_MIN);
		}
	}
	// その他の場合はフロントに対して処理
	else
	{
		// メッセージバックが開いている場合はメッセージバックより上のボタンのみ稼働

		// ステージのボタン登録処理
		m_stage_list[TNM_STAGE_FRONT].regist_button(INT_MIN);
	}

	return true;
}

// ****************************************************************
// ボタン当たり判定処理
// ================================================================
bool C_tnm_eng::button_hit_test_proc()
{
	// ゲームが動いていない場合は処理しない
	if (!m_global.game_timer_move_flag)
		return true;

	m_btn_mng.hit_test_proc();

	return true;
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
bool C_tnm_eng::button_event_proc()
{
	// ゲームが動いていない場合は処理しない
	if (!m_global.game_timer_move_flag)
		return true;

	// メッセージバックが開いている
	if (m_msg_back.is_open())	{

		// メッセージバックのボタンイベント処理
		if (!Gp_local->pod.msg_back_disp_off)	{
			m_msg_back.button_event();
		}
	}

	// ＥＸコール中（else ではない！ママきたモード対策）
	if (m_global.ex_call_flag)	{

		// ＥＸコールが準備されている場合
		if (m_excall.is_ready())	{

			// 選択中の場合はターゲットのＭＷを取得
			C_elm_mwnd* p_mwnd = NULL;
			if (Gp_local->cur_proc.type == TNM_PROC_TYPE_SEL)
				p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

			// オブジェクトのボタンイベント処理
			m_excall.m_stage_list[TNM_STAGE_FRONT].button_event(p_mwnd);

			// グループのキャンセル処理
			{
				C_elm_group_list* p_gl = &m_excall.m_stage_list[TNM_STAGE_FRONT].m_group_list;
				int excall_group_cnt = p_gl->get_size();

				// 優先度順に並び替える
				ARRAY<S_pair> pair_list(excall_group_cnt);
				for (int i = 0; i < excall_group_cnt; i++)	{
					pair_list[i].index = i;
					pair_list[i].value = (*p_gl)[i].get_cancel_priority();
				}
				std::stable_sort(pair_list.begin(), pair_list.end(), pair_value_greater_func);

				// 優先度順にキャンセル処理
				for (int i = 0; i < excall_group_cnt; i++)	{
					int index = pair_list[i].index;
					(*p_gl)[index].cancel_check();
				}
			}
		}
	}

	// その他の場合はフロントに対して処理
	else	{

		// 選択中の場合はターゲットのＭＷを取得
		C_elm_mwnd* p_mwnd = NULL;
		if (Gp_local->cur_proc.type == TNM_PROC_TYPE_SEL)
			p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

		// フロントのボタンイベント処理
		m_stage_list[TNM_STAGE_FRONT].button_event(p_mwnd);

		// グループのキャンセル処理
		{
			C_elm_group_list* p_gl = &m_stage_list[TNM_STAGE_FRONT].m_group_list;
			int group_cnt = p_gl->get_size();

			// 優先度順に並び替える
			ARRAY<S_pair> pair_list(group_cnt);
			for (int i = 0; i < group_cnt; i++)	{
				pair_list[i].index = i;
				pair_list[i].value = (*p_gl)[i].get_cancel_priority();
			}
			std::stable_sort(pair_list.begin(), pair_list.end(), pair_value_greater_func);

			// 優先度順にキャンセル処理
			for (int i = 0; i < group_cnt; i++)	{
				int index = pair_list[i].index;
				(*p_gl)[index].cancel_check();
			}
		}
	}

	return true;
}

// ****************************************************************
// キャンセルコール処理
// ================================================================
//		右クリックを押したときにキャンセルシーンを呼びます。
// ================================================================
bool C_tnm_eng::cancel_call_proc()
{
	// ゲームが動いていない場合は「キャンセルコール」は使えない
	if (!m_global.game_timer_move_flag)
		return true;

	// ムービー再生中は「キャンセルコール」は使えない
	if (m_sound.m_mov.is_playing())
		return true;

	// ＥＸコール：「ＥＸコール」中は「キャンセルコール」は使えない
	if (tnm_excall_is_excall())
		return true;

	// ＥＸコール：「メッセージバック」中は「キャンセルコール」は使えない
	if (tnm_excall_is_msg_back())
		return true;

	// ＥＸコール：「メッセージウィンドウを隠す」中は「キャンセルコール」は使えない
	if (tnm_excall_is_hide_mwnd())
		return true;

	// ローカル：システムメニューが使えない場合は「キャンセルコール」は使えない
	if (m_local.pod.syscom_menu_disable)
		return true;

	// フリック
	if (Gp_input->now.mouse.left.check_flick_stock())
	{
		double angle = Gp_input->now.mouse.left.get_flick_angle();

		for (int i = 0; i < Gp_ini->flick_scene_cnt; i++)
		{
			if (Gp_ini->flick_scene_angle[i] == 1)		// ↑ / 2
			{
				if (angle < -D3DX_PI / 2 || D3DX_PI / 2 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 2)		// ↓ / 2
			{
				if (-D3DX_PI / 2 <= angle && angle < D3DX_PI / 2)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 3)		// → / 2
			{
				if (0 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 4)		// ← / 2
			{
				if (angle < 0)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 5)		// ↑ / 4
			{
				if (angle < -D3DX_PI * 3 / 4 || D3DX_PI * 3 / 4 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 6)		// → / 4
			{
				if (D3DX_PI * 1 / 4 <= angle && angle < D3DX_PI * 3 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 7)		// ↓ / 4
			{
				if (-D3DX_PI * 1 / 4 <= angle && angle < D3DX_PI * 1 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 8)		// ← / 4
			{
				if (-D3DX_PI * 3 / 4 <= angle && angle < -D3DX_PI * 1 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// システムコマンド：早送りを止める
					tnm_syscom_read_skip_set_onoff_flag(false);
					// フリックシーンを呼ぶ
					tnm_scene_proc_call_flick_scene(i);
				}
			}

		}
	}

	// キャンセルキーでシステムコマンドシーンを開く
	if (tnm_input_use_key_down_up(VK_EX_CANCEL))
	{
		// システムコマンド：早送りを止める
		tnm_syscom_read_skip_set_onoff_flag(false);
		// システムコマンド：システムコマンドを開く
		tnm_syscom_open();
	}

	return true;
}

// ****************************************************************
// フレームアクション処理
// ================================================================
bool C_tnm_eng::frame_action_proc()
{
	// ================================================================
	// ★ロード直後コール処理
	// ================================================================
	if (m_global.do_load_after_call_flag)	{

		if (!Gp_ini->load_after_call_scene.empty())	{

			// シーンをコール
			tnm_scene_proc_farcall(Gp_ini->load_after_call_scene, Gp_ini->load_after_call_z_no, FM_VOID, false, true);

			// 再度スクリプトを実行する
			tnm_proc_script();
		}
	}

	// ================================================================
	// ★選択肢開始コール処理
	// ================================================================
	if (m_global.sel_start_call_flag)	{

		m_stage_list[TNM_STAGE_FRONT].m_btn_sel.do_sel_start_call();
	}

	// ================================================================
	// ★フレームアクション処理
	// ================================================================

	// ゲームタイマーが動いているまたはフレームアクションを実行するフラグが立っている場合のみ処理
	if (m_global.game_timer_move_flag || m_global.do_frame_action_flag)	{

		// 描画を行うときのみ処理
		if (m_global.disp_flag)	{

			// フレームアクション
			m_frame_action.do_action();

			// フレームアクションチャンネル
			int frame_action_ch_cnt = m_frame_action_ch_list.get_size(); 
			for (int i = 0; i < frame_action_ch_cnt; i++)
				m_frame_action_ch_list[i].do_action();

			// ステージのフレームアクション
			int stage_cnt = m_stage_list.get_size();
			for (int i = 0; i < stage_cnt; i++)
				m_stage_list[i].frame_action();

			// ＥＸコールのフレームアクション
			if (m_excall.is_ready())	{

				// フレームアクション
				m_excall.m_frame_action.do_action();

				// フレームアクションチャンネル
				int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size(); 
				for (int i = 0; i < excall_frame_action_ch_cnt; i++)
					m_excall.m_frame_action_ch_list[i].do_action();
			
				// ステージのフレームアクション
				int excall_stage_cnt = m_excall.m_stage_list.get_size();
				for (int s = 0; s < excall_stage_cnt; s++)
					m_excall.m_stage_list[s].frame_action();
			}
		}
	}

	return true;
}

// ****************************************************************
// スクリプト入力の処理
// ================================================================
bool C_tnm_eng::script_input_proc()
{
	// 入力の現在の状態を移しこむ
	m_script_input.next = Gp_input->now;
	// ★マウスの位置だけは特殊
	m_script_input.next.mouse.pos = Gp_cur_input->mouse.pos;

	// フレーム処理
	m_script_input.frame();

	return true;
}

// ****************************************************************
// スクリーンサイズ処理
// ================================================================
bool C_tnm_eng::screen_size_proc()
{
	WINDOWPLACEMENT wp = {0};
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(Gp_wnd->get_handle(), &wp);
	C_rect window_rect = wp.rcNormalPosition;

	// ウィンドウサイズが変更された（タブレットを回転したときなど）
	if (m_global.total_window_size != window_rect.size())
	{
		tnm_change_screen_size_mode_by_force(true);		// 強制的に変更する
	}

	// ウィンドウがデスクトップに収まるようにする
	Gp_wnd->rep_window_pos_outside_desktop();

	// 合計ゲーム画面サイズを求める
	{
		m_global.total_game_screen_pos = C_point(0, 0);
		m_global.total_game_screen_size = m_global.total_screen_size;

		// ウィンドウモード
		if (m_config.screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)
		{
//			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
//			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
//			m_global.total_game_screen_pos.x = 0;
//			m_global.total_game_screen_pos.y = 0;

			float scale_x = (float)m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			float scale_y = (float)m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			float scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = (int)ceil(m_global.game_screen_size.cx * scale / 1000);
			m_global.total_game_screen_size.cy = (int)ceil(m_global.game_screen_size.cy * scale / 1000);
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フリーモード
		else if (m_config.screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
		{
//			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
//			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
//			m_global.total_game_screen_pos.x = 0;
//			m_global.total_game_screen_pos.y = 0;

			float scale_x = (float)m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			float scale_y = (float)m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			float scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = (int)ceil(m_global.game_screen_size.cx * scale / 1000);
			m_global.total_game_screen_size.cy = (int)ceil(m_global.game_screen_size.cy * scale / 1000);
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：原寸
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_NORMAL)
		{
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：全画面
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_FIT)
		{
			int scale_x = m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			int scale_y = m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			int scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：全画面フィット
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_UDLRFIT)
		{
			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：上下フィット
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_UDFIT)
		{
			int scale = m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：左右フィット
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_LRFIT)
		{
			int scale = m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// フルスクリーンモード：カスタム
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_CUSTOM)
		{
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * m_config.fullscreen_scale.cx / 100;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * m_config.fullscreen_scale.cy / 100;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2 + m_global.game_screen_size.cx * m_config.fullscreen_move.cx / 100;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2 + m_global.game_screen_size.cy * m_config.fullscreen_move.cy / 100;
		}
	}

	// ゲームスクリーン矩形が変わったら色々と更新
	if (m_global.total_game_screen_pos != m_global.total_game_screen_pos_last || m_global.total_game_screen_size != m_global.total_game_screen_size_last)	{

		// ムービーの矩形を更新
		//m_sound.m_mov.update_rect();

		// 拡縮比率を計算
		float size_rate_x = (float)m_global.total_game_screen_size.cx / m_global.game_screen_size.cx;
		float size_rate_y = (float)m_global.total_game_screen_size.cy / m_global.game_screen_size.cy;

		m_sound.m_mov.set_size_rate(size_rate_x, size_rate_y);
		m_sound.m_mov.set_total_game_screen_pos(m_global.total_game_screen_pos);
		m_sound.m_mov.update_rect_org();

		// エディットボックス
		for (int i = 0; i < m_editbox_list.get_size(); i++)
			m_editbox_list[i].update_rect();
	}

	return true;
}

