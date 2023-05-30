#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：システム情報
// ================================================================
class C_db_wnd_info_save_history : public C_db_wnd_info_listview
{

public:

	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int		m_save_history_top;				// 履歴の先頭
		int		m_save_history_cnt;				// 履歴の数
	}	m_ld;

	// 作業変数
	struct WORK
	{
	}	work;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
	bool		on_notify(WPARAM wp, LPARAM lp);	// ＮＯＴＩＦＹ

	void		load(int click_index);	// ロード

};

