#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_gan.h"
#include	"data/tnm_sprite.h"

#include	"element/elm_int_event.h"
#include	"element/elm_frame_action.h"

#include	"engine/eng_graphics.h"

const int TNM_OBJECT_FLAGS_NO_EVENT = 0x0001;

const int TNM_OBJECT_EMOTE_TIMELINE_NUM = 8;

struct	S_tnm_btn_action_template_pat;
class	C_elm_object;
class	C_elm_object_list;
class	C_elm_group;
class	C_elm_group_list;
class	C_elm_world;
class	C_elm_world_list;

class	IEmotePlayer;

// ****************************************************************
// オブジェクトの種類
// ================================================================
enum TNM_OBJECT_TYPE
{
	TNM_OBJECT_TYPE_NONE = 0,
	TNM_OBJECT_TYPE_RECT = 1,
	TNM_OBJECT_TYPE_PCT = 2,
	TNM_OBJECT_TYPE_STRING = 3,
	TNM_OBJECT_TYPE_WEATHER = 4,
	TNM_OBJECT_TYPE_NUMBER = 5,
	TNM_OBJECT_TYPE_MESH = 6,
	TNM_OBJECT_TYPE_BILLBOARD = 7,
	TNM_OBJECT_TYPE_SAVE_THUMB = 8,
	TNM_OBJECT_TYPE_MOVIE = 9,
	TNM_OBJECT_TYPE_CAPTURE = 10,
	TNM_OBJECT_TYPE_THUMB = 11,
	TNM_OBJECT_TYPE_EMOTE = 12,
	TNM_OBJECT_TYPE_MAX = 13,
};

// ****************************************************************
// オブジェクトの種類 → 文字列
// ================================================================
inline TSTR tnm_object_type_get_str(int type)
{
	switch (type)	{
		case TNM_OBJECT_TYPE_NONE:			return _T("none");
		case TNM_OBJECT_TYPE_RECT:			return _T("矩形");
		case TNM_OBJECT_TYPE_PCT:			return _T("ﾋﾟｸﾁｬ");
		case TNM_OBJECT_TYPE_STRING:		return _T("文字列");
		case TNM_OBJECT_TYPE_WEATHER:		return _T("環境");
		case TNM_OBJECT_TYPE_NUMBER:		return _T("数値");
		case TNM_OBJECT_TYPE_MESH:			return _T("ﾒｯｼｭ");
		case TNM_OBJECT_TYPE_BILLBOARD:		return _T("ﾋﾞﾙﾎﾞｰﾄﾞ");
		case TNM_OBJECT_TYPE_SAVE_THUMB:	return _T("ｾｰﾌﾞｻﾑﾈｲﾙ");
		case TNM_OBJECT_TYPE_MOVIE:			return _T("ﾑｰﾋﾞｰ");
		case TNM_OBJECT_TYPE_CAPTURE:		return _T("ｷｬﾌﾟﾁｬｰ");
		case TNM_OBJECT_TYPE_THUMB:			return _T("ｻﾑﾈｲﾙ");
		case TNM_OBJECT_TYPE_EMOTE:			return _T("emote");
		default:							return _T("???");
	}
}

// ****************************************************************
// 環境オブジェクトの種類
// ================================================================
const int	TNM_OBJECT_WEATHER_TYPE_NONE = 0;
const int	TNM_OBJECT_WEATHER_TYPE_A    = 1;
const int	TNM_OBJECT_WEATHER_TYPE_B    = 2;
const int	TNM_OBJECT_WEATHER_TYPE_MAX  = 3;

// ****************************************************************
// 子供のソートタイプ
// ================================================================
const int	TNM_OBJECT_SORT_TYPE_DEFAULT = 0;
const int	TNM_OBJECT_SORT_TYPE_NONE = 1;
const int	TNM_OBJECT_SORT_TYPE_TEXTURE = 2;
const int	TNM_OBJECT_SORT_TYPE_X = 3;
const int	TNM_OBJECT_SORT_TYPE_X_REV = 4;
const int	TNM_OBJECT_SORT_TYPE_Y = 5;
const int	TNM_OBJECT_SORT_TYPE_Y_REV = 6;
const int	TNM_OBJECT_SORT_TYPE_Z = 7;
const int	TNM_OBJECT_SORT_TYPE_Z_REV = 8;
const int	TNM_OBJECT_SORT_TYPE_MAX = 9;

// ****************************************************************
// オブジェクトの固定パラメータ
// ================================================================
struct C_elm_object_def_param
{
	bool		use_flag;				// オブジェクトを使うフラグ
	bool		use_group_flag;			// グループを使うフラグ
	bool		set_cg_table;			// ＣＧテーブルを使うフラグ
	bool		space_hide_flag;		// スペースキーで消えるフラグ
	bool		excall_button_enable;	// ＥＸコール中もボタンが使える
	int			object_disp_no;			// オブジェクトを表示するの設定番号
	S_element	elm_group_list;			// グループリストのエレメント
	S_element	elm_world_list;			// ワールドリストのエレメント
};

// ****************************************************************
// フィルタオブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_filter
{
	C_rect	rect;
	C_argb	color;
};

// ****************************************************************
// 文字列オブジェクトの文字パラメータ
// ================================================================
struct C_elm_object_param_moji
{
	int		type;
	int		code;
	C_point	pos;
	int		size;
	C_argb	color;
};

// ****************************************************************
// 文字列オブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_string
{
	int		moji_size;
	int		moji_space_x;
	int		moji_space_y;
	int		moji_cnt;
	int		moji_color;
	int		shadow_color;
	int		fuchi_color;
	int		shadow_mode;
};

// ****************************************************************
// 数値オブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_number
{
	int		number;			// 数値
	int		keta_max;		// 桁数
	int		disp_zero;		// 先頭の０を表示
	int		disp_sign;		// 先頭の符号を表示
	int		tumeru_sign;	// 先頭の符号を詰める
	int		space_mod;		// 0:間隔=space、1:間隔=カットサイズ+space
	int		space;			// 間隔
};

// ****************************************************************
// 環境オブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_weather
{
	int		type;
	int		cnt;
	int		pat_mode;
	int		pat_no_00;
	int		pat_no_01;
	int		pat_time;
	int		move_time_x;
	int		move_time_y;
	int		sin_time_x;
	int		sin_time_y;
	int		sin_power_x;
	int		sin_power_y;
	int		center_x;
	int		center_y;
	int		center_rotate;
	int		appear_range;
	int		zoom_min;
	int		zoom_max;
	int		scale_x;
	int		scale_y;
	int		active_time;
	bool	real_time_flag;		// 実時間フラグ
};

// ****************************************************************
// 環境オブジェクトの一時パラメータ
// ================================================================
struct C_elm_object_work_param_weather_sub
{
	int		state;				// 0:sleep, 1:appear, 2:active, 3:disappear
	int		state_cur_time;
	int		state_time_len;
	int		move_start_pos_x;
	int		move_start_pos_y;
	int		move_start_distance;
	int		move_start_degree;
	int		move_time_x;		// 1000px 進むのにかかる時間
	int		move_time_y;		// 1000px 進むのにかかる時間
	int		move_cur_time;
	int		sin_time_x;
	int		sin_time_y;
	int		sin_power_x;
	int		sin_power_y;
	int		sin_cur_time;
	int		center_rotate;
	int		zoom_min;
	int		zoom_max;
	int		scale_x;
	int		scale_y;
	int		active_time_len;
	bool	real_time_flag;		// 実時間フラグ
	bool	restruct_flag;		// 再構築フラグ
};

struct C_elm_object_work_param_weather
{
	int		cnt_max;			// 今までの最大個数
	ARRAY<C_elm_object_work_param_weather_sub>	sub;
};

// ****************************************************************
// サムネイルオブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_thumb
{
	int		save_no;
};

// ****************************************************************
// ムービーオブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_movie
{
	bool	loop_flag;			// ループフラグ
	bool	auto_free_flag;		// 自動解放フラグ
	bool	real_time_flag;		// 実時間フラグ
	bool	pause_flag;			// 一時停止フラグ
};

// ****************************************************************
// E-mote オブジェクトのパラメータ
// ================================================================
struct C_elm_object_param_emote_timeline
{
	int		option;
};
struct C_elm_object_param_emote
{
	C_size		size;
	C_elm_object_param_emote_timeline	timeline[TNM_OBJECT_EMOTE_TIMELINE_NUM];
	int			koe_chara_no;
	int			koe_mouth_volume;
	int			rep_x;
	int			rep_y;
};

// ****************************************************************
// オブジェクトのボタンパラメータ
// ================================================================
struct C_elm_object_param_button
{
	int			sys_type;		// システムタイプ
	int			sys_type_opt;	// システムタイプオプション
	int			action_no;		// アクション番号
	int			se_no;			// 効果音番号
	int			button_no;		// ボタン番号
	S_element	elm_group;		// グループ番号
	int			push_keep;		// 押した状態を維持する
	int			state;			// 通常／押している／使えない
	int			mode;			// モード
	int			cut_no;			// カット番号
	int			decided_action_scn_no;	// 決定アクションのシーン番号
	int			decided_action_cmd_no;	// 決定アクションのコマンド番号
	int			decided_action_z_no;	// 決定アクションのＺラベル番号
	int			excall_enable;	// ＥＸコール中も使える
	int			alpha_test;		// 透明な部分は当たらない
};

// ****************************************************************
// オブジェクトの基本パラメータ（ＰＯＤ）
// ================================================================
struct C_elm_object_param_base
{
	// 基本
	int				disp;				// 表示フラグ
	C_elm_int_event	pat_no;				// パターン番号
	S_tnm_sorter	sorter;				// ソーター
	int				world_no;			// ワールド番号
	int				child_sort_type;	// 子供のソートタイプ

	// 頂点関連
	C_elm_int_event	pos_x;			// 位置
	C_elm_int_event	pos_y;			// 位置
	C_elm_int_event	pos_z;			// 位置
	C_elm_int_event	center_x;		// 中心
	C_elm_int_event	center_y;		// 中心
	C_elm_int_event	center_z;		// 中心
	C_elm_int_event	center_rep_x;	// 中心補正
	C_elm_int_event	center_rep_y;	// 中心補正
	C_elm_int_event	center_rep_z;	// 中心補正
	C_elm_int_event	scale_x;		// 拡縮（等倍 = 1000）
	C_elm_int_event	scale_y;		// 拡縮（等倍 = 1000）
	C_elm_int_event	scale_z;		// 拡縮（等倍 = 1000）
	C_elm_int_event	rotate_x;		// 回転（１周 = 3600）
	C_elm_int_event	rotate_y;		// 回転（１周 = 3600）
	C_elm_int_event	rotate_z;		// 回転（１周 = 3600）
	int				dst_clip_use;	// 転送先表示範囲使用
	C_elm_int_event	dst_clip_left;	// 転送先表示範囲
	C_elm_int_event	dst_clip_top;	// 転送先表示範囲
	C_elm_int_event	dst_clip_right;	// 転送先表示範囲
	C_elm_int_event	dst_clip_bottom;// 転送先表示範囲
	int				src_clip_use;	// 転送元表示
	C_elm_int_event	src_clip_left;	// 転送元表示範囲
	C_elm_int_event	src_clip_top;	// 転送元表示範囲
	C_elm_int_event	src_clip_right;	// 転送元表示範囲
	C_elm_int_event	src_clip_bottom;// 転送元表示範囲

	// ピクセル関連
	C_elm_int_event	tr;				// 不透明度
	C_elm_int_event	mono;			// モノクロ
	C_elm_int_event	reverse;		// 反転度
	C_elm_int_event	bright;			// 明度
	C_elm_int_event	dark;			// 暗度
	C_elm_int_event	color_r;		// 色変換（Ｒ）
	C_elm_int_event	color_g;		// 色変換（Ｇ）
	C_elm_int_event	color_b;		// 色変換（Ｂ）
	C_elm_int_event	color_rate;		// 色変換（強度）
	C_elm_int_event	color_add_r;	// 色加算（Ｒ）
	C_elm_int_event	color_add_g;	// 色加算（Ｇ）
	C_elm_int_event	color_add_b;	// 色加算（Ｂ）
	int				mask_no;		// マスク
	int				tone_curve_no;	// トーンカーブ
	int				light_no;		// ライト
	int				use_fog;		// フォグ

	// 合成関連
	int				culling;		// カリング
	int				alpha_test;		// アルファテスト
	int				alpha_blend;	// アルファブレンド
	int				blend_type;		// 合成方法

	// その他のフラグ
	int				flags;
};

// ****************************************************************
// オブジェクトの共通パラメータ
// ================================================================
struct C_elm_object_param
{
	// タイプ別パラメータ（ＰＯＤ）
	TNM_OBJECT_TYPE					type;			// タイプ
	int								wipe_copy;		// ワイプコピー
	int								wipe_erase;		// ワイプ消滅
	int								click_disable;	// クリック禁止
	C_elm_object_param_filter		filter;			// フィルタパラメータ
	C_elm_object_param_string		string;			// 文字列パラメータ
	C_elm_object_param_number		number;			// 数値パラメータ
	C_elm_object_param_weather		weather;		// 環境パラメータ
	C_elm_object_param_thumb		thumb;			// サムネイルパラメータ
	C_elm_object_param_movie		movie;			// ムービーパラメータ
	C_elm_object_param_emote		emote;			// E-mote パラメータ
	C_elm_object_param_button		button;			// ボタンパラメータ
	C_elm_object_param_base			obp;			// 基本パラメータ

	// ＰＯＤでないパラメータ
	C_elm_int_event_list		x_rep_list;				// 修正座標Ｘ
	C_elm_int_event_list		y_rep_list;				// 修正座標Ｙ
	C_elm_int_event_list		z_rep_list;				// 修正座標Ｚ
	C_elm_int_event_list		tr_rep_list;			// 修正不透明度Ｘ
	C_elm_int_list				f;						// 汎用フラグ
	TSTR						file_path;				// 画像パス
	TSTR						str;					// 文字列（文字列オブジェクト）
	TSTR						emote_timeline[TNM_OBJECT_EMOTE_TIMELINE_NUM];		// E-mote タイムライン
	TSTR						button_decided_action_scn_name;	// ボタン決定アクションのシーン名
	TSTR						button_decided_action_cmd_name;	// ボタン決定アクションのコマンド名
	C_elm_frame_action			frame_action;			// フレームアクション
	C_elm_frame_action_list		frame_action_ch;		// フレームアクションチャンネル
	C_tnm_gan					gan;					// ＧＡＮ
};

// ****************************************************************
// オブジェクトリスト
// ================================================================
class C_elm_object_list : public C_elm_list_ex<C_elm_object>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, bool use_ini, bool use_group, bool set_cg_table, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list);	// 初期化
	void	finish();

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end, bool restruct_flag);		// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	bool		m_use_ini;
	bool		m_use_group;
	bool		m_set_cg_table;
	S_element	m_elm_group_list;
	S_element	m_elm_world_list;
};

// ****************************************************************
// オブジェクト
// ================================================================
class C_elm_object : public C_tnm_element
{
public:

	C_elm_object()
#if __USE_EMOTE
		: m_emote_player(NULL)
#endif
	{}

	// エレメントの基本処理
	void	init(S_element element, CTSTR& name, bool use_flag, bool use_group_flag, bool set_cg_table, bool space_hide_flag, int object_disp_no, C_tnm_element* p_parent, S_element elm_group_list, S_element elm_world_list);	// 初期化
	void	reinit(bool finish_free_flag);					// 再初期化
	void	free();											// 解放（初期化でない点に注意）
	void	finish();										// 処理を終了する
	void	save(C_tnm_save_stream& stream);				// セーブ
	void	load(C_tnm_save_stream& stream);				// ロード
	void	copy(const C_elm_object* src, bool free_flag);	// コピー

	bool	is_use()				{	return m_op_def.use_flag;		}
	int		get_object_disp_no()	{	return m_op_def.object_disp_no;	}

	// 初期化と解放処理
	void	init_type(bool free_flag);					// タイプの初期化
	void	init_param(bool end_flag);					// パラメータの初期化
	void	init_button_param();						// ボタンパラメータの初期化
	void	free_type(bool child_flag);					// タイプの解放

	// 構築処理
	bool	create_rect(C_rect rect, C_argb color);		// 矩形オブジェクトの作成
	bool	create_pct(TSTR file_path);					// ピクチャオブジェクトの作成
	bool	create_string(TSTR str);					// 文字列オブジェクトの作成
	bool	create_number(TSTR file_path);				// 数値オブジェクトの作成
	bool	create_weather(TSTR file_path);				// 環境オブジェクトの作成
	bool	create_save_thumb(int save_no);				// セーブサムネイルオブジェクトの作成
	bool	create_thumb(int thumb_no);					// サムネイルオブジェクトの作成
	bool	create_movie(TSTR file_path, bool loop, bool auto_free, bool real_time, bool ready_only);	// ムービーオブジェクトの作成
	bool	create_mesh(TSTR file_path);				// メッシュオブジェクトの作成
	bool	create_billboard(TSTR file_path);			// ビルボードオブジェクトの作成
	bool	create_capture();							// キャプチャーの作成
	bool	create_emote(int width, int height, TSTR file_path);	// E-mote オブジェクトの作成

	// フレーム処理
	void	frame_init();																// フレーム初期化処理
	void	update_time(int past_game_time, int past_real_time, int parent_flags);		// 時間を更新
	bool	frame(const S_tnm_render_param* parent_trp, int order, int parent_flags);	// フレーム処理
	void	do_frame_action();															// フレームアクション処理

	// 描画処理
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// スプライトツリーの取得

	// 当たり判定処理
	void	regist_button(int min_order);		// ボタンを登録
	void	button_event();						// ボタンイベント処理
	bool	is_hit();							// 当たっている？
	bool	is_pushed();						// 押されている？
	bool	is_decided();						// 決定された？
	bool	is_hit_this_frame();				// 当たっている？
	bool	is_pushed_this_frame();				// 押されている？
	bool	is_decided_this_frame();			// 決定された？

	// ソーターの取得／設定
	void			set_sorter(S_tnm_sorter value)	{	m_op.obp.sorter = value;	}
	S_tnm_sorter	get_sorter()					{	return m_op.obp.sorter;		}

	// 基本パラメータの設定
	void	set_wipe_copy(int value)		{ m_op.wipe_copy = value ? 1 : 0; }
	void	set_wipe_erase(int value)		{ m_op.wipe_erase = value ? 1 : 0; }
	void	set_click_disable(int value)	{ m_op.click_disable = value ? 1 : 0; }
	void	set_disp(int value)				{ m_op.obp.disp = value ? 1 : 0; }
	void	set_pat_no(int value);
	void	set_order(int value)			{ m_op.obp.sorter.order = value; }
	void	set_layer(int value)			{ m_op.obp.sorter.layer = value; }
	void	set_world_no(int value)			{ m_op.obp.world_no = value; }
	void	set_pos_x(int value)			{ m_op.obp.pos_x.set_value(value); }
	void	set_pos_y(int value)			{ m_op.obp.pos_y.set_value(value); }
	void	set_pos_z(int value)			{ m_op.obp.pos_z.set_value(value); }
	void	set_center_x(int value)			{ m_op.obp.center_x.set_value(value); }
	void	set_center_y(int value)			{ m_op.obp.center_y.set_value(value); }
	void	set_center_z(int value)			{ m_op.obp.center_z.set_value(value); }
	void	set_center_rep_x(int value)		{ m_op.obp.center_rep_x.set_value(value); }
	void	set_center_rep_y(int value)		{ m_op.obp.center_rep_y.set_value(value); }
	void	set_center_rep_z(int value)		{ m_op.obp.center_rep_z.set_value(value); }
	void	set_scale_x(int value)			{ m_op.obp.scale_x.set_value(value); }
	void	set_scale_y(int value)			{ m_op.obp.scale_y.set_value(value); }
	void	set_scale_z(int value)			{ m_op.obp.scale_z.set_value(value); }
	void	set_rotate_x(int value)			{ m_op.obp.rotate_x.set_value(value); }
	void	set_rotate_y(int value)			{ m_op.obp.rotate_y.set_value(value); }
	void	set_rotate_z(int value)			{ m_op.obp.rotate_z.set_value(value); }
	void	set_clip_use(int value)			{ m_op.obp.dst_clip_use = value ? 1 : 0; }
	void	set_clip_left(int value)		{ m_op.obp.dst_clip_left.set_value(value); }
	void	set_clip_top(int value)			{ m_op.obp.dst_clip_top.set_value(value); }
	void	set_clip_right(int value)		{ m_op.obp.dst_clip_right.set_value(value); }
	void	set_clip_bottom(int value)		{ m_op.obp.dst_clip_bottom.set_value(value); }
	void	set_src_clip_use(int value)		{ m_op.obp.src_clip_use = value ? 1 : 0; }
	void	set_src_clip_left(int value)	{ m_op.obp.src_clip_left.set_value(value); }
	void	set_src_clip_top(int value)		{ m_op.obp.src_clip_top.set_value(value); }
	void	set_src_clip_right(int value)	{ m_op.obp.src_clip_right.set_value(value); }
	void	set_src_clip_bottom(int value)	{ m_op.obp.src_clip_bottom.set_value(value); }
	void	set_tr(int value)				{ m_op.obp.tr.set_value(limit(0, value, 255)); }
	void	set_mono(int value)				{ m_op.obp.mono.set_value(limit(0, value, 255)); }
	void	set_reverse(int value)			{ m_op.obp.reverse.set_value(limit(0, value, 255)); }
	void	set_bright(int value)			{ m_op.obp.bright.set_value(limit(0, value, 255)); }
	void	set_dark(int value)				{ m_op.obp.dark.set_value(limit(0, value, 255)); }
	void	set_color_r(int value)			{ m_op.obp.color_r.set_value(limit(0, value, 255)); } 
	void	set_color_g(int value)			{ m_op.obp.color_g.set_value(limit(0, value, 255)); }
	void	set_color_b(int value)			{ m_op.obp.color_b.set_value(limit(0, value, 255)); }
	void	set_color_rate(int value)		{ m_op.obp.color_rate.set_value(limit(0, value, 255)); }
	void	set_color_add_r(int value)		{ m_op.obp.color_add_r.set_value(limit(0, value, 255)); }
	void	set_color_add_g(int value)		{ m_op.obp.color_add_g.set_value(limit(0, value, 255)); }
	void	set_color_add_b(int value)		{ m_op.obp.color_add_b.set_value(limit(0, value, 255)); }
	void	set_mask_no(int value)			{ m_op.obp.mask_no = value; }
	void	set_tone_curve_no(int value)	{ m_op.obp.tone_curve_no = value; }
	void	set_culling(int value)			{ m_op.obp.culling = value ? 1 : 0; }
	void	set_alpha_test(int value)		{ m_op.obp.alpha_test = value ? 1 : 0; }
	void	set_alpha_blend(int value)		{ m_op.obp.alpha_blend = value ? 1 : 0; }
	void	set_blend_type(int value)		{ m_op.obp.blend_type = limit(0, value, TNM_BLEND_TYPE_MAX - 1); }
	void	set_light_no(int value)			{ m_op.obp.light_no = value; }
	void	set_fog_use(int value)			{ m_op.obp.use_fog = value ? 1 : 0; }
	void	set_child_sort_type(int value)	{ m_op.obp.child_sort_type = limit(0, value, TNM_OBJECT_SORT_TYPE_MAX - 1); }

	// 基本パラメータの取得
	int		get_type()					{	return m_op.type;								}
	int		get_wipe_copy()				{	return m_op.wipe_copy;							}
	int		get_wipe_erase()			{	return m_op.wipe_erase;							}
	int		get_click_disable()			{	return m_op.click_disable;						}
	TSTR	get_file_path()				{	return m_op.file_path;							}
	int		get_disp()					{	return m_op.obp.disp;							}
	int		get_pat_no()				{	return m_op.obp.pat_no.get_value();				}
	int		get_order()					{	return m_op.obp.sorter.order;					}
	int		get_layer()					{	return m_op.obp.sorter.layer;					}
	int		get_world_no()				{	return m_op.obp.world_no;						}
	int		get_pos_x()					{	return m_op.obp.pos_x.get_value();				}
	int		get_pos_y()					{	return m_op.obp.pos_y.get_value();				}
	int		get_pos_z()					{	return m_op.obp.pos_z.get_value();				}
	int		get_center_x()				{	return m_op.obp.center_x.get_value();			}
	int		get_center_y()				{	return m_op.obp.center_y.get_value();			}
	int		get_center_z()				{	return m_op.obp.center_z.get_value();			}
	int		get_center_rep_x()			{	return m_op.obp.center_rep_x.get_value();		}
	int		get_center_rep_y()			{	return m_op.obp.center_rep_y.get_value();		}
	int		get_center_rep_z()			{	return m_op.obp.center_rep_z.get_value();		}
	int		get_scale_x()				{	return m_op.obp.scale_x.get_value();			}
	int		get_scale_y()				{	return m_op.obp.scale_y.get_value();			}
	int		get_scale_z()				{	return m_op.obp.scale_z.get_value();			}
	int		get_rotate_x()				{	return m_op.obp.rotate_x.get_value();			}
	int		get_rotate_y()				{	return m_op.obp.rotate_y.get_value();			}
	int		get_rotate_z()				{	return m_op.obp.rotate_z.get_value();			}
	int		get_clip_use()				{	return m_op.obp.dst_clip_use;					}
	int		get_clip_left()				{	return m_op.obp.dst_clip_left.get_value();		}
	int		get_clip_top()				{	return m_op.obp.dst_clip_top.get_value();		}
	int		get_clip_right()			{	return m_op.obp.dst_clip_right.get_value();		}
	int		get_clip_bottom()			{	return m_op.obp.dst_clip_bottom.get_value();	}
	int		get_src_clip_use()			{	return m_op.obp.src_clip_use;					}
	int		get_src_clip_left()			{	return m_op.obp.src_clip_left.get_value();		}
	int		get_src_clip_top()			{	return m_op.obp.src_clip_top.get_value();		}
	int		get_src_clip_right()		{	return m_op.obp.src_clip_right.get_value();		}
	int		get_src_clip_bottom()		{	return m_op.obp.src_clip_bottom.get_value();	}
	int		get_tr()					{	return m_op.obp.tr.get_value();					}
	int		get_mono()					{	return m_op.obp.mono.get_value();				}
	int		get_reverse()				{	return m_op.obp.reverse.get_value();			}
	int		get_bright()				{	return m_op.obp.bright.get_value();				}
	int		get_dark()					{	return m_op.obp.dark.get_value();				}
	int		get_color_r()				{	return m_op.obp.color_r.get_value();			}
	int		get_color_g()				{	return m_op.obp.color_g.get_value();			}
	int		get_color_b()				{	return m_op.obp.color_b.get_value();			}
	int		get_color_rate()			{	return m_op.obp.color_rate.get_value();			}
	int		get_color_add_r()			{	return m_op.obp.color_add_r.get_value();		}
	int		get_color_add_g()			{	return m_op.obp.color_add_g.get_value();		}
	int		get_color_add_b()			{	return m_op.obp.color_add_b.get_value();		}
	int		get_mask_no()				{	return m_op.obp.mask_no;						}
	int		get_tone_curve_no()			{	return m_op.obp.tone_curve_no;					}
	int		get_culling()				{	return m_op.obp.culling;						}
	int		get_alpha_test()			{	return m_op.obp.alpha_test;						}
	int		get_alpha_blend()			{	return m_op.obp.alpha_blend;					}
	int		get_blend_type()			{	return m_op.obp.blend_type;						}
	int		get_light_no()				{	return m_op.obp.light_no;						}
	int		get_fog_use()				{	return m_op.obp.use_fog;						}
	int		get_child_sort_type()		{	return m_op.obp.child_sort_type;				}

	// 画像から情報を取得
	int		get_pat_cnt();
	int		get_size_x(int cut_no);
	int		get_size_y(int cut_no);
	int		get_size_z(int cut_no);
	bool	get_pixel_color(int x, int y, int cut_no, C_argb* p_color);
	int		get_pixel_color_r(int x, int y, int cut_no);
	int		get_pixel_color_g(int x, int y, int cut_no);
	int		get_pixel_color_b(int x, int y, int cut_no);
	int		get_pixel_color_a(int x, int y, int cut_no);
	bool	check_click_disable(C_point mp, bool child_flag);

	// ヒント
	void	clear_hints()					{	m_op.obp.flags = 0;			}
	void	set_hints(int value)			{	m_op.obp.flags = value;		}
	void	add_hints(int value)			{	m_op.obp.flags |= value;	}
	void	sub_hints(int value)			{	m_op.obp.flags &= ~value;	}
	int		get_hints()						{	return m_op.obp.flags;		}

	// イベントパラメータ
	C_elm_int_event&		get_pat_no_event()			{	return m_op.obp.pat_no;				}
	C_elm_int_event&		get_x_event()				{	return m_op.obp.pos_x;				}
	C_elm_int_event&		get_y_event()				{	return m_op.obp.pos_y;				}
	C_elm_int_event&		get_z_event()				{	return m_op.obp.pos_z;				}
	C_elm_int_event&		get_center_x_event()		{	return m_op.obp.center_x;			}
	C_elm_int_event&		get_center_y_event()		{	return m_op.obp.center_y;			}
	C_elm_int_event&		get_center_z_event()		{	return m_op.obp.center_z;			}
	C_elm_int_event&		get_center_rep_x_event()	{	return m_op.obp.center_rep_x;		}
	C_elm_int_event&		get_center_rep_y_event()	{	return m_op.obp.center_rep_y;		}
	C_elm_int_event&		get_center_rep_z_event()	{	return m_op.obp.center_rep_z;		}
	C_elm_int_event&		get_scale_x_event()			{	return m_op.obp.scale_x;			}
	C_elm_int_event&		get_scale_y_event()			{	return m_op.obp.scale_y;			}
	C_elm_int_event&		get_scale_z_event()			{	return m_op.obp.scale_z;			}
	C_elm_int_event&		get_rotate_x_event()		{	return m_op.obp.rotate_x;			}
	C_elm_int_event&		get_rotate_y_event()		{	return m_op.obp.rotate_y;			}
	C_elm_int_event&		get_rotate_z_event()		{	return m_op.obp.rotate_z;			}
	C_elm_int_event&		get_clip_left_event()		{	return m_op.obp.dst_clip_left;		}
	C_elm_int_event&		get_clip_top_event()		{	return m_op.obp.dst_clip_top;		}
	C_elm_int_event&		get_clip_right_event()		{	return m_op.obp.dst_clip_right;		}
	C_elm_int_event&		get_clip_bottom_event()		{	return m_op.obp.dst_clip_bottom;	}
	C_elm_int_event&		get_src_clip_left_event()	{	return m_op.obp.src_clip_left;		}
	C_elm_int_event&		get_src_clip_top_event()	{	return m_op.obp.src_clip_top;		}
	C_elm_int_event&		get_src_clip_right_event()	{	return m_op.obp.src_clip_right;		}
	C_elm_int_event&		get_src_clip_bottom_event()	{	return m_op.obp.src_clip_bottom;	}
	C_elm_int_event&		get_tr_event()				{	return m_op.obp.tr;					}
	C_elm_int_event&		get_mono_event()			{	return m_op.obp.mono;				}
	C_elm_int_event&		get_reverse_event()			{	return m_op.obp.reverse;			}
	C_elm_int_event&		get_bright_event()			{	return m_op.obp.bright;				}
	C_elm_int_event&		get_dark_event()			{	return m_op.obp.dark;				}
	C_elm_int_event&		get_color_r_event()			{	return m_op.obp.color_r;			}
	C_elm_int_event&		get_color_g_event()			{	return m_op.obp.color_g;			}
	C_elm_int_event&		get_color_b_event()			{	return m_op.obp.color_b;			}
	C_elm_int_event&		get_color_rate_event()		{	return m_op.obp.color_rate;			}
	C_elm_int_event&		get_color_add_r_event()		{	return m_op.obp.color_add_r;		}
	C_elm_int_event&		get_color_add_g_event()		{	return m_op.obp.color_add_g;		}
	C_elm_int_event&		get_color_add_b_event()		{	return m_op.obp.color_add_b;		}

	// 補正イベントパラメータ
	C_elm_int_event_list&	x_rep_event_list()		{	return m_op.x_rep_list;		}
	C_elm_int_event_list&	y_rep_event_list()		{	return m_op.y_rep_list;		}
	C_elm_int_event_list&	z_rep_event_list()		{	return m_op.z_rep_list;		}
	C_elm_int_event_list&	tr_rep_event_list()		{	return m_op.tr_rep_list;	}

	// フラグ
	C_elm_int_list&			F()						{	return m_op.f;				}

	// 全体イベント
	void					end_all_event();	// 全てのイベントを停止
	bool					check_all_event();	// 全てのイベントを判定

	// 矩形パラメータ
	C_rect	get_filter_rect()			{	return m_op.filter.rect;	}
	C_argb	get_filter_color()			{	return m_op.filter.color;	}

	// ファイル
	bool	change_file(CTSTR& file_path);	// ファイルを変更

	// 文字列パラメータ
	bool	set_string(TSTR str);
	bool	set_string_param(int moji_size, int moji_space_x, int moji_space_y, int moji_cnt, int moji_color, int shadow_color, int fuchi_color, int shadow_mode);
	TSTR	get_string()				{	return m_op.str;					}

	// 数値パラメータ
	void	set_number(int value)		{	m_op.number.number = value;			}
	void	set_number_param(int keta, int disp_zero, int disp_sign, int tumeru_sign, int space_mod, int space);
	int		get_number()				{	return m_op.number.number;			}

	// 環境パラメータ
	bool	set_weather_param_type_a(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int move_time_x, int move_time_y, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int scale_x, int scale_y, int active_time, bool real_time);
	bool	set_weather_param_type_b(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int center_x, int center_y, int appear_range, int move_time, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int center_rotate, int zoom_min, int zoom_max, int scale_x, int scale_y, bool real_time);

	// ムービーパラメータ
	int		get_omv_timer()				{	return m_omv_timer;			}
	void	pause_movie()				{	m_op.movie.pause_flag = 1;	}
	void	resume_movie()				{	m_op.movie.pause_flag = 0;	}
	void	seek_movie(int time);
	int		get_movie_seek_time();
	bool	check_movie();
	void	wait_movie(bool key_skip_flag, bool return_value_flag);
	void	end_movie_loop();
	void	set_movie_auto_free(bool value);

	// E-mote パラメータ
	void	emote_play_timeline(int buf, CTSTR& timeline);
	void	emote_play_timeline(int buf, CTSTR& timeline, int option);
	void	emote_stop_timeline(int buf);
	void	emote_stop_timeline_all();
	bool	emote_check_playing();
	void	emote_wait(bool key_skip_flag, bool return_value_flag);
	void	emote_skip();
	void	emote_pass();
	int		emote_find_timeline(CTSTR& timeline);
	void	emote_set_koe_chara_no(int no)	{ m_op.emote.koe_chara_no = no; }
	int		emote_get_koe_chara_no()		{ return m_op.emote.koe_chara_no; }
	void	emote_set_koe_mouth_volume(int v)	{ m_op.emote.koe_mouth_volume = v; }
	int		emote_get_koe_mouth_volume()		{ return m_op.emote.koe_mouth_volume; }
	void	emote_set_rep_pos(int rep_x, int rep_y)		{ m_op.emote.rep_x = rep_x; m_op.emote.rep_y = rep_y; }

	// ボタンパラメータ
	bool	clear_button_decide_action();
	void	set_button_param(int button_no, int group_no, int action_no, int se_no);
	void	set_button_no(int button_no)				{	m_op.button.button_no = button_no;	}
	void	set_button_group(const S_element& element);
	void	set_button_group_no(int group_no);
	void	set_button_action_no(int action_no)			{	m_op.button.action_no = action_no;	}
	void	set_button_se_no(int se_no)					{	m_op.button.se_no = se_no;			}
	void	set_button_state(int state)					{	m_op.button.state = state;			}
	int		get_button_state()							{	return m_op.button.state;			}
	int		get_button_hit_state();
	int		get_button_real_state();
	void	set_button_push_keep(int push_keep)			{	m_op.button.push_keep = push_keep;										}
	int		get_button_push_keep()						{	return m_op.button.push_keep;											}
	void	set_button_alpha_test(int alpha_test)		{ m_op.button.alpha_test = alpha_test; }
	int		get_button_alpha_test()						{ return m_op.button.alpha_test; }
	void	set_button_cut_no(int cut_no)				{ m_op.button.cut_no = cut_no; }
	void	set_button_sys_type(int sys_type, int opt)	{	m_op.button.sys_type = sys_type;	m_op.button.sys_type_opt = opt;		}
	void	set_button_mode(int mode)					{	m_op.button.mode = mode;												}
	int		get_button_no()								{	return m_op.button.button_no;			}
	int		get_button_group_no();
	int		get_button_action_no()						{	return m_op.button.action_no;			}
	int		get_button_se_no()							{	return m_op.button.se_no;				}
	bool	set_button_decided_action(CTSTR& scn_name, CTSTR& cmd_name);
	bool	set_button_decided_action(CTSTR& scn_name, int z_no);
	TSTR	get_button_decided_action_scn_name()		{	return m_op.button_decided_action_scn_name;	}
	TSTR	get_button_decided_action_cmd_name()		{	return m_op.button_decided_action_cmd_name;	}
	int		get_button_decided_action_z_no()			{	return m_op.button.decided_action_z_no;		}

	// フレームアクション
	C_elm_frame_action*			get_frame_action()		{	return &m_op.frame_action;				}
	C_elm_frame_action_list*	get_frame_action_ch()	{	return &m_op.frame_action_ch;			}
	void						set_frame_action(int end_time, int real_flag, CTSTR& scn_name, CTSTR& cmd_name, C_tnm_prop_list& arg_list);

	// ＧＡＮ
	C_tnm_gan&			gan()							{	return m_op.gan;						}

	// 子供
	bool				alloc_child(int cnt);
	int					get_child_cnt()					{	return (int)m_child_list.get_size();	}
	C_elm_object_list&	child_list()					{	return m_child_list;					}

	// グループとアクション
	C_elm_group*						get_group();
	S_tnm_btn_action_template_pat*		get_action(int btn_state);

	// ワールド
	C_elm_world*						get_world();

	// その他
	BSP<C_d3d_album>	get_album() const				{	return m_album;							}
	BSP<C_tnm_sprite>	get_sprite() const				{	return m_sprite;						}
	S_tnm_sorter		get_total_sorter() const		{	return m_trp.sorter;					}

private:

	// パラメータ
	C_elm_object_def_param			m_op_def;
	C_elm_object_param				m_op;
	S_tnm_render_param				m_trp;

	// 子エレメント
	C_elm_object_list				m_child_list;		// 子供

	// 一時パラメータ
	ARRAY<C_elm_object_param_moji>	m_moji_list;		// 文字
	C_elm_object_work_param_weather	m_weather;			// 環境
	BSP<C_d3d_album>				m_album;			// アルバム
	BSP<C_d3d_mesh>					m_mesh;				// メッシュ
	BSP<C_d3d_frame_set>			m_frame_set;		// フレームセット
	BSP<C_tnm_sprite>				m_sprite;			// スプライト
	ARRAY<C_tnm_sprite>				m_sprite_list;		// スプライトリスト
	C_point							m_total_pos;		// 最終位置
	C_tnm_btn						m_button;			// ボタン
	BSP<C_omv_player>				m_omv_player;		// omv プレイヤー
	int								m_omv_timer;		// omv タイマー
	BSP<C_d3d_texture>				m_emote_texture;	// E-mote
	BSP<C_d3d_surface>				m_emote_dpst;		// E-mote

	// 数値オブジェクトのスプライト数
	static const int	m_sprite_cnt_number = 16;

	// E-mote
#if __USE_EMOTE
	IEmotePlayer*					m_emote_player;		// E-mote プレイヤー
#endif

	// 初期化と解放処理
	void	init_filter_param();					// フィルタパラメータの初期化
	void	init_string_param();					// 文字列パラメータの初期化
	void	init_number_param();					// 数値パラメータの初期化
	void	init_weather_param();					// 環境パラメータの初期化
	void	init_frame_action(bool end_flag);		// フレームアクションの初期化

	// 再構築処理
	bool	restruct_type();				// タイプの再構築
	bool	restruct_rect();				// 矩形オブジェクトの再構築
	bool	restruct_pct();					// ピクチャオブジェクトの再構築
	bool	restruct_string();				// 文字列オブジェクトの再構築
	bool	restruct_number();				// 数値オブジェクトの再構築
	bool	restruct_weather();				// 環境オブジェクトの再構築
	bool	restruct_mesh();				// メッシュオブジェクトの再構築
	bool	restruct_billboard();			// ビルボードオブジェクトの再構築
	bool	restruct_save_thumb();			// セーブサムネイルオブジェクトの再構築
	bool	restruct_thumb();				// サムネイルオブジェクトの再構築
	bool	restruct_capture();				// キャプチャーオブジェクトの再構築
	bool	restruct_movie();				// ムービーオブジェクトの再構築
	bool	restruct_emote();				// E-mote オブジェクトの再構築
	bool	restruct_button_action();		// ボタンアクションを再構築
	void	setup_weather_param(int sub_no, int init_state);		// 環境パラメータをセットアップ

	// フレーム処理
	void	update_weather_time(int past_game_time, int past_real_time);	// 環境オブジェクトの時間を更新
	void	string_frame();					// 文字列オブジェクトのフレーム処理
	void	number_frame();					// 数値オブジェクトのフレーム処理
	void	weather_frame();				// 環境オブジェクトのフレーム処理
	void	movie_frame();					// ムービーオブジェクトのフレーム処理

	// 変換処理 obp (object base param) → trp (tnm render param) → rp (render param) 
	void	create_trp();
	void	trp_to_rp(S_tnm_sprite_render_param* rpex, S_d3d_render_param* rp);

	// フレームアクション
	void	do_frame_action_func();		// フレームアクションを実行する
};

