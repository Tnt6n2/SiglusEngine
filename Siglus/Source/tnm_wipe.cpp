#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_wipe.h"
#include	"element/elm_excall.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// ワイプ：初期化
// ================================================================
void C_tnm_wipe::init()
{
	m_type = -1;
	m_speed_mode = 0;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = 0;

	m_start_time = 0;
	m_end_time = 0;
	m_begin_sorter = S_tnm_sorter(0, 0);
	m_end_sorter = S_tnm_sorter(0, 0);
	m_wipe_range = TNM_WIPE_RANGE_NORMAL;
	m_mask_file.clear();
	m_wipe_step = -1;
	m_wipe_prg = 0;
}

// ****************************************************************
// ワイプ：システムコンフィグによってワイプタイプを置き換える
// ================================================================
void tnm_change_wipe_type(int* p_wipe_type, int* option)
{
	// システムコンフィグによって、ワイプの種類を制限する
	if (Gp_global->wipe_buffer_cnt < 2)	{

		// クロスラスタ（220）
		if (*p_wipe_type == 220)	{
			*p_wipe_type = 221;		// ラスタ（221）に置き換える
			option[4] = 0;			// 次絵をフェードインさせる
		}
		// クロス爆発ブラー（240）／爆発ブラー（241）
		else if (*p_wipe_type == 240 || *p_wipe_type == 241)	{
			int center_x = option[0];
			int center_y = option[1];

			*p_wipe_type = 214;		// 元絵をフェードＯＵＴしながら拡大表示（214）に置き換える
			option[0] = 11;			// 指定座標から
			option[1] = 0;			// 未使用
			option[2] = center_x;	// Ｘ座標
			option[3] = center_y;	// Ｙ座標
		}
		// クロスクエイクブラー（242）／クエイクブラー（243）
		else if (*p_wipe_type == 242 || *p_wipe_type == 243)	{

			*p_wipe_type = 214;		// 元絵をフェードＯＵＴしながら拡大表示（214）に置き換える
			option[0] = 0;			// 中心から
			option[1] = 0;			// 未使用
		}
	}
}

// ****************************************************************
// ワイプ開始
// ================================================================
//		key_skip: 0=キーで飛ばせない、1=キーで飛ばせる、-1=システム設定に従う
// ================================================================
void C_tnm_wipe::start(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter
					   , int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int speed_mode, int* option
					   , bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// 現在のワイプを終了させる
	end();

	// システムコンフィグによって、ワイプの種類を制限する
	tnm_change_wipe_type(&wipe_type, option);

	// ワイプパラメータを設定
	m_type = wipe_type;
	m_speed_mode = speed_mode;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = option[i];
	m_cur_time = 0;
	m_start_time = start_time;
	m_end_time = wipe_time;
	m_begin_sorter = begin_sorter;
	m_end_sorter = end_sorter;
	m_wipe_range = wipe_range;
	m_with_low_order = with_low_order;

	// 共通処理
	start_sub(wait_flag, key_skip_mode, return_value_flag, wipe_flag);
}

// ****************************************************************
// マスクワイプ開始
// ================================================================
//		key_skip: 0=キーで飛ばせない、1=キーで飛ばせる、-1=システム設定に従う
// ================================================================
void C_tnm_wipe::start_mask(CTSTR& mask_file, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter
							, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int speed_mode, int* option
							, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// 現在のワイプを終了させる
	end();

	// システムコンフィグによって、ワイプの種類を制限する
	tnm_change_wipe_type(&wipe_type, option);

	// ワイプ開始
	m_type = wipe_type;
	m_speed_mode = speed_mode;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = option[i];
	m_cur_time = 0;
	m_start_time = start_time;
	m_end_time = wipe_time;
	m_begin_sorter = begin_sorter;
	m_end_sorter = end_sorter;
	m_wipe_range = wipe_range;
	m_with_low_order = with_low_order;
	m_mask_file = mask_file;

	// 共通処理
	start_sub(wait_flag, key_skip_mode, return_value_flag, wipe_flag);
}

// ****************************************************************
// ワイプ開始（共通処理）
// ================================================================
void C_tnm_wipe::start_sub(bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// ワイプを行う
	if (wipe_flag)	{

		// ＥＸコールワイプの場合
		if (m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

			if (Gp_excall->is_ready())	{

				Gp_excall->m_stage_list.wipe(m_begin_sorter, m_end_sorter);
			}
		}
		// 通常ワイプの場合
		else if (m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

			Gp_stage_list->wipe(m_begin_sorter, m_end_sorter);
		}
	}

	// ワイプはステップ０から
	m_wipe_step = 0;

	// ワイプ待ちの場合はプロセスを変更
	if (wait_flag)	{
		wait(key_skip_mode, return_value_flag);
	}
	// ワイプ待ちでない場合はすぐに戻り値を返す
	else if (return_value_flag)	{
		tnm_stack_push_int(0);
	}

	// システム設定の「ワイプしない」チェック
	if (Gp_config->system.no_wipe_anime_flag)
		end();		// すぐにワイプを終わらせる
}

// ****************************************************************
// ワイプ終了
// ================================================================
void C_tnm_wipe::end()
{
	// ワイプ中の場合
	if (m_type >= 0)	{

		// ＥＸコールワイプの場合
		if (m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

			if (Gp_excall->is_ready())	{

				// ネクストステージを再初期化
				Gp_excall->m_stage_list[TNM_STAGE_NEXT].reinit(false);
			}
		}
		// 通常ワイプの場合
		else if (m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

			// ネクストステージを再初期化
			(*Gp_stage_list)[TNM_STAGE_NEXT].reinit(false);
		}
	}

	// ワイプを初期化
	init();
}

// ****************************************************************
// ワイプ待ち
// ================================================================
void C_tnm_wipe::wait(int key_skip_mode, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_WIPE_WAIT;
	proc.key_skip_enable_flag = 
		key_skip_mode == 0 ? false :
		key_skip_mode == 1 ? true :
		Gp_config->system.skip_wipe_anime_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// ワイプを判定
// ================================================================
bool C_tnm_wipe::check()
{
	return m_type >= 0;
}

// ****************************************************************
// ワイプ：時間を更新
// ================================================================
void C_tnm_wipe::update_time(int past_time)
{
	// 時刻を進める
	m_cur_time += past_time;
}

// ****************************************************************
// ワイプ：フレーム処理
// ================================================================
void C_tnm_wipe::frame()
{
	if (m_wipe_step == 0)	{

		// 次のステップへ
		m_wipe_step = 1;

		// 時間更新のために処理を抜ける
		return;
	}

	if (m_wipe_step == 1)	{

		// 時刻をここで再設定
		// ワイプ開始時に遅延でカクつかないようにするための処理
		m_cur_time = m_start_time;

		// 次のステップへ
		m_wipe_step = 2;
	}

	if (m_wipe_step == 2)	{

		// ワイプの進行度を計算（0～wipe_prg_max）
		double et = (double)m_end_time;
		double ct = (double)m_cur_time;

		m_wipe_prg = 
			m_end_time <= 0 ? TNM_WIPE_PRG_MAX : 
			m_speed_mode == 0 ? (int)((ct) * TNM_WIPE_PRG_MAX / et) : 
			m_speed_mode == 1 ? (int)((ct) * (ct) * TNM_WIPE_PRG_MAX / et / et) : 
			m_speed_mode == 2 ? (int)(- (ct - et) * (ct - et) * TNM_WIPE_PRG_MAX / et / et) + TNM_WIPE_PRG_MAX : 0;

		// 時間が来たらワイプ終了
		if (m_cur_time - m_end_time >= 0)
			end();
	}
}

