#pragma		once

#include	"db_wnd_info.h"

class	C_elm_mwnd_list;

// ****************************************************************
// デバッグ情報ウィンドウ：メッセージウィンドウ
// ================================================================
class C_db_wnd_info_mwnd : public C_db_wnd_info
{
public:
	void		init();					// 初期化
	void		load_state();			// ステータス読み込み
	void		save_state();			// ステータス保存
	void		frame(C_elm_mwnd_list** mwnd_list_list, int list_cnt);		// フレーム

private:

	enum
	{
		PAGE_CNT = 2,
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lv[PAGE_CNT];
	}	ci;

	// ステータス
	struct STATE
	{
		struct SUB
		{
			int		mwnd_no;
			int		world;
			int		layer;
			bool	appear_flag;
			TSTR	msg;
		};

		ARRAY<SUB>	mwnd_list[PAGE_CNT];

	}	m_ld;

	// 作業変数
	int			m_page_no;
	bool		m_first[PAGE_CNT];

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知

	void		on_change_page(int page_no);					// ページが変更された

	void		change_page(int page_no);						// ページを変更する
	void		update_one_item(int item_no, bool change);		// １つのアイテムを更新する
};

