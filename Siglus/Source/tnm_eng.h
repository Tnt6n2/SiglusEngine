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
// となまエンジン
// ================================================================
class C_tnm_eng
{
public:
	C_tnm_eng();

	// メイン処理
	bool	main();					// メイン処理

	// 初期化処理
	bool	init();					// 初期化
	bool	init_global();			// 初期化（グローバル）
	bool	init_local();			// 初期化（ローカル）

	// 再初期化処理
	bool	reinit_local(bool restruct_flag);	// 再初期化（ローカル）

	// 解放処理
	bool	free();					// 解放
	bool	free_global();			// 解放（グローバル）

	// 開始終了処理
	bool	start();				// 開始
	bool	end();					// 終了

	// 終了処理
	bool	finish_local();			// ★これはエレメントの終了処理です。

	// セーブロード処理
	bool	save_global(C_tnm_save_stream& stream);	// セーブ（グローバル）
	bool	load_global(C_tnm_save_stream& stream);	// ロード（グローバル）
	bool	save_config(C_tnm_save_stream& stream);	// セーブ（グローバル）
	bool	load_config(C_tnm_save_stream& stream, int major_version, int minor_version);	// ロード（グローバル）
	bool	save_local_msg(CTSTR& msg);				// セーブ（ローカルの現在の文章）
	bool	save_local();							// セーブ（ローカル）
	bool	load_local();							// ロード（ローカル）
	bool	save_local_ex();						// セーブ（ローカル設定）
	bool	load_local_ex();						// ロード（ローカル設定）
	bool	save_call();							// セーブ（コール）
	bool	load_call();							// ロード（コール）

	// フレーム処理
	bool	frame();					// フレーム処理
	bool	frame_local();				// フレームローカル処理
	bool	frame_global();				// フレームグローバル処理
	bool	frame_main_proc();			// フレームメイン処理
	bool	timer_proc();				// タイマー処理
	bool	mouse_move_proc();			// マウス移動処理
	bool	debug_key_proc();			// デバッグキー処理
	bool	system_key_proc();			// システムキー処理
	bool	button_proc();				// ボタン処理
	bool	button_regist_proc();		// ボタン登録処理
	bool	button_hit_test_proc();		// ボタン当たり判定処理
	bool	button_event_proc();		// ボタンイベント処理
	bool	cancel_call_proc();			// キャンセルコール処理
	bool	script_input_proc();		// スクリプト入力処理
	bool	frame_action_proc();		// フレームアクション処理
	bool	load_after_call_proc();		// ロード直後コール処理
	bool	screen_size_proc();			// スクリーンサイズ処理

	bool	system_key_proc_msg_back();		// システム処理：メッセージバック
	bool	system_key_proc_hide_mwnd();	// システム処理：メッセージウィンドウを消す
	bool	system_key_proc_shortcut();		// システム処理：ショートカットキー

	// グローバルセーブ
	C_tnm_save_stream				m_global_save;				// グローバルのセーブデータ

	// ローカルセーブ
	S_tnm_local_save				m_local_save;				// ローカルセーブデータ
	int								m_local_save_history_index;	// ローカルセーブデータの現在位置（セーブヒストリー用）

	// セルセーブ
	S_tnm_sel_save					m_sel_save;

	// セーブ履歴
	S_tnm_save_history				m_save_history;

	// バックログセーブ
	S_tnm_backlog_save				m_backlog_save;

	// 特殊：選択肢ポイントストック（ゆめいろアルエット！）
	S_tnm_local_save				m_sel_save_stock;			// 前の選択肢に戻る用のローカルセーブデータ（ゆめある用）

	// 特殊：インナーセーブ（と～めいヘブン！）
	ARRAY<S_tnm_local_save>			m_inner_save_list;			// インナーセーブ

public:
	C_tnm_wnd						m_wnd;						// ウィンドウ
	C_tnm_movie_wnd					m_movie_wnd;				// ムービーウィンドウ
	C_tnm_dlg_manager				m_dlg_mng;					// ダイアログマネージャ
	C_tnm_net						m_net;						// ネットワーク
	C_tnm_input						m_input;					// 入力
	C_input_state					m_cur_input;				// 現在の入力
	C_input_state					m_last_input;				// 前回の入力
	C_tnm_flag_ini					m_flag_ini;					// flag.ini
	C_tnm_system_info				m_system_info;				// システム情報
	C_tnm_system_config				m_system_config;			// システムコンフィグ
	C_tnm_global_data				m_global;					// グローバルデータ
	C_tnm_dir						m_dir;						// ディレクトリ
	C_tnm_config					m_config;					// コンフィグ
	C_tnm_syscom					m_syscom;					// システムコマンド
	C_tnm_local_data				m_local;					// ローカルデータ
	C_tnm_scene_lexer				m_lexer;					// レクサー
	C_tnm_stack						m_stack;					// スタック
	C_tnm_timer						m_timer;					// タイマー
	C_tnm_save_cache				m_save_cache;				// セーブキャッシュ
	C_tnm_icon						m_icon;						// アイコン
	C_tnm_cursor_list				m_cursor_list;				// カーソル
	C_tnm_font						m_font;						// フォント
	C_font_name_list				m_font_name_list;			// フォント名リスト
	C_tnm_cg_table					m_cg_table;					// ＣＧテーブル
	C_tnm_bgm_table					m_bgm_table;				// ＢＧＭテーブル
	C_tnm_thumb_table				m_thumb_table;				// サムネイルテーブル
	ARRAY<ARRAY<BYTE>>				m_read_flag_list;			// 既読フラグ
	C_tnm_namae						m_namae;					// 【名前】
	ARRAY<C_tnm_chrkoe>				m_chrkoe;					// キャラクター音声
	C_tnm_color_table				m_color_table;				// カラーテーブル
	C_tnm_tonecurve					m_tonecurve;				// トーンカーブ
	C_tnm_fog						m_fog;						// フォグ
	C_tnm_wipe						m_wipe;						// ワイプ
	C_tnm_btn_mng					m_btn_mng;					// ボタンマネージャ
	C_tnm_btn_action_template_list	m_btn_action_template_list;	// ボタンアクションテンプレート
	C_tnm_btn_se_template_list		m_btn_se_template_list;		// ボタンシステム音テンプレート
	C_tnm_script_input				m_script_input;				// スクリプトが管理する入力
	C_tnm_fetch_point				m_fetch_point;				// フェッチポイント
	C_tnm_click_disable				m_click_disable;			// クリック禁止
	C_tnm_debug_dll					m_debug_dll;				// デバッグＤＬＬ
	C_tnm_error						m_error;					// エラー

	// エレメント
	C_elm_flag						m_flag;						// フラグ
	C_elm_user_inc_prop				m_user_inc_prop;			// ユーザインクプロパティ
	C_elm_user_scn_prop_list		m_user_scn_prop_list;		// ユーザシーンプロパティリスト
	C_elm_database_list				m_database_list;			// データベース
	C_elm_counter_list				m_counter_list;				// カウンタ
	C_elm_frame_action				m_frame_action;				// フレームアクション
	C_elm_frame_action_list			m_frame_action_ch_list;		// フレームアクションチャンネル
	C_elm_g00_buf_list				m_g00_buf_list;				// Ｇ００バッファ
	C_elm_mask_list					m_mask_list;				// マスク
	C_elm_stage_list				m_stage_list;				// ステージ
	C_elm_msg_back					m_msg_back;					// メッセージバック
	C_elm_screen					m_screen;					// スクリーン
	C_elm_sound						m_sound;					// サウンド
	C_elm_pcm_event_list			m_pcm_event_list;			// 効果音イベント
	C_elm_editbox_list				m_editbox_list;				// エディットボックス
	C_elm_call_list					m_call_list;				// コール
	C_elm_excall					m_excall;					// ＥＸコール

#if __USE_STEAM
	C_tnm_steam						m_steam;
#endif

public:
	bool							m_init_success_flag;		// 初期化成功フラグ
};

