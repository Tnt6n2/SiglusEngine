#pragma		once

#include	"data/tnm_timer.h"

// ****************************************************************
// �G���W���F���b�Z�[�W�o�b�N�F�C���^�[�t�F�[�X
// ================================================================

// ���b�Z�[�W�o�b�N���N���A
void tnm_msg_back_clear();

// ���b�Z�[�W�o�b�N�ɕ��������邩�𔻒�
bool tnm_msg_back_is_exist_msg();

// ���b�Z�[�W�o�b�N���g�p�\���𔻒�
bool tnm_msg_back_is_enable();

// ���b�Z�[�W�o�b�N���J��
void tnm_msg_back_open();

// ���b�Z�[�W�o�b�N�����
void tnm_msg_back_close();

// ���b�Z�[�W�o�b�N���J���Ă��邩�𔻒�
bool tnm_msg_back_is_open();

// ���b�Z�[�W�o�b�N��i�߂�
void tnm_msg_back_next();

// ���b�Z�[�W�o�b�N�ɃZ�[�uID��ݒ肷��
void tnm_msg_back_set_save_id(S_tid save_id);

// ���b�Z�[�W�o�b�N�ɃZ�[�uID�`�F�b�N�t���O��ݒ肷��
void tnm_msg_back_set_save_id_check_flag(bool save_id_check_flag);

// ���b�Z�[�W�o�b�N�̃Z�[�uID�`�F�b�N�t���O���擾����
bool tnm_msg_back_get_save_id_check_flag();

// ���b�Z�[�W�o�b�N�ɐ���ǉ�����
bool tnm_msg_back_add_koe(int koe_no, int chr_no, int scn_no, int line_no);

// ���b�Z�[�W�o�b�N�ɖ��O��ǉ�����
bool tnm_msg_back_add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no);

// ���b�Z�[�W�o�b�N�Ƀ��b�Z�[�W��ǉ�����
bool tnm_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no);

// ���b�Z�[�W�o�b�N�ɉ摜��ǉ�����
bool tnm_msg_back_add_pct(CTSTR& file_name, int x, int y);

// ���b�Z�[�W�o�b�N�̃V�[���ԍ����擾
int tnm_msg_back_get_scn_no();

// ���b�Z�[�W�o�b�N�̍s�ԍ����擾
int tnm_msg_back_get_line_no();

// ���b�Z�[�W�o�b�N�̐��ԍ����擾
int tnm_msg_back_get_koe_no();

