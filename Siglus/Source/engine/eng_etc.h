#pragma		once

// ****************************************************************
// エンジン：エトセトラ
// ================================================================
bool	tnm_excall_is_excall();							// ＥＸコール中
bool	tnm_excall_is_msg_back();						// メッセージバック中
bool	tnm_excall_is_hide_mwnd();						// メッセージウィンドウ消去中

bool	tnm_is_skipping();								// 早送り中かを判定
bool	tnm_is_skipping_without_skip_trigger();			// 早送り中かを判定（スキップトリガーを除く）
bool	tnm_is_skipping_msg();							// メッセージ早送り中かを判定
bool	tnm_is_skip_unread_msg();						// 未読を早送りするかを判定
bool	tnm_is_auto_mode();								// オートモードを判定
int		tnm_get_auto_mode_moji_wait();					// オートモードの文字待ち時間を取得
int		tnm_get_auto_mode_min_wait();					// オートモードの最小待ち時間を取得
bool	tnm_is_mwnd_anime_use();						// ウィンドウアニメを判定
bool	tnm_is_hide_mwnd();								// メッセージウィンドウを隠すかを判定
bool	tnm_is_cursor_disp();							// カーソルを表示するかを判定
bool	tnm_is_koe_play(int chara_no);					// 声を再生するかを判定
bool	tnm_is_stop_koe();								// 声を止めるかを判定
bool	tnm_get_chrkoe_onoff(int chara_no);				// キャラクター音声オンオフを取得
int		tnm_get_chrkoe_volume(int chara_no);			// キャラクター音声ボリュームを取得
void	tnm_is_chrkoe_name_check(CTSTR& name_str);		// キャラクター音声の名前の伏字を公開するかを判定（公開フラグをＯＮにします）
void	tnm_update_read_skip();							// 既読早送りを更新
void	tnm_update_start_skip();						// 自動早送りを開始
int		tnm_get_total_msg_speed();						// メッセージスピードを取得

// ゲーム名を取得
TSTR tnm_get_game_name();

// シーン番号からシーン名を取得
TSTR tnm_get_scn_name(int scn_no);

// シーン番号からフルパス名を取得
TSTR tnm_get_scn_full_path_name(int scn_no);

// シーンタイトルを取得
TSTR tnm_get_scene_title();

// ウィンドウに表示するフルタイトルを取得
TSTR tnm_get_full_title();

// ウィンドウ名を取得
TSTR tnm_get_window_name();

