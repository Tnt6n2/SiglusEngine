#include	"pch.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_bgm_table.h"
#include	"engine/eng_bgm_table.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// �a�f�l�e�[�u���ւ̃|�C���^
// ================================================================
class	C_tnm_bgm_table;
extern	C_tnm_bgm_table*	Gp_bgm_table;

// ****************************************************************
// �a�f�l�e�[�u���ւ̃|�C���^
// ================================================================
int tnm_bgm_table_get_bgm_cnt()
{
	return Gp_bgm_table->get_bgm_cnt();
}

// ****************************************************************
// �a�f�l�e�[�u���̒l��ݒ�i�ԍ�����j
// ================================================================
void tnm_bgm_table_set_listened(int bgm_no, bool value)
{
	Gp_bgm_table->set_listened(bgm_no, value);
}

// ****************************************************************
// �a�f�l�e�[�u���̒l��ݒ�i�o�^������j
// ================================================================
void tnm_bgm_table_set_listened(CTSTR& bgm_name, bool value)
{
	int bgm_no = tnm_get_bgm_no_by_regist_name(bgm_name);
	if (bgm_no == -1)
		return;

	Gp_bgm_table->set_listened(bgm_no, value);
}

// ****************************************************************
// �a�f�l�e�[�u���̑S�Ă̒l��ݒ�
// ================================================================
void tnm_bgm_table_set_all_flag(bool value)
{
	Gp_bgm_table->set_all_flag(value);
}

// ****************************************************************
// �a�f�l�e�[�u���̒l���擾�i�ԍ�����j
// ================================================================
bool tnm_bgm_table_get_listened(int bgm_no)
{
	return Gp_bgm_table->get_listened(bgm_no);
}

// ****************************************************************
// �a�f�l�e�[�u���̒l���擾�i�o�^������j
// ================================================================
int tnm_bgm_table_get_listened(CTSTR& bgm_name)
{
	int bgm_no = tnm_get_bgm_no_by_regist_name(bgm_name);
	if (bgm_no == -1)
		return -1;

	return Gp_bgm_table->get_listened(bgm_no);
}

