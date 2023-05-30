#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// Select.ini
// ================================================================
class C_tnm_select_ini
{
public:

	class C_item
	{
	public:
		TSTR	m_dir;		// �f�B���N�g����
		TSTR	m_name;		// ���[�h��ʂȂǂŕ\������镶����
	};

	C_tnm_select_ini();

	// ���
	bool	analize();

	// �擪�̃f�B���N�g��
	TSTR	first_dir();
	TSTR	first_name();

	// �A�y���h���X�g
	ARRAY<C_item>	m_append_list;
	// ���݂̃A�y���h
	TSTR			m_cur_append_dir;
	TSTR			m_cur_append_name;
};

// ****************************************************************
// Gameexe.ini
// ================================================================
class C_tnm_ini
{
public:

	C_tnm_ini();

	// ���
	bool	analize();

	// �萔
	enum
	{
		// ���ϐ�
		INIMIN_SYSTEM_VALUE_CNT = 0,
		INIMAX_SYSTEM_VALUE_CNT = 256,
		INIDEF_SYSTEM_VALUE_CNT = 64,

		// �A�C�R��
		INIMIN_ICON_CNT = 0,
		INIMAX_ICON_CNT = 256,
		INIDEF_ICON_CNT = 16,

		// �}�E�X�J�[�\��
		INIMIN_MOUSE_CURSOR_CNT = 0,
		INIMAX_MOUSE_CURSOR_CNT = 256,
		INIDEF_MOUSE_CURSOR_CNT = 16,

		// �V���[�g�J�b�g�L�[
		INIMIN_SHORTCUT_CNT = 0,
		INIMAX_SHORTCUT_CNT = 256,
		INIDEF_SHORTCUT_CNT = 0,

		// �t���b�N�V�[��
		INIMIN_FLICK_SCENE_CNT = 0,
		INIMAX_FLICK_SCENE_CNT = 256,
		INIDEF_FLICK_SCENE_CNT = 0,

		// �V�X�e���R�}���h���j���[

		// �V�X�e���R�}���h���j���[�F���[�J���ėp�X�C�b�`
		INIMIN_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,
		INIMAX_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,
		INIDEF_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,

		// �V�X�e���R�}���h���j���[�F���[�J���ėp���[�h
		INIMIN_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,
		INIMAX_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,
		INIDEF_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,

		// �V�X�e���R�}���h���j���[�F���[�J���ėp���[�h�̃A�C�e����
		INIMIN_LOCAL_EXTRA_MODE_ITEM_CNT = 0,		// ������� 0 �łn�j�ł��B
		INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT = TNM_LOCAL_EXTRA_MODE_ITEM_CNT,
		INIDEF_LOCAL_EXTRA_MODE_ITEM_CNT = 3,

		// ���[�U�[�R���t�B�O

		// ���[�U�[�R���t�B�O�F�L�����N�^�[����
		INIMIN_CHRKOE_CNT = 0,
		INIMAX_CHRKOE_CNT = 256,
		INIDEF_CHRKOE_CNT = 64,

		// ���[�U�[�R���t�B�O�F�I�u�W�F�N�g�̕\���t���O
		INIMIN_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,
		INIMAX_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,
		INIDEF_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,

		// ���[�U�[�R���t�B�O�F�O���[�o���ėp�X�C�b�`
		INIMIN_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,
		INIMAX_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,
		INIDEF_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,

		// ���[�U�[�R���t�B�O�F�O���[�o���ėp���[�h
		INIMIN_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,
		INIMAX_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,
		INIDEF_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,

		// ���[�U�[�R���t�B�O�F�O���[�o���ėp���[�h�̃A�C�e����
		INIMIN_GLOBAL_EXTRA_MODE_ITEM_CNT = 0,		// ������� 0 �łn�j�ł��B
		INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT = TNM_GLOBAL_EXTRA_MODE_ITEM_CNT,
		INIDEF_GLOBAL_EXTRA_MODE_ITEM_CNT = TNM_GLOBAL_EXTRA_MODE_ITEM_CNT,

		// ���[�U�R���t�B�O�F�Z�[�u���[�h�_�C�A���O�F�P�y�[�W�i�P�^�u�j������̃f�[�^��
		INIMIN_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 1,
		INIMAX_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 1000,
		INIDEF_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 10,

		// �Z�[�u�f�[�^��
		INIMIN_SAVE_CNT = 0,
		INIMAX_SAVE_CNT = 10000,
		INIDEF_SAVE_CNT = 10,

		// �N�C�b�N�Z�[�u�f�[�^��
		INIMIN_QUICK_SAVE_CNT = 0,
		INIMAX_QUICK_SAVE_CNT = 10000,
		INIDEF_QUICK_SAVE_CNT = 3,

		// �G���h�Z�[�u�f�[�^��
		INIMIN_END_SAVE_CNT = 0,
		INIMAX_END_SAVE_CNT = 1,
		INIDEF_END_SAVE_CNT = 0,

		// �C���i�[�Z�[�u�f�[�^��
		INIMIN_INNER_SAVE_CNT = 0,
		INIMAX_INNER_SAVE_CNT = 100,
		INIDEF_INNER_SAVE_CNT = 0,

		// �Z�[�u�|�C���g����
		INIMIN_SAVE_HISTORY_CNT = 0,
		INIMAX_SAVE_HISTORY_CNT = 10000,
		INIDEF_SAVE_HISTORY_CNT = 100,

		// �o�b�N���O�Z�[�u��
		INIMIN_BACKLOG_SAVE_CNT = 0,
		INIMAX_BACKLOG_SAVE_CNT = 10000,
		INIDEF_BACKLOG_SAVE_CNT = 0,

		// �O�̑I�����ɖ߂�̗���
		INIMIN_SEL_SAVE_CNT = 1,
		INIMAX_SEL_SAVE_CNT = 100,
		INIDEF_SEL_SAVE_CNT = 1,

		// �t���O
		INIMIN_FLAG_CNT = 0,
		INIMAX_FLAG_CNT = 10000,
		INIDEF_FLAG_CNT = 1000,

		// �O���[�o���t���O
		INIMIN_GLOBAL_FLAG_CNT = 0,
		INIMAX_GLOBAL_FLAG_CNT = 10000,
		INIDEF_GLOBAL_FLAG_CNT = 1000,

		// �R�[���t���O
		INIMIN_CALL_FLAG_CNT = 0,
		INIMAX_CALL_FLAG_CNT = 256,
		INIDEF_CALL_FLAG_CNT = 50,

		// �f�[�^�x�[�X
		INIMIN_DATABASE_CNT = 0,
		INIMAX_DATABASE_CNT = 256,
		INIDEF_DATABASE_CNT = 0,

		// �J�E���^
		INIMIN_COUNTER_CNT = 0,
		INIMAX_COUNTER_CNT = 256,
		INIDEF_COUNTER_CNT = 16,

		// �J���[�e�[�u��
		INIMIN_COLOR_TABLE_CNT = 256,
		INIMAX_COLOR_TABLE_CNT = 256,
		INIDEF_COLOR_TABLE_CNT = 256,

		// �b�f�e�[�u��
		INIMIN_CG_TABLE_FLAG_CNT = 0,
		INIMAX_CG_TABLE_FLAG_CNT = 10000,
		INIDEF_CG_TABLE_FLAG_CNT = 1000,

		// �f�O�O�o�b�t�@
		INIMIN_G00_BUF_CNT = 0,
		INIMAX_G00_BUF_CNT = 256,
		INIDEF_G00_BUF_CNT = 16,

		// �}�X�N
		INIMIN_MASK_CNT = 0,
		INIMAX_MASK_CNT = 256,
		INIDEF_MASK_CNT = 16,

		// �G����
		INIMIN_EMOJI_CNT = 0,
		INIMAX_EMOJI_CNT = 256,
		INIDEF_EMOJI_CNT = 16,

		// �ǉ��t�H���g
		INIMIN_FONT_FILE_CNT = 0,
		INIMAX_FONT_FILE_CNT = 256,
		INIDEF_FONT_FILE_CNT = 0,

		// �V�X�e���R�[��
		INIDEF_MSG_BACK_ORDER = 10000,
		INIDEF_EXCALL_ORDER = 20000,

		// ���[���h
		INIMIN_WORLD_CNT = 0,
		INIMAX_WORLD_CNT = 256,
		INIDEF_WORLD_CNT = 1,

		// �{�^���F�A�N�V����
		INIMIN_BTN_ACTION_CNT = 0,
		INIMAX_BTN_ACTION_CNT = 256,
		INIDEF_BTN_ACTION_CNT = 16,

		// �{�^���F���ʉ�
		INIMIN_BTN_SE_CNT = 0,
		INIMAX_BTN_SE_CNT = 256,
		INIDEF_BTN_SE_CNT = 16,

		// �{�^���F�O���[�v
		INIMIN_BTN_GROUP_CNT = 0,
		INIMAX_BTN_GROUP_CNT = 256,
		INIDEF_BTN_GROUP_CNT = 4,

		// ���b�Z�[�W�{�^��
		INIMIN_MSGBTN_CNT = 0,
		INIMAX_MSGBTN_CNT = 256,
		INIDEF_MSGBTN_CNT = 16,

		// �I�u�W�F�N�g
		INIMIN_OBJECT_CNT = 0,
		INIMAX_OBJECT_CNT = 1024,
		INIDEF_OBJECT_CNT = 256,

		// ���b�Z�[�W�E�B���h�E
		INIMIN_MWND_SUB_CNT = 0,
		INIMAX_MWND_SUB_CNT = 256,
		INIDEF_MWND_SUB_CNT = 2,

		// ���b�Z�[�W�E�B���h�E�g
		INIMIN_MWND_WAKU_CNT = 0,
		INIMAX_MWND_WAKU_CNT = 256,
		INIDEF_MWND_WAKU_CNT = 4,

		// ���b�Z�[�W�E�B���h�E�g�F�{�^��
		INIMIN_MWND_WAKU_BTN_CNT = 0,
		INIMAX_MWND_WAKU_BTN_CNT = 256,
		INIDEF_MWND_WAKU_BTN_CNT = 8,

		// ���b�Z�[�W�E�B���h�E�g�F��O��
		INIMIN_MWND_WAKU_FACE_CNT = 0,
		INIMAX_MWND_WAKU_FACE_CNT = 16,
		INIDEF_MWND_WAKU_FACE_CNT = 1,

		// ���b�Z�[�W�E�B���h�E�g�F�I�u�W�F�N�g
		INIMIN_MWND_WAKU_OBJECT_CNT = 0,
		INIMAX_MWND_WAKU_OBJECT_CNT = 16,
		INIDEF_MWND_WAKU_OBJECT_CNT = 1,

		// �{�^���I����
		INIMIN_SEL_BTN_CNT = 0,
		INIMAX_SEL_BTN_CNT = 256,
		INIDEF_SEL_BTN_CNT = 16,

		// ���b�Z�[�W�o�b�N
		INIMIN_MSG_BACK_HISTORY_CNT = 0,
		INIMAX_MSG_BACK_HISTORY_CNT = 1024,
		INIDEF_MSG_BACK_HISTORY_CNT = 256,

		// �G�t�F�N�g
		INIMIN_EFFECT_CNT = 0,
		INIMAX_EFFECT_CNT = 256,
		INIDEF_EFFECT_CNT = 4,

		// �V�F�C�N
		INIMIN_SHAKE_CNT = 0,
		INIMAX_SHAKE_CNT = 256,
		INIDEF_SHAKE_CNT = 16,

		// �N�G�C�N
		INIMIN_QUAKE_CNT = 0,
		INIMAX_QUAKE_CNT = 256,
		INIDEF_QUAKE_CNT = 16,

		// �t���[���A�N�V�����`�����l��
		INIMIN_FRAME_ACTION_CH_CNT = 0,
		INIMAX_FRAME_ACTION_CH_CNT = 256,
		INIDEF_FRAME_ACTION_CH_CNT = 4,

		// �a�f�l
		INIMIN_BGM_CNT = 0,
		INIMAX_BGM_CNT = 256,
		INIDEF_BGM_CNT = 32,

		// ���ʉ��`�����l����
		INIMIN_PCMCH_CNT = 0,
		INIMAX_PCMCH_CNT = 256,
		INIDEF_PCMCH_CNT = 16,

		// ���ʉ��C�x���g��
		INIMIN_PCM_EVENT_CNT = 0,
		INIMAX_PCM_EVENT_CNT = 256,
		INIDEF_PCM_EVENT_CNT = 16,

		// �V�X�e����
		INIMIN_SE_CNT = 8,
		INIMAX_SE_CNT = 256,
		INIDEF_SE_CNT = 16,

		// �a�f�l�t�F�[�h�Q
		INIDEF_BGMFADE2_IN_START_TIME = 0,
		INIDEF_BGMFADE2_IN_TIME_LEN = 500,
		INIDEF_BGMFADE2_OUT_START_TIME = 0,
		INIDEF_BGMFADE2_OUT_TIME_LEN = 500,
		INIDEF_BGMFADE2_VOLUME = 0,

		// �G�f�B�b�g�{�b�N�X
		INIMIN_EDITBOX_CNT = 0,
		INIMAX_EDITBOX_CNT = 256,
		INIDEF_EDITBOX_CNT = 4,
	};

	// �f�o�b�O
	int		debug_error_patno_out_of_range;

	// �V�X�e��
	TSTR	game_id;			// �Q�[��ID
	TSTR	game_name;			// �Q�[����
	TSTR	game_version_str;	// �Q�[���o�[�W����������
	TSTR	disc_mark;			// �f�B�X�N�F���t�@�C����
	TSTR	manual_path_str;	// �}�j���A���p�X
	C_size	screen_size;		// �X�N���[���T�C�Y
	TSTR	start_scene_name;	// �X�^�[�g�V�[����
	int		start_scene_z_no;	// �X�^�[�g�V�[���y���x��
	TSTR	menu_scene_name;	// ���j���[�V�[����
	int		menu_scene_z_no;	// ���j���[�V�[���y���x��
	TSTR	cancel_scene_name;	// �L�����Z���V�[����
	int		cancel_scene_z_no;	// �L�����Z���V�[���y���x��
	TSTR	config_scene_name;	// ���ݒ�V�[����
	int		config_scene_z_no;	// ���ݒ�V�[���y���x��
	TSTR	save_scene_name;	// �Z�[�u�V�[����
	int		save_scene_z_no;	// �Z�[�u�V�[���y���x��
	TSTR	load_scene_name;	// ���[�h�V�[����
	int		load_scene_z_no;	// ���[�h�V�[���y���x��
	TSTR	load_after_call_scene;	// ���[�h����R�[���V�[����
	int		load_after_call_z_no;	// ���[�h����R�[���y���x��
	TSTR	dummy_check_str;	// �_�~�[�`�F�b�N�̌x����
	TSTR	dummy_check_ok_str;	// �_�~�[�`�F�b�N�̐������̕�

	// �t���b�N�V�[��
	int		flick_scene_cnt;
	TSTR	flick_scene_name[INIMAX_FLICK_SCENE_CNT];
	int		flick_scene_z_no[INIMAX_FLICK_SCENE_CNT];
	int		flick_scene_angle[INIMAX_FLICK_SCENE_CNT];

	// ���ϐ�
	int		system_int_value_cnt;
	int		system_int_value[INIMAX_SYSTEM_VALUE_CNT];
	int		system_str_value_cnt;
	TSTR	system_str_value[INIMAX_SYSTEM_VALUE_CNT];

	// �V���[�g�J�b�g�L�[
	int		shortcut_cnt;
	int		shortcut_key[INIMAX_SHORTCUT_CNT];
	int		shortcut_cond[INIMAX_SHORTCUT_CNT];
	TSTR	shortcut_scn[INIMAX_SHORTCUT_CNT];
	int		shortcut_z_no[INIMAX_SHORTCUT_CNT];
	TSTR	shortcut_cmd[INIMAX_SHORTCUT_CNT];

	// �A�C�R��
	struct Sicon
	{
		TSTR	file_name;
		int		anime_pat_cnt;
		int		anime_speed;
	};
	Sicon			icon[INIMAX_ICON_CNT];
	int				icon_cnt;

	// �}�E�X�J�[�\��
	struct Smouse_cursor
	{
		TSTR	file_name;
		int		anime_speed;
	};
	Smouse_cursor	mouse_cursor[INIMAX_MOUSE_CURSOR_CNT];
	int				mouse_cursor_cnt;
	int				mouse_cursor_default;

	// �V�X�e���R�}���h���j���[
	struct Ssyscommenu
	{
		// ���݁E���^�֎~�E������
		struct Sexistenablestr
		{
			bool	exist;		// ���݂̗L��
			bool	enable;		// ���^�֎~
			TSTR	str;		// ������
		};

		// ���݁E���^�֎~�EON/OFF��ԁE������
		struct Sexistenableonoffstr
		{
			bool	exist;		// ���݂̗L��
			bool	enable;		// ���^�֎~
			bool	onoff;		// ON/OFF���
			TSTR	str;		// ������
		};

		Sexistenablestr		read_skip;		// ��x�ǂ񂾕��͂̑�������J�n���܂��B
		Sexistenablestr		unread_skip;	// ���ǂ̕��͂ł���������J�n���܂��B�i���V�X�e���ݒ�́u���ǂ̕��͂�������ł���悤�ɂ���B�v���L���̏ꍇ�ł��j
		Sexistenablestr		auto_skip;		// ����������@�\���n�m�^�n�e�e���܂��B
		Sexistenablestr		auto_mode;		// �I�[�g���[�h���J�n���܂��B
		Sexistenablestr		hide_mwnd;		// ���b�Z�[�W�E�B���h�E���B���܂��B
		Sexistenablestr		msg_back;		// ���b�Z�[�W�o�b�N���J���܂��B
		Sexistenablestr		save;			// �Z�[�u���Ăяo���܂��B�i#SAVE_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A�Z�[�u�_�C�A���O��\�����܂��j
		Sexistenablestr		load;			// ���[�h���Ăяo���܂��B�i#LOAD_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���[�h�_�C�A���O��\�����܂��j
		Sexistenablestr		return_to_sel;	// �O�̑I�����ɖ߂�܂��B
		Sexistenablestr		config;			// ���ݒ���Ăяo���܂��B�i#CONFIG_SCENE ���ݒ肳��Ă���ꍇ�́A���̃V�[�����Ăяo���܂��B�ݒ肳��Ă��Ȃ��ꍇ�́A���ݒ�_�C�A���O��\�����܂��j
		Sexistenablestr		manual;			// �}�j���A����\�����܂��B�i�}�j���A���ւ̃p�X�� #MANUAL_PATH �Őݒ肵�ĉ������j
		Sexistenablestr		version;		// �o�[�W��������\�����܂��B�i���� #GAMEVERSION �Őݒ肵�ĉ������j
		Sexistenablestr		return_to_menu;	// �^�C�g�����j���[�ɖ߂�܂��B
		Sexistenablestr		game_end;		// �Q�[�����I�����܂��B
		Sexistenablestr		cancel;			// �V�X�e���R�}���h���j���[���L�����Z�����܂��B

		// ���[�J���ėp�X�C�b�`
		int						local_extra_switch_cnt;								// ���[�J���ėp�X�C�b�`�̐�
		Sexistenableonoffstr	local_extra_switch[INIMAX_LOCAL_EXTRA_SWITCH_CNT];	// ���[�J���ėp�X�C�b�`

		// ���[�J���ėp���[�h
		struct Slocal_extra_mode
		{
			bool		exist;			// ���݂̗L��
			bool		enable;			// ���^�֎~
			int			mode;			// ���[�h�l
			TSTR		str;			// ���[�h��

			// �A�C�e��
			struct Sitem
			{
				TSTR		str;		// ���ڂ̕�����
			};
			int			item_cnt;		// �A�C�e����
			Sitem		item[INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT];
		};
		int						local_extra_mode_cnt;								// ���[�J���ėp���[�h�̐�
		Slocal_extra_mode		local_extra_mode[INIMAX_LOCAL_EXTRA_MODE_CNT];		// ���[�J���ėp���[�h

	};
	Ssyscommenu	syscommenu;

	// ���[�U�[�R���t�B�O
	struct S_config
	{
		// ���݁EON/OFF��ԁE������
		struct S_existonoffstr
		{
			bool	exist;		// ���݂̗L��
			bool	onoff;		// ON/OFF���
			TSTR	str;		// ������
		};

		// ���݁E������
		struct S_existstr
		{
			bool	exist;		// ���݂̗L��
			TSTR	str;		// ������
		};

		// �_�C�A���O�X�^�C��
		int		dlg_style_volume;		// ���ʃ_�C�A���O�̃X�^�C���i0=���ʁ��a�f�l�t�F�[�h  1=���ʂ̂݁j
		int		dlg_style_koe;			// �����_�C�A���O�̃X�^�C���i0=�������[�h���L�����N�^�[����  1=�������[�h�̂�  2=�L�����N�^�[�����̂݁j

		// �^�u�̑��݂̗L��
		bool	tab_exist_screen;		// �u��ʁv�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_volume;		// �u���ʁv�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_message;		// �u���́v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_mwndbk;		// �u�w�i�F�v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_koe;			// �u�����v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_automode;		// �u�I�[�g���[�h�v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_jitan;		// �u���Z�Đ��v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_else;			// �u���̑��v�^�u�̑��݂̗L���i0=���� 1=�L��j
		bool	tab_exist_system;		// �u�V�X�e���v�^�u�̑��݂̗L���i0=���� 1=�L��j

		// ���ڂ̑��݂̗L��
		bool	exist_bgm;				// �u�a�f�l�v���ڂ̑��݂̗L��
		bool	exist_koe;				// �u�����v���ڂ̑��݂̗L��
		bool	exist_pcm;				// �u���ʉ��v���ڂ̑��݂̗L��
		bool	exist_se;				// �u�V�X�e�����v���ڂ̑��݂̗L��
		bool	exist_mov;				// �u���[�r�[�v���ڂ̑��݂̗L��

		// �X�N���[���T�C�Y
		int		screen_size_mode;		// �X�N���[���T�C�Y�̃��[�h
		C_size	screen_size_scale;		// �X�N���[���T�C�Y�̃X�P�[��

		// �t���X�N���[���ڍ�
		int		fullscreen_mode;				// �t���X�N���[���̕\�����[�h
		C_size	fullscreen_scale;				// �t���X�N���[���̃X�P�[��
		bool	fullscreen_scale_sync_switch;	// �t���X�N���[���̃X�P�[���̓����X�C�b�`
		C_size	fullscreen_move;				// �t���X�N���[���̈ړ�

		// ����
		int		all_user_volume;		// �S�̃{�����[��
		int		bgm_user_volume;		// BGM �{�����[��
		int		koe_user_volume;		// KOE �{�����[��
		int		pcm_user_volume;		// PCM �{�����[��
		int		se_user_volume;			// SE �{�����[��
		int		mov_user_volume;		// MOV �{�����[��
		bool	play_all_check;			// �S�̍Đ��`�F�b�N
		bool	play_bgm_check;			// BGM �Đ��`�F�b�N
		bool	play_koe_check;			// KOE �Đ��`�F�b�N
		bool	play_pcm_check;			// PCM �Đ��`�F�b�N
		bool	play_se_check;			// SE �Đ��`�F�b�N
		bool	play_mov_check;			// MOV �Đ��`�F�b�N

		// �a�f�l�t�F�[�h
		int		bgmfade_volume;			// �a�f�l�t�F�[�h�{�����[��
		bool	bgmfade_use_check;		// �a�f�l�t�F�[�h�`�F�b�N

		// �t�B���^�[�F
		C_argb	filter_color;			// �t�B���^�[�F

		// �t�H���g
		TSTR	font_name;				// �t�H���g�F���O
		int		font_type;				// �t�H���g�F��ށi0=�l�r�S�V�b�N 1=�l�r���� 2=���C���I 3=�Ǝ��t�H���g�j
		bool	font_futoku;			// �t�H���g�F��������i0=OFF 1=ON�j
		int		font_shadow;			// �t�H���g�F�e���[�h�i0=�e�Ȃ� 1=�e���� 2=�����j
		TSTR	font_sample_str_short;	// �t�H���g�F�T���v��������V���[�g
		TSTR	font_sample_str_long;	// �t�H���g�F�����O������V���[�g

		// �������x
		int		message_speed;			// �������x
		bool	message_speed_nowait;	// �������x�i�m�[�E�F�C�g�j

		// �I�[�g���[�h
		bool	auto_mode_onoff;		// �I�[�g���[�h
		int		auto_mode_moji_wait;	// �I�[�g���[�h��������
		int		auto_mode_min_wait;		// �I�[�g���[�h�ŏ�����

		// �}�E�X�J�[�\��������
		bool	mouse_cursor_hide_onoff;	// �����Ń}�E�X�J�[�\��������
		int		mouse_cursor_hide_time;		// �����Ń}�E�X�J�[�\��������

		// ���Z�Đ�
		bool	jitan_normal_onoff;		// ���Z�Đ��`�F�b�N�i���͂𕁒ʂɓǂݐi�߂Ă��鎞�j
		bool	jitan_auto_mode_onoff;	// ���Z�Đ��`�F�b�N�i�I�[�g���[�h���j
		bool	jitan_msgbk_onoff;		// ���Z�Đ��`�F�b�N�i���͂̊����߂����j
		int		jitan_speed;			// ���Z�Đ����x

		// �������[�h
		int		koe_mode;				// �������[�h

		// �L�����N�^�[����
		struct Schrkoe
		{
			TSTR		name_str;			// ���O
			int			check_mode;			// �`�F�b�N���[�h�i0=������ԂŃ`�F�b�N���Ȃ�  1=���J��ԂŃ`�F�b�N���Ȃ�  2=������ԂŃ`�F�b�N����j
			TSTR		check_name_str;		// �`�F�b�N�p�̖��O
			bool		onoff;				// ON/OFF���
			int			volume;				// ����
			ARRAY<int>	chr_no_list;		// �L�����N�^�[�ԍ����X�g�i�����̃����o�[�̓G���W�����ɂ͖����j
		};
		Schrkoe	chrkoe[INIMAX_CHRKOE_CNT];
		int		chrkoe_cnt;
		TSTR	chrkoe_not_look_name_str;

		// ���͂̐F����
		S_existonoffstr		message_chrcolor;	// ���͂̐F����

		// �I�u�W�F�N�g�̕\���t���O�̐�
		int					object_disp_cnt;		// �I�u�W�F�N�g�̕\���t���O�̐�
		S_existonoffstr		object_disp[INIMAX_OBJECT_DISP_FLAG_CNT];				// �I�u�W�F�N�g�̕\���t���O

		// �O���[�o���ėp�X�C�b�`�̐�
		int					global_extra_switch_cnt;								// �O���[�o���ėp�X�C�b�`�̐�
		S_existonoffstr		global_extra_switch[INIMAX_GLOBAL_EXTRA_SWITCH_CNT];	// �O���[�o���ėp�X�C�b�`

		// �O���[�o���ėp���[�h
		struct S_global_extra_mode
		{
			bool		exist;			// ���݂̗L��
			TSTR		str;			// ���[�h��
			int			mode;			// ���[�h�l

			// �A�C�e��
			struct S_item
			{
				TSTR		str;		// ���ڂ̕�����
			};
			int			item_cnt;		// �A�C�e����
			S_item		item[INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT];
		};
		int						global_extra_mode_cnt;								// �O���[�o���ėp���[�h�̐�
		S_global_extra_mode		global_extra_mode[INIMAX_GLOBAL_EXTRA_MODE_CNT];	// �O���[�o���ėp���[�h

		// �V�X�e���ݒ�
		struct Ssystem
		{
			S_existonoffstr		sleep;						// �{�v���O�����̓����x�����āA���̃v���O�������X���[�Y�ɓ��삷��悤�ɂ���B
			S_existonoffstr		no_wipe_anime;				// ��ʈÓ]���ʂ̃A�j���𖳌��ɂ���B
			S_existonoffstr		skip_wipe_anime;			// ��ʈÓ]���ʂ��}�E�X�N���b�N�Ŕ�΂��B
			S_existonoffstr		no_mwnd_anime;				// ���b�Z�[�W�E�B���h�E�̊J���̃A�j���𖳌��ɂ���B
			S_existonoffstr		wheel_next_message;			// �}�E�X�̃z�C�[���{�^���̉��񂵂ŕ��͂�ǂݐi�߂�B
			S_existonoffstr		koe_dont_stop;				// ���̍Đ����Ɏ��̕��͂ɐi��ł��Đ��𑱂���B
			S_existonoffstr		skip_unread_message;		// ���ǂ̕��͂�������ł���悤�ɂ���B
			S_existstr			play_silent_sound;			// �T�E���h�Đ����ɎG��������ꍇ�̓`�F�b�N���ĉ������B

			int					movie_play_type;			// ���[�r�[�̍Đ����@

			bool				saveload_alert_onoff;		// �Z�[�u���[�h�̌x��
			bool				saveload_dblclick_onoff;	// �Z�[�u���[�h�̃_�u���N���b�N
		}	system;

		struct S_saveload_dialog
		{
			int					data_cnt_par_page;			// �P�y�[�W�i�P�^�u�j������̃f�[�^��
		};

		// �Z�[�u���[�h�_�C�A���O
		S_saveload_dialog		saveload_dialog;			// �Z�[�u���[�h�_�C�A���O

		// �Z�[�u�ƃ��[�h�̃C���t�H���[�V�������
		struct S_saveloadinfo
		{
			TSTR	dlgwnd_caption_title_str;				// �Z�[�u�_�C�A���O�̃L���v�V�����^�C�g���̕�����
			TSTR	dlgwnd_deside_button_str;				// �Z�[�u�_�C�A���O�̌���{�^���̕�����
			TSTR	dlgwnd_datalist_nameheader_str;			// �Z�[�u�_�C�A���O�̃f�[�^���X�g�̐擪������
			TSTR	dlgwnd_warning_chkbox_str;				// �Z�[�u�_�C�A���O�̊m�F�`�F�b�N�{�b�N�X�̕�����
			TSTR	dlgwnd_dblclick_chkbox_str;				// �Z�[�u�_�C�A���O�̃_�u���N���b�N�`�F�b�N�{�b�N�X�̕�����
			TSTR	warning_str;							// �Z�[�u�m�F�̊m�F���͂̕�����
			TSTR	quick_warning_str;						// �N�C�b�N�Z�[�u�m�F�̊m�F���͂̕�����
			TSTR	msgbk_warning_str;						// ���b�Z�[�W�o�b�N�Z�[�u�m�F�̊m�F���͂̕�����
		};

		// �Z�[�u�̃C���t�H���[�V�������
		S_saveloadinfo		save_info;						// �Z�[�u�̃C���t�H���[�V�������

		// ���[�h�̃C���t�H���[�V�������
		S_saveloadinfo		load_info;						// ���[�h�̃C���t�H���[�V�������

		// �e��m�F���b�Z�[�W�̕�����
		struct Swarninginfo
		{
			TSTR		restart_warning_str;				// �Q�[�����ŏ�����n�߂鎞�̊m�F���b�Z�[�W�̕�����i�f�o�b�O�@�\�j
			TSTR		scenestart_warning_str;				// �Q�[�����V�[������n�߂鎞�̊m�F���b�Z�[�W�̕�����i�f�o�b�O�@�\�j
			TSTR		returnmenu_warning_str;				// �^�C�g�����j���[�ɖ߂鎞�̊m�F���b�Z�[�W�̕�����
			TSTR		returnsel_warning_str;				// �O�̑I�����ɖ߂鎞�̊m�F���b�Z�[�W�̕�����
			TSTR		gameend_warning_str;				// �Q�[�����I�����鎞�̊m�F���b�Z�[�W�̕�����
		};
		Swarninginfo	warninginfo;						// �e��m�F���b�Z�[�W�̕�����
	};
	S_config	config;

	// �Z�[�u�f�[�^��
	int		save_cnt;

	// �N�C�b�N�Z�[�u�f�[�^��
	int		quick_save_cnt;

	// �G���h�Z�[�u�f�[�^��
	int		end_save_cnt;

	// �C���i�[�Z�[�u�̐�
	int		inner_save_cnt;

	// �Z�[�u�|�C���g�����̐�
	int		save_history_cnt;

	// �o�b�N���O�Z�[�u��
	int		backlog_save_cnt;
	int		backlog_save_interval;

	// �O�̑I�����ɖ߂闚���̌�
	int		sel_save_cnt;

	// ���݂��Ȃ��Z�[�u�f�[�^�̕�����
	TSTR	save_no_data_str;

	// �Z�[�u�T���l�C���̐ݒ�
	bool	use_save_thumb;
	int		save_thumb_type;
	C_size	save_thumb_size;

	// ���[�h���̃��C�v
	int		load_wipe_type;		// ���[�h���Ɏg�����C�v�̔ԍ�
	int		load_wipe_time;		// ���[�h���Ɏg�����C�v�̎���

	// �c�����^������
	int		tateyoko_mode;		// �c�����^������

	// �y���O�z
	struct S_namae
	{
		TSTR		regist_name_str;		// �o�^��
		TSTR		change_name_str;		// �ϊ���
		int			color_mod;				// �F���[�h
		int			moji_color_no;			// �����F
		int			shadow_color_no;		// �e�F
		int			fuchi_color_no;			// ���F
	};
	ARRAY<S_namae>		namae_list;

	// �t���O
	int		flag_cnt;
	int		global_flag_cnt;
	int		call_flag_cnt;

	// �f�[�^�x�[�X
	TSTR	database_name[INIMAX_DATABASE_CNT];
	int		database_cnt;

	// �J�E���^
	int		counter_cnt;

	// �J���[�e�[�u��
	C_rgb	color_table[INIMAX_COLOR_TABLE_CNT];
	int		color_table_cnt;

	// �g�[���J�[�u
	TSTR	tonecurve_file;

	// �b�f�e�[�u��
	TSTR	cg_table_file;
	int		cg_table_flag_cnt;

	// �T���l�C���e�[�u��
	TSTR	thumb_table_file;

	// �f�O�O�o�b�t�@
	int		g00_buf_cnt;

	// �}�X�N
	int		mask_cnt;

	// �G����
	struct S_emoji
	{
		TSTR	file_name;
		int		font_size;
	};
	S_emoji	emoji[INIMAX_EMOJI_CNT];
	int		emoji_cnt;

	// �t�H���g�t�@�C��
	struct S_font_file
	{
		TSTR	file_name;
	};
	S_font_file	font_file[INIMAX_EMOJI_CNT];
	int			font_file_cnt;
	S_font_file	private_font_file[INIMAX_EMOJI_CNT];
	int			private_font_file_cnt;

	// �V�X�e���R�[��
	int		msg_back_order;
	int		excall_order;

	// ���[���h
	struct S_world
	{
		int		layer;
	};
	S_world			world[INIMAX_WORLD_CNT];
	int				world_cnt;

	// �{�^��
	struct Sbutton
	{
		// �{�^���F�A�N�V����
		struct Saction
		{
			struct Sstate
			{
				int		rep_pat_no;
				C_point	rep_pos;
				BYTE	rep_tr;
				BYTE	rep_bright;
				BYTE	rep_dark;
			};

			Sstate	state[TNM_BTN_STATE_MAX];
		};
		Saction		action[INIMAX_BTN_ACTION_CNT];
		int			action_cnt;

		// �{�^���F���ʉ�
		struct Sse
		{
			int		hit_no;
			int		push_no;
			int		decide_no;
		};
		Sse			se[INIMAX_BTN_SE_CNT];
		int			se_cnt;

		// �{�^���F�O���[�v
		int			group_cnt;
	};
	Sbutton		button;
	
	// ���b�Z�[�W�{�^��
	struct S_msgbtn
	{
		struct S_state
		{
			int		color_no;
		};

		S_state state[TNM_BTN_STATE_MAX];;
	};
	S_msgbtn	msgbtn[INIMAX_MSGBTN_CNT];
	int			msgbtn_cnt;

	// �I�u�W�F�N�g
	struct Sobject
	{
		bool	use;
		bool	save;
		bool	space_hide;
		int		object_disp_no;
	};
	Sobject			object[INIMAX_OBJECT_CNT];
	int				object_cnt;

	// ���b�Z�[�W
	int		moji_internal_leading_y_rep;

	// ���b�Z�[�W�E�B���h�E�g
	struct Smwnd_waku
	{
		int		extend_type;				// 0:�Œ�T�C�Y�A1:�σT�C�Y
		TSTR	waku_file;					// �g�̃t�@�C����
		TSTR	filter_file;				// �t�B���^�[�̃t�@�C����
		C_rect	filter_margin;				// �t�B���^�[�̃}�[�W���i���b�Z�[�W�E�B���h�E����̍��W�j
		C_argb	filter_color;				// �t�B���^�[�̐F
		bool	filter_config_color;		// ���ݒ�̃t�B���^�[�F��K�p����
		bool	filter_config_tr;			// ���ݒ�̃t�B���^�[�����x��K�p����
		int		icon_no;					// �L�[�҂��A�C�R��
		int		page_icon_no;				// �y�[�W�҂��A�C�R��
		int		icon_pos_type;				// �L�[�҂��A�C�R���̈ʒu�^�C�v
		int		icon_pos_base;				// �L�[�҂��A�C�R���̈ʒu
		C_point	icon_pos;					// �L�[�҂��A�C�R���̈ʒu

		// ���b�Z�[�W�E�B���h�E�g�F�{�^��
		struct S_mwnd_waku_btn
		{
			TSTR	btn_file;				// �{�^���̃t�@�C����
			int		cut_no;					// �J�b�g�ԍ�
			int		pos_base;				// �{�^���̈ʒu
			C_point	pos;					// �{�^���̈ʒu
			int		action_no;				// �A�N�V�����ԍ�
			int		se_no;					// ���ʉ��ԍ�
			int		sys_type;				// �V�X�e���^�C�v
			int		sys_type_opt;			// �V�X�e���^�C�v�I�v�V����
			int		btn_mode;				// �{�^�����[�h�ԍ�
			TSTR	scn_name;				// �R�[����̃V�[����
			TSTR	cmd_name;				// �R�[����̃R�}���h��
			int		z_no;					// �R�[����̂y���x���ԍ�
			TSTR	frame_action_scn_name;	// �t���[���A�N�V�����p�̃V�[����
			TSTR	frame_action_cmd_name;	// �t���[���A�N�V�����p�̃R�}���h��
		};
		S_mwnd_waku_btn	btn[INIMAX_MWND_WAKU_BTN_CNT];

		// ���b�Z�[�W�E�B���h�E�g�F��O��
		C_point	face_pos[INIMAX_MWND_WAKU_FACE_CNT];	// ��O���̈ʒu
	};
	Smwnd_waku		mwnd_waku[INIMAX_MWND_WAKU_CNT];
	int				mwnd_waku_cnt;
	int				mwnd_waku_btn_cnt;
	int				mwnd_waku_face_cnt;
	int				mwnd_waku_object_cnt;

	// ���b�Z�[�W�E�B���h�E
	struct S_mwnd
	{
		int		default_mwnd_no;
		int		default_sel_mwnd_no;
		int		order;
		int		filter_layer_rep;
		int		waku_layer_rep;
		int		shadow_layer_rep;
		int		fuchi_layer_rep;
		int		moji_layer_rep;
		int		face_layer_rep;
		int		shadow_color;
		int		fuchi_color;
		int		moji_color;

		struct S_mwnd_sub
		{
			int		novel_mode;			// 0:�`�u�f���[�h�A1:�m�x�����[�h

			int		extend_type;		// 0:�Œ�T�C�Y�A1:�σT�C�Y
			C_point	window_pos;			// �E�B���h�E�ʒu
			C_size	window_size;		// �E�B���h�E�T�C�Y�i�Œ�T�C�Y�̂݁j
			C_point	msg_pos;			// ���b�Z�[�W�ʒu�i�Œ�T�C�Y�̂݁j
			C_rect	msg_margin;			// ���b�Z�[�W��`�i�E�B���h�E��`����̃}�[�W���j�i�σT�C�Y�̂݁j
			C_size	moji_cnt;			// �c���̕�����
			int		moji_size;			// �����̃T�C�Y
			C_size	moji_space;			// �����Ԃ̃X�y�[�X
			int		moji_color;			// ���b�Z�[�W�̐F�i-1:�W���j
			int		shadow_color;		// ���b�Z�[�W�̐F�i-1:�W���j
			int		fuchi_color;		// ���b�Z�[�W�̐F�i-1:�W���j
			int		ruby_size;			// ���r�̕����T�C�Y
			int		ruby_space;			// ���r�ƕ����Ƃ̃X�y�[�X
			int		waku_no;			// �g�ԍ�
			C_point	waku_pos;			// �g�̈ʒu

			int		name_disp_mode;		// ���O�̕\�����@�i0:���O�E�B���h�E�ɕ\���A1:���b�Z�[�W�̐擪�ɕ\���A2:�\�����Ȃ��j
			int		name_newline;		// ���O�̌�ɉ��s����
			int		name_bracket;		// ���O�����ʂł�����i0=���ʂȂ��A1=�y�z�j

			int		name_extend_type;	// 0:�Œ�T�C�Y�A1:�σT�C�Y
			int		name_window_align;	// ���O�E�B���h�E�̑����ʒu
			C_point	name_window_pos;	// ���O�E�B���h�E�̃E�B���h�E�ʒu
			C_size	name_window_size;	// ���O�E�B���h�E�̃E�B���h�E�T�C�Y�i�Œ�T�C�Y�̂݁j
			C_point	name_msg_pos;		// ���O�E�B���h�E�̃��b�Z�[�W�ʒu�i�Œ�T�C�Y�̂݁j
			C_point	name_msg_pos_rep;	// ���O�E�B���h�E�̃��b�Z�[�W�C���ʒu�i�Œ�T�C�Y�̂݁j
			C_rect	name_msg_margin;	// ���O�E�B���h�E�̃��b�Z�[�W��`�i�E�B���h�E��`����̃}�[�W���j�i�σT�C�Y�̂݁j
			int		name_moji_size;		// ���O�E�B���h�E�̕����̃T�C�Y
			C_size	name_moji_space;	// ���O�E�B���h�E�̕����Ԃ̃X�y�[�X
			int		name_moji_cnt;		// ���O�E�B���h�E�̕�����
			int		name_moji_color;	// ���O�̐F�i-1:�W���j
			int		name_shadow_color;	// ���O�̐F�i-1:�W���j
			int		name_fuchi_color;	// ���O�̐F�i-1:�W���j
			int		name_waku_no;		// ���O�E�B���h�E�̘g�ԍ�
			C_point	name_waku_pos;		// ���O�E�B���h�E�̘g�̈ʒu

			C_rect	talk_margin;			// �Z���t�̃}�[�W���i�m�x�����[�h�j
			int		over_flow_check_size;	// �I�[�o�[�t���[�Ɣ��f����T�C�Y
			int		msg_back_insert_nl;		// ���b�Z�[�W�o�b�N�ɉ��s������

			int		face_hide_name;		// ��O����\�������Ƃ����O���B��

			int		open_anime_type;	// �J���A�j���^�C�v
			int		open_anime_time;	// �J���A�j������
			int		close_anime_type;	// ���A�j���^�C�v
			int		close_anime_time;	// ���A�j������
		};
		S_mwnd_sub	sub[INIMAX_MWND_SUB_CNT];
		int			sub_cnt;
	};
	S_mwnd	mwnd;

	// �{�^���I����
	struct Ssel_btn
	{
		TSTR	base_file;
		TSTR	filter_file;
		C_point	base_pos;
		C_point	rep_pos;
		int		max_y_cnt;
		int		line_width;
		int		x_align;
		int		y_align;
		int		moji_size;
		C_size	moji_space;
		int		moji_cnt;
		C_point	moji_pos;	
		int		moji_x_align;
		int		moji_y_align;
		int		moji_color;
		int		moji_hit_color;
		int		btn_action_no;
		int		open_anime_type;
		int		open_anime_time;
		int		close_anime_type;
		int		close_anime_time;
		int		decide_anime_type;
		int		decide_anime_time;
	};
	Ssel_btn	sel_btn[INIMAX_SEL_BTN_CNT];
	int			sel_btn_cnt;

	// ���b�Z�[�W�o�b�N
	struct S_msg_back
	{
		C_point	window_pos;
		C_size	window_size;
		C_rect	disp_margin;
		int		msg_pos;
		C_size	moji_cnt;
		int		moji_size;
		C_size	moji_space;
		int		moji_color;
		int		moji_shadow_color;
		int		moji_fuchi_color;
		int		active_moji_color;
		int		active_moji_shadow_color;
		int		active_moji_fuchi_color;
		int		debug_moji_color;
		int		debug_moji_shadow_color;
		int		debug_moji_fuchi_color;
		int		name_disp_mode;
		int		name_bracket_type;
		TSTR	waku_file;
		TSTR	filter_file;
		C_rect	filter_margin;
		C_argb	filter_color;
		TSTR	slider_file;
		C_rect	slider_rect;
		int		slider_action_no;
		int		slider_se_no;
		TSTR	close_btn_file;
		C_point	close_btn_pos;
		int		close_btn_action_no;
		int		close_btn_se_no;
		TSTR	msg_up_btn_file;
		C_point	msg_up_btn_pos;
		int		msg_up_btn_action_no;
		int		msg_up_btn_se_no;
		TSTR	msg_down_btn_file;
		C_point	msg_down_btn_pos;
		int		msg_down_btn_action_no;
		int		msg_down_btn_se_no;
		TSTR	koe_btn_file;
		C_point	koe_btn_pos;
		int		koe_btn_action_no;
		int		koe_btn_se_no;
		TSTR	load_btn_file;
		C_point	load_btn_pos;
		int		load_btn_action_no;
		int		load_btn_se_no;
		TSTR	load_call_scene_name;
		TSTR	load_call_command_name;
		TSTR	ex_btn_1_file;
		C_point	ex_btn_1_pos;
		int		ex_btn_1_action_no;
		int		ex_btn_1_se_no;
		TSTR	ex_btn_1_scene_name;
		TSTR	ex_btn_1_command_name;
		TSTR	ex_btn_2_file;
		C_point	ex_btn_2_pos;
		int		ex_btn_2_action_no;
		int		ex_btn_2_se_no;
		TSTR	ex_btn_2_scene_name;
		TSTR	ex_btn_2_command_name;
		TSTR	ex_btn_3_file;
		C_point	ex_btn_3_pos;
		int		ex_btn_3_action_no;
		int		ex_btn_3_se_no;
		TSTR	ex_btn_3_scene_name;
		TSTR	ex_btn_3_command_name;
		TSTR	ex_btn_4_file;
		C_point	ex_btn_4_pos;
		int		ex_btn_4_action_no;
		int		ex_btn_4_se_no;
		TSTR	ex_btn_4_scene_name;
		TSTR	ex_btn_4_command_name;
		TSTR	separator_file;
		TSTR	separator_top_file;
		TSTR	separator_bottom_file;
		int		msg_click_action;
		int		history_cnt;
	};
	S_msg_back	msg_back;

	// �G�t�F�N�g
	int		effect_cnt;

	// �V�F�C�N
	struct Sshake
	{
		struct SUB
		{
			int		x;
			int		y;
			int		time;
		};
		ARRAY<SUB>	sub;
	};
	Sshake	shake[INIMAX_SHAKE_CNT];
	int		shake_cnt;

	// �N�G�C�N
	int		quake_cnt;

	// �t���[���A�N�V�����`�����l��
	int		frame_action_ch_cnt;

	// �a�f�l
	struct Sbgm
	{
		TSTR	regist_name;
		TSTR	file_name;
		int		start_pos;
		int		end_pos;
		int		repeat_pos;
	};
	Sbgm		bgm[INIMAX_BGM_CNT];
	int			bgm_cnt;

	// ���ʉ��`�����l����
	int		pcmch_cnt;

	// ���ʉ��C�x���g��
	int		pcm_event_cnt;

	// �V�X�e����
	struct Sse
	{
		TSTR	file_name;
	};
	Sse		se[INIMAX_SE_CNT];
	int		se_cnt;

	// �a�f�l�t�F�[�h�Q
	int		bgmfade2_in_start_time;
	int		bgmfade2_in_time_len;
	int		bgmfade2_out_start_time;
	int		bgmfade2_out_time_len;
	int		bgmfade2_volume;

	// �G�f�B�b�g�{�b�N�X
	int		editbox_cnt;

	// Twitter
	TSTR	twitter_api_key;
	TSTR	twitter_api_secret;
	TSTR	twitter_callback_url;
	TSTR	twitter_initial_tweet_text;
	TSTR	twitter_overlap_image;

private:

	// ��͗p�ϐ�
	int		line_no;
	TSTR	line_data;

	bool	analize_func();											// ���
	bool	analize_step_1(C_str_analizer& anl, int line_no);		// ��̓X�e�b�v�P
	bool	analize_step_2(C_str_analizer& anl, int line_no);		// ��̓X�e�b�v�Q
	bool	analize_step_waku(C_str_analizer& anl, int line_no, int waku_no);	// ��̓X�e�b�v�F�g
	bool	analize_step_mwnd(C_str_analizer& anl, int line_no, int mw_no);		// ��̓X�e�b�v�F�l�v
	bool	analize_check_value(int val, int min, int max);			// �l�`�F�b�N
	bool	analize_check_alloc(int val, int min, int max);			// �m�ې��`�F�b�N
	bool	analize_check_over(int val, int alloc, int max);		// �͈͊O�A�N�Z�X�`�F�b�N

	bool	error(CTSTR& str);									// �G���[
	bool	error_dont_analize();								// �G���[�F��͂ł��܂���ł���
};

