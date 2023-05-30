#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：指定方向から
// ================================================================

bool tnm_make_32bit_mask_for_direction(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	DWORD *dw_dp;
	int ex1, ey1, ex2, ey2;
	if (dir == 0 || dir == 1)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, (BYTE **)&dw_dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, (BYTE **)&dw_dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}

	if (dir == 1 || dir == 3)	{	gray_reverse = (gray_reverse) ? false : true;	}

	int cnt = (dir == 0 || dir == 1) ? make_h : make_w;
	int pal;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0, 0, cnt - 1, 255);
		pal = (gray_reverse) ? pal : 255 - pal;
		*dw_dp++ = (pal << 24) | (pal << 16) | (pal << 8) | pal;
	}
	return true;
}

bool tnm_make_mask_texture_for_direction(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_direction(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：指定方向からスライス
// ================================================================

bool tnm_make_32bit_mask_for_direction_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int slice_len)
{
	DWORD *dw_dp;
	int ex1, ey1, ex2, ey2;
	int len;
	if (dir == 0 || dir == 1)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, (BYTE **)&dw_dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		len = make_h;
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, (BYTE **)&dw_dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		len = make_w;
	}

	int cnt = len / slice_len;
	if (len % slice_len)	{	cnt++;	}

	DWORD *end_dw_dp = dw_dp + len;

	if (dir == 1 || dir == 3)	{	gray_reverse = (gray_reverse) ? false : true;	}

	DWORD *wp;
	int s_pal_ind = 0;
	int s_pal, e_pal, pal;

#if 1	// 正順方向スライス
	for (int i = 0; i < cnt; i++)	{
		s_pal = linear_limit(s_pal_ind, 0, 0, cnt - 1, 127);
		e_pal = s_pal + 255 - 127;
		wp = dw_dp;
		for (int j = 0; j < slice_len; j++)	{
			if ((DWORD)(PTR)wp < (DWORD)(PTR)end_dw_dp)	{
				pal = linear_limit(j, 0, s_pal, slice_len - 1, e_pal);
				pal = (gray_reverse) ? pal : 255 - pal;
				*wp++ = (pal << 24) | (pal << 16) | (pal << 8) | pal;
			}
		}
		dw_dp += slice_len;
		s_pal_ind++;
	}
#else	// 逆順方向スライス
	for (int i = 0; i < cnt; i++)	{
		s_pal = linear_limit(s_pal_ind, 0, 0, cnt, 127);
		e_pal = s_pal + 255 - 127;
		dw_dp += slice_len;
		wp = dw_dp - 1;
		for (int j = 0; j < slice_len; j++)	{
			if ((DWORD)(PTR)wp < (DWORD)(PTR)end_dw_dp)	{
				pal = linear_limit(j, 0, s_pal, slice_len, e_pal);
				pal = (gray_reverse) ? pal : 255 - pal;
				*wp-- = (pal << 24) | (pal << 16) | (pal << 8) | pal;
			}
		}
		s_pal_ind++;
	}
#endif

	return true;
}

bool tnm_make_mask_texture_for_direction_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len)
{
	slice_len = limit(2, slice_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_direction_slice(&buf, target_w, target_h, gray_reverse, dir, slice_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：指定方向からブラインド
// ================================================================

bool tnm_make_32bit_mask_for_direction_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len)
{
	if (dir == 0 || dir == 1)	{
		return tnm_make_32bit_mask_for_direction(buf, make_w, blind_len, gray_reverse, dir);
	}
	else	{
		return tnm_make_32bit_mask_for_direction(buf, blind_len, make_h, gray_reverse, dir);
	}
}

bool tnm_make_mask_texture_for_direction_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len)
{
	blind_len = limit(2, blind_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_direction_blind(&buf, target_w, target_h, gray_reverse, dir, blind_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：指定の斜め方向から
// ================================================================

bool tnm_make_32bit_mask_for_direction_slant(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (dir == 2 || dir == 3)	{	gray_reverse = (gray_reverse) ? false : true;	}

	int cnt = make_w + make_h - 1;
	int x1, y1, x2, y2, pal;

	if (dir == 0 || dir == 3)	{
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 < (make_w - 1))	{	x1++;	}		else	{	y1++;	}
			if (y2 < (make_h - 1))	{	y2++;	}		else	{	x2++;	}
		}
	}
	else	{
		x1 = make_w - 1;
		y1 = 0;
		x2 = make_w - 1;
		y2 = 0;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 > 0)				{	x1--;	}		else	{	y1++;	}
			if (y2 < (make_h - 1))	{	y2++;	}		else	{	x2--;	}
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_direction_slant(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_direction_slant(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：指定方向からブロックブラインド
// ================================================================

bool tnm_make_32bit_mask_for_direction_block_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len, int block_len)
{
	blind_len /= 2;

	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (dir == 0 || dir == 1)	{
		if (!tnm_alloc_mask_buffer(buf, block_len * 2, blind_len * 2, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, blind_len * 2, block_len * 2, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}

	if (dir == 1 || dir == 3)	{	gray_reverse = (gray_reverse) ? false : true;	}

	int cnt, pal, alpha;

	if (dir == 0 || dir == 1)	{
		cnt = buf->h;
		alpha = blind_len;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h,         0,     i,             block_len - 1,     i, 255 - pal, gray_reverse, -1, -1, -1, -1);
			tnm_draw_32bit_line(dp, buf->w, buf->h, block_len, alpha, block_len + block_len - 1, alpha, 255 - pal, gray_reverse, -1, -1, -1, -1);
			alpha++;	alpha %= cnt;
		}
	}
	else	{
		cnt = buf->w;
		alpha = blind_len;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h,     i,         0,     i,             block_len - 1, 255 - pal, gray_reverse, -1, -1, -1, -1);
			tnm_draw_32bit_line(dp, buf->w, buf->h, alpha, block_len, alpha, block_len + block_len - 1, 255 - pal, gray_reverse, -1, -1, -1, -1);
			alpha++;	alpha %= cnt;
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_direction_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len)
{
	blind_len = limit(2, blind_len, 128);
	block_len = limit(1, block_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_direction_block_blind(&buf, target_w, target_h, gray_reverse, dir, blind_len, block_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}
