#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_msg_back.h"
#include	"engine/ifc_msg_back.h"

// ****************************************************************
// �G���W���F���b�Z�[�W�o�b�N�F�C���^�[�t�F�[�X
// ================================================================

// ���b�Z�[�W�o�b�N���N���A
void tnm_msg_back_clear()
{
	Gp_msg_back->clear_msg();
}

// ���b�Z�[�W�o�b�N�ɕ��������邩�𔻒�
bool tnm_msg_back_is_exist_msg()
{
	return Gp_msg_back->is_exist_msg();
}

// ���b�Z�[�W�o�b�N���g�p�\�����擾
bool tnm_msg_back_is_enable()
{
	return Gp_msg_back->is_exist_msg() && !Gp_local->pod.msg_back_disable;
}

// ���b�Z�[�W�o�b�N���J��
void tnm_msg_back_open()
{
	Gp_msg_back->open();
}

// ���b�Z�[�W�o�b�N�����
void tnm_msg_back_close()
{
	Gp_msg_back->close();
}

// ���b�Z�[�W�o�b�N���J���Ă��邩�𔻒肷��
bool tnm_msg_back_is_open()
{
	return Gp_msg_back->is_open();
}

// ���b�Z�[�W�o�b�N��i�߂�
void tnm_msg_back_next()
{
	Gp_msg_back->next_msg();
}

// ���b�Z�[�W�o�b�N�ɃZ�[�u ID ��ݒ肷��
void tnm_msg_back_set_save_id(S_tid save_id)
{
	Gp_msg_back->set_save_id(save_id);
}

// ���b�Z�[�W�o�b�N�ɃZ�[�uID�`�F�b�N�t���O��ݒ肷��
void tnm_msg_back_set_save_id_check_flag(bool save_id_check_flag)
{
	Gp_msg_back->set_save_id_check_flag(save_id_check_flag);
}

// ���b�Z�[�W�o�b�N�̃C���^�[�o���`�F�b�N�t���O���擾����
bool tnm_msg_back_get_save_id_check_flag()
{
	return Gp_msg_back->get_save_id_check_flag();
}

// ���b�Z�[�W�o�b�N�ɐ���ǉ�����
bool tnm_msg_back_add_koe(int koe_no, int chara_no, int scn_no, int line_no)
{
	return Gp_msg_back->add_koe(koe_no, chara_no, scn_no, line_no);
}

// ���b�Z�[�W�o�b�N�ɖ��O��ǉ�����
bool tnm_msg_back_add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no)
{
	// ���O��ǉ�����
	return Gp_msg_back->add_name(original_name, disp_name, scn_no, line_no);
}

// ���b�Z�[�W�o�b�N�Ƀ��b�Z�[�W��ǉ�����
bool tnm_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	return Gp_msg_back->add_msg(msg, debug_msg, scn_no, line_no);
}

// ���b�Z�[�W�o�b�N�ɉ摜��ǉ�����
bool tnm_msg_back_add_pct(CTSTR& file_name, int x, int y)
{
	return Gp_msg_back->add_pct(file_name, C_point(x, y));
}

// ���b�Z�[�W�o�b�N�̃V�[���ԍ����擾
int tnm_msg_back_get_scn_no()
{
	return Gp_msg_back->get_scn_no();
}

// ���b�Z�[�W�o�b�N�̍s�ԍ����擾
int tnm_msg_back_get_line_no()
{
	return Gp_msg_back->get_line_no();
}

// ���b�Z�[�W�o�b�N�̐��ԍ����擾
int tnm_msg_back_get_koe_no()
{
	return Gp_msg_back->get_koe_no();
}

