#include	"pch.h"

#include	"tnm_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_timer.h"

#include	"engine/eng_etc.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// セーブ（ローカル）
// ================================================================
bool C_tnm_eng::save_local()
{
	// セーブデータをクリアする
	m_local_save_history_index = -1;
	m_local_save.save_id = S_tid();
	m_local_save.append_dir = Gp_dir->append_dir;
	m_local_save.append_name = Gp_dir->append_name;
	m_local_save.save_scene_title = tnm_get_scene_title();
	m_local_save.save_msg.clear();
	m_local_save.save_full_msg = Gp_local->cur_full_message;
	m_local_save.save_stream.clear();
	m_local_save.save_stream_ex.clear();

	// セーブデータ ID
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		m_local_save.save_id = sys_time;
	}

	// バッファを確保する
	m_local_save.save_stream.get_buffer().reserve(2000000);		// とりあえず 2MB 予約

	// レクサー
	m_local_save.save_stream.save(m_lexer.get_scn_name(m_lexer.get_cur_scn_no()));
	m_local_save.save_stream.save(m_lexer.get_cur_line_no());
	m_local_save.save_stream.save(m_lexer.get_prg_cntr());

	// シーン
	m_local.cur_proc.save(m_local_save.save_stream);
	m_local_save.save_stream.save(m_local.proc_stack);
	m_local_save.save_stream.save(m_local.cur_mwnd);
	m_local_save.save_stream.save(m_local.cur_sel_mwnd);
	m_local_save.save_stream.save(m_local.last_mwnd);
	m_local_save.save_stream.save(m_local.scene_title);
	m_local_save.save_stream.save(m_local.cur_full_message);
	m_local_save.save_stream.save(m_local.mwnd_btn_disable.get(), (int)m_local.mwnd_btn_disable.size());
	m_local_save.save_stream.save(m_local.font_name);

	// シーン（pod）
	m_local_save.save_stream.save(m_local.pod);

	// スタック
	m_local_save.save_stream.save(m_stack.int_now_cnt);									// int 個数
	m_local_save.save_stream.save(m_stack.int_p, m_stack.int_now_cnt * sizeof(int));	// int スタック

	// 文字列スタック
	m_local_save.save_stream.save((int)m_stack.str_stack.size());
	for (int i = 0; i < (int)m_stack.str_stack.size(); i++)
		m_local_save.save_stream.save(m_stack.str_stack[i]);

	// エレメントポイント
	m_local_save.save_stream.save((int)m_stack.stack_point_list.size());
	m_local_save.save_stream.save(m_stack.stack_point_list.get(), (int)m_stack.stack_point_list.size() * sizeof(int));

	// タイマー
	m_local_save.save_stream.save(m_timer.local_real_time);
	m_local_save.save_stream.save(m_timer.local_game_time);
	m_local_save.save_stream.save(m_timer.local_wipe_time);

	// システムコマンドメニュー
	m_local_save.save_stream.save(m_syscom.syscommenu);

	// フォグ
	m_fog.save_local(m_local_save.save_stream);

	// フラグ
	m_flag.save_local(m_local_save.save_stream);

	// ユーザインクプロパティ
	m_user_inc_prop.save(m_local_save.save_stream);

	// ユーザシーンプロパティリスト
	{
		// シーン番号 → シーン名に直しながら保存
		int scn_cnt = m_user_scn_prop_list.get_size();
		m_local_save.save_stream.save(scn_cnt);
		for (int i = 0; i < scn_cnt; i++)	{
			TSTR scn_name = m_lexer.get_scn_name(i);
			m_local_save.save_stream.save(scn_name);
			m_user_scn_prop_list[i].prop_list.save(m_local_save.save_stream);
		}
	}

	// カウンタ
	m_counter_list.save(m_local_save.save_stream);

	// フレームアクション
	m_frame_action.save(m_local_save.save_stream);

	// フレームアクションチャンネル
	m_frame_action_ch_list.save(m_local_save.save_stream);

	// Ｇ００バッファ
	m_g00_buf_list.save(m_local_save.save_stream);

	// マスク
	m_mask_list.save(m_local_save.save_stream);

	// ステージ
	m_stage_list[TNM_STAGE_BACK].save(m_local_save.save_stream);
	m_stage_list[TNM_STAGE_FRONT].save(m_local_save.save_stream);

	// スクリーン
	m_screen.save(m_local_save.save_stream);

	// サウンド
	m_sound.save(m_local_save.save_stream);

	// 効果音イベント
	m_pcm_event_list.save(m_local_save.save_stream);

	// エディットボックス
	m_editbox_list.save(m_local_save.save_stream);

	// コール
	m_call_list.save(m_local_save.save_stream);

	// メッセージバック
	m_msg_back.save(m_local_save.save_stream);

	// セーブポイントスタック
	m_sel_save_stock.save_stream.save(m_local_save.save_stream);

	// インナーセーブ
	m_local_save.save_stream.save((int)m_inner_save_list.size());
	for (int i = 0; i < (int)m_inner_save_list.size(); i++)
		m_inner_save_list[i].save_stream.save(m_local_save.save_stream);

	// 選択肢セーブリスト
	m_local_save.save_stream.save(m_sel_save.list.size());
	for (int i = 0; i < (int)m_sel_save.list.size(); i++)
		m_local_save.save_stream.save(m_sel_save.list[i]);

	// ローカル設定もセーブする
	save_local_ex();

	// セーブ履歴に追加
	if (m_global.debug_flag && Gp_ini->save_history_cnt > 0)
	{
		DWORD now_time = ::timeGetTime();
		int past_time = now_time - m_save_history.update_time;
		if (past_time >= 3000)
		{
			m_save_history.update_time = now_time;
			int index = (m_save_history.top + m_save_history.cnt) % Gp_ini->save_history_cnt;
			m_save_history.item[index].flag = 0;
			GetLocalTime(&Gp_eng->m_save_history.item[index].systemtime);
			m_save_history.item[index].local_save = m_local_save;
			if (m_save_history.cnt < Gp_ini->save_history_cnt)
			{
				m_save_history.cnt++;
			}
			else
			{
				m_save_history.top++;

				if (m_save_history.top >= Gp_ini->save_history_cnt)
				{
					m_save_history.top = 0;
				}
			}
			m_save_history.update_flag = true;
			m_local_save_history_index = index;
		}
	}

	return true;
}

// ****************************************************************
// ロード（ローカル）
// ================================================================
bool C_tnm_eng::load_local()
{
	if (m_local_save.save_stream.empty())
		return false;

	// シークセット
	m_local_save.save_stream.set_seek();

	// セーブストリームから順にデータをロード

	// レクサー
	{
		// 分割
		TSTR scn_name = m_local_save.save_stream.load_ret<TSTR>();
		int line_no = m_local_save.save_stream.load_ret<int>();
		int prg_cntr = m_local_save.save_stream.load_ret<int>();

		// シーン番号に変換
		int scn_no = m_lexer.get_scn_no(scn_name);
		if (scn_no == -1)
		{
			tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
			return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
		}
		// ロードしたパラメータを設定
		m_lexer.load_local(scn_no, line_no, prg_cntr);
	}

	// シーン
	m_local.cur_proc.load(m_local_save.save_stream);
	m_local_save.save_stream.load(m_local.proc_stack);
	m_local_save.save_stream.load(m_local.cur_mwnd);
	m_local_save.save_stream.load(m_local.cur_sel_mwnd);
	m_local_save.save_stream.load(m_local.last_mwnd);
	m_local_save.save_stream.load(m_local.scene_title);
	m_local_save.save_stream.load(m_local.cur_full_message);
	m_local_save.save_stream.load(m_local.mwnd_btn_disable.get(), (int)m_local.mwnd_btn_disable.size());
	m_local_save.save_stream.load(m_local.font_name);

	// シーン（pod）
	m_local_save.save_stream.load(m_local.pod);

	// スタック
	m_local_save.save_stream.load(m_stack.int_now_cnt);									// 個数をロード
	tnm_stack_realloc(m_stack.int_now_cnt);												// 個数分再確保を行う
	m_local_save.save_stream.load(m_stack.int_p, m_stack.int_now_cnt * sizeof(int));	// データをロード

	// 文字列スタック
	m_stack.str_stack.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_stack.str_stack.size(); i++)
		m_local_save.save_stream.load(m_stack.str_stack[i]);

	// エレメントポイント
	m_stack.stack_point_list.resize(m_local_save.save_stream.load_ret<int>());
	m_local_save.save_stream.load(m_stack.stack_point_list.get(), (int)m_stack.stack_point_list.size() * sizeof(int));

	// タイマー
	m_local_save.save_stream.load(m_timer.local_real_time);
	m_local_save.save_stream.load(m_timer.local_game_time);
	m_local_save.save_stream.load(m_timer.local_wipe_time);

	// システムコマンドメニュー
	m_local_save.save_stream.load(m_syscom.syscommenu);

	// フォグ
	m_fog.load_local(m_local_save.save_stream);

	// フラグ
	m_flag.load_local(m_local_save.save_stream);

	// ユーザインクプロパティ
	m_user_inc_prop.load(m_local_save.save_stream);

	// ユーザシーンプロパティリスト
	{
		// シーン名 → シーン番号に直しながら読み込み
		int scn_cnt = m_local_save.save_stream.load_ret<int>();
		for (int i = 0; i < scn_cnt; i++)	{
			TSTR scn_name = m_local_save.save_stream.load_ret<TSTR>();

			// シーン番号に変換
			int scn_no = Gp_lexer->get_scn_no(scn_name);
			if (scn_no == -1)	{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
				// シーンがなかったのでデータをスキップ
				m_local_save.save_stream.skip_fixed_array();
			}
			else	{
				// シーンがあったのでそのシーンに読み込む
				m_user_scn_prop_list[scn_no].prop_list.load(m_local_save.save_stream);
			}
		}
	}

	// カウンタ
	m_counter_list.load(m_local_save.save_stream);

	// フレームアクション
	m_frame_action.load(m_local_save.save_stream);

	// フレームアクションチャンネル
	m_frame_action_ch_list.load(m_local_save.save_stream);

	// Ｇ００バッファ
	m_g00_buf_list.load(m_local_save.save_stream);

	// マスク
	m_mask_list.load(m_local_save.save_stream);

	// ステージ
	m_stage_list[TNM_STAGE_BACK].load(m_local_save.save_stream);
	m_stage_list[TNM_STAGE_FRONT].load(m_local_save.save_stream);

	// スクリーン
	m_screen.load(m_local_save.save_stream);

	// サウンド
	m_sound.load(m_local_save.save_stream);

	// 効果音イベント
	m_pcm_event_list.load(m_local_save.save_stream);

	// エディットボックス
	m_editbox_list.load(m_local_save.save_stream);

	// コール
	m_call_list.load(m_local_save.save_stream);

	// メッセージバック
	m_msg_back.load(m_local_save.save_stream);

	// セーブポイントスタック
	m_sel_save_stock.save_stream.load(m_local_save.save_stream);

	// インナーセーブ
	m_inner_save_list.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_inner_save_list.size(); i++)
		m_inner_save_list[i].save_stream.load(m_local_save.save_stream);

	// 選択肢セーブリスト
	m_sel_save.list.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_sel_save.list.size(); i++)
		m_local_save.save_stream.load(m_sel_save.list[i]);

	// 選択肢セーブリストを調整する
	{
		// 念のためリストをソートする
		std::sort(m_sel_save.list.begin(), m_sel_save.list.end());

		// マージの要領で、両方に存在するものだけを残していく
		ARRAY<S_tid>::iterator itr_id = m_sel_save.list.begin();
		std::map<S_tid, BSP<S_tnm_local_save>>::iterator itr_map = m_sel_save.map.begin();
		while (1)	{

			if (itr_id == m_sel_save.list.end())	{
				m_sel_save.map.erase(itr_map, m_sel_save.map.end());
				break;
			}
			else if (itr_map == m_sel_save.map.end())	{
				m_sel_save.list.erase(itr_id, m_sel_save.list.end());
				break;
			}
			else if (*itr_id < itr_map->first)	{
				itr_id = m_sel_save.list.erase(itr_id);
			}
			else if (itr_map->first < *itr_id)	{
				itr_map = m_sel_save.map.erase(itr_map);
			}
			else	{
				++itr_id;
				++itr_map;
			}
		}
	}

	// ローカル設定もロードする
	load_local_ex();

	// セーブメッセージはクリアする
	// セーブされた時は、セーブメッセージは空のはずだからである。
	// セーブ後に追加されたメッセージは、ロード後も追加されるはず。
	m_local_save.save_msg.clear();

	// 似た理由で、フルメッセージはローカルからコピーします。
	m_local_save.save_full_msg = m_local.cur_full_message;

	return true;
}

// ****************************************************************
// セーブ（ローカルの現在の文章）
// ================================================================
bool C_tnm_eng::save_local_msg(CTSTR& msg)
{
	if (msg.empty())
		return false;

	m_local_save.save_msg += msg;
	if ((int)m_local_save.save_msg.size() > TNM_SAVE_MESSAGE_MAX_LEN)
		m_local_save.save_msg.resize(TNM_SAVE_MESSAGE_MAX_LEN);

	m_local_save.save_full_msg += msg;
	if ((int)m_local_save.save_full_msg.size() > TNM_SAVE_FULL_MESSAGE_MAX_LEN)
		m_local_save.save_full_msg.resize(TNM_SAVE_FULL_MESSAGE_MAX_LEN);

	// セーブ履歴に反映
	if (Gp_global->debug_flag)
	{
		if (m_local_save_history_index >= 0)
		{
			m_save_history.item[m_local_save_history_index].local_save.save_msg = m_local_save.save_msg;
			m_save_history.item[m_local_save_history_index].local_save.save_full_msg = m_local_save.save_full_msg;
			m_save_history.update_flag = true;
		}
	}

	return true;
}

// ****************************************************************
// セーブ（ローカル設定）
// ================================================================
bool C_tnm_eng::save_local_ex()
{
	// ストリームをクリアする
	m_local_save.save_stream_ex.clear();
	
	// ローカル設定をセーブする
	m_local_save.save_stream_ex.save(Gp_syscom->syscommenu.local_extra_switch, sizeof(Gp_syscom->syscommenu.local_extra_switch));
	m_local_save.save_stream_ex.save(Gp_syscom->syscommenu.local_extra_mode, sizeof(Gp_syscom->syscommenu.local_extra_mode));

	return true;
}

// ****************************************************************
// ロード（ローカル設定）
// ================================================================
bool C_tnm_eng::load_local_ex()
{
	if (m_local_save.save_stream_ex.empty())
		return false;

	// シークセット
	m_local_save.save_stream_ex.set_seek();
	
	// ローカル設定をロードする
	m_local_save.save_stream_ex.load(Gp_syscom->syscommenu.local_extra_switch, sizeof(Gp_syscom->syscommenu.local_extra_switch));
	m_local_save.save_stream_ex.load(Gp_syscom->syscommenu.local_extra_mode, sizeof(Gp_syscom->syscommenu.local_extra_mode));

	return true;
}

// ****************************************************************
// セーブ（コールデータ）
// ================================================================
bool C_tnm_eng::save_call()
{
	C_elm_call* p_cur_call = &m_call_list[m_call_list.get_call_cnt() - 1];

	// データを順にセーブ

	// レクサー
	p_cur_call->m_call_save.lexer_scn_name = m_lexer.get_scn_name(m_lexer.get_cur_scn_no());
	p_cur_call->m_call_save.lexer_line_no = m_lexer.get_cur_line_no();
	p_cur_call->m_call_save.lexer_prg_cntr = m_lexer.get_prg_cntr();

	// コールをプッシュ
	m_call_list.add_call();

	return true;
}

// ****************************************************************
// ロード（コールデータ）
// ================================================================
bool C_tnm_eng::load_call()
{
	// コールスタックが空なら終了
	if (m_call_list.get_call_cnt() == 1)	{	// 初期状態では１、これを空とする
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return m_error.set_error(TNM_ERROR_TYPE_FATAL, _T("コールスタックが空です。"));
	}

	// コールデータをポップ
	m_call_list.sub_call();

	// 現在のコールを取得
	C_elm_call* p_cur_call = &m_call_list[m_call_list.get_call_cnt() - 1];

	// データを順にロード

	// レクサー
	{
		TSTR scn_name = p_cur_call->m_call_save.lexer_scn_name;
		int line_no = p_cur_call->m_call_save.lexer_line_no;
		int prg_cntr = p_cur_call->m_call_save.lexer_prg_cntr;

		// シーン番号に変換
		int scn_no = Gp_lexer->get_scn_no(scn_name);
		if (scn_no == -1)	{
			tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
			return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
		}
		// ロードしたパラメータを設定
		m_lexer.load_local(scn_no, line_no, prg_cntr);
	}

	return true;
}

