#pragma		once

#include	"dialog/cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：フォント
// ================================================================
class C_cfg_wnd_func_font : public C_cfg_wnd_func
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

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_group	grp_font;
		Cc_listbox	lst_font;
		Cc_check	chk_proportional;
		Cc_static	stc_proportional;
		Cc_check	chk_font_futoku;
		Cc_radio	rad_font_shadow_none;
		Cc_radio	rad_font_shadow_shadow;
		Cc_radio	rad_font_shadow_fuchidoru;
		Cc_radio	rad_font_shadow_fuchidoru_shadow;
		Cc_static	stc_font_sample;
		Cc_button	btn_font_init;
		Cc_button	btn_font_close;
	}	ci;

	// 作業変数
	struct WORK
	{
		bool				frame_first_flag;	// 初回フレームフラグ
		C_font_name_list	fnlst;		// フォント名リスト
		TSTR				cur_font_name;		// 現在のフォント名

		TSTR		now_text;			// 現在の表示されている文字列
	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログ更新
	void		set_radio_shadow();			// 影のラジオボタンを設定
	int			get_radio_shadow();			// 影のラジオボタンを取得
	void		create_font_list();			// フォントリストを作成
	void		update_font();				// フォントを更新
	void		update_font_list();			// フォントリストを更新
	void		set_font_list();			// リストを現在のフォントに設定
	void		get_font_list();			// リストからフォント取得

	// 作業用フォント
	void		init_work_font();						// 初期化
	void		free_work_font();						// 開放
	void		load_work_font(CTSTR& font_name);		// 読み込み

	// 作業用フォント変数
	struct WORK_FONT
	{
		HFONT		original_font;		// 初期状態のフォント
		HFONT		now_font;			// 現在のフォント
		TSTR		now_font_name;		// 現在のフォントの名前
	}	work_font;

};
