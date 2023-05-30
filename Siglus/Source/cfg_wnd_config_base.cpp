#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"dialog/cfg_wnd.h"
#include	"dialog/cfg_wnd_config_base.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_config_base")

// ������
void C_cfg_wnd_config_base::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_BASE);
	cfg_wnd_full_mode_flag = false;		// �� false �͒ʏ�̊��ݒ胂�[�h�ł��B
	init_child_window();
}

// �E�B���h�E�v���V�[�W��
LRESULT C_cfg_wnd_config_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
		case WM_MOUSEWHEEL:		if (!on_mousewheel(wp, lp))	return FALSE;	break;

		// ���W�I�{�^���̃t�H�[�J�X���O���ׂ̏���
		case WM_ACTIVATE:
			if (LOWORD(wp) == WA_INACTIVE)	{
				int tab_no = ci.tab_base.get_selected_item_no();
				if (0 <= tab_no && tab_no < (int)child_window_list.size())	{
					child_window_list[tab_no]->kill_focus_for_radio_item();
					kill_focus_for_radio_item();
				}
			}
			break;

#if 0
		case WM_CTLCOLORDLG:
			{
//				HDC hdc = GetDC((HWND)lp);
//				SetBkMode(hdc, TRANSPARENT);
//				ReleaseDC((HWND)lp, hdc);
//				SetBkMode((HDC)wp, TRANSPARENT);
//				return (LRESULT)GetSysColorBrush(COLOR_3DFACE);
//				return (LRESULT)GetSysColorBrush(COLOR_3DHIGHLIGHT);
				return (LRESULT)GetStockObject(WHITE_BRUSH);
//				return (LRESULT)0;
			}
			break;
		case WM_CTLCOLORSTATIC:
			{
//				SetBkMode((HDC)wp, TRANSPARENT);
//				return (LRESULT)GetSysColorBrush(COLOR_3DHIGHLIGHT);
				return (LRESULT)GetStockObject(WHITE_BRUSH);
			}
			break;
#endif

	}

	return C_tnm_dlg::window_proc(msg, wp, lp);
}

// ������
bool C_cfg_wnd_config_base::on_init()
{
	// �X�e�[�^�X�o�[�̃T�C�Y�O���b�v�쐬
	statusbar.create_sizegrip(m_h_wnd);

	// �A�C�e���o�^
	bind_control(&ci.tab_base, IDC_TAB_BASE, _T("TAB_BASE"), 5);
	bind_control(&ci.stc_game_version, IDC_STC_GAME_VERSION, _T(""), 1);
	bind_control(&ci.btn_all_config_init, IDC_BTN_ALL_CONFIG_INIT, _T(""), 3);
	bind_control(&ci.btn_close, IDC_BTN_CLOSE, _T(""), 3);

	// �q�E�B���h�E��o�^�i�E�B���h�E���X�g�쐬�j�i�^�u��ǉ��j
	regist_child_window();

	// �^�u�ɐe�̃N���X�|�C���^�[��ݒ�
	ci.tab_base.parent_class = this;

	// ���ݑI������Ă���^�u�̐ݒ�
	int tab_no = ci.tab_base.load_ini_int(_T("TAB_SELECT_NO"), 0);
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;	}
	ci.tab_base.set_selected_item_no(tab_no);

	// �o�[�W�������
	ci.stc_game_version.set_text_adjust_width(Gp_ini->game_version_str, 20);	// 20 ���炢�]�T����������

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �E�B���h�E�^�C�g��
	if (cfg_wnd_full_mode_flag)	{
		set_text(_T("�S���ݒ�i�f�o�b�O�p�j"));
	}

	// �E�B���h�E�\��
	update();
	set_show(true);

	// �q�E�B���h�E���J��
	open_child_window();

	return true;
}

// ����
bool C_cfg_wnd_config_base::on_close()
{
	// �S�Ă̎q�E�B���h�E�����
	close_all_child_window();

	// �q�E�B���h�E���X�g�J��
	child_window_list.clear();

	// �A�C�e���ݒ�Z�[�u
	ci.tab_base.save_ini_int(_T("TAB_SELECT_NO"), ci.tab_base.get_selected_item_no());

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_cfg_wnd_config_base::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_ALL_CONFIG_INIT:
			// �q�́u������Ԃɖ߂��v���Ăяo��
			cfg_wnd_base_screen.cfg_wnd_func_screen.init_state();
			cfg_wnd_base_volume_bgmfade.cfg_wnd_func_volume.init_state();
			cfg_wnd_base_volume_bgmfade.cfg_wnd_func_bgmfade.init_state();
			cfg_wnd_base_volume.cfg_wnd_func_volume.init_state();
			cfg_wnd_base_font_msgspd.cfg_wnd_func_font.init_state();
			cfg_wnd_base_font_msgspd.cfg_wnd_func_msgspd.init_state();
			cfg_wnd_base_mwndbk.cfg_wnd_func_mwndbk.init_state();
			cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_koemode.init_state();
			cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_chrkoe.init_state();
			cfg_wnd_base_koemode.cfg_wnd_func_koemode.init_state();
			cfg_wnd_base_chrkoe.cfg_wnd_func_chrkoe.init_state();
			cfg_wnd_base_automode.cfg_wnd_func_automode.init_state();
			cfg_wnd_base_jitan.cfg_wnd_func_jitan.init_state();
			cfg_wnd_base_else.cfg_wnd_func_else.init_state();
			cfg_wnd_base_system.cfg_wnd_func_system.init_state();
			break;
		case IDC_BTN_CLOSE:
			close();
			break;
	}
	return true;
}

// �ʒm
bool C_cfg_wnd_config_base::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	// �^�u�R���g���[��
	if (hdr->hwndFrom == ci.tab_base.get_handle())	{
		switch (hdr->code)	{
			case TCN_SELCHANGE:
				open_child_window();
				break;
		}
	}

	return true;
}

// �}�E�X�z�C�[��
bool C_cfg_wnd_config_base::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (child_window_list.empty())
		return true;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))
		return true;

	// �^�u�Ƀt�H�[�J�X�����鎞�ŁA�����̂Ƃ��̓L�����N�^�[���X�g���X�N���[��������
	if (child_window_list[tab_no] == &cfg_wnd_base_koemode_chrkoe && GetFocus() == ci.tab_base.get_handle())	{
		cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_chrkoe.on_mousewheel_character_list(wp, lp);
		cfg_wnd_base_chrkoe.cfg_wnd_func_chrkoe.on_mousewheel_character_list(wp, lp);
		return false;
	}

	return true;
}

// �t���[��
void C_cfg_wnd_config_base::frame()
{
	if (!m_h_wnd)
		return;

	// �q�̃t���[��
	cfg_wnd_base_screen.frame();					// �x�[�X�F��ʃ��[�h
	cfg_wnd_base_font_msgspd.frame();				// �x�[�X�F���b�Z�[�W�i�t�H���g�A�������x�j
	cfg_wnd_base_volume_bgmfade.frame();			// �x�[�X�F���ʁi���ʁA�a�f�l�t�F�[�h�j
	cfg_wnd_base_koemode_chrkoe.frame();			// �x�[�X�F�����i�������[�h�A�L�����N�^�[�����j
	cfg_wnd_base_koemode.frame();					// �x�[�X�F�������[�h
	cfg_wnd_base_chrkoe.frame();					// �x�[�X�F�L�����N�^�[����
	cfg_wnd_base_volume.frame();					// �x�[�X�F����
	cfg_wnd_base_mwndbk.frame();					// �x�[�X�F�E�B���h�E�w�i�F
	cfg_wnd_base_automode.frame();					// �x�[�X�F�I�[�g���[�h
	cfg_wnd_base_jitan.frame();						// �x�[�X�F���Z�Đ�
	cfg_wnd_base_else.frame();						// �x�[�X�F���̑�
	cfg_wnd_base_system.frame();					// �x�[�X�F�V�X�e��
}

// ****************************************************************
// �q�E�B���h�E��������
// ================================================================
void C_cfg_wnd_config_base::init_child_window()
{
	all_child_window_list.push_back(&cfg_wnd_base_screen);			// �x�[�X�F��ʃ��[�h
	all_child_window_list.push_back(&cfg_wnd_base_font_msgspd);		// �x�[�X�F���b�Z�[�W�i�t�H���g�A�������x�j
	all_child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	// �x�[�X�F���ʁi���ʁA�a�f�l�t�F�[�h�j
	all_child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	// �x�[�X�F�����i�������[�h�A�L�����N�^�[�����j
	all_child_window_list.push_back(&cfg_wnd_base_koemode);			// �x�[�X�F�������[�h
	all_child_window_list.push_back(&cfg_wnd_base_chrkoe);			// �x�[�X�F�L�����N�^�[����
	all_child_window_list.push_back(&cfg_wnd_base_volume);			// �x�[�X�F����
	all_child_window_list.push_back(&cfg_wnd_base_mwndbk);			// �x�[�X�F�E�B���h�E�w�i�F
	all_child_window_list.push_back(&cfg_wnd_base_automode);		// �x�[�X�F�I�[�g���[�h
	all_child_window_list.push_back(&cfg_wnd_base_jitan);			// �x�[�X�F���Z�Đ�
	all_child_window_list.push_back(&cfg_wnd_base_else);			// �x�[�X�F���̑�
	all_child_window_list.push_back(&cfg_wnd_base_system);			// �x�[�X�F�V�X�e��

	for (int i = 0; i < (int)all_child_window_list.size(); i++)	{
		all_child_window_list[i]->init();											// ������
		all_child_window_list[i]->cfg_wnd_full_mode_flag = cfg_wnd_full_mode_flag;	// �t�����[�h�̐ݒ�
	}
}	

// ****************************************************************
// �q�E�B���h�E��o�^
// ================================================================
void C_cfg_wnd_config_base::regist_child_window()
{
	// �S���ݒ�i�f�o�b�O�p�j
	if (cfg_wnd_full_mode_flag)
	{
		child_window_list.push_back(&cfg_wnd_base_screen);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SCREEN_STR);
		child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);
		child_window_list.push_back(&cfg_wnd_base_font_msgspd);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_MESSAGE_STR);
		child_window_list.push_back(&cfg_wnd_base_mwndbk);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR);
		child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);
		child_window_list.push_back(&cfg_wnd_base_automode);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR);
		child_window_list.push_back(&cfg_wnd_base_jitan);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_JITAN_STR);
		child_window_list.push_back(&cfg_wnd_base_else);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_OTHERS_STR);
		child_window_list.push_back(&cfg_wnd_base_system);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SYSTEM_STR);
	}
	// ���ݒ�
	else
	{
		if (Gp_ini->config.tab_exist_screen)	{
			child_window_list.push_back(&cfg_wnd_base_screen);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SCREEN_STR);
		}
		if (Gp_ini->config.tab_exist_volume)	{
			switch (Gp_ini->config.dlg_style_volume)	{
				case 0:		child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
				case 1:		child_window_list.push_back(&cfg_wnd_base_volume);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
				default:	child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
			}
		}
		if (Gp_ini->config.tab_exist_message)	{
			child_window_list.push_back(&cfg_wnd_base_font_msgspd);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_MESSAGE_STR);
		}
		if (Gp_ini->config.tab_exist_mwndbk)	{
			child_window_list.push_back(&cfg_wnd_base_mwndbk);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR);
		}
		if (Gp_ini->config.tab_exist_koe)	{
			switch (Gp_ini->config.dlg_style_koe)	{
				case 0:		child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				case 1:		child_window_list.push_back(&cfg_wnd_base_koemode);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				case 2:		child_window_list.push_back(&cfg_wnd_base_chrkoe);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				default:	child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
			}
		}
		if (Gp_ini->config.tab_exist_automode)	{
			child_window_list.push_back(&cfg_wnd_base_automode);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR);
		}
		if (Gp_ini->config.tab_exist_jitan)	{
			child_window_list.push_back(&cfg_wnd_base_jitan);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_JITAN_STR);
		}
		if (Gp_ini->config.tab_exist_else)	{
			child_window_list.push_back(&cfg_wnd_base_else);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_OTHERS_STR);
		}
		if (Gp_ini->config.tab_exist_system)	{
			child_window_list.push_back(&cfg_wnd_base_system);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SYSTEM_STR);
		}
	}
}

// ****************************************************************
// �q�E�B���h�E���J��
// ================================================================
void C_cfg_wnd_config_base::open_child_window()
{
	if (child_window_list.empty())
		return;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;		ci.tab_base.set_selected_item_no(tab_no);	}

	child_window_list[tab_no]->open(m_h_wnd);
	adjust_child_window();
	close_all_child_window(child_window_list[tab_no]);
}

// ****************************************************************
// �S�Ă̎q�E�B���h�E�����
// ================================================================
void C_cfg_wnd_config_base::close_all_child_window(C_cfg_wnd_solo* no_close_window)
{
	if (child_window_list.empty())
		return;

	for (int i = 0; i < (int)child_window_list.size(); i++)	{
		if (child_window_list[i] != no_close_window)	{
			child_window_list[i]->close();
		}
	}
}

// ****************************************************************
// �q�E�B���h�E�𒲐�����
// ================================================================
void C_cfg_wnd_config_base::adjust_child_window()
{
	if (child_window_list.empty())
		return;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;		ci.tab_base.set_selected_item_no(tab_no);	}

	ARRAY<C_cfg_wnd_solo *>::iterator itr = child_window_list.begin() + tab_no;
	C_rect rect = screen_to_client(ci.tab_base.get_display_rect());	// �^�u�̕\���̈�����߂�
	rect.left -= 2;		// ������
	child_window_list[tab_no]->set_window_rect(rect);
}

// ****************************************************************
// �^�u�x�[�X�̃E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_cfg_wnd_config_base::C_tab_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SIZE:
		case WM_MOVE:
			parent_class->adjust_child_window();
			break;
	}
	return Cc_tab::window_proc(msg, wp, lp);
}

