#pragma		once

#include	"db_wnd_info_listview.h"

class	C_elm_group_list;

// ****************************************************************
// デバッグ情報ウィンドウ：グループ
// ================================================================
class C_db_wnd_info_group : public C_db_wnd_info_listview
{
public:
	void		init();				// 初期化
	void		frame(C_elm_group_list** p_group_list, int group_list_cnt);				// フレーム

private:
	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

