#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_cmd_line.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_error.h"

#include	"engine/eng_msgbox.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_proc_stack.h"

#include	"localize/localize.h"

HHOOK G_h_hook = NULL;

static void set_yes_no_button_text(HWND h_wnd, CTSTR& yes_text, CTSTR& no_text)
{
	::SetDlgItemText(h_wnd, IDYES, yes_text.c_str());
	::SetDlgItemText(h_wnd, IDNO, no_text.c_str());
}

LRESULT CALLBACK msg_box_hook_proc(int n_code, WPARAM wp, LPARAM lp)
{
	if (n_code >= 0)	{
		set_yes_no_button_text((HWND)wp, _T("���s"), _T("�ްяI��"));
	}
	return CallNextHookEx(G_h_hook, n_code, wp, lp);
}

const int TNM_ERROR_DEPTH_NONE = 0;		// �G���[�\�����Ȃ�
const int TNM_ERROR_DEPTH_LOG = 1;		// ���O�ɂ̂ݏo��
const int TNM_ERROR_DEPTH_BOX = 2;		// ���b�Z�[�W�{�b�N�X���o��


void write_debug_log(CTSTR& str)
{
	// �f�o�b�O���̂ݎ��s
	if (Gp_global->debug_flag)
	{
		TSTR log;
		C_file log_file;

		// �f�B���N�g���쐬
		if (!create_dir_from_file_path(Gp_dir->get_debug_log_file_path()))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���O�t�@�C�������o���p�̃t�H���_���쐬�ł��܂���ł����B\n") + get_last_error_msg());
		}
		else if (!log_file.open(Gp_dir->get_debug_log_file_path(), _T("at")))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���O�t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
		}
		else
		{
			// ���t������
			log = _T("[") + get_sys_time_str() + _T("] ");

			// �V�[���ԍ���ǉ�
			int scn_no = Gp_lexer->get_cur_scn_no();
			if (scn_no >= 0)
			{
				TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
				int line_no = Gp_lexer->get_cur_line_no();
				log += _T("\t(") + scn_name + _T(".ss line=") + tostr(line_no) + _T(")");
			}

			// �G���[���O�ɏo��
			log_file.write_in_MB(log + _T("\t") + str + _T("\n"));
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str);
		}
	}
}

// ****************************************************************
// �G���W���F�G���[����
// ================================================================
//		�K���G���[�{�b�N�X���o���ׂ����́i�t�@�C���G���[�Ȃǁj
//		�f�o�b�O�ł̂݃G���[�{�b�N�X���o���ׂ����́i�I�[�o�[�t���[�Ȃǁj
//		�f�o�b�O�_�C�A���O�ɕ\�����邾���̂��́i�����Ȃǁj
// ================================================================
void C_tnm_error::init()
{
	m_last_error = TNM_ERROR_TYPE_NONE;
}

bool C_tnm_error::set_error(TNM_ERROR_TYPE error_type, CTSTR& error_str)
{
	// �ݒ�ɂ���Ă̓G���[�Ƃ��Ȃ����̂��͂���

	// �p�^�[���ԍ��G���[
	if (error_type == TNM_ERROR_TYPE_PAT_NO)	{
		if (!Gp_global->debug_flag)	{
			return true;	// �����[�X�Ȃ�G���[�ł͂Ȃ�
		}
		else if (Gp_ini->debug_error_patno_out_of_range == TNM_ERROR_DEPTH_NONE)	{
			return true;	// �f�o�b�O�ݒ�ɂ���Ă̓G���[�ł͂Ȃ�
		}
	}

	// �G���[�\����
	S_tnm_error error;
	error.error_type = error_type;
	error.error_str = error_str;
	
	// ���t������
	error.error_str = _T("[") + get_sys_time_str() + _T("] \n\n") + error.error_str;

	// �V�[���ԍ����m�肵�Ă���΃G���[�ɒǉ��i�f�o�b�O���̂݁j
	if (Gp_global->debug_flag)	{
		int scn_no = Gp_lexer->get_cur_scn_no();
		if (scn_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			int line_no = Gp_lexer->get_cur_line_no();
			error.error_str += _T("\n\n(") + scn_name + _T(".ss ") + tostr(line_no) + _T("�s��)");
		}
	}

	// �f�o�b�O���̓��b�Z�[�W�{�b�N�X�̋������t�b�N����
	DWORD additional_flag = 0;
	if (Gp_global->debug_flag)	{
		G_h_hook = ::SetWindowsHookEx(WH_CBT, msg_box_hook_proc, NULL, ::GetCurrentThreadId());
		additional_flag = MB_YESNO;
	}

	// �G���[�{�b�N�X���o��
	int res = 0;
	switch (error_type)	{
		case TNM_ERROR_TYPE_DIVIDED_BY_ZERO:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_OUT_OF_RANGE:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_OVERFLOW:		// �ݒ�ɂ���Ă̓G���[�{�b�N�X���o��
			if (Gp_global->debug_flag && !Gp_global->debug_not_disp_overflow_error)	{
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_FILE_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_PCT_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_SOUND_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_KOE_NOT_FOUND:	// �G���[�{�b�N�X���o���Ȃ��I�I�I
			break;
		case TNM_ERROR_TYPE_PAT_NO:
			if (Gp_ini->debug_error_patno_out_of_range == TNM_ERROR_DEPTH_BOX)	{	// ini �̐ݒ�ɂ���Ă̓G���[�{�b�N�X���o��
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_INFO:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONINFORMATION | additional_flag);
			break;
		case TNM_ERROR_TYPE_FATAL:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_DEBUG:			// �f�o�b�O���̂݃G���[�{�b�N�X���o��
			if (Gp_global->debug_flag)	{
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_WARNING:		// �G���[�{�b�N�X�͏o���Ȃ�
			break;
		case TNM_ERROR_TYPE_SUCCESS:		// �G���[�{�b�N�X�͏o���Ȃ�
			break;
	}

	// �f�o�b�O���̓��b�Z�[�W�{�b�N�X�̃t�b�N���I������
	if (Gp_global->debug_flag)	{
		::UnhookWindowsHookEx(G_h_hook);	// �t�b�N�I��
		if (res == IDNO)	{

				// �Q�[�����I������t���O
				Gp_global->game_end_flag = true;
				// �x���Ȃ��ŃQ�[�����I������t���O
				Gp_global->game_end_no_warning = true;
				// �G���h�Z�[�u�Ȃ��ŃQ�[�����I������t���O
				Gp_global->game_end_save_done = true;
				// ���[�v�𔲂���悤
				tnm_push_proc(TNM_PROC_TYPE_NONE);

			//tnm_push_proc(TNM_PROC_TYPE_END_GAME);		// �Q�[�����I������
		}
	}

	// �G���[���X�g�ɒǉ�
	if (Gp_global->debug_flag || !Gp_global->debug_decided_flag)
		m_error_list.push_back(error);

	// ���O�ɓf���o��
	if (Gp_cmd_line->log_set)	{

		// �Z�[�u�t�H���_���o���Ă���ꍇ
		if (!Gp_dir->save_dir.empty())	{

			C_file file;
			if (!file.open(Gp_dir->get_log_file_path(), _T("at")))
				return false;

			// �c���Ă���G���[��f���o��
			for (int i = 0; i < (int)m_rest_error_list.size(); i++)	{
				file.write_in_MB((m_rest_error_list[i] + _T("\n")).c_str());
			}
			m_rest_error_list.clear();

			// ����̃G���[��f���o��
			file.write_in_MB((error_str + _T("\n")).c_str());

			// �Y�ꂸ�Ƀt�@�C�������I
			file.close();
		}

		// �Z�[�u�t�H���_���o���Ă��Ȃ��ꍇ
		else	{

			m_rest_error_list.push_back(error_str);
		}
	}

	// ���X�g�G���[��ݒ�
	m_last_error = error_type;

	return false;
}
