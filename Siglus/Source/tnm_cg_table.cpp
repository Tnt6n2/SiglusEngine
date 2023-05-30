#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_cg_table.h"
#include	"data/tnm_save_00.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// �b�f�e�[�u���F������
// ================================================================
bool C_tnm_cg_table::init()
{
	// �b�f�e�[�u����ǂݍ���
	return load_cg_table_data(Gp_ini->cg_table_file);
}

// ****************************************************************
// �b�f�e�[�u���F�Z�[�u
// ================================================================
void C_tnm_cg_table::save(C_tnm_save_stream& stream)
{
	flag.save(stream);
}

// ****************************************************************
// �b�f�e�[�u���F���[�h
// ================================================================
void C_tnm_cg_table::load(C_tnm_save_stream& stream)
{
	flag.load(stream);
}

// ****************************************************************
// �b�f�e�[�u���F�t�@�C����ǂݍ���
// ================================================================
bool C_tnm_cg_table::load_cg_table_data(TSTR file_name)
{
	if (file_name.empty())
		return true;

	file_name = change_extension_ret(file_name, _T("cgm"));

	TSTR file_path = tnm_find_dat(file_name);
	if (!cg_table.load(file_path))
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�b�f�e�[�u�� ") + file_name + _T(" ��������܂���ł����B"));

	// �t���O���m�ۂ���i�Œ蒷�j
	flag.init(S_element(), _T("cg_table.flag"), Gp_ini->cg_table_flag_cnt, false);

	return true;
}

// ****************************************************************
// �b�f�e�[�u���F�t���O�̑����擾
// ================================================================
int C_tnm_cg_table::get_cg_cnt()
{
	return (int)cg_table.cg_table_data_list.size();
}

// ****************************************************************
// �b�f�e�[�u���F�u�����v�t���O�̑����擾
// ================================================================
int C_tnm_cg_table::get_look_cnt()
{
	int look_cnt = 0;
	int list_cnt = get_cg_cnt();
	for (int i = 0; i < list_cnt; i++)	{
		S_cg_table_data_sub* sub = cg_table.get_sub_pointer_from_list_no_func(i);
		if (sub)	{
			if (flag.get_value(sub->flag_no) == 1)	{
				look_cnt ++;
			}
		}
	}

	return look_cnt;
}

// ****************************************************************
// �b�f�e�[�u���F�u�����v�p�[�Z���g�̎擾
// ================================================================
int C_tnm_cg_table::get_look_percent()
{
	int cg_cnt = get_cg_cnt();
	if (cg_cnt == 0)
		return 0;		// �o�^���Ȃ�

	int look_cnt = get_look_cnt();
	if (look_cnt == 0)
		return 0;		// �P�������Ă��Ȃ�

	int percent = look_cnt * 100 / cg_cnt;
	if (percent == 0)
		percent = 1;	// �P���ł����Ă�����Œ� 1% �ɕ␳����

	return percent;
}

// ****************************************************************
// �b�f�e�[�u���F�S�Ẵt���O�l��ݒ�
// ================================================================
void C_tnm_cg_table::set_flag_value_for_all(int flag_value)
{
	int list_cnt = get_cg_cnt();
	for (int i = 0; i < list_cnt; i++)	{
		S_cg_table_data_sub* sub = cg_table.get_sub_pointer_from_list_no_func(i);
		if (sub)	{
			flag.set_value(sub->flag_no, flag_value);
		}
	}
}

// ****************************************************************
// �b�f�e�[�u���F�t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j
// ================================================================

// ���O����
int C_tnm_cg_table::set_flag_value_from_name(CTSTR& name, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (!sub)	{	return -1;	}

	// �t���O��ݒ�
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// ���X�g�ԍ�����
int C_tnm_cg_table::set_flag_value_from_list_no(int list_no, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (!sub)	{	return -1;	}

	// �t���O��ݒ�
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// �t���O�ԍ�����
int C_tnm_cg_table::set_flag_value_from_flag_no(int flag_no, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (!sub)	{	return -1;	}

	// �t���O��ݒ�
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// �O���[�v�R�[�h����
int C_tnm_cg_table::set_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (!sub)	{	return -1;	}

	// �t���O��ݒ�
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// ****************************************************************
// �b�f�e�[�u���F�t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j
// ================================================================

// ���O����
bool C_tnm_cg_table::get_flag_value_from_name(CTSTR& name, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// ���X�g�ԍ�����
bool C_tnm_cg_table::get_flag_value_from_list_no(int list_no, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// �t���O�ԍ�����
bool C_tnm_cg_table::get_flag_value_from_flag_no(int flag_no, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// �O���[�v�R�[�h����
bool C_tnm_cg_table::get_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// ****************************************************************
// �b�f�e�[�u���F���O���擾�i���݂��Ȃ��ꍇ�A��� TSTR ���߂�܂��j
// ================================================================

// ���X�g�ԍ�����
TSTR C_tnm_cg_table::get_name_from_list_no(int list_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->name : _T("");
}

// �t���O�ԍ�����
TSTR C_tnm_cg_table::get_name_from_flag_no(int flag_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	return (sub) ? sub->name : _T("");
}

// �O���[�v�R�[�h����
TSTR C_tnm_cg_table::get_name_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->name : _T("");
}

// ****************************************************************
// �b�f�e�[�u���F�t���O�ԍ����擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
// ================================================================

// ���O����
int C_tnm_cg_table::get_flag_no_from_name(CTSTR& name)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	return (sub) ? sub->flag_no : -1;
}

// ���X�g�ԍ�����
int C_tnm_cg_table::get_flag_no_from_list_no(int list_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->flag_no : -1;
}

// �O���[�v�R�[�h����
int C_tnm_cg_table::get_flag_no_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->flag_no : -1;
}

// ****************************************************************
// �b�f�e�[�u���F�P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
// ================================================================

// ���O����
int C_tnm_cg_table::get_one_code_value_from_name(CTSTR& name, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	return (sub) ? sub->code[get_code_no] : -1;
}

// ���X�g�ԍ�����
int C_tnm_cg_table::get_one_code_value_from_list_no(int list_no, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->code[get_code_no] : -1;
}

// �t���O�ԍ�����
int C_tnm_cg_table::get_one_code_value_from_flag_no(int flag_no, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	return (sub) ? sub->code[get_code_no] : -1;
}

// �O���[�v�R�[�h����
int C_tnm_cg_table::get_one_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->code[get_code_no] : -1;
}

// ****************************************************************
// �b�f�e�[�u���F�S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j
// ================================================================

// ���O����
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_name(CTSTR& name)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// ���X�g�ԍ�����
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_list_no(int list_no)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// �t���O�ԍ�����
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_flag_no(int flag_no)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// �O���[�v�R�[�h����
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// ****************************************************************
// �b�f�e�[�u���F�O���[�v�R�[�h����O���[�v�̃����o�[�����擾
// ================================================================
int C_tnm_cg_table::get_group_member_cnt(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_group_tree *group = cg_table.get_groupe_tree_pointer_func(gc0, gc1, gc2, gc3, gc4);
	return (group) ? (int)group->tree.size() : 0;
}

// ****************************************************************
// �b�f�e�[�u���F�O���[�v�R�[�h����O���[�v�́u�����v�����o�[�����擾
// ================================================================
int C_tnm_cg_table::get_group_member_cnt_for_look(int cg0, int cg1, int cg2, int cg3, int cg4)
{
	ARRAY<int> flag_list;
	cg_table.get_flag_list_func(cg0, cg1, cg2, cg3, cg4, flag_list);
	int look_cnt = 0;
	for (int i = 0; i < (int)flag_list.size(); i++)	{
		// ���b�f�e�[�u���F�����Ńt���O�̒l���`�F�b�N���� look_cnt++ ����
	}
	return look_cnt;
}

// ****************************************************************
// �b�f�e�[�u���F�O���[�v�R�[�h����O���[�v�̃����o�[���ꊇ�Ńt���O�l��ݒ�
// ================================================================
void C_tnm_cg_table::set_flag_value_for_group_member(int cg0, int cg1, int cg2, int cg3, int cg4, int flag_value)
{
	ARRAY<int> flag_list;
	cg_table.get_flag_list_func(cg0, cg1, cg2, cg3, cg4, flag_list);
	for (int i = 0; i < (int)flag_list.size(); i++)	{
		// ���b�f�e�[�u���F�����Ńt���O�̒l��ݒ肷��
	}
}

#if 0		// ����ō폜���� RealLive �̃I���W�i���\�[�X

/*=============================================================================
	�b�f�e�[�u�� �t���O�ԍ��G���[
void avg_cg_table_flag_no_err_func(UC *name, int flag_no);
-----------------------------------------------------------------------------*/
void avg_cg_table_flag_no_err_func(UC *name, int flag_no){
	if (name)	{
		if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"%s : Z[%05d]", name, flag_no);	}		// �P�r�b�g�t���O���[�h
		else						{	format(Gavg_err_mess, (UC *)"%s : Z[%04d]", name, flag_no);	}		// �R�Q�r�b�g�t���O���[�h
	}
	else	{
		UC name_str[MAXX_PATH];
		if (avg_cg_table_get_name_str_from_flag_no(flag_no, name_str))	{
			if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"%s : Z[%05d]", name_str, flag_no);	}		// �P�r�b�g�t���O���[�h
			else						{	format(Gavg_err_mess, (UC *)"%s : Z[%04d]", name_str, flag_no);	}		// �R�Q�r�b�g�t���O���[�h
		}
		else	{
			if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"UnKnownName : Z[%05d]", name, flag_no);	}		// �P�r�b�g�t���O���[�h
			else						{	format(Gavg_err_mess, (UC *)"UnKnownName : Z[%04d]", name, flag_no);	}		// �R�Q�r�b�g�t���O���[�h
		}
	}
	avg_dbwin11_message((UC *)"���d�q�q�F�b�f�e�[�u���̃t���O�ԍ����ُ�ł��B", Gavg_err_mess, AVGDBM_SE_WARNING, AVGDBM_TYPE_NONE);
}

/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/

/*=============================================================================
	�b�f�e�[�u�� �t���O�l�̐ݒ�i�t���O�ԍ�����j
int avg_cg_table_set_flag_value(int flag_no, int nom);
-----------------------------------------------------------------------------*/
int avg_cg_table_set_flag_value(int flag_no, int nom){
	int alpha = 0;
	if (Gavg_ini.cg_table_mod)	{		// �P�r�b�g�t���O���[�h
		alpha = 26;
		if (nom != 0)	{	nom = 1;	}
	}
	return avg_math_set_flag(('Z'-'A') + alpha, flag_no, nom, &Gavg_save.flag);
}

/*=============================================================================
	�b�f�e�[�u�� �t���O�l�̎擾�i�t���O�ԍ�����j
int avg_cg_table_get_flag_value(int flag_no, int *ret_nom);
-----------------------------------------------------------------------------*/
int avg_cg_table_get_flag_value(int flag_no, int *ret_nom){
	int alpha = 0;
	if (Gavg_ini.cg_table_mod)	{		// �P�r�b�g�t���O���[�h
		alpha = 26;
	}
	return avg_math_get_flag(('Z'-'A') + alpha, flag_no, ret_nom, &Gavg_save.flag);
}

#endif
