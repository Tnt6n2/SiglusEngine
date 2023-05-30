#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_sprite.h"
#include	"element/elm_object.h"

class	C_tnm_save_stream;
class	C_elm_mwnd_moji;

// ****************************************************************
// ボタン選択肢アイテム：パラメータ
// ================================================================
struct S_tnm_btn_select_item_param
{
	int		item_type;
	int		moji_color;
};

// ****************************************************************
// ボタン選択肢アイテム
// ================================================================
class C_elm_btn_select_item : public C_tnm_element
{
public:

	// 子エレメント
	C_elm_object_list		m_object_list;		// オブジェクト

	// 初期化処理
	void	init(S_element element, CTSTR& name);	// 初期化
	void	reinit(bool restruct_flag);				// 再初期化
	void	finish();								// 終了
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード
	void	copy(C_elm_btn_select_item* src);		// コピー
	void	set_template(int template_no);			// テンプレートの値を設定

	// フレーム処理
	void	frame_init();											// フレーム初期化処理
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	void	frame(C_point window_pos, const S_tnm_render_param* parent_trp, int hit_color, int action_no, bool draw_futoku, bool draw_shadow);	// フレーム処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタンイベント処理
	void	regist_button();					// ボタンを登録
	bool	button_event();						// ボタンイベント処理

	// アイテム処理
	void	set_msg(CTSTR& msg, int item_type, int moji_size, C_point moji_pos, C_size moji_space, int x_align, int y_align, int moji_color, int shadow_color, int fuchi_color);	// メッセージを設定
	TSTR	get_msg()				{ return m_msg; }				// メッセージを取得
	void	set_pos(C_point pos)	{ m_pos = pos; }				// 表示座標を設定
	C_point	get_pos()				{ return m_pos; }				// 表示座標を取得
	void	set_disp();												// 選択肢を表示する
	C_size	get_item_size();										// アイテムサイズを取得する
	int		get_item_type()			{ return m_param.item_type; }	// アイテムの状態を取得する

private:

	void	init_work_variable();			// 処理変数初期化
	void	restruct_template();			// テンプレートを再構築

	int							m_template_no;
	TSTR						m_base_file;
	TSTR						m_filter_file;
	TSTR						m_msg;				// メッセージ
	S_tnm_btn_select_item_param	m_param;			// パラメータ
	C_point						m_pos;				// 表示座標
	C_size						m_item_size;		// アイテムサイズ
	C_tnm_btn					m_button;			// ボタン

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// 文字リスト

	BSP<C_d3d_album>	m_album_base;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_base;
	C_tnm_sprite		m_spr_filter;
};

// ****************************************************************
// ボタン選択肢アイテムリスト
// ================================================================
class C_elm_btn_select_item_list : public C_elm_list_ex<C_elm_btn_select_item>
{
public:
	void	finish();												// 終了

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end, bool restruct_flag);		// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

