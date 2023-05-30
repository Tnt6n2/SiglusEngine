#pragma		once

// ****************************************************************
// システムコマンドメニュー
// ================================================================
bool	tnm_rmenu_open();										// 開く
bool	tnm_rmenu_create(C_popup_menu& menu);					// 作成
bool	tnm_rmenu_create_funk(C_menu& menu, int id_rep = 0);	// 作成ファンクション
bool	tnm_rmenu_select(C_popup_menu& menu);					// 選択開始
bool	tnm_rmenu_execute(int syscom_id);						// 処理実行

