#include	"pch.h"

#include	"tnm_common_header.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_rmenu.h"

#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// �h�c��`
// ================================================================

// ����`�l���P�O�O�O�𒴂���ƁA�f�o�b�O���j���[�̃V�X�e���R�}���h���j���[�ɖ�肪�N����܂��B�����Ӊ������B

const int	IDM_SYSCOMMENU_CANCEL			= 0;			// �����ڂ�I�����Ȃ������ꍇ�� 0 ���A��̂ŁA�L�����Z���� 0 �Œ�ł��B
const int	IDM_SYSCOMMENU_READ_SKIP		= 1;
const int	IDM_SYSCOMMENU_AUTO_SKIP		= 2;
const int	IDM_SYSCOMMENU_AUTO_MODE		= 3;
const int	IDM_SYSCOMMENU_HIDE_MWND		= 4;
const int	IDM_SYSCOMMENU_SAVE				= 5;
const int	IDM_SYSCOMMENU_LOAD				= 6;
const int	IDM_SYSCOMMENU_RETURN_SEL		= 7;
const int	IDM_SYSCOMMENU_CONFIG			= 8;
const int	IDM_SYSCOMMENU_MANUAL			= 9;
const int	IDM_SYSCOMMENU_VERSION			= 10;
const int	IDM_SYSCOMMENU_RETURN_MENU		= 11;
const int	IDM_SYSCOMMENU_GAME_END			= 12;

const int	IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000		= 100;		// ���j���[�̍ő區�ڐ����S�Ȃ̂� 100�`103 ���g�p���܂��i2009/10/21�j
const int	IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_0000		= 200;		// ���j���[�̍ő區�ڐ����S�Ȃ̂� 200�`203 ���g�p���܂��i2009/10/21�j
const int	IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000	= 300;		// ���j���[�̍ő區�ڐ����S�ŃA�C�e���̍ő吔���W�Ȃ̂� 300�`307, 308�`315, 316�`323, 324�`331 ���g�p���܂��i2009/10/21�j

// ****************************************************************
// �V�X�e���R�}���h���j���[�F�J��
// ================================================================
bool tnm_rmenu_open()
{
	C_popup_menu menu;

	// ���j���[���쐬
	tnm_rmenu_create(menu);

	// �I���J�n
	return tnm_rmenu_select(menu);
}

// ****************************************************************
// �V�X�e���R�}���h���j���[�F�쐬
// ================================================================
bool tnm_rmenu_create(C_popup_menu& menu)
{
	// ���j���[���쐬
	menu.create();

	return tnm_rmenu_create_funk(menu);
}

// ****************************************************************
// �V�X�e���R�}���h���j���[�F�쐬�t�@���N�V����
// ================================================================
bool tnm_rmenu_create_funk(C_menu& menu, int id_rep)
{
	bool exist_once_flag = false;

	// �������V�X�e���R�}���h���j���[�̍��ڂ̕��т͂���ł����̂��������鎖�B

	// ����������������������������������������������������������������������������������������������������
	if (tnm_syscom_read_skip_get_exist_flag() || tnm_syscom_auto_skip_get_exist_flag() || tnm_syscom_auto_mode_get_exist_flag() || tnm_syscom_hide_mwnd_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// ���Ǖ��͂𑁑���
	if (tnm_syscom_read_skip_get_exist_flag())	{
		// ���ǃX�L�b�v���n�m�̏ꍇ
		if (Gp_config->system.skip_unread_message_flag)	{
			menu.add_item(IDM_SYSCOMMENU_READ_SKIP + id_rep, Gp_ini->syscommenu.unread_skip.str, tnm_syscom_read_skip_is_enable(), tnm_syscom_read_skip_get_onoff_flag());
		}
		else	{
			menu.add_item(IDM_SYSCOMMENU_READ_SKIP + id_rep, Gp_ini->syscommenu.read_skip.str, tnm_syscom_read_skip_is_enable(), tnm_syscom_read_skip_get_onoff_flag());
		}
	}

	// ����������
	if (tnm_syscom_auto_skip_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_AUTO_SKIP + id_rep, Gp_ini->syscommenu.auto_skip.str, tnm_syscom_auto_skip_is_enable(), tnm_syscom_auto_skip_get_onoff_flag());
	}

	// �I�[�g���[�h
	if (tnm_syscom_auto_mode_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_AUTO_MODE + id_rep, Gp_ini->syscommenu.auto_mode.str, tnm_syscom_auto_mode_is_enable(), tnm_syscom_auto_mode_get_onoff_flag());
	}

	// �E�B���h�E���B��
	if (tnm_syscom_hide_mwnd_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_HIDE_MWND + id_rep, Gp_ini->syscommenu.hide_mwnd.str, tnm_syscom_hide_mwnd_is_enable(), tnm_syscom_hide_mwnd_get_onoff_flag());
	}

	// ����������������������������������������������������������������������������������������������������
	if (tnm_syscom_open_save_get_exist_flag() || tnm_syscom_open_load_get_exist_flag() || tnm_syscom_return_to_sel_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// �Z�[�u
	if (tnm_syscom_open_save_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_SAVE + id_rep, Gp_ini->syscommenu.save.str, tnm_syscom_open_save_is_enable()) ;
	}

	// ���[�h
	if (tnm_syscom_open_load_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_LOAD + id_rep, Gp_ini->syscommenu.load.str, tnm_syscom_open_load_is_enable()) ;
	}

	// �O�̑I�����ɖ߂�
	if (tnm_syscom_return_to_sel_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_RETURN_SEL + id_rep, Gp_ini->syscommenu.return_to_sel.str, tnm_syscom_return_to_sel_is_enable());
	}

	// ����������������������������������������������������������������������������������������������������
	if (tnm_syscom_open_config_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// ���ݒ�
	if (tnm_syscom_open_config_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_CONFIG + id_rep, Gp_ini->syscommenu.config.str, tnm_syscom_open_config_get_enable_flag(), tnm_dlg_is_config_dialog_opened());
	}

	// ����������������������������������������������������������������������������������������������������

	// ���[�J���ėp�X�C�b�`�^���[�h�����邩�ǂ����𔻒�
	bool local_switch_exist_flag = false;
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < Gp_ini->syscommenu.local_extra_switch_cnt; i++)	{
			if (tnm_syscom_local_extra_switch_get_exist_flag(i))	{
				local_switch_exist_flag = true;
				break;
			}
		}
	}
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		for (int i = 0; i < (int)Gp_ini->syscommenu.local_extra_mode_cnt; i++)	{
			if (tnm_syscom_local_extra_mode_get_exist_flag(i))	{
				local_switch_exist_flag = true;
				break;
			}
		}
	}

	// ���݂���΃Z�p���[�^������
	if (local_switch_exist_flag)	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// ���[�J���ėp�X�C�b�`
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < Gp_ini->syscommenu.local_extra_switch_cnt; i++)	{
			if (tnm_syscom_local_extra_switch_get_exist_flag(i))	{
				menu.add_item(IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 + i + id_rep, Gp_ini->syscommenu.local_extra_switch[i].str, tnm_syscom_local_extra_switch_is_enable(i), tnm_syscom_local_extra_switch_get_onoff_flag(i));
			}
		}
	}

	// ���[�J���ėp���[�h
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		C_menu sub_menu;
		for (int i = 0; i < (int)Gp_ini->syscommenu.local_extra_mode_cnt; i++)	{
			if (tnm_syscom_local_extra_mode_get_exist_flag(i))	{
				sub_menu.create();
				if (tnm_syscom_local_extra_mode_get_enable_flag(i))	{
					for (int j = 0; j < Gp_ini->syscommenu.local_extra_mode[i].item_cnt; j++)	{
						int value = tnm_syscom_local_extra_mode_get_value(i);
						sub_menu.add_item(IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 + (i * TNM_LOCAL_EXTRA_MODE_ITEM_CNT) + j + id_rep, Gp_ini->syscommenu.local_extra_mode[i].item[j].str, true, (j == value) ? true : false);
					}
				}
				menu.add_sub_menu(IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_0000 + i + id_rep, Gp_ini->syscommenu.local_extra_mode[i].str, sub_menu.get_handle(), tnm_syscom_local_extra_mode_get_enable_flag(i));
			}
		}
	}

	// ����������������������������������������������������������������������������������������������������
	if (tnm_syscom_manual_get_exist_flag() || tnm_syscom_version_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// �}�j���A��
	if (tnm_syscom_manual_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_MANUAL + id_rep, Gp_ini->syscommenu.manual.str, true);
	}

	// �o�[�W�������
	if (tnm_syscom_version_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_VERSION + id_rep, Gp_ini->syscommenu.version.str, true);
	}

	// ����������������������������������������������������������������������������������������������������
	if (tnm_syscom_return_to_menu_get_exist_flag() || tnm_syscom_end_game_get_exist_flag() || tnm_syscom_cancel_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// �Z�p���[�^
		exist_once_flag = true;
	}

	// ���j���[�ɖ߂�
	if (tnm_syscom_return_to_menu_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_RETURN_MENU + id_rep, Gp_ini->syscommenu.return_to_menu.str, tnm_syscom_return_to_menu_get_enable_flag());
	}

	// �Q�[�����I������
	if (tnm_syscom_end_game_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_GAME_END + id_rep, Gp_ini->syscommenu.game_end.str, tnm_syscom_end_game_get_enable_flag());
	}

	// �L�����Z��
	if (tnm_syscom_cancel_get_exist_flag() || !exist_once_flag)	{
		menu.add_item(IDM_SYSCOMMENU_CANCEL + id_rep, Gp_ini->syscommenu.cancel.str, tnm_syscom_cancel_get_enable_flag());
	}
	return true;
}

// ****************************************************************
// �V�X�e���R�}���h���j���[�F�I���J�n
// ================================================================
bool tnm_rmenu_select(C_popup_menu& menu)
{
	int res = 0;

	// ���j���[���J���Ă���Ԃ͎��Ԃ��~�߂�I
	if (!Gp_global->alt_menu_wait_flag)
		Gp_global->alt_menu_wait_flag = true;

	// ���j���[���J���I���J�n
	C_point mp = get_mouse_pos();	// ���������͓Ǝ��Ń}�E�X���W�����o��
	int syscom_id = menu.select(Gp_wnd->get_handle(), mp.x, mp.y);

	// ���j���[���J�����̂łP�t���[���������͂��֎~����
	Gp_global->skip_click_1_frame = true;

	// �������s
	return tnm_rmenu_execute(syscom_id);
}

// ****************************************************************
// �V�X�e���R�}���h���j���[�F�������s
// ================================================================
bool tnm_rmenu_execute(int syscom_id)
{
	switch (syscom_id)	{

		// �L�����Z��
		case IDM_SYSCOMMENU_CANCEL:
			break;

		// ���ǃX�L�b�v
		case IDM_SYSCOMMENU_READ_SKIP:
			tnm_syscom_read_skip_set_onoff_flag(!tnm_syscom_read_skip_get_onoff_flag());
			break;

		// ����������
		case IDM_SYSCOMMENU_AUTO_SKIP:
			tnm_syscom_auto_skip_set_onoff_flag(!tnm_syscom_auto_skip_get_onoff_flag());
			break;

		// �I�[�g���[�h
		case IDM_SYSCOMMENU_AUTO_MODE:
			tnm_syscom_auto_mode_set_onoff_flag(!tnm_syscom_auto_mode_get_onoff_flag());
			break;

		// ���b�Z�[�W�E�B���h�E���B��
		case IDM_SYSCOMMENU_HIDE_MWND:
			tnm_syscom_hide_mwnd_set_onoff_flag(!tnm_syscom_hide_mwnd_get_onoff_flag());
			break;

		// �Z�[�u
		case IDM_SYSCOMMENU_SAVE:
			tnm_syscom_open_save(false, true, true);		// �i���Z�[�u�F�E�N���b�N���j���[�j�_�C�A���O���Ă΂��ꍇ�̓L���v�`���[����
			break;

		// ���[�h
		case IDM_SYSCOMMENU_LOAD:
			tnm_syscom_open_load(false, true);
			break;

		// �O�̑I�����ɖ߂�
		case IDM_SYSCOMMENU_RETURN_SEL:
			tnm_syscom_return_to_sel(true, true, true);		// �x������
			break;

		// ���ݒ�
		case IDM_SYSCOMMENU_CONFIG:
			tnm_syscom_open_config();
			break;

		// �}�j���A��
		case IDM_SYSCOMMENU_MANUAL:
		{
			if (!Gp_ini->manual_path_str.empty())	{
				shell_execute_open(Gp_dir->base_dir + _T("\\") + Gp_ini->manual_path_str);
			}
			break;
		}

		// �o�[�W�������
		case IDM_SYSCOMMENU_VERSION:
			tnm_game_info_box(Gp_ini->game_name + _T(" ") + Gp_ini->game_version_str + _T("\nSiglus Ver.") + SIGLUS_VERSION);
			break;

		// ���j���[�ɖ߂�
		case IDM_SYSCOMMENU_RETURN_MENU:
			tnm_syscom_return_to_menu(true, true, true, false);		// �x������
			break;

		// �Q�[�����I������
		case IDM_SYSCOMMENU_GAME_END:
			tnm_syscom_end_game(true, false, false);				// �x���݂̂���
			break;

		default:

			if (false);

			// ���[�J���ėp�X�C�b�`
			else if (IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 <= syscom_id && syscom_id < IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 + (int)Gp_ini->syscommenu.local_extra_switch_cnt)	{
				int no = syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000;
				tnm_syscom_local_extra_switch_turn_onoff_flag(no);
			}

			// ���[�J���ėp���[�h
			else if (IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 <= syscom_id && syscom_id < IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 + ((int)Gp_ini->syscommenu.local_extra_mode_cnt * TNM_LOCAL_EXTRA_MODE_ITEM_CNT))	{
				int value = (syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000) % TNM_LOCAL_EXTRA_MODE_ITEM_CNT;
				int no = (syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000) / TNM_LOCAL_EXTRA_MODE_ITEM_CNT;
				tnm_syscom_local_extra_mode_set_value(no, value);
			}

			break;
	}
	return true;
}
