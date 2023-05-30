#pragma		once

#include	"data/tnm_save_00.h"
#include	"data/tnm_timer.h"

// ****************************************************************
// ���[�J���Z�[�u
// ================================================================
struct S_tnm_local_save
{
	S_tid				save_id;
	TSTR				append_dir;
	TSTR				append_name;
	TSTR				save_scene_title;			// �V�[���^�C�g��
	TSTR				save_msg;					// ���݂̕���
	TSTR				save_full_msg;				// ���݂̕���
	C_tnm_save_stream	save_stream;				// ���[�J���f�[�^
	C_tnm_save_stream	save_stream_ex;				// ���[�J���ݒ�
};

// ****************************************************************
// �I�����Z�[�u�V�X�e��
// ================================================================
struct S_tnm_sel_save
{
	ARRAY<S_tid>							list;	// �O�̑I�����ɖ߂�p�̃��[�J���Z�[�u���X�g
	std::map<S_tid, BSP<S_tnm_local_save>>	map;	// �O�̑I�����ɖ߂�p�̃��[�J���Z�[�u���f�[�^
};


// ****************************************************************
// �Z�[�u����
// ================================================================
struct S_tnm_save_history_item
{
	BYTE				flag;						// �t���O
	SYSTEMTIME			systemtime;					// ����
	S_tnm_local_save	local_save;					// ���[�J���Z�[�u
};

struct S_tnm_save_history
{
	bool							update_flag;	// �����̍X�V�t���O�i�_�C�A���O�ɔ��f�������_�ō~�낷�j
	DWORD							update_time;	// �������X�V���������i�����ԂōX�V����̂Ɏg���j
	int								top;			// �����̐擪
	int								cnt;			// �����̐�
	ARRAY<S_tnm_save_history_item>	item;			// ����
};

// ****************************************************************
// �o�b�N���O�Z�[�u
// ================================================================
struct S_tnm_backlog_save
{
	std::map<S_tid, BSP<S_tnm_local_save>>	map;
};
