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
// エンジンの呼び出し
// ================================================================
bool tnm_init_local()
{
	Gp_global->__bgm_stop_cntr = 0;

	// エンジンのローカル初期化
	return Gp_eng->init_local();
}
bool tnm_reinit_local(bool restruct_flag)
{
	Gp_global->__bgm_stop_cntr = 0;

	// エンジンのローカル再初期化
	return Gp_eng->reinit_local(restruct_flag);
}
bool tnm_finish_local()
{
	// エンジンのローカル終了
	return Gp_eng->finish_local();
}
bool tnm_free()
{
	// エンジンの解放
	return Gp_eng->free();
}

// ****************************************************************
// エンジン：プロトタイプ宣言
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
// エンジン：初期化
// ================================================================
//		ゲーム起動時に一度だけ通ります。
// ================================================================
bool C_tnm_eng::init()
{
	// グローバルの初期化処理を行います。
	if (!init_global())
		return false;

	return true;
}

// ****************************************************************
// エンジン：初期化処理：グローバル
// ================================================================
//		ゲーム起動時に一度だけ通ります。
//		メインウィンドウを作成し、表示します。
//		各種ステートの初期化を行います。
// ================================================================
bool C_tnm_eng::init_global()
{
	// エラーを初期化
	m_error.init();

	// エラー表示のためにシーン番号を初期化
	m_lexer.set_cur_scn_no(-1);
	m_lexer.set_cur_line_no(0);

	// タイマーを初期化
	m_timer.real_time = ::timeGetTime();
	m_timer.real_time_past = 0;
	m_timer.global_real_time = 0;

	// グローバルパラメータを初期化
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

	// 起動日時を取得（エラーログの先頭に持ってくるため最初に行います）
	tnm_get_start_time();

	// ディレクトリを設定（これをしないと log.txt が出力できないので優先順位は高いです）
	if (!tnm_init_dir())
		return false;

	// ディレクトリが設定されたので、ログファイルを削除します。
	// エラーログが出力される前に行う必要があるので、タイミングはここ以外ありません。
	if (file_get_type(Gp_dir->get_log_file_path()) == 1)
		filesys_delete_file(Gp_dir->get_log_file_path());

	// セーブフォルダを作成（これで log.txt が作成できるようになります）
	if (!tnm_create_save_dir())
		return false;

	// デバッグ dll を読み込む
	m_debug_dll.load();

	// ようやくデバッグかどうかが確定した
	m_global.debug_decided_flag = true;

	// システム設定データを初期化
	{
		m_system_config.d3d_adapter_no = D3DADAPTER_DEFAULT;
		m_system_config.d3d_hardware_vertex_flag = true;
		m_system_config.d3d_present_interval = 1;
		m_system_config.d3d_use_mipmap_flag = true;
		m_system_config.d3d_wipe_buffer_cnt = 2;
		m_system_config.ds_play_silent_sound_flag = false;
		m_system_config.movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
	}

	// システム設定データを読み込む
	if (!tnm_load_system_on_file())
		return false;

	// システム設定データからシステム値を設定する
	m_global.wait_display_vsync_def = 
		Gp_cmd_line->no_interval_set ? false :					// コマンドラインで「待たない」が設定されている
		m_system_config.d3d_present_interval ? true : false;	// システム設定ファイルに従う
	m_global.wait_display_vsync_total = m_global.wait_display_vsync_def;
	m_global.wipe_buffer_cnt = m_system_config.d3d_wipe_buffer_cnt;

	// 日本語版チェック
#if 0
#if defined(__SIGLUS_JA)
	if (!tnm_check_japanese_edition_windows())
		return false;
#endif
#endif

	// システム情報を取得する
	tnm_get_system_info();

	// DirectX のバージョンを取得する
	tnm_get_directx_info();

	// Flag.ini を読み込む
	if (!m_flag_ini.analize_flag_ini())
		return false;

	// Steam を初期化
#if __USE_STEAM

	if (!SteamAPI_Init())
	{
		if (m_global.debug_flag)
		{
			tnm_set_error(TNM_ERROR_TYPE_INFO, _T("Steam の初期化に失敗しました。\nデバッグモードのため、Steam の機能を使わずに起動します。"));
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Steam の初期化に失敗しました。"));
			return false;
		}
	}

	steam_first_proc();

#endif

	// Direct3D を初期化
	if (!tnm_init_direct_3d())
		return false;

	// アイコンファイルを読み込む
	m_icon.create_icon();

	// フォントを読み込む（デフォルトのアペンドを使うことに注意）
	for (int i = 0; i < Gp_ini->font_file_cnt; i++)
	{
		TSTR file_name = Gp_ini->font_file[i].file_name;
		if (!file_name.empty())
		{
			// ファイルを検索
			TSTR file_path = tnm_find_dat(file_name);
			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("フォントファイル ") + file_name + _T(" が見つかりませんでした。"));
				return false;
			}
			// ファイルからフォントを追加
			int font_num = AddFontResourceEx(file_path.c_str(), FR_PRIVATE, NULL);
			if (font_num == 0)
			{
				set_last_error_win(_T("AddFontResourceEx"));
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("フォントファイル ") + file_name + _T(" の追加に失敗しました。\n") + get_last_error_msg());
			}
		}
	}
	// フォントをプライベートに読み込む
	for (int i = 0; i < Gp_ini->private_font_file_cnt; i++)
	{
		TSTR file_name = Gp_ini->private_font_file[i].file_name;
		if (!file_name.empty())
		{
			// ファイルを検索
			TSTR file_path = tnm_find_dat(file_name);
			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("フォントファイル ") + file_name + _T(" が見つかりませんでした。"));
				return false;
			}
			// ファイルを読み込む
			ARRAY<BYTE> file_data;
			if (!C_file::read_full_data(file_path, file_data))
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("フォントファイル ") + file_name + _T(" が読み込めませんでした。\n") + get_last_error_msg());
				return false;
			}
			// 復号
			tpc_unangou(file_data.get(), file_data.size());
			int size = lzss_unpack(file_data.get(), NULL);
			ARRAY<BYTE> unangou_data(size);
			lzss_unpack(file_data.get(), unangou_data.get());

			// メモリからフォントを追加
			DWORD read_num = 0;
			DESIGNVECTOR design_vector = { 0 };
			HANDLE h_mem_font = AddFontMemResourceEx(unangou_data.get(), unangou_data.size(), NULL, &read_num);
			if (h_mem_font == 0)
			{
				set_last_error_win(_T("AddFontMemResourceEx"));
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("フォントファイル ") + file_name + _T(" の追加に失敗しました。\n") + get_last_error_msg());
				return false;
			}
		}
	}

	// ウィンドウを初期化する
	Gp_wnd->init();

	// ウィンドウ用に window.ini を読み込む
	C_ini_file::body().load_ini_file(Gp_dir->get_window_ini_file_path());

	// Twitter 情報を読み込む
	//twitter_load_state();

	// メインウィンドウを作成する（ここではまだ表示されません。）
	TSTR window_name = tnm_get_window_name();
	if (!Gp_wnd->create(_T("siglus_engine_main_window"), window_name, Gp_ini->game_name, -1, -1, m_global.total_screen_size.cx, m_global.total_screen_size.cy, NULL, NULL, -1, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_THICKFRAME, 0, true, true, NULL))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ウィンドウの作成に失敗しました。"));
		return false;
	}

	// ウィンドウサイズをここで更新
	m_global.total_window_size = Gp_wnd->get_window_rect().size();

	// メニューを作成してウィンドウにアタッチする（デバッグモードのみ）
	if (m_global.debug_flag)
	{
		Gp_wnd->m_wnd_menu.create();
		Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	}

	// ウィンドウサイズを再調整する
	Gp_wnd->set_client_size(m_global.total_screen_size);

	// Direct3D デバイスを初期化
	if (!tnm_init_direct_3d_device())
		return false;

	// DirectSound を初期化
	if (!tnm_init_direct_sound())
		return false;

	// Windows Media Player のバージョンを取得
	tnm_get_wmp_info();

	// theora のテーブルを準備
	C_omv_player::ready_table();

	// E-mote
#if __USE_EMOTE
	tnm_emote_init();
#endif

	// ウィンドウを更新して表示
	Gp_wnd->update();
	Gp_wnd->set_show(true);

	// マウスカーソルを作成（デフォルトのアペンドを使うことに注意）
	m_cursor_list.sub.resize(Gp_ini->mouse_cursor_cnt);
	for (int i = 0; i < Gp_ini->mouse_cursor_cnt; i++)
	{
		m_cursor_list.sub[i].create_cursor(i);
	}

	// スタックを確保
	m_stack.int_now_cnt = 0;
	m_stack.int_alloc_cnt = 1024;
	m_stack.int_p = (int *)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_stack.int_alloc_cnt * sizeof(int));

	// セーブキャッシュを初期化
	m_save_cache.init();

	// セーブ履歴を初期化
	tnm_init_save_history();

	// バックログセーブを初期化
	tnm_init_backlog_save();

	// ユーザコンフィグをフォーマット
	tnm_format_config();

	// ダイアログの情報を設定
	m_global.dialog_fullscreen_change_resolution = m_config.fullscreen_change_resolution;
	m_global.dialog_fullscreen_display_no = m_config.fullscreen_display_no;
	m_global.dialog_fullscreen_resolution_no = m_config.fullscreen_resolution_no;

	// 日本語のみフォントを絞る（それ以外は分からないので全部出します…）
#if defined(__SIGLUS_JA)
	BYTE chara_set = SHIFTJIS_CHARSET;
#else
	BYTE chara_set = ANSI_CHARSET;
#endif

	// フォントリストを作成
	m_font_name_list.make_font_list(Gp_wnd->get_handle(), false, false, chara_set, Gp_ini->tateyoko_mode == 1 ? FONT_YOKOTATE_MODE_TATE_ONLY : FONT_YOKOTATE_MODE_YOKO_ONLY);

	// ↑ここまでグローバル
	// ↓ここからローカル

	// シーンを開く
	if (!m_lexer.load_scn(LCL_SCENE_PCK))
	{
		m_error.set_error(TNM_ERROR_TYPE_FATAL, _T("シーンデータ（") LCL_SCENE_PCK _T("）が開けませんでした。"));
		return false;
	}

	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("シーンデータを読み込みました。"));

	// フラグを初期化
	m_flag.init_global();

	// データベースを初期化
	tnm_database_init();

	// ＣＧテーブルを初期化
	m_cg_table.init();

	// ＢＧＭテーブルを初期化
	m_bgm_table.init();

	// サムネイルテーブルを初期化
	m_thumb_table.create_table();

	// 既読フラグを作成
	m_read_flag_list.resize(m_lexer.get_scn_cnt());
	for (int i = 0; i < (int)m_read_flag_list.size(); i++)
		m_read_flag_list[i].resize(m_lexer.get_read_flag_cnt(i), 0);

	// 【名前】を初期化
	m_namae.init();

	// キャラクター音声を初期化
	if (Gp_ini->config.chrkoe_cnt > 0)	{
		m_chrkoe.resize(Gp_ini->config.chrkoe_cnt);
		for (int i = 0; i < Gp_ini->config.chrkoe_cnt; i++)	{
			m_chrkoe[i].name_str = Gp_ini->config.chrkoe[i].name_str;	// 名前を移植
			switch (Gp_ini->config.chrkoe[i].check_mode)	{				// チェックモード（0=伏字状態でチェックしない  1=公開状態でチェックしない  2=伏字状態でチェックする）
				case TNM_CHRKOE_CHECK_MODE_NONE:		m_chrkoe[i].look_flag = false;	break;
				case TNM_CHRKOE_CHECK_MODE_LOOK:		m_chrkoe[i].look_flag = true;	break;
				case TNM_CHRKOE_CHECK_MODE_AUTO:		m_chrkoe[i].look_flag = false;	break;
				default:								m_chrkoe[i].look_flag = true;	break;
			}
		}
	}

	// カラーテーブルを初期化
	m_color_table.init();

	// トーンカーブを初期化
	m_tonecurve.init();

	// ボタンアクションテンプレートを初期化
	m_btn_action_template_list.init();

	// ボタン効果音テンプレートを初期化
	m_btn_se_template_list.init();

	// サウンドを初期化
	m_sound.init();

	// クリック禁止を初期化
	m_click_disable.init();

	// エレメント初期化終了
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("グローバルなエレメントを初期化しました。"));

	// グローバルデータをロード
	if (!tnm_load_global_on_file())
		return false;

	// コマンドラインから環境設定を上書きする
	if (Gp_cmd_line->fullscreen_mode_set)
		m_config.screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	if (Gp_cmd_line->window_mode_set)
		m_config.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;

	// 環境設定を適用する
	tnm_update_screen_size_mode();

	// ボリュームを更新する
	tnm_update_sound_volume(true);

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("環境設定を再構築しました。"));

	// デバッグウィンドウを登録
	Gp_dlg_mng->regist();

	// デバッグウィンドウを初期化
	Gp_dlg_mng->init();

	// デバッグウィンドウの初期ステータスを読み込む
	Gp_dlg_mng->load_initial_state();

	// デバッグウィンドウの初期オープン
	Gp_dlg_mng->initial_open();

	// メインウィンドウをアクティブにする
	Gp_wnd->set_active();

	// エラー情報
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("━━━━━━━━━━━━━━━━━━━━━"));
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("■アプリケーションの初期化に成功しました。"));
	m_error.set_error(TNM_ERROR_TYPE_SUCCESS, _T("━━━━━━━━━━━━━━━━━━━━━"));

	// 初期化成功
	m_init_success_flag = true;

	// 成功
	return true;
}

// ****************************************************************
// エンジン：日本語チェック
// ================================================================

// ================================================================
//	kernel32.dll バージョン情報の「言語」を調べる
// ----------------------------------------------------------------
//	バージョン情報の「言語コード」が「日本語」であれば「日本語版 Windows」と判定します。
//	Windows の本体とも言うべきＤＬＬファイルである kernel32.dll や、rundll32.exe 、user32.dll など
//	Windows の根幹に関わるファイルなら何でも良いと思います。
// ----------------------------------------------------------------
bool tnm_check_kernel32_language()
{
	// 言語とコードページ
	struct GFV_LANGANDCODEPAGE
	{
		WORD language;
		WORD codepage;
	};

	bool japanese_flag = false;

	// システムディレクトリのパスを取得し、kernel32.dll へのパスを作成
	TCHAR system_dir_path[MAX_PATH];
	if (GetSystemDirectory(system_dir_path, MAX_PATH) == 0)	{
		set_last_error_win(_T("GetSystemDirectory"));
		return false;
	}

	TSTR kernel32_file_path = TSTR() + system_dir_path + _T("\\kernel32.dll");
	if (file_get_type(kernel32_file_path) != 1)	{
		set_last_error(kernel32_file_path + _T(" が見つかりませんでした。"), _T("CheckLanguage"));
		return false;
	}

	// バージョン情報サイズ取得（エラー時は0）
	int version_info_size = GetFileVersionInfoSize(kernel32_file_path.c_str(), NULL);
	if (version_info_size == 0)	{
		set_last_error_win(_T("GetFileVersionInfoSize"));
		return false;
	}

	// バージョン情報ブロックをコピーするバッファ確保
	ARRAY<BYTE> version_info(version_info_size);

	// バージョン情報ブロックの取得
	if (GetFileVersionInfo(kernel32_file_path.c_str(), 0, version_info_size, (void *)version_info.get()) == FALSE)	{
		set_last_error_win(_T("GetFileVersionInfo"));
		return false;
	}

	// バージョン情報の取得
	GFV_LANGANDCODEPAGE* lang_p;
	unsigned int lang_size;
	if (VerQueryValue((void * const)version_info.get(), _T("\\VarFileInfo\\Translation"), (void **)&lang_p, &lang_size) == FALSE)	{
		set_last_error_win(_T("VerQueryValue"));
		return false;	// エラー詳細は出さない！
	}

	// 言語コードが日本語であるかどうか調べる
	int lang_cnt = lang_size / sizeof(GFV_LANGANDCODEPAGE);		// ループ数は１回しかないと思います。
	for (int i = 0; i < lang_cnt; i++)	{
		if (lang_p->language == 0x0411)	{		// 0x0411 が日本語です。
			japanese_flag = true;
			break;
		}
		lang_p++;
	}

	if (!japanese_flag)	{
		set_last_error(_T("日本語情報が見つかりませんでした。"), _T("CheckLanguage"));
		return false;
	}

	return japanese_flag;
}

// ================================================================
// システム言語の文字列を取得する
// ----------------------------------------------------------------
//	LOCALE_SLANGUAGE は「言語名」の文字列を取得します。
//	取得される文字列は、その国の言語の文字列で取得されます。
//	英語版 Windows XP の場合、「JAPANESE」と言う文字列になります。
//	日本語版 Windows XP の場合、「日本語」と言う文字列になります。
//	しかし、日本語版 Windows Vista の場合、「日本語（日本）」となるので、
//	頭の文字列の３文字（６バイト）のみで判定しています。
//	日本語以外の設定であっても取得した文字列が「日本語文字列」であれば、
//	日本語版と判定しても良いかもです。
//	例えば、英語が選択されていれば「英語」と言う文字列が返ってきます。
//	これが英語版であれば「English」と返ってきます。
//
//	※日本語版 Windows 7 RC1 32bit で「JAPANESE」が戻ってきたﾖ…ＴＴ
//		→ 怖いので、言語コードで判定することにしました。日本語は "0411" です。
// ----------------------------------------------------------------

TSTR tnm_get_system_language_str()
{
	TCHAR buf[1024] = {0};
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SLANGUAGE, buf, 1024);
	return buf;
}

// システム言語のコードを取得する
TSTR tnm_get_system_language_code()
{
	TCHAR buf[1024] = {0};
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, buf, 1024);
	return buf;
}

// システム言語を判定する
bool tnm_check_system_language()
{
#if 0		// 言語文字列で判定
	avg_get_system_language_str(str, 4096);
	if (strcomp2(str, (UC *)"日本語", 6) == 0)	{	return true;	}
	else	{
		change_str(str, 1);
		if (strcomp2(str, (UC *)"JAPANESE", 8) == 0)	{	return true;	}
		else									{	return false;	}
	}
#else		// 言語コードで判定
	if (tnm_get_system_language_code().substr(0, 4) == _T("0411"))
		return true;

	return false;
#endif
}

// ================================================================
// タイムゾーンを調べる
// ----------------------------------------------------------------
//	「タイムゾーン」が「東京」であれば、「日本語版Windows」と判定します。
//	取得される文字列は、その国の言語の文字列で取得されます。
//	英語版Windows XP の場合、「Tokyo Standard Time」と言う文字列になります。
//	日本語版Windows XP の場合、「東京 (標準時)」と言う文字列になります。
//	今後表記が変わる可能性があるので、安全を考え「東京」の２文字（4バイト）のみを判定しています。
//	日本語以外の設定であっても取得した文字列が日本語であれば、日本語版と判定しても良いかもです。
// ----------------------------------------------------------------

// タイムゾーンを取得する
TSTR tnm_get_timezone_str()
{
	TIME_ZONE_INFORMATION info = {0};
	GetTimeZoneInformation(&info);
	return info.StandardName;
}

bool tnm_check_timezone()
{
	TSTR timezone = tnm_get_timezone_str();
	if (timezone.substr(0, 2) == _T("東京"))
		return true;

	if (timezone.substr(0, 5) == _T("TOKYO"))
		return true;

	return false;
}

// ================================================================
// 日本語版 Windows を判定する
// ----------------------------------------------------------------
// kernel32.dll バージョン情報の「言語」、
// システムの規定言語、タイムゾーンの３つの情報から、
// 日本語版かどうかをチェックします。
// ----------------------------------------------------------------
bool tnm_check_japanese_edition_windows()
{
#if 1
	// kernel32.dll バージョン情報の「言語」を調べる
	if (!tnm_check_kernel32_language())	{
		TSTR message = TSTR() +
			_T("This Game is Japan Only\n\n")
			_T("このゲームは日本国内でのみプレイ可能です。\n\n")
			_T("動作させるには 『 日本語版Windows 』 が必要です。\n\n")
			_T("ゲームを終了します。\n")
#if 0
			+ _T("\n") + get_last_error_msg()
#endif
			;
		message_box(message, _T("日本語版Windows判定"), MB_OK | MB_ICONWARNING);
		return false;
	}

	// システムの規定言語を調べる
	if (!tnm_check_system_language())	{
		TSTR language_str = tnm_get_system_language_str();
		TSTR message = str_format(
			_T("This Game is Japan Only\n\n")
			_T("このゲームは日本国内でのみプレイ可能です。\n\n")
			_T("動作させるには 『 日本語版Windows 』 が必要です。\n\n")
			_T("コンピュータの現在の言語設定は 『 %s 』 です。\n\n")
			_T("言語を 『 日本語 』 に設定して下さい。\n\n")
			_T("ゲームを終了します。\n")
			, language_str.c_str());
		message_box(message, _T("日本語版Windows判定"), MB_OK | MB_ICONWARNING);
		return false;
	}

	// タイムゾーンを調べる
	if (!tnm_check_timezone())	{
		TSTR timezone_str = tnm_get_timezone_str();
		TSTR message = str_format(
			_T("This Game is Japan Only\n\n")
			_T("このゲームは日本国内でのみプレイ可能です。\n\n")
			_T("動作させるには 『 日本語版Windows 』 が必要です。\n\n")
			_T("コンピュータの現在のタイムゾーン設定は 『 %s 』 です。\n\n")
			_T("タイムゾーンを 『 日本（大阪、札幌、東京） 』 に設定して下さい。\n\n")
			_T("ゲームを終了します。\n")
			, timezone_str.c_str());
		message_box(message, _T("日本語版Windows判定"), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;

#else

	return true;

#endif
}

// ****************************************************************
// エンジン：ディレクトリを初期化
// ================================================================
bool tnm_init_dir()
{
	if (Gp_reg_ini->reg_name.empty())
	{
		// レジストリがない場合

		// 警告
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セットアップされていません。"));

		// セットアップタイプ：セットアップされていない
		Gp_global->setup_type = TNM_SETUP_TYPE_NONE;

		// デフォルトのフォルダを指定
		Gp_dir->exe_dir = G_app.module_dir;						// SiglusEngine.exe やアイコンはこちら
		Gp_dir->base_dir = G_app.module_dir;					// g00 フォルダなどはこちら
		Gp_dir->work_dir = G_app.module_dir;					// 作業フォルダのデフォルトは exe の位置
		if (Gp_cmd_line->work_dir_set)							// 作業フォルダはコマンドラインで指定できる
		{
			if (file_get_type(Gp_cmd_line->work_dir) != 0)		// 作業フォルダチェック
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("作業フォルダが見つかりません。"));
			}
			else
			{
				Gp_dir->work_dir = Gp_cmd_line->work_dir;
			}
		}
	}
	else 
	{
		// レジストリがある場合
		TSTR reg_path = _T("SoftWare\\") + Gp_reg_ini->reg_name;
		TSTR exe_dir, work_dir, dat_dir, dat_setup;

		// レジストリを探す
		if (!reg_check_key(HKEY_LOCAL_MACHINE, reg_path))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セットアップ情報が見つかりません。レジストリが壊れています。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
			return false;
		}
		// exe_dir を取得
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("exe_dir"), exe_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("実行ファイルのパスが指定されていません。レジストリが壊れています。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
			return false;
		}
		// マイドキュメントのパスを取得
		if (!get_my_doument_path(work_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("マイドキュメントのパスが取得できませんでした。\n") + get_last_error_msg());
			return false;
		}

		// dat_dir を取得
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("dat_dir"), dat_dir))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダが指定されていません。レジストリが壊れています。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
			return false;
		}
		// dat_setup を取得
		if (!reg_get_value_str(HKEY_LOCAL_MACHINE, reg_path, _T("dat_setup"), dat_setup))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダが指定されていません。レジストリが壊れています。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
			return false;
		}
		// フォルダチェック
		if (file_get_type(exe_dir) != 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("実行フォルダが見つかりません。レジストリが壊れています。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
			return false;
		}

		// 最小セットアップの場合
		if (dat_setup == _T("min"))
		{
			// セットアップタイプ：最小セットアップ
			Gp_global->setup_type = TNM_SETUP_TYPE_MIN;

			// 警告
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("最小セットアップされています。"));

			// データフォルダチェック
			if (file_get_type(dat_dir) != 0)
			{
				// データフォルダがない場合、ディスクドライブを検索する

				// ディスクドライブを検索
				TCHAR disc_letter = tnm_search_disc_drive();
				if (disc_letter != 0)
				{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("認識ファイル: ") + Gp_ini->disc_mark);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, TSTR() + disc_letter + _T(":\\ にディスクが見つかりました。"));
				}
				else
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, Gp_ini->game_name + _T(" のゲームディスクを挿入してください。"));
					return false;
				}

				// ドライブレターをディスクに変更してみる
				dat_dir[0] = disc_letter;
				if (file_get_type(dat_dir) != 0)
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("データフォルダが見つかりません。正しいゲームディスクが挿入されていることを確認してください。"));
					return false;
				}
				else
				{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("データフォルダをディスクに再設定しました。"));
				}
			}

			// ディレクトリを指定
			Gp_dir->exe_dir = exe_dir;
			Gp_dir->base_dir = dat_dir;
			Gp_dir->work_dir = work_dir + _T("\\") + Gp_reg_ini->reg_name;
		}

		// 完全セットアップの場合
		else
		{
			// セットアップタイプ：完全セットアップ
			Gp_global->setup_type = TNM_SETUP_TYPE_FULL;

			// 警告
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("完全セットアップされています。"));

			// データフォルダチェック
			if (file_get_type(dat_dir) != 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("データフォルダが見つかりません。レジストリが壊れている可能性があります。\n\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
				return false;
			}

			// ディレクトリを指定
			Gp_dir->exe_dir = exe_dir;
			Gp_dir->base_dir = dat_dir;
			Gp_dir->work_dir = work_dir + _T("\\") + Gp_reg_ini->reg_name;
		}
	}

	// ユーザーフォルダ
	TSTR user_dir = Gp_dir->work_dir;
	if (Gp_cmd_line->multi_user_id_set)
	{
		user_dir += _T("\\") LCL_USERDATA_DIR _T("\\") + Gp_cmd_line->multi_user_id;
	}

	Gp_dir->save_dir = user_dir + _T("\\") + LCL_SAVEDATA_DIR;		// セーブフォルダはユーザーフォルダの中
	Gp_dir->debug_log_dir = user_dir + _T("\\__DEBUG_LOG");			// デバッグログフォルダはユーザーフォルダの中

	// アペンドディレクトリを探す
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
		Gp_dir->append_dir = Gp_select_ini->first_dir();	// 先頭のディレクトリを探す
		Gp_dir->append_name = Gp_select_ini->first_name();
	}

	return true;
}

// ****************************************************************
// エンジン：セーブフォルダを作成する
// ================================================================
bool tnm_create_save_dir()
{
	// フォルダがなければフォルダを作成する
	if (file_get_type(Gp_dir->save_dir) != 0)	{
		if (!::create_dir_from_dir_path(Gp_dir->save_dir + _T("\\")))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダの作成に失敗しました。\nセーブデータの保存先が無効なドライブになっていませんか？\nゲームを再セットアップすることでこの問題は解決する可能性があります。\n") + get_last_error_msg());
			return false;
		}
	}

	// まだフォルダがなければ失敗
	if (file_get_type(Gp_dir->save_dir) != 0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダが作成されていません。\nセーブデータの保存先が無効なドライブになっていませんか？\nゲームを再セットアップすることでこの問題は解決する可能性があります。"));
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブフォルダを作成しました。"));

	return true;
}

// ****************************************************************
// エンジン：起動日時を取得する
// ================================================================
void tnm_get_start_time()
{
	// 起動日時を取得して、エラーログに出力します。
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("■") + get_sys_time_str() + _T(" ") + Gp_ini->game_name);
}

// ****************************************************************
// エンジン：システム情報を取得する
// ================================================================
void tnm_get_system_info()
{
	// ＯＳのバージョンを取得する
	OSVERSIONINFOEX os_version_info = {0};
	os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&os_version_info);

	TSTR os_version_str;
	if (!tnm_get_os_name(os_version_str))	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("OS 情報が取得できませんでした。\n") + get_last_error_msg());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("OS バージョン=%d.%d (%s)"), os_version_info.dwMajorVersion, os_version_info.dwMinorVersion, os_version_str.c_str()));
	}

	// メモリの情報を取得する
	MEMORYSTATUS memory = {0};
	memory.dwLength = sizeof(memory);
	::GlobalMemoryStatus(&memory);
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("メモリ=%d MByte"), (int)(memory.dwTotalPhys / 1024 / 1024)));

	// ディスプレイの情報を取得する
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("ディスプレイデバイスが見つかりました。")));
	for (int i = 0; ; i++)	{
		DISPLAY_DEVICE dd = {0};
		dd.cb = sizeof(DISPLAY_DEVICE);
		if (EnumDisplayDevices(NULL, i, &dd, 0) == FALSE)
			break;

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("- デバイス%d=%s(%s)"), i, dd.DeviceString, dd.DeviceName));
	}
}

// ****************************************************************
// エンジン：DirectX の情報を取得
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

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("DirectX バージョン=%s(%s)"), directx_version_str.c_str(), directx_version_str_reg.c_str()));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("DirectX=不明なバージョン"));
	}
}

// ****************************************************************
// エンジン：Windows Media Player の情報を取得
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
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player バージョン=") + version);
		}
		else	{
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player が見つかりませんでした。"));
		}
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Windows Media Player バージョンが取得できませんでした。"));
	}
}

// ****************************************************************
// エンジン：Direct3D 初期化
// ================================================================
bool tnm_init_direct_3d()
{
	// Direct3D を初期化します。
	if (!G_d3d.create())
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D の初期化に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D を初期化しました。"));

	// ディスプレイ情報を取得します。
	int display_cnt = G_d3d->GetAdapterCount();
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D ディスプレイアダプタが見つかりました。"));
	for (int i = 0; i < display_cnt; i++)	{
		D3DADAPTER_IDENTIFIER9 di = {0};

		// アイデンティファー取得（アダプター名などの情報）
		G_d3d->GetAdapterIdentifier(i, 0, &di);
		TSTR version = tostr(HIWORD(di.DriverVersion.HighPart)) + _T(".") + tostr(LOWORD(di.DriverVersion.HighPart)) + _T(".") + tostr(HIWORD(di.DriverVersion.LowPart)) + _T(".") + tostr(LOWORD(di.DriverVersion.LowPart));
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("- アダプタ%d=%s version=%s (%s)"), i, MBSTR_to_TSTR(di.Description).c_str(), version.c_str(), MBSTR_to_TSTR(di.DeviceName).c_str()));
		Gp_system_info->display_list.push_back(di);

		// ディスプレイモードの情報を取得します。
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

		// リストを並び替えて同じものを削除します。
		std::sort(resolution_list.resolution_list.begin(), resolution_list.resolution_list.end());
		resolution_list.resolution_list.erase(std::unique(resolution_list.resolution_list.begin(), resolution_list.resolution_list.end()), resolution_list.resolution_list.end());

		// デフォルトの解像度番号を初期化
		resolution_list.default_resolution_no = 0;

		// 現在の解像度の番号を調べてデフォルトの解像度に設定
		DEVMODE dm = {0};
		dm.dmSize = sizeof(DEVMODE);
		EnumDisplaySettingsEx(MBSTR_to_TSTR(di.DeviceName).c_str(), ENUM_CURRENT_SETTINGS, &dm, EDS_RAWMODE);
		for (int j = 0; j < (int)resolution_list.resolution_list.size(); j++)	{
			if (resolution_list.resolution_list[j].resolution == C_size(dm.dmPelsWidth, dm.dmPelsHeight))	{
				resolution_list.default_resolution_no = j;
				break;
			}
		}

		// この情報をリストに追加
		Gp_system_info->resolution_table.push_back(resolution_list);
	}

	// 実際の値に反映する
	Gp_global->fullscreen_display_no = 0;
	Gp_global->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_global->fullscreen_display_no].default_resolution_no;

	// ウィンドウサイズを補正する（上記 Direct3D 初期化処理で解像度を取得してから行う必要がある）
	tnm_rep_screen_size(Gp_global->screen_size_mode, &Gp_global->screen_size_scale, &Gp_global->screen_size_free);
	Gp_global->total_screen_size.cx = Gp_global->game_screen_size.cx * Gp_global->screen_size_scale.cx / 100;
	Gp_global->total_screen_size.cy = Gp_global->game_screen_size.cy * Gp_global->screen_size_scale.cy / 100;

	return true;
}

// ****************************************************************
// エンジン：Direct3D デバイス初期化
// ================================================================
bool tnm_init_direct_3d_device()
{
	// Direct3D のデバイスを作成します。
	int present_interval = Gp_global->wait_display_vsync_def ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	C_size size = Gp_ini->screen_size;
	int shadow_map_size = 0;
	int adapter_no = Gp_system_config->d3d_adapter_no;
	DWORD vertex_process_type = Gp_system_config->d3d_hardware_vertex_flag ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	bool mipmap_use_flag = Gp_system_config->d3d_use_mipmap_flag;
	bool pow2_limit_flag = false;	// 2^n に制限するフラグ。デフォルトは false で制限しない。
	if (!G_d3d.device.create(Gp_wnd->get_handle(), true, size.cx, size.cy, shadow_map_size, adapter_no, vertex_process_type, present_interval, mipmap_use_flag, pow2_limit_flag))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D のデバイスの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// デバイスの情報を取得
	const C_d3d_device_state& state = G_d3d.device.state();
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D のデバイスを作成しました。"));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ビデオメモリ=") + tostr((double)state.vm_size / 1024 / 1024) + _T("MByte"));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 頂点処理=") + get_str_d3d_vertex_process_type(state.vertex_process_type));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 頂点シェーダバージョン=") + tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.VertexShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.VertexShaderVersion)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 頂点シェーダの定数ﾚｼﾞｽﾀ数=") + tostr(state.device_caps.MaxVertexShaderConst));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ピクセルシェーダバージョン=") + tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.PixelShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.PixelShaderVersion)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 最大テクスチャサイズ=") + tostr(C_size(state.device_caps.MaxTextureWidth, state.device_caps.MaxTextureHeight)));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 2の累乗以外のテクスチャ=") + TSTR((state.device_caps.TextureCaps & D3DPTEXTURECAPS_POW2) ? _T("No") : _T("Yes")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾃｸｽﾁｬｱﾄﾞﾚｯｼﾝｸﾞ BORDER=") + TSTR((state.device_caps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) ? _T("Yes") : _T("No")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾐｯﾌﾟﾏｯﾌﾟの自動生成=") + TSTR((state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) ? _T("Yes") : _T("No")));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾐｯﾌﾟﾏｯﾌﾟの自動生成(X8R8G8B8 ﾃｸｽﾁｬ)=") + get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾐｯﾌﾟﾏｯﾌﾟの自動生成(A8R8G8B8 ﾃｸｽﾁｬ)=") + get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾌｨﾙﾀｰの検索(X8R8G8B8 ﾃｸｽﾁｬ)=") + get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture));
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ﾌｨﾙﾀｰの検索(A8R8G8B8 ﾃｸｽﾁｬ)=") + get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture));

	// Direct3D のステートを初期化します。
	if (!G_d3d.state.init())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D のステートの初期化に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// Direct3D のステートを初期設定を行います。
	if (!G_d3d.state.set_template())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D のステートの初期設定に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D ステートの初期化に成功しました。"));

	// Direct3D のビューを初期化します。
	if (!Gp_wnd->m_view.init())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D のビューの初期化に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D ビューの初期化に成功しました。"));

	// Direct3D エフェクトを作成します。

#if 0
	if (!Gp_wnd->m_view.m_default_effect.create_default_effect())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D エフェクトの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	else	{
		Gp_global->shader_from_source = true;
	}
#else
	if (!Gp_wnd->m_view.m_default_effect.load_effect_from_resource((LPCTSTR)IDR_EFFECT1, _T("EFFECT")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D エフェクトの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
#endif

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D エフェクトを作成しました。"));

	// ビューを黒に塗りつぶします。（これがないと、GeForce 系でゴミ描画が行われてしまう）
	Gp_wnd->m_view.clear_render_target();

	//// レンダラーを作成します。
	//if (!Gp_wnd->m_renderer.init())	{
	//	tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D レンダラーの作成に失敗しました。\n") + get_last_error_msg());
	//	return false;
	//}

	// 頂点バッファを作成します。
	if (!tnm_create_vertex_buffer())
		return false;

	// ワイプ用のバッファを作成します。
	if (!tnm_create_wipe_buffer())
		return false;

	// デバッグテキストを作成します。
	if (!G_debug_text.create(false))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D テキストの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	return true;
}

// ****************************************************************
// エンジン：頂点バッファを作成
// ================================================================
bool tnm_create_vertex_buffer()
{
	// 矩形スプライト用の頂点バッファとインデックスバッファを作成します。
	// このバッファは全ての２Ｄオブジェクトで使いまわします。

	// 頂点バッファを作ります。
	// ここでは大きさ 32 で作成します。描画の際に必要な分だけ拡張します。
	// 毎フレーム書き換えるので、動的バッファを使います。
	G_rect_vertex_buffer_d2_filter = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_filter->create_vertex_buffer(32, S_vtx_d2_filter::FVF, sizeof(S_vtx_d2_filter), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_filter = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_filter->create_vertex_buffer(32, S_vtx_d3_filter::FVF, sizeof(S_vtx_d3_filter), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_pct = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_pct->create_vertex_buffer(32, S_vtx_d2_pct::FVF, sizeof(S_vtx_d2_pct), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_pct_mask = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_pct_mask->create_vertex_buffer(32, S_vtx_d2_pct_mask::FVF, sizeof(S_vtx_d2_pct_mask), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_pct = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_pct->create_vertex_buffer(32, S_vtx_d3_pct::FVF, sizeof(S_vtx_d3_pct), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_screen = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_screen->create_vertex_buffer(32, S_vtx_d2_screen::FVF, sizeof(S_vtx_d2_screen), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d3_screen = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d3_screen->create_vertex_buffer(32, S_vtx_d3_screen::FVF, sizeof(S_vtx_d3_screen), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	G_rect_vertex_buffer_d2_mask_wipe = BSP<C_d3d_vertex_buffer>(new C_d3d_vertex_buffer);
	if (!G_rect_vertex_buffer_d2_mask_wipe->create_vertex_buffer(32, S_vtx_d2_mask_wipe::FVF, sizeof(S_vtx_d2_mask_wipe), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D 共用の頂点バッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// インデックスバッファを作ります。
	// ここではスプライト 256 個分だけ作成します。描画の際に必要な分だけ拡張します。
	// 内容を書き換えるのは拡張時のみなので、静的バッファを使います。
	G_rect_index_buffer = BSP<C_d3d_index_buffer>(new C_d3d_index_buffer);
	if (!tnm_update_rect_index_buffer(256))
		return false;

	return true;
}

// ****************************************************************
// エンジン：ワイプバッファを作成
// ================================================================
bool tnm_create_wipe_buffer()
{
	C_size size = Gp_global->game_screen_size;

	// レンダーターゲットとなるテクスチャを作成する
	// テクスチャはレンダーターゲットにするため、D3DUSAGE_RENDERTARGET を指定
	// D3DUSAGE_RENDERTARGET にするには、D3DPOOL_DEFAULT を指定する必要がある
	// D3DPOOL_DEFAULT を指定した場合は、デバイスが消失したら自分で作成しなおす必要があるため、リソースマネージャを利用する
	// カラーフォーマットは、デバイスの作成時に調べた D3DFMT_A8R8G8B8 にすること。詳しくはデバイスの作成参照。
	Gp_wnd->m_wipe_back_buffer_texture_0 = G_rsm.create_texture(_T("ﾜｲﾌﾟﾊﾞｯﾌｧ0"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!Gp_wnd->m_wipe_back_buffer_texture_0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ワイプ用のテクスチャ０の作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 深度ステンシルバッファを作成する
	// デバイスが消失した場合、深度ステンシルバッファは必ず作り直す必要があるため、リソースマネージャを利用する
	Gp_wnd->m_wipe_dpst_buffer_surface_0 = G_rsm.create_dpst_buffer(_T("ﾜｲﾌﾟﾊﾞｯﾌｧ"), size.cx, size.cy, true);
	if (!Gp_wnd->m_wipe_dpst_buffer_surface_0)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Direct3D ワイプ用の深度ステンシルバッファの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// ワイプバッファを２個以上作る場合
	if (Gp_global->wipe_buffer_cnt >= 2)	{

		// レンダーターゲットとなるテクスチャを作成する
		Gp_wnd->m_wipe_back_buffer_texture_1 = G_rsm.create_texture(_T("ﾜｲﾌﾟﾊﾞｯﾌｧ1"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
		if (!Gp_wnd->m_wipe_back_buffer_texture_1)	{
			Gp_global->wipe_buffer_cnt = 1;
			tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T(" - ２枚目のワイプバッファが作れませんでした。一部ワイプが簡易表現になります。\n（この警告はデバッグ時にしか表示されません。）\n") + get_last_error_msg());
		}
	}

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("Direct3D ワイプ用のバッファを作成しました。"));

	return true;
}

// ****************************************************************
// エンジン：DirectSound 初期化
// ================================================================
BOOL CALLBACK DSEnumCallback(LPGUID p_guid, LPCWSTR p_description, LPCWSTR p_module, LPVOID p_context)
{
	if (p_guid == NULL)	{	// 最初に列挙されるのはプライマリデバイスで、guid は NULL である
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("利用可能なサウンドデバイスが見つかりました。"));
	}
	else	{				// ２回目以降に呼び出されるのが実在のデバイス
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ") + TSTR(p_description));
	}

	return TRUE;
}

bool tnm_init_direct_sound()
{
	HRESULT hr;

	// サウンドドライバを列挙する
	hr = DirectSoundEnumerate(DSEnumCallback, NULL);
	if (FAILED(hr))
	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("利用可能なサウンドデバイスが見つかりませんでした。"));
		// 次の DirectSound の初期化でエラーが出ると思うが、試しはしたいので、処理を続行する
	}

	// DirectSound を初期化します。
	if (!G_ds.init(G_app.h_wnd))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("DirectSound の初期化に失敗しました。\nスピーカーもしくはヘッドホンが接続されていないか、サウンドドライバが壊れている可能性があります。\n\n≪エラー詳細≫\n") + get_last_error_msg());
		return false;
	}

	// プレイマネージャを初期化します。
	G_pm.init();

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("DirectSound の初期化に成功しました。"));

	return true;
}

// ****************************************************************
// エンジン：解放処理
// ================================================================
//		ウィンドウが閉じられる直前の処理です。
//		ウィンドウに関連しているものは、必ずここで解放してください。
//		Direct3D はもちろん、Direct3D スプライトを持つ物体も解放しないといけません。（▲未実装）
// ================================================================
//		DirectSound を解放します。
//		デバッグウィンドウを解放します。
// ================================================================
bool C_tnm_eng::free()
{
	if (!free_global())
		return false;

	return true;
}

bool C_tnm_eng::free_global()
{
	// 初期化が成功した場合のみの作業
	// 初期化エラー時にこれを行ってしまうと、グローバルデータなどが全て飛んでしまう！
	if (m_init_success_flag)	{

		// 最後にもう１度タイマーを更新する
		timer_proc();

		// グローバルデータをセーブします。
		tnm_save_global_on_file();

		// デバッグウィンドウの情報を保存します。
		Gp_dlg_mng->save_initial_state();
	}

	// デバッグウィンドウを解放します。
	Gp_dlg_mng->free();

	// メインウィンドウを非表示にします。
	Gp_wnd->set_show(false);

	// ウィンドウからメニューをデタッチして破棄します。
	Gp_wnd->set_menu(NULL);
	Gp_wnd->m_wnd_menu.destroy();

	// ウィンドウからアイコンをデタッチします。
	Gp_wnd->set_icon(NULL);
	Gp_wnd->set_small_icon(NULL);

	// サウンドを解放します。
	m_sound.free();

	// プレイヤーマネージャを解放します。
	G_pm.release();

	// DirectSound を解放します。
	G_ds.release();

	// デバッグテキストを解放します。
	G_debug_text.release();

	// レンダラーを解放します。
	//Gp_wnd->m_renderer.release();

	// Direct3D デバイスを解放します。
	G_d3d.device.release();

	// フォントを解放します。
	m_font.release();

	// Steam
#if __USE_STEAM
	SteamAPI_Shutdown();
#endif

	// 再度フラグを立てる（終了判定用）
	Gp_global->game_end_flag = true;

	// 解放完了
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("全ての解放処理が完了しました。"));

	return true;
}

// ****************************************************************
// エンジン：終了処理
// ================================================================
//		アプリケーション終了直前の処理です。
//		この時点でウィンドウは閉じられていることに注意してください。
// ================================================================
bool C_tnm_eng::end()
{
	// Direct3D を解放します。
	G_d3d.release();

	// アイコンを解放します。
	Gp_icon->destroy_icon();

	// スタックを解放します。
	if (m_stack.int_p)
	{
		::HeapFree(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_stack.int_p);
	}

	// window.ini の読み取り専用を外します。
	::file_set_read_only(Gp_dir->get_window_ini_file_path(), false);

	// window.ini を保存します。
	C_ini_file::body().save_ini_file();

	// デバッグ dll を解放します。
	m_debug_dll.free();

	return true;
}

// ****************************************************************
// エンジン：開始処理
// ================================================================
bool C_tnm_eng::start()
{
	// スタートシーンから始める
	tnm_scene_proc_start_from_start_scene();

	// ★ゲームタイマー開始
	m_global.game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// エンジン：初期化処理：ローカル
// ================================================================
//		ゲーム起動時に１回だけ呼ばれます。
//		２回目以降は reinit が呼ばれます。
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

	// メッセージウィンドウ
	mwnd_btn_disable.resize(Gp_ini->mwnd_waku_btn_cnt);
	for (int i = 0; i < Gp_ini->mwnd_waku_btn_cnt; i++)	{
		mwnd_btn_disable[i].value = false;
	}

	// フォント
	font_name.clear();

	// pod
	pod.init();
}

bool C_tnm_eng::init_local()
{
	// セーブを初期化します。
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

	// ボタンマネージャを初期化します。
	m_btn_mng.init();

	// システムデータを初期化します。
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

	// システムコマンドメニューを初期化します。
	tnm_syscom_init_syscom_flag();

	// シーンデータを初期化します。
	m_local.init();

	// スタックを初期化します。
	m_stack.int_now_cnt = 0;
	m_stack.stack_point_list.clear();

	// タイマーを初期化します。
	m_timer.local_real_time = 0;
	m_timer.local_real_time_past = 0;
	m_timer.local_real_time_amari = 0;
	m_timer.local_game_time = 0;
	m_timer.local_game_time_past = 0;
	m_timer.local_game_time_amari = 0;
	m_timer.local_wipe_time = 0;
	m_timer.local_wipe_time_past = 0;
	m_timer.local_wipe_time_amari = 0;

	// マウスカーソルを変更します。
	tnm_cursor_frame(m_timer.local_real_time);

	// フェッチポイントを初期化します。
	tnm_init_fetch_point();

	// フォグを初期化します。
	m_fog.init_local();

	// ================================================================
	// エレメントの初期化
	// ----------------------------------------------------------------

	// フラグを初期化します。
	m_flag.init_local();

	// ユーザインクプロパティを初期化します。
	m_user_inc_prop.init();

	// ユーザシーンプロパティリストを初期化します。
	m_user_scn_prop_list.init(S_element(), FM_INTLIST, _T("user_prop"), Gp_lexer->get_scn_cnt(), false, NULL);

	// カウンタリストを作成します。
	m_counter_list.init(S_element(ELM_GLOBAL_COUNTER), FM_COUNTERLIST, _T("counter"), Gp_ini->counter_cnt, false, NULL);

	// フレームアクションを初期化します。
	m_frame_action.init(S_element(ELM_GLOBAL_FRAME_ACTION), _T("frame_action"), S_element());

	// フレームアクションチャンネルを初期化します。
	m_frame_action_ch_list.init(S_element(ELM_GLOBAL_FRAME_ACTION_CH), _T("frame_action_ch"), Gp_ini->frame_action_ch_cnt, false, S_element());

	// Ｇ００バッファを初期化します。
	m_g00_buf_list.init(S_element(ELM_GLOBAL_G00BUF), FM_G00BUFLIST, _T("g00buf"), Gp_ini->g00_buf_cnt, false, NULL);

	// マスクを初期化します。
	m_mask_list.init(S_element(ELM_GLOBAL_MASK), FM_MASKLIST, _T("mask"), Gp_ini->mask_cnt, false, NULL);

	// ステージを初期化します。
	m_stage_list.init(S_element(ELM_GLOBAL_STAGE), _T("stage"), _T(""), TNM_STAGE_CNT, false, true, NULL);

	// メッセージバックを初期化します。
	m_msg_back.init();

	// スクリーンを初期化します。
	m_screen.init(S_element(ELM_GLOBAL_SCREEN), _T("screen"));

	// ワイプを初期化します。
	m_wipe.init();

	// サウンドを再初期化します。
	m_sound.reinit();

	// 効果音イベントを初期化します。
	m_pcm_event_list.init(S_element(ELM_GLOBAL_PCMEVENT), FM_PCMEVENTLIST, _T("pcmevent"), Gp_ini->pcm_event_cnt, false, NULL);

	// エディットボックスを初期化します。
	m_editbox_list.init(S_element(ELM_GLOBAL_EDITBOX), FM_EDITBOXLIST, _T("editbox"), Gp_ini->editbox_cnt, false, NULL);

	// コールを初期化します。
	m_call_list.init(S_element(ELM_GLOBAL_CALL), FM_CALLLIST, _T("call"), 1, true, NULL);		// 初期コール数は１

	// ＥＸコールを初期化します。
	m_excall.init(S_element(ELM_GLOBAL_EXCALL), _T("excall"));

	// キャプチャーを初期化します。
	m_global.capture_prior = TNM_CAPTURE_PRIOR_NONE;

	// 初期化完了メッセージ
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ローカルデータを初期化しました。"));

	return true;
}

// ****************************************************************
// エンジン：ローカル再初期化
// ================================================================
//		「最初から始める」などの時に呼び出されます。
//		各種エレメントの初期化などを行います。
// ================================================================
bool C_tnm_eng::reinit_local(bool restruct_flag)
{
	// セーブを初期化します。
	m_local_save_history_index = -1;
	m_local_save.save_id = S_tid();
	m_local_save.append_dir.clear();
	m_local_save.append_name.clear();
	m_local_save.save_scene_title.clear();
	m_local_save.save_msg.clear();
	m_local_save.save_full_msg.clear();
	m_local_save.save_stream.clear();
	m_local_save.save_stream_ex.clear();

	// 再構築する場合のみ選択肢セーブは消す
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

	// ボタンマネージャを初期化します。
	m_btn_mng.init();

	// システムデータを初期化します。
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

	// システムコマンドメニューを初期化します。
	tnm_syscom_init_syscom_flag();

	// シーンデータを初期化します。
	m_local.init();

	// スタックを初期化します。
	m_stack.int_now_cnt = 0;
	m_stack.stack_point_list.clear();

	// タイマーを初期化します。
	m_timer.local_real_time = 0;
	m_timer.local_real_time_past = 0;
	m_timer.local_real_time_amari = 0;
	m_timer.local_game_time = 0;
	m_timer.local_game_time_past = 0;
	m_timer.local_game_time_amari = 0;
	m_timer.local_wipe_time = 0;
	m_timer.local_wipe_time_past = 0;
	m_timer.local_wipe_time_amari = 0;

	// マウスカーソルを変更します。
	tnm_cursor_frame(m_timer.local_real_time);

	// フェッチポイントを初期化します。
	tnm_init_fetch_point();

	// フォグを初期化します。
	m_fog.init_local();

	// ================================================================
	// エレメントの初期化
	// ----------------------------------------------------------------

	// フラグを初期化します。
	m_flag.init_local();

	// ユーザインクプロパティを初期化します。
	m_user_inc_prop.reinit();

	// ユーザシーンプロパティリストを初期化します。
	m_user_scn_prop_list.reinit();

	// カウンタリストを作成します。
	m_counter_list.reinit();

	// フレームアクションを初期化します。
	m_frame_action.reinit(false);			// これは finish フラグ

	// フレームアクションチャンネルを初期化します。
	m_frame_action_ch_list.reinit(false, restruct_flag);

	// Ｇ００バッファを初期化します。
	m_g00_buf_list.reinit();

	// マスクを初期化します。
	m_mask_list.reinit();

	// ステージを初期化します。
	m_stage_list.reinit(restruct_flag);

	// メッセージバックを初期化します。
	if (!Gp_global->reinit_msgbk_except_flag)
	{
		m_msg_back.init();
	}

	// スクリーンを初期化します。
	m_screen.reinit();

	// ワイプを初期化します。
	m_wipe.init();

	// サウンドを再初期化します。
	m_sound.reinit();

	// 効果音イベントを初期化します。
	m_pcm_event_list.reinit();

	// エディットボックスを初期化します。
	m_editbox_list.reinit();

	// コールを初期化します。
	m_call_list.reinit();

	// ＥＸコールを初期化します。
	m_excall.reinit(false, restruct_flag);

	// 文字マネージャをクリアします。
	tnm_update_font(true);

	// キャプチャーを初期化します。
	Gp_wnd->m_capture_texture_for_save_thumb.reset();
	Gp_wnd->m_capture_texture_for_object.reset();
	Gp_wnd->m_capture_texture_for_tweet.reset();
	Gp_global->capture_prior = TNM_CAPTURE_PRIOR_NONE;

	// 初期化完了メッセージ
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ローカルデータを再初期化しました。"));

	return true;
}

// ****************************************************************
// エンジン：初期化処理：終了
// ================================================================
//		★エレメントの終了処理です！
// ================================================================
bool C_tnm_eng::finish_local()
{
	// フレームアクション
	m_frame_action.finish();

	// フレームアクションチャンネル
	m_frame_action_ch_list.finish();

	// ステージ
	m_stage_list.finish();

	// ＥＸコール
	m_excall.finish();

	return true;
}
