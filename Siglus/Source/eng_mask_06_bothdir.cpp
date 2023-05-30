#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：両方向から
// ================================================================

bool tnm_make_32bit_mask_for_bothdir(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	BYTE *dp;
	int ex1, ey1, ex2, ey2;

	S_mask_buffer tmp;
	BYTE *sp;

	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		tnm_make_32bit_mask_for_direction(&tmp, 1, up_h, gray_reverse, 0);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, 0);

		tnm_make_32bit_mask_for_direction(&tmp, 1, dw_h, gray_reverse, 1);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, up_h);
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		tnm_make_32bit_mask_for_direction(&tmp, lf_w, 1, gray_reverse, 2);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, 0);

		tnm_make_32bit_mask_for_direction(&tmp, ri_w, 1, gray_reverse, 3);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, 0);
	}

	return true;
}

bool tnm_make_mask_texture_for_bothdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：両方向からスライス
// ================================================================

bool tnm_make_32bit_mask_for_bothdir_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int slice_len)
{
	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	BYTE *dp;
	int ex1, ey1, ex2, ey2;

	S_mask_buffer tmp;
	BYTE *sp;
	int alpha;

	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		alpha = (slice_len - (up_h % slice_len)) % slice_len;
		tnm_make_32bit_mask_for_direction_slice(&tmp, 1, up_h + alpha, gray_reverse, 0, slice_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, 0 - alpha);

		alpha = (slice_len - (dw_h % slice_len)) % slice_len;
		tnm_make_32bit_mask_for_direction_slice(&tmp, 1, dw_h + alpha, gray_reverse, 1, slice_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, up_h);
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		alpha = (slice_len - (lf_w % slice_len)) % slice_len;
		tnm_make_32bit_mask_for_direction_slice(&tmp, lf_w + alpha, 1, gray_reverse, 2, slice_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0 - alpha, 0);

		alpha = (slice_len - (ri_w % slice_len)) % slice_len;
		tnm_make_32bit_mask_for_direction_slice(&tmp, ri_w + alpha, 1, gray_reverse, 3, slice_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, 0);
	}

	return true;
}

bool tnm_make_mask_texture_for_bothdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int slice_len)
{
	slice_len = limit(2, slice_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir_slice(&buf, target_w, target_h, gray_reverse, dir, slice_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：両方向からブラインド
// ================================================================

bool tnm_make_32bit_mask_for_bothdir_blind_func(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	S_mask_buffer tmp;
	int cnt;
	BYTE *sp;

	if (dir == 0 || dir == 1)	{
		if (!tnm_make_32bit_mask_for_direction(&tmp, make_w, blind_len, gray_reverse, dir))	{	return false;	}
		sp = tmp.p.get();
		cnt = make_h / blind_len;
		if (make_h % blind_len)	{	cnt++;	}
		int y;
		if (dir == 0)	{
			y = make_h - blind_len;
			for (int i = 0; i < cnt; i++)	{
				tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, y);
				y -= blind_len;
			}
		}
		else	{
			y = 0;
			for (int i = 0; i < cnt; i++)	{
				tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, y);
				y += blind_len;
			}
		}
	}
	else	{
		if (!tnm_make_32bit_mask_for_direction(&tmp, blind_len, make_h, gray_reverse, dir))	{	return false;	}
		sp = tmp.p.get();
		cnt = make_w / blind_len;
		if (make_w % blind_len)	{	cnt++;	}
		int x;
		if (dir == 2)	{
			x = make_w - blind_len;
			for (int i = 0; i < cnt; i++)	{
				tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, x, 0);
				x -= blind_len;
			}
		}
		else	{
			x = 0;
			for (int i = 0; i < cnt; i++)	{
				tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, x, 0);
				x += blind_len;
			}
		}
	}
	return true;
}

bool tnm_make_32bit_mask_for_bothdir_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len)
{
	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	BYTE *dp;
	int ex1, ey1, ex2, ey2;

	S_mask_buffer tmp;
	BYTE *sp;

	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, 1, up_h, gray_reverse, 0, blind_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, 0);

		tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, 1, dw_h, gray_reverse, 1, blind_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, up_h);
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

		tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, lf_w, 1, gray_reverse, 2, blind_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, 0);

		tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, ri_w, 1, gray_reverse, 3, blind_len);
		sp = tmp.p.get();
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, 0);
	}

	return true;
}

bool tnm_make_mask_texture_for_bothdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len)
{
	blind_len = limit(2, blind_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir_blind(&buf, target_w, target_h, gray_reverse, dir, blind_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：両方向からストライプ
// ================================================================

bool tnm_make_32bit_mask_for_bothdir_stripe(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int stripe_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;

	int stripe_pitch = stripe_len * 2;

	int pal, cnt;

	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, 1, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		cnt = make_h / stripe_pitch;
		if (make_h % stripe_pitch)	{	cnt++;	}
		int y1 = 0;
		int y2 = stripe_len - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, 0, y1, 0, y2, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
			y1 += stripe_len;
			y2 += stripe_len;
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, 0, y1, 0, y2,       pal, gray_reverse, ex1, ey1, ex2, ey2);
			y1 += stripe_len;
			y2 += stripe_len;
		}
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, 1, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		cnt = make_w / stripe_pitch;
		if (make_w % stripe_pitch)	{	cnt++;	}
		int x1 = 0;
		int x2 = stripe_len - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x1, 0, x2, 0, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
			x1 += stripe_len;
			x2 += stripe_len;
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x1, 0, x2, 0,       pal, gray_reverse, ex1, ey1, ex2, ey2);
			x1 += stripe_len;
			x2 += stripe_len;
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_bothdir_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len)
{
	stripe_len = limit(1, stripe_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir_stripe(&buf, target_w, target_h, gray_reverse, dir, stripe_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：両方向からストライプ２
// ================================================================

bool tnm_make_32bit_mask_for_bothdir_stripe2(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int stripe_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;

	int pal, cnt;

	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, stripe_len * 2, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		cnt = make_h;
		int ax1 = 0;
		int ax2 = stripe_len - 1;
		int bx1 = stripe_len;
		int bx2 = stripe_len + stripe_len - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, ax1, i, ax2, i, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
			tnm_draw_32bit_line(dp, buf->w, buf->h, bx1, i, bx2, i,       pal, gray_reverse, ex1, ey1, ex2, ey2);
		}
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, make_w, stripe_len * 2, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
		cnt = make_w;
		int ay1 = 0;
		int ay2 = stripe_len - 1;
		int by1 = stripe_len;
		int by2 = stripe_len + stripe_len - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0, 0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, i, ay1, i, ay2, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
			tnm_draw_32bit_line(dp, buf->w, buf->h, i, by1, i, by2,       pal, gray_reverse, ex1, ey1, ex2, ey2);
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_bothdir_stripe2(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len)
{
	stripe_len = limit(1, stripe_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir_stripe2(&buf, target_w, target_h, gray_reverse, dir, stripe_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：両方向からブロックブラインド
// ================================================================

bool tnm_make_32bit_mask_for_bothdir_block_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int blind_len, int block_len)
{
	if (dir == 0)	{
		return tnm_make_32bit_mask_for_bothdir_stripe2(buf, make_w, blind_len, gray_reverse, dir, block_len);
	}
	else	{
		return tnm_make_32bit_mask_for_bothdir_stripe2(buf, blind_len, make_h, gray_reverse, dir, block_len);
	}
}

bool tnm_make_mask_texture_for_bothdir_block_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int blind_len, int block_len)
{
	blind_len = limit(2, blind_len, 128);
	block_len = limit(1, block_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_bothdir_block_blind(&buf, target_w, target_h, gray_reverse, dir, blind_len, block_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

