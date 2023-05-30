#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_timer.h"
#include	"element/elm_sound.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"

#include	"engine/ifc_sound.h"

// ****************************************************************
// サウンド：インターフェース：BGM の個数を取得
// ================================================================
int tnm_get_bgm_cnt()
{
	return Gp_ini->bgm_cnt;
}

// ****************************************************************
// サウンド：インターフェース：BGM 番号から BGM 登録名を取得
// ================================================================
TSTR tnm_get_regist_name_by_bgm_no(int bgm_no)
{
	if (bgm_no < 0 || Gp_ini->bgm_cnt <= bgm_no)
		return _T("");

	return Gp_ini->bgm[bgm_no].regist_name;
}

// ****************************************************************
// サウンド：インターフェース：BGM 登録名から BGM 番号を取得
// ================================================================
int tnm_get_bgm_no_by_regist_name(CTSTR& bgm_regist_name)
{
	int bgm_no = -1;

	// Gameexe.ini から登録名を探す
	for (int i = 0; i < Gp_ini->bgm_cnt; i++)	{
		if (str_to_lower_ret(bgm_regist_name) == str_to_lower_ret(Gp_ini->bgm[i].regist_name))	{
			bgm_no = i;
			break;
		}
	}
	if (bgm_no == -1)	{
		TSTR error_str;
		error_str += _T("BGM の登録 ") + bgm_regist_name + _T(" が見つかりません。");
		error_str += _T("\nBGM を再生するには、Gameexe.ini で BGM を登録する必要があります。");
		error_str += _T("\n\n≪よくある間違い≫");
		error_str += _T("\nBGM の登録名とファイル名が逆になっていませんか？");
		tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, error_str);
		return -1;
	}

	return bgm_no;
}

// ****************************************************************
// サウンド：インターフェース：BGM がなっているかを判定
// ================================================================
bool tnm_check_bgm_playing()
{
	int state = Gp_sound->m_bgm.check();

	if (state == TNM_PLAYER_STATE_PLAY)
	{
		return true;
	}

	return false;
}

// ****************************************************************
// サウンド：インターフェース：声を再生する
// ================================================================
void tnm_play_koe(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only_flag)
{
	Gp_sound->m_koe.play(koe_no, chara_no, jitan_rate, play_pos_mills, wait_flag, key_skip_flag, return_value_flag, ex_koe_flag, check_only_flag);
}

// ****************************************************************
// サウンド：インターフェース：声を停止する
// ================================================================
void tnm_stop_koe()
{
	Gp_sound->m_koe.stop();
}

// ****************************************************************
// サウンド：インターフェース：SE を再生する
// ================================================================
void tnm_play_se(int se_no)
{
	Gp_sound->m_se.play_se(se_no, NULL);
}

// ****************************************************************
// サウンド：インターフェース：ムービーが準備中かを判定
// ================================================================
bool tnm_is_movie_readying()
{
	return Gp_sound->m_mov.is_readying();
}

// ****************************************************************
// サウンド：インターフェース：ムービーが再生中かを判定
// ================================================================
bool tnm_is_movie_playing()
{
	return Gp_sound->m_mov.is_playing();
}

// ****************************************************************
// サウンド：インターフェース：omv を先読みする（ディスクキャッシュに乗せて高速化を計る！）
// ================================================================
bool tnm_preload_omv(CTSTR& file_name)
{
	TSTR file_path = tnm_find_omv(file_name);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(".omv が見つかりませんでした。"));
		return false;
	}

	C_file file;
	if (!file.open(file_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(".omv が開けませんでした。\n") + get_last_error_msg());
		return false;
	}

	const int buf_size = 1024 * 1024;	// 1MB
	BUFFER buf(buf_size);
	file.read(buf.get(), buf_size);	// 空読みしてみる
	file.close();

	return true;
}

// ****************************************************************
// サウンド：インターフェース：サウンドのボリュームを更新する
// ================================================================

// static：ボリューム
int G_all_total_volume = 255;
int G_bgm_total_volume = 255;
int G_koe_total_volume = 255;
int G_exkoe_total_volume = 255;
int G_pcm_total_volume = 255;
int G_se_total_volume = 255;
int G_mov_total_volume = 255;

// static：ＢＧＭフェードを使う？
static bool tnm_is_bgmfade_use(int koe_buf_total_volume, bool is_pcmch_bgm_fade_use)
{
	if (!Gp_config->bgmfade_use_check)	{
		return false;	// 環境設定でオフなら使わない
	}
	else if (Gp_local->pod.bgmfade_flag)	{
		return true;	// ローカルのＢＧＭフェードフラグが立っていれば使う
	}
	else if (koe_buf_total_volume > 0 && Gp_sound->m_koe.is_playing())	{
		return true;	// 声のボリュームが０以上で声が再生中なら使う
	}
	else if (is_pcmch_bgm_fade_use)	{
		return true;	// pcmch で使うと判定された場合は使う
	}
	else	{
		return false;	// それ以外なら使わない
	}
}

// static：ＢＧＭフェード２を使う？
static bool tnm_is_bgmfade2_use(int koe_buf_total_volume, bool is_pcmch_bgm_fade_use)
{
	if (koe_buf_total_volume > 0 && Gp_sound->m_koe.is_playing())	{
		return true;	// 声のボリュームが０以上で声が再生中なら使う
	}
	else if (is_pcmch_bgm_fade_use)	{
		return true;	// pcmch で使うと判定された場合は使う
	}
	else	{
		return false;	// それ以外なら使わない
	}
}

// static：pcmch のボリュームを更新
static void tnm_update_sound_volume_pcmch(int step, bool change_force, bool* p_is_bgm_fade_use)
{
	// step 0: bgmfade のソースとなる pcmch
	// step 1: bgmfade のソースとならない pcmch（通常）

	for (int i = 0; i < (int)Gp_sound->m_pcmch_list.get_size(); i++)	{
		if ((step == 0 && Gp_sound->m_pcmch_list[i].is_bgm_fade_source()) || (step == 1 && !Gp_sound->m_pcmch_list[i].is_bgm_fade_source()))	{

			int new_volume = 255;
			int volume_type = Gp_sound->m_pcmch_list[i].get_volume_type();
			switch (volume_type)	{
				case TNM_VOLUME_TYPE_NONE:		new_volume = G_all_total_volume;	break;
				case TNM_VOLUME_TYPE_BGM:		new_volume = G_bgm_total_volume;	break;
				case TNM_VOLUME_TYPE_KOE:		new_volume = G_koe_total_volume;	break;
				case TNM_VOLUME_TYPE_PCM:		new_volume = G_pcm_total_volume;	break;
				case TNM_VOLUME_TYPE_SE:		new_volume = G_se_total_volume;		break;
				case TNM_VOLUME_TYPE_MOV:		new_volume = G_mov_total_volume;	break;
				default:
					// 汎用ボリューム
					if (16 <= volume_type && volume_type < TNM_VOLUME_TYPE_MAX)	{
						if (Gp_config->play_sound_check[volume_type])	{
							new_volume = G_all_total_volume * Gp_config->sound_user_volume[volume_type] / 255;
						}
						else	{
							new_volume = 0;
						}
					}
					break;
			}

			// ＢＧＭフェード
			if (Gp_sound->m_pcmch_list[i].is_bgm_fade_target())	{
				new_volume = new_volume * Gp_global->bgmfade_total_volume / 255;
			}
			if (Gp_sound->m_pcmch_list[i].is_bgm_fade2_target())	{
				new_volume = new_volume * Gp_global->bgmfade2_total_volume / 255;
			}

			// キャラクタ音声
			int pcmch_chara_no = Gp_sound->m_pcmch_list[i].get_chara_no();
			new_volume = new_volume * tnm_get_chrkoe_volume(pcmch_chara_no) / 255;	// ボリューム
			new_volume = tnm_get_chrkoe_onoff(pcmch_chara_no) ? new_volume : 0;		// オンオフ

			// ボリュームを変更する
			if (change_force || new_volume != Gp_sound->m_pcmch_list[i].get_system_volume())	{
				Gp_sound->m_pcmch_list[i].set_system_volume(new_volume);
			}

			// ＢＧＭフェードを使うかを判定
			if (p_is_bgm_fade_use && step == 0 && new_volume > 0 && Gp_sound->m_pcmch_list[i].is_playing())	{
				*p_is_bgm_fade_use = true;
			}
		}
	}
}


// ボリュームを更新：本体
bool tnm_update_sound_volume(bool change_force)
{
	bool is_pcmch_bgm_fade_use = false;

	// 声の特殊処理です。
	// 声の再生にチェックがついていない場合、koe は止めます。exkoe は通常通り再生します。
	// 全体の再生にチェックがついていない場合、koe は止めます。exkoe はボリューム０をにします。
	// 声をループさせるならこの仕様ダメだな…

	// 今回の声をフレーム初期化
	G_all_total_volume = 255;
	G_bgm_total_volume = 255;
	G_koe_total_volume = 255;
	G_exkoe_total_volume = 255;
	G_pcm_total_volume = 255;
	G_se_total_volume = 255;
	G_mov_total_volume = 255;

	// 通常の声の場合、声を再生しない場合は声を止める
	if (!Gp_sound->m_koe.get_ex_koe_flag())	{
		int koe_chara_no = Gp_sound->m_koe.get_chara_no();
		bool koe_play_flag = tnm_is_koe_play(koe_chara_no);

		// 声を再生しないなら声を止める
		if (!koe_play_flag && Gp_sound->m_koe.is_playing())
			Gp_sound->m_koe.stop();
	}

	// 全体ボリュームを計算
	G_all_total_volume = G_all_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;	// 全体ボリュームによる調整

	// koe のボリュームを計算

	// 通常の声の場合は声オフに影響される
	// ここで計算されるのは、pcmch の「声チャンネルの音量で再生」で使われる音量
	G_koe_total_volume = G_koe_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;										// 全体ボリュームによる調整
	G_koe_total_volume = G_koe_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] : 0) / 255;		// 声ボリュームによる調整

	// ＥＸ声の場合は声オフを無視する
	// ここで計算されるのは、pcmch の「ＥＸ声チャンネルの音量で再生」で使われる音量
	G_exkoe_total_volume = G_exkoe_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// 全体ボリュームによる調整
	G_exkoe_total_volume = G_exkoe_total_volume * Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] / 255;															// 声ボリュームによる調整

	// 実際の声バッファは声かＥＸ声かによって変わる
	int koe_buf_total_volume = 255;
	if (Gp_sound->m_koe.get_ex_koe_flag())	{
		koe_buf_total_volume = G_exkoe_total_volume;
	}
	else	{
		koe_buf_total_volume = G_koe_total_volume;
	}

	// 声バッファにキャラクタ音声を適用する
	int koe_chara_no = Gp_sound->m_koe.get_chara_no();
	koe_buf_total_volume = koe_buf_total_volume * tnm_get_chrkoe_volume(koe_chara_no) / 255;	// キャラクタ音声によるボリューム調整

	// ＢＧＭ
	G_bgm_total_volume = G_bgm_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// 全体ボリュームによる調整
	G_bgm_total_volume = G_bgm_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] : 0) / 255;	// ＢＧＭボリュームによる調整

	// 効果音
	G_pcm_total_volume = G_pcm_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// 全体ボリュームによる調整
	G_pcm_total_volume = G_pcm_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] : 0) / 255;	// ＰＣＭボリュームによる調整

	// システム音
	G_se_total_volume = G_se_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;										// 全体ボリュームによる調整
	G_se_total_volume = G_se_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] : 0) / 255;		// ＳＥボリュームによる調整

	// ムービー
	G_mov_total_volume = G_mov_total_volume * (Gp_config->play_all_sound_check ? Gp_config->all_sound_user_volume : 0) / 255;									// 全体ボリュームによる調整
	G_mov_total_volume = G_mov_total_volume * (Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] ? Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] : 0) / 255;	// ＭＯＶボリュームによる調整

	// pcmch がＢＧＭフェードを使うかどうかを判定し、同時にボリュームを更新する
	tnm_update_sound_volume_pcmch(0, change_force, &is_pcmch_bgm_fade_use);

	// 声がＢＧＭフェードを使うかどうかを判定する
	// koe_buf（実際に流れてる音量）を使うことに注意
	bool is_bgm_fade_use = tnm_is_bgmfade_use(koe_buf_total_volume, is_pcmch_bgm_fade_use);
	bool is_bgm_fade2_use = tnm_is_bgmfade2_use(koe_buf_total_volume, is_pcmch_bgm_fade_use);
	
	// ＢＧＭフェードの値を計算

	// ＢＧＭフェードの時間を進める
	Gp_global->bgmfade_cur_time += Gp_timer->real_time_past;
	Gp_global->bgmfade2_cur_time += Gp_timer->real_time_past;

	// ＢＧＭフェード開始処理（声の合計ボリュームが０のときは処理しない）

	if (!Gp_global->bgmfade_flag)	{
		if (is_bgm_fade_use)	{

			// ＢＧＭフェード開始処理開始！
			Gp_global->bgmfade_cur_time = 0;
			Gp_global->bgmfade_start_value = Gp_global->bgmfade_total_volume;	// 現在のＢＧＭフェードのボリュームから開始
			Gp_global->bgmfade_flag = true;
		}
	}
	if (Gp_global->bgmfade_flag)	{
		if (!is_bgm_fade_use)	{

			// ＢＧＭフェード終了処理開始！
			Gp_global->bgmfade_cur_time = 0;
			Gp_global->bgmfade_start_value = Gp_global->bgmfade_total_volume;	// 現在のＢＧＭフェードのボリュームから開始
			Gp_global->bgmfade_flag = false;
		}
	}
	if (!Gp_global->bgmfade2_flag)	{
		if (is_bgm_fade2_use)	{

			// ＢＧＭフェード２開始処理開始！
			Gp_global->bgmfade2_cur_time = 0;
			Gp_global->bgmfade2_start_value = Gp_global->bgmfade2_total_volume;	// 現在のＢＧＭフェードのボリュームから開始
			Gp_global->bgmfade2_flag = true;
		}
	}
	if (Gp_global->bgmfade2_flag)	{
		if (!is_bgm_fade2_use)	{

			// ＢＧＭフェード２終了処理開始！
			Gp_global->bgmfade2_cur_time = 0;
			Gp_global->bgmfade2_start_value = Gp_global->bgmfade2_total_volume;	// 現在のＢＧＭフェードのボリュームから開始
			Gp_global->bgmfade2_flag = false;
		}
	}

	// ＢＧＭフェードの値を計算
	Gp_global->bgmfade_total_volume = 255;
	Gp_global->bgmfade2_total_volume = 255;
	if (Gp_global->bgmfade_flag)	{
		// 0～1000 ﾐﾘ秒の間に音量を bgmfade 値まで持っていく
		Gp_global->bgmfade_total_volume = linear_limit(Gp_global->bgmfade_cur_time, 0, Gp_global->bgmfade_start_value, 1000, Gp_config->bgmfade_volume);
	}
	else	{
		// 2000～3000 ﾐﾘ秒の間に音量を 255 まで持っていく
		Gp_global->bgmfade_total_volume = linear_limit(Gp_global->bgmfade_cur_time, 2000, Gp_global->bgmfade_start_value, 3000, 255);
	}
	if (Gp_global->bgmfade2_flag)	{
		// 0～500 ﾐﾘ秒の間に音量を 0 まで持っていく
		Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, Gp_ini->bgmfade2_in_start_time, Gp_global->bgmfade2_start_value, Gp_ini->bgmfade2_in_start_time + Gp_ini->bgmfade2_in_time_len, Gp_ini->bgmfade2_volume);
	}
	else	{
		if (Gp_global->bgmfade2_need_flag)	{
			// 0～500 ﾐﾘ秒の間に音量を 255 まで持っていく
			Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, Gp_ini->bgmfade2_out_start_time, Gp_global->bgmfade2_start_value, Gp_ini->bgmfade2_out_start_time + Gp_ini->bgmfade2_out_time_len, 255);
		}
		else	{	// ★初恋１／１対策、声のない文章の場合はすぐに復帰（でも500msは待とう。声と声の境で一瞬ＢＧＶがなるのを防ぐため）
			// 0～500 ﾐﾘ秒の間に音量を 255 まで持っていく
			Gp_global->bgmfade2_total_volume = linear_limit(Gp_global->bgmfade2_cur_time, 500, Gp_global->bgmfade2_start_value, 500 + Gp_ini->bgmfade2_out_time_len, 255);
		}
	}

	// ＢＧＭフェードを適用
	G_bgm_total_volume = G_bgm_total_volume * Gp_global->bgmfade_total_volume / 255;

	// 各バッファのボリュームを求める
	int bgm_buf_total_volume = G_bgm_total_volume;
	int pcm_buf_total_volume = G_pcm_total_volume;
	int se_buf_total_volume = G_se_total_volume;
	int mov_buf_total_volume = G_mov_total_volume;

	// ボリュームを更新する
	if (change_force || bgm_buf_total_volume != Gp_global->bgm_buf_total_volume)	{
		Gp_sound->m_bgm.set_system_volume(bgm_buf_total_volume);
	}
	if (change_force || koe_buf_total_volume != Gp_global->koe_buf_total_volume)	{
		Gp_sound->m_koe.set_system_volume(koe_buf_total_volume);
	}
	if (change_force || pcm_buf_total_volume != Gp_global->pcm_buf_total_volume)	{
		Gp_sound->m_pcm.set_system_volume(pcm_buf_total_volume);
	}
	if (change_force || se_buf_total_volume != Gp_global->se_buf_total_volume)	{
		Gp_sound->m_se.set_system_volume(se_buf_total_volume);
	}
	if (change_force || mov_buf_total_volume != Gp_global->mov_buf_total_volume)	{
		Gp_sound->m_mov.set_volume(mov_buf_total_volume);
	}

	// 効果音チャンネルもボリュームを更新する
	tnm_update_sound_volume_pcmch(1, change_force, NULL);

	// 値を保存する
	Gp_global->all_total_volume = G_all_total_volume;
	Gp_global->bgm_buf_total_volume = bgm_buf_total_volume;
	Gp_global->koe_buf_total_volume = koe_buf_total_volume;
	Gp_global->pcm_buf_total_volume = pcm_buf_total_volume;
	Gp_global->se_buf_total_volume = se_buf_total_volume;
	Gp_global->mov_buf_total_volume = mov_buf_total_volume;

	// 専用チャンネルで無音を鳴らし続ける
	{
		bool play_silent_sound = Gp_system_config->ds_play_silent_sound_flag;

		// 開始
		if (play_silent_sound && !Gp_global->play_silent_sound)	{
			Gp_sound->m_muon_sound = BSP<C_sound>(new C_sound);
			Gp_sound->m_muon_sound->create(1, 16, 44100, 1024);
			Gp_sound->m_muon_stream = BSP<C_sound_stream>(new C_sound_stream);
			Gp_sound->m_muon_stream->set_sound(Gp_sound->m_muon_sound);
			Gp_sound->m_muon_player = BSP<C_player>(new C_player);
			Gp_sound->m_muon_player->set_stream(Gp_sound->m_muon_stream);
			Gp_sound->m_muon_player->play();
		}
		// 停止
		else if (!play_silent_sound && Gp_global->play_silent_sound)	{
			if (Gp_sound->m_muon_player)
				Gp_sound->m_muon_player.reset();
			if (Gp_sound->m_muon_stream)
				Gp_sound->m_muon_stream.reset();
			if (Gp_sound->m_muon_sound)
				Gp_sound->m_muon_sound.reset();
		}

		// 値を保存する
		Gp_global->play_silent_sound = play_silent_sound;
	}

	return true;
}

