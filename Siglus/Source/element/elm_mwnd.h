#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_element.h"

#include	"element/elm_mwnd_waku.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_mwnd_name.h"
#include	"element/elm_mwnd_sel.h"

struct	S_tnm_read_flag;
class	C_tnm_save_stream;
class	C_elm_mwnd_list;
class	C_elm_object_list;

// ****************************************************************
// メッセージウィンドウ
// ================================================================
class C_elm_mwnd : public C_tnm_element
{
public:
	// エレメント基本処理
	void	init(int mwnd_no, S_element element, CTSTR& name, C_elm_mwnd_list* p_parent, S_element elm_group_list);				// 初期化
	void	reinit(bool restruct_flag);										// 再初期化
	void	finish();														// 終了
	void	save(C_tnm_save_stream& stream);								// セーブ
	void	load(C_tnm_save_stream& stream);								// ロード
	void	copy(C_elm_mwnd* src);											// コピー
	void	set_template(int template_no);									// テンプレートを設定

	// フレーム処理
	void	frame_init();											// フレーム初期化処理
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	bool	frame(bool draw_futoku, bool draw_shadow);				// フレーム処理
	void	window_anime(bool anime_open, int anime_type, int anime_prog, C_rect *window_rect, C_rect *win_name_rect, C_point *ret_pos_rep, C_point *ret_abs_center, C_size *ret_abs_scale, int *ret_abs_rotate, BYTE *ret_tr_rep);		// ウィンドウアニメ処理
	void	do_frame_action();										// フレームアクション処理

	void	msg_frame();											// メッセージを進める
	void	jump_to_msg_wait_by_nowait();							// メッセージウェイトまで処理を進める（名前変えたい）
	void	jump_to_msg_wait_by_skip();								// メッセージウェイトまで処理を進める（名前変えたい）

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタンイベント処理
	void	regist_button();
	void	button_event(bool wait_flag);

	// 初期処理
	void	open();						// 開く
	void	close();					// 閉じる
	void	end_close();				// 閉じるを終了
	void	set_auto_proc_ready();		// 自動処理開始
	void	clear_auto_proc_ready();	// 自動処理終了

	// メッセージブロックフラグ
	void	clear_msg_block_start_flag();	// メッセージブロックを開始したフラグをクリア
	void	set_msg_block_start_flag();		// メッセージブロックを開始したフラグをセット
	bool	is_msg_block_started();			// メッセージブロックを開始したフラグを判定

	// 声処理
	void	clear_koe();													// 声のクリア
	void	set_koe(int koe_no, bool play_flag, bool no_auto_mode_flag);	// 声の設定

	// 名前処理
	void	clear_name();				// 名前のクリア
	void	set_name(CTSTR& name);		// 名前の設定
	TSTR	get_name();					// 名前の取得

	// 顔グラ処理
	void	clear_face();								// 顔グラのクリア
	void	set_face(int face_no, CTSTR& file_path);	// 顔グラの設定

	// メッセージ処理
	void	clear_msg();					// メッセージのクリア
	bool	add_msg_check(bool new_line_flag);						// メッセージが入るかをチェック
	void	add_msg(CTSTR& msg, TSTR* ret_over_flow_msg);			// メッセージを追加
	void	add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg);	// 名前をメッセージとして追加
	int		get_moji_cnt();					// 文字数を取得
	int		get_disp_moji_cnt();			// 文字数を取得
	void	disp_one_moji();				// １つの文字を表示
	void	disp_all_moji();				// 全ての文字を表示
	void	set_line_head();				// 行の先頭にする（インデント文字の判定を行う）
	void	new_line_indent();				// 改行（インデントあり）
	void	new_line_no_indent();			// 改行（インデントなし）
	void	set_indent();					// インデントを設定
	void	clear_indent();					// インデントをクリア
	void	set_moji_rep_pos(C_point pos);	// 文字の位置を設定
	void	set_moji_rep_pos_default();		// 文字の位置をデフォルトに戻す
	void	set_moji_size(int size);		// 文字の大きさを設定
	void	set_moji_size_default();		// 文字の大きさをデフォルトに戻す
	void	set_moji_color(int color_no);	// 文字の色を設定
	void	set_moji_color_default();		// 文字の色をデフォルトに戻す
	void	clear_chara_moji_color();		// キャラクタ文字色をクリア
	void	clear_chara_shadow_color();		// キャラクタ影色をクリア
	void	clear_chara_fuchi_color();		// キャラクタ縁色をクリア
	void	set_chara_moji_color(int color_no);		// キャラクタ文字色を設定
	void	set_chara_shadow_color(int color_no);	// キャラクタ影色を設定
	void	set_chara_fuchi_color(int color_no);	// キャラクタ縁色を設定
	void	start_ruby(CTSTR& str);			// ルビを開始
	void	end_ruby();						// ルビを終了
	void	start_button(int btn_no, int group_no, int action_no, int se_no);		// ボタンを開始
	void	end_button();					// ボタンを終了
	void	set_last_moji_disp_time();		// 最後に表示した時間を設定する
	TSTR	get_debug_msg();				// デバッグメッセージを取得

	// マルチメッセージ
	void	next_msg();						// 次のメッセージにターゲットを移す

	// メッセージ速度計算処理
	void	set_auto_mode_end_moji_cnt()		{ m_auto_mode_end_moji_cnt = m_msg_list[0].get_disp_moji_cnt(); }
	int		get_auto_mode_end_moji_cnt()		{ return m_auto_mode_end_moji_cnt; }
	void	set_progress_start_flag(bool value);

	// 選択処理
	void	clear_sel();					// 選択肢をクリア
	void	ready_sel();					// 選択肢を準備
	void	add_sel(CTSTR& msg, int item_type, int color);	// 選択肢を追加
	void	set_sel_cancel_enable(bool enable);				// キャンセル可能フラグを設定
	void	restruct_sel();					// 選択肢を整える
	int		get_select_no(C_point point);	// マウス位置から現在選んでいる選択肢番号を取得

	// 枠の処理
	void	set_waku(int msg_waku_no, int name_waku_no, bool init_flag);		// 枠を設定
	int		get_waku_no()		{ return m_msg_waku.get_template_no(); }	// メッセージ枠番号を取得
	int		get_name_waku_no()	{ return m_name_waku.get_template_no(); }	// 名前枠番号を取得
	bool	restruct_msg_waku();				// 枠の再構築
	bool	restruct_name_waku();				// 名前枠の再構築

	// 枠の付属処理
	void	init_waku_file()			{ m_msg_waku.init_waku_file(); }	// 枠ファイルを初期化
	void	set_waku_file(CTSTR& file)	{ m_msg_waku.set_waku_file(file); }	// 枠ファイルを設定
	TSTR	get_waku_file()				{ return m_msg_waku.get_waku_file(); }	// 枠ファイルを取得
	void	init_filter_file()				{ m_msg_waku.init_filter_file(); }	// フィルターファイルを初期化
	void	set_filter_file(CTSTR& file)	{ m_msg_waku.set_filter_file(file); }	// フィルターファイルを設定
	TSTR	get_filter_file()				{ return m_msg_waku.get_filter_file(); }	// フィルターファイルを取得
	void	set_key_icon_appear(bool appear);	// キー待ちアイコン
	void	set_key_icon_mode(int mode);		// キー待ちアイコン

	// 共通処理
	bool	is_empty();						// テキストが空かどうかを調べる
	void	clear();						// クリア
	void	clear_for_novel_one_msg();		// ノベルに対して行単位の開始処理
	void	set_clear_ready_flag()				{ m_clear_ready_flag = true; }
	bool	get_clear_ready_flag()				{ return m_clear_ready_flag; }
	bool	check_not_appear_moji();		// まだ出現していない文字が存在するか判定する

	// ウィンドウ処理
	bool	get_window_appear_flag()			{ return m_window_appear; }
	bool	get_name_appear_flag()				{ return m_name_appear; }
	int		get_koe_no()						{ return m_koe_no; }
	bool	get_koe_play_flag()					{ return m_koe_play_flag; }
	bool	get_koe_no_auto_mode_flag()			{ return m_koe_no_auto_mode_flag; }

	// 既読フラグ処理
	void	add_read_flag(int scn_no, int flag_no);		// 既読フラグを追加する
	void	set_read_flag();							// 登録した既読フラグを設定する
	void	clear_read_flag();							// 既読フラグをクリアする

	// スライドメッセージ
	void	slide_msg();					// 文字スライドアニメ
	void	set_slide_msg(bool value, int time)		{ m_slide_msg = value;	m_slide_time = time; }
	bool	is_slide_msg()							{ return m_slide_msg; }

	// アニメ処理
	bool	is_open_anime_doing();		// 開きアニメ中
	bool	is_close_anime_doing();		// 閉じアニメ中
	bool	is_slide_anime_doing();		// スライドアニメ中
	void	end_open_anime();			// 開きアニメを終わらせる
	void	end_close_anime();			// 閉じアニメを終わらせる
	void	end_slide_anime();			// 文字スライドアニメを終わらせる


	// メッセージウィンドウ番号
	int				get_mwnd_no()					{ return m_mwnd_no; }

	// ソーターの取得／設定
	void			set_sorter(S_tnm_sorter value)	{ m_param.sorter = value; }
	S_tnm_sorter	get_sorter()					{ return m_param.sorter; }
	void			set_order(int value)			{ m_param.sorter.order = value; }
	void			set_layer(int value)			{ m_param.sorter.layer = value; }
	void			set_world(int value)			{ m_param.world = value; }
	int				get_order()						{ return m_param.sorter.order; }
	int				get_layer()						{ return m_param.sorter.layer; }
	int				get_world()						{ return m_param.world; }

	// パラメータの取得／設定
	void	init_window_pos();
	void	init_window_size();
	void	init_open_anime_type();
	void	init_open_anime_time();
	void	init_close_anime_type();
	void	init_close_anime_time();

	void	set_window_pos(C_point pos)			{ m_param.window_pos = pos; }
	void	set_window_size(C_size size)		{ m_param.window_size = size; }
	void	set_open_anime_type(int value)		{ m_param.open_anime_type = value; }
	void	set_open_anime_time(int value)		{ m_param.open_anime_time = value; }
	void	set_close_anime_type(int value)		{ m_param.close_anime_type = value; }
	void	set_close_anime_time(int value)		{ m_param.close_anime_time = value; }

	C_point	get_window_pos()					{ return m_param.window_pos; }
	C_size	get_window_size()					{ return m_param.window_size; }
	int		get_open_anime_type()				{ return m_param.open_anime_type; }
	int		get_open_anime_time()				{ return m_param.open_anime_time; }
	int		get_close_anime_type()				{ return m_param.close_anime_type; }
	int		get_close_anime_time()				{ return m_param.close_anime_time; }
	int		get_default_open_anime_type();
	int		get_default_open_anime_time();
	int		get_default_close_anime_type();
	int		get_default_close_anime_time();

	// メッセージパラメータの取得／設定
	void	init_window_moji_cnt();
	void	set_window_moji_cnt(C_size moji_cnt);
	C_size	get_window_moji_cnt();

	// その他のパラメータの取得／設定
	int		get_novel_flag()					{ return m_param.novel_mode; }
	int		get_name_disp_mode()				{ return m_param.name_disp_mode; }	// 名前の表示方法（0:名前ウィンドウに表示、1:メッセージの先頭に表示、2:表示しない）
	int		get_name_bracket()					{ return m_param.name_bracket; }	// 名前を括弧でくくる（0=括弧なし、1=【】）
	int		get_over_flow_check_size()			{ return m_param.over_flow_check_size; }

	C_elm_object_list&	button_list()			{ return m_msg_waku.button_list(); }
	C_elm_object_list&	face_list()				{ return m_msg_waku.face_list(); }
	C_elm_object_list&	object_list()			{ return m_msg_waku.object_list(); }

private:

	void	restruct_template();			// テンプレートを再構築
	void	init_work_variable();			// 処理変数を初期化

	// 固定パラメータ
	int						m_mwnd_no;
	S_element				m_def_elm_group_list;

	// 通常パラメータ
	struct	PARAM
	{
		S_tnm_sorter		sorter;					// ソーター
		int					world;					// ワールド
		int					novel_mode;				// ノベルモード

		int					extend_type;			// 0:固定サイズ、1:可変サイズ
		C_point				window_pos;				// ウィンドウ位置
		C_size				window_size;			// ウィンドウサイズ（固定サイズのみ）
		C_point				msg_pos;				// メッセージ位置（固定サイズのみ）
		C_rect				msg_margin;				// メッセージマージン（可変サイズのみ）
		C_size				msg_moji_cnt;			// メッセージの文字数
		int					name_disp_mode;			// 名前表示モード
		int					name_bracket;			// 名前を括弧でくくる（0=括弧なし、1=【】）

		int					name_extend_type;		// 0:固定サイズ、1:可変サイズ
		int					name_window_align;		// 揃え位置
		C_point				name_window_pos;		// ウィンドウ位置
		C_size				name_window_size;		// ウィンドウサイズ（固定サイズのみ）
		C_rect				name_window_rect;		// ウィンドウ矩形
		C_point				name_msg_pos;			// メッセージ位置（固定サイズのみ）
		C_point				name_msg_pos_rep;		// メッセージ位置（固定サイズのみ）
		C_rect				name_msg_margin;		// メッセージマージン（可変サイズのみ）

		int					over_flow_check_size;	// オーバーフローと判断するサイズ

		int					face_hide_name;			// 顔グラを表示したとき名前を隠す

		int					open_anime_type;		// 閉きアニメの種類
		int					open_anime_time;		// 閉きアニメにかける時間
		int					close_anime_type;		// 閉じアニメの種類
		int					close_anime_time;		// 閉じアニメにかける時間
	};
	PARAM				m_param;					// パラメータ
	int					m_time;						// 時刻

	bool				m_msg_block_started_flag;	// メッセージブロックを開始したフラグ
	bool				m_auto_proc_ready_flag;		// 自動処理開始フラグ（自動処理≒メッセージウィンドウを開いてメッセージを表示）
	bool				m_window_appear;			// ウィンドウ出現フラグ（ただし、開きアニメ中は true、閉じアニメ中は false）
	bool				m_name_appear;				// 名前出現フラグ
	bool				m_clear_ready_flag;			// クリア準備フラグ
	int					m_auto_mode_end_moji_cnt;	// ここまで既にオートモードで待った
	int					m_target_msg_no;			// ターゲットのメッセージ番号
	bool				m_slide_msg;				// スライドメッセージ
	int					m_slide_time;				// スライドタイム

	ARRAY<S_tnm_read_flag>	m_read_flag_stock_list;	// 抱えている既読フラグリスト
	int						m_koe_no;				// 割り当てられた声番号
	bool					m_koe_play_flag;		// 割り当てられた声を再生するフラグ
	bool					m_koe_no_auto_mode_flag;// オートモードの判定に声を使わない（セーブしないフラグ）

	// ウィンドウアニメ
	int					m_open_anime_type;			// 実行中のウィンドウ開きアニメタイプ
	int					m_open_anime_time;			// 実行中のウィンドウ開きアニメ時間
	int					m_open_anime_start_time;	// 実行中のウィンドウ開きアニメ開始時刻
	int					m_close_anime_type;			// 実行中のウィンドウ閉じアニメタイプ
	int					m_close_anime_time;			// 実行中のウィンドウ閉じアニメ時間
	int					m_close_anime_start_time;	// 実行中のウィンドウ閉じアニメ開始時刻
	int					m_slide_anime_type;
	int					m_slide_anime_time;
	int					m_slide_anime_start_time;
	int					m_slide_pixel;

	C_tnm_class_array<C_elm_mwnd_msg>	m_msg_list;		// メッセージ
	C_elm_mwnd_waku						m_msg_waku;		// メッセージ枠
	C_elm_mwnd_name						m_name;			// 名前
	C_elm_mwnd_waku						m_name_waku;	// 名前枠
	C_elm_mwnd_select					m_sel;			// 選択肢
};

// ****************************************************************
// メッセージウィンドウリスト
// ================================================================
class C_elm_mwnd_list : public C_elm_list_ex<C_elm_mwnd>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent, S_element elm_group_list);	// 初期化
	void	finish();

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end, bool restruct_flag);		// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	S_element	m_elm_group_list;
};

