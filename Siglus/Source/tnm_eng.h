#pragma		once

#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_wnd_menu.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_net.h"
#include	"data/tnm_input.h"
#include	"data/tnm_click_disable.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_flag_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_save_cache.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_icon.h"
#include	"data/tnm_font.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_color_table.h"
#include	"data/tnm_tonecurve.h"
#include	"data/tnm_fog.h"
#include	"data/tnm_cg_table.h"
#include	"data/tnm_bgm_table.h"
#include	"data/tnm_thumb_table.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_btn_template.h"
#include	"data/tnm_namae.h"
#include	"data/tnm_error.h"
#include	"data/tnm_cursor.h"
#include	"data/tnm_steam.h"

#include	"element/elm_prop.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"
#include	"element/elm_database.h"
#include	"element/elm_flag.h"
#include	"element/elm_counter.h"
#include	"element/elm_g00_buf.h"
#include	"element/elm_mask.h"
#include	"element/elm_screen.h"
#include	"element/elm_world.h"
#include	"element/elm_stage.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"
#include	"element/elm_editbox.h"
#include	"element/elm_call.h"
#include	"element/elm_excall.h"

#include	"engine/eng_system.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/flow_proc.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_fetch_point.h"
#include	"engine/eng_debug.h"

// ****************************************************************
// �ƂȂ܃G���W��
// ================================================================
class C_tnm_eng
{
public:
	C_tnm_eng();

	// ���C������
	bool	main();					// ���C������

	// ����������
	bool	init();					// ������
	bool	init_global();			// �������i�O���[�o���j
	bool	init_local();			// �������i���[�J���j

	// �ď���������
	bool	reinit_local(bool restruct_flag);	// �ď������i���[�J���j

	// �������
	bool	free();					// ���
	bool	free_global();			// ����i�O���[�o���j

	// �J�n�I������
	bool	start();				// �J�n
	bool	end();					// �I��

	// �I������
	bool	finish_local();			// ������̓G�������g�̏I�������ł��B

	// �Z�[�u���[�h����
	bool	save_global(C_tnm_save_stream& stream);	// �Z�[�u�i�O���[�o���j
	bool	load_global(C_tnm_save_stream& stream);	// ���[�h�i�O���[�o���j
	bool	save_config(C_tnm_save_stream& stream);	// �Z�[�u�i�O���[�o���j
	bool	load_config(C_tnm_save_stream& stream, int major_version, int minor_version);	// ���[�h�i�O���[�o���j
	bool	save_local_msg(CTSTR& msg);				// �Z�[�u�i���[�J���̌��݂̕��́j
	bool	save_local();							// �Z�[�u�i���[�J���j
	bool	load_local();							// ���[�h�i���[�J���j
	bool	save_local_ex();						// �Z�[�u�i���[�J���ݒ�j
	bool	load_local_ex();						// ���[�h�i���[�J���ݒ�j
	bool	save_call();							// �Z�[�u�i�R�[���j
	bool	load_call();							// ���[�h�i�R�[���j

	// �t���[������
	bool	frame();					// �t���[������
	bool	frame_local();				// �t���[�����[�J������
	bool	frame_global();				// �t���[���O���[�o������
	bool	frame_main_proc();			// �t���[�����C������
	bool	timer_proc();				// �^�C�}�[����
	bool	mouse_move_proc();			// �}�E�X�ړ�����
	bool	debug_key_proc();			// �f�o�b�O�L�[����
	bool	system_key_proc();			// �V�X�e���L�[����
	bool	button_proc();				// �{�^������
	bool	button_regist_proc();		// �{�^���o�^����
	bool	button_hit_test_proc();		// �{�^�������蔻�菈��
	bool	button_event_proc();		// �{�^���C�x���g����
	bool	cancel_call_proc();			// �L�����Z���R�[������
	bool	script_input_proc();		// �X�N���v�g���͏���
	bool	frame_action_proc();		// �t���[���A�N�V��������
	bool	load_after_call_proc();		// ���[�h����R�[������
	bool	screen_size_proc();			// �X�N���[���T�C�Y����

	bool	system_key_proc_msg_back();		// �V�X�e�������F���b�Z�[�W�o�b�N
	bool	system_key_proc_hide_mwnd();	// �V�X�e�������F���b�Z�[�W�E�B���h�E������
	bool	system_key_proc_shortcut();		// �V�X�e�������F�V���[�g�J�b�g�L�[

	// �O���[�o���Z�[�u
	C_tnm_save_stream				m_global_save;				// �O���[�o���̃Z�[�u�f�[�^

	// ���[�J���Z�[�u
	S_tnm_local_save				m_local_save;				// ���[�J���Z�[�u�f�[�^
	int								m_local_save_history_index;	// ���[�J���Z�[�u�f�[�^�̌��݈ʒu�i�Z�[�u�q�X�g���[�p�j

	// �Z���Z�[�u
	S_tnm_sel_save					m_sel_save;

	// �Z�[�u����
	S_tnm_save_history				m_save_history;

	// �o�b�N���O�Z�[�u
	S_tnm_backlog_save				m_backlog_save;

	// ����F�I�����|�C���g�X�g�b�N�i��߂���A���G�b�g�I�j
	S_tnm_local_save				m_sel_save_stock;			// �O�̑I�����ɖ߂�p�̃��[�J���Z�[�u�f�[�^�i��߂���p�j

	// ����F�C���i�[�Z�[�u�i�Ɓ`�߂��w�u���I�j
	ARRAY<S_tnm_local_save>			m_inner_save_list;			// �C���i�[�Z�[�u

public:
	C_tnm_wnd						m_wnd;						// �E�B���h�E
	C_tnm_movie_wnd					m_movie_wnd;				// ���[�r�[�E�B���h�E
	C_tnm_dlg_manager				m_dlg_mng;					// �_�C�A���O�}�l�[�W��
	C_tnm_net						m_net;						// �l�b�g���[�N
	C_tnm_input						m_input;					// ����
	C_input_state					m_cur_input;				// ���݂̓���
	C_input_state					m_last_input;				// �O��̓���
	C_tnm_flag_ini					m_flag_ini;					// flag.ini
	C_tnm_system_info				m_system_info;				// �V�X�e�����
	C_tnm_system_config				m_system_config;			// �V�X�e���R���t�B�O
	C_tnm_global_data				m_global;					// �O���[�o���f�[�^
	C_tnm_dir						m_dir;						// �f�B���N�g��
	C_tnm_config					m_config;					// �R���t�B�O
	C_tnm_syscom					m_syscom;					// �V�X�e���R�}���h
	C_tnm_local_data				m_local;					// ���[�J���f�[�^
	C_tnm_scene_lexer				m_lexer;					// ���N�T�[
	C_tnm_stack						m_stack;					// �X�^�b�N
	C_tnm_timer						m_timer;					// �^�C�}�[
	C_tnm_save_cache				m_save_cache;				// �Z�[�u�L���b�V��
	C_tnm_icon						m_icon;						// �A�C�R��
	C_tnm_cursor_list				m_cursor_list;				// �J�[�\��
	C_tnm_font						m_font;						// �t�H���g
	C_font_name_list				m_font_name_list;			// �t�H���g�����X�g
	C_tnm_cg_table					m_cg_table;					// �b�f�e�[�u��
	C_tnm_bgm_table					m_bgm_table;				// �a�f�l�e�[�u��
	C_tnm_thumb_table				m_thumb_table;				// �T���l�C���e�[�u��
	ARRAY<ARRAY<BYTE>>				m_read_flag_list;			// ���ǃt���O
	C_tnm_namae						m_namae;					// �y���O�z
	ARRAY<C_tnm_chrkoe>				m_chrkoe;					// �L�����N�^�[����
	C_tnm_color_table				m_color_table;				// �J���[�e�[�u��
	C_tnm_tonecurve					m_tonecurve;				// �g�[���J�[�u
	C_tnm_fog						m_fog;						// �t�H�O
	C_tnm_wipe						m_wipe;						// ���C�v
	C_tnm_btn_mng					m_btn_mng;					// �{�^���}�l�[�W��
	C_tnm_btn_action_template_list	m_btn_action_template_list;	// �{�^���A�N�V�����e���v���[�g
	C_tnm_btn_se_template_list		m_btn_se_template_list;		// �{�^���V�X�e�����e���v���[�g
	C_tnm_script_input				m_script_input;				// �X�N���v�g���Ǘ��������
	C_tnm_fetch_point				m_fetch_point;				// �t�F�b�`�|�C���g
	C_tnm_click_disable				m_click_disable;			// �N���b�N�֎~
	C_tnm_debug_dll					m_debug_dll;				// �f�o�b�O�c�k�k
	C_tnm_error						m_error;					// �G���[

	// �G�������g
	C_elm_flag						m_flag;						// �t���O
	C_elm_user_inc_prop				m_user_inc_prop;			// ���[�U�C���N�v���p�e�B
	C_elm_user_scn_prop_list		m_user_scn_prop_list;		// ���[�U�V�[���v���p�e�B���X�g
	C_elm_database_list				m_database_list;			// �f�[�^�x�[�X
	C_elm_counter_list				m_counter_list;				// �J�E���^
	C_elm_frame_action				m_frame_action;				// �t���[���A�N�V����
	C_elm_frame_action_list			m_frame_action_ch_list;		// �t���[���A�N�V�����`�����l��
	C_elm_g00_buf_list				m_g00_buf_list;				// �f�O�O�o�b�t�@
	C_elm_mask_list					m_mask_list;				// �}�X�N
	C_elm_stage_list				m_stage_list;				// �X�e�[�W
	C_elm_msg_back					m_msg_back;					// ���b�Z�[�W�o�b�N
	C_elm_screen					m_screen;					// �X�N���[��
	C_elm_sound						m_sound;					// �T�E���h
	C_elm_pcm_event_list			m_pcm_event_list;			// ���ʉ��C�x���g
	C_elm_editbox_list				m_editbox_list;				// �G�f�B�b�g�{�b�N�X
	C_elm_call_list					m_call_list;				// �R�[��
	C_elm_excall					m_excall;					// �d�w�R�[��

#if __USE_STEAM
	C_tnm_steam						m_steam;
#endif

public:
	bool							m_init_success_flag;		// �����������t���O
};

