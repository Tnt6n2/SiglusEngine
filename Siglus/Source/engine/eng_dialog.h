#pragma		once

// ****************************************************************
// ダイアログマネージャ処理
// ================================================================

// 全てのラジオボタンのフォーカスをはずす
void	tnm_dlg_kill_focus_for_all_radio();

// 全てのダイアログの許可／禁止を設定する
void	tnm_dlg_set_enable(bool enable);

// マウスがダイアログに乗っているかを判定する
bool	tnm_dlg_check_mouse_over();

// 環境設定ダイアログが開いているかを判定する
bool	tnm_dlg_is_config_dialog_opened();

// 環境設定ダイアログを開く
void	tnm_dlg_open_config_dialog();

// デバッグ設定ダイアログを開く
void	tnm_dlg_open_debug_setting_dialog();

// トレースダイアログを開く
void	tnm_dlg_open_trace_dialog();

// エラー情報ダイアログを開く
void	tnm_dlg_open_error_info_dialog();

// Twitter ダイアログを開く
void	tnm_dlg_open_twitter_dialog();

// Twitter ダイアログを閉じる
void	tnm_dlg_close_twitter_dialog();

// Twitter ダイアログを更新する
void	tnm_dlg_update_twitter_dialog();

// Twitter 投稿に成功した
void	tnm_dlg_twitter_dialog_on_success_tweet();

