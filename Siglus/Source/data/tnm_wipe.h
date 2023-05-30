#pragma		once

#include	"data/tnm_sorter.h"

// ****************************************************************
// ワイプ
// ================================================================

// 定数
const int TNM_WIPE_PRG_MAX = 65536;
const int TNM_WIPE_OPTION_MAX = 8;

const int TNM_WIPE_RANGE_NORMAL = 0;		// 通常ステージのみ
const int TNM_WIPE_RANGE_EX_CALL = 1;		// ＥＸステージのみ
const int TNM_WIPE_RANGE_SYSTEM_OUT = 2;	// 全て（フェードイン）
const int TNM_WIPE_RANGE_SYSTEM_IN = 3;		// 全て（フェードアウト）

// ワイプ
class C_tnm_wipe
{
public:
	void			init();
	void			start(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int m_speed_mode, int* option, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
	void			start_mask(CTSTR& mask_file, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int m_speed_mode, int* option, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
	void			end();
	void			wait(int key_skip_mode, bool return_value_flag);
	bool			check();
	void			update_time(int past_time);
	void			frame();
	bool			is_wipe_doing()	{	return m_type >= 0;	}

	int				m_type;							// ワイプタイプ
	int				m_speed_mode;					// スピードモード
	int				m_option[TNM_WIPE_OPTION_MAX];	// オプション
	int				m_cur_time;						// ワイプ現在時刻
	int				m_start_time;					// ワイプ開始時刻
	int				m_end_time;						// ワイプ終了時刻
	S_tnm_sorter	m_begin_sorter;					// 開始ソーター
	S_tnm_sorter	m_end_sorter;					// 終了ソーター
	int				m_wipe_range;					// ワイプ範囲
	int				m_with_low_order;				// 低いオーダーまで含める
	TSTR			m_mask_file;					// マスクファイル
	int				m_wipe_step;					// ステップ
	int				m_wipe_prg;						// ワイプ進行度

private:
	void			start_sub(bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
};
