#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// �V�X�e���R�}���h���j���[
// ================================================================
//		���[�J���ɃZ�[�u����܂��B
// ================================================================
struct S_tnm_syscommenu
{
	// ���݁E���^�֎~
	struct S_existenable
	{
		bool	exist;		// ���݂̗L��
		bool	enable;		// ���^�֎~
	};

	// ���݁E���^�֎~�EON/OFF���
	struct S_existenableonoff
	{
		bool	exist;		// ���݂̗L��
		bool	enable;		// ���^�֎~
		bool	onoff;		// ON/OFF���
	};

	// ���݁E���^�֎~�E���[�h�l
	struct S_existenablemode
	{
		bool	exist;		// ���݂̗L��
		bool	enable;		// ���^�֎~
		int		mode;		// ���[�h�l
	};

	S_existenable		read_skip;		// ��x�ǂ񂾕��͂̑�������J�n���܂��B
	S_existenable		unread_skip;	// ���ǂ̕��͂ł���������J�n���܂��B
	S_existenable		auto_skip;		// ����������@�\���n�m�^�n�e�e���܂��B
	S_existenable		auto_mode;		// �I�[�g���[�h���J�n���܂��B
	S_existenable		hide_mwnd;		// ���b�Z�[�W�E�B���h�E���B���܂��B
	S_existenable		msg_back;		// ���b�Z�[�W�o�b�N��\�����܂��B
	S_existenable		save;			// �Z�[�u���Ăяo���܂��B
	S_existenable		load;			// ���[�h���Ăяo���܂��B
	S_existenable		return_to_sel;	// �O�̑I�����ɖ߂�܂��B
	S_existenable		config;			// ���ݒ���Ăяo���܂��B
	S_existenable		manual;			// �}�j���A����\�����܂��B
	S_existenable		version;		// �o�[�W��������\�����܂��B
	S_existenable		return_to_menu;	// �^�C�g�����j���[�ɖ߂�܂��B
	S_existenable		game_end;		// �Q�[�����I�����܂��B
	S_existenable		cancel;			// �V�X�e���R�}���h���j���[���L�����Z�����܂��B

	S_existenableonoff	local_extra_switch[TNM_LOCAL_EXTRA_SWITCH_CNT];		// ���[�J���ėp�X�C�b�`
	S_existenablemode	local_extra_mode[TNM_LOCAL_EXTRA_MODE_CNT];			// ���[�J���ėp���[�h
};

// ****************************************************************
// �V�X�e���R�}���h
// ================================================================
class C_tnm_syscom
{
public:
	bool				read_skip_flag;		// ���Ǒ�����
	bool				auto_skip_flag;		// ����������
	bool				hide_mwnd_flag;		// ���b�Z�[�W�E�B���h�E���B��
	bool				msg_back_flag;		// ���b�Z�[�W�o�b�N���J��
	S_tnm_syscommenu	syscommenu;			// �V�X�e���R�}���h���j���[
};
