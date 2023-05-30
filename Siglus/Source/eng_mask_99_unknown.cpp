#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_mask_wipe.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// マスクテクスチャ作成：ストライプ（※マスクワイプ用じゃない）
// ================================================================

bool tnm_make_32bit_mask_for_stripe(S_mask_buffer *buf, int make_w, int make_h, bool gray_reverse, int dir, int stripe_len)
{
	BYTE *dp;
	int ex1, ey1, ex2, ey2;
	if (dir == 0)	{
		if (!tnm_alloc_mask_buffer(buf, stripe_len * 2, 1, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}
	else	{
		if (!tnm_alloc_mask_buffer(buf, 1, stripe_len * 2, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	}

	if (dir == 0)	{
		int x1 = 0;
		int x2 = stripe_len - 1;
		tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x1, 0, x2, 0, 255, gray_reverse, ex1, ey1, ex2, ey2);
		x1 += stripe_len;
		x2 += stripe_len;
		tnm_draw_32bit_fill_box(dp, buf->w, buf->h, x1, 0, x2, 0,   0, gray_reverse, ex1, ey1, ex2, ey2);
	}
	else	{
		int y1 = 0;
		int y2 = stripe_len - 1;
		tnm_draw_32bit_fill_box(dp, buf->w, buf->h, 0, y1, 0, y2, 255, gray_reverse, ex1, ey1, ex2, ey2);
		y1 += stripe_len;
		y2 += stripe_len;
		tnm_draw_32bit_fill_box(dp, buf->w, buf->h, 0, y1, 0, y2,   0, gray_reverse, ex1, ey1, ex2, ey2);
	}
	return true;
}

bool tnm_make_mask_texture_for_stripe(BSP<C_d3d_texture> *texture, int target_w, int target_h, bool gray_reverse, int dir, int stripe_len)
{
	stripe_len = limit(1, stripe_len, 128);
	S_mask_buffer buf;
	if (!tnm_make_32bit_mask_for_stripe(&buf, target_w, target_h, gray_reverse, dir, stripe_len))	{	return false;	}
	return tnm_make_mask_texture(texture, &buf);
}

// ****************************************************************
// マスクテクスチャ作成：マスクファイルを使う
// ================================================================
bool tnm_make_mask_texture_from_file(BSP<C_d3d_texture> *texture, BSP<C_dib_chip> *dib, int target_w, int target_h, CTSTR& mask_file)
{
	int pct_type = 0;
	TSTR file_path = tnm_find_g00(mask_file, &pct_type);

	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("マスクファイル ") + mask_file + _T(" が見つかりませんでした。"));
		return false;
	}
	if (pct_type != 1)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("マスクファイルは g00 しか対応していません。\n") + mask_file);
		return false;
	}

	// アルバムに g00 を読み込む
	C_album album;
	if (!::load_g00_to_album(&album, file_path))	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("マスクファイル ") + mask_file + _T(" が読み込めませんでした。\n") + get_last_error_msg());
		return false;
	}

	BSP<C_pct> pct = album.get_pct(0);
	if (!pct)
		return false;

	*dib = pct->expand_chips();
	if (!*dib)
		return false;

	// α補正
	BYTE *dp = (*dib)->get_ptr();
	BYTE *sp = dp + 2;		// Ｒ成分
	dp += 3;				// α成分
	int cnt = (*dib)->get_width() * (*dib)->get_height();
	for (int i = 0; i < cnt; i++)	{
		*dp = *sp;
		dp += 4;
		sp += 4;
	}

	// コピー描画
	S_mask_buffer buf;
	int ex1, ey1, ex2, ey2;
	if (!tnm_alloc_mask_buffer(&buf, target_w, target_h, &dp, &ex1, &ey1, &ex2, &ey2))	{	return false;	}
	tc_draw_dib_easy(dp, buf.w, buf.h, (*dib)->get_ptr(), (*dib)->get_width(), (*dib)->get_height(), 0, 0);

	// テクスチャー作成
	return tnm_make_mask_texture(texture, &buf);
}

