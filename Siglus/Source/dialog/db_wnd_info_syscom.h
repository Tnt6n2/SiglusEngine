#pragma		once

#include	"eng_proc.h"
#include	"db_wnd_info_listview.h"

class	Ctnm_input;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e�����
// ================================================================
class Cdb_wnd_info_system : public Cdb_wnd_info_listview
{

public:

	void		init();		// ������
	void		open();		// �J���i�쐬�j
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int				cur_scn_no;
		int				cur_line_no;
		TNM_PROC_TYPE	proc_type;
		int				stack_size;
		int				element_stack_size;
		int				real_time;
		int				local_real_time;
		int				local_game_time;
		bool			ctrl_skip_flag;

		int				moji_map_use_cnt;		// �g�p���̕����}�b�v��
		int				moji_map_now_cnt;		// ���݂̕����}�b�v��
		int				moji_map_max_cnt;		// �ő�̕����}�b�v��

		int				gan_data_map_now_cnt;	// ���݂̂f�`�m�f�[�^�}�b�v��
		int				gan_data_map_max_cnt;	// �ő�̂f�`�m�f�[�^�}�b�v��

		int				test;

	}	m_ld;

	// ��ƕϐ�
	bool	m_first;	// ����
};

