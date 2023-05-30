#pragma		once

#include	"data/tnm_def.h"
#include	"data/tnm_sorter.h"
#include	"element/elm_world.h"

const int	TNM_SPRITE_SORT_TYPE_DEFAULT = 0;
const int	TNM_SPRITE_SORT_TYPE_NONE = 1;
const int	TNM_SPRITE_SORT_TYPE_PAT_NO = 2;

// ****************************************************************
// となま：レンダーパラメータＥＸ
// ================================================================
struct S_tnm_sprite_render_param
{
	S_tnm_sprite_render_param()
	{
		init();
	}
	void init()
	{
		sorter.order = 0;
		sorter.layer = 0;
		p_world = NULL;
		child_sort_type = TNM_SPRITE_SORT_TYPE_DEFAULT;
	}

	S_tnm_sorter	sorter;
	C_elm_world*	p_world;
	int				child_sort_type;
};

// ****************************************************************
// となま：スプライト
// ================================================================
class C_tnm_sprite : public C_d3d_sprite
{
public:
	C_tnm_sprite();
	~C_tnm_sprite();

	// スプライトを解放＆初期化
	void init();
	// スプライトを初期化（解放しない）
	void init_no_release();
	// スプライトを解放
	void release();

	// レンダーパラメータＥＸ
	S_tnm_sprite_render_param		rpex;
};

// ****************************************************************
// となま：レンダーパラメータ
// ================================================================
struct S_tnm_render_param
{
	int				disp;			// 表示フラグ
	int				pat_no;			// パターン番号
	S_tnm_sorter	sorter;			// ソーター
	C_elm_world*	p_world;		// ワールド

	// 頂点関連
	float			pos_x;			// 位置
	float			pos_y;			// 位置
	float			pos_z;			// 位置
	float			center_x;		// 中心
	float			center_y;		// 中心
	float			center_z;		// 中心
	float			center_rep_x;	// 中心補正
	float			center_rep_y;	// 中心補正
	float			center_rep_z;	// 中心補正
	float			scale_x;		// 拡縮
	float			scale_y;		// 拡縮
	float			scale_z;		// 拡縮
	float			rotate_x;		// 回転
	float			rotate_y;		// 回転
	float			rotate_z;		// 回転
	int				dst_clip_use;	// 表示範囲使用
	int				dst_clip_left;	// 表示範囲
	int				dst_clip_top;	// 表示範囲
	int				dst_clip_right;	// 表示範囲
	int				dst_clip_bottom;// 表示範囲
	int				src_clip_use;	// 表示矩形使用
	int				src_clip_left;	// 表示矩形
	int				src_clip_top;	// 表示矩形
	int				src_clip_right;	// 表示矩形
	int				src_clip_bottom;// 表示矩形

	// ピクセル関連
	int				tr;				// 不透明度
	int				mono;			// モノクロ
	int				reverse;		// 反転度
	int				bright;			// 明度
	int				dark;			// 暗度
	int				color_r;		// 色変換（Ｒ）
	int				color_g;		// 色変換（Ｇ）
	int				color_b;		// 色変換（Ｂ）
	int				color_rate;		// 色変換（強度）
	int				color_add_r;	// 色加算（Ｒ）
	int				color_add_g;	// 色加算（Ｇ）
	int				color_add_b;	// 色加算（Ｂ）
	int				mask_no;		// マスク
	int				tone_curve_no;	// トーンカーブ
	int				light_no;		// ライト

	// 合成関連
	bool			culling;		// カリング
	bool			alpha_test;		// アルファテスト
	bool			alpha_blend;	// アルファブレンド
	int				blend_type;		// 合成方法

	// その他
	int				button_state;	// ボタンステート

	// コンストラクタ
	S_tnm_render_param()
	{
		init();
	}

	// 初期化
	void init()
	{
		disp = false;
		pat_no = 0;
		sorter.order = 0;
		sorter.layer = 0;
		p_world = NULL;
		pos_x = 0.0f;
		pos_y = 0.0f;
		pos_z = 0.0f;
		center_x = 0.0f;
		center_y = 0.0f;
		center_z = 0.0f;
		center_rep_x = 0.0f;
		center_rep_y = 0.0f;
		center_rep_z = 0.0f;
		scale_x = 1.0f;
		scale_y = 1.0f;
		scale_z = 1.0f;
		rotate_x = 0.0f;
		rotate_y = 0.0f;
		rotate_z = 0.0f;
		dst_clip_use = 0;
		dst_clip_left = 0;
		dst_clip_top = 0;
		dst_clip_right = 0;
		dst_clip_bottom = 0;
		src_clip_use = 0;
		src_clip_left = 0;
		src_clip_top = 0;
		src_clip_right = 0;
		src_clip_bottom = 0;
		tr = 255;
		mono = 0;
		reverse = 0;
		bright = 0;
		dark = 0;
		color_r = 0;
		color_g = 0;
		color_b = 0;
		color_rate = 0;
		color_add_r = 0;
		color_add_g = 0;
		color_add_b = 0;
		mask_no = TNM_MASK_NONE;
		tone_curve_no = TNM_TONE_CURVE_NONE;
		light_no = TNM_LIGHT_NONE;
		culling = false;
		alpha_test = true;
		alpha_blend = true;
		blend_type = TNM_BLEND_TYPE_NORMAL;
		button_state = TNM_BTN_STATE_NORMAL;
	}
};

