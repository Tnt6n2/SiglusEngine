#include	"pch.h"

#include	"localize/localize.h"

#include	"ifc_eng.h"
#include	"data/tnm_ini.h"

#include	"lzss.h"
#include	"tnm_common_header.h"

#include	<boost/algorithm/string.hpp>

// ****************************************************************
// 簡易暗号コード
// ================================================================
#include	"tnm_exe_angou.h"
BYTE tnm_gameexe_dat_angou_code[TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE] = TNM_GAMEEXE_DAT_ANGOU_CODE;

// ****************************************************************
// ＥＸＥセット暗号コード
// ================================================================
volatile extern BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード


// ****************************************************************
// Select.ini：情報を初期化
// ================================================================
C_tnm_select_ini::C_tnm_select_ini()
{
}

// ****************************************************************
// Select.ini：解析
// ================================================================

bool C_tnm_select_ini::analize()
{
	ARRAY<TSTR> ini_file_data;

	// Select.ini がある場合
	if (file_get_type(_T("Select.ini")) == 1)	{

		// 読み込み
		ARRAY<TSTR> file_data;
		if (!C_file::read_full_text(G_app.module_dir + _T("\\Select.ini"), file_data))
		{
			set_last_error(_T("Select.ini が開けませんでした。"), _T(""));
			return false;
		}
		if (file_data.empty())
		{
			set_last_error(_T("Select.ini が空です。"), _T(""));
			return false;
		}
		if (file_data[0].empty())
		{
			set_last_error(_T("Select.ini の１行目が空です。"), _T(""));
			return false;
		}

		// 行を解析
		for (int i = 0; i < (int)file_data.size(); i++)
		{
			ARRAY<TSTR> line_data;
			boost::split(line_data, file_data[i], boost::is_any_of(_T("\t")));

			if (!line_data.empty())
			{
				if (line_data.size() != 2)
				{
					set_last_error(_T("Select.ini の ") + tostr(i + 1) + _T(" 行目のパラメータが足りません。"), _T(""));
					return false;
				}

				C_item item;
				item.m_dir = line_data[0];
				item.m_name = line_data[1];

				m_append_list.push_back(item);
			}
		}
	}
	// Select.ini がない場合
	else
	{
		// 番兵を入れる
		C_item item;

		m_append_list.push_back(item);
	}

	return true;
}

// ****************************************************************
// Select.ini：先頭のディレクトリを取得
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
// Gameexe.ini：情報を初期化
// ================================================================
C_tnm_ini::C_tnm_ini()
{
	// ================================================================
	// gameexe.ini のデフォルト値をここで指定します。
	// ----------------------------------------------------------------

	// デバッグ
	debug_error_patno_out_of_range = 0;

	// システム
	game_id = _T("SampleProject");
	game_name = _T("サンプルプロジェクト");
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
	dummy_check_str = _T("（ゲーム名）のディスクを入れてください。\n\n※プロテクトの誤認識ではありません。");
	dummy_check_ok_str = _T("この度は（ゲーム名）をお買い上げいただき、ありがとうございます。");

	// フリックシーン
	flick_scene_cnt = 0;
	for (int i = 0; i < INIMIN_FLICK_SCENE_CNT; i++)
	{
		flick_scene_name[i] = _T("");
		flick_scene_z_no[i] = 0;
		flick_scene_angle[i] = 0;
	}

	// 環境変数
	system_int_value_cnt = 0;
	system_str_value_cnt = 0;
	for (int i = 0; i < INIMAX_SYSTEM_VALUE_CNT; i++)	{
		system_int_value[i] = 0;
		system_str_value[i].clear();
	}

	// ショートカットキー
	shortcut_cnt = INIDEF_SHORTCUT_CNT;
	for (int i = 0; i < INIMAX_SHORTCUT_CNT; i++)	{
		shortcut_key[i] = -1;
		shortcut_cond[i] = 0;
		shortcut_scn[i] = _T("");
		shortcut_z_no[i] = 0;
		shortcut_cmd[i] = _T("");
	}

// ▲▲▲メモ：殺る事リスト▲▲▲
// ▲名前の変換テーブル＋文字色
// ▲モーダル版の場合、アップデートしないと問題？（デバッグメニューの項目に問題点及び対応策を記述してあります）
// ▲バックログのスムーススクロール
// ▲Ｇ０１

	// アイコン
	icon_cnt = INIDEF_ICON_CNT;
	for (int i = 0; i < INIMAX_ICON_CNT; i++)	{
		icon[i].anime_speed = 100;
		icon[i].anime_pat_cnt = 1;
	}

	// マウスカーソル
	mouse_cursor_cnt = INIDEF_MOUSE_CURSOR_CNT;
	for (int i = 0; i < INIMAX_MOUSE_CURSOR_CNT; i++)	{
		mouse_cursor[i].anime_speed = 100;
	}
	mouse_cursor_default = -1;

	// システムコマンドメニュー：存在の有無
	syscommenu.read_skip.exist = true;								// ★：一度読んだ文章の早送りを開始します。
	syscommenu.unread_skip.exist = true;							// ★：未読の文章でも早送りを開始します。（★システム設定の「未読の文章も早送りできるようにする。」が有効の場合です）
	syscommenu.auto_skip.exist = true;								// ★：自動早送り機能をＯＮ／ＯＦＦします。
	syscommenu.auto_mode.exist = true;								// ★：オートモードを開始します。
	syscommenu.hide_mwnd.exist = true;								// ★：メッセージウィンドウを隠します。
	syscommenu.msg_back.exist = true;								// ★：メッセージバックを開きます。
	syscommenu.save.exist = true;									// ★：セーブを呼び出します。（#SAVE_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、セーブダイアログを表示します）
	syscommenu.load.exist = true;									// ★：ロードを呼び出します。（#LOAD_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、ロードダイアログを表示します）
	syscommenu.return_to_sel.exist = true;							// ★：前の選択肢に戻ります。
	syscommenu.config.exist = true;									// ★：環境設定を呼び出します。（#CONFIG_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、環境設定ダイアログを表示します）
	syscommenu.manual.exist = true;									// ★：マニュアルを表示します。（マニュアルへのパスは #MANUAL_PATH で設定して下さい）
	syscommenu.version.exist = true;								// ★：バージョン情報を表示します。（情報は #GAMEVERSION で設定して下さい）
	syscommenu.return_to_menu.exist = true;							// ★：タイトルメニューに戻ります。
	syscommenu.game_end.exist = true;								// ★：ゲームを終了します。
	syscommenu.cancel.exist = true;									// ★：システムコマンドメニューをキャンセルします。

	// システムコマンドメニュー：許可／禁止
	syscommenu.read_skip.enable = true;								// ★：一度読んだ文章の早送りを開始します。
	syscommenu.unread_skip.enable = true;							// ★：未読の文章でも早送りを開始します。（★システム設定の「未読の文章も早送りできるようにする。」が有効の場合です）
	syscommenu.auto_skip.enable = true;								// ★：自動早送り機能をＯＮ／ＯＦＦします。
	syscommenu.auto_mode.enable = true;								// ★：オートモードを開始します。
	syscommenu.hide_mwnd.enable = true;								// ★：メッセージウィンドウを隠します。
	syscommenu.msg_back.enable = true;								// ★：メッセージバックを開きます。
	syscommenu.save.enable = true;									// ★：セーブを呼び出します。（#SAVE_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、セーブダイアログを表示します）
	syscommenu.load.enable = true;									// ★：ロードを呼び出します。（#LOAD_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、ロードダイアログを表示します）
	syscommenu.return_to_sel.enable = true;							// ★：前の選択肢に戻ります。
	syscommenu.config.enable = true;								// ★：環境設定を呼び出します。（#CONFIG_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、環境設定ダイアログを表示します）
	syscommenu.manual.enable = true;								// ★：マニュアルを表示します。（マニュアルへのパスは #MANUAL_PATH で設定して下さい）
	syscommenu.version.enable = true;								// ★：バージョン情報を表示します。（情報は #GAMEVERSION で設定して下さい）
	syscommenu.return_to_menu.enable = true;						// ★：タイトルメニューに戻ります。
	syscommenu.game_end.enable = true;								// ★：ゲームを終了します。
	syscommenu.cancel.enable = true;								// ★：システムコマンドメニューをキャンセルします。

	// システムコマンドメニュー：文字列
	syscommenu.read_skip.str = LCL_STR_SYSCOMMENU_READ_SKIP_STR;			// ★：一度読んだ文章の早送りを開始します。
	syscommenu.unread_skip.str = LCL_STR_SYSCOMMENU_UNREAD_SKIP_STR;		// ★：未読の文章でも早送りを開始します。（★システム設定の「未読の文章も早送りできるようにする。」が有効の場合です）
	syscommenu.auto_skip.str = LCL_STR_SYSCOMMENU_AUTO_AKIP_STR;			// ★：自動早送り機能をＯＮ／ＯＦＦします。
	syscommenu.auto_mode.str = LCL_STR_SYSCOMMENU_AUTO_MODE_STR;			// ★：オートモードを開始します。
	syscommenu.hide_mwnd.str = LCL_STR_SYSCOMMENU_HIDE_MWND_STR;			// ★：メッセージウィンドウを隠します。
	syscommenu.msg_back.str = LCL_STR_SYSCOMMENU_MSG_BACK_STR;				// ★：メッセージバックを開きます。
	syscommenu.save.str = LCL_STR_SYSCOMMENU_SAVE_STR;						// ★：セーブを呼び出します。（#SAVE_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、セーブダイアログを表示します）
	syscommenu.load.str = LCL_STR_SYSCOMMENU_LOAD_STR;						// ★：ロードを呼び出します。（#LOAD_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、ロードダイアログを表示します）
	syscommenu.return_to_sel.str = LCL_STR_SYSCOMMENU_RETURN_SEL_STR;		// ★：前の選択肢に戻ります。
	syscommenu.config.str = LCL_STR_SYSCOMMENU_CONFIG_STR;					// ★：環境設定を呼び出します。（#CONFIG_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、環境設定ダイアログを表示します）
	syscommenu.manual.str = LCL_STR_SYSCOMMENU_MANUAL_STR;					// ★：マニュアルを表示します。（マニュアルへのパスは #MANUAL_PATH で設定して下さい）
	syscommenu.version.str = LCL_STR_SYSCOMMENU_VERSION_STR;				// ★：バージョン情報を表示します。（情報は #GAMEVERSION で設定して下さい）
	syscommenu.return_to_menu.str = LCL_STR_SYSCOMMENU_RETURN_MENU_STR;		// ★：タイトルメニューに戻ります。
	syscommenu.game_end.str = LCL_STR_SYSCOMMENU_GAME_END_STR;				// ★：ゲームを終了します。
	syscommenu.cancel.str = LCL_STR_SYSCOMMENU_CANCEL_STR;					// ★：システムコマンドメニューをキャンセルします。

	// システムコマンドメニュー：ローカル汎用スイッチ
	syscommenu.local_extra_switch_cnt = INIDEF_LOCAL_EXTRA_SWITCH_CNT;		// －：ローカル汎用スイッチの数
	for (int i = 0; i < INIMAX_LOCAL_EXTRA_SWITCH_CNT; i++)	{
		syscommenu.local_extra_switch[i].exist = (i < 4) ? true : false;	// ★：ローカル汎用スイッチ：存在の有無（★デフォルトで４つ存在する）
		syscommenu.local_extra_switch[i].enable = true;						// ★：ローカル汎用スイッチ：許可／禁止
		syscommenu.local_extra_switch[i].onoff = true;						// ★：ローカル汎用スイッチ：ON/OFF状態
		syscommenu.local_extra_switch[i].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_SWITCH_STR(i);	// ★：ローカル汎用スイッチ：モード名
	}

	// システムコマンドメニュー：ローカル汎用モード
	syscommenu.local_extra_mode_cnt = INIDEF_LOCAL_EXTRA_MODE_CNT;		// －：ローカル汎用モードの数
	for (int i = 0; i < INIMAX_LOCAL_EXTRA_MODE_CNT; i++)	{
		syscommenu.local_extra_mode[i].exist = (i < 4) ? true : false;		// ★：ローカル汎用モード：存在の有無（★デフォルトで４つ存在する）
		syscommenu.local_extra_mode[i].enable = true;						// ★：ローカル汎用モード：許可／禁止
		syscommenu.local_extra_mode[i].mode = 0;							// ★：ローカル汎用モード：モード値
		syscommenu.local_extra_mode[i].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_STR(i);	// ★：ローカル汎用モード：項目名
		syscommenu.local_extra_mode[i].item_cnt = INIDEF_LOCAL_EXTRA_MODE_ITEM_CNT;			// ★：ローカル汎用モードのアイテムの数
		for (int j = 0; j < INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT; j++)	{
			syscommenu.local_extra_mode[i].item[j].str = LCL_STR_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_STR(j);	// ★：ローカル汎用モードのアイテム：項目名
		}
	}

	// ユーザコンフィグ：ダイアログスタイル
	config.dlg_style_volume = 0;									// ★：音量ダイアログのスタイル（0=音量＆ＢＧＭフェード  1=音量のみ）
	config.dlg_style_koe = 0;										// ★：音声ダイアログのスタイル（0=音声モード＆キャラクター音声  1=音声モードのみ  2=キャラクター音声のみ）

	// ユーザコンフィグ：タブの存在の有無
	config.tab_exist_screen = true;									// ★：「画面」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_volume = true;									// ★：「音量」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_message = true;								// ★：「文章」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_mwndbk = true;									// ★：「背景色」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_koe = true;									// ★：「音声」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_automode = true;								// ★：「オートモード」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_jitan = true;									// ★：「時短再生」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_else = true;									// ★：「その他」タブの存在の有無（0=無し 1=有り）
	config.tab_exist_system = true;									// ★：「システム」タブの存在の有無（0=無し 1=有り）

	// ユーザコンフィグ：項目の存在の有無
	config.exist_bgm = true;										// ★：「ＢＧＭ」項目の存在の有無
	config.exist_koe = true;										// ★：「音声」項目の存在の有無
	config.exist_pcm = true;										// ★：「効果音」項目の存在の有無
	config.exist_se = true;											// ★：「システム音」項目の存在の有無
	config.exist_mov = true;										// ★：「ムービー」項目の存在の有無

	// ユーザコンフィグ：スクリーンサイズ
	config.screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;			// ★：スクリーンサイズのモード
	config.screen_size_scale = C_size(100, 100);						// －：スクリーンサイズのスケール

	// ユーザコンフィグ：フルスクリーン詳細
	config.fullscreen_mode = TNM_FULLSCREEN_MODE_FIT;				// －：フルスクリーンの表示モード
	config.fullscreen_scale = C_size(100, 100);						// －：フルスクリーンのスケール
	config.fullscreen_scale_sync_switch = true;						// －：フルスクリーンのスケールの同期スイッチ
	config.fullscreen_move = C_size(0, 0);							// －：フルスクリーンの移動

	// ユーザコンフィグ：音量
	config.all_user_volume = 255;									// ★：全体ボリューム
	config.bgm_user_volume = 255;									// ★：BGM ボリューム
	config.koe_user_volume = 255;									// ★：KOE ボリューム
	config.pcm_user_volume = 255;									// ★：PCM ボリューム
	config.se_user_volume = 255;									// ★：SE ボリューム
	config.mov_user_volume = 255;									// ★：MOV ボリューム
	config.play_all_check = true;									// －：全体再生チェック
	config.play_bgm_check = true;									// －：BGM 再生チェック
	config.play_koe_check = true;									// －：KOE 再生チェック
	config.play_pcm_check = true;									// －：PCM 再生チェック
	config.play_se_check = true;									// －：SE 再生チェック
	config.play_mov_check = true;									// －：MOV 再生チェック

	// ユーザコンフィグ：ＢＧＭフェード
	config.bgmfade_volume = 192;									// ★：ＢＧＭフェードボリューム
	config.bgmfade_use_check = true;								// ★：ＢＧＭフェードチェック

	// ユーザコンフィグ：フィルター色
	config.filter_color = C_argb(128, 0, 0, 0);						// ★：フィルター色

	// ユーザコンフィグ：フォント
	config.font_name = TNM_FONT_NAME_MS_GOTHIC;						// ★：フォント：名前（★名前を直接指定するのではなく、タイプで指定します）
	config.font_type = TNM_FONT_TYPE_MS_GOTHIC;						// ★：フォント：種類（0=ＭＳゴシック 1=ＭＳ明朝 2=メイリオ 3=独自フォント）
	config.font_futoku = false;										// ★：フォント：太くする（0=OFF 1=ON）
	config.font_shadow = TNM_FONT_SHADOW_MODE_FUTIDORU;				// ★：フォント：影モード（0=影なし 1=影あり 2=縁取る）
	config.font_sample_str_short = LCL_STR_FONT_DIALOG_SAMPLE_STR_SHORT;	// ★：フォント：サンプル文字列ショート
	config.font_sample_str_long  = LCL_STR_FONT_DIALOG_SAMPLE_STR_LONG;		// ★：フォント：サンプル文字列ロング

	// ユーザコンフィグ：文字速度
	config.message_speed = 20;										// ★：文字速度
	config.message_speed_nowait = false;							// ★：文字速度（ノーウェイト）

	// ユーザコンフィグ：オートモード
	config.auto_mode_onoff = false;									// －：オートモード
	config.auto_mode_moji_wait = 70;								// －：オートモード文字時間
	config.auto_mode_min_wait = 300;								// －：オートモード最小時間

	// ユーザコンフィグ：自動でマウスカーソルを隠す
	config.mouse_cursor_hide_onoff = false;							// －：自動でマウスカーソルを消す
	config.mouse_cursor_hide_time = 5000;							// －：自動でマウスカーソルを消す

	// ユーザコンフィグ：時短再生
	config.jitan_normal_onoff = false;								// －：時短再生（文章を普通に読み進めている時）
	config.jitan_auto_mode_onoff = false;							// －：時短再生（オートモード中）
	config.jitan_msgbk_onoff = false;								// －：時短再生（文章の巻き戻し中）
	config.jitan_speed = 100;										// －：時短再生速度

	// ユーザコンフィグ：音声モード
	config.koe_mode = TNM_KOE_MODE_NORMAL;							// －：音声モード

	// ユーザコンフィグ：キャラクター音声
	config.chrkoe_cnt = INIDEF_CHRKOE_CNT;
	config.chrkoe_not_look_name_str = _T("？？？");					// ★：伏字用の文字列
	for (int i = 0; i < INIMAX_CHRKOE_CNT; i++)	{
		config.chrkoe[i].name_str = _T("");							// ★：名前
		config.chrkoe[i].check_mode = TNM_CHRKOE_CHECK_MODE_LOOK;	// ★：チェックモード（0=伏字状態でチェックしない  1=公開状態でチェックしない  2=伏字状態でチェックする）
		config.chrkoe[i].check_name_str = _T("");					// ★：チェック用の名前
		config.chrkoe[i].onoff = true;								// －：ON/OFF状態
		config.chrkoe[i].volume = 255;								// －：音量
		config.chrkoe[i].chr_no_list.clear();						// ★：声の制御番号リスト
	}

	// ユーザコンフィグ：文章の色分け
	config.message_chrcolor.exist = true;							// ★：文章の色分け
	config.message_chrcolor.onoff = true;							// ★：文章の色分け
	config.message_chrcolor.str = _T("文章を色分けする。");			// ★：文章の色分け

	// ユーザコンフィグ：オブジェクトの表示フラグ
	config.object_disp_cnt = INIDEF_OBJECT_DISP_FLAG_CNT;			// －：オブジェクトの表示フラグの数
	for (int i = 0; i < INIMAX_OBJECT_DISP_FLAG_CNT; i++)	{
		config.object_disp[i].exist =  (i < 2) ? true : false;		// ★：オブジェクトの表示フラグ（★デフォルトで２つ存在する）
		config.object_disp[i].onoff = true;							// ★：オブジェクトの表示フラグ
		config.object_disp[i].str = _T("オブジェクト表示") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("番を表示する。");		// ★：オブジェクトの表示フラグ
	}

	// ユーザコンフィグ：グローバル汎用スイッチ
	config.global_extra_switch_cnt = INIDEF_GLOBAL_EXTRA_SWITCH_CNT;	// －：グローバル汎用スイッチの数
	for (int i = 0; i < INIMAX_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		config.global_extra_switch[i].exist = (i < 2) ? true : false;	// ★：グローバル汎用スイッチ：存在の有無（★デフォルトで２つ存在する）
		config.global_extra_switch[i].onoff = true;						// ★：グローバル汎用スイッチ：ON/OFF状態
		config.global_extra_switch[i].str = _T("グローバル汎用スイッチ") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("番を使用する。");		// ★：グローバル汎用スイッチ：項目名
	}

	// ユーザコンフィグ：グローバル汎用モード
	config.global_extra_mode_cnt = INIDEF_GLOBAL_EXTRA_MODE_CNT;		// －：グローバル汎用モードの数
	for (int i = 0; i < INIMAX_GLOBAL_EXTRA_MODE_CNT; i++)	{
		config.global_extra_mode[i].exist = true;						// ★：グローバル汎用モード：存在の有無
		config.global_extra_mode[i].mode = 0;							// ★：グローバル汎用モード：モード値
		config.global_extra_mode[i].str = _T("グローバル汎用モード") + str_to_zenkaku_ret(str_format(_T("%d"), i)) + _T("番");			// ★：グローバル汎用モード：項目名
		switch (i)	{
			case 0:
			case 1:
				config.global_extra_mode[i].item_cnt = 3;				// ★：グローバル汎用モードのアイテムの数（★デフォルトで３つ存在する）
				break;
			default:
				config.global_extra_mode[i].item_cnt = 1;				// ★：グローバル汎用モードのアイテムの数（★デフォルトで１つ存在する）
				break;
		}
		for (int j = 0; j < INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT; j++)	{
			config.global_extra_mode[i].item[j].str = _T("モード") + str_to_zenkaku_ret(str_format(_T("%d"), j));		// ★：グローバル汎用モードのアイテム：項目名
		}
	}

	// ユーザコンフィグ：システム設定
	// （存在の有無）
	config.system.sleep.exist = true;								// ★：本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。
	config.system.no_wipe_anime.exist = true;						// ★：画面暗転効果のアニメを無効にする。
	config.system.skip_wipe_anime.exist = true;						// ★：画面暗転効果をマウスクリックで飛ばす。
	config.system.no_mwnd_anime.exist = true;						// ★：メッセージウィンドウの開閉時のアニメを無効にする。
	config.system.wheel_next_message.exist = true;					// ★：マウスのホイールボタンの下回しで文章を読み進める。
	config.system.koe_dont_stop.exist = true;						// ★：声の再生中に次の文章に進んでも再生を続ける。
	config.system.skip_unread_message.exist = true;					// ★：未読の文章も早送りできるようにする。
	config.system.play_silent_sound.exist = true;					// ★：サウンド再生時に雑音が入る場合はチェックして下さい。
	// （初期状態）
	config.system.sleep.onoff = false;								// ★：本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。
	config.system.no_wipe_anime.onoff = false;						// ★：画面暗転効果のアニメを無効にする。
	config.system.skip_wipe_anime.onoff = true;						// ★：画面暗転効果をマウスクリックで飛ばす。
	config.system.no_mwnd_anime.onoff = false;						// ★：メッセージウィンドウの開閉時のアニメを無効にする。
	config.system.wheel_next_message.onoff = true;					// ★：マウスのホイールボタンの下回しで文章を読み進める。
	config.system.koe_dont_stop.onoff = false;						// ★：声の再生中に次の文章に進んでも再生を続ける。
	config.system.skip_unread_message.onoff = false;				// ★：未読の文章も早送りできるようにする。
	// （文字列）
	config.system.sleep.str = LCL_STR_SYSTEM_DIALOG_SLEEP_STR;								// ★：本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。
	config.system.no_wipe_anime.str = LCL_STR_SYSTEM_DIALOG_NO_WIPE_ANIME_STR;				// ★：画面暗転効果のアニメを無効にする。
	config.system.skip_wipe_anime.str = LCL_STR_SYSTEM_DIALOG_SKIP_WIPE_ANIME_STR;			// ★：画面暗転効果をマウスクリックで飛ばす。
	config.system.no_mwnd_anime.str = LCL_STR_SYSTEM_DIALOG_NO_MWND_ANIME_STR;				// ★：メッセージウィンドウの開閉時のアニメを無効にする。
	config.system.wheel_next_message.str = LCL_STR_SYSTEM_DIALOG_WHEEL_NEXT_MESSAGE_STR;	// ★：マウスのホイールボタンの下回しで文章を読み進める。
	config.system.koe_dont_stop.str = LCL_STR_SYSTEM_DIALOG_KOE_DONT_STOP_STR;				// ★：声の再生中に次の文章に進んでも再生を続ける。
	config.system.skip_unread_message.str = LCL_STR_SYSTEM_DIALOG_SKIP_UNREAD_MESSAGE_STR;	// ★：未読の文章も早送りできるようにする。
	config.system.play_silent_sound.str = LCL_STR_SYSTEM_DIALOG_PLAY_SILENT_SOUND_STR;		// ★：サウンド再生時に雑音が入る場合はチェックして下さい。

	// ユーザコンフィグ：システム設定（ムービーの再生方法）
	config.system.movie_play_type = TNM_MOVIE_PLAY_TYPE_WMP;		// －：ムービーの再生方法

	// ユーザコンフィグ：システム設定（セーブロードの警告）
	config.system.saveload_alert_onoff = true;						// －：セーブロードの警告

	// ユーザコンフィグ：システム設定（セーブロードの警告）
	config.system.saveload_dblclick_onoff = false;					// －：セーブロードのダブルクリック

	// ユーザコンフィグ：セーブロードダイアログ
	config.saveload_dialog.data_cnt_par_page = INIDEF_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE;					// ★：１ページ（１タブ）あたりのデータ数

	// ユーザコンフィグ：セーブのインフォメーション情報
	config.save_info.dlgwnd_caption_title_str = LCL_STR_SAVE_DIALOG_CAPTION_TITLE_STR;						// ★：セーブダイアログのキャプションタイトルの文字列
	config.save_info.dlgwnd_deside_button_str = LCL_STR_SAVE_DIALOG_DECIDE_BUTTON_STR;						// ★：セーブダイアログの決定ボタンの文字列
	config.save_info.dlgwnd_datalist_nameheader_str = LCL_STR_SAVE_DIALOG_DATALIST_NAMEHEADER_STR;			// ★：セーブダイアログのデータリストの先頭文字列
	config.save_info.dlgwnd_warning_chkbox_str = LCL_STR_SAVE_DIALOG_WARNING_CHECKBOX_STR;					// ★：セーブダイアログの確認チェックボックスの文字列
	config.save_info.dlgwnd_dblclick_chkbox_str = LCL_STR_SAVE_DIALOG_DBLCLK_CHECKBOX_STR;					// ★：セーブダイアログのダブルクリックチェックボックスの文字列
	config.save_info.warning_str = LCL_STR_CONFIG_SAVE_WARNING_STR;											// ★：セーブ確認の確認文章の文字列
	config.save_info.quick_warning_str = LCL_STR_CONFIG_QUICK_SAVE_WARNING_STR;								// ★：クイックセーブ確認の確認文章の文字列
	config.save_info.msgbk_warning_str = _T("");															// ★：なし

	// ユーザコンフィグ：ロードのインフォメーション情報
	config.load_info.dlgwnd_caption_title_str = LCL_STR_LOAD_DIALOG_CAPTION_TITLE_STR;						// ★：ロードダイアログのキャプションタイトルの文字列
	config.load_info.dlgwnd_deside_button_str = LCL_STR_LOAD_DIALOG_DECIDE_BUTTON_STR;						// ★：ロードダイアログの決定ボタンの文字列
	config.load_info.dlgwnd_datalist_nameheader_str = LCL_STR_LOAD_DIALOG_DATALIST_NAMEHEADER_STR;			// ★：ロードダイアログのデータリストの先頭文字列
	config.load_info.dlgwnd_warning_chkbox_str = LCL_STR_LOAD_DIALOG_WARNING_CHECKBOX_STR;					// ★：ロードダイアログの確認チェックボックスの文字列
	config.load_info.dlgwnd_dblclick_chkbox_str = LCL_STR_LOAD_DIALOG_DBLCLK_CHECKBOX_STR;					// ★：ロードダイアログのダブルクリックチェックボックスの文字列
	config.load_info.warning_str = LCL_STR_CONFIG_LOAD_WARNING_STR;											// ★：ロード確認の確認文章の文字列
	config.load_info.quick_warning_str = LCL_STR_CONFIG_QUICK_LOAD_WARNING_STR;								// ★：クイックロード確認の確認文章の文字列
	config.load_info.msgbk_warning_str = LCL_STR_CONFIG_MSGBK_LOAD_WARNING_STR;								// ★：メッセージバックロード確認の確認文章の文字列

	// ユーザコンフィグ：各種確認メッセージの文字列
	config.warninginfo.restart_warning_str = LCL_STR_CONFIG_RESTART_WARNING_STR;							// ★：ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
	config.warninginfo.scenestart_warning_str = LCL_STR_CONFIG_SCENESTART_WARNING_STR;						// ★：ゲームを途中から始める時の確認メッセージの文字列（デバッグ機能）
	config.warninginfo.returnmenu_warning_str = LCL_STR_CONFIG_RETURNMENU_WARNING_STR;						// ★：タイトルメニューに戻る時の確認メッセージの文字列
	config.warninginfo.returnsel_warning_str = LCL_STR_CONFIG_RETURNSEL_WARNING_STR;						// ★：前の選択肢に戻る時の確認メッセージの文字列
	config.warninginfo.gameend_warning_str = LCL_STR_CONFIG_GAMEEND_WARNING_STR;							// ★：ゲームを終了する時の確認メッセージの文字列

	// セーブデータ数
	save_cnt = INIDEF_SAVE_CNT;

	// クイックセーブデータ数
	quick_save_cnt = INIDEF_QUICK_SAVE_CNT;

	// エンドセーブ
	end_save_cnt = INIDEF_END_SAVE_CNT;

	// セーブポイント履歴のデータ数
	save_history_cnt = INIDEF_SAVE_HISTORY_CNT;

	// バックログセーブのデータ数
	backlog_save_cnt = INIDEF_BACKLOG_SAVE_CNT;
	backlog_save_interval = 1;

	// 前の選択肢に戻るの履歴数
	sel_save_cnt = INIDEF_SEL_SAVE_CNT;

	// インナーセーブデータ数
	inner_save_cnt = INIDEF_INNER_SAVE_CNT;

	// 存在しないセーブデータの文字列
	save_no_data_str = LCL_STR_SAVELOAD_DIALOG_NO_DATA_STR;

	// セーブサムネイル
	use_save_thumb = false;
	save_thumb_type = TNM_SAVE_THUMB_TYPE_BMP;
	save_thumb_size = C_size(200, 150);

	// ロード時のワイプ
	load_wipe_type = 0;
	load_wipe_time = 1000;

	// 縦書き／横書き
	tateyoko_mode = 0;

	// 【名前】
	namae_list.clear();

	// フラグ
	flag_cnt = INIDEF_FLAG_CNT;
	global_flag_cnt = INIDEF_GLOBAL_FLAG_CNT;
	call_flag_cnt = INIDEF_CALL_FLAG_CNT;

	// データベース
	database_cnt = INIDEF_DATABASE_CNT;
	for (int i = 0; i < INIMAX_DATABASE_CNT; i++)	{
		database_name[i].clear();
	}

	// カウンタ
	counter_cnt = INIDEF_COUNTER_CNT;

	// カラーテーブル
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

	// トーンカーブ
	tonecurve_file = _T("");

	// ＣＧテーブル
	cg_table_file = _T("");
	cg_table_flag_cnt = INIDEF_CG_TABLE_FLAG_CNT;

	// Ｇ００バッファ
	g00_buf_cnt = INIDEF_G00_BUF_CNT;

	// マスク
	mask_cnt = INIDEF_MASK_CNT;

	// 絵文字
	emoji_cnt = INIDEF_EMOJI_CNT;
	for (int i = 0; i < INIMAX_EMOJI_CNT; i++)	{
		emoji[i].file_name = _T("");
		emoji[i].font_size = 0;
	}

	// フォントファイル
	font_file_cnt = INIDEF_FONT_FILE_CNT;
	for (int i = 0; i < INIMAX_FONT_FILE_CNT; i++)	{
		font_file[i].file_name = _T("");
	}
	private_font_file_cnt = INIDEF_FONT_FILE_CNT;
	for (int i = 0; i < INIMAX_FONT_FILE_CNT; i++)	{
		private_font_file[i].file_name = _T("");
	}

	// システムコール
	msg_back_order = INIDEF_MSG_BACK_ORDER;
	excall_order = INIDEF_EXCALL_ORDER;

	// ワールド
	world_cnt = INIDEF_WORLD_CNT;
	for (int i = 0; i < INIMAX_WORLD_CNT; i++)	{
		world[i].layer = 0;
	}

	// ボタン：アクション
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

	// ボタン：効果音
	button.se_cnt = INIDEF_BTN_SE_CNT;
	for (int i = 0; i < INIMAX_BTN_SE_CNT; i++)	{
		button.se[i].hit_no = 0;
		button.se[i].push_no = -1;
		button.se[i].decide_no = 1;
	}

	// ボタン：グループ
	button.group_cnt = INIDEF_BTN_GROUP_CNT;

	// メッセージボタン
	msgbtn_cnt = INIDEF_MSGBTN_CNT;
	for (int i = 0; i < INIMAX_MSGBTN_CNT; i++)	{
		for (int j = 0; j < TNM_BTN_STATE_MAX; j++)	{
			msgbtn[i].state[j].color_no = 0;
		}

		msgbtn[i].state[TNM_BTN_STATE_NORMAL].color_no = 0;
		msgbtn[i].state[TNM_BTN_STATE_HIT].color_no = 2;
		msgbtn[i].state[TNM_BTN_STATE_PUSH].color_no = 2;
	}

	// オブジェクト
	object_cnt = INIDEF_OBJECT_CNT;
	for (int i = 0; i < INIMAX_OBJECT_CNT; i++)	{
		object[i].use = true;
		object[i].save = true;
		object[i].space_hide = false;
		object[i].object_disp_no = -1;	// 従わない
	}

	// メッセージウィンドウ枠
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

		// メッセージウィンドウ枠：ボタン
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

		// メッセージウィンドウ枠：顔グラ
		for (int f = 0; f < INIMAX_MWND_WAKU_FACE_CNT; f++)	{
			mwnd_waku[i].face_pos[f] = C_point(0, 0);
		}
	}

	// メッセージウィンドウ
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

	// ボタン選択肢
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

	// メッセージバック
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

	// エフェクト
	effect_cnt = INIDEF_EFFECT_CNT;

	// シェイク
	shake_cnt = INIDEF_SHAKE_CNT;
	for (int i = 0; i < INIMAX_SHAKE_CNT; i++)	{
	}

	// クエイク
	quake_cnt = INIDEF_QUAKE_CNT;

	// フレームアクションチャンネル
	frame_action_ch_cnt = INIDEF_FRAME_ACTION_CH_CNT;

	// ＢＧＭ
	bgm_cnt = INIDEF_BGM_CNT;
	for (int i = 0; i < INIMAX_BGM_CNT; i++)	{
		bgm[i].regist_name = _T("");
		bgm[i].file_name = _T("");
		bgm[i].start_pos = 0;
		bgm[i].end_pos = -1;
		bgm[i].repeat_pos = 0;
	}

	// 効果音チャンネル数
	pcmch_cnt = INIDEF_PCMCH_CNT;

	// 効果音イベント数
	pcm_event_cnt = INIDEF_PCM_EVENT_CNT;

	// システム音
	se_cnt = INIDEF_SE_CNT;
	for (int i = 0; i < INIMAX_SE_CNT; i++)	{
	}

	// ＢＧＭフェード２
	bgmfade2_in_start_time = INIDEF_BGMFADE2_IN_START_TIME;
	bgmfade2_in_time_len = INIDEF_BGMFADE2_IN_TIME_LEN;
	bgmfade2_out_start_time = INIDEF_BGMFADE2_OUT_START_TIME;
	bgmfade2_out_time_len = INIDEF_BGMFADE2_OUT_TIME_LEN;
	bgmfade2_volume = INIDEF_BGMFADE2_VOLUME;

	// エディットボックス
	editbox_cnt = INIDEF_EDITBOX_CNT;
}

// ****************************************************************
// Gameexe.ini：解析
// ================================================================
bool C_tnm_ini::analize()
{
	bool ret = analize_func();

	line_data.clear();

	// フォントを補正
	{
		// フォントのファイル名を設定
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
		case TNM_FONT_TYPE_ORIGINAL:	// 設定済み
			break;
		default:
			set_last_error(_T("デフォルトのフォントタイプが不正です。"), _T(""));
			return false;
		}

		// 縦書きの場合
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

	// 読み込み
	BUFFER ged_data;
	if (!C_file::read_full_data(G_app.module_dir + _T("\\") + Gp_select_ini->first_dir() + _T("\\") + LCL_GAMEEXE_DAT, ged_data))	{
		set_last_error(LCL_GAMEEXE_DAT _T(" が開けませんでした。"), _T(""));
		return false;
	}

	// 圧縮データへのポインタ
	S_tnm_gameexe_dat_header *header = (S_tnm_gameexe_dat_header *)ged_data.get();
	BYTE *sp = ged_data.get() + sizeof(S_tnm_gameexe_dat_header);
	int size = (int)ged_data.size() - sizeof(S_tnm_gameexe_dat_header);

	if (size)	{

		// ＥＸＥセット暗号解除
		BYTE *dp;
		int eac;
		if (header->exe_angou_mode)	{

			// ＥＸＥセット暗号のコードを取得
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

		// 簡易暗号解除
		dp = sp;
		eac = 0;
		for (int i = 0; i < size; i++)	{
			*dp++ ^= tnm_gameexe_dat_angou_code[eac++];
			eac %= TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE;
		}

		// 解凍
		size = lzss_unpack(sp, NULL);
		BUFFER ged_original;
		ged_original.resize(size);
		dp = ged_original.get();
		lzss_unpack(sp, dp);

		// 行分割
		TSTR ged_text((TCHAR *)ged_original.get(), (TCHAR *)ged_original.get_end());
		separate_text_line(ged_text, ini_file_data);
	}

// ▲▲▲メモ：ＩＮＩ関連▲▲▲

// ▲コンパイラの Gameexe.ini のコメントカットをもう少し綺麗にしたい。

// ▲未使用
// INIMAX_MWND_SET_CNT

// ▲この項目名は正しいのでしょうか？（※このままでＯＫ？）
// "#OBJBTNGROUP . CNT = %d" → "#BUTTON . GROUP . CNT = %d"

// ▲最小値、最大値、デフォルト値の定義がないです。なのでリミッターチェックもしてないです。（※枠の設定に移動予定）
// #MWND.FACE_CNT

// ▲最大の確保数が適正かどうか精査する事。
// ▲項目によっては最小値も必要？例えばカラーテーブルを０にしたらどうなるん？（※最小値の設定は作ったので、値を設定して下さい）

	for (line_no = 0; line_no < (int)ini_file_data.size(); line_no ++)	{
		line_data = ini_file_data[line_no];
		C_str_analizer anl(line_data.begin(), line_data.end());

		if (!analize_step_1(anl, line_no))
			return false;
	}

	return true;
}

// ****************************************************************
// Gameexe.ini：解析：ステップ１
// ================================================================
bool C_tnm_ini::analize_step_1(C_str_analizer& anl, int line_no)
{
	int num[16] = {0};
	TSTR str[16];

	if (false);
	// システム
	else if (anl.is_end())	{
		// NOP
	}
	else if (anl.get_format(_T("//")))	{
		// コメントなので NOP
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
	// 環境変数
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
	// フリックシーン
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

	// ショートカットキー
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
	// アイコン
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
	// マウスカーソル
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
	// コンフィグ初期値の設定
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
	// システムコマンドメニュー：存在の有無／文字列
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
		// システムコマンドメニュー：ローカル汎用スイッチ
		else if (anl.get_format(_T("LOCAL_EXTRA_SWITCH . %d = %d , %d , %d , %s"), &num[0], &num[1], &num[2], &num[3], &str[0]))	{
			if (!analize_check_over(num[0], syscommenu.local_extra_switch_cnt, INIMAX_LOCAL_EXTRA_SWITCH_CNT))	{	return false;	}
			syscommenu.local_extra_switch[num[0]].exist = (num[1]) ? true : false;
			syscommenu.local_extra_switch[num[0]].enable = (num[2]) ? true : false;
			syscommenu.local_extra_switch[num[0]].onoff = (num[3]) ? true : false;
			syscommenu.local_extra_switch[num[0]].str = str[0];
		}
		// システムコマンドメニュー：ローカル汎用モード
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
	// ダイアログウィンドウのスタイル
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
	// ダイアログウィンドウのタブの存在の有無
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
	// ダイアログウィンドウの項目の存在の有無
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
	// ダイアログウィンドウの項目状態の設定
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
	// 各種確認メッセージの文字列
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
	// セーブロードダイアログ
	else if (anl.get_format(_T("#SAVELOAD_DIALOG . ")))	{
		if (false);
		else if (anl.get_format(_T("DATA_CNT_PAR_PAGE = %d"), &num[0]))	{
			if (!analize_check_alloc(num[0], INIMIN_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE, INIMAX_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE))	{	return false;	}
			config.saveload_dialog.data_cnt_par_page = num[0];
		}
		else	{	return error_dont_analize();	}
	}
	// セーブのインフォメーション情報
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
	// ロードのインフォメーション情報
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
	// セーブデータ数
	else if (anl.get_format(_T("#SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SAVE_CNT, INIMAX_SAVE_CNT))	{	return false;	}
		save_cnt = num[0];
	}
	// クイックセーブデータ数
	else if (anl.get_format(_T("#QUICK_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_QUICK_SAVE_CNT, INIMAX_QUICK_SAVE_CNT))	{	return false;	}
		quick_save_cnt = num[0];
	}
	// エンドセーブデータ数
	else if (anl.get_format(_T("#END_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_END_SAVE_CNT, INIMAX_END_SAVE_CNT))	{	return false;	}
		end_save_cnt = num[0];
	}
	// インナーセーブデータ数
	else if (anl.get_format(_T("#INNER_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_INNER_SAVE_CNT, INIMAX_INNER_SAVE_CNT))	{	return false;	}
		inner_save_cnt = num[0];
	}
	// バックログセーブデータ数
	else if (anl.get_format(_T("#MESSAGE_BACK_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BACKLOG_SAVE_CNT, INIMAX_BACKLOG_SAVE_CNT))	{	return false;	}
		backlog_save_cnt = num[0];
	}
	else if (anl.get_format(_T("#MESSAGE_BACK_SAVE . INTERVAL = %d"), &num[0]))	{
		backlog_save_interval = num[0];
	}
	// セーブポイント履歴数
	else if (anl.get_format(_T("#SAVE_HISTORY . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SAVE_HISTORY_CNT, INIMAX_SAVE_HISTORY_CNT))	{	return false;	}
		save_history_cnt = num[0];
	}
	// 前の選択肢に戻るの履歴数
	else if (anl.get_format(_T("#SEL_SAVE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_SEL_SAVE_CNT, INIMAX_SEL_SAVE_CNT))	{	return false;	}
		sel_save_cnt = num[0];
	}
	// 存在しないセーブデータの文字列
	else if (anl.get_format(_T("#SAVE . NO_DATA_STR = %s"), &str[0]))	{
		save_no_data_str = str[0];
	}
	// セーブサムネイル
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
	// ロード時のワイプ
	else if (anl.get_format(_T("#LOAD . WIPE = %d , %d"), &num[0], &num[1]))	{
		load_wipe_type = num[0];
		load_wipe_time = num[1];
	}
	// キャラクター音声
	// ▲：名前と公開フラグのスクリプト制御
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

		// オプション（なくてもＯＫ）
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
					return error(tostr(kakko_cnt) + _T(" 個目の括弧のパラメータが不正です。"));
				}
				if (anl.check_moji(_T(')')))	{
					break;	// 正常終了
				}
				else if (!anl.check_moji(_T(',')))	{
					return error_dont_analize();
				}
				kakko_cnt ++;
			}
		}
		else	{
			return error(_T("パラメータがありません。"));
		}
	}
	else	{
		return analize_step_2(anl, line_no);
	}

	return true;
}

// ****************************************************************
// Gameexe.ini：解析：ステップ２
// ================================================================
bool C_tnm_ini::analize_step_2(C_str_analizer& anl, int line_no)
{
	int num[16] = {0};
	TSTR str[16];

	if (false);

	// 縦書き／横書き
	else if (anl.get_format(_T("#TATEYOKO_MODE = %d"), &num[0]))	{
		tateyoko_mode = num[0];
	}
	// 【名前】
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
	// フラグ
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
	// データベース
	else if (anl.get_format(_T("#DATABASE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_DATABASE_CNT, INIMAX_DATABASE_CNT))	{	return false;	}
		database_cnt = num[0];
	}
	else if (anl.get_format(_T("#DATABASE . %d = %s"), &num[0], &str[0]))	{
		if (!analize_check_over(num[0], database_cnt, INIMAX_DATABASE_CNT))	{	return false;	}
		database_name[num[0]] = str[0];
	}
	// カウンタ
	else if (anl.get_format(_T("#COUNTER . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_COUNTER_CNT, INIMAX_COUNTER_CNT))	{	return false;	}
		counter_cnt = num[0];
	}
	// カラーテーブル
	else if (anl.get_format(_T("#COLOR_TABLE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_COLOR_TABLE_CNT, INIMAX_COLOR_TABLE_CNT))	{	return false;	}
		color_table_cnt = num[0];
	}
	else if (anl.get_format(_T("#COLOR_TABLE . %d = %d , %d , %d"), &num[0], &num[1], &num[2], &num[3]))	{
		if (!analize_check_over(num[0], color_table_cnt, INIMAX_COLOR_TABLE_CNT))	{	return false;	}
		color_table[num[0]] = C_rgb((BYTE)num[1], (BYTE)num[2], (BYTE)num[3]);
	}
	// トーンカーブ
	else if (anl.get_format(_T("#TONECURVE_FILE = %s"), &str[0]))	{
		tonecurve_file = str[0];
	}
	// ＣＧテーブル
	else if (anl.get_format(_T("#CGTABLE_FILE = %s"), &str[0]))	{
		cg_table_file = str[0];
	}
	else if (anl.get_format(_T("#CGTABLE_FLAG_CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_CG_TABLE_FLAG_CNT, INIMAX_CG_TABLE_FLAG_CNT))	{	return false;	}
		cg_table_flag_cnt = num[0];
	}
	// サムネイルテーブル
	else if (anl.get_format(_T("#THUMBTABLE_FILE = %s"), &str[0]))	{
		thumb_table_file = str[0];
	}
	// Ｇ００バッファ
	else if (anl.get_format(_T("#G00BUF . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_G00_BUF_CNT, INIMAX_G00_BUF_CNT))	{	return false;	}
		g00_buf_cnt = num[0];
	}
	// マスク
	else if (anl.get_format(_T("#MASK . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_MASK_CNT, INIMAX_MASK_CNT))	{	return false;	}
		mask_cnt = num[0];
	}
	// 絵文字
	else if (anl.get_format(_T("#EMOJI . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EMOJI_CNT, INIMAX_EMOJI_CNT))	{	return false;	}
		emoji_cnt = num[0];
	}
	else if (anl.get_format(_T("#EMOJI . %d = %s , %d"), &num[0], &str[0], &num[1]))	{
		if (!analize_check_over(num[0], emoji_cnt, INIMAX_EMOJI_CNT))	{	return false;	}
		emoji[num[0]].file_name = str[0];
		emoji[num[0]].font_size = num[1];
	}
	// フォントファイル
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
	// システムコール
	else if (anl.get_format(_T("#EXCALL . ORDER = %d"), &num[0]))	{
		excall_order = num[0];
	}
	// ボタン：アクション
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
	// ボタン：効果音
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
	// ボタン：グループ
	else if (anl.get_format(_T("#OBJBTNGROUP . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_BTN_GROUP_CNT, INIMAX_BTN_GROUP_CNT))	{	return false;	}
		button.group_cnt = num[0];
	}
	// メッセージボタン
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
	// ワールド
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
	// オブジェクト
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
	// メッセージウィンドウ枠
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
	// メッセージウィンドウ
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
	// ボタン選択肢
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
	// メッセージバック
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
	// ＢＧＭ
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
				return error(_T("終了位置が 0 になっています。ファイルの最後まで再生する場合は -1 を指定して下さい。"));
			}
		}
		else	{	return error_dont_analize();	}
	}
	// 効果音チャンネル数
	else if (anl.get_format(_T("#PCMCH . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_PCMCH_CNT, INIMAX_PCMCH_CNT))	{	return false;	}
		pcmch_cnt = num[0];
	}
	// 効果音イベント数
	else if (anl.get_format(_T("#PCMEVENT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_PCM_EVENT_CNT, INIMAX_PCM_EVENT_CNT))	{	return false;	}
		pcm_event_cnt = num[0];
	}
	// システム音
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
	// エフェクト
	else if (anl.get_format(_T("#EFFECT . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EFFECT_CNT, INIMAX_EFFECT_CNT))	{	return false;	}
		effect_cnt = num[0];
	}
	// シェイク
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
					return error(tostr(kakko_cnt) + _T(" 個目の括弧のパラメータが不正です。"));
				}
				if (!anl.check_moji(_T('(')))	{
					break;	// 正常終了
				}
				kakko_cnt ++;
			}
		}
		else	{
			return error(_T("パラメータがありません。"));
		}
	}
	// クエイク
	else if (anl.get_format(_T("#QUAKE . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_QUAKE_CNT, INIMAX_QUAKE_CNT))	{	return false;	}
		quake_cnt = num[0];
	}
	// フレームアクションチャンネル
	else if (anl.get_format(_T("#FRAME_ACTION_CH . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_FRAME_ACTION_CH_CNT, INIMAX_FRAME_ACTION_CH_CNT))	{	return false;	}
		frame_action_ch_cnt = num[0];
	}
	// ＢＧＭフェード２
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
	// エディットボックス
	else if (anl.get_format(_T("#EDITBOX . CNT = %d"), &num[0]))	{
		if (!analize_check_alloc(num[0], INIMIN_EDITBOX_CNT, INIMAX_EDITBOX_CNT))	{	return false;	}
		editbox_cnt = num[0];
	}
	// ツイッター
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
	// 解析エラー
	else	{
		return error_dont_analize();
	}

	return true;
}

// ****************************************************************
// Gameexe.ini：解析ステップ：枠
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
	// メッセージウィンドウ枠：ボタン
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
				return error(_T("無効なシステムタイプです。") + str[0]);
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
				return error(_T("無効なシステムタイプです。") + str[0]);
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
				return error(_T("無効なシステムタイプです。") + str[0]);
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
	// メッセージウィンドウ枠：顔グラ
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
// Gameexe.ini：解析ステップ：ＭＷ
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
// Gameexe.ini：値チェック
// ================================================================
bool C_tnm_ini::analize_check_value(int val, int min, int max)
{
	if (!(min <= val && val <= max))	{
		error(_T("値が異常です。（ ") + tostr(min) + _T(" ～ ") + tostr(max) + _T(" ）"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini：確保数チェック
// ================================================================
bool C_tnm_ini::analize_check_alloc(int val, int min, int max)
{
	if (!(min <= val && val <= max))	{
		error(_T("値が異常です。（ ") + tostr(min) + _T(" ～ ") + tostr(max) + _T(" ）"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini：範囲外アクセスチェック
// ================================================================
bool C_tnm_ini::analize_check_over(int val, int alloc, int max)
{
	if (!(0 <= val && val < alloc))	{
		error(_T("範囲外アクセスです。（ 0 ～ ") + tostr(alloc) + _T(" [") + tostr(max) + _T("] ）"));
		return false;
	}
	return true;
}

// ****************************************************************
// Gameexe.ini：エラー：解析できませんでした
// ================================================================
bool C_tnm_ini::error_dont_analize()
{
	return error(_T("解析できませんでした。"));
}

// ****************************************************************
// Gameexe.ini：エラー
// ================================================================
bool C_tnm_ini::error(CTSTR& str)
{
	set_last_error(str_format(_T("Gameexe.dat エラー %d 行目\n"), line_no + 1) + line_data + _T("\n") + str, _T(""));
	return false;
}

#if 0

// ****************************************************************
// Flag.ini：解析
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

						// プロパティチェック
						if (property == _T('A') || property == _T('B') || property == _T('C') || property == _T('D') || property == _T('E') || property == _T('F') || property == _T('G') || property == _T('Z') || property == _T('L') || property == _T('H') || property == _T('I') || property == _T('J') || property == _T('S') || property == _T('M') || property == _T('K'))	{

							// インデックスチェック
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

								// コメントチェック
								if (anl.get_moji(&moji) && moji == _T(':'))	{
									var.comment = TSTR(anl.get_cur(), anl.get_end());
								}

								// 変数確定
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
