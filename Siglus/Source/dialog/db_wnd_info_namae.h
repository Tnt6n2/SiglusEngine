#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：【名前】
// ================================================================
class C_db_wnd_info_namae : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:
	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

