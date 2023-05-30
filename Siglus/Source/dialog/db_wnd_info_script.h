#pragma		once

#include	"data/tnm_proc_def.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�X�N���v�g���
// ================================================================
class C_db_wnd_info_script : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int				cur_scn_no;							// ���݂̃V�[���ԍ�
		int				cur_line_no;						// ���݂̍s�ԍ�
		TNM_PROC_TYPE	proc_type;							// ���݂̃v���Z�X
		bool			syscom_menu_disable;				// �V�X�e���R�}���h���j���[�֎~
		bool			hide_mwnd_disable;					// ���b�Z�[�W�E�B���h�E���B���֎~
		bool			msg_back_disable;					// ���b�Z�[�W�o�b�N�֎~
		bool			shortcut_disable;					// �V���[�g�J�b�g�֎~
		bool			mwnd_btn_disable_all;				// �E�B���h�E�{�^���֎~
		bool			mwnd_btn_touch_disable;				// �E�B���h�E�{�^���G��Ȃ�
		bool			skip_disable;						// �X�L�b�v�֎~
		bool			ctrl_disable;						// Ctrl �X�L�b�v�֎~
		bool			not_stop_skip_by_click;				// �N���b�N���Ă��X�L�b�v���~�߂Ȃ�
		bool			not_skip_msg_by_click;				// �N���b�N���Ă����b�Z�[�W���ꊇ�\�����Ȃ�
		bool			skip_unread_message;				// Ctrl �X�L�b�v�֎~
		bool			msg_back_off;						// ���b�Z�[�W�o�b�N���g��Ȃ�
		bool			msg_back_disp_off;					// ���b�Z�[�W�o�b�N���\���ɂ���
		bool			auto_mode_flag;						// �I�[�g���[�h
		int				msg_speed;							// ���b�Z�[�W�X�s�[�h
		bool			msg_nowait;							// ���b�Z�[�W�m�[�E�F�C�g
		bool			async_msg_mode;						// �񓯊����b�Z�[�W���[�h
		bool			multi_msg_mode;						// �}���`���b�Z�[�W���[�h
		bool			skip_trigger;						// �X�L�b�v�g���K�[
		bool			cursor_disp_off;					// �J�[�\��������
		bool			cursor_move_by_key_disable;			// �L�[�{�[�h�ŃJ�[�\���𓮂����Ȃ�
		bool			mwnd_anime_on_flag;					// ���b�Z�[�W�E�B���h�E�̃A�j�����s���t���O
		bool			mwnd_anime_off_flag;				// ���b�Z�[�W�E�B���h�E�̃A�j�����s��Ȃ��t���O
		bool			mwnd_disp_off_flag;					// ���b�Z�[�W�E�B���h�E�������t���O
		bool			koe_dont_stop_on_flag;				// �����~�߂Ȃ��I���t���O
		bool			koe_dont_stop_off_flag;				// �����~�߂Ȃ��I�t�t���O
		bool			quake_stop_flag;					// �N�G�C�N���~�߂�t���O
		bool			cg_table_off_flag;					// �b�f�e�[�u�����g��Ȃ��t���O
		bool			bgmfade_flag;						// �a�f�l�t�F�[�h��
		bool			dont_set_save_point;				// �Z�[�u�|�C���g��ݒ肵�Ȃ�
		bool			wait_display_vsync_off_flag;		// �f�B�X�v���C�̐���������҂��I�t�ɂ���t���O

		bool			syscom_read_skip;					// �i���Ѻ���ށj���Ǒ�����
		bool			syscom_auto_skip;					// �i���Ѻ���ށj����������
		bool			syscom_auto_mode;					// �i���Ѻ���ށj�I�[�g���[�h
		bool			syscom_hide_mwnd;					// �i���Ѻ���ށj���b�Z�[�W�E�B���h�E���B��
		bool			syscom_msg_back;					// �i���Ѻ���ށj���b�Z�[�W�o�b�N

		bool			syscom_read_skip_disable;			// �i���Ѻ�����ƭ��j���Ǒ�����֎~
		bool			syscom_auto_skip_disable;			// �i���Ѻ�����ƭ��j����������֎~
		bool			syscom_auto_mode_disable;			// �i���Ѻ�����ƭ��j�I�[�g���[�h�֎~
		bool			syscom_hide_mwnd_disable;			// �i���Ѻ�����ƭ��j���b�Z�[�W�E�B���h�E���B���֎~
		bool			syscom_msg_back_disable;			// �i���Ѻ�����ƭ��j���b�Z�[�W�o�b�N�֎~
		bool			syscom_save_disable;				// �i���Ѻ�����ƭ��j�Z�[�u�֎~
		bool			syscom_load_disable;				// �i���Ѻ�����ƭ��j���[�h�֎~
		bool			syscom_return_to_sel_disable;		// �i���Ѻ�����ƭ��j�O�̑I�����ɖ߂�֎~

		bool			syscom_read_skip_off;				// �i���Ѻ�����ƭ��j���Ǒ�����֎~
		bool			syscom_auto_skip_off;				// �i���Ѻ�����ƭ��j����������֎~
		bool			syscom_auto_mode_off;				// �i���Ѻ�����ƭ��j�I�[�g���[�h�֎~
		bool			syscom_hide_mwnd_off;				// �i���Ѻ�����ƭ��j���b�Z�[�W�E�B���h�E�������֎~
		bool			syscom_msg_back_off;				// �i���Ѻ�����ƭ��j���b�Z�[�W�o�b�N�֎~
		bool			syscom_save_off;					// �i���Ѻ�����ƭ��j�Z�[�u��\��
		bool			syscom_load_off;					// �i���Ѻ�����ƭ��j���[�h��\��
		bool			syscom_return_to_sel_off;			// �i���Ѻ�����ƭ��j�O�̑I�����ɖ߂�֎~

	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

