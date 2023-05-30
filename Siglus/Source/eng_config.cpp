#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_system.h"
#include	"engine/eng_display.h"
#include	"engine/eng_config.h"

// ****************************************************************
// ���[�U�[�R���t�B�O�F������
// ================================================================

// �S�ăt�H�[�}�b�g�i�N�����̏������͂�����j
void tnm_format_config()
{
	tnm_init_config_screen_mode();					// ��ʃ��[�h
	tnm_init_config_fullscreen();					// �t���X�N���[���̏ڍאݒ�
	tnm_init_config_volume();						// ����
	tnm_init_config_bgmfade();						// �a�f�l�t�F�[�h
	tnm_init_config_filter();						// �t�B���^�[�F
	tnm_init_config_font();							// �t�H���g
	tnm_init_config_message_speed();				// �������x
	tnm_init_config_auto_mode();					// �I�[�g���[�h
	tnm_init_config_mouse_cursor_hide();			// �����Ń}�E�X�J�[�\�����B��
	tnm_init_config_jitan();						// ���Z�Đ�
	tnm_init_config_koe_mode();						// �������[�h
	tnm_init_config_chrkoe();						// �L�����N�^�[����
	tnm_init_config_message_chrcolor();				// ���͂̐F����
	tnm_init_config_object_disp();					// �I�u�W�F�N�g�\��
	tnm_init_config_global_extra_switch();			// �O���[�o���ėp�X�C�b�`
	tnm_init_config_global_extra_mode();			// �O���[�o���ėp���[�h
	tnm_init_config_system();						// �V�X�e���ݒ�
	tnm_init_config_system_saveload_alert();		// �V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
	tnm_init_config_system_saveload_dblclick();		// �V�X�e���ݒ�i�Z�[�u���[�h�̃_�u���N���b�N�j
}

// ��ʃ��[�h
void tnm_init_config_screen_mode()
{
	Gp_config->screen_size_mode = Gp_ini->config.screen_size_mode;
	Gp_config->screen_size_mode_window = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_config->screen_size_scale = Gp_ini->config.screen_size_scale;
	Gp_config->screen_size_free = Gp_ini->screen_size;
	tnm_rep_screen_size(Gp_config->screen_size_mode, &Gp_config->screen_size_scale, &Gp_config->screen_size_free);
}

// �t���X�N���[���̏ڍאݒ�
void tnm_init_config_fullscreen()
{
	Gp_config->fullscreen_change_resolution = false;
	Gp_config->fullscreen_display_cnt = (int)Gp_system_info->display_list.size();
	Gp_config->fullscreen_display_no = 0;
	Gp_config->fullscreen_resolution_cnt = (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size();
	Gp_config->fullscreen_resolution_no = (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	Gp_config->fullscreen_resolution = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list[Gp_config->fullscreen_resolution_no].resolution;
	Gp_config->fullscreen_mode = Gp_ini->config.fullscreen_mode;
	Gp_config->fullscreen_scale = Gp_ini->config.fullscreen_scale;
	Gp_config->fullscreen_scale_sync_switch = Gp_ini->config.fullscreen_scale_sync_switch;
	Gp_config->fullscreen_move = Gp_ini->config.fullscreen_move;
}

// ����
void tnm_init_config_volume()
{
	// �S�̃{�����[��
	Gp_config->all_sound_user_volume = Gp_ini->config.all_user_volume;
	Gp_config->play_all_sound_check = Gp_ini->config.play_all_check;

	// ��������S�Ẵ`�����l����������
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		Gp_config->sound_user_volume[i] = 255;
		Gp_config->play_sound_check[i] = true;
	}

	// ini �Őݒ�ł�����̂��ʏ�����
	Gp_config->sound_user_volume[TNM_VOLUME_TYPE_BGM] = Gp_ini->config.bgm_user_volume;
	Gp_config->sound_user_volume[TNM_VOLUME_TYPE_KOE] = Gp_ini->config.koe_user_volume;
	Gp_config->sound_user_volume[TNM_VOLUME_TYPE_PCM] = Gp_ini->config.pcm_user_volume;
	Gp_config->sound_user_volume[TNM_VOLUME_TYPE_SE] = Gp_ini->config.se_user_volume;
	Gp_config->sound_user_volume[TNM_VOLUME_TYPE_MOV] = Gp_ini->config.mov_user_volume;
	Gp_config->play_sound_check[TNM_VOLUME_TYPE_BGM] = Gp_ini->config.play_bgm_check;
	Gp_config->play_sound_check[TNM_VOLUME_TYPE_KOE] = Gp_ini->config.play_koe_check;
	Gp_config->play_sound_check[TNM_VOLUME_TYPE_PCM] = Gp_ini->config.play_pcm_check;
	Gp_config->play_sound_check[TNM_VOLUME_TYPE_SE] = Gp_ini->config.play_se_check;
	Gp_config->play_sound_check[TNM_VOLUME_TYPE_MOV] = Gp_ini->config.play_mov_check;
}

// �a�f�l�t�F�[�h
void tnm_init_config_bgmfade()
{
	Gp_config->bgmfade_volume = Gp_ini->config.bgmfade_volume;
	Gp_config->bgmfade_use_check = Gp_ini->config.bgmfade_use_check;
}

// �t�B���^�[�F
void tnm_init_config_filter()
{
	Gp_config->filter_color = Gp_ini->config.filter_color;
}

// �t�H���g
void tnm_init_config_font()
{
	Gp_config->font_proportional = false;
	Gp_config->font_name = Gp_ini->config.font_name;
	Gp_config->font_futoku = Gp_ini->config.font_futoku;
	Gp_config->font_shadow = Gp_ini->config.font_shadow;
}

// �������x
void tnm_init_config_message_speed()
{
	Gp_config->message_speed = Gp_ini->config.message_speed;
	Gp_config->message_speed_nowait = Gp_ini->config.message_speed_nowait;
}

// �I�[�g���[�h
void tnm_init_config_auto_mode()
{
	Gp_config->auto_mode_onoff = Gp_ini->config.auto_mode_onoff;
	Gp_config->auto_mode_moji_wait = Gp_ini->config.auto_mode_moji_wait;
	Gp_config->auto_mode_min_wait = Gp_ini->config.auto_mode_min_wait;
}

// �����Ń}�E�X�J�[�\��������
void tnm_init_config_mouse_cursor_hide()
{
	Gp_config->mouse_cursor_hide_onoff = Gp_ini->config.mouse_cursor_hide_onoff;
	Gp_config->mouse_cursor_hide_time = Gp_ini->config.mouse_cursor_hide_time;
}

// ���Z�Đ�
void tnm_init_config_jitan()
{
	Gp_config->jitan_normal_onoff = Gp_ini->config.jitan_normal_onoff;
	Gp_config->jitan_auto_mode_onoff = Gp_ini->config.jitan_auto_mode_onoff;
	Gp_config->jitan_msgbk_onoff = Gp_ini->config.jitan_msgbk_onoff;
	Gp_config->jitan_speed = Gp_ini->config.jitan_speed;
}

// �������[�h
void tnm_init_config_koe_mode()
{
	Gp_config->koe_mode = Gp_ini->config.koe_mode;
}

// �L�����N�^�[����
void tnm_init_config_chrkoe()
{
	Gp_config->chrkoe.clear();
	if (Gp_ini->config.chrkoe_cnt > 0)	{	Gp_config->chrkoe.resize(Gp_ini->config.chrkoe_cnt);	}	// �o�b�t�@���m�ۂ���
	for (int i = 0; i < (int)Gp_config->chrkoe.size(); i++)	{
		Gp_config->chrkoe[i].onoff = Gp_ini->config.chrkoe[i].onoff;
		Gp_config->chrkoe[i].volume = Gp_ini->config.chrkoe[i].volume;
	}
}

// ���͂̐F����
void tnm_init_config_message_chrcolor()
{
	Gp_config->message_chrcolor_flag = Gp_ini->config.message_chrcolor.onoff;
}

// �I�u�W�F�N�g�\��
void tnm_init_config_object_disp()
{
	Gp_config->object_disp_flag.clear();
	if (Gp_ini->config.object_disp_cnt > 0)	{	Gp_config->object_disp_flag.resize(Gp_ini->config.object_disp_cnt);	}	// �o�b�t�@���m�ۂ���
	for (int i = 0; i < (int)Gp_config->object_disp_flag.size(); i++)	{
		Gp_config->object_disp_flag[i].value = Gp_ini->config.object_disp[i].onoff;
	}
}

// �O���[�o���ėp�X�C�b�`
void tnm_init_config_global_extra_switch()
{
	Gp_config->global_extra_switch_flag.clear();
	if (Gp_ini->config.global_extra_switch_cnt > 0)	{	Gp_config->global_extra_switch_flag.resize(Gp_ini->config.global_extra_switch_cnt);	}	// �o�b�t�@���m�ۂ���
	for (int i = 0; i < (int)Gp_config->global_extra_switch_flag.size(); i++)	{
		Gp_config->global_extra_switch_flag[i].value = Gp_ini->config.global_extra_switch[i].onoff;
	}
}

// �O���[�o���ėp���[�h
void tnm_init_config_global_extra_mode()
{
	Gp_config->global_extra_mode_flag.clear();
	if (Gp_ini->config.global_extra_mode_cnt > 0)	{	Gp_config->global_extra_mode_flag.resize(Gp_ini->config.global_extra_mode_cnt);	}	// �o�b�t�@���m�ۂ���
	for (int i = 0; i < (int)Gp_config->global_extra_mode_flag.size(); i++)	{
		Gp_config->global_extra_mode_flag[i] = Gp_ini->config.global_extra_mode[i].mode;
	}
}

// �V�X�e���ݒ�
void tnm_init_config_system()
{
	Gp_config->system.sleep_flag = Gp_ini->config.system.sleep.onoff;
	Gp_config->system.no_wipe_anime_flag = Gp_ini->config.system.no_wipe_anime.onoff;
	Gp_config->system.skip_wipe_anime_flag = Gp_ini->config.system.skip_wipe_anime.onoff;
	Gp_config->system.no_mwnd_anime_flag = Gp_ini->config.system.no_mwnd_anime.onoff;
	Gp_config->system.wheel_next_message_flag = Gp_ini->config.system.wheel_next_message.onoff;
	Gp_config->system.koe_dont_stop_flag = Gp_ini->config.system.koe_dont_stop.onoff;
	Gp_config->system.skip_unread_message_flag = Gp_ini->config.system.skip_unread_message.onoff;
}

// �V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
void tnm_init_config_system_saveload_alert()
{
	Gp_config->system.saveload_alert_flag = Gp_ini->config.system.saveload_alert_onoff;
}

// �V�X�e���ݒ�i�Z�[�u���[�h�̃_�u���N���b�N�j
void tnm_init_config_system_saveload_dblclick()
{
	Gp_config->system.saveload_dblclick_flag = Gp_ini->config.system.saveload_dblclick_onoff;
}


// ���V�X�e���ݒ�i�������Đ�����j
void tnm_init_system_config_play_silent_sound()
{
	Gp_system_config->ds_play_silent_sound_flag = false;		// �����l�Ffalse �ŌŒ�B
}

// ���V�X�e���ݒ�i���[�r�[�̍Đ����@�j
void tnm_init_system_config_movie_play_type()
{
	Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_WMP;		// �����l�FWMP �ŌŒ�B
}
