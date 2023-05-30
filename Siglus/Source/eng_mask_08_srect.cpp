#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// 小矩形の準備
void tnm_make_mask_texture_for_srect_ready(int *pat_w, int *pat_h, int pat_gap_mod)
{
	*pat_w = limit(4, *pat_w, 128);
	*pat_h = limit(4, *pat_h, 128);

	// 半ずらしの場合
	if (pat_gap_mod)	{
		*pat_w = *pat_w & 0xffffffe;		// 偶数値化
		*pat_h = *pat_h & 0xffffffe;		// 偶数値化
	}
}

// 小矩形の準備（４方向用）
void tnm_make_mask_texture_for_srect_crossdir_ready(int *pat_w, int *pat_h)
{
	tnm_make_mask_texture_for_srect_ready(pat_w, pat_h, 0);
	*pat_w = *pat_w * 2;	// 倍化
	*pat_h = *pat_h * 2;	// 倍化
}

// 半ずらし展開コピー
bool tnm_make_mask_texture_for_gap(S_mask_buffer *buf, int pat_w, int pat_h, S_mask_buffer *tmp)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, pat_w, pat_h * 2, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	BYTE *sp = tmp->p.get();

	tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp->w, tmp->h, 0, 0);
	tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp->w, tmp->h, -(pat_w / 2), pat_h);
	tc_draw_dib_easy(dp, buf->w, buf->h, sp, tmp->w, tmp->h, (pat_w / 2), pat_h);

	return true;
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の１周
// ================================================================

bool tnm_make_32bit_mask_for_srect_around_one(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_around_one(buf, pat_w, pat_h, gray_reverse, dir);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_around_one(&tmp, pat_w, pat_h, gray_reverse, dir)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_around_one(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod, dir))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の半周
// ================================================================

bool tnm_make_32bit_mask_for_srect_around_half(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_around_half(buf, pat_w, pat_h, gray_reverse, dir);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_around_half(&tmp, pat_w, pat_h, gray_reverse, dir)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_around_half(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod, dir))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の分割周
// ================================================================

bool tnm_make_32bit_mask_for_srect_around_divide(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int divide_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_around_divide(buf, pat_w, pat_h, gray_reverse, divide_mod);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_around_divide(&tmp, pat_w, pat_h, gray_reverse, divide_mod)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int divide_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	divide_mod = limit(0, divide_mod, 4);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_around_divide(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod, divide_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の中心扇
// ================================================================

bool tnm_make_32bit_mask_for_srect_oogi_center(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_oogi_center(buf, pat_w, pat_h, gray_reverse);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_oogi_center(&tmp, pat_w, pat_h, gray_reverse)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_oogi_center(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の角扇
// ================================================================

bool tnm_make_32bit_mask_for_srect_oogi_corner(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, bool is_distant)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_oogi_corner(buf, pat_w, pat_h, gray_reverse, dir, is_distant);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_oogi_corner(&tmp, pat_w, pat_h, gray_reverse, dir, is_distant)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, bool is_distant)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_oogi_corner(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod, dir, is_distant))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の縁扇
// ================================================================

bool tnm_make_32bit_mask_for_srect_oogi_edge(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, int oogi_mod, bool is_distant)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_oogi_edge(buf, pat_w, pat_h, gray_reverse, dir, oogi_mod, is_distant);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_oogi_edge(&tmp, pat_w, pat_h, gray_reverse, dir, oogi_mod, is_distant)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod, int dir, int oogi_mod, bool is_distant)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_oogi_edge(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod, dir, oogi_mod, is_distant))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の四角形
// ================================================================

bool tnm_make_32bit_mask_for_srect_square(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_square(buf, pat_w, pat_h, gray_reverse);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_square(&tmp, pat_w, pat_h, gray_reverse)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_square(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の菱形
// ================================================================

bool tnm_make_32bit_mask_for_srect_rhombus(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_rhombus(buf, pat_w, pat_h, gray_reverse);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_rhombus(&tmp, pat_w, pat_h, gray_reverse)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_rhombus(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の十字
// ================================================================

bool tnm_make_32bit_mask_for_srect_jyuuji(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_jyuuji(buf, pat_w, pat_h, gray_reverse, 0, 0);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_jyuuji(&tmp, pat_w, pat_h, gray_reverse, 0, 0)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_jyuuji(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形のテレビ
// ================================================================

bool tnm_make_32bit_mask_for_srect_television(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	if (pat_gap_mod == 0)	{
		return tnm_make_32bit_mask_for_television(buf, pat_w, pat_h, gray_reverse);
	}
	else	{
		S_mask_buffer tmp;
		if (!(tnm_make_32bit_mask_for_television(&tmp, pat_w, pat_h, gray_reverse)))	{	return false;	}
		return tnm_make_mask_texture_for_gap(buf, pat_w, pat_h, &tmp);
	}
}

bool tnm_make_mask_texture_for_srect_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int pat_gap_mod)
{
	tnm_make_mask_texture_for_srect_ready(&pat_w, &pat_h, pat_gap_mod);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_television(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, pat_gap_mod))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の４方向から
// ================================================================

bool tnm_make_32bit_mask_for_srect_crossdir(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h)
{
	return tnm_make_32bit_mask_for_crossdir(buf, pat_w, pat_h, gray_reverse);
}

bool tnm_make_mask_texture_for_srect_crossdir(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h)
{
	tnm_make_mask_texture_for_srect_crossdir_ready(&pat_w, &pat_h);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_crossdir(&buf, target_w, target_h, gray_reverse, pat_w, pat_h))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の４方向からスライス
// ================================================================

bool tnm_make_32bit_mask_for_srect_crossdir_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int slice_len)
{
	return tnm_make_32bit_mask_for_crossdir_slice(buf, pat_w, pat_h, gray_reverse, slice_len);
}

bool tnm_make_mask_texture_for_srect_crossdir_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int slice_len)
{
	tnm_make_mask_texture_for_srect_crossdir_ready(&pat_w, &pat_h);
	slice_len = limit(2, slice_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_crossdir_slice(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, slice_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：小矩形の４方向からブラインド
// ================================================================

bool tnm_make_32bit_mask_for_srect_crossdir_blind(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int pat_w, int pat_h, int blind_len)
{
	return tnm_make_32bit_mask_for_crossdir_blind(buf, pat_w, pat_h, gray_reverse, blind_len);
}

bool tnm_make_mask_texture_for_srect_crossdir_blind(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int pat_w, int pat_h, int blind_len)
{
	tnm_make_mask_texture_for_srect_crossdir_ready(&pat_w, &pat_h);
	blind_len = limit(2, blind_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_srect_crossdir_blind(&buf, target_w, target_h, gray_reverse, pat_w, pat_h, blind_len))	{	return false;	}
	return tnm_make_mask_texture_with_duplicate(texture, target_w, target_h, &buf);
}

