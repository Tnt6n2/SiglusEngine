#pragma		once

#include	"tnm_sorter.h"
#include	"tnm_timer.h"

// ****************************************************************
// �G���W���F�O���[�o���f�[�^
// ================================================================
//		�X�N���v�g����͐G��Ȃ��ݒ���������ł��B
//		�܂��A�����̓Z�[�u�̑ΏۂɂȂ�܂���B
// ================================================================
class C_tnm_global_data
{
public:
	bool		debug_flag;						// �f�o�b�O�t���O
	bool		debug_decided_flag;				// �f�o�b�O���ǂ������m�肵���t���O
	bool		debug_not_disp_overflow_error;	// �I�[�o�[�t���[���ɃG���[���o���Ȃ�
	bool		shader_from_source;				// �V�F�[�_�[���\�[�X����쐬����
	int			setup_type;						// �Z�b�g�A�b�v�^�C�v

	C_size		game_screen_size;				// �Q�[���X�N���[���T�C�Y�i�N�����ɌŒ�j
	C_point		total_game_screen_pos;			// �Q�[���X�N���[���ʒu���v�i�t���X�N���[���̒����Ȃǂ����v�����Q�[����ʈʒu�j
	C_point		total_game_screen_pos_last;		// �Q�[���X�N���[���ʒu���v�i�t���X�N���[���̒����Ȃǂ����v�����Q�[����ʈʒu�j
	C_size		total_game_screen_size;			// �Q�[���X�N���[���T�C�Y���v�i�t���X�N���[���̒����Ȃǂ����v�����Q�[����ʃT�C�Y�j
	C_size		total_game_screen_size_last;	// �Q�[���X�N���[���T�C�Y���v�i�t���X�N���[���̒����Ȃǂ����v�����Q�[����ʃT�C�Y�j
	C_size		total_screen_size;				// �X�N���[���T�C�Y���v�i�t���X�N���[���̒����Ȃǂ����v�����X�N���[���T�C�Y�j
	C_size		total_window_size;				// �E�B���h�E�T�C�Y���v�i�t���X�N���[���̒����Ȃǂ����v�����E�B���h�E�T�C�Y�j

	int			screen_size_mode;						// �X�N���[���T�C�Y���[�h
	int			screen_size_mode_window;				// �X�N���[���T�C�Y���[�h�i�E�B���h�E���j
	C_size		screen_size_scale;						// �X�N���[���T�C�Y�X�P�[��
	C_size		screen_size_free;						// �X�N���[���T�C�Y�i�t���[�j
	bool		fullscreen_change_resolution;			// �t���X�N���[���̉𑜓x�ύX�t���O
	int			fullscreen_display_no;					// �t���X�N���[���̃f�B�X�v���C�ԍ�
	int			fullscreen_resolution_no;				// �t���X�N���[���̉𑜓x�ԍ�
	int			screen_size_mode_change_start_time;		// �X�N���[����؂�ւ������ԁi��ʐݒ�E�B���h�E�p�j

	bool		dialog_fullscreen_change_resolution;	// �t���X�N���[�����ɉ𑜓x��ύX����i�_�C�A���O�̐ݒ�j
	int			dialog_fullscreen_display_no;			// �t���X�N���[���Ɏg���f�B�X�v���C�ԍ��i�_�C�A���O�̐ݒ�j
	int			dialog_fullscreen_resolution_no;		// �t���X�N���[���Ɏg���f�B�X�v���C���[�h�ԍ��i�_�C�A���O�̐ݒ�j

	int			change_display_mode_proc_cnt;							// �𑜓x��ύX���������t���O
	int			change_display_mode_proc_old_screen_size_mode;			// �𑜓x��ύX����O�̃X�N���[���T�C�Y���[�h
	int			change_display_mode_proc_old_screen_size_mode_window;	// �𑜓x��ύX����O�̃X�N���[���T�C�Y���[�h�i�E�B���h�E���j
	C_size		change_display_mode_proc_old_screen_size_scale;			// �𑜓x��ύX����O�̃X�N���[���T�C�Y�X�P�[��
	C_size		change_display_mode_proc_old_screen_size_free;			// �𑜓x��ύX����O�̃X�N���[���T�C�Y�i�t���[�j
	C_point		change_display_mode_proc_old_window_pos;				// �𑜓x��ύX����O�̃E�B���h�E�ʒu
	bool		change_display_mode_proc_old_change_resolution;			// �𑜓x��ύX����O�̉𑜓x�ύX�t���O
	int			change_display_mode_proc_old_display_no;				// �𑜓x��ύX����O�̃f�B�X�v���C�ԍ�
	int			change_display_mode_proc_old_resolution_no;				// �𑜓x��ύX����O�̉𑜓x�ԍ�

	bool		wait_display_vsync_def;			// ����������҂i�f�t�H���g�l�j
	bool		wait_display_vsync_total;		// ����������҂i���v�j
	int			wipe_buffer_cnt;				// ���C�v�o�b�t�@�̌�

	TSTR		start_scene_name;				// �X�^�[�g�V�[����
	int			start_z_no;						// �X�^�[�g�y�ԍ�

	bool		active_flag;					// �E�B���h�E���A�N�e�B�u�ł���
	bool		focused_flag;					// �E�B���h�E�Ƀt�H�[�J�X������
	bool		focused_flag_on_edit_box;		// �G�f�B�b�g�{�b�N�X�Ƀt�H�[�J�X������
	bool		editbox_hide_flag_total;		// �G�f�B�b�g�{�b�N�X���B���t���O

	bool		ex_call_flag;					// �d�w�R�[����
	bool		ctrl_skip_flag;					// Ctrl ��������s��
	bool		cs_skip_flag;					// Ctrl+Shift ��������s��
	bool		alt_menu_wait_flag;				// Alt ���j���[���J����
	bool		skip_click_1_frame;				// �P�t���[�������N���b�N���֎~����
	bool		cant_auto_skip_before_click;	// �N���b�N����܂Ŏ�����������s��Ȃ�

	int			frame_rate_100msec_total;		// �t���[�����[�g���v
	int			frame_rate_100msec_index;		// �t���[�����[�g�C���f�b�N�X
	int			frame_rate_100msec[10];			// �t���[�����[�g
	bool		read_skip_enable_flag;			// ���Ǒ����肪�\���ǂ���
	bool		is_auto_mode_count_start;		// �I�[�g���[�h�J�E���g���J�n����
	int			auto_mode_count_start_time;		// �I�[�g���[�h�J�E���g�J�n����
	bool		auto_mode_cursor_off;			// �I�[�g���[�h�̂��߃J�[�\��������
	int			mouse_cursor_hide_time;			// �I�[�g���[�h�J�E���g�J�n����
	bool		is_koe_only_count_start;		// ���̂݃��[�h�̐����I����Ă���̑҂����ԃJ�E���g���J�n����
	int			koe_only_count_start_time;		// ���̂݃��[�h�̐����I����Ă���̑҂����ԃJ�E���g�J�n����
	int			all_total_volume;				// �S�̂̍��v�{�����[��
	int			bgm_buf_total_volume;			// �a�f�l�̍��v�{�����[��
	int			koe_buf_total_volume;			// ���̍��v�{�����[���i���������͂d�w���A�g���Ă�����̃{�����[���ɂȂ�j
	int			pcm_buf_total_volume;			// ���ʉ��̍��v�{�����[��
	int			se_buf_total_volume;			// �V�X�e�����̍��v�{�����[��
	int			mov_buf_total_volume;			// ���[�r�[�̍��v�{�����[��
	bool		play_silent_sound;				// �o�b�N�O���E���h�Ŗ������Đ�����
	bool		bgmfade_flag;					// �a�f�l�t�F�[�h��
	int			bgmfade_cur_time;				// �a�f�l�t�F�[�h�J�n�̌��ݎ���
	int			bgmfade_start_value;			// �a�f�l�t�F�[�h�J�n�̊J�n�l
	int			bgmfade_total_volume;			// �a�f�l�t�F�[�h�̍ŏI�v�Z����
	bool		bgmfade2_flag;					// �a�f�l�t�F�[�h�Q��
	bool		bgmfade2_need_flag;				// �a�f�l�t�F�[�h�Q���K�v
	int			bgmfade2_cur_time;				// �a�f�l�t�F�[�h�Q�J�n�̌��ݎ���
	int			bgmfade2_start_value;			// �a�f�l�t�F�[�h�Q�J�n�̊J�n�l
	int			bgmfade2_total_volume;			// �a�f�l�t�F�[�h�Q�̍ŏI�v�Z����
	double		mouse_move_left_amari;			// �}�E�X�J�[�\�����L�[�{�[�h�ő���
	double		mouse_move_right_amari;			// �}�E�X�J�[�\�����L�[�{�[�h�ő���
	double		mouse_move_up_amari;			// �}�E�X�J�[�\�����L�[�{�[�h�ő���
	double		mouse_move_down_amari;			// �}�E�X�J�[�\�����L�[�{�[�h�ő���
	TSTR		total_font_name;				// �t�H���g��
	bool		total_font_futoku;				// �t�H���g����
	int			total_font_shadow;				// �t�H���g�e

	bool		disp_flag;						// �`����s���t���O
	bool		present_flag;					// ��ʍX�V���s���t���O
	int			disp_because_msg_wait_cnt;		// ���b�Z�[�W�E�F�C�g�����R�ŕ`����s��
	int			disp_because_msg_wait_cnt_max;	// ���b�Z�[�W�E�F�C�g�����R�ŕ`����s��
	bool		system_wipe_flag;				// �V�X�e�����C�v���t���O�i���[�h���Ȃǂ̃��C�v�j
	bool		skip_because_skip_trigger;		// �X�L�b�v�g���K�[�ɂ��X�L�b�v

	int				capture_type;				// �L���v�`���[�^�C�v
	S_tnm_sorter	capture_sorter;				// �L���v�`���[�\�[�^�[
	int				capture_prior;				// ���݂̃L���v�`���[�̗D��x
	int				capture_time;				// �L���v�`���[��������
	int				capture_x;					// �L���v�`���[��̂w���W
	int				capture_y;					// �L���v�`���[��̂x���W
	int				capture_w;					// �L���v�`���[�T�C�Y
	int				capture_h;					// �L���v�`���[�T�C�Y
	TSTR			capture_file_name;			// �L���v�`���[�ۑ��t�@�C����

	bool		game_timer_move_flag;			// �Q�[���^�C�}�[�𓮂����t���O
	bool		do_frame_action_flag;			// �t���[���A�N�V�����������I�Ɏ��s����t���O
	bool		do_load_after_call_flag;		// ���[�h�A�t�^�[�R�[�������s����t���O
	bool		sel_start_call_flag;			// �I�����J�n�R�[�������s����t���O
	bool		msg_wait_skip_by_click;			// �N���b�N�Ń��b�Z�[�W�E�F�C�g���X�L�b�v����
	bool		is_breaking;					// �u���[�N��
	bool		break_step_flag;				// �P�X�e�b�v�i��
	bool		game_end_flag;					// �Q�[���I���F�Q�[���I���t���O
	bool		game_end_no_warning;			// �Q�[���I���F�x�����o�����ɃQ�[�����I������
	bool		game_end_save_done;				// �Q�[���I���F�G���h�Z�[�u���s����
	bool		reinit_msgbk_except_flag;		// �������F���b�Z�[�W�o�b�N���c���t���O
	S_tid		msg_back_load_tid;				// ���b�Z�[�W�o�b�N����̃��[�hID
	TSTR		last_sel_msg;					// �Ō�ɑI���������b�Z�[�W

	int			max_vertex_buffer_size;			// ���_�o�b�t�@�T�C�Y
	int			index_buffer_size;				// �C���f�b�N�X�o�b�t�@�T�C�Y
	int			video_memory_rest_size;			// �r�f�I�������c��T�C�Y
	int			video_memory_use_size;			// �r�f�I�������g�p�T�C�Y
	int			video_memory_use_max_size;		// �r�f�I�������g�p�T�C�Y�i�ő�l�j

	TSTR		return_scene_name;
	int			return_scene_z_no;

	int			__bgm_stop_cntr;
};
