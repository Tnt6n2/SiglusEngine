#pragma		once

#include	"data/tnm_proc_def.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e�����
// ================================================================
class C_db_wnd_info_system : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int				frame_rate;					// �t���[�����[�g
		TSTR			save_point;					// �Z�[�u�|�C���g
		bool			sel_point_exist;			// �I�����|�C���g
		bool			capture_exist_flag;			// �L���v�`���[�����݂���
		bool			ex_call_flag;				// �d�w�R�[����
		bool			system_wipe_flag;			// �V�X�e�����C�v��
		bool			game_timer_move_flag;		// �Q�[�������������Ă�
		int				stack_size;					// �X�^�b�N�̃T�C�Y
		int				element_stack_size;			// �G�������g�X�^�b�N�̃T�C�Y
		int				msg_back_save_size;			// ���b�Z�[�W�o�b�N�Z�[�u�̃T�C�Y

		int				real_time;					// ���݂̎�����
		int				local_real_time;			// ���[�J���̎�����
		int				local_game_time;			// ���[�J���̃Q�[������
		int				local_wipe_time;			// ���[�J���̃��C�v����
		bool			ctrl_skip_flag;				// Ctrl �X�L�b�v��
		int				skip_speed;					// �X�L�b�v���x
		bool			cant_auto_skip_before_click;	// �X�L�b�v���~�߂�

		bool			active_flag;				// �E�B���h�E���A�N�e�B�u�ł���
		bool			focused_flag;				// �E�B���h�E�Ƀt�H�[�J�X������
		bool			focused_flag_on_edit_box;	// �G�f�B�b�g�{�b�N�X�Ƀt�H�[�J�X������

		int				total_bgm_fade;				// �ŏI�a�f�l�t�F�[�h�l
		int				total_bgm_fade2;			// �ŏI�a�f�l�t�F�[�h�l
		int				total_bgm_volume;			// �ŏI�a�f�l�{�����[��
		int				total_koe_volume;			// �ŏI���{�����[��
		int				total_pcm_volume;			// �ŏI���ʉ��{�����[��
		int				total_se_volume;			// �ŏI�V�X�e�����{�����[��
		int				total_mov_volume;			// �ŏI���[�r�[�{�����[��
		int				mov_volume;
		int				mov_volume_real;
		bool			play_silent_sound;			// �o�b�N�O���E���h�Ŗ������Đ�����

		int				video_memory_use_size;		// �r�f�I�������g�p�T�C�Y
		int				video_memory_use_max_size;	// �r�f�I�������g�p�T�C�Y�i�ő�l�j
		int				max_vertex_buffer_size;		// ���_�o�b�t�@�T�C�Y
		int				moji_map_use_cnt;			// �g�p���̕����}�b�v��
		int				moji_map_now_cnt;			// ���݂̕����}�b�v��
		int				moji_map_max_cnt;			// �ő�̕����}�b�v��
		int				moji_memory_use_size;		// �g�p���̕����������[�T�C�Y
		int				moji_memory_now_size;		// ���݂̕����g�p�������[�T�C�Y
		int				moji_memory_max_size;		// �ő�̕����g�p�������[�T�C�Y
		int				moji_kind_useful_cnt;		// �L�v�ȕ�����
		int				moji_kind_unuseful_cnt;		// ��L�v�ȕ�����
		int				gan_data_map_now_cnt;		// ���݂̂f�`�m�f�[�^�}�b�v��
		int				gan_data_map_max_cnt;		// �ő�̂f�`�m�f�[�^�}�b�v��

		int				test1;
		int				test2;
		int				test3;
		int				test4;
		int				test5;
		int				test6;
		int				test7;
		int				test8;
		int				test9;

	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

