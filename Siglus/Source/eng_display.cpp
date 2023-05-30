#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_icon.h"

#include	"engine/eng_system.h"
#include	"engine/eng_display.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_editbox.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_emote.h"

#include	"localize/localize.h"

// ****************************************************************
// �X�N���[���T�C�Y��ύX�J�n����
// ================================================================
static int G_change_screen_semaphore = 0;

bool tnm_change_screen_size_start(bool draw_black)
{
	// �N�������������s���Ȃ̂ŏ������L�����Z��
	// ���̊֐����� Sleep ���ɁA���ݒ�_�C�A���O�́u�E�B���h�E�v�{�^�����������Ƃ��p�̏����ł��B
	if (G_change_screen_semaphore > 0)
		return false;

	// �ȈՃZ�}�t�H�𗧂Ă܂��B
	G_change_screen_semaphore ++;

	// �G�f�B�b�g�{�b�N�X��S�ĉB���܂��B�i���A�����̓t���[�������ł���Ă���܂��B�j
	tnm_editbox_set_hide_all();

	if (draw_black)	{

		// ��ʂ����ɃN���A���܂��B
		Gp_wnd->m_view.clear_render_target();

		// �ĕ`��������܂��B
		Gp_wnd->invalidate_rect(NULL, false);
		G_app.default_message_loop();
	}

	// E-mote ��������܂��B
#if __USE_EMOTE
	tnm_emote_on_device_lost();
#endif

	// Direct3D �G�t�F�N�g���ꎞ������܂��B
	if (!Gp_wnd->m_view.m_default_effect.on_lost_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�G�t�F�N�g�̉���Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// Direct3D �e�L�X�g���ꎞ������܂��B
	if (!G_debug_text.on_lost_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�e�L�X�g�̉���Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// Direct3D ���\�[�X���ꎞ������܂��B
	G_rsm.release();

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y��ύX�I������
// ================================================================
bool tnm_change_screen_size_end_sub()
{
	// �ĕ`��������܂��B

	// ���������R�����g�A�E�g���܂����B�i2010/4/23�j
	// ������ WM_CLOSE �����������ƁA�f�o�C�X�̕����Ɏ��s���ė����܂��B
	//Gp_wnd->invalidate_rect(NULL, false);
	//Gapp.default_message_loop();

	// Direct3D �X�N���[���T�C�Y��ݒ肵�܂��B
	if (!G_d3d.device.change_screen_size(Gp_global->total_screen_size))
		return false;

	// Direct3D �f�o�C�X����������܂ŌJ��Ԃ�
	while (1)	{

		// �f�o�C�X�𕜌����܂��B
		if (!G_d3d.device.restore())
			throw _T("�f�o�C�X�̕����Ɏ��s���܂����B\n") + get_last_error_msg();		// �v���I�G���[�I

		// �f�o�C�X���������Ă���ꍇ�͍ēx�J��Ԃ��܂��B
		if (G_d3d.device.is_lost())	{
			Sleep(100);
			continue;
		}

		// ����
		break;
	}

	// Direct3D �X�e�[�g�����������܂��B
	G_d3d.state.init();

	// Direct3D �X�e�[�g�������ݒ���s���܂��B
	if (!G_d3d.state.set_template())
		return false;

	// Direct3D ���\�[�X�𕜌����܂��B
	if (!G_rsm.resume())
		return false;

	// Direct3D �e�L�X�g�𕜌����܂��B
	if (!G_debug_text.on_reset_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�e�L�X�g�̕����Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// Direct3D �G�t�F�N�g�𕜌����܂��B
	if (!Gp_wnd->m_view.m_default_effect.on_reset_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�G�t�F�N�g�̕����Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �x��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("��Direct3D �f�o�C�X�𕜌����܂����B"));

	return true;
}

bool tnm_change_screen_size_end()
{
	bool res = tnm_change_screen_size_end_sub();

	// �ȈՃZ�}�t�H���~�낵�܂��B
	G_change_screen_semaphore --;

	return res;
}

// ****************************************************************
// ���ݒ�̃X�N���[�����[�h��␳����
// ================================================================
void tnm_correct_fullscreen_resolution()
{
	// �͈̓`�F�b�N
	if (Gp_config->fullscreen_display_no < 0 || (int)Gp_system_info->display_list.size() <= Gp_config->fullscreen_display_no)	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_no < 0 || (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size() <= Gp_config->fullscreen_resolution_no)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}

	// �l���X�V����
	Gp_config->fullscreen_display_cnt = (int)Gp_system_info->display_list.size();
	Gp_config->fullscreen_resolution_cnt = (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size();
	Gp_config->fullscreen_resolution = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list[Gp_config->fullscreen_resolution_no].resolution;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h���E�B���h�E�ɕύX����
// ================================================================
//		window_pos: �E�B���h�E���[�h�̏ꍇ�̂ݎw��iNULL �Œ����j
//		first: �N����
// ================================================================
bool tnm_update_screen_size_mode_window_sub_sub(C_point* window_pos, bool first)
{
	// �E�B���h�E�X�^�C����ύX���܂��B
	Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	Gp_wnd->set_style(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME);
	Gp_wnd->set_top_most(false);

	// �X�N���[���T�C�Y�␳
	tnm_rep_screen_size(TNM_SCREEN_SIZE_MODE_WINDOW, &Gp_config->screen_size_scale, &Gp_config->screen_size_free);

	// �X�N���[���T�C�Y�����߂܂��B
	C_size screen_size;
	screen_size.cx = Gp_global->game_screen_size.cx * Gp_config->screen_size_scale.cx / 100;
	screen_size.cy = Gp_global->game_screen_size.cy * Gp_config->screen_size_scale.cy / 100;

	// �E�B���h�E�T�C�Y��ݒ肵�܂��B
	Gp_wnd->set_client_size(screen_size);

	// �E�B���h�E�ʒu���w�肵�܂��B
	// �w�肪�Ȃ��ꍇ�́A�E�B���h�E�����݂��郂�j�^�̒����ɔz�u���܂��B
	// �N������ ini �ɂ��킹��̂ł�����s���܂���B

	if (first)	{}	// NOP
	else if (window_pos)	{
		Gp_wnd->set_window_pos(*window_pos);
	}
	else	{
		Gp_wnd->set_pos_my_desktop_center();
	}

	// ��������΃X�N���[���T�C�Y��ݒ肵�܂��B
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_global->screen_size_mode_window = Gp_config->screen_size_mode_window;
	Gp_global->screen_size_scale = Gp_config->screen_size_scale;
	Gp_global->screen_size_free = Gp_config->screen_size_free;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B

	return true;
}

bool tnm_update_screen_size_mode_window_sub(C_point* window_pos, bool first)
{
	// �𑜓x��ύX���Ă����ꍇ�A���ɖ߂�
	if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
		ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
	}

	// �X�N���[���T�C�Y��ύX
	tnm_update_screen_size_mode_window_sub_sub(window_pos, first);

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h���t���[�ɕύX����
// ================================================================
//		window_pos: �E�B���h�E���[�h�̏ꍇ�̂ݎw��iNULL �Œ����j
//		first: �N����
// ================================================================
bool tnm_update_screen_size_mode_free_sub_sub(C_point* window_pos, bool first)
{
	// �E�B���h�E�X�^�C����ύX���܂��B
	Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	Gp_wnd->set_style(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME);
	Gp_wnd->set_top_most(false);

	// �X�N���[���T�C�Y�␳
	tnm_rep_screen_size(TNM_SCREEN_SIZE_MODE_FREE, &Gp_config->screen_size_scale, &Gp_config->screen_size_free);

	// �E�B���h�E�T�C�Y��ݒ肵�܂��B
	Gp_wnd->set_client_size(Gp_config->screen_size_free);

	// �E�B���h�E�ʒu���w�肵�܂��B
	// �w�肪�Ȃ��ꍇ�́A�E�B���h�E�����݂��郂�j�^�̒����ɔz�u���܂��B
	// �N������ ini �ɂ��킹��̂ł�����s���܂���B

	if (first)	{}	// NOP
	else if (window_pos)	{
		Gp_wnd->set_window_pos(*window_pos);
	}
	else	{
		Gp_wnd->set_pos_my_desktop_center();
	}

	// ��������΃X�N���[���T�C�Y��ݒ肵�܂��B
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_global->screen_size_mode_window = Gp_config->screen_size_mode_window;
	Gp_global->screen_size_scale = Gp_config->screen_size_scale;
	Gp_global->screen_size_free = Gp_config->screen_size_free;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B

	return true;
}

bool tnm_update_screen_size_mode_free_sub(C_point* window_pos, bool first)
{
	// �𑜓x��ύX���Ă����ꍇ�A���ɖ߂�
	if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
	{
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
		ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
	}

	// �X�N���[���T�C�Y��ύX
	tnm_update_screen_size_mode_free_sub_sub(window_pos, first);

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h���t���X�N���[���ɕύX����
// ================================================================
//		first: �N����
// ================================================================
bool tnm_update_screen_size_mode_fullscreen_sub_sub(C_point* window_pos, bool first)
{
	// �E�B���h�E�X�^�C����ύX���܂��B
	Gp_wnd->set_menu(NULL);
	Gp_wnd->set_style(WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN);

#ifndef _DEBUG	// �v���O�������f�o�b�O�łł́A�t���X�N���[���ɂ��Ă��őO�ʂɈړ����Ȃ��悤�ɂ���
//	Gp_wnd->set_top_most(true);		// �őO�ʂɈړ�
#endif

	C_rect client_rect = Gp_wnd->get_client_rect();
	C_rect window_rect = Gp_wnd->client_to_screen(client_rect);

	// ���j�^�̋�`���擾���܂��B
	C_rect desktop_rect;
	if (window_pos)
		get_desktop_rect_from_pos(&desktop_rect, *window_pos);	// �w��̈ʒu�̃��j�^��`���擾
	else
		Gp_wnd->get_my_desktop_rect(&desktop_rect);				// ���j�^�̒��������`���擾

	// �N���C�A���g��`��ݒ肵�܂��B
	Gp_wnd->set_client_rect(desktop_rect);

	// ���������̂ŃV�X�e���f�[�^���X�V���܂��B
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	Gp_global->fullscreen_change_resolution = Gp_config->fullscreen_change_resolution;
	Gp_global->fullscreen_display_no = Gp_config->fullscreen_display_no;
	Gp_global->fullscreen_resolution_no = Gp_config->fullscreen_resolution_no;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// �K���Ď擾�I�ς���Ă��邱�Ƃ�����܂��B

	// �_�C�A���O�ɔ��f������
	Gp_global->dialog_fullscreen_change_resolution = Gp_global->fullscreen_change_resolution;
	Gp_global->dialog_fullscreen_display_no = Gp_global->fullscreen_display_no;
	Gp_global->dialog_fullscreen_resolution_no = Gp_global->fullscreen_resolution_no;

	return true;
}

bool tnm_update_screen_size_mode_fullscreen_sub(C_point* window_pos, bool first, bool warning)
{
	// �P�D���z�t���X�N���[��
	if (!Gp_config->fullscreen_change_resolution)	{

		// �𑜓x��ύX���Ă����ꍇ�A���ɖ߂�
		if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
			if (Gp_global->fullscreen_change_resolution)	{
				if (Gp_global->fullscreen_display_no >= 0)	{
					TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
					ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�𑜓x�����ɖ߂��܂����B�i") + desplay_name + _T("�j"));
				}
			}
		}

		// �P���Ƀt���X�N���[���ɂ���
		tnm_update_screen_size_mode_fullscreen_sub_sub(window_pos, first);
	}

	// �Q�D�𑜓x��ύX����
	else	{

		int display_no = Gp_config->fullscreen_display_no;
		int resolution_no = Gp_config->fullscreen_resolution_no;
		C_size resolution = Gp_config->fullscreen_resolution;

		if (display_no < 0 || (int)Gp_system_info->resolution_table.size() <= display_no)
			return false;

		// ���݂̏�Ԃ��L�����Ă����i�����p�j
		int old_screen_size_mode = Gp_global->screen_size_mode;
		int old_screen_size_mode_window = Gp_global->screen_size_mode_window;
		C_size old_screen_size_scale = Gp_global->screen_size_scale;
		C_size old_screen_size_free = Gp_global->screen_size_free;
		C_point old_window_pos = Gp_wnd->get_window_rect().left_top();
		bool old_change_resolution = Gp_global->fullscreen_change_resolution;
		int old_display_no = Gp_global->fullscreen_display_no;
		int old_resolution_no = Gp_global->fullscreen_resolution_no;

		// �f�t�H���g�̉𑜓x���擾����
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[display_no].DeviceName);
		int default_resolution_no = Gp_system_info->resolution_table[display_no].default_resolution_no;
		C_size default_resolution = Gp_system_info->resolution_table[display_no].resolution_list[default_resolution_no].resolution;

		// ���݂̉𑜓x���擾����
		DEVMODE old_dm = {0};
		old_dm.dmSize = sizeof(DEVMODE);
		if (EnumDisplaySettingsEx(desplay_name.c_str(), ENUM_CURRENT_SETTINGS, &old_dm, EDS_RAWMODE) == 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���݂̉𑜓x���擾�ł��܂���ł����B"));
			return false;
		}

		// �V�����𑜓x���w��
		DEVMODE new_dm = {0};
		new_dm.dmSize = sizeof(new_dm);
		new_dm.dmPelsWidth = resolution.cx;
		new_dm.dmPelsHeight = resolution.cy;
		new_dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

		// �𑜓x���ς��Όx�����o��
		if (warning && (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight))	{

			int res = tnm_game_message_box(LCL_STR_CHANGE_RESOLUTION_WARNING_STR, MB_OKCANCEL | MB_DEFBUTTON2);
			if (res == IDCANCEL)	{

				// �𑜓x�����ɖ߂�
				Gp_config->screen_size_mode = old_screen_size_mode;
				Gp_config->screen_size_mode_window = old_screen_size_mode_window;
				Gp_config->screen_size_scale = old_screen_size_scale;
				Gp_config->screen_size_free = old_screen_size_free;
				Gp_config->fullscreen_change_resolution = old_change_resolution;
				Gp_config->fullscreen_display_no = old_display_no;
				Gp_config->fullscreen_resolution_no = old_resolution_no;
				tnm_correct_fullscreen_resolution();	// �l��␳����

				return true;
			}
		}

		// �Ⴄ�f�B�X�v���C��I�񂾏ꍇ�A�𑜓x�����ɖ߂�
		if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
			if (Gp_global->fullscreen_change_resolution)	{
				if (Gp_global->fullscreen_display_no >= 0 && display_no != Gp_global->fullscreen_display_no)	{
					TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
					ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�𑜓x�����ɖ߂��܂����B�i") + desplay_name + _T("�j"));
				}
			}
		}

		// �𑜓x���ς��ΕύX����
		if (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight)	{

			if (new_dm.dmPelsWidth == default_resolution.cx && new_dm.dmPelsHeight == default_resolution.cy)	{

				if (ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�𑜓x�̕ύX�Ɏ��s���܂����B"));
					return false;
				}

				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�𑜓x��ύX���܂����B�i�f�t�H���g�j") + desplay_name + _T("�j"));
			}
			else	{

				if (ChangeDisplaySettingsEx(desplay_name.c_str(), &new_dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�𑜓x�̕ύX�Ɏ��s���܂����B"));
					return false;
				}

				C_size resolution = C_size(new_dm.dmPelsWidth, new_dm.dmPelsHeight);
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�𑜓x��ύX���܂����B�i") + tostr(resolution) + _T("�j") + desplay_name + _T("�j"));
			}
		}

		// �V�����𑜓x�ɂ��ďڍׂ��󂯎��
		if (EnumDisplaySettingsEx(desplay_name.c_str(), ENUM_CURRENT_SETTINGS, &new_dm, EDS_RAWMODE) == 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�V�����𑜓x���Ď擾�ł��܂���ł����B"));
			return false;
		}

		// �w��̃��j�^�Ńt���X�N���[���ɂ���
		tnm_update_screen_size_mode_fullscreen_sub_sub(&C_point(new_dm.dmPosition.x, new_dm.dmPosition.y), first);

		// �𑜓x���ς���Ă���Ƃ��̂݊m�F�_�C�A���O���o��
		// ��������I������Ɖ𑜓x�����ɖ߂���
		if (warning && (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight))	{

			// �����Ŕ����Ĉ�U�`��ɓ���܂��B
			// ���̃t���[���Ŋm�F�_�C�A���O �� �u�������v�Ȃ猳�ɖ߂��������s���܂��B

			// ���������s�����߂̏���
			Gp_global->change_display_mode_proc_cnt = 2;	// �Q�t���[���܂킷�i�P�t���[���ځ��`��A�Q�t���[���ڂŊm�F�_�C�A���O�j
			Gp_global->change_display_mode_proc_old_screen_size_mode = old_screen_size_mode;
			Gp_global->change_display_mode_proc_old_screen_size_mode_window = old_screen_size_mode_window;
			Gp_global->change_display_mode_proc_old_screen_size_scale = old_screen_size_scale;
			Gp_global->change_display_mode_proc_old_screen_size_free = old_screen_size_free;
			Gp_global->change_display_mode_proc_old_window_pos = old_window_pos;
			Gp_global->change_display_mode_proc_old_change_resolution = old_change_resolution;
			Gp_global->change_display_mode_proc_old_display_no = old_display_no;
			Gp_global->change_display_mode_proc_old_resolution_no = old_resolution_no;

			// �Q�[���^�C�}�[��i�߂Ȃ�
			Gp_global->alt_menu_wait_flag = true;
		}
	}

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h��ύX����
// ================================================================
//		�N�����A�������͉𑜓x�ύX�����ɖ߂��Ƃ��Ɏg���Ă��܂��B
//		�E�B���h�E�ʒu���w��ł��܂��B
// ================================================================
bool tnm_update_screen_size_mode_sub(C_point* window_pos, bool first, bool warning)
{
	// �E�B���h�E�^�t���X�N���[����K�p
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)	{
		return tnm_update_screen_size_mode_window_sub(window_pos, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)	{
		return tnm_update_screen_size_mode_free_sub(window_pos, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		return tnm_update_screen_size_mode_fullscreen_sub(window_pos, first, warning);
	}

	return false;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h��ύX�F�Ăяo����
// ================================================================
//		�J�n �� �ύX �� �I���@�̗�����s���܂��B
// ================================================================

// �E�B���h�E�T�C�Y���ς�����Ƃ��̏����i�����j
void tnm_change_screen_size_mode_by_force(bool first)
{
	// �𑜓x�ύX���͏������Ȃ�
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))
		return;

	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)	{
		tnm_update_screen_size_mode_window_sub(NULL, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)	{
		tnm_update_screen_size_mode_free_sub(NULL, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		tnm_update_screen_size_mode_fullscreen_sub(NULL, first, false);
	}

	tnm_change_screen_size_end();
}

// �u�E�B���h�E�v�{�^�����������Ƃ��̏����i�E�B���h�E�܂��̓t���[�ɂȂ�܂��j
void tnm_change_screen_size_mode_window(bool first)
{
	// �𑜓x�ύX���͏������Ȃ�
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// ���݂Ɠ����l�Ȃ珈�����Ȃ�
	if (Gp_config->screen_size_mode == Gp_config->screen_size_mode_window && Gp_config->screen_size_scale == Gp_global->screen_size_scale)
		return;

	// ���ݒ�̒l���X�V����
	Gp_config->screen_size_mode = Gp_config->screen_size_mode_window;
	Gp_config->screen_size_scale = Gp_config->screen_size_scale;

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))	{ return; }

	switch (Gp_config->screen_size_mode)
	{
	case TNM_SCREEN_SIZE_MODE_WINDOW:
		tnm_update_screen_size_mode_window_sub(NULL, first);
		break;
	case TNM_SCREEN_SIZE_MODE_FREE:
		tnm_update_screen_size_mode_free_sub(NULL, first);
		break;
	}

	tnm_change_screen_size_end();
}

// �u50%�v�Ȃǂ̃{�^�����������Ƃ��̏����i�E�B���h�E�ɂȂ�܂��j
void tnm_change_screen_size_mode_window(C_size screen_size_scale, bool first)
{
	// �𑜓x�ύX���͏������Ȃ�
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// ���݂Ɠ����l�Ȃ珈�����Ȃ�
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW && Gp_config->screen_size_scale == screen_size_scale)
		return;

	// ���ݒ�̒l���X�V����
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_config->screen_size_mode_window = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_config->screen_size_scale = screen_size_scale;

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_window_sub(NULL, first);
	tnm_change_screen_size_end();
}

// �u�t���X�N���[���v�{�^�����������Ƃ��̏���
void tnm_change_screen_size_mode_fullscreen(bool first, bool warning)
{
	// �𑜓x�ύX���͏������Ȃ�
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// ���݂Ɠ����l�Ȃ珈�����Ȃ�
	if (
		Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL && 
		Gp_config->fullscreen_change_resolution == Gp_global->dialog_fullscreen_change_resolution && 
		Gp_config->fullscreen_display_no == Gp_global->dialog_fullscreen_display_no && 
		Gp_config->fullscreen_resolution_no == Gp_global->dialog_fullscreen_resolution_no
		)
	{
		return;
	}

	// ���ݒ�̒l���X�V����
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW || Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
	{
		Gp_config->screen_size_mode_window = Gp_config->screen_size_mode;
	}
	Gp_config->fullscreen_change_resolution = Gp_global->dialog_fullscreen_change_resolution;
	Gp_config->fullscreen_display_no = Gp_global->dialog_fullscreen_display_no;
	Gp_config->fullscreen_resolution_no = Gp_global->dialog_fullscreen_resolution_no;
	tnm_correct_fullscreen_resolution();	// �l��␳����

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_fullscreen_sub(NULL, first, warning);	// ���j�^�̒��S����Ƀt���X�N���[���ɂ���
	tnm_change_screen_size_end();
}

// �uAlt+Enter�v���������Ƃ��̏���
void tnm_switch_screen_size_mode(bool first)
{
	// �t���X�N���[���Ȃ�E�B���h�E��
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
	{
		tnm_change_screen_size_mode_window(first);
	}
	// �E�B���h�E�Ȃ�t���X�N���[����
	else
	{
		tnm_change_screen_size_mode_fullscreen(first, true);
	}
}

// �E�B���h�E�T�C�Y��ύX�����Ƃ��̏���
void tnm_change_screen_size_mode_free(C_point window_pos, C_size screen_size_free, bool first)
{
	// �𑜓x�ύX���͏������Ȃ�
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// ���݂Ɠ����l�Ȃ珈�����Ȃ�
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE && Gp_config->screen_size_free == screen_size_free)
		return;

	// ���ݒ�̒l���X�V����
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_config->screen_size_mode_window = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_config->screen_size_free = screen_size_free;

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_free_sub(&window_pos, first);
	tnm_change_screen_size_end();
}

// ****************************************************************
// �X�N���[�����[�h���X�V
// ================================================================
//		�Q�[�����������͂�����ʂ�܂��B
//		�O��̋N������f�B�X�v���C�̌��Ȃǂ��ς�����ꍇ�͕ύX�����������܂��B
// ================================================================
void tnm_update_screen_size_mode()
{
	if (Gp_config->fullscreen_display_cnt != (int)Gp_system_info->display_list.size())	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_display_no < 0 || (int)Gp_system_info->display_list.size() <= Gp_config->fullscreen_display_no)	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_cnt != (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size())	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_no < 0 || (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size() <= Gp_config->fullscreen_resolution_no)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution != Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list[Gp_config->fullscreen_resolution_no].resolution)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}

	// �l���X�V����
	tnm_correct_fullscreen_resolution();	// �l��␳����

	// ���ݒ�̒l��K�p����
	if (!tnm_change_screen_size_start(true))	{	return;	}
	tnm_update_screen_size_mode_sub(NULL, true, true);
	tnm_change_screen_size_end();
}

// ****************************************************************
// �X�N���[���T�C�Y�����ɖ߂�
// ================================================================
//		�𑜓x�ύX�O�ɋL��������Ԃɖ߂��܂��B
// ================================================================
class Cchange_resolution_warning_dialog : public C_modal_dialog
{
public:
	Cc_static	stc_text;
	Cc_button	btn_ok;
	Cc_button	btn_cancel;

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	bool		on_init_dialog();
	bool		on_close();
	bool		on_command(WPARAM wp, LPARAM lp);
	bool		on_timer();

private:
	int			rest_cnt;
};

LRESULT Cchange_resolution_warning_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_TIMER:			if (!on_timer())			return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

bool Cchange_resolution_warning_dialog::on_init_dialog()
{
	set_text(tnm_get_game_name());
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	bind_control(&stc_text, IDC_STC_CHANGE_RESOLUTION_TEXT);
	bind_control(&btn_ok, IDC_CHANGE_RESOLUTION_OK);
	bind_control(&btn_cancel, IDC_CHANGE_RESOLUTION_CANCEL);

	rest_cnt = 20;	// 20�b�Ō��ɖ߂�܂��B
	on_timer();		// �ŏ��� on_timer ���P�x�Ă�ł���

	::SetTimer(m_h_wnd, 0, 1000, NULL);

	btn_cancel.set_focus();

	return true;
}

bool Cchange_resolution_warning_dialog::on_close()
{
	EndDialog(m_h_wnd, IDCANCEL);

	// �f�t�H���g�̓�������s���Ȃ��I
	return false;
}

bool Cchange_resolution_warning_dialog::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);

	switch (ctrl_id)	{
		case IDC_CHANGE_RESOLUTION_OK:		EndDialog(m_h_wnd, IDOK);		return false;
		case IDC_CHANGE_RESOLUTION_CANCEL:	EndDialog(m_h_wnd, IDCANCEL);	return false;
		case IDCANCEL:						EndDialog(m_h_wnd, IDCANCEL);	return false;	// ��Esc �L�[�������Ƃ�����ʂ�
	}

	return true;
}

bool Cchange_resolution_warning_dialog::on_timer()
{
	if (rest_cnt == 0)	{
		EndDialog(m_h_wnd, IDCANCEL);
		return false;
	}

	TSTR str = LCL_STR_CHANGE_RESOLUTION_AFTER_STR(rest_cnt);
		
	stc_text.set_text(str);

	rest_cnt--;

	return true;
}

void tnm_reset_screen_size_mode()
{
	// �S�Ẵ_�C�A���O�ɑ΂��ă��W�I�{�^���̃t�H�[�J�X���͂���
	// ���b�Z�[�W�{�b�N�X���o��O�ɏ�������K�v������܂��B
	tnm_dlg_kill_focus_for_all_radio();

	tnm_dlg_set_enable(false);		// ���_�C�A���O����֎~

	// �𑜓x��ύX�����ʒm���o��
	Cchange_resolution_warning_dialog dlg;
	int res = dlg.create(LCL_IDD_CHANGE_RESOLUTION_WARNING, _T("change_resolution_warning"), false, G_app.h_wnd);
	if (res == IDCANCEL)	{

		// �𑜓x�����ɖ߂�
		Gp_config->screen_size_mode = Gp_global->change_display_mode_proc_old_screen_size_mode;
		Gp_config->screen_size_mode_window = Gp_global->change_display_mode_proc_old_screen_size_mode_window;
		Gp_config->screen_size_scale = Gp_global->change_display_mode_proc_old_screen_size_scale;
		Gp_config->screen_size_free = Gp_global->change_display_mode_proc_old_screen_size_free;
		C_point window_pos = Gp_global->change_display_mode_proc_old_window_pos;
		Gp_config->fullscreen_change_resolution = Gp_global->change_display_mode_proc_old_change_resolution;
		Gp_config->fullscreen_display_no = Gp_global->change_display_mode_proc_old_display_no;
		Gp_config->fullscreen_resolution_no = Gp_global->change_display_mode_proc_old_resolution_no;
		tnm_correct_fullscreen_resolution();	// �l��␳����

		// ���ݒ�̒l��K�p����
		if (!tnm_change_screen_size_start(true))	{	return;	}
		tnm_update_screen_size_mode_sub(&window_pos, false, false);
		tnm_change_screen_size_end();
	}

	tnm_dlg_set_enable(true);		// ���_�C�A���O����L��
}

// ****************************************************************
// �X�N���[���T�C�Y�␳
// ================================================================
bool tnm_rep_screen_size(int screen_size_mode, C_size* screen_size_scale, C_size* screen_size_free)
{
	// �t���X�N���[�����[�h�͊֌W�Ȃ�
	if (screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
		return false;

	if (screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)
	{
		return tnm_rep_screen_size_window(screen_size_scale);
	}
	else if (screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
	{
		return tnm_rep_screen_size_free(screen_size_free);
	}

	return false;
}

bool tnm_rep_screen_size_window(C_size* screen_size_scale)
{
	// �X�N���[���T�C�Y�䗦�����[�h�ɕϊ�����i�K��O�̏ꍇ�A�P�O�O���ɕ␳����j
	bool change_flag = false;
	int size_mode = tnm_format_screen_size_to_screen_mode(*screen_size_scale);
	if (size_mode == -1)	// �K��O�̏ꍇ�A�P�O�O���ɕ␳����
	{
		screen_size_scale->cx = 100;
		screen_size_scale->cy = 100;
		size_mode = 2;
		change_flag = true;
	}

	// �f�X�N�g�b�v�̃X�N���[���T�C�Y���ɔ[�܂邩�`�F�b�N����i�[�܂�Ȃ���΁A�T�C�Y�������Ă����܂��j
	int display_w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int display_h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	C_size window_size, client_size;
	while (size_mode)
	{
		window_size = tnm_get_window_size_from_size_mode(size_mode);
		client_size = tnm_get_screen_size_from_size_mode(size_mode);
		if (client_size.cx <= display_w && window_size.cy <= display_h)
			break;	// �����̓N���C�A���g�T�C�Y�A�c�̓E�B���h�E�T�C�Y�Ŕ��肵�܂��B
		change_flag = true;
		size_mode--;
	}

	// �T�C�Y���ς�����̂Œl��␳����
	if (change_flag)
		tnm_format_screen_mode_to_screen_size_scale(size_mode, screen_size_scale);

	return change_flag;
}

bool tnm_rep_screen_size_free(C_size* screen_size_free)
{
	bool change_flag = false;

	// ���Ƃ肠�����������Ȃ��c�B

	return change_flag;
}

// ****************************************************************
// �X�N���[���T�C�Y�䗦����X�N���[�����[�h�ɕϊ��i�擾�H�j
// ================================================================
int tnm_format_screen_size_to_screen_mode(C_size screen_size_scale)
{
	if (screen_size_scale.cx == 50 && screen_size_scale.cy == 50)			{	return 0;	}
	else if (screen_size_scale.cx == 75 && screen_size_scale.cy == 75)		{	return 1;	}
	else if (screen_size_scale.cx == 100 && screen_size_scale.cy == 100)	{	return 2;	}
	else if (screen_size_scale.cx == 150 && screen_size_scale.cy == 150)	{	return 3;	}
	else if (screen_size_scale.cx == 200 && screen_size_scale.cy == 200)	{	return 4;	}
	return -1;		// �K��O�� -1 ���߂�܂��B
}

// ****************************************************************
// �X�N���[�����[�h����X�N���[���T�C�Y�䗦�ɕϊ�
// ================================================================
void tnm_format_screen_mode_to_screen_size_scale(int size_mode, C_size* screen_size_scale)
{
	switch (size_mode)	{
		case 0:		// 50%
			screen_size_scale->cx = 50;
			screen_size_scale->cy = 50;
			break;
		case 1:		// 75%
			screen_size_scale->cx = 75;
			screen_size_scale->cy = 75;
			break;
		case 2:		// 100%
			screen_size_scale->cx = 100;
			screen_size_scale->cy = 100;
			break;
		case 3:		// 150%
			screen_size_scale->cx = 150;
			screen_size_scale->cy = 150;
			break;
		case 4:		// 200%
			screen_size_scale->cx = 200;
			screen_size_scale->cy = 200;
			break;
	}
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h����X�N���[���T�C�Y���擾
// ================================================================
C_size tnm_get_screen_size_from_size_mode(int size_mode)
{
	C_size screen_size = Gp_global->game_screen_size;

	switch (size_mode)	{
		case 0:		// 50%
			return screen_size / 2;
		case 1:		// 75%
			return screen_size / 2 + screen_size / 4;
		case 2:		// 100%
			return screen_size;
		case 3:		// 150%
			return screen_size * 3 / 2;
		case 4:		// 200%
			return screen_size * 2;
	}
	return screen_size;
}

// ****************************************************************
// �X�N���[���T�C�Y���[�h����E�B���h�E�T�C�Y���擾
// ================================================================
C_size tnm_get_window_size_from_size_mode(int size_mode)
{
	// �X�N���[���T�C�Y�����߂�
	C_size client_size = tnm_get_screen_size_from_size_mode(size_mode);

	// �E�B���h�E�T�C�Y�ɕϊ�����
	C_rect window_rect = C_rect(0, 0, client_size.cx, client_size.cy);
	::AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME, Gp_global->debug_flag ? TRUE : FALSE);
	return window_rect.size();
}

// ****************************************************************
// �X�N���[���T�C�Y���g���邩�ǂ����𔻒�
// ================================================================
bool tnm_get_window_size_from_size_scale(C_size client_size_scale)
{
	C_size client_size = Gp_global->game_screen_size;
	client_size.cx = client_size.cx * client_size_scale.cx / 100;
	client_size.cy = client_size.cy * client_size_scale.cy / 100;

	// �E�B���h�E�T�C�Y�ɕϊ�����
	C_rect window_rect = C_rect(0, 0, client_size.cx, client_size.cy);
	::AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME, Gp_global->debug_flag ? TRUE : FALSE);

	int default_resolution_no = Gp_system_info->resolution_table[0].default_resolution_no;
	C_size default_resolution = Gp_system_info->resolution_table[0].resolution_list[default_resolution_no].resolution;
	return (client_size.cx <= default_resolution.cx && window_rect.height() <= default_resolution.cy) ? 1 : 0;
}

// ****************************************************************
// ����������҂ݒ�̕ύX����
// ================================================================
void tnm_set_wait_display_vsync_sub(bool wait_display_vsync_flag)
{
	G_d3d.device.change_present_interval(wait_display_vsync_flag ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE);
}

void tnm_set_wait_display_vsync(bool wait_display_vsync_flag)
{
	if (Gp_global->wait_display_vsync_total == wait_display_vsync_flag)
		return;

	// ����������҂ݒ��ύX����
	if (tnm_change_screen_size_start(false))	{
		tnm_set_wait_display_vsync_sub(wait_display_vsync_flag);
		tnm_change_screen_size_end();

		// ���������̂ŃV�X�e���f�[�^���X�V����
		Gp_global->wait_display_vsync_total = wait_display_vsync_flag;
	}
}
