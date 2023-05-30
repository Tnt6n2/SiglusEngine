#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_namae.h"

// ****************************************************************
// �y���O�z������
// ================================================================
void C_tnm_namae::init()
{
	// �N���A
	namae_find_map.clear();
	namae_data_base_list.clear();

	// �V�[���f�[�^���牼�}�b�v���쐬
	std::set<TSTR> set;
	int scn_cnt = (int)Gp_lexer->get_scn_cnt();
	int namae_cnt;
	for (int i = 0; i < scn_cnt; i++)	{
		namae_cnt = (int)Gp_lexer->get_namae_cnt(i);
		for (int j = 0; j < namae_cnt; j++)	{
			set.insert(Gp_lexer->get_namae_str(i, j));
		}
	}

	// �����}�b�v�쐬
	// �f�[�^�x�[�X�쐬
	if (!Gp_ini->namae_list.empty())	{
		NAMAE_FIND_PAIR pair;
		Snamae_data_base data_base;
		std::set<TSTR>::iterator set_itr;
		std::set<TSTR>::iterator set_end = set.end();
		int list_cnt = (int)Gp_ini->namae_list.size();
		for (int i = 0; i < list_cnt; i++)	{

			// �����}�b�v
			pair.first = Gp_ini->namae_list[i].regist_name_str;
			pair.second = i;
			namae_find_map.insert(pair);

			// �f�[�^�x�[�X
			data_base.regist_flag = true;	// Gameexe.ini �ɓo�^����Ă���
			set_itr = set.find(Gp_ini->namae_list[i].regist_name_str);		// ���}�b�v���猟��
			data_base.exist_flag = (set_itr != set_end) ? true : false;		// �V�[�����Ŏg�p����Ă��邩�ǂ����i���}�b�v�Ō�����Ύg�p����Ă��鎖�ɂȂ�j
			data_base.regist_name_str = Gp_ini->namae_list[i].regist_name_str;
			data_base.change_name_str = Gp_ini->namae_list[i].change_name_str;
			data_base.color_mod = Gp_ini->namae_list[i].color_mod;
			data_base.moji_color_no = Gp_ini->namae_list[i].moji_color_no;
			data_base.shadow_color_no = Gp_ini->namae_list[i].shadow_color_no;
			namae_data_base_list.push_back(data_base);
		}
	}

	// ���o�^�̖��O���f�[�^�x�[�X�ɒǉ�����
	if (!set.empty())	{
		int set_cnt = (int)set.size();
		NAMAE_FIND_MAP::iterator map_itr;
		NAMAE_FIND_MAP::iterator map_end = namae_find_map.end();
		std::set<TSTR>::iterator set_itr = set.begin();
		for (int i = 0; i < set_cnt; i++)	{
			map_itr = namae_find_map.find(*set_itr);
			if (map_itr == map_end)	{	// �����}�b�v�Ō�����Ȃ��Ƃ������́A�o�^����Ă��Ȃ��Ƃ������ł��B
				Snamae_data_base data_base;
				data_base.regist_flag = false;		// Gameexe.ini �ɓo�^����Ă��Ȃ�
				data_base.exist_flag = true;		// �V�[�����Ŏg�p����Ă���
				data_base.regist_name_str = *set_itr;
				namae_data_base_list.push_back(data_base);
			}
			set_itr++;
		}
	}
}

// ****************************************************************
// �y���O�z�ϊ�
// ================================================================
TSTR C_tnm_namae::change(CTSTR& name, int *color_mod, int *moji_color_no, int *shadow_color_no, int *fuchi_color_no)
{
	NAMAE_FIND_MAP::iterator map_itr;
	NAMAE_FIND_MAP::iterator map_end = namae_find_map.end();
	map_itr = namae_find_map.find(name);
	if (map_itr == map_end)	{	// �����}�b�v�Ō�����Ȃ��Ƃ������́A�o�^����Ă��Ȃ��Ƃ������ł��B
		if (color_mod)			{ *color_mod = -1; }
		if (moji_color_no)		{ *moji_color_no = -1; }
		if (shadow_color_no)	{ *shadow_color_no = -1; }
		if (fuchi_color_no)		{ *fuchi_color_no = -1; }
		return name;
	}

	C_tnm_ini::S_namae* p_namae = &Gp_ini->namae_list[map_itr->second];
	if (color_mod)			{ *color_mod = p_namae->color_mod; }
	if (moji_color_no)		{ *moji_color_no = p_namae->moji_color_no >= 0 ? p_namae->moji_color_no : Gp_ini->mwnd.moji_color; }
	if (shadow_color_no)	{ *shadow_color_no = p_namae->shadow_color_no >= 0 ? p_namae->shadow_color_no : Gp_ini->mwnd.shadow_color; }
	if (fuchi_color_no)		{ *fuchi_color_no = p_namae->fuchi_color_no >= 0 ? p_namae->fuchi_color_no : Gp_ini->mwnd.fuchi_color; }
	return Gp_ini->namae_list[map_itr->second].change_name_str;
}
