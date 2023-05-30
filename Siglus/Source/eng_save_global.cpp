#include	"pch.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"
#include	"engine/eng_twitter.h"

// ****************************************************************
// �Z�[�u�i�R���t�B�O�j
// ================================================================
bool C_tnm_eng::save_config(C_tnm_save_stream& stream)
{
	// �Z�[�u�f�[�^���N���A
	stream.clear();

	// �Z�[�u�X�g���[���Ƀf�[�^���Z�[�u

	// ���[�U�R���t�B�O�F�X�N���[���T�C�Y
	stream.save(m_config.screen_size_mode);
	stream.save(m_config.screen_size_mode_window);
	stream.save(m_config.screen_size_scale);
	stream.save(m_config.screen_size_free);

	// ���[�U�R���t�B�O�F�t���X�N���[���ڍ�
	stream.save(m_config.fullscreen_change_resolution);
	stream.save(m_config.fullscreen_display_cnt);
	stream.save(m_config.fullscreen_display_no);
	stream.save(m_config.fullscreen_resolution_cnt);
	stream.save(m_config.fullscreen_resolution_no);
	stream.save(m_config.fullscreen_resolution);
	stream.save(m_config.fullscreen_mode);
	stream.save(m_config.fullscreen_scale);
	stream.save(m_config.fullscreen_scale_sync_switch);
	stream.save(m_config.fullscreen_move);

	// ���[�U�R���t�B�O�F����
	stream.save(m_config.all_sound_user_volume);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.save(m_config.sound_user_volume[i]);
	}
	stream.save(m_config.play_all_sound_check);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.save(m_config.play_sound_check[i]);
	}

	// ���[�U�R���t�B�O�F�a�f�l�t�F�[�h
	stream.save(m_config.bgmfade_volume);
	stream.save(m_config.bgmfade_use_check);

	// ���[�U�R���t�B�O�F�t�B���^�[�F
	stream.save(m_config.filter_color);

	// ���[�U�R���t�B�O�F�t�H���g
	stream.save(m_config.font_proportional);
	stream.save(m_config.font_name);
	stream.save(m_config.font_shadow);
	stream.save(m_config.font_futoku);

	// ���[�U�R���t�B�O�F�������x
	stream.save(m_config.message_speed);
	stream.save(m_config.message_speed_nowait);

	// ���[�U�R���t�B�O�F�I�[�g���[�h
	stream.save(m_config.auto_mode_onoff);
	stream.save(m_config.auto_mode_moji_wait);
	stream.save(m_config.auto_mode_min_wait);

	// ���[�U�R���t�B�O�F�}�E�X�J�[�\��������
	stream.save(m_config.mouse_cursor_hide_onoff);
	stream.save(m_config.mouse_cursor_hide_time);

	// ���[�U�R���t�B�O�F���Z�Đ�
	stream.save(m_config.jitan_normal_onoff);
	stream.save(m_config.jitan_auto_mode_onoff);
	stream.save(m_config.jitan_msgbk_onoff);
	stream.save(m_config.jitan_speed);

	// ���[�U�R���t�B�O�F�������[�h
	stream.save(m_config.koe_mode);

	// ���[�U�R���t�B�O�F�L�����N�^�[����
	stream.save(m_config.chrkoe.size());
	stream.save(m_config.chrkoe.get(), (int)m_config.chrkoe.size() * sizeof(S_tnm_config_chrkoe));

	// ���[�U�R���t�B�O�F���͂̐F����
	stream.save(m_config.message_chrcolor_flag);

	// ���[�U�R���t�B�O�F�I�u�W�F�N�g�\��
	stream.save(m_config.object_disp_flag.size());
	for (int i = 0; i < (int)m_config.object_disp_flag.size(); i++)	{
		bool tmp = m_config.object_disp_flag[i].value;
		stream.save(tmp);
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp�X�C�b�`
	stream.save(m_config.global_extra_switch_flag.size());
	for (int i = 0; i < (int)m_config.global_extra_switch_flag.size(); i++)	{
		bool tmp = m_config.global_extra_switch_flag[i].value;
		stream.save(tmp);
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp���[�h
	stream.save(m_config.global_extra_mode_flag.size());
	for (int i = 0; i < (int)m_config.global_extra_mode_flag.size(); i++)	{
		int tmp = m_config.global_extra_mode_flag[i];
		stream.save(tmp);
	}

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�
	stream.save(m_config.system.sleep_flag);
	stream.save(m_config.system.no_wipe_anime_flag);
	stream.save(m_config.system.skip_wipe_anime_flag);
	stream.save(m_config.system.no_mwnd_anime_flag);
	stream.save(m_config.system.wheel_next_message_flag);
	stream.save(m_config.system.koe_dont_stop_flag);
	stream.save(m_config.system.skip_unread_message_flag);

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
	stream.save(m_config.system.saveload_alert_flag);

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̃_�u���N���b�N�j
	stream.save(m_config.system.saveload_dblclick_flag);

	// ���[�U�R���t�B�O�F���̑�
	stream.save(m_config.ss_path);
	stream.save(m_config.editor_path);
	stream.save(m_config.koe_path);
	stream.save(m_config.koe_tool_path);

	return true;
}

// ****************************************************************
// ���[�h�i�R���t�B�O�j
// ================================================================
bool C_tnm_eng::load_config(C_tnm_save_stream& stream, int major_version, int minor_version)
{
	if (stream.empty())
		return false;

	// �V�[�N�����Z�b�g
	stream.set_seek();

	// �Z�[�u�X�g���[�����珇�Ƀf�[�^�����[�h

	// ���[�U�R���t�B�O�F�X�N���[���T�C�Y
	if (major_version >= 2 || (major_version == 1 && minor_version >= 3))
	{
		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_mode_window);
		stream.load(m_config.screen_size_scale);
		stream.load(m_config.screen_size_free);
	}
	else if (major_version == 1 && minor_version == 2 && Gp_ini->game_id == _T("planetarian [HD Edition]"))
	{
		// planetarian [HD Edition] �ŗB��A�w�b�_�� v1.2 �Œ��g�� v1.3 �̂��Ƃ�����܂��B

		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_mode_window);
		stream.load(m_config.screen_size_scale);
		stream.load(m_config.screen_size_free);
	}
	else
	{
		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_scale);
	}

	// ���[�U�R���t�B�O�F�t���X�N���[���ڍ�
	stream.load(m_config.fullscreen_change_resolution);
	stream.load(m_config.fullscreen_display_cnt);
	stream.load(m_config.fullscreen_display_no);
	stream.load(m_config.fullscreen_resolution_cnt);
	stream.load(m_config.fullscreen_resolution_no);
	stream.load(m_config.fullscreen_resolution);
	stream.load(m_config.fullscreen_mode);
	stream.load(m_config.fullscreen_scale);
	stream.load(m_config.fullscreen_scale_sync_switch);
	stream.load(m_config.fullscreen_move);

	// ���[�U�R���t�B�O�F����
	stream.load(m_config.all_sound_user_volume);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.load(m_config.sound_user_volume[i]);
	}
	stream.load(m_config.play_all_sound_check);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.load(m_config.play_sound_check[i]);
	}

	// ���[�U�R���t�B�O�F�a�f�l�t�F�[�h
	stream.load(m_config.bgmfade_volume);
	stream.load(m_config.bgmfade_use_check);

	// ���[�U�R���t�B�O�F�t�B���^�[�F
	stream.load(m_config.filter_color);

	// ���[�U�R���t�B�O�F�t�H���g
	stream.load(m_config.font_proportional);
	stream.load(m_config.font_name);
	stream.load(m_config.font_shadow);
	stream.load(m_config.font_futoku);

	// ���[�U�R���t�B�O�F�������x
	stream.load(m_config.message_speed);
	stream.load(m_config.message_speed_nowait);

	// ���[�U�R���t�B�O�F�I�[�g���[�h
	stream.load(m_config.auto_mode_onoff);
	stream.load(m_config.auto_mode_moji_wait);
	stream.load(m_config.auto_mode_min_wait);

	// ���[�U�R���t�B�O�F�}�E�X�J�[�\��������
	stream.load(m_config.mouse_cursor_hide_onoff);
	stream.load(m_config.mouse_cursor_hide_time);

	// ���[�U�R���t�B�O�F���Z�Đ�
	stream.load(m_config.jitan_normal_onoff);
	stream.load(m_config.jitan_auto_mode_onoff);
	stream.load(m_config.jitan_msgbk_onoff);
	stream.load(m_config.jitan_speed);

	// ���[�U�R���t�B�O�F�������[�h
	stream.load(m_config.koe_mode);

	// ���[�U�R���t�B�O�F�L�����N�^�[����
	m_config.chrkoe.resize(stream.load_ret<size_t>());
	stream.load(m_config.chrkoe.get(), (int)m_config.chrkoe.size() * sizeof(S_tnm_config_chrkoe));

	// ���[�U�R���t�B�O�F���͂̐F����
	stream.load(m_config.message_chrcolor_flag);

	// ���[�U�R���t�B�O�F�I�u�W�F�N�g�\��
	m_config.object_disp_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.object_disp_flag.size(); i++)	{
		bool tmp;
		stream.load(tmp);
		m_config.object_disp_flag[i].value = tmp;
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp�X�C�b�`
	m_config.global_extra_switch_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.global_extra_switch_flag.size(); i++)	{
		bool tmp;
		stream.load(tmp);
		m_config.global_extra_switch_flag[i].value = tmp;
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp���[�h
	m_config.global_extra_mode_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.global_extra_mode_flag.size(); i++)	{
		int tmp;
		stream.load(tmp);
		m_config.global_extra_mode_flag[i] = tmp;
	}

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�
	stream.load(m_config.system.sleep_flag);
	stream.load(m_config.system.no_wipe_anime_flag);
	stream.load(m_config.system.skip_wipe_anime_flag);
	stream.load(m_config.system.no_mwnd_anime_flag);
	stream.load(m_config.system.wheel_next_message_flag);
	stream.load(m_config.system.koe_dont_stop_flag);
	stream.load(m_config.system.skip_unread_message_flag);

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
	stream.load(m_config.system.saveload_alert_flag);

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̃_�u���N���b�N�j
	stream.load(m_config.system.saveload_dblclick_flag);

	// ���[�U�R���t�B�O�F���̑�
	stream.load(m_config.ss_path);
	stream.load(m_config.editor_path);
	stream.load(m_config.koe_path);
	stream.load(m_config.koe_tool_path);

	// �Z�[�u�f�[�^���N���A���܂��B
	stream.clear();

	return true;
}

// ****************************************************************
// �Z�[�u�i�O���[�o���j
// ================================================================
bool C_tnm_eng::save_global(C_tnm_save_stream& stream)
{
	// �Z�[�u�f�[�^���N���A
	stream.clear();

	// �Z�[�u�X�g���[���Ƀf�[�^���Z�[�u

	// ���v���C����
	stream.save(m_timer.global_real_time);

	// �t���O
	m_flag.save_global(stream);

	// �b�f�e�[�u��
	m_cg_table.save(stream);

	// �a�f�l�e�[�u��
	m_bgm_table.save(stream);

	// Twitter
	twitter_save_state(stream);

	// �L�����N�^�[����
	stream.save(m_chrkoe.size());
	for (int i = 0; i < (int)m_chrkoe.size(); i++)	{
		stream.save(m_chrkoe[i].name_str);
		stream.save(m_chrkoe[i].look_flag);
	}

	return true;
}

// ****************************************************************
// ���[�h�i�O���[�o���j
// ================================================================
bool C_tnm_eng::load_global(C_tnm_save_stream& stream)
{
	if (stream.empty())
		return false;

	// �V�[�N�����Z�b�g
	stream.set_seek();

	// �Z�[�u�X�g���[�����珇�Ƀf�[�^�����[�h

	// ���v���C����
	stream.load(m_timer.global_real_time);

	// �t���O
	m_flag.load_global(stream);

	// �b�f�e�[�u��
	m_cg_table.load(stream);

	// �a�f�l�e�[�u��
	m_bgm_table.load(stream);

	// Twitter
	twitter_load_state(stream);

	// �O���[�o���V�X�e���F�L�����N�^�[����
	m_chrkoe.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_chrkoe.size(); i++)	{
		stream.load(m_chrkoe[i].name_str);
		stream.load(m_chrkoe[i].look_flag);
	}

	// �Z�[�u�f�[�^���N���A���܂��B
	stream.clear();

	return true;
}

