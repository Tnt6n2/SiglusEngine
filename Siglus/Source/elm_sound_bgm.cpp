#include	"pch.h"

#include	"data/tnm_def.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"


// ****************************************************************
// BGM：初期化
// ================================================================
void C_elm_bgm::init(S_element element, int form, CTSTR& name)
{
	// 基底の初期化
	C_tnm_element::init(element, form, name, NULL);

	// プレイヤーを作成
	m_player_list.resize(TNM_BGM_PLAYER_CNT);

	// プレイヤーを初期化
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].init();

	// パラメータを初期化
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_pause_flag = false;

	// 作業変数を初期化
	m_cur_player_id = -1;
}

// ****************************************************************
// BGM：解放
// ================================================================
void C_elm_bgm::free()
{
	// プレイヤーを解放
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].free();

	// プレイヤーを削除
	m_player_list.clear();
}

// ****************************************************************
// BGM：再初期化
// ================================================================
void C_elm_bgm::reinit()
{
	// パラメータを初期化
	m_regist_name.clear();
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_pause_flag = false;

	// プレイヤーを再初期化
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].reinit();

	// プレイヤーのボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(m_volume, 0);
}

// ****************************************************************
// BGM：セーブ
// ================================================================
void C_elm_bgm::save(C_tnm_save_stream& stream)
{
	// パラメータをセーブ
	stream.save(m_regist_name);
	stream.save(m_volume);
	stream.save(m_delay_time);
	stream.save(m_loop_flag);
	stream.save(m_pause_flag);
}

// ****************************************************************
// BGM：ロード
// ================================================================
void C_elm_bgm::load(C_tnm_save_stream& stream)
{
	TSTR regist_name;
	int volume;
	int delay_time;
	bool loop_flag;
	bool ready_flag;

	// 一時変数にパラメータをロード
	stream.load(regist_name);
	stream.load(volume);
	stream.load(delay_time);
	stream.load(loop_flag);
	stream.load(ready_flag);

	// パラメータをプレイヤーに反映させる
	set_volume(volume);

	// ループ再生または準備していた場合は、再生を復元する
	if (loop_flag || ready_flag || delay_time > 0)
		play(regist_name, loop_flag, false, 0, 0, TNM_BGM_START_POS_INI, ready_flag, 0);
}

// ****************************************************************
// BGM：時間を進める
// ================================================================
void C_elm_bgm::update_time(int past_real_time)
{
	// 遅延再生の場合、時間が経ったら再開する
	if (m_delay_time > 0)	{
		m_delay_time -= past_real_time;
		if (m_delay_time <= 0)	{

			// 再開
			resume(false, m_fade_in_time, 0); 
		}
	}
}

// ****************************************************************
// BGM：再生（内部用）
// ================================================================
void C_elm_bgm::play(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	bool total_ready_only = ready_only;

	// 登録名を小文字に直す
	str_to_lower(regist_name);

	// 同じ登録名の場合
	if (regist_name == m_regist_name)	{
		// ループ中かつループ指定なら再生を行わない
		if (m_loop_flag && loop_flag)
			return;
	}

	// 現在のプレイヤーをフェードアウトさせる
	stop(fade_out_time);

	// 待ち時間がある場合は準備のみになる
	if (delay_time > 0)	{
		total_ready_only = true;
	}

	// BGM 登録名から BGM 番号を取得
	int bgm_no = tnm_get_bgm_no_by_regist_name(regist_name);

	// 再生
	int player_id = (m_cur_player_id + 1) % TNM_BGM_PLAYER_CNT;
	if (!m_player_list[player_id].play_bgm(bgm_no, loop_flag, fade_in_time, start_pos, total_ready_only))
		return;

	// パラメータを設定
	m_cur_player_id = player_id;
	m_regist_name = regist_name;
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_pause_flag = ready_only;

	// 終了待ち？
	if (wait_flag)
		wait(false, false);		// 終了待ち
}

// ****************************************************************
// BGM：再生
// ================================================================
void C_elm_bgm::play(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, true, false, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM：再生（ワンショット）
// ================================================================
void C_elm_bgm::play_oneshot(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, false, false, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM：再生（終了待ち）
// ================================================================
void C_elm_bgm::play_wait(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool ready_only, int delay_time)
{
	play(regist_name, false, true, fade_in_time, fade_out_time, start_pos, ready_only, delay_time);
}

// ****************************************************************
// BGM：停止
// ================================================================
void C_elm_bgm::stop(int fade_out_time)
{
	// 停止
	if (m_cur_player_id >= 0)
		m_player_list[m_cur_player_id].stop(fade_out_time);

	// フラグを初期化（2013/7/2 登録名も初期化しました。get_regist_name 対策。大丈夫かな…？）
	m_regist_name.clear();
	m_loop_flag = false;
}

// ****************************************************************
// BGM：一時停止
// ================================================================
void C_elm_bgm::pause(int fade_out_time)
{
	// 停止
	if (m_cur_player_id >= 0)
		m_player_list[m_cur_player_id].pause(fade_out_time);

	// ポーズフラグを立てる
	m_pause_flag = true;

	// ディレイをキャンセル
	m_delay_time = 0;
}

// ****************************************************************
// BGM：再開
// ================================================================
void C_elm_bgm::resume(bool wait_flag, int fade_in_time, int delay_time)
{
	if (delay_time < 0)
		delay_time = 0;

	// ポーズフラグを降ろす
	m_pause_flag = false;

	// 遅延再開でなければ再開する
	if (delay_time == 0)	{
		if (m_cur_player_id >= 0)
			m_player_list[m_cur_player_id].resume(fade_in_time);

		// 終了待ち
		if (wait_flag)
			wait(false, false);
	}

	// パラメータを設定
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
}

// ****************************************************************
// BGM：再生終了待ち
// ================================================================
void C_elm_bgm::wait(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BGM_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// BGM：フェードアウト終了待ち
// ================================================================
void C_elm_bgm::wait_fade(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BGM_FADE_OUT_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// BGM：再生中を判定
// ================================================================
int C_elm_bgm::check()
{
	if (m_cur_player_id >= 0)
		return m_player_list[m_cur_player_id].get_play_state();

	return TNM_PLAYER_STATE_FREE;
}

// ****************************************************************
// BGM：再生中かどうかを判定
// ================================================================
bool C_elm_bgm::is_playing()
{
	if (m_cur_player_id >= 0)	{
		if (m_player_list[m_cur_player_id].is_playing())
			return true;
	}

	return false;
}

// ****************************************************************
// BGM：フェードアウト中かどうかを判定
// ================================================================
bool C_elm_bgm::is_fade_out_doing()
{
	if (m_cur_player_id >= 0)	{
		if (m_player_list[m_cur_player_id].is_fade_out())
			return true;
	}

	return false;
}

// ****************************************************************
// BGM：ボリュームを設定／取得
// ================================================================
void C_elm_bgm::set_volume(int volume, int fade_time)
{
	// プレイヤーのゲームボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(volume, fade_time);

	// パラメータを設定
	m_volume = volume;
}

void C_elm_bgm::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_bgm::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// BGM：システムボリュームを設定
// ================================================================
void C_elm_bgm::set_system_volume(int volume)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		m_player_list[i].set_system_volume(volume);
	}
}

// ****************************************************************
// BGM：再生位置を取得
// ================================================================
int C_elm_bgm::get_play_pos()
{
	if (m_cur_player_id >= 0)	{
		return m_player_list[m_cur_player_id].get_play_pos();
	}
	else	{
		return 0;
	}
}

// ****************************************************************
// BGM：プレイヤーを取得
// ================================================================
C_tnm_player* C_elm_bgm::get_player(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// パラメータを取得
	return &m_player_list[player_no];
}
