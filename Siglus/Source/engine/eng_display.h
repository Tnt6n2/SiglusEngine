#pragma		once

// ****************************************************************
// ディスプレイ関係の処理
// ================================================================
void	tnm_change_screen_size_mode_by_force(bool first);							// スクリーンサイズモードを変更（強制）
void	tnm_change_screen_size_mode_window(bool first);								// スクリーンサイズモードを変更（ウィンドウ）
void	tnm_change_screen_size_mode_window(C_size screen_size_scale, bool first);	// スクリーンサイズモードを変更（ウィンドウ・サイズ指定）
void	tnm_change_screen_size_mode_free(C_point window_pos, C_size screen_size_free, bool first);		// スクリーンサイズモードを変更（フリー・サイズ指定）
void	tnm_change_screen_size_mode_fullscreen(bool first, bool warning);			// スクリーンサイズモードを変更（フルスクリーン）
void	tnm_switch_screen_size_mode(bool first);									// スクリーンサイズモードを変更（切り替え）
void	tnm_update_screen_size_mode();												// スクリーンサイズモードを更新（起動時）
void	tnm_reset_screen_size_mode();												// スクリーンサイズモードを元に戻す（確認ダイアログでいいえを選んだ）

C_size	tnm_get_screen_size_from_size_mode(int screen_size_mode);									// スクリーンサイズモードからスクリーンサイズを取得
C_size	tnm_get_window_size_from_size_mode(int screen_size_mode);									// スクリーンサイズモードからウィンドウサイズを取得
bool	tnm_get_window_size_from_size_scale(C_size screen_size_scale);								// スクリーンサイズが使用可能かを判定
bool	tnm_rep_screen_size(int screen_size_mode, C_size* screen_size_scale, C_size* screen_size_free);		// スクリーンサイズ補正
bool	tnm_rep_screen_size_window(C_size* screen_size_scale);										// スクリーンサイズ補正
bool	tnm_rep_screen_size_free(C_size* screen_size_free);											// スクリーンサイズ補正
int		tnm_format_screen_size_to_screen_mode(C_size screen_size_scale);							// スクリーンサイズ比率からスクリーンモードに変換（取得？）
void	tnm_format_screen_mode_to_screen_size_scale(int size_mode, C_size* screen_size_scale);		// スクリーンモードからスクリーンサイズ比率に変換

void	tnm_set_wait_display_vsync(bool wait_display_vsync_flag);					// 垂直同期を待つ設定を変更する

