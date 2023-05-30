#pragma		once

#include	"tnm_element_code.h"

#include	"data/tnm_array.h"
#include	"data/tnm_proc.h"

// ****************************************************************
// �G���W���F���[�J���f�[�^
// ================================================================
//		�V�[�����s�Ɋւ���ꎞ�I�ȃf�[�^�������܂��B
//		�����̓Z�[�u�f�[�^�ɕۑ�����܂��B
// ================================================================
struct C_tnm_local_data_pod
{
	void				init();

	int					cur_koe_no;						// ���݂̕��͂Ɋ��蓖�Ă��Ă��鐺�ԍ�
	int					cur_chr_no;						// ���݂̕��͂Ɋ��蓖�Ă��Ă��鐺�L�����N�^�[�ԍ�
	int					cur_read_flag_scn_no;			// ���݂̕��͂Ɋ��蓖�Ă��Ă�����ǃt���O�̃V�[���ԍ�
	int					cur_read_flag_flag_no;			// ���݂̕��͂Ɋ��蓖�Ă��Ă�����ǃt���O�̃t���O�ԍ�
	int					cursor_no;						// �J�[�\���ԍ�

	// �悭�g��
	bool				syscom_menu_disable;			// �V�X�e���R�}���h���j���[�֎~
	bool				hide_mwnd_disable;				// ���b�Z�[�W�E�B���h�E���B���֎~
	bool				msg_back_disable;				// ���b�Z�[�W�o�b�N�֎~
	bool				shortcut_disable;				// �V���[�g�J�b�g�֎~

	// ���b�Z�[�W
	bool				skip_disable;					// �X�L�b�v�֎~
	bool				ctrl_disable;					// �X�L�b�v�֎~�iCtrl �܂ށj
	bool				not_stop_skip_by_click;			// �N���b�N���Ă��X�L�b�v���~�߂Ȃ�
	bool				not_skip_msg_by_click;			// �N���b�N���Ă����b�Z�[�W���X�L�b�v���Ȃ�
	bool				skip_unread_message;			// ���ǂ̕��͂��X�L�b�v
	bool				auto_mode_flag;					// �I�[�g���[�h�t���O
	int					auto_mode_moji_wait;			// �I�[�g���[�h�����҂�����
	int					auto_mode_min_wait;				// �I�[�g���[�h�ŏ��҂�����
	int					auto_mode_moji_cnt;				// �I�[�g���[�h���̕�����
	int					mouse_cursor_hide_onoff;		// �����Ń}�E�X�J�[�\�����B��
	int					mouse_cursor_hide_time;			// �����Ń}�E�X�J�[�\�����B������
	int					msg_back_save_cntr;				// ���b�Z�[�W�o�b�N�𐔌u���ɕۑ����邽�߂̃J�E���^

	int					msg_speed;						// ���b�Z�[�W�X�s�[�h
	bool				msg_nowait;						// ���b�Z�[�W�m�[�E�F�C�g
	bool				async_msg_mode;					// �񓯊����b�Z�[�W���[�h
	bool				async_msg_mode_once;			// �񓯊����b�Z�[�W���[�h�i�P�x����j
	bool				multi_msg_mode;					// �}���`���b�Z�[�W���[�h
	bool				skip_trigger;					// �X�L�b�v�g���K�[
	bool				koe_dont_stop_on_flag;			// �����~�߂Ȃ�
	bool				koe_dont_stop_off_flag;			// �����~�߂�

	// ���b�Z�[�W�E�B���h�E
	bool				mwnd_btn_disable_all;			// ���b�Z�[�W�E�B���h�E�{�^���֎~�i�S�āj
	bool				mwnd_btn_touch_disable;			// ���b�Z�[�W�E�B���h�E�{�^���G��Ȃ�
	bool				mwnd_anime_on_flag;				// ���b�Z�[�W�E�B���h�E�̃A�j�����s��
	bool				mwnd_anime_off_flag;			// ���b�Z�[�W�E�B���h�E�̃A�j�����s��Ȃ�
	bool				mwnd_disp_off_flag;				// ���b�Z�[�W�E�B���h�E������

	// ���b�Z�[�W�o�b�N
	bool				msg_back_off;					// ���b�Z�[�W�o�b�N�g��Ȃ�
	bool				msg_back_disp_off;				// ���b�Z�[�W�o�b�N���\���ɂ���

	// �t�H���g
	int					font_futoku;					// �t�H���g����
	int					font_shadow;					// �t�H���g�e

	// ����
	bool				cursor_disp_off;				// �J�[�\��������
	bool				cursor_move_by_key_disable;		// �L�[�{�[�h�ŃJ�[�\���𓮂����@�\���֎~
	bool				key_disable[256];				// �L�[�𖳌��ɂ���

	// ���̑�
	bool				quake_stop_flag;				// �N�G�C�N���~�߂�
	bool				emote_mouth_stop_flag;			// E-mote �̌��p�N���~�߂�
	bool				cg_table_off_flag;				// �b�f�e�[�u�����g��Ȃ�
	bool				bgmfade_flag;					// �a�f�l�t�F�[�h������
	bool				dont_set_save_point;			// �Z�[�u�|�C���g��ݒu���Ȃ�
	bool				ignore_r_flag;					// �q�𖳎�����I�I�I�I
	bool				wait_display_vsync_off_flag;	// �f�B�X�v���C�̐���������҂��Ȃ�

	bool				time_stop_flag;					// ���Ԃ�i�߂Ȃ�
	bool				counter_time_stop_flag;			// ���Ԃ�i�߂Ȃ�
	bool				frame_action_time_stop_flag;	// ���Ԃ�i�߂Ȃ�
	bool				stage_time_stop_flag;			// ���Ԃ�i�߂Ȃ�
};

class C_tnm_local_data
{
public:
	void				init();

	C_tnm_proc						cur_proc;			// ���݂̃v���Z�X
	C_tnm_class_array<C_tnm_proc>	proc_stack;			// �v���Z�X�L���[

	S_element			cur_mwnd;						// ���݂̃��b�Z�[�W�E�B���h�E
	S_element			cur_sel_mwnd;					// ���݂̑I�����p�̃��b�Z�[�W�E�B���h�E
	S_element			last_mwnd;						// �Ō�ɕ\�����s�������b�Z�[�W�E�B���h�E�i���b�Z�[�W�o�b�N�Ŏ��ɍs�����̔���Ɏg���j
	TSTR				scene_title;					// �V�[���^�C�g��
	TSTR				cur_full_message;				// ���݂̃t�����b�Z�[�W

	// ���b�Z�[�W�E�B���h�E
	ARRAY<S_bool>		mwnd_btn_disable;				// ���b�Z�[�W�E�B���h�E�{�^���֎~

	// �t�H���g
	TSTR				font_name;						// �t�H���g��

	// pod
	C_tnm_local_data_pod			pod;
};

