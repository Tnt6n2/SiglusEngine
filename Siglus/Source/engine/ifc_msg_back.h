#pragma		once

#include	"data/tnm_timer.h"

// ****************************************************************
// エンジン：メッセージバック：インターフェース
// ================================================================

// メッセージバックをクリア
void tnm_msg_back_clear();

// メッセージバックに文字があるかを判定
bool tnm_msg_back_is_exist_msg();

// メッセージバックが使用可能かを判定
bool tnm_msg_back_is_enable();

// メッセージバックを開く
void tnm_msg_back_open();

// メッセージバックを閉じる
void tnm_msg_back_close();

// メッセージバックが開いているかを判定
bool tnm_msg_back_is_open();

// メッセージバックを進める
void tnm_msg_back_next();

// メッセージバックにセーブIDを設定する
void tnm_msg_back_set_save_id(S_tid save_id);

// メッセージバックにセーブIDチェックフラグを設定する
void tnm_msg_back_set_save_id_check_flag(bool save_id_check_flag);

// メッセージバックのセーブIDチェックフラグを取得する
bool tnm_msg_back_get_save_id_check_flag();

// メッセージバックに声を追加する
bool tnm_msg_back_add_koe(int koe_no, int chr_no, int scn_no, int line_no);

// メッセージバックに名前を追加する
bool tnm_msg_back_add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no);

// メッセージバックにメッセージを追加する
bool tnm_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no);

// メッセージバックに画像を追加する
bool tnm_msg_back_add_pct(CTSTR& file_name, int x, int y);

// メッセージバックのシーン番号を取得
int tnm_msg_back_get_scn_no();

// メッセージバックの行番号を取得
int tnm_msg_back_get_line_no();

// メッセージバックの声番号を取得
int tnm_msg_back_get_koe_no();

