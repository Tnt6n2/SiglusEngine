#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_mask.h"
#include	"engine/eng_mask.h"

// ****************************************************************
// マスク用のパラメータを取得する
// ================================================================
void tnm_get_mask_total_param(int mask_no, int* ret_x, int* ret_y)
{
	int mask_cnt = Gp_mask_list->get_size();
	if (0 <= mask_no && mask_no < mask_cnt)	{
		C_elm_mask* p_mask = &(*Gp_mask_list)[mask_no];
		if (ret_x)	{
			*ret_x = p_mask->get_total_x();
		}
		if (ret_y)	{
			*ret_y = p_mask->get_total_y();
		}
	}
}

// ****************************************************************
// マスク用のテクスチャを取得する
// ================================================================
BSP<C_d3d_texture> tnm_get_mask_texture(int mask_no)
{
	int mask_cnt = Gp_mask_list->get_size();
	if (0 <= mask_no && mask_no < mask_cnt)	{
		C_elm_mask* p_mask = &(*Gp_mask_list)[mask_no];
		BSP<C_d3d_album> p_album = p_mask->get_mask_album();
		if (p_album)	{
			BSP<C_d3d_texture> p_texture = p_album->get_texture(0);
			if (p_texture)
				return p_texture;
		}
	}

	return BSP<C_d3d_texture>();
}

