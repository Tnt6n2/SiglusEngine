#include	"pch.h"
#include	"ifc_eng.h"
#include	"dialog/db_wnd_info.h"
#include	"engine/eng_debug.h"

// ****************************************************************
// デバッグウィンドウ
// ================================================================

// ****************************************************************
// デバッグ用のリストボックス
// ================================================================
//		set_text などをのっとります。
//		SiglusDebuger.dll がない限りリストビューにテキストを表示できません。
// ================================================================
int C_tnm_dbg_listbox::add_item(CTSTR& str)
{
	if (Gp_debug_dll->lb_add_item_001)	{
		return Gp_debug_dll->lb_add_item_001(get_handle(), str);
	}

	return 0;
}

int C_tnm_dbg_listbox::insert_item(int pos, CTSTR& str)
{
	if (Gp_debug_dll->lb_add_item_001)	{
		return Gp_debug_dll->lb_insert_item_001(get_handle(), pos, str);
	}

	return 0;
}

// ****************************************************************
// デバッグ用のリストビュー
// ================================================================
//		set_cell_text などをのっとります。
//		SiglusDebuger.dll がない限りリストビューにテキストを表示できません。
// ================================================================
int C_tnm_dbg_listview::add_item()
{
	if (Gp_debug_dll->lv_add_item_001)	{
		return Gp_debug_dll->lv_add_item_001(get_handle());
	}

	return 0;
}

int C_tnm_dbg_listview::add_item(int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_add_item_002)	{
		return Gp_debug_dll->lv_add_item_002(get_handle(), column_no, str);
	}

	return 0;
}

int C_tnm_dbg_listview::add_item_visible(int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_add_item_visible_001)	{
		return Gp_debug_dll->lv_add_item_visible_001(get_handle(), column_no, str);
	}

	return 0;
}

int C_tnm_dbg_listview::insert_item(int item_no)
{
	if (Gp_debug_dll->lv_insert_item_001)	{
		return Gp_debug_dll->lv_insert_item_001(get_handle(), item_no);
	}

	return 0;
}

int C_tnm_dbg_listview::insert_item(int item_no, int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_insert_item_002)	{
		return Gp_debug_dll->lv_insert_item_002(get_handle(), item_no, column_no, str);
	}

	return 0;
}

int C_tnm_dbg_listview::insert_item_visible(int item_no, int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_insert_item_visible_001)	{
		return Gp_debug_dll->lv_insert_item_visible_001(get_handle(), item_no, column_no, str);
	}

	return 0;
}

void C_tnm_dbg_listview::reduce_item(int item_cnt)
{
	if (Gp_debug_dll->lv_reduce_item_001)	{
		Gp_debug_dll->lv_reduce_item_001(get_handle(), item_cnt);
	}
}

void C_tnm_dbg_listview::set_item_cnt(int item_cnt)
{
	if (Gp_debug_dll->lv_set_item_cnt_001)	{
		Gp_debug_dll->lv_set_item_cnt_001(get_handle(), item_cnt);
	}
}

void C_tnm_dbg_listview::set_cell_text(int item_no, int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_set_cell_text_001)	{
		Gp_debug_dll->lv_set_cell_text_001(get_handle(), item_no, column_no, str);
	}
}

void C_tnm_dbg_listview::set_cell_text_visible(int item_no, int column_no, CTSTR& str)
{
	if (Gp_debug_dll->lv_set_cell_text_visible_001)	{
		Gp_debug_dll->lv_set_cell_text_visible_001(get_handle(), item_no, column_no, str);
	}
}
