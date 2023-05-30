#pragma		once

#include	"data/tnm_dlg.h"

// コンフィグウィンドウ
#include	"dialog/cfg_wnd.h"

// デバッグウィンドウ
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
// ダイアログマネージャ
// ================================================================
class C_tnm_dlg_manager
{
public:

	ARRAY<C_tnm_dlg *>	dlg_list;

	// 環境設定ベース
	C_cfg_wnd_config_base			cfg_wnd_config_base;			// ベース
	C_cfg_wnd_config_base_full_mode	cfg_wnd_config_base_full_mode;	// ベースフルモード

	// 環境設定ソロ
	C_cfg_wnd_solo_screen			cfg_wnd_solo_screen;			// ソロ：画面モード
	C_cfg_wnd_solo_font_msgspd		cfg_wnd_solo_font_msgspd;		// ソロ：メッセージ（フォント、文字速度）
	C_cfg_wnd_solo_font				cfg_wnd_solo_font;				// ソロ：フォント
	C_cfg_wnd_solo_msgspd			cfg_wnd_solo_msgspd;			// ソロ：文字速度
	Ccfg_wnd_solo_volume_bgmfade	cfg_wnd_solo_volume_bgmfade;	// ソロ：音量（音量、ＢＧＭフェード）
	Ccfg_wnd_solo_volume			cfg_wnd_solo_volume;			// ソロ：音量
	Ccfg_wnd_solo_bgmfade			cfg_wnd_solo_bgmfade;			// ソロ：ＢＧＭフェード
	Ccfg_wnd_solo_koemode_chrkoe	cfg_wnd_solo_koemode_chrkoe;	// ソロ：音声（音声モード、キャラクター音声）
	Ccfg_wnd_solo_koemode			cfg_wnd_solo_koemode;			// ソロ：音声モード
	Ccfg_wnd_solo_chrkoe			cfg_wnd_solo_chrkoe;			// ソロ：キャラクター音声
	Ccfg_wnd_solo_mwndbk			cfg_wnd_solo_mwndbk;			// ソロ：ウィンドウ背景色
	Ccfg_wnd_solo_automode			cfg_wnd_solo_automode;			// ソロ：オートモード
	Ccfg_wnd_solo_jitan				cfg_wnd_solo_jitan;				// ソロ：時短再生
	Ccfg_wnd_solo_else				cfg_wnd_solo_else;				// ソロ：その他
	Ccfg_wnd_solo_system			cfg_wnd_solo_system;			// ソロ：システム
	Ccfg_wnd_solo_movtype			cfg_wnd_solo_movtype;			// ソロ：ムービーの再生方法

	// セーブダイアログ
	C_sys_wnd_solo_save				sys_wnd_solo_save;				// セーブダイアログ
	// ロードダイアログ
	C_sys_wnd_solo_load				sys_wnd_solo_load;				// ロードダイアログ
	// ツイートダイアログ
	C_sys_wnd_solo_tweet			sys_wnd_solo_tweet;				// ツイートダイアログ

	// デバッグ情報ウィンドウ
	C_db_wnd_info_debug				db_wnd_info_debug;				// デバッグ設定
	C_db_wnd_info_scn_control		db_wnd_info_scn_control;		// シーンコントロール
	C_db_wnd_info_trace				db_wnd_info_trace;				// トレース
	C_db_wnd_info_error				db_wnd_info_error;				// エラー情報

	C_db_wnd_info_d3d_device		db_wnd_info_d3d_device;			// Direct3D デバイス情報
	C_db_wnd_info_d3d_resource		db_wnd_info_d3d_resource;		// Direct3D リソース情報
	C_db_wnd_info_input				db_wnd_info_input;				// 入力情報
	C_db_wnd_info_system			db_wnd_info_system;				// システム情報
	C_db_wnd_info_dir				db_wnd_info_dir;				// ディレクトリ情報
	C_db_wnd_info_script			db_wnd_info_script;				// スクリプト情報
	C_db_wnd_info_stack				db_wnd_info_stack;				// スタック情報
	C_db_wnd_info_proc_stack		db_wnd_info_proc_stack;			// プロセススタック情報
	C_db_wnd_info_call_histroy		db_wnd_info_call_history;		// 呼び出し履歴
	C_db_wnd_info_namae				db_wnd_info_namae;				// 【名前】情報
	C_db_wnd_info_read_flag			db_wnd_info_read_flag;			// 既読フラグ情報
	C_db_wnd_info_cg_table			db_wnd_info_cg_table;			// ＣＧテーブル情報
	C_db_wnd_info_bgm_table			db_wnd_info_bgm_table;			// ＢＧＭテーブル情報
	C_db_wnd_info_element			db_wnd_info_element;			// エレメント情報
	C_db_wnd_info_flag				db_wnd_info_flag[TNM_FLAG_CNT];	// フラグ情報
	C_db_wnd_info_flag_ini			db_wnd_info_flag_ini[20];		// flag.ini 情報
	C_db_wnd_info_user_flag			db_wnd_info_user_flag;			// ユーザフラグ情報
	C_db_wnd_info_user_call_flag	db_wnd_info_user_call_flag;		// ユーザコールフラグ情報
	C_db_wnd_info_namae_flag		db_wnd_info_namae_flag;			// 名前フラグ情報
	C_db_wnd_info_counter			db_wnd_info_counter;			// カウンタ情報
	C_db_wnd_info_btn				db_wnd_info_btn;				// ボタン情報
	C_db_wnd_info_group				db_wnd_info_group;				// グループ情報
	C_db_wnd_info_world				db_wnd_info_world;				// ワールド情報
	C_db_wnd_info_object			db_wnd_info_object[6];			// オブジェクト情報
	C_db_wnd_info_mwnd				db_wnd_info_mwnd;				// メッセージウィンドウ情報
	C_db_wnd_info_sound				db_wnd_info_sound;				// サウンド情報
	C_db_wnd_info_system_sound		db_wnd_info_system_sound;		// ボリューム情報
	C_db_wnd_info_koe				db_wnd_info_koe;				// 声ウォッチ情報
	C_db_wnd_info_save_history		db_wnd_info_save_history;		// セーブポイント履歴

	void		regist();						// 登録
	void		init();							// 初期化
	void		free();							// 解放
	void		load_initial_state();			// 初期ステータス読み込み
	void		save_initial_state();			// 初期ステータス保存
	void		initial_open();					// 初期オープン
	void		set_enable(bool flag);			// 禁止／許可を設定
	bool		check_mouse_over();				// マウスが当たっている／当たっていないを取得
	void		frame();						// フレーム処理
};

