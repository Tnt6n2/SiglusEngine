#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：４×４
// ================================================================

bool tnm_make_32bit_mask_for_4x4(S_mask_buffer *buf, bool gray_reverse, int pat_w, int pat_h, BYTE *color_map)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, pat_w * 4, pat_h * 4, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	BYTE *cm = color_map;
	int y = 0;
	for(int i = 0; i < 4; i++)	{
		int x = 0;
		for (int j = 0; j < 4; j++)	{
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x, y, (x + pat_w - 1), (y + pat_h - 1), *cm, gray_reverse, ex1, ey1, ex2, ey2);
			x += pat_w;
			cm++;
		}
		y += pat_h;
	}
	return true;
}

bool tnm_make_mask_texture_for_4x4(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, BYTE *color_map)
{
	pat_w = limit(1, pat_w, 128);
	pat_h = limit(1, pat_h, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_4x4(&buf, gray_reverse, pat_w, pat_h, color_map))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：４×４のじわだし９パターン
// ================================================================
bool tnm_make_mask_texture_for_4x4_jiwa9(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h)
{
	BYTE color_map[16] = 
	{
		255, 64,192, 32,
		 96,160,128,160,
		192,  0,224, 64,
		128,160, 96,160,
	};
	return tnm_make_mask_texture_for_4x4(texture, target_w, target_h, gray_reverse, pat_w, pat_h, color_map);
}

// ****************************************************************
// マスクテクスチャ作成：４×４のじわだし７パターン
// ================================================================
bool tnm_make_mask_texture_for_4x4_jiwa7(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h)
{
	BYTE color_map[16] = 
	{
		255, 43,213,  0,
		 85,170,128,170,
		213,  0,255, 43,
		128,170, 85,170,
	};
	return tnm_make_mask_texture_for_4x4(texture, target_w, target_h, gray_reverse, pat_w, pat_h, color_map);
}

// ****************************************************************
// マスクテクスチャ作成：４×４の回るように
// ================================================================
bool tnm_make_mask_texture_for_4x4_turn_around(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod)
{
	BYTE color_map[8][16] = 
	{
		255,238,221,204,		// →□□□
		 68, 51, 34,187,		// □□□□
		 85,  0, 17,170,		// □□□□
		102,119,136,153,		// □□□□

		102, 85, 68,255,		// □□□↓
		119,  0, 51,238,		// □□□□
		136, 17, 34,221,		// □□□□
		153,170,187,204,		// □□□□

		153,136,119,102,		// □□□□
		170, 17,  0, 85,		// □□□□
		187, 34, 51, 68,		// □□□□
		204,221,238,255,		// □□□←

		204,187,170,153,		// □□□□
		221, 34, 17,136,		// □□□□
		238, 51,  0,119,		// □□□□
		255, 68, 85,102,		// ↑□□□

		204,221,238,255,		// □□□←
		187, 34, 51, 68,		// □□□□
		170, 17,  0, 85,		// □□□□
		153,136,119,102,		// □□□□

		255, 68, 85,102,		// ↓□□□
		238, 51,  0,119,		// □□□□
		221, 34, 17,136,		// □□□□
		204,187,170,153,		// □□□□

		102,119,136,153,		// □□□□
		 85,  0, 17,170,		// □□□□
		 68, 51, 34,187,		// □□□□
		255,238,221,204,		// →□□□

		153,170,187,204,		// □□□□
		136, 17, 34,221,		// □□□□
		119,  0, 51,238,		// □□□□
		102, 85, 68,255,		// □□□↑
	};
	if (!(0 <= pat_mod && pat_mod < 8))	{	pat_mod = 0;	}
	return tnm_make_mask_texture_for_4x4(texture, target_w, target_h, gray_reverse, pat_w, pat_h, color_map[pat_mod]);
}

// ****************************************************************
// マスクテクスチャ作成：４×４の改行すように
// ================================================================
bool tnm_make_mask_texture_for_4x4_turn_ret(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod)
{
	BYTE color_map[8][16] = 
	{
		255,238,221,204,		// →□□□
		187,170,153,136,		// □□□□
		119,102, 85, 68,		// □□□□
		 51, 34, 17,  0,		// □□□□

		204,221,238,255,		// □□□←
		136,153,170,187,		// □□□□
		 68, 85,102,119,		// □□□□
		  0, 17, 34, 51,		// □□□□

		 51, 34, 17,  0,		// □□□□
		119,102, 85, 68,		// □□□□
		187,170,153,136,		// □□□□
		255,238,221,204,		// →□□□

		  0, 17, 34, 51,		// □□□□
		 68, 85,102,119,		// □□□□
		136,153,170,187,		// □□□□
		204,221,238,255,		// □□□←

		255,187,119, 51,		// ↓□□□
		238,170,102, 34,		// □□□□
		221,153, 85, 17,		// □□□□
		204,136, 68,  0,		// □□□□

		204,136, 68,  0,		// □□□□
		221,153, 85, 17,		// □□□□
		238,170,102, 34,		// □□□□
		255,187,119, 51,		// ↑□□□

		 51,119,187,255,		// □□□↓
		 34,102,170,238,		// □□□□
		 17, 85,153,221,		// □□□□
		  0, 68,136,204,		// □□□□
										 
		  0, 68,136,204,		// □□□□
		 17, 85,153,221,		// □□□□
		 34,102,170,238,		// □□□□
		 51,119,187,255,		// □□□↑
	};
	if (!(0 <= pat_mod && pat_mod < 8))	{	pat_mod = 0;	}
	return tnm_make_mask_texture_for_4x4(texture, target_w, target_h, gray_reverse, pat_w, pat_h, color_map[pat_mod]);
}

// ****************************************************************
// マスクテクスチャ作成：４×４の折り返すように
// ================================================================
bool tnm_make_mask_texture_for_4x4_turn_down(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_mod)
{
	BYTE color_map[8][16] = 
	{
		255,238,221,204,		// →□□□
		136,153,170,187,		// □□□□
		119,102, 85, 68,		// □□□□
		  0, 17, 34, 51,		// □□□□

		204,221,238,255,		// □□□←
		187,170,153,136,		// □□□□
		 68, 85,102,119,		// □□□□
		 51, 34, 17,  0,		// □□□□

		  0, 17, 34, 51,		// □□□□
		119,102, 85, 68,		// □□□□
		136,153,170,187,		// □□□□
		255,238,221,204,		// →□□□

		 51, 34, 17,  0,		// □□□□
		 68, 85,102,119,		// □□□□
		187,170,153,136,		// □□□□
		204,221,238,255,		// □□□←

		255,136,119,  0,		// ↓□□□
		238,153,102, 17,		// □□□□
		221,170, 85, 34,		// □□□□
		204,187, 68, 51,		// □□□□

		204,187, 68, 51,		// □□□□
		221,170, 85, 34,		// □□□□
		238,153,102, 17,		// □□□□
		255,136,119,  0,		// ↑□□□

		  0,119,136,255,		// □□□↓
		 17,102,153,238,		// □□□□
		 34, 85,170,221,		// □□□□
		 51, 68,187,204,		// □□□□
										 
		 51, 68,187,204,		// □□□□
		 34, 85,170,221,		// □□□□
		 17,102,153,238,		// □□□□
		  0,119,136,255,		// □□□↑
	};
	if (!(0 <= pat_mod && pat_mod < 8))	{	pat_mod = 0;	}
	return tnm_make_mask_texture_for_4x4(texture, target_w, target_h, gray_reverse, pat_w, pat_h, color_map[pat_mod]);
}

