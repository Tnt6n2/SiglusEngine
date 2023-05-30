
#include	<windows.h>
#include	<tchar.h>
#include	<string>
#include	<commctrl.h>

typedef	std::basic_string<char>		MBSTR;	// マルチバイト文字列
typedef	std::basic_string<wchar_t>	WSTR;	// ワイド文字列
typedef	std::basic_string<TCHAR>	TSTR;	// 文字列
typedef const MBSTR					CMBSTR;
typedef const WSTR					CWSTR;
typedef const TSTR					CTSTR;

// ****************************************************************
// ここから dll
// ================================================================
#include	<tnmd.h>

static	HINSTANCE	g_h_inst = NULL;
static	int			g_major_version = 3;
static	int			g_minor_version = 0;

// ****************************************************************
// dll エントリ
// ================================================================
extern "C" int APIENTRY DllMain(HINSTANCE h_inst, DWORD reason, LPVOID reserved)
{
	g_h_inst = h_inst;

	return TRUE;
}

// ****************************************************************
// dll のバージョンを取得
// ================================================================
int get_major_version(char* p_password_buf)
{
	memcpy(p_password_buf, "aQpbZF$ijgDSLL19hY#)An36;Ij@ln+G", 32);
	return g_major_version;
}
int get_minor_version(char* p_key_buf)
{
	memcpy(p_key_buf, "7hG)o!bNa>tvV{wcc-D>EXpPfq7Bn*sU", 32);
	return g_minor_version;
}

// ****************************************************************
// 汎用：メッセージを送る
// ================================================================
int send_message(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return (int)SendMessage(h_wnd, msg, wp, lp);
}

// ****************************************************************
// リストボックス：アイテムの追加
// ================================================================
int lb_add_item_001(HWND h_lb, CTSTR &str)
{
	return (int)send_message(h_lb, LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

// ****************************************************************
// リストボックス：アイテムの追加（位置を指定）
// ================================================================
int lb_insert_item_001(HWND h_lb, int pos, CTSTR &str)
{
	return (int)send_message(h_lb, LB_INSERTSTRING, pos, (LPARAM)str.c_str());
}

// ****************************************************************
// リストビュー：可視範囲に入るようにスクロール
// ================================================================
void lv_set_item_visible_001(HWND h_lv, int item_no)
{
	send_message(h_lv, LVM_ENSUREVISIBLE, item_no, TRUE);
}

// ****************************************************************
// リストビュー：アイテム数の取得
// ================================================================
int lv_get_item_cnt(HWND h_lv)
{
	return send_message(h_lv, LVM_GETITEMCOUNT, 0, 0);
}

// ****************************************************************
// リストビュー：テキストの取得
// ================================================================
TSTR lv_get_cell_text_001(HWND h_lv, int item_no, int column_no)
{
	TCHAR t_str[MAX_PATH];

	LV_ITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.iSubItem   = column_no;
	item.pszText    = t_str;
	item.cchTextMax = MAX_PATH;
	send_message(h_lv, LVM_GETITEMTEXT, item_no, (LPARAM)&item);

	return t_str;
}

// ****************************************************************
// リストビュー：テキストを設定
// ================================================================
void lv_set_cell_text_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	if (lv_get_cell_text_001(h_lv, item_no, column_no) == str)
		return;

	LV_ITEM	item;
	item.mask     = LVIF_TEXT;
	item.iSubItem = column_no;
	item.pszText  = (LPTSTR)str.c_str();
	send_message(h_lv, LVM_SETITEMTEXT, item_no, (LPARAM)&item);
}

// ****************************************************************
// リストビュー：テキストを設定して可視範囲にスクロール
// ================================================================
void lv_set_cell_text_visible_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	lv_set_cell_text_001(h_lv, item_no, column_no, str);
	lv_set_item_visible_001(h_lv, item_no);
}

// ****************************************************************
// リストビュー：アイテムの挿入
// ================================================================
int	lv_insert_item_001(HWND h_lv, int item_no)
{
	LV_ITEM item;
	item.mask     = 0;				// 項目を指定しない
	item.iItem    = item_no;		// 挿入位置
	item.iSubItem = 0;				// サブアイテム番号

	return (int)send_message(h_lv, LVM_INSERTITEM, 0, (LPARAM)&item);
}

int	lv_insert_item_002(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	item_no = lv_insert_item_001(h_lv, item_no);
	lv_set_cell_text_001(h_lv, item_no, column_no, str);

	return item_no;
}

int	lv_insert_item_visible_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	item_no = lv_insert_item_001(h_lv, item_no);
	lv_set_cell_text_visible_001(h_lv, item_no, column_no, str);

	return item_no;
}

// ****************************************************************
// リストビュー：アイテムの追加
// ================================================================
int	lv_add_item_001(HWND h_lv)
{
	return lv_insert_item_001(h_lv, lv_get_item_cnt(h_lv));
}

int	lv_add_item_002(HWND h_lv, int column_no, CTSTR& str)
{
	int item_no = lv_add_item_001(h_lv);
	lv_set_cell_text_001(h_lv, item_no, column_no, str);

	return item_no;
}

int	lv_add_item_visible_001(HWND h_lv, int column_no, CTSTR& str)
{
	int item_no = lv_add_item_001(h_lv);
	lv_set_cell_text_visible_001(h_lv, item_no, column_no, str);

	return item_no;
}

// ****************************************************************
// リストビュー：アイテム数の縮小
// ================================================================
void lv_reduce_item_001(HWND h_lv, int item_cnt)
{
	int item_no = item_cnt;
	while (send_message(h_lv, LVM_DELETEITEM, item_no, 0));
}

// ****************************************************************
// リストビュー：アイテム数の設定
// ================================================================
void lv_set_item_cnt_001(HWND h_lv, int item_cnt)
{
	int item_cnt_old = lv_get_item_cnt(h_lv);

	// アイテム数が少ないとき
	if (item_cnt_old < item_cnt)	{
		for (int i = 0; i < item_cnt - item_cnt_old; i++)	{
			lv_add_item_001(h_lv);
		}
	}
	// アイテム数が多いとき
	else if (item_cnt_old > item_cnt)	{
		lv_reduce_item_001(h_lv, item_cnt);
	}
}
