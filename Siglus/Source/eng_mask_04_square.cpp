#include	"pch.h"
#include	"engine/eng_mask_wipe.h"

// ****************************************************************
// マスクテクスチャ作成：四角形
// ================================================================

bool tnm_make_32bit_mask_for_square(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int cnt = (make_w >= make_h) ? (make_w + 1) / 2 : (make_h + 1) / 2;

	int ax1 = 0;
	int ay1 = 0;
	int ax2 = make_w - 1;
	int ay2 = make_h - 1;

	int bx1 = (make_w / 2) - 1;
	int by1 = (make_h / 2) - 1;
	int bx2 = bx1 + 1;
	int by2 = by1 + 1;

	int pal, x1, y1, x2, y2;

	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 255);
		x1  = linear_limit(i, 0, ax1, cnt - 1, bx1);
		y1  = linear_limit(i, 0, ay1, cnt - 1, by1);
		x2  = linear_limit(i, 0, ax2, cnt - 1, bx2);
		y2  = linear_limit(i, 0, ay2, cnt - 1, by2);
		tnm_draw_32bit_empty_box(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
	}

	return true;
}

bool tnm_make_mask_texture_for_square(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_square(&buf, target_w, target_h, gray_reverse))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：菱形
// ================================================================

bool tnm_make_32bit_mask_for_rhombus(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int x1, y1, x2, y2, cnt, pal;

	int x_len = make_w / 2;
	int y_len = make_h / 2;
	if (x_len && y_len)	{
		cnt = x_len + y_len - 1;
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0,   0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 < (x_len - 1))	{	x1++;	}		else	{	y1++;	}
			if (y2 < (y_len - 1))	{	y2++;	}		else	{	x2++;	}
		}
	}

	x_len = (make_w + 1) / 2;
	y_len = make_h / 2;
	if (x_len && y_len)	{
		cnt = x_len + y_len - 1;
		x1 = make_w - 1;
		y1 = 0;
		x2 = make_w - 1;
		y2 = 0;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0,   0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 > (make_w - x_len))	{	x1--;	}		else	{	y1++;	}
			if (y2 < (y_len - 1))		{	y2++;	}		else	{	x2--;	}
		}
	}

	x_len = make_w / 2;
	y_len = (make_h + 1) / 2;
	if (x_len && y_len)	{
		cnt = x_len + y_len - 1;
		x1 = 0;
		y1 = make_h - 1;
		x2 = 0;
		y2 = make_h - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0,   0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 < (x_len - 1))		{	x1++;	}		else	{	y1--;	}
			if (y2 > (make_h - y_len))	{	y2--;	}		else	{	x2++;	}
		}
	}

	x_len = (make_w + 1) / 2;
	y_len = (make_h + 1) / 2;
	if (x_len && y_len)	{
		cnt = x_len + y_len - 1;
		x1 = make_w - 1;
		y1 = make_h - 1;
		x2 = make_w - 1;
		y2 = make_h - 1;
		for (int i = 0; i < cnt; i++)	{
			pal = linear_limit(i, 0,   0, cnt - 1, 255);
			tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y2, 255 - pal, gray_reverse, -1, -1, -1, -1);
			if (x1 > (make_w - x_len))	{	x1--;	}		else	{	y1--;	}
			if (y2 > (make_h - y_len))	{	y2--;	}		else	{	x2--;	}
		}
	}

	return true;
}

bool tnm_make_mask_texture_for_rhombus(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_rhombus(&buf, target_w, target_h, gray_reverse))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：十字
// ================================================================

bool tnm_make_32bit_mask_for_jyuuji(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int center_x, int center_y)
{
	if (center_x == 0)	{	center_x = (make_w / 2) - 1;	}
	if (center_y == 0)	{	center_y = (make_h / 2) - 1;	}

	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int len = 0;

	int tmp;

	if (center_x > buf->w)	{
		tmp = center_x;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}
	if (center_x < 0)	{
		tmp = buf->w - center_x;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}
	else	{
		tmp = center_x;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
		tmp = buf->w - center_x;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}
	if (center_y > buf->h)	{
		tmp = center_y;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}
	if (center_y < 0)	{
		tmp = buf->h - center_y;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}
	else	{
		tmp = center_y;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
		tmp = buf->h - center_y;
		if (tmp < 0)	{	tmp = -tmp;	}
		if (tmp > len)	{	len = tmp;	}
	}

	int cnt = len;
	if (cnt == 0)	{	cnt = 1;	}

	int x1, y1, x2, y2, pal;

	x2 = center_x;
	y2 = center_y;
	x1 = x2 - len;
	y1 = y2 - len;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 255);
		tnm_draw_32bit_empty_box(dp, buf->w, buf->h, x1, y1, x2, y2, pal, gray_reverse, -1, -1, -1, -1);
		x2--;	y2--;
	}

	x2 = center_x + 1;
	y2 = center_y;
	x1 = x2 + len;
	y1 = y2 - len;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 255);
		tnm_draw_32bit_empty_box(dp, buf->w, buf->h, x1, y1, x2, y2, pal, gray_reverse, -1, -1, -1, -1);
		x2++;	y2--;
	}

	x2 = center_x;
	y2 = center_y + 1;
	x1 = x2 - len;
	y1 = y2 + len;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 255);
		tnm_draw_32bit_empty_box(dp, buf->w, buf->h, x1, y1, x2, y2, pal, gray_reverse, -1, -1, -1, -1);
		x2--;	y2++;
	}

	x2 = center_x + 1;
	y2 = center_y + 1;
	x1 = x2 + len;
	y1 = y2 + len;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 255);
		tnm_draw_32bit_empty_box(dp, buf->w, buf->h, x1, y1, x2, y2, pal, gray_reverse, -1, -1, -1, -1);
		x2++;	y2++;
	}

	return true;
}

bool tnm_make_mask_texture_for_jyuuji(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_jyuuji(&buf, target_w, target_h, gray_reverse, 0, 0))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：テレビ
// ================================================================

bool tnm_make_32bit_mask_for_television(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(buf, make_w, make_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}

	int cnt = (make_h * 1) / 16;
	if (cnt == 0)	{	cnt = 1;	}

	int pal;

	int x1 = 0;
	int y1 = 0;
	int x2 = make_w - 1;
	int y2 = make_h - 1;
	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0,   0, cnt - 1, 15);
		tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y1, x2, y1, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
		tnm_draw_32bit_line(dp, buf->w, buf->h, x1, y2, x2, y2, 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
		y1++;	y2--;
	}

	int y_alpha = (make_h - (cnt * 2)) / 2;
	int y_add;

	cnt = make_w / 2;
	if (cnt & 0x01)	{	cnt++;	}

	for (int i = 0; i < cnt; i++)	{
		pal = linear_limit(i, 0, 16, cnt - 1, 255);
		y_add = linear_limit(i * 3, 0, 0, cnt - 1, y_alpha);
		tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x1, (y1 + y_add), x2, (y2 - y_add), 255 - pal, gray_reverse, ex1, ey1, ex2, ey2);
		x1++;	x2--;
	}
	return true;
}

bool tnm_make_mask_texture_for_television(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse)
{
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_television(&buf, target_w, target_h, gray_reverse))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

