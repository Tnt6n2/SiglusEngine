#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sprite.h"

class	C_elm_mwnd_moji;
struct	S_tnm_render_param;

// ****************************************************************
// メッセージウィンドウ：名前
// ================================================================
class C_elm_mwnd_name
{
public:

	// 初期化処理
	void	init();									// 初期化
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード
	void	copy(C_elm_mwnd_name* src);				// コピー
	void	set_template(int template_no);			// テンプレートの値を設定
	void	restruct_template();					// テンプレートの再構築

	// フレーム処理
	void	frame_init();							// フレーム初期化処理
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow);

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);		// スプライトツリーに設定

	// 名前処理
	bool	is_empty();					// 空かどうか判定
	void	clear_name();				// 名前のクリア
	void	set_name(CTSTR& name);		// 名前の設定
	TSTR	get_name();					// 名前の取得
	void	clear_chara_moji_color();	// キャラクタ文字色を設定
	void	clear_chara_shadow_color();	// キャラクタ影色を設定
	void	clear_chara_fuchi_color();	// キャラクタ縁色を設定
	void	set_chara_moji_color(int color_no);		// キャラクタ文字色を設定
	void	set_chara_shadow_color(int color_no);	// キャラクタ影色を設定
	void	set_chara_fuchi_color(int color_no);	// キャラクタ縁色を設定
	C_rect	get_msg_rect();				// メッセージ矩形の取得

private:

	void	init_work_variable();				// 処理変数初期化
	void	init_work_variable_sub();			// 処理変数初期化ＳＵＢ

	// プライベート処理変数
	struct STATUS
	{
		C_point	moji_pos;			// 文字位置
		int		moji_size;			// 文字の大きさ
		C_size	moji_space;			// 文字間のスペース
		int		moji_cnt;			// 文字数
		int		moji_align;			// 文字揃え位置
		int		moji_color_no;		// 文字色
		int		shadow_color_no;	// 影色
		int		fuchi_color_no;		// 縁色
	};

	int						m_template_no;				// テンプレート番号
	STATUS					m_def;						// デフォルトの文字ステータス
	STATUS					m_cur;						// 現在の文字ステータス
	int						m_chara_moji_color_no;		// キャラクタ文字色
	int						m_chara_shadow_color_no;	// キャラクタ影色
	int						m_chara_fuchi_color_no;		// キャラクタ縁色
	TSTR					m_name;						// 名前
	C_rect					m_msg_rect;					// メッセージ矩形

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// 表示した文字リスト

	C_tnm_sprite			m_spr_moji_root;
	C_tnm_sprite			m_spr_shadow_root;
};

