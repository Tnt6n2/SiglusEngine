#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"element/elm_sound.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_bgm_table.h"
#include	"utility/util_jitan_cnv.h"

// ****************************************************************
// ovk ヘッダ
// ================================================================
struct OVKPACKHEADER
{
	DWORD size;						// ファイルサイズ
	DWORD offset;					// オフセット
	int no;							// 番号
	long smp_cnt;					// サンプル数
};

// ****************************************************************
// プレイヤー：初期化
// ================================================================
void C_tnm_player::init()
{
	// プレイヤーを初期化
	m_player.init();

	// プレイヤーをマネージャに登録
	G_pm.regist_player(&m_player);
}

// ****************************************************************
// プレイヤー：解放
// ================================================================
void C_tnm_player::free()
{
	// プレイマネージャからプレイヤーを抹消
	G_pm.delete_player(&m_player);

	// プレイヤーを解放
	m_player.release();
}

// ****************************************************************
// プレイヤー：再初期化
// ================================================================
void C_tnm_player::reinit()
{
	// サウンドを解放する
	m_player.release_sound();

	// 情報を初期化
	m_sound_type = TNM_VOLUME_TYPE_NONE;
	m_bgm_no = -1;
	m_koe_no = -1;
	m_file_name.clear();
	m_se_no = -1;

	// 口パクボリューム
	m_mouth_volume_table.clear();
}

// ****************************************************************
// プレイヤー：wav を再生：private なので初期化不要
// ================================================================
bool C_tnm_player::__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop_flag, int fade_time, bool ready_only, bool check_only)
{
	return __play_wav(base_dir, sub_dir, file_name, loop_flag, 0, -1, 0, fade_time, ready_only, check_only);	// 最初から最後まで
}

bool C_tnm_player::__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop_flag, int start_pos, int end_pos, int repeat_pos, int fade_time, bool ready_only, bool check_only)
{
	// ファイル名が空だったら正常終了
	if (file_name.empty())
		return true;

	int sound_type = 0;	// 1: wav, 2: nwa, 3: ogg, 4: owp

	// ファイルを検索
	TSTR file_path = tnm_find_wav(base_dir, sub_dir, file_name, &sound_type);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("サウンドファイル ") + file_name + _T(" が見つかりませんでした。"));
		return false;
	}

	// チェックのみならここで正常終了
	if (check_only)
		return true;

	// ストリームを準備する
	switch (sound_type)	{
		case 1:
			{
				// wav ストリームを準備する
				BSP<C_wave_stream> wav_stream(new C_wave_stream);
				if (!wav_stream->open(file_path))	{
					tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("サウンドファイル ") + file_name + _T(" が開けませんでした。\n") + get_last_error_msg());
					return false;
				}

				// プレイヤーにストリームを設定
				if (!m_player.set_stream(wav_stream))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サウンドファイル ") + file_name + _T(" の準備中にエラーが発生しました。"));
					return false;
				}
			}
			break;
		case 2:
			{
				// nwa ストリームを準備する
				BSP<C_nwa_stream> nwa_stream(new C_nwa_stream);
				if (!nwa_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("サウンドファイル ") + file_name + _T(" が開けませんでした。"));

				// プレイヤーにストリームを設定
				if (!m_player.set_stream(nwa_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サウンドファイル ") + file_name + _T(" の準備中にエラーが発生しました。"));
			}
			break;
		case 3:
			{
				// ogg ストリームを準備する
				BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0));
				if (!ogg_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("サウンドファイル ") + file_name + _T(" が開けませんでした。"));

				// プレイヤーにストリームを設定
				if (!m_player.set_stream(ogg_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サウンドファイル ") + file_name + _T(" の準備中にエラーが発生しました。"));
			}
			break;
		case 4:
			{
				// owp ストリームを準備する
				BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0x39));
				if (!ogg_stream->open(file_path))
					return tnm_set_error(TNM_ERROR_TYPE_SOUND_NOT_FOUND, _T("サウンドファイル ") + file_name + _T(" が開けませんでした。"));

				// プレイヤーにストリームを設定
				if (!m_player.set_stream(ogg_stream))
					return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サウンドファイル ") + file_name + _T(" の準備中にエラーが発生しました。"));
			}
			break;
	}

	// パラメータを設定
	m_player.set_loop(loop_flag);
	m_player.set_range(start_pos, end_pos, repeat_pos);

	// 再生
	if (ready_only)	{
		m_player.ready();
	}
	else	{
		if (fade_time <= 0)		m_player.play();
		else					m_player.fade_in(fade_time);
	}

	return true;
}

// ****************************************************************
// プレイヤー：ＢＧＭを再生：public なので初期化が必要
// ================================================================
bool C_tnm_player::play_bgm(int bgm_no, bool loop_flag, int fade_in_time, int start_pos, bool ready_only)
{
	reinit();

	if (bgm_no < 0 || Gp_ini->bgm_cnt <= bgm_no)
		return false;

	// スタート位置が指定されていない場合は ini に従う
	if (start_pos == TNM_BGM_START_POS_INI)
		start_pos = Gp_ini->bgm[bgm_no].start_pos;

	// 再生
	if (!__play_wav(Gp_dir->base_dir, _T("bgm"), Gp_ini->bgm[bgm_no].file_name, loop_flag, start_pos, Gp_ini->bgm[bgm_no].end_pos, Gp_ini->bgm[bgm_no].repeat_pos, fade_in_time, ready_only, false))
		return false;

	// パラメータを設定
	m_sound_type = TNM_VOLUME_TYPE_BGM;
	m_bgm_no = bgm_no;

	// ＢＧＭテーブルに登録
	tnm_bgm_table_set_listened(bgm_no, true);

	return true;
}

#include <boost/algorithm/string/classification.hpp> // is_any_of
#include <boost/algorithm/string/split.hpp>

// ****************************************************************
// プレイヤー：声を再生：public なので初期化が必要
// ================================================================
//		時短再生レート：100%～400%
// ================================================================
bool C_tnm_player::play_koe(int koe_no, bool loop_flag, int jitan_rate, int play_pos_mills, bool ready_only, bool check_only)
{
	reinit();

	// リリースモードでチェックのみの場合はチェックしない
	if (!Gp_global->debug_flag && check_only)
		return true;

	// 声番号が不正だったら終了
	if (koe_no < 0)
		return true;

	// 声番号を分割する
	int scn_no = koe_no / 100000;
	int line_no = koe_no % 100000;

	// ファイルを検索
	int sound_type = 0;
	TSTR base_dir = Gp_dir->base_dir;
	TSTR file_path = tnm_find_koe(base_dir, _T("koe"), koe_no, &sound_type);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("声ファイル ") + tostr(koe_no) + _T(" が見つかりませんでした。"));
		return false;
	}

	// チェックのみならここで正常終了（ovk はもうちょっと終了しない）
	if (sound_type != 3)	{
		if (check_only)
			return true;
	}

	// オンメモリ展開用のサウンド
	BSP<C_sound> memory_sound(new C_sound);
	BSP<C_sound_stream> memory_stream(new C_sound_stream);

	if (false);
	else if (sound_type == 1)	{	// wav

		// wav ストリームを準備する
		BSP<C_wave_stream> wav_stream(new C_wave_stream);
		if (!wav_stream->open(file_path))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("声ファイル ") + tostr(koe_no) + _T(" が開けませんでした。"));

		// オンメモリにサウンドをロード
		if (!memory_sound->load_sound(wav_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("声ファイル ") + tostr(koe_no) + _T(" の展開に失敗しました。"));
	}
	else if (sound_type == 2)	{	// nwa

		// nwa ストリームを準備する
		BSP<C_nwa_stream> nwa_stream(new C_nwa_stream);
		if (!nwa_stream->open(file_path))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("声ファイル ") + tostr(koe_no) + _T(" が開けませんでした。"));

		// オンメモリにサウンドをロード
		if (!memory_sound->load_sound(nwa_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("声ファイル ") + tostr(koe_no) + _T(" の展開に失敗しました。"));
	}
	else if (sound_type == 3)	{	// ovk

		// ファイルを開く
		C_file file;
		if (!file.open(file_path, _T("rb")))
			return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("声ファイル ") + tostr(koe_no) + _T(" が開けませんでした。"));

		// オフセットを求める
		DWORD offset = -1;
		DWORD size = 0;
		DWORD koe_cnt = 0;
		file.read(&koe_cnt, 4);
		if (koe_cnt > 0)	{
			ARRAY<BYTE> read_buf;
			read_buf.resize(sizeof(OVKPACKHEADER) * koe_cnt);
			file.read(read_buf.get(), sizeof(OVKPACKHEADER) * koe_cnt);
			OVKPACKHEADER *oph = (OVKPACKHEADER *)read_buf.get();

			// 声番号を探す
			for (int k = 0; k < (int)koe_cnt; k++)	{
				if (oph->no == line_no)	{
					offset = oph->offset;
					size = oph->size;
					break;
				}
				oph ++;
			}
			if (offset == -1)	{
				return tnm_set_error(TNM_ERROR_TYPE_KOE_NOT_FOUND, _T("声ファイル ") + tostr(koe_no) + _T(" が見つかりませんでした。"));
			}
			read_buf.clear();
		}

		// チェックのみならここで正常終了
		if (check_only)
			return true;

		// ogg ストリームを準備する
		BSP<C_ogg_stream> ogg_stream(new C_ogg_stream(0));
		if (!ogg_stream->open(file_path, offset, size))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("声ファイル ") + tostr(koe_no) + _T(" が開けませんでした。"));

		// オンメモリにサウンドをロード
		if (!memory_sound->load_sound(ogg_stream.get()))
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("声ファイル ") + tostr(koe_no) + _T(" の展開に失敗しました。"));
	}

	// 時短再生をする場合
	if (jitan_rate != 100)	{

		// モノラルなら普通につくる
		if (memory_sound->get_channel_cnt() == 1)	{

			// 時短再生用のサウンドを作成
			BSP<C_sound> jitan_sound(new C_sound);
			jitan_sound->create(memory_sound->get_channel_cnt(), memory_sound->get_bit_cnt_per_sample(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_data_size());

			// 時短再生を行う
			C_jitan_cnv jitan_converter;
			jitan_converter.convert(jitan_sound->get_buffer(), memory_sound->get_ptr(), memory_sound->get_data_size(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_channel_cnt(), memory_sound->get_bit_cnt_per_sample(), jitan_rate);
			jitan_sound->update_data_size();

			// オンメモリからサウンドを読み込むストリームを作成
			memory_stream->set_sound(jitan_sound);
		}
		// ステレオなら時短再生に対応していないのでモノラル化してみる！
		else if (memory_sound->get_channel_cnt() == 2)	{

			// 16bit 前提です…

			// モノラル化用のバッファを作成（モノラル化なのでデータサイズは半分！）
			BSP<C_sound> mono_sound(new C_sound);
			mono_sound->create(1, memory_sound->get_bit_cnt_per_sample(), memory_sound->get_sample_cnt_per_sec(), memory_sound->get_data_size() / 2);

			// モノラル化してみる（実は足し算してるだけ）
			int sample_cnt = memory_sound->get_sample_cnt();
			short* p_src = (short *)memory_sound->get_ptr();
			short* p_dst = (short *)mono_sound->get_ptr();
			short* p_dst_end = p_dst + sample_cnt;
			int value;
			while (p_dst != p_dst_end)	{
				 value = *p_src++;		// 1ch目を代入
				 value += *p_src++;		// 2ch目を加算
				 *p_dst++ = limit<int>(-30000, value, 30000);	// 念のため30000で補正
			}

			// 時短再生用のサウンドを作成（モノラル化するのでデータサイズは半分！）
			BSP<C_sound> jitan_sound(new C_sound);
			jitan_sound->create(1, mono_sound->get_bit_cnt_per_sample(), mono_sound->get_sample_cnt_per_sec(), mono_sound->get_data_size());

			// 時短再生を行う
			C_jitan_cnv jitan_converter;
			jitan_converter.convert(jitan_sound->get_buffer(), mono_sound->get_ptr(), mono_sound->get_data_size(), mono_sound->get_sample_cnt_per_sec(), mono_sound->get_channel_cnt(), mono_sound->get_bit_cnt_per_sample(), jitan_rate);
			jitan_sound->update_data_size();

			// オンメモリからサウンドを読み込むストリームを作成
			memory_stream->set_sound(jitan_sound);
		}
	}
	// 時短再生をしない場合
	else	{

		// オンメモリからサウンドを読み込むストリームを作成
		memory_stream->set_sound(memory_sound);
	}

	// プレイヤーにオンメモリストリームを設定
	m_player.set_stream(memory_stream);

	// プレイヤーのパラメータを設定
	m_player.set_loop(loop_flag);
	m_player.set_range(0, -1, 0);
	m_player.set_play_pos_by_millsecond(play_pos_mills);

	// 口パクボリュームを解析する
	TSTR mouse_volume_file_path = tnm_find_dat(str_format(_T("z%09d.vol.csv"), koe_no));
	if (!mouse_volume_file_path.empty())
	{
		ARRAY<TSTR> mouse_volume_file_text;
		if (!C_file::read_full_text_UTF16(mouse_volume_file_path, mouse_volume_file_text))
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("口パクデータ ") + tostr(koe_no) + _T(" の展開に失敗しました。"));
		}
		if (mouse_volume_file_text.empty())
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("口パクデータ ") + tostr(koe_no) + _T(" が空です。"));
		}

		std::vector<std::string> mouse_volume_table_mbstr;
		boost::algorithm::split(mouse_volume_table_mbstr, TSTR_to_MBSTR(mouse_volume_file_text[0]), boost::is_any_of(","));
		for (int i = 0; i < (int)mouse_volume_table_mbstr.size(); i++)
		{
			float volume = std::stof(mouse_volume_table_mbstr[i]);
			m_mouth_volume_table.push_back(volume);
		}
	}

	// 再生
	if (ready_only)	{
		m_player.ready();
	}
	else	{
		m_player.play();
	}

	// パラメータを設定
	m_sound_type = TNM_SOUND_TYPE_KOE;
	m_koe_no = koe_no;

	return true;
}

float C_tnm_player::get_current_mouth_volume()
{
	if (m_player.is_playing())
	{
		int play_pos_mill = m_player.get_play_pos_by_millsecond();
		int play_pos_frame = play_pos_mill * 60 / 1000;
		if (0 <= play_pos_frame && play_pos_frame < (int)m_mouth_volume_table.size())
		{
			return m_mouth_volume_table[play_pos_frame];
		}
	}

	return 0.0f;
}

// ****************************************************************
// プレイヤー：効果音を再生：public なので初期化必要
// ================================================================
bool C_tnm_player::play_pcm(TSTR file_name, bool loop_flag, int fade_in_time, bool ready_only)
{
	reinit();

	// 再生
	if (!__play_wav(Gp_dir->base_dir, _T("wav"), file_name, loop_flag, fade_in_time, ready_only, false))
		return false;

	// パラメータを設定
	m_sound_type = TNM_SOUND_TYPE_PCM;
	m_file_name = file_name;

	return true;
}

// ****************************************************************
// プレイヤー：システム音を再生：public なので初期化必要
// ================================================================
bool C_tnm_player::play_se(int se_no, bool ready_only)
{
	reinit();

	if (se_no < 0 || Gp_ini->se_cnt <= se_no)
		return false;

	// SE 番号からファイルパスを求める
	TSTR file_name = Gp_ini->se[se_no].file_name;

	// 再生
	if (!__play_wav(Gp_dir->base_dir, _T("wav"), file_name, false, 0, ready_only, false))
		return false;

	// パラメータを設定
	m_sound_type = TNM_SOUND_TYPE_SE;
	m_se_no = se_no;

	return true;
}

// ****************************************************************
// プレイヤー：停止
// ================================================================
void C_tnm_player::stop(int fade_time)
{
	// 停止
	if (fade_time <= 0)		m_player.release_sound();					// サウンドごと解放する（次回再生時は set_stream から入るのでこれでＯＫ）
	else					m_player.fade_out_and_release(fade_time);	// フェードアウトが終わってもサウンドは残りっぱなしだが仕方ない
}

// ****************************************************************
// プレイヤー：一時停止
// ================================================================
void C_tnm_player::pause(int fade_time)
{
	if (fade_time <= 0)		m_player.pause();
	else					m_player.fade_out_and_pause(fade_time);
}

// ****************************************************************
// プレイヤー：再開
// ================================================================
void C_tnm_player::resume(int fade_time)
{
	if (fade_time <= 0)		m_player.play();
	else					m_player.fade_in(fade_time);
}

// ****************************************************************
// プレイヤー：サウンドを解放
// ================================================================
void C_tnm_player::release_sound()
{
	m_player.release_sound();
}

// ****************************************************************
// プレイヤー：ボリュームを設定
// ================================================================
void C_tnm_player::__set_volume(int volume_channel_no, int volume, int fade_time)
{
	// 範囲制限
	volume = limit(TNM_VOLUME_MIN, volume, TNM_VOLUME_MAX);

	// ボリュームを設定する
	m_player.set_volume(volume_channel_no, volume, fade_time);
}

// ****************************************************************
// プレイヤー：ボリュームを取得
// ================================================================
int C_tnm_player::__get_volume(int volume_channel_no)
{
	return m_player.get_volume(volume_channel_no);
}

// ****************************************************************
// プレイヤー：システムボリュームを設定
// ================================================================
void C_tnm_player::set_system_volume(int volume)
{
	// ボリュームを設定する
	__set_volume(TNM_PLAYER_VOLUME_CHANNEL_SYSTEM, volume, 0);
}

// ****************************************************************
// プレイヤー：システムボリュームを取得
// ================================================================
int C_tnm_player::get_system_volume()
{
	return __get_volume(TNM_PLAYER_VOLUME_CHANNEL_SYSTEM);
}

// ****************************************************************
// プレイヤー：ゲームボリュームを設定
// ================================================================
void C_tnm_player::set_game_volume(int volume, int fade_time)
{
	// ボリュームを設定する
	__set_volume(TNM_PLAYER_VOLUME_CHANNEL_GAME, volume, fade_time);
}

// ****************************************************************
// プレイヤー：ゲームボリュームを取得
// ================================================================
int C_tnm_player::get_game_volume()
{
	return __get_volume(TNM_PLAYER_VOLUME_CHANNEL_GAME);
}

// ****************************************************************
// プレイヤー：プレイ位置を取得
// ================================================================
int C_tnm_player::get_play_pos()
{
	return m_player.get_play_pos_by_sample();
}
int C_tnm_player::get_play_pos_by_millsecond()
{
	return m_player.get_play_pos_by_millsecond();
}

// ****************************************************************
// プレイヤー：プレイ位置を設定
// ================================================================
void C_tnm_player::set_play_pos_by_sample(int sample)
{
	m_player.set_play_pos_by_sample(sample);
}
void C_tnm_player::set_play_pos_by_millsecond(int mills)
{
	m_player.set_play_pos_by_millsecond(mills);
}

// ****************************************************************
// プレイヤー：プレイサイズを取得
// ================================================================
int C_tnm_player::get_length()
{
	return m_player.get_sample_cnt();
}
int C_tnm_player::get_length_by_millsecond()
{
	return m_player.get_length_by_millsecond();
}
