#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_cache.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/eng_etc.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_dialog.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_error.h"
#include	"dialog/sys_wnd_modal_save.h"
#include	"dialog/sys_wnd_modal_load.h"

// ****************************************************************
// �V�X�e���R�}���h�F�u���[�N�^�u���[�N����
// ================================================================
bool tnm_syscom_change_break()
{
	Gp_global->is_breaking = !Gp_global->is_breaking;

	// �u���[�N�����ꍇ�A�g���[�X�E�B���h�E�������I�ɊJ��
	if (Gp_global->is_breaking)	{
		tnm_dlg_open_trace_dialog();

		// �f�o�b�O�ݒ肪����ĂȂ��ꍇ�͂���ɐݒ�E�B���h�E���J��
		if (Gp_config->ss_path.empty())
			tnm_dlg_open_debug_setting_dialog();
	}

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�X�e�b�v���s
// ================================================================
bool tnm_syscom_do_one_step()
{
	Gp_global->break_step_flag = true;

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�ŏ�����͂��߂�
// ================================================================
bool tnm_syscom_restart_from_start()
{
	int res = tnm_game_warning_box(Gp_ini->config.warninginfo.restart_warning_str);
	if (res == IDNO)
		return false;

	// ���O���[�o���f�[�^���Z�[�u����
	tnm_save_global_on_file();

	// �ŏ�����͂��߂�
	tnm_scene_proc_restart_from_start_scene();

	// ���Q�[���^�C�}�[�J�n
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�V�[������͂��߂�
// ================================================================
bool tnm_syscom_restart_from_scene(int scn_no, int z_no)
{
	int res = tnm_game_warning_box(Gp_ini->config.warninginfo.scenestart_warning_str);
	if (res == IDNO)
		return false;

	// ���O���[�o���f�[�^���Z�[�u����
	tnm_save_global_on_file();

	// ���X�^�[�g
	tnm_scene_proc_restart(scn_no, z_no);

	// ���Q�[���^�C�}�[�J�n
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[���X�V
// ================================================================
void tnm_syscom_update_menu()
{
	// ���Ǒ����肪�g���Ȃ��ꍇ�A�����I�� OFF �ɂ��܂��B
	if (!tnm_syscom_read_skip_is_enable())
		Gp_syscom->read_skip_flag = false;
}

// ****************************************************************
// �V�X�e���R�}���h�F�V�X�e���R�}���h�̃t���O��������
// ================================================================
void tnm_syscom_init_syscom_flag()
{
	Gp_syscom->read_skip_flag = false;
	Gp_syscom->auto_skip_flag = false;
	Gp_syscom->hide_mwnd_flag = false;
	Gp_syscom->msg_back_flag = false;

	// ���݂̗L��
	Gp_syscom->syscommenu.read_skip.exist = Gp_ini->syscommenu.read_skip.exist;
	Gp_syscom->syscommenu.unread_skip.exist = Gp_ini->syscommenu.unread_skip.exist;
	Gp_syscom->syscommenu.auto_skip.exist = Gp_ini->syscommenu.auto_skip.exist;
	Gp_syscom->syscommenu.auto_mode.exist = Gp_ini->syscommenu.auto_mode.exist;
	Gp_syscom->syscommenu.hide_mwnd.exist = Gp_ini->syscommenu.hide_mwnd.exist;
	Gp_syscom->syscommenu.msg_back.exist = Gp_ini->syscommenu.msg_back.exist;
	Gp_syscom->syscommenu.save.exist = Gp_ini->syscommenu.save.exist;
	Gp_syscom->syscommenu.load.exist = Gp_ini->syscommenu.load.exist;
	Gp_syscom->syscommenu.return_to_sel.exist = Gp_ini->syscommenu.return_to_sel.exist;
	Gp_syscom->syscommenu.config.exist = Gp_ini->syscommenu.config.exist;
	Gp_syscom->syscommenu.manual.exist = Gp_ini->syscommenu.manual.exist;
	Gp_syscom->syscommenu.version.exist = Gp_ini->syscommenu.version.exist;
	Gp_syscom->syscommenu.return_to_menu.exist = Gp_ini->syscommenu.return_to_menu.exist;
	Gp_syscom->syscommenu.game_end.exist = Gp_ini->syscommenu.game_end.exist;
	Gp_syscom->syscommenu.cancel.exist = Gp_ini->syscommenu.cancel.exist;

	// ���^�֎~
	Gp_syscom->syscommenu.read_skip.enable = Gp_ini->syscommenu.read_skip.enable;
	Gp_syscom->syscommenu.unread_skip.enable = Gp_ini->syscommenu.unread_skip.enable;
	Gp_syscom->syscommenu.auto_skip.enable = Gp_ini->syscommenu.auto_skip.enable;
	Gp_syscom->syscommenu.auto_mode.enable = Gp_ini->syscommenu.auto_mode.enable;
	Gp_syscom->syscommenu.hide_mwnd.enable = Gp_ini->syscommenu.hide_mwnd.enable;
	Gp_syscom->syscommenu.msg_back.enable = Gp_ini->syscommenu.msg_back.enable;;
	Gp_syscom->syscommenu.save.enable = Gp_ini->syscommenu.save.enable;
	Gp_syscom->syscommenu.load.enable = Gp_ini->syscommenu.load.enable;
	Gp_syscom->syscommenu.return_to_sel.enable = Gp_ini->syscommenu.return_to_sel.enable;
	Gp_syscom->syscommenu.config.enable = Gp_ini->syscommenu.config.enable;
	Gp_syscom->syscommenu.manual.enable = Gp_ini->syscommenu.manual.enable;
	Gp_syscom->syscommenu.version.enable = Gp_ini->syscommenu.version.enable;
	Gp_syscom->syscommenu.return_to_menu.enable = Gp_ini->syscommenu.return_to_menu.enable;
	Gp_syscom->syscommenu.game_end.enable = Gp_ini->syscommenu.game_end.enable;
	Gp_syscom->syscommenu.cancel.enable = Gp_ini->syscommenu.cancel.enable;

	// ���[�J���ėp�X�C�b�`
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < TNM_LOCAL_EXTRA_SWITCH_CNT; i++)	{
			Gp_syscom->syscommenu.local_extra_switch[i].exist = Gp_ini->syscommenu.local_extra_switch[i].exist;
			Gp_syscom->syscommenu.local_extra_switch[i].enable = Gp_ini->syscommenu.local_extra_switch[i].enable;
			Gp_syscom->syscommenu.local_extra_switch[i].onoff = Gp_ini->syscommenu.local_extra_switch[i].onoff;
		}
	}

	// ���[�J���ėp���[�h
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		for (int i = 0; i < TNM_LOCAL_EXTRA_MODE_CNT; i++)	{
			Gp_syscom->syscommenu.local_extra_mode[i].exist = Gp_ini->syscommenu.local_extra_mode[i].exist;
			Gp_syscom->syscommenu.local_extra_mode[i].enable = Gp_ini->syscommenu.local_extra_mode[i].enable;
			Gp_syscom->syscommenu.local_extra_mode[i].mode = Gp_ini->syscommenu.local_extra_mode[i].mode;
		}
	}

	// ���j���[���X�V
	tnm_syscom_update_menu();
}

// ****************************************************************
// �V�X�e���R�}���h���J��
// ================================================================
bool tnm_syscom_open()
{
	// �V�X�e���R�}���h�V�[�����w�肳��Ă��Ȃ��ꍇ
	if (Gp_ini->cancel_scene_name.empty())	{

		// �V�X�e���R�}���h���j���[���J��
		if (!tnm_rmenu_open())
			return false;
	}
	else	{

		// �V�X�e���R�}���h�V�[�����Ă�
		if (!tnm_scene_proc_call_syscom_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ������ ON/OFF ��ݒ�
// ================================================================
void tnm_syscom_read_skip_set_onoff_flag(bool onoff)
{
	Gp_syscom->read_skip_flag = onoff;

	// ���j���[���X�V
	tnm_syscom_update_menu();
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ������ ON/OFF ���擾
// ================================================================
bool tnm_syscom_read_skip_get_onoff_flag()
{
	return Gp_syscom->read_skip_flag;
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ�����̋��^�s����ݒ�
// ================================================================
void tnm_syscom_read_skip_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.read_skip.enable = enable;

	// ���j���[���X�V
	tnm_syscom_update_menu();
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ�����̋��^�s�����擾
// ================================================================
bool tnm_syscom_read_skip_get_enable_flag()
{
	return Gp_syscom->syscommenu.read_skip.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ�����̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_read_skip_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.read_skip.exist = exist;

	// ���j���[���X�V
	tnm_syscom_update_menu();
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ�����̕\���^��\�����擾
// ================================================================
bool tnm_syscom_read_skip_get_exist_flag()
{
	return Gp_syscom->syscommenu.read_skip.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���Ǒ����肪�g�p�\�����擾
// ================================================================
bool tnm_syscom_read_skip_is_enable()
{
	// ���ǃX�L�b�v���n�m�̏ꍇ
	if (tnm_is_skip_unread_msg())	{
		return Gp_syscom->syscommenu.read_skip.enable && Gp_syscom->syscommenu.read_skip.exist;
	}
	else	{
		return Gp_syscom->syscommenu.read_skip.enable && Gp_syscom->syscommenu.read_skip.exist && Gp_global->read_skip_enable_flag;
	}
}

// ****************************************************************
// �V�X�e���R�}���h�F����������� ON/OFF ��ݒ�
// ================================================================
void tnm_syscom_auto_skip_set_onoff_flag(bool onoff)
{
	Gp_syscom->auto_skip_flag = onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F����������� ON/OFF ���擾
// ================================================================
bool tnm_syscom_auto_skip_get_onoff_flag()
{
	return Gp_syscom->auto_skip_flag;
}

// ****************************************************************
// �V�X�e���R�}���h�F����������̋��^�s����ݒ�
// ================================================================
void tnm_syscom_auto_skip_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.auto_skip.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F����������̋��^�s�����擾
// ================================================================
bool tnm_syscom_auto_skip_get_enable_flag()
{
	return Gp_syscom->syscommenu.auto_skip.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F����������̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_auto_skip_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.auto_skip.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F����������̕\���^��\�����擾
// ================================================================
bool tnm_syscom_auto_skip_get_exist_flag()
{
	return Gp_syscom->syscommenu.auto_skip.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���������肪�g�p�\�����擾
// ================================================================
bool tnm_syscom_auto_skip_is_enable()
{
	return Gp_syscom->syscommenu.auto_skip.enable && Gp_syscom->syscommenu.auto_skip.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�� ON/OFF ��ݒ�
// ================================================================
void tnm_syscom_auto_mode_set_onoff_flag(bool onoff)
{
	Gp_config->auto_mode_onoff = onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�� ON/OFF ���擾
// ================================================================
bool tnm_syscom_auto_mode_get_onoff_flag()
{
	return Gp_config->auto_mode_onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_auto_mode_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.auto_mode.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�̋��^�s�����擾
// ================================================================
bool tnm_syscom_auto_mode_get_enable_flag()
{
	return Gp_syscom->syscommenu.auto_mode.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_auto_mode_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.auto_mode.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_auto_mode_get_exist_flag()
{
	return Gp_syscom->syscommenu.auto_mode.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�I�[�g���[�h���g�p�\�����擾
// ================================================================
bool tnm_syscom_auto_mode_is_enable()
{
	return Gp_syscom->syscommenu.auto_mode.enable && Gp_syscom->syscommenu.auto_mode.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E������ ON/OFF ��ݒ�
// ================================================================
void tnm_syscom_hide_mwnd_set_onoff_flag(bool onoff)
{
	Gp_syscom->hide_mwnd_flag = onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E������ ON/OFF ���擾
// ================================================================
bool tnm_syscom_hide_mwnd_get_onoff_flag()
{
	return Gp_syscom->hide_mwnd_flag;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E�����̋��^�s����ݒ�
// ================================================================
void tnm_syscom_hide_mwnd_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.hide_mwnd.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E�����̋��^�s�����擾
// ================================================================
bool tnm_syscom_hide_mwnd_get_enable_flag()
{
	return Gp_syscom->syscommenu.hide_mwnd.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E�����̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_hide_mwnd_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.hide_mwnd.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E�����̕\���^��\�����擾
// ================================================================
bool tnm_syscom_hide_mwnd_get_exist_flag()
{
	return Gp_syscom->syscommenu.hide_mwnd.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�E�B���h�E����邪�g�p�\�����擾
// ================================================================
bool tnm_syscom_hide_mwnd_is_enable()
{
	return Gp_syscom->syscommenu.hide_mwnd.enable && Gp_syscom->syscommenu.hide_mwnd.exist && !Gp_local->pod.hide_mwnd_disable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N���J��
// ================================================================
void tnm_syscom_open_msg_back()
{
	if (!tnm_msg_back_is_enable())
		return;

	// ��������~�߂�
	tnm_syscom_read_skip_set_onoff_flag(false);

	// ���b�Z�[�W�o�b�N���J��
	tnm_msg_back_open();

	// ���b�Z�[�W�o�b�N�v���Z�X��
	tnm_push_proc(TNM_PROC_TYPE_MSG_BACK);
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�����
// ================================================================
void tnm_syscom_close_msg_back()
{
	// ���b�Z�[�W�o�b�N���J���Ă���ꍇ�̂�
	if (tnm_msg_back_is_open())	{

		// ���b�Z�[�W�o�b�N�����
		tnm_msg_back_close();

		// ���̃v���Z�X��
		tnm_pop_proc();
	}
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_msg_back_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.msg_back.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�̋��^�s�����擾
// ================================================================
bool tnm_syscom_msg_back_get_enable_flag()
{
	return Gp_syscom->syscommenu.msg_back.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_msg_back_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.msg_back.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_msg_back_get_exist_flag()
{
	return Gp_syscom->syscommenu.msg_back.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N���g�p�\�����擾
// ================================================================
bool tnm_syscom_msg_back_is_enable()
{
	return Gp_syscom->syscommenu.msg_back.enable && Gp_syscom->syscommenu.msg_back.exist && tnm_msg_back_is_exist_msg() && !Gp_local->pod.msg_back_disable;
}

// ****************************************************************
// �V�X�e���F�Z�[�u�_�C�A���O���J���ăZ�[�u����
// ================================================================
bool tnm_open_save_dialog(bool capture_flag)
{
	// �L���v�`���[����ꍇ
	if (capture_flag && Gp_ini->use_save_thumb)	{

		// �Q�D�Z�[�u�_�C�A���O���Ăяo��
		tnm_push_proc(TNM_PROC_TYPE_SAVE_DIALOG);	// �Z�[�u�_�C�A���O

		// �P�D�L���v�`���[���s��
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_SAVE, true);
	}
	else	{

		// �_�C�A���O���J��
		C_sys_wnd_modal_save sys_wnd_modal_save;
		int res = sys_wnd_modal_save.open_modal_result_int();		// res : �f�[�^�ԍ��i-1 = �L�����Z���j
		if (res == -1)
			return false;

		// �_�C�A���O�őI�������ꍇ�� syscom enable �`�F�b�N���s�킸�ɕK���Z�[�u
		if (!tnm_syscom_save(res, true, true))
			return false;
	}

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J��
// ================================================================
bool tnm_syscom_open_save(bool dialog_flag, bool capture_flag, bool enable_check)
{
	// �Z�[�u�ł��邩�𔻒�
	if (!tnm_is_save_enable())
		return true;

	// �Z�[�u�V�[�����w�肳��Ă��Ȃ��ꍇ
	if (dialog_flag || Gp_ini->save_scene_name.empty())	{

		// �Z�[�u�_�C�A���O���J��
		if (!tnm_open_save_dialog(capture_flag))
			return false;
	}
	else	{
 
		// �Z�[�u�V�[�����Ă�
		if (!tnm_scene_proc_call_save_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_open_save_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.save.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J���̋��^�s�����擾
// ================================================================
bool tnm_syscom_open_save_get_enable_flag()
{
	return Gp_syscom->syscommenu.save.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_open_save_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.save.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_open_save_get_exist_flag()
{
	return Gp_syscom->syscommenu.save.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���J�����\���𔻒�
// ================================================================
bool tnm_syscom_open_save_is_enable()
{
	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable();
}

// ****************************************************************
// �V�X�e���F���[�h�_�C�A���O���J���ă��[�h����
// ================================================================
int tnm_open_load_dialog(bool is_execute)
{
	// �_�C�A���O���J��
	C_sys_wnd_modal_load sys_wnd_modal_load;
	int res = sys_wnd_modal_load.open_modal_result_int();		// res : �f�[�^�ԍ��i-1 = �L�����Z���j
	if (res == -1)
		return -1;

	// ���[�h
	if (is_execute)	{

		// �_�C�A���O�őI�������ꍇ�� syscom enable �`�F�b�N���s�킸�ɕK�����[�h
		if (!tnm_syscom_load(res, true, true, true))
			return -2;		// ���[�h�ł��Ȃ������ꍇ�� -2 ��߂�
	}

	return res;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J��
// ================================================================
bool tnm_syscom_open_load(bool dialog_flag, bool enable_check)
{
	// ���[�h�ł��邩�𔻒�
	if (!tnm_is_load_enable())
		return true;

	// ���[�h�V�[�����w�肳��Ă��Ȃ��ꍇ
	if (dialog_flag || Gp_ini->load_scene_name.empty())	{

		// ���[�h�_�C�A���O���J��
		if (tnm_open_load_dialog(true) < 0)
			return false;
	}
	else	{
 
		// ���[�h�V�[�����Ă�
		if (!tnm_scene_proc_call_load_scene())
			return false;
	}

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_open_load_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.load.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J���̋��^�s�����擾
// ================================================================
bool tnm_syscom_open_load_get_enable_flag()
{
	return Gp_syscom->syscommenu.load.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_open_load_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.load.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_open_load_get_exist_flag()
{
	return Gp_syscom->syscommenu.load.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���J�����\���𔻒�
// ================================================================
bool tnm_syscom_open_load_is_enable()
{
	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable();
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u
// ================================================================
bool tnm_syscom_save(int save_no, bool warning, bool se_play)
{
	// �Z�[�u�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_save_enable(save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// �t�@�C���̑��݊m�F
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// �㏑���m�F
			int res = tnm_game_warning_box(Gp_ini->config.save_info.warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// �Z�[�u�i�ԍ��w��j
	if (!tnm_saveload_proc_save(save_no))
		return false;

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Z�[�u���\���𔻒�
// ================================================================
bool tnm_syscom_save_is_enable(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt <= save_no)
		return false;

	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(save_no);
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h
// ================================================================
bool tnm_syscom_load(int save_no, bool warning, bool se_play, bool fade_out)
{
	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_load_enable(save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ���Q�[���^�C�}�[��~�i���[�h�j
	Gp_global->game_timer_move_flag = false;

	// �u���[�h�v��\��
	tnm_push_proc(TNM_PROC_TYPE_LOAD, save_no);

	// �t�F�[�h�A�E�g
	if (fade_out)	{

		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�h���\���𔻒�
// ================================================================
bool tnm_syscom_load_is_enable(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt <= save_no)
		return false;

	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(save_no);
}

// ****************************************************************
// �V�X�e���R�}���h�F�N�C�b�N�Z�[�u
// ================================================================
bool tnm_syscom_quick_save(int quick_save_no, bool warning, bool se_play)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// �Z�[�u�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_save_enable(Gp_ini->save_cnt + quick_save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// �t�@�C���̑��݊m�F
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// �㏑���m�F
			int res = tnm_game_warning_box(Gp_ini->config.save_info.quick_warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// �N�C�b�N�Z�[�u
	if (!tnm_saveload_proc_quick_save(quick_save_no))
		return false;

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�N�C�b�N�Z�[�u���\���𔻒�
// ================================================================
bool tnm_syscom_quick_save_is_enable(int quick_save_no)
{
	// �͈̓`�F�b�N
	if (quick_save_no < 0 || Gp_ini->quick_save_cnt <= quick_save_no)
		return false;

	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(Gp_ini->save_cnt + quick_save_no);
}

// ****************************************************************
// �V�X�e���R�}���h�F�N�C�b�N���[�h
// ================================================================
bool tnm_syscom_quick_load(int quick_save_no, bool warning, bool se_play, bool fade_out)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_load_enable(save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.quick_warning_str);
		if (res == IDNO)
			return false;
	}

	// ���Q�[���^�C�}�[��~�i�N�C�b�N���[�h�j
	Gp_global->game_timer_move_flag = false;

	// �u�N�C�b�N���[�h�v��\��
	tnm_push_proc(TNM_PROC_TYPE_QUICK_LOAD, quick_save_no);

	// �t�F�[�h�A�E�g
	if (fade_out)	{

		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�N�C�b�N���[�h���\���𔻒�
// ================================================================
bool tnm_syscom_quick_load_is_enable(int quick_save_no)
{
	// �͈̓`�F�b�N
	if (quick_save_no < 0 || Gp_ini->quick_save_cnt <= quick_save_no)
		return false;

	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(Gp_ini->save_cnt + quick_save_no);
}

// ****************************************************************
// �V�X�e���R�}���h�F�G���h�Z�[�u
// ================================================================
bool tnm_syscom_end_save_period_2(bool capture_flag)
{
	// �L���v�`���[����ꍇ
	if (capture_flag && Gp_ini->use_save_thumb)	{

		// �Q�D�G���h�Z�[�u
		tnm_push_proc(TNM_PROC_TYPE_END_SAVE);		// �G���h�Z�[�u

		// �P�D�L���v�`���[���s��
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_END, true);
	}
	else	{

		// �G���h�Z�[�u���s��
		if (!tnm_saveload_proc_end_save())
			return false;
	}

	return true;
}

bool tnm_syscom_end_save(bool warning, bool se_play)
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// �Z�[�u�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_save_enable(save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		// �t�@�C���̑��݊m�F
		TSTR path = Gp_dir->get_local_save_file_path(save_no);
		if (file_get_type(path) == 1)
		{
			// �㏑���m�F
			int res = tnm_game_warning_box(Gp_ini->config.save_info.warning_str);
			if (res == IDNO)
				return false;
		}
	}

	// �G���h�Z�[�u�i�Q�i�K�ځj
	if (!tnm_syscom_end_save_period_2(true))
		return false;

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_SAVE);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�G���h�Z�[�u���\���𔻒�
// ================================================================
bool tnm_syscom_end_save_is_enable()
{
	return Gp_syscom->syscommenu.save.enable && Gp_syscom->syscommenu.save.exist && tnm_is_save_enable(Gp_ini->save_cnt + Gp_ini->quick_save_cnt);
}

// ****************************************************************
// �V�X�e���R�}���h�F�G���h���[�h
// ================================================================
bool tnm_syscom_end_load(bool warning, bool se_play, bool fade_out)
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_is_load_enable(save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ���Q�[���^�C�}�[��~�i�G���h���[�h�j
	Gp_global->game_timer_move_flag = false;

	// �u�G���h���[�h�v��\��
	tnm_push_proc(TNM_PROC_TYPE_END_LOAD);

	// �t�F�[�h�A�E�g
	if (fade_out)	{

		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�C���i�[���[�h
// ================================================================
bool tnm_syscom_inner_load(int inner_save_no, bool warning, bool se_play, bool fade_out)
{
	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_savedata_system_check_inner_save(inner_save_no))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.warning_str);
		if (res == IDNO)
			return false;
	}

	// ���Q�[���^�C�}�[��~�i�C���i�[���[�h�j
	Gp_global->game_timer_move_flag = false;

	// �u�C���i�[���[�h�v��\��
	tnm_push_proc(TNM_PROC_TYPE_INNER_LOAD, inner_save_no);

	// �t�F�[�h�A�E�g
	if (fade_out)	{

		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);

		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N���[�h
// ================================================================
bool tnm_syscom_msgbk_load(S_tid save_id, bool warning, bool se_play, bool fade_out)
{
	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_check_backlog_save(save_id))
		return false;

	// �m�F���b�Z�[�W
	if (warning && Gp_config->system.saveload_alert_flag)
	{
		int res = tnm_game_warning_box(Gp_ini->config.load_info.msgbk_warning_str);
		if (res == IDNO)
			return false;
	}

	// ���Q�[���^�C�}�[��~�i�o�b�N���O���[�h�j
	Gp_global->game_timer_move_flag = false;

	// �u�o�b�N���O���[�h�v��\��
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_BACKLOG_LOAD;
	proc.option_time_id = save_id;
	tnm_push_proc(proc);

	// �t�F�[�h�A�E�g
	if (fade_out)
	{
		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_LOAD);

	return true;
}

// ****************************************************************
// �V�X�e���R�}���h�F�G���h���[�h���\���𔻒�
// ================================================================
bool tnm_syscom_end_load_is_enable()
{
	return Gp_syscom->syscommenu.load.enable && Gp_syscom->syscommenu.load.exist && tnm_is_load_enable(Gp_ini->save_cnt + Gp_ini->quick_save_cnt);
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂�
// ================================================================
void tnm_syscom_return_to_sel(bool warning, bool se_play, bool fade_out)
{
	// �m�F���b�Z�[�W
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.returnsel_warning_str);
		if (res == IDNO)
			return;
	}

	// ���Q�[���^�C�}�[��~�i�O�̑I�����ɖ߂�j
	Gp_global->game_timer_move_flag = false;

	// �u�O�̑I�����ɖ߂�v��\��
	tnm_push_proc(TNM_PROC_TYPE_RETURN_TO_SEL);

	// �t�F�[�h�A�E�g
	if (fade_out)
	{
		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_PREV_SEL);
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_return_to_sel_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.return_to_sel.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂�̋��^�s�����擾
// ================================================================
bool tnm_syscom_return_to_sel_get_enable_flag()
{
	return Gp_syscom->syscommenu.return_to_sel.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_return_to_sel_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.return_to_sel.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_return_to_sel_get_exist_flag()
{
	return Gp_syscom->syscommenu.return_to_sel.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�O�̑I�����ɖ߂邪�\���𔻒�
// ================================================================
bool tnm_syscom_return_to_sel_is_enable()
{
	return Gp_syscom->syscommenu.return_to_sel.enable && Gp_syscom->syscommenu.return_to_sel.exist && tnm_is_exist_sel_save();
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂�
// ================================================================
void tnm_syscom_return_to_menu(bool warning, bool se_play, bool fade_out, bool leave_msgbk)
{
	// �m�F���b�Z�[�W
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.returnmenu_warning_str);
		if (res == IDNO)
			return;
	}

	// ���O���[�o���f�[�^���Z�[�u����
	tnm_save_global_on_file();

	// ���Q�[���^�C�}�[��~�i���j���[�ɖ߂�j
	Gp_global->game_timer_move_flag = false;

	// �u���j���[�ɖ߂�v��\��
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_RETURN_TO_MENU;
	proc.option = (leave_msgbk ? 1 : 0);
	tnm_push_proc(proc);

	// �t�F�[�h�A�E�g
	if (fade_out)
	{
		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
		// �u�`��v��\��
		tnm_push_proc(TNM_PROC_TYPE_DISP);
	}

	// �V�X�e������炷
	if (se_play)
		tnm_play_se(TNM_SE_TYPE_MENU);
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_return_to_menu_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.return_to_menu.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂�̋��^�s�����擾
// ================================================================
bool tnm_syscom_return_to_menu_get_enable_flag()
{
	return Gp_syscom->syscommenu.return_to_menu.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_return_to_menu_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.return_to_menu.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_return_to_menu_get_exist_flag()
{
	return Gp_syscom->syscommenu.return_to_menu.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���j���[�ɖ߂邪�\���𔻒�
// ================================================================
bool tnm_syscom_return_to_menu_is_enable()
{
	return Gp_syscom->syscommenu.return_to_menu.enable && Gp_syscom->syscommenu.return_to_menu.exist;
}

// ****************************************************************
// �E�B���h�E�����
// ================================================================
void tnm_syscom_close_window()
{
	Gp_wnd->close();
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I������
// ================================================================
void tnm_syscom_end_game(bool warning, bool se_play, bool fade_out)
{
	// �m�F���b�Z�[�W
	if (warning)
	{
		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.gameend_warning_str);
		if (res == IDNO)
			return;
	}

	// ���Q�[���^�C�}�[��~�i�Q�[�����I������j
	Gp_global->game_timer_move_flag = false;

	// �u�`��v��\��
	tnm_push_proc(TNM_PROC_TYPE_DISP);

	// �u�Q�[�����I������v��\��
	tnm_push_proc(TNM_PROC_TYPE_END_GAME);

	// �t�F�[�h�A�E�g
	if (fade_out)
	{
		// �u�Q�[���I�����̃��C�v�v��\��
		tnm_push_proc(TNM_PROC_TYPE_GAME_END_WIPE);
	}

	// �G���h�Z�[�u�i�L���v�`���[�̂��߂̕`�悪����\��������܂��j
	tnm_syscom_end_save(false, false);		// �������G���h�Z�[�u�isyscom�j
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_end_game_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.game_end.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I���̋��^�s�����擾
// ================================================================
bool tnm_syscom_end_game_get_enable_flag()
{
	return Gp_syscom->syscommenu.game_end.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_end_game_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.game_end.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_end_game_get_exist_flag()
{
	return Gp_syscom->syscommenu.game_end.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�Q�[�����I�����邪�\���𔻒�
// ================================================================
bool tnm_syscom_end_game_is_enable()
{
	return Gp_syscom->syscommenu.game_end.enable && Gp_syscom->syscommenu.game_end.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�����Đ�
// ================================================================
void tnm_syscom_koe_play()
{
	// ���̃��v���C�̓��b�Z�[�W�o�b�N����
	int jitan_rate = Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL;
	tnm_play_koe(Gp_local->pod.cur_koe_no, Gp_local->pod.cur_chr_no, jitan_rate, 0, false, false, false, true, false);
}

// ****************************************************************
// �V�X�e���R�}���h�F�����Đ����\���𔻒�
// ================================================================
bool tnm_syscom_koe_play_is_enable()
{
	return Gp_local->pod.cur_koe_no >= 0;
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J��
// ================================================================
void tnm_syscom_open_config()
{
	// ���ݒ�V�[�����w�肳��Ă��Ȃ��ꍇ
	if (Gp_ini->config_scene_name.empty())
	{
		// ���ݒ�_�C�A���O���J��
		tnm_dlg_open_config_dialog();
	}
	else
	{
		// ���ݒ�V�[�����Ă�
		if (!tnm_scene_proc_call_config_scene())
			return;
	}
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_open_config_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.config.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J���̋��^�s�����擾
// ================================================================
bool tnm_syscom_open_config_get_enable_flag()
{
	return Gp_syscom->syscommenu.config.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_open_config_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.config.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_open_config_get_exist_flag()
{
	return Gp_syscom->syscommenu.config.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���ݒ���J�����\���𔻒�
// ================================================================
bool tnm_syscom_open_config_is_enable()
{
	return Gp_syscom->syscommenu.config.enable && Gp_syscom->syscommenu.config.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�}�j���A�����J���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_manual_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.manual.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�}�j���A�����J���̋��^�s�����擾
// ================================================================
bool tnm_syscom_manual_get_enable_flag()
{
	return Gp_syscom->syscommenu.manual.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�}�j���A�����J���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_manual_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.manual.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�}�j���A�����J���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_manual_get_exist_flag()
{
	return Gp_syscom->syscommenu.manual.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�o�[�W���������J���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_version_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.version.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�o�[�W���������J���̋��^�s�����擾
// ================================================================
bool tnm_syscom_version_get_enable_flag()
{
	return Gp_syscom->syscommenu.version.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�o�[�W���������J���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_version_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.version.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�o�[�W���������J���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_version_get_exist_flag()
{
	return Gp_syscom->syscommenu.version.exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�� ON/OFF ��ݒ�
// ================================================================
void tnm_syscom_local_extra_switch_set_onoff_flag(int switch_no, bool onoff)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff = onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�� ON/OFF ���擾
// ================================================================
bool tnm_syscom_local_extra_switch_get_onoff_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�� ON/OFF �𔽓]
// ================================================================
void tnm_syscom_local_extra_switch_turn_onoff_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff = !Gp_syscom->syscommenu.local_extra_switch[switch_no].onoff;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_local_extra_switch_set_enable_flag(int switch_no, bool enable)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�̋��^�s�����擾
// ================================================================
bool tnm_syscom_local_extra_switch_get_enable_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].enable;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_local_extra_switch_set_exist_flag(int switch_no, bool exist)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		Gp_syscom->syscommenu.local_extra_switch[switch_no].exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_local_extra_switch_get_exist_flag(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].exist;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp�X�C�b�`���g���邩����
// ================================================================
bool tnm_syscom_local_extra_switch_is_enable(int switch_no)
{
	if (0 <= switch_no && switch_no < TNM_LOCAL_EXTRA_SWITCH_CNT)
		return Gp_syscom->syscommenu.local_extra_switch[switch_no].enable && Gp_syscom->syscommenu.local_extra_switch[switch_no].exist;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̒l��ݒ�
// ================================================================
void tnm_syscom_local_extra_mode_set_value(int mode_no, int value)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode = value;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̒l���擾
// ================================================================
int tnm_syscom_local_extra_mode_get_value(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].mode;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̒l��i�߂�
// ================================================================
void tnm_syscom_local_extra_mode_increment(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)	{
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode ++;
		Gp_syscom->syscommenu.local_extra_mode[mode_no].mode %= Gp_ini->syscommenu.local_extra_mode[mode_no].item_cnt;
	}
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̋��^�s����ݒ�
// ================================================================
void tnm_syscom_local_extra_mode_set_enable_flag(int mode_no, bool enable)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̋��^�s�����擾
// ================================================================
bool tnm_syscom_local_extra_mode_get_enable_flag(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].enable;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_local_extra_mode_set_exist_flag(int mode_no, bool exist)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		Gp_syscom->syscommenu.local_extra_mode[mode_no].exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h�̕\���^��\�����擾
// ================================================================
bool tnm_syscom_local_extra_mode_get_exist_flag(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].exist;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F���[�J���ėp���[�h���g���邩����
// ================================================================
bool tnm_syscom_local_extra_mode_is_enable(int mode_no)
{
	if (0 <= mode_no && mode_no < TNM_LOCAL_EXTRA_MODE_CNT)
		return Gp_syscom->syscommenu.local_extra_mode[mode_no].enable && Gp_syscom->syscommenu.local_extra_mode[mode_no].exist;

	return false;
}

// ****************************************************************
// �V�X�e���R�}���h�F�L�����Z���̋��^�s����ݒ�
// ================================================================
void tnm_syscom_cancel_set_enable_flag(bool enable)
{
	Gp_syscom->syscommenu.cancel.enable = enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�L�����Z���̋��^�s�����擾
// ================================================================
bool tnm_syscom_cancel_get_enable_flag()
{
	return Gp_syscom->syscommenu.cancel.enable;
}

// ****************************************************************
// �V�X�e���R�}���h�F�L�����Z���̕\���^��\����ݒ�
// ================================================================
void tnm_syscom_cancel_set_exist_flag(bool exist)
{
	Gp_syscom->syscommenu.cancel.exist = exist;
}

// ****************************************************************
// �V�X�e���R�}���h�F�L�����Z���̕\���^��\�����擾
// ================================================================
bool tnm_syscom_cancel_get_exist_flag()
{
	return Gp_syscom->syscommenu.cancel.exist;
}

