#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"element/elm_mwnd_moji.h"

class	C_tnm_save_stream;
struct	S_tnm_sorter;
class	C_tnm_sprite;
class	C_elm_group;
class	C_elm_mwnd;
class	C_elm_mwnd_moji;
struct	S_tnm_render_param;

// ****************************************************************
// メッセージウィンドウ：メッセージ：ボタン
// ================================================================
struct C_msg_btn
{
	C_tnm_btn	m_button;
	int			m_btn_no;
	int			m_action_no;
	int			m_se_no;
};

// ****************************************************************
// メッセージウィンドウ：メッセージ
// ================================================================
class C_elm_mwnd_msg
{
public:

	// 初期化処理
	void	init(S_element elm_group_list, C_elm_mwnd* p_parent);			// 初期化
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード
	void	copy(C_elm_mwnd_msg* src);				// コピー

	// デフォルトのパラメータを設定
	void	set_def_param(C_size moji_cnt
		, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color, int ruby_size, int ruby_space
		, int name_moji_size, int name_newline, int name_bracket, C_rect talk_margin);

	// フレーム処理
	void	frame_init();							// フレーム初期化処理
	bool	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow);

	// テクスチャのクリア
	void	clear_texture();

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーに設定

	// ボタン処理
	void	regist_button();

	// メッセージ処理
	bool	is_empty();						// 空かどうか判定
	void	clear_msg();					// メッセージのクリア
	bool	add_msg_check(int check_size, bool new_line_flag);		// メッセージが入るかをチェック
	void	add_msg(CTSTR& msg, TSTR* ret_over_flow_msg);			// メッセージの追加
	void	add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg);	// 名前を追加
	C_size	get_msg_size();					// メッセージサイズの取得
	int		get_msg_height();				// 表示したメッセージの高さを求める
	int		get_moji_cnt();					// 文字数を取得
	int		get_disp_moji_cnt();			// 表示した文字数を取得
	int		get_hide_moji_cnt();			// オーバーフロー前の既に表示していない文字数を取得
	void	disp_one_moji();				// 次の１文字表示
	void	disp_all_moji();				// 全ての文字の表示
	void	set_line_head();				// 行の先頭にする（インデント文字の判定を行う）
	void	new_line_indent();				// 改行（インデントあり）
	void	new_line_no_indent();			// 改行（インデントなし）
	void	new_line_if_not_line_head();	// 改行（行の先頭でない場合のみ）
	void	set_indent(TCHAR moji);			// インデントを設定
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
	void	start_ruby(CTSTR& ruby);		// ルビを開始
	void	end_ruby();						// ルビを終了
	void	start_button(int btn_no, int group_no, int action_no, int se_no);		// ボタンを開始
	void	end_button();					// ボタンを終了
	void	slide_msg();					// スライドメッセージ
	int		get_slide_pixel()				{	return m_def.moji_size + m_def.moji_space.cy;	}
	C_point	get_cur_moji_pos();				// 現在の文字位置を取得
	void	set_cur_moji_pos(C_point pos);	// 現在の文字位置を設定
	bool	check_not_appear_moji();		// まだ表示していない文字が存在するか判定する
	TSTR	get_debug_msg();				// デバッグメッセージの取得

	int		get_last_moji_disp_time()				{	return m_last_moji_disp_time;	}
	void	set_last_moji_disp_time(int time)		{	m_last_moji_disp_time = time;	}
	bool	get_progress_start_flag()				{	return m_progress_start_flag;	}
	void	set_progress_start_flag(bool value)		{	m_progress_start_flag = value;	}
	void	set_cur_msg_type_decided(bool value)	{	m_cur_msg_type_decided = value;	}

	// 文字数を初期化／取得／設定
	void	init_moji_cnt();
	void	set_moji_cnt(C_size moji_cnt);
	
	// ボタン処理
	void			button_event();
	C_elm_group*	get_group();


private:

	void	init_param();										// パラメータ初期化
	void	init_param_sub();									// パラメータ初期化ＳＵＢ
	void	add_msg_sub(CTSTR& msg, TSTR* ret_over_flow_msg);	// メッセージの追加

	// 文字パラメータ
	struct PARAM
	{
		C_size	moji_cnt;			// 文字数
		C_point	moji_pos;			// 文字位置
		C_point	moji_rep_pos;		// 文字補正位置
		int		moji_size;			// 文字の大きさ
		C_size	moji_space;			// 文字間のスペース
		int		moji_color;			// 文字色
		int		shadow_color;		// 文字影色
		int		fuchi_color;		// 文字影色
		int		ruby_size;			// ルビの大きさ
		int		ruby_space;			// ルビ文字間のスペース
		int		name_moji_size;		// 名前の文字サイズ
		int		name_newline;		// 名前の後に改行する
		int		name_bracket;		// 名前に括弧をつける
		C_rect	talk_margin;		// セリフのマージン（ノベルモード）
	};

	// 固定パラメータ
	PARAM					m_def;					// デフォルトの文字ステータス
	C_elm_mwnd*				m_def_parent;			// 親エレメント
	S_element				m_def_elm_group_list;	// グループリストのエレメント

	// 通常パラメータ
	PARAM					m_cur;					// 現在の文字ステータス
	C_size					m_msg_size;				// メッセージサイズ
	TSTR					m_debug_msg;			// デバッグメッセージ
	TSTR					m_ruby;					// ルビ

	// 一時パラメータ
	int						m_chara_moji_color_no;		// キャラクタ文字色
	int						m_chara_shadow_color_no;	// キャラクタ影色
	int						m_chara_fuchi_color_no;		// キャラクタ縁色
	int						m_indent_pos;				// インデント位置
	TCHAR					m_indent_moji;				// インデント文字
	int						m_indent_cnt;				// インデント文字の出現した回数
	int						m_cur_msg_type;				// 最後に表示したメッセージのタイプ（-1:クリア後、0:ト書き、1:名前ありメッセージ）
	C_point					m_ruby_start_pos;			// ルビ開始位置
	int						m_disp_moji_cnt;			// 表示した文字数
	int						m_hide_moji_cnt;			// 隠す文字数（オーバーフロー解除後など）
	int						m_last_moji_disp_time;		// 最後の文字を表示した時刻
	int						m_btn_no;					// ボタン番号
	int						m_btn_group_no;				// ボタングループ番号
	int						m_btn_action_no;			// ボタンアクション番号
	int						m_btn_se_no;				// ボタン効果音番号

	bool					m_cur_msg_type_decided;		// 最後に表示したメッセージのタイプが決定した
	bool					m_line_head_flag;			// 行頭フラグ
	bool					m_ruby_start_ready;			// ルビ開始準備
	bool					m_button_flag;				// ボタン処理中
	bool					m_progress_start_flag;		// メッセージ進み開始フラグ（メッセージウィンドウが開いてメッセージ表示が開始される）

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;				// 表示した文字リスト

	std::map<int, C_msg_btn>			m_msg_btn;					// メッセージボタン
};

