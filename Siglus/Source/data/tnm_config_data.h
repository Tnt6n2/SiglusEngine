#pragma		once

#include	"tnm_def.h"

// ****************************************************************
// ユーザーコンフィグ
// ================================================================
//		ダイアログなどで設定できる項目です。
//		グローバルにセーブされます。
// ================================================================

// ****************************************************************
// ユーザーコンフィグ：キャラクター音声
// ================================================================
struct S_tnm_config_chrkoe
{
	bool	onoff;
	int		volume;
};

// ****************************************************************
// ユーザーコンフィグ
// ================================================================
class C_tnm_config
{
public:

	// スクリーンサイズ
	int		screen_size_mode;			// スクリーンサイズのモード
	int		screen_size_mode_window;	// スクリーンサイズのモード（ウィンドウ時）
	C_size	screen_size_scale;			// スクリーンサイズのスケール
	C_size	screen_size_free;			// スクリーンサイズ（フリー）

	// フルスクリーン詳細
	bool	fullscreen_change_resolution;	// フルスクリーン時に解像度を変更する
	int		fullscreen_display_cnt;			// フルスクリーンに使うディスプレイ番号の個数
	int		fullscreen_display_no;			// フルスクリーンに使うディスプレイ番号
	int		fullscreen_resolution_cnt;		// フルスクリーンに使うディスプレイモード番号の個数
	int		fullscreen_resolution_no;		// フルスクリーンに使うディスプレイモード番号
	C_size	fullscreen_resolution;			// フルスクリーンに使うディスプレイサイズ

	bool	total_fullscreen_change_resolution;		// フルスクリーン時に解像度を変更する（実際の値）
	int		total_fullscreen_display_cnt;			// フルスクリーンに使うディスプレイの個数
	int		total_fullscreen_display_no;			// フルスクリーンに使うディスプレイ番号
	int		total_fullscreen_resolution_cnt;		// フルスクリーンに使うディスプレイモードの個数
	int		total_fullscreen_resolution_no;			// フルスクリーンに使うディスプレイモード番号
	C_size	total_fullscreen_resolution;			// フルスクリーンに使うディスプレイサイズ

	int		fullscreen_mode;				// フルスクリーンの表示モード
	C_size	fullscreen_scale;				// フルスクリーンのスケール
	bool	fullscreen_scale_sync_switch;	// フルスクリーンのスケールの同期スイッチ
	C_size	fullscreen_move;				// フルスクリーンの移動

	// 音量
	int		all_sound_user_volume;
	int		sound_user_volume[TNM_VOLUME_TYPE_MAX];		// ボリューム
	bool	play_all_sound_check;
	bool	play_sound_check[TNM_VOLUME_TYPE_MAX];		// 再生チェック

	// ＢＧＭフェード
	int		bgmfade_volume;			// ＢＧＭフェードボリューム
	bool	bgmfade_use_check;		// ＢＧＭフェードチェック

	// フィルター色
	C_argb	filter_color;			// フィルター色

	// フォント
	bool	font_proportional;		// プロポーショナルフォントを表示
	TSTR	font_name;				// フォント名
	bool	font_futoku;			// フォント太く
	int		font_shadow;			// フォント影

	// 文字速度
	int		message_speed;			// 文字速度
	bool	message_speed_nowait;	// 文字速度（ノーウェイト）

	// オートモード
	bool	auto_mode_onoff;		// オートモード
	int		auto_mode_moji_wait;	// オートモード文字時間
	int		auto_mode_min_wait;		// オートモード最小時間

	// 自動でマウスカーソルを消す
	bool	mouse_cursor_hide_onoff;	// 自動でマウスカーソルを消す
	int		mouse_cursor_hide_time;		// 自動でマウスカーソルを消す時間

	// 時短再生
	bool	jitan_normal_onoff;		// 時短再生（文章を普通に読み進めている時）
	bool	jitan_auto_mode_onoff;	// 時短再生（オートモード中）
	bool	jitan_msgbk_onoff;		// 時短再生（文章の巻き戻し中）
	int		jitan_speed;			// 時短再生速度

	// 音声モード
	int		koe_mode;				// 音声モード

	// キャラクター音声
	ARRAY<S_tnm_config_chrkoe>	chrkoe;			// キャラクター音声

	// 文章の色分け
	bool	message_chrcolor_flag;	// 文章の色分け

	// オブジェクトの表示フラグ
	ARRAY<S_bool>	object_disp_flag;			// オブジェクトの表示フラグ

	// グローバル汎用スイッチ
	ARRAY<S_bool>	global_extra_switch_flag;	// グローバル汎用スイッチ

	// グローバル汎用モード
	ARRAY<int>		global_extra_mode_flag;		// グローバル汎用モード

	// システム設定
	struct S_system
	{
		bool	sleep_flag;					// 本プログラムの動作を遅くして、他のプログラムがスムーズに動作するようにする。
		bool	no_wipe_anime_flag;			// 画面暗転効果のアニメを無効にする。
		bool	skip_wipe_anime_flag;		// 画面暗転効果をマウスクリックで飛ばす。
		bool	no_mwnd_anime_flag;			// メッセージウィンドウの開閉時のアニメを無効にする。
		bool	wheel_next_message_flag;	// マウスのホイールボタンの下回しで文章を読み進める。
		bool	koe_dont_stop_flag;			// 声の再生中に次の文章に進んでも再生を続ける。
		bool	skip_unread_message_flag;	// 未読の文章も早送りできるようにする。
		bool	saveload_alert_flag;		// セーブロードの警告
		bool	saveload_dblclick_flag;		// セーブロードのダブルクリック
	}	system;

	// 他
	TSTR	ss_path;				// シーンフォルダパス
	TSTR	editor_path;			// エディタパス
	TSTR	koe_path;				// 声パス
	TSTR	koe_tool_path;			// 声ツールパス
};

