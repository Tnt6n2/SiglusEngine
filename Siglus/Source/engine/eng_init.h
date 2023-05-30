#pragma		once

// エンジンのローカル初期化
bool	tnm_init_local();

// エンジンのローカル再初期化
bool	tnm_reinit_local(bool restruct_flag);

// エンジンのローカル終了処理
bool	tnm_finish_local();

// エンジンの解放
bool	tnm_free();


