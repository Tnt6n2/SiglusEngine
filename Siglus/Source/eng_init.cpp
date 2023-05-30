#include	"pch.h"

#include	"../resource.h"

#include	"tnm_eng.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_reg_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_wnd_menu.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_moji_manager.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_cursor.h"

#include	"element/elm_database.h"
#include	"element/elm_counter.h"
#include	"element/elm_frame_action.h"
#include	"element/elm_g00_buf.h"
#include	"element/elm_mask.h"
#include	"element/elm_world.h"
#include	"element/elm_stage.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_screen.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"
#include	"element/elm_call.h"

#include	"engine/eng_dir.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_display.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_input.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_database.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_font.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_debug.h"
#include	"engine/eng_twitter.h"
#include	"engine/eng_etc.h"

#include	"engine/ifc_steam.h"

#include	"localize/localize.h"

#if __USE_EMOTE
#include	"engine/eng_emote.h"
#endif

#include	<lzss.h>

// ****************************************************************
// �G���W���̌Ăяo��
// ================================================================
bool tnm_init_local()
{
	Gp_global->__bgm_stop_cntr = 0;

	// �G���W���̃��[�J��������
	return Gp_eng->init_local();
}
bool tnm_reinit_local(bool restruct_flag)
{
	Gp_global->__bgm_stop_cntr = 0;

	// �G���W���̃��[�J���ď�����
	return Gp_eng->reinit_local(restruct_flag);
}
bool tnm_finish_local()
{
	// �G���W���̃��[�J���I��
	return Gp_eng->finish_local();
}
bool tnm_free()
{
	// �G���W���̉��
	return Gp_eng->free();
}

// ****************************************************************
// �G���W���F�v���g�^�C�v�錾
// ================================================================
void	tnm_get_start_time();
void	tnm_get_system_info();
void	tnm_get_directx_info();
void	tnm_get_wmp_info();
bool	tnm_check_japanese_edition_windows();

bool	tnm_init_dir();
bool	tnm_create_save_dir();
bool	tnm_init_direct_3d();
bool	tnm_init_direct_3d_device();
bool	tnm_create_vertex_buffer();
bool	tnm_create_wipe_buffer();
bool	tnm_init_direct_sound();

// ****************************************************************
// �G���W���F������
// ================================================================
//		�Q�[���N�����Ɉ�x�����ʂ�܂��B
// ================================================================
bool C_tnm_eng::init()
{
	// �O���[�o���̏������������s���܂��B
	if (!init_global())
		return false;

	return true;
}

// ****************************************************************
// �G���W���F�����������F�O���[�o��
// ================================================================
//		�Q�[���N�����Ɉ�x�����ʂ�܂��B
//		���C���E�B���h�E���쐬���A�\�����܂��B
//		�e��X�e�[�g�̏��������s���܂��B
// ================================================================
bool C_tnm_eng::init_global()
{
	// �G���[��������
	m_error.init();

	// �G���[�\���̂��߂ɃV�[���ԍ���������
	m_lexer.set_cur_scn_no(-1);
	m_lexer.set_cur_line_no(0);

	// �^�C�}�[��������
	m_timer.real_time = ::timeGetTime();
	m_timer.real_time_past = 0;
	m_timer.global_real_time = 0;

	// �O���[�o���p�����[�^��������
	{
		m_global.debug_flag = false;
		m_global.debug_decided_flag = false;
		m_global.debug_not_disp_overflow_error = Gp_cmd_line->not_disp_overflow_error_set;
		m_global.shader_from_source = false;
		m_global.setup_type = TNM_SETUP_TYPE_NONE;

		m_global.game_screen_size = Gp_ini->screen_size;
		m_global.total_game_screen_pos = C_point(0, 0);
		m_global.total_game_screen_pos_last = C_point(0, 0);
		m_global.total_game_screen_size = Gp_ini->screen_size;
		m_global.total_game_screen_size_last = Gp_ini->screen_size;
		m_global.total_screen_size = Gp_ini->screen_size;
		m_global.total_window_size = C_size(0, 0);

		m_global.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
		m_global.screen_size_mode_window = TNM_SCREEN_SIZE_MODE_WINDOW;
		m_global.screen_size_scale = C_size(100, 100);
		m_global.screen_size_free = Gp_ini->screen_size;
		m_global.fullscreen_change_resolution = false;
		m_global.fullscreen_display_no = 0;
		m_global.fullscreen_resolution_no = 0;
		m_global.screen_size_mode_change_start_time = 0;

		m_global.dialog_fullscreen_change_resolution = false;
		m_global.dialog_fullscreen_display_no = 0;
		m_global.dialog_fullscreen_resolution_no = 0;

		m_global.change_display_mode_proc_cnt = 0;
		m_global.change_display_mode_proc_old_screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
		m_global.change_display_mode_proc_old_screen_size_mode_window = TNM_SCREEN_SIZE_MODE_WINDOW;
		m_global.change_display_mode_proc_old_screen_size_scale = C_size(0, 0);
		m_global.change_display_mode_proc_old_screen_size_free = C_size(0, 0);
		m_global.change_display_mode_proc_old_window_pos = C_point(0, 0);
		m_global.change_display_mode_proc_old_change_resolution = false;
		m_global.change_display_mode_proc_old_display_no = 0;
		m_global.change_display_mode_proc_old_resolution_no = 0;

		m_global.wait_display_vsync_def = true;
		m_global.wait_display_vsync_total = true;
		m_global.wipe_buffer_cnt = 2;

		m_global.start_scene_name = Gp_cmd_line->start_scene_set ? Gp_cmd_line->start_scene : Gp_ini->start_scene_name;
		m_global.start_z_no = Gp_cmd_line->start_z_no_set ? Gp_cmd_line->start_z_no : Gp_ini->start_scene_z_no;

		m_global.active_flag = false;
		m_global.focused_flag = false;
		m_global.focused_flag_on_edit_box = false;
		m_global.editbox_hide_flag_total = false;

		m_global.ex_call_flag = false;
		m_global.ctrl_skip_flag = false;
		m_global.cs_skip_flag = false;
		m_global.alt_menu_wait_flag = false;
		m_global.skip_click_1_frame = false;
		m_global.cant_auto_skip_before_click = false;

		m_global.frame_rate_100msec_total = 0;
		m_global.frame_rate_100msec_index = 0;
		ZeroMemory(m_global.frame_rate_100msec, 10 * sizeof(DWORD));
		m_global.read_skip_enable_flag = false;
		m_global.is_auto_mode_count_start = false;
		m_global.auto_mode_count_start_time = 0;
		m_global.auto_mode_cursor_off = false;
		m_global.mouse_cursor_hide_time = 0;
		m_global.is_koe_only_count_start = false;
		m_global.koe_only_count_start_time = 0;
		m_global.all_total_volume = Gp_ini->config.all_user_volume;
		m_global.bgm_buf_total_volume = Gp_ini->config.bgm_user_volume;
		m_global.koe_buf_total_volume = Gp_ini->config.koe_user_volume;
		m_global.pcm_buf_total_volume = Gp_ini->config.pcm_user_volume;
		m_global.se_buf_total_volume = Gp_ini->config.se_user_volume;
		m_global.mov_buf_total_volume = Gp_ini->config.mov_user_volume;
		m_global.play_silent_sound = false;
		m_global.bgmfade_flag = false;
		m_global.bgmfade_cur_time = 0;
		m_global.bgmfade_start_value = 255;
		m_global.bgmfade_total_volume = 255;
		m_global.bgmfade2_flag = false;
		m_global.bgmfade2_need_flag = false;
		m_global.bgmfade2_cur_time = 0;
		m_global.bgmfade2_start_value = 255;
		m_global.bgmfade2_total_volume = 255;
		m_global.mouse_move_left_amari = 0.0f;
		m_global.mouse_move_right_amari = 0.0f;
		m_global.mouse_move_up_amari = 0.0f;
		m_global.mouse_move_down_amari = 0.0f;
		m_global.total_font_name = Gp_ini->config.font_name;
		m_global.total_font_futoku = Gp_ini->config.font_futoku;
		m_global.total_font_shadow = Gp_ini->config.font_shadow;

		m_global.disp_flag = true;
		m_global.present_flag = true;
		m_global.disp_because_msg_wait_cnt = 0;
		m_global.disp_because_msg_wait_cnt_max = 0;
		m_global.system_wipe_flag = false;
		m_global.skip_because_skip_trigger = false;
		m_global.capture_type = TNM_CAPTURE_TYPE_NONE;
		m_global.capture_sorter = S_tnm_sorter(INT_MAX, INT_MAX);
		m_global.capture_prior = TNM_CAPTURE_PRIOR_NONE;
		m_global.game_timer_move_flag = false;
		m_global.do_frame_action_flag = false;
		m_global.do_load_after_call_flag = false;
		m_global.sel_start_call_flag = false;
		m_global.msg_wait_skip_by_click = false;
		m_global.is_breaking = false;
		m_global.break_step_flag = false;
		m_global.game_end_flag = false;
		m_global.game_end_no_warning = false;
		m_global.game_end_save_done = false;
		m_global.reinit_msgbk_except_flag = false;
		m_global.max_vertex_buffer_size = 0;
		m_global.index_buffer_size = 0;
		m_global.video_memory_rest_size = 0;
		m_global.video_memory_use_size = 0;
		m_global.video_memory_use_max_size = 0;
		m_global.__bgm_stop_cntr = 0;
		m_global.return_scene_name.clear();
		m_global.return_scene_z_no = 0;
	}

	// �N���������擾�i�G���[���O�̐擪�Ɏ����Ă��邽�ߍŏ��ɍs���܂��j
	tnm_get_start_time();

	// �f�B���N�g����ݒ�i��������Ȃ��� log.txt ���o�͂ł��Ȃ��̂ŗD�揇�ʂ͍����ł��j
	if (!tnm_init_dir())
		return false;

	// �f�B���N�g�����ݒ肳�ꂽ�̂ŁA���O�t�@�C�����폜���܂��B
	// �G���[���O���o�͂����O�ɍs���K�v������̂ŁA�^�C�~���O�͂����ȊO����܂���B
	if (file_get_type(Gp_dir->get_log_file_path()) == 1)
		filesys_delete_file(Gp_dir->get_log_file_path());

	// �Z�[�u�t�H���_���쐬�i����� log.txt ���쐬�ł���悤�ɂȂ�܂��j
	if (!tnm_create_save_dir())
		return false;

	// �f�o�b�O dll ��ǂݍ���
	m_debug_dll.load();

	// �悤�₭�f�o�b�O���ǂ������m�肵��
	m_global.debug_decided_flag = true;

	// �V�X�e���ݒ�f�[�^��������
	{
		m_system_config.d3d_adapter_no = D3DADAPTER_DEFAULT;
		m_system_config.d3d_hardware_vertex_flag = true;
		m_system_config.d3d_present_interval = 1;
		m_system_config.d3d_use_mipmap_flag = true;
		m_system_config.d3d_wipe_buffer_cnt = 2;
		m_system_config.ds_play_silent_sound_flag = false;
		m_system_config.movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
	}

	// �V�X�e���ݒ�f�[�^��ǂݍ���
	if (!tnm_load_system_on_file())
		return false;

	// �V�X�e���ݒ�f�[�^����V�X�e���l��ݒ肷��
	m_global.wait_display_vsync_def = 
		Gp_cmd_line->no_interval_set ? false :					// �R�}���h���C���Łu�҂��Ȃ��v���ݒ肳��Ă���
		m_system_config.d3d_present_interval ? true : false;	// �V�X�e���ݒ�t�@�C���ɏ]��
	m_global.wait_display_vsync_total = m_global.wait_display_vsync_def;
	m_global.wipe_buffer_cnt = m_system_config.d3d_wipe_buffer_cnt;

	// ���{��Ń`�F�b�N
#if 0
#if defined(__SIGLUS_JA)
	if (!tnm_check_japanese_edition_windows())
		return false;
#endif
#endif

	// �V�X�e�������擾����
	tnm_get_system_info();

	// DirectX �̃o�[�W�������擾����
	tnm_get_directx_info();

	// Flag.ini ��ǂݍ���
	if (!m_flag_ini.analize_flag_ini())
		return false;

	// Steam ��������
#if __USE_STEAM

	if (!SteamAPI_Init())
	{
		if (m_global.debug_flag)
		{
			tnm_set_error(TNM_ERROR_TYPE_INFO, _T("Steam �̏������Ɏ��s���܂����B\n�f�o�b�O���[�h�̂��߁ASteam �̋@�\���g�킸�ɋN�����܂��B"));
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam �̏������Ɏ��s���܂����B"));
			return false;
		}
	}

	steam_first_proc();

#endif

	// Direct3D ��������
	if (!tnm_init_direct_3d())
		return false;

	// �A�C�R���t�@�C����ǂݍ���
	m_icon.create_icon();

	// �t�H���g��ǂݍ��ށi�f�t�H���g�̃A�y���h���g�����Ƃɒ��Ӂj
	for (int i = 0; i < Gp_ini->font_file_cnt; i++)
	{
		TSTR file_name = Gp_ini->font_file[i].file_name;
		if (!file_name.empty())
		{
			// �t�@�C��������
			TSTR file_path = tnm_find_dat(file_name);
			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�t�H���g�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
				return false;
			}
			// �t�@�C������t�H���g��ǉ�
			int font_num = AddFontResourceEx(file_path.c_str(), FR_PRIVATE, NULL);
			if (font_num == 0)
			{
				set_last_error_win(_T("AddFontResourceEx"));
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�t�H���g�t�@�C�� ") + file_name + _T(" �̒ǉ��Ɏ��s���܂����B\n") + get_last_error_msg());
			}
		}
	}
	// �t�H���g���v���C�x�[�g�ɓǂݍ���
	for (int i = 0; i < Gp_ini->private_font_file_cnt; i++)
	{
		TSTR file_name = Gp_ini->private_font_file[i].file_name;
		if (!file_name.empty())
		{
			// �t�@�C��������
			TSTR file_path = tnm_find_dat(file_name);
			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�t�H���g�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
				return false;
			}
			// �t�@�C����ǂݍ���
			ARRAY<BYTE> file_data;
			if (!C_file::read_full_data(file_path, file_data))
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�t�H���g�t�@�C�� ") + file_name + _T(" ���ǂݍ��߂܂���ł����B\n") + get_last_error_msg());
				return false;
			}
			// ����
			tpc_unangou(file_data.get(), file_data.size());
			int size = lzss_unpack(file_data.get(), NULL);
			ARRAY<BYTE> unangou_data(size);
			lzss_unpack(file_data.get(), unangou_data.get());

			// ����������t�H���g��ǉ�
			DWORD read_num = 0;
			DESIGNVECTOR design_vector = { 0 };
			HANDLE h_mem_font = AddFontMemResourceEx(unangou_data.get(), unangou_data.size(), NULL, &read_num);
			if (h_mem_font == 0)
			{
				set_last_error_win(_T("AddFontMemResourceEx"));
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�t�H���g�t�@�C�� ") + file_name + _T(" �̒ǉ��Ɏ��s���܂����B\n") + get_last_error_msg());
				return false;
			}
		}
	}

	// �E�B���h�E������������
	Gp_wnd->init();

	// �E�B���h�E�p�� window.ini ��ǂݍ���
	C_ini_file::body().load_ini_file(Gp_dir->get_window_ini_file_path());

	// Twitter ����ǂݍ���
	//twitter_load_state();

	// ���C���E�B���h�E���쐬����i�����ł͂܂��\������܂���B�j
	TSTR window_name = tnm_get_window_name();
	if (!Gp_wnd->create(_T("siglus_engine_main_window"), window_name, Gp_ini->game_name, -1, -1, m_global.total_screen_size.cx, m_global.total_screen_size.cy, NULL, NULL, -1, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_THICKFRAME, 0, true, true, NULL))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�E�B���h�E�̍쐬�Ɏ��s���܂����B"));
		return false;
	}

	// �E�B���h�E�T�C�Y�������ōX�V
	m_global.total_window_size = Gp_wnd->get_window_rect().size();

	// ���j���[���쐬���ăE�B���h�E�ɃA�^�b�`����i�f�o�b�O���[�h�̂݁j
	if (m_global.debug_flag)
	{
		Gp_wnd->m_wnd_menu.create();
		Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	}

	// �E�B���h�E�T�C�Y���Ē�������
	Gp_wnd->set_client_size(m_global.total_screen_size);

	// Direct3D �f�o�C�X��������
	if (!tnm_init_direct_3d_device())
		return false;

	// DirectSound ��������
	if (!tnm_init_direct_sound())
		return false;

	// Windows Media Player �̃o�[�W�������擾
	tnm_get_wmp_info();

	// theora �̃e�[�u��������
	C_omv_player::ready_table();

	// E-mote
#if __USE_EMOTE
	tnm_emote_init();
#endif

	// �E�B���h�E���X�V���ĕ\��
	Gp_wnd->update();
	Gp_wnd->set_show(true);

	// �}�E�X�J�[�\�����쐬�i�f�t�H���g�̃A�y���h���g�����Ƃɒ��Ӂj
	m_cursor_list.sub.resize(Gp_ini->mouse_cursor_cnt);
	for (int i = 0; i < Gp_ini->mouse_cursor_cnt; i++)
	{
		m_cursor_list.sub[i].create_cursor(i);
	}

	// �X�^�b�N���m��
	m_stack.int_now_cnt = 0;
	m_stack.int_alloc_cnt = 1024;
	m_stack.int_p = (int *)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_stack.int_alloc_cnt * sizeof(int));

	// �Z�[�u�L���b�V����������
	m_save_cache.init();

	// �Z�[�u������������
	tnm_init_save_history();

	// �o�b�N���O�Z�[�u��������
	tnm_init_backlog_save();

	// ���[�U�R���t�B�O���t�H�[�}�b�g
	tnm_format_config();

	// �_�C�A���O�̏���ݒ�
	m_global.dialog_fullscreen_change_resolution = m_config.fullscreen_change_resolution;
	m_global.dialog_fullscreen_display_no = m_config.fullscreen_display_no;
	m_global.dialog_fullscreen_resolution_no = m_config.fullscreen_resolution_no;

	// ���{��̂݃t�H���g���i��i����ȊO�͕�����Ȃ��̂őS���o���܂��c�j
#if defined(__SIGLUS_JA)
	BYTE chara_set = SHIFTJIS_CHARSET;
#else
	BYTE chara_set = ANSI_CHARSET;
#endif

	// �t�H���g���X�g���쐬
	m_font_name_list.make_font_list(Gp_wnd->get_handle(), false, false, chara_set, Gp_ini->tateyoko_mode == 1 ? FONT_YOKOTATE_MODE_TATE_ONLY : FONT_YOKOTATE_MODE_YOKO_ONLY);

	// �������܂ŃO���[�o��
	// ���������烍�[�J��

	// �V�[�����J��
	if (!m_lexer.load_scn(LCL_SCENE_PCK))
	{
		m_error.set_error(TNM_ERROR_TYPE_FATAL, _T("�V�[���f�[�^�i") LCL_SCENE_PCK _T("�j���J���܂���ł����B"));
		return false;
	}

	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("�V�[���f�[�^��ǂݍ��݂܂����B"));

	// �t���O��������
	m_flag.init_global();

	// �f�[�^�x�[�X��������
	tnm_database_init();

	// �b�f�e�[�u����������
	m_cg_table.init();

	// �a�f�l�e�[�u����������
	m_bgm_table.init();

	// �T���l�C���e�[�u����������
	m_thumb_table.create_table();

	// ���ǃt���O���쐬
	m_read_flag_list.resize(m_lexer.get_scn_cnt());
	for (int i = 0; i < (int)m_read_flag_list.size(); i++)
		m_read_flag_list[i].resize(m_lexer.get_read_flag_cnt(i), 0);

	// �y���O�z��������
	m_namae.init();

	// �L�����N�^�[������������
	if (Gp_ini->config.chrkoe_cnt > 0)	{
		m_chrkoe.resize(Gp_ini->config.chrkoe_cnt);
		for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
			m_chrkoe[i].name_str = Gp_ini->config.chrkoe[i].name_str;	// ���O���ڐA
			switch (Gp_ini->config.chrkoe[i].check_mode)	{				// �`�F�b�N���[�h�i0=������ԂŃ`�F�b�N���Ȃ�  1=���J��ԂŃ`�F�b�N���Ȃ�  2=������ԂŃ`�F�b�N����j
				case TNM_CHRKOE_CHECK_MODE_NONE:		m_chrkoe[i].look_flag = false;	break;
				case TNM_CHRKOE_CHECK_MODE_LOOK:		m_chrkoe[i].look_flag = true;	break;
				case TNM_CHRKOE_CHECK_MODE_AUTO:		m_chrkoe[i].look_flag = false;	break;
				default:								m_chrkoe[i].look_flag = true;	break;
			}
		}
	}

	// �J���[�e�[�u����������
	m_color_table.init();

	// �g�[���J�[�u��������
	m_tonecurve.init();

	// �{�^���A�N�V�����e���v���[�g��������
	m_btn_action_template_list.init();

	// �{�^�����ʉ��e���v���[�g��������
	m_btn_se_template_list.init();

	// �T�E���h��������
	m_sound.init();

	// �N���b�N�֎~��������
	m_click_disable.init();

	// �G�������g�������I��
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("�O���[�o���ȃG�������g�����������܂����B"));

	// �O���[�o���f�[�^�����[�h
	if (!tnm_load_global_on_file())
		return false;

	// �R�}���h���C��������ݒ���㏑������
	if (Gp_cmd_line->fullscreen_mode_set)
		m_config.screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	if (Gp_cmd_line->window_mode_set)
		m_config.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;

	// ���ݒ��K�p����
	tnm_update_screen_size_mode();

	// �{�����[�����X�V����
	tnm_update_sound_volume(true);

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���ݒ���č\�z���܂����B"));

	// �f�o�b�O�E�B���h�E��o�^
	Gp_dlg_mng->regist();

	// �f�o�b�O�E�B���h�E��������
	Gp_dlg_mng->init();

	// �f�o�b�O�E�B���h�E�̏����X�e�[�^�X��ǂݍ���
	Gp_dlg_mng->load_initial_state();

	// �f�o�b�O�E�B���h�E�̏����I�[�v��
	Gp_dlg_mng->initial_open();

	// ���C���E�B���h�E���A�N�e�B�u�ɂ���
	Gp_wnd->set_active();

	// �G���[���
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("������������������������������������������"));
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("���A�v���P�[�V�����̏������ɐ������܂����B"));
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("������������������������������������������"));

	// ����������
	m_init_success_flag = true;

	// ����
	return true;
}

// ****************************************************************
// �G���W���F���{��`�F�b�N
// ================================================================

// ================================================================
//	kernel32.dll �o�[�W�������́u����v�𒲂ׂ�
// ----------------------------------------------------------------
//	�o�[�W�������́u����R�[�h�v���u���{��v�ł���΁u���{��� Windows�v�Ɣ��肵�܂��B
//	Windows �̖{�̂Ƃ������ׂ��c�k�k�t�@�C���ł��� kernel32.dll ��Arundll32.exe �Auser32.dll �Ȃ�
//	Windows �̍����Ɋւ��t�@�C���Ȃ牽�ł��ǂ��Ǝv���܂��B
// ----------------------------------------------------------------
bool tnm_check_kernel32_language()
{
	// ����ƃR�[�h�y�[�W
	struct GFV_LANGANDCODEPAGE
	{
		WORD language;
		WORD codepage;
	};

	bool japanese_flag = false;

	// �V�X�e���f�B���N�g���̃p�X���擾���Akernel32.dll �ւ̃p�X���쐬
	TCHAR system_dir_path[MAX_PATH];
	if (GetSystemDirectory(system_dir_path, MAX_PATH) == 0)	{
		set_last_error_win(_T("GetSystemDirectory"));
		return false;
	}

	TSTR kernel32_file_path = TSTR() + system_dir_path + _T("\\kernel32.dll");
	if (file_get_type(kernel32_file_path) != 1)	{
		set_last_error(kernel32_file_path + _T(" ��������܂���ł����B"), _T("CheckLanguage"));
		return false;
	}

	// �o�[�W�������T�C�Y�擾�i�G���[����0�j
	int version_info_size = GetFileVersionInfoSize(kernel32_file_path.c_str(), NULL);
	if (version_info_size == 0)	{
		set_last_error_win(_T("GetFileVersionInfoSize"));
		return false;
	}

	// �o�[�W�������u���b�N���R�s�[����o�b�t�@�m��
	ARRAY<BYTE> version_info(version_info_size);

	// �o�[�W�������u���b�N�̎擾
	if (GetFileVersionInfo(kernel32_file_path.c_str(), 0, version_info_size, (void *)version_info.get()) == FALSE)	{
		set_last_error_win(_T("GetFileVersionInfo"));
		return false;
	}

	// �o�[�W�������̎擾
	GFV_LANGANDCODEPAGE* lang_p;
	unsigned int lang_size;
	if (VerQueryValue((void * const)version_info.get(), _T("\\VarFileInfo\\Translation"), (void **)&lang_p, &lang_size) == FALSE)	{
		set_last_error_win(_T("VerQueryValue"));
		return false;	// �G���[�ڍׂ͏o���Ȃ��I
	}

	// ����R�[�h�����{��ł��邩�ǂ������ׂ�
	int lang_cnt = lang_size / sizeof(GFV_LANGANDCODEPAGE);		// ���[�v���͂P�񂵂��Ȃ��Ǝv���܂��B
	for (int i = 0; i < lang_cnt; i++)	{
		if (lang_p->language == 0x0411)	{		// 0x0411 �����{��ł��B
			japanese_flag = true;
			break;
		}
		lang_p++;
	}

	if (!japanese_flag)	{
		set_last_error(_T("���{���񂪌�����܂���ł����B"), _T("CheckLanguage"));
		return false;
	}

	return japanese_flag;
}

// ================================================================
// �V�X�e������̕�������擾����
// ----------------------------------------------------------------
//	LOCALE_SLANGUAGE �́u���ꖼ�v�̕�������擾���܂��B
//	�擾����镶����́A���̍��̌���̕�����Ŏ擾����܂��B
//	�p��� Windows XP �̏ꍇ�A�uJAPANESE�v�ƌ���������ɂȂ�܂��B
//	���{��� Windows XP �̏ꍇ�A�u���{��v�ƌ���������ɂȂ�܂��B
//	�������A���{��� Windows Vista �̏ꍇ�A�u���{��i���{�j�v�ƂȂ�̂ŁA
//	���̕�����̂R�����i�U�o�C�g�j�݂̂Ŕ��肵�Ă��܂��B
//	���{��ȊO�̐ݒ�ł����Ă��擾���������񂪁u���{�ꕶ����v�ł���΁A
//	���{��łƔ��肵�Ă��ǂ������ł��B
//	�Ⴆ�΁A�p�ꂪ�I������Ă���΁u�p��v�ƌ��������񂪕Ԃ��Ă��܂��B
//	���ꂪ�p��łł���΁uEnglish�v�ƕԂ��Ă��܂��B
//
//	�����{��� Windows 7 RC1 32bit �ŁuJAPANESE�v���߂��Ă���ցc�s�s
//		�� �|���̂ŁA����R�[�h�Ŕ��肷�邱�Ƃɂ��܂����B���{��� "0411" �ł��B
// ----------------------------------------------------------------

TSTR tnm_get_system_language_str()
{
	TCHAR buf[1024] = {0};
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SLANGUAGE, buf, 1024);
	return buf;
}

// �V�X�e������̃R�[�h���擾����
TSTR tnm_get_system_language_code()
{
	TCHAR buf[1024] = {0};
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, buf, 1024);
	return buf;
}

// �V�X�e������𔻒肷��
bool tnm_check_system_language()
{
#if 0		// ���ꕶ����Ŕ���
	avg_get_system_language_str(str, 4096);
	if (strcomp2(str, (UC *)"���{��", 6) == 0)	{	return true;	}
	else	{
		change_str(str, 1);
		if (strcomp2(str, (UC *)"JAPANESE", 8) == 0)	{	return true;	}
		else									{	return false;	}
	}
#else		// ����R�[�h�Ŕ���
	if (tnm_get_system_language_code().substr(0, 4) == _T("0411"))
		return true;

	return false;
#endif
}

// ================================================================
// �^�C���]�[���𒲂ׂ�
// ----------------------------------------------------------------
//	�u�^�C���]�[���v���u�����v�ł���΁A�u���{���Windows�v�Ɣ��肵�܂��B
//	�擾����镶����́A���̍��̌���̕�����Ŏ擾����܂��B
//	�p���Windows XP �̏ꍇ�A�uTokyo Standard Time�v�ƌ���������ɂȂ�܂��B
//	���{���Windows XP �̏ꍇ�A�u���� (�W����)�v�ƌ���������ɂȂ�܂��B
//	����\�L���ς��\��������̂ŁA���S���l���u�����v�̂Q�����i4�o�C�g�j�݂̂𔻒肵�Ă��܂��B
//	���{��ȊO�̐ݒ�ł����Ă��擾���������񂪓��{��ł���΁A���{��łƔ��肵�Ă��ǂ������ł��B
// ----------------------------------------------------------------

// �^�C���]�[�����擾����
TSTR tnm_get_timezone_str()
{
	TIME_ZONE_INFORMATION info = {0};
	GetTimeZoneInformation(&info);
	return info.StandardName;
}

bool tnm_check_timezone()
{
	TSTR timezone = tnm_get_timezone_str();
	if (timezone.substr(0, 2) == _T("����"))
		return true;

	if (timezone.substr(0, 5) == _T("TOKYO"))
		return true;

	return false;
}

// ================================================================
// ���{��� Windows �𔻒肷��
// ----------------------------------------------------------------
// kernel32.dll �o�[�W�������́u����v�A
// �V�X�e���̋K�茾��A�^�C���]�[���̂R�̏�񂩂�A
// ���{��ł��ǂ������`�F�b�N���܂��B
// ----------------------------------------------------------------
bool tnm_check_japanese_edition_windows()
{
#if 1
	// kernel32.dll �o�[�W�������́u����v�𒲂ׂ�
	if (!tnm_check_kernel32_language())	{
		TSTR message = TSTR() +
			_T("This Game is Japan Only\n\n")
			_T("���̃Q�[���͓��{�����ł̂݃v���C�\�ł��B\n\n")
			_T("���삳����ɂ� �w ���{���Windows �x ���K�v�ł��B\n\n")
			_T("�Q�[�����I�����܂��B\n")
#if 0
			+ _T("\n") + get_last_error_msg()
#endif
			;
		message_box(message, _T("���{���Windows����"), MB_OK | MB_ICONWARNING);
		return false;
	}

	// �V�X�e���̋K�茾��𒲂ׂ�
	if (!tnm_check_system_language())	{
		TSTR language_str = tnm_get_system_language_str();
		TSTR message = str_format(
			_T("This Game is Japan Only\n\n")
			_T("���̃Q�[���͓��{�����ł̂݃v���C�\�ł��B\n\n")
			_T("���삳����ɂ� �w ���{���Windows �x ���K�v�ł��B\n\n")
			_T("�R���s���[�^�̌��݂̌���ݒ�� �w %s �x �ł��B\n\n")
			_T("����� �w ���{�� �x �ɐݒ肵�ĉ������B\n\n")
			_T("�Q�[�����I�����܂��B\n")
			, language_str.c_str());
		message_box(message, _T("���{���Windows����"), MB_OK | MB_ICONWARNING);
		return false;
	}

	// �^�C���]�[���𒲂ׂ�
	if (!tnm_check_timezone())	{
		TSTR timezone_str = tnm_get_timezone_str();
		TSTR message = str_format(
			_T("This Game is Japan Only\n\n")
			_T("���̃Q�[���͓��{�����ł̂݃v���C�\�ł��B\n\n")
			_T("���삳����ɂ� �w ���{���Windows �x ���K�v�ł��B\n\n")
			_T("�R���s���[�^�̌��݂̃^�C���]�[���ݒ�� �w %s �x �ł��B\n\n")
			_T("�^�C���]�[���� �w ���{�i���A�D�y�A�����j �x �ɐݒ肵�ĉ������B\n\n")
			_T("�Q�[�����I�����܂��B\n")
			, timezone_str.c_str());
		message_box(message, _T("���{���Windows����"), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;

#else

	return true;

#endif
}

// ****************************************************************
// �G���W���F�f�B���N�g����������
// ================================================================
bool tnm_init_dir()
{
	if (Gp_reg_ini->reg_name.empty())
	{
		// ���W�X�g�����Ȃ��ꍇ

		// �x��
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�b�g�A�b�v����Ă��܂���B"));

		// �Z�b�g�A�b�v�^�C�v�F�Z�b�g�A�b�v����Ă��Ȃ�
		Gp_global->setup_type = TNM_SETUP_TYPE_NONE;

		// �f�t�H���g�̃t�H���_���w��
		Gp_dir->exe_dir = G_app.module_dir;						// SiglusEngine.exe ��A�C�R���͂�����
		Gp_dir->base_dir = G_app.module_dir;					// g00 �t�H���_�Ȃǂ͂�����
		Gp_dir->work_dir = G_app.module_dir;					// ��ƃt�H���_�̃f�t�H���g�� exe �̈ʒu
		if (Gp_cmd_line->work_dir_set)							// ��ƃt�H���_�̓R�}���h���C���Ŏw��ł���
		{
			if (file_get_type(Gp_cmd_line->work_dir) != 0)		// ��ƃt�H���_�`�F�b�N
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("��ƃt�H���_��������܂���B"));
			}
			else
			{
				Gp_dir->work_dir = Gp_cmd_line->work_dir;
			}
		}
	}
	else 
	{
		// ���W�X�g��������ꍇ
		TSTR reg_path = _T("SoftWare\\") + Gp_reg_ini->reg_name;
		TSTR exe_dir, work_dir, dat_dir, dat_setup;

		// ���W�X�g����T��
		if (!reg_check_key(HKEY_LOCAL_MACHINE, reg_path))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�b�g�A�b�v��񂪌�����܂���B���W�X�g�������Ă��܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
			return false;
		}
		// exe_dir ���擾
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("exe_dir"), exe_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���s�t�@�C���̃p�X���w�肳��Ă��܂���B���W�X�g�������Ă��܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
			return false;
		}
		// �}�C�h�L�������g�̃p�X���擾
		if (!get_my_doument_path(work_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�}�C�h�L�������g�̃p�X���擾�ł��܂���ł����B\n") + get_last_error_msg());
			return false;
		}

		// dat_dir ���擾
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("dat_dir"), dat_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_���w�肳��Ă��܂���B���W�X�g�������Ă��܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
			return false;
		}
		// dat_setup ���擾
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("dat_setup"), dat_setup))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_���w�肳��Ă��܂���B���W�X�g�������Ă��܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
			return false;
		}
		// �t�H���_�`�F�b�N
		if (file_get_type(exe_dir) != 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���s�t�H���_��������܂���B���W�X�g�������Ă��܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
			return false;
		}

		// �ŏ��Z�b�g�A�b�v�̏ꍇ
		if (dat_setup == _T("min"))
		{
			// �Z�b�g�A�b�v�^�C�v�F�ŏ��Z�b�g�A�b�v
			Gp_global->setup_type = TNM_SETUP_TYPE_MIN;

			// �x��
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�ŏ��Z�b�g�A�b�v����Ă��܂��B"));

			// �f�[�^�t�H���_�`�F�b�N
			if (file_get_type(dat_dir) != 0)
			{
				// �f�[�^�t�H���_���Ȃ��ꍇ�A�f�B�X�N�h���C�u����������

				// �f�B�X�N�h���C�u������
				TCHAR disc_letter = tnm_search_disc_drive();
				if (disc_letter != 0)
				{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�F���t�@�C��: ") + Gp_ini->disc_mark);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, TSTR() + disc_letter + _T(":\\ �Ƀf�B�X�N��������܂����B"));
				}
				else
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, Gp_ini->game_name + _T(" �̃Q�[���f�B�X�N��}�����Ă��������B"));
					return false;
				}

				// �h���C�u���^�[���f�B�X�N�ɕύX���Ă݂�
				dat_dir[0] = disc_letter;
				if (file_get_type(dat_dir) != 0)
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�f�[�^�t�H���_��������܂���B�������Q�[���f�B�X�N���}������Ă��邱�Ƃ��m�F���Ă��������B"));
					return false;
				}
				else
				{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�f�[�^�t�H���_���f�B�X�N�ɍĐݒ肵�܂����B"));
				}
			}

			// �f�B���N�g�����w��
			Gp_dir->exe_dir = exe_dir;
			Gp_dir->base_dir = dat_dir;
			Gp_dir->work_dir = work_dir + _T("\\") + Gp_reg_ini->reg_name;
		}

		// ���S�Z�b�g�A�b�v�̏ꍇ
		else
		{
			// �Z�b�g�A�b�v�^�C�v�F���S�Z�b�g�A�b�v
			Gp_global->setup_type = TNM_SETUP_TYPE_FULL;

			// �x��
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���S�Z�b�g�A�b�v����Ă��܂��B"));

			// �f�[�^�t�H���_�`�F�b�N
			if (file_get_type(dat_dir) != 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�f�[�^�t�H���_��������܂���B���W�X�g�������Ă���\��������܂��B\n\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
				return false;
			}

			// �f�B���N�g�����w��
			Gp_dir->exe_dir = exe_dir;
			Gp_dir->base_dir = dat_dir;
			Gp_dir->work_dir = work_dir + _T("\\") + Gp_reg_ini->reg_name;
		}
	}

	// ���[�U�[�t�H���_
	TSTR user_dir = Gp_dir->work_dir;
	if (Gp_cmd_line->multi_user_id_set)
	{
		user_dir += _T("\\") LCL_USERDATA_DIR _T("\\") + Gp_cmd_line->multi_user_id;
	}

	Gp_dir->save_dir = user_dir + _T("\\") + LCL_SAVEDATA_DIR;		// �Z�[�u�t�H���_�̓��[�U�[�t�H���_�̒�
	Gp_dir->debug_log_dir = user_dir + _T("\\__DEBUG_LOG");			// �f�o�b�O���O�t�H���_�̓��[�U�[�t�H���_�̒�

	// �A�y���h�f�B���N�g����T��
	if (Gp_cmd_line->select_scene_pck_set)
	{
		Gp_dir->append_dir = Gp_cmd_line->select_scene_pck;
		Gp_dir->append_name = _T("[dummy]");

		for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
			{
				Gp_dir->append_name = Gp_select_ini->m_append_list[i].m_name;
			}
		}
	}
	else
	{
		Gp_dir->append_dir = Gp_select_ini->first_dir();	// �擪�̃f�B���N�g����T��
		Gp_dir->append_name = Gp_select_ini->first_name();
	}

	return true;
}

// ****************************************************************
// �G���W���F�Z�[�u�t�H���_���쐬����
// ================================================================
bool tnm_create_save_dir()
{
	// �t�H���_���Ȃ���΃t�H���_���쐬����
	if (file_get_type(Gp_dir->save_dir) != 0)	{
		if (!::create_dir_from_dir_path(Gp_dir->save_dir + _T("\\")))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_�̍쐬�Ɏ��s���܂����B\n�Z�[�u�f�[�^�̕ۑ��悪�����ȃh���C�u�ɂȂ��Ă��܂��񂩁H\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B\n") + get_last_error_msg());
			return false;
		}
	}

	// �܂��t�H���_���Ȃ���Ύ��s
	if (file_get_type(Gp_dir->save_dir) != 0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_���쐬����Ă��܂���B\n�Z�[�u�f�[�^�̕ۑ��悪�����ȃh���C�u�ɂȂ��Ă��܂��񂩁H\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"));
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�t�H���_���쐬���܂����B"));

	return true;
}

// ****************************************************************
// �G���W���F�N���������擾����
// ================================================================
void tnm_get_start_time()
{
	// �N���������擾���āA�G���[���O�ɏo�͂��܂��B
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("��") + get_sys_time_str() + _T(" ") + Gp_ini->game_name);
}

// ****************************************************************
// �G���W���F�V�X�e�������擾����
// ================================================================
void tnm_get_system_info()
{
	// �n�r�̃o�[�W�������擾����
	OSVERSIONINFOEX os_version_info = {0};
	os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&os_version_info);

	TSTR os_version_str;
	if (!tnm_get_os_name(os_version_str))	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("OS ��񂪎擾�ł��܂���ł����B\n") + get_last_error_msg());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("OS �o�[�W����=%d.%d (%s)"), os_version_info.dwMajorVersion, os_version_info.dwMinorVersion, os_version_str.c_str()));
	}

	// �������̏����擾����
	MEMORYSTATUS memory = {0};
	memory.dwLength = sizeof(memory);
	::GlobalMemoryStatus(&memory);
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("������=%d MByte"), (int)(memory.dwTotalPhys / 1024 / 1024)));

	// �f�B�X�v���C�̏����擾����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("�f�B�X�v���C�f�o�C�X��������܂����B")));
	for (int i = 0; ; i++)	{
		DISPLAY_DEVICE dd = {0};
		dd.cb = sizeof(DISPLAY_DEVICE);
		if (EnumDisplayDevices(NULL, i, &dd, 0) == FALSE)
			break;

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("- �f�o�C�X%d=%s(%s)"), i, dd.DeviceString, dd.DeviceName));
	}
}

// ****************************************************************
// �G���W���FDirectX �̏����擾
// ================================================================
void tnm_get_directx_info()
{
	TSTR directx_version_str_reg;
	if (::reg_get_value_str(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\DirectX"), _T("Version"), directx_version_str_reg))	{
		TSTR directx_version_str;

		if (false);
		else if (directx_version_str_reg == _T("4.02.0095"))		directx_version_str = _T("1.0");
		else if (directx_version_str_reg == _T("4.03.00.1096"))		directx_version_str = _T("2.0");
		else if (directx_version_str_reg == _T("4.04.0068"))		directx_version_str = _T("3.0");
		else if (directx_version_str_reg == _T("4.04.0069"))		directx_version_str = _T("3.0");
		else if (directx_version_str_reg == _T("4.05.00.0155"))		directx_version_str = _T("5.0");
		else if (directx_version_str_reg == _T("4.05.01.1721"))		directx_version_str = _T("5.0");
		else if (directx_version_str_reg == _T("4.05.01.1998"))		directx_version_str = _T("5.0");
		else if (directx_version_str_reg == _T("4.06.02.0436"))		directx_version_str = _T("6.0");
		else if (directx_version_str_reg == _T("4.07.00.0700"))		directx_version_str = _T("7.0");
		else if (directx_version_str_reg == _T("4.07.00.0716"))		directx_version_str = _T("7.0a");
		else if (directx_version_str_reg == _T("4.08.00.0400"))		directx_version_str = _T("8.0");
		else if (directx_version_str_reg == _T("4.08.01.0881"))		directx_version_str = _T("8.1");
		else if (directx_version_str_reg == _T("4.08.01.0810"))		directx_version_str = _T("8.1");
		else if (directx_version_str_reg == _T("4.09.0000.0900"))	directx_version_str = _T("9.0");
		else if (directx_version_str_reg == _T("4.09.00.0900"))		directx_version_str = _T("9.0");
		else if (directx_version_str_reg == _T("4.09.0000.0901"))	directx_version_str = _T("9.0a");
		else if (directx_version_str_reg == _T("4.09.00.0901"))		directx_version_str = _T("9.0a");
		else if (directx_version_str_reg == _T("4.09.0000.0902"))	directx_version_str = _T("9.0b");
		else if (directx_version_str_reg == _T("4.09.00.0902"))		directx_version_str = _T("9.0b");
		else if (directx_version_str_reg == _T("4.09.00.0904"))		directx_version_str = _T("9.0c or later");
		else if (directx_version_str_reg == _T("4.09.0000.0904"))	directx_version_str = _T("9.0c or later");
		else														directx_version_str = _T("9.0c or later");

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("DirectX �o�[�W����=%s(%s)"), directx_version_str.c_str(), directx_version_str_reg.c_str()));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("DirectX=�s���ȃo�[�W����"));
	}
}

// ****************************************************************
// �G���W���FWindows Media Player �̏����擾
// ================================================================
void tnm_get_wmp_info()
{
	TSTR version;

	TCHAR system_dir_buf[MAX_PATH] = {0};
	if (GetSystemDirectory(system_dir_buf, MAX_PATH) > 0)	{
		TSTR system_dir = system_dir_buf;
		TSTR dll_path = system_dir + _T("\\wmp.dll");
		if (file_get_type(dll_path) == 1)	{
			TSTR version = file_get_product_version(dll_path);
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player �o�[�W����=") + version);
		}
		else	{
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player ��������܂���ł����B"));
		}
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player �o�[�W�������擾�ł��܂���ł����B"));
	}
}

// ****************************************************************
// �G���W���FDirect3D ������
// ================================================================
bool tnm_init_direct_3d()
{
	// Direct3D �����������܂��B
	if (!G_d3d.create())
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �̏������Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �����������܂����B"));

	// �f�B�X�v���C�����擾���܂��B
	int display_cnt = G_d3d->GetAdapterCount();
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �f�B�X�v���C�A�_�v�^��������܂����B"));
	for (int i = 0; i < display_cnt; i++)	{
		D3DADAPTER_IDENTIFIER9 di = {0};

		// �A�C�f���e�B�t�@�[�擾�i�A�_�v�^�[���Ȃǂ̏��j
		G_d3d->GetAdapterIdentifier(i, 0, &di);
		TSTR version = tostr(HIWORD(di.DriverVersion.HighPart)) + _T(".") + tostr(LOWORD(di.DriverVersion.HighPart)) + _T(".") + tostr(HIWORD(di.DriverVersion.LowPart)) + _T(".") + tostr(LOWORD(di.DriverVersion.LowPart));
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("- �A�_�v�^%d=%s version=%s (%s)"), i, MBSTR_to_TSTR(di.Description).c_str(), version.c_str(), MBSTR_to_TSTR(di.DeviceName).c_str()));
		Gp_system_info->display_list.push_back(di);

		// �f�B�X�v���C���[�h�̏����擾���܂��B
		S_tnm_resolution_list resolution_list;
		for (int j = 0; ; j++)	{
			DEVMODE dm = {0};
			dm.dmSize = sizeof(dm);
			if (::EnumDisplaySettingsEx(MBSTR_to_TSTR(di.DeviceName).c_str(), j, &dm, 0) == 0)
				break;

			S_tnm_resolution resolution;
			resolution.resolution.cx = dm.dmPelsWidth;
			resolution.resolution.cy = dm.dmPelsHeight;
			resolution_list.resolution_list.push_back(resolution);
		}

		// ���X�g����ёւ��ē������̂��폜���܂��B
		std::sort(resolution_list.resolution_list.begin(), resolution_list.resolution_list.end());
		resolution_list.resolution_list.erase(std::unique(resolution_list.resolution_list.begin(), resolution_list.resolution_list.end()), resolution_list.resolution_list.end());

		// �f�t�H���g�̉𑜓x�ԍ���������
		resolution_list.default_resolution_no = 0;

		// ���݂̉𑜓x�̔ԍ��𒲂ׂăf�t�H���g�̉𑜓x�ɐݒ�
		DEVMODE dm = {0};
		dm.dmSize = sizeof(DEVMODE);
		EnumDisplaySettingsEx(MBSTR_to_TSTR(di.DeviceName).c_str(), ENUM_CURRENT_SETTINGS, &dm, EDS_RAWMODE);
		for (int j = 0; j < (int)resolution_list.resolution_list.size(); j++)	{
			if (resolution_list.resolution_list[j].resolution == C_size(dm.dmPelsWidth, dm.dmPelsHeight))	{
				resolution_list.default_resolution_no = j;
				break;
			}
		}

		// ���̏������X�g�ɒǉ�
		Gp_system_info->resolution_table.push_back(resolution_list);
	}

	// ���ۂ̒l�ɔ��f����
	Gp_global->fullscreen_display_no = 0;
	Gp_global->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_global->fullscreen_display_no].default_resolution_no;

	// �E�B���h�E�T�C�Y��␳����i��L Direct3D �����������ŉ𑜓x���擾���Ă���s���K�v������j
	tnm_rep_screen_size(Gp_global->screen_size_mode, &Gp_global->screen_size_scale, &Gp_global->screen_size_free);
	Gp_global->total_screen_size.cx = Gp_global->game_screen_size.cx * Gp_global->screen_size_scale.cx / 100;
	Gp_global->total_screen_size.cy = Gp_global->game_screen_size.cy * Gp_global->screen_size_scale.cy / 100;

	return true;
}

// ****************************************************************
// �G���W���FDirect3D �f�o�C�X������
// ================================================================
bool tnm_init_direct_3d_device()
{
	// Direct3D �̃f�o�C�X���쐬���܂��B
	int present_interval = Gp_global->wait_display_vsync_def ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	C_size size = Gp_ini->screen_size;
	int shadow_map_size = 0;
	int adapter_no = Gp_system_config->d3d_adapter_no;
	DWORD vertex_process_type = Gp_system_config->d3d_hardware_vertex_flag ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	bool mipmap_use_flag = Gp_system_config->d3d_use_mipmap_flag;
	bool pow2_limit_flag = false;	// 2^n �ɐ�������t���O�B�f�t�H���g�� false �Ő������Ȃ��B
	if (!G_d3d.device.create(Gp_wnd->get_handle(), true, size.cx, size.cy, shadow_map_size, adapter_no, vertex_process_type, present_interval, mipmap_use_flag, pow2_limit_flag))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �̃f�o�C�X�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �f�o�C�X�̏����擾
	const C_d3d_device_state& state = G_d3d.device.state();
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �̃f�o�C�X���쐬���܂����B"));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �r�f�I������=") + tostr((double)state.vm_size / 1024 / 1024) + _T("MByte"));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ���_����=") + get_str_d3d_vertex_process_type(state.vertex_process_type));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ���_�V�F�[�_�o�[�W����=") + tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.VertexShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.VertexShaderVersion)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ���_�V�F�[�_�̒萔ڼ޽���=") + tostr(state.device_caps.MaxVertexShaderConst));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �s�N�Z���V�F�[�_�o�[�W����=") + tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.PixelShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.PixelShaderVersion)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �ő�e�N�X�`���T�C�Y=") + tostr(C_size(state.device_caps.MaxTextureWidth, state.device_caps.MaxTextureHeight)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 2�̗ݏ�ȊO�̃e�N�X�`��=") + TSTR((state.device_caps.TextureCaps & D3DPTEXTURECAPS_POW2) ? _T("No") : _T("Yes")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ø������گ�ݸ� BORDER=") + TSTR((state.device_caps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) ? _T("Yes") : _T("No")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- Я��ϯ�߂̎�������=") + TSTR((state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) ? _T("Yes") : _T("No")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- Я��ϯ�߂̎�������(X8R8G8B8 ø���)=") + get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- Я��ϯ�߂̎�������(A8R8G8B8 ø���)=") + get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ̨����̌���(X8R8G8B8 ø���)=") + get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ̨����̌���(A8R8G8B8 ø���)=") + get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture));

	// Direct3D �̃X�e�[�g�����������܂��B
	if (!G_d3d.state.init())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �̃X�e�[�g�̏������Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// Direct3D �̃X�e�[�g�������ݒ���s���܂��B
	if (!G_d3d.state.set_template())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �̃X�e�[�g�̏����ݒ�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �X�e�[�g�̏������ɐ������܂����B"));

	// Direct3D �̃r���[�����������܂��B
	if (!Gp_wnd->m_view.init())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �̃r���[�̏������Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �r���[�̏������ɐ������܂����B"));

	// Direct3D �G�t�F�N�g���쐬���܂��B

#if 0
	if (!Gp_wnd->m_view.m_default_effect.create_default_effect())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �G�t�F�N�g�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	else	{
		Gp_global->shader_from_source = true;
	}
#else
	if (!Gp_wnd->m_view.m_default_effect.load_effect_from_resource((LPCTSTR)IDR_EFFECT1, _T("EFFECT")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �G�t�F�N�g�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
#endif

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D �G�t�F�N�g���쐬���܂����B"));

	// �r���[�����ɓh��Ԃ��܂��B�i���ꂪ�Ȃ��ƁAGeForce �n�ŃS�~�`�悪�s���Ă��܂��j
	Gp_wnd->m_view.clear_render_target();

	//// �����_���[���쐬���܂��B
	//if (!Gp_wnd->m_renderer.init())	{
	//	tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �����_���[�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
	//	return false;
	//}

	// ���_�o�b�t�@���쐬���܂��B
	if (!tnm_create_vertex_buffer())
		return false;

	// ���C�v�p�̃o�b�t�@���쐬���܂��B
	if (!tnm_create_wipe_buffer())
		return false;

	// �f�o�b�O�e�L�X�g���쐬���܂��B
	if (!G_debug_text.create(false))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D �e�L�X�g�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	return true;
}

// ****************************************************************
// �G���W���F���_�o�b�t�@���쐬
// ================================================================
bool tnm_create_vertex_buffer()
{
	// ��`�X�v���C�g�p�̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬���܂��B
	// ���̃o�b�t�@�͑S�Ă̂Q�c�I�u�W�F�N�g�Ŏg���܂킵�܂��B

	// ���_�o�b�t�@�����܂��B
	// �����ł͑傫�� 32 �ō쐬���܂��B�`��̍ۂɕK�v�ȕ������g�����܂��B
	// ���t���[������������̂ŁA���I�o�b�t�@���g���܂��B
	G_rect_vertex_buffer_d2_filter = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_filter->create_vertex_buffer(32, S_vtx_d2_filter::FVF, sizeof(S_vtx_d2_filter), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_filter = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_filter->create_vertex_buffer(32, S_vtx_d3_filter::FVF, sizeof(S_vtx_d3_filter), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_pct = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_pct->create_vertex_buffer(32, S_vtx_d2_pct::FVF, sizeof(S_vtx_d2_pct), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_pct_mask = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_pct_mask->create_vertex_buffer(32, S_vtx_d2_pct_mask::FVF, sizeof(S_vtx_d2_pct_mask), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_pct = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_pct->create_vertex_buffer(32, S_vtx_d3_pct::FVF, sizeof(S_vtx_d3_pct), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_screen = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_screen->create_vertex_buffer(32, S_vtx_d2_screen::FVF, sizeof(S_vtx_d2_screen), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_screen = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_screen->create_vertex_buffer(32, S_vtx_d3_screen::FVF, sizeof(S_vtx_d3_screen), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_mask_wipe = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_mask_wipe->create_vertex_buffer(32, S_vtx_d2_mask_wipe::FVF, sizeof(S_vtx_d2_mask_wipe), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�����܂��B
	// �����ł̓X�v���C�g 256 �������쐬���܂��B�`��̍ۂɕK�v�ȕ������g�����܂��B
	// ���e������������̂͊g�����݂̂Ȃ̂ŁA�ÓI�o�b�t�@���g���܂��B
	G_rect_index_buffer = BSP<C_d3d_index_buffer>(new C_d3d_index_buffer);
	if (!tnm_update_rect_index_buffer(256))
		return false;

	return true;
}

// ****************************************************************
// �G���W���F���C�v�o�b�t�@���쐬
// ================================================================
bool tnm_create_wipe_buffer()
{
	C_size size = Gp_global->game_screen_size;

	// �����_�[�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬����
	// �e�N�X�`���̓����_�[�^�[�Q�b�g�ɂ��邽�߁AD3DUSAGE_RENDERTARGET ���w��
	// D3DUSAGE_RENDERTARGET �ɂ���ɂ́AD3DPOOL_DEFAULT ���w�肷��K�v������
	// D3DPOOL_DEFAULT ���w�肵���ꍇ�́A�f�o�C�X�����������玩���ō쐬���Ȃ����K�v�����邽�߁A���\�[�X�}�l�[�W���𗘗p����
	// �J���[�t�H�[�}�b�g�́A�f�o�C�X�̍쐬���ɒ��ׂ� D3DFMT_A8R8G8B8 �ɂ��邱�ƁB�ڂ����̓f�o�C�X�̍쐬�Q�ƁB
	Gp_wnd->m_wipe_back_buffer_texture_0 = G_rsm.create_texture(_T("ܲ���ޯ̧0"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!Gp_wnd->m_wipe_back_buffer_texture_0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���C�v�p�̃e�N�X�`���O�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �[�x�X�e���V���o�b�t�@���쐬����
	// �f�o�C�X�����������ꍇ�A�[�x�X�e���V���o�b�t�@�͕K����蒼���K�v�����邽�߁A���\�[�X�}�l�[�W���𗘗p����
	Gp_wnd->m_wipe_dpst_buffer_surface_0 = G_rsm.create_dpst_buffer(_T("ܲ���ޯ̧"), size.cx, size.cy, true);
	if (!Gp_wnd->m_wipe_dpst_buffer_surface_0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ���C�v�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// ���C�v�o�b�t�@���Q�ȏ���ꍇ
	if (Gp_global->wipe_buffer_cnt >= 2)	{

		// �����_�[�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬����
		Gp_wnd->m_wipe_back_buffer_texture_1 = G_rsm.create_texture(_T("ܲ���ޯ̧1"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
		if (!Gp_wnd->m_wipe_back_buffer_texture_1)	{
			Gp_global->wipe_buffer_cnt = 1;
			tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T(" - �Q���ڂ̃��C�v�o�b�t�@�����܂���ł����B�ꕔ���C�v���ȈՕ\���ɂȂ�܂��B\n�i���̌x���̓f�o�b�O���ɂ����\������܂���B�j\n") + get_last_error_msg());
		}
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D ���C�v�p�̃o�b�t�@���쐬���܂����B"));

	return true;
}

// ****************************************************************
// �G���W���FDirectSound ������
// ================================================================
BOOL CALLBACK DSEnumCallback(LPGUID p_guid, LPCWSTR p_description, LPCWSTR p_module, LPVOID p_context)
{
	if (p_guid == NULL)	{	// �ŏ��ɗ񋓂����̂̓v���C�}���f�o�C�X�ŁAguid �� NULL �ł���
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���p�\�ȃT�E���h�f�o�C�X��������܂����B"));
	}
	else	{				// �Q��ڈȍ~�ɌĂяo�����̂����݂̃f�o�C�X
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ") + TSTR(p_description));
	}

	return TRUE;
}

bool tnm_init_direct_sound()
{
	HRESULT hr;

	// �T�E���h�h���C�o��񋓂���
	hr = DirectSoundEnumerate(DSEnumCallback, NULL);
	if (FAILED(hr))
	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���p�\�ȃT�E���h�f�o�C�X��������܂���ł����B"));
		// ���� DirectSound �̏������ŃG���[���o��Ǝv�����A�����͂������̂ŁA�����𑱍s����
	}

	// DirectSound �����������܂��B
	if (!G_ds.init(G_app.h_wnd))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("DirectSound �̏������Ɏ��s���܂����B\n�X�s�[�J�[�������̓w�b�h�z�����ڑ�����Ă��Ȃ����A�T�E���h�h���C�o�����Ă���\��������܂��B\n\n��G���[�ڍׁ�\n") + get_last_error_msg());
		return false;
	}

	// �v���C�}�l�[�W�������������܂��B
	G_pm.init();

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("DirectSound �̏������ɐ������܂����B"));

	return true;
}

// ****************************************************************
// �G���W���F�������
// ================================================================
//		�E�B���h�E�������钼�O�̏����ł��B
//		�E�B���h�E�Ɋ֘A���Ă�����̂́A�K�������ŉ�����Ă��������B
//		Direct3D �͂������ADirect3D �X�v���C�g�������̂�������Ȃ��Ƃ����܂���B�i���������j
// ================================================================
//		DirectSound ��������܂��B
//		�f�o�b�O�E�B���h�E��������܂��B
// ================================================================
bool C_tnm_eng::free()
{
	if (!free_global())
		return false;

	return true;
}

bool C_tnm_eng::free_global()
{
	// �����������������ꍇ�݂̂̍��
	// �������G���[���ɂ�����s���Ă��܂��ƁA�O���[�o���f�[�^�Ȃǂ��S�Ĕ��ł��܂��I
	if (m_init_success_flag)	{

		// �Ō�ɂ����P�x�^�C�}�[���X�V����
		timer_proc();

		// �O���[�o���f�[�^���Z�[�u���܂��B
		tnm_save_global_on_file();

		// �f�o�b�O�E�B���h�E�̏���ۑ����܂��B
		Gp_dlg_mng->save_initial_state();
	}

	// �f�o�b�O�E�B���h�E��������܂��B
	Gp_dlg_mng->free();

	// ���C���E�B���h�E���\���ɂ��܂��B
	Gp_wnd->set_show(false);

	// �E�B���h�E���烁�j���[���f�^�b�`���Ĕj�����܂��B
	Gp_wnd->set_menu(NULL);
	Gp_wnd->m_wnd_menu.destroy();

	// �E�B���h�E����A�C�R�����f�^�b�`���܂��B
	Gp_wnd->set_icon(NULL);
	Gp_wnd->set_small_icon(NULL);

	// �T�E���h��������܂��B
	m_sound.free();

	// �v���C���[�}�l�[�W����������܂��B
	G_pm.release();

	// DirectSound ��������܂��B
	G_ds.release();

	// �f�o�b�O�e�L�X�g��������܂��B
	G_debug_text.release();

	// �����_���[��������܂��B
	//Gp_wnd->m_renderer.release();

	// Direct3D �f�o�C�X��������܂��B
	G_d3d.device.release();

	// �t�H���g��������܂��B
	m_font.release();

	// Steam
#if __USE_STEAM
	SteamAPI_Shutdown();
#endif

	// �ēx�t���O�𗧂Ă�i�I������p�j
	Gp_global->game_end_flag = true;

	// �������
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�S�Ẳ���������������܂����B"));

	return true;
}

// ****************************************************************
// �G���W���F�I������
// ================================================================
//		�A�v���P�[�V�����I�����O�̏����ł��B
//		���̎��_�ŃE�B���h�E�͕����Ă��邱�Ƃɒ��ӂ��Ă��������B
// ================================================================
bool C_tnm_eng::end()
{
	// Direct3D ��������܂��B
	G_d3d.release();

	// �A�C�R����������܂��B
	Gp_icon->destroy_icon();

	// �X�^�b�N��������܂��B
	if (m_stack.int_p)
	{
		::HeapFree(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_stack.int_p);
	}

	// window.ini �̓ǂݎ���p���O���܂��B
	::file_set_read_only(Gp_dir->get_window_ini_file_path(), false);

	// window.ini ��ۑ����܂��B
	C_ini_file::body().save_ini_file();

	// �f�o�b�O dll ��������܂��B
	m_debug_dll.free();

	return true;
}

// ****************************************************************
// �G���W���F�J�n����
// ================================================================
bool C_tnm_eng::start()
{
	// �X�^�[�g�V�[������n�߂�
	tnm_scene_proc_start_from_start_scene();

	// ���Q�[���^�C�}�[�J�n
	m_global.game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// �G���W���F�����������F���[�J��
// ================================================================
//		�Q�[���N�����ɂP�񂾂��Ă΂�܂��B
//		�Q��ڈȍ~�� reinit ���Ă΂�܂��B
// ================================================================

void C_tnm_local_data_pod::init()
{
	// pod
	cur_chr_no = -1;
	cur_koe_no = -1;
	cur_read_flag_scn_no = -1;
	cur_read_flag_flag_no = -1;
	cursor_no = Gp_ini->mouse_cursor_default;
	syscom_menu_disable = false;
	hide_mwnd_disable = false;
	msg_back_disable = false;
	shortcut_disable = false;
	skip_disable = false;
	ctrl_disable = false;
	not_stop_skip_by_click = false;
	not_skip_msg_by_click = false;
	skip_unread_message = false;
	auto_mode_flag = false;
	auto_mode_moji_wait = -1;
	auto_mode_min_wait = -1;
	auto_mode_moji_cnt = 0;
	mouse_cursor_hide_onoff = -1;
	mouse_cursor_hide_time = -1;
	msg_back_save_cntr = 0;
	msg_speed = -1;
	msg_nowait = false;
	async_msg_mode = false;
	async_msg_mode_once = false;
	multi_msg_mode = false;
	skip_trigger = false;
	koe_dont_stop_on_flag = false;
	koe_dont_stop_off_flag = false;
	mwnd_btn_disable_all = false;
	mwnd_btn_touch_disable = false;
	mwnd_anime_on_flag = false;
	mwnd_anime_off_flag = false;
	mwnd_disp_off_flag = false;
	msg_back_off = false;
	msg_back_disp_off = false;
	font_futoku = -1;
	font_shadow = -1;
	cursor_disp_off = false;
	cursor_move_by_key_disable = false;
	for (int i = 0; i < 256; i++)	{
		key_disable[i] = false;
	}
	quake_stop_flag = false;
	emote_mouth_stop_flag = false;
	cg_table_off_flag = false;
	bgmfade_flag = false;
	dont_set_save_point = false;
	ignore_r_flag = false;
	wait_display_vsync_off_flag = false;
	time_stop_flag = false;
	counter_time_stop_flag = false;
	frame_action_time_stop_flag = false;
	stage_time_stop_flag = false;
}

void C_tnm_local_data::init()
{
	cur_proc = C_tnm_proc(TNM_PROC_TYPE_NONE);
	proc_stack.clear();
	cur_mwnd = S_element(ELM_GLOBAL_FRONT, ELM_STAGE_MWND, ELM_ARRAY, Gp_ini->mwnd.default_mwnd_no);
	cur_sel_mwnd = S_element(ELM_GLOBAL_FRONT, ELM_STAGE_MWND, ELM_ARRAY, Gp_ini->mwnd.default_sel_mwnd_no);
	last_mwnd.clear();
	scene_title.clear();
	cur_full_message.clear();

	// ���b�Z�[�W�E�B���h�E
	mwnd_btn_disable.resize(Gp_ini->mwnd_waku_btn_cnt);
	for (int i = 0; i < Gp_ini->mwnd_waku_btn_cnt; i++)	{
		mwnd_btn_disable[i].value = false;
	}

	// �t�H���g
	font_name.clear();

	// pod
	pod.init();
}

bool C_tnm_eng::init_local()
{
	// �Z�[�u�����������܂��B
	m_local_save_history_index = -1;
	m_local_save.save_id = S_tid();
	m_local_save.append_dir.clear();
	m_local_save.append_name.clear();
	m_local_save.save_scene_title.clear();
	m_local_save.save_msg.clear();
	m_local_save.save_full_msg.clear();
	m_local_save.save_stream.clear();
	m_local_save.save_stream_ex.clear();

	m_sel_save.list.clear();
	m_sel_save.map.clear();

	m_sel_save_stock.save_id = S_tid();
	m_sel_save_stock.append_dir.clear();
	m_sel_save_stock.append_name.clear();
	m_sel_save_stock.save_scene_title.clear();
	m_sel_save_stock.save_msg.clear();
	m_sel_save_stock.save_full_msg.clear();
	m_sel_save_stock.save_stream.clear();
	m_sel_save_stock.save_stream_ex.clear();

	m_inner_save_list.clear();
	m_inner_save_list.resize(Gp_ini->inner_save_cnt);

	// �{�^���}�l�[�W�������������܂��B
	m_btn_mng.init();

	// �V�X�e���f�[�^�����������܂��B
	m_global.ex_call_flag = false;
	m_global.ctrl_skip_flag = false;
	m_global.cs_skip_flag = false;
	m_global.read_skip_enable_flag = false;
	m_global.is_auto_mode_count_start = false;
	m_global.auto_mode_count_start_time = 0;
	m_global.bgmfade_flag = false;
	m_global.bgmfade_cur_time = 0;
	m_global.bgmfade_start_value = 255;
	m_global.bgmfade_total_volume = 255;
	m_global.bgmfade2_flag = false;
	m_global.bgmfade2_need_flag = false;
	m_global.bgmfade2_cur_time = 0;
	m_global.bgmfade2_start_value = 255;
	m_global.bgmfade2_total_volume = 255;
	m_global.system_wipe_flag = false;
	m_global.skip_because_skip_trigger = false;

	// �V�X�e���R�}���h���j���[�����������܂��B
	tnm_syscom_init_syscom_flag();

	// �V�[���f�[�^�����������܂��B
	m_local.init();

	// �X�^�b�N�����������܂��B
	m_stack.int_now_cnt = 0;
	m_stack.stack_point_list.clear();

	// �^�C�}�[�����������܂��B
	m_timer.local_real_time = 0;
	m_timer.local_real_time_past = 0;
	m_timer.local_real_time_amari = 0;
	m_timer.local_game_time = 0;
	m_timer.local_game_time_past = 0;
	m_timer.local_game_time_amari = 0;
	m_timer.local_wipe_time = 0;
	m_timer.local_wipe_time_past = 0;
	m_timer.local_wipe_time_amari = 0;

	// �}�E�X�J�[�\����ύX���܂��B
	tnm_cursor_frame(m_timer.local_real_time);

	// �t�F�b�`�|�C���g�����������܂��B
	tnm_init_fetch_point();

	// �t�H�O�����������܂��B
	m_fog.init_local();

	// ================================================================
	// �G�������g�̏�����
	// ----------------------------------------------------------------

	// �t���O�����������܂��B
	m_flag.init_local();

	// ���[�U�C���N�v���p�e�B�����������܂��B
	m_user_inc_prop.init();

	// ���[�U�V�[���v���p�e�B���X�g�����������܂��B
	m_user_scn_prop_list.init(S_element(), FM_INTLIST, _T("user_prop"), Gp_lexer->get_scn_cnt(), false, NULL);

	// �J�E���^���X�g���쐬���܂��B
	m_counter_list.init(S_element(ELM_GLOBAL_COUNTER), FM_COUNTERLIST, _T("counter"), Gp_ini->counter_cnt, false, NULL);

	// �t���[���A�N�V���������������܂��B
	m_frame_action.init(S_element(ELM_GLOBAL_FRAME_ACTION), _T("frame_action"), S_element());

	// �t���[���A�N�V�����`�����l�������������܂��B
	m_frame_action_ch_list.init(S_element(ELM_GLOBAL_FRAME_ACTION_CH), _T("frame_action_ch"), Gp_ini->frame_action_ch_cnt, false, S_element());

	// �f�O�O�o�b�t�@�����������܂��B
	m_g00_buf_list.init(S_element(ELM_GLOBAL_G00BUF), FM_G00BUFLIST, _T("g00buf"), Gp_ini->g00_buf_cnt, false, NULL);

	// �}�X�N�����������܂��B
	m_mask_list.init(S_element(ELM_GLOBAL_MASK), FM_MASKLIST, _T("mask"), Gp_ini->mask_cnt, false, NULL);

	// �X�e�[�W�����������܂��B
	m_stage_list.init(S_element(ELM_GLOBAL_STAGE), _T("stage"), _T(""), TNM_STAGE_CNT, false, true, NULL);

	// ���b�Z�[�W�o�b�N�����������܂��B
	m_msg_back.init();

	// �X�N���[�������������܂��B
	m_screen.init(S_element(ELM_GLOBAL_SCREEN), _T("screen"));

	// ���C�v�����������܂��B
	m_wipe.init();

	// �T�E���h���ď��������܂��B
	m_sound.reinit();

	// ���ʉ��C�x���g�����������܂��B
	m_pcm_event_list.init(S_element(ELM_GLOBAL_PCMEVENT), FM_PCMEVENTLIST, _T("pcmevent"), Gp_ini->pcm_event_cnt, false, NULL);

	// �G�f�B�b�g�{�b�N�X�����������܂��B
	m_editbox_list.init(S_element(ELM_GLOBAL_EDITBOX), FM_EDITBOXLIST, _T("editbox"), Gp_ini->editbox_cnt, false, NULL);

	// �R�[�������������܂��B
	m_call_list.init(S_element(ELM_GLOBAL_CALL), FM_CALLLIST, _T("call"), 1, true, NULL);		// �����R�[�����͂P

	// �d�w�R�[�������������܂��B
	m_excall.init(S_element(ELM_GLOBAL_EXCALL), _T("excall"));

	// �L���v�`���[�����������܂��B
	m_global.capture_prior = TNM_CAPTURE_PRIOR_NONE;

	// �������������b�Z�[�W
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�J���f�[�^�����������܂����B"));

	return true;
}

// ****************************************************************
// �G���W���F���[�J���ď�����
// ================================================================
//		�u�ŏ�����n�߂�v�Ȃǂ̎��ɌĂяo����܂��B
//		�e��G�������g�̏������Ȃǂ��s���܂��B
// ================================================================
bool C_tnm_eng::reinit_local(bool restruct_flag)
{
	// �Z�[�u�����������܂��B
	m_local_save_history_index = -1;
	m_local_save.save_id = S_tid();
	m_local_save.append_dir.clear();
	m_local_save.append_name.clear();
	m_local_save.save_scene_title.clear();
	m_local_save.save_msg.clear();
	m_local_save.save_full_msg.clear();
	m_local_save.save_stream.clear();
	m_local_save.save_stream_ex.clear();

	// �č\�z����ꍇ�̂ݑI�����Z�[�u�͏���
	if (restruct_flag)	{
		m_sel_save.list.clear();
		m_sel_save.map.clear();
	}

	m_sel_save_stock.save_scene_title.clear();
	m_sel_save_stock.save_msg.clear();
	m_sel_save_stock.save_full_msg.clear();
	m_sel_save_stock.save_stream.clear();
	m_sel_save_stock.save_stream_ex.clear();

	m_inner_save_list.clear();
	m_inner_save_list.resize(Gp_ini->inner_save_cnt);

	// �{�^���}�l�[�W�������������܂��B
	m_btn_mng.init();

	// �V�X�e���f�[�^�����������܂��B
	m_global.ex_call_flag = false;
	m_global.ctrl_skip_flag = false;
	m_global.cs_skip_flag = false;
	m_global.read_skip_enable_flag = false;
	m_global.is_auto_mode_count_start = false;
	m_global.auto_mode_count_start_time = 0;
	m_global.bgmfade_flag = false;
	m_global.bgmfade_cur_time = 0;
	m_global.bgmfade_start_value = 255;
	m_global.bgmfade_total_volume = 255;
	m_global.bgmfade2_flag = false;
	m_global.bgmfade2_need_flag = false;
	m_global.bgmfade2_cur_time = 0;
	m_global.bgmfade2_start_value = 255;
	m_global.bgmfade2_total_volume = 255;
	m_global.system_wipe_flag = false;
	m_global.skip_because_skip_trigger = false;

	// �V�X�e���R�}���h���j���[�����������܂��B
	tnm_syscom_init_syscom_flag();

	// �V�[���f�[�^�����������܂��B
	m_local.init();

	// �X�^�b�N�����������܂��B
	m_stack.int_now_cnt = 0;
	m_stack.stack_point_list.clear();

	// �^�C�}�[�����������܂��B
	m_timer.local_real_time = 0;
	m_timer.local_real_time_past = 0;
	m_timer.local_real_time_amari = 0;
	m_timer.local_game_time = 0;
	m_timer.local_game_time_past = 0;
	m_timer.local_game_time_amari = 0;
	m_timer.local_wipe_time = 0;
	m_timer.local_wipe_time_past = 0;
	m_timer.local_wipe_time_amari = 0;

	// �}�E�X�J�[�\����ύX���܂��B
	tnm_cursor_frame(m_timer.local_real_time);

	// �t�F�b�`�|�C���g�����������܂��B
	tnm_init_fetch_point();

	// �t�H�O�����������܂��B
	m_fog.init_local();

	// ================================================================
	// �G�������g�̏�����
	// ----------------------------------------------------------------

	// �t���O�����������܂��B
	m_flag.init_local();

	// ���[�U�C���N�v���p�e�B�����������܂��B
	m_user_inc_prop.reinit();

	// ���[�U�V�[���v���p�e�B���X�g�����������܂��B
	m_user_scn_prop_list.reinit();

	// �J�E���^���X�g���쐬���܂��B
	m_counter_list.reinit();

	// �t���[���A�N�V���������������܂��B
	m_frame_action.reinit(false);			// ����� finish �t���O

	// �t���[���A�N�V�����`�����l�������������܂��B
	m_frame_action_ch_list.reinit(false, restruct_flag);

	// �f�O�O�o�b�t�@�����������܂��B
	m_g00_buf_list.reinit();

	// �}�X�N�����������܂��B
	m_mask_list.reinit();

	// �X�e�[�W�����������܂��B
	m_stage_list.reinit(restruct_flag);

	// ���b�Z�[�W�o�b�N�����������܂��B
	if (!Gp_global->reinit_msgbk_except_flag)
	{
		m_msg_back.init();
	}

	// �X�N���[�������������܂��B
	m_screen.reinit();

	// ���C�v�����������܂��B
	m_wipe.init();

	// �T�E���h���ď��������܂��B
	m_sound.reinit();

	// ���ʉ��C�x���g�����������܂��B
	m_pcm_event_list.reinit();

	// �G�f�B�b�g�{�b�N�X�����������܂��B
	m_editbox_list.reinit();

	// �R�[�������������܂��B
	m_call_list.reinit();

	// �d�w�R�[�������������܂��B
	m_excall.reinit(false, restruct_flag);

	// �����}�l�[�W�����N���A���܂��B
	tnm_update_font(true);

	// �L���v�`���[�����������܂��B
	Gp_wnd->m_capture_texture_for_save_thumb.reset();
	Gp_wnd->m_capture_texture_for_object.reset();
	Gp_wnd->m_capture_texture_for_tweet.reset();
	Gp_global->capture_prior = TNM_CAPTURE_PRIOR_NONE;

	// �������������b�Z�[�W
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�J���f�[�^���ď��������܂����B"));

	return true;
}

// ****************************************************************
// �G���W���F�����������F�I��
// ================================================================
//		���G�������g�̏I�������ł��I
// ================================================================
bool C_tnm_eng::finish_local()
{
	// �t���[���A�N�V����
	m_frame_action.finish();

	// �t���[���A�N�V�����`�����l��
	m_frame_action_ch_list.finish();

	// �X�e�[�W
	m_stage_list.finish();

	// �d�w�R�[��
	m_excall.finish();

	return true;
}
