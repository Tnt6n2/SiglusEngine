#pragma		once

// ユーザフラグＩＤからユーザプロパティを取得
C_tnm_prop* tnm_flag_get_user_prop(int user_prop_id);

// ダミーチェックフラグをクリア
void clear_dummy_check_flag();

// ダミーチェックフラグを設定
void set_dummy_check_flag(int value);

// ダミーチェックフラグを判定
bool check_dummy_check_flag(int value);
