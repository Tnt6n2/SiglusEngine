#pragma		once

// ****************************************************************
// G00 バッファのインターフェース
// ================================================================

// G00 バッファの個数を取得する
int tnm_g00_buf_get_g00_buf_cnt();

// G00 バッファにファイルを読み込む
void tnm_g00_buf_load_file(C_elm_g00_buf* p_g00_buf, CTSTR& file_name);

// G00 バッファを解放する
void tnm_g00_buf_free_buf(C_elm_g00_buf* p_g00_buf);

// G00 バッファを全て解放する
void tnm_g00_buf_free_all_buf();

// G00 バッファにファイルが読み込まれているかを取得する
bool tnm_g00_buf_is_load(C_elm_g00_buf* p_g00_buf);

// G00 バッファに読み込まれたファイルのファイル名を取得する
TSTR tnm_g00_buf_get_file_name(C_elm_g00_buf* p_g00_buf);
