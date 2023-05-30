#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_bgm_table.h"

// ****************************************************************
// �a�f�l�e�[�u���F������
// ================================================================
void C_tnm_bgm_table::init()
{
	flag_list.init(S_element(), _T("bgmtable.flag"), Gp_ini->bgm_cnt, false);
}

// ****************************************************************
// �a�f�l�e�[�u���F�Z�[�u
// ================================================================
void C_tnm_bgm_table::save(C_tnm_save_stream& stream)
{
	flag_list.save(stream);
}

// ****************************************************************
// �a�f�l�e�[�u���F���[�h
// ================================================================
void C_tnm_bgm_table::load(C_tnm_save_stream& stream)
{
	flag_list.load(stream);
}

// ****************************************************************
// �a�f�l�e�[�u���F�l��ݒ�
// ================================================================
void C_tnm_bgm_table::set_listened(int bgm_no, bool value)
{
	flag_list.set_value(bgm_no, value);
}

// ****************************************************************
// �a�f�l�e�[�u���F�S�Ă̒l��ݒ�
// ================================================================
void C_tnm_bgm_table::set_all_flag(bool value)
{
	for (int i = 0; i < Gp_ini->bgm_cnt; i++)
		flag_list.set_value(i, value ? 1 : 0);
}

// ****************************************************************
// �a�f�l�e�[�u���F�l���擾
// ================================================================
bool C_tnm_bgm_table::get_listened(int bgm_no)
{
	return flag_list.get_value(bgm_no) ? true : false;
}
