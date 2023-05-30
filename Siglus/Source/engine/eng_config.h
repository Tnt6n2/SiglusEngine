#pragma		once

// ****************************************************************
// �V�X�e���R���t�B�O
// ================================================================
//		system.ini �Œ�`����A�V�X�e���n�̃R���t�B�O
// ================================================================

// ****************************************************************
// �V�X�e���R���t�B�O�\����
// ================================================================
class C_tnm_system_config
{
public:
	int		d3d_adapter_no;
	int		d3d_present_interval;
	int		d3d_wipe_buffer_cnt;
	bool	d3d_hardware_vertex_flag;
	bool	d3d_use_mipmap_flag;
	bool	ds_play_silent_sound_flag;
	int		movie_type;
};

// ****************************************************************
// ���[�U�R���t�B�O
// ================================================================
//		���ݒ�ȂǁA���[�U������ł���R���t�B�O
// ================================================================
void	tnm_format_config();							// �S�ăt�H�[�}�b�g
void	tnm_init_config_screen_mode();					// ��ʃ��[�h
void	tnm_init_config_fullscreen();					// �t���X�N���[���̏ڍאݒ�
void	tnm_init_config_volume();						// ����
void	tnm_init_config_bgmfade();						// �a�f�l�t�F�[�h
void	tnm_init_config_filter();						// �t�B���^�[�F
void	tnm_init_config_font();							// �t�H���g
void	tnm_init_config_message_speed();				// �������x
void	tnm_init_config_auto_mode();					// �I�[�g���[�h
void	tnm_init_config_mouse_cursor_hide();			// �����Ń}�E�X�J�[�\�����B��
void	tnm_init_config_jitan();						// ���Z�Đ�
void	tnm_init_config_koe_mode();						// �������[�h
void	tnm_init_config_chrkoe();						// �L�����N�^�[����
void	tnm_init_config_message_chrcolor();				// ���͂̐F����
void	tnm_init_config_object_disp();					// �I�u�W�F�N�g�\��
void	tnm_init_config_global_extra_switch();			// �O���[�o���ėp�X�C�b�`
void	tnm_init_config_global_extra_mode();			// �O���[�o���ėp���[�h
void	tnm_init_config_system();						// �V�X�e���ݒ�
void	tnm_init_config_system_saveload_alert();		// �V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
void	tnm_init_config_system_saveload_dblclick();		// �V�X�e���ݒ�i�Z�[�u���[�h�̃_�u���N���b�N�j

void	tnm_init_system_config_play_silent_sound();		// �V�X�e���ݒ�i�������Đ�����j
void	tnm_init_system_config_movie_play_type();		// �V�X�e���ݒ�i���[�r�[�̍Đ����@�j
