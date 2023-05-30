#include	"pch.h"
#include	"engine/eng_mask.h"

// ****************************************************************
// ‚R‚Qƒrƒbƒg‰ñ“]Šg‘åk¬•`‰æ
// ================================================================

// ‚r‚h‚m^‚b‚n‚rƒe[ƒuƒ‹ì¬
static bool tnm_draw_32bit_mask_rotate_sin_cos_table_create_flag = false;
static double tnm_draw_32bit_mask_rotate_sin_table[3600];
static double tnm_draw_32bit_mask_rotate_cos_table[3600];
void tnm_draw_32bit_mask_func_create_sin_cos_table(void){
	double rt1 = 0.0;
	double rt2;
	for (int i = 0; i < 3600; i++)	{
		rt2 = (rt1 * 3.1415) / 180;
		tnm_draw_32bit_mask_rotate_sin_table[i] = sin(rt2);
		tnm_draw_32bit_mask_rotate_cos_table[i] = cos(rt2);
		rt1 += 0.1;
	}
	tnm_draw_32bit_mask_rotate_sin_cos_table_create_flag = true;
}

#define		GRPMAC_CLRADD				\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, edx			\
__asm		add		eax, [ebx]

#define		GRPMAC_GETCLRSET_B			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi]			\
__asm		mov		al, [edi]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi], al

#define		GRPMAC_GETCLRSET_G			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi+1]			\
__asm		mov		al, [edi+1]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi+1], al

#define		GRPMAC_GETCLRSET_R			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi+2]			\
__asm		mov		al, [edi+2]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi+2], al

// ’á•i¿
void tnm_draw_32bit_mask_rotate_func_low_quality(
	int sx1, int sy1, int sx2, int sy2,
	double dbl_src_line_x1, double dbl_src_line_y1, double dbl_src_line_x2, double dbl_src_line_y2,
	BYTE *srcp, int sxl, int syl,
	BYTE *dstp, double dbl_line_add_x, double dbl_line_add_y, int pixel_cnt, int dummy
){
	double dbl_src_x = dbl_src_line_x1;
	double dbl_src_y = dbl_src_line_y1;

	double dbl_8 = 8;
	double dbl_05 = 0.5;
	double dbl_ix, dbl_iy;

	int sxl4 = sxl << 2;

	int left_limit = -1;
	int top_limit = -1;
	int right_limit = sxl - 1;
	int bottom_limit = syl - 1;

	// ƒ\[ƒX”ÍˆÍ‚Ì•â³
	if (sx1 < left_limit)	{	sx1 = left_limit;	}
	if (sx2 > right_limit)	{	sx2 = right_limit;	}
	if (sy1 < top_limit)	{	sy1 = top_limit;	}
	if (sy2 > bottom_limit)	{	sy2 = bottom_limit;	}

	__asm{
		mov		ecx, pixel_cnt
		mov		edx, srcp
		mov		edi, dstp

	__loop_top:

		// if (dbl_src_x >= sx1 && dbl_src_x <= sx2 && dbl_src_y >= sy1 && dbl_src_y <= sy2)	{	}
		// ix = (int)dbl_src_x;
		fld		dbl_src_x
		fsub	dbl_05
		fistp	dbl_ix
		mov		ebx, dword ptr dbl_ix		// ebx = ix
		cmp		ebx, sx1
		jl		__draw_end
		cmp		ebx, sx2
		jg		__draw_end

		// iy = (int)dbl_src_y;
		fld		dbl_src_y
		fsub	dbl_05
		fistp	dbl_iy
		mov		eax, dword ptr dbl_iy		// eax = iy
		cmp		eax, sy1
		jl		__draw_end
		cmp		eax, sy2
		jg		__draw_end

		// ƒhƒbƒg‚ÌF‚ğæ“¾^İ’è
		imul	eax, sxl
		add		eax, ebx
		shl		eax, 2
		mov		esi, edx
		add		esi, eax
		mov		eax, [esi]		// ƒsƒNƒZƒ‹’læ“¾
		mov		[edi], eax		// ƒsƒNƒZƒ‹’lİ’è

	__draw_end:

		dec		ecx
		jz		__draw_end_end

		// dbl_src_x += dbl_line_add_x;
		fld		dbl_src_x
		fadd	dbl_line_add_x
		fstp	dbl_src_x

		// dbl_src_y += dbl_line_add_y;
		fld		dbl_src_y
		fadd	dbl_line_add_y
		fstp	dbl_src_y

		// dword_dp++;
		add		edi, 4

		jmp		__loop_top
	__draw_end_end:
	}
}

// ‚•i¿
void tnm_draw_32bit_mask_rotate_func_high_quality(
	int sx1, int sy1, int sx2, int sy2,
	double dbl_src_line_x1, double dbl_src_line_y1, double dbl_src_line_x2, double dbl_src_line_y2,
	BYTE *srcp, int sxl, int syl,
	BYTE *dstp, double dbl_line_add_x, double dbl_line_add_y, int pixel_cnt, int dummy
){
	double dbl_src_x = dbl_src_line_x1;
	double dbl_src_y = dbl_src_line_y1;

	double dbl_8 = 8;
	double dbl_05 = 0.5;
	double dbl_ix, dbl_iy;

	int sxl4 = sxl << 2;

	int left_limit = -1;
	int top_limit = -1;
	int right_limit = sxl - 1;
	int bottom_limit = syl - 1;

	// ƒ\[ƒX”ÍˆÍ‚Ì•â³
	sx1 -= 1;
	sy1 -= 1;
	if (sx1 < left_limit)	{	sx1 = left_limit;	}
	if (sx2 > right_limit)	{	sx2 = right_limit;	}
	if (sy1 < top_limit)	{	sy1 = top_limit;	}
	if (sy2 > bottom_limit)	{	sy2 = bottom_limit;	}

	__asm{
		mov		ecx, pixel_cnt
		mov		edi, dstp

	__loop_top:

		// if (dbl_src_x >= sx1 && dbl_src_x <= sx2 && dbl_src_y >= sy1 && dbl_src_y <= sy2)	{	}
		// ix = (int)dbl_src_x;
		fld		dbl_src_x
		fsub	dbl_05
		fistp	dbl_ix
		mov		ebx, dword ptr dbl_ix		// ebx = ix
		cmp		ebx, sx1
		jl		__draw_end
		cmp		ebx, sx2
		jg		__draw_end

		// iy = (int)dbl_src_y;
		fld		dbl_src_y
		fsub	dbl_05
		fistp	dbl_iy
		mov		eax, dword ptr dbl_iy		// eax = iy
		cmp		eax, sy1
		jl		__draw_end
		cmp		eax, sy2
		jg		__draw_end

		// ‚±‚±‚©‚çƒsƒNƒZƒ‹ˆ—
		push	ecx
		mov		ecx, eax					// ecx = iy

		// ‹ß–T‚Ìƒhƒbƒg‚ÌF‚ğ“¾‚é
		imul	eax, sxl
		add		eax, ebx			// ebx = ix
		shl		eax, 2
		add		eax, srcp
		add		eax, 3				// šƒ¿¬•ªi“§–¾“x‚ÌƒoƒCƒgî•ñj‚ğæ“¾‚·‚é‚Ì‚Å‚·
		mov		esi, eax
		mov		edx, eax
		add		edx, 4				// š‚P‚Â‰E‘¤‚Ìƒhƒbƒg‚ğæ“¾‚·‚é‚Ì‚Å‚·

		xor		eax, eax

		cmp		ebx, left_limit			// ebx = ix
		jle		__get_pixel_left_side
		cmp		ebx, right_limit		// ebx = ix
		jge		__get_pixel_right_side
		cmp		ecx, top_limit			// ecx = iy
		jle		__get_pixel_midle_top_side
		cmp		ecx, bottom_limit		// ecx = iy
		jge		__get_pixel_midle_bottom_side

		// œœ
		// œœ
		mov		al, [esi]
		mov		ebx, eax
		mov		al, [edx]
		add		ebx, eax
		add		esi, sxl4
		mov		al, [esi]
		add		ebx, eax
		add		edx, sxl4
		mov		al, [edx]
		add		ebx, eax
		shr		ebx, 2					// €‚S
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_midle_top_side:
		// ››
		// œœ
		add		esi, sxl4
		mov		al, [esi]
		mov		ebx, eax
		add		edx, sxl4
		mov		al, [edx]
		add		ebx, eax
		shr		ebx, 1					// €‚Q
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_midle_bottom_side:
		// œœ
		// ››
		mov		al, [esi]
		mov		ebx, eax
		mov		al, [edx]
		add		ebx, eax
		shr		ebx, 1					// €‚Q
		jmp		__get_pixel_end			// I‚í‚èI

	//------------------------------------------------------------------------------------
	__get_pixel_left_side:
		cmp		ecx, top_limit			// ecx = iy
		jle		__get_pixel_left_top_side
		cmp		ecx, bottom_limit		// ecx = iy
		jge		__get_pixel_left_bottom_side

		// ›œ
		// ›œ
		mov		al, [edx]
		mov		ebx, eax
		add		edx, sxl4
		mov		al, [edx]
		add		ebx, eax
		shr		ebx, 1					// €‚Q
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_left_top_side:
		// ››
		// ›œ
		add		edx, sxl4
		mov		al, [edx]
		mov		ebx, eax
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_left_bottom_side:
		// ›œ
		// ››
		mov		al, [edx]
		mov		ebx, eax
		jmp		__get_pixel_end			// I‚í‚èI

	//------------------------------------------------------------------------------------
	__get_pixel_right_side:
		cmp		ecx, top_limit			// ecx = iy
		jle		__get_pixel_right_top_side
		cmp		ecx, bottom_limit		// ecx = iy
		jge		__get_pixel_right_bottom_side

		// œ›
		// œ›
		mov		al, [esi]
		mov		ebx, eax
		add		esi, sxl4
		mov		al, [esi]
		add		ebx, eax
		shr		ebx, 1					// €‚Q
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_right_top_side:
		// ››
		// œ›
		add		esi, sxl4
		mov		al, [esi]
		mov		ebx, eax
		jmp		__get_pixel_end			// I‚í‚èI

	__get_pixel_right_bottom_side:
		// œ›
		// ››
		mov		al, [esi]
		mov		ebx, eax
		jmp		__get_pixel_end			// I‚í‚èI

	//------------------------------------------------------------------------------------
	__get_pixel_end:

		// ƒsƒNƒZƒ‹’lİ’è
		shl		ebx, 24
		mov		[edi], ebx

		pop		ecx

	__draw_end:

		dec		ecx
		jz		__draw_end_end

		// dbl_src_x += dbl_line_add_x;
		fld		dbl_src_x
		fadd	dbl_line_add_x
		fstp	dbl_src_x

		// dbl_src_y += dbl_line_add_y;
		fld		dbl_src_y
		fadd	dbl_line_add_y
		fstp	dbl_src_y

		// dword_dp++;
		add		edi, 4

		jmp		__loop_top
	__draw_end_end:

	}
}

// ‰ñ“]‚ ‚è•`‰æ
void tnm_draw_32bit_mask_rotate_func(
	BYTE *dstp, int dst_w, int dst_h, int dst_ex1, int dst_ey1, int dst_ex2, int dst_ey2,
	BYTE *srcp, int src_w, int src_h, int src_ex1, int src_ey1, int src_ex2, int src_ey2, int src_center_x, int src_center_y,
	int dcx, int dcy, int reverse_rotate, double dbl_x_scale, double dbl_y_scale,
	double *dbl_dvx, double *dbl_dvy, bool is_high_quality
){
	double dlp1x1 = dbl_dvx[0];
	double dlp1y1 = dbl_dvy[0];
	double dlp1x2 = dbl_dvx[1];
	double dlp1y2 = dbl_dvy[1];
	double dlp2x1 = dbl_dvx[0];
	double dlp2y1 = dbl_dvy[0];
	double dlp2x2 = dbl_dvx[3];
	double dlp2y2 = dbl_dvy[3];

	double dbl_yyy = dbl_dvy[0];

	double reverse_rotate_sin = tnm_draw_32bit_mask_rotate_sin_table[reverse_rotate];
	double reverse_rotate_cos = tnm_draw_32bit_mask_rotate_cos_table[reverse_rotate];

	double dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y2;
	double line_x_add, line_y_add;

	double y_rotate_sin_tmp;
	double y_rotate_cos_tmp;

	double dbl_x1, dbl_x2, dbl_y, dbl_tmp;

	double dbl_05 = 0.5;

	int p1_flag = 0;
	int p2_flag = 0;

	int x1, x2;

	int y = (int)dbl_yyy;	// ‚c‚n‚t‚a‚k‚dŒ^‚ÌÀ•W‚ğ‚h‚m‚sŒ^‚ÌÀ•W‚É•â³‚·‚é

	BYTE *dword_dp;

	int x_cnt;

	while (1)	{
		if (dbl_yyy > dlp1y2)	{
			if (p1_flag)	{	break;	}
			dlp1x1 = dbl_dvx[1];	dlp1y1 = dbl_dvy[1];	dlp1x2 = dbl_dvx[2];	dlp1y2 = dbl_dvy[2];	p1_flag = 1;
		}
		if (dbl_yyy > dlp2y2)	{
			if (p2_flag)	{	break;	}
			dlp2x1 = dbl_dvx[3];	dlp2y1 = dbl_dvy[3];	dlp2x2 = dbl_dvx[2];	dlp2y2 = dbl_dvy[2];	p2_flag = 1;
		}

		if (y >= dst_ey1 && y <= dst_ey2)	{

			// ’¼üã‚Ì“_‚ÌÀ•W‚ğ‹‚ß‚é
			dbl_x1 = (((dlp1x2 - dlp1x1) * (dbl_yyy - dlp1y1)) / (dlp1y2 - dlp1y1)) + dlp1x1;
			dbl_x2 = (((dlp2x2 - dlp2x1) * (dbl_yyy - dlp2y1)) / (dlp2y2 - dlp2y1)) + dlp2x1;

			// ‚c‚n‚t‚a‚k‚dŒ^‚ÌÀ•W‚ğ‚h‚m‚sŒ^‚ÌÀ•W‚É•â³‚·‚é
			if (dbl_x1 > dbl_x2)	{	dbl_tmp = dbl_x1;	dbl_x1 = dbl_x2;	dbl_x2 = dbl_tmp;	}
			x1 = (int)dbl_x1;
			x2 = (int)ceil(dbl_x2);

			if (x1 <= dst_ex2 && x2 >= dst_ex1)	{

				// ’¼ü‚Ìˆ—”
				x_cnt = (x2 - x1) + 1;

				// (dcx, dcy) ‚ğ’†S‚Æ‚µ‚½À•W‚ğŒ³‚É–ß‚·
				dbl_y = y - dcy;
				dbl_x1 = x1 - dcx;
				dbl_x2 = x2 - dcx;

				// Œ³‚ÌÀ•W‚É–ß‚·
				y_rotate_sin_tmp = dbl_y * reverse_rotate_sin;
				y_rotate_cos_tmp = dbl_y * reverse_rotate_cos;
				dbl_src_line_x1 = (((dbl_x1 * reverse_rotate_cos) - y_rotate_sin_tmp) / dbl_x_scale) + src_center_x;
				dbl_src_line_y1 = (((dbl_x1 * reverse_rotate_sin) + y_rotate_cos_tmp) / dbl_y_scale) + src_center_y;
				dbl_src_line_x2 = (((dbl_x2 * reverse_rotate_cos) - y_rotate_sin_tmp) / dbl_x_scale) + src_center_x;
				dbl_src_line_y2 = (((dbl_x2 * reverse_rotate_sin) + y_rotate_cos_tmp) / dbl_y_scale) + src_center_y;

				// ƒ\[ƒXÀ•W‚Ì‘•ª
				line_x_add = (dbl_src_line_x2 - dbl_src_line_x1) / x_cnt;
				line_y_add = (dbl_src_line_y2 - dbl_src_line_y1) / x_cnt;

				// •`‰æ”ÍˆÍ•â³
				if (x1 < dst_ex1)	{
					dbl_src_line_x1 += (line_x_add * (double)(dst_ex1 - x1));
					dbl_src_line_y1 += (line_y_add * (double)(dst_ex1 - x1));
					x1 = dst_ex1;
				}
				if (x2 > dst_ex2)	{	x2 = dst_ex2;	}
				x_cnt = (x2 - x1) + 1;

				// •`‰ææƒAƒhƒŒƒX
				dword_dp = (BYTE *)(dstp + (((y * dst_w) + x1) << 2));

				if (is_high_quality)	{
					tnm_draw_32bit_mask_rotate_func_high_quality(src_ex1, src_ey1, src_ex2, src_ey2, dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y2, srcp, src_w, src_h, dword_dp, line_x_add, line_y_add, x_cnt, 0);
				}
				else	{
					tnm_draw_32bit_mask_rotate_func_low_quality(src_ex1, src_ey1, src_ex2, src_ey2, dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y2, srcp, src_w, src_h, dword_dp, line_x_add, line_y_add, x_cnt, 0);
				}
			}
		}

		dbl_yyy += 1;
		y++;
	}
}

// ‰ñ“]‚È‚µ•`‰æ
void tnm_draw_32bit_mask_rotate_less_func(
	BYTE *dstp, int dst_w, int dst_h, int dst_ex1, int dst_ey1, int dst_ex2, int dst_ey2,
	BYTE *srcp, int src_w, int src_h, int src_ex1, int src_ey1, int src_ex2, int src_ey2, int src_center_x, int src_center_y,
	int dcx, int dcy, double dbl_x_scale, double dbl_y_scale,
	double *dbl_dvx, double *dbl_dvy, bool is_high_quality
){
	double dbl_x1, dbl_y1, dbl_x2, dbl_y2;
	double dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y2;
	double line_x_add, line_y_add;
	BYTE *dword_dp;

	int x1, y1, x2, y2, y_cnt, x_cnt, dst_w4, i, tmp;

	// ‚c‚n‚t‚a‚k‚dŒ^‚ÌÀ•W‚ğ‚h‚m‚sŒ^‚ÌÀ•W‚É•â³‚·‚é
	y1 = (int)dbl_dvy[0];
	y2 = (int)ceil(dbl_dvy[2]);
	if (y1 > dst_ey2 || y2 < dst_ey1)	{	return;	}
	y_cnt = (y2 - y1) + 1;

	x1 = (int)dbl_dvx[0];
	x2 = (int)ceil(dbl_dvx[1]);
	if (x1 > x2)	{	tmp = x1;	x1 = x2;	x2 = tmp;	}
	if (x1 > dst_ex2 || x2 < dst_ex1)	{	return;	}
	x_cnt = (x2 - x1) + 1;

	// (dcx, dcy) ‚ğ’†S‚Æ‚µ‚½À•W‚ğŒ³‚É–ß‚·
	dbl_x1 = x1 - dcx;
	dbl_y1 = y1 - dcy;
	dbl_x2 = x2 - dcx;
	dbl_y2 = y2 - dcy;

	// Œ³‚ÌÀ•W‚É–ß‚·
	dbl_src_line_x1 = (dbl_x1 / dbl_x_scale) + src_center_x;
	dbl_src_line_y1 = (dbl_y1 / dbl_y_scale) + src_center_y;
	dbl_src_line_x2 = (dbl_x2 / dbl_x_scale) + src_center_x;
	dbl_src_line_y2 = (dbl_y2 / dbl_y_scale) + src_center_y;

	// ƒ\[ƒXÀ•W‚Ì‘•ª
	line_x_add = (dbl_src_line_x2 - dbl_src_line_x1) / x_cnt;
	line_y_add = (dbl_src_line_y2 - dbl_src_line_y1) / y_cnt;

	// •`‰æ”ÍˆÍ•â³
	if (x1 < dst_ex1)	{
		dbl_src_line_x1 += (line_x_add * (double)(dst_ex1 - x1));
		x1 = dst_ex1;
	}
	if (x2 > dst_ex2)	{	x2 = dst_ex2;	}
	x_cnt = (x2 - x1) + 1;

	if (y1 < dst_ey1)	{
		dbl_src_line_y1 += (line_y_add * (double)(dst_ey1 - y1));
		y1 = dst_ey1;
	}
	if (y2 > dst_ey2)	{	y2 = dst_ey2;	}
	y_cnt = (y2 - y1) + 1;

	// •`‰ææƒAƒhƒŒƒX
	dword_dp = (BYTE *)(dstp + (((y1 * dst_w) + x1) << 2));
	dst_w4 = dst_w * 4;

	for (i = 0; i < y_cnt; i++)	{
		if (is_high_quality)	{
			tnm_draw_32bit_mask_rotate_func_high_quality(src_ex1, src_ey1, src_ex2, src_ey2, dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y1, srcp, src_w, src_h, dword_dp, line_x_add, 0, x_cnt, 0);
		}
		else	{
			tnm_draw_32bit_mask_rotate_func_low_quality(src_ex1, src_ey1, src_ex2, src_ey2, dbl_src_line_x1, dbl_src_line_y1, dbl_src_line_x2, dbl_src_line_y1, srcp, src_w, src_h, dword_dp, line_x_add, 0, x_cnt, 0);
		}
		dword_dp += dst_w4;
		dbl_src_line_y1 += line_y_add;
		y1++;
	}
}

// –{‘Ì
void tnm_draw_32bit_mask_rotate_scale(BYTE *dst, int dst_w, int dst_h, BYTE *src, int src_w, int src_h, int src_center_x, int src_center_y, int dst_x, int dst_y, int rotate, double dbl_scale_x, double dbl_scale_y, bool is_high_quality)
{
	// ”{—¦‚O
	if (dbl_scale_x == 0 || dbl_scale_y == 0)	{	return;	}

	// ‚r‚h‚m^‚b‚n‚rƒe[ƒuƒ‹ì¬
	if (!tnm_draw_32bit_mask_rotate_sin_cos_table_create_flag)	{
		tnm_draw_32bit_mask_func_create_sin_cos_table();
	}

	// Šp“x•â³
	int reverse_rotate;
	if (rotate >= 0)	{	rotate %= 3600;								}
	else				{	rotate = (3600 + (rotate % 3600)) % 3600;	}
	if (rotate == 0)	{	reverse_rotate = 0;							}
	else				{	reverse_rotate = 3600 - rotate;				}

	// “]‘—æ‚Ì—Ìˆæ‹éŒ`
	int dst_ex1 = 0;
	int dst_ey1 = 0;
	int dst_ex2 = dst_w - 1;
	int dst_ey2 = dst_h - 1;

	// “]‘—Œ³‚Ì—Ìˆæ‹éŒ`
	int src_ex1 = 0;
	int src_ey1 = 0;
	int src_ex2 = src_w - 1;
	int src_ey2 = src_h - 1;

	// ‚Pƒhƒbƒg‰ÁZ
	src_ex2 += 1;
	src_ey2 += 1;
	if (src_ex2 == 0 || src_ey2 == 0)	{	return;	}

	//„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª

	int i;
	double dbl_tmp;

	// ”{—¦•â³
	if (dbl_scale_x >= 0)	{	dbl_tmp = (double)src_ex2 *   dbl_scale_x  - 1;		dbl_scale_x =   dbl_tmp / (double)src_ex2;	}
	else					{	dbl_tmp = (double)src_ex2 * (-dbl_scale_x) - 1;		dbl_scale_x = -(dbl_tmp / (double)src_ex2);	}
	if (dbl_scale_y >= 0)	{	dbl_tmp = (double)src_ey2 *   dbl_scale_y  - 1;		dbl_scale_y =   dbl_tmp / (double)src_ey2;	}
	else					{	dbl_tmp = (double)src_ey2 * (-dbl_scale_y) - 1;		dbl_scale_y = -(dbl_tmp / (double)src_ey2);	}

	// (src_center_x, src_center_y) ‚ğ (0, 0) ‚Æ‚µ‚½À•W‚É•ÏŠ·
	int svx[4], svy[4];
	svx[0] = (src_ex1 - src_center_x);	svy[0] = (src_ey1 - src_center_y);
	svx[1] = (src_ex2 - src_center_x);	svy[1] = (src_ey1 - src_center_y);
	svx[2] = (src_ex2 - src_center_x);	svy[2] = (src_ey2 - src_center_y);
	svx[3] = (src_ex1 - src_center_x);	svy[3] = (src_ey2 - src_center_y);

	// š‰ñ“]‚ ‚è‚Ìê‡
	double dbl_dvx[4], dbl_dvy[4];
	double dbl_tmp_dvx[4], dbl_tmp_dvy[4];
	if (rotate != 0)	{
		double dbl_xxx1, dbl_yyy1;
		// À•W•ÏŠ·
		for (i = 0; i < 4; i++)	{
			// ”{—¦
			dbl_xxx1 = (double)svx[i] * dbl_scale_x;
			dbl_yyy1 = (double)svy[i] * dbl_scale_y;
			// ‰ñ“]
			dbl_tmp_dvx[i] = (dbl_xxx1 * tnm_draw_32bit_mask_rotate_cos_table[rotate]) - (dbl_yyy1 * tnm_draw_32bit_mask_rotate_sin_table[rotate]);
			dbl_tmp_dvy[i] = (dbl_xxx1 * tnm_draw_32bit_mask_rotate_sin_table[rotate]) + (dbl_yyy1 * tnm_draw_32bit_mask_rotate_cos_table[rotate]);
			// (dst_x, dst_y) ‚ğ’†S‚Æ‚µ‚½À•W‚É•ÏŠ·
			dbl_tmp_dvx[i] += dst_x;
			dbl_tmp_dvy[i] += dst_y;
		}
		// ˆê”Ôã‚ÌÀ•W‚ğ’T‚·
		int top = 0;
		dbl_tmp = dbl_tmp_dvy[0];
		for (i = 1; i < 4; i++)	{
			if (dbl_tmp_dvy[i] < dbl_tmp)	{	dbl_tmp = dbl_tmp_dvy[i];	top = i;	}
		}
		// À•W‚ğ•À‚Ñ‘Ö‚¦‚é
		for (i = 0; i < 4; i++)	{
			dbl_dvx[i] = dbl_tmp_dvx[top];
			dbl_dvy[i] = dbl_tmp_dvy[top];
			top = (top + 1) & 0x03;
		}
	}
	// š‰ñ“]‚È‚µ‚Ìê‡
	else	{
		// À•W•ÏŠ·
		for (i = 0; i < 4; i++)	{
			// ”{—¦
			dbl_tmp_dvx[i] = (double)svx[i] * dbl_scale_x;
			dbl_tmp_dvy[i] = (double)svy[i] * dbl_scale_y;
			// (dst_x, dst_y) ‚ğ’†S‚Æ‚µ‚½À•W‚É•ÏŠ·
			dbl_tmp_dvx[i] += dst_x;
			dbl_tmp_dvy[i] += dst_y;
		}
		// À•W‚ğ•À‚Ñ‘Ö‚¦‚é
		if (dbl_scale_y >= 0)	{
			dbl_dvx[0] = dbl_tmp_dvx[0];	dbl_dvy[0] = dbl_tmp_dvy[0];
			dbl_dvx[1] = dbl_tmp_dvx[1];	dbl_dvy[1] = dbl_tmp_dvy[1];
			dbl_dvx[2] = dbl_tmp_dvx[2];	dbl_dvy[2] = dbl_tmp_dvy[2];
			dbl_dvx[3] = dbl_tmp_dvx[3];	dbl_dvy[3] = dbl_tmp_dvy[3];
		}
		else	{
			dbl_dvx[0] = dbl_tmp_dvx[3];	dbl_dvy[0] = dbl_tmp_dvy[3];
			dbl_dvx[1] = dbl_tmp_dvx[2];	dbl_dvy[1] = dbl_tmp_dvy[2];
			dbl_dvx[2] = dbl_tmp_dvx[1];	dbl_dvy[2] = dbl_tmp_dvy[1];
			dbl_dvx[3] = dbl_tmp_dvx[0];	dbl_dvy[3] = dbl_tmp_dvy[0];
		}
	}

	//„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª
	// ‹éŒ`”ÍˆÍ

	double dbl_redisp_x1 = dbl_dvx[0];
	double dbl_redisp_y1 = dbl_dvy[0];
	double dbl_redisp_x2 = dbl_dvx[0];
	double dbl_redisp_y2 = dbl_dvy[0];
	for (i = 1; i < 4; i++)	{
		if (dbl_redisp_x1 > dbl_dvx[i])	{	dbl_redisp_x1 = dbl_dvx[i];	}
		if (dbl_redisp_y1 > dbl_dvy[i])	{	dbl_redisp_y1 = dbl_dvy[i];	}
		if (dbl_redisp_x2 < dbl_dvx[i])	{	dbl_redisp_x2 = dbl_dvx[i];	}
		if (dbl_redisp_y2 < dbl_dvy[i])	{	dbl_redisp_y2 = dbl_dvy[i];	}
	}

	// ƒhƒbƒg”ÍˆÍ
	int redisp_x1 = (int)dbl_redisp_x1;
	int redisp_y1 = (int)dbl_redisp_y1;
	int redisp_x2 = (int)ceil(dbl_redisp_x2);
	int redisp_y2 = (int)ceil(dbl_redisp_y2);

	// ƒGƒŠƒA•â³
	if (redisp_x1 < dst_ex1)	{	redisp_x1 = dst_ex1;	}
	if (redisp_y1 < dst_ey1)	{	redisp_y1 = dst_ey1;	}
	if (redisp_x2 > dst_ex2)	{	redisp_x2 = dst_ex2;	}
	if (redisp_y2 > dst_ey2)	{	redisp_y2 = dst_ey2;	}

	// •\¦”ÍˆÍŠOƒ`ƒFƒbƒN
	if (redisp_x1 > dst_ex2)	{	return;	}
	if (redisp_y1 > dst_ey2)	{	return;	}
	if (redisp_x2 < dst_ex1)	{	return;	}
	if (redisp_y2 < dst_ey1)	{	return;	}

	//„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª„ª
	// •`‰æ

	// š‰ñ“]‚ ‚è‚Ìê‡
	if (rotate != 0)	{
		tnm_draw_32bit_mask_rotate_func(
			dst, dst_w, dst_h, dst_ex1, dst_ey1, dst_ex2, dst_ey2,
			src, src_w, src_h, src_ex1, src_ey1, src_ex2, src_ey2, src_center_x, src_center_y,
			dst_x, dst_y, reverse_rotate, dbl_scale_x, dbl_scale_y,
			dbl_dvx, dbl_dvy, is_high_quality
		);
	}
	// š‰ñ“]‚È‚µ‚Ìê‡
	else	{
		tnm_draw_32bit_mask_rotate_less_func(
			dst, dst_w, dst_h, dst_ex1, dst_ey1, dst_ex2, dst_ey2,
			src, src_w, src_h, src_ex1, src_ey1, src_ex2, src_ey2, src_center_x, src_center_y,
			dst_x, dst_y, dbl_scale_x, dbl_scale_y,
			dbl_dvx, dbl_dvy, is_high_quality
		);
	}

}

