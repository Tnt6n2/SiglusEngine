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
		TSTR	m_dir;		// ディレクトリ名
		TSTR	m_name;		// ロード画面などで表示される文字列
	};

	C_tnm_select_ini();

	// 解析
	bool	analize();

	// 先頭のディレクトリ
	TSTR	first_dir();
	TSTR	first_name();

	// アペンドリスト
	ARRAY<C_item>	m_append_list;
	// 現在のアペンド
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

	// 解析
	bool	analize();

	// 定数
	enum
	{
		// 環境変数
		INIMIN_SYSTEM_VALUE_CNT = 0,
		INIMAX_SYSTEM_VALUE_CNT = 256,
		INIDEF_SYSTEM_VALUE_CNT = 64,

		// アイコン
		INIMIN_ICON_CNT = 0,
		INIMAX_ICON_CNT = 256,
		INIDEF_ICON_CNT = 16,

		// マウスカーソル
		INIMIN_MOUSE_CURSOR_CNT = 0,
		INIMAX_MOUSE_CURSOR_CNT = 256,
		INIDEF_MOUSE_CURSOR_CNT = 16,

		// ショートカットキー
		INIMIN_SHORTCUT_CNT = 0,
		INIMAX_SHORTCUT_CNT = 256,
		INIDEF_SHORTCUT_CNT = 0,

		// フリックシーン
		INIMIN_FLICK_SCENE_CNT = 0,
		INIMAX_FLICK_SCENE_CNT = 256,
		INIDEF_FLICK_SCENE_CNT = 0,

		// システムコマンドメニュー

		// システムコマンドメニュー：ローカル汎用スイッチ
		INIMIN_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,
		INIMAX_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,
		INIDEF_LOCAL_EXTRA_SWITCH_CNT = TNM_LOCAL_EXTRA_SWITCH_CNT,

		// システムコマンドメニュー：ローカル汎用モード
		INIMIN_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,
		INIMAX_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,
		INIDEF_LOCAL_EXTRA_MODE_CNT = TNM_LOCAL_EXTRA_MODE_CNT,

		// システムコマンドメニュー：ローカル汎用モードのアイテム数
		INIMIN_LOCAL_EXTRA_MODE_ITEM_CNT = 0,		// ←これは 0 でＯＫです。
		INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT = TNM_LOCAL_EXTRA_MODE_ITEM_CNT,
		INIDEF_LOCAL_EXTRA_MODE_ITEM_CNT = 3,

		// ユーザーコンフィグ

		// ユーザーコンフィグ：キャラクター音声
		INIMIN_CHRKOE_CNT = 0,
		INIMAX_CHRKOE_CNT = 256,
		INIDEF_CHRKOE_CNT = 64,

		// ユーザーコンフィグ：オブジェクトの表示フラグ
		INIMIN_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,
		INIMAX_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,
		INIDEF_OBJECT_DISP_FLAG_CNT = TNM_OBJECT_DISP_CNT,

		// ユーザーコンフィグ：グローバル汎用スイッチ
		INIMIN_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,
		INIMAX_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,
		INIDEF_GLOBAL_EXTRA_SWITCH_CNT = TNM_GLOBAL_EXTRA_SWITCH_CNT,

		// ユーザーコンフィグ：グローバル汎用モード
		INIMIN_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,
		INIMAX_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,
		INIDEF_GLOBAL_EXTRA_MODE_CNT = TNM_GLOBAL_EXTRA_MODE_CNT,

		// ユーザーコンフィグ：グローバル汎用モードのアイテム数
		INIMIN_GLOBAL_EXTRA_MODE_ITEM_CNT = 0,		// ←これは 0 でＯＫです。
		INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT = TNM_GLOBAL_EXTRA_MODE_ITEM_CNT,
		INIDEF_GLOBAL_EXTRA_MODE_ITEM_CNT = TNM_GLOBAL_EXTRA_MODE_ITEM_CNT,

		// ユーザコンフィグ：セーブロードダイアログ：１ページ（１タブ）あたりのデータ数
		INIMIN_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 1,
		INIMAX_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 1000,
		INIDEF_SAVELOAD_DIALOG_DATA_CNT_PAR_PAGE = 10,

		// セーブデータ数
		INIMIN_SAVE_CNT = 0,
		INIMAX_SAVE_CNT = 10000,
		INIDEF_SAVE_CNT = 10,

		// クイックセーブデータ数
		INIMIN_QUICK_SAVE_CNT = 0,
		INIMAX_QUICK_SAVE_CNT = 10000,
		INIDEF_QUICK_SAVE_CNT = 3,

		// エンドセーブデータ数
		INIMIN_END_SAVE_CNT = 0,
		INIMAX_END_SAVE_CNT = 1,
		INIDEF_END_SAVE_CNT = 0,

		// インナーセーブデータ数
		INIMIN_INNER_SAVE_CNT = 0,
		INIMAX_INNER_SAVE_CNT = 100,
		INIDEF_INNER_SAVE_CNT = 0,

		// セーブポイント履歴数
		INIMIN_SAVE_HISTORY_CNT = 0,
		INIMAX_SAVE_HISTORY_CNT = 10000,
		INIDEF_SAVE_HISTORY_CNT = 100,

		// バックログセーブ数
		INIMIN_BACKLOG_SAVE_CNT = 0,
		INIMAX_BACKLOG_SAVE_CNT = 10000,
		INIDEF_BACKLOG_SAVE_CNT = 0,

		// 前の選択肢に戻るの履歴数
		INIMIN_SEL_SAVE_CNT = 1,
		INIMAX_SEL_SAVE_CNT = 100,
		INIDEF_SEL_SAVE_CNT = 1,

		// フラグ
		INIMIN_FLAG_CNT = 0,
		INIMAX_FLAG_CNT = 10000,
		INIDEF_FLAG_CNT = 1000,

		// グローバルフラグ
		INIMIN_GLOBAL_FLAG_CNT = 0,
		INIMAX_GLOBAL_FLAG_CNT = 10000,
		INIDEF_GLOBAL_FLAG_CNT = 1000,

		// コールフラグ
		INIMIN_CALL_FLAG_CNT = 0,
		INIMAX_CALL_FLAG_CNT = 256,
		INIDEF_CALL_FLAG_CNT = 50,

		// データベース
		INIMIN_DATABASE_CNT = 0,
		INIMAX_DATABASE_CNT = 256,
		INIDEF_DATABASE_CNT = 0,

		// カウンタ
		INIMIN_COUNTER_CNT = 0,
		INIMAX_COUNTER_CNT = 256,
		INIDEF_COUNTER_CNT = 16,

		// カラーテーブル
		INIMIN_COLOR_TABLE_CNT = 256,
		INIMAX_COLOR_TABLE_CNT = 256,
		INIDEF_COLOR_TABLE_CNT = 256,

		// ＣＧテーブル
		INIMIN_CG_TABLE_FLAG_CNT = 0,
		INIMAX_CG_TABLE_FLAG_CNT = 10000,
		INIDEF_CG_TABLE_FLAG_CNT = 1000,

		// Ｇ００バッファ
		INIMIN_G00_BUF_CNT = 0,
		INIMAX_G00_BUF_CNT = 256,
		INIDEF_G00_BUF_CNT = 16,

		// マスク
		INIMIN_MASK_CNT = 0,
		INIMAX_MASK_CNT = 256,
		INIDEF_MASK_CNT = 16,

		// 絵文字
		INIMIN_EMOJI_CNT = 0,
		INIMAX_EMOJI_CNT = 256,
		INIDEF_EMOJI_CNT = 16,

		// 追加フォント
		INIMIN_FONT_FILE_CNT = 0,
		INIMAX_FONT_FILE_CNT = 256,
		INIDEF_FONT_FILE_CNT = 0,

		// システムコール
		INIDEF_MSG_BACK_ORDER = 10000,
		INIDEF_EXCALL_ORDER = 20000,

		// ワールド
		INIMIN_WORLD_CNT = 0,
		INIMAX_WORLD_CNT = 256,
		INIDEF_WORLD_CNT = 1,

		// ボタン：アクション
		INIMIN_BTN_ACTION_CNT = 0,
		INIMAX_BTN_ACTION_CNT = 256,
		INIDEF_BTN_ACTION_CNT = 16,

		// ボタン：効果音
		INIMIN_BTN_SE_CNT = 0,
		INIMAX_BTN_SE_CNT = 256,
		INIDEF_BTN_SE_CNT = 16,

		// ボタン：グループ
		INIMIN_BTN_GROUP_CNT = 0,
		INIMAX_BTN_GROUP_CNT = 256,
		INIDEF_BTN_GROUP_CNT = 4,

		// メッセージボタン
		INIMIN_MSGBTN_CNT = 0,
		INIMAX_MSGBTN_CNT = 256,
		INIDEF_MSGBTN_CNT = 16,

		// オブジェクト
		INIMIN_OBJECT_CNT = 0,
		INIMAX_OBJECT_CNT = 1024,
		INIDEF_OBJECT_CNT = 256,

		// メッセージウィンドウ
		INIMIN_MWND_SUB_CNT = 0,
		INIMAX_MWND_SUB_CNT = 256,
		INIDEF_MWND_SUB_CNT = 2,

		// メッセージウィンドウ枠
		INIMIN_MWND_WAKU_CNT = 0,
		INIMAX_MWND_WAKU_CNT = 256,
		INIDEF_MWND_WAKU_CNT = 4,

		// メッセージウィンドウ枠：ボタン
		INIMIN_MWND_WAKU_BTN_CNT = 0,
		INIMAX_MWND_WAKU_BTN_CNT = 256,
		INIDEF_MWND_WAKU_BTN_CNT = 8,

		// メッセージウィンドウ枠：顔グラ
		INIMIN_MWND_WAKU_FACE_CNT = 0,
		INIMAX_MWND_WAKU_FACE_CNT = 16,
		INIDEF_MWND_WAKU_FACE_CNT = 1,

		// メッセージウィンドウ枠：オブジェクト
		INIMIN_MWND_WAKU_OBJECT_CNT = 0,
		INIMAX_MWND_WAKU_OBJECT_CNT = 16,
		INIDEF_MWND_WAKU_OBJECT_CNT = 1,

		// ボタン選択肢
		INIMIN_SEL_BTN_CNT = 0,
		INIMAX_SEL_BTN_CNT = 256,
		INIDEF_SEL_BTN_CNT = 16,

		// メッセージバック
		INIMIN_MSG_BACK_HISTORY_CNT = 0,
		INIMAX_MSG_BACK_HISTORY_CNT = 1024,
		INIDEF_MSG_BACK_HISTORY_CNT = 256,

		// エフェクト
		INIMIN_EFFECT_CNT = 0,
		INIMAX_EFFECT_CNT = 256,
		INIDEF_EFFECT_CNT = 4,

		// シェイク
		INIMIN_SHAKE_CNT = 0,
		INIMAX_SHAKE_CNT = 256,
		INIDEF_SHAKE_CNT = 16,

		// クエイク
		INIMIN_QUAKE_CNT = 0,
		INIMAX_QUAKE_CNT = 256,
		INIDEF_QUAKE_CNT = 16,

		// フレームアクションチャンネル
		INIMIN_FRAME_ACTION_CH_CNT = 0,
		INIMAX_FRAME_ACTION_CH_CNT = 256,
		INIDEF_FRAME_ACTION_CH_CNT = 4,

		// ＢＧＭ
		INIMIN_BGM_CNT = 0,
		INIMAX_BGM_CNT = 256,
		INIDEF_BGM_CNT = 32,

		// 効果音チャンネル数
		INIMIN_PCMCH_CNT = 0,
		INIMAX_PCMCH_CNT = 256,
		INIDEF_PCMCH_CNT = 16,

		// 効果音イベント数
		INIMIN_PCM_EVENT_CNT = 0,
		INIMAX_PCM_EVENT_CNT = 256,
		INIDEF_PCM_EVENT_CNT = 16,

		// システム音
		INIMIN_SE_CNT = 8,
		INIMAX_SE_CNT = 256,
		INIDEF_SE_CNT = 16,

		// ＢＧＭフェード２
		INIDEF_BGMFADE2_IN_START_TIME = 0,
		INIDEF_BGMFADE2_IN_TIME_LEN = 500,
		INIDEF_BGMFADE2_OUT_START_TIME = 0,
		INIDEF_BGMFADE2_OUT_TIME_LEN = 500,
		INIDEF_BGMFADE2_VOLUME = 0,

		// エディットボックス
		INIMIN_EDITBOX_CNT = 0,
		INIMAX_EDITBOX_CNT = 256,
		INIDEF_EDITBOX_CNT = 4,
	};

	// デバッグ
	int		debug_error_patno_out_of_range;

	// システム
	TSTR	game_id;			// ゲームID
	TSTR	game_name;			// ゲーム名
	TSTR	game_version_str;	// ゲームバージョン文字列
	TSTR	disc_mark;			// ディスク認識ファイル名
	TSTR	manual_path_str;	// マニュアルパス
	C_size	screen_size;		// スクリーンサイズ
	TSTR	start_scene_name;	// スタートシーン名
	int		start_scene_z_no;	// スタートシーンＺラベル
	TSTR	menu_scene_name;	// メニューシーン名
	int		menu_scene_z_no;	// メニューシーンＺラベル
	TSTR	cancel_scene_name;	// キャンセルシーン名
	int		cancel_scene_z_no;	// キャンセルシーンＺラベル
	TSTR	config_scene_name;	// 環境設定シーン名
	int		config_scene_z_no;	// 環境設定シーンＺラベル
	TSTR	save_scene_name;	// セーブシーン名
	int		save_scene_z_no;	// セーブシーンＺラベル
	TSTR	load_scene_name;	// ロードシーン名
	int		load_scene_z_no;	// ロードシーンＺラベル
	TSTR	load_after_call_scene;	// ロード直後コールシーン名
	int		load_after_call_z_no;	// ロード直後コールＺラベル
	TSTR	dummy_check_str;	// ダミーチェックの警告文
	TSTR	dummy_check_ok_str;	// ダミーチェックの成功時の文

	// フリックシーン
	int		flick_scene_cnt;
	TSTR	flick_scene_name[INIMAX_FLICK_SCENE_CNT];
	int		flick_scene_z_no[INIMAX_FLICK_SCENE_CNT];
	int		flick_scene_angle[INIMAX_FLICK_SCENE_CNT];

	// 環境変数
	int		system_int_value_cnt;
	int		system_int_value[INIMAX_SYSTEM_VALUE_CNT];
	int		system_str_value_cnt;
	TSTR	system_str_value[INIMAX_SYSTEM_VALUE_CNT];

	// ショートカットキー
	int		shortcut_cnt;
	int		shortcut_key[INIMAX_SHORTCUT_CNT];
	int		shortcut_cond[INIMAX_SHORTCUT_CNT];
	TSTR	shortcut_scn[INIMAX_SHORTCUT_CNT];
	int		shortcut_z_no[INIMAX_SHORTCUT_CNT];
	TSTR	shortcut_cmd[INIMAX_SHORTCUT_CNT];

	// アイコン
	struct Sicon
	{
		TSTR	file_name;
		int		anime_pat_cnt;
		int		anime_speed;
	};
	Sicon			icon[INIMAX_ICON_CNT];
	int				icon_cnt;

	// マウスカーソル
	struct Smouse_cursor
	{
		TSTR	file_name;
		int		anime_speed;
	};
	Smouse_cursor	mouse_cursor[INIMAX_MOUSE_CURSOR_CNT];
	int				mouse_cursor_cnt;
	int				mouse_cursor_default;

	// システムコマンドメニュー
	struct Ssyscommenu
	{
		// 存在・許可／禁止・文字列
		struct Sexistenablestr
		{
			bool	exist;		// 存在の有無
			bool	enable;		// 許可／禁止
			TSTR	str;		// 文字列
		};

		// 存在・許可／禁止・ON/OFF状態・文字列
		struct Sexistenableonoffstr
		{
			bool	exist;		// 存在の有無
			bool	enable;		// 許可／禁止
			bool	onoff;		// ON/OFF状態
			TSTR	str;		// 文字列
		};

		Sexistenablestr		read_skip;		// 一度読んだ文章の早送りを開始します。
		Sexistenablestr		unread_skip;	// 未読の文章でも早送りを開始します。（★システム設定の「未読の文章も早送りできるようにする。」が有効の場合です）
		Sexistenablestr		auto_skip;		// 自動早送り機能をＯＮ／ＯＦＦします。
		Sexistenablestr		auto_mode;		// オートモードを開始します。
		Sexistenablestr		hide_mwnd;		// メッセージウィンドウを隠します。
		Sexistenablestr		msg_back;		// メッセージバックを開きます。
		Sexistenablestr		save;			// セーブを呼び出します。（#SAVE_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、セーブダイアログを表示します）
		Sexistenablestr		load;			// ロードを呼び出します。（#LOAD_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、ロードダイアログを表示します）
		Sexistenablestr		return_to_sel;	// 前の選択肢に戻ります。
		Sexistenablestr		config;			// 環境設定を呼び出します。（#CONFIG_SCENE が設定されている場合は、そのシーンを呼び出します。設定されていない場合は、環境設定ダイアログを表示します）
		Sexistenablestr		manual;			// マニュアルを表示します。（マニュアルへのパスは #MANUAL_PATH で設定して下さい）
		Sexistenablestr		version;		// バージョン情報を表示します。（情報は #GAMEVERSION で設定して下さい）
		Sexistenablestr		return_to_menu;	// タイトルメニューに戻ります。
		Sexistenablestr		game_end;		// ゲームを終了します。
		Sexistenablestr		cancel;			// システムコマンドメニューをキャンセルします。

		// ローカル汎用スイッチ
		int						local_extra_switch_cnt;								// ローカル汎用スイッチの数
		Sexistenableonoffstr	local_extra_switch[INIMAX_LOCAL_EXTRA_SWITCH_CNT];	// ローカル汎用スイッチ

		// ローカル汎用モード
		struct Slocal_extra_mode
		{
			bool		exist;			// 存在の有無
			bool		enable;			// 許可／禁止
			int			mode;			// モード値
			TSTR		str;			// モード名

			// アイテム
			struct Sitem
			{
				TSTR		str;		// 項目の文字列
			};
			int			item_cnt;		// アイテム数
			Sitem		item[INIMAX_LOCAL_EXTRA_MODE_ITEM_CNT];
		};
		int						local_extra_mode_cnt;								// ローカル汎用モードの数
		Slocal_extra_mode		local_extra_mode[INIMAX_LOCAL_EXTRA_MODE_CNT];		// ローカル汎用モード

	};
	Ssyscommenu	syscommenu;

	// ユーザーコンフィグ
	struct S_config
	{
		// 存在・ON/OFF状態・文字列
		struct S_existonoffstr
		{
			bool	exist;		// 存在の有無
			bool	onoff;		// ON/OFF状態
			TSTR	str;		// 文字列
		};

		// 存在・文字列
		struct S_existstr
		{
			bool	exist;		// 存在の有無
			TSTR	str;		// 文字列
		};

		// ダイアログスタイル
		int		dlg_style_volume;		// 音量ダイアログのスタイル（0=音量＆ＢＧＭフェード  1=音量のみ）
		int		dlg_style_koe;			// 音声ダイアログのスタイル（0=音声モード＆キャラクター音声  1=音声モードのみ  2=キャラクター音声のみ）

		// タブの存在の有無
		bool	tab_exist_screen;		// 「画面」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_volume;		// 「音量」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_message;		// 「文章」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_mwndbk;		// 「背景色」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_koe;			// 「音声」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_automode;		// 「オートモード」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_jitan;		// 「時短再生」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_else;			// 「その他」タブの存在の有無（0=無し 1=有り）
		bool	tab_exist_system;		// 「システム」タブの存在の有無（0=無し 1=有り）

		// 項目の存在の有無
		bool	exist_bgm;				// 「ＢＧＭ」項目の存在の有無
		bool	exist_koe;				// 「音声」項目の存在の有無
		bool	exist_pcm;				// 「効果音」項目の存在の有無
		bool	exist_se;				// 「システム音」項目の存在の有無
		bool	exist_mov;				// 「ムービー」項目の存在の有無

		// スクリーンサイズ
		int		screen_size_mode;		// スクリーンサイズのモード
		C_size	screen_size_scale;		// スクリーンサイズのスケール

		// フルスクリーン詳細
		int		fullscreen_mode;				// フルスクリーンの表示モード
		C_size	fullscreen_scale;				// フルスクリーンのスケール
		bool	fullscreen_scale_sync_switch;	// フルスクリーンのスケールの同期スイッチ
		C_size	fullscreen_move;				// フルスクリーンの移動

		// 音量
		int		all_user_volume;		// 全体ボリューム
		int		bgm_user_volume;		// BGM ボリューム
		int		koe_user_volume;		// KOE ボリューム
		int		pcm_user_volume;		// PCM ボリューム
		int		se_user_volume;			// SE ボリューム
		int		mov_user_volume;		// MOV ボリューム
		bool	play_all_check;			// 全体再生チェック
		bool	play_bgm_check;			// BGM 再生チェック
		bool	play_koe_check;			// KOE 再生チェック
		bool	play_pcm_check;			// PCM 再生チェック
		bool	play_se_check;			// SE 再生チェック
		bool	play_mov_check;			// MOV 再生チェック

		// ＢＧＭフェード
		int		bgmfade_volume;			// ＢＧＭフェードボリューム
		bool	bgmfade_use_check;		// ＢＧＭフェードチェック

		// フィルター色
		C_argb	filter_color;			// フィルター色

		// フォント
		TSTR	font_name;				// フォント：名前
		int		font_type;				// フォント：種類（0=ＭＳゴシック 1=ＭＳ明朝 2=メイリオ 3=独自フォント）
		bool	font_futoku;			// フォント：太くする（0=OFF 1=ON）
		int		font_shadow;			// フォント：影モード（0=影なし 1=影あり 2=縁取る）
		TSTR	font_sample_str_short;	// フォント：サンプル文字列ショート
		TSTR	font_sample_str_long;	// フォント：ロング文字列ショート

		// 文字速度
		int		message_speed;			// 文字速度
		bool	message_speed_nowait;	// 文字速度（ノーウェイト）

		// オートモード
		bool	auto_mode_onoff;		// オートモード
		int		auto_mode_moji_wait;	// オートモード文字時間
		int		auto_mode_min_wait;		// オートモード最小時間

		// マウスカーソルを消す
		bool	mouse_cursor_hide_onoff;	// 自動でマウスカーソルを消す
		int		mouse_cursor_hide_time;		// 自動でマウスカーソルを消す

		// 時短再生
		bool	jitan_normal_onoff;		// 時短再生チェック（文章を普通に読み進めている時）
		bool	jitan_auto_mode_onoff;	// 時短再生チェック（オートモード中）
		bool	jitan_msgbk_onoff;		// 時短再生チェック（文章の巻き戻し中）
		int		jitan_speed;			// 時短再生速度

		// 音声モード
		int		koe_mode;				// 音声モード

		// キャラクター音声
		struct Schrkoe
		{
			TSTR		name_str;			// 名前
			int			check_mode;			// チェックモード（0=伏字状態でチェックしない  1=公開状態でチェックしない  2=伏字状態でチェックする）
			TSTR		check_name_str;		// チェック用の名前
			bool		onoff;				// ON/OFF状態
			int			volume;				// 音量
			ARRAY<int>	chr_no_list;		// キャラクター番号リスト（★このメンバーはエンジン側には無い）
		};
		Schrkoe	chrkoe[INIMAX_CHRKOE_CNT];
		int		chrkoe_cnt;
		TSTR	chrkoe_not_look_name_str;

		// 文章の色分け
		S_existonoffstr		message_chrcolor;	// 文章の色分け

		// オブジェクトの表示フラグの数
		int					object_disp_cnt;		// オブジェクトの表示フラグの数
		S_existonoffstr		object_disp[INIMAX_OBJECT_DISP_FLAG_CNT];				// オブジェクトの表示フラグ

		// グローバル汎用スイッチの数
		int					global_extra_switch_cnt;								// グローバル汎用スイッチの数
		S_existonoffstr		global_extra_switch[INIMAX_GLOBAL_EXTRA_SWITCH_CNT];	// グローバル汎用スイッチ

		// グローバル汎用モード
		struct S_global_extra_mode
		{
			bool		exist;			// 存在の有無
			TSTR		str;			// モード名
			int			mode;			// モード値

			// アイテム
			struct S_item
			{
				TSTR		str;		// 項目の文字列
			};
			int			item_cnt;		// アイテム数
			S_item		item[INIMAX_GLOBAL_EXTRA_MODE_ITEM_CNT];
		};
		int						global_extra_mode_cnt;								// グローバル汎用モードの数
		S_global_extra_mode		global_extra_mode[INIMAX_GLOBAL_EXTRA_MODE_CNT];	// グローバル汎用モード

		// システム設定
		struct Ssystem
		{
			S_existonoffstr		sleep;						// 本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。
			S_existonoffstr		no_wipe_anime;				// 画面暗転効果のアニメを無効にする。
			S_existonoffstr		skip_wipe_anime;			// 画面暗転効果をマウスクリックで飛ばす。
			S_existonoffstr		no_mwnd_anime;				// メッセージウィンドウの開閉時のアニメを無効にする。
			S_existonoffstr		wheel_next_message;			// マウスのホイールボタンの下回しで文章を読み進める。
			S_existonoffstr		koe_dont_stop;				// 声の再生中に次の文章に進んでも再生を続ける。
			S_existonoffstr		skip_unread_message;		// 未読の文章も早送りできるようにする。
			S_existstr			play_silent_sound;			// サウンド再生時に雑音が入る場合はチェックして下さい。

			int					movie_play_type;			// ムービーの再生方法

			bool				saveload_alert_onoff;		// セーブロードの警告
			bool				saveload_dblclick_onoff;	// セーブロードのダブルクリック
		}	system;

		struct S_saveload_dialog
		{
			int					data_cnt_par_page;			// １ページ（１タブ）あたりのデータ数
		};

		// セーブロードダイアログ
		S_saveload_dialog		saveload_dialog;			// セーブロードダイアログ

		// セーブとロードのインフォメーション情報
		struct S_saveloadinfo
		{
			TSTR	dlgwnd_caption_title_str;				// セーブダイアログのキャプションタイトルの文字列
			TSTR	dlgwnd_deside_button_str;				// セーブダイアログの決定ボタンの文字列
			TSTR	dlgwnd_datalist_nameheader_str;			// セーブダイアログのデータリストの先頭文字列
			TSTR	dlgwnd_warning_chkbox_str;				// セーブダイアログの確認チェックボックスの文字列
			TSTR	dlgwnd_dblclick_chkbox_str;				// セーブダイアログのダブルクリックチェックボックスの文字列
			TSTR	warning_str;							// セーブ確認の確認文章の文字列
			TSTR	quick_warning_str;						// クイックセーブ確認の確認文章の文字列
			TSTR	msgbk_warning_str;						// メッセージバックセーブ確認の確認文章の文字列
		};

		// セーブのインフォメーション情報
		S_saveloadinfo		save_info;						// セーブのインフォメーション情報

		// ロードのインフォメーション情報
		S_saveloadinfo		load_info;						// ロードのインフォメーション情報

		// 各種確認メッセージの文字列
		struct Swarninginfo
		{
			TSTR		restart_warning_str;				// ゲームを最初から始める時の確認メッセージの文字列（デバッグ機能）
			TSTR		scenestart_warning_str;				// ゲームをシーンから始める時の確認メッセージの文字列（デバッグ機能）
			TSTR		returnmenu_warning_str;				// タイトルメニューに戻る時の確認メッセージの文字列
			TSTR		returnsel_warning_str;				// 前の選択肢に戻る時の確認メッセージの文字列
			TSTR		gameend_warning_str;				// ゲームを終了する時の確認メッセージの文字列
		};
		Swarninginfo	warninginfo;						// 各種確認メッセージの文字列
	};
	S_config	config;

	// セーブデータ数
	int		save_cnt;

	// クイックセーブデータ数
	int		quick_save_cnt;

	// エンドセーブデータ数
	int		end_save_cnt;

	// インナーセーブの数
	int		inner_save_cnt;

	// セーブポイント履歴の数
	int		save_history_cnt;

	// バックログセーブ数
	int		backlog_save_cnt;
	int		backlog_save_interval;

	// 前の選択肢に戻る履歴の個数
	int		sel_save_cnt;

	// 存在しないセーブデータの文字列
	TSTR	save_no_data_str;

	// セーブサムネイルの設定
	bool	use_save_thumb;
	int		save_thumb_type;
	C_size	save_thumb_size;

	// ロード時のワイプ
	int		load_wipe_type;		// ロード時に使うワイプの番号
	int		load_wipe_time;		// ロード時に使うワイプの時間

	// 縦書き／横書き
	int		tateyoko_mode;		// 縦書き／横書き

	// 【名前】
	struct S_namae
	{
		TSTR		regist_name_str;		// 登録名
		TSTR		change_name_str;		// 変換名
		int			color_mod;				// 色モード
		int			moji_color_no;			// 文字色
		int			shadow_color_no;		// 影色
		int			fuchi_color_no;			// 縁色
	};
	ARRAY<S_namae>		namae_list;

	// フラグ
	int		flag_cnt;
	int		global_flag_cnt;
	int		call_flag_cnt;

	// データベース
	TSTR	database_name[INIMAX_DATABASE_CNT];
	int		database_cnt;

	// カウンタ
	int		counter_cnt;

	// カラーテーブル
	C_rgb	color_table[INIMAX_COLOR_TABLE_CNT];
	int		color_table_cnt;

	// トーンカーブ
	TSTR	tonecurve_file;

	// ＣＧテーブル
	TSTR	cg_table_file;
	int		cg_table_flag_cnt;

	// サムネイルテーブル
	TSTR	thumb_table_file;

	// Ｇ００バッファ
	int		g00_buf_cnt;

	// マスク
	int		mask_cnt;

	// 絵文字
	struct S_emoji
	{
		TSTR	file_name;
		int		font_size;
	};
	S_emoji	emoji[INIMAX_EMOJI_CNT];
	int		emoji_cnt;

	// フォントファイル
	struct S_font_file
	{
		TSTR	file_name;
	};
	S_font_file	font_file[INIMAX_EMOJI_CNT];
	int			font_file_cnt;
	S_font_file	private_font_file[INIMAX_EMOJI_CNT];
	int			private_font_file_cnt;

	// システムコール
	int		msg_back_order;
	int		excall_order;

	// ワールド
	struct S_world
	{
		int		layer;
	};
	S_world			world[INIMAX_WORLD_CNT];
	int				world_cnt;

	// ボタン
	struct Sbutton
	{
		// ボタン：アクション
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

		// ボタン：効果音
		struct Sse
		{
			int		hit_no;
			int		push_no;
			int		decide_no;
		};
		Sse			se[INIMAX_BTN_SE_CNT];
		int			se_cnt;

		// ボタン：グループ
		int			group_cnt;
	};
	Sbutton		button;
	
	// メッセージボタン
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

	// オブジェクト
	struct Sobject
	{
		bool	use;
		bool	save;
		bool	space_hide;
		int		object_disp_no;
	};
	Sobject			object[INIMAX_OBJECT_CNT];
	int				object_cnt;

	// メッセージ
	int		moji_internal_leading_y_rep;

	// メッセージウィンドウ枠
	struct Smwnd_waku
	{
		int		extend_type;				// 0:固定サイズ、1:可変サイズ
		TSTR	waku_file;					// 枠のファイル名
		TSTR	filter_file;				// フィルターのファイル名
		C_rect	filter_margin;				// フィルターのマージン（メッセージウィンドウからの座標）
		C_argb	filter_color;				// フィルターの色
		bool	filter_config_color;		// 環境設定のフィルター色を適用する
		bool	filter_config_tr;			// 環境設定のフィルター透明度を適用する
		int		icon_no;					// キー待ちアイコン
		int		page_icon_no;				// ページ待ちアイコン
		int		icon_pos_type;				// キー待ちアイコンの位置タイプ
		int		icon_pos_base;				// キー待ちアイコンの位置
		C_point	icon_pos;					// キー待ちアイコンの位置

		// メッセージウィンドウ枠：ボタン
		struct S_mwnd_waku_btn
		{
			TSTR	btn_file;				// ボタンのファイル名
			int		cut_no;					// カット番号
			int		pos_base;				// ボタンの位置
			C_point	pos;					// ボタンの位置
			int		action_no;				// アクション番号
			int		se_no;					// 効果音番号
			int		sys_type;				// システムタイプ
			int		sys_type_opt;			// システムタイプオプション
			int		btn_mode;				// ボタンモード番号
			TSTR	scn_name;				// コール先のシーン名
			TSTR	cmd_name;				// コール先のコマンド名
			int		z_no;					// コール先のＺラベル番号
			TSTR	frame_action_scn_name;	// フレームアクション用のシーン名
			TSTR	frame_action_cmd_name;	// フレームアクション用のコマンド名
		};
		S_mwnd_waku_btn	btn[INIMAX_MWND_WAKU_BTN_CNT];

		// メッセージウィンドウ枠：顔グラ
		C_point	face_pos[INIMAX_MWND_WAKU_FACE_CNT];	// 顔グラの位置
	};
	Smwnd_waku		mwnd_waku[INIMAX_MWND_WAKU_CNT];
	int				mwnd_waku_cnt;
	int				mwnd_waku_btn_cnt;
	int				mwnd_waku_face_cnt;
	int				mwnd_waku_object_cnt;

	// メッセージウィンドウ
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
			int		novel_mode;			// 0:ＡＶＧモード、1:ノベルモード

			int		extend_type;		// 0:固定サイズ、1:可変サイズ
			C_point	window_pos;			// ウィンドウ位置
			C_size	window_size;		// ウィンドウサイズ（固定サイズのみ）
			C_point	msg_pos;			// メッセージ位置（固定サイズのみ）
			C_rect	msg_margin;			// メッセージ矩形（ウィンドウ矩形からのマージン）（可変サイズのみ）
			C_size	moji_cnt;			// 縦横の文字数
			int		moji_size;			// 文字のサイズ
			C_size	moji_space;			// 文字間のスペース
			int		moji_color;			// メッセージの色（-1:標準）
			int		shadow_color;		// メッセージの色（-1:標準）
			int		fuchi_color;		// メッセージの色（-1:標準）
			int		ruby_size;			// ルビの文字サイズ
			int		ruby_space;			// ルビと文字とのスペース
			int		waku_no;			// 枠番号
			C_point	waku_pos;			// 枠の位置

			int		name_disp_mode;		// 名前の表示方法（0:名前ウィンドウに表示、1:メッセージの先頭に表示、2:表示しない）
			int		name_newline;		// 名前の後に改行する
			int		name_bracket;		// 名前を括弧でくくる（0=括弧なし、1=【】）

			int		name_extend_type;	// 0:固定サイズ、1:可変サイズ
			int		name_window_align;	// 名前ウィンドウの揃え位置
			C_point	name_window_pos;	// 名前ウィンドウのウィンドウ位置
			C_size	name_window_size;	// 名前ウィンドウのウィンドウサイズ（固定サイズのみ）
			C_point	name_msg_pos;		// 名前ウィンドウのメッセージ位置（固定サイズのみ）
			C_point	name_msg_pos_rep;	// 名前ウィンドウのメッセージ修正位置（固定サイズのみ）
			C_rect	name_msg_margin;	// 名前ウィンドウのメッセージ矩形（ウィンドウ矩形からのマージン）（可変サイズのみ）
			int		name_moji_size;		// 名前ウィンドウの文字のサイズ
			C_size	name_moji_space;	// 名前ウィンドウの文字間のスペース
			int		name_moji_cnt;		// 名前ウィンドウの文字数
			int		name_moji_color;	// 名前の色（-1:標準）
			int		name_shadow_color;	// 名前の色（-1:標準）
			int		name_fuchi_color;	// 名前の色（-1:標準）
			int		name_waku_no;		// 名前ウィンドウの枠番号
			C_point	name_waku_pos;		// 名前ウィンドウの枠の位置

			C_rect	talk_margin;			// セリフのマージン（ノベルモード）
			int		over_flow_check_size;	// オーバーフローと判断するサイズ
			int		msg_back_insert_nl;		// メッセージバックに改行を入れる

			int		face_hide_name;		// 顔グラを表示したとき名前を隠す

			int		open_anime_type;	// 開きアニメタイプ
			int		open_anime_time;	// 開きアニメ時間
			int		close_anime_type;	// 閉じアニメタイプ
			int		close_anime_time;	// 閉じアニメ時間
		};
		S_mwnd_sub	sub[INIMAX_MWND_SUB_CNT];
		int			sub_cnt;
	};
	S_mwnd	mwnd;

	// ボタン選択肢
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

	// メッセージバック
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

	// エフェクト
	int		effect_cnt;

	// シェイク
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

	// クエイク
	int		quake_cnt;

	// フレームアクションチャンネル
	int		frame_action_ch_cnt;

	// ＢＧＭ
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

	// 効果音チャンネル数
	int		pcmch_cnt;

	// 効果音イベント数
	int		pcm_event_cnt;

	// システム音
	struct Sse
	{
		TSTR	file_name;
	};
	Sse		se[INIMAX_SE_CNT];
	int		se_cnt;

	// ＢＧＭフェード２
	int		bgmfade2_in_start_time;
	int		bgmfade2_in_time_len;
	int		bgmfade2_out_start_time;
	int		bgmfade2_out_time_len;
	int		bgmfade2_volume;

	// エディットボックス
	int		editbox_cnt;

	// Twitter
	TSTR	twitter_api_key;
	TSTR	twitter_api_secret;
	TSTR	twitter_callback_url;
	TSTR	twitter_initial_tweet_text;
	TSTR	twitter_overlap_image;

private:

	// 解析用変数
	int		line_no;
	TSTR	line_data;

	bool	analize_func();											// 解析
	bool	analize_step_1(C_str_analizer& anl, int line_no);		// 解析ステップ１
	bool	analize_step_2(C_str_analizer& anl, int line_no);		// 解析ステップ２
	bool	analize_step_waku(C_str_analizer& anl, int line_no, int waku_no);	// 解析ステップ：枠
	bool	analize_step_mwnd(C_str_analizer& anl, int line_no, int mw_no);		// 解析ステップ：ＭＷ
	bool	analize_check_value(int val, int min, int max);			// 値チェック
	bool	analize_check_alloc(int val, int min, int max);			// 確保数チェック
	bool	analize_check_over(int val, int alloc, int max);		// 範囲外アクセスチェック

	bool	error(CTSTR& str);									// エラー
	bool	error_dont_analize();								// エラー：解析できませんでした
};

