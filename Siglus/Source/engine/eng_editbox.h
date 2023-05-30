#pragma		once

// ****************************************************************
// エディットボックス関連の関数
// ================================================================

// エディットボックスを全て非表示にする
void tnm_editbox_set_hide_all();

// エディットボックスのフォーカスを次に移動（Tab キーによる処理）
void tnm_editbox_move_focus_forward();

// エディットボックスのフォーカスを前に移動（Shift+Tab キーによる処理）
void tnm_editbox_move_focus_backward();

// エディットボックスにマウスが乗っているかを判定
bool tnm_editbox_check_mouse_over();


