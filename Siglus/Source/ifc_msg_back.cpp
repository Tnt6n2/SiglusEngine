#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_msg_back.h"
#include	"engine/ifc_msg_back.h"

// ****************************************************************
// エンジン：メッセージバック：インターフェース
// ================================================================

// メッセージバックをクリア
void tnm_msg_back_clear()
{
	Gp_msg_back->clear_msg();
}

// メッセージバックに文字があるかを判定
bool tnm_msg_back_is_exist_msg()
{
	return Gp_msg_back->is_exist_msg();
}

// メッセージバックが使用可能かを取得
bool tnm_msg_back_is_enable()
{
	return Gp_msg_back->is_exist_msg() && !Gp_local->pod.msg_back_disable;
}

// メッセージバックを開く
void tnm_msg_back_open()
{
	Gp_msg_back->open();
}

// メッセージバックを閉じる
void tnm_msg_back_close()
{
	Gp_msg_back->close();
}

// メッセージバックが開いているかを判定する
bool tnm_msg_back_is_open()
{
	return Gp_msg_back->is_open();
}

// メッセージバックを進める
void tnm_msg_back_next()
{
	Gp_msg_back->next_msg();
}

// メッセージバックにセーブ ID を設定する
void tnm_msg_back_set_save_id(S_tid save_id)
{
	Gp_msg_back->set_save_id(save_id);
}

// メッセージバックにセーブIDチェックフラグを設定する
void tnm_msg_back_set_save_id_check_flag(bool save_id_check_flag)
{
	Gp_msg_back->set_save_id_check_flag(save_id_check_flag);
}

// メッセージバックのインターバルチェックフラグを取得する
bool tnm_msg_back_get_save_id_check_flag()
{
	return Gp_msg_back->get_save_id_check_flag();
}

// メッセージバックに声を追加する
bool tnm_msg_back_add_koe(int koe_no, int chara_no, int scn_no, int line_no)
{
	return Gp_msg_back->add_koe(koe_no, chara_no, scn_no, line_no);
}

// メッセージバックに名前を追加する
bool tnm_msg_back_add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no)
{
	// 名前を追加する
	return Gp_msg_back->add_name(original_name, disp_name, scn_no, line_no);
}

// メッセージバックにメッセージを追加する
bool tnm_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	return Gp_msg_back->add_msg(msg, debug_msg, scn_no, line_no);
}

// メッセージバックに画像を追加する
bool tnm_msg_back_add_pct(CTSTR& file_name, int x, int y)
{
	return Gp_msg_back->add_pct(file_name, C_point(x, y));
}

// メッセージバックのシーン番号を取得
int tnm_msg_back_get_scn_no()
{
	return Gp_msg_back->get_scn_no();
}

// メッセージバックの行番号を取得
int tnm_msg_back_get_line_no()
{
	return Gp_msg_back->get_line_no();
}

// メッセージバックの声番号を取得
int tnm_msg_back_get_koe_no()
{
	return Gp_msg_back->get_koe_no();
}

