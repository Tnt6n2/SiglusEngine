#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// PCMCH：初期化
// ================================================================
void C_elm_pcmch::init(S_element element, int form, CTSTR& name)
{
	// 基底の初期化
	C_tnm_element::init(element, form, name, NULL);

	// プレイヤーを初期化
	m_player.init();
}

// ****************************************************************
// PCMCH：解放
// ================================================================
void C_elm_pcmch::free()
{
	// プレイヤーを解放
	m_player.free();
}

// ****************************************************************
// PCMCH：再初期化
// ================================================================
void C_elm_pcmch::reinit()
{
	// パラメータを初期化
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = TNM_VOLUME_TYPE_PCM;
	m_chara_no = -1;
	m_volume = TNM_VOLUME_MAX;
	m_delay_time = 0;
	m_fade_in_time = 0;
	m_loop_flag = false;
	m_bgm_fade_target_flag = false;
	m_bgm_fade2_target_flag = false;
	m_bgm_fade_source_flag = false;
	m_ready_flag = false;

	// プレイヤーを再初期化
	m_player.reinit();

	// プレイヤーのボリュームを設定
	m_player.set_game_volume(m_volume, 0);
}

// ****************************************************************
// PCMCH：セーブ
// ================================================================
void C_elm_pcmch::save(C_tnm_save_stream& stream)
{
	// パラメータをセーブ
	stream.save(m_pcm_name);
	stream.save(m_bgm_name);
	stream.save(m_koe_no);
	stream.save(m_se_no);
	stream.save(m_volume_type);
	stream.save(m_chara_no);
	stream.save(m_volume);
	stream.save(m_delay_time);
	stream.save(m_loop_flag);
	stream.save(m_bgm_fade_target_flag);
	stream.save(m_bgm_fade2_target_flag);
	stream.save(m_bgm_fade_source_flag);
	stream.save(m_ready_flag);
}

// ****************************************************************
// PCMCH：ロード
// ================================================================
void C_elm_pcmch::load(C_tnm_save_stream& stream)
{
	TSTR pcm_name, bgm_name;
	int koe_no, se_no, volume_type, chara_no, volume, delay_time;
	bool loop_flag, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag;

	// 一時変数にパラメータをロード
	stream.load(pcm_name);
	stream.load(bgm_name);
	stream.load(koe_no);
	stream.load(se_no);
	stream.load(volume_type);
	stream.load(chara_no);
	stream.load(volume);
	stream.load(delay_time);
	stream.load(loop_flag);
	stream.load(bgm_fade_target_flag);
	stream.load(bgm_fade2_target_flag);
	stream.load(bgm_fade_source_flag);
	stream.load(ready_flag);

	// パラメータをプレイヤーに反映させる
	set_volume(volume);

	// ループ再生または準備していた場合は、再生を復元する
	if (loop_flag || ready_flag || delay_time > 0)	{
		if (!pcm_name.empty())	{
			play_pcm(pcm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		if (!bgm_name.empty())	{
			play_bgm(bgm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		else if (koe_no >= 0)	{
			play_koe(koe_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
		else if (se_no >= 0)	{
			play_se(se_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, ready_flag);
		}
	}
}

// ****************************************************************
// PCMCH：時間を進める
// ================================================================
void C_elm_pcmch::update_time(int past_real_time)
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
// PCMCH：再生（フルパラメータ）
// ================================================================
void C_elm_pcmch::play_pcm(TSTR pcm_name, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// 現在のサウンドを解放する
	m_player.release_sound();

	// ボリュームタイプが変わるならボリュームを再設定する
	tnm_update_sound_volume(false);

	// プレイヤーで再生
	if (!m_player.play_pcm(pcm_name, loop_flag, fade_in_time, ready_only))
		return;

	// パラメータを設定
	m_pcm_name = pcm_name;
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// 終了待ち
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH：BGM を再生（フルパラメータ）
// ================================================================
void C_elm_pcmch::play_bgm(TSTR bgm_name, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// 現在のサウンドを解放する
	m_player.release_sound();

	// ボリュームタイプが変わるならボリュームを再設定する
	tnm_update_sound_volume(false);

	// BGM 登録名から BGM 番号を取得
	int bgm_no = tnm_get_bgm_no_by_regist_name(bgm_name);
	if (bgm_no == -1)
		return;

	// プレイヤーで再生
	if (!m_player.play_bgm(bgm_no, loop_flag, fade_in_time, TNM_BGM_START_POS_INI, ready_only))
		return;

	// パラメータを設定
	m_pcm_name.clear();
	m_bgm_name = bgm_name;
	m_koe_no = -1;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// 終了待ち
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH：声を再生（フルパラメータ）
// ================================================================
void C_elm_pcmch::play_koe(int koe_no, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// 現在のサウンドを解放する
	m_player.release_sound();

	// ボリュームタイプが変わるならボリュームを再設定する
	tnm_update_sound_volume(false);

	// プレイヤーで再生
	if (!m_player.play_koe(koe_no, loop_flag, TNM_JITAN_RATE_NORMAL, 0, ready_only, false))
		return;

	// パラメータを設定
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = koe_no;
	m_se_no = -1;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// 終了待ち
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH：システム音を再生（フルパラメータ）
// ================================================================
void C_elm_pcmch::play_se(int se_no, bool loop_flag, bool wait_flag, int fade_in_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only)
{
	// 現在のサウンドを解放する
	m_player.release_sound();

	// ボリュームタイプが変わるならボリュームを再設定する
	tnm_update_sound_volume(false);

	// プレイヤーで再生
	if (!m_player.play_se(se_no, ready_only))
		return;

	// パラメータを設定
	m_pcm_name.clear();
	m_bgm_name.clear();
	m_koe_no = -1;
	m_se_no = se_no;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_delay_time = 0;
	m_fade_in_time = fade_in_time;
	m_loop_flag = loop_flag;
	m_bgm_fade_target_flag = bgm_fade_target_flag;
	m_bgm_fade2_target_flag = bgm_fade2_target_flag;
	m_bgm_fade_source_flag = bgm_fade_source_flag;
	m_ready_flag = ready_only;

	// 終了待ち
	if (wait_flag)
		wait(false, false);
}

// ****************************************************************
// PCMCH：停止
// ================================================================
void C_elm_pcmch::stop(int fade_out_time)
{
	m_player.stop(fade_out_time);

	// ループフラグだけは初期化する
	m_loop_flag = false;
}

// ****************************************************************
// PCMCH：一時停止
// ================================================================
void C_elm_pcmch::pause(int fade_out_time)
{
	m_player.pause(fade_out_time);
}

// ****************************************************************
// PCMCH：再開
// ================================================================
void C_elm_pcmch::resume(bool wait_flag, int fade_in_time, int delay_time)
{
	if (delay_time < 0)
		delay_time = 0;

	// 準備中フラグを降ろす
	m_ready_flag = false;

	// 遅延再開でなければ再開する
	if (delay_time == 0)	{
		m_player.resume(fade_in_time);

		// 終了待ち
		if (wait_flag)
			wait(false, false);
	}

	// パラメータを設定
	m_delay_time = delay_time;
	m_fade_in_time = fade_in_time;
}

// ****************************************************************
// PCMCH：再生終了待ち
// ================================================================
void C_elm_pcmch::wait(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCMCH_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCMCH：フェードアウト終了待ち
// ================================================================
void C_elm_pcmch::wait_fade(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCMCH_FADE_OUT_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// PCMCH：再生中かどうかを判定
// ================================================================
bool C_elm_pcmch::is_playing()
{
	return m_player.is_playing();
}

// ****************************************************************
// PCMCH：フェードアウト中かどうかを判定
// ================================================================
bool C_elm_pcmch::is_fade_out_doing()
{
	return m_player.is_fade_out();
}

// ****************************************************************
// PCMCH：ボリュームを設定／取得
// ================================================================
void C_elm_pcmch::set_volume(int volume, int fade_time)
{
	// プレイヤーのゲームボリュームを設定
	m_player.set_game_volume(volume, fade_time);

	// パラメータを設定
	m_volume = volume;
}

void C_elm_pcmch::set_volume_max(int fade_time)
{
	set_volume(TNM_VOLUME_MAX, fade_time);
}

void C_elm_pcmch::set_volume_min(int fade_time)
{
	set_volume(TNM_VOLUME_MIN, fade_time);
}

// ****************************************************************
// PCMCH：システムボリュームを設定
// ================================================================
void C_elm_pcmch::set_system_volume(int volume)
{
	m_player.set_system_volume(volume);
}

// ****************************************************************
// PCMCH：システムボリュームを取得
// ================================================================
int C_elm_pcmch::get_system_volume()
{
	return m_player.get_system_volume();
}

// ****************************************************************
// PCMCH リスト：サブの初期化
// ================================================================
void C_elm_pcmch_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), FM_PCMCH, get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// PCMCH リスト：サブの再初期化
// ================================================================
void C_elm_pcmch_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// PCMCH リスト：サブのセーブ
// ================================================================
void C_elm_pcmch_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// PCMCH リスト：サブのロード
// ================================================================
void C_elm_pcmch_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
