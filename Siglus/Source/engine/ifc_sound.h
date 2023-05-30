#pragma		once

// ****************************************************************
// サウンド：インターフェース
// ================================================================

// BGM の個数を取得
int tnm_bgm_bgm_cnt();

// BGM 番号から BGM 登録名を取得
TSTR tnm_get_regist_name_by_bgm_no(int bgm_no);

// BGM 登録名から BGM 番号を取得
int tnm_get_bgm_no_by_regist_name(CTSTR& bgm_regist_name);

// BGM がなっているかを判定
bool tnm_check_bgm_playing();

// 声を再生する
void tnm_play_koe(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only_flag);

// 声を停止する
void tnm_stop_koe();

// SE を再生する
void tnm_play_se(int se_no);

// ムービーが準備中かを判定
bool tnm_is_movie_readying();

// ムービーが再生中かを判定
bool tnm_is_movie_playing();

// ムービーを空読みしてみる
bool tnm_preload_omv(CTSTR& file_name);

// サウンドのボリュームを更新する
bool tnm_update_sound_volume(bool change_force);
