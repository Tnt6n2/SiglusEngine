#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dlg_manager.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// �S�Ẵ��W�I�{�^������t�H�[�J�X���O��
// ================================================================
void tnm_dlg_kill_focus_for_all_radio()
{
	for (int i = 0; i < (int)Gp_dlg_mng->dlg_list.size(); i++)	{
		Gp_dlg_mng->dlg_list[i]->kill_focus_for_radio_item();
	}
	for (ARRAY<C_cfg_wnd_solo *>::iterator itr = Gp_dlg_mng->cfg_wnd_config_base.get_child_window_list_begin(); itr != Gp_dlg_mng->cfg_wnd_config_base.get_child_window_list_end(); ++itr)	{
		(*itr)->kill_focus_for_radio_item();
	}
	for (ARRAY<C_cfg_wnd_solo *>::iterator itr = Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_all_child_window_list_begin(); itr != Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_all_child_window_list_end(); ++itr)	{
		(*itr)->kill_focus_for_radio_item();
	}
}

// ****************************************************************
// ���b�Z�[�W�{�b�N�X
// ================================================================
int tnm_game_message_box(CTSTR& msg, UINT type)
{
	// �S�Ẵ_�C�A���O�ɑ΂��ă��W�I�{�^���̃t�H�[�J�X���͂���
	// ���b�Z�[�W�{�b�N�X���o��O�ɏ�������K�v������܂��B
	tnm_dlg_kill_focus_for_all_radio();

	// �_�C�A���O�𖳌��ɂ��Ă��烁�b�Z�[�W�{�b�N�X���o���܂��B
	tnm_dlg_set_enable(false);
	int res = message_box(msg, Gp_ini->game_name, type);
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// �m�F���b�Z�[�W
// ================================================================
int tnm_game_warning_box(CTSTR& msg)
{
	// �S�Ẵ_�C�A���O�ɑ΂��ă��W�I�{�^���̃t�H�[�J�X���͂���
	// ���b�Z�[�W�{�b�N�X���o��O�ɏ�������K�v������܂��B
	tnm_dlg_kill_focus_for_all_radio();

	// �_�C�A���O�𖳌��ɂ��Ă��烁�b�Z�[�W�{�b�N�X���o���܂��B
	tnm_dlg_set_enable(false);
	int res = warning_box(msg + _T("�@�@�@"), Gp_ini->game_name, MB_YESNO);		// �S�p�X�y�[�X���R����ăE�B���h�E�T�C�Y�𒲐�����
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// ��񃁃b�Z�[�W
// ================================================================
int tnm_game_info_box(CTSTR& msg)
{
	// �S�Ẵ_�C�A���O�ɑ΂��ă��W�I�{�^���̃t�H�[�J�X���͂���
	// ���b�Z�[�W�{�b�N�X���o��O�ɏ�������K�v������܂��B
	tnm_dlg_kill_focus_for_all_radio();

	// �_�C�A���O�𖳌��ɂ��Ă��烁�b�Z�[�W�{�b�N�X���o���܂��B
	tnm_dlg_set_enable(false);
	int res = info_box(msg + _T("�@�@�@"), Gp_ini->game_name, MB_OK);			// �S�p�X�y�[�X���R����ăE�B���h�E�T�C�Y�𒲐�����
	tnm_dlg_set_enable(true);

	return res;
}

// ****************************************************************
// �S�Ẵ_�C�A���O�ɑ΂��ċ��^�֎~��ݒ肷��
// ================================================================
void tnm_dlg_set_enable(bool enable)
{
	Gp_dlg_mng->set_enable(enable);
}

// ****************************************************************
// �}�E�X���_�C�A���O�ɏ���Ă��邩�𔻒肷��
// ================================================================
bool tnm_dlg_check_mouse_over()
{
	return Gp_dlg_mng->check_mouse_over();
}

// ****************************************************************
// ���ݒ�_�C�A���O���J���Ă��邩�𔻒肷��
// ================================================================
bool tnm_dlg_is_config_dialog_opened()
{
	return Gp_dlg_mng->cfg_wnd_config_base.get_handle() ? true : false;
}

// ****************************************************************
// ���ݒ�_�C�A���O���J��
// ================================================================
void tnm_dlg_open_config_dialog()
{
	if (!Gp_dlg_mng->cfg_wnd_config_base.get_handle())	{
		Gp_dlg_mng->cfg_wnd_config_base.open();
	}
}

// ****************************************************************
// �f�o�b�O�ݒ�_�C�A���O���J��
// ================================================================
void tnm_dlg_open_debug_setting_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_debug.get_handle())	{
		Gp_dlg_mng->db_wnd_info_debug.open();
	}
}

// ****************************************************************
// �g���[�X�_�C�A���O���J��
// ================================================================
void tnm_dlg_open_trace_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_trace.get_handle())	{
		Gp_dlg_mng->db_wnd_info_trace.open();
	}
}

// ****************************************************************
// �G���[���_�C�A���O���J��
// ================================================================
void tnm_dlg_open_error_info_dialog()
{
	if (!Gp_dlg_mng->db_wnd_info_error.get_handle())	{
		Gp_dlg_mng->db_wnd_info_error.open();
	}
}

// ****************************************************************
// Twitter �_�C�A���O���J��
// ================================================================
void tnm_dlg_open_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.open();
}

// ****************************************************************
// Twitter �_�C�A���O�����
// ================================================================
void tnm_dlg_close_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.close();
}

// ****************************************************************
// Twitter �_�C�A���O���X�V����
// ================================================================
void tnm_dlg_update_twitter_dialog()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.update_state();
}

// ****************************************************************
// Twitter ���e�ɐ���
// ================================================================
void tnm_dlg_twitter_dialog_on_success_tweet()
{
	Gp_dlg_mng->sys_wnd_solo_tweet.on_success_tweet();
}
