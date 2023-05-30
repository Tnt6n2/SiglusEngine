#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：１周
// ================================================================

int tnm_make_32bit_mask_for_around_len(int make_w, int make_h)
{
	int len = (make_w >= make_h) ? make_w + 64 : make_h + 64;		// + 64 = 余分

	double tmp = len * len * 2;
	tmp = sqrt(tmp);
	len = (int)tmp;

	return len;
}

void tnm_make_32bit_mask_for_around_ready(int make_w, int make_h, int *sx, int *sy, int *ex, int *ey)
{
	int len = tnm_make_32bit_mask_for_around_len(make_w / 2, make_h / 2);

	*(sx + 0) = (make_w / 2) - 1;
	*(sy + 0) = (make_h / 2) - 1;
	*(ex + 0) = *(sx + 0) - len;
	*(ey + 0) = *(sy + 0);

	*(sx + 1) = *(sx + 0) + 1;
	*(sy + 1) = *(sy + 0);
	*(ex + 1) = *(sx + 1);
	*(ey + 1) = *(sy + 1) - len;

	*(sx + 2) = *(sx + 0) + 1;
	*(sy + 2) = *(sy + 0) + 1;
	*(ex + 2) = *(sx + 2) + len;
	*(ey + 2) = *(sy + 2);

	*(sx + 3) = *(sx + 0);
	*(sy + 3) = *(sy + 0) + 1;
	*(ex + 3) = *(sx + 3);
	*(ey + 3) = *(sy + 3) + len;
}

void tnm_make_32bit_mask_for_around_func(S_mask_buffer *buf, bool gray_reverse, int sx, int sy, int ex, int ey, double sr, double er, int s_pal, int e_pal)
{
	BYTE *dp = buf->p.get();
	int ex1 = 0;
	int ey1 = 0;
	int ex2 = buf->w - 1;
	int ey2 = buf->h - 1;

	double dbl_ex = ex - sx;
	double dbl_ey = ey - sy;

	sr = (sr * 3.1415) / 180;
	er = (er * 3.1415) / 180;

	double r, sin_val, cos_val;
	int ex_now, ey_now;

	r = linear_limit(s_pal, s_pal, sr, e_pal, er);
	sin_val = sin(r);
	cos_val = cos(r);
	int ex_last = sx + (int)(((dbl_ex * cos_val) - (dbl_ey * sin_val)) + 0.5);
	int ey_last = sy + (int)(((dbl_ex * sin_val) + (dbl_ey * cos_val)) + 0.5);

	for (int i = s_pal; i <= e_pal; i++)	{
		r = linear_limit(i, s_pal, sr, e_pal, er);
		sin_val = sin(r);
		cos_val = cos(r);
		ex_now = sx + (int)(((dbl_ex * cos_val) - (dbl_ey * sin_val)) + 0.5);
		ey_now = sy + (int)(((dbl_ex * sin_val) + (dbl_ey * cos_val)) + 0.5);
		if (ex_last != ex_now || ey_last != ey_now)	{
			tnm_draw_32bit_fillpolygon(dp, buf->w, buf->h, sx, sy, ex_last, ey_last, ex_now, ey_now, 255 - i, gray_reverse, ex1, ey1, ex2, ey2);
			ex_last = ex_now;
			ey_last = ey_now;
		}
	}
}

bool tnm_make_32bit_mask_for_around_one(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (!(0 <= dir && dir < 4))	{	dir = 0;	}

	int sx[4], sy[4], ex[4], ey[4];
	tnm_make_32bit_mask_for_around_ready(make_w, make_h, sx, sy, ex, ey);

	int s_pal = 0;
	int e_pal = 63;
	for (int i = 0; i < 4; i++)	{
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[dir], sy[dir], ex[dir], ey[dir], 0, 90, s_pal, e_pal);
		s_pal = e_pal + 1;
		e_pal += 64;
		dir++;
		dir %= 4;
	}

	return true;
}

bool tnm_make_mask_texture_for_around_one(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_around_one(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：半周
// ================================================================

bool tnm_make_32bit_mask_for_around_half(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (!(0 <= dir && dir < 2))	{	dir = 0;	}

	int sx[4], sy[4], ex[4], ey[4];
	tnm_make_32bit_mask_for_around_ready(make_w, make_h, sx, sy, ex, ey);

	if (dir == 0)	{
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[0], sy[0], ex[0], ey[0], 0, 90,   0, 127);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[1], sy[1], ex[1], ey[1], 0, 90, 128, 255);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[2], sy[2], ex[2], ey[2], 0, 90,   0, 127);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[3], sy[3], ex[3], ey[3], 0, 90, 128, 255);
	}
	else	{
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[0], sy[0], ex[0], ey[0], 0, 90, 128, 255);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[1], sy[1], ex[1], ey[1], 0, 90,   0, 127);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[2], sy[2], ex[2], ey[2], 0, 90, 128, 255);
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[3], sy[3], ex[3], ey[3], 0, 90,   0, 127);
	}

	return true;
}

bool tnm_make_mask_texture_for_around_half(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_around_half(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：分割周
// ================================================================

bool tnm_make_32bit_mask_for_around_divide(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int divide_mod)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int sx[4], sy[4], ex[4], ey[4];
	tnm_make_32bit_mask_for_around_ready(make_w, make_h, sx, sy, ex, ey);

	int divide_cnt = 1 << divide_mod;
	double r_add = 90.0 / divide_cnt;

	for (int i = 0; i < 4; i++)	{
		double sr = 0;
		double er = r_add;
		for (int j = 0; j < divide_cnt; j++)	{
			tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[i], sy[i], ex[i], ey[i], sr, er, 0, 255);
			sr = er;
			er += r_add;
		}
	}

	return true;
}

bool tnm_make_mask_texture_for_around_divide(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int divide_mod)
{
	divide_mod = limit(0, divide_mod, 4);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_around_divide(&buf, target_w, target_h, gray_reverse, divide_mod))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：中心扇
// ================================================================

bool tnm_make_32bit_mask_for_oogi_center(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int sx[4], sy[4], ex[4], ey[4];
	tnm_make_32bit_mask_for_around_ready(make_w, make_h, sx, sy, ex, ey);

	tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[0], sy[0], ex[0], ey[0], 0, 90, 0, 255);
	tnm_make_32bit_mask_for_around_func(buf, gray_reverse ? false : true, sx[1], sy[1], ex[1], ey[1], 0, 90, 0, 255);
	tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[2], sy[2], ex[2], ey[2], 0, 90, 0, 255);
	tnm_make_32bit_mask_for_around_func(buf, gray_reverse ? false : true, sx[3], sy[3], ex[3], ey[3], 0, 90, 0, 255);

	return true;
}

bool tnm_make_mask_texture_for_oogi_center(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_oogi_center(&buf, target_w, target_h, gray_reverse))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：角扇
// ================================================================

bool tnm_make_32bit_mask_for_oogi_clip(S_mask_buffer *buf, int sx, int sy, int ex, int ey, double *ret_sr, double *ret_er)
{
	int ex1 = 0;
	int ey1 = 0;
	int ex2 = buf->w - 1;
	int ey2 = buf->h - 1;

	double dbl_ex = ex - sx;
	double dbl_ey = ey - sy;

	double sr = ( 0 * 3.1415) / 180;
	double er = (90 * 3.1415) / 180;

	double r, sin_val, cos_val;

	int cx1, cy1, cx2, cy2;
	bool first_clip = false;
	bool ret = false;
	*ret_sr = 0;
	*ret_er = 90;
	for (int i = 0; i <= 90; i++)	{
		cx1 = sx;
		cy1 = sy;
		r = linear_limit(i, 0, sr, 90, er);
		sin_val = sin(r);
		cos_val = cos(r);
		cx2 = sx + (int)(((dbl_ex * cos_val) - (dbl_ey * sin_val)) + 0.5);
		cy2 = sy + (int)(((dbl_ex * sin_val) + (dbl_ey * cos_val)) + 0.5);
		if (first_clip == false)	{
			if (tnm_clip_line(&cx1, &cy1, &cx2, &cy2, ex1, ey1, ex2, ey2))	{
				*ret_sr = (i == 0) ? 0 : i - 1;
				first_clip = true;
				ret = true;
			}
		}
		else	{
			if (!tnm_clip_line(&cx1, &cy1, &cx2, &cy2, ex1, ey1, ex2, ey2))	{
				*ret_er = i;
				break;
			}
		}
	}
	return ret;
}

bool tnm_make_32bit_mask_for_oogi_corner(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, bool is_distant)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (!(0 <= dir && dir < 4))	{	dir = 0;	}

	int distant_len = 0;
	if (is_distant)	{
		distant_len = (make_w < make_h) ? make_w : make_h;
	}

	int len = tnm_make_32bit_mask_for_around_len(make_w + distant_len, make_h + distant_len);

	int sx, sy, ex, ey;
	switch (dir)	{
		case 0:
			sx = 0 - distant_len;
			sy = 0 - distant_len;
			ex = sx + len;
			ey = sy;
			break;
		case 1:
			sx = make_w - 1 + distant_len;
			sy = 0 - distant_len;
			ex = sx;
			ey = sy + len;
			break;
		case 2:
			sx = 0 - distant_len;
			sy = make_h - 1 + distant_len;
			ex = sx;
			ey = sy - len;
			break;
		case 3:
			sx = make_w - 1 + distant_len;
			sy = make_h - 1 + distant_len;
			ex = sx - len;
			ey = sy;
			break;
	}

	double sr = 0;
	double er = 90;
	if (is_distant)	{	tnm_make_32bit_mask_for_oogi_clip(buf, sx, sy, ex, ey, &sr, &er);	}
	tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx, sy, ex, ey, sr, er, 0, 255);

	return true;
}

bool tnm_make_mask_texture_for_oogi_corner(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, bool is_distant)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_oogi_corner(&buf, target_w, target_h, gray_reverse, dir, is_distant))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：縁扇
// ================================================================

bool tnm_make_32bit_mask_for_oogi_edge(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (!(0 <= dir && dir < 4))	{	dir = 0;	}

	int distant_len, len, a_sx, a_sy, a_ex, a_ey, b_sx, b_sy, b_ex, b_ey;
	switch (dir)	{
		case 0:
			distant_len = (is_distant) ? make_h : 0;
			len = tnm_make_32bit_mask_for_around_len(make_w / 2, make_h + distant_len);
			a_sx = make_w / 2;
			a_sy = 0 - distant_len;
			a_ex = a_sx + len;
			a_ey = a_sy;
			b_sx = a_sx - 1;
			b_sy = a_sy;
			b_ex = b_sx;
			b_ey = b_sy + len;
			break;
		case 1:
			distant_len = (is_distant) ? make_h : 0;
			len = tnm_make_32bit_mask_for_around_len(make_w / 2, make_h + distant_len);
			a_sx = (make_w / 2) - 1;
			a_sy = make_h - 1 + distant_len;
			a_ex = a_sx - len;
			a_ey = a_sy;
			b_sx = a_sx + 1;
			b_sy = a_sy;
			b_ex = b_sx;
			b_ey = b_sy - len;
			break;
		case 2:
			distant_len = (is_distant) ? make_w : 0;
			len = tnm_make_32bit_mask_for_around_len(make_w + distant_len, make_h / 2);
			a_sx = 0 - distant_len;
			a_sy = (make_h / 2) - 1;
			a_ex = a_sx;
			a_ey = a_sy - len;
			b_sx = a_sx;
			b_sy = a_sy + 1;
			b_ex = b_sx + len;
			b_ey = b_sy;
			break;
		case 3:
			distant_len = (is_distant) ? make_w : 0;
			len = tnm_make_32bit_mask_for_around_len(make_w + distant_len, make_h / 2);
			a_sx = make_w - 1 + distant_len;
			a_sy = make_h / 2;
			a_ex = a_sx;
			a_ey = a_sy + len;
			b_sx = a_sx;
			b_sy = a_sy - 1;
			b_ex = b_sx - len;
			b_ey = b_sy;
			break;
	}

	double sr = 0;
	double er = 90;

	if (oogi_mod == 0)	{
		if (is_distant)	{	tnm_make_32bit_mask_for_oogi_clip(buf, a_sx, a_sy, a_ex, a_ey, &sr, &er);	}
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, a_sx, a_sy, a_ex, a_ey, sr, er,   0, 127);
		if (is_distant)	{	tnm_make_32bit_mask_for_oogi_clip(buf, b_sx, b_sy, b_ex, b_ey, &sr, &er);	}
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, b_sx, b_sy, b_ex, b_ey, sr, er, 128, 255);
	}
	else	{
		if (is_distant)	{	tnm_make_32bit_mask_for_oogi_clip(buf, a_sx, a_sy, a_ex, a_ey, &sr, &er);	}
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse, a_sx, a_sy, a_ex, a_ey, sr, er,   0, 255);
		if (is_distant)	{	tnm_make_32bit_mask_for_oogi_clip(buf, b_sx, b_sy, b_ex, b_ey, &sr, &er);	}
		tnm_make_32bit_mask_for_around_func(buf, gray_reverse ? false : true, b_sx, b_sy, b_ex, b_ey, sr, er, 0, 255);
	}

	return true;
}

bool tnm_make_mask_texture_for_oogi_edge(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int oogi_mod, bool is_distant)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_oogi_edge(&buf, target_w, target_h, gray_reverse, dir, oogi_mod, is_distant))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：１周スライス
// ================================================================

#if 0	// 未使用

bool tnm_make_32bit_mask_for_around_one_slice(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	if (!(0 <= dir && dir < 4))	{	dir = 0;	}

	int sx[4], sy[4], ex[4], ey[4];
	tnm_make_32bit_mask_for_around_ready(make_w, make_h, sx, sy, ex, ey);

	int divide_cnt = 8;
	double r_add = 90.0 / divide_cnt;

	int draw_cnt = 4 * divide_cnt;

	int s_pal_ind = 0;
	int s_pal, e_pal;

	for (int i = 0; i < 4; i++)	{
		double sr = 0;
		double er = r_add;
		for (int j = 0; j < divide_cnt; j++)	{
			s_pal = linear_limit(s_pal_ind, 0, 0, draw_cnt, 143);
			e_pal = s_pal + 255 - 143;
			tnm_make_32bit_mask_for_around_func(buf, gray_reverse, sx[dir], sy[dir], ex[dir], ey[dir], sr, er, s_pal, e_pal);
			sr = er;
			er += r_add;
			s_pal_ind++;
		}
		dir++;
		dir %= 4;
	}

	return true;
}

bool tnm_make_mask_texture_for_around_one_slice(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_around_one_slice(&buf, target_w, target_h, gray_reverse, dir))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

#endif