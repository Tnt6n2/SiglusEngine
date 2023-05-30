#pragma		once

#include	"dialog/db_wnd_info_listview.h"

class	C_elm_object_list;

// ****************************************************************
// デバッグ情報ウィンドウ：オブジェクト
// ================================================================
class C_db_wnd_info_object : public C_db_wnd_info_listview
{
public:
	void	init();		// 初期化
	void	frame();	// フレーム

	// ページ番号を設定
	void	set_page_no(int page_no)	{	m_page_no = page_no;	}

private:

	void	on_init_func();		// 初期化ファンクション
	void	on_close_func();	// 閉じるファンクション

	// ウィンドウプロシージャ
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知

	// リストビューをダブルクリックした
	void	on_dblclk_lv();

	// リストビューの値を変更した
	void	on_change_lv_value(int item_no, int column_no, CTSTR& value);

	// オブジェクトを取得する
	void	get_page_object(C_elm_object_list** pp_int_list);

	// パラメータ
	int		m_page_no;
};
