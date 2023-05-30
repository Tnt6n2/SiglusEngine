#include	"pch.h"

#include	"localize/localize.h"

#include	"ifc_eng.h"
#include	"data/tnm_ini.h"

#include	"lzss.h"
#include	"tnm_common_header.h"

#include	<boost/algorithm/string.hpp>

// ****************************************************************
// �ȈՈÍ��R�[�h
// ================================================================
#include	"tnm_exe_angou.h"
BYTE tnm_gameexe_dat_angou_code[TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE] = TNM_GAMEEXE_DAT_ANGOU_CODE;

// ****************************************************************
// �d�w�d�Z�b�g�Í��R�[�h
// ================================================================
volatile extern BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h


// ****************************************************************
// Select.ini�F����������
// ================================================================
C_tnm_select_ini::C_tnm_select_ini()
{
}

// ****************************************************************
// Select.ini�F���
// ================================================================

bool C_tnm_select_ini::analize()
{
	ARRAY<TSTR> ini_file_data;

	// Select.ini ������ꍇ
	if (file_get_type(_T("Select.ini")) == 1)	{

		// �ǂݍ���
		ARRAY<TSTR> file_data;
		if (!C_file::read_full_text(G_app.module_dir + _T("\\Select.ini"), file_data))
		{
			set_last_error(_T("Select.ini ���J���܂���ł����B"), _T(""));
			return false;
		}
		if (file_data.empty())
		{
			set_last_error(_T("Select.ini ����ł��B"), _T(""));
			return false;
		}
		if (file_data[0].empty())
		{
			set_last_error(_T("Select.ini �̂P�s�ڂ���ł��B"), _T(""));
			return false;
		}

		// �s�����
		for (int i = 0; i < (int)file_data.size(); i++)
		{
			ARRAY<TSTR> line_data;
			boost::split(line_data, file_data[i], boost::is_any_of(_T("\t")));

			if (!line_data.empty())
			{
				if (line_data.size() != 2)
				{
					set_last_error(_T("Select.ini �� ") + tostr(i + 1) + _T(" �s�ڂ̃p�����[�^������܂���B"), _T(""));
					return false;
				}

				C_item item;
				item.m_dir = line_data[0];
				item.m_name = line_data[1];

				m_append_list.push_back(item);
			}
		}
	}
	// Select.ini ���Ȃ��ꍇ
	else
	{
		// �ԕ�������
		C_item item;

		m_append_list.push_back(item);
	}

	return true;
}

// ****************************************************************
// Select.ini�F�擪�̃f�B���N�g�����擾
// ================================================================
TSTR C_tnm_select_ini::first_dir()
{
	if (m_append_list.empty())
	{
		return _T("");
	}

	return m_append_list[0].m_dir;
}

TSTR C_tnm_select_ini::first_name()
{
	if (m_append_list.empty())
	{
		return _T("");
	}

	return m_append_list[0].m_name;
}

// ****************************************************************
// Gameexe.ini�F����������
// ================================================================
C_tnm_ini::C_tnm_ini()
{
	// ================================================================
	// gameexe.ini �̃f�t�H���g�l�������Ŏw�肵�܂��B
	// ----------------------------------------------------------------

	// �f�o�b�O
	debug_error_patno_out_of_range = 0;

	// �V�X�e��
	game_id = _T("SampleProject");
	game_name = _T("�T���v���v���W�F�N�g");
	game_version_str = _T("");
	disc_mark = _T("SampleProject.env");
	manual_path_str = _T("");
	screen_size = C_size(800, 600);
	start_scene_name = _T("_start");
	start_scene_z_no = 0;
	menu_scene_name = _T("_menu");
	menu_scene_z_no = 0;
	cancel_scene_name = _T("");
	cancel_scene_z_no = 0;
	config_scene_name = _T("");
	config_scene_z_no = 0;
	save_scene_name = _T("");
	save_scene_z_no = 0;
	load_scene_name = _T("");
	load_scene_z_no = 0;
	load_after_call_scene = _T("");
	load_after_call_z_no = 0;
	dummy_check_str = _T("�i�Q�[�����j�̃f�B�X�N�����Ă��������B\n\n���v���e�N�g�̌�F���ł͂���܂���B");
	dummy_check_ok_str = _T("���̓x�́i�Q�[�����j���������グ���������A���肪�Ƃ��������܂��B");

	// �t���b�N�V�[��
	flick_scene_cnt = 0;
	for (int i = 0; i < INIMIN_FLICK_SCENE_CNT; i++)
	{
		flick_scene_name[i] = _T("");
		flick_scene_z_no[i] = 0;
		flick_scene_angle[i] = 0;
	}

	// ���ϐ�
	system_int_value_cnt = 0;
	system_str_value_cnt = 0;
	for (int i = 0; i < INIMAX_SYSTEM_VALUE_CNT; i++)	{
		system_int_value[i] = 0;
		system_str_value[i].clear();
	}

	// �V���[�g�J�b�g�L�[
	shortcut_cnt = INIDEF_SHORTCUT_CNT;
	for (int i = 0; i < INIMAX_SHORTCUT_CNT; i++)	{
		shortcut_key[i] = -1;
		shortcut_cond[i] = 0;
		shortcut_scn[i] = _T("");
		shortcut_z_no[i] = 0;
		shortcut_cmd[i] = _T("");
	}

// �����������F�E�鎖���X�g������
// �����O�̕ϊ��e�[�u���{�����F
// �����[�_���ł̏ꍇ�A�A�b�v�f�[�g���Ȃ��Ɩ��H�i�f�o�b�O���j���[�̍��ڂɖ��_�y�ёΉ�����L�q���Ă���܂��j
// ���o�b�N���O�̃X���[�X�X�N���[��
// ���f�O�P

	// �A�C�R��
	icon_cnt = INIDEF_ICON_CNT;
	for (int i = 0; i < INIMAX_ICON_CNT; i++)	{
		icon[i].anime_speed = 100;
		icon[i].anime_pat_cnt = 1;
	}

	// �}�E�X�J�[�\��
	mouse_cursor_cnt = INIDEF_MOUSE_CURSOR_CNT;
	for (int i = 0; i < INIMAX_MOUSE_CURSOR_CNT; i++)	{
		mouse_cursor[i].anime_speed = 100;
	}
	mouse_cursor_default = -1;

	// �V�X�e���R�}���h���j���[�F���݂̗L��
	syscommenu.read_skip.exist = true;								// ���F��x�ǂ񂾕��͂̑�������J�n���܂��B
	syscommenu.unread_skip.exist = true;							// ���F���ǂ̕��͂ł���������J�n���܂��B�i���V�X�e���ݒ�́u���ǂ̕��͂�������ł���悤�ɂ���B�v���L���̏ꍇ�ł��j
	syscommenu.auto_skip.exist = true;								// ���F����������@�\���n�m�^�n�e�e���܂��B
	syscommenu.auto_mode.exist = true;								// ���F�I�[�g���[�h���J�n���܂��B
	syscommenu.hide_mwnd.exist = true;								// ���F���b�Z�[�W�E�B���h�E���B���܂��B
	syscommenu.msg_back.exist = true;								// ���F���b�Z�[�W�o�b�N���J���܂��B
	syscommenu.save.exist = true;									// ���F�Z�[�u���Ăяo���܂��B�i#SAVE_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A�Z�[�u�_�C�A���O��\�����܂��j
	syscommenu.load.exist = true;									// ���F���[�h���Ăяo���܂��B�i#LOAD_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���[�h�_�C�A���O��\�����܂��j
	syscommenu.return_to_sel.exist = true;							// ���F�O�̑I�����ɖ߂�܂��B
	syscommenu.config.exist = true;									// ���F���ݒ���Ăяo���܂��B�i#CONFIG_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���ݒ�_�C�A���O��\�����܂��j
	syscommenu.manual.exist = true;									// ���F�}�j���A����\�����܂��B�i�}�j���A���ւ̃p�X�� #MANUAL_PATH �Őݒ肵�ĉ������j
	syscommenu.version.exist = true;								// ���F�o�[�W��������\�����܂��B�i���� #GAMEVERSION �Őݒ肵�ĉ������j
	syscommenu.return_to_menu.exist = true;							// ���F�^�C�g�����j���[�ɖ߂�܂��B
	syscommenu.game_end.exist = true;								// ���F�Q�[�����I�����܂��B
	syscommenu.cancel.exist = true;									// ���F�V�X�e���R�}���h���j���[���L�����Z�����܂��B

	// �V�X�e���R�}���h���j���[�F���^�֎~
	syscommenu.read_skip.enable = true;								// ���F��x�ǂ񂾕��͂̑�������J�n���܂��B
	syscommenu.unread_skip.enable = true;							// ���F���ǂ̕��͂ł���������J�n���܂��B�i���V�X�e���ݒ�́u���ǂ̕��͂�������ł���悤�ɂ���B�v���L���̏ꍇ�ł��j
	syscommenu.auto_skip.enable = true;								// ���F����������@�\���n�m�^�n�e�e���܂��B
	syscommenu.auto_mode.enable = true;								// ���F�I�[�g���[�h���J�n���܂��B
	syscommenu.hide_mwnd.enable = true;								// ���F���b�Z�[�W�E�B���h�E���B���܂��B
	syscommenu.msg_back.enable = true;								// ���F���b�Z�[�W�o�b�N���J���܂��B
	syscommenu.save.enable = true;									// ���F�Z�[�u���Ăяo���܂��B�i#SAVE_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A�Z�[�u�_�C�A���O��\�����܂��j
	syscommenu.load.enable = true;									// ���F���[�h���Ăяo���܂��B�i#LOAD_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���[�h�_�C�A���O��\�����܂��j
	syscommenu.return_to_sel.enable = true;							// ���F�O�̑I�����ɖ߂�܂��B
	syscommenu.config.enable = true;								// ���F���ݒ���Ăяo���܂��B�i#CONFIG_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���ݒ�_�C�A���O��\�����܂��j
	syscommenu.manual.enable = true;								// ���F�}�j���A����\�����܂��B�i�}�j���A���ւ̃p�X�� #MANUAL_PATH �Őݒ肵�ĉ������j
	syscommenu.version.enable = true;								// ���F�o�[�W��������\�����܂��B�i���� #GAMEVERSION �Őݒ肵�ĉ������j
	syscommenu.return_to_menu.enable = true;						// ���F�^�C�g�����j���[�ɖ߂�܂��B
	syscommenu.game_end.enable = true;								// ���F�Q�[�����I�����܂��B
	syscommenu.cancel.enable = true;								// ���F�V�X�e���R�}���h���j���[���L�����Z�����܂��B

	// �V�X�e���R�}���h���j���[�F������
	syscommenu.read_skip.str = LCL_STR_SYSCOMMENU_READ_SKIP_STR;			// ���F��x�ǂ񂾕��͂̑�������J�n���܂��B
	syscommenu.unread_skip.str = LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR;		// ���F���ǂ̕��͂ł���������J�n���܂��B�i���V�X�e���ݒ�́u���ǂ̕��͂�������ł���悤�ɂ���B�v���L���̏ꍇ�ł��j
	syscommenu.auto_skip.str = LCL_STR_SYSCOMMENU_AUTO_AKIP_STR;			// ���F����������@�\���n�m�^�n�e�e���܂��B
	syscommenu.auto_mode.str = LCL_STR_SYSCOMMENU_AUTO_MODE_STR;			// ���F�I�[�g���[�h���J�n���܂��B
	syscommenu.hide_mwnd.str = LCL_STR_SYSCOMMENU_HIDE_MWND_STR;			// ���F���b�Z�[�W�E�B���h�E���B���܂��B
	syscommenu.msg_back.str = LCL_STR_SYSCOMMENU_MSG_BACK_STR;				// ���F���b�Z�[�W�o�b�N���J���܂��B
	syscommenu.save.str = LCL_STR_SYSCOMMENU_SAVE_STR;						// ���F�Z�[�u���Ăяo���܂��B�i#SAVE_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A�Z�[�u�_�C�A���O��\�����܂��j
	syscommenu.load.str = LCL_STR_SYSCOMMENU_LOAD_STR;						// ���F���[�h���Ăяo���܂��B�i#LOAD_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���[�h�_�C�A���O��\�����܂��j
	syscommenu.return_to_sel.str = LCL_STR_SYSCOMMENU_RETURN_SEL_STR;		// ���F�O�̑I�����ɖ߂�܂��B
	syscommenu.config.str = LCL_STR_SYSCOMMENU_CONFIG_STR;					// ���F���ݒ���Ăяo���܂��B�i#CONFIG_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���ݒ�_�C�A���O��\�����܂��j
	syscommenu.manual.str = LCL_STR_SYSCOMMENU_MANUAL_STR;					// ���F�}�j���A����\�����܂��B�i�}�j���A���ւ̃p�X�� #MANUAL_PATH �Őݒ肵�ĉ������j
	syscommenu.version.str = LCL_STR_SYSCOMMENU_VERSION_STR;				// ���F�o�[�W��������\�����܂��B�i���� #GAMEVERSION �Őݒ肵�ĉ������j
	syscommenu.return_to_menu.str = LCL_STR_SYSCOMMENU_RETURN_MENU_STR;		// ���F�^�C�g�����j���[�ɖ߂�܂��B
	syscommenu.game_end.str = LCL_STR_SYSCOMMENU_GAME_END_STR;				// ���F�Q�[�����I�����܂��B
	syscommenu.cancel.str = LCL_STR_SYSCOMMENU_CANCEL_STR;					// ���F�V�X�e���R�}���h���j���[���L�����Z�����܂��B

	// �V�X�e���R�}���h���j���[�F���[�J���ėp�X�C�b�`
	syscommenu.local_extra_switch_cnt = INIDEF_LOCAL_EXTRA_SWITCH_CNT;		// �|�F���[�J���ėp�X�C�b�`�̐�
	for (int i = 0; i < INIMAX_LOCAL_EXTRA_SWITCH_CNT; i++)	{
		syscommenu.local_extra_switch[i].exist = (i < 4) ? true : false;	// ���F���[�J���ėp�X�C�b�`�F���݂̗L���i���f�t�H���g�łS���݂���j
		syscommenu.local_extra_switch[i].enable = true;						// ���F���[�J���ėp�X�C�b�`�F���^�֎~
		syscommenu.local_extra_switch[i].onoff = true;						// ���F���[�J���ėp�X�C�b�`�FON/OFF���
		syscommenu.local_extra_switch[i].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i);	// ���F���[�J���ėp�X�C�b�`�F���[�h��
	}

	// �V�X�e���R�}���h���j���[�F���[�J���ėp���[�h
	syscommenu.local_extra_mode_cnt = INIDEF_LOCAL_EXTRA_MODE_CNT;		// �|�F���[�J���ėp���[�h�̐�
	for (int i = 0; i < INIMAX_LOCAL_EXTRA_MODE_CNT; i++)	{
		syscommenu.local_extra_mode[i].exist = (i < 4) ? true : false;		// ���F���[�J���ėp���[�h�F���݂̗L���i���f�t�H���g�łS���݂���j
		syscommenu.local_extra_mode[i].enable = true;						// ���F���[�J���ėp���[�h�F���^�֎~
		syscommenu.local_extra_mode[i].mode = 0;							// ���F���[�J���ėp���[�h�F���[�h�l
		syscommenu.local_extra_mode[i].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i);	// ���F���[�J���ėp���[�h�F���ږ�
		syscommenu.local_extra_mode[i].item_cnt = INIDEF_LOCAL_EXTRA_MODE_ITEM_CNT;			// ���F���[�J���ėp���[�h�̃A�C�e���̐�
		for (int j = 0; j < INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT; j++)	{
			syscommenu.local_extra_mode[i].item[j].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(j);	// ���F���[�J���ėp���[�h�̃A�C�e���F���ږ�
		}
	}

	// ���[�U�R���t�B�O�F�_�C�A���O�X�^�C��
	config.dlg_style_volume = 0;									// ���F���ʃ_�C�A���O�̃X�^�C���i0=���ʁ��a�f�l�t�F�[�h  1=���ʂ̂݁j
	config.dlg_style_koe = 0;										// ���F�����_�C�A���O�̃X�^�C���i0=�������[�h���L�����N�^�[����  1=�������[�h�̂�  2=�L�����N�^�[�����̂݁j

	// ���[�U�R���t�B�O�F�^�u�̑��݂̗L��
	config.tab_exist_screen = true;									// ���F�u��ʁv�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_volume = true;									// ���F�u���ʁv�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_message = true;								// ���F�u���́v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_mwndbk = true;									// ���F�u�w�i�F�v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_koe = true;									// ���F�u�����v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_automode = true;								// ���F�u�I�[�g���[�h�v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_jitan = true;									// ���F�u���Z�Đ��v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_else = true;									// ���F�u���̑��v�^�u�̑��݂̗L���i0=���� 1=�L��j
	config.tab_exist_system = true;									// ���F�u�V�X�e���v�^�u�̑��݂̗L���i0=���� 1=�L��j

	// ���[�U�R���t�B�O�F���ڂ̑��݂̗L��
	config.exist_bgm = true;										// ���F�u�a�f�l�v���ڂ̑��݂̗L��
	config.exist_koe = true;										// ���F�u�����v���ڂ̑��݂̗L��
	config.exist_pcm = true;										// ���F�u���ʉ��v���ڂ̑��݂̗L��
	config.exist_se = true;											// ���F�u�V�X�e�����v���ڂ̑��݂̗L��
	config.exist_mov = true;										// ���F�u���[�r�[�v���ڂ̑��݂̗L��

	// ���[�U�R���t�B�O�F�X�N���[���T�C�Y
	config.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;			// ���F�X�N���[���T�C�Y�̃��[�h
	config.screen_size_scale = C_size(100, 100);						// �|�F�X�N���[���T�C�Y�̃X�P�[��

	// ���[�U�R���t�B�O�F�t���X�N���[���ڍ�
	config.fullscreen_mode = TNM_FULLSCREEN_MODE_FIT;				// �|�F�t���X�N���[���̕\�����[�h
	config.fullscreen_scale = C_size(100, 100);						// �|�F�t���X�N���[���̃X�P�[��
	config.fullscreen_scale_sync_switch = true;						// �|�F�t���X�N���[���̃X�P�[���̓����X�C�b�`
	config.fullscreen_move = C_size(0, 0);							// �|�F�t���X�N���[���̈ړ�

	// ���[�U�R���t�B�O�F����
	config.all_user_volume = 255;									// ���F�S�̃{�����[��
	config.bgm_user_volume = 255;									// ���FBGM �{�����[��
	config.koe_user_volume = 255;									// ���FKOE �{�����[��
	config.pcm_user_volume = 255;									// ���FPCM �{�����[��
	config.se_user_volume = 255;									// ���FSE �{�����[��
	config.mov_user_volume = 255;									// ���FMOV �{�����[��
	config.play_all_check = true;									// �|�F�S�̍Đ��`�F�b�N
	config.play_bgm_check = true;									// �|�FBGM �Đ��`�F�b�N
	config.play_koe_check = true;									// �|�FKOE �Đ��`�F�b�N
	config.play_pcm_check = true;									// �|�FPCM �Đ��`�F�b�N
	config.play_se_check = true;									// �|�FSE �Đ��`�F�b�N
	config.play_mov_check = true;									// �|�FMOV �Đ��`�F�b�N

	// ���[�U�R���t�B�O�F�a�f�l�t�F�[�h
	config.bgmfade_volume = 192;									// ���F�a�f�l�t�F�[�h�{�����[��
	config.bgmfade_use_check = true;								// ���F�a�f�l�t�F�[�h�`�F�b�N

	// ���[�U�R���t�B�O�F�t�B���^�[�F
	config.filter_color = C_argb(128, 0, 0, 0);						// ���F�t�B���^�[�F

	// ���[�U�R���t�B�O�F�t�H���g
	config.font_name = TNM_FONT_NAME_MS_GOTHIC;						// ���F�t�H���g�F���O�i�����O�𒼐ڎw�肷��̂ł͂Ȃ��A�^�C�v�Ŏw�肵�܂��j
	config.font_type = TNM_FONT_TYPE_MS_GOTHIC;						// ���F�t�H���g�F��ށi0=�l�r�S�V�b�N 1=�l�r���� 2=���C���I 3=�Ǝ��t�H���g�j
	config.font_futoku = false;										// ���F�t�H���g�F��������i0=OFF 1=ON�j
	config.font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU;				// ���F�t�H���g�F�e���[�h�i0=�e�Ȃ� 1=�e���� 2=�����j
	config.font_sample_str_short = LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT;	// ���F�t�H���g�F�T���v��������V���[�g
	config.font_sample_str_long  = LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG;		// ���F�t�H���g�F�T���v�������񃍃��O

	// ���[�U�R���t�B�O�F�������x
	config.message_speed = 20;										// ���F�������x
	config.message_speed_nowait = false;							// ���F�������x�i�m�[�E�F�C�g�j

	// ���[�U�R���t�B�O�F�I�[�g���[�h
	config.auto_mode_onoff = false;									// �|�F�I�[�g���[�h
	config.auto_mode_moji_wait = 70;								// �|�F�I�[�g���[�h��������
	config.auto_mode_min_wait = 300;								// �|�F�I�[�g���[�h�ŏ�����

	// ���[�U�R���t�B�O�F�����Ń}�E�X�J�[�\�����B��
	config.mouse_cursor_hide_onoff = false;							// �|�F�����Ń}�E�X�J�[�\��������
	config.mouse_cursor_hide_time = 5000;							// �|�F�����Ń}�E�X�J�[�\��������

	// ���[�U�R���t�B�O�F���Z�Đ�
	config.jitan_normal_onoff = false;								// �|�F���Z�Đ��i���͂𕁒ʂɓǂݐi�߂Ă��鎞�j
	config.jitan_auto_mode_onoff = false;							// �|�F���Z�Đ��i�I�[�g���[�h���j
	config.jitan_msgbk_onoff = false;								// �|�F���Z�Đ��i���͂̊����߂����j
	config.jitan_speed = 100;										// �|�F���Z�Đ����x

	// ���[�U�R���t�B�O�F�������[�h
	config.koe_mode = TNM_KOE_MODE_NORMAL;							// �|�F�������[�h

	// ���[�U�R���t�B�O�F�L�����N�^�[����
	config.chrkoe_cnt = INIDEF_CHRKOE_CNT;
	config.chrkoe_not_look_name_str = _T("�H�H�H");					// ���F�����p�̕�����
	for (int i = 0; i < INIMAX_CHRKOE_CNT; i++)	{
		config.chrkoe[i].name_str = _T("");							// ���F���O
		config.chrkoe[i].check_mode = TNM_CHRKOE_CHECK_MODE_LOOK;	// ���F�`�F�b�N���[�h�i0=������ԂŃ`�F�b�N���Ȃ�  1=���J��ԂŃ`�F�b�N���Ȃ�  2=������ԂŃ`�F�b�N����j
		config.chrkoe[i].check_name_str = _T("");					// ���F�`�F�b�N�p�̖��O
		config.chrkoe[i].onoff = true;								// �|�FON/OFF���
		config.chrkoe[i].volume = 255;								// �|�F����
		config.chrkoe[i].chr_no_list.clear();						// ���F���̐���ԍ����X�g
	}

	// ���[�U�R���t�B�O�F���͂̐F����
	config.message_chrcolor.exist = true;							// ���F���͂̐F����
	config.message_chrcolor.onoff = true;							// ���F���͂̐F����
	config.message_chrcolor.str = _T("���͂�F��������B");			// ���F���͂̐F����

	// ���[�U�R���t�B�O�F�I�u�W�F�N�g�̕\���t���O
	config.object_disp_cnt = INIDEF_OBJECT_DISP_FLAG_CNT;			// �|�F�I�u�W�F�N�g�̕\���t���O�̐�
	for (int i = 0; i < INIMAX_OBJECT_DISP_FLAG_CNT; i++)	{
		config.object_disp[i].exist =  (i < 2) ? true : false;		// ���F�I�u�W�F�N�g�̕\���t���O�i���f�t�H���g�łQ���݂���j
		config.object_disp[i].onoff = true;							// ���F�I�u�W�F�N�g�̕\���t���O
		config.object_disp[i].str = _T("�I�u�W�F�N�g�\��") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("�Ԃ�\������B");		// ���F�I�u�W�F�N�g�̕\���t���O
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp�X�C�b�`
	config.global_extra_switch_cnt = INIDEF_GLOBAL_EXTRA_SWITCH_CNT;	// �|�F�O���[�o���ėp�X�C�b�`�̐�
	for (int i = 0; i < INIMAX_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		config.global_extra_switch[i].exist = (i < 2) ? true : false;	// ���F�O���[�o���ėp�X�C�b�`�F���݂̗L���i���f�t�H���g�łQ���݂���j
		config.global_extra_switch[i].onoff = true;						// ���F�O���[�o���ėp�X�C�b�`�FON/OFF���
		config.global_extra_switch[i].str = _T("�O���[�o���ėp�X�C�b�`") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("�Ԃ��g�p����B");		// ���F�O���[�o���ėp�X�C�b�`�F���ږ�
	}

	// ���[�U�R���t�B�O�F�O���[�o���ėp���[�h
	config.global_extra_mode_cnt = INIDEF_GLOBAL_EXTRA_MODE_CNT;		// �|�F�O���[�o���ėp���[�h�̐�
	for (int i = 0; i < INIMAX_GLOBAL_EXTRA_MODE_CNT; i++)	{
		config.global_extra_mode[i].exist = true;						// ���F�O���[�o���ėp���[�h�F���݂̗L��
		config.global_extra_mode[i].mode = 0;							// ���F�O���[�o���ėp���[�h�F���[�h�l
		config.global_extra_mode[i].str = _T("�O���[�o���ėp���[�h") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("��");			// ���F�O���[�o���ėp���[�h�F���ږ�
		switch (i)	{
			case 0:
			case 1:
				config.global_extra_mode[i].item_cnt = 3;				// ���F�O���[�o���ėp���[�h�̃A�C�e���̐��i���f�t�H���g�łR���݂���j
				break;
			default:
				config.global_extra_mode[i].item_cnt = 1;				// ���F�O���[�o���ėp���[�h�̃A�C�e���̐��i���f�t�H���g�łP���݂���j
				break;
		}
		for (int j = 0; j < INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT; j++)	{
			config.global_extra_mode[i].item[j].str = _T("���[�h") + str_to_zenkaku_ret(str_format(_T("%d"), j));		// ���F�O���[�o���ėp���[�h�̃A�C�e���F���ږ�
		}
	}

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�
	// �i���݂̗L���j
	config.system.sleep.exist = true;								// ���F�{�v���O�����̓����x�����āA���̃v���O�������X���[�Y�ɓ��삷��悤�ɂ���B
	config.system.no_wipe_anime.exist = true;						// ���F��ʈÓ]���ʂ̃A�j���𖳌��ɂ���B
	config.system.skip_wipe_anime.exist = true;						// ���F��ʈÓ]���ʂ��}�E�X�N���b�N�Ŕ�΂��B
	config.system.no_mwnd_anime.exist = true;						// ���F���b�Z�[�W�E�B���h�E�̊J���̃A�j���𖳌��ɂ���B
	config.system.wheel_next_message.exist = true;					// ���F�}�E�X�̃z�C�[���{�^���̉��񂵂ŕ��͂�ǂݐi�߂�B
	config.system.koe_dont_stop.exist = true;						// ���F���̍Đ����Ɏ��̕��͂ɐi��ł��Đ��𑱂���B
	config.system.skip_unread_message.exist = true;					// ���F���ǂ̕��͂�������ł���悤�ɂ���B
	config.system.play_silent_sound.exist = true;					// ���F�T�E���h�Đ����ɎG��������ꍇ�̓`�F�b�N���ĉ������B
	// �i������ԁj
	config.system.sleep.onoff = false;								// ���F�{�v���O�����̓����x�����āA���̃v���O�������X���[�Y�ɓ��삷��悤�ɂ���B
	config.system.no_wipe_anime.onoff = false;						// ���F��ʈÓ]���ʂ̃A�j���𖳌��ɂ���B
	config.system.skip_wipe_anime.onoff = true;						// ���F��ʈÓ]���ʂ��}�E�X�N���b�N�Ŕ�΂��B
	config.system.no_mwnd_anime.onoff = false;						// ���F���b�Z�[�W�E�B���h�E�̊J���̃A�j���𖳌��ɂ���B
	config.system.wheel_next_message.onoff = true;					// ���F�}�E�X�̃z�C�[���{�^���̉��񂵂ŕ��͂�ǂݐi�߂�B
	config.system.koe_dont_stop.onoff = false;						// ���F���̍Đ����Ɏ��̕��͂ɐi��ł��Đ��𑱂���B
	config.system.skip_unread_message.onoff = false;				// ���F���ǂ̕��͂�������ł���悤�ɂ���B
	// �i������j
	config.system.sleep.str = LCL_STR_SYSTEM_DIALOG_SLEEP_STR;								// ���F�{�v���O�����̓����x�����āA���̃v���O�������X���[�Y�ɓ��삷��悤�ɂ���B
	config.system.no_wipe_anime.str = LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR;				// ���F��ʈÓ]���ʂ̃A�j���𖳌��ɂ���B
	config.system.skip_wipe_anime.str = LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR;			// ���F��ʈÓ]���ʂ��}�E�X�N���b�N�Ŕ�΂��B
	config.system.no_mwnd_anime.str = LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR;				// ���F���b�Z�[�W�E�B���h�E�̊J���̃A�j���𖳌��ɂ���B
	config.system.wheel_next_message.str = LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR;	// ���F�}�E�X�̃z�C�[���{�^���̉��񂵂ŕ��͂�ǂݐi�߂�B
	config.system.koe_dont_stop.str = LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR;				// ���F���̍Đ����Ɏ��̕��͂ɐi��ł��Đ��𑱂���B
	config.system.skip_unread_message.str = LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR;	// ���F���ǂ̕��͂�������ł���悤�ɂ���B
	config.system.play_silent_sound.str = LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR;		// ���F�T�E���h�Đ����ɎG��������ꍇ�̓`�F�b�N���ĉ������B

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i���[�r�[�̍Đ����@�j
	config.system.movie_play_type = TNM_MOVIE_PLAY_TYPE_WMP;		// �|�F���[�r�[�̍Đ����@

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
	config.system.saveload_alert_onoff = true;						// �|�F�Z�[�u���[�h�̌x��

	// ���[�U�R���t�B�O�F�V�X�e���ݒ�i�Z�[�u���[�h�̌x���j
	config.system.saveload_dblclick_onoff = false;					// �|�F�Z�[�u���[�h�̃_�u���N���b�N

	// ���[�U�R���t�B�O�F�Z�[�u���[�h�_�C�A���O
	config.saveload_dialog.data_cnt_par_page = INIDEF_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE;					// ���F�P�y�[�W�i�P�^�u�j������̃f�[�^��

	// ���[�U�R���t�B�O�F�Z�[�u�̃C���t�H���[�V�������
	config.save_info.dlgwnd_caption_title_str = LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR;						// ���F�Z�[�u�_�C�A���O�̃L���v�V�����^�C�g���̕�����
	config.save_info.dlgwnd_deside_button_str = LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR;						// ���F�Z�[�u�_�C�A���O�̌���{�^���̕�����
	config.save_info.dlgwnd_datalist_nameheader_str = LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR;			// ���F�Z�[�u�_�C�A���O�̃f�[�^���X�g�̐擪������
	config.save_info.dlgwnd_warning_chkbox_str = LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR;					// ���F�Z�[�u�_�C�A���O�̊m�F�`�F�b�N�{�b�N�X�̕�����
	config.save_info.dlgwnd_dblclick_chkbox_str = LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR;					// ���F�Z�[�u�_�C�A���O�̃_�u���N���b�N�`�F�b�N�{�b�N�X�̕�����
	config.save_info.warning_str = LCL_STR_CONFIG_SAVE_WARNING_STR;											// ���F�Z�[�u�m�F�̊m�F���͂̕�����
	config.save_info.quick_warning_str = LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR;								// ���F�N�C�b�N�Z�[�u�m�F�̊m�F���͂̕�����
	config.save_info.msgbk_warning_str = _T("");															// ���F�Ȃ�

	// ���[�U�R���t�B�O�F���[�h�̃C���t�H���[�V�������
	config.load_info.dlgwnd_caption_title_str = LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR;						// ���F���[�h�_�C�A���O�̃L���v�V�����^�C�g���̕�����
	config.load_info.dlgwnd_deside_button_str = LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR;						// ���F���[�h�_�C�A���O�̌���{�^���̕�����
	config.load_info.dlgwnd_datalist_nameheader_str = LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR;			// ���F���[�h�_�C�A���O�̃f�[�^���X�g�̐擪������
	config.load_info.dlgwnd_warning_chkbox_str = LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR;					// ���F���[�h�_�C�A���O�̊m�F�`�F�b�N�{�b�N�X�̕�����
	config.load_info.dlgwnd_dblclick_chkbox_str = LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR;					// ���F���[�h�_�C�A���O�̃_�u���N���b�N�`�F�b�N�{�b�N�X�̕�����
	config.load_info.warning_str = LCL_STR_CONFIG_LOAD_WARNING_STR;											// ���F���[�h�m�F�̊m�F���͂̕�����
	config.load_info.quick_warning_str = LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR;								// ���F�N�C�b�N���[�h�m�F�̊m�F���͂̕�����
	config.load_info.msgbk_warning_str = LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR;								// ���F���b�Z�[�W�o�b�N���[�h�m�F�̊m�F���͂̕�����

	// ���[�U�R���t�B�O�F�e��m�F���b�Z�[�W�̕�����
	config.warninginfo.restart_warning_str = LCL_STR_CONFIG_RESTART_WARNING_STR;							// ���F�Q�[�����ŏ�����n�߂鎞�̊m�F���b�Z�[�W�̕�����i�f�o�b�O�@�\�j
	config.warninginfo.scenestart_warning_str = LCL_STR_CONFIG_SCENESTART_WARNING_STR;						// ���F�Q�[����r������n�߂鎞�̊m�F���b�Z�[�W�̕�����i�f�o�b�O�@�\�j
	config.warninginfo.returnmenu_warning_str = LCL_STR_CONFIG_RETURNMENU_WARNING_STR;						// ���F�^�C�g�����j���[�ɖ߂鎞�̊m�F���b�Z�[�W�̕�����
	config.warninginfo.returnsel_warning_str = LCL_STR_CONFIG_RETURNSEL_WARNING_STR;						// ���F�O�̑I�����ɖ߂鎞�̊m�F���b�Z�[�W�̕�����
	config.warninginfo.gameend_warning_str = LCL_STR_CONFIG_GAMEEND_WARNING_STR;							// ���F�Q�[�����I�����鎞�̊m�F���b�Z�[�W�̕�����

	// �Z�[�u�f�[�^��
	save_cnt = INIDEF_SAVE_CNT;

	// �N�C�b�N�Z�[�u�f�[�^��
	quick_save_cnt = INIDEF_QUICK_SAVE_CNT;

	// �G���h�Z�[�u
	end_save_cnt = INIDEF_END_SAVE_CNT;

	// �Z�[�u�|�C���g�����̃f�[�^��
	save_history_cnt = INIDEF_SAVE_HISTORY_CNT;

	// �o�b�N���O�Z�[�u�̃f�[�^��
	backlog_save_cnt = INIDEF_BACKLOG_SAVE_CNT;
	backlog_save_interval = 1;

	// �O�̑I�����ɖ߂�̗���
	sel_save_cnt = INIDEF_SEL_SAVE_CNT;

	// �C���i�[�Z�[�u�f�[�^��
	inner_save_cnt = INIDEF_INNER_SAVE_CNT;

	// ���݂��Ȃ��Z�[�u�f�[�^�̕�����
	save_no_data_str = LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR;

	// �Z�[�u�T���l�C��
	use_save_thumb = false;
	save_thumb_type = TNM_SAVE_THUMB_TYPE_BMP;
	save_thumb_size = C_size(200, 150);

	// ���[�h���̃��C�v
	load_wipe_type = 0;
	load_wipe_time = 1000;

	// �c�����^������
	tateyoko_mode = 0;

	// �y���O�z
	namae_list.clear();

	// �t���O
	flag_cnt = INIDEF_FLAG_CNT;
	global_flag_cnt = INIDEF_GLOBAL_FLAG_CNT;
	call_flag_cnt = INIDEF_CALL_FLAG_CNT;

	// �f�[�^�x�[�X
	database_cnt = INIDEF_DATABASE_CNT;
	for (int i = 0; i < INIMAX_DATABASE_CNT; i++)	{
		database_name[i].clear();
	}

	// �J�E���^
	counter_cnt = INIDEF_COUNTER_CNT;

	// �J���[�e�[�u��
	color_table_cnt = INIDEF_COLOR_TABLE_CNT;
	for (int i = 0; i < INIMAX_COLOR_TABLE_CNT; i++)	{
		color_table[i] = C_rgb(255, 255, 255);
	}
	color_table[0] = C_rgb(255, 255, 255);
	color_table[1] = C_rgb(  0,   0,   0);
	color_table[2] = C_rgb(255,   0,   0);
	color_table[3] = C_rgb(  0, 255,   0);
	color_table[4] = C_rgb(  0,   0, 255);
	color_table[5] = C_rgb(255, 255,   0);
	color_table[6] = C_rgb(255,   0, 255);
	color_table[7] = C_rgb(  0, 255, 255);

	// �g�[���J�[�u
	tonecurve_file = _T("");

	// �b�f�e�[�u��
	cg_table_file = _T("");
	cg_table_flag_cnt = INIDEF_CG_TABLE_FLAG_CNT;

	// �f�O�O�o�b�t�@
	g00_buf_cnt = INIDEF_G00_BUF_CNT;

	// �}�X�N
	mask_cnt = INIDEF_MASK_CNT;

	// �G����
	emoji_cnt = INIDEF_EMOJI_CNT;
	for (int i = 0; i < INIMAX_EMOJI_CNT; i++)	{
		emoji[i].file_name = _T("");
		emoji[i].font_size = 0;
	}

	// �t�H���g�t�@�C��
	font_file_cnt = INIDEF_FONT_FILE_CNT;
	for (int i = 0; i < INIMAX_FONT_FILE_CNT; i++)	{
		font_file[i].file_name = _T("");
	}
	private_font_file_cnt = INIDEF_FONT_FILE_CNT;
	for (int i = 0; i < INIMAX_FONT_FILE_CNT; i++)	{
		private_font_file[i].file_name = _T("");
	}

	// �V�X�e���R�[��
	msg_back_order = INIDEF_MSG_BACK_ORDER;
	excall_order = INIDEF_EXCALL_ORDER;

	// ���[���h
	world_cnt = INIDEF_WORLD_CNT;
	for (int i = 0; i < INIMAX_WORLD_CNT; i++)	{
		world[i].layer = 0;
	}

	// �{�^���F�A�N�V����
	button.action_cnt = INIDEF_BTN_ACTION_CNT;
	for (int i = 0; i < INIMAX_BTN_ACTION_CNT; i++)	{
		for (int j = 0; j < TNM_BTN_STATE_MAX; j++)	{
			button.action[i].state[j].rep_pat_no = 0;
			button.action[i].state[j].rep_pos = C_point(0, 0);
			button.action[i].state[j].rep_tr = 255;
			button.action[i].state[j].rep_bright = 0;
			button.action[i].state[j].rep_dark = 0;
		}

		button.action[i].state[TNM_BTN_STATE_HIT].rep_bright = 32;
		button.action[i].state[TNM_BTN_STATE_PUSH].rep_bright = 32;
		button.action[i].state[TNM_BTN_STATE_PUSH].rep_pos = C_point(1, 1);
	}

	// �{�^���F���ʉ�
	button.se_cnt = INIDEF_BTN_SE_CNT;
	for (int i = 0; i < INIMAX_BTN_SE_CNT; i++)	{
		button.se[i].hit_no = 0;
		button.se[i].push_no = -1;
		button.se[i].decide_no = 1;
	}

	// �{�^���F�O���[�v
	button.group_cnt = INIDEF_BTN_GROUP_CNT;

	// ���b�Z�[�W�{�^��
	msgbtn_cnt = INIDEF_MSGBTN_CNT;
	for (int i = 0; i < INIMAX_MSGBTN_CNT; i++)	{
		for (int j = 0; j < TNM_BTN_STATE_MAX; j++)	{
			msgbtn[i].state[j].color_no = 0;
		}

		msgbtn[i].state[TNM_BTN_STATE_NORMAL].color_no = 0;
		msgbtn[i].state[TNM_BTN_STATE_HIT].color_no = 2;
		msgbtn[i].state[TNM_BTN_STATE_PUSH].color_no = 2;
	}

	// �I�u�W�F�N�g
	object_cnt = INIDEF_OBJECT_CNT;
	for (int i = 0; i < INIMAX_OBJECT_CNT; i++)	{
		object[i].use = true;
		object[i].save = true;
		object[i].space_hide = false;
		object[i].object_disp_no = -1;	// �]��Ȃ�
	}

	// ���b�Z�[�W�E�B���h�E�g
	mwnd_waku_cnt = INIDEF_MWND_WAKU_CNT;
	mwnd_waku_btn_cnt = INIDEF_MWND_WAKU_BTN_CNT;
	mwnd_waku_face_cnt = INIDEF_MWND_WAKU_FACE_CNT;
	mwnd_waku_object_cnt = INIDEF_MWND_WAKU_OBJECT_CNT;
	for (int i = 0; i < INIMAX_MWND_WAKU_CNT; i++)	{
		mwnd_waku[i].extend_type = 0;
		mwnd_waku[i].waku_file = _T("");
		mwnd_waku[i].filter_file = _T("");
		mwnd_waku[i].filter_margin = C_rect(0, 0, 0, 0);
		mwnd_waku[i].filter_color = C_argb(128, 0, 0, 255);
		mwnd_waku[i].filter_config_color = true;
		mwnd_waku[i].filter_config_tr = true;
		mwnd_waku[i].icon_no = -1;
		mwnd_waku[i].page_icon_no = -1;
		mwnd_waku[i].icon_pos_type = 0;
		mwnd_waku[i].icon_pos_base = 0;
		mwnd_waku[i].icon_pos = C_point(0, 0);

		// ���b�Z�[�W�E�B���h�E�g�F�{�^��
		for (int b = 0; b < INIMAX_MWND_WAKU_BTN_CNT; b++)	{
			mwnd_waku[i].btn[b].btn_file = _T("");
			mwnd_waku[i].btn[b].cut_no = 0;
			mwnd_waku[i].btn[b].pos_base = 0;
			mwnd_waku[i].btn[b].pos = C_point(0, 0);
			mwnd_waku[i].btn[b].action_no = 0;
			mwnd_waku[i].btn[b].se_no = 0;
			mwnd_waku[i].btn[b].sys_type = TNM_SYSCOM_TYPE_NONE;
			mwnd_waku[i].btn[b].sys_type_opt = 0;
			mwnd_waku[i].btn[b].btn_mode = 0;
			mwnd_waku[i].btn[b].scn_name = _T("");
			mwnd_waku[i].btn[b].cmd_name = _T("");
			mwnd_waku[i].btn[b].z_no = 0;
		}

		// ���b�Z�[�W�E�B���h�E�g�F��O��
		for (int f = 0; f < INIMAX_MWND_WAKU_FACE_CNT; f++)	{
			mwnd_waku[i].face_pos[f] = C_point(0, 0);
		}
	}

	// ���b�Z�[�W�E�B���h�E
	mwnd.default_mwnd_no = 0;
	mwnd.default_sel_mwnd_no = 1;
	mwnd.order = 1;
	mwnd.filter_layer_rep = 0;
	mwnd.waku_layer_rep = 1;
	mwnd.face_layer_rep = 2;
	mwnd.shadow_layer_rep = 3;
	mwnd.fuchi_layer_rep = 4;
	mwnd.moji_layer_rep = 5;
	mwnd.shadow_color = 1;
	mwnd.fuchi_color = 1;
	mwnd.moji_color = 0;

	mwnd.sub_cnt = INIDEF_MWND_SUB_CNT;
	for (int i = 0; i < INIMAX_MWND_SUB_CNT; i++)	{
		mwnd.sub[i].novel_mode = 0;

		mwnd.sub[i].extend_type = 0;
		mwnd.sub[i].window_pos = C_point(50, 400);
		mwnd.sub[i].window_size = C_size(700, 150);
		mwnd.sub[i].msg_pos = C_point(20, 20);
		mwnd.sub[i].msg_margin = C_rect(20, 20, 20, 20);
		mwnd.sub[i].moji_cnt = C_size(26, 3);
		mwnd.sub[i].moji_size = 25;
		mwnd.sub[i].moji_space = C_size(-1, 10);
		mwnd.sub[i].moji_color = -1;
		mwnd.sub[i].shadow_color = -1;
		mwnd.sub[i].fuchi_color = -1;
		mwnd.sub[i].ruby_size = 10;
		mwnd.sub[i].ruby_space = 1;
		mwnd.sub[i].waku_no = 0;
		mwnd.sub[i].waku_pos = C_point(0, 0);

		mwnd.sub[i].name_disp_mode = 0;

		mwnd.sub[i].name_extend_type = 0;
		mwnd.sub[i].name_window_align = 0;
		mwnd.sub[i].name_window_pos = C_point(0, -100);
		mwnd.sub[i].name_window_size = C_size(300, 100);
		mwnd.sub[i].name_msg_pos = C_point(8, 8);
		mwnd.sub[i].name_msg_pos_rep = C_point(0, 0);
		mwnd.sub[i].name_msg_margin = C_rect(8, 8, 8, 8);
		mwnd.sub[i].name_moji_size = 16;
		mwnd.sub[i].name_moji_space = C_size(-1, 8);
		mwnd.sub[i].name_moji_cnt = 8;
		mwnd.sub[i].name_moji_color = -1;
		mwnd.sub[i].name_shadow_color = -1;
		mwnd.sub[i].name_fuchi_color = -1;
		mwnd.sub[i].name_waku_no = -1;
		mwnd.sub[i].name_waku_pos = C_point(0, 0);

		mwnd.sub[i].talk_margin = C_rect(0, 0, 0, 0);
		mwnd.sub[i].over_flow_check_size = 0;
		mwnd.sub[i].msg_back_insert_nl = 0;

		mwnd.sub[i].face_hide_name = 0;

		mwnd.sub[i].open_anime_type = 0;
		mwnd.sub[i].open_anime_time = 0;
		mwnd.sub[i].close_anime_type = 0;
		mwnd.sub[i].close_anime_time = 0;
	}

	// �{�^���I����
	sel_btn_cnt = INIDEF_SEL_BTN_CNT;
	for (int i = 0; i < INIMAX_SEL_BTN_CNT; i++)	{
		sel_btn[i].base_file = _T("");
		sel_btn[i].filter_file = _T("");
		sel_btn[i].base_pos = C_point(0, 0);
		sel_btn[i].rep_pos = C_point(0, 0);
		sel_btn[i].max_y_cnt = 0;
		sel_btn[i].line_width = 100;
		sel_btn[i].moji_size = 25;
		sel_btn[i].moji_space = C_size(0, 0);
		sel_btn[i].moji_cnt = 0;
		sel_btn[i].moji_x_align = 0;
		sel_btn[i].moji_y_align = 0;
		sel_btn[i].moji_color = 0;
		sel_btn[i].moji_hit_color = 5;
		sel_btn[i].open_anime_type = 1;
		sel_btn[i].open_anime_time = 500;
		sel_btn[i].close_anime_type = 1;
		sel_btn[i].close_anime_time = 500;
		sel_btn[i].decide_anime_type = 1;
		sel_btn[i].decide_anime_time = 500;
	}

	// ���b�Z�[�W�o�b�N
	msg_back.history_cnt = INIDEF_MSG_BACK_HISTORY_CNT;

	msg_back.window_pos = C_point(10, 10);
	msg_back.window_size = C_size(780, 580);
	msg_back.disp_margin = C_rect(20, 20, 20, 20);
	msg_back.msg_pos = 30;
	msg_back.moji_cnt = C_size(20, 15);
	msg_back.moji_space = C_size(-1, 10);
	msg_back.moji_size = 24;

	msg_back.moji_color = 1;
	msg_back.moji_shadow_color = 0;
	msg_back.moji_fuchi_color = 0;
	msg_back.active_moji_color = 7;
	msg_back.active_moji_shadow_color = 0;
	msg_back.active_moji_fuchi_color = 0;
	msg_back.debug_moji_color = 5;
	msg_back.debug_moji_shadow_color = 0;
	msg_back.debug_moji_fuchi_color = 0;

	msg_back.name_disp_mode = TNM_MSGBK_NAME_DISP_TYPE_DEFAULT;
	msg_back.name_bracket_type = TNM_MSGBK_NAME_BRACKET_TYPE_SUMI;
	msg_back.slider_file = _T("");
	msg_back.slider_rect = C_rect(0, 0, 0, 0);
	msg_back.slider_action_no = 0;
	msg_back.slider_se_no = 0;
	msg_back.close_btn_file = _T("");
	msg_back.close_btn_pos = C_point(0, 0);
	msg_back.close_btn_action_no = 0;
	msg_back.close_btn_se_no = 0;
	msg_back.msg_up_btn_file = _T("");
	msg_back.msg_up_btn_pos = C_point(0, 0);
	msg_back.msg_up_btn_action_no = 0;
	msg_back.msg_up_btn_se_no = 0;
	msg_back.msg_down_btn_file = _T("");
	msg_back.msg_down_btn_pos = C_point(0, 0);
	msg_back.msg_down_btn_action_no = 0;
	msg_back.msg_down_btn_se_no = 0;
	msg_back.koe_btn_file = _T("");
	msg_back.koe_btn_pos = C_point(-20, -10);
	msg_back.koe_btn_action_no = 0;
	msg_back.koe_btn_se_no = 0;
	msg_back.load_btn_file = _T("");
	msg_back.load_btn_pos = C_point(-20, 0);
	msg_back.load_btn_action_no = 0;
	msg_back.load_btn_se_no = 0;
	msg_back.separator_file = _T("");
	msg_back.separator_top_file = _T("");
	msg_back.separator_bottom_file = _T("");
	msg_back.msg_click_action = 1;

	// �G�t�F�N�g
	effect_cnt = INIDEF_EFFECT_CNT;

	// �V�F�C�N
	shake_cnt = INIDEF_SHAKE_CNT;
	for (int i = 0; i < INIMAX_SHAKE_CNT; i++)	{
	}

	// �N�G�C�N
	quake_cnt = INIDEF_QUAKE_CNT;

	// �t���[���A�N�V�����`�����l��
	frame_action_ch_cnt = INIDEF_FRAME_ACTION_CH_CNT;

	// �a�f�l
	bgm_cnt = INIDEF_BGM_CNT;
	for (int i = 0; i < INIMAX_BGM_CNT; i++)	{
		bgm[i].regist_name = _T("");
		bgm[i].file_name = _T("");
		bgm[i].start_pos = 0;
		bgm[i].end_pos = -1;
		bgm[i].repeat_pos = 0;
	}

	// ���ʉ��`�����l����
	pcmch_cnt = INIDEF_PCMCH_CNT;

	// ���ʉ��C�x���g��
	pcm_event_cnt = INIDEF_PCM_EVENT_CNT;

	// �V�X�e����
	se_cnt = INIDEF_SE_CNT;
	for (int i = 0; i < INIMAX_SE_CNT; i++)	{
	}

	// �a�f�l�t�F�[�h�Q
	bgmfade2_in_start_time = INIDEF_BGMFADE2_IN_START_TIME;
	bgmfade2_in_time_len = INIDEF_BGMFADE2_IN_TIME_LEN;
	bgmfade2_out_start_time = INIDEF_BGMFADE2_OUT_START_TIME;
	bgmfade2_out_time_len = INIDEF_BGMFADE2_OUT_TIME_LEN;
	bgmfade2_volume = INIDEF_BGMFADE2_VOLUME;

	// �G�f�B�b�g�{�b�N�X
	editbox_cnt = INIDEF_EDITBOX_CNT;
}

// ****************************************************************
// Gameexe.ini�F���
// ================================================================
bool C_tnm_ini::analize()
{
	bool ret = analize_func();

	line_data.clear();

	// �t�H���g��␳
	{
		// �t�H���g�̃t�@�C������ݒ�
		switch (config.font_type)
		{
		case TNM_FONT_TYPE_MS_GOTHIC:
			config.font_name = TNM_FONT_NAME_MS_GOTHIC;
			break;
		case TNM_FONT_TYPE_MS_MINTYOU:
			config.font_name = TNM_FONT_NAME_MS_MINTYOU;
			break;
		case TNM_FONT_TYPE_MEIRYO:
			config.font_name = TNM_FONT_NAME_MS_MEIRYO;
			break;
		case TNM_FONT_TYPE_ORIGINAL:	// �ݒ�ς�
			break;
		default:
			set_last_error(_T("�f�t�H���g�̃t�H���g�^�C�v���s���ł��B"), _T(""));
			return false;
		}

		// �c�����̏ꍇ
		if (tateyoko_mode == 1)
		{
			config.font_name = _T("@") + config.font_name;
		}
	}

	return ret;
}

bool C_tnm_ini::analize_func()
{
	ARRAY<TSTR> ini_file_data;

	// �ǂݍ���
	BUFFER ged_data;
	if (!C_file::read_full_data(G_app.module_dir + _T("\\") + Gp_select_ini->first_dir() + _T("\\") + LCL_GAMEEXE_DAT, ged_data))	{
		set_last_error(LCL_GAMEEXE_DAT _T(" ���J���܂���ł����B"), _T(""));
		return false;
	}

	// ���k�f�[�^�ւ̃|�C���^
	S_tnm_gameexe_dat_header *header = (S_tnm_gameexe_dat_header *)ged_data.get();
	BYTE *sp = ged_data.get() + sizeof(S_tnm_gameexe_dat_header);
	int size = (int)ged_data.size() - sizeof(S_tnm_gameexe_dat_header);

	if (size)	{

		// �d�w�d�Z�b�g�Í�����
		BYTE *dp;
		int eac;
		if (header->exe_angou_mode)	{

			// �d�w�d�Z�b�g�Í��̃R�[�h���擾
			BYTE exe_angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT00A] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT01A] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT02A] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT03A] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT04A] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT05A] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT06A] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT07A] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07A];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT00B] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT01B] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT02B] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT03B] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT04B] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT05B] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT06B] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06B];
			exe_angou_element[TNM_EXE_ANGOU_ELEMENT07B] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07B];

			dp = sp;
			eac = 0;
			for (int i = 0; i < size; i++)	{
				*dp++ ^= exe_angou_element[eac++];
				eac %= TNM_EXE_ANGOU_ELEMENT_CNT;
			}
		}

		// �ȈՈÍ�����
		dp = sp;
		eac = 0;
		for (int i = 0; i < size; i++)	{
			*dp++ ^= tnm_gameexe_dat_angou_code[eac++];
			eac %= TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE;
		}

		// ��
		size = lzss_unpack(sp, NULL);
		BUFFER ged_original;
		ged_original.resize(size);
		dp = ged_original.get();
		lzss_unpack(sp, dp);

		// �s����
		TSTR ged_text((TCHAR *)ged_original.get(), (TCHAR *)ged_original.get_end());
		separate_text_line(ged_text, ini_file_data);
	}

// �����������F�h�m�h�֘A������

// ���R���p�C���� Gameexe.ini �̃R�����g�J�b�g�����������Y��ɂ������B

// �����g�p
// INIMAX_MWND_SET_CNT

// �����̍��ږ��͐������̂ł��傤���H�i�����̂܂܂łn�j�H�j
// "#OBJBTNGROUP . CNT = %d" �� "#BUTTON . GROUP . CNT = %d"

// ���ŏ��l�A�ő�l�A�f�t�H���g�l�̒�`���Ȃ��ł��B�Ȃ̂Ń��~�b�^�[�`�F�b�N�����ĂȂ��ł��B�i���g�̐ݒ�Ɉړ��\��j
// #MWND.FACE_CNT

// ���ő�̊m�ې����K�����ǂ����������鎖�B
// �����ڂɂ���Ă͍ŏ��l���K�v�H�Ⴆ�΃J���[�e�[�u�����O�ɂ�����ǂ��Ȃ��H�i���ŏ��l�̐ݒ�͍�����̂ŁA�l��ݒ肵�ĉ������j

	for (line_no = 0; line_no < (int)ini_file_data.size(); line_no ++)	{
		line_data = ini_file_data[line_no];
		C_str_analizer anl(line_data.begin(), line_data.end());

		if (!analize_step_1(anl, line_no))
			return false;
	}

	return true;
}

// ****************************************************************
// Gameexe.ini�F��́F�X�e�b�v�P
// ================================================================
bool C_tnm_ini::analize_step_1(C_str_analizer& anl, int line_no)
{
	int num[16] = {0};
	TSTR str[16];

	if (false);
	// �V�X�e��
	else if (anl.is_end())	{
		// NOP
	}
	else if (anl.get_format(_T("//")))	{
		// �R�����g�Ȃ̂� NOP
	}
	else if (anl.get_format(_T("#DEBUG_ERROR_PATNO_OUT_OF_RANGE = %d"), &num[0]))	{
		debug_error_patno_out_of_range = num[0];
	}
	else if (anl.get_format(_T("#GAMEID = %s"), &str[0]))	{
		game_id = str[0];
	}
	else if (anl.get_format(_T("#GAMENAME = %s"), &str[0]))	{
		game_name = str[0];
	}
	else if (anl.get_format(_T("#GAMEVERSION = %s"), &str[0]))	{
		game_version_str = str[0];
	}
	else if (anl.get_format(_T("#DISCMARK = %s"), &str[0]))	{
		disc_mark = str[0];
	}
	else if (anl.get_format(_T("#MANUAL_PATH = %s"), &str[0]))	{
		manual_path_str = str[0];
	}
	else if (anl.get_format(_T("#SCREEN_SIZE = %d , %d"), &num[0], &num[1]))	{
		screen_size = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("#START_SCENE = %s , %d"), &str[0], &num[0]))	{
		start_scene_name = str[0];
		start_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#START_SCENE = %s"), &str[0]))	{
		start_scene_name = str[0];
	}
	else if (anl.get_format(_T("#MENU_SCENE = %s , %d"), &str[0], &num[0]))	{
		menu_scene_name = str[0];
		menu_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#MENU_SCENE = %s"), &str[0]))	{
		menu_scene_name = str[0];
	}
	else if (anl.get_format(_T("#CANCEL_SCENE = %s , %d"), &str[0], &num[0]))	{
		cancel_scene_name = str[0];
		cancel_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#CANCEL_SCENE = %s"), &str[0]))	{
		cancel_scene_name = str[0];
	}
	else if (anl.get_format(_T("#CONFIG_SCENE = %s , %d"), &str[0], &num[0]))	{
		config_scene_name = str[0];
		config_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#CONFIG_SCENE = %s"), &str[0]))	{
		config_scene_name = str[0];
	}
	else if (anl.get_format(_T("#SAVE_SCENE = %s , %d"), &str[0], &num[0]))	{
		save_scene_name = str[0];
		save_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#SAVE_SCENE = %s"), &str[0]))	{
		save_scene_name = str[0];
	}
	else if (anl.get_format(_T("#LOAD_SCENE = %s , %d"), &str[0], &num[0]))	{
		load_scene_name = str[0];
		load_scene_z_no = num[0];
	}
	else if (anl.get_format(_T("#LOAD_SCENE = %s"), &str[0]))	{
		load_scene_name = str[0];
	}
	else if (anl.get_format(_T("#LOAD_AFTER_CALL = %s , %d"), &str[0], &num[0]))	{
		load_after_call_scene = str[0];
		load_after_call_z_no = num[0];
	}
	else if (anl.get_format(_T("#LOAD_AFTER_CALL = %s"), &str[0]))	{
		load_after_call_scene = str[0];
	}
	else if (anl.get_format(_T("#DUMMY_CHECK_STR = %s"), &str[0]))	{
		dummy_check_str = str[0];
	}
	else if (anl.get_format(_T("#DUMMY_CHECK_OK_STR = %s"), &str[0]))	{
		dummy_check_ok_str = str[0];
	}
	// ���ϐ�
	else if (anl.get_format(_T("#SYSTEM . EXTRA_INT_VALUE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SYSTEM_VALUE_CNT, INIMAX_SYSTEM_VALUE_CNT))	{ return false; }
		system_int_value_cnt = num[0];
	}
	else if (anl.get_format(_T("#SYSTEM . EXTRA_INT_VALUE . %d = %d"), &num[0], &num[1]))	{
		if (!analize_check_over(num[0], system_int_value_cnt, INIMAX_SYSTEM_VALUE_CNT))	{ return false; }
		system_int_value[num[0]] = num[1];
	}
	else if (anl.get_format(_T("#SYSTEM . EXTRA_STR_VALUE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SYSTEM_VALUE_CNT, INIMAX_SYSTEM_VALUE_CNT))	{ return false; }
		system_str_value_cnt = num[0];
	}
	else if (anl.get_format(_T("#SYSTEM . EXTRA_STR_VALUE . %d = %s"), &num[0], &str[0]))	{
		if (!analize_check_over(num[0], system_str_value_cnt, INIMAX_SYSTEM_VALUE_CNT))	{ return false; }
		system_str_value[num[0]] = str[0];
	}
	// �t���b�N�V�[��
	else if (anl.get_format(_T("#FLICK_SCENE . CNT = %d"), &num[0]))
	{
		if (!analize_check_alloc(num[0], INIMIN_FLICK_SCENE_CNT, INIMAX_FLICK_SCENE_CNT))	{ return false; }
		flick_scene_cnt = num[0];
	}
	else if (anl.get_format(_T("#FLICK_SCENE . %d . "), &num[0]))
	{
		int flick_scene_num = num[0];

		if (false);
		else if (anl.get_format(_T("SCENE = %s , %d"), &str[0], &num[0]))	{
			flick_scene_name[flick_scene_num] = str[0];
			flick_scene_z_no[flick_scene_num] = num[0];
		}
		else if (anl.get_format(_T("ANGLE = %d"), &num[0]))	{
			flick_scene_angle[flick_scene_num] = num[0];
		}
	}

	// �V���[�g�J�b�g�L�[
	else if (anl.get_format(_T("#SHORTCUT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SHORTCUT_CNT, INIMAX_SHORTCUT_CNT))	{	return false;	}
		shortcut_cnt = num[0];
	}
	else if (anl.get_format(_T("#SHORTCUT . %d . "), &num[0]))	{
		int shortcut_num = num[0];

		if (false);
		else if (anl.get_format(_T("KEY = %d"), &num[0]))	{
			shortcut_key[shortcut_num] = num[0];
		}
		else if (anl.get_format(_T("COND = %d"), &num[0]))	{
			shortcut_cond[shortcut_num] = num[0];
		}
		else if (anl.get_format(_T("SCENE = %s, %s"), &str[0], &str[1]))	{
			shortcut_scn[shortcut_num] = str[0];
			shortcut_cmd[shortcut_num] = str[1];
		}
		else if (anl.get_format(_T("SCENE = %s, %d"), &str[0], &num[0]))	{
			shortcut_scn[shortcut_num] = str[0];
			shortcut_z_no[shortcut_num] = num[0];
		}
		else if (anl.get_format(_T("SCENE = %s"), &str[0]))	{
			shortcut_scn[shortcut_num] = str[0];
			shortcut_z_no[shortcut_num] = 0;
		}
		else	{
		}
	}
	// �A�C�R��
	else if (anl.get_format(_T("#ICON . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_ICON_CNT, INIMAX_ICON_CNT))	{	return false;	}
		icon_cnt = num[0];
	}
	else if (anl.get_format(_T("#ICON . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], icon_cnt, INIMAX_ICON_CNT))	{	return false;	}
		int icon_no = num[0];
		if (false);
		else if (anl.get_format(_T("FILE = %s"), &str[0]))	{
			icon[icon_no].file_name = str[0];
		}
		else if (anl.get_format(_T("CNT = %d"), &num[0]))	{
			icon[icon_no].anime_pat_cnt = num[0];
		}
		else if (anl.get_format(_T("SPEED = %d"), &num[0]))	{
			icon[icon_no].anime_speed = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �}�E�X�J�[�\��
	else if (anl.get_format(_T("#MOUSE_CURSOR . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MOUSE_CURSOR_CNT, INIMAX_MOUSE_CURSOR_CNT))	{	return false;	}
		mouse_cursor_cnt = num[0];
	}
	else if (anl.get_format(_T("#MOUSE_CURSOR . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], mouse_cursor_cnt, INIMAX_MOUSE_CURSOR_CNT))	{	return false;	}
		int mouse_cursor_no = num[0];
		if (false);
		else if (anl.get_format(_T("FILE = %s"), &str[0]))	{
			mouse_cursor[mouse_cursor_no].file_name = str[0];
		}
		else if (anl.get_format(_T("SPEED = %d"), &num[0]))	{
			mouse_cursor[mouse_cursor_no].anime_speed = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	else if (anl.get_format(_T("#MOUSE_CURSOR . DEFAULT = %d"), &num[0]))	{
		if (num[0] != -1 && !analize_check_over(num[0], mouse_cursor_cnt, INIMAX_MOUSE_CURSOR_CNT))	{	return false;	}
		mouse_cursor_default = num[0];
	}
	// �R���t�B�O�����l�̐ݒ�
	else if (anl.get_format(_T("#CONFIG . ")))	{

		if (false);
		else if (anl.get_format(_T("WINDOW_MODE = %d"), &num[0]))	{
			switch (num[0])	{
				case 0:		config.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;	break;
				case 1:		config.screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;	break;
			}
		}
		else if (anl.get_format(_T("VOLUME . ")))	{
			if (false);
			else if (anl.get_format(_T("ALL = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.all_user_volume = num[0];
			}
			else if (anl.get_format(_T("BGM = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.bgm_user_volume = num[0];
			}
			else if (anl.get_format(_T("KOE = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.koe_user_volume = num[0];
			}
			else if (anl.get_format(_T("PCM = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.pcm_user_volume = num[0];
			}
			else if (anl.get_format(_T("SE = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.se_user_volume = num[0];
			}
			else if (anl.get_format(_T("MOV = %d"), &num[0]))	{
				if (!analize_check_value(num[0], 0, 255))	{	return false;	}
				config.mov_user_volume = num[0];
			}
			else	{	return error_dont_analize();	}
		}
		else if (anl.get_format(_T("BGMFADE_VOLUME = %d"), &num[0]))	{
			if (!analize_check_value(num[0], 0, 255))	{	return false;	}
			config.bgmfade_volume = num[0];
		}
		else if (anl.get_format(_T("BGMFADE_ONOFF = %d"), &num[0]))	{
			config.bgmfade_use_check = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("FILTER_COLOR = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
			config.filter_color = C_argb(num[3], num[0], num[1], num[2]);
		}
		else if (anl.get_format(_T("FONT . ")))	{
			if (false);
			else if (anl.get_format(_T("TYPE = %d"), &num[0]))	{
				switch (num[0])	{
				case 0:		config.font_type = TNM_FONT_TYPE_MS_GOTHIC;		break;
				case 1:		config.font_type = TNM_FONT_TYPE_MS_MINTYOU;	break;
				case 2:		config.font_type = TNM_FONT_TYPE_MEIRYO;		break;
				case 3:		config.font_type = TNM_FONT_TYPE_ORIGINAL;		break;
				}
			}
			else if (anl.get_format(_T("NAME = %s"), &str[0]))	{
				config.font_name = str[0];
			}
			else if (anl.get_format(_T("FUTOKU = %d"), &num[0]))	{
				config.font_futoku = (num[0]) ? true : false;
			}
			else if (anl.get_format(_T("SHADOW = %d"), &num[0]))	{
				switch (num[0])	{
				case 0:		config.font_shadow = TNM_FONT_SHADOW_MODE_NONE;				break;
				case 1:		config.font_shadow = TNM_FONT_SHADOW_MODE_SHADOW;			break;
				case 2:		config.font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU;			break;
				case 3:		config.font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW;	break;
				}
			}
			else if (anl.get_format(_T("SAMPLE_STR_SHORT = %s"), &str[0]))	{
				config.font_sample_str_short = str[0];
			}
			else if (anl.get_format(_T("SAMPLE_STR_LONG = %s"), &str[0]))	{
				config.font_sample_str_long = str[0];
			}
			else	{ return error_dont_analize(); }
		}
		else if (anl.get_format(_T("MESSAGE_SPEED = %d"), &num[0]))	{
			config.message_speed = num[0];
		}
		else if (anl.get_format(_T("MESSAGE_SPEED_NOWAIT . ONOFF = %d"), &num[0]))	{
			config.message_speed_nowait = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MESSAGE_CHRCOLOR . ONOFF = %d"), &num[0]))	{
			config.message_chrcolor.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MOUSE_CURSOR_HIDE_ONOFF = %d"), &num[0]))	{
			config.mouse_cursor_hide_onoff = num[0] ? true : false;
		}
		else if (anl.get_format(_T("MOUSE_CURSOR_HIDE_TIME = %d"), &num[0]))	{
			config.mouse_cursor_hide_time = num[0];
		}
		else if (anl.get_format(_T("OBJECT_DISP . %d . ONOFF = %d"), &num[0], &num[1]))	{
			if (!analize_check_over(num[0], config.object_disp_cnt, INIMAX_OBJECT_DISP_FLAG_CNT))	{	return false;	}
			config.object_disp[num[0]].onoff = (num[1]) ? true : false;
		}
		else if (anl.get_format(_T("GLOBAL_EXTRA_SWITCH . %d . ONOFF = %d"), &num[0], &num[1]))	{
			if (!analize_check_over(num[0], config.global_extra_switch_cnt, INIMAX_GLOBAL_EXTRA_SWITCH_CNT))	{	return false;	}
			config.global_extra_switch[num[0]].onoff = (num[1]) ? true : false;
		}
		else if (anl.get_format(_T("GLOBAL_EXTRA_MODE . %d . MODE = %d"), &num[0], &num[1]))	{
			if (!analize_check_over(num[0], config.global_extra_mode_cnt, INIMAX_GLOBAL_EXTRA_MODE_CNT))	{	return false;	}
			config.global_extra_mode[num[0]].mode = num[1];
		}
		else if (anl.get_format(_T("SLEEP . ONOFF = %d"), &num[0]))	{
			config.system.sleep.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("NO_WIPE_ANIME . ONOFF = %d"), &num[0]))	{
			config.system.no_wipe_anime.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SKIP_WIPE_ANIME . ONOFF = %d"), &num[0]))	{
			config.system.skip_wipe_anime.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("NO_MWND_ANIME . ONOFF = %d"), &num[0]))	{
			config.system.no_mwnd_anime.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("WHEEL_NEXT_MESSAGE . ONOFF = %d"), &num[0]))	{
			config.system.wheel_next_message.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("KOE_DONT_STOP . ONOFF = %d"), &num[0]))	{
			config.system.koe_dont_stop.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SKIP_UNREAD_MESSAGE . ONOFF = %d"), &num[0]))	{
			config.system.skip_unread_message.onoff = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("PLAY_SILENT_SOUND . ONOFF = %d"), &num[0]))	{
			//config.system.play_silent_sound.onoff = (num[0]) ? true : false;
		}
		else	{	return error_dont_analize();	}
	}
	// �V�X�e���R�}���h���j���[�F���݂̗L���^������
	else if (anl.get_format(_T("#SYSCOMMENU . ")))	{
		if (false);
		else if (anl.get_format(_T("READ_SKIP = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.read_skip.exist = (num[0]) ? true : false;
			syscommenu.read_skip.enable = (num[1]) ? true : false;
			syscommenu.read_skip.str = str[0];
		}
		else if (anl.get_format(_T("UNREAD_SKIP = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.unread_skip.exist = (num[0]) ? true : false;
			syscommenu.unread_skip.enable = (num[1]) ? true : false;
			syscommenu.unread_skip.str = str[0];
		}
		else if (anl.get_format(_T("AUTO_SKIP = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.auto_skip.exist = (num[0]) ? true : false;
			syscommenu.auto_skip.enable = (num[1]) ? true : false;
			syscommenu.auto_skip.str = str[0];
		}
		else if (anl.get_format(_T("AUTO_MODE = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.auto_mode.exist = (num[0]) ? true : false;
			syscommenu.auto_mode.enable = (num[1]) ? true : false;
			syscommenu.auto_mode.str = str[0];
		}
		else if (anl.get_format(_T("HIDE_MWND = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.hide_mwnd.exist = (num[0]) ? true : false;
			syscommenu.hide_mwnd.enable = (num[1]) ? true : false;
			syscommenu.hide_mwnd.str = str[0];
		}
		else if (anl.get_format(_T("MSG_BACK = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.msg_back.exist = (num[0]) ? true : false;
			syscommenu.msg_back.enable = (num[1]) ? true : false;
			syscommenu.msg_back.str = str[0];
		}
		else if (anl.get_format(_T("SAVE = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.save.exist = (num[0]) ? true : false;
			syscommenu.save.enable = (num[1]) ? true : false;
			syscommenu.save.str = str[0];
		}
		else if (anl.get_format(_T("LOAD = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.load.exist = (num[0]) ? true : false;
			syscommenu.load.enable = (num[1]) ? true : false;
			syscommenu.load.str = str[0];
		}
		else if (anl.get_format(_T("RETURN_SEL = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.return_to_sel.exist = (num[0]) ? true : false;
			syscommenu.return_to_sel.enable = (num[1]) ? true : false;
			syscommenu.return_to_sel.str = str[0];
		}
		else if (anl.get_format(_T("CONFIG = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.config.exist = (num[0]) ? true : false;
			syscommenu.config.enable = (num[1]) ? true : false;
			syscommenu.config.str = str[0];
		}
		else if (anl.get_format(_T("MANUAL = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.manual.exist = (num[0]) ? true : false;
			syscommenu.manual.enable = (num[1]) ? true : false;
			syscommenu.manual.str = str[0];
		}
		else if (anl.get_format(_T("VERSION = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.version.exist = (num[0]) ? true : false;
			syscommenu.version.enable = (num[1]) ? true : false;
			syscommenu.version.str = str[0];
		}
		else if (anl.get_format(_T("RETURN_MENU = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.return_to_menu.exist = (num[0]) ? true : false;
			syscommenu.return_to_menu.enable = (num[1]) ? true : false;
			syscommenu.return_to_menu.str = str[0];
		}
		else if (anl.get_format(_T("GAME_END = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.game_end.exist = (num[0]) ? true : false;
			syscommenu.game_end.enable = (num[1]) ? true : false;
			syscommenu.game_end.str = str[0];
		}
		else if (anl.get_format(_T("CANCEL = %d , %d , %s"), &num[0], &num[1], &str[0]))	{
			syscommenu.cancel.exist = (num[0]) ? true : false;
			syscommenu.cancel.enable = (num[1]) ? true : false;
			syscommenu.cancel.str = str[0];
		}
		// �V�X�e���R�}���h���j���[�F���[�J���ėp�X�C�b�`
		else if (anl.get_format(_T("LOCAL_EXTRA_SWITCH . %d = %d , %d , %d , %s"), &num[0], &num[1], &num[2], &num[3], &str[0]))	{
			if (!analize_check_over(num[0], syscommenu.local_extra_switch_cnt, INIMAX_LOCAL_EXTRA_SWITCH_CNT))	{	return false;	}
			syscommenu.local_extra_switch[num[0]].exist = (num[1]) ? true : false;
			syscommenu.local_extra_switch[num[0]].enable = (num[2]) ? true : false;
			syscommenu.local_extra_switch[num[0]].onoff = (num[3]) ? true : false;
			syscommenu.local_extra_switch[num[0]].str = str[0];
		}
		// �V�X�e���R�}���h���j���[�F���[�J���ėp���[�h
		else if (anl.get_format(_T("LOCAL_EXTRA_MODE . %d = %d , %d , %d , %s"), &num[0], &num[1], &num[2], &num[3], &str[0]))	{
			if (!analize_check_over(num[0], syscommenu.local_extra_mode_cnt, INIMAX_LOCAL_EXTRA_MODE_CNT))	{	return false;	}
			syscommenu.local_extra_mode[num[0]].exist = (num[1]) ? true : false;
			syscommenu.local_extra_mode[num[0]].enable = (num[2]) ? true : false;
			syscommenu.local_extra_mode[num[0]].mode = num[3];
			syscommenu.local_extra_mode[num[0]].str = str[0];
		}
		else if (anl.get_format(_T("LOCAL_EXTRA_MODE . %d . "), &num[0]))	{
			if (!analize_check_over(num[0], syscommenu.local_extra_mode_cnt, INIMAX_LOCAL_EXTRA_MODE_CNT))	{	return false;	}
			int local_extra_mode_no = num[0];
			if (false);
			else if (anl.get_format(_T("ITEM_CNT = %d"), &num[0]))	{
				if (!analize_check_alloc(num[0], INIMIN_LOCAL_EXTRA_MODE_ITEM_CNT, INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT))	{	return false;	}
				syscommenu.local_extra_mode[local_extra_mode_no].item_cnt = num[0];
			}
			else if (anl.get_format(_T("ITEM . %d . STR = %s"), &num[0], &str[0]))	{
				if (!analize_check_over(num[0], INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT, INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT))	{	return false;	}
				syscommenu.local_extra_mode[local_extra_mode_no].item[num[0]].str = str[0];
			}
			else	{	return error_dont_analize();	}
		}
		else	{	return error_dont_analize();	}
	}
	// �_�C�A���O�E�B���h�E�̃X�^�C��
	else if (anl.get_format(_T("#DIALOG_STYLE . ")))	{
		if (false);
		else if (anl.get_format(_T("VOLUME = %d"), &num[0]))	{
			config.dlg_style_volume = num[0];
		}
		else if (anl.get_format(_T("KOE = %d"), &num[0]))	{
			config.dlg_style_koe = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �_�C�A���O�E�B���h�E�̃^�u�̑��݂̗L��
	else if (anl.get_format(_T("#DIALOG_TAB_EXIST . ")))	{
		if (false);
		else if (anl.get_format(_T("SCREEN = %d"), &num[0]))	{
			config.tab_exist_screen = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("VOLUME = %d"), &num[0]))	{
			config.tab_exist_volume = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MESSAGE = %d"), &num[0]))	{
			config.tab_exist_message = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MWNDBK = %d"), &num[0]))	{
			config.tab_exist_mwndbk = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("KOE = %d"), &num[0]))	{
			config.tab_exist_koe = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("AUTOMODE = %d"), &num[0]))	{
			config.tab_exist_automode = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("JITAN = %d"), &num[0]))	{
			config.tab_exist_jitan = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("ELSE = %d"), &num[0]))	{
			config.tab_exist_else = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SYSTEM = %d"), &num[0]))	{
			config.tab_exist_system = (num[0]) ? true : false;
		}
		else	{	return error_dont_analize();	}
	}
	// �_�C�A���O�E�B���h�E�̍��ڂ̑��݂̗L��
	else if (anl.get_format(_T("#DIALOG_EXIST . ")))	{
		if (false);
		else if (anl.get_format(_T("BGM = %d"), &num[0]))	{
			config.exist_bgm = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("KOE = %d"), &num[0]))	{
			config.exist_koe = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("PCM = %d"), &num[0]))	{
			config.exist_pcm = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SE = %d"), &num[0]))	{
			config.exist_se = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MOVIE = %d"), &num[0]))	{
			config.exist_mov = (num[0]) ? true : false;
		}
		else	{	return error_dont_analize();	}
	}
	// �_�C�A���O�E�B���h�E�̍��ڏ�Ԃ̐ݒ�
	else if (anl.get_format(_T("#DIALOG . ")))	{
		if (false);
		else if (anl.get_format(_T("MESSAGE_CHRCOLOR . EXIST = %d"), &num[0]))	{
			config.message_chrcolor.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("MESSAGE_CHRCOLOR . STR = %s"), &str[0]))	{
			config.message_chrcolor.str = str[0];
		}
		else if (anl.get_format(_T("OBJECT_DISP . %d . EXIST = %d"), &num[0], &num[1]))	{
			if (!analize_check_over(num[0], config.object_disp_cnt, INIMAX_OBJECT_DISP_FLAG_CNT))	{	return false;	}
			config.object_disp[num[0]].exist = (num[1]) ? true : false;
		}
		else if (anl.get_format(_T("OBJECT_DISP . %d . STR = %s"), &num[0], &str[0]))	{
			if (!analize_check_over(num[0], config.object_disp_cnt, INIMAX_OBJECT_DISP_FLAG_CNT))	{	return false;	}
			config.object_disp[num[0]].str = str[0];
		}
		else if (anl.get_format(_T("GLOBAL_EXTRA_SWITCH . %d . EXIST = %d"), &num[0], &num[1]))	{
			if (!analize_check_over(num[0], config.global_extra_switch_cnt, INIMAX_GLOBAL_EXTRA_SWITCH_CNT))	{	return false;	}
			config.global_extra_switch[num[0]].exist = (num[1]) ? true : false;
		}
		else if (anl.get_format(_T("GLOBAL_EXTRA_SWITCH . %d . STR = %s"), &num[0], &str[0]))	{
			if (!analize_check_over(num[0], config.global_extra_switch_cnt, INIMAX_GLOBAL_EXTRA_SWITCH_CNT))	{	return false;	}
			config.global_extra_switch[num[0]].str = str[0];
		}
		else if (anl.get_format(_T("GLOBAL_EXTRA_MODE . %d . "), &num[0]))	{
			if (!analize_check_over(num[0], config.global_extra_mode_cnt, INIMAX_GLOBAL_EXTRA_MODE_CNT))	{	return false;	}
			int global_extra_mode_no = num[0];
			if (false);
			else if (anl.get_format(_T("EXIST = %d"), &num[0]))	{
				config.global_extra_mode[global_extra_mode_no].exist = (num[0]) ? true : false;
			}
			else if (anl.get_format(_T("STR = %s"), &str[0]))	{
				config.global_extra_mode[global_extra_mode_no].str = str[0];
			}
			else if (anl.get_format(_T("ITEM_CNT = %d"), &num[0]))	{
				if (!analize_check_alloc(num[0], INIMIN_GLOBAL_EXTRA_MODE_ITEM_CNT, INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT))	{	return false;	}
				config.global_extra_mode[global_extra_mode_no].item_cnt = num[0];
			}
			else if (anl.get_format(_T("ITEM . %d . STR = %s"), &num[0], &str[0]))	{
				if (!analize_check_over(num[0], INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT, INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT))	{	return false;	}
				config.global_extra_mode[global_extra_mode_no].item[num[0]].str = str[0];
			}
			else	{	return error_dont_analize();	}
		}
		else if (anl.get_format(_T("SLEEP . EXIST = %d"), &num[0]))	{
			config.system.sleep.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SLEEP . STR = %s"), &str[0]))	{
			config.system.sleep.str = str[0];
		}
		else if (anl.get_format(_T("NO_WIPE_ANIME . EXIST = %d"), &num[0]))	{
			config.system.no_wipe_anime.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("NO_WIPE_ANIME . STR = %s"), &str[0]))	{
			config.system.no_wipe_anime.str = str[0];
		}
		else if (anl.get_format(_T("SKIP_WIPE_ANIME . EXIST = %d"), &num[0]))	{
			config.system.skip_wipe_anime.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SKIP_WIPE_ANIME . STR = %s"), &str[0]))	{
			config.system.skip_wipe_anime.str = str[0];
		}
		else if (anl.get_format(_T("NO_MWND_ANIME . EXIST = %d"), &num[0]))	{
			config.system.no_mwnd_anime.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("NO_MWND_ANIME . STR = %s"), &str[0]))	{
			config.system.no_mwnd_anime.str = str[0];
		}
		else if (anl.get_format(_T("WHEEL_NEXT_MESSAGE . EXIST = %d"), &num[0]))	{
			config.system.wheel_next_message.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("WHEEL_NEXT_MESSAGE . STR = %s"), &str[0]))	{
			config.system.wheel_next_message.str = str[0];
		}
		else if (anl.get_format(_T("KOE_DONT_STOP . EXIST = %d"), &num[0]))	{
			config.system.koe_dont_stop.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("KOE_DONT_STOP . STR = %s"), &str[0]))	{
			config.system.koe_dont_stop.str = str[0];
		}
		else if (anl.get_format(_T("SKIP_UNREAD_MESSAGE . EXIST = %d"), &num[0]))	{
			config.system.skip_unread_message.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("SKIP_UNREAD_MESSAGE . STR = %s"), &str[0]))	{
			config.system.skip_unread_message.str = str[0];
		}
		else if (anl.get_format(_T("PLAY_SILENT_SOUND . EXIST = %d"), &num[0]))	{
			config.system.play_silent_sound.exist = (num[0]) ? true : false;
		}
		else if (anl.get_format(_T("PLAY_SILENT_SOUND . STR = %s"), &str[0]))	{
			config.system.play_silent_sound.str = str[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �e��m�F���b�Z�[�W�̕�����
	else if (anl.get_format(_T("#WARNINGINFO . ")))	{
		if (false);
		else if (anl.get_format(_T("RESTART_WARNING_STR = %s"), &str[0]))	{
			config.warninginfo.restart_warning_str = str[0];
		}
		else if (anl.get_format(_T("SCENESTART_WARNING_STR = %s"), &str[0]))	{
			config.warninginfo.scenestart_warning_str = str[0];
		}
		else if (anl.get_format(_T("RETURNMENU_WARNING_STR = %s"), &str[0]))	{
			config.warninginfo.returnmenu_warning_str = str[0];
		}
		else if (anl.get_format(_T("RETURNSEL_WARNING_STR = %s"), &str[0]))	{
			config.warninginfo.returnsel_warning_str = str[0];
		}
		else if (anl.get_format(_T("GAMEEND_WARNING_STR = %s"), &str[0]))	{
			config.warninginfo.gameend_warning_str = str[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �Z�[�u���[�h�_�C�A���O
	else if (anl.get_format(_T("#SAVELOAD_DIALOG . ")))	{
		if (false);
		else if (anl.get_format(_T("DATA_CNT_PAR_PAGE = %d"), &num[0]))	{
			if (!analize_check_alloc(num[0], INIMIN_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE, INIMAX_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE))	{	return false;	}
			config.saveload_dialog.data_cnt_par_page = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �Z�[�u�̃C���t�H���[�V�������
	else if (anl.get_format(_T("#SAVEINFO . ")))	{
		if (false);
		else if (anl.get_format(_T("DLGWND_CAPTION_TITLE_STR = %s"), &str[0]))	{
			config.save_info.dlgwnd_caption_title_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DESIDE_BUTTON_STR = %s"), &str[0]))	{
			config.save_info.dlgwnd_deside_button_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DATALIST_NAMEHEADER_STR = %s"), &str[0]))	{
			config.save_info.dlgwnd_datalist_nameheader_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_WARNING_CHKBOX_STR = %s"), &str[0]))	{
			config.save_info.dlgwnd_warning_chkbox_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DBLCLICK_CHKBOX_STR = %s"), &str[0]))	{
			config.save_info.dlgwnd_dblclick_chkbox_str = str[0];
		}
		else if (anl.get_format(_T("WARNING_STR = %s"), &str[0]))	{
			config.save_info.warning_str = str[0];
		}
		else if (anl.get_format(_T("QUICK_WARNING_STR = %s"), &str[0]))	{
			config.save_info.quick_warning_str = str[0];
		}
		else if (anl.get_format(_T("MSGBK_WARNING_STR = %s"), &str[0]))	{
			config.save_info.msgbk_warning_str = str[0];
		}
		else	{ return error_dont_analize(); }
	}
	// ���[�h�̃C���t�H���[�V�������
	else if (anl.get_format(_T("#LOADINFO . ")))	{
		if (false);
		else if (anl.get_format(_T("DLGWND_CAPTION_TITLE_STR = %s"), &str[0]))	{
			config.load_info.dlgwnd_caption_title_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DESIDE_BUTTON_STR = %s"), &str[0]))	{
			config.load_info.dlgwnd_deside_button_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DATALIST_NAMEHEADER_STR = %s"), &str[0]))	{
			config.load_info.dlgwnd_datalist_nameheader_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_WARNING_CHKBOX_STR = %s"), &str[0]))	{
			config.load_info.dlgwnd_warning_chkbox_str = str[0];
		}
		else if (anl.get_format(_T("DLGWND_DBLCLICK_CHKBOX_STR = %s"), &str[0]))	{
			config.load_info.dlgwnd_dblclick_chkbox_str = str[0];
		}
		else if (anl.get_format(_T("WARNING_STR = %s"), &str[0]))	{
			config.load_info.warning_str = str[0];
		}
		else if (anl.get_format(_T("QUICK_WARNING_STR = %s"), &str[0]))	{
			config.load_info.quick_warning_str = str[0];
		}
		else if (anl.get_format(_T("MSGBK_WARNING_STR = %s"), &str[0]))	{
			config.load_info.msgbk_warning_str = str[0];
		}
		else	{ return error_dont_analize(); }
	}
	// �Z�[�u�f�[�^��
	else if (anl.get_format(_T("#SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SAVE_CNT, INIMAX_SAVE_CNT))	{	return false;	}
		save_cnt = num[0];
	}
	// �N�C�b�N�Z�[�u�f�[�^��
	else if (anl.get_format(_T("#QUICK_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_QUICK_SAVE_CNT, INIMAX_QUICK_SAVE_CNT))	{	return false;	}
		quick_save_cnt = num[0];
	}
	// �G���h�Z�[�u�f�[�^��
	else if (anl.get_format(_T("#END_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_END_SAVE_CNT, INIMAX_END_SAVE_CNT))	{	return false;	}
		end_save_cnt = num[0];
	}
	// �C���i�[�Z�[�u�f�[�^��
	else if (anl.get_format(_T("#INNER_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_INNER_SAVE_CNT, INIMAX_INNER_SAVE_CNT))	{	return false;	}
		inner_save_cnt = num[0];
	}
	// �o�b�N���O�Z�[�u�f�[�^��
	else if (anl.get_format(_T("#MESSAGE_BACK_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BACKLOG_SAVE_CNT, INIMAX_BACKLOG_SAVE_CNT))	{	return false;	}
		backlog_save_cnt = num[0];
	}
	else if (anl.get_format(_T("#MESSAGE_BACK_SAVE . INTERVAL = %d"), &num[0]))	{
		backlog_save_interval = num[0];
	}
	// �Z�[�u�|�C���g����
	else if (anl.get_format(_T("#SAVE_HISTORY . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SAVE_HISTORY_CNT, INIMAX_SAVE_HISTORY_CNT))	{	return false;	}
		save_history_cnt = num[0];
	}
	// �O�̑I�����ɖ߂�̗���
	else if (anl.get_format(_T("#SEL_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SEL_SAVE_CNT, INIMAX_SEL_SAVE_CNT))	{	return false;	}
		sel_save_cnt = num[0];
	}
	// ���݂��Ȃ��Z�[�u�f�[�^�̕�����
	else if (anl.get_format(_T("#SAVE . NO_DATA_STR = %s"), &str[0]))	{
		save_no_data_str = str[0];
	}
	// �Z�[�u�T���l�C��
	else if (anl.get_format(_T("#SAVE_THUMB . ")))	{

		if (false);
		else if (anl.get_format(_T("USE = %d"), &num[0]))	{
			use_save_thumb = num[0] ? true : false;
		}
		else if (anl.get_format(_T("TYPE = %d"), &num[0]))	{
			save_thumb_type = num[0];
		}
		else if (anl.get_format(_T("SIZE = %d , %d"), &num[0], &num[1]))	{
			save_thumb_size = C_size(num[0], num[1]);
		}
		else	{	return error_dont_analize();	}
	}
	// ���[�h���̃��C�v
	else if (anl.get_format(_T("#LOAD . WIPE = %d , %d"), &num[0], &num[1]))	{
		load_wipe_type = num[0];
		load_wipe_time = num[1];
	}
	// �L�����N�^�[����
	// ���F���O�ƌ��J�t���O�̃X�N���v�g����
	else if (anl.get_format(_T("#CHRKOE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_CHRKOE_CNT, INIMAX_CHRKOE_CNT))	{	return false;	}
		config.chrkoe_cnt = num[0];
	}
	else if (anl.get_format(_T("#CHRKOE . NOT_LOOK_NAME_STR = %s"), &str[0]))	{
		config.chrkoe_not_look_name_str = str[0];
	}
	else if (anl.get_format(_T("#CHRKOE . %d = %s , %d , %s , "), &num[0], &str[0], &num[1], &str[1]))	{
		if (!analize_check_over(num[0], config.chrkoe_cnt, INIMAX_CHRKOE_CNT))	{	return false;	}
		int chrkoe_no = num[0];
		config.chrkoe[chrkoe_no].name_str = str[0];
		config.chrkoe[chrkoe_no].check_mode = num[1];
		config.chrkoe[chrkoe_no].check_name_str = str[1];

		// �I�v�V�����i�Ȃ��Ă��n�j�j
		if (anl.get_format(_T("%d , %d , "), &num[2], &num[3]))	{
			config.chrkoe[chrkoe_no].onoff = num[2] ? 1 : 0;
			config.chrkoe[chrkoe_no].volume = limit(0, num[3], 255);
		}

		if (anl.get_format(_T("( ")))	{
			int kakko_cnt = 1;
			while (1)	{
				if (anl.get_format(_T("%d "), &num[0]))	{
					int list_no = (int)config.chrkoe[chrkoe_no].chr_no_list.size();
					config.chrkoe[chrkoe_no].chr_no_list.resize(list_no + 1);
					config.chrkoe[chrkoe_no].chr_no_list[list_no] = num[0];
				}
				else	{
					return error(tostr(kakko_cnt) + _T(" �ڂ̊��ʂ̃p�����[�^���s���ł��B"));
				}
				if (anl.check_moji(_T(')')))	{
					break;	// ����I��
				}
				else if (!anl.check_moji(_T(',')))	{
					return error_dont_analize();
				}
				kakko_cnt ++;
			}
		}
		else	{
			return error(_T("�p�����[�^������܂���B"));
		}
	}
	else	{
		return analize_step_2(anl, line_no);
	}

	return true;
}

// ****************************************************************
// Gameexe.ini�F��́F�X�e�b�v�Q
// ================================================================
bool C_tnm_ini::analize_step_2(C_str_analizer& anl, int line_no)
{
	int num[16] = {0};
	TSTR str[16];

	if (false);

	// �c�����^������
	else if (anl.get_format(_T("#TATEYOKO_MODE = %d"), &num[0]))	{
		tateyoko_mode = num[0];
	}
	// �y���O�z
	else if (anl.get_format(_T("#NAMAE = %s , %s , %d , %d , %d"), &str[0], &str[1], &num[0], &num[1], &num[2]))	{
		S_namae namae;
		namae.regist_name_str = str[0];
		namae.change_name_str = str[1];
		namae.color_mod = num[0];
		namae.moji_color_no = num[1];
		namae.shadow_color_no = num[2];
		namae.fuchi_color_no = -1;
		namae_list.push_back(namae);
	}
	else if (anl.get_format(_T("#NAMAE = %s , %s , %d , %d , %d, %d"), &str[0], &str[1], &num[0], &num[1], &num[2], &num[3]))	{
		S_namae namae;
		namae.regist_name_str = str[0];
		namae.change_name_str = str[1];
		namae.color_mod = num[0];
		namae.moji_color_no = num[1];
		namae.shadow_color_no = num[2];
		namae.fuchi_color_no = num[3];
		namae_list.push_back(namae);
	}
	// �t���O
	else if (anl.get_format(_T("#FLAG . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_FLAG_CNT, INIMAX_FLAG_CNT))	{	return false;	}
		flag_cnt = num[0];
	}
	else if (anl.get_format(_T("#GLOBAL_FLAG . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_GLOBAL_FLAG_CNT, INIMAX_GLOBAL_FLAG_CNT))	{	return false;	}
		global_flag_cnt = num[0];
	}
	else if (anl.get_format(_T("#CALL_FLAG . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_CALL_FLAG_CNT, INIMAX_CALL_FLAG_CNT))	{	return false;	}
		call_flag_cnt = num[0];
	}
	// �f�[�^�x�[�X
	else if (anl.get_format(_T("#DATABASE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_DATABASE_CNT, INIMAX_DATABASE_CNT))	{	return false;	}
		database_cnt = num[0];
	}
	else if (anl.get_format(_T("#DATABASE . %d = %s"), &num[0], &str[0]))	{
		if (!analize_check_over(num[0], database_cnt, INIMAX_DATABASE_CNT))	{	return false;	}
		database_name[num[0]] = str[0];
	}
	// �J�E���^
	else if (anl.get_format(_T("#COUNTER . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_COUNTER_CNT, INIMAX_COUNTER_CNT))	{	return false;	}
		counter_cnt = num[0];
	}
	// �J���[�e�[�u��
	else if (anl.get_format(_T("#COLOR_TABLE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_COLOR_TABLE_CNT, INIMAX_COLOR_TABLE_CNT))	{	return false;	}
		color_table_cnt = num[0];
	}
	else if (anl.get_format(_T("#COLOR_TABLE . %d = %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		if (!analize_check_over(num[0], color_table_cnt, INIMAX_COLOR_TABLE_CNT))	{	return false;	}
		color_table[num[0]] = C_rgb((BYTE)num[1], (BYTE)num[2], (BYTE)num[3]);
	}
	// �g�[���J�[�u
	else if (anl.get_format(_T("#TONECURVE_FILE = %s"), &str[0]))	{
		tonecurve_file = str[0];
	}
	// �b�f�e�[�u��
	else if (anl.get_format(_T("#CGTABLE_FILE = %s"), &str[0]))	{
		cg_table_file = str[0];
	}
	else if (anl.get_format(_T("#CGTABLE_FLAG_CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_CG_TABLE_FLAG_CNT, INIMAX_CG_TABLE_FLAG_CNT))	{	return false;	}
		cg_table_flag_cnt = num[0];
	}
	// �T���l�C���e�[�u��
	else if (anl.get_format(_T("#THUMBTABLE_FILE = %s"), &str[0]))	{
		thumb_table_file = str[0];
	}
	// �f�O�O�o�b�t�@
	else if (anl.get_format(_T("#G00BUF . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_G00_BUF_CNT, INIMAX_G00_BUF_CNT))	{	return false;	}
		g00_buf_cnt = num[0];
	}
	// �}�X�N
	else if (anl.get_format(_T("#MASK . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MASK_CNT, INIMAX_MASK_CNT))	{	return false;	}
		mask_cnt = num[0];
	}
	// �G����
	else if (anl.get_format(_T("#EMOJI . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EMOJI_CNT, INIMAX_EMOJI_CNT))	{	return false;	}
		emoji_cnt = num[0];
	}
	else if (anl.get_format(_T("#EMOJI . %d = %s , %d"), &num[0], &str[0], &num[1]))	{
		if (!analize_check_over(num[0], emoji_cnt, INIMAX_EMOJI_CNT))	{	return false;	}
		emoji[num[0]].file_name = str[0];
		emoji[num[0]].font_size = num[1];
	}
	// �t�H���g�t�@�C��
	else if (anl.get_format(_T("#FONT_FILE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_FONT_FILE_CNT, INIMAX_FONT_FILE_CNT))	{ return false; }
		font_file_cnt = num[0];
	}
	else if (anl.get_format(_T("#FONT_FILE . %d . NAME = %s"), &num[0], &str[0]))	{
		if (!analize_check_over(num[0], font_file_cnt, INIMAX_FONT_FILE_CNT))	{ return false; }
		font_file[num[0]].file_name = str[0];
	}
	else if (anl.get_format(_T("#PRIVATE_FONT_FILE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_FONT_FILE_CNT, INIMAX_FONT_FILE_CNT))	{ return false; }
		private_font_file_cnt = num[0];
	}
	else if (anl.get_format(_T("#PRIVATE_FONT_FILE . %d . NAME = %s"), &num[0], &str[0]))	{
		if (!analize_check_over(num[0], private_font_file_cnt, INIMAX_FONT_FILE_CNT))	{ return false; }
		private_font_file[num[0]].file_name = str[0];
	}
	// �V�X�e���R�[��
	else if (anl.get_format(_T("#EXCALL . ORDER = %d"), &num[0]))	{
		excall_order = num[0];
	}
	// �{�^���F�A�N�V����
	else if (anl.get_format(_T("#BUTTON . ACTION . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BTN_ACTION_CNT, INIMAX_BTN_ACTION_CNT))	{	return false;	}
		button.action_cnt = num[0];
	}
	else if (anl.get_format(_T("#BUTTON . ACTION . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], button.action_cnt, INIMAX_BTN_ACTION_CNT))	{	return false;	}
		int button_no = num[0];
		if (false);
		else if (anl.get_format(_T("NORMAL = %d , %d , %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]))	{
			button.action[button_no].state[TNM_BTN_STATE_NORMAL].rep_pat_no = num[0];
			button.action[button_no].state[TNM_BTN_STATE_NORMAL].rep_pos = C_point(num[1], num[2]);
			button.action[button_no].state[TNM_BTN_STATE_NORMAL].rep_tr = limit(0, num[3], 255);
			button.action[button_no].state[TNM_BTN_STATE_NORMAL].rep_bright = limit(0, num[4], 255);
			button.action[button_no].state[TNM_BTN_STATE_NORMAL].rep_dark = limit(0, num[5], 255);
		}
		else if (anl.get_format(_T("HIT = %d , %d , %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]))	{
			button.action[button_no].state[TNM_BTN_STATE_HIT].rep_pat_no = num[0];
			button.action[button_no].state[TNM_BTN_STATE_HIT].rep_pos = C_point(num[1], num[2]);
			button.action[button_no].state[TNM_BTN_STATE_HIT].rep_tr = limit(0, num[3], 255);
			button.action[button_no].state[TNM_BTN_STATE_HIT].rep_bright = limit(0, num[4], 255);
			button.action[button_no].state[TNM_BTN_STATE_HIT].rep_dark = limit(0, num[5], 255);
		}
		else if (anl.get_format(_T("PUSH = %d , %d , %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]))	{
			button.action[button_no].state[TNM_BTN_STATE_PUSH].rep_pat_no = num[0];
			button.action[button_no].state[TNM_BTN_STATE_PUSH].rep_pos = C_point(num[1], num[2]);
			button.action[button_no].state[TNM_BTN_STATE_PUSH].rep_tr = limit(0, num[3], 255);
			button.action[button_no].state[TNM_BTN_STATE_PUSH].rep_bright = limit(0, num[4], 255);
			button.action[button_no].state[TNM_BTN_STATE_PUSH].rep_dark = limit(0, num[5], 255);
		}
		else if (anl.get_format(_T("SELECT = %d , %d , %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]))	{
			button.action[button_no].state[TNM_BTN_STATE_SELECT].rep_pat_no = num[0];
			button.action[button_no].state[TNM_BTN_STATE_SELECT].rep_pos = C_point(num[1], num[2]);
			button.action[button_no].state[TNM_BTN_STATE_SELECT].rep_tr = limit(0, num[3], 255);
			button.action[button_no].state[TNM_BTN_STATE_SELECT].rep_bright = limit(0, num[4], 255);
			button.action[button_no].state[TNM_BTN_STATE_SELECT].rep_dark = limit(0, num[5], 255);
		}
		else if (anl.get_format(_T("DISABLE = %d , %d , %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]))	{
			button.action[button_no].state[TNM_BTN_STATE_DISABLE].rep_pat_no = num[0];
			button.action[button_no].state[TNM_BTN_STATE_DISABLE].rep_pos = C_point(num[1], num[2]);
			button.action[button_no].state[TNM_BTN_STATE_DISABLE].rep_tr = limit(0, num[3], 255);
			button.action[button_no].state[TNM_BTN_STATE_DISABLE].rep_bright = limit(0, num[4], 255);
			button.action[button_no].state[TNM_BTN_STATE_DISABLE].rep_dark = limit(0, num[5], 255);
		}
		else	{	return error_dont_analize();	}
	}
	// �{�^���F���ʉ�
	else if (anl.get_format(_T("#BUTTON . SE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BTN_SE_CNT, INIMAX_BTN_SE_CNT))	{	return false;	}
		button.se_cnt = num[0];
	}
	else if (anl.get_format(_T("#BUTTON . SE . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], button.se_cnt, INIMAX_BTN_SE_CNT))	{	return false;	}
		int button_no = num[0];
		if (false);
		else if (anl.get_format(_T("HIT = %d"), &num[0]))	{
			button.se[button_no].hit_no = num[0];
		}
		else if (anl.get_format(_T("PUSH = %d"), &num[0]))	{
			button.se[button_no].push_no = num[0];
		}
		else if (anl.get_format(_T("DECIDE = %d"), &num[0]))	{
			button.se[button_no].decide_no = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �{�^���F�O���[�v
	else if (anl.get_format(_T("#OBJBTNGROUP . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BTN_GROUP_CNT, INIMAX_BTN_GROUP_CNT))	{	return false;	}
		button.group_cnt = num[0];
	}
	// ���b�Z�[�W�{�^��
	else if (anl.get_format(_T("#MSGBTN . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MSGBTN_CNT, INIMAX_MSGBTN_CNT))	{ return false; }
		msgbtn_cnt = num[0];
	}
	else if (anl.get_format(_T("#MSGBTN . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], msgbtn_cnt, INIMAX_MSGBTN_CNT))	{ return false; }
		int msgbtn_no = num[0];
		if (false);
		else if (anl.get_format(_T("NORMAL . COLOR = %d"), &num[0]))	{
			msgbtn[msgbtn_no].state[TNM_BTN_STATE_NORMAL].color_no = num[0];
		}
		else if (anl.get_format(_T("HIT . COLOR = %d"), &num[0]))	{
			msgbtn[msgbtn_no].state[TNM_BTN_STATE_HIT].color_no = num[0];
		}
		else if (anl.get_format(_T("PUSH . COLOR = %d"), &num[0]))	{
			msgbtn[msgbtn_no].state[TNM_BTN_STATE_PUSH].color_no = num[0];
		}
		else	{ return error_dont_analize(); }
	}
	// ���[���h
	else if (anl.get_format(_T("#WORLD . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_WORLD_CNT, INIMAX_WORLD_CNT))	{	return false;	}
		world_cnt = num[0];
	}
	else if (anl.get_format(_T("#WORLD . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], world_cnt, INIMAX_WORLD_CNT))	{	return false;	}
		int world_no = num[0];
		if (false);
		else if (anl.get_format(_T("LAYER = %d"), &num[0]))	{
			world[world_no].layer = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �I�u�W�F�N�g
	else if (anl.get_format(_T("#OBJECT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_OBJECT_CNT, INIMAX_OBJECT_CNT))	{	return false;	}
		object_cnt = num[0];
	}
	else if (anl.get_format(_T("#OBJECT . %d . "), &num[0]))	{

		if (!analize_check_over(num[0], object_cnt, INIMAX_OBJECT_CNT))	{	return false;	}
		int object_no = num[0];
		if (false);
		else if (anl.get_format(_T("USE = %d"), &num[0]))	{
			object[object_no].use = num[0] ? true : false;
		}
		else if (anl.get_format(_T("SAVE = %d"), &num[0]))	{
			object[object_no].save = num[0] ? true : false;
		}
		else if (anl.get_format(_T("SPACE_HIDE = %d"), &num[0]))	{
			object[object_no].space_hide = num[0] ? true : false;
		}
		else if (anl.get_format(_T("OBJECT_DISP_NO = %d"), &num[0]))	{
			object[object_no].object_disp_no = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	else if (anl.get_format(_T("#OBJECT . %d - %d . "), &num[0], &num[1]))	{

		if (!analize_check_over(num[0], object_cnt, INIMAX_OBJECT_CNT))	{	return false;	}
		if (!analize_check_over(num[1], object_cnt, INIMAX_OBJECT_CNT))	{	return false;	}
		int top = num[0], tail = num[1];
		if (false);
		else if (anl.get_format(_T("USE = %d"), &num[0]))	{
			for (int object_no = top; object_no <= tail; object_no++)	{
				object[object_no].use = num[0] ? true : false;
			}
		}
		else if (anl.get_format(_T("SAVE = %d"), &num[0]))	{
			for (int object_no = top; object_no <= tail; object_no++)	{
				object[object_no].save = num[0] ? true : false;
			}
		}
		else if (anl.get_format(_T("SPACE_HIDE = %d"), &num[0]))	{
			for (int object_no = top; object_no <= tail; object_no++)	{
				object[object_no].space_hide = num[0] ? true : false;
			}
		}
		else if (anl.get_format(_T("OBJECT_DISP_NO = %d"), &num[0]))	{
			for (int object_no = top; object_no <= tail; object_no++)	{
				object[object_no].object_disp_no = num[0];
			}
		}
		else	{	return error_dont_analize();	}
	}
	// ���b�Z�[�W�E�B���h�E�g
	else if (anl.get_format(_T("#WAKU . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MWND_WAKU_CNT, INIMAX_MWND_WAKU_CNT))	{	return false;	}
		mwnd_waku_cnt = num[0];
	}
	else if (anl.get_format(_T("#WAKU . BTN . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MWND_WAKU_BTN_CNT, INIMAX_MWND_WAKU_BTN_CNT))	{	return false;	}
		mwnd_waku_btn_cnt = num[0];
	}
	else if (anl.get_format(_T("#WAKU . FACE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MWND_WAKU_FACE_CNT, INIMAX_MWND_WAKU_FACE_CNT))	{	return false;	}
		mwnd_waku_face_cnt = num[0];
	}
	else if (anl.get_format(_T("#WAKU . OBJECT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MWND_WAKU_OBJECT_CNT, INIMAX_MWND_WAKU_OBJECT_CNT))	{	return false;	}
		mwnd_waku_object_cnt = num[0];
	}
	else if (anl.get_format(_T("#WAKU . %d . "), &num[0]))	{
		if (!analize_step_waku(anl, line_no, num[0]))
			return false;
	}
	// ���b�Z�[�W�E�B���h�E
	else if (anl.get_format(_T("#MWND . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MWND_SUB_CNT, INIMAX_MWND_SUB_CNT))	{	return false;	}
		mwnd.sub_cnt = num[0];
	}
	else if (anl.get_format(_T("#MWND . DEFAULT_MWND_NO = %d"), &num[0]))	{
		mwnd.default_mwnd_no = num[0];
	}
	else if (anl.get_format(_T("#MWND . DEFAULT_SEL_MWND_NO = %d"), &num[0]))	{
		mwnd.default_sel_mwnd_no = num[0];
	}
	else if (anl.get_format(_T("#MWND . ORDER = %d"), &num[0]))	{
		mwnd.order = num[0];
	}
	else if (anl.get_format(_T("#MWND . FILTER_LAYER_REP = %d"), &num[0]))	{
		mwnd.filter_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . WAKU_LAYER_REP = %d"), &num[0]))	{
		mwnd.waku_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . SHADOW_LAYER_REP = %d"), &num[0]))	{
		mwnd.shadow_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . FUCHI_LAYER_REP = %d"), &num[0]))	{
		mwnd.fuchi_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . MOJI_LAYER_REP = %d"), &num[0]))	{
		mwnd.moji_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . FACE_LAYER_REP = %d"), &num[0]))	{
		mwnd.face_layer_rep = num[0];
	}
	else if (anl.get_format(_T("#MWND . SHADOW_COLOR = %d"), &num[0]))	{
		mwnd.shadow_color = num[0];
	}
	else if (anl.get_format(_T("#MWND . FUCHI_COLOR = %d"), &num[0]))	{
		mwnd.fuchi_color = num[0];
	}
	else if (anl.get_format(_T("#MWND . MOJI_COLOR = %d"), &num[0]))	{
		mwnd.moji_color = num[0];
	}
	else if (anl.get_format(_T("#MWND . %d . "), &num[0]))	{
		if (!analize_step_mwnd(anl, line_no, num[0]))
			return false;
	}
	// �{�^���I����
	else if (anl.get_format(_T("#SELBTN . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SEL_BTN_CNT, INIMAX_SEL_BTN_CNT))	{	return false;	}
		sel_btn_cnt = num[0];
	}
	else if (anl.get_format(_T("#SELBTN . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], sel_btn_cnt, INIMAX_SEL_BTN_CNT))	{	return false;	}
		int sb_no = num[0];
		if (false);
		else if (anl.get_format(_T("BASE_FILE = %s"), &str[0]))	{
			sel_btn[sb_no].base_file = str[0];
		}
		else if (anl.get_format(_T("BACK_FILE = %s"), &str[0]))	{
			sel_btn[sb_no].filter_file = str[0];
		}
		else if (anl.get_format(_T("BASE_POS = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].base_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("REP_POS = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].rep_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("ALIGN = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].x_align = num[0];
			sel_btn[sb_no].y_align = num[1];
		}
		else if (anl.get_format(_T("MAX_Y_CNT = %d"), &num[0]))	{
			sel_btn[sb_no].max_y_cnt = num[0];
		}
		else if (anl.get_format(_T("LINE_WIDTH = %d"), &num[0]))	{
			sel_btn[sb_no].line_width = num[0];
		}
		else if (anl.get_format(_T("MOJI_SIZE = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
			sel_btn[sb_no].moji_size = num[0];
			sel_btn[sb_no].moji_space = C_size(num[1], num[2]);
			sel_btn[sb_no].moji_cnt = num[3];
		}
		else if (anl.get_format(_T("MOJI_POS = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].moji_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("MOJI_ALIGN = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].moji_x_align = num[0];
			sel_btn[sb_no].moji_y_align = num[1];
		}
		else if (anl.get_format(_T("MOJI_COLOR = %d"), &num[0]))	{
			sel_btn[sb_no].moji_color = num[0];
		}
		else if (anl.get_format(_T("MOJI_HIT_COLOR = %d"), &num[0]))	{
			sel_btn[sb_no].moji_hit_color = num[0];
		}
		else if (anl.get_format(_T("BTN_ACTION = %d"), &num[0]))	{
			sel_btn[sb_no].btn_action_no = num[0];
		}
		else if (anl.get_format(_T("OPEN_ANIME = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].open_anime_type = num[0];
			sel_btn[sb_no].open_anime_time = num[1];
		}
		else if (anl.get_format(_T("CLOSE_ANIME = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].close_anime_type = num[0];
			sel_btn[sb_no].close_anime_time = num[1];
		}
		else if (anl.get_format(_T("DECIDE_ANIME = %d , %d"), &num[0], &num[1]))	{
			sel_btn[sb_no].decide_anime_type = num[0];
			sel_btn[sb_no].decide_anime_time = num[1];
		}
		else	{	return error_dont_analize();	}
	}
	// ���b�Z�[�W�o�b�N
	else if (anl.get_format(_T("#MSGBK . ")))	{

		if (false);
		else if (anl.get_format(_T("ORDER = %d"), &num[0]))	{
			msg_back_order = num[0];
		}
		else if (anl.get_format(_T("HISTORY_CNT = %d"), &num[0]))	{
			if (!analize_check_alloc(num[0], INIMIN_MSG_BACK_HISTORY_CNT, INIMAX_MSG_BACK_HISTORY_CNT))	{ return false; }
			msg_back.history_cnt = num[0];
		}
		else if (anl.get_format(_T("WINDOW_POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.window_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("WINDOW_SIZE = %d , %d"), &num[0], &num[1]))	{
			msg_back.window_size = C_size(num[0], num[1]);
		}
		else if (anl.get_format(_T("DISP_MARGIN = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
			msg_back.disp_margin = C_rect(num[0], num[1], num[2], num[3]);
		}
		else if (anl.get_format(_T("MESSAGE_POS = %d"), &num[0]))	{
			msg_back.msg_pos = num[0];
		}
		else if (anl.get_format(_T("MOJI_CNT = %d , %d"), &num[0], &num[1]))	{
			msg_back.moji_cnt = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("MOJI_SIZE = %d"), &num[0]))	{
			msg_back.moji_size = num[0];
		}
		else if (anl.get_format(_T("MOJI_SPACE = %d , %d"), &num[0], &num[1]))	{
			msg_back.moji_space = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("MOJI_COLOR = %d"), &num[0]))	{
			msg_back.moji_color = num[0];
		}
		else if (anl.get_format(_T("MOJI_SHADOW_COLOR = %d"), &num[0]))	{
			msg_back.moji_shadow_color = num[0];
		}
		else if (anl.get_format(_T("MOJI_FUCHI_COLOR = %d"), &num[0]))	{
			msg_back.moji_fuchi_color = num[0];
		}
		else if (anl.get_format(_T("ACTIVE_MOJI_COLOR = %d"), &num[0]))	{
			msg_back.active_moji_color = num[0];
		}
		else if (anl.get_format(_T("ACTIVE_MOJI_SHADOW_COLOR = %d"), &num[0]))	{
			msg_back.active_moji_shadow_color = num[0];
		}
		else if (anl.get_format(_T("ACTIVE_MOJI_FUCHI_COLOR = %d"), &num[0]))	{
			msg_back.active_moji_fuchi_color = num[0];
		}
		else if (anl.get_format(_T("DEBUG_MOJI_COLOR = %d"), &num[0]))	{
			msg_back.debug_moji_color = num[0];
		}
		else if (anl.get_format(_T("DEBUG_MOJI_SHADOW_COLOR = %d"), &num[0]))	{
			msg_back.debug_moji_shadow_color = num[0];
		}
		else if (anl.get_format(_T("DEBUG_MOJI_FUCHI_COLOR = %d"), &num[0]))	{
			msg_back.debug_moji_fuchi_color = num[0];
		}
		else if (anl.get_format(_T("NAME_DISP_MODE = %d"), &num[0]))	{
			msg_back.name_disp_mode = num[0];
		}
		else if (anl.get_format(_T("NAME_BRACKET = %d"), &num[0]))	{
			msg_back.name_bracket_type = num[0];
		}
		else if (anl.get_format(_T("BACK_FILE = %s"), &str[0]))	{
			msg_back.waku_file = str[0];
		}
		else if (anl.get_format(_T("FILTER_FILE = %s"), &str[0]))	{
			msg_back.filter_file = str[0];
		}
		else if (anl.get_format(_T("FILTER_MARGIN = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
			msg_back.filter_margin = C_rect(num[0], num[1], num[2], num[3]);
		}
		else if (anl.get_format(_T("FILTER_COLOR = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
			msg_back.filter_color = C_argb(num[3], num[0], num[1], num[2]);
		}
		else if (anl.get_format(_T("SEPARATOR_FILE = %s"), &str[0]))	{
			msg_back.separator_file = str[0];
		}
		else if (anl.get_format(_T("SEPARATOR_TOP_FILE = %s"), &str[0]))	{
			msg_back.separator_top_file = str[0];
		}
		else if (anl.get_format(_T("SEPARATOR_BOTTOM_FILE = %s"), &str[0]))	{
			msg_back.separator_bottom_file = str[0];
		}
		else if (anl.get_format(_T("MSG_CLICK_ACTION = %d"), &num[0]))	{
			msg_back.msg_click_action = num[0];
		}
		else if (anl.get_format(_T("LOAD_CALL = %s, %s"), &str[0], &str[1]))	{
			msg_back.load_call_scene_name = str[0];
			msg_back.load_call_command_name = str[1];
		}
		else	{ return error_dont_analize(); }
	}
	else if (anl.get_format(_T("#MSGBK_ITEM . ")))	{

		if (false);
		else if (anl.get_format(_T("SLIDER . FILE = %s"), &str[0]))	{
			msg_back.slider_file = str[0];
		}
		else if (anl.get_format(_T("SLIDER . POS = %d , %d , %d"), &num[0], &num[1], &num[2]))	{
			msg_back.slider_rect = C_rect(num[0], num[1], num[0], num[2]);
		}
		else if (anl.get_format(_T("SLIDER . ACTION = %d"), &num[0]))	{
			msg_back.slider_action_no = num[0];
		}
		else if (anl.get_format(_T("SLIDER . SE = %d"), &num[0]))	{
			msg_back.slider_se_no = num[0];
		}
		else if (anl.get_format(_T("CLOSE_BTN . FILE = %s"), &str[0]))	{
			msg_back.close_btn_file = str[0];
		}
		else if (anl.get_format(_T("CLOSE_BTN . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.close_btn_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("CLOSE_BTN . ACTION = %d"), &num[0]))	{
			msg_back.close_btn_action_no = num[0];
		}
		else if (anl.get_format(_T("CLOSE_BTN . SE = %d"), &num[0]))	{
			msg_back.close_btn_se_no = num[0];
		}
		else if (anl.get_format(_T("MSG_UP_BTN . FILE = %s"), &str[0]))	{
			msg_back.msg_up_btn_file = str[0];
		}
		else if (anl.get_format(_T("MSG_UP_BTN . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.msg_up_btn_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("MSG_UP_BTN . ACTION = %d"), &num[0]))	{
			msg_back.msg_up_btn_action_no = num[0];
		}
		else if (anl.get_format(_T("MSG_UP_BTN . SE = %d"), &num[0]))	{
			msg_back.msg_up_btn_se_no = num[0];
		}
		else if (anl.get_format(_T("MSG_DOWN_BTN . FILE = %s"), &str[0]))	{
			msg_back.msg_down_btn_file = str[0];
		}
		else if (anl.get_format(_T("MSG_DOWN_BTN . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.msg_down_btn_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("MSG_DOWN_BTN . ACTION = %d"), &num[0]))	{
			msg_back.msg_down_btn_action_no = num[0];
		}
		else if (anl.get_format(_T("MSG_DOWN_BTN . SE = %d"), &num[0]))	{
			msg_back.msg_down_btn_se_no = num[0];
		}
		else if (anl.get_format(_T("KOE_BTN . FILE = %s"), &str[0]))	{
			msg_back.koe_btn_file = str[0];
		}
		else if (anl.get_format(_T("KOE_BTN . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.koe_btn_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("KOE_BTN . ACTION = %d"), &num[0]))	{
			msg_back.koe_btn_action_no = num[0];
		}
		else if (anl.get_format(_T("KOE_BTN . SE = %d"), &num[0]))	{
			msg_back.koe_btn_se_no = num[0];
		}
		else if (anl.get_format(_T("LOAD_BTN . FILE = %s"), &str[0]))	{
			msg_back.load_btn_file = str[0];
		}
		else if (anl.get_format(_T("LOAD_BTN . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.load_btn_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("LOAD_BTN . ACTION = %d"), &num[0]))	{
			msg_back.load_btn_action_no = num[0];
		}
		else if (anl.get_format(_T("LOAD_BTN . SE = %d"), &num[0]))	{
			msg_back.load_btn_se_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_1 . FILE = %s"), &str[0]))	{
			msg_back.ex_btn_1_file = str[0];
		}
		else if (anl.get_format(_T("EX_BTN_1 . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.ex_btn_1_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("EX_BTN_1 . ACTION = %d"), &num[0]))	{
			msg_back.ex_btn_1_action_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_1 . SE = %d"), &num[0]))	{
			msg_back.ex_btn_1_se_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_1 . CALL = %s, %s"), &str[0], &str[1]))	{
			msg_back.ex_btn_1_scene_name = str[0];
			msg_back.ex_btn_1_command_name = str[1];
		}
		else if (anl.get_format(_T("EX_BTN_2 . FILE = %s"), &str[0]))	{
			msg_back.ex_btn_2_file = str[0];
		}
		else if (anl.get_format(_T("EX_BTN_2 . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.ex_btn_2_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("EX_BTN_2 . ACTION = %d"), &num[0]))	{
			msg_back.ex_btn_2_action_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_2 . SE = %d"), &num[0]))	{
			msg_back.ex_btn_2_se_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_2 . CALL = %s, %s"), &str[0], &str[1]))	{
			msg_back.ex_btn_2_scene_name = str[0];
			msg_back.ex_btn_2_command_name = str[1];
		}
		else if (anl.get_format(_T("EX_BTN_3 . FILE = %s"), &str[0]))	{
			msg_back.ex_btn_3_file = str[0];
		}
		else if (anl.get_format(_T("EX_BTN_3 . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.ex_btn_3_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("EX_BTN_3 . ACTION = %d"), &num[0]))	{
			msg_back.ex_btn_3_action_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_3 . SE = %d"), &num[0]))	{
			msg_back.ex_btn_3_se_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_3 . CALL = %s, %s"), &str[0], &str[1]))	{
			msg_back.ex_btn_3_scene_name = str[0];
			msg_back.ex_btn_3_command_name = str[1];
		}
		else if (anl.get_format(_T("EX_BTN_4 . FILE = %s"), &str[0]))	{
			msg_back.ex_btn_4_file = str[0];
		}
		else if (anl.get_format(_T("EX_BTN_4 . POS = %d , %d"), &num[0], &num[1]))	{
			msg_back.ex_btn_4_pos = C_point(num[0], num[1]);
		}
		else if (anl.get_format(_T("EX_BTN_4 . ACTION = %d"), &num[0]))	{
			msg_back.ex_btn_4_action_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_4 . SE = %d"), &num[0]))	{
			msg_back.ex_btn_4_se_no = num[0];
		}
		else if (anl.get_format(_T("EX_BTN_4 . CALL = %s, %s"), &str[0], &str[1]))	{
			msg_back.ex_btn_4_scene_name = str[0];
			msg_back.ex_btn_4_command_name = str[1];
		}
		else	{ return error_dont_analize(); }
	}
	// �a�f�l
	else if (anl.get_format(_T("#BGM . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BGM_CNT, INIMAX_BGM_CNT))	{	return false;	}
		bgm_cnt = num[0];
	}
	else if (anl.get_format(_T("#BGM . %d = "), &num[0]))	{
		if (!analize_check_over(num[0], bgm_cnt, INIMAX_BGM_CNT))	{	return false;	}
		int bgm_no = num[0];
		if (false);
		else if (anl.get_format(_T("%s , %s , %d , %d , %d"), &str[0], &str[1], &num[0], &num[1], &num[2]))	{
			bgm[bgm_no].regist_name = str[0];
			bgm[bgm_no].file_name = str[1];
			bgm[bgm_no].start_pos = num[0];
			bgm[bgm_no].end_pos = num[1];
			bgm[bgm_no].repeat_pos = num[2];
			if (bgm[bgm_no].end_pos == 0)	{
				return error(_T("�I���ʒu�� 0 �ɂȂ��Ă��܂��B�t�@�C���̍Ō�܂ōĐ�����ꍇ�� -1 ���w�肵�ĉ������B"));
			}
		}
		else	{	return error_dont_analize();	}
	}
	// ���ʉ��`�����l����
	else if (anl.get_format(_T("#PCMCH . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_PCMCH_CNT, INIMAX_PCMCH_CNT))	{	return false;	}
		pcmch_cnt = num[0];
	}
	// ���ʉ��C�x���g��
	else if (anl.get_format(_T("#PCMEVENT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_PCM_EVENT_CNT, INIMAX_PCM_EVENT_CNT))	{	return false;	}
		pcm_event_cnt = num[0];
	}
	// �V�X�e����
	else if (anl.get_format(_T("#SE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SE_CNT, INIMAX_SE_CNT))	{	return false;	}
		se_cnt = num[0];
	}
	else if (anl.get_format(_T("#SE . %d = "), &num[0]))	{
		if (!analize_check_over(num[0], se_cnt, INIMAX_SE_CNT))	{	return false;	}
		int se_no = num[0];
		if (false);
		else if (anl.get_format(_T("%s"), &str[0]))	{
			se[se_no].file_name = str[0];
		}
		else	{	return error_dont_analize();	}
	}
	// �G�t�F�N�g
	else if (anl.get_format(_T("#EFFECT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EFFECT_CNT, INIMAX_EFFECT_CNT))	{	return false;	}
		effect_cnt = num[0];
	}
	// �V�F�C�N
	else if (anl.get_format(_T("#SHAKE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SHAKE_CNT, INIMAX_SHAKE_CNT))	{	return false;	}
		shake_cnt = num[0];
	}
	else if (anl.get_format(_T("#SHAKE . %d = "), &num[0]))	{
		if (!analize_check_over(num[0], shake_cnt, INIMAX_SHAKE_CNT))	{	return false;	}
		int shake_no = num[0];
		if (anl.get_format(_T("( ")))	{
			int kakko_cnt = 1;
			while (1)	{
				if (anl.get_format(_T("%d , %d , %d )"), &num[0], &num[1], &num[2]))	{
					int sub_no = (int)shake[shake_no].sub.size();
					shake[shake_no].sub.resize(sub_no + 1);
					shake[shake_no].sub[sub_no].x = num[0];
					shake[shake_no].sub[sub_no].y = num[1];
					shake[shake_no].sub[sub_no].time = num[2];
				}
				else	{
					return error(tostr(kakko_cnt) + _T(" �ڂ̊��ʂ̃p�����[�^���s���ł��B"));
				}
				if (!anl.check_moji(_T('(')))	{
					break;	// ����I��
				}
				kakko_cnt ++;
			}
		}
		else	{
			return error(_T("�p�����[�^������܂���B"));
		}
	}
	// �N�G�C�N
	else if (anl.get_format(_T("#QUAKE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_QUAKE_CNT, INIMAX_QUAKE_CNT))	{	return false;	}
		quake_cnt = num[0];
	}
	// �t���[���A�N�V�����`�����l��
	else if (anl.get_format(_T("#FRAME_ACTION_CH . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_FRAME_ACTION_CH_CNT, INIMAX_FRAME_ACTION_CH_CNT))	{	return false;	}
		frame_action_ch_cnt = num[0];
	}
	// �a�f�l�t�F�[�h�Q
	else if (anl.get_format(_T("#BGMFADE2 . ")))	{
		if (anl.get_format(_T("IN_START_TIME = %d"), &num[0]))	{
			bgmfade2_in_start_time = num[0];
		}
		else if (anl.get_format(_T("IN_TIME_LEN = %d"), &num[0]))	{
			bgmfade2_in_time_len = num[0];
		}
		else if (anl.get_format(_T("OUT_START_TIME = %d"), &num[0]))	{
			bgmfade2_out_start_time = num[0];
		}
		else if (anl.get_format(_T("OUT_TIME_LEN = %d"), &num[0]))	{
			bgmfade2_out_time_len = num[0];
		}
		else if (anl.get_format(_T("VOLUME = %d"), &num[0]))	{
			bgmfade2_volume = num[0];
		}
		else	{ return error_dont_analize(); }
	}
	// �G�f�B�b�g�{�b�N�X
	else if (anl.get_format(_T("#EDITBOX . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EDITBOX_CNT, INIMAX_EDITBOX_CNT))	{	return false;	}
		editbox_cnt = num[0];
	}
	// �c�C�b�^�[
	else if (anl.get_format(_T("#TWITTER . ")))	{
		if (anl.get_format(_T("API_KEY = %s"), &str[0]))	{
			twitter_api_key = str[0];
		}
		else if (anl.get_format(_T("API_SECRET = %s"), &str[0]))	{
			twitter_api_secret = str[0];
		}
		else if (anl.get_format(_T("CALLBACK_URL = %s"), &str[0]))	{
			twitter_callback_url = str[0];
		}
		else if (anl.get_format(_T("INITIAL_TWEET_TEXT = %s"), &str[0]))	{
			twitter_initial_tweet_text = str[0];
		}
		else if (anl.get_format(_T("OVERLAP_IMAGE = %s"), &str[0]))	{
			twitter_overlap_image = str[0];
		}
		else	{ return error_dont_analize(); }
	}
	// ��̓G���[
	else	{
		return error_dont_analize();
	}

	return true;
}

// ****************************************************************
// Gameexe.ini�F��̓X�e�b�v�F�g
// ================================================================
bool C_tnm_ini::analize_step_waku(C_str_analizer& anl, int line_no, int waku_no)
{
	if (!analize_check_over(waku_no, mwnd_waku_cnt, INIMAX_MWND_WAKU_CNT))	{	return false;	}

	int num[16] = {0};
	TSTR str[16];

	if (false);
	else if (anl.get_format(_T("EXTEND_TYPE = %d"), &num[0]))	{
		mwnd_waku[waku_no].extend_type = num[0];
	}
	else if (anl.get_format(_T("WAKU_FILE = %s"), &str[0]))	{
		mwnd_waku[waku_no].waku_file = str[0];
	}
	else if (anl.get_format(_T("FILTER_FILE = %s"), &str[0]))	{
		mwnd_waku[waku_no].filter_file = str[0];
	}
	else if (anl.get_format(_T("FILTER_MARGIN = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		mwnd_waku[waku_no].filter_margin = C_rect(num[0], num[1], num[2], num[3]);
	}
	else if (anl.get_format(_T("FILTER_COLOR = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		mwnd_waku[waku_no].filter_color = C_argb(num[3], num[0], num[1], num[2]);
	}
	else if (anl.get_format(_T("FILTER_CONFIG_COLOR = %d"), &num[0]))	{
		mwnd_waku[waku_no].filter_config_color = num[0] ? true : false;
	}
	else if (anl.get_format(_T("FILTER_CONFIG_TR = %d"), &num[0]))	{
		mwnd_waku[waku_no].filter_config_tr = num[0] ? true : false;
	}
	else if (anl.get_format(_T("ICON_NO = %d"), &num[0]))	{
		mwnd_waku[waku_no].icon_no = num[0];
	}
	else if (anl.get_format(_T("PAGE_ICON_NO = %d"), &num[0]))	{
		mwnd_waku[waku_no].page_icon_no = num[0];
	}
	else if (anl.get_format(_T("ICON_POS_TYPE = %d"), &num[0]))	{
		mwnd_waku[waku_no].icon_pos_type = num[0];
	}
	else if (anl.get_format(_T("ICON_POS = %d , %d , %d"), &num[0], &num[1], &num[2]))	{
		mwnd_waku[waku_no].icon_pos_base = num[0];
		mwnd_waku[waku_no].icon_pos = C_point(num[1], num[2]);
	}
	// ���b�Z�[�W�E�B���h�E�g�F�{�^��
	else if (anl.get_format(_T("BTN . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], mwnd_waku_btn_cnt, INIMAX_MWND_WAKU_BTN_CNT))	{	return false;	}
		int btn_no = num[0];
		if (false);
		else if (anl.get_format(_T("FILE = %s"), &str[0]))	{
			mwnd_waku[waku_no].btn[btn_no].btn_file = str[0];
		}
		else if (anl.get_format(_T("CUT_NO = %d"), &num[0]))	{
			mwnd_waku[waku_no].btn[btn_no].cut_no = num[0];
		}
		else if (anl.get_format(_T("POS = %d , %d , %d"), &num[0], &num[1], &num[2]))	{
			mwnd_waku[waku_no].btn[btn_no].pos_base = num[0];
			mwnd_waku[waku_no].btn[btn_no].pos.x = num[1];
			mwnd_waku[waku_no].btn[btn_no].pos.y = num[2];
		}
		else if (anl.get_format(_T("ACTION = %d"), &num[0]))	{
			mwnd_waku[waku_no].btn[btn_no].action_no = num[0];
		}
		else if (anl.get_format(_T("SE = %d"), &num[0]))	{
			mwnd_waku[waku_no].btn[btn_no].se_no = num[0];
		}
		else if (anl.get_format(_T("TYPE = %s , %d , %d"), &str[0], &num[0], &num[1]))	{

			if (false);
			else if (str[0] == _T("local_switch"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[1];
			}
			else if (str[0] == _T("global_switch"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[1];
			}
			else if (str[0] == _T("local_mode"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[1];
			}
			else if (str[0] == _T("global_mode"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[1];
			}
			else	{
				return error(_T("�����ȃV�X�e���^�C�v�ł��B") + str[0]);
			}
		}
		else if (anl.get_format(_T("TYPE = %s , %d"), &str[0], &num[0]))	{

			if (false);
			else if (str[0] == _T("qsave"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_QUICK_SAVE;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
			}
			else if (str[0] == _T("qload"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_QUICK_LOAD;
				mwnd_waku[waku_no].btn[btn_no].sys_type_opt = num[0];
			}
			else if (str[0] == _T("read_skip"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_READ_SKIP;
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[0];
			}
			else if (str[0] == _T("auto_mode"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_AUTO_MODE;
				mwnd_waku[waku_no].btn[btn_no].btn_mode = num[0];
			}
			else	{
				return error(_T("�����ȃV�X�e���^�C�v�ł��B") + str[0]);
			}
		}
		else if (anl.get_format(_T("TYPE = %s"), &str[0]))	{

			if (false);
			else if (str[0] == _T("none"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_NONE;
			}
			else if (str[0] == _T("save"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_SAVE;
			}
			else if (str[0] == _T("load"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_LOAD;
			}
			else if (str[0] == _T("return_sel"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_RETURN_SEL;
			}
			else if (str[0] == _T("close_mwnd"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_HIDE_MWND;
			}
			else if (str[0] == _T("msg_log"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_MSG_BACK;
			}
			else if (str[0] == _T("koe_play"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_KOE_PLAY;
			}
			else if (str[0] == _T("config"))	{
				mwnd_waku[waku_no].btn[btn_no].sys_type = TNM_SYSCOM_TYPE_CONFIG;
			}
			else	{
				return error(_T("�����ȃV�X�e���^�C�v�ł��B") + str[0]);
			}
		}
		else if (anl.get_format(_T("CALL = %s , %s"), &str[0], &str[1]))	{
			mwnd_waku[waku_no].btn[btn_no].scn_name = str[0];
			mwnd_waku[waku_no].btn[btn_no].cmd_name = str[1];
		}
		else if (anl.get_format(_T("CALL = %s , %d"), &str[0], &num[0]))	{
			mwnd_waku[waku_no].btn[btn_no].scn_name = str[0];
			mwnd_waku[waku_no].btn[btn_no].z_no = num[0];
		}
		else if (anl.get_format(_T("FRAME_ACTION = %s , %s"), &str[0], &str[1]))	{
			mwnd_waku[waku_no].btn[btn_no].frame_action_scn_name = str[0];
			mwnd_waku[waku_no].btn[btn_no].frame_action_cmd_name = str[1];
		}
		else	{	return error_dont_analize();	}
	}
	// ���b�Z�[�W�E�B���h�E�g�F��O��
	else if (anl.get_format(_T("FACE . %d . "), &num[0]))	{
		if (!analize_check_over(num[0], mwnd_waku_face_cnt, INIMAX_MWND_WAKU_FACE_CNT))	{	return false;	}
		int face_no = num[0];
		if (false);
		else if (anl.get_format(_T("POS = %d , %d"), &num[0], &num[1]))	{
			mwnd_waku[waku_no].face_pos[face_no] = C_point(num[0], num[1]);
		}
		else	{	return error_dont_analize();	}
	}
	else	{	return error_dont_analize();	}

	return true;
}

// ****************************************************************
// Gameexe.ini�F��̓X�e�b�v�F�l�v
// ================================================================
bool C_tnm_ini::analize_step_mwnd(C_str_analizer& anl, int line_no, int mw_no)
{
	if (!analize_check_over(mw_no, mwnd.sub_cnt, INIMAX_MWND_SUB_CNT))	{	return false;	}

	int num[16] = {0};
	TSTR str[16];

	if (false);
	else if (anl.get_format(_T("NOVEL_MODE = %d"), &num[0]))	{
		mwnd.sub[mw_no].novel_mode = num[0];
	}
	else if (anl.get_format(_T("EXTEND_TYPE = %d"), &num[0]))	{
		mwnd.sub[mw_no].extend_type = num[0];
	}
	else if (anl.get_format(_T("WINDOW_POS = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].window_pos = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("WINDOW_SIZE = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].window_size = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("MESSAGE_POS = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].msg_pos = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("MESSAGE_MARGIN = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		mwnd.sub[mw_no].msg_margin = C_rect(num[0], num[1], num[2], num[3]);
	}
	else if (anl.get_format(_T("MOJI_CNT = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].moji_cnt = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("MOJI_SIZE = %d"), &num[0]))	{
		mwnd.sub[mw_no].moji_size = num[0];
	}
	else if (anl.get_format(_T("MOJI_SPACE = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].moji_space = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("MOJI_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].moji_color = num[0];
	}
	else if (anl.get_format(_T("SHADOW_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].shadow_color = num[0];
	}
	else if (anl.get_format(_T("FUCHI_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].fuchi_color = num[0];
	}
	else if (anl.get_format(_T("RUBY_SIZE = %d"), &num[0]))	{
		mwnd.sub[mw_no].ruby_size = num[0];
	}
	else if (anl.get_format(_T("RUBY_SPACE = %d"), &num[0]))	{
		mwnd.sub[mw_no].ruby_space = num[0];
	}
	else if (anl.get_format(_T("WAKU_NO = %d"), &num[0]))	{
		mwnd.sub[mw_no].waku_no = num[0];
	}
	else if (anl.get_format(_T("WAKU_POS = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].waku_pos = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_DISP_MODE = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_disp_mode = num[0];
	}
	else if (anl.get_format(_T("NAME_NEWLINE = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_newline = num[0];
	}
	else if (anl.get_format(_T("NAME_BRACKET = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_bracket = num[0];
	}
	else if (anl.get_format(_T("NAME_MOJI_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_moji_color = num[0];
	}
	else if (anl.get_format(_T("NAME_SHADOW_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_shadow_color = num[0];
	}
	else if (anl.get_format(_T("NAME_FUCHI_COLOR = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_fuchi_color = num[0];
	}
	else if (anl.get_format(_T("TALK_MARGIN = %d, %d, %d, %d"), &num[0], &num[1], &num[2], &num[3]))	{
		mwnd.sub[mw_no].talk_margin = C_rect(num[0], num[1], num[2], num[3]);
	}
	else if (anl.get_format(_T("OVERFLOW_CHECK_SIZE = %d"), &num[0]))	{
		mwnd.sub[mw_no].over_flow_check_size = num[0];
	}
	else if (anl.get_format(_T("MSG_BACK_INSERT_NL = %d"), &num[0]))	{
		mwnd.sub[mw_no].msg_back_insert_nl = num[0];
	}
	else if (anl.get_format(_T("NAME_EXTEND_TYPE = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_extend_type = num[0];
	}
	else if (anl.get_format(_T("NAME_WINDOW_ALIGN = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_window_align = num[0];
	}
	else if (anl.get_format(_T("NAME_WINDOW_POS = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].name_window_pos = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_WINDOW_SIZE = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].name_window_size = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_MESSAGE_POS = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].name_msg_pos = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_MESSAGE_POS_REP = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].name_msg_pos_rep = C_point(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_MESSAGE_MARGIN = %d , %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		mwnd.sub[mw_no].name_msg_margin = C_rect(num[0], num[1], num[2], num[3]);
	}
	else if (anl.get_format(_T("NAME_MOJI_SIZE = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_moji_size = num[0];
	}
	else if (anl.get_format(_T("NAME_MOJI_SPACE = %d , %d"), &num[0], &num[1]))	{
		mwnd.sub[mw_no].name_moji_space = C_size(num[0], num[1]);
	}
	else if (anl.get_format(_T("NAME_MOJI_CNT = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_moji_cnt = num[0];
	}
	else if (anl.get_format(_T("NAME_WAKU_NO = %d"), &num[0]))	{
		mwnd.sub[mw_no].name_waku_no = num[0];
	}
	else if (anl.get_format(_T("FACE_HIDE_NAME = %d"), &num[0]))	{
		mwnd.sub[mw_no].face_hide_name = num[0];
	}
	else if (anl.get_format(_T("OPEN_ANIME_TYPE = %d"), &num[0]))	{
		mwnd.sub[mw_no].open_anime_type = num[0];
	}
	else if (anl.get_format(_T("OPEN_ANIME_TIME = %d"), &num[0]))	{
		mwnd.sub[mw_no].open_anime_time = num[0];
	}
	else if (anl.get_format(_T("CLOSE_ANIME_TYPE = %d"), &num[0]))	{
		mwnd.sub[mw_no].close_anime_type = num[0];
	}
	else if (anl.get_format(_T("CLOSE_ANIME_TIME = %d"), &num[0]))	{
		mwnd.sub[mw_no].close_anime_time = num[0];
	}
	else	{	return error_dont_analize();	}

	return true;
}

// ****************************************************************
// Gameexe.ini�F�l�`�F�b�N
// ================================================================
bool C_tnm_ini::analize_check_value(int val, int min, int max)
{
	if (!(min <= val && val <= max))	{
		error(_T("�l���ُ�ł��B�i ") + tostr(min) + _T(" �` ") + tostr(max) + _T(" �j"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini�F�m�ې��`�F�b�N
// ================================================================
bool C_tnm_ini::analize_check_alloc(int val, int min, int max)
{
	if (!(min <= val && val <= max))	{
		error(_T("�l���ُ�ł��B�i ") + tostr(min) + _T(" �` ") + tostr(max) + _T(" �j"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini�F�͈͊O�A�N�Z�X�`�F�b�N
// ================================================================
bool C_tnm_ini::analize_check_over(int val, int alloc, int max)
{
	if (!(0 <= val && val < alloc))	{
		error(_T("�͈͊O�A�N�Z�X�ł��B�i 0 �` ") + tostr(alloc) + _T(" [") + tostr(max) + _T("] �j"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini�F�G���[�F��͂ł��܂���ł���
// ================================================================
bool C_tnm_ini::error_dont_analize()
{
	return error(_T("��͂ł��܂���ł����B"));
}

// ****************************************************************
// Gameexe.ini�F�G���[
// ================================================================
bool C_tnm_ini::error(CTSTR& str)
{
	set_last_error(str_format(_T("Gameexe.dat �G���[ %d �s��\n"), line_no + 1) + line_data + _T("\n") + str, _T(""));
	return false;
}

#if 0

// ****************************************************************
// Flag.ini�F���
// ================================================================
bool Cflag_ini::analize()
{
	ARRAY<TSTR> ini_file_data;
	C_file::read_full_text(_T("Flag.ini"), ini_file_data);

	for (int line = 0; line < (int)ini_file_data.size(); line ++)	{
		analize_line(ini_file_data[line]);
	}

	return true;
};

bool Cflag_ini::analize_line(CTSTR& line_data)
{
	/*
	TCHAR moji;

	Cstr_analizer anl(line_data.begin(), line_data.end());
	while (!anl.is_end())	{

		TCHAR property;
		if (anl.get_moji(&property))	{
			if (anl.get_moji(&moji) && moji == _T('['))	{
				int index;
				if (anl.get_num(&index))	{
					if (anl.get_moji(&moji) && moji == _T(']'))	{

						// �v���p�e�B�`�F�b�N
						if (property == _T('A') || property == _T('B') || property == _T('C') || property == _T('D') || property == _T('E') || property == _T('F') || property == _T('G') || property == _T('Z') || property == _T('L') || property == _T('H') || property == _T('I') || property == _T('J') || property == _T('S') || property == _T('M') || property == _T('K'))	{

							// �C���f�b�N�X�`�F�b�N
//							if (0 <= index && index < TNM_VARIABLE_CNT)	{

								VAR var;
								switch (property)	{
									case _T('A'):	var.property = PR_A;	var.form = FM_INTEGER;	break;
									case _T('B'):	var.property = PR_B;	var.form = FM_INTEGER;	break;
									case _T('C'):	var.property = PR_C;	var.form = FM_INTEGER;	break;
									case _T('D'):	var.property = PR_D;	var.form = FM_INTEGER;	break;
									case _T('E'):	var.property = PR_E;	var.form = FM_INTEGER;	break;
									case _T('F'):	var.property = PR_F;	var.form = FM_INTEGER;	break;
									case _T('G'):	var.property = PR_G;	var.form = FM_INTEGER;	break;
									case _T('Z'):	var.property = PR_Z;	var.form = FM_INTEGER;	break;
									case _T('L'):	var.property = PR_L;	var.form = FM_INTEGER;	break;
									case _T('H'):	var.property = PR_H;	var.form = FM_INTEGER;	break;
									case _T('I'):	var.property = PR_I;	var.form = FM_INTEGER;	break;
									case _T('J'):	var.property = PR_J;	var.form = FM_INTEGER;	break;
									case _T('S'):	var.property = PR_S;	var.form = FM_STRING;	break;
									case _T('M'):	var.property = PR_M;	var.form = FM_STRING;	break;
									case _T('K'):	var.property = PR_K;	var.form = FM_STRING;	break;
								}
								var.index = index;

								// �R�����g�`�F�b�N
								if (anl.get_moji(&moji) && moji == _T(':'))	{
									var.comment = TSTR(anl.get_cur(), anl.get_end());
								}

								// �ϐ��m��
								var_list.push_back(var);

								return true;
//							}
						}
					}
				}
			}
		}
	}
	*/

	return false;
}

#endif
