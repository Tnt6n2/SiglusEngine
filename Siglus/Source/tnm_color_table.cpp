#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_color_table.h"

// ****************************************************************
// �J���[�e�[�u���F������
// ================================================================
void C_tnm_color_table::init()
{
	m_color.resize(Gp_ini->color_table_cnt);
	
	// �J���[�e�[�u�������������܂��B
	for (int color_no = 0; color_no < (int)m_color.size(); color_no++)
		m_color[color_no] = C_argb(255, Gp_ini->color_table[color_no]);
}
