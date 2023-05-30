#pragma		once

// ****************************************************************
// ＢＧＭテーブルのインターフェース
// ================================================================

// BGM テーブルの BGM の個数を取得
int tnm_bgm_table_get_bgm_cnt();

// BGM テーブルの既聴フラグを設定する
void tnm_bgm_table_set_listened(int bgm_no, bool value);

// BGM テーブルの既聴フラグを設定する
void tnm_bgm_table_set_listened(CTSTR& bgm_name, bool value);

// BGM テーブルの既聴フラグを全て設定する
void tnm_bgm_table_set_all_flag(bool value);

// BGM テーブルの既聴フラグを取得する
bool tnm_bgm_table_get_listened(int bgm_no);

// BGM テーブルの既聴フラグを取得する（-1: 登録されていない）
int tnm_bgm_table_get_listened(CTSTR& bgm_name);

