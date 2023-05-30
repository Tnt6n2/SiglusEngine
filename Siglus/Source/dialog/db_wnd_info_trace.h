#pragma		once

#include	"db_wnd_info.h"
#include	"db_wnd_info_trace_text_window.h"


// ****************************************************************
// デバッグ情報ウィンドウ：トレース
// ================================================================
class C_db_wnd_info_trace : public C_db_wnd_info
{

public:

	void		init();						// 初期化
	void		load_state();				// ステータス読み込み
	void		save_state();				// ステータス保存
	void		frame();					// フレーム

private:

	// テキストタブ
	class Ctab_text : public Cc_tab
	{
	public:
		C_db_wnd_info_trace *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_check	chk_break;
		Cc_check	chk_click;
		Cc_check	chk_trace;
		Cc_combobox	cmb_font_size;
		Cc_button	btn_next;
		Ctab_text	tab_text;
	}	ci;

	// トレース情報
	struct TRACE_STATE
	{
		int		scn_no;
		int		line_no;
		bool	is_breaking;
	};

	// 作業変数
	struct WORK
	{
		TRACE_STATE		cd;					// 現在のトレース情報
		TRACE_STATE		ld;					// 前回のトレース情報
		int				page_cnt;			// ページ数（タブ数）
		bool			next_click_state;	// 画面をクリックした情報
		bool			next_key_state;		// 画面をクリックした情報
	}	work;

	// テキストウィンドウ
	BSP<C_db_wnd_info_trace_text_window>		p_text_window;


	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知

	void		set_break(bool is_breaking);					// ブレーク状態を設定

	void		next_step();									// 次のステップへ進む

	void		draw_scn_for_trace(int scn_no, int line_no);	// テキスト描画（トレース中）
	void		draw_scn_for_change_select_tab(int tab_no);		// テキスト描画（タブ変更時）
	void		draw_text_for_change_font_size(int font_size);	// テキスト描画（文字サイズ変更時）

	void		set_focus_draw_target();						// 描画ターゲットをフォーカスする
	bool		check_focus_draw_target();						// 描画ターゲットがフォーカスされているか判定する

	void		set_tab_str();									// タブの文字列変更
};

