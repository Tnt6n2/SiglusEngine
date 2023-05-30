#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"engine/ifc_stack.h"
#include	"dialog/db_wnd_info_stack.h"

// ****************************************************************
// デバッグ情報ウィンドウ：スタック
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_stack")

// 初期化
void C_db_wnd_info_stack::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("スタック情報"));

	// リストビュー項目
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("form"), _T("型"), 50);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_stack::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_stack::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_stack::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在のデータを作る
	cd.item_list.resize(Gp_stack->int_now_cnt);
	for (int i = 0; i < (int)Gp_stack->int_now_cnt; i++)	{
		cd.item_list[i].id = i;
		cd.item_list[i].form = FM_INT;
		cd.item_list[i].Int = *(Gp_stack->int_p + i);
	}

	// リストビューのアイテム数を調整する
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		int col_no = -1;
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// ID
		col_no ++;
		if (change || cd.item_list[i].id != m_ld.item_list[i].id)
			lv.set_cell_text(i, col_no, tostr(cd.item_list[i].id));

		// 型
		col_no ++;
		if (change || cd.item_list[i].form != m_ld.item_list[i].form)	{
			switch (cd.item_list[i].form)	{
				case FM_VOID:	lv.set_cell_text(i, col_no, _T("FM_VOID"));	break;
				case FM_INT:	lv.set_cell_text(i, col_no, _T("FM_INT"));	break;
				case FM_STR:	lv.set_cell_text(i, col_no, _T("FM_STR"));	break;
				default:		lv.set_cell_text(i, col_no, _T("???"));		break;
			}
		}

		// 値
		col_no ++;
		if (change || cd.item_list[i].Int != m_ld.item_list[i].Int)
			lv.set_cell_text(i, col_no, tostr(cd.item_list[i].Int));
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

