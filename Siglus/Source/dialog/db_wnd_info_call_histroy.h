#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：呼び出し履歴
// ================================================================
class C_db_wnd_info_call_histroy : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// アイテム
	struct	ITEM
	{
		int		call_type;
		bool	excall_flag;
		bool	frame_action_flag;
		int		src_scn;
		int		dst_scn;
		int		dst_label;
	};

	// ステータス
	struct STATE
	{
		ARRAY<ITEM>	item_list;
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

