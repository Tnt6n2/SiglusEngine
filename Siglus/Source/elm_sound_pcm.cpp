#include	"pch.h"

#include	"data/tnm_def.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// PCM：初期化
// ================================================================
void C_elm_pcm::init(S_element element, int form, CTSTR& name, int player_type)
{
	// 基底の初期化
	C_tnm_element::init(element, form, name, NULL);

	// 固定パラメータの初期化
	m_player_type = player_type;

	// プレイヤーを作成
	switch (player_type)	{
		//case TNM_PLAYER_TYPE_BGM:	m_player_list.resize(TNM_BGM_PLAYER_CNT);	break;
		//case TNM_PLAYER_TYPE_KOE:	m_player_list.resize(TNM_KOE_PLAYER_CNT);	break;
		case TNM_PLAYER_TYPE_PCM:	m_player_list.resize(TNM_PCM_PLAYER_CNT);	break;
		case TNM_PLAYER_TYPE_SE:	m_player_list.resize(TNM_SE_PLAYER_CNT);	break;
	}

	// プレイヤーを初期化
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].init();

	// パラメータを初期化
	m_next_player_id = 0;
}

// ****************************************************************
// PCM：解放
// ================================================================
void C_elm_pcm::free()
{
	// プレイヤーを解放
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].free();

	// プレイヤーを削除
	m_player_list.clear();
}

// ****************************************************************
// PCM：再初期化
// ================================================================
void C_elm_pcm::reinit()
{
	// パラメータを初期化
	m_volume = TNM_VOLUME_MAX;

	// プレイヤーを再初期化
	if (m_player_type == TNM_PLAYER_TYPE_SE)	{
		// SE の場合は初期化しない！
	}
	else	{
		for (int i = 0; i < (int)m_player_list.size(); i++)
			m_player_list[i].reinit();
	}

	// プレイヤーのボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(m_volume, 0);
}

// ****************************************************************
// PCM：セーブ
// ================================================================
void C_elm_pcm::save(C_tnm_save_stream& stream)
{
	// パラメータをセーブ
	stream.save(m_volume);
}

// ****************************************************************
// PCM：ロード
// ================================================================
void C_elm_pcm::load(C_tnm_save_stream& stream)
{
	// パラメータをロード
	stream.load(m_volume);
}

// ****************************************************************
// PCM：声を再生
// ================================================================
void C_elm_pcm::play_koe(int koe_no, int* p_used_player_no)
{
	// 空いているプレイヤーを探す
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// 再生中でないプレイヤーが見つかったので抜ける！

		// 次のプレイヤーへ
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// 再生
	m_player_list[m_next_player_id].play_koe(koe_no, false, TNM_JITAN_RATE_NORMAL, 0, false, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// 次のプレイヤーへ
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM：効果音を再生
// ================================================================
void C_elm_pcm::play_pcm(TSTR file_name, int* p_used_player_no)
{
	// 空いているプレイヤーを探す
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// 再生中でないプレイヤーが見つかったので抜ける！

		// 次のプレイヤーへ
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// 再生
	m_player_list[m_next_player_id].play_pcm(file_name, false, 0, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// 次のプレイヤーへ
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM：システム音を再生
// ================================================================
void C_elm_pcm::play_se(int se_no, int* p_used_player_no)
{
	// 空いているプレイヤーを探す
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		if (!m_player_list[m_next_player_id].is_using())
			break;	// 再生中でないプレイヤーが見つかったので抜ける！

		// 次のプレイヤーへ
		m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
	}

	// 再生
	m_player_list[m_next_player_id].play_se(se_no, false);
	if (p_used_player_no)	{
		*p_used_player_no = m_next_player_id;
	}

	// 次のプレイヤーへ
	m_next_player_id = (m_next_player_id + 1) % (int)m_player_list.size();
}

// ****************************************************************
// PCM：停止
// ================================================================
void C_elm_pcm::stop(int fade_out_time)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].stop(fade_out_time);
}

// ****************************************************************
// PCM：再生終了待ち
// ================================================================
void C_elm_pcm::wait(bool is_key_wait)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCM_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = is_key_wait;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCM：再生中判定
// ================================================================
int C_elm_pcm::check()
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
// PCM：ボリュームを設定／取得
// ================================================================
void C_elm_pcm::set_volume(int volume, int fade_time)
{
	// 各プレイヤーのゲームボリュームを設定
	for (int i = 0; i < (int)m_player_list.size(); i++)
		m_player_list[i].set_game_volume(volume, fade_time);

	// パラメータを設定
	m_volume = volume;
}

void C_elm_pcm::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_pcm::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// PCM：システムボリュームを設定
// ================================================================
void C_elm_pcm::set_system_volume(int volume)
{
	for (int i = 0; i < (int)m_player_list.size(); i++)	{
		m_player_list[i].set_system_volume(volume);
	}
}

// ****************************************************************
// PCM：プレイヤーを取得
// ================================================================
C_tnm_player* C_elm_pcm::get_player(int player_no)
{
	if (player_no < 0 || (int)m_player_list.size() <= player_no)
		return NULL;

	// パラメータを取得
	return &m_player_list[player_no];
}
