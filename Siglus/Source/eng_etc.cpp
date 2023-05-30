#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_reg_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_namae.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_call.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// �Q�[�������擾
// ================================================================
TSTR tnm_get_game_name()
{
	return Gp_ini->game_name;
}

// ****************************************************************
// �V�[���ԍ�����V�[�������擾
// ================================================================
TSTR tnm_get_scn_name(int scn_no)
{
	return Gp_lexer->get_scn_name(scn_no);
}

// ****************************************************************
// �V�[���ԍ�����t���p�X�����擾
// ================================================================
TSTR tnm_get_scn_full_path_name(int scn_no)
{
	return Gp_config->ss_path + _T("\\") + Gp_lexer->get_scn_name(scn_no) + _T(".ss");
}

// ****************************************************************
// �V�[���^�C�g�����擾
// ================================================================
TSTR tnm_get_scene_title()
{
	return Gp_local->scene_title;
}

// ****************************************************************
// �t���^�C�g�����擾
// ================================================================
TSTR tnm_get_full_title()
{
	// �f�o�b�O���͐F�X�ȏ���Y����
	if (Gp_global->debug_flag)	{

		TSTR info_str;

		// �ȈՃV�[���p�b�N
		if (!Gp_lexer->is_exist_org_source())	{
			info_str += _T("[�������k��]");
		}
		// �G�t�F�N�g���\�[�X����쐬����
		if (Gp_global->shader_from_source)	{
			info_str += _T("[���̪�ĉ���]");
		}

		// �Q�[����
		info_str += Gp_ini->game_name;

		// �u���[�N��
		if (Gp_global->is_breaking)	{
			info_str += _T(" [�u���[�N��]");
		}

		// �V�[����
		int scn_no = Gp_lexer->get_cur_scn_no();
		if (scn_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			int line_no = Gp_lexer->get_cur_line_no();
			info_str += _T(" ") + Gp_local->scene_title + _T(" / scene=") + scn_name + _T("(") + tostr(line_no) + _T(")");
		}

		// �d�v�ȏ��
		info_str += _T(" rate=" + tostr(Gp_global->frame_rate_100msec_total)
			+ _T(", cstack=") + tostr(Gp_call_list->get_call_cnt()) + _T("(max=") + tostr(Gp_call_list->get_size()) + _T(")")
			+ _T(", pgstack=") + tostr(Gp_stack->int_now_cnt + (int)Gp_stack->str_stack.size()));

		// �����肨��уI�[�g���[�h
		if (tnm_is_skipping())	{
			info_str += _T("[��=") + tostr(Gp_global->disp_because_msg_wait_cnt_max / 100) + _T("]");
		}
		if (tnm_is_auto_mode())	{
			info_str += _T("[��]");
		}

		// �L���v�`���[�̂���Ȃ�
		if (Gp_global->capture_prior)	{
			info_str += _T("���������") + tostr(Gp_global->capture_prior);
		}

		return info_str;
	}
	else	{

		TSTR info_str;

		// �Q�[����
		info_str += Gp_ini->game_name;

		// �V�[����
		info_str += _T(" ") + Gp_local->scene_title;

		return info_str;
	}
}

// ****************************************************************
// �E�B���h�E�����擾
// ================================================================
TSTR tnm_get_window_name()
{
	TSTR window_name = _T("siglus_engine_main_window_") + Gp_ini->game_id + _T("_") + Gp_reg_ini->reg_name;
	if (Gp_cmd_line->multi_user_id_set)
	{
		window_name += _T("_") + Gp_cmd_line->multi_user_id;
	}

	return window_name;
}

// ****************************************************************
// �d�w�V�[���F�d�w�R�[����
// ================================================================
bool tnm_excall_is_excall()
{
	return Gp_global->ex_call_flag;
}

// ****************************************************************
// �d�w�V�[���F���b�Z�[�W�o�b�N��
// ================================================================
bool tnm_excall_is_msg_back()
{
	return tnm_msg_back_is_open();
}

// ****************************************************************
// �d�w�V�[���F���b�Z�[�W�E�B���h�E������
// ================================================================
bool tnm_excall_is_hide_mwnd()
{
	return tnm_syscom_hide_mwnd_get_onoff_flag();
}

// ****************************************************************
// �����蒆���𔻒�
// ================================================================
bool tnm_is_skipping()
{
	// ���͂ɂ��uCtrl �X�L�b�v�v
	// �V�X�e���R�}���h���j���[�́u���Ǒ�����v
	// �X�N���v�g�́uCtrl �֎~�v
	// �X�N���v�g�́u������֎~�v
	return 
		tnm_excall_is_msg_back() ? false :				// ���b�Z�[�W�o�b�N���͑�����ł��Ȃ�
		Gp_local->pod.ctrl_disable ? false :			// Ctrl �֎~�Ȃ������Α�����łȂ�
		Gp_global->ctrl_skip_flag ? true :				// Ctrl �������Ă����瑁����
		Gp_local->pod.skip_disable ? false :			// ������֎~�Ȃ瑁����ł��Ȃ�
		Gp_global->skip_because_skip_trigger ? true:	// �X�L�b�v�g���K�[�ɂ��X�L�b�v
		Gp_syscom->read_skip_flag;						// ������t���O
}

// ****************************************************************
// �����蒆���𔻒�i�X�L�b�v�g���K�[�������j
// ================================================================
bool tnm_is_skipping_without_skip_trigger()
{
	// ���͂ɂ��uCtrl �X�L�b�v�v
	// �V�X�e���R�}���h���j���[�́u���Ǒ�����v
	// �X�N���v�g�́uCtrl �֎~�v
	// �X�N���v�g�́u������֎~�v
	return 
		tnm_excall_is_msg_back() ? false :				// ���b�Z�[�W�o�b�N���͑�����ł��Ȃ�
		Gp_local->pod.ctrl_disable ? false :			// Ctrl �֎~�Ȃ������Α�����łȂ�
		Gp_global->ctrl_skip_flag ? true :				// Ctrl �������Ă����瑁����
		Gp_local->pod.skip_disable ? false :			// ������֎~�Ȃ瑁����ł��Ȃ�
		Gp_syscom->read_skip_flag;						// ������t���O
}

// ****************************************************************
// ���b�Z�[�W�����蒆���𔻒�
// ================================================================
bool tnm_is_skipping_msg()
{
	// ���͂ɂ��uCtrl �X�L�b�v�v
	// �V�X�e���R�}���h���j���[�́u���Ǒ�����v
	// �X�N���v�g�́uCtrl �֎~�v
	// �X�N���v�g�́u������֎~�v
	return 
		tnm_excall_is_msg_back() ? false :				// ���b�Z�[�W�o�b�N���͑�����ł��Ȃ�
		Gp_local->pod.ctrl_disable ? false :			// Ctrl �֎~�Ȃ������Α�����łȂ�
		Gp_global->ctrl_skip_flag ? true :				// Ctrl �������Ă����瑁����
		Gp_global->cs_skip_flag ? true :				// Ctrl+Shift �������Ă����瑁����
		Gp_local->pod.skip_disable ? false :			// ������֎~�Ȃ瑁����ł��Ȃ�
		Gp_global->skip_because_skip_trigger ? true:	// �X�L�b�v�g���K�[�ɂ��X�L�b�v
		Gp_syscom->read_skip_flag;						// ������t���O
}

// ****************************************************************
// ���ǂ𑁑��肷�邩�𔻒�
// ================================================================
bool tnm_is_skip_unread_msg()
{
	return
		Gp_local->pod.skip_unread_message ? true :		// �ꎞ�I�ɖ��ǂł������肷��
		Gp_config->system.skip_unread_message_flag;		// ���ݒ�
}

// ****************************************************************
// �I�[�g���[�h���𔻒�
// ================================================================
bool tnm_is_auto_mode()
{
	// ���ݒ�́u�I�[�h���[�h�v
	// �X�N���v�g�́u�ꎞ�I�ɃI�[�g���[�h�v
	return Gp_local->pod.auto_mode_flag || Gp_config->auto_mode_onoff;
}

// ****************************************************************
// �I�[�g���[�h�̕����҂����Ԃ��擾
// ================================================================
int tnm_get_auto_mode_moji_wait()
{
	// �X�N���v�g�̃I�[�g���[�h
	if (Gp_local->pod.auto_mode_flag)	{
		if (Gp_local->pod.auto_mode_moji_wait >= 0)	{
			return Gp_local->pod.auto_mode_moji_wait;
		}
		else	{
			return Gp_config->auto_mode_moji_wait;
		}
	}
	// �R���t�B�O�̃I�[�g���[�h
	else	{
		return Gp_config->auto_mode_moji_wait;
	}
}

// ****************************************************************
// �I�[�g���[�h�̍ŏ��҂����Ԃ��擾
// ================================================================
int tnm_get_auto_mode_min_wait()
{
	// �X�N���v�g�̃I�[�g���[�h
	if (Gp_local->pod.auto_mode_flag)	{
		if (Gp_local->pod.auto_mode_min_wait >= 0)	{
			return Gp_local->pod.auto_mode_min_wait;
		}
		else	{
			return Gp_config->auto_mode_min_wait;
		}
	}
	// �R���t�B�O�̃I�[�g���[�h
	else	{
		return Gp_config->auto_mode_min_wait;
	}
}

// ****************************************************************
// �����Ń}�E�X�J�[�\�����B�����𔻒�
// ================================================================
bool tnm_is_mouse_cursor_hide()
{
	// ���ݒ�́u�����Ń}�E�X�J�[�\�����B���v
	// �X�N���v�g�́u�����Ń}�E�X�J�[�\�����B�����֎~�v
	return
		Gp_local->pod.mouse_cursor_hide_onoff == 1 ? true :
		Gp_local->pod.mouse_cursor_hide_onoff == 0 ? false :
		Gp_config->mouse_cursor_hide_onoff;
}

// ****************************************************************
// �����Ń}�E�X�J�[�\�����B������
// ================================================================
int tnm_get_mouse_cursor_hide_time()
{
	// ���ݒ�́u�����Ń}�E�X�J�[�\�����B���v
	// �X�N���v�g�́u�����Ń}�E�X�J�[�\�����B�����֎~�v
	if (Gp_local->pod.mouse_cursor_hide_time != -1)
		return Gp_local->pod.mouse_cursor_hide_time;

	return Gp_config->mouse_cursor_hide_time;
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�̃A�j�����s��������
// ================================================================
bool tnm_is_mwnd_anime_use()
{
	// �X�N���v�g�́u�ꎞ�I�ɃE�B���h�E�A�j�����s���v
	// �X�N���v�g�́u�ꎞ�I�ɃE�B���h�E�A�j�����s��Ȃ��v
	// ���ݒ�́u�E�B���h�E�A�j�����s��Ȃ��v
	return 
		Gp_local->pod.mwnd_anime_on_flag ? true :
		Gp_local->pod.mwnd_anime_off_flag ? false :
		!Gp_config->system.no_mwnd_anime_flag;
}

// ****************************************************************
// �����~�߂邩�𔻒�
// ================================================================
bool tnm_is_stop_koe()
{
	// �X�N���v�g�́u�ꎞ�I�ɐ����~�߂�v
	// �X�N���v�g�́u�ꎞ�I�ɐ����~�߂Ȃ��v
	// ���ݒ�́u�����~�߂Ȃ��v
	return 
		Gp_local->pod.koe_dont_stop_on_flag ? false :
		Gp_local->pod.koe_dont_stop_off_flag ? true :
		!Gp_config->system.koe_dont_stop_flag;
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���B�����𔻒�
// ================================================================
bool tnm_is_hide_mwnd()
{
	// �V�X�e���R�}���h�F�u���b�Z�[�W�E�B���h�E���B���v���I���̏ꍇ�̓��b�Z�[�W�E�B���h�E���B��
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return true;

	// �d�w�R�[���F�u���b�Z�[�W�o�b�N�v���̓��b�Z�[�W�E�B���h�E���B��
	if (tnm_excall_is_msg_back())
		return true;

	// ���[�J���f�[�^�F�u���b�Z�[�W�E�B���h�E��\�����Ȃ��v�̏ꍇ�͉B��
	if (Gp_local->pod.mwnd_disp_off_flag)
		return true;

	return false;
}

// ****************************************************************
// �J�[�\����\�����邩�𔻒�
// ================================================================
bool tnm_is_cursor_disp()
{
	// �d�w�R�[�����̓J�[�\����\������
	if (tnm_excall_is_excall())
		return true;

	// ���b�Z�[�W�o�b�N���̓J�[�\����\������
	if (tnm_excall_is_msg_back())
		return true;

	// �I�[�g���[�h���ɂ��J�[�\�����B��
	if (Gp_global->auto_mode_cursor_off)
		return false;

	// �����ŃJ�[�\���������@�\
	if (tnm_is_mouse_cursor_hide())	{
		if (Gp_global->mouse_cursor_hide_time >= tnm_get_mouse_cursor_hide_time())	{
			return false;
		}
	}

	// �V�[���f�[�^�F�J�[�\�����B��
	if (Gp_local->pod.cursor_disp_off)
		return false;

	return true;
}

// ****************************************************************
// �����Đ����邩�ǂ����𔻒�
// ================================================================
bool tnm_is_koe_play(int chara_no)
{
	// �X�L�b�v���͐����Đ����Ȃ��i�X�L�b�v�g���K�[�͏����j
	if (tnm_is_skipping_without_skip_trigger())
		return false;

	// ���S�̂̃`�F�b�N�������Ă��Ȃ��Ȃ�Đ����Ȃ�
	if (!Gp_config->play_all_sound_check)
		return false;

	// �����̃`�F�b�N�������Ă��Ȃ��Ȃ�Đ����Ȃ�
	if (!Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE])
		return false;

	// �u�����Ȃ��v�Ȃ琺���Đ����Ȃ�
	if (Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY)
		return false;

	// �L�����ԍ����}�C�i�X�Ȃ�Đ�����
	if (chara_no < 0)
		return true;

	// �L�����ԍ��ɑΉ�����o�^�ԍ������߂�
	for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
		for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
			if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
				// ���̓o�^�ԍ����Đ�����H
				if (i < (int)Gp_config->chrkoe.size() && !Gp_config->chrkoe[i].onoff)
					return false;
			}
		}
	}

	// �Đ�����
	return true;
}

// ****************************************************************
// �L�����N�^�[�����I���I�t���擾
// ================================================================
bool tnm_get_chrkoe_onoff(int chara_no)
{
	// �L�����ԍ����}�C�i�X�Ȃ�Đ�����
	if (chara_no < 0)
		return true;

	// �L�����ԍ��ɑΉ�����o�^�ԍ������߂�
	for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
		for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
			if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
				// ���̓o�^�ԍ����Đ�����H
				if (i < (int)Gp_config->chrkoe.size() && !Gp_config->chrkoe[i].onoff)
					return false;
			}
		}
	}

	return true;
}

// ****************************************************************
// �L�����N�^�[�����{�����[�����擾
// ================================================================
int tnm_get_chrkoe_volume(int chara_no)
{
	int volume = 255;

	// �L�����ԍ��ɑΉ�����o�^�ԍ������߂�
	if (chara_no >= 0)	{
		for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
			for (int j = 0; j < (int)Gp_ini->config.chrkoe[i].chr_no_list.size(); j++)	{
				if (Gp_ini->config.chrkoe[i].chr_no_list[j] == chara_no)	{
					volume = volume * Gp_config->chrkoe[i].volume / 255;
				}
			}
		}
	}

	return volume;
}

// ****************************************************************
// �L�����N�^�[�����̖��O�̕��������J���邩�ǂ�������i���J�t���O���n�m�ɂ��܂��j
// ================================================================
void tnm_is_chrkoe_name_check(CTSTR& name_str)
{
	if (Gp_chrkoe->empty())
		return;

	for (int i = 0; i < (int)Gp_chrkoe->size(); i++)	{
		if (!(*Gp_chrkoe)[i].look_flag && Gp_ini->config.chrkoe[i].check_mode == TNM_CHRKOE_CHECK_MODE_AUTO)	{		// �܂�������ԂŁA���[�h�� TNM_CHRKOE_CHECK_MODE_AUTO �̏ꍇ
			if (Gp_ini->config.chrkoe[i].name_str == name_str || Gp_ini->config.chrkoe[i].check_name_str == name_str)	{
				(*Gp_chrkoe)[i].look_flag = true;
			}
		}
	}
}

// ****************************************************************
// ���Ǒ�������X�V
// ================================================================
void tnm_update_read_skip()
{
	int rf_scn_no = Gp_local->pod.cur_read_flag_scn_no;
	int rf_flag_no = Gp_local->pod.cur_read_flag_flag_no;

	// ���Ǒ����肪�\���ǂ����𔻒肷��
	if (false);
	else if (rf_scn_no < 0 || (int)Gp_read_flag->size() <= rf_scn_no)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else if (rf_flag_no < 0 || (int)(*Gp_read_flag)[rf_scn_no].size() <= rf_flag_no)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else if ((*Gp_read_flag)[rf_scn_no][rf_flag_no] == 0)	{
		Gp_global->read_skip_enable_flag = false;
	}
	else	{
		Gp_global->read_skip_enable_flag = true;
	}

	// �����肪�s�\�ȏꍇ�͑�������~�߂�
	if (!Gp_global->read_skip_enable_flag && !tnm_is_skip_unread_msg())	{
		tnm_syscom_read_skip_set_onoff_flag(false);
		Gp_global->cant_auto_skip_before_click = true;	// �N���b�N����܂Ŏ�����������֎~����
	}
}

// ****************************************************************
// ������������J�n����
// ================================================================
void tnm_update_start_skip()
{
	// �u�N���b�N����܂Ŏ�����������֎~����v�t���O�������Ă���Ή������Ȃ�
	if (Gp_global->cant_auto_skip_before_click)
		return;

	// ���������肪�n�m�̏ꍇ
	if (tnm_syscom_auto_skip_get_onoff_flag())	{
		// ���ǂ������́A���ǃX�L�b�v���n�m�̏ꍇ
		if (Gp_global->read_skip_enable_flag || tnm_is_skip_unread_msg())	{
			// ��������J�n
			tnm_syscom_read_skip_set_onoff_flag(true);
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�X�s�[�h���擾����
// ================================================================
int tnm_get_total_msg_speed()
{
	// �V�[���Ńm�[�E�F�C�g���w�肵�Ă���΃m�[�E�F�C�g
	if (Gp_local->pod.msg_nowait)
		return 0;

	// �V�[���ő��x���w�肵�Ă���΂��̒l
	if (Gp_local->pod.msg_speed != -1)
		return Gp_local->pod.msg_speed;

	// ���ݒ�Ńm�[�E�F�C�g�Ȃ�O�b
	if (Gp_config->message_speed_nowait)
		return 0;

	// ���ݒ�Ńm�[�E�F�C�g�łȂ��Ȃ���ݒ�̒l
	return Gp_config->message_speed;
}



