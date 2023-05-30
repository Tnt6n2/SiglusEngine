#pragma		once

// ****************************************************************
// �y���O�z
// ================================================================

typedef		std::pair<TSTR, int>	NAMAE_FIND_PAIR;
typedef		std::map<TSTR, int>		NAMAE_FIND_MAP;

class C_tnm_namae
{

public:

	void		init();						// �y���O�z������
	TSTR		change(CTSTR& name, int *color_mod, int *moji_color_no, int *shadow_color_no, int *fuchi_color_no);		// �y���O�z�ϊ�

	// �f�[�^�x�[�X�p���O�i�f�o�b�O���p�j
	struct Snamae_data_base
	{
		bool		regist_flag;			// �o�^�̗L���iGameexe.ini �ɓo�^����Ă��邩�ǂ����j
		bool		exist_flag;				// ���݂̗L���i�V�[�����Ŏg�p����Ă��邩�ǂ����j
		TSTR		regist_name_str;		// �o�^��
		TSTR		change_name_str;		// �ϊ���
		int			color_mod;				// �F���[�h
		int			moji_color_no;			// �����F
		int			shadow_color_no;		// �e�F
	};
	ARRAY<Snamae_data_base>		namae_data_base_list;

private:

	// �����p���O�}�b�v
	NAMAE_FIND_MAP		namae_find_map;

};

// ****************************************************************
// �L�����N�^����
// ================================================================
class C_tnm_chrkoe
{
public:
	TSTR	name_str;
	bool	look_flag;
};

