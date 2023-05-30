#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// デバッグ情報ウィンドウ：汎用リストビュー
// ================================================================
class C_db_wnd_info_listview : public C_db_wnd_info
{
public:
	void		regist_caption_name(CTSTR& name);								// キャプション名登録
	void		listview_add_column(CTSTR& name, CTSTR& caption, int width);	// リストビューの項目追加
	void		open(HWND parent_h_wnd = G_app.h_wnd);							// 開く（作成）

	C_tnm_dbg_listview*	get_lv()	{	return &lv;	}

protected:

	// フレーム初回フラグ
	bool	frame_first_flag;

	// キャプション名
	TSTR	caption_name_str;


	// リストビューコラムリスト
	struct LISTVIEW_COLUMN
	{
		TSTR	name;
		TSTR	caption;
		int		width;
	};
	ARRAY<LISTVIEW_COLUMN>	lvc_list;

	// リストビュー
	C_tnm_dbg_listview		lv;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる

	virtual		void		on_init_func() = 0;					// 初期化
	virtual		void		on_close_func() = 0;				// 閉じる
};

