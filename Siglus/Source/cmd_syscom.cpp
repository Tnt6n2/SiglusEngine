#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_save_cache.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_icon.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_moji_manager.h"
#include	"element/elm_prop.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_system.h"
#include	"engine/eng_display.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_dummy.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_twitter.h"
#include	"engine/eng_chihaya.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_font.h"
#include	"localize/localize.h"
#include	"../resource.h"

// ****************************************************************
// �R�}���h�����F�V�X�e��
// ================================================================
bool tnm_command_proc_system(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	switch (elm_begin[0])	{

		// ���݂̓��t�Ǝ������擾
		case ELM_SYSTEM_GET_CALENDAR:
		{
			// �����̃G�������g�R�[�h���擾
			S_element* p_code_year = &p_ai->al_begin[0].element;
			S_element* p_code_month = &p_ai->al_begin[1].element;
			S_element* p_code_day = &p_ai->al_begin[2].element;
			S_element* p_code_weekday = &p_ai->al_begin[3].element;
			S_element* p_code_hour = &p_ai->al_begin[4].element;
			S_element* p_code_minute = &p_ai->al_begin[5].element;
			S_element* p_code_second = &p_ai->al_begin[6].element;
			S_element* p_code_millsecond = &p_ai->al_begin[7].element;

			// �G�������g�̃t���O���擾
			C_elm_int_list* p_element_year = (C_elm_int_list *)tnm_get_element_ptr(p_code_year->begin(), p_code_year->end() - 2, true);
			C_elm_int_list* p_element_month = (C_elm_int_list *)tnm_get_element_ptr(p_code_month->begin(), p_code_month->end() - 2, true);
			C_elm_int_list* p_element_day = (C_elm_int_list *)tnm_get_element_ptr(p_code_day->begin(), p_code_day->end() - 2, true);
			C_elm_int_list* p_element_weekday = (C_elm_int_list *)tnm_get_element_ptr(p_code_weekday->begin(), p_code_weekday->end() - 2, true);
			C_elm_int_list* p_element_hour = (C_elm_int_list *)tnm_get_element_ptr(p_code_hour->begin(), p_code_hour->end() - 2, true);
			C_elm_int_list* p_element_minute = (C_elm_int_list *)tnm_get_element_ptr(p_code_minute->begin(), p_code_minute->end() - 2, true);
			C_elm_int_list* p_element_second = (C_elm_int_list *)tnm_get_element_ptr(p_code_second->begin(), p_code_second->end() - 2, true);
			C_elm_int_list* p_element_millsecond = (C_elm_int_list *)tnm_get_element_ptr(p_code_millsecond->begin(), p_code_millsecond->end() - 2, true);

			// �t���O�̃C���f�b�N�X���擾
			int index_year = *(p_code_year->end() - 1);
			int index_month = *(p_code_month->end() - 1);
			int index_day = *(p_code_day->end() - 1);
			int index_weekday = *(p_code_weekday->end() - 1);
			int index_hour = *(p_code_hour->end() - 1);
			int index_minute = *(p_code_minute->end() - 1);
			int index_second = *(p_code_second->end() - 1);
			int index_millsecond = *(p_code_millsecond->end() - 1);

			// ���ݎ������擾
			SYSTEMTIME system_time;
			GetLocalTime(&system_time);

			int value_year = system_time.wYear;
			int value_month = system_time.wMonth;
			int value_day = system_time.wDay;
			int value_weekday = system_time.wDayOfWeek;
			int value_hour = system_time.wHour;
			int value_minute = system_time.wMinute;
			int value_second = system_time.wSecond;
			int value_millsecond = system_time.wMilliseconds;

			// ���v���C���Ԃ�ݒ�
			p_element_year->set_value(index_year, value_year);
			p_element_month->set_value(index_month, value_month);
			p_element_day->set_value(index_day, value_day);
			p_element_weekday->set_value(index_weekday, value_weekday);
			p_element_hour->set_value(index_hour, value_hour);
			p_element_minute->set_value(index_minute, value_minute);
			p_element_second->set_value(index_second, value_second);
			p_element_millsecond->set_value(index_millsecond, value_millsecond);

			break;
		}

		// ���݂̎������擾
		case ELM_SYSTEM_GET_UNIX_TIME:
		{
			time_t t = time(NULL);
			tnm_stack_push_int((int)t);
			break;
		}

		// �E�B���h�E���A�N�e�B�u���ǂ����𔻒�
		case ELM_SYSTEM_CHECK_ACTIVE:
		{
			tnm_stack_push_int(Gp_global->active_flag ? 1 : 0);
			break;
		}

		// �f�o�b�O���ǂ����𒲂ׂ�
		case ELM_SYSTEM_CHECK_DEBUG_FLAG:
		{
			tnm_stack_push_int(Gp_global->debug_flag ? 1 : 0);
			break;
		};

		// �V�F���Ńt�@�C�����J��
		case ELM_SYSTEM_SHELL_OPEN_FILE:
		{
			TSTR path = Gp_dir->base_dir + _T("\\") + p_ai->al_begin[0].str;
			if (file_get_type(path) == 1)	{
				shell_execute_open(path);
			}
			else	{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, p_ai->al_begin[0].str + _T(" ��������܂���B"));
			}
			break;
		}

		// �V�F���ŃE�F�u�y�[�W���J��
		case ELM_SYSTEM_SHELL_OPEN_WEB:
		{
			TSTR url = p_ai->al_begin[0].str;
			shell_execute_open(url);
			break;
		}

		// �t�@�C���̑��݃`�F�b�N
		case ELM_SYSTEM_CHECK_FILE_EXIST:
		{
			TSTR path = Gp_dir->base_dir + _T("\\") + p_ai->al_begin[0].str;
			if (file_get_type(path) >= 0)	{
				tnm_stack_push_int(1);	// ���݂���
			}
			else	{
				tnm_stack_push_int(0);	// ���݂��Ȃ�
			}
			break;
		}

		// �t�@�C���̑��݃`�F�b�N
		case ELM_SYSTEM_CHECK_FILE_EXIST_SAVE_DIR:
		{
			TSTR path = Gp_dir->save_dir + _T("\\") + p_ai->al_begin[0].str;
			if (file_get_type(path) >= 0)	{
				tnm_stack_push_int(1);	// ���݂���
			}
			else	{
				tnm_stack_push_int(0);	// ���݂��Ȃ�
			}
			break;
		}

		// �_�~�[�t�@�C��
		case ELM_SYSTEM_CHECK_DUMMY_FILE_ONCE:
		{
			tnm_check_dummy_file(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].str);
			break;
		}

		case ELM_SYSTEM_CLEAR_DUMMY_FILE:
		{
			clear_dummy_check_flag();
			break;
		}

		// ���b�Z�[�W�{�b�N�X
		case ELM_SYSTEM_MESSAGEBOX_OK:
		case ELM_SYSTEM_MESSAGEBOX_OKCANCEL:
		{
			int ret_value = 0;

			// ���b�Z�[�W�{�b�N�X���o��
			int type = MB_OK;
			int res = IDOK;

			switch (elm_begin[0])	{
			case ELM_SYSTEM_MESSAGEBOX_OK:			type = MB_OK;			break;
			case ELM_SYSTEM_MESSAGEBOX_OKCANCEL:	type = MB_OKCANCEL;		break;
			}
			switch (p_ai->al_id)	{
			case 0:		res = tnm_game_message_box(tostr(p_ai->al_begin[0].Int), type);		break;
			case 1:		res = tnm_game_message_box(p_ai->al_begin[0].str, type);			break;
			}
			switch (res)	{
			case IDOK:		ret_value = 0;	break;
			case IDCANCEL:	ret_value = 1;	break;
			}

			tnm_stack_push_int(ret_value);
			break;
		}
		case ELM_SYSTEM_MESSAGEBOX_YESNO:
		case ELM_SYSTEM_MESSAGEBOX_YESNOCANCEL:
		{
			int ret_value = 0;

			// ���b�Z�[�W�{�b�N�X���o��
			int type = MB_YESNO;
			int res = IDYES;

			switch (elm_begin[0])	{
			case ELM_SYSTEM_MESSAGEBOX_YESNO:		type = MB_YESNO;		break;
			case ELM_SYSTEM_MESSAGEBOX_YESNOCANCEL:	type = MB_YESNOCANCEL;	break;
			}
			switch (p_ai->al_id)	{
			case 0:		res = tnm_game_message_box(tostr(p_ai->al_begin[0].Int), type);		break;
			case 1:		res = tnm_game_message_box(p_ai->al_begin[0].str, type);			break;
			}
			switch (res)	{
			case IDYES:		ret_value = 0;	break;
			case IDNO:		ret_value = 1;	break;
			case IDCANCEL:	ret_value = 2;	break;
			}

			tnm_stack_push_int(ret_value);
			break;
		}

		// �f�o�b�O���b�Z�[�W
		case ELM_SYSTEM_DEBUG_MESSAGEBOX_OK:
		case ELM_SYSTEM_DEBUG_MESSAGEBOX_OKCANCEL:
		{
			int ret_value = 0;

			// �f�o�b�O���̂݃��b�Z�[�W�{�b�N�X���o��
			if (Gp_global->debug_flag)	{
				int type = MB_OK;
				int res = IDOK;

				switch (elm_begin[0])	{
					case ELM_SYSTEM_DEBUG_MESSAGEBOX_OK:		type = MB_OK;			break;
					case ELM_SYSTEM_DEBUG_MESSAGEBOX_OKCANCEL:	type = MB_OKCANCEL;		break;
				}
				switch (p_ai->al_id)	{
					case 0:		res = tnm_game_message_box(tostr(p_ai->al_begin[0].Int), type);		break;
					case 1:		res = tnm_game_message_box(p_ai->al_begin[0].str, type);			break;
				}
				switch (res)	{
					case IDOK:		ret_value = 0;	break;
					case IDCANCEL:	ret_value = 1;	break;
				}
			}

			tnm_stack_push_int(ret_value);
			break;
		}
		case ELM_SYSTEM_DEBUG_MESSAGEBOX_YESNO:
		case ELM_SYSTEM_DEBUG_MESSAGEBOX_YESNOCANCEL:
		{
			int ret_value = 0;

			// �f�o�b�O���̂݃��b�Z�[�W�{�b�N�X���o��
			if (Gp_global->debug_flag)	{
				int type = MB_YESNO;
				int res = IDYES;

				switch (elm_begin[0])	{
					case ELM_SYSTEM_DEBUG_MESSAGEBOX_YESNO:			type = MB_YESNO;		break;
					case ELM_SYSTEM_DEBUG_MESSAGEBOX_YESNOCANCEL:	type = MB_YESNOCANCEL;	break;
				}
				switch (p_ai->al_id)	{
					case 0:		res = tnm_game_message_box(tostr(p_ai->al_begin[0].Int), type);		break;
					case 1:		res = tnm_game_message_box(p_ai->al_begin[0].str, type);			break;
				}
				switch (res)	{
					case IDYES:		ret_value = 0;	break;
					case IDNO:		ret_value = 1;	break;
					case IDCANCEL:	ret_value = 2;	break;
				}
			}

			tnm_stack_push_int(ret_value);
			break;
		}

		// �f�o�b�O���O
		case ELM_SYSTEM_DEBUG_WRITE_LOG:
		{
			TSTR log;

			// �f�o�b�O���̂ݎ��s
			if (Gp_global->debug_flag)	{

				C_file log_file;

				// �f�B���N�g���쐬
				if (!create_dir_from_file_path(Gp_dir->get_debug_log_file_path()))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���O�t�@�C�������o���p�̃t�H���_���쐬�ł��܂���ł����B\n") + get_last_error_msg());
				}
				else if (!log_file.open(Gp_dir->get_debug_log_file_path(), _T("at")))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���O�t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
				}
				else	{

					// ���t������
					log = _T("[") + get_sys_time_str() + _T("] ");

					// �V�[���ԍ���ǉ�
					int scn_no = Gp_lexer->get_cur_scn_no();
					if (scn_no >= 0)	{
						TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
						int line_no = Gp_lexer->get_cur_line_no();
						log += _T("\t(") + scn_name + _T(".ss line=") + tostr(line_no) + _T(")");
					}

					// �^����ꂽ��������Ō�ɒǉ�
					TSTR str;
					switch (p_ai->al_id)	{
						case 0:		str = tostr(p_ai->al_begin[0].Int);		break;
						case 1:		str = p_ai->al_begin[0].str;			break;
					}

					// �G���[���O�ɏo��
					log_file.write_in_MB(log + _T("\t") + str + _T("\n"));
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str);
				}
			}
			break;
		}

		// ���͂�x���`�F�����擾
		case ELM_SYSTEM_GET_SPEC_INFO_FOR_CHIHAYA_BENCH:
		{
			tnm_stack_push_str(tnm_get_spec_info_for_chihaya_bench());
			break;
		}

		// ���͂�x���`�F�_�C�A���O��\��
		case ELM_SYSTEM_OPEN_DIALOG_FOR_CHIHAYA_BENCH:
		{
			TSTR str = p_ai->al_begin[0].str;

			// ���_�C�A���O��\��
			tnm_open_chihaya_bench_dialog(str);
			break;
		}

		// ������擾
		case ELM_SYSTEM_GET_LANGUAGE:
		{
			tnm_stack_push_str(LCL_LANGUAGE_CODE);
			break;
		}

		default:
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�����ȃR�}���h���w�肳��܂����B(system)"));
			break;
	}

	return true;
}

// ****************************************************************
// �R�}���h�����F�V�X�e���R�}���h
// ================================================================
bool tnm_command_proc_syscom(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	switch (elm_begin[0])	{
		case ELM_SYSCOM_CALL_EX:
			tnm_command_proc_farcall_ex(p_ai->al_id, p_ai->al_begin, p_ai->al_end, FM_VOID);	// �߂�l��ςނ̂� return �ōs��
			break;
		case ELM_SYSCOM_CALL_SYSCOM_MENU:
			tnm_syscom_open();
			break;
		case ELM_SYSCOM_SET_SYSCOM_MENU_ENABLE:
			Gp_local->pod.syscom_menu_disable = false;
			break;
		case ELM_SYSCOM_SET_SYSCOM_MENU_DISABLE:
			Gp_local->pod.syscom_menu_disable = true;
			break;
		case ELM_SYSCOM_SET_MWND_BTN_ENABLE:
			switch (p_ai->al_id)	{
				case 0:
					Gp_local->pod.mwnd_btn_disable_all = false;
					break;
				case 1:
					if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_local->mwnd_btn_disable.size())	{
						Gp_local->mwnd_btn_disable[p_ai->al_begin[0].Int].value = false;
					}
					break;
			}
			break;
		case ELM_SYSCOM_SET_MWND_BTN_DISABLE:
			switch (p_ai->al_id)	{
				case 0:
					Gp_local->pod.mwnd_btn_disable_all = true;
					break;
				case 1:
					if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_local->mwnd_btn_disable.size())	{
						Gp_local->mwnd_btn_disable[p_ai->al_begin[0].Int].value = true;
					}
					break;
			}
			break;
		case ELM_SYSCOM_SET_MWND_BTN_TOUCH_ENABLE:
			Gp_local->pod.mwnd_btn_touch_disable = false;
			break;
		case ELM_SYSCOM_SET_MWND_BTN_TOUCH_DISABLE:
			Gp_local->pod.mwnd_btn_touch_disable = true;
			break;

	// �t���O��������
		case ELM_SYSCOM_INIT_SYSCOM_FLAG:
			tnm_syscom_init_syscom_flag();
			break;

	// ������Ȃ�
		case ELM_SYSCOM_SET_READ_SKIP_ONOFF_FLAG:
			tnm_syscom_read_skip_set_onoff_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_READ_SKIP_ONOFF_FLAG:
			tnm_stack_push_int(tnm_syscom_read_skip_get_onoff_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_READ_SKIP_ENABLE_FLAG:
			tnm_syscom_read_skip_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_READ_SKIP_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_read_skip_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_READ_SKIP_EXIST_FLAG:
			tnm_syscom_read_skip_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_READ_SKIP_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_read_skip_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_READ_SKIP_ENABLE:
			tnm_stack_push_int(tnm_syscom_read_skip_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_SKIP_ONOFF_FLAG:
			tnm_syscom_auto_skip_set_onoff_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_SKIP_ONOFF_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_skip_get_onoff_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_SKIP_ENABLE_FLAG:
			tnm_syscom_auto_skip_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_SKIP_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_skip_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_SKIP_EXIST_FLAG:
			tnm_syscom_auto_skip_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_SKIP_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_skip_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_AUTO_SKIP_ENABLE:
			tnm_stack_push_int(tnm_syscom_auto_skip_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_ONOFF_FLAG:
			tnm_syscom_auto_mode_set_onoff_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_MODE_ONOFF_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_mode_get_onoff_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_ENABLE_FLAG:
			tnm_syscom_auto_mode_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_MODE_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_mode_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_EXIST_FLAG:
			tnm_syscom_auto_mode_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_AUTO_MODE_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_auto_mode_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_AUTO_MODE_ENABLE:
			tnm_stack_push_int(tnm_syscom_auto_mode_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_HIDE_MWND_ONOFF_FLAG:
			tnm_syscom_hide_mwnd_set_onoff_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_HIDE_MWND_ONOFF_FLAG:
			tnm_stack_push_int(tnm_syscom_hide_mwnd_get_onoff_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_HIDE_MWND_ENABLE_FLAG:
			tnm_syscom_hide_mwnd_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_HIDE_MWND_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_hide_mwnd_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_HIDE_MWND_EXIST_FLAG:
			tnm_syscom_hide_mwnd_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_HIDE_MWND_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_hide_mwnd_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_HIDE_MWND_ENABLE:
			tnm_stack_push_int(tnm_syscom_hide_mwnd_is_enable() ? 1 : 0);
			break;

	// ���[�J���ėp�X�C�b�`
		case ELM_SYSCOM_SET_LOCAL_EXTRA_SWITCH_ONOFF_FLAG:
			tnm_syscom_local_extra_switch_set_onoff_flag(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_SWITCH_ONOFF_FLAG:
			tnm_stack_push_int(tnm_syscom_local_extra_switch_get_onoff_flag(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_LOCAL_EXTRA_SWITCH_ENABLE_FLAG:
			tnm_syscom_local_extra_switch_set_enable_flag(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_SWITCH_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_local_extra_switch_get_enable_flag(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_LOCAL_EXTRA_SWITCH_EXIST_FLAG:
			tnm_syscom_local_extra_switch_set_exist_flag(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_SWITCH_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_local_extra_switch_get_exist_flag(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_LOCAL_EXTRA_SWITCH_ENABLE:
			tnm_stack_push_int(tnm_syscom_local_extra_switch_is_enable(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
			
	// ���[�J�����[�h
		case ELM_SYSCOM_SET_LOCAL_EXTRA_MODE_VALUE:
			tnm_syscom_local_extra_mode_set_value(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_MODE_VALUE:
			tnm_stack_push_int(tnm_syscom_local_extra_mode_get_value(p_ai->al_begin[0].Int));
			break;
		case ELM_SYSCOM_SET_LOCAL_EXTRA_MODE_ENABLE_FLAG:
			tnm_syscom_local_extra_mode_set_enable_flag(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_MODE_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_local_extra_mode_get_enable_flag(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_LOCAL_EXTRA_MODE_EXIST_FLAG:
			tnm_syscom_local_extra_mode_set_exist_flag(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOCAL_EXTRA_MODE_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_local_extra_mode_get_exist_flag(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_LOCAL_EXTRA_MODE_ENABLE:
			tnm_stack_push_int(tnm_syscom_local_extra_mode_is_enable(p_ai->al_begin[0].Int) ? 1 : 0);
			break;

	// �V�[������
		case ELM_SYSCOM_OPEN_MSG_BACK:
			tnm_syscom_open_msg_back();
			break;
		case ELM_SYSCOM_CLOSE_MSG_BACK:
			tnm_syscom_close_msg_back();
			break;
		case ELM_SYSCOM_SET_MSG_BACK_ENABLE_FLAG:
			tnm_syscom_msg_back_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_MSG_BACK_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_msg_back_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_MSG_BACK_EXIST_FLAG:
			tnm_syscom_msg_back_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_MSG_BACK_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_msg_back_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_MSG_BACK_ENABLE:
			tnm_stack_push_int(tnm_syscom_msg_back_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_MSG_BACK_OPEN:
			tnm_stack_push_int(tnm_msg_back_is_open() ? 1 : 0);
			break;
		case ELM_SYSCOM_RETURN_TO_SEL:
			tnm_syscom_return_to_sel(p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false);
			break;
		case ELM_SYSCOM_SET_RETURN_TO_SEL_ENABLE_FLAG:
			tnm_syscom_return_to_sel_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_RETURN_TO_SEL_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_return_to_sel_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_RETURN_TO_SEL_EXIST_FLAG:
			tnm_syscom_return_to_sel_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_RETURN_TO_SEL_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_return_to_sel_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_RETURN_TO_SEL_ENABLE:
			tnm_stack_push_int(tnm_syscom_return_to_sel_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_RETURN_TO_MENU:
		{
			// ���O������
			bool msgback_except_flag = false;
			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
				case 0:		msgback_except_flag = arg->Int ? true : false;		break;
				}
			}

			tnm_syscom_return_to_menu(p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false, msgback_except_flag);
			break;
		}
		case ELM_SYSCOM_SET_RETURN_TO_MENU_ENABLE_FLAG:
			tnm_syscom_return_to_menu_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_RETURN_TO_MENU_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_return_to_menu_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_RETURN_TO_MENU_EXIST_FLAG:
			tnm_syscom_return_to_menu_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_RETURN_TO_MENU_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_return_to_menu_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_RETURN_TO_MENU_ENABLE:
			tnm_stack_push_int(tnm_syscom_return_to_menu_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_END_GAME:
			if (p_ai->al_id == 1)	{		// �V�o�[�W�����i2013/7/1�j
				tnm_syscom_end_game(p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false);
				//tnm_push_proc(TNM_PROC_TYPE_DISP);							// �`����s���i���ɑg�ݍ��񂾂̂ł�������Ȃ� 2013/7/1�j
			}
			else	{						// ���o�[�W����
				tnm_syscom_end_game(p_ai->al_begin[0].Int ? true : false, false, false);
				//tnm_push_proc(TNM_PROC_TYPE_DISP);							// �`����s���i���ɑg�ݍ��񂾂̂ł�������Ȃ� 2013/7/1�j
			}
			break;
		case ELM_SYSCOM_SET_END_GAME_ENABLE_FLAG:
			tnm_syscom_end_game_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_END_GAME_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_end_game_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_END_GAME_EXIST_FLAG:
			tnm_syscom_end_game_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_END_GAME_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_end_game_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_END_GAME_ENABLE:
			tnm_stack_push_int(tnm_syscom_end_game_is_enable() ? 1 : 0);
			break;

		case ELM_SYSCOM_REPLAY_KOE:
			tnm_syscom_koe_play();
			break;
		case ELM_SYSCOM_CHECK_REPLAY_KOE:
			tnm_stack_push_int(Gp_local->pod.cur_koe_no >= 0 ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_REPLAY_KOE_KOE_NO:
			tnm_stack_push_int(Gp_local->pod.cur_koe_no);
			break;
		case ELM_SYSCOM_GET_REPLAY_KOE_CHARA_NO:
			tnm_stack_push_int(Gp_local->pod.cur_chr_no);
			break;
		case ELM_SYSCOM_CLEAR_REPLAY_KOE:
			Gp_local->pod.cur_chr_no = -1;
			Gp_local->pod.cur_koe_no = -1;
			break;

		case ELM_SYSCOM_GET_CURRENT_SAVE_SCENE_TITLE:
			tnm_stack_push_str(Gp_local_save->save_scene_title);
			break;
		case ELM_SYSCOM_GET_CURRENT_SAVE_MESSAGE:
			tnm_stack_push_str(Gp_local_save->save_msg);
			break;

	// �^�C�}�[����
		case ELM_SYSCOM_GET_TOTAL_PLAY_TIME:
		{
			// �����̃G�������g�R�[�h���擾
			S_element* p_code_day = &p_ai->al_begin[0].element;
			S_element* p_code_hour = &p_ai->al_begin[1].element;
			S_element* p_code_minute = &p_ai->al_begin[2].element;
			S_element* p_code_second = &p_ai->al_begin[3].element;
			S_element* p_code_millsecond = &p_ai->al_begin[4].element;

			// �G�������g�̃t���O���擾
			C_elm_int_list* p_element_day = (C_elm_int_list *)tnm_get_element_ptr(p_code_day->begin(), p_code_day->end() - 2, true);
			C_elm_int_list* p_element_hour = (C_elm_int_list *)tnm_get_element_ptr(p_code_hour->begin(), p_code_hour->end() - 2, true);
			C_elm_int_list* p_element_minute = (C_elm_int_list *)tnm_get_element_ptr(p_code_minute->begin(), p_code_minute->end() - 2, true);
			C_elm_int_list* p_element_second = (C_elm_int_list *)tnm_get_element_ptr(p_code_second->begin(), p_code_second->end() - 2, true);
			C_elm_int_list* p_element_millsecond = (C_elm_int_list *)tnm_get_element_ptr(p_code_millsecond->begin(), p_code_millsecond->end() - 2, true);

			// �t���O�̃C���f�b�N�X���擾
			int index_day = *(p_code_day->end() - 1);
			int index_hour = *(p_code_hour->end() - 1);
			int index_minute = *(p_code_minute->end() - 1);
			int index_second = *(p_code_second->end() - 1);
			int index_millsecond = *(p_code_millsecond->end() - 1);

			// ���v���C���Ԃ��擾
			__int64 time = Gp_timer->global_real_time;
			int value_millsecond = (int)(time % 1000);	time /= 1000;
			int value_second = (int)(time % 60);		time /= 60;
			int value_minute = (int)(time % 60);		time /= 60;
			int value_hour = (int)(time % 24);			time /= 24;
			int value_day = (int)(time);

			// ���v���C���Ԃ�ݒ�
			p_element_day->set_value(index_day, value_day);
			p_element_hour->set_value(index_hour, value_hour);
			p_element_minute->set_value(index_minute, value_minute);
			p_element_second->set_value(index_second, value_second);
			p_element_millsecond->set_value(index_millsecond, value_millsecond);

			break;
		}

		case ELM_SYSCOM_SET_TOTAL_PLAY_TIME:
		{
			// ���v���C���Ԃ��擾
			int value_day = p_ai->al_begin[0].Int;
			int value_hour = p_ai->al_begin[1].Int;
			int value_minute = p_ai->al_begin[2].Int;
			int value_second = p_ai->al_begin[3].Int;
			int value_millsecond = p_ai->al_begin[4].Int;

			// ���v���C���Ԃ�ݒ�
			Gp_timer->global_real_time = ((((__int64)value_day * 24 + value_hour) * 60 + value_minute) * 60 + value_second) * 1000 + value_millsecond;

			break;
		}

	// �Z�[�u�^���[�h���J��
		case ELM_SYSCOM_CALL_SAVE_MENU:
			tnm_syscom_open_save(false, false, false);	// �i���Z�[�u�F�R�}���h�j�_�C�A���O���Ă΂��ꍇ���L���v�`���[���Ȃ�
			break;
		case ELM_SYSCOM_SET_SAVE_ENABLE_FLAG:
			tnm_syscom_open_save_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_SAVE_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_open_save_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_SAVE_EXIST_FLAG:
			tnm_syscom_open_save_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_SAVE_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_open_save_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_SAVE_ENABLE:
			tnm_stack_push_int(tnm_syscom_open_save_is_enable() ? 1 : 0);
			break;
		case ELM_SYSCOM_CALL_LOAD_MENU:
			tnm_syscom_open_load(false, false);
			break;
		case ELM_SYSCOM_SET_LOAD_ENABLE_FLAG:
			tnm_syscom_open_load_set_enable_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOAD_ENABLE_FLAG:
			tnm_stack_push_int(tnm_syscom_open_load_get_enable_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_LOAD_EXIST_FLAG:
			tnm_syscom_open_load_set_exist_flag(p_ai->al_begin[0].Int ? true : false);
			break;
		case ELM_SYSCOM_GET_LOAD_EXIST_FLAG:
			tnm_stack_push_int(tnm_syscom_open_load_get_exist_flag() ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_LOAD_ENABLE:
			tnm_stack_push_int(tnm_syscom_open_load_is_enable() ? 1 : 0);
			break;

	// �Z�[�u�^���[�h
		case ELM_SYSCOM_SAVE:
			tnm_stack_push_int(tnm_syscom_save(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false) ? 1 : 0);
			break;
		case ELM_SYSCOM_LOAD:
			tnm_syscom_load(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false, p_ai->al_begin[3].Int ? true : false);
			break;
		case ELM_SYSCOM_QUICK_SAVE:
			tnm_stack_push_int(tnm_syscom_quick_save(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false) ? 1 : 0);
			break;
		case ELM_SYSCOM_QUICK_LOAD:
			tnm_syscom_quick_load(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false, p_ai->al_begin[3].Int ? true : false);
			break;
		case ELM_SYSCOM_END_SAVE:
			tnm_stack_push_int(tnm_syscom_end_save(p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false) ? 1 : 0);
			break;
		case ELM_SYSCOM_END_LOAD:
			tnm_syscom_end_load(p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false);
			break;

	// �C���i�[�Z�[�u
		case ELM_SYSCOM_INNER_SAVE:
			tnm_stack_push_int(tnm_saveload_proc_create_inner_save(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_INNER_LOAD:
			tnm_stack_push_int(tnm_syscom_inner_load(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false, p_ai->al_begin[3].Int ? true : false) ? 1 : 0);
			break;
		case ELM_SYSCOM_CLEAR_INNER_SAVE:
			tnm_stack_push_int(tnm_saveload_proc_clear_inner_save(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_COPY_INNER_SAVE:
			tnm_stack_push_int(tnm_saveload_proc_copy_inner_save(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_CHECK_INNER_SAVE:
			tnm_stack_push_int(tnm_saveload_proc_check_inner_save(p_ai->al_begin[0].Int) ? 1 : 0);
			break;

	// ���b�Z�[�W�o�b�N�Z�[�u
		case ELM_SYSCOM_MSG_BACK_LOAD:
			tnm_syscom_msgbk_load(Gp_global->msg_back_load_tid, p_ai->al_begin[0].Int ? true : false, p_ai->al_begin[1].Int ? true : false, p_ai->al_begin[2].Int ? true : false);
			break;

	// �Z�[�u�f�[�^
		case ELM_SYSCOM_GET_SAVE_CNT:
		{
			tnm_stack_push_int(Gp_ini->save_cnt);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_CNT:
		{
			tnm_stack_push_int(Gp_ini->quick_save_cnt);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_NEW_NO:
		{
			if (p_ai->al_id == 0)
			{
				int new_save_no = tnm_get_new_save_no();
				tnm_stack_push_int(new_save_no);
			}
			else if (p_ai->al_id == 1)
			{
				int new_save_no = tnm_get_new_save_no(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
				tnm_stack_push_int(new_save_no);
			}
			else
			{
				tnm_stack_push_int(-1);		// �ʂ�Ȃ�
			}
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_NEW_NO:
		{
			if (p_ai->al_id == 0)
			{
				int new_quick_save_no = tnm_get_new_quick_save_no();
				tnm_stack_push_int(new_quick_save_no);
			}
			else if (p_ai->al_id == 1)
			{
				int new_save_no = tnm_get_new_quick_save_no(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
				tnm_stack_push_int(new_save_no);
			}
			else
			{
				tnm_stack_push_int(-1);		// �ʂ�Ȃ�
			}
			break;
		}
		case ELM_SYSCOM_GET_SAVE_EXIST:
		{
			bool exist = tnm_check_save_file(p_ai->al_begin[0].Int);
			tnm_stack_push_int(exist == 1 ? 1 : 0);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_YEAR:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.year);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_MONTH:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.month);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_DAY:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.day);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_WEEKDAY:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.weekday);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_HOUR:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.hour);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_MINUTE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.minute);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_SECOND:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.second);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_MILLISECOND:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.millisecond);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_TITLE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.title);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_MESSAGE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.message);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_FULL_MESSAGE:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.full_message);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_COMMENT:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.comment);
			break;
		}
		case ELM_SYSCOM_SET_SAVE_COMMENT:
		{
			S_tnm_save_header header = {0};
			if (tnm_load_save_header(p_ai->al_begin[0].Int, &header))	{
				ZeroMemory(header.comment, sizeof(header.comment));		// �R�����g����������
				_tcsncpy_s(header.comment, TNM_SAVE_COMMENT_MAX_LEN, p_ai->al_begin[1].str.c_str(), TNM_SAVE_COMMENT_MAX_LEN - 1);
				tnm_save_save_header(p_ai->al_begin[0].Int, &header);
			}
			break;
		}
		case ELM_SYSCOM_GET_SAVE_VALUE:
		{
			int save_no = p_ai->al_begin[0].Int;					// �Z�[�u�f�[�^�ԍ�
			S_element* p_element = &p_ai->al_begin[1].element;		// �R�s�[��̃t���O�G�������g
			int flag_index = p_ai->al_begin[2].Int;				// �t���O�J�n�ʒu
			int flag_cnt = p_ai->al_begin[3].Int;					// �t���O��
			if (flag_cnt > TNM_SAVE_FLAG_MAX_CNT)
				flag_cnt = TNM_SAVE_FLAG_MAX_CNT;

			// �R�s�[��̃t���O�G�������g���擾����
			C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_element->begin(), p_element->end(), true);
			if (p_flag)	{
				S_tnm_save_header header = {0};
				if (tnm_load_save_header(save_no, &header))	{
					for (int i = 0; i < flag_cnt; i++)		// �t���O�����Ɏ擾����
						p_flag->set_value(flag_index + i, header.flag[i]);
				}
			}
			break;
		}
		case ELM_SYSCOM_SET_SAVE_VALUE:
		{
			int save_no = p_ai->al_begin[0].Int;					// �Z�[�u�f�[�^�ԍ�
			S_element* p_element = &p_ai->al_begin[1].element;		// �R�s�[��̃t���O�G�������g
			int flag_index = p_ai->al_begin[2].Int;				// �t���O�J�n�ʒu
			int flag_cnt = p_ai->al_begin[3].Int;					// �t���O��
			if (flag_cnt > TNM_SAVE_FLAG_MAX_CNT)
				flag_cnt = TNM_SAVE_FLAG_MAX_CNT;

			// �R�s�[���̃t���O�G�������g���擾����
			C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_element->begin(), p_element->end(), true);
			if (p_flag)	{
				S_tnm_save_header header = {0};
				if (tnm_load_save_header(save_no, &header))	{
					for (int i = 0; i < flag_cnt; i++)		// �t���O�����ɐݒ肷��
						header.flag[i] = p_flag->get_value(flag_index + i);
					tnm_save_save_header(save_no, &header);	// �f�[�^�������߂�
				}
			}
			break;
		}
		case ELM_SYSCOM_GET_SAVE_APPEND_DIR:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.append_dir);
			break;
		}
		case ELM_SYSCOM_GET_SAVE_APPEND_NAME:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.append_name);
			break;
		}



		case ELM_SYSCOM_GET_QUICK_SAVE_EXIST:
		{
			bool exist = tnm_check_save_file(Gp_ini->save_cnt + p_ai->al_begin[0].Int);
			tnm_stack_push_int(exist == 1 ? 1 : 0);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_YEAR:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.year);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_MONTH:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.month);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_DAY:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.day);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_WEEKDAY:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.weekday);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_HOUR:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.hour);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_MINUTE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.minute);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_SECOND:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.second);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_MILLISECOND:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_int(header.millisecond);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_TITLE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.title);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_MESSAGE:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.message);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_FULL_MESSAGE:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.full_message);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_COMMENT:
		{
			S_tnm_save_header header = {0};
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.comment);
			break;
		}
		case ELM_SYSCOM_SET_QUICK_SAVE_COMMENT:
		{
			S_tnm_save_header header = {0};
			if (tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header))	{
				ZeroMemory(header.comment, sizeof(header.comment));		// �R�����g����������
				_tcsncpy_s(header.comment, TNM_SAVE_COMMENT_MAX_LEN, p_ai->al_begin[1].str.c_str(), TNM_SAVE_COMMENT_MAX_LEN - 1);
				tnm_save_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			}
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_VALUE:
		{
			int save_no = Gp_ini->save_cnt + p_ai->al_begin[0].Int;	// �Z�[�u�f�[�^�ԍ�
			S_element* p_element = &p_ai->al_begin[1].element;		// �R�s�[��̃t���O�G�������g
			int flag_index = p_ai->al_begin[2].Int;					// �t���O�J�n�ʒu
			int flag_cnt = p_ai->al_begin[3].Int;					// �t���O��
			if (flag_cnt > TNM_SAVE_FLAG_MAX_CNT)
				flag_cnt = TNM_SAVE_FLAG_MAX_CNT;

			// �R�s�[��̃t���O�G�������g���擾����
			C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_element->begin(), p_element->end(), true);
			if (p_flag)	{
				S_tnm_save_header header = {0};
				if (tnm_load_save_header(save_no, &header))	{
					for (int i = 0; i < flag_cnt; i++)			// �t���O�����Ɏ擾����
						p_flag->set_value(flag_index + i, header.flag[i]);
				}
			}
			break;
		}
		case ELM_SYSCOM_SET_QUICK_SAVE_VALUE:
		{
			int save_no = Gp_ini->save_cnt + p_ai->al_begin[0].Int;	// �Z�[�u�f�[�^�ԍ�
			S_element* p_element = &p_ai->al_begin[1].element;		// �R�s�[��̃t���O�G�������g
			int flag_index = p_ai->al_begin[2].Int;					// �t���O�J�n�ʒu
			int flag_cnt = p_ai->al_begin[3].Int;					// �t���O��
			if (flag_cnt > TNM_SAVE_FLAG_MAX_CNT)
				flag_cnt = TNM_SAVE_FLAG_MAX_CNT;

			// �R�s�[���̃t���O�G�������g���擾����
			C_elm_int_list* p_flag = (C_elm_int_list *)tnm_get_element_ptr(p_element->begin(), p_element->end(), true);
			if (p_flag)	{
				S_tnm_save_header header = {0};
				if (tnm_load_save_header(save_no, &header))	{
					for (int i = 0; i < flag_cnt; i++)			// �t���O�����ɐݒ肷��
						header.flag[i] = p_flag->get_value(flag_index + i);
					tnm_save_save_header(save_no, &header);		// �f�[�^�������߂�
				}
			}
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_APPEND_DIR:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.append_dir);
			break;
		}
		case ELM_SYSCOM_GET_QUICK_SAVE_APPEND_NAME:
		{
			S_tnm_save_header header = { 0 };
			tnm_load_save_header(Gp_ini->save_cnt + p_ai->al_begin[0].Int, &header);
			tnm_stack_push_str(header.append_name);
			break;
		}

		case ELM_SYSCOM_GET_END_SAVE_EXIST:
		{
			bool exist = tnm_check_save_file(Gp_ini->save_cnt + Gp_ini->quick_save_cnt + p_ai->al_begin[0].Int);
			tnm_stack_push_int(exist ? 1 : 0);
			break;
		}

		case ELM_SYSCOM_COPY_SAVE:
			tnm_stack_push_int(tnm_copy_save_file(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_CHANGE_SAVE:
			tnm_stack_push_int(tnm_change_save_file(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_DELETE_SAVE:
			tnm_stack_push_int(tnm_delete_save_file(p_ai->al_begin[0].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_COPY_QUICK_SAVE:
			tnm_stack_push_int(tnm_copy_save_file(Gp_ini->save_cnt + p_ai->al_begin[0].Int, Gp_ini->save_cnt + p_ai->al_begin[1].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_CHANGE_QUICK_SAVE:
			tnm_stack_push_int(tnm_change_save_file(Gp_ini->save_cnt + p_ai->al_begin[0].Int, Gp_ini->save_cnt + p_ai->al_begin[1].Int) ? 1 : 0);
			break;
		case ELM_SYSCOM_DELETE_QUICK_SAVE:
			tnm_stack_push_int(tnm_delete_save_file(Gp_ini->save_cnt + p_ai->al_begin[0].Int) ? 1 : 0);
			break;

	// ���ݒ�
		case ELM_SYSCOM_CALL_CONFIG_MENU:
			tnm_syscom_open_config();
			break;
		case ELM_SYSCOM_CALL_CONFIG_WINDOW_MODE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_screen.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_VOLUME_MENU:
			Gp_dlg_mng->cfg_wnd_solo_volume.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_BGMFADE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_bgmfade.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_KOEMODE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_koemode.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_CHARAKOE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_chrkoe.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_JITAN_MENU:
			Gp_dlg_mng->cfg_wnd_solo_jitan.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_MESSAGE_SPEED_MENU:
			Gp_dlg_mng->cfg_wnd_solo_msgspd.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_FILTER_COLOR_MENU:
			Gp_dlg_mng->cfg_wnd_solo_mwndbk.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_AUTO_MODE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_automode.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_FONT_MENU:
			Gp_dlg_mng->cfg_wnd_solo_font.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_SYSTEM_MENU:
			Gp_dlg_mng->cfg_wnd_solo_system.open();
			break;
		case ELM_SYSCOM_CALL_CONFIG_MOVIE_MENU:
			Gp_dlg_mng->cfg_wnd_solo_movtype.open();
			break;
	// ���ݒ�F��ʃ��[�h
		case ELM_SYSCOM_SET_WINDOW_MODE:
			if (false);
			else if (p_ai->al_begin[0].Int == TNM_SCREEN_SIZE_MODE_WINDOW)	{
				tnm_change_screen_size_mode_window(false);
			}
			else if (p_ai->al_begin[0].Int == TNM_SCREEN_SIZE_MODE_FULL)	{
				tnm_change_screen_size_mode_fullscreen(false, true);
			}
			break;
		case ELM_SYSCOM_SET_WINDOW_MODE_DEFAULT:
			if (false);
			else if (Gp_ini->config.screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)	{
				tnm_change_screen_size_mode_window(Gp_ini->config.screen_size_scale, false);	// Gp_ini
			}
			else if (Gp_ini->config.screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
				tnm_change_screen_size_mode_fullscreen(false, true);
			}
			break;
		case ELM_SYSCOM_GET_WINDOW_MODE:
			tnm_stack_push_int(Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_WINDOW_MODE_SIZE:
			tnm_change_screen_size_mode_window(C_size(p_ai->al_begin[0].Int, p_ai->al_begin[0].Int), false);	// �����I�ɃE�B���h�E���[�h�ɕύX����I
			break;
		case ELM_SYSCOM_SET_WINDOW_MODE_SIZE_DEFAULT:
			tnm_change_screen_size_mode_window(Gp_ini->config.screen_size_scale, false);		// �����I�ɃE�B���h�E���[�h�ɕύX����I
			break;
		case ELM_SYSCOM_GET_WINDOW_MODE_SIZE:
			tnm_stack_push_int(Gp_config->screen_size_scale.cx);
			break;
		case ELM_SYSCOM_CHECK_WINDOW_MODE_SIZE_ENABLE:
			tnm_stack_push_int(tnm_get_window_size_from_size_scale(C_size(p_ai->al_begin[0].Int, p_ai->al_begin[0].Int)) ? 1 : 0);
			break;

	// ���ݒ�F���ʐݒ�
		case ELM_SYSCOM_SET_ALL_VOLUME:
			Gp_config->all_sound_user_volume = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_BGM_VOLUME:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_KOE_VOLUME:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_PCM_VOLUME:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_SE_VOLUME:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_MOV_VOLUME:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_SOUND_VOLUME:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				Gp_config->sound_user_volume[p_ai->al_begin[0].Int] = limit(0, p_ai->al_begin[1].Int, 255);
			}
			break;
		case ELM_SYSCOM_SET_ALL_ONOFF:
			Gp_config->play_all_sound_check = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_BGM_ONOFF:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_KOE_ONOFF:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_PCM_ONOFF:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SE_ONOFF:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_MOV_ONOFF:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SOUND_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				Gp_config->play_sound_check[p_ai->al_begin[0].Int] = p_ai->al_begin[1].Int ? true : false;
			}
			break;
		case ELM_SYSCOM_SET_ALL_VOLUME_DEFAULT:
			Gp_config->all_sound_user_volume = Gp_ini->config.all_user_volume;
			break;
		case ELM_SYSCOM_SET_BGM_VOLUME_DEFAULT:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] = Gp_ini->config.bgm_user_volume;
			break;
		case ELM_SYSCOM_SET_KOE_VOLUME_DEFAULT:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] = Gp_ini->config.koe_user_volume;
			break;
		case ELM_SYSCOM_SET_PCM_VOLUME_DEFAULT:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] = Gp_ini->config.pcm_user_volume;
			break;
		case ELM_SYSCOM_SET_SE_VOLUME_DEFAULT:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] = Gp_ini->config.se_user_volume;
			break;
		case ELM_SYSCOM_SET_MOV_VOLUME_DEFAULT:
			Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] = Gp_ini->config.mov_user_volume;
			break;
		case ELM_SYSCOM_SET_SOUND_VOLUME_DEFAULT:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				Gp_config->sound_user_volume[p_ai->al_begin[0].Int] = 255;	// ��ini �܂��Ȃ�
			}
			break;
		case ELM_SYSCOM_SET_ALL_ONOFF_DEFAULT:
			Gp_config->play_all_sound_check = Gp_ini->config.play_all_check;
			break;
		case ELM_SYSCOM_SET_BGM_ONOFF_DEFAULT:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] = Gp_ini->config.play_bgm_check;
			break;
		case ELM_SYSCOM_SET_KOE_ONOFF_DEFAULT:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] = Gp_ini->config.play_koe_check;
			break;
		case ELM_SYSCOM_SET_PCM_ONOFF_DEFAULT:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] = Gp_ini->config.play_pcm_check;
			break;
		case ELM_SYSCOM_SET_SE_ONOFF_DEFAULT:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] = Gp_ini->config.play_se_check;
			break;
		case ELM_SYSCOM_SET_MOV_ONOFF_DEFAULT:
			Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] = Gp_ini->config.play_mov_check;
			break;
		case ELM_SYSCOM_SET_SOUND_ONOFF_DEFAULT:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				Gp_config->play_sound_check[p_ai->al_begin[0].Int] = true;	// ��ini �܂��Ȃ�
			}
			break;
		case ELM_SYSCOM_GET_ALL_VOLUME:
			tnm_stack_push_int(Gp_config->all_sound_user_volume);
			break;
		case ELM_SYSCOM_GET_BGM_VOLUME:
			tnm_stack_push_int(Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM]);
			break;
		case ELM_SYSCOM_GET_KOE_VOLUME:
			tnm_stack_push_int(Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE]);
			break;
		case ELM_SYSCOM_GET_PCM_VOLUME:
			tnm_stack_push_int(Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM]);
			break;
		case ELM_SYSCOM_GET_SE_VOLUME:
			tnm_stack_push_int(Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE]);
			break;
		case ELM_SYSCOM_GET_MOV_VOLUME:
			tnm_stack_push_int(Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV]);
			break;
		case ELM_SYSCOM_GET_SOUND_VOLUME:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				tnm_stack_push_int(Gp_config->sound_user_volume[p_ai->al_begin[0].Int]);
			}
			else	{
				tnm_stack_push_int(255);
			}
			break;
		case ELM_SYSCOM_GET_ALL_ONOFF:
			tnm_stack_push_int(Gp_config->play_all_sound_check ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_BGM_ONOFF:
			tnm_stack_push_int(Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_KOE_ONOFF:
			tnm_stack_push_int(Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_PCM_ONOFF:
			tnm_stack_push_int(Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_SE_ONOFF:
			tnm_stack_push_int(Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_MOV_ONOFF:
			tnm_stack_push_int(Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] ? 1 : 0);
			break;
		case ELM_SYSCOM_GET_SOUND_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < TNM_VOLUME_TYPE_MAX)	{
				tnm_stack_push_int(Gp_config->play_sound_check[p_ai->al_begin[0].Int] ? 1 : 0);
			}
			else	{
				tnm_stack_push_int(1);
			}
			break;
	// ���ݒ�F�a�f�l�t�F�[�h
		case ELM_SYSCOM_SET_BGMFADE_VOLUME:
			Gp_config->bgmfade_volume = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_BGMFADE_ONOFF:
			Gp_config->bgmfade_use_check = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_BGMFADE_VOLUME_DEFAULT:
			Gp_config->bgmfade_volume = Gp_ini->config.bgmfade_volume;
			break;
		case ELM_SYSCOM_SET_BGMFADE_ONOFF_DEFAULT:
			Gp_config->bgmfade_use_check = Gp_ini->config.bgmfade_use_check;
			break;
		case ELM_SYSCOM_GET_BGMFADE_VOLUME:
			tnm_stack_push_int(Gp_config->bgmfade_volume);
			break;
		case ELM_SYSCOM_GET_BGMFADE_ONOFF:
			tnm_stack_push_int(Gp_config->bgmfade_use_check ? 1 : 0);
			break;
	// ���ݒ�F�����ݒ�
		case ELM_SYSCOM_SET_KOEMODE:
			Gp_config->koe_mode = 
				(p_ai->al_begin[0].Int == TNM_KOE_MODE_NORMAL || p_ai->al_begin[0].Int == TNM_KOE_MODE_TEXTONLY || p_ai->al_begin[0].Int == TNM_KOE_MODE_KOEONLY) ? 
				p_ai->al_begin[0].Int : TNM_KOE_MODE_NORMAL;
			break;
		case ELM_SYSCOM_SET_KOEMODE_DEFAULT:
			Gp_config->koe_mode = Gp_ini->config.koe_mode;
			break;
		case ELM_SYSCOM_GET_KOEMODE:
			tnm_stack_push_int(Gp_config->koe_mode);
			break;
	// ���ݒ�F�L�����N�^����
		case ELM_SYSCOM_SET_CHARAKOE_ONOFF:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size())	{
				Gp_config->chrkoe[regist_no].onoff = p_ai->al_begin[1].Int ? true : false;
			}
			break;
		}
		case ELM_SYSCOM_SET_CHARAKOE_ONOFF_DEFAULT:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size() && regist_no < Gp_ini->config.chrkoe_cnt)	{
				Gp_config->chrkoe[regist_no].onoff = Gp_ini->config.chrkoe[regist_no].onoff;
			}
			break;
		}
		case ELM_SYSCOM_GET_CHARAKOE_ONOFF:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size() && regist_no < Gp_ini->config.chrkoe_cnt)	{
				tnm_stack_push_int(Gp_config->chrkoe[regist_no].onoff ? 1 : 0);
			}
			else	{
				tnm_stack_push_int(0);
			}
			break;
		}
		case ELM_SYSCOM_SET_CHARAKOE_VOLUME:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size())	{
				Gp_config->chrkoe[regist_no].volume = limit(0, p_ai->al_begin[1].Int, 255);
			}
			break;
		}
		case ELM_SYSCOM_SET_CHARAKOE_VOLUME_DEFAULT:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size() && regist_no < Gp_ini->config.chrkoe_cnt)	{
				Gp_config->chrkoe[regist_no].volume = Gp_ini->config.chrkoe[regist_no].volume;
			}
			break;
		}
		case ELM_SYSCOM_GET_CHARAKOE_VOLUME:
		{
			int regist_no = p_ai->al_begin[0].Int;
			if (0 <= regist_no && regist_no < (int)Gp_config->chrkoe.size() && regist_no < Gp_ini->config.chrkoe_cnt)	{
				tnm_stack_push_int(Gp_config->chrkoe[regist_no].volume);
			}
			else	{
				tnm_stack_push_int(0);
			}
			break;
		}
	// ���ݒ�F���Z�Đ�
		case ELM_SYSCOM_SET_JITAN_NORMAL_ONOFF:
			Gp_config->jitan_normal_onoff = p_ai->al_begin[0].Int ? 1 : 0;
			break;
		case ELM_SYSCOM_SET_JITAN_NORMAL_ONOFF_DEFAULT:
			Gp_config->jitan_normal_onoff = Gp_ini->config.jitan_normal_onoff;
			break;
		case ELM_SYSCOM_GET_JITAN_NORMAL_ONOFF:
			tnm_stack_push_int(Gp_config->jitan_normal_onoff);
			break;
		case ELM_SYSCOM_SET_JITAN_AUTO_MODE_ONOFF:
			Gp_config->jitan_auto_mode_onoff = p_ai->al_begin[0].Int ? 1 : 0;
			break;
		case ELM_SYSCOM_SET_JITAN_AUTO_MODE_ONOFF_DEFAULT:
			Gp_config->jitan_auto_mode_onoff = Gp_ini->config.jitan_auto_mode_onoff;
			break;
		case ELM_SYSCOM_GET_JITAN_AUTO_MODE_ONOFF:
			tnm_stack_push_int(Gp_config->jitan_auto_mode_onoff);
			break;
		case ELM_SYSCOM_SET_JITAN_KOE_REPLAY_ONOFF:
			Gp_config->jitan_msgbk_onoff = p_ai->al_begin[0].Int ? 1 : 0;
			break;
		case ELM_SYSCOM_SET_JITAN_KOE_REPLAY_ONOFF_DEFAULT:
			Gp_config->jitan_msgbk_onoff = Gp_ini->config.jitan_msgbk_onoff;
			break;
		case ELM_SYSCOM_GET_JITAN_KOE_REPLAY_ONOFF:
			tnm_stack_push_int(Gp_config->jitan_msgbk_onoff);
			break;
		case ELM_SYSCOM_SET_JITAN_SPEED:
			Gp_config->jitan_speed = limit(TNM_JITAN_SPEED_MIN, p_ai->al_begin[0].Int, TNM_JITAN_SPEED_MAX);
			break;
		case ELM_SYSCOM_SET_JITAN_SPEED_DEFAULT:
			Gp_config->jitan_speed = Gp_ini->config.jitan_speed;
			break;
		case ELM_SYSCOM_GET_JITAN_SPEED:
			tnm_stack_push_int(Gp_config->jitan_speed);
			break;
	// ���ݒ�F���b�Z�[�W���x
		case ELM_SYSCOM_SET_MESSAGE_SPEED:
			Gp_config->message_speed = limit(0, p_ai->al_begin[0].Int, TNM_MESSAGE_SPEED_MAX);
			break;
		case ELM_SYSCOM_SET_MESSAGE_SPEED_DEFAULT:
			Gp_config->message_speed = Gp_ini->config.message_speed;
			break;
		case ELM_SYSCOM_GET_MESSAGE_SPEED:
			tnm_stack_push_int(Gp_config->message_speed);
			break;
		case ELM_SYSCOM_SET_MESSAGE_NOWAIT:
			Gp_config->message_speed_nowait = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_MESSAGE_NOWAIT_DEFAULT:
			Gp_config->message_speed_nowait = Gp_ini->config.message_speed_nowait;
			break;
		case ELM_SYSCOM_GET_MESSAGE_NOWAIT:
			tnm_stack_push_int(Gp_config->message_speed_nowait ? 1 : 0);
			break;
	// ���ݒ�F�I�[�g���[�h
		case ELM_SYSCOM_SET_AUTO_MODE_MOJI_WAIT:
			Gp_config->auto_mode_moji_wait = limit(TNM_AUTO_MODE_MOJI_WAIT_MIN, p_ai->al_begin[0].Int, TNM_AUTO_MODE_MOJI_WAIT_MAX);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_MOJI_WAIT_DEFAULT:
			Gp_config->auto_mode_moji_wait = limit(TNM_AUTO_MODE_MOJI_WAIT_MIN, Gp_ini->config.auto_mode_moji_wait, TNM_AUTO_MODE_MOJI_WAIT_MAX);
			break;
		case ELM_SYSCOM_GET_AUTO_MODE_MOJI_WAIT:
			tnm_stack_push_int(Gp_config->auto_mode_moji_wait);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_MIN_WAIT:
			Gp_config->auto_mode_min_wait = limit(TNM_AUTO_MODE_MIN_WAIT_MIN, p_ai->al_begin[0].Int, TNM_AUTO_MODE_MIN_WAIT_MAX);
			break;
		case ELM_SYSCOM_SET_AUTO_MODE_MIN_WAIT_DEFAULT:
			Gp_config->auto_mode_min_wait = limit(TNM_AUTO_MODE_MIN_WAIT_MIN, Gp_ini->config.auto_mode_min_wait, TNM_AUTO_MODE_MIN_WAIT_MAX);
			break;
		case ELM_SYSCOM_GET_AUTO_MODE_MIN_WAIT:
			tnm_stack_push_int(Gp_config->auto_mode_min_wait);
			break;
	// ���ݒ�F�����Ń}�E�X�J�[�\�����B��
		case ELM_SYSCOM_SET_MOUSE_CURSOR_HIDE_ONOFF:
			Gp_config->mouse_cursor_hide_onoff = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_MOUSE_CURSOR_HIDE_ONOFF_DEFAULT:
			Gp_config->mouse_cursor_hide_onoff = Gp_ini->config.mouse_cursor_hide_onoff;
			break;
		case ELM_SYSCOM_GET_MOUSE_CURSOR_HIDE_ONOFF:
			tnm_stack_push_int(Gp_config->mouse_cursor_hide_onoff ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_MOUSE_CURSOR_HIDE_TIME:
			Gp_config->mouse_cursor_hide_time = p_ai->al_begin[0].Int;
			break;
		case ELM_SYSCOM_SET_MOUSE_CURSOR_HIDE_TIME_DEFAULT:
			Gp_config->mouse_cursor_hide_time = Gp_ini->config.mouse_cursor_hide_time;
			break;
		case ELM_SYSCOM_GET_MOUSE_CURSOR_HIDE_TIME:
			tnm_stack_push_int(Gp_config->mouse_cursor_hide_time);
			break;

	// ���ݒ�F�E�B���h�E�w�i�F
		case ELM_SYSCOM_SET_FILTER_COLOR_R:
			Gp_config->filter_color.r = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_G:
			Gp_config->filter_color.g = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_B:
			Gp_config->filter_color.b = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_A:
			Gp_config->filter_color.a = limit(0, p_ai->al_begin[0].Int, 255);
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_R_DEFAULT:
			Gp_config->filter_color.r = Gp_ini->config.filter_color.r;
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_G_DEFAULT:
			Gp_config->filter_color.g = Gp_ini->config.filter_color.g;
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_B_DEFAULT:
			Gp_config->filter_color.b = Gp_ini->config.filter_color.b;
			break;
		case ELM_SYSCOM_SET_FILTER_COLOR_A_DEFAULT:
			Gp_config->filter_color.a = Gp_ini->config.filter_color.a;
			break;
		case ELM_SYSCOM_GET_FILTER_COLOR_R:
			tnm_stack_push_int(Gp_config->filter_color.r);
			break;
		case ELM_SYSCOM_GET_FILTER_COLOR_G:
			tnm_stack_push_int(Gp_config->filter_color.g);
			break;
		case ELM_SYSCOM_GET_FILTER_COLOR_B:
			tnm_stack_push_int(Gp_config->filter_color.b);
			break;
		case ELM_SYSCOM_GET_FILTER_COLOR_A:
			tnm_stack_push_int(Gp_config->filter_color.a);
			break;

	// �����ߒǉ��F���ݒ�F���̑��̐ݒ�F���͂�F��������

	// ���ݒ�F���̑��̐ݒ�F�I�u�W�F�N�g�̕\��
		case ELM_SYSCOM_SET_OBJECT_DISP_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->object_disp_flag.size())	{
				Gp_config->object_disp_flag[p_ai->al_begin[0].Int].value = p_ai->al_begin[1].Int ? true : false;
			}
			break;
		case ELM_SYSCOM_SET_OBJECT_DISP_ONOFF_DEFAULT:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->object_disp_flag.size())	{
				Gp_config->object_disp_flag[p_ai->al_begin[0].Int].value = Gp_ini->config.global_extra_switch[p_ai->al_begin[0].Int].onoff;
			}
			break;
		case ELM_SYSCOM_GET_OBJECT_DISP_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->object_disp_flag.size())	{
				tnm_stack_push_int(Gp_config->object_disp_flag[p_ai->al_begin[0].Int].value ? 1 : 0);
			}
			else	{
				tnm_stack_push_int(0);
			}
			break;

	// ���ݒ�F���̑��̐ݒ�F�O���[�o���ėp�X�C�b�`
		case ELM_SYSCOM_SET_GLOBAL_EXTRA_SWITCH_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_switch_flag.size())	{
				Gp_config->global_extra_switch_flag[p_ai->al_begin[0].Int].value = p_ai->al_begin[1].Int ? true : false;
			}
			break;
		case ELM_SYSCOM_SET_GLOBAL_EXTRA_SWITCH_ONOFF_DEFAULT:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_switch_flag.size())	{
				Gp_config->global_extra_switch_flag[p_ai->al_begin[0].Int].value = Gp_ini->config.global_extra_switch[p_ai->al_begin[0].Int].onoff;
			}
			break;
		case ELM_SYSCOM_GET_GLOBAL_EXTRA_SWITCH_ONOFF:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_switch_flag.size())	{
				tnm_stack_push_int(Gp_config->global_extra_switch_flag[p_ai->al_begin[0].Int].value ? 1 : 0);
			}
			else	{
				tnm_stack_push_int(0);
			}
			break;

	// ���ݒ�F���̑��̐ݒ�F�O���[�o���ėp���[�h
		case ELM_SYSCOM_SET_GLOBAL_EXTRA_MODE_VALUE:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_mode_flag.size())	{
				Gp_config->global_extra_mode_flag[p_ai->al_begin[0].Int] = p_ai->al_begin[1].Int;
			}
			break;
		case ELM_SYSCOM_SET_GLOBAL_EXTRA_MODE_VALUE_DEFAULT:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_mode_flag.size())	{
				Gp_config->global_extra_mode_flag[p_ai->al_begin[0].Int] = Gp_ini->config.global_extra_mode[p_ai->al_begin[0].Int].mode;
			}
			break;
		case ELM_SYSCOM_GET_GLOBAL_EXTRA_MODE_VALUE:
			if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < (int)Gp_config->global_extra_mode_flag.size())	{
				tnm_stack_push_int(Gp_config->global_extra_mode_flag[p_ai->al_begin[0].Int]);
			}
			else	{
				tnm_stack_push_int(0);
			}
			break;

	// ���ݒ�F�V�X�e���ݒ�
		case ELM_SYSCOM_SET_SAVELOAD_ALERT_ONOFF:
			Gp_config->system.saveload_alert_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SAVELOAD_ALERT_ONOFF_DEFAULT:
			Gp_config->system.saveload_alert_flag = Gp_ini->config.system.saveload_alert_onoff;
			break;
		case ELM_SYSCOM_GET_SAVELOAD_ALERT_ONOFF:
			tnm_stack_push_int(Gp_config->system.saveload_alert_flag ? 1 : 0);
			break;

		// �������������i�Z�[�u���[�h�̃_�u���N���b�N�̍��ڑ��얽�߁j
//		case ELM_SYSCOM_SET_SAVELOAD_DBLCLICK_ONOFF:
//			Gp_config->system.saveload_dblclick_flag = p_ai->al_begin[0].Int ? true : false;
//			break;
//		case ELM_SYSCOM_SET_SAVELOAD_DBLCLICK_ONOFF_DEFAULT:
//			Gp_config->system.saveload_dblclick_flag = Gp_ini->config.system.saveload_dblclick_onoff;
//			break;
//		case ELM_SYSCOM_GET_SAVELOAD_DBLCLICK_ONOFF:
//			stack_push_int(Gp_config->system.saveload_dblclick_flag ? 1 : 0);
//			break;

		case ELM_SYSCOM_SET_SLEEP_ONOFF:
			Gp_config->system.sleep_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SLEEP_ONOFF_DEFAULT:
			Gp_config->system.sleep_flag = Gp_ini->config.system.sleep.onoff;
			break;
		case ELM_SYSCOM_GET_SLEEP_ONOFF:
			tnm_stack_push_int(Gp_config->system.sleep_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_NO_WIPE_ANIME_ONOFF:
			Gp_config->system.no_wipe_anime_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_NO_WIPE_ANIME_ONOFF_DEFAULT:
			Gp_config->system.no_wipe_anime_flag = Gp_ini->config.system.no_wipe_anime.onoff;
			break;
		case ELM_SYSCOM_GET_NO_WIPE_ANIME_ONOFF:
			tnm_stack_push_int(Gp_config->system.no_wipe_anime_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_SKIP_WIPE_ANIME_ONOFF:
			Gp_config->system.skip_wipe_anime_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SKIP_WIPE_ANIME_ONOFF_DEFAULT:
			Gp_config->system.skip_wipe_anime_flag = Gp_ini->config.system.skip_wipe_anime.onoff;
			break;
		case ELM_SYSCOM_GET_SKIP_WIPE_ANIME_ONOFF:
			tnm_stack_push_int(Gp_config->system.skip_wipe_anime_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_NO_MWND_ANIME_ONOFF:
			Gp_config->system.no_mwnd_anime_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_NO_MWND_ANIME_ONOFF_DEFAULT:
			Gp_config->system.no_mwnd_anime_flag = Gp_ini->config.system.no_mwnd_anime.onoff;
			break;
		case ELM_SYSCOM_GET_NO_MWND_ANIME_ONOFF:
			tnm_stack_push_int(Gp_config->system.no_mwnd_anime_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_WHEEL_NEXT_MESSAGE_ONOFF:
			Gp_config->system.wheel_next_message_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_WHEEL_NEXT_MESSAGE_ONOFF_DEFAULT:
			Gp_config->system.wheel_next_message_flag = Gp_ini->config.system.wheel_next_message.onoff;
			break;
		case ELM_SYSCOM_GET_WHEEL_NEXT_MESSAGE_ONOFF:
			tnm_stack_push_int(Gp_config->system.wheel_next_message_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_KOE_DONT_STOP_ONOFF:
			Gp_config->system.koe_dont_stop_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_KOE_DONT_STOP_ONOFF_DEFAULT:
			Gp_config->system.koe_dont_stop_flag = Gp_ini->config.system.koe_dont_stop.onoff;
			break;
		case ELM_SYSCOM_GET_KOE_DONT_STOP_ONOFF:
			tnm_stack_push_int(Gp_config->system.koe_dont_stop_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_SKIP_UNREAD_MESSAGE_ONOFF:
			Gp_config->system.skip_unread_message_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_SKIP_UNREAD_MESSAGE_ONOFF_DEFAULT:
			Gp_config->system.skip_unread_message_flag = Gp_ini->config.system.skip_unread_message.onoff;
			break;
		case ELM_SYSCOM_GET_SKIP_UNREAD_MESSAGE_ONOFF:
			tnm_stack_push_int(Gp_config->system.skip_unread_message_flag ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_PLAY_SILENT_SOUND_ONOFF:
			Gp_system_config->ds_play_silent_sound_flag = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_PLAY_SILENT_SOUND_ONOFF_DEFAULT:
			Gp_system_config->ds_play_silent_sound_flag = false;	// �f�t�H���g�Ffalse �Œ�
			break;
		case ELM_SYSCOM_GET_PLAY_SILENT_SOUND_ONOFF:
			tnm_stack_push_int(Gp_system_config->ds_play_silent_sound_flag ? 1 : 0);
			break;

	// �t�H���g
		case ELM_SYSCOM_SET_FONT_NAME:
			Gp_config->font_name = p_ai->al_begin[0].str;
			tnm_update_font(false);
			break;
		case ELM_SYSCOM_SET_FONT_NAME_DEFAULT:
			Gp_config->font_name = Gp_ini->config.font_name;
			tnm_update_font(false);
			break;
		case ELM_SYSCOM_GET_FONT_NAME:
			tnm_stack_push_str(Gp_config->font_name);
			break;
		case ELM_SYSCOM_IS_FONT_EXIST:
		{
			TSTR font_name = p_ai->al_begin[0].str;
			ARRAY<ENUMLOGFONT>::iterator itr = std::find_if(Gp_font_name_list->m_font_list.begin(), Gp_font_name_list->m_font_list.end(), C_font_name_functor(font_name));
			if (itr == Gp_font_name_list->m_font_list.end())	{
				tnm_stack_push_int(0);	// ������Ȃ�����
			}
			else	{
				tnm_stack_push_int(1);	// ��������
			}
			break;
		}
		case ELM_SYSCOM_SET_FONT_BOLD:
			Gp_config->font_futoku = p_ai->al_begin[0].Int ? true : false;
			break;
		case ELM_SYSCOM_SET_FONT_BOLD_DEFAULT:
			Gp_config->font_futoku = Gp_ini->config.font_futoku;
			break;
		case ELM_SYSCOM_GET_FONT_BOLD:
			tnm_stack_push_int(Gp_config->font_futoku ? 1 : 0);
			break;
		case ELM_SYSCOM_SET_FONT_DECORATION:
			Gp_config->font_shadow = p_ai->al_begin[0].Int;
			break;
		case ELM_SYSCOM_SET_FONT_DECORATION_DEFAULT:
			Gp_config->font_shadow = Gp_ini->config.font_shadow;
			break;
		case ELM_SYSCOM_GET_FONT_DECORATION:
			tnm_stack_push_int(Gp_config->font_shadow);
			break;

	// �L���v�`���[�i�L���v�`���[������ĉ摜�ɏo�͂���n�j
		case ELM_SYSCOM_CREATE_CAPTURE_BUFFER:
			tnm_syscom_create_capture_buffer(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
			break;

		case ELM_SYSCOM_DESTROY_CAPTURE_BUFFER:
			tnm_syscom_destroy_capture_buffer();
			break;

		case ELM_SYSCOM_CAPTURE_TO_CAPTURE_BUFFER:
			tnm_syscom_buffer_capture(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, _T(""));	// �t�@�C�������w�肵�Ȃ�
			break;

		case ELM_SYSCOM_SAVE_CAPTURE_BUFFER_TO_FILE:
		{
			bool dialog_flag = false;
			int flag_index = 0;
			int flag_cnt = 0;
			int str_flag_index = 0;
			int str_flag_cnt = 0;
			S_element flag_element;
			S_element str_flag_element;
			TSTR file_name = p_ai->al_begin[0].str;
			TSTR extension = p_ai->al_begin[1].str;
			TSTR dialog_title = _T("�ۑ���̑I��");

			// ���O�t������
			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
				case 0:		dialog_flag = arg->Int ? true : false;		break;
				case 1:		dialog_title = arg->str;					break;
				case 2:		flag_element = arg->element;				break;
				case 3:		flag_index = arg->Int;						break;
				case 4:		flag_cnt = arg->Int;						break;
				case 5:		str_flag_element = arg->element;			break;
				case 6:		str_flag_index = arg->Int;					break;
				case 7:		str_flag_cnt = arg->Int;					break;
				}
			}

			tnm_stack_push_int(tnm_syscom_save_capture_buffer_to_file(
				file_name, extension, dialog_flag, dialog_title
				, &flag_element, flag_index, flag_cnt
				, &str_flag_element, str_flag_index, str_flag_cnt
				) ? 1 : 0);
			break;
		}

		case ELM_SYSCOM_LOAD_FLAG_FROM_CAPTURE_FILE:
		{
			bool dialog_flag = false;
			int flag_index = 0;
			int flag_cnt = 0;
			int str_flag_index = 0;
			int str_flag_cnt = 0;
			S_element flag_element;
			S_element str_flag_element;
			TSTR file_name = p_ai->al_begin[0].str;
			TSTR extension = p_ai->al_begin[1].str;
			TSTR dialog_title = _T("�t�@�C���̑I��");

			// ���O�t������
			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
				case 0:		dialog_flag = arg->Int ? true : false;		break;
				case 1:		dialog_title = arg->str;					break;
				case 2:		flag_element = arg->element;				break;
				case 3:		flag_index = arg->Int;						break;
				case 4:		flag_cnt = arg->Int;						break;
				case 5:		str_flag_element = arg->element;			break;
				case 6:		str_flag_index = arg->Int;					break;
				case 7:		str_flag_cnt = arg->Int;					break;
				}
			}

			tnm_stack_push_int(tnm_syscom_load_flag_from_capture_file(
				file_name, extension, dialog_flag, dialog_title
				, &flag_element, flag_index, flag_cnt
				, &str_flag_element, str_flag_index, str_flag_cnt
				) ? 1 : 0);
			break;
		}

		case ELM_SYSCOM_CAPTURE_AND_SAVE_BUFFER_TO_PNG:
			tnm_syscom_buffer_capture(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].str);
			break;

		case ELM_SYSCOM_OPEN_TWEET_DIALOG:
			tnm_twitter_start();
			break;

		case ELM_SYSCOM_SET_RETURN_SCENE_ONCE:
		{
			if (p_ai->al_id == 0)
			{
				Gp_global->return_scene_name = p_ai->al_begin[0].str;
				Gp_global->return_scene_z_no = 0;
			}
			else
			{
				Gp_global->return_scene_name = p_ai->al_begin[0].str;
				Gp_global->return_scene_z_no = p_ai->al_begin[1].Int;
			}
			break;
		}

		case ELM_SYSCOM_GET_SYSTEM_EXTRA_INT_VALUE:
			if (!(0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < Gp_ini->system_int_value_cnt))
			{
				tnm_stack_push_int(0);
			}
			else
			{
				tnm_stack_push_int(Gp_ini->system_int_value[p_ai->al_begin[0].Int]);
			}
			break;

		case ELM_SYSCOM_GET_SYSTEM_EXTRA_STR_VALUE:
			if (!(0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < Gp_ini->system_int_value_cnt))
			{
				tnm_stack_push_str(_T(""));
			}
			else
			{
				tnm_stack_push_str(Gp_ini->system_str_value[p_ai->al_begin[0].Int]);
			}
			break;

		// default
		default:
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�����ȃR�}���h���w�肳��܂����B(syscom)"));
			break;
	}

	return true;
}
