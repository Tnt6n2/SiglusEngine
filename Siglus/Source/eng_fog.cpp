#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_fog.h"
#include	"engine/eng_fog.h"

// �t�H�O�̃A���o�����擾
BSP<C_d3d_album> tnm_get_fog_album()
{
	return Gp_fog->get_fog_album();
}

