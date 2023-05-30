#pragma		once

#include	"data/tnm_cg_table_data.h"
#include	"element/elm_list.h"

// ****************************************************************
// �b�f�e�[�u��
// ================================================================
class C_tnm_cg_table
{
public:

	bool		init();
	void		save(C_tnm_save_stream& stream);
	void		load(C_tnm_save_stream& stream);

	// �b�f�e�[�u���f�[�^�̓ǂݍ���
	bool		load_cg_table_data(TSTR file_name);																	// �b�f�e�[�u���f�[�^�̓ǂݍ���

	// �b�f�̑����擾
	int			get_cg_cnt();																						// �t���O�̑����擾

	// �u�����v�t���O�̑����擾
	int			get_look_cnt();																						// �u�����v�t���O�̑����擾

	// �u�����v�p�[�Z���g�̎擾
	int			get_look_percent();																					// �u�����v�p�[�Z���g�̎擾

	// �S�Ẵt���O�l��ݒ�
	void		set_flag_value_for_all(int flag_value);																// �S�Ẵt���O�l��ݒ�

	// �t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j
	int			set_flag_value_from_name(CTSTR& name, int flag_value);												// ���O����t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j
	int			set_flag_value_from_list_no(int list_no, int flag_value);											// ���X�g�ԍ�����t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j
	int			set_flag_value_from_flag_no(int flag_no, int flag_value);											// �t���O�ԍ�����t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j
	int			set_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int flag_value);		// �O���[�v�R�[�h����t���O�l��ݒ�i�ύX�O�̒l���߂�܂��B���݂��Ȃ��ꍇ�A-1 ���߂�܂��B�j

	// �t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j
	bool		get_flag_value_from_name(CTSTR& name, int *ret_value);												// ���O����t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j
	bool		get_flag_value_from_list_no(int list_no, int *ret_value);											// ���X�g�ԍ�����t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j
	bool		get_flag_value_from_flag_no(int flag_no, int *ret_value);											// �t���O�ԍ�����t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j
	bool		get_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int *ret_value);		// �O���[�v�R�[�h����t���O�l���擾�i���݂��Ȃ��ꍇ�Afalse ���߂�܂��B�j

	// ���O���擾�i���݂��Ȃ��ꍇ�A��� TSTR ���߂�܂��j
	TSTR		get_name_from_list_no(int list_no);																	// ���X�g�ԍ����疼�O���擾�i���݂��Ȃ��ꍇ�A��� TSTR ���߂�܂��j
	TSTR		get_name_from_flag_no(int flag_no);																	// �t���O�ԍ����疼�O���擾�i���݂��Ȃ��ꍇ�A��� TSTR ���߂�܂��j
	TSTR		get_name_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);								// �O���[�v�R�[�h���疼�O���擾�i���݂��Ȃ��ꍇ�A��� TSTR ���߂�܂��j

	// �t���O�ԍ����擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_flag_no_from_name(CTSTR& name);																	// ���O����t���O�ԍ����擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_flag_no_from_list_no(int list_no);																// ���X�g�ԍ�����t���O�ԍ����擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_flag_no_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);							// �O���[�v�R�[�h����t���O�ԍ����擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j

	// �P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_one_code_value_from_name(CTSTR& name, int get_code_no);											// ���O����P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_one_code_value_from_list_no(int list_no, int get_code_no);										// ���X�g�ԍ�����P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_one_code_value_from_flag_no(int flag_no, int get_code_no);										// �t���O�ԍ�����P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j
	int			get_one_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int get_code_no);	// �O���[�v�R�[�h����P�̃R�[�h�l���擾�i���݂��Ȃ��ꍇ�A-1 ���߂�܂��j

	// �S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j
	ARRAY<int>	get_all_code_value_from_name(CTSTR& name);															// ���O����S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j
	ARRAY<int>	get_all_code_value_from_list_no(int list_no);														// ���X�g�ԍ�����S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j
	ARRAY<int>	get_all_code_value_from_flag_no(int flag_no);														// �t���O�ԍ�����S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j
	ARRAY<int>	get_all_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);					// �O���[�v�R�[�h����S�R�[�h�l���擾�i���݂��Ȃ��ꍇ�A��� ARRAY<int> ���߂�܂��j

	// �O���[�v�R�[�h����O���[�v�̃����o�[�����擾
	int			get_group_member_cnt(int gc0, int gc1, int gc2, int gc3, int gc4);									// �O���[�v�R�[�h����O���[�v�̃����o�[�����擾

	// �O���[�v�R�[�h����O���[�v�́u�����v�����o�[�����擾
	int			get_group_member_cnt_for_look(int cg0, int cg1, int cg2, int cg3, int cg4);							// �O���[�v�R�[�h����O���[�v�́u�����v�����o�[�����擾

	// �O���[�v�R�[�h����O���[�v���ꊇ���ăt���O�l��ݒ�
	void		set_flag_value_for_group_member(int cg0, int cg1, int cg2, int cg3, int cg4, int flag_value);		// �O���[�v�R�[�h����O���[�v���ꊇ���ăt���O�l��ݒ�

	// �b�f�e�[�u���f�[�^�ւ̃|�C���^���擾����
	C_cg_table_data*	get_ptr_to_cg_table_data()	{	return &cg_table;	}

	// �t���O�ւ̃|�C���^���擾����
	C_elm_int_list*		get_ptr_to_flag()			{	return &flag;	}

private:

	C_cg_table_data		cg_table;
	C_elm_int_list		flag;
};


