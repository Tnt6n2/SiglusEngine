#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_world.h"
#include	"element/elm_excall.h"
#include	"dialog/db_element_info.h"
#include	"dialog/db_wnd_info_world.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ワールド
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_world")

// 初期化
void C_db_wnd_info_world::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ワールド情報"));
}

// 初期化ファンクション
void C_db_wnd_info_world::on_init_func()
{
	// リストビュー項目
	tnm_init_lv_element(FM_WORLDLIST, true, &lv);
}

// 閉じるファンクション
void C_db_wnd_info_world::on_close_func()
{
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_world::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// 通知
bool C_db_wnd_info_world::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	if (ctrl_id == lv.get_id())	{
		if (hdr->code == NM_DBLCLK)	{
			on_dblclk_lv();
		}
		else if (hdr->code == LVN_CHANGE_TEXT)	{
			on_change_lv_value(((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->column_no, ((NMLVCHANGE *)lp)->new_text);
		}
	}

	return true;
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_world::frame()
{
	if (!m_h_wnd)
		return;

	// フロントのグループ情報を表示する
	int item_no = 0;
	int stage_no = TNM_STAGE_FRONT;
	tnm_update_lv_list_table(&(*Gp_stage_list)[stage_no].m_world_list, get_lv(), &item_no, false);
	if (Gp_excall->is_ready())	{	// ＥＸコール中はＥＸグループも表示
		tnm_update_lv_list_table(&Gp_excall->m_stage_list[stage_no].m_group_list, get_lv(), &item_no, false);
	}
	tnm_reduce_item_cnt(get_lv(), item_no);
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_world::on_dblclk_lv()
{
	// 選択状態取得
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// マウスカーソルの乗っているセル番号
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// アイテムを変更する
	if (select_cnt == 1 && select_no == item_no)	{
		tnm_on_dblclk_lv_world_list_table(&lv, item_no, column_no);
	}
}

// ****************************************************************
// リストビューの値を変更した
// ================================================================
void C_db_wnd_info_world::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	int stage_no = TNM_STAGE_FRONT;
	tnm_on_change_lv_value_world_list_table(&lv, item_no, column_no, value, &(*Gp_stage_list)[stage_no].m_world_list);
}
