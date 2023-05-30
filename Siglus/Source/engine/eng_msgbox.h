#pragma		once

// ****************************************************************
// メッセージボックスの処理
// ================================================================
int		tnm_game_message_box(CTSTR& msg, UINT type);	// ゲーム用メッセージ
int		tnm_game_warning_box(CTSTR& msg);				// ゲーム用確認メッセージ
int		tnm_game_info_box(CTSTR& msg);					// ゲーム用情報メッセージ

