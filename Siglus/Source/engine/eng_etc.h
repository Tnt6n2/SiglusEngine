#pragma		once

// ****************************************************************
// �G���W���F�G�g�Z�g��
// ================================================================
bool	tnm_excall_is_excall();							// �d�w�R�[����
bool	tnm_excall_is_msg_back();						// ���b�Z�[�W�o�b�N��
bool	tnm_excall_is_hide_mwnd();						// ���b�Z�[�W�E�B���h�E������

bool	tnm_is_skipping();								// �����蒆���𔻒�
bool	tnm_is_skipping_without_skip_trigger();			// �����蒆���𔻒�i�X�L�b�v�g���K�[�������j
bool	tnm_is_skipping_msg();							// ���b�Z�[�W�����蒆���𔻒�
bool	tnm_is_skip_unread_msg();						// ���ǂ𑁑��肷�邩�𔻒�
bool	tnm_is_auto_mode();								// �I�[�g���[�h�𔻒�
int		tnm_get_auto_mode_moji_wait();					// �I�[�g���[�h�̕����҂����Ԃ��擾
int		tnm_get_auto_mode_min_wait();					// �I�[�g���[�h�̍ŏ��҂����Ԃ��擾
bool	tnm_is_mwnd_anime_use();						// �E�B���h�E�A�j���𔻒�
bool	tnm_is_hide_mwnd();								// ���b�Z�[�W�E�B���h�E���B�����𔻒�
bool	tnm_is_cursor_disp();							// �J�[�\����\�����邩�𔻒�
bool	tnm_is_koe_play(int chara_no);					// �����Đ����邩�𔻒�
bool	tnm_is_stop_koe();								// �����~�߂邩�𔻒�
bool	tnm_get_chrkoe_onoff(int chara_no);				// �L�����N�^�[�����I���I�t���擾
int		tnm_get_chrkoe_volume(int chara_no);			// �L�����N�^�[�����{�����[�����擾
void	tnm_is_chrkoe_name_check(CTSTR& name_str);		// �L�����N�^�[�����̖��O�̕��������J���邩�𔻒�i���J�t���O���n�m�ɂ��܂��j
void	tnm_update_read_skip();							// ���Ǒ�������X�V
void	tnm_update_start_skip();						// ������������J�n
int		tnm_get_total_msg_speed();						// ���b�Z�[�W�X�s�[�h���擾

// �Q�[�������擾
TSTR tnm_get_game_name();

// �V�[���ԍ�����V�[�������擾
TSTR tnm_get_scn_name(int scn_no);

// �V�[���ԍ�����t���p�X�����擾
TSTR tnm_get_scn_full_path_name(int scn_no);

// �V�[���^�C�g�����擾
TSTR tnm_get_scene_title();

// �E�B���h�E�ɕ\������t���^�C�g�����擾
TSTR tnm_get_full_title();

// �E�B���h�E�����擾
TSTR tnm_get_window_name();

