#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_element_code.h"
#include	"dialog/db_wnd_info_flag.h"
#include	"dialog/db_wnd_info_element_lv.h"
#include	"element/elm_flag.h"
#include	"element/elm_call.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_scene.h"

// ****************************************************************
// デバッグ情報ウィンドウ：フラグ
// ================================================================

// 初期化
void C_db_wnd_info_flag::init()
{
	// リストビュー項目
	listview_add_column(_T("Flag"), _T("ﾌﾗｸﾞ"), 50);
	listview_add_column(_T("value"), _T("値"), 50);
}

// 初期化ファンクション
void C_db_wnd_info_flag::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_flag::on_close_func()
{
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// 通知
bool C_db_wnd_info_flag::on_notify(WPARAM wp, LPARAM lp)
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
void C_db_wnd_info_flag::frame()
{
	if (!m_h_wnd)
		return;

	// フラグを取得する
	C_elm_int_list* p_int_list = NULL;
	C_elm_str_list* p_str_list = NULL;
	get_page_flag(&p_int_list, &p_str_list);

	if (p_int_list)	{

		// リストビューのアイテム数を設定
		if (frame_first_flag || p_int_list->get_size() != m_ld.int_flag.size())	{
			lv.set_item_cnt(p_int_list->get_size());
		}
		// 増えた分に関してフラグ名を表示
		for (int i = frame_first_flag ? 0 : (int)m_ld.int_flag.size(); i < p_int_list->get_size(); i++)	{
			lv.set_cell_text(i, 0, tostr(i));
		}
		// 増えたもしくは変更された分に関して値を表示
		for (int i = 0; i < (int)p_int_list->get_size(); i++)	{
			if (frame_first_flag || i >= (int)m_ld.int_flag.size() || (*p_int_list)[i] != m_ld.int_flag[i])	{
				lv.set_cell_text(i, 1, tostr((*p_int_list)[i]));
			}
		}

		// 前回の値を保存
		m_ld.int_flag = p_int_list->get_sub();
	}
	else if (p_str_list)	{

		// リストビューのアイテム数を設定
		if (frame_first_flag || p_str_list->get_size() != m_ld.str_flag.size())	{
			lv.set_item_cnt(p_str_list->get_size());
		}
		// 増えた分に関してフラグ名を表示
		for (int i = frame_first_flag ? 0 : (int)m_ld.str_flag.size(); i < p_str_list->get_size(); i++)	{
			lv.set_cell_text(i, 0, tostr(i));
		}
		// 増えたもしくは変更された分に関して値を表示
		for (int i = 0; i < (int)p_str_list->get_size(); i++)	{
			if (frame_first_flag || i >= (int)m_ld.str_flag.size() || (*p_str_list)[i] != m_ld.str_flag[i])	{
				lv.set_cell_text(i, 1, (*p_str_list)[i]);
			}
		}

		// 前回の値を保存
		m_ld.str_flag = p_str_list->get_sub();
	}

	frame_first_flag = false;
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_flag::on_dblclk_lv()
{
	// 選択状態取得
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// マウスカーソルの乗っているセル番号
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// アイテムを変更する
	if (select_cnt == 1 && select_no == item_no && column_no == 1)	{

		// フラグを取得する
		C_elm_int_list* p_int_list;
		C_elm_str_list* p_str_list;
		get_page_flag(&p_int_list, &p_str_list);
		if (p_int_list)	{
			tnm_on_dblclk_lv_int_list_table(&lv, item_no, column_no, 32);
		}
		else if (p_str_list)	{
			tnm_on_dblclk_lv_str_list_table(&lv, item_no, column_no);
		}
	}
}

// ****************************************************************
// リストビューの値を変更した
// ================================================================
void C_db_wnd_info_flag::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	// フラグを取得する
	C_elm_int_list* p_int_list = NULL;
	C_elm_str_list* p_str_list = NULL;
	get_page_flag(&p_int_list, &p_str_list);

	if (p_int_list)	{
		tnm_on_change_lv_value_int_list_table(&lv, item_no, column_no, value, p_int_list, 32, 0);
	}
	else if (p_str_list)	{
		tnm_on_change_lv_value_str_list_table(&lv, item_no, column_no, value, p_str_list);
	}
}

// ****************************************************************
// フラグを取得する
// ================================================================
void C_db_wnd_info_flag::get_page_flag(C_elm_int_list** pp_int_list, C_elm_str_list** pp_str_list)
{
	*pp_int_list = NULL;
	*pp_str_list = NULL;
	switch (m_page_no)	{
		case 0:		*pp_int_list = &Gp_flag->A;	break;
		case 1:		*pp_int_list = &Gp_flag->B;	break;
		case 2:		*pp_int_list = &Gp_flag->C;	break;
		case 3:		*pp_int_list = &Gp_flag->D;	break;
		case 4:		*pp_int_list = &Gp_flag->E;	break;
		case 5:		*pp_int_list = &Gp_flag->F;	break;
		case 6:		*pp_int_list = &Gp_flag->X;	break;
		case 7:		*pp_str_list = &Gp_flag->S;	break;
		case 8:		*pp_int_list = &Gp_flag->G;	break;
		case 9:		*pp_int_list = &Gp_flag->Z;	break;
		case 10:	*pp_str_list = &Gp_flag->M;	break;
		case 11:	*pp_int_list = &tnm_get_cur_call()->L;	break;
		case 12:	*pp_str_list = &tnm_get_cur_call()->K;	break;
	}
}




