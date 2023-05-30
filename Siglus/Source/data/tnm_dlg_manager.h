#pragma		once

#include	"data/tnm_dlg.h"

// �R���t�B�O�E�B���h�E
#include	"dialog/cfg_wnd.h"

// �f�o�b�O�E�B���h�E
#include	"dialog/db_wnd_info_debug.h"
#include	"dialog/db_wnd_info_scn_control.h"
#include	"dialog/db_wnd_info_trace.h"
#include	"dialog/db_wnd_info_error.h"
#include	"dialog/db_wnd_info_d3d.h"
#include	"dialog/db_wnd_info_input.h"
#include	"dialog/db_wnd_info_system.h"
#include	"dialog/db_wnd_info_dir.h"
#include	"dialog/db_wnd_info_script.h"
#include	"dialog/db_wnd_info_stack.h"
#include	"dialog/db_wnd_info_call_histroy.h"
#include	"dialog/db_wnd_info_proc_stack.h"
#include	"dialog/db_wnd_info_namae.h"
#include	"dialog/db_wnd_info_read_flag.h"
#include	"dialog/db_wnd_info_cg_table.h"
#include	"dialog/db_wnd_info_bgm_table.h"
#include	"dialog/db_wnd_info_flag.h"
#include	"dialog/db_wnd_info_flag_ini.h"
#include	"dialog/db_wnd_info_user_call_flag.h"
#include	"dialog/db_wnd_info_user_flag.h"
#include	"dialog/db_wnd_info_namae_flag.h"
#include	"dialog/db_wnd_info_counter.h"
#include	"dialog/db_wnd_info_button.h"
#include	"dialog/db_wnd_info_group.h"
#include	"dialog/db_wnd_info_world.h"
#include	"dialog/db_wnd_info_object.h"
#include	"dialog/db_wnd_info_mwnd.h"
#include	"dialog/db_wnd_info_sound.h"
#include	"dialog/db_wnd_info_system_sound.h"
#include	"dialog/db_wnd_info_koe.h"
#include	"dialog/db_wnd_info_save_history.h"
#include	"dialog/db_wnd_info_element.h"

// ****************************************************************
// �_�C�A���O�}�l�[�W��
// ================================================================
class C_tnm_dlg_manager
{
public:

	ARRAY<C_tnm_dlg *>	dlg_list;

	// ���ݒ�x�[�X
	C_cfg_wnd_config_base			cfg_wnd_config_base;			// �x�[�X
	C_cfg_wnd_config_base_full_mode	cfg_wnd_config_base_full_mode;	// �x�[�X�t�����[�h

	// ���ݒ�\��
	C_cfg_wnd_solo_screen			cfg_wnd_solo_screen;			// �\���F��ʃ��[�h
	C_cfg_wnd_solo_font_msgspd		cfg_wnd_solo_font_msgspd;		// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
	C_cfg_wnd_solo_font				cfg_wnd_solo_font;				// �\���F�t�H���g
	C_cfg_wnd_solo_msgspd			cfg_wnd_solo_msgspd;			// �\���F�������x
	Ccfg_wnd_solo_volume_bgmfade	cfg_wnd_solo_volume_bgmfade;	// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
	Ccfg_wnd_solo_volume			cfg_wnd_solo_volume;			// �\���F����
	Ccfg_wnd_solo_bgmfade			cfg_wnd_solo_bgmfade;			// �\���F�a�f�l�t�F�[�h
	Ccfg_wnd_solo_koemode_chrkoe	cfg_wnd_solo_koemode_chrkoe;	// �\���F�����i�������[�h�A�L�����N�^�[�����j
	Ccfg_wnd_solo_koemode			cfg_wnd_solo_koemode;			// �\���F�������[�h
	Ccfg_wnd_solo_chrkoe			cfg_wnd_solo_chrkoe;			// �\���F�L�����N�^�[����
	Ccfg_wnd_solo_mwndbk			cfg_wnd_solo_mwndbk;			// �\���F�E�B���h�E�w�i�F
	Ccfg_wnd_solo_automode			cfg_wnd_solo_automode;			// �\���F�I�[�g���[�h
	Ccfg_wnd_solo_jitan				cfg_wnd_solo_jitan;				// �\���F���Z�Đ�
	Ccfg_wnd_solo_else				cfg_wnd_solo_else;				// �\���F���̑�
	Ccfg_wnd_solo_system			cfg_wnd_solo_system;			// �\���F�V�X�e��
	Ccfg_wnd_solo_movtype			cfg_wnd_solo_movtype;			// �\���F���[�r�[�̍Đ����@

	// �Z�[�u�_�C�A���O
	C_sys_wnd_solo_save				sys_wnd_solo_save;				// �Z�[�u�_�C�A���O
	// ���[�h�_�C�A���O
	C_sys_wnd_solo_load				sys_wnd_solo_load;				// ���[�h�_�C�A���O
	// �c�C�[�g�_�C�A���O
	C_sys_wnd_solo_tweet			sys_wnd_solo_tweet;				// �c�C�[�g�_�C�A���O

	// �f�o�b�O���E�B���h�E
	C_db_wnd_info_debug				db_wnd_info_debug;				// �f�o�b�O�ݒ�
	C_db_wnd_info_scn_control		db_wnd_info_scn_control;		// �V�[���R���g���[��
	C_db_wnd_info_trace				db_wnd_info_trace;				// �g���[�X
	C_db_wnd_info_error				db_wnd_info_error;				// �G���[���

	C_db_wnd_info_d3d_device		db_wnd_info_d3d_device;			// Direct3D �f�o�C�X���
	C_db_wnd_info_d3d_resource		db_wnd_info_d3d_resource;		// Direct3D ���\�[�X���
	C_db_wnd_info_input				db_wnd_info_input;				// ���͏��
	C_db_wnd_info_system			db_wnd_info_system;				// �V�X�e�����
	C_db_wnd_info_dir				db_wnd_info_dir;				// �f�B���N�g�����
	C_db_wnd_info_script			db_wnd_info_script;				// �X�N���v�g���
	C_db_wnd_info_stack				db_wnd_info_stack;				// �X�^�b�N���
	C_db_wnd_info_proc_stack		db_wnd_info_proc_stack;			// �v���Z�X�X�^�b�N���
	C_db_wnd_info_call_histroy		db_wnd_info_call_history;		// �Ăяo������
	C_db_wnd_info_namae				db_wnd_info_namae;				// �y���O�z���
	C_db_wnd_info_read_flag			db_wnd_info_read_flag;			// ���ǃt���O���
	C_db_wnd_info_cg_table			db_wnd_info_cg_table;			// �b�f�e�[�u�����
	C_db_wnd_info_bgm_table			db_wnd_info_bgm_table;			// �a�f�l�e�[�u�����
	C_db_wnd_info_element			db_wnd_info_element;			// �G�������g���
	C_db_wnd_info_flag				db_wnd_info_flag[TNM_FLAG_CNT];	// �t���O���
	C_db_wnd_info_flag_ini			db_wnd_info_flag_ini[20];		// flag.ini ���
	C_db_wnd_info_user_flag			db_wnd_info_user_flag;			// ���[�U�t���O���
	C_db_wnd_info_user_call_flag	db_wnd_info_user_call_flag;		// ���[�U�R�[���t���O���
	C_db_wnd_info_namae_flag		db_wnd_info_namae_flag;			// ���O�t���O���
	C_db_wnd_info_counter			db_wnd_info_counter;			// �J�E���^���
	C_db_wnd_info_btn				db_wnd_info_btn;				// �{�^�����
	C_db_wnd_info_group				db_wnd_info_group;				// �O���[�v���
	C_db_wnd_info_world				db_wnd_info_world;				// ���[���h���
	C_db_wnd_info_object			db_wnd_info_object[6];			// �I�u�W�F�N�g���
	C_db_wnd_info_mwnd				db_wnd_info_mwnd;				// ���b�Z�[�W�E�B���h�E���
	C_db_wnd_info_sound				db_wnd_info_sound;				// �T�E���h���
	C_db_wnd_info_system_sound		db_wnd_info_system_sound;		// �{�����[�����
	C_db_wnd_info_koe				db_wnd_info_koe;				// ���E�H�b�`���
	C_db_wnd_info_save_history		db_wnd_info_save_history;		// �Z�[�u�|�C���g����

	void		regist();						// �o�^
	void		init();							// ������
	void		free();							// ���
	void		load_initial_state();			// �����X�e�[�^�X�ǂݍ���
	void		save_initial_state();			// �����X�e�[�^�X�ۑ�
	void		initial_open();					// �����I�[�v��
	void		set_enable(bool flag);			// �֎~�^����ݒ�
	bool		check_mouse_over();				// �}�E�X���������Ă���^�������Ă��Ȃ����擾
	void		frame();						// �t���[������
};

