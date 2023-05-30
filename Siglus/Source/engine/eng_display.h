#pragma		once

// ****************************************************************
// �f�B�X�v���C�֌W�̏���
// ================================================================
void	tnm_change_screen_size_mode_by_force(bool first);							// �X�N���[���T�C�Y���[�h��ύX�i�����j
void	tnm_change_screen_size_mode_window(bool first);								// �X�N���[���T�C�Y���[�h��ύX�i�E�B���h�E�j
void	tnm_change_screen_size_mode_window(C_size screen_size_scale, bool first);	// �X�N���[���T�C�Y���[�h��ύX�i�E�B���h�E�E�T�C�Y�w��j
void	tnm_change_screen_size_mode_free(C_point window_pos, C_size screen_size_free, bool first);		// �X�N���[���T�C�Y���[�h��ύX�i�t���[�E�T�C�Y�w��j
void	tnm_change_screen_size_mode_fullscreen(bool first, bool warning);			// �X�N���[���T�C�Y���[�h��ύX�i�t���X�N���[���j
void	tnm_switch_screen_size_mode(bool first);									// �X�N���[���T�C�Y���[�h��ύX�i�؂�ւ��j
void	tnm_update_screen_size_mode();												// �X�N���[���T�C�Y���[�h���X�V�i�N�����j
void	tnm_reset_screen_size_mode();												// �X�N���[���T�C�Y���[�h�����ɖ߂��i�m�F�_�C�A���O�ł�������I�񂾁j

C_size	tnm_get_screen_size_from_size_mode(int screen_size_mode);									// �X�N���[���T�C�Y���[�h����X�N���[���T�C�Y���擾
C_size	tnm_get_window_size_from_size_mode(int screen_size_mode);									// �X�N���[���T�C�Y���[�h����E�B���h�E�T�C�Y���擾
bool	tnm_get_window_size_from_size_scale(C_size screen_size_scale);								// �X�N���[���T�C�Y���g�p�\���𔻒�
bool	tnm_rep_screen_size(int screen_size_mode, C_size* screen_size_scale, C_size* screen_size_free);		// �X�N���[���T�C�Y�␳
bool	tnm_rep_screen_size_window(C_size* screen_size_scale);										// �X�N���[���T�C�Y�␳
bool	tnm_rep_screen_size_free(C_size* screen_size_free);											// �X�N���[���T�C�Y�␳
int		tnm_format_screen_size_to_screen_mode(C_size screen_size_scale);							// �X�N���[���T�C�Y�䗦����X�N���[�����[�h�ɕϊ��i�擾�H�j
void	tnm_format_screen_mode_to_screen_size_scale(int size_mode, C_size* screen_size_scale);		// �X�N���[�����[�h����X�N���[���T�C�Y�䗦�ɕϊ�

void	tnm_set_wait_display_vsync(bool wait_display_vsync_flag);					// ����������҂ݒ��ύX����

