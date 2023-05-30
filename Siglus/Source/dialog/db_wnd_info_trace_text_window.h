#pragma		once

#include	"db_wnd_info.h"
#include	"db_wnd_info_trace_draw_target.h"

// ****************************************************************
// デバッグ情報ウィンドウ：トレースのテキストウィンドウ
// ================================================================
class C_db_wnd_info_trace_text_window
{

public:
	C_db_wnd_info_trace_text_window();										// コンストラクタ
	~C_db_wnd_info_trace_text_window();										// デストラクタ
	LRESULT		message_proc(UINT msg, WPARAM wp, LPARAM lp);				// メッセージプロシージャ（※ウィンドウプロシージャではない）
	void		create(HWND parent_h_wnd, C_rect rect, int page_cnt);		// 作成
	void		destroy();													// 破棄
	void		resize_text_window_rect(C_rect rect);						// テキストウィンドウをリサイズする
	int			get_scn_no(int page_no);									// シーン番号を取得する
	void		set_focus_draw_target();									// 描画ターゲットをフォーカスする
	bool		check_focus_draw_target();									// 描画ターゲットがフォーカスされているか判定する
	void		alloc_seen_file(int alloc_cnt);								// シーンファイルの確保

	// 描画クリア
	void		draw_clear();

	// シーンファイルの描画
	void		draw_seen_file(int page_no, int scn_no, int draw_top_line_no, int target_line_no, CTSTR& font_name, int font_size, bool is_target);

	// ページ変更のシーンファイルの再描画
	void		change_draw_seen_file(int page_no);

	// シーンファイルの再描画
	void		re_draw_seen_file(int top_line_no);
	void		re_draw_seen_file(CTSTR& font_name, int font_size);
	void		re_draw_seen_file();

private:

	// 文字情報（フォント情報）
	struct MOJI_STATE
	{
		TSTR		font_name;				// フォント名
		int			font_size;				// フォントサイズ
		HFONT		h_font;					// フォントハンドル
		int			x_space;				// 字間
		int			y_space;				// 行間

		MOJI_STATE();						// コンストラクタ
		~MOJI_STATE();						// デストラクタ
		void		init();					// 初期化
		void		free();					// 開放
		bool		set(CTSTR& name, int size);		// 設定
	};

	// 描画情報
	struct DRAW_STATE
	{
		int			scn_no;					// ターゲットのシーン番号
		int			target_line_no;			// ターゲットの行番号
		int			focus_line_no;			// フォーカスの行番号
		int			in_line_no;				// ＩＮの行番号
		int			out_line_no;			// ＯＵＴの行番号
		int			draw_top_line_no;		// 描画の先頭行番号
		int			draw_end_line_no;		// 描画の最終行番号
		int			draw_line_cnt;			// 描画の行数

		DRAW_STATE();						// コンストラクタ
		void		init();					// 初期化
	};

	// シーン情報
	struct SEEN_STATE
	{
		ARRAY<TSTR>	text_data;				// テキストデータ
		ARRAY<int>	text_state_now;			// テキスト情報（現在）
		ARRAY<int>	text_state_last;		// テキスト情報（前回）
		int			draw_cnt;				// 描画回数
		DRAW_STATE	draw_state;				// 描画情報

		SEEN_STATE();						// コンストラクタ
		~SEEN_STATE();						// デストラクタ
		void		init();					// 初期化
		void		free();					// 開放
		bool		load(int scn_no);		// 読み込み
		void		copy_text_state();		// テキスト情報のコピー
	};

	// 作業変数
	struct WORK
	{
		HWND				parent_h_wnd;			// 親のハンドル
		MOJI_STATE			moji_state;				// 文字情報（フォント情報）
		ARRAY<SEEN_STATE>	seen_state;				// シーン情報
		DRAW_STATE			current_draw_state;		// 現在の描画情報
		int					current_page_no;		// 現在のページ番号
		bool				no_data_draw_flag;		// ノーデータ描画フラグ
	}	work;

	// 描画ターゲット
	C_db_wnd_info_trace_draw_target	draw_target;	// 描画ターゲット

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void		shift_seen_file();								// シーンファイルのシフト

	void		init_current_draw_state();						// カレントの描画情報を初期化

	C_point		change_draw_target_pos(C_point pos);			// 描画ターゲットの座標に変換する
	C_point		change_draw_target_pos(int x, int y);			// 描画ターゲットの座標に変換する
	C_point		change_draw_target_pos(LPARAM lp);				// 描画ターゲットの座標に変換する

	bool		check_draw_target_on_mouse(C_point pos);		// 描画ターゲットにマウスが乗っているか判定する
	bool		check_draw_target_on_mouse(int x, int y);		// 描画ターゲットにマウスが乗っているか判定する
	bool		check_draw_target_on_mouse(LPARAM lp);			// 描画ターゲットにマウスが乗っているか判定する

	void		set_focus_draw_target(WPARAM wp, LPARAM lp);	// 描画ターゲットをフォーカスする
	void		on_mousewheel(WPARAM wp, LPARAM lp);			// マウスホイール
	void		on_vscroll(WPARAM wp, LPARAM lp);				// 縦スクロール

	void		draw_text(ARRAY<SEEN_STATE>::iterator seen_state_itr, bool redisp_flag);		// テキストの描画

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 縦スクロールバー
	class Citem_v_scroll : public Cc_scroll
	{
	public:
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};
	Citem_v_scroll		v_scroll;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 横スクロールバー
	class Citem_h_scroll : public Cc_scroll
	{
	public:
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};
	Citem_h_scroll		h_scroll;
};

