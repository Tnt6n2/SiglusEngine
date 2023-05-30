#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_gan.h"
#include	"data/tnm_gan_manager.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// ▲▲▲ＧＡＮ関連
// ▲命令を作らないとね。
// ▲新型のＧＡＮを作りたい。（ツールも作らないと…）

// ****************************************************************
// コンストラクタ
// ================================================================
C_tnm_gan::C_tnm_gan()
{
	init();
}

// ****************************************************************
// デストラクタ
// ================================================================
C_tnm_gan::~C_tnm_gan()
{
}

// ****************************************************************
// 初期化
// ================================================================
void C_tnm_gan::init()
{
	work.p_gan_data.reset();
	work.p_pat_data = NULL;

	work.gan_name.clear();
	work.now_time = 0;
	work.anm_set_no = 0;
	work.next_anm_set_no = 0;

	work.anm_start = false;
	work.anm_pause = false;
	work.anm_loop_flag = false;
	work.anm_real_time_flag = false;

	work.next_anm_flag = false;
	work.next_anm_loop_flag = false;
	work.next_anm_real_time_flag = false;
}

// ****************************************************************
// 解放
// ================================================================
void C_tnm_gan::free()
{
	init();
}

// ****************************************************************
// セーブ
// ================================================================
void C_tnm_gan::save(C_tnm_save_stream& stream)
{
	stream.save(work.gan_name);
	stream.save(work.now_time);
	stream.save(work.anm_set_no);
	stream.save(work.next_anm_set_no);

	stream.save(work.anm_start);
	stream.save(work.anm_pause);
	stream.save(work.anm_loop_flag);
	stream.save(work.anm_real_time_flag);
	stream.save(work.next_anm_flag);
	stream.save(work.next_anm_loop_flag);
	stream.save(work.next_anm_real_time_flag);
}

// ****************************************************************
// ロード
// ================================================================
void C_tnm_gan::load(C_tnm_save_stream& stream)
{
	stream.load(work.gan_name);
	stream.load(work.now_time);
	stream.load(work.anm_set_no);
	stream.load(work.next_anm_set_no);

	stream.load(work.anm_start);
	stream.load(work.anm_pause);
	stream.load(work.anm_loop_flag);
	stream.load(work.anm_real_time_flag);
	stream.load(work.next_anm_flag);
	stream.load(work.next_anm_loop_flag);
	stream.load(work.next_anm_real_time_flag);

	// 再構築
	load_gan_only(work.gan_name);
}

// ****************************************************************
// コピー
// ================================================================
void C_tnm_gan::copy(const C_tnm_gan* src)
{
	work.p_gan_data = src->work.p_gan_data;
	work.p_pat_data = src->work.p_pat_data;

	work.gan_name = src->work.gan_name;
	work.now_time = src->work.now_time;
	work.anm_set_no = src->work.anm_set_no;
	work.next_anm_set_no = src->work.next_anm_set_no;

	work.anm_start = src->work.anm_start;
	work.anm_pause = src->work.anm_pause;
	work.anm_loop_flag = src->work.anm_loop_flag;
	work.anm_real_time_flag = src->work.anm_real_time_flag;
	work.next_anm_flag = src->work.next_anm_flag;
	work.next_anm_loop_flag = src->work.next_anm_loop_flag;
	work.next_anm_real_time_flag = src->work.next_anm_real_time_flag;
}

// ****************************************************************
// 読み込み
// ================================================================
bool C_tnm_gan::load_gan(CTSTR& file_name)
{
	free();								// 解放
	return load_gan_only(file_name);	// 読み込みのみ
}

// ****************************************************************
// 読み込みのみ（ファイルのみの変更です。アニメの状態が初期化されません）
// ================================================================
bool C_tnm_gan::load_gan_only(CTSTR& gan_name)
{
	if (gan_name.empty())
		return true;

	work.gan_name = gan_name;

	TSTR gan_path = tnm_find_gan(gan_name);

	work.p_gan_data = G_gan_manager.load(gan_path);
	if (!work.p_gan_data.get())	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, gan_name + _T(".gan が見つかりませんでした。"));
		return false;
	}

	return true;
}

// ****************************************************************
// アニメ開始
// ================================================================
void C_tnm_gan::start_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag)
{
	work.now_time = 0;
	work.anm_start = true;
	work.anm_pause = false;
	work.anm_set_no = anm_set_no;
	work.anm_loop_flag = anm_loop_flag;
	work.anm_real_time_flag = anm_real_time_flag;
	work.next_anm_flag = false;
}

// ****************************************************************
// 次のアニメを設定（現在のアニメはワンショットになります）
// ================================================================
void C_tnm_gan::next_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag)
{
	// 現在アニメ中
	if (work.anm_start)	{
		if (work.next_anm_flag)	{	// 次のアニメが既に準備されている場合は、それをワンショットで開始する
			start_anm(work.next_anm_set_no, false, work.next_anm_real_time_flag);
		}
		else	{
			work.anm_loop_flag = false;					// 現在のアニメはワンショットになります
		}
		work.next_anm_flag = true;
		work.next_anm_set_no = anm_set_no;
		work.next_anm_loop_flag = anm_loop_flag;
		work.next_anm_real_time_flag = anm_real_time_flag;
	}
	// アニメしていない場合は即実行
	else	{
		start_anm(anm_set_no, anm_loop_flag, anm_real_time_flag);
	}
}

// ****************************************************************
// アニメのセット番号を変更
// ================================================================
void C_tnm_gan::change_anm_set_no(int anm_set_no)
{
	if (work.next_anm_flag)	{	// 次のアニメが既に準備されている場合は、そちらを設定する
		work.next_anm_set_no = anm_set_no;
	}
	else	{
		work.anm_set_no = anm_set_no;
	}
}

// ****************************************************************
// アニメのワンショット／ループの変更
// ================================================================
void C_tnm_gan::change_anm_loop(bool anm_loop_flag)
{
	if (work.next_anm_flag)	{	// 次のアニメが既に準備されている場合は、そちらを設定する
		work.next_anm_loop_flag = anm_loop_flag;
	}
	else	{
		work.anm_loop_flag = anm_loop_flag;
	}
}

// ****************************************************************
// アニメポーズ
// ================================================================
void C_tnm_gan::pause_anm()
{
	work.anm_pause = true;
}

// ****************************************************************
// アニメポーズ解除
// ================================================================
void C_tnm_gan::resume_anm()
{
	work.anm_pause = false;
}

// ****************************************************************
// 時間を進める
// ================================================================
void C_tnm_gan::update_time(int past_game_time, int past_real_time)
{
	// 時間がマイナスの場合は補正（next への切り替わりで、再帰的に呼ばれる場合に起こり得る）
	if (past_game_time < 0)	{
		past_game_time = 0;
	}
	if (past_real_time < 0)	{
		past_real_time = 0;
	}

	// ＧＡＮデータが無い
	if (!work.p_gan_data.get())	{
		work.p_pat_data = NULL;
		return;
	}

	// セット番号がおかしい
	if (!(0 <= work.anm_set_no && work.anm_set_no < (int)work.p_gan_data->set_list.size()))	{
		work.p_pat_data = NULL;
		return;
	}

	// セットへのイテレータ
	ARRAY<S_gan_set_data>::iterator set_itr = work.p_gan_data->set_list.begin() + work.anm_set_no;

	// パターンが無い
	if (set_itr->pat_list.empty())	{
		work.p_pat_data = NULL;
		return;
	}

	// アニメが開始されていない（０番パターンを設定）
	if (!work.anm_start)	{
		work.p_pat_data = &*(set_itr->pat_list.begin());
		return;
	}

	// アニメ時間が０（０番パターンを設定）
	if (set_itr->total_time <= 0)	{
		work.p_pat_data = &*(set_itr->pat_list.begin());
		return;
	}

	// 時間を進める
	if (!work.anm_pause)	{
		if (work.anm_real_time_flag)	{
			work.now_time += past_real_time;
		}
		else	{
			work.now_time += past_game_time;
		}
	}

	// ワンショットの場合
	if (!work.anm_loop_flag)	{
		if (work.now_time >= set_itr->total_time)	{
			if (work.next_anm_flag)	{
				// ワンショット次へ（この中で work.anm_real_time_flag が変更されていることに注意！）
				start_anm(work.next_anm_set_no, work.next_anm_loop_flag, work.next_anm_real_time_flag);
				// 時間を進める
				update_time(past_game_time - (work.now_time - set_itr->total_time), past_real_time - (work.now_time - set_itr->total_time));
			}
			else	{
				// ワンショット待ち（最後のパターンを表示）
				work.now_time = set_itr->total_time;
				work.p_pat_data = &*(set_itr->pat_list.end() - 1);
			}
			return;
		}
	}

	// 時間を補正
	work.now_time %= set_itr->total_time;

	// アニメ処理
	for (ARRAY<S_gan_pat_data>::iterator pat_itr = set_itr->pat_list.begin(); pat_itr != set_itr->pat_list.end(); ++pat_itr)	{
		if (pat_itr->keika_time >= work.now_time)	{
			work.p_pat_data = &*pat_itr;
			break;
		}
	}
}



