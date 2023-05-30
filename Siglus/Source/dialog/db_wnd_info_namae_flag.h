#pragma		once

#include	"db_wnd_info.h"

class	C_elm_flag;
class	C_elm_str_list;
class	C_tnm_dbg_listview;

// ****************************************************************
// デバッグ情報ウィンドウ：名前フラグ
// ================================================================
class C_db_wnd_info_namae_flag : public C_db_wnd_info
{
public:
	void		init();						// 初期化
	void		load_state();				// ステータス読み込み
	void		save_state();				// ステータス保存
	void		frame();					// フレーム

private:

	enum	{	PAGE_CNT = 2,	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lv_flag[PAGE_CNT];
	}	ci;

	// ステータス
	struct STATE
	{
		ARRAY<TSTR>	str_flag[PAGE_CNT];
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
	void		on_dblclk_lv(int ctrl_id, int form);			// リストビューをダブルクリックした

	void		change_page(int page_no);									// ページを変更する
	void		change_str_flag(int ctrl_id, int flag_no, CTSTR& value);	// 文字列フラグを更新する

	C_tnm_dbg_listview*	get_lv_by_ctrl_id(int ctrl_id);			// コントロールＩＤからリストビューを取得する
	C_elm_str_list*		get_str_list_by_ctrl_id(int ctrl_id);	// コントロールＩＤから文字列リストを取得する
};

