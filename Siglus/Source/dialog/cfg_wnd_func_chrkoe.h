#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：キャラクター音声
// ================================================================
class Ccfg_wnd_func_chrkoe : public C_cfg_wnd_func
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

	void		on_mousewheel_character_list(WPARAM wp, LPARAM lp);	// キャラクターリストのマウスホイール

private:

	// キャラクターリスト
	class Cc_chalst : public Cc_static
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// キャラクターチェックボックス
	class Cc_character_chk : public Cc_check
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// キャラクタースライダー
	class Cc_character_sld : public Cc_slider
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_group	grp_chrkoe;
		Cc_chalst	lst_chrkoe;
		Cc_scroll	scr_chrkoe;
		Cc_button	btn_chrkoe_all_on;
		Cc_button	btn_chrkoe_all_off;
		Cc_button	btn_chrkoe_init;
		Cc_button	btn_chrkoe_close;
	}	ci;

	// 作業変数
	struct WORK
	{
		int			character_cnt;
		int			character_list_scrool_pos;
		int			character_list_width;
		int			character_chkbox_max_width;

		// キャラクター構造体
		struct CHARACTER_STRUCT
		{
			int					ini_no;
			Cc_character_chk	item_chk;
			Cc_character_sld	item_sld;
		};

		ARRAY<CHARACTER_STRUCT>		character;

	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログ更新
	void		focus_scrollbar();			// スクロールバーにフォーカスを合わせる

	// キャラクターリスト
	void		update_character_list();	// キャラクターリストの更新
	void		resize_character_list();	// キャラクターリストのリサイズ
	bool		on_command_character_list(WPARAM wp, LPARAM lp);	// キャラクターリストのコマンド
	bool		on_h_scroll_character_list(WPARAM wp, LPARAM lp);	// キャラクターリストのＨスクロール

};
