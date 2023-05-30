#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_system.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F���[�r�[�̍Đ����@
// ================================================================

// ������
void C_cfg_wnd_func_system::on_init_funcfunc()
{
	// �A�C�e���o�^
	p_dlg->bind_control(&ci.grp_system, IDC_GRP_SYSTEM, _T(""), 5);
	p_dlg->bind_control(&ci.grp_system_movie, IDC_GRP_SYSTEM_MOVIE, _T(""), 2);

	p_dlg->bind_control(&ci.chk_system_sleep, IDC_CHK_SYSTEM_SLEEP, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_no_wipe_anime, IDC_CHK_SYSTEM_NO_WIPE_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_skip_wipe_anime, IDC_CHK_SYSTEM_SKIP_WIPE_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_no_mwnd_anime, IDC_CHK_SYSTEM_NO_MWND_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_wheel_next_message, IDC_CHK_SYSTEM_WHEEL_NEXT_MESSAGE, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_koe_dont_stop, IDC_CHK_SYSTEM_KOE_DONT_STOP, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_skip_unread_message, IDC_CHK_SYSTEM_SKIP_UNREAD_MESSAGE, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_play_silent_sound, IDC_CHK_SYSTEM_PLAY_SILENT_SOUND, _T(""), 7);
	p_dlg->bind_control(&ci.btn_system_init, IDC_BTN_SYSTEM_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.rad_system_movie_play_type_mci, IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_MCI, _T(""), 1);
	p_dlg->bind_control(&ci.rad_system_movie_play_type_wmp, IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_WMP, _T(""), 1);
	p_dlg->bind_control(&ci.btn_system_close, IDC_BTN_SYSTEM_CLOSE, _T(""), 3);

	// �������ݒ��A�e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕��𒲐�����
	ci.chk_system_sleep.set_text_adjust_width(Gp_ini->config.system.sleep.str, 20);
	ci.chk_system_no_wipe_anime.set_text_adjust_width(Gp_ini->config.system.no_wipe_anime.str, 20);
	ci.chk_system_skip_wipe_anime.set_text_adjust_width(Gp_ini->config.system.skip_wipe_anime.str, 20);
	ci.chk_system_no_mwnd_anime.set_text_adjust_width(Gp_ini->config.system.no_mwnd_anime.str, 20);
	ci.chk_system_wheel_next_message.set_text_adjust_width(Gp_ini->config.system.wheel_next_message.str, 20);
	ci.chk_system_koe_dont_stop.set_text_adjust_width(Gp_ini->config.system.koe_dont_stop.str, 20);
	ci.chk_system_skip_unread_message.set_text_adjust_width(Gp_ini->config.system.skip_unread_message.str, 20);
	ci.chk_system_play_silent_sound.set_text_adjust_width(Gp_ini->config.system.play_silent_sound.str, 20);

	// �g�p���Ȃ����ڂ��폜���A���ڂ��l�߂�
	struct item_struct
	{
		bool exist;
		C_control* ctrl;
		C_rect rect;
	};
	ARRAY<item_struct>	item;
	item.resize(8);
	item[0].exist = Gp_ini->config.system.sleep.exist;
	item[1].exist = Gp_ini->config.system.no_wipe_anime.exist;
	item[2].exist = Gp_ini->config.system.skip_wipe_anime.exist;
	item[3].exist = Gp_ini->config.system.no_mwnd_anime.exist;
	item[4].exist = Gp_ini->config.system.wheel_next_message.exist;
	item[5].exist = Gp_ini->config.system.koe_dont_stop.exist;
	item[6].exist = Gp_ini->config.system.skip_unread_message.exist;
	item[7].exist = Gp_ini->config.system.play_silent_sound.exist;
	item[0].ctrl = &ci.chk_system_sleep;
	item[1].ctrl = &ci.chk_system_no_wipe_anime;
	item[2].ctrl = &ci.chk_system_skip_wipe_anime;
	item[3].ctrl = &ci.chk_system_no_mwnd_anime;
	item[4].ctrl = &ci.chk_system_wheel_next_message;
	item[5].ctrl = &ci.chk_system_koe_dont_stop;
	item[6].ctrl = &ci.chk_system_skip_unread_message;
	item[7].ctrl = &ci.chk_system_play_silent_sound;
	if (full_mode_flag)	{		// �f�o�b�O�p�t�����[�h
		for (int i = 0; i < (int)item.size(); i++)	{
			item[i].exist = true;
		}
	}
	for (int i = 0; i < (int)item.size(); i++)	{
		item[i].rect = item[i].ctrl->get_rltv_rect();
	}
	int y_rep = 0;
	int last_item_bottom = item[0].rect.top;
	for (int i = 0; i < (int)item.size(); i++)	{
		if (item[i].exist)	{
			item[i].ctrl->add_rltv_pos_with_def_rect(0, -y_rep);
			last_item_bottom = item[i].ctrl->get_rltv_rect().bottom;
		}
		else	{
			item[i].ctrl->set_show(false);		// ����
			if (i < ((int)item.size() - 1))	{	y_rep += item[i + 1].rect.top - item[i].rect.top;	}	// ���̍��ڂƂ̋���
			else							{	y_rep += item[i].rect.height();						}	// �Ō�̍��ڂ͍�
		}
	}
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		ci.grp_system.add_window_size_with_def_size(0, -y_rep);		// �O���[�v�{�b�N�X
		ci.btn_system_init.add_rltv_pos_with_def_rect(0, -y_rep);	// �������{�^��
		ci.btn_system_close.add_rltv_pos_with_def_rect(0, -y_rep);	// ����{�^��
		ci.grp_system_movie.add_rltv_pos_with_def_rect(0, -y_rep);					// ���[�r�[�̍Đ����@�F�O���[�v�{�b�N�X
		ci.rad_system_movie_play_type_mci.add_rltv_pos_with_def_rect(0, -y_rep);	// ���[�r�[�̍Đ����@�F���W�I�{�^���F�l�b�h
		ci.rad_system_movie_play_type_wmp.add_rltv_pos_with_def_rect(0, -y_rep);	// ���[�r�[�̍Đ����@�F���W�I�{�^���F�v�l�o
		p_dlg->add_window_size_with_def_size(0, -y_rep);			// �E�B���h�E�T�C�Y
		p_dlg->send_wm_size();										// ������d�v�I�i�N�����A�E�B���h�E�����T�C�Y���Ȃ������ꍇ�A�A�C�e���̍��W�������̂ŋ����I�Ƀ��T�C�Y������j
	}

	// ���[�r�[�̍Đ����@�̃A�C�e��������
	if (!(Gp_ini->config.exist_mov || full_mode_flag))	{	// �f�o�b�O�p�t�����[�h
		ci.grp_system_movie.set_show(false);
		ci.rad_system_movie_play_type_mci.set_show(false);
		ci.rad_system_movie_play_type_wmp.set_show(false);
		int yy_rep;
		if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
			yy_rep = ci.grp_system_movie.get_rltv_rect().bottom - last_item_bottom;
			ci.btn_system_init.add_rltv_pos_with_def_rect(0, -yy_rep);	// �������{�^��
			ci.btn_system_close.add_rltv_pos_with_def_rect(0, -yy_rep);	// ����{�^��
		}
		else	{
			yy_rep = ci.grp_system_movie.get_rltv_rect().bottom - ci.grp_system.get_rltv_rect().bottom;
		}
		p_dlg->add_window_size_with_def_size(0, -yy_rep);			// �E�B���h�E�T�C�Y
		p_dlg->send_wm_size();										// ������d�v�I�i�N�����A�E�B���h�E�����T�C�Y���Ȃ������ꍇ�A�A�C�e���̍��W�������̂ŋ����I�Ƀ��T�C�Y������j
	}

	// �A�C�e���̐ݒ�
	if (modal_mode_flag)	{ ci.btn_system_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void C_cfg_wnd_func_system::on_close_funcfunc()
{
}

// �R�}���h
bool C_cfg_wnd_func_system::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_SYSTEM_SLEEP:
			Gp_config->system.sleep_flag = ci.chk_system_sleep.get_check();
			return true;

		case IDC_CHK_SYSTEM_NO_WIPE_ANIME:
			Gp_config->system.no_wipe_anime_flag = ci.chk_system_no_wipe_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_SKIP_WIPE_ANIME:
			Gp_config->system.skip_wipe_anime_flag = ci.chk_system_skip_wipe_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_NO_MWND_ANIME:
			Gp_config->system.no_mwnd_anime_flag = ci.chk_system_no_mwnd_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_WHEEL_NEXT_MESSAGE:
			Gp_config->system.wheel_next_message_flag = ci.chk_system_wheel_next_message.get_check();
			return true;

		case IDC_CHK_SYSTEM_KOE_DONT_STOP:
			Gp_config->system.koe_dont_stop_flag = ci.chk_system_koe_dont_stop.get_check();
			return true;

		case IDC_CHK_SYSTEM_SKIP_UNREAD_MESSAGE:
			Gp_config->system.skip_unread_message_flag = ci.chk_system_skip_unread_message.get_check();
			return true;

		case IDC_CHK_SYSTEM_PLAY_SILENT_SOUND:
			Gp_system_config->ds_play_silent_sound_flag = ci.chk_system_play_silent_sound.get_check();
			return true;

		case IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_MCI:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_MCI;
			return true;

		case IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_WMP:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
			return true;

		case IDC_BTN_SYSTEM_INIT:
			init_state();
			return true;

		case IDC_BTN_SYSTEM_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// �ʒm
bool C_cfg_wnd_func_system::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_func_system::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool C_cfg_wnd_func_system::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �u�X�N���[��
bool C_cfg_wnd_func_system::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_cfg_wnd_func_system::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void C_cfg_wnd_func_system::frame_funcfunc()
{
	if (!h_wnd)
		return;

	// �O���ŕύX���ꂽ
	if (Gp_config->system.sleep_flag != ci.chk_system_sleep.get_check())	{
		ci.chk_system_sleep.set_check(Gp_config->system.sleep_flag);
	}
	if (Gp_config->system.no_wipe_anime_flag != ci.chk_system_no_wipe_anime.get_check())	{
		ci.chk_system_no_wipe_anime.set_check(Gp_config->system.no_wipe_anime_flag);
	}
	if (Gp_config->system.skip_wipe_anime_flag != ci.chk_system_skip_wipe_anime.get_check())	{
		ci.chk_system_skip_wipe_anime.set_check(Gp_config->system.skip_wipe_anime_flag);
	}
	if (Gp_config->system.no_mwnd_anime_flag != ci.chk_system_no_mwnd_anime.get_check())	{
		ci.chk_system_no_mwnd_anime.set_check(Gp_config->system.no_mwnd_anime_flag);
	}
	if (Gp_config->system.wheel_next_message_flag != ci.chk_system_wheel_next_message.get_check())	{
		ci.chk_system_wheel_next_message.set_check(Gp_config->system.wheel_next_message_flag);
	}
	if (Gp_config->system.koe_dont_stop_flag != ci.chk_system_koe_dont_stop.get_check())	{
		ci.chk_system_koe_dont_stop.set_check(Gp_config->system.koe_dont_stop_flag);
	}
	if (Gp_config->system.skip_unread_message_flag != ci.chk_system_skip_unread_message.get_check())	{
		ci.chk_system_skip_unread_message.set_check(Gp_config->system.skip_unread_message_flag);
	}
	if (Gp_system_config->ds_play_silent_sound_flag != ci.chk_system_play_silent_sound.get_check())	{
		ci.chk_system_play_silent_sound.set_check(Gp_system_config->ds_play_silent_sound_flag);
	}
	if (Gp_system_config->movie_type != get_radio_movie_play_type())	{
		set_radio_movie_play_type();
	}
}

// ������Ԃɖ߂�
void C_cfg_wnd_func_system::init_state()
{
	tnm_init_config_system();

	// �ȉ����_�C�A���O�ɑ��݂��邽�ߓ���ɏ�����
	tnm_init_system_config_play_silent_sound();

	// �_�C�A���O���X�V
	update_dialog();
}

// �_�C�A���O�X�V
void C_cfg_wnd_func_system::update_dialog()
{
	if (!h_wnd)
		return;

	ci.chk_system_sleep.set_check(Gp_config->system.sleep_flag);
	ci.chk_system_no_wipe_anime.set_check(Gp_config->system.no_wipe_anime_flag);
	ci.chk_system_skip_wipe_anime.set_check(Gp_config->system.skip_wipe_anime_flag);
	ci.chk_system_no_mwnd_anime.set_check(Gp_config->system.no_mwnd_anime_flag);
	ci.chk_system_wheel_next_message.set_check(Gp_config->system.wheel_next_message_flag);
	ci.chk_system_koe_dont_stop.set_check(Gp_config->system.koe_dont_stop_flag);
	ci.chk_system_skip_unread_message.set_check(Gp_config->system.skip_unread_message_flag);
	set_radio_movie_play_type();
}

// ****************************************************************
// ���[�r�[�̍Đ����@�̃��W�I�{�^����ݒ�
// ================================================================
void C_cfg_wnd_func_system::set_radio_movie_play_type()
{
	if (!h_wnd)
		return;

	ci.rad_system_movie_play_type_mci.set_check(false);
	ci.rad_system_movie_play_type_wmp.set_check(false);
	switch (Gp_system_config->movie_type)	{
		case TNM_MOVIE_PLAY_TYPE_MCI:	ci.rad_system_movie_play_type_mci.set_check(true);	break;
		case TNM_MOVIE_PLAY_TYPE_WMP:	ci.rad_system_movie_play_type_wmp.set_check(true);	break;
	}
}

// ****************************************************************
// ���[�r�[�̍Đ����@�̃��W�I�{�^�����擾
// ================================================================
int C_cfg_wnd_func_system::get_radio_movie_play_type()
{
	if (!h_wnd)
		return -1;

	if (ci.rad_system_movie_play_type_mci.get_check())		{	return TNM_MOVIE_PLAY_TYPE_MCI;	}
	else if (ci.rad_system_movie_play_type_wmp.get_check())	{	return TNM_MOVIE_PLAY_TYPE_WMP;	}
	return -1;
}


