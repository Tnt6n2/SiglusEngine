#pragma		once

#include	"elm_mwnd_sel_item.h"

class	C_tnm_save_stream;

// ****************************************************************
// メッセージウィンドウ：選択肢
// ================================================================
class C_elm_mwnd_select
{
public:

	// 初期化処理
	void	init();								// 初期化
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード
	void	copy(C_elm_mwnd_select* src);		// コピー
	void	set_template(int template_no);		// テンプレートの値を設定

	// フレーム処理
	void	frame_init();									// フレーム初期化処理
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow);	// フレーム処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタンイベント処理
	void	regist_button();				// ボタンを登録
	void	button_event(bool wait_flag);	// ボタンイベント処理

	// 選択肢処理
	bool	is_empty();						// 空かどうか判定
	void	clear_sel();					// 選択肢のクリア
	void	set_def_pos(C_point pos);		// 選択肢の初期位置を設定
	void	add_sel(CTSTR& text, int item_type, int color);		// 選択肢の追加
	void	set_cancel_enable(bool enable);	// キャンセル可能フラグを設定
	void	restruct_item(int extend_type);	// アイテムを再構築
	void	set_disp();						// 表示する
	TSTR	get_msg();						// メッセージの取得
	C_size	get_msg_size();					// メッセージサイズの取得
	bool	check_not_appear_moji();		// まだ出現していない文字が存在するか判定する

private:

	void	init_work_variable();			// 処理変数初期化
	void	restruct_template();			// テンプレートを再構築

	// 文字ステータス
	struct STATE
	{
		C_size	moji_cnt;			// 文字数
		C_point	moji_pos;			// 文字位置
		int		moji_size;			// 文字の大きさ
		C_size	moji_space;			// 文字間のスペース
		int		moji_color;			// 文字色
		int		shadow_color;		// 文字影色
		int		fuchi_color;		// 文字縁色
	};

	int				m_template_no;			// テンプレート番号
	STATE			m_def;					// デフォルトの文字ステータス
	STATE			m_cur;					// 現在の文字ステータス
	TSTR			m_msg;					// メッセージ
	C_size			m_msg_size;				// メッセージサイズ
	int				m_disp_item_cnt;		// 表示したアイテム数
	bool			m_cancel_enable_flag;	// キャンセル可能フラグ

	C_tnm_class_array<C_elm_mwnd_select_item>	m_item_list;			// アイテムリスト
};

