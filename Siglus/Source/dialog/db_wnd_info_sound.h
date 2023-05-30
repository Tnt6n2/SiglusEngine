#pragma		once

#include	"db_wnd_info_listview.h"

class	C_tnm_ini;
class	C_elm_sound;

// ****************************************************************
// デバッグ情報ウィンドウ：サウンド
// ================================================================
class C_db_wnd_info_sound : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		struct ITEM
		{
			int		type;
			int		player_no;
			TSTR	file_name;
			int		volume;
			int		delay_time;
			int		state;
			int		state2;
			int		option;
			bool	pause_flag;
			bool	bgmfade_target;
			bool	bgmfade2_target;
		};

		ARRAY<ITEM>	item_list;
		
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

