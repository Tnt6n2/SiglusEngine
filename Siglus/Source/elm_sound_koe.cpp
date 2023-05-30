#include	"pch.h"

#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// KOE：初期化
// ================================================================
void C_elm_koe::init(S_element element, int form, CTSTR& name)
{
	// 基底の初期化
	C_tnm_element::init(element, form, name, NULL);

	// プレイヤーを作成
	m_player_list.resize(TNM_KOE_PLAYER_CNT);

	// プレイヤーを初期化
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].init();
}

// ****************************************************************
// KOE：解放（グローバル）
// ================================================================
void C_elm_koe::free()
{
	// プレイヤーを解放
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].free();

	// プレイヤーを削除
	m_player_list.clear();
}

// ****************************************************************
// KOE：再初期化
// ================================================================
void C_elm_koe::reinit()
{
	// パラメータを初期化
	m_koe_no = -1;
	m_chara_no = -1;
	m_volume = TNM_VOLUME_MAX;
	m_ex_koe_flag = false;

	// プレイヤーを再初期化
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].reinit();

	// プレイヤーのボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(m_volume, 0);
}

// ****************************************************************
// KOE：セーブ
// ================================================================
void C_elm_koe::save(C_tnm_save_stream& stream)
{
	// パラメータをセーブ
	stream.save(m_volume);
}

// ****************************************************************
// KOE：ロード
// ================================================================
void C_elm_koe::load(C_tnm_save_stream& stream)
{
	int volume;

	// 一時変数にパラメータをロード
	stream.load(volume);

	// パラメータをプレイヤーに反映させる
	set_volume(volume);
}

// ****************************************************************
// KOE：再生
// ================================================================
void C_elm_koe::play(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only)
{
	// 現在の声を完全に停止する
	stop(0);

	// パラメータを設定
	m_koe_no = koe_no;
	m_chara_no = chara_no;
	m_ex_koe_flag = ex_koe_flag;

	// ボリュームタイプが変わるならボリュームを再設定する
	tnm_update_sound_volume(false);

	// 再生
	m_player_list[0].play_koe(koe_no, false, jitan_rate, play_pos_mills, false, check_only);

	// 再生終了待ち
	if (wait_flag)
	{
		wait(key_skip_flag, return_value_flag);
	}
	else if (return_value_flag)
	{
		tnm_stack_push_int(0);
	}
}

// ****************************************************************
// KOE：停止
// ================================================================
void C_elm_koe::stop(int fade_time)
{
	// 停止
	m_player_list[0].stop(fade_time);
}

// ****************************************************************
// KOE：再生終了待ち
// ================================================================
void C_elm_koe::wait(bool key_wait_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_KOE_WAIT;
	proc.key_skip_enable_flag = key_wait_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// KOE：再生中を判定
// ================================================================
int C_elm_koe::check()
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (m_player_list[i].get_play_state() == TNM_PLAYER_STATE_PLAY)
			return TNM_PLAYER_STATE_PLAY;
	}
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (m_player_list[i].get_play_state() == TNM_PLAYER_STATE_FADE_OUT)
			return TNM_PLAYER_STATE_FADE_OUT;
	}

	return TNM_PLAYER_STATE_FREE;
}

// ****************************************************************
// KOE：ボリュームを設定／取得
// ================================================================
void C_elm_koe::set_volume(int volume, int fade_time)
{
	// プレイヤーのゲームボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(volume, fade_time);

	// パラメータを設定
	m_volume = volume;
}

void C_elm_koe::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_koe::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// KOE：システムボリュームを設定
// ================================================================
void C_elm_koe::set_system_volume(int volume)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		m_player_list[i].set_system_volume(volume);
	}
}

// ****************************************************************
// KOE：プレイヤーを取得
// ================================================================
C_tnm_player* C_elm_koe::get_player(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// パラメータを取得
	return &m_player_list[player_no];
}

// ****************************************************************
// KOE：ストリームを取得
// ================================================================
C_sound_stream_base* C_elm_koe::get_stream(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// パラメータを取得
	return m_player_list[player_no].get_stream();
}

// ****************************************************************
// KOE：再生位置を取得（最大）
// ================================================================
int C_elm_koe::get_length()
{
	return m_player_list[0].get_length();
}
int C_elm_koe::get_length_by_millsecond()
{
	return m_player_list[0].get_length_by_millsecond();
}
int C_elm_koe::get_play_pos()
{
	return m_player_list[0].get_play_pos();
}
int C_elm_koe::get_play_pos_by_millsecond()
{
	return m_player_list[0].get_play_pos_by_millsecond();
}

// ****************************************************************
// KOE：口パクボリュームを取得
// ================================================================
float C_elm_koe::get_current_mouth_volume()
{
	return m_player_list[0].get_current_mouth_volume();
}
