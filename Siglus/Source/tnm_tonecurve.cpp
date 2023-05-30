#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_tonecurve.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// トーンカーブ：定数
// ================================================================
const int		TNM_TONE_CURVE_MAX = 1000;

const int		TNM_TCTYPE_TONE_CURVE = 0;
const int		TNM_TCTYPE_RGB_SAT = 1;

// ****************************************************************
// トーンカーブ：構造体
// ================================================================
struct S_tnm_tonecurve_header
{
	int		max;
	int		cnt;
	int		offset[TNM_TONE_CURVE_MAX];
};

struct S_tnm_tonecurve_sub_header
{
	int		type;
	int		data_size;
	int		keep[14];
};

// ****************************************************************
// トーンカーブ：初期化
// ================================================================
void C_tnm_tonecurve::init()
{
	TSTR file_name = Gp_ini->tonecurve_file;

	// ファイル名が空なら何もしない
	if (file_name.empty())
		return;

	TSTR file_path = tnm_find_dat(file_name);

	// ファイルを読み込む
	ARRAY<BYTE> file_data;
	if (!C_file::read_full_data(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(" が開けませんでした。"));
		return;
	}

	// ヘッダを取得
	S_tnm_tonecurve_header* p_header = (S_tnm_tonecurve_header *)file_data.get();

	// 個数制限（新システムでは 256 個までしか読めない）
	if (p_header->cnt > 256)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("トーンカーブは 256 個までしか作成できません。"));
		return;
	}

	// サブを作成
	m_sub_list.resize(256);

	// テクスチャを作成
	// 横幅は 256 固定
	// ミップマップを作成しない（ステージ１に設定するので作成しても使われません）
	D3DLOCKED_RECT rect;
	m_texture = G_rsm.create_texture(_T("ﾄｰﾝｶｰﾌﾞ"), 256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, false);
	m_texture->lock_rect(0, &rect, NULL, 0);

	// ファイルデータをコピーしていく
	for (int i = 0; i < 256; i++)	{
		if (p_header->offset[i] != 0)	{
			S_tnm_tonecurve_sub_header* p_sub_header = (S_tnm_tonecurve_sub_header *)(file_data.get() + p_header->offset[i]);
			BYTE* p = file_data.get() + p_header->offset[i] + sizeof(S_tnm_tonecurve_sub_header);
			BYTE* src_r = p;
			BYTE* src_g = p + 256;
			BYTE* src_b = p + 512;
			C_argb* dst = (C_argb *)((BYTE *)rect.pBits + rect.Pitch * i);

			// サブの読み込み
			switch (p_sub_header->type)	{
				case TNM_TCTYPE_TONE_CURVE:
					for (int j = 0; j < 256; j++)	{
						dst->r = *src_r++;
						dst->g = *src_g++;
						dst->b = *src_b++;
						dst->a = 255;		dst++;
					}
					m_sub_list[i].exist = 1;	// 存在したフラグ
					break;
				case TNM_TCTYPE_RGB_SAT:
					for (int j = 0; j < 256; j++)	{
						dst->r = *src_r++;
						dst->g = *src_g++;
						dst->b = *src_b++;
						dst->a = 255;		dst++;
					}
					m_sub_list[i].sat = *(int *)(p + 768);
					m_sub_list[i].exist = 1;	// 存在したフラグ
					break;
			}
		}
	}

	// テクスチャをアンロック
	m_texture->unlock_rect(0);
}

// ****************************************************************
// トーンカーブ：データの存在チェック
// ================================================================
bool C_tnm_tonecurve::check_tonecurve_no(int tonecurve_no)
{
	return (0 <= tonecurve_no && tonecurve_no < (int)m_sub_list.size()) && m_sub_list[tonecurve_no].exist;
}

// ****************************************************************
// トーンカーブ：テクスチャを取得
// ================================================================
BSP<C_d3d_texture> C_tnm_tonecurve::get_texture()
{
	return m_texture;
}

// ****************************************************************
// トーンカーブ：彩度を取得
// ================================================================
int C_tnm_tonecurve::get_sat(int tonecurve_no)
{
	if (tonecurve_no < 0 || (int)m_sub_list.size() <= tonecurve_no)
		return 0;

	return m_sub_list[tonecurve_no].sat;
}
