#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：４方向から
// ================================================================

bool tnm_make_32bit_mask_for_crossdir(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	S_mask_buffer tmp;
	tnm_make_32bit_mask_for_direction(&tmp, lf_w, 1, gray_reverse, 2);
	BYTE *sp = tmp.p.get();
	for (int i = 0; i < up_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
	}

	tnm_make_32bit_mask_for_direction(&tmp, ri_w, 1, gray_reverse, 3);
	sp = tmp.p.get();
	for (int i = 0; i < dw_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, up_h + i);
	}

	tnm_make_32bit_mask_for_direction(&tmp, 1, up_h, gray_reverse, 0);
	sp = tmp.p.get();
	for (int i = 0; i < ri_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w + i, 0);
	}

	tnm_make_32bit_mask_for_direction(&tmp, 1, dw_h, gray_reverse, 1);
	sp = tmp.p.get();
	for (int i = 0; i < lf_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, i, up_h);
	}

	return true;
}

bool tnm_make_mask_texture_for_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_crossdir(&buf, target_w, target_h, gray_reverse))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：４方向からスライス
// ================================================================

bool tnm_make_32bit_mask_for_crossdir_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int slice_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	S_mask_buffer tmp;
	int alpha = (slice_len - (lf_w % slice_len)) % slice_len;
	tnm_make_32bit_mask_for_direction_slice(&tmp, lf_w + alpha, 1, gray_reverse, 2, slice_len);
	BYTE *sp = tmp.p.get();
	for (int i = 0; i < up_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0 - alpha, i);
	}

	alpha = (slice_len - (ri_w % slice_len)) % slice_len;
	tnm_make_32bit_mask_for_direction_slice(&tmp, ri_w + alpha, 1, gray_reverse, 3, slice_len);
	sp = tmp.p.get();
	for (int i = 0; i < dw_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, up_h + i);
	}

	alpha = (slice_len - (up_h % slice_len)) % slice_len;
	tnm_make_32bit_mask_for_direction_slice(&tmp, 1, up_h + alpha, gray_reverse, 0, slice_len);
	sp = tmp.p.get();
	for (int i = 0; i < ri_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w + i, 0 - alpha);
	}

	alpha = (slice_len - (dw_h % slice_len)) % slice_len;
	tnm_make_32bit_mask_for_direction_slice(&tmp, 1, dw_h + alpha, gray_reverse, 1, slice_len);
	sp = tmp.p.get();
	for (int i = 0; i < lf_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, i, up_h);
	}

	return true;
}

bool tnm_make_mask_texture_for_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int slice_len)
{
	slice_len = limit(2, slice_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_crossdir_slice(&buf, target_w, target_h, gray_reverse, slice_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：４方向からブラインド
// ================================================================

bool tnm_make_32bit_mask_for_crossdir_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int blind_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int lf_w = make_w / 2;
	int ri_w = (make_w + 1) / 2;
	int up_h = make_h / 2;
	int dw_h = (make_h + 1) / 2;

	S_mask_buffer tmp;
	tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, lf_w, 1, gray_reverse, 2, blind_len);
	BYTE *sp = tmp.p.get();
	for (int i = 0; i < up_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, 0, i);
	}

	tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, ri_w, 1, gray_reverse, 3, blind_len);
	sp = tmp.p.get();
	for (int i = 0; i < dw_h; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w, up_h + i);
	}

	tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, 1, up_h, gray_reverse, 0, blind_len);
	sp = tmp.p.get();
	for (int i = 0; i < ri_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, lf_w + i, 0);
	}

	tnm_make_32bit_mask_for_bothdir_blind_func(&tmp, 1, dw_h, gray_reverse, 1, blind_len);
	sp = tmp.p.get();
	for (int i = 0; i < lf_w; i++)	{
		tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp.w, tmp.h, i, up_h);
	}

	return true;
}

bool tnm_make_mask_texture_for_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int blind_len)
{
	blind_len = limit(2, blind_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_crossdir_blind(&buf, target_w, target_h, gray_reverse, blind_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

