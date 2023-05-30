#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_list.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_object.h"

class	C_tnm_save_stream;

// ****************************************************************
// ボタン選択肢
// ================================================================
struct S_tnm_btn_select_param
{
	C_point	base_pos;
	C_point	rep_pos;
	int		x_align;
	int		y_align;
	int		max_y_cnt;			// 縦に置ける最大数
	int		line_width;			// １行の横幅

	int		moji_cnt;			// 文字数
	C_point	moji_pos;			// 文字位置
	int		moji_size;			// 文字の大きさ
	C_size	moji_space;			// 文字間のスペース
	int		moji_x_align;		// 揃え位置Ｘ
	int		moji_y_align;		// 揃え位置Ｙ
	int		moji_color;			// 文字色
	int		moji_hit_color;		// 文字色
	int		shadow_color;		// 文字影色
	int		fuchi_color;		// 文字縁色

	int		btn_action_no;		// ボタンアクション番号

	int		open_anime_type;	// 開きアニメ
	int		open_anime_time;	// 開きアニメ時間
	int		close_anime_type;	// 閉じアニメ
	int		close_anime_time;	// 閉じアニメ時間
	int		decide_anime_type;	// 決定アニメ
	int		decide_anime_time;	// 決定アニメ時間

	int		sync_type;			// どこまで同期させるか
};

// ****************************************************************
// ボタン選択肢
// ================================================================
class C_elm_btn_select
{
public:

	// 子エレメント
	C_elm_btn_select_item_list	m_item_list;			// アイテムリスト

	// 初期化処理
	void	init(S_element element, CTSTR& name);		// 初期化
	void	reinit();									// 再初期化
	void	finish();									// 終了
	void	save(C_tnm_save_stream& stream);			// セーブ
	void	load(C_tnm_save_stream& stream);			// ロード
	void	copy(C_elm_btn_select* src);				// コピー
	void	set_template(int template_no);				// テンプレートの値を設定

	// フレーム処理
	void	frame_init();											// フレーム初期化処理
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	void	frame(bool draw_futoku, bool draw_shadow);				// フレーム処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタンイベント処理
	void	regist_button();					// ボタンを登録
	void	button_event();						// ボタンイベント処理

	// 選択肢処理
	bool	is_empty();								// 空かどうか判定
	void	set_def_pos(C_point pos);				// 選択肢の初期位置を設定
	void	add_sel(CTSTR& text, int item_type, int color);		// 選択肢の追加
	void	restruct_item();						// アイテムを再構築
	void	set_cancel_enable(bool enable);			// キャンセル可能を設定する
	void	set_capture_flag(bool value);			// キャプチャーフラグを設定する
	void	set_sel_start_call(CTSTR& scn_name, int z_no);	// 選択開始時に呼ばれるコマンドを設定する
	void	do_sel_start_call();							// 選択開始時に呼ばれるコマンドをコールする

	void	start_sel(int sync_type);			// 選択を開始する
	void	close();							// 閉じる
	void	decide(int sel_no);					// 決定する
	void	end_capture();						// キャプチャーを終了する
	void	end_open_anime();					// 開きアニメを終了する
	void	end_close_anime();					// 閉じアニメを終了する
	void	end_decide_anime();					// 決定アニメを終了する
	bool	is_capture_doing();					// キャプチャー中を判定
	bool	is_open_anime_doing();				// 開きアニメ中を判定
	bool	is_close_anime_doing();				// 閉じアニメ中を判定
	bool	is_decide_anime_doing();			// 決定アニメ中を判定
	bool	is_processing();					// 処理中かどうかを判定
	void	regist_read_flag(int scn_no, int flag_no);	// 既読フラグを登録する
	void	set_read_flag();							// 登録した既読フラグを設定する

private:

	void	init_work_variable();			// 処理変数初期化
	void	restruct_template();			// テンプレートを再構築

	int						m_template_no;				// テンプレート番号
	S_tnm_btn_select_param	m_def;						// デフォルトのパラメータ
	S_tnm_btn_select_param	m_cur;						// 現在のパラメータ

	// 作業パラメータ
	int						m_open_anime_type;			// 実行中の開きアニメ
	int						m_open_anime_time;			// 実行中の開きアニメ時間
	int						m_open_anime_cur_time;		// 実行中の開きアニメ開始時刻
	int						m_close_anime_type;			// 実行中の閉じアニメ
	int						m_close_anime_time;			// 実行中の閉じアニメ時間
	int						m_close_anime_cur_time;		// 実行中の閉じアニメ開始時刻
	int						m_decide_anime_type;		// 実行中の決定アニメ
	int						m_decide_anime_time;		// 実行中の決定アニメ時間
	int						m_decide_anime_cur_time;	// 実行中の決定アニメ開始時刻
	int						m_decide_sel_no;			// 決定した選択肢番号
	int						m_sync_type;				// 同期タイプ
	bool					m_appear_flag;				// 出現フラグ
	bool					m_processing_flag_0;		// 処理中フラグ
	bool					m_processing_flag_1;		// 処理中フラグ（閉じアニメを除く）
	bool					m_processing_flag_2;		// 処理中フラグ（さらに決定アニメを除く）
	bool					m_cancel_enable_flag;		// キャンセル可能フラグ
	bool					m_capture_flag;				// キャプチャーを行うフラグ
	TSTR					m_sel_start_call_scn;		// 選択開始時に呼ばれるシーン
	int						m_sel_start_call_z_no;		// 選択開始時に呼ばれるＺ番号
	S_tnm_read_flag			m_read_flag;				// 既読フラグ

	bool					m_capture_now_flag;			// 今回キャプチャーを行うフラグ
};

