#pragma		once

#include	"db_wnd_info_listview.h"

class	C_tnm_ini;
class	C_elm_sound;

// ****************************************************************
// デバッグ情報ウィンドウ：システム：サウンド
// ================================================================
class C_db_wnd_info_system_sound : public C_db_wnd_info_listview
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
			int		volume;
			bool	check;
		};

		ARRAY<ITEM>	item_list;
		
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

