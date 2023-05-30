#pragma		once

#include	"eng_proc.h"
#include	"db_wnd_info_listview.h"

class	Ctnm_input;

// ****************************************************************
// デバッグ情報ウィンドウ：システム情報
// ================================================================
class Cdb_wnd_info_system : public Cdb_wnd_info_listview
{

public:

	void		init();		// 初期化
	void		open();		// 開く（作成）
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int				cur_scn_no;
		int				cur_line_no;
		TNM_PROC_TYPE	proc_type;
		int				stack_size;
		int				element_stack_size;
		int				real_time;
		int				local_real_time;
		int				local_game_time;
		bool			ctrl_skip_flag;

		int				moji_map_use_cnt;		// 使用中の文字マップ数
		int				moji_map_now_cnt;		// 現在の文字マップ数
		int				moji_map_max_cnt;		// 最大の文字マップ数

		int				gan_data_map_now_cnt;	// 現在のＧＡＮデータマップ数
		int				gan_data_map_max_cnt;	// 最大のＧＡＮデータマップ数

		int				test;

	}	m_ld;

	// 作業変数
	bool	m_first;	// 初回
};

