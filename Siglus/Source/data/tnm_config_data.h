#pragma		once

#include	"tnm_def.h"

// ****************************************************************
// ���[�U�[�R���t�B�O
// ================================================================
//		�_�C�A���O�ȂǂŐݒ�ł��鍀�ڂł��B
//		�O���[�o���ɃZ�[�u����܂��B
// ================================================================

// ****************************************************************
// ���[�U�[�R���t�B�O�F�L�����N�^�[����
// ================================================================
struct S_tnm_config_chrkoe
{
	bool	onoff;
	int		volume;
};

// ****************************************************************
// ���[�U�[�R���t�B�O
// ================================================================
class C_tnm_config
{
public:

	// �X�N���[���T�C�Y
	int		screen_size_mode;			// �X�N���[���T�C�Y�̃��[�h
	int		screen_size_mode_window;	// �X�N���[���T�C�Y�̃��[�h�i�E�B���h�E���j
	C_size	screen_size_scale;			// �X�N���[���T�C�Y�̃X�P�[��
	C_size	screen_size_free;			// �X�N���[���T�C�Y�i�t���[�j

	// �t���X�N���[���ڍ�
	bool	fullscreen_change_resolution;	// �t���X�N���[�����ɉ𑜓x��ύX����
	int		fullscreen_display_cnt;			// �t���X�N���[���Ɏg���f�B�X�v���C�ԍ��̌�
	int		fullscreen_display_no;			// �t���X�N���[���Ɏg���f�B�X�v���C�ԍ�
	int		fullscreen_resolution_cnt;		// �t���X�N���[���Ɏg���f�B�X�v���C���[�h�ԍ��̌�
	int		fullscreen_resolution_no;		// �t���X�N���[���Ɏg���f�B�X�v���C���[�h�ԍ�
	C_size	fullscreen_resolution;			// �t���X�N���[���Ɏg���f�B�X�v���C�T�C�Y

	bool	total_fullscreen_change_resolution;		// �t���X�N���[�����ɉ𑜓x��ύX����i���ۂ̒l�j
	int		total_fullscreen_display_cnt;			// �t���X�N���[���Ɏg���f�B�X�v���C�̌�
	int		total_fullscreen_display_no;			// �t���X�N���[���Ɏg���f�B�X�v���C�ԍ�
	int		total_fullscreen_resolution_cnt;		// �t���X�N���[���Ɏg���f�B�X�v���C���[�h�̌�
	int		total_fullscreen_resolution_no;			// �t���X�N���[���Ɏg���f�B�X�v���C���[�h�ԍ�
	C_size	total_fullscreen_resolution;			// �t���X�N���[���Ɏg���f�B�X�v���C�T�C�Y

	int		fullscreen_mode;				// �t���X�N���[���̕\�����[�h
	C_size	fullscreen_scale;				// �t���X�N���[���̃X�P�[��
	bool	fullscreen_scale_sync_switch;	// �t���X�N���[���̃X�P�[���̓����X�C�b�`
	C_size	fullscreen_move;				// �t���X�N���[���̈ړ�

	// ����
	int		all_sound_user_volume;
	int		sound_user_volume[TNM_VOLUME_TYPE_MAX];		// �{�����[��
	bool	play_all_sound_check;
	bool	play_sound_check[TNM_VOLUME_TYPE_MAX];		// �Đ��`�F�b�N

	// �a�f�l�t�F�[�h
	int		bgmfade_volume;			// �a�f�l�t�F�[�h�{�����[��
	bool	bgmfade_use_check;		// �a�f�l�t�F�[�h�`�F�b�N

	// �t�B���^�[�F
	C_argb	filter_color;			// �t�B���^�[�F

	// �t�H���g
	bool	font_proportional;		// �v���|�[�V���i���t�H���g��\��
	TSTR	font_name;				// �t�H���g��
	bool	font_futoku;			// �t�H���g����
	int		font_shadow;			// �t�H���g�e

	// �������x
	int		message_speed;			// �������x
	bool	message_speed_nowait;	// �������x�i�m�[�E�F�C�g�j

	// �I�[�g���[�h
	bool	auto_mode_onoff;		// �I�[�g���[�h
	int		auto_mode_moji_wait;	// �I�[�g���[�h��������
	int		auto_mode_min_wait;		// �I�[�g���[�h�ŏ�����

	// �����Ń}�E�X�J�[�\��������
	bool	mouse_cursor_hide_onoff;	// �����Ń}�E�X�J�[�\��������
	int		mouse_cursor_hide_time;		// �����Ń}�E�X�J�[�\������������

	// ���Z�Đ�
	bool	jitan_normal_onoff;		// ���Z�Đ��i���͂𕁒ʂɓǂݐi�߂Ă��鎞�j
	bool	jitan_auto_mode_onoff;	// ���Z�Đ��i�I�[�g���[�h���j
	bool	jitan_msgbk_onoff;		// ���Z�Đ��i���͂̊����߂����j
	int		jitan_speed;			// ���Z�Đ����x

	// �������[�h
	int		koe_mode;				// �������[�h

	// �L�����N�^�[����
	ARRAY<S_tnm_config_chrkoe>	chrkoe;			// �L�����N�^�[����

	// ���͂̐F����
	bool	message_chrcolor_flag;	// ���͂̐F����

	// �I�u�W�F�N�g�̕\���t���O
	ARRAY<S_bool>	object_disp_flag;			// �I�u�W�F�N�g�̕\���t���O

	// �O���[�o���ėp�X�C�b�`
	ARRAY<S_bool>	global_extra_switch_flag;	// �O���[�o���ėp�X�C�b�`

	// �O���[�o���ėp���[�h
	ARRAY<int>		global_extra_mode_flag;		// �O���[�o���ėp���[�h

	// �V�X�e���ݒ�
	struct S_system
	{
		bool	sleep_flag;					// �{�v���O�����̓����x�����āA���̃v���O�������X���[�Y�ɓ��삷��悤�ɂ���B
		bool	no_wipe_anime_flag;			// ��ʈÓ]���ʂ̃A�j���𖳌��ɂ���B
		bool	skip_wipe_anime_flag;		// ��ʈÓ]���ʂ��}�E�X�N���b�N�Ŕ�΂��B
		bool	no_mwnd_anime_flag;			// ���b�Z�[�W�E�B���h�E�̊J���̃A�j���𖳌��ɂ���B
		bool	wheel_next_message_flag;	// �}�E�X�̃z�C�[���{�^���̉��񂵂ŕ��͂�ǂݐi�߂�B
		bool	koe_dont_stop_flag;			// ���̍Đ����Ɏ��̕��͂ɐi��ł��Đ��𑱂���B
		bool	skip_unread_message_flag;	// ���ǂ̕��͂�������ł���悤�ɂ���B
		bool	saveload_alert_flag;		// �Z�[�u���[�h�̌x��
		bool	saveload_dblclick_flag;		// �Z�[�u���[�h�̃_�u���N���b�N
	}	system;

	// ��
	TSTR	ss_path;				// �V�[���t�H���_�p�X
	TSTR	editor_path;			// �G�f�B�^�p�X
	TSTR	koe_path;				// ���p�X
	TSTR	koe_tool_path;			// ���c�[���p�X
};

