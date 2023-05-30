#pragma		once

#include	"cfg_wnd_func.h"

struct S_tnm_save_header;

// ****************************************************************
// システムウィンドウ：ファンクション：セーブロード
// ================================================================
class C_sys_wnd_func_saveload : public C_cfg_wnd_func
{
public:
	void		on_init_funcfunc();										// 初期化
	void		on_close_funcfunc();									// 閉じる
	bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	void		frame_funcfunc();										// フレーム

	void		init_state();							// 初期状態に戻す

	int			select_return_value;					// モーダル版の選択データ番号（-1 = キャンセル）

private:

	// セーブロードタブ
	class Cc_tab_saveload : public Cc_tab
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// セーブロードリスト
	class Cc_lst_saveload : public Cc_listview
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// セーブロード関連のチェックボックス
	class Cc_chk_saveload : public Cc_check
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// セーブロード関連のボタン
	class Cc_btn_saveload : public Cc_button
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_tab_saveload		tab_saveload;
		Cc_lst_saveload		lst_saveload;
		Cc_chk_saveload		chk_warning;
		Cc_chk_saveload		chk_dblclick;
		Cc_btn_saveload		btn_new;
		Cc_btn_saveload		btn_saveload;
		Cc_btn_saveload		btn_close;
	}	ci;

	// 作業変数
	struct WORK
	{
		int		page_cnt;
		int		data_cnt_par_page;
		int		data_index_keta_mode;

		int		now_page_no;
		int		data_top_no;
		int		data_end_no;
		ARRAY<S_tnm_save_header>	data_header_list;
		ARRAY<bool>					data_header_exist;

		int		new_save_data_no;
		int		new_tab_page_no;
	}	work;

	void		update_dialog();						// ダイアログ更新
	void		create_data_list();						// データリスト作成
	void		set_save_bnt_enable();					// セーブボタンの禁止・許可
	TSTR		create_data_list_str(int data_no, S_tnm_save_header *p_header);	// データリストの文字列作成

	void		execute_saveload();						// セーブロードの実行
	void		execute_saveload_func(int data_no);		// セーブロードの実行

	void		set_focus_list(bool up_down_flag);		// リストをフォーカス
	void		set_focus_tab(bool left_right_flag);	// タブをフォーカス

	void		escape_key_func();						// エスケープキー
	void		cursor_control(WPARAM wp);				// カーソルコントロール
	bool		on_getdlgcode(WPARAM wp, LPARAM lp, LRESULT *ret);	// GETDLGCODE

};
