#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_btn.h"

#include	"element/elm_mwnd_moji.h"

class	C_tnm_save_stream;

// ****************************************************************
// メッセージウィンドウ：選択肢：アイテム
// ================================================================
class C_elm_mwnd_select_item
{
public:

	// 初期化処理
	void	init();								// 初期化
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード
	void	copy(C_elm_mwnd_select_item* src);	// コピー

	// フレーム処理
	void	frame_init();									// フレーム初期化処理
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int dark_rep, bool draw_futoku, bool draw_shadow);	// フレーム処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタンイベント処理
	void	regist_button();					// ボタンを登録
	bool	button_event(bool wait_flag);		// ボタンイベント処理
	bool	is_active_hit();					// アクティブなヒットを判定

	// アイテム処理
	void	set_msg(CTSTR& msg, int item_type, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color);	// メッセージを設定
	void	set_hit_rect(C_rect hit_rect);	// 当たり矩形を設定
	void	set_pos(C_point pos);			// 表示座標を設定
	void	set_disp();						// 選択肢を表示する
	TSTR	get_msg();						// メッセージを取得する
	C_size	get_msg_size();					// メッセージサイズを取得する
	int		get_item_type()	{ return m_item_type; }

private:

	int					m_item_type;
	C_point				m_pos;				// 表示座標
	TSTR				m_msg;				// メッセージ
	C_size				m_msg_size;			// メッセージサイズ
	C_rect				m_hit_rect_temp;	// 当たり矩形（仮）
	C_tnm_btn			m_button;			// ボタン

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// 文字リスト
};

