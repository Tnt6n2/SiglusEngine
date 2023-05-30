#pragma		once

#include	"data/tnm_def.h"
#include	"data/tnm_sprite.h"
#include	"data/tnm_rsc_manager.h"

// ****************************************************************
// Direct3D リソースマネージャー
// ================================================================
class	C_tnm_d3d_resource_manager;
extern	C_tnm_d3d_resource_manager	G_rsm;

// ****************************************************************
// Direct3D 頂点フォーマット
// ================================================================
struct S_vtx_d2_filter
{
	D3DXVECTOR3		pos;		// 位置
	FLOAT			rhw;		// 固定
	D3DCOLOR		color;		// ディフューズ色
	static const DWORD	FVF;
};

struct S_vtx_d3_filter
{
	D3DXVECTOR3		pos;		// 位置
	D3DCOLOR		color;		// ディフューズ色
	static const DWORD	FVF;
};

struct S_vtx_d2_pct
{
	D3DXVECTOR3		pos;		// 位置
	FLOAT			rhw;		// 固定
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[1];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d2_pct_mask
{
	D3DXVECTOR3		pos;		// 位置
	FLOAT			rhw;		// 固定
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[2];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d3_pct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		normal;		// 法線
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[1];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d3_billboard
{
	D3DXVECTOR3		pos;		// 位置
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[1];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d2_screen
{
	D3DXVECTOR3		pos;		// 位置
	FLOAT			rhw;		// 固定
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[1];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d3_screen
{
	D3DXVECTOR3		pos;		// 位置
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[1];		// ＵＶ座標
	static const DWORD	FVF;
};

struct S_vtx_d2_mask_wipe
{
	D3DXVECTOR3		pos;		// 位置
	FLOAT			rhw;		// 固定
	D3DCOLOR		color;		// ディフューズ色
	D3DXVECTOR2		tex[2];		// ＵＶ座標
	static const DWORD	FVF;
};

// ****************************************************************
// 矩形スプライト共通の頂点バッファとインデックスバッファ
// ================================================================
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_filter;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_filter;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_pct;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_pct_mask;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_pct;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_screen;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_screen;
extern	BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_mask_wipe;
extern	BSP<C_d3d_index_buffer>		G_rect_index_buffer;

// ****************************************************************
// 共用のインデックスバッファを更新する
// ================================================================
bool tnm_update_rect_index_buffer(int rect_cnt);

// ****************************************************************
// 頂点バッファを更新する
// ================================================================
bool tnm_update_vertex_list(ARRAY<C_tnm_sprite *>& sprite_list, ARRAY<C_tnm_sprite *>& sprite_list_ret);

// ****************************************************************
// Direct3D 画像を読み込む
// ================================================================
bool				tnm_check_pct(CTSTR& file_name, bool set_error);
BSP<C_album>		tnm_load_pct(CTSTR& file_name, bool set_cg_table);
BSP<C_d3d_album>	tnm_load_pct_d3d(CTSTR& file_name, bool fuchidori, bool set_cg_table);
BSP<C_d3d_album>	tnm_load_pct_d3d_from_save_thumb(int save_no);
BSP<C_d3d_album>	tnm_load_pct_d3d_from_thumb(int thumb_no);

// ****************************************************************
// bmp / png を作成
// ================================================================
bool tnm_create_bmp_from_texture(ARRAY<BYTE>& buffer, int width, int height, D3DLOCKED_RECT* p_rect);
bool tnm_create_png_from_texture_and_save_to_file(CTSTR& file_path, int width, int height, D3DLOCKED_RECT* p_rect, bool use_alpha);

// ****************************************************************
// スプライトパラメータとレンダーパラメータを結合する
// ================================================================
void tnm_affect_sp_and_rp(S_tnm_sprite_render_param* rpex, const S_tnm_sprite_render_param* parent_rpex, S_d3d_render_param* rp, const S_d3d_render_param* parent_rp);

// ****************************************************************
// 親のtrpを合成する
// ================================================================
void tnm_add_parent_trp(S_tnm_render_param* trp, const S_tnm_render_param* parent_trp);

// ****************************************************************
// trpをrpに変換
// ================================================================
void tnm_trp_to_rp(const S_tnm_render_param* trp, S_tnm_sprite_render_param* rpex, S_d3d_render_param* rp);

// ****************************************************************
// 2d ⇔ 3d 変換
// ================================================================
inline float tnm_d2to3(int rhs)
{
	return (float)rhs;
}
inline C_float3 tnm_d2to3(C_point rhs)
{
	return C_float3((float)rhs.x, (float)rhs.y, 0.5f);
}
inline int tnm_d3to2(float rhs)
{
	return (int)rhs;
}
inline C_point tnm_d3to2(C_float3 rhs)
{
	return C_point((int)rhs.x, (int)rhs.y);
}

// ****************************************************************
// 矩形＋マージン
// ================================================================
inline C_rect tnm_add_margin(C_rect lhs, C_rect margin)
{
	return C_rect(lhs.left + margin.left, lhs.top + margin.top, lhs.right - margin.right, lhs.bottom - margin.bottom);
}
inline C_rect tnm_sub_margin(C_rect lhs, C_rect margin)
{
	return C_rect(lhs.left - margin.left, lhs.top - margin.top, lhs.right + margin.right, lhs.bottom + margin.bottom);
}

