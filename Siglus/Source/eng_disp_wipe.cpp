#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_tool.h"
#include	"element/elm_stage.h"
#include	"element/elm_screen.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_mask_wipe.h"
#include	"../resource.h"

// ****************************************************************
// ワイプパラメータチェック＆コピー
// ================================================================
bool C_tnm_wnd::check_and_copy_for_wipe_param()
{
	if (m_ready_wipe_type != Gp_wipe->m_type
		|| m_ready_wipe_file != Gp_wipe->m_mask_file
		|| memcmp(m_ready_wipe_option, Gp_wipe->m_option, sizeof(int) * TNM_WIPE_OPTION_MAX)
		)
	{
		m_ready_wipe_type = Gp_wipe->m_type;
		m_ready_wipe_file = Gp_wipe->m_mask_file;
		memcpy(m_ready_wipe_option, Gp_wipe->m_option, sizeof(int) * TNM_WIPE_OPTION_MAX);
		return true;
	}

	return false;
}

// ****************************************************************
// マスク作成
// ================================================================
void C_tnm_wnd::make_mask()
{
	m_mask_buffer_dib.reset();

	switch (Gp_wipe->m_type)	{

		case 5:		// ▲ワイプテスト
			tnm_make_mask_texture_for_direction_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 10:	// ４×４のじわだし９パターン
			tnm_make_mask_texture_for_4x4_jiwa9(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 11:	// ４×４のじわだし７パターン
			tnm_make_mask_texture_for_4x4_jiwa7(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 20:	// ４×４の回るように
			tnm_make_mask_texture_for_4x4_turn_around(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 21:	// ４×４の改行すように
			tnm_make_mask_texture_for_4x4_turn_ret(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 22:	// ４×４の折り返すように
			tnm_make_mask_texture_for_4x4_turn_down(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 30:	// ８×８の回るように
			tnm_make_mask_texture_for_8x8_turn_around(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 31:	// ８×８の改行すように
			tnm_make_mask_texture_for_8x8_turn_ret(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 32:	// ８×８の折り返すように
			tnm_make_mask_texture_for_8x8_turn_down(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 40:	// ランダム
			tnm_make_mask_texture_for_random(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 41:	// ランダムライン
			tnm_make_mask_texture_for_random_line(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 42:	// ランダム斜めライン
			tnm_make_mask_texture_for_random_slant_line(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 43:	// ランダムクロス
			tnm_make_mask_texture_for_random_cross(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 44:	// ランダム斜めクロス
			tnm_make_mask_texture_for_random_slant_cross(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1]);
			break;

		case 60:	// １周
			tnm_make_mask_texture_for_around_one(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 61:	// 半周
			tnm_make_mask_texture_for_around_half(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 62:	// 分割周
			tnm_make_mask_texture_for_around_divide(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 63:	// 中心扇
			tnm_make_mask_texture_for_oogi_center(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 64:	// 角扇
			tnm_make_mask_texture_for_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], false);
			break;
		case 65:	// 縁扇
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 0, false);
			break;
		case 66:	// 縁扇２
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 1, false);
			break;
		case 67:	// 角扇（遠）
			tnm_make_mask_texture_for_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], true);
			break;
		case 68:	// 縁扇（遠）
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 0, true);
			break;
		case 69:	// 縁扇２（遠）
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 1, true);
			break;

		case 70:	// 小矩形の１周
			tnm_make_mask_texture_for_srect_around_one(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 71:	// 小矩形の半周
			tnm_make_mask_texture_for_srect_around_half(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 72:	// 小矩形の分割周
			tnm_make_mask_texture_for_srect_around_divide(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 73:	// 小矩形の中心扇
			tnm_make_mask_texture_for_srect_oogi_center(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 74:	// 小矩形の角扇
			tnm_make_mask_texture_for_srect_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], false);
			break;
		case 75:	// 小矩形の縁扇
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 0, false);
			break;
		case 76:	// 小矩形の縁扇２
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 1, false);
			break;
		case 77:	// 小矩形の角扇（遠）
			tnm_make_mask_texture_for_srect_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], true);
			break;
		case 78:	// 小矩形の縁扇（遠）
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 0, true);
			break;
		case 79:	// 小矩形の縁扇２（遠）
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 1, true);
			break;

		case 80:	// 四角形
			tnm_make_mask_texture_for_square(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 81:	// 菱形
			tnm_make_mask_texture_for_rhombus(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 82:	// 十字
			tnm_make_mask_texture_for_jyuuji(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 83:	// テレビ
			tnm_make_mask_texture_for_television(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;

		case 90:	// 小矩形の四角形
			tnm_make_mask_texture_for_srect_square(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 91:	// 小矩形の菱形
			tnm_make_mask_texture_for_srect_rhombus(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 92:	// 小矩形の十字
			tnm_make_mask_texture_for_srect_jyuuji(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 93:	// 小矩形のテレビ
			tnm_make_mask_texture_for_srect_television(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 100:	// 指定方向から
			tnm_make_mask_texture_for_direction(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1]);
			break;
		case 101:	// 指定方向からスライス
			tnm_make_mask_texture_for_direction_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 102:	// 指定方向からブラインド
			tnm_make_mask_texture_for_direction_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 110:	// 指定の斜め方向から
			tnm_make_mask_texture_for_direction_slant(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1]);
			break;
		case 111:	// 指定方向からブロックブラインド
			tnm_make_mask_texture_for_direction_block_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 120:	// 両方向から
			tnm_make_mask_texture_for_bothdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 121:	// 両方向からスライス
			tnm_make_mask_texture_for_bothdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 122:	// 両方向からブラインド
			tnm_make_mask_texture_for_bothdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 130:	// 両方向からストライプ
			tnm_make_mask_texture_for_bothdir_stripe(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 131:	// 両方向からストライプ２
			tnm_make_mask_texture_for_bothdir_stripe2(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 132:	// 両方向からブロックブラインド
			tnm_make_mask_texture_for_bothdir_block_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 140:	// ４方向から
			tnm_make_mask_texture_for_crossdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 141:	// ４方向からスライス
			tnm_make_mask_texture_for_crossdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 142:	// ４方向からブラインド
			tnm_make_mask_texture_for_crossdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;

		case 150:	// 小矩形の４方向から
			tnm_make_mask_texture_for_srect_crossdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 151:	// 小矩形の４方向からスライス
			tnm_make_mask_texture_for_srect_crossdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 152:	// 小矩形の４方向からブラインド
			tnm_make_mask_texture_for_srect_crossdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 900:	// マスクファイルを使う
		case 901:	// マスクファイルを使う
			tnm_make_mask_texture_from_file(&m_mask_buffer_texture, &m_mask_buffer_dib, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_mask_file);
			break;
	}
}

// ****************************************************************
// ワイプ描画
// ================================================================
bool C_tnm_wnd::disp_proc_wipe()
{
	int wipe_prg = Gp_wipe->m_wipe_prg;

	if (false);
	else if (Gp_wipe->m_type == 0)	{
		if (!disp_proc_wipe_for_cross_fade(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 1)	{
		if (!disp_proc_wipe_for_not_wipe_after_wait(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 2)	{
		if (!disp_proc_wipe_for_not_wipe_befor_wait(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 5 ||		// ▲ワイプテスト

		Gp_wipe->m_type == 10 ||
		Gp_wipe->m_type == 11 ||

		Gp_wipe->m_type == 20 ||
		Gp_wipe->m_type == 21 ||
		Gp_wipe->m_type == 22 ||

		Gp_wipe->m_type == 30 ||
		Gp_wipe->m_type == 31 ||
		Gp_wipe->m_type == 32 ||

		Gp_wipe->m_type == 40 ||
		Gp_wipe->m_type == 41 ||
		Gp_wipe->m_type == 42 ||
		Gp_wipe->m_type == 43 ||
		Gp_wipe->m_type == 44 ||

		Gp_wipe->m_type == 60 ||
		Gp_wipe->m_type == 61 ||
		Gp_wipe->m_type == 62 ||
		Gp_wipe->m_type == 63 ||
		Gp_wipe->m_type == 64 ||
		Gp_wipe->m_type == 65 ||
		Gp_wipe->m_type == 66 ||
		Gp_wipe->m_type == 67 ||
		Gp_wipe->m_type == 68 ||
		Gp_wipe->m_type == 69 ||

		Gp_wipe->m_type == 70 ||
		Gp_wipe->m_type == 71 ||
		Gp_wipe->m_type == 72 ||
		Gp_wipe->m_type == 73 ||
		Gp_wipe->m_type == 74 ||
		Gp_wipe->m_type == 75 ||
		Gp_wipe->m_type == 76 ||
		Gp_wipe->m_type == 77 ||
		Gp_wipe->m_type == 78 ||
		Gp_wipe->m_type == 79 ||

		Gp_wipe->m_type == 80 ||
		Gp_wipe->m_type == 81 ||
		Gp_wipe->m_type == 82 ||
		Gp_wipe->m_type == 83 ||

		Gp_wipe->m_type == 90 ||
		Gp_wipe->m_type == 91 ||
		Gp_wipe->m_type == 92 ||
		Gp_wipe->m_type == 93 ||

		Gp_wipe->m_type == 100 ||
		Gp_wipe->m_type == 101 ||
		Gp_wipe->m_type == 102 ||

		Gp_wipe->m_type == 110 ||
		Gp_wipe->m_type == 111 ||

		Gp_wipe->m_type == 120 ||
		Gp_wipe->m_type == 121 ||
		Gp_wipe->m_type == 122 ||

		Gp_wipe->m_type == 130 ||
		Gp_wipe->m_type == 131 ||
		Gp_wipe->m_type == 132 ||

		Gp_wipe->m_type == 140 ||
		Gp_wipe->m_type == 141 ||
		Gp_wipe->m_type == 142 ||

		Gp_wipe->m_type == 150 ||
		Gp_wipe->m_type == 151 ||
		Gp_wipe->m_type == 152 ||

		Gp_wipe->m_type == 900 ||
		Gp_wipe->m_type == 901 ||

		false
	)	{
		if (!disp_proc_wipe_for_mask(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 50)	{
		if (!disp_proc_wipe_for_shimi(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 200)	{
		if (!disp_proc_wipe_for_move(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 210 ||
		Gp_wipe->m_type == 211 ||
		Gp_wipe->m_type == 212 ||
		Gp_wipe->m_type == 213 ||
		Gp_wipe->m_type == 214 ||
		Gp_wipe->m_type == 215 ||
		false
	)	{
		if (!disp_proc_wipe_for_scale(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 220)	{
		if (!disp_proc_wipe_for_cross_raster(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 221)	{
		if (!disp_proc_wipe_for_raster(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 230 ||
		Gp_wipe->m_type == 231 ||
		false
	)	{
		if (!disp_proc_wipe_for_mosaic(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 240 ||
		Gp_wipe->m_type == 241 ||
		Gp_wipe->m_type == 242 ||
		Gp_wipe->m_type == 243 ||
		false
	)	{
		if (!disp_proc_wipe_for_explosion_blur(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 300 || Gp_wipe->m_type == 301)	{
		if (!disp_proc_wipe_for_move(wipe_prg))
			return false;
	}

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプなし（表示してから待つ）
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_not_wipe_after_wait(int wipe_prg)
{
	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントを描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントを描画する
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：ワイプなし（待ってから表示）
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_not_wipe_befor_wait(int wipe_prg)
{
	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：クロスフェード
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_cross_fade(int wipe_prg)
{
	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントを描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ワイプバッファを準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// 最背面を描画する
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ワイプバッファを描画する
	{
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ワイプスプライトの追加設定
		wipe_sprite.rp.tr = linear_limit(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 255);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：マスク
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mask(int wipe_prg)
{
	// 初回のみマスクを作成する
	if (check_and_copy_for_wipe_param())
		make_mask();

	// ステージ取得
	bool is_front = (Gp_wipe->m_type == 901 && Gp_wipe->m_option[0] != 0) ? true : false;

	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプバッファを準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプバッファを描画する
	{
		// マスクワイプ用のスプライトの初期化
		C_d3d_sprite mask_wipe_sprite;
		mask_wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&mask_wipe_sprite);
		mask_wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_mask_wipe);
		mask_wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// マスクワイプ用のスプライトの追加設定
		disp_proc_wipe_for_mask_func(&mask_wipe_sprite, wipe_prg);

		// マスクワイプ用のスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_mask_wipe->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (mask_wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_mask_wipe->unlock();

			// レンダーターゲットをゲームバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// マスクワイプ用のスプライトを描画する
			if (!disp_proc_sprite(&mask_wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_mask_wipe->unlock();
		}
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：マスクＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_mask_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	switch (Gp_wipe->m_type)	{
		case 901:
			if (m_mask_buffer_dib)	{
				S_mask_buffer buf;
				BYTE *dp;
				int ex1, ey1, ex2, ey2;
				int buf_w = Gp_global->game_screen_size.cx;
				int buf_h = Gp_global->game_screen_size.cy;
				if (Gp_wipe->m_option[3] > 0)	{
					int alpha = (Gp_wipe->m_option[3] >= 64) ? 64 : Gp_wipe->m_option[3];
					buf_w = Gp_global->game_screen_size.cx / alpha;
					buf_h = Gp_global->game_screen_size.cy / alpha;
				}
				if (tnm_alloc_mask_buffer(&buf, buf_w, buf_h, &dp, &ex1, &ey1, &ex2, &ey2))	{
					int dib_w = m_mask_buffer_dib->get_width();
					int dib_h = m_mask_buffer_dib->get_height();
					double max = sqrt((double)Gp_wipe->m_option[1] / 1000);
					double now;
					if (Gp_wipe->m_option[0] == 0)	{
						now = linear_limit<double, double>(wipe_prg, 0, (double)0.0, TNM_WIPE_PRG_MAX, (double)max);
					}
					else	{
						now = linear_limit<double, double>(wipe_prg, 0, (double)max, TNM_WIPE_PRG_MAX, (double)0.0);
					}
					double scale = now * now;
					int rotate = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 3600 * Gp_wipe->m_option[2]);
					tnm_draw_32bit_mask_rotate_scale(
						dp, buf.w, buf.h,
						m_mask_buffer_dib->get_ptr(), dib_w, dib_h,
						(dib_w / 2 - 1), (dib_h / 2 - 1),
						(buf.w / 2 - 1), (buf.h / 2 - 1),
						rotate, scale, scale, false
					);
					if (buf.w == Gp_global->game_screen_size.cx && buf.h == Gp_global->game_screen_size.cy)	{
						tnm_make_mask_texture(&m_mask_buffer_texture, &buf);
					}
					else	{
						if (Gp_wipe->m_option[4] == 0)	{
							tnm_make_mask_texture_with_duplicate(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, &buf);
						}
						else	{
							S_mask_buffer buf2;
							tnm_make_mask_texture_for_gap(&buf2, buf.w, buf.h, &buf);
							tnm_make_mask_texture_with_duplicate(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, &buf2);
						}
					}
				}
			}
			p_sprite->set_effect_constant_f(0, 1.0, 1.0, 1.0, 0.5);
			break;
		default:
			{
				float fade_float;
				float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
				switch (Gp_wipe->m_option[0])	{
					case 0:		fade_float = 0.0f;				break;
					case 1:		fade_float = 1.0f - 1.0f / 2;	break;
					case 2:		fade_float = 1.0f - 1.0f / 4;	break;
					case 3:		fade_float = 1.0f - 1.0f / 8;	break;
					case 4:		fade_float = 1.0f - 1.0f / 16;	break;
					case 5:		fade_float = 1.0f - 1.0f / 32;	break;
					case 6:		fade_float = 1.0f - 1.0f / 64;	break;
					case 7:		fade_float = 1.0f - 1.0f / 128;	break;
					case 8:		fade_float = 1.0f;				break;
					default:	fade_float = 1.0f;				break;
				}
				p_sprite->set_effect_constant_f(0, fade_float, fade_float, fade_float, wipe_prg_float);
			}
			break;
	}

	// 理由はわからないが上に持って行くとバグる
	p_sprite->set_texture(m_mask_buffer_texture, TNM_TEXTURE_STAGE_MASK);
	p_sprite->set_effect_technique(m_view.m_default_effect.m_mask_technique);
	p_sprite->rp.size_fit_to_texture = 0;
}

// ****************************************************************
// ワイプ描画：シミ
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_shimi(int wipe_prg)
{
	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ワイプバッファを準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
	// ここからRTを変更し、ワイプバッファをスプライトとして描画する

	// ワイプバッファを描画する
	{
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// しみワイプ用のスプライトの追加設定
		disp_proc_wipe_for_shimi_func(&wipe_sprite, wipe_prg);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：シミＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_shimi_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	//switch (Gp_wipe->m_type)	{
	//	default:
			{
				float fade_float;
				float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
				switch (Gp_wipe->m_option[0])	{
					case 0:		fade_float = 0.0f;				break;
					case 1:		fade_float = 1.0f - 1.0f / 2;	break;
					case 2:		fade_float = 1.0f - 1.0f / 4;	break;
					case 3:		fade_float = 1.0f - 1.0f / 8;	break;
					case 4:		fade_float = 1.0f - 1.0f / 16;	break;
					case 5:		fade_float = 1.0f - 1.0f / 32;	break;
					case 6:		fade_float = 1.0f - 1.0f / 64;	break;
					case 7:		fade_float = 1.0f - 1.0f / 128;	break;
					case 8:		fade_float = 1.0f;				break;
					default:	fade_float = 1.0f;				break;
				}
				p_sprite->set_effect_constant_f(0, fade_float, fade_float, fade_float, wipe_prg_float);
			}
	//		break;
	//}

	// 暗→明
	if(Gp_wipe->m_option[1] == 0)
		p_sprite->set_effect_technique(m_view.m_default_effect.m_shimi_technique);
	// 明→暗
	else// if(Gp_wipe->m_option[1] == 1)
		p_sprite->set_effect_technique(m_view.m_default_effect.m_shimi_inv_technique);
}


// ****************************************************************
// ワイプ描画：クロスラスタ
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_cross_raster(int wipe_prg)
{
	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---
	
	// ワイプバッファ１を準備する
	if (!disp_proc_ready_wipe_buffer_1())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ワイプバッファ０を準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
	// ここからRTを変更し、ワイプバッファをスプライトとして描画する

	// ワイプバッファを描画する
	{
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_texture(m_wipe_back_buffer_texture_1, TNM_TEXTURE_STAGE_MASK);		// ワイプバッファ１も設定する
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ラスタワイプ用のスプライトの追加設定
		disp_proc_wipe_for_raster_func(&wipe_sprite, wipe_prg);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：ラスタ共通
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_raster(int wipe_prg)
{
	// ステージ取得
	// シングルラスタの場合
	bool is_front = (Gp_wipe->m_type == 221 && Gp_wipe->m_option[4] != 0) ? true : false;

	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプバッファを準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
	// ここからRTを変更し、ワイプバッファをスプライトとして描画する

	// ワイプバッファを描画する
	{
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ラスタワイプ用のスプライトの追加設定
		disp_proc_wipe_for_raster_func(&wipe_sprite, wipe_prg);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}


// ****************************************************************
// ワイプ描画：ラスタ共通ＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_raster_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	p_sprite->set_effect_constant_f(0, wipe_prg_float, wipe_prg_float, wipe_prg_float, wipe_prg_float);

	switch(Gp_wipe->m_type) {
		// クロスラスタ
		case 220:
			// Gp_wipe->m_option[1]ピクセルごとにずれる,基本的な波数は3,補正は無視
			if(Gp_wipe->m_option[1] <= 0)
				Gp_wipe->m_option[1] = 1;

			// 縦
			if(Gp_wipe->m_option[0] == 0) {
				p_sprite->set_effect_technique(m_view.m_default_effect.m_cross_raster_vertical_technique);
				//p_sprite->set_effect_constant_f((float)Gp_global->game_screen_size.cy / Gp_wipe->m_option[1], Gp_wipe->m_option[2], wipe_prg_float, wipe_prg_float);
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.y / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wipe_prg_float);
			}
			// 横
			else// if(Gp_wipe->m_option[0] == 1) {
			{
				p_sprite->set_effect_technique(m_view.m_default_effect.m_cross_raster_horizontal_technique);
				//p_sprite->set_effect_constant_f((float)Gp_global->game_screen_size.cx / Gp_wipe->m_option[1], Gp_wipe->m_option[2], wipe_prg_float, wipe_prg_float);
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.x / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wipe_prg_float);
			}
			break;
		// 通常ラスタ
		case 221:
			// Gp_wipe->m_option[1]ピクセルごとにずれる,基本的な波数は3,補正は無視
			if(Gp_wipe->m_option[1] <= 0)
				Gp_wipe->m_option[1] = 1;

			// 縦
			if(Gp_wipe->m_option[0] == 0) {
				p_sprite->set_effect_technique(m_view.m_default_effect.m_raster_vertical_technique);
				float wpf = (Gp_wipe->m_option[4]) ? 1.0f - wipe_prg_float : wipe_prg_float;
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.y / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wpf);
			}
			// 横
			else// if(Gp_wipe->m_option[0] == 1) {
			{
				p_sprite->set_effect_technique(m_view.m_default_effect.m_raster_horizontal_technique);
				float wpf = (Gp_wipe->m_option[4]) ? 1.0f - wipe_prg_float : wipe_prg_float;
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.x / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wpf);
			}
			break;
		default:
			break;
	}
}

// ****************************************************************
// ワイプ描画：移動系
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move(int wipe_prg)
{
	// 初回のみマスクを作成する
	if (check_and_copy_for_wipe_param())
		tnm_make_mask_texture_for_direction(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, 0);

	// ステージ取得
	bool is_front = disp_proc_wipe_for_move_get_stage(wipe_prg);

	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// 下位のオーダーも含めたワイプ
	if (Gp_wipe->m_with_low_order)	{

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプバッファをゲームバッファに描画する
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? true : false))
			return false;

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ワイプバッファをゲームバッファに描画する
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? false : true))
			return false;
	}
	// 指定したオーダーのみのワイプ
	else	{

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプするオーダーのネクストをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプバッファをゲームバッファに描画する
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? true : false))
			return false;

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ワイプバッファをゲームバッファに描画する
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? false : true))
			return false;
	}

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：移動系ステージ取得
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move_get_stage(int wipe_prg)
{
	switch (Gp_wipe->m_type)	{
		case 200:		// スクロール
			if (Gp_wipe->m_option[1] == 0)			{	return true;	}
			break;
		case 301:		// ページめくり
			if (wipe_prg < TNM_WIPE_PRG_MAX / 2)	{	return true;	}
			break;
	}
	return false;
}

// ****************************************************************
// ワイプ描画：移動系スイッチ
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move_switch(int wipe_prg, bool is_front)
{
	if (Gp_wipe->m_type == 200)	{

		// ワイプスプライト
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// 分岐
		switch (Gp_wipe->m_type)	{
			case 200:		// スクロールと伸縮変形
				disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(&wipe_sprite, wipe_prg, is_front);
				break;
		}

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// スプライトを描画する
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}
	else if (Gp_wipe->m_type == 300)	{

		float sx = (float)Gp_global->game_screen_size.cx;
		float sy = (float)Gp_global->game_screen_size.cy;

		// ワイプスプライト
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.rp.d2_rect = false;			// ２Ｄ矩形を外す
		wipe_sprite.rp.culling = true;			// カリングする
		wipe_sprite.rp.alpha_test = true;		// アルファテストする
		wipe_sprite.rp.alpha_blend = true;		// アルファブレンドする
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d3_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		bool reverse_flag = Gp_wipe->m_option[0] ? true : false;
		int range_type = Gp_wipe->m_option[2];

		float s_rad = 0.0f, h_rad = 0.0f, e_rad = 0.0f;	// start, half, end
		if (is_front)	{
			if (reverse_flag)	{	s_rad = D3DX_PI;	e_rad = 0.0f;			}
			else				{	s_rad = D3DX_PI;	e_rad = D3DX_PI * 2;	}
		}
		else	{
			if (reverse_flag)	{	s_rad = 0.0f;		e_rad = - D3DX_PI;		}
			else				{	s_rad = 0.0f;		e_rad = D3DX_PI;		}
		}
		h_rad = (s_rad + e_rad) / 2;

		switch (range_type)	{
			case 1:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, h_rad, TNM_WIPE_PRG_MAX, e_rad);
				break;
			case 2:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, h_rad);
				break;
			default:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, e_rad);
				break;
		}

		// 頂点の設定
		// ワールドの置くときは x-=0.5, y+=0.5
		D3DXVECTOR4 vertex_pos[4];
		vertex_pos[0] = D3DXVECTOR4(- sx / 2 - 0.5f, sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[1] = D3DXVECTOR4(sx / 2 - 0.5f, sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[2] = D3DXVECTOR4(- sx / 2 - 0.5f, - sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[3] = D3DXVECTOR4(sx / 2 - 0.5f, - sy / 2 + 0.5f, 0.0f, 1.0f);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d3_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, vertex_pos, NULL))	{
			G_rect_vertex_buffer_d3_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューのカメラを設定
			D3DXVECTOR3 camera_eye, camera_pint, camera_up;
			float camera_aspect, view_angle_degree, view_angle_radian, zn, zf;
			view_angle_degree = (float)Gp_wipe->m_option[1] / TNM_ANGLE_UNIT;
			camera_eye.x = 0.0f;
			camera_eye.y = 0.0f;
			camera_eye.z = -(sy / 2) * tan(D3DXToRadian(90.0f - view_angle_degree / 2));
			camera_pint.x = 0.0f;
			camera_pint.y = 0.0f;
			camera_pint.z = 0.0f;
			camera_up.x = 0.0f;
			camera_up.y = 1.0f;
			camera_up.z = 0.0f;
			camera_aspect = (float)Gp_global->game_screen_size.cx / Gp_global->game_screen_size.cy;
			view_angle_radian = D3DXToRadian(view_angle_degree);
			zn = 1.0f;
			zf = 10000.0f;

			// ビュー行列の設定
			D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

			// 射影行列の設定
			D3DXMatrixPerspectiveFovLH(&m_view.m_mat_proj, view_angle_radian, camera_aspect, zn, zf);
			
			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ライトの設定
			wipe_sprite.rp.light_no = TNM_LIGHT_NONE;

			// スプライトを描画する
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d3_screen->unlock();
		}
	}

	// ワイプ描画：移動系：ページめくり（３０１番）
	else if (Gp_wipe->m_type == 301)	{

		for (int i = 0; i < 2; i++)	{	// 左、右

			float sx = (float)Gp_global->game_screen_size.cx;
			float sy = (float)Gp_global->game_screen_size.cy;

			// ワイプスプライト
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.rp.d2_rect = false;			// ２Ｄ矩形を外す
			wipe_sprite.rp.culling = true;			// カリングする
			wipe_sprite.rp.alpha_test = true;		// アルファテストする
			wipe_sprite.rp.alpha_blend = true;		// アルファブレンドする
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d3_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);
		
			bool reverse_flag = Gp_wipe->m_option[0] ? true : false;
			int range_type = Gp_wipe->m_option[2];

			float z = 0.0f;
			float s_rad = 0.0f, h_rad = 0.0f, e_rad = 0.0f;	// start, half, end
			if (i == 0)	{
				if (is_front)	{
					if (reverse_flag)	{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
					else				{	s_rad = D3DX_PI;		e_rad = D3DX_PI * 2;	z = -1.0f;	}
				}
				else	{
					if (reverse_flag)	{	s_rad = D3DX_PI * 2;	e_rad = D3DX_PI;		z = -1.0f;	}
					else				{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
				}
			}
			else	{
				if (is_front)	{
					if (reverse_flag)	{	s_rad = D3DX_PI;		e_rad = 0.0f;			z = -1.0f;	}
					else				{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
				}
				else	{
					if (reverse_flag)	{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
					else				{	s_rad = 0.0f;			e_rad = D3DX_PI;		z = -1.0f;	}
				}
			}

			h_rad = (s_rad + e_rad) / 2;

			switch (range_type)	{
				case 1:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, h_rad, TNM_WIPE_PRG_MAX, e_rad);
					break;
				case 2:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, h_rad);
					break;
				default:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, e_rad);
					break;
			}

			// 位置
			// ワールドに置くときは x-=0.5, y+=0.5
			D3DXVECTOR4 vertex_pos[4];
			if (i == 0)	{
				vertex_pos[0] = D3DXVECTOR4(- sx / 2 - 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(- 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(- sx / 2 - 0.5f, - sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(- 0.5f, - sy / 2 + 0.5f, z, 1.0f);
			}
			else	{
				vertex_pos[0] = D3DXVECTOR4(- 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(sx / 2 - 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(- 0.5f, - sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(sx / 2 - 0.5f, - sy / 2 + 0.5f, z, 1.0f);
			}

			// テクスチャＵＶ
			D3DXVECTOR2 tex_pos[4];
			if (i == 0)	{
				tex_pos[0] = D3DXVECTOR2(0.0f, 0.0f);
				tex_pos[1] = D3DXVECTOR2(0.5f, 0.0f);
				tex_pos[2] = D3DXVECTOR2(0.0f, 1.0f);
				tex_pos[3] = D3DXVECTOR2(0.5f, 1.0f);
			}
			else	{
				tex_pos[0] = D3DXVECTOR2(0.5f, 0.0f);
				tex_pos[1] = D3DXVECTOR2(1.0f, 0.0f);
				tex_pos[2] = D3DXVECTOR2(0.5f, 1.0f);
				tex_pos[3] = D3DXVECTOR2(1.0f, 1.0f);
			}

			// ワイプスプライトを更新する
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d3_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, vertex_pos, tex_pos))	{
				G_rect_vertex_buffer_d3_screen->unlock();

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// カメラの設定
				D3DXVECTOR3 camera_eye, camera_pint, camera_up;
				float camera_aspect, view_angle_degree, view_angle_radian, zn, zf;

				view_angle_degree = (float)Gp_wipe->m_option[1] / TNM_ANGLE_UNIT;
				camera_eye.x = 0.0f;
				camera_eye.y = 0.0f;
				camera_eye.z = -(sy / 2) * tan(D3DXToRadian(90.0f - view_angle_degree / 2));
				camera_pint.x = 0.0f;
				camera_pint.y = 0.0f;
				camera_pint.z = 0.0f;
				camera_up.x = 0.0f;
				camera_up.y = 1.0f;
				camera_up.z = 0.0f;
				camera_aspect = (float)Gp_global->game_screen_size.cx / Gp_global->game_screen_size.cy;
				view_angle_radian = D3DXToRadian(view_angle_degree);
				zn = 1.0f;
				zf = 10000.0f;

				// ビュー行列の設定
				D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

				// 射影行列の設定
				D3DXMatrixPerspectiveFovLH(&m_view.m_mat_proj, view_angle_radian, camera_aspect, zn, zf);
			
				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ライトの設定
				wipe_sprite.rp.light_no = TNM_LIGHT_NONE;

				// スプライトを描画する
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d3_screen->unlock();
			}
		}
	}

	return true;
}


// ****************************************************************
// ワイプ描画：移動系：スクロールと伸縮変形
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(C_d3d_sprite* p_sprite, int wipe_prg, bool is_front)
{
	int scroll_mod = is_front ? Gp_wipe->m_option[1] : Gp_wipe->m_option[2];
	switch (scroll_mod)	{
		case 1:		disp_proc_wipe_for_move_func_scroll(wipe_prg, p_sprite, Gp_wipe->m_option[0], is_front);						break;
		case 2:		disp_proc_wipe_for_move_func_expansion_and_contraction(wipe_prg, p_sprite, Gp_wipe->m_option[0], is_front);		break;
	}
}

// ****************************************************************
// ワイプ描画：移動系ＦＵＮＣ：スクロール
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_func_scroll(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in)
{
	if (is_in)	{
		switch (dir)	{
			case 0:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, - Gp_global->game_screen_size.cy, TNM_WIPE_PRG_MAX, 0);	break;
			case 1:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, Gp_global->game_screen_size.cy, TNM_WIPE_PRG_MAX, 0);		break;
			case 2:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, - Gp_global->game_screen_size.cx, TNM_WIPE_PRG_MAX, 0);	break;
			case 3:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, Gp_global->game_screen_size.cx, TNM_WIPE_PRG_MAX, 0);		break;
		}
	}
	else	{
		switch (dir)	{
			case 0:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, Gp_global->game_screen_size.cy);		break;
			case 1:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, - Gp_global->game_screen_size.cy);	break;
			case 2:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, Gp_global->game_screen_size.cx);		break;
			case 3:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, - Gp_global->game_screen_size.cx);	break;
		}
	}
}

// ****************************************************************
// ワイプ描画：移動系ＦＵＮＣ：伸縮変形
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_func_expansion_and_contraction(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in)
{
	if (is_in)	{
		switch (dir)	{
			case 0:
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 1:
				sprite->rp.center.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.pos.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 2:
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 3:
				sprite->rp.center.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.pos.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
		}
	}
	else	{
		switch (dir)	{
			case 0:
				sprite->rp.center.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.pos.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 1:
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 2:
				sprite->rp.center.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.pos.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 3:
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
		}
	}
}





// ****************************************************************
// ワイプ描画：スケール系
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale(int wipe_prg)
{
	// ステージ取得
	bool is_front = disp_proc_wipe_for_scale_get_stage(wipe_prg);

	// --- 準備 ---

	// ワイプより下のオーダーを描画準備する
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画準備する
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプより上のオーダーを描画準備する
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- 描画 ---

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのネクストを描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ワイプバッファを準備する
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ワイプバッファを塗りつぶす
	m_view.clear_render_target(m_wipe_back_color);

	// ワイプより下のオーダーを描画する
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ワイプするオーダーのフロントをワイプバッファに描画する
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ワイプバッファをゲームバッファに描画する
	if (!disp_proc_wipe_for_scale_switch(wipe_prg, is_front ? false : true))
		return false;

	// ワイプより上のオーダーを描画する
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ワイプ描画：スケール系ステージ取得
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	
	switch (Gp_wipe->m_type)	{
		case 210:
			// 次絵を拡大表示
			return false;
			break;
		case 211:
			// 元絵を縮小表示
			return true;
			break;
		case 212:
			// 元絵を拡大後、次絵に変更して縮小表示
			if(wipe_prg_float < 0.5f) {
				return true;
			}
			else {
				return false;
			}
			break;
		case 213:
			// 次絵を拡大状態からフェードＩＮしながら縮小表示
			return false;
			break;
		case 214:
			// 元絵をフェードＯＵＴしながら拡大表示
			return true;
			break;
		case 215:
			// ワイプ開始時の矩形範囲を指定して、次絵を拡大表示（２１０の少し変わったバージョンです。）
			if(Gp_wipe->m_option[1] == 0)
				return false;

			// ワイプ終了時の矩形範囲を指定して、元絵を縮小表示（２１１の少し変わったバージョンです。）
			else
				return true;
			break;
	}
	return false;
}


// ****************************************************************
// ワイプ描画：スケール系スイッチ
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale_switch(int wipe_prg, bool is_front)
{
	// ワイプバッファを描画する
	if (Gp_wipe->m_type == 210 || Gp_wipe->m_type == 211)	{
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// スケールワイプ用のスプライトの追加設定
		disp_proc_wipe_for_scale_func(&wipe_sprite, wipe_prg);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}
	// 212は拡大率がかなり高くなり頂点座標がGPU性能の限界を越えてクリップされるため、このスケール系ワイプのみ直接頂点とUVを指定する
	// 213,214はほとんどのマシンでは問題はないけどかなりの低スペックマシンだとオーバーしてしまうのでこちらで書く
	// 215は直接指定の方が楽なので
	else if(Gp_wipe->m_type == 212 || Gp_wipe->m_type == 213 || Gp_wipe->m_type == 214 || Gp_wipe->m_type == 215) {
		// ワイプスプライトの初期化
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// スケールワイプ用のスプライトの追加設定
		D3DXVECTOR4 v_pos[4];
		D3DXVECTOR2 v_uv[4];
		disp_proc_wipe_for_scale_func(v_pos, v_uv, &wipe_sprite, wipe_prg);

		// ワイプスプライトを更新する
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, v_pos, v_uv))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// レンダーターゲットをターゲットバッファに設定する
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// ビューポートをゲーム画面サイズにあわせる
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ワイプスプライトを描画
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	return true;
}

// ****************************************************************
// ワイプ描画：スケール系ＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_scale_func(C_d3d_sprite* sprite, int wipe_prg)
{
	if(Gp_wipe->m_type == 210 || Gp_wipe->m_type == 211) {
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// スケール処理
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		// 次絵を拡大表示
		if(Gp_wipe->m_type == 210) {
			scale_linear_st = 0.0f;
			scale_linear_ed = 1.0f;
		}
		// 元絵を縮小表示
		else if(Gp_wipe->m_type == 211) {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.0f;
		}

		// 透明度処理
		BYTE tr_linear_st = 255;
		BYTE tr_linear_ed = 255;
		if(Gp_wipe->m_type == 210) {
			if(Gp_wipe->m_option[1] == 1) {
				// フェードイン
				tr_linear_st = 0;
				tr_linear_ed = 255;
			}
		}
		else if(Gp_wipe->m_type == 211) {
			if(Gp_wipe->m_option[1] == 1) {
				// フェードアウト
				tr_linear_st = 255;
				tr_linear_ed = 0;
			}
		}
		sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, tr_linear_st, TNM_WIPE_PRG_MAX, tr_linear_ed);

		// 共通処理
		bool set_scale_x = false;
		bool set_scale_y = false;
		switch (Gp_wipe->m_option[0]) {
			case 0:		// 中心から
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 1:		// 左上から
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 2:		// 右上から
				sprite->rp.pos.x = sprite->rp.size.x;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 3:		// 左下から
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 4:		// 右下から
				sprite->rp.pos.x = sprite->rp.size.x;
				sprite->rp.pos.y = sprite->rp.size.y;
				sprite->rp.center.x = sprite->rp.size.x;
				sprite->rp.center.y = sprite->rp.size.y;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 5:		// 中心から上下
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 6:		// 中心から左右
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 7:		// 上から
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 8:		// 下から
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 9:		// 左から
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 10:	// 右から
				sprite->rp.pos.x = sprite->rp.size.x;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 11:
				// 値補正
				if (Gp_wipe->m_option[2] < 0)					Gp_wipe->m_option[2] = 0;
				if (Gp_wipe->m_option[2] > sprite->rp.size.x)	Gp_wipe->m_option[2] = (int)sprite->rp.size.x;
				if (Gp_wipe->m_option[3] < 0)					Gp_wipe->m_option[3] = 0;
				if (Gp_wipe->m_option[3] > sprite->rp.size.y)	Gp_wipe->m_option[3] = (int)sprite->rp.size.y;

				float x_rate = (float)Gp_wipe->m_option[2] / sprite->rp.size.x;
				float y_rate = (float)Gp_wipe->m_option[3] / sprite->rp.size.y;
				
				sprite->rp.pos.x = sprite->rp.size.x * x_rate;
				sprite->rp.pos.y = sprite->rp.size.y * y_rate;
				sprite->rp.center.x = sprite->rp.size.x * x_rate;
				sprite->rp.center.y = sprite->rp.size.y * y_rate;
				set_scale_x = true;
				set_scale_y = true;
				break;
		}
		if(set_scale_x) {
			sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, scale_linear_st, TNM_WIPE_PRG_MAX, scale_linear_ed);
		}
		if(set_scale_y) {
			sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, scale_linear_st, TNM_WIPE_PRG_MAX, scale_linear_ed);
		}
	}
	
	// テクニックを設定しない場合はデフォルトになる為、設定しない
	//p_sprite->set_effect_technique(m_view.m_default_effect.m_sprite_technique);
}

// ****************************************************************
// ワイプ描画：スケール系ＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_scale_func(D3DXVECTOR4 pos[], D3DXVECTOR2 uv[], C_d3d_sprite* sprite, int wipe_prg)
{
	if(Gp_wipe->m_type == 212 || Gp_wipe->m_type == 213 || Gp_wipe->m_type == 214) {
		// 0.0 ~ 1.0 に補正する
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// 頂点座標を設定する
		pos[0] = D3DXVECTOR4(0.0f,				0.0f,				0.5f,	1.0f);
		pos[1] = D3DXVECTOR4(sprite->rp.size.x,	0.0f,				0.5f,	1.0f);
		pos[2] = D3DXVECTOR4(0.0f,				sprite->rp.size.y,	0.5f,	1.0f);
		pos[3] = D3DXVECTOR4(sprite->rp.size.x, sprite->rp.size.y,	0.5f,	1.0f);;

		// スケール処理
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		float u_rate;
		float v_rate;
		// 元絵を拡大後、次絵に変更して縮小表示
		if(Gp_wipe->m_type == 212) {
			if(wipe_prg_float < 0.5f) {
				scale_linear_st = 1.0f;
				scale_linear_ed = 0.001f;
				u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX / 2.0f, scale_linear_ed);
				v_rate = u_rate;
			}
			else {
				scale_linear_st = 0.001f;
				scale_linear_ed = 1.0f;
				u_rate = linear_limit<float, float>((float)wipe_prg, (float)TNM_WIPE_PRG_MAX / 2.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
				v_rate = u_rate;
			}
		}
		// 次絵を拡大状態からフェードＩＮしながら縮小表示
		else if(Gp_wipe->m_type == 213) {
			scale_linear_st = 0.333f;
			scale_linear_ed = 1.0f;
			u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
			v_rate = u_rate;
			// 透明度処理
			sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 255);
		}
		// 元絵をフェードＯＵＴしながら拡大表示
		else if(Gp_wipe->m_type == 214) {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.333f;
			u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
			v_rate = u_rate;
			// 透明度処理
			sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, 255, TNM_WIPE_PRG_MAX, 0);
		}

		float su = 0.0f;
		float sv = 0.0f;
		float eu = 1.0f;
		float ev = 1.0f;

		// 共通処理
		bool set_scale_x = false;
		bool set_scale_y = false;
		switch (Gp_wipe->m_option[0]) {
			case 0:		// 中心から
				su = 0.5f - 0.5f * u_rate;
				eu = 0.5f + 0.5f * u_rate;
				sv = 0.5f - 0.5f * v_rate;
				ev = 0.5f + 0.5f * v_rate;
				break;
			case 1:		// 左上から
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 2:		// 右上から
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 3:		// 左下から
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 4:		// 右下から
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 5:		// 中心から上下
				v_rate = linear_limit<float, float>(v_rate, 0.0f, 0.49f, 1.0f, 1.0f);	// 0.5以上で反転するので
				su = 0.0f;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f * v_rate;
				break;
			case 6:		// 中心から左右
				u_rate = linear_limit<float, float>(u_rate, 0.0f, 0.49f, 1.0f, 1.0f);	// 0.5以上で反転するので
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 7:		// 上から
				su = 0.0f;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 8:		// 下から
				su = 0.0f;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 9:		// 左から
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 10:	// 右から
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 11:
				// 値補正
				if (Gp_wipe->m_option[2] < 0)					Gp_wipe->m_option[2] = 0;
				if (Gp_wipe->m_option[2] > sprite->rp.size.x)	Gp_wipe->m_option[2] = (int)sprite->rp.size.x;
				if (Gp_wipe->m_option[3] < 0)					Gp_wipe->m_option[3] = 0;
				if (Gp_wipe->m_option[3] > sprite->rp.size.y)	Gp_wipe->m_option[3] = (int)sprite->rp.size.y;

				float x_rate = (float)Gp_wipe->m_option[2] / sprite->rp.size.x;
				float y_rate = (float)Gp_wipe->m_option[3] / sprite->rp.size.y;
				
				su = x_rate - (1.0f-(1.0f - x_rate)) * u_rate;
				eu = x_rate + (1.0f - x_rate) * u_rate;
				sv = y_rate - (1.0f-(1.0f - y_rate)) * v_rate;
				ev = y_rate + (1.0f - y_rate) * v_rate;
				break;
		}
		uv[0] = D3DXVECTOR2(su, sv);
		uv[1] = D3DXVECTOR2(eu, sv);
		uv[2] = D3DXVECTOR2(su, ev);
		uv[3] = D3DXVECTOR2(eu, ev);
	}
	// ワイプ開始(終了)時の矩形範囲を指定して、次(元)絵を拡大(縮小)表示
	else if(Gp_wipe->m_type == 215) {
		// 0.0 ~ 1.0 に補正する
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// UV座標を設定する
		uv[0] = D3DXVECTOR2(0.0f, 0.0f);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f);

		// スケール処理
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		// 拡大
		if(Gp_wipe->m_option[1] == 0) {
			scale_linear_st = 0.0f;
			scale_linear_ed = 1.0f;
		}
		// 縮小
		else {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.0f;
		}

		// 透明度処理
		BYTE tr_linear_st = 255;
		BYTE tr_linear_ed = 255;
		// 透明度処理なし
		if(Gp_wipe->m_option[0] == 0) {
			tr_linear_st = 255;
			tr_linear_ed = 255;
		}
		// フェードイン
		else if(Gp_wipe->m_option[0] == 1) {
			tr_linear_st = 0;
			tr_linear_ed = 255;
		}
		// フェードアウト
		else if(Gp_wipe->m_option[0] == 2) {
			tr_linear_st = 255;
			tr_linear_ed = 0;
		}
		sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, tr_linear_st, TNM_WIPE_PRG_MAX, tr_linear_ed);



		int sx = Gp_wipe->m_option[2];
		int sy = Gp_wipe->m_option[3];
		int ex = Gp_wipe->m_option[4];
		int ey = Gp_wipe->m_option[5];
		// まずは範囲補正
		if (sx < 0)						sx = 0;
		if (sx > sprite->rp.size.x)		sx = (int)sprite->rp.size.x;
		if (sy < 0)						sy = 0;
		if (sy > sprite->rp.size.y)		sy = (int)sprite->rp.size.y;
		if (ex < 0)						ex = 0;
		if (ex > sprite->rp.size.x)		ex = (int)sprite->rp.size.x;
		if (ey < 0)						ey = 0;
		if (ey > sprite->rp.size.y)		ey = (int)sprite->rp.size.y;
		// 反転・同値補正
		if (sx >= ex) {
			// 同値の場合は最低でも1を確保
			if (sx == ex) {
				// endに+1で範囲外になる場合はstartを-1
				if (sx + 1 >= sprite->rp.size.x)
					sx = ex - 1;
				else
					ex = sx + 1;
			}
			// sx > ex の場合は sx < ex になるようにXORスワップ(floatは無理なので注意)
			else {
				sx ^= ex;
				ex ^= sx;
				sx ^= ex;
			}
		}
		// 反転・同値補正
		if (sy >= ey) {
			// 同値の場合は最低でも1を確保
			if (sy == ey) {
				// endに+1で範囲外になる場合はstartを-1
				if (sy + 1 >= sprite->rp.size.y)
					sy = ey - 1;
				else
					ey = sy + 1;
			}
			// sy > ey の場合は sy < ey になるようにXORスワップ(floatは無理なので注意)
			else {
				sy ^= ey;
				ey ^= sy;
				sy ^= ey;
			}
		}

		int fsx = 0;
		int fsy = 0;
		int fex = (int)sprite->rp.size.x;
		int fey = (int)sprite->rp.size.y;

		// 拡大
		if (Gp_wipe->m_option[1] == 0) {
			fsx = linear_limit<int, int>(wipe_prg, 0, sx, TNM_WIPE_PRG_MAX, 0);
			fsy = linear_limit<int, int>(wipe_prg, 0, sy, TNM_WIPE_PRG_MAX, 0);
			fex = linear_limit<int, int>(wipe_prg, 0, ex, TNM_WIPE_PRG_MAX, (int)sprite->rp.size.x);
			fey = linear_limit<int, int>(wipe_prg, 0, ey, TNM_WIPE_PRG_MAX, (int)sprite->rp.size.y);
		}
		else if (Gp_wipe->m_option[1] == 1) {
			fsx = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, sx);
			fsy = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, sy);
			fex = linear_limit<int, int>(wipe_prg, 0, (int)sprite->rp.size.x, TNM_WIPE_PRG_MAX, ex);
			fey = linear_limit<int, int>(wipe_prg, 0, (int)sprite->rp.size.y, TNM_WIPE_PRG_MAX, ey);
		}

		// 頂点座標を設定する
		pos[0] = D3DXVECTOR4((float)fsx,	(float)fsy,	0.5f,	1.0f);
		pos[1] = D3DXVECTOR4((float)fex,	(float)fsy,	0.5f,	1.0f);
		pos[2] = D3DXVECTOR4((float)fsx,	(float)fey,	0.5f,	1.0f);
		pos[3] = D3DXVECTOR4((float)fex,	(float)fey,	0.5f,	1.0f);
	}
}

// ****************************************************************
// ワイプ描画：爆発ブラー
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_explosion_blur(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	if (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 242) {
		// --- 準備 ---

		// ワイプより下のオーダーを描画準備する
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストを描画準備する
		if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画準備する
		if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
			return false;

		// ワイプより上のオーダーを描画準備する
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- 描画 ---
		
		// ワイプバッファ１を準備する
		if (!disp_proc_ready_wipe_buffer_1())
			return false;

		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストを描画する
		if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
			return false;

		// ワイプバッファ０を準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画する
		if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
			return false;

		// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
		// ここからRTを変更し、ワイプバッファをスプライトとして描画する

		// ワイプバッファを描画する
		{
			// ワイプスプライトの初期化
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_texture(m_wipe_back_buffer_texture_1, TNM_TEXTURE_STAGE_MASK);		// ワイプバッファ１も設定する
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// クロス爆発ブラーワイプ用のスプライトの追加設定
			disp_proc_wipe_for_explosion_blur_func(&wipe_sprite, wipe_prg);

			// ワイプスプライトを更新する
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ワイプスプライトを描画
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ワイプより上のオーダーを描画する
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;

		return true;
	}
	else if(Gp_wipe->m_type == 241 || Gp_wipe->m_type == 243) {
		// ステージ取得
		bool is_front = disp_proc_wipe_for_explosion_blur_get_stage(wipe_prg);

		// --- 準備 ---

		// ワイプより下のオーダーを描画準備する
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストをワイプバッファに描画準備する
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画準備する
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ワイプより上のオーダーを描画準備する
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- 描画 ---

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストを描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;
	
		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
		// ここからRTを変更し、ワイプバッファをスプライトとして描画する

		// ワイプバッファを描画する
		{
			// ワイプスプライトの初期化
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// 爆発ブラーワイプ用のスプライトの追加設定
			disp_proc_wipe_for_explosion_blur_func(&wipe_sprite, wipe_prg);

			// ワイプスプライトを更新する
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ワイプスプライトを描画
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ワイプより上のオーダーを描画する
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;
	}

	return true;
}
// ****************************************************************
// ワイプ描画：爆発ブラーステージ取得
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_explosion_blur_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	// 元絵 = true
	// 次絵 = false
	switch (Gp_wipe->m_type)	{
		case 240:
			return false;
			break;
		case 241:
			// 元絵
			if(Gp_wipe->m_option[7] == 0)
				return true;
			else
				return false;
			break;
		case 242:
			return false;
			break;
		case 243:
			// 元絵
			if(Gp_wipe->m_option[5] == 0)
				return true;
			else
				return false;
			break;
	}
	return false;
}
// ****************************************************************
// ワイプ描画：爆発ブラーＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_explosion_blur_func(C_d3d_sprite* sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	// 透明度処理
	int alpha_type    = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[2] : Gp_wipe->m_option[0];
	int alpha_reverse = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[3] : Gp_wipe->m_option[1];
	BYTE tr = 255;
	if(alpha_type == 0 || alpha_type == 10) {
		// 右肩下がり
		if(alpha_type == 0)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f));
		// 右肩上がり
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f));

		// アルファ反転あり
		if(alpha_reverse)
			sprite->rp.tr = 255 - tr;
		// アルファ反転なし
		else
			sprite->rp.tr = tr;
	}
	else if(alpha_type > 0 || alpha_type < 10) {
		float threshold = (float)alpha_type / 10.0f;	// 閾値
		// 閾値までは右肩上がり
		if(wipe_prg_float < threshold)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, threshold, 1.0f));
		// 閾値以降は右肩下がり
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, threshold, 1.0f, 1.0f, 0.0f));
		
		// アルファ反転あり
		if(alpha_reverse)
			sprite->rp.tr = 255 - tr;
		// アルファ反転なし
		else
			sprite->rp.tr = tr;
	}
	else {
		// タイプ範囲外はアルファ反転も無効として全て255とする
		tr = 255;
	}
	
	// ブラーパワー処理
	int bp_type    = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[4] : Gp_wipe->m_option[2];
	int bp_reverse = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[5] : Gp_wipe->m_option[3];
	float bp = 0.0f;
	if(bp_type == 0 || bp_type == 10) {
		// 右肩下がり
		if(bp_type == 0)
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f);
		// 右肩上がり
		else
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f);

		// 反転あり
		if(bp_reverse)
			bp = 1.0f - bp;
		// 反転なし
		else
			bp = bp;
	}
	else if(bp_type > 0 || bp_type < 10) {
		float threshold = (float)bp_type / 10.0f;	// 閾値
		// 閾値までは右肩上がり
		if(wipe_prg_float < threshold)
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, threshold, 1.0f);
		// 閾値以降は右肩下がり
		else
			bp = linear_limit<float, float>(wipe_prg_float, threshold, 1.0f, 1.0f, 0.0f);
		
		// 反転あり
		if(bp_reverse)
			bp = 1.0f - bp;
		// 反転なし
		else
			bp = bp;
	}
	else {
		// タイプ範囲外は反転も無効として全て0.0fとする
		bp = 0.0f;
	}

	
	if(!sprite->get_texture(0))
		return;
	// 定数設定
	float texel_size_u = (float)1.0f / sprite->get_texture(0)->get_width_ex();
	float texel_size_v = (float)1.0f / sprite->get_texture(0)->get_height_ex();
	//float texel_size_u = (float)1.0f / sprite->rp.size.cx;
	//float texel_size_v = (float)1.0f / sprite->rp.size.cy;
	//float texel_center_u = (float)Gp_wipe->m_option[0] / sprite->get_texture(0)->get_width_ex();
	//float texel_center_v = (float)Gp_wipe->m_option[1] / sprite->get_texture(0)->get_height_ex();
	float texel_center_u = (float)Gp_wipe->m_option[0] / sprite->get_texture(0)->get_width_ex();
	float texel_center_v = (float)Gp_wipe->m_option[1] / sprite->get_texture(0)->get_height_ex();
	if (Gp_wipe->m_type == 242 || Gp_wipe->m_type == 243) {
		// ランダムで中心を設定
		texel_center_u = (float)(rand() % (int)sprite->rp.size.x) / sprite->get_texture(0)->get_width_ex();
		texel_center_v = (float)(rand() % (int)sprite->rp.size.y) / sprite->get_texture(0)->get_height_ex();
	}
	float blur_coefficient = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? (float)Gp_wipe->m_option[6] : (float)Gp_wipe->m_option[4];
	sprite->set_effect_constant_f(0, texel_size_u, texel_size_v, texel_center_u, texel_center_v);
	sprite->set_effect_constant_f(1, bp, blur_coefficient, 0, wipe_prg_float);

	// テクニック設定
	if(Gp_wipe->m_type == 240 || Gp_wipe->m_type == 242)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_cross_explosion_blur_technique);
	}
	else if(Gp_wipe->m_type == 241 || Gp_wipe->m_type == 243)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_explosion_blur_technique);
	}
}

// ****************************************************************
// ワイプ描画：モザイク
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mosaic(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	if(Gp_wipe->m_type == 230 || Gp_wipe->m_type == 231) {
		// ステージ取得
		bool is_front = disp_proc_wipe_for_mosaic_get_stage(wipe_prg);

		// --- 準備 ---

		// ワイプより下のオーダーを描画準備する
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストをワイプバッファに描画準備する
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画準備する
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ワイプより上のオーダーを描画準備する
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- 描画 ---

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのネクストを描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ワイプバッファを準備する
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ワイプバッファを塗りつぶす
		m_view.clear_render_target(m_wipe_back_color);

		// ワイプより下のオーダーを描画する
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ワイプするオーダーのフロントをワイプバッファに描画する
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ここまででRTはワイプバッファに設定され、ワイプバッファ自体への描画は完了
		// ここからRTを変更し、ワイプバッファをスプライトとして描画する

		// ワイプバッファを描画する
		{
			// ワイプスプライトの初期化
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// モザイクワイプ用のスプライトの追加設定
			disp_proc_wipe_for_mosaic_func(&wipe_sprite, wipe_prg);

			// ワイプスプライトを更新する
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// レンダーターゲットをターゲットバッファに設定する
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// ビューポートをゲーム画面サイズにあわせる
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ワイプスプライトを描画
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ワイプより上のオーダーを描画する
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;
	}

	return true;
}
// ****************************************************************
// ワイプ描画：モザイクステージ取得
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mosaic_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	// 元絵 = true
	// 次絵 = false
	switch (Gp_wipe->m_type)	{
		case 230:
			if(wipe_prg_float < 0.5f)
				return true;
			else
				return false;
			break;
		case 231:
			// 元絵
			if(Gp_wipe->m_option[1] == 0)
				return true;
			else
				return false;
			break;
	}
	return false;
}
// ****************************************************************
// ワイプ描画：モザイクＦＵＮＣ
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_mosaic_func(C_d3d_sprite* sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	// 透明度処理
	BYTE tr = 255;
	if(Gp_wipe->m_type == 230) {
		// 行わない
		sprite->rp.tr = tr;
	}
	else if(Gp_wipe->m_type == 231) {
		// 元絵フェードアウト
		if(Gp_wipe->m_option[1] == 0)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f));
		// 次絵フェードイン
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f));

		sprite->rp.tr = tr;
	}

	// モザイク処理
	float st_texel_size = 1.0f;
	float ed_texel_size = 1.0f;
	float cut_per_texel_size = 0.1f;
	if(Gp_wipe->m_type == 230) {
		// st_texel_size - ed_texel_size - st_texel_size になる
		switch(Gp_wipe->m_option[0]) {
			case 0:
				st_texel_size = 0.001f;
				ed_texel_size = 0.025f;
				break;
			case 1:
				st_texel_size = 0.002f;
				ed_texel_size = 0.04f;
				break;
			case 2:
				st_texel_size = 0.003f;
				ed_texel_size = 0.06f;
				break;
			case 3:
				st_texel_size = 0.004f;
				ed_texel_size = 0.08f;
				break;
			case 4:
				st_texel_size = 0.005f;
				ed_texel_size = 0.1f;
				break;
				// 中間 0.005
				// 中間 0.025 - 0.25 - 0.5
			case 5:
				st_texel_size = 0.006f;
				ed_texel_size = 0.15f;
				break;
			case 6:
				st_texel_size = 0.007f;
				ed_texel_size = 0.2f;
				break;
			case 7:
				st_texel_size = 0.008f;
				ed_texel_size = 0.3f;
				break;
			case 8:
				st_texel_size = 0.009f;
				ed_texel_size = 0.4f;
				break;
			case 9:
				st_texel_size = 0.01f;
				ed_texel_size = 0.5f;
				break;
		}

		// 時間経過を離散的に行う
		//float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
		//if(discrete_wipe_prg_float < 0.5f)
		//	cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 0.5f, ed_texel_size);
		//else
		//	cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.5f, ed_texel_size, 1.0f, st_texel_size);
		float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 50)) / 50.0f;
		int discrete_wipe_prg = (int)(discrete_wipe_prg_float * TNM_WIPE_PRG_MAX);
		if(discrete_wipe_prg < TNM_WIPE_PRG_MAX / 2)
			cut_per_texel_size = speed_up_limit(discrete_wipe_prg, 0, (int)(st_texel_size * 10000), TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000)) / 10000.0f;
		else {
			// 最後ぶつ切れになるのを防ぐ
			if(discrete_wipe_prg_float < 0.95f)
				cut_per_texel_size = speed_down_limit(discrete_wipe_prg, TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000), TNM_WIPE_PRG_MAX, (int)(st_texel_size * 10000)) / 10000.0f;
			else
				cut_per_texel_size = speed_down_limit(wipe_prg, TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000), TNM_WIPE_PRG_MAX, (int)(st_texel_size * 10000)) / 10000.0f;
		}
	}
	else if(Gp_wipe->m_type == 231) {
		//// 元絵フェードアウト
		//if(Gp_wipe->m_option[1] == 0) {
		//	st_texel_size = 0.01f;
		//	ed_texel_size = 0.1f;
		//}
		//// 次絵フェードイン
		//else {
		//	st_texel_size = 0.1f;
		//	ed_texel_size = 0.01f;
		//}

		// 元絵フェードアウトを基準に設定する
		switch(Gp_wipe->m_option[0]) {
			case 0:
				st_texel_size = 0.001f;
				ed_texel_size = 0.025f;
				break;
			case 1:
				st_texel_size = 0.002f;
				ed_texel_size = 0.04f;
				break;
			case 2:
				st_texel_size = 0.003f;
				ed_texel_size = 0.06f;
				break;
			case 3:
				st_texel_size = 0.004f;
				ed_texel_size = 0.08f;
				break;
			case 4:
				st_texel_size = 0.005f;
				ed_texel_size = 0.1f;
				break;
				// 中間 0.005
				// 中間 0.025 - 0.25 - 0.5
			case 5:
				st_texel_size = 0.006f;
				ed_texel_size = 0.15f;
				break;
			case 6:
				st_texel_size = 0.007f;
				ed_texel_size = 0.2f;
				break;
			case 7:
				st_texel_size = 0.008f;
				ed_texel_size = 0.3f;
				break;
			case 8:
				st_texel_size = 0.009f;
				ed_texel_size = 0.4f;
				break;
			case 9:
				st_texel_size = 0.01f;
				ed_texel_size = 0.5f;
				break;
		}

		// 次絵フェードインの場合はstとedを入れ替える
		if(Gp_wipe->m_option[1] == 1) {
			float tmp = st_texel_size;
			st_texel_size = ed_texel_size;
			ed_texel_size = tmp;
		}

		// 時間経過処理
		if(Gp_wipe->m_option[1] == 0) {
			// 時間経過を離散的に行う
			float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
			cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
		}
		else {
			// 時間経過を離散的に行う
			float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
			// 最後ぶつ切れになるのを防ぐ
			if(discrete_wipe_prg_float < 0.95f)
				cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
			else
				cut_per_texel_size = linear_limit<float, float>(wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
		}
	}

	if(!sprite->get_texture(0))
		return;

	// 定数設定
	float final_cut_per_texel_size = cut_per_texel_size;

	// 2のn乗テクスチャが作成できない環境に対応する為(作成できる環境では * 1.0f になるので問題なし)
	float cut_per_texel_size_ex = cut_per_texel_size * sprite->get_texture(0)->get_width() / sprite->get_texture(0)->get_width_ex();
	// 正方形にする為の縦横比(こちらはexは必要なし)
	float tex_rate_for_square = (float)sprite->get_texture(0)->get_width() / sprite->get_texture(0)->get_height();
	
	// 実際に定数を設定する
	sprite->set_effect_constant_f(0, cut_per_texel_size, tex_rate_for_square, 0, 0);

	// テクニック設定
	if(Gp_wipe->m_type == 230 || Gp_wipe->m_type == 231)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_mosaic_technique);
	}
}