#pragma		once

#include	"tnm_element_code.h"

#include	"data/tnm_array.h"
#include	"data/tnm_proc.h"

// ****************************************************************
// エンジン：ローカルデータ
// ================================================================
//		シーン実行に関する一時的なデータを扱います。
//		これらはセーブデータに保存されます。
// ================================================================
struct C_tnm_local_data_pod
{
	void				init();

	int					cur_koe_no;						// 現在の文章に割り当てられている声番号
	int					cur_chr_no;						// 現在の文章に割り当てられている声キャラクター番号
	int					cur_read_flag_scn_no;			// 現在の文章に割り当てられている既読フラグのシーン番号
	int					cur_read_flag_flag_no;			// 現在の文章に割り当てられている既読フラグのフラグ番号
	int					cursor_no;						// カーソル番号

	// よく使う
	bool				syscom_menu_disable;			// システムコマンドメニュー禁止
	bool				hide_mwnd_disable;				// メッセージウィンドウを隠す禁止
	bool				msg_back_disable;				// メッセージバック禁止
	bool				shortcut_disable;				// ショートカット禁止

	// メッセージ
	bool				skip_disable;					// スキップ禁止
	bool				ctrl_disable;					// スキップ禁止（Ctrl 含む）
	bool				not_stop_skip_by_click;			// クリックしてもスキップを止めない
	bool				not_skip_msg_by_click;			// クリックしてもメッセージをスキップしない
	bool				skip_unread_message;			// 未読の文章もスキップ
	bool				auto_mode_flag;					// オートモードフラグ
	int					auto_mode_moji_wait;			// オートモード文字待ち時間
	int					auto_mode_min_wait;				// オートモード最小待ち時間
	int					auto_mode_moji_cnt;				// オートモード時の文字数
	int					mouse_cursor_hide_onoff;		// 自動でマウスカーソルを隠す
	int					mouse_cursor_hide_time;			// 自動でマウスカーソルを隠す時間
	int					msg_back_save_cntr;				// メッセージバックを数個置きに保存するためのカウンタ

	int					msg_speed;						// メッセージスピード
	bool				msg_nowait;						// メッセージノーウェイト
	bool				async_msg_mode;					// 非同期メッセージモード
	bool				async_msg_mode_once;			// 非同期メッセージモード（１度きり）
	bool				multi_msg_mode;					// マルチメッセージモード
	bool				skip_trigger;					// スキップトリガー
	bool				koe_dont_stop_on_flag;			// 声を止めない
	bool				koe_dont_stop_off_flag;			// 声を止める

	// メッセージウィンドウ
	bool				mwnd_btn_disable_all;			// メッセージウィンドウボタン禁止（全て）
	bool				mwnd_btn_touch_disable;			// メッセージウィンドウボタン触れない
	bool				mwnd_anime_on_flag;				// メッセージウィンドウのアニメを行う
	bool				mwnd_anime_off_flag;			// メッセージウィンドウのアニメを行わない
	bool				mwnd_disp_off_flag;				// メッセージウィンドウを消す

	// メッセージバック
	bool				msg_back_off;					// メッセージバック使わない
	bool				msg_back_disp_off;				// メッセージバックを非表示にする

	// フォント
	int					font_futoku;					// フォント太く
	int					font_shadow;					// フォント影

	// 入力
	bool				cursor_disp_off;				// カーソルを消す
	bool				cursor_move_by_key_disable;		// キーボードでカーソルを動かす機能を禁止
	bool				key_disable[256];				// キーを無効にする

	// その他
	bool				quake_stop_flag;				// クエイクを止める
	bool				emote_mouth_stop_flag;			// E-mote の口パクを止める
	bool				cg_table_off_flag;				// ＣＧテーブルを使わない
	bool				bgmfade_flag;					// ＢＧＭフェードをする
	bool				dont_set_save_point;			// セーブポイントを設置しない
	bool				ignore_r_flag;					// Ｒを無視する！！！！
	bool				wait_display_vsync_off_flag;	// ディスプレイの垂直同期を待たない

	bool				time_stop_flag;					// 時間を進めない
	bool				counter_time_stop_flag;			// 時間を進めない
	bool				frame_action_time_stop_flag;	// 時間を進めない
	bool				stage_time_stop_flag;			// 時間を進めない
};

class C_tnm_local_data
{
public:
	void				init();

	C_tnm_proc						cur_proc;			// 現在のプロセス
	C_tnm_class_array<C_tnm_proc>	proc_stack;			// プロセスキュー

	S_element			cur_mwnd;						// 現在のメッセージウィンドウ
	S_element			cur_sel_mwnd;					// 現在の選択肢用のメッセージウィンドウ
	S_element			last_mwnd;						// 最後に表示を行ったメッセージウィンドウ（メッセージバックで次に行くかの判定に使う）
	TSTR				scene_title;					// シーンタイトル
	TSTR				cur_full_message;				// 現在のフルメッセージ

	// メッセージウィンドウ
	ARRAY<S_bool>		mwnd_btn_disable;				// メッセージウィンドウボタン禁止

	// フォント
	TSTR				font_name;						// フォント名

	// pod
	C_tnm_local_data_pod			pod;
};

