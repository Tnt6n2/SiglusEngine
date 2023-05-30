#pragma		once

struct	S_tnm_save_header;
struct	S_tnm_local_save;


// ****************************************************************
// セーブロード処理
// ================================================================

bool	tnm_save_local();						// セーブ（ローカル）
bool	tnm_save_local_msg(CTSTR& msg);			// セーブ（ローカルの現在の文章）
bool	tnm_load_local();						// ロード（ローカル）
bool	tnm_save_call();						// セーブ（コール）
bool	tnm_load_call();						// ロード（コール）

// セーブデータシステム：ローカルセーブ
void	tnm_init_local_save();								// ローカルセーブデータを初期化
void	tnm_set_local_save(S_tnm_local_save* p_local_save);	// ローカルセーブデータを設定
bool	tnm_is_exist_local_save();							// ローカルセーブデータを判定

// セーブデータシステム：選択肢セーブ
void	tnm_init_sel_save();								// 選択肢セーブデータを初期化
void	tnm_get_sel_save(S_tnm_local_save* p_local_save);	// 選択肢セーブデータを取得
bool	tnm_is_exist_sel_save();							// 選択肢セーブデータを判定
void	tnm_copy_local_save_to_sel_save();					// ローカルセーブデータを選択肢セーブデータにコピー

// セーブデータシステム：選択肢セーブストック
void	tnm_init_sel_save_stock();							// 選択肢セーブデータを初期化
void	tnm_copy_local_save_to_sel_save_stock();			// ローカルセーブデータを選択肢セーブデータにコピー
void	tnm_copy_sel_save_stock_to_sel_save();				// 選択肢セーブデータをコピー

// セーブデータシステム：インナーセーブ
void	tnm_savedata_system_clear_inner_save(int inner_save_no);
void	tnm_savedata_system_copy_local_save_to_inner_save(int inner_save_no);
void	tnm_savedata_system_copy_inner_save(int inner_save_no_from, int inner_save_no_to);
void	tnm_savedata_system_get_inner_save(int inner_save_no, S_tnm_local_save* p_local_save);
bool	tnm_savedata_system_check_inner_save(int inner_save_no);
int		tnm_savedata_system_get_inner_save_size(int inner_save_no);

// セーブデータシステム：バックログセーブ
bool	tnm_savedata_system_check_backlog_save(int save_index);

// セーブデータシステム：セーブ履歴
void	tnm_init_save_history();				// セーブ履歴を初期化

// セーブデータシステム：メッセージバックセーブ
void	tnm_init_backlog_save();					// メッセージバックセーブを初期化
bool	tnm_check_backlog_save(S_tid save_id);		// メッセージバックセーブの存在判定
void	tnm_remove_backlog_save();					// メッセージバックを整理

// ファイルＩＯ
bool	tnm_load_system_on_file();				// システム設定データを読み込む
bool	tnm_save_global_on_file();				// グローバルデータをファイルにセーブ
bool	tnm_load_global_on_file();				// グローバルデータをファイルからロード
bool	tnm_save_local_on_file(int save_no);	// ローカルデータをファイルにセーブ
bool	tnm_load_local_on_file(int save_no);	// ローカルデータをファイルからロード
bool	tnm_save_save_header(int save_no, S_tnm_save_header* p_header);		// セーブヘッダをセーブ
bool	tnm_load_save_header(int save_no, S_tnm_save_header* p_header);		// セーブヘッダをロード
int		tnm_get_new_save_no();							// 最新のセーブデータ番号を取得する
int		tnm_get_new_save_no(int start, int cnt);		// 最新のセーブデータ番号を取得する
int		tnm_get_new_quick_save_no();					// 最新のクイックセーブデータ番号を取得する
int		tnm_get_new_quick_save_no(int start, int cnt);	// 最新のクイックセーブデータ番号を取得する

bool	tnm_is_save_enable();					// セーブ可能かどうか判定
bool	tnm_is_save_enable(int save_no);		// セーブ可能かどうか判定（番号指定）
bool	tnm_is_load_enable();					// ロード可能かどうか判定
bool	tnm_is_load_enable(int save_no);		// ロード可能かどうか判定（番号指定）

bool	tnm_check_save_file(int save_no);						// セーブデータの存在判定
bool	tnm_copy_save_file(int save_no_from, int save_no_to);	// セーブデータをコピーする
bool	tnm_delete_save_file(int save_no);						// セーブデータを削除する
bool	tnm_change_save_file(int save_no_0, int save_no_1);		// セーブデータを交換する

