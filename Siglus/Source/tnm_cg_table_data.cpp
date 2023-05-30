#include	"pch.h"
#include	"lzss.h"

#include	"data/tnm_cg_table_data.h"

// ****************************************************************
// �ǂݍ���
// ================================================================
bool C_cg_table_data::load(CTSTR& file_path)
{
	cg_table_data_list.clear();
	cg_table_name_find_map.clear();
	cg_table_flag_find_map.clear();
	cg_table_sort_list.clear();
	cg_table_group_tree.tree.clear();

	ARRAY<BYTE> buf;
	if (!C_file::read_full_data(file_path, buf))	{	return false;	}
	if (!expand(buf))	{	return false;	}

	create_name_find_map();			// ���O�����}�b�v�쐬
	create_flag_find_map();			// �t���O�����}�b�v�쐬
	create_sort_list();				// �\�[�g���X�g�쐬
	create_group_tree();			// �O���[�v�c���[�쐬
	return true;
}

// ****************************************************************
// ��
// ================================================================

const int	AVG_CG_TABLE_NAME_LEN = 32;

bool C_cg_table_data::expand(ARRAY<BYTE>& buf)
{
	cg_table_data_list.clear();

	struct AVG_CG_TABLE_HEADER
	{
		BYTE head[16];		// �F���q
		int cnt;			// �o�^��
		int auto_flag;		// �t���O�l�̎������蓖�āi1 = �t���O�l�͒ʂ��ԍ��Ŏ����I�Ɋ��蓖�Ă��Ă��܂��B�f�[�^���G���R�[�h���鎞�ɕK�v�Ȓl�ł��j
		int rev[2];			// ���U�[�u
	};

	if (buf.size() < sizeof(AVG_CG_TABLE_HEADER))	{	return false;	}

	AVG_CG_TABLE_HEADER *header = (AVG_CG_TABLE_HEADER *)buf.get();
	int data_cnt = header->cnt;

	if (MBSTR((char *)header->head) == "CGTABLE2")	{
		BYTE *wp = buf.get() + sizeof(AVG_CG_TABLE_HEADER);
		int size = (int)buf.size() - sizeof(AVG_CG_TABLE_HEADER);
		tpc_angou(wp, size);
		ARRAY<BYTE> expand_data;
		size = lzss_unpack(wp, NULL);
		expand_data.resize(size);
		lzss_unpack(wp, expand_data.get());
		struct AVG_CG_TABLE_LIST_NEW
		{
			BYTE name[AVG_CG_TABLE_NAME_LEN];
			int flag_no;
			int code[CG_TABLE_DATA_CODE_MAX];
			int code_exist_cnt;
		};
		AVG_CG_TABLE_LIST_NEW *list = (AVG_CG_TABLE_LIST_NEW *)expand_data.get();
		cg_table_data_list.resize(data_cnt);
		for (int i = 0; i < data_cnt; i++)	{
			cg_table_data_list[i].name = MBSTR_to_TSTR((char *)list->name);
			str_to_upper(cg_table_data_list[i].name);
			cg_table_data_list[i].flag_no = list->flag_no;
			cg_table_data_list[i].code_exist_cnt = list->code_exist_cnt;
			cg_table_data_list[i].list_no = i;
			for (int j = 0; j < CG_TABLE_DATA_CODE_MAX; j++)	{
				cg_table_data_list[i].code[j] = list->code[j];
				cg_table_data_list[i].group[j] = -1;
			}
			list++;
		}
		expand_data.clear();
		return true;
	}

	if (MBSTR((char *)header->head) == "CGTABLE")	{
		BYTE *wp = buf.get() + sizeof(AVG_CG_TABLE_HEADER);
		int size = (int)buf.size() - sizeof(AVG_CG_TABLE_HEADER);
		tpc_angou(wp, size);
		ARRAY<BYTE> expand_data;
		size = lzss_unpack(wp, NULL);
		expand_data.resize(size);
		lzss_unpack(wp, expand_data.get());
		struct AVG_CG_TABLE_LIST_OLD
		{
			BYTE name[AVG_CG_TABLE_NAME_LEN];
			int flag_no;
		};
		AVG_CG_TABLE_LIST_OLD *list = (AVG_CG_TABLE_LIST_OLD *)expand_data.get();
		cg_table_data_list.resize(data_cnt);
		for (int i = 0; i < data_cnt; i++)	{
			cg_table_data_list[i].name = MBSTR_to_TSTR((char *)list->name);
			str_to_upper(cg_table_data_list[i].name);
			cg_table_data_list[i].flag_no = list->flag_no;
			cg_table_data_list[i].code_exist_cnt = 0;
			cg_table_data_list[i].list_no = i;
			for (int j = 0; j < CG_TABLE_DATA_CODE_MAX; j++)	{
				cg_table_data_list[i].code[j] = 0;
				cg_table_data_list[i].group[j] = -1;
			}
			list++;
		}
		expand_data.clear();
		return true;
	}

	return false;
}

// ****************************************************************
// ���O�����}�b�v�쐬
// ================================================================
bool C_cg_table_data::create_name_find_map()
{
	cg_table_name_find_map.clear();
	CG_TABLE_NAME_FIND_PAIR pair;
	for (int i = 0; i < (int)cg_table_data_list.size(); i++)	{
		pair.first = cg_table_data_list[i].name;
		pair.second = &cg_table_data_list[i];
		cg_table_name_find_map.insert(pair);
	}
	return true;
}

// ****************************************************************
// �t���O�����}�b�v�쐬
// ================================================================
bool C_cg_table_data::create_flag_find_map()
{
	cg_table_flag_find_map.clear();
	CG_TABLE_FLAG_FIND_PAIR pair;
	for (int i = 0; i < (int)cg_table_data_list.size(); i++)	{
		pair.first = cg_table_data_list[i].flag_no;
		pair.second = &cg_table_data_list[i];
		cg_table_flag_find_map.insert(pair);
	}
	return true;
}

// ****************************************************************
// �\�[�g���X�g�쐬
// ================================================================

// �\�[�g�֐�
int cg_table_data_create_sort_list_func_comp(S_cg_table_data_sub*& lhs, S_cg_table_data_sub*& rhs)
{
	for (int i = 0; i < CG_TABLE_DATA_CODE_MAX; i++)	{
		if (lhs->code[i] < rhs->code[i])		{	return true;	}
		else if (lhs->code[i] > rhs->code[i])	{	return false;	}
	}
	if (lhs->list_no < rhs->list_no)			{	return true;	}
	else if (lhs->list_no > rhs->list_no)		{	return false;		}
	return false;
}

bool C_cg_table_data::create_sort_list()
{
	cg_table_sort_list.clear();
	if (cg_table_data_list.empty())	{	return true;	}
	cg_table_sort_list.resize((int)cg_table_data_list.size());
	for (int i = 0; i < (int)cg_table_data_list.size(); i++)	{
		cg_table_sort_list[i] = &cg_table_data_list[i];
	}
	std::sort(cg_table_sort_list.begin(), cg_table_sort_list.end(), cg_table_data_create_sort_list_func_comp);
	return true;
}

// ****************************************************************
// �O���[�v�c���[�쐬
// ================================================================

bool C_cg_table_data::create_group_tree()
{
	cg_table_group_tree.tree.clear();
	int code[CG_TABLE_DATA_CODE_MAX];
	create_group_tree_func(cg_table_group_tree, 0, code, 0);
	return true;
}

// �ċA�֐�
void C_cg_table_data::create_group_tree_func(S_cg_table_group_tree& group_tree, int sort_list_index, int *code, int code_index)
{
	int sort_list_index_backup = sort_list_index;

	S_cg_table_data_sub *sub;
	bool loop_out;
	int i;

	// �m�ې��Z�o
	int group_cnt = 0;
	int now_code = -1;
	while (1)	{
		sub = cg_table_sort_list[sort_list_index];
		loop_out = false;
		for (i = 0; i < code_index; i++)	{
			if (sub->code[i] != code[i])	{	loop_out = true;	break;	}
		}
		if (loop_out)	{	break;	}

		if (sub->code[code_index] != now_code || code_index == (CG_TABLE_DATA_CODE_MAX - 1))	{
			now_code = sub->code[code_index];
			group_cnt++;
		}

		sort_list_index++;
		if (sort_list_index == (int)cg_table_sort_list.size())	{	break;	}
	}

	if (group_cnt <= 0)	{	return;	}

	sort_list_index = sort_list_index_backup;

	// �c���[�m��
	group_tree.tree.resize(group_cnt);

	// �������g��ݒ�
	group_tree.sub = cg_table_sort_list[sort_list_index];

	// �ċA�����J�n
	now_code = -1;
	int groupe_no = 0;
	int tree_index = 0;
	while (1)	{
		sub = cg_table_sort_list[sort_list_index];
		loop_out = false;
		for (i = 0; i < code_index; i++)	{
			if (sub->code[i] != code[i])	{	loop_out = true;	break;	}
		}
		if (loop_out)	{	break;	}

		if (sub->code[code_index] != now_code || code_index == (CG_TABLE_DATA_CODE_MAX - 1))	{
			if (now_code != -1)	{	groupe_no++;	}
			now_code = sub->code[code_index];
			if ((code_index + 1) < CG_TABLE_DATA_CODE_MAX)	{
				code[code_index] = now_code;
				create_group_tree_func(group_tree.tree[tree_index], sort_list_index, code, code_index + 1);
			}
			group_tree.tree[tree_index].sub = sub;
			tree_index++;
		}

		sub->group[code_index] = groupe_no;
		sort_list_index++;
		if (sort_list_index == (int)cg_table_sort_list.size())	{	break;	}
	}
}

// ****************************************************************
// �b�f�e�[�u���f�[�^�\���̂̃|�C���^�擾
// ================================================================

// ���O����擾
S_cg_table_data_sub * C_cg_table_data::get_sub_pointer_from_name_func(CTSTR& name)
{
	if (cg_table_name_find_map.empty())	{	return NULL;	}
	TSTR upper_name = str_to_upper_ret(name);
	CG_TABLE_NAME_FIND_MAP::iterator itr = cg_table_name_find_map.find(upper_name);
	if (itr == cg_table_name_find_map.end())	{	return NULL;	}
	return itr->second;
}

// ���X�g�ԍ�����擾
S_cg_table_data_sub * C_cg_table_data::get_sub_pointer_from_list_no_func(int list_no)
{
	if (cg_table_data_list.empty())	{	return NULL;	}
	if (!(list_no >= 0 && list_no < (int)cg_table_data_list.size()))	{	return NULL;	}
	return &cg_table_data_list[list_no];
}

// �t���O�ԍ�����擾
S_cg_table_data_sub * C_cg_table_data::get_sub_pointer_from_flag_no_func(int flag_no)
{
	if (cg_table_flag_find_map.empty())	{	return NULL;	}
	CG_TABLE_FLAG_FIND_MAP::iterator itr = cg_table_flag_find_map.find(flag_no);
	if (itr == cg_table_flag_find_map.end())	{	return NULL;	}
	return itr->second;
}

// �O���[�v�R�[�h����擾
S_cg_table_data_sub * C_cg_table_data::get_sub_pointer_from_group_code_func(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_group_tree * group = get_groupe_tree_pointer_func(gc0, gc1, gc2, gc3, gc4);
	return (group) ? group->sub : NULL;
}

// ****************************************************************
// �b�f�e�[�u���O���[�v�c���[�̃|�C���^�擾
// ================================================================

S_cg_table_group_tree * C_cg_table_data::get_groupe_tree_pointer_func(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	if (cg_table_group_tree.tree.empty())	{	return NULL;	}
	int code[CG_TABLE_DATA_CODE_MAX];
	code[0] = gc0;	code[1] = gc1;	code[2] = gc2;	code[3] = gc3;	code[4] = gc4;
	return get_groupe_tree_pointer_funcfunc(&cg_table_group_tree.tree[0], code, 0);
}

// �ċA�֐�
S_cg_table_group_tree * C_cg_table_data::get_groupe_tree_pointer_funcfunc(S_cg_table_group_tree *group, int *code, int code_index)
{
	int gc = code[code_index];
	if (gc == -1)									{	return group;				}
	if (!(gc >= 0 && gc < (int)group->tree.size()))	{	return NULL;				}
	if (code_index + 1 >= CG_TABLE_DATA_CODE_MAX)	{	return &group->tree[gc];	}
	if (code[code_index + 1] == -1)					{	return &group->tree[gc];	}
	return get_groupe_tree_pointer_funcfunc(&group->tree[gc], code, code_index + 1);
}

// ****************************************************************
// �O���[�v�R�[�h����t���O���X�g�擾����i���݂��Ȃ��ꍇ�AARRAY<int> �͋�ɂȂ�܂��j
// ================================================================

void C_cg_table_data::get_flag_list_func(int gc0, int gc1, int gc2, int gc3, int gc4, ARRAY<int>& flag_list)
{
	flag_list.clear();
	S_cg_table_group_tree *group = get_groupe_tree_pointer_func(gc0, gc1, gc2, gc3, gc4);
	if (group == NULL || (int)group->tree.size() == 0)	{	return;	}
	get_flag_list_funcfunc(group, flag_list);
}

// �ċA�֐�
void C_cg_table_data::get_flag_list_funcfunc(S_cg_table_group_tree *group, ARRAY<int>& flag_list)
{
	if (group->tree.size() <= 1)	{
		flag_list.push_back(group->sub->flag_no);
		return;
	}
	for (int i = 0; i < (int)group->tree.size(); i++)	{
		get_flag_list_funcfunc(&group->tree[i], flag_list);
	}
}
