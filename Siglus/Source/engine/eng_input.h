#pragma		once

// ****************************************************************
// エンジン：入力関係の関数
// ================================================================

// マウスの位置を取得
C_point tnm_input_get_mouse_pos();

// キーが押されているかを判定
bool tnm_input_is_key_down(int key);

// キーが放されているかを判定
bool tnm_input_is_key_up(int key);

// キーの押しを判定
bool tnm_input_check_key_down(int key);

// キーの放しを判定
bool tnm_input_check_key_up(int key);

// キーの押し放しを判定
bool tnm_input_check_key_down_up(int key);

// キーの押しを使用
bool tnm_input_use_key_down(int key);

// キーの放しを使用
bool tnm_input_use_key_up(int key);

// キーの押し放しを使用
bool tnm_input_use_key_down_up(int key);
