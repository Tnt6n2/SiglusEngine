#pragma		once

#include	"data/tnm_sorter.h"
#include	"data/tnm_timer.h"

struct S_element;

// ****************************************************************
// �V�X�e���R�}���h����
// ================================================================
//		�E�N���b�N���j���[�A�E�B���h�E�{�^���Ȃǂ���s������
// ================================================================
void	tnm_syscom_init_syscom_flag();							// �V�X�e���R�}���h�t���O��������

bool	tnm_syscom_open();										// �V�X�e���R�}���h���J��

bool	tnm_syscom_restart_from_start();						// �ŏ�����͂��߂�
bool	tnm_syscom_restart_from_scene(int scn_no, int z_no);	// �V�[������͂��߂�
bool	tnm_syscom_change_break();								// �u���[�N�J�n�^����
bool	tnm_syscom_do_one_step();								// �X�e�b�v���s

void	tnm_syscom_read_skip_set_onoff_flag(bool onoff);		// ���Ǒ������ ON/OFF ��ݒ�
bool	tnm_syscom_read_skip_get_onoff_flag();					// ���Ǒ������ ON/OFF ���擾
void	tnm_syscom_read_skip_set_enable_flag(bool enable);		// ���Ǒ�����̋��^�s����ݒ�
bool	tnm_syscom_read_skip_get_enable_flag();					// ���Ǒ�����̋��^�s�����擾
void	tnm_syscom_read_skip_set_exist_flag(bool exist);		// ���Ǒ�����̕\���^��\����ݒ�
bool	tnm_syscom_read_skip_get_exist_flag();					// ���Ǒ�����̕\���^��\�����擾
bool	tnm_syscom_read_skip_is_enable();						// ���Ǒ����肪�g�p�\���𔻒�

void	tnm_syscom_auto_skip_set_onoff_flag(bool onoff);		// ����������� ON/OFF ��ݒ�
bool	tnm_syscom_auto_skip_get_onoff_flag();					// ����������� ON/OFF ���擾
void	tnm_syscom_auto_skip_set_enable_flag(bool enable);		// ����������̋��^�s����ݒ�
bool	tnm_syscom_auto_skip_get_enable_flag();					// ����������̋��^�s�����擾
void	tnm_syscom_auto_skip_set_exist_flag(bool exist);		// ����������̕\���^��\����ݒ�
bool	tnm_syscom_auto_skip_get_exist_flag();					// ����������̕\���^��\�����擾
bool	tnm_syscom_auto_skip_is_enable();						// ���������肪�g�p�\���𔻒�

void	tnm_syscom_auto_mode_set_onoff_flag(bool onoff);		// �I�[�g���[�h�� ON/OFF ��ݒ�
bool	tnm_syscom_auto_mode_get_onoff_flag();					// �I�[�g���[�h�� ON/OFF ���擾
void	tnm_syscom_auto_mode_set_enable_flag(bool enable);		// �I�[�g���[�h�̋��^�s����ݒ�
bool	tnm_syscom_auto_mode_get_enable_flag();					// �I�[�g���[�h�̋��^�s�����擾
void	tnm_syscom_auto_mode_set_exist_flag(bool exist);		// �I�[�g���[�h�̕\���^��\����ݒ�
bool	tnm_syscom_auto_mode_get_exist_flag();					// �I�[�g���[�h�̕\���^��\�����擾
bool	tnm_syscom_auto_mode_is_enable();						// �I�[�g���[�h���g�p�\���𔻒�

void	tnm_syscom_hide_mwnd_set_onoff_flag(bool onoff);		// ���b�Z�[�W�E�B���h�E������ ON/OFF ��ݒ�
bool	tnm_syscom_hide_mwnd_get_onoff_flag();					// ���b�Z�[�W�E�B���h�E������ ON/OFF ���擾
void	tnm_syscom_hide_mwnd_set_enable_flag(bool enable);		// ���b�Z�[�W�E�B���h�E�����̋��^�s����ݒ�
bool	tnm_syscom_hide_mwnd_get_enable_flag();					// ���b�Z�[�W�E�B���h�E�����̋��^�s�����擾
void	tnm_syscom_hide_mwnd_set_exist_flag(bool exist);		// ���b�Z�[�W�E�B���h�E�����̕\���^��\����ݒ�
bool	tnm_syscom_hide_mwnd_get_exist_flag();					// ���b�Z�[�W�E�B���h�E�����̕\���^��\�����擾
bool	tnm_syscom_hide_mwnd_is_enable();						// ���b�Z�[�W�E�B���h�E����邪�g�p�\���𔻒�

void	tnm_syscom_open_msg_back();								// ���b�Z�[�W�o�b�N���J��
void	tnm_syscom_close_msg_back();							// ���b�Z�[�W�o�b�N�����
void	tnm_syscom_msg_back_set_enable_flag(bool enable);		// ���b�Z�[�W�o�b�N�̋��^�s����ݒ�
bool	tnm_syscom_msg_back_get_enable_flag();					// ���b�Z�[�W�o�b�N�̋��^�s�����擾
void	tnm_syscom_msg_back_set_exist_flag(bool exist);			// ���b�Z�[�W�o�b�N�̕\���^��\����ݒ�
bool	tnm_syscom_msg_back_get_exist_flag();					// ���b�Z�[�W�o�b�N�̕\���^��\�����擾
bool	tnm_syscom_msg_back_is_enable();						// ���b�Z�[�W�o�b�N���g�p�\���𔻒�

bool	tnm_syscom_open_save(bool dialog_flag, bool capture_flag, bool enable_check);	// �Z�[�u���J��
void	tnm_syscom_open_save_set_enable_flag(bool enable);								// �Z�[�u�̋��^�s����ݒ�
bool	tnm_syscom_open_save_get_enable_flag();											// �Z�[�u�̋��^�s�����擾
void	tnm_syscom_open_save_set_exist_flag(bool exist);								// �Z�[�u�̕\���^��\����ݒ�
bool	tnm_syscom_open_save_get_exist_flag();											// �Z�[�u�̕\���^��\�����擾
bool	tnm_syscom_open_save_is_enable();												// �Z�[�u���g�p�\���𔻒�

bool	tnm_syscom_open_load(bool dialog_flag, bool enable_check);						// ���[�h���J��
void	tnm_syscom_open_load_set_enable_flag(bool enable);								// ���[�h�̋��^�s����ݒ�
bool	tnm_syscom_open_load_get_enable_flag();											// ���[�h�̋��^�s�����擾
void	tnm_syscom_open_load_set_exist_flag(bool exist);								// ���[�h�̕\���^��\����ݒ�
bool	tnm_syscom_open_load_get_exist_flag();											// ���[�h�̕\���^��\�����擾
bool	tnm_syscom_open_load_is_enable();												// ���[�h���g�p�\���𔻒�

bool	tnm_syscom_save(int save_no, bool warning, bool se_play);						// �Z�[�u
bool	tnm_syscom_save_is_enable(int save_no);											// �Z�[�u���g�p�\���𔻒�

bool	tnm_syscom_quick_save(int quick_save_no, bool warning, bool se_play);			// �N�C�b�N�Z�[�u
bool	tnm_syscom_quick_save_is_enable(int quick_save_no);								// �N�C�b�N�Z�[�u���g�p�\���𔻒�

bool	tnm_syscom_end_save(bool warning, bool se_play);								// �G���h�Z�[�u
bool	tnm_syscom_end_save_period_2(bool capture_flag);								// �G���h�Z�[�u�i�Q�i�K�ځj
bool	tnm_syscom_end_save_is_enable();												// �G���h�Z�[�u���g�p�\���𔻒�

bool	tnm_syscom_load(int save_no, bool warning, bool se_play, bool fade_out);		// ���[�h
bool	tnm_syscom_load_easy(int save_no);												// ���[�h�i�f�o�b�O�p�F�ȈՃo�[�W�����j
bool	tnm_syscom_load_is_enable(int save_no);											// ���[�h���g�p�\���𔻒�

bool	tnm_syscom_quick_load(int save_no, bool warning, bool se_play, bool fade_out);	// �N�C�b�N���[�h
bool	tnm_syscom_quick_load_is_enable(int save_no);									// �N�C�b�N���[�h���g�p�\���𔻒�

bool	tnm_syscom_end_load(bool warning, bool se_play, bool fade_out);					// �G���h���[�h
bool	tnm_syscom_end_load_is_enable();												// �G���h���[�h���g�p�\���𔻒�

bool	tnm_syscom_inner_load(int inner_save_no, bool warning, bool se_play, bool fade_out);	// �C���i�[���[�h
bool	tnm_syscom_inner_load_is_enable();														// �C���i�[���[�h���g�p�\���𔻒�

bool	tnm_syscom_msgbk_load(S_tid save_id, bool warning, bool se_play, bool fade_out);		// ���b�Z�[�W�o�b�N���[�h

bool	tnm_syscom_is_exist_capture();													// �L���v�`���[�����邩�𔻒肷��
void	tnm_syscom_create_capture(int prior, bool disp);								// �L���v�`���[����������
void	tnm_syscom_free_capture(int prior);												// �L���v�`���[���������
void	tnm_syscom_create_capture_for_object(S_tnm_sorter sorter, bool disp);			// �I�u�W�F�N�g�p�̃L���v�`���[����������
void	tnm_syscom_free_capture_for_object();											// �I�u�W�F�N�g�p�̃L���v�`���[���������
void	tnm_syscom_create_capture_for_local_save(S_tnm_sorter sorter, int width, int height, bool disp);		// ���[�J���Z�[�u�p�̃L���v�`���[����������
void	tnm_syscom_create_capture_for_tweet();											// �c�C�[�g�p�̃L���v�`���[����������
void	tnm_syscom_free_capture_for_tweet();											// �c�C�[�g�p�̃L���v�`���[���������

void	tnm_syscom_create_capture_buffer(int width, int height);											// �L���v�`���[�o�b�t�@���쐬
void	tnm_syscom_destroy_capture_buffer();																// �L���v�`���[�o�b�t�@��j��
void	tnm_syscom_buffer_capture(int x, int y, CTSTR& file_name);											// �o�b�t�@�L���v�`���[���s��

bool	tnm_syscom_save_capture_buffer_to_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt);
bool	tnm_syscom_load_flag_from_capture_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt);

void	tnm_syscom_return_to_sel(bool warning, bool se_play, bool fade_out);			// �O�̑I�����ɖ߂�
void	tnm_syscom_return_to_sel_set_enable_flag(bool enable);							// �O�̑I�����ɖ߂�̋��^�s����ݒ�
bool	tnm_syscom_return_to_sel_get_enable_flag();										// �O�̑I�����ɖ߂�̋��^�s�����擾
void	tnm_syscom_return_to_sel_set_exist_flag(bool exist);							// �O�̑I�����ɖ߂�̕\���^��\����ݒ�
bool	tnm_syscom_return_to_sel_get_exist_flag();										// �O�̑I�����ɖ߂�̕\���^��\�����擾
bool	tnm_syscom_return_to_sel_is_enable();											// �O�̑I�����ɖ߂邪�g�p�\���𔻒�

void	tnm_syscom_return_to_menu(bool warning, bool se_play, bool fade_out, bool msgback_except);		// ���j���[�ɖ߂�
void	tnm_syscom_return_to_menu_set_enable_flag(bool enable);							// ���j���[�ɖ߂�̋��^�s����ݒ�
bool	tnm_syscom_return_to_menu_get_enable_flag();									// ���j���[�ɖ߂�̋��^�s�����擾
void	tnm_syscom_return_to_menu_set_exist_flag(bool exist);							// ���j���[�ɖ߂�̕\���^��\����ݒ�
bool	tnm_syscom_return_to_menu_get_exist_flag();										// ���j���[�ɖ߂�̕\���^��\�����擾
bool	tnm_syscom_return_to_menu_is_enable();											// ���j���[�ɖ߂邪�g�p�\���𔻒�

void	tnm_syscom_end_game(bool warning, bool se_play, bool fade_out);					// �Q�[�����I������
void	tnm_syscom_end_game_set_enable_flag(bool enable);								// �Q�[�����I������̋��^�s����ݒ�
bool	tnm_syscom_end_game_get_enable_flag();											// �Q�[�����I������̋��^�s�����擾
void	tnm_syscom_end_game_set_exist_flag(bool exist);									// �Q�[�����I������̕\���^��\����ݒ�
bool	tnm_syscom_end_game_get_exist_flag();											// �Q�[�����I������̕\���^��\�����擾
bool	tnm_syscom_end_game_is_enable();												// �Q�[�����I�����邪�g�p�\���𔻒�

void	tnm_syscom_koe_play();															// �����Đ�����
bool	tnm_syscom_koe_play_is_enable();												// �����Đ����邪�g�p�\���𔻒�

void	tnm_syscom_open_config();														// ���ݒ���J��
void	tnm_syscom_open_config_set_enable_flag(bool enable);							// ���ݒ���J���̋��^�s����ݒ�
bool	tnm_syscom_open_config_get_enable_flag();										// ���ݒ���J���̋��^�s�����擾
void	tnm_syscom_open_config_set_exist_flag(bool exist);								// ���ݒ���J���̕\���^��\����ݒ�
bool	tnm_syscom_open_config_get_exist_flag();										// ���ݒ���J���̕\���^��\�����擾
bool	tnm_syscom_open_config_is_enable();												// ���ݒ���J�����g�p�\���𔻒�

void	tnm_syscom_local_extra_switch_set_onoff_flag(int switch_no, bool onoff);		// ���[�J���ėp�X�C�b�`�� ON/OFF ��ݒ�
bool	tnm_syscom_local_extra_switch_get_onoff_flag(int switch_no);					// ���[�J���ėp�X�C�b�`�� ON/OFF ���擾
void	tnm_syscom_local_extra_switch_turn_onoff_flag(int switch_no);					// ���[�J���ėp�X�C�b�`�� ON/OFF �𔽓]
void	tnm_syscom_local_extra_switch_set_enable_flag(int switch_no, bool enable);		// ���[�J���ėp�X�C�b�`�̋��^�s����ݒ�
bool	tnm_syscom_local_extra_switch_get_enable_flag(int switch_no);					// ���[�J���ėp�X�C�b�`�̋��^�s�����擾
void	tnm_syscom_local_extra_switch_set_exist_flag(int switch_no, bool exist);		// ���[�J���ėp�X�C�b�`�̕\���^��\����ݒ�
bool	tnm_syscom_local_extra_switch_get_exist_flag(int switch_no);					// ���[�J���ėp�X�C�b�`�̕\���^��\�����擾
bool	tnm_syscom_local_extra_switch_is_enable(int switch_no);							// ���[�J���ėp�X�C�b�`���g�p�\���𔻒�

void	tnm_syscom_local_extra_mode_set_value(int mode_no, int value);					// ���[�J���ėp���[�h�̒l��ݒ�
int		tnm_syscom_local_extra_mode_get_value(int mode_no);								// ���[�J���ėp���[�h�̒l���擾
void	tnm_syscom_local_extra_mode_increment(int mode_no);								// ���[�J���ėp���[�h�̒l��i�߂�
void	tnm_syscom_local_extra_mode_set_enable_flag(int mode_no, bool enable);			// ���[�J���ėp���[�h�̋��^�s����ݒ�
bool	tnm_syscom_local_extra_mode_get_enable_flag(int mode_no);						// ���[�J���ėp���[�h�̋��^�s�����擾
void	tnm_syscom_local_extra_mode_set_exist_flag(int mode_no, bool exist);			// ���[�J���ėp���[�h�̕\���^��\����ݒ�
bool	tnm_syscom_local_extra_mode_get_exist_flag(int mode_no);						// ���[�J���ėp���[�h�̕\���^��\�����擾
bool	tnm_syscom_local_extra_mode_is_enable(int mode_no);								// ���[�J���ėp���[�h���g�p�\���𔻒�

void	tnm_syscom_manual_set_enable_flag(bool enable);									// �}�j���A���̋��^�s����ݒ�
bool	tnm_syscom_manual_get_enable_flag();											// �}�j���A���̋��^�s�����擾
void	tnm_syscom_manual_set_exist_flag(bool exist);									// �}�j���A���̕\���^��\����ݒ�
bool	tnm_syscom_manual_get_exist_flag();												// �}�j���A���̕\���^��\�����擾

void	tnm_syscom_version_set_enable_flag(bool enable);								// �o�[�W�������̋��^�s����ݒ�
bool	tnm_syscom_version_get_enable_flag();											// �o�[�W�������̋��^�s�����擾
void	tnm_syscom_version_set_exist_flag(bool exist);									// �o�[�W�������̕\���^��\����ݒ�
bool	tnm_syscom_version_get_exist_flag();											// �o�[�W�������̕\���^��\�����擾

void	tnm_syscom_cancel_set_enable_flag(bool enable);									// �L�����Z���̋��^�s����ݒ�
bool	tnm_syscom_cancel_get_enable_flag();											// �L�����Z���̋��^�s�����擾
void	tnm_syscom_cancel_set_exist_flag(bool exist);									// �L�����Z���̕\���^��\����ݒ�
bool	tnm_syscom_cancel_get_exist_flag();												// �L�����Z���̕\���^��\�����擾
bool	tnm_syscom_cancel_is_enable();													// �L�����Z�����g�p�\���𔻒�


void	tnm_syscom_close_window();														// �E�B���h�E�����

// ****************************************************************
// �V�X�e���R�}���h�����i�⏕�j
// ================================================================
//		�ʏ�͒��ڂ͌Ă΂�Ȃ����A�Q�d�����Ȃǂ̂��߂ɒ��ڌĂ΂�邱�Ƃ�����
// ================================================================

bool	tnm_open_save_dialog(bool capture_flag);				// �Z�[�u�_�C�A���O���J��


