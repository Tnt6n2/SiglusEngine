#pragma		once

// ****************************************************************
// �b�f�e�[�u���f�[�^
// ================================================================

const int	CG_TABLE_DATA_CODE_MAX = 5;
const int	CG_TABLE_DATA_CODE_VALUE_MAX = 999;

// �b�f�e�[�u���f�[�^�\����
struct S_cg_table_data_sub
{
	TSTR		name;
	int			flag_no;
	int			code_exist_cnt;
	int			code[CG_TABLE_DATA_CODE_MAX];
	int			list_no;
	int			group[CG_TABLE_DATA_CODE_MAX];
};

// �b�f�e�[�u�����O�����}�b�v
typedef		std::pair<TSTR, S_cg_table_data_sub*>	CG_TABLE_NAME_FIND_PAIR;
typedef		std::map<TSTR, S_cg_table_data_sub*>	CG_TABLE_NAME_FIND_MAP;

// �b�f�e�[�u���t���O�����}�b�v
typedef		std::pair<int, S_cg_table_data_sub*>	CG_TABLE_FLAG_FIND_PAIR;
typedef		std::map<int, S_cg_table_data_sub*>		CG_TABLE_FLAG_FIND_MAP;

// �b�f�e�[�u���O���[�v�c���[
struct S_cg_table_group_tree
{
	S_cg_table_data_sub *sub;
	ARRAY<S_cg_table_group_tree> tree;

	S_cg_table_group_tree()	{	sub = NULL;	}		// �R���X�g���N�^
};

// �{��
class C_cg_table_data
{
public:

	ARRAY<S_cg_table_data_sub>		cg_table_data_list;			// �b�f�e�[�u���f�[�^���X�g
	CG_TABLE_NAME_FIND_MAP			cg_table_name_find_map;		// �b�f�e�[�u�����O�����}�b�v
	CG_TABLE_FLAG_FIND_MAP			cg_table_flag_find_map;		// �b�f�e�[�u���t���O�����}�b�v
	ARRAY<S_cg_table_data_sub *>	cg_table_sort_list;			// �b�f�e�[�u���\�[�g���X�g
	S_cg_table_group_tree			cg_table_group_tree;		// �b�f�e�[�u���O���[�v�c���[

	// �ǂݍ���
	bool		load(CTSTR& file_path);											// �ǂݍ���

	// �b�f�e�[�u���f�[�^�\���̂̃|�C���^�擾
	S_cg_table_data_sub * get_sub_pointer_from_name_func(CTSTR& name);											// ���O����擾
	S_cg_table_data_sub * get_sub_pointer_from_list_no_func(int list_no);										// ���X�g�ԍ�����擾
	S_cg_table_data_sub * get_sub_pointer_from_flag_no_func(int flag_no);										// �t���O�ԍ�����擾
	S_cg_table_data_sub * get_sub_pointer_from_group_code_func(int gc0, int gc1, int gc2, int gc3, int gc4);		// �O���[�v�R�[�h����擾

	// �b�f�e�[�u���O���[�v�c���[�̃|�C���^�擾
	S_cg_table_group_tree * get_groupe_tree_pointer_func(int cg0, int cg1, int cg2, int cg3, int cg4);

	// �O���[�v�R�[�h����t���O���X�g�擾����i���݂��Ȃ��ꍇ�AARRAY<int> �͋�ɂȂ�܂��j
	void		get_flag_list_func(int gc0, int gc1, int gc2, int gc3, int gc4, ARRAY<int>& flag_list);

private:

	// �ǂݍ��ݎ��̏����֐�
	bool	expand(ARRAY<BYTE>& buf);		// ��
	bool	create_name_find_map();			// ���O�����}�b�v�쐬
	bool	create_flag_find_map();			// �t���O�����}�b�v�쐬
	bool	create_sort_list();				// �\�[�g���X�g�쐬
	bool	create_group_tree();			// �O���[�v�c���[�쐬
	void	create_group_tree_func(S_cg_table_group_tree& group_tree, int sort_list_index, int *code, int code_index);		// �O���[�v�c���[�쐬�t�@���N�V����

	// �u�b�f�e�[�u���O���[�v�c���[�̃|�C���^�擾�v�̍ċN�֐�
	S_cg_table_group_tree * get_groupe_tree_pointer_funcfunc(S_cg_table_group_tree *group, int *code, int code_index);

	// �u�O���[�v�R�[�h����t���O���X�g�擾����v�̍ċN�֐�
	void		get_flag_list_funcfunc(S_cg_table_group_tree *group, ARRAY<int>& flag_list);

};


