#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：ランダム
// ================================================================

bool tnm_make_32bit_mask_for_random(S_mask_buffer *buf, int make_w, int make_h, int pat_w, int pat_h)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int i, x, y, xx, yy;

	int w_pat_cnt = (buf->w + (pat_w - 1)) / pat_w;
	int h_pat_cnt = (buf->h + (pat_h - 1)) / pat_h;

	int wh_cnt = w_pat_cnt * h_pat_cnt;

	BUFFER gray_buf;
	gray_buf.resize(wh_cnt);
	BYTE *gbp = gray_buf.get();
	for (i = 0; i < wh_cnt; i++)	{
		*gbp++ = (BYTE)linear_limit(i, 0, 000, wh_cnt - 1, 255);
	}

	std::random_shuffle(gray_buf.begin(), gray_buf.end());

	gbp = gray_buf.get();
	yy = 0;
	for (y = 0; y < h_pat_cnt; y++)	{
		xx = 0;
		for (x = 0; x < w_pat_cnt; x++)	{
			tnm_draw_32bit_fill_box(dp, buf->w, buf->h, xx, yy, xx + pat_w - 1, yy + pat_h - 1, *gbp, false, ex1, ey1, ex2, ey2);
			xx += pat_w;
			gbp++;
		}
		yy += pat_h;
	}
	return true;
}

bool tnm_make_mask_texture_for_random(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h)
{
	pat_w = limit(1, pat_w, 128);
	pat_h = limit(1, pat_h, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_random(&buf, target_w, target_h, pat_w, pat_h))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：ランダムライン
// ================================================================

bool tnm_make_32bit_mask_for_random_line(S_mask_buffer *buf, int make_w, int make_h, int dir, int line_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	S_mask_buffer tmp;
	if (dir == 0)	{
		if (!tnm_make_32bit_mask_for_random(&tmp, make_w, 1, line_len, 1))	{	return false;	}
		BYTE *sp = tmp.p.get();
		for (int i = 0; i < make_h; i++)	{
			tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
		}
	}
	else	{
		if (!tnm_make_32bit_mask_for_random(&tmp, 1, make_h, 1, line_len))	{	return false;	}
		BYTE *sp = tmp.p.get();
		for (int i = 0; i < make_w; i++)	{
			tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, i, 0);
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_random_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len)
{
	line_len = limit(1, line_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_random_line(&buf, target_w, target_h, dir, line_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：ランダム斜めライン
// ================================================================

bool tnm_make_32bit_mask_for_random_slant_line(S_mask_buffer *buf, int make_w, int make_h, int dir, int line_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	S_mask_buffer tmp;
	int tmp_len = make_w + (make_h - 1);
	if (!tnm_make_32bit_mask_for_random(&tmp, tmp_len, 1, line_len, 1))	{	return false;	}
	BYTE *sp = tmp.p.get();

	if (dir == 0)	{
		for (int i = 0; i < make_h; i++)	{
			tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
			sp += 4;
		}
	}
	else	{
		sp += (make_h - 1) * 4;
		for (int i = 0; i < make_h; i++)	{
			tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
			sp -= 4;
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_random_slant_line(BSP<C_d3d_texture> *texture, int target_w, int target_h, int dir, int line_len)
{
	line_len = limit(1, line_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_random_slant_line(&buf, target_w, target_h, dir, line_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：ランダムクロス
// ================================================================

bool tnm_make_32bit_mask_for_random_cross(S_mask_buffer *buf, int make_w, int make_h, int pat_w, int pat_h)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	S_mask_buffer tmp;
	if (!tnm_make_32bit_mask_for_random(&tmp, 1, make_h, 1, pat_h))	{	return false;	}
	BYTE *sp = tmp.p.get();

	for (int i = 0; i < make_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, i, 0);
	}

	if (!tnm_make_32bit_mask_for_random(&tmp, make_w, 1, pat_w, 1))	{	return false;	}
	sp = tmp.p.get();

	DWORD *dw_dp = (DWORD *)dp;
	DWORD *dw_sp;

	for (int i = 0; i < make_h; i++)	{
		dw_sp = (DWORD *)sp;
		for (int j = 0; j < make_w; j++)	{
			if (*dw_dp < *dw_sp)	{	*dw_dp = *dw_sp;	}
			dw_dp++;	dw_sp++;
		}
	}
	return true;
}

bool tnm_make_mask_texture_for_random_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int pat_w, int pat_h)
{
	pat_w = limit(1, pat_w, 128);
	pat_h = limit(1, pat_h, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_random_cross(&buf, target_w, target_h, pat_w, pat_h))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：ランダム斜めクロス
// ================================================================

bool tnm_make_32bit_mask_for_random_slant_cross(S_mask_buffer *buf, int make_w, int make_h, int line_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	S_mask_buffer tmp;
	int tmp_len = make_w + (make_h - 1);
	if (!tnm_make_32bit_mask_for_random(&tmp, tmp_len, 1, line_len, 1))	{	return false;	}
	BYTE *sp = tmp.p.get();

	for (int i = 0; i < make_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
		sp += 4;
	}

	if (!tnm_make_32bit_mask_for_random(&tmp, tmp_len, 1, line_len, 1))	{	return false;	}
	sp = tmp.p.get();
	sp += (make_h - 1) * 4;

	DWORD *dw_dp = (DWORD *)dp;
	DWORD *dw_sp;

	for (int i = 0; i < make_h; i++)	{
		dw_sp = (DWORD *)sp;
		for (int j = 0; j < make_w; j++)	{
			if (*dw_dp < *dw_sp)	{	*dw_dp = *dw_sp;	}
			dw_dp++;	dw_sp++;
		}
		sp -= 4;
	}
	return true;
}

bool tnm_make_mask_texture_for_random_slant_cross(BSP<C_d3d_texture> *texture, int target_w, int target_h, int line_len)
{
	line_len = limit(1, line_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_random_slant_cross(&buf, target_w, target_h, line_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

