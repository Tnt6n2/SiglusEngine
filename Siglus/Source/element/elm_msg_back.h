#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_timer.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_object.h"

class C_save_stream;

// ****************************************************************
// エレメント：メッセージバック
// ================================================================
class C_elm_msg_back : public C_tnm_element
{
public:
	// エレメント基本処理
	void	init();								// 初期化
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード

	// フレーム処理
	void	frame_init();											// フレーム初期化
	void	frame(int time, bool draw_futoku, bool draw_shadow);	// フレーム処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);			// スプライトツリーに設定

	// ボタン処理
	void	regist_button(int min_order);	// ボタンを登録
	void	button_event();					// ボタンイベント処理

	// スクロールの余波が発生しているかどうかを判定する
	bool	check_aftereffects();

	// 整理する
	void	remove();

	// 初期処理
	void	open();			// 開く
	void	close();		// 閉じる
	bool	is_open()		{	return m_window_appear;		}
	bool	is_exist_msg()	{	return m_history_cnt > 0;	}

	// メッセージ処理
	bool	add_koe(int koe_no, int chara_no, int scn_no, int line_no);					// 声を追加
	bool	add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no);	// 名前を追加
	bool	add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no);				// メッセージを追加
	bool	add_new_line_indent(int scn_no, int line_no);								// 改行を追加
	bool	add_new_line_no_indent(int scn_no, int line_no);							// 改行を追加
	bool	add_pct(CTSTR& file_name, C_point pos);

	void	set_save_id(S_tid save_id);
	void	set_save_id_check_flag(bool save_id_check_flag);
	bool	get_save_id_check_flag();

	void	clear_msg();				// メッセージをクリア
	void	ready_msg();				// メッセージを追加準備
	void	next_msg();					// 次のメッセージへ
	void	page_up_key_down();
	void	page_dw_key_down();
	void	target_up();
	void	target_down();
	void	target_up_for_wheel();
	void	target_down_for_wheel();

	// 情報を取得
	int		get_koe_no();
	TSTR	get_original_name();
	TSTR	get_disp_name();
	TSTR	get_debug_msg();
	int		get_scn_no();
	int		get_line_no();

	// ヒストリー構造体
	struct S_history
	{
		bool					pct_flag;			// 画像フラグ（0=文字列、1=画像）
		TSTR					msg_str;			// メッセージ（文字列）／ファイル名
		TSTR					original_name;		// オリジナルの名前
		TSTR					disp_name;			// 表示用の名前
		C_elm_mwnd_msg			msg;				// メッセージ
		C_point					pct_pos;			// 画像の位置
		int						total_pos;			// 全体での位置
		int						height;				// 高さ
		C_elm_object			koe_btn;			// 声ボタン／画像
		C_elm_object			load_btn;			// ロードボタン／画像
		C_tnm_pod_array<int>	koe_no_list;		// 声番号リスト
		C_tnm_pod_array<int>	chr_no_list;		// キャラクタ番号リスト
		int						koe_play_no;		// 声再生番号
		TSTR					debug_msg;			// デバッグメッセージ（文字列）
		int						scn_no;				// シーン名
		int						line_no;			// 行番号
		S_tid					save_id;			// 関連付けるセーブデータのID
		bool					save_id_check_flag;	// 関連付けるセーブデータのIDをチェックした
	};

	// セパレータ構造体
	struct S_separator
	{
		int						total_pos;			// 全体での位置
		int						height;				// 高さ
		C_tnm_sprite			sprite;				// スプライト
	};

private:

	// ウィンドウステート
	struct STATE
	{
		C_rect	window_rect;		// ウィンドウの位置（スクリーンからの座標）
		C_rect	disp_margin;		// 表示矩形
		int		msg_pos;			// メッセージの位置
		C_point	koe_btn_pos;		// 声ボタンの位置
		C_point	load_btn_pos;		// ロードボタンの位置
	};

	STATE				m_def;				// デフォルトのウィンドウステート
	STATE				m_cur;				// 現在のウィンドウステート
	int					m_history_cnt_max;	// 履歴の最大数

	// パラメータ
	ARRAY<S_history>	m_history_list;				// 履歴
	int					m_history_cnt;				// 履歴数
	int					m_history_start_pos;		// 履歴開始位置
	int					m_history_last_pos;			// 履歴終了位置
	int					m_history_insert_pos;		// 履歴を入れる位置
	int					m_new_msg_flag;				// 新しいメッセージフラグ

	// セパレータ
	ARRAY<S_separator>	m_separator_list;			// セパレータ

	// 作業パラメータ
	bool				m_window_appear;			// ウィンドウ出現フラグ
	int					m_msg_total_height;			// メッセージ全体の高さ
	int					m_target_no;				// ターゲット番号
	int					m_mouse_target_no;			// マウスが当たっているターゲット番号

	int					m_scroll_pos;				// スクロール位置
	int					m_slider_pos;				// スライダー位置
	bool				m_slider_operating;			// スライダーを操作中
	int					m_slider_start_slider_pos;	// スライダー操作開始時のスライダー位置
	int					m_slider_start_mouse_pos;	// スライダー操作開始時のマウス位置

	bool				m_drag_ready_flag;			// ドラッグ準備フラグ
	bool				m_drag_flag;				// ドラッグ中
	C_point				m_drag_start_mouse_pos;		// ドラッグ開始時のマウス位置
	int					m_drag_start_scroll_pos;	// ドラッグ開始時のスクロール位置

	bool				m_drag_aftereffects_flag;				// ドラッグの余波：余波フラグ
	DWORD				m_drag_aftereffects_start_time;			// ドラッグの余波：計測開始の時間
	int					m_drag_aftereffects_mouse_pos;			// ドラッグの余波：計測開始のマウス位置
	int					m_drag_aftereffects_distance;			// ドラッグの余波：余波の距離
	int					m_drag_aftereffects_start_scroll_pos;	// ドラッグの余波：余波の開始のスクロール位置
	int					m_drag_aftereffects_end_scroll_pos;		// ドラッグの余波：余波の終了のスクロール位置

	bool				m_page_up_key;
	bool				m_page_dw_key;
	int					m_target_up_start_time;		// PageUp の開始時間
	int					m_target_dw_start_time;		// PageDown の開始時間

	BSP<C_d3d_album>	m_album_waku;
	C_tnm_sprite		m_spr_waku;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_filter;

	BSP<C_d3d_album>	m_album_separator;
	BSP<C_d3d_album>	m_album_separator_top;
	BSP<C_d3d_album>	m_album_separator_bottom;

	C_elm_object		m_slider;
	C_elm_object		m_close_btn;
	C_elm_object		m_msg_up_btn;
	C_elm_object		m_msg_down_btn;
	C_elm_object		m_ex_btn_1;
	C_elm_object		m_ex_btn_2;
	C_elm_object		m_ex_btn_3;
	C_elm_object		m_ex_btn_4;

	void	restruct_template();				// テンプレートを再構築
	void	init_work_variable();				// 処理変数初期化

	void	create_msg();
	void	target_up_sub(int cnt);
	void	target_down_sub(int cnt);
	void	update_pos_from_target();
	void	update_pos_from_slider();
	void	update_pos_from_scroll();
	void	calc_scroll_pos_from_target();
	void	calc_scroll_pos_from_slider();
	void	calc_slider_pos_from_scroll();
	void	calc_target_no_from_scroll();
	void	msg_click_event();
	void	load_call(S_tid save_id);
	void	ex_btn_call(int btn_no);
};




