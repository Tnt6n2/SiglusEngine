#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"

#include	"engine/eng_system.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_dummy.h"

#include	"utility/util_dummy_file.h"

#include	"../resource.h"

// ****************************************************************
// �_�~�[�t�@�C���`�F�b�N�i�T�u�j
// ================================================================
bool tnm_check_dummy_file_sub(CTSTR& file_name, int key, CTSTR& code)
{
	// �f�B�X�N�h���C�u��T��
	TCHAR disc_letter = tnm_search_disc_drive();

	// �f�B�X�N�������Ă����^�[�̕ύX�͍s���܂���B
	// �ŏ��Z�b�g�A�b�v�̏ꍇ�́A�N�����ɒT���ɍs���ăp�X��ݒ肵�Ă���͂��B

	// �f�B�X�N��������Ȃ��ꍇ�͎��s
	if (disc_letter == 0)
		return false;

	// �_�~�[�t�@�C���̃p�X��ݒ�
	int version = 1;
	TSTR dummy_dir = TSTR() + disc_letter + _T(":\\dummy");
	if (file_get_type(dummy_dir) == 0)	{
		version = 2;	// dummy �t�H���_���������̂Ńo�[�W�����͂Q�ȏ�
	}
	else	{
		// dummy �t�H���_���Ȃ������̂Ńo�[�W�����P
		version = 1;
		// �t�H���_�����[�g�ɐݒ肵�Ȃ���
		dummy_dir = TSTR() + disc_letter + _T(":");
	}

	// �_�~�[�t�@�C�����`�F�b�N
	C_dummy_file_checker dummy_file_checker;
	if (!dummy_file_checker.check_dummy_file(version, dummy_dir, file_name, key, code))	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�_�~�[�t�@�C����������܂���ł����B"));
		return false;
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�_�~�[�t�@�C���̔���ɐ������܂����B"));
	return true;
}

// ****************************************************************
// �_�~�[�t�@�C���`�F�b�N
// ================================================================
bool tnm_check_dummy_file(CTSTR& file_name, int key, CTSTR& code)
{
	bool return_value = false;

	// �R���s���[�^�h�c�ƃ_�~�[�`�F�b�N�h�c���r����
	int computer_id = get_computer_id();
	if (check_dummy_check_flag(computer_id))
		return true;	// ���Ɋ������Ă���ꍇ�͉������Ȃ�

	// �`�F�b�N���_�C�A���O���쐬
	C_dialog dlg;
	dlg.create(IDD_DUMMY, _T("dummy"), false, false, G_app.h_wnd);
	dlg.set_text(Gp_ini->game_name);
	dlg.update();

	// ���ʂ��o��܂Ń`�F�b�N���J��Ԃ�
	while (1)	{

		// �`�F�b�N���_�C�A���O��\��
		dlg.set_show(true);

		// �_�C�A���O���X�V���邽�߃��b�Z�[�W���[�v
		G_app.default_message_loop();

		// �i�D�����̂ł�����Ƒ҂�
		Sleep(1000);

		// �_�~�[�t�@�C�����`�F�b�N����
		bool dummy_check = tnm_check_dummy_file_sub(file_name, key, code);
		
		// �`�F�b�N���_�C�A���O���\��
		dlg.set_show(false);

		// �f�B�X�N��������Ȃ�����
		if (!dummy_check)	{

			// �x��
			TSTR str = str_replace_ret(Gp_ini->dummy_check_str, _T("\\n"), _T("\n"));
			int res = tnm_game_message_box(str, MB_OKCANCEL | MB_ICONINFORMATION);
			if (res == IDCANCEL)	{

				// �Q�[�����I������t���O
				Gp_global->game_end_flag = true;
				// �x���Ȃ��ŃQ�[�����I������t���O
				Gp_global->game_end_no_warning = true;
				// �G���h�Z�[�u�Ȃ��ŃQ�[�����I������t���O
				Gp_global->game_end_save_done = true;
				// ���[�v�𔲂���悤
				tnm_push_proc(TNM_PROC_TYPE_NONE);

				return_value = false;
				break;
			}
		}
		// ������
		else	{

			// �_�~�[�`�F�b�N�h�c�ɃR���s���[�^�h�c��ݒ�
			set_dummy_check_flag(computer_id);

			// �F�؂ɐ���
			TSTR str = str_replace_ret(Gp_ini->dummy_check_ok_str, _T("\\n"), _T("\n"));
			tnm_game_message_box(str, MB_OK | MB_ICONINFORMATION);

			// ���������ɏI��
			return_value = true;
			break;
		}
	}

	// �`�F�b�N���_�C�A���O�����
	dlg.close();

	return return_value;
}

