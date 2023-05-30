#pragma		once

#include	"tnm_sorter.h"
#include	"tnm_timer.h"

// ****************************************************************
// エンジン：グローバルデータ
// ================================================================
//		スクリプトからは触れない設定を入れるつもりです。
//		また、これらはセーブの対象になりません。
// ================================================================
class C_tnm_global_data
{
public:
	bool		debug_flag;						// デバッグフラグ
	bool		debug_decided_flag;				// デバッグかどうかが確定したフラグ
	bool		debug_not_disp_overflow_error;	// オーバーフロー時にエラーを出さない
	bool		shader_from_source;				// シェーダーをソースから作成した
	int			setup_type;						// セットアップタイプ

	C_size		game_screen_size;				// ゲームスクリーンサイズ（起動時に固定）
	C_point		total_game_screen_pos;			// ゲームスクリーン位置合計（フルスクリーンの調整などを合計したゲーム画面位置）
	C_point		total_game_screen_pos_last;		// ゲームスクリーン位置合計（フルスクリーンの調整などを合計したゲーム画面位置）
	C_size		total_game_screen_size;			// ゲームスクリーンサイズ合計（フルスクリーンの調整などを合計したゲーム画面サイズ）
	C_size		total_game_screen_size_last;	// ゲームスクリーンサイズ合計（フルスクリーンの調整などを合計したゲーム画面サイズ）
	C_size		total_screen_size;				// スクリーンサイズ合計（フルスクリーンの調整などを合計したスクリーンサイズ）
	C_size		total_window_size;				// ウィンドウサイズ合計（フルスクリーンの調整などを合計したウィンドウサイズ）

	int			screen_size_mode;						// スクリーンサイズモード
	int			screen_size_mode_window;				// スクリーンサイズモード（ウィンドウ時）
	C_size		screen_size_scale;						// スクリーンサイズスケール
	C_size		screen_size_free;						// スクリーンサイズ（フリー）
	bool		fullscreen_change_resolution;			// フルスクリーンの解像度変更フラグ
	int			fullscreen_display_no;					// フルスクリーンのディスプレイ番号
	int			fullscreen_resolution_no;				// フルスクリーンの解像度番号
	int			screen_size_mode_change_start_time;		// スクリーンを切り替えた時間（画面設定ウィンドウ用）

	bool		dialog_fullscreen_change_resolution;	// フルスクリーン時に解像度を変更する（ダイアログの設定）
	int			dialog_fullscreen_display_no;			// フルスクリーンに使うディスプレイ番号（ダイアログの設定）
	int			dialog_fullscreen_resolution_no;		// フルスクリーンに使うディスプレイモード番号（ダイアログの設定）

	int			change_display_mode_proc_cnt;							// 解像度を変更した処理フラグ
	int			change_display_mode_proc_old_screen_size_mode;			// 解像度を変更する前のスクリーンサイズモード
	int			change_display_mode_proc_old_screen_size_mode_window;	// 解像度を変更する前のスクリーンサイズモード（ウィンドウ時）
	C_size		change_display_mode_proc_old_screen_size_scale;			// 解像度を変更する前のスクリーンサイズスケール
	C_size		change_display_mode_proc_old_screen_size_free;			// 解像度を変更する前のスクリーンサイズ（フリー）
	C_point		change_display_mode_proc_old_window_pos;				// 解像度を変更する前のウィンドウ位置
	bool		change_display_mode_proc_old_change_resolution;			// 解像度を変更する前の解像度変更フラグ
	int			change_display_mode_proc_old_display_no;				// 解像度を変更する前のディスプレイ番号
	int			change_display_mode_proc_old_resolution_no;				// 解像度を変更する前の解像度番号

	bool		wait_display_vsync_def;			// 垂直同期を待つ（デフォルト値）
	bool		wait_display_vsync_total;		// 垂直同期を待つ（合計）
	int			wipe_buffer_cnt;				// ワイプバッファの個数

	TSTR		start_scene_name;				// スタートシーン名
	int			start_z_no;						// スタートＺ番号

	bool		active_flag;					// ウィンドウがアクティブである
	bool		focused_flag;					// ウィンドウにフォーカスがある
	bool		focused_flag_on_edit_box;		// エディットボックスにフォーカスがある
	bool		editbox_hide_flag_total;		// エディットボックスを隠すフラグ

	bool		ex_call_flag;					// ＥＸコール中
	bool		ctrl_skip_flag;					// Ctrl 早送り実行中
	bool		cs_skip_flag;					// Ctrl+Shift 早送り実行中
	bool		alt_menu_wait_flag;				// Alt メニューを開いた
	bool		skip_click_1_frame;				// １フレームだけクリックを禁止する
	bool		cant_auto_skip_before_click;	// クリックするまで自動早送りを行わない

	int			frame_rate_100msec_total;		// フレームレート合計
	int			frame_rate_100msec_index;		// フレームレートインデックス
	int			frame_rate_100msec[10];			// フレームレート
	bool		read_skip_enable_flag;			// 既読早送りが可能かどうか
	bool		is_auto_mode_count_start;		// オートモードカウントを開始した
	int			auto_mode_count_start_time;		// オートモードカウント開始時刻
	bool		auto_mode_cursor_off;			// オートモードのためカーソルを消去
	int			mouse_cursor_hide_time;			// オートモードカウント開始時刻
	bool		is_koe_only_count_start;		// 声のみモードの声が終わってからの待ち時間カウントを開始した
	int			koe_only_count_start_time;		// 声のみモードの声が終わってからの待ち時間カウント開始時刻
	int			all_total_volume;				// 全体の合計ボリューム
	int			bgm_buf_total_volume;			// ＢＧＭの合計ボリューム
	int			koe_buf_total_volume;			// 声の合計ボリューム（声もしくはＥＸ声、使っている方のボリュームになる）
	int			pcm_buf_total_volume;			// 効果音の合計ボリューム
	int			se_buf_total_volume;			// システム音の合計ボリューム
	int			mov_buf_total_volume;			// ムービーの合計ボリューム
	bool		play_silent_sound;				// バックグラウンドで無音を再生する
	bool		bgmfade_flag;					// ＢＧＭフェード中
	int			bgmfade_cur_time;				// ＢＧＭフェード開始の現在時刻
	int			bgmfade_start_value;			// ＢＧＭフェード開始の開始値
	int			bgmfade_total_volume;			// ＢＧＭフェードの最終計算結果
	bool		bgmfade2_flag;					// ＢＧＭフェード２中
	bool		bgmfade2_need_flag;				// ＢＧＭフェード２が必要
	int			bgmfade2_cur_time;				// ＢＧＭフェード２開始の現在時刻
	int			bgmfade2_start_value;			// ＢＧＭフェード２開始の開始値
	int			bgmfade2_total_volume;			// ＢＧＭフェード２の最終計算結果
	double		mouse_move_left_amari;			// マウスカーソルをキーボードで操作
	double		mouse_move_right_amari;			// マウスカーソルをキーボードで操作
	double		mouse_move_up_amari;			// マウスカーソルをキーボードで操作
	double		mouse_move_down_amari;			// マウスカーソルをキーボードで操作
	TSTR		total_font_name;				// フォント名
	bool		total_font_futoku;				// フォント太く
	int			total_font_shadow;				// フォント影

	bool		disp_flag;						// 描画を行うフラグ
	bool		present_flag;					// 画面更新を行うフラグ
	int			disp_because_msg_wait_cnt;		// メッセージウェイトが理由で描画を行う
	int			disp_because_msg_wait_cnt_max;	// メッセージウェイトが理由で描画を行う
	bool		system_wipe_flag;				// システムワイプ中フラグ（ロード時などのワイプ）
	bool		skip_because_skip_trigger;		// スキップトリガーによるスキップ

	int				capture_type;				// キャプチャータイプ
	S_tnm_sorter	capture_sorter;				// キャプチャーソーター
	int				capture_prior;				// 現在のキャプチャーの優先度
	int				capture_time;				// キャプチャーした時刻
	int				capture_x;					// キャプチャー先のＸ座標
	int				capture_y;					// キャプチャー先のＹ座標
	int				capture_w;					// キャプチャーサイズ
	int				capture_h;					// キャプチャーサイズ
	TSTR			capture_file_name;			// キャプチャー保存ファイル名

	bool		game_timer_move_flag;			// ゲームタイマーを動かすフラグ
	bool		do_frame_action_flag;			// フレームアクションを強制的に実行するフラグ
	bool		do_load_after_call_flag;		// ロードアフターコールを実行するフラグ
	bool		sel_start_call_flag;			// 選択肢開始コールを実行するフラグ
	bool		msg_wait_skip_by_click;			// クリックでメッセージウェイトをスキップした
	bool		is_breaking;					// ブレーク中
	bool		break_step_flag;				// １ステップ進む
	bool		game_end_flag;					// ゲーム終了：ゲーム終了フラグ
	bool		game_end_no_warning;			// ゲーム終了：警告を出さずにゲームを終了する
	bool		game_end_save_done;				// ゲーム終了：エンドセーブを行った
	bool		reinit_msgbk_except_flag;		// 初期化：メッセージバックを残すフラグ
	S_tid		msg_back_load_tid;				// メッセージバックからのロードID
	TSTR		last_sel_msg;					// 最後に選択したメッセージ

	int			max_vertex_buffer_size;			// 頂点バッファサイズ
	int			index_buffer_size;				// インデックスバッファサイズ
	int			video_memory_rest_size;			// ビデオメモリ残りサイズ
	int			video_memory_use_size;			// ビデオメモリ使用サイズ
	int			video_memory_use_max_size;		// ビデオメモリ使用サイズ（最大値）

	TSTR		return_scene_name;
	int			return_scene_z_no;

	int			__bgm_stop_cntr;
};
