#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_flag.h"
#include	"element/elm_call.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_flag.h"
#include	"dialog/db_wnd_info_user_call_flag.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ユーザコールフラグ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_user_call_flag")

// 初期化
void C_db_wnd_info_user_call_flag::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ユーザコールフラグ情報"));

	// リストビュー項目
	listview_add_column(_T("form"), _T("型"), 100);
	listview_add_column(_T("name"), _T("名前"), 100);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_user_call_flag::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_user_call_flag::on_close_func()
{
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_user_call_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// 通知
bool C_db_wnd_info_user_call_flag::on_notify(WPARAM wp, LPARAM lp)
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
void C_db_wnd_info_user_call_flag::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在のデータを作る
	int item_no = -1;
	int user_prop_cnt = tnm_get_cur_call()->user_prop_list.get_size();
	for (int i = 0; i < user_prop_cnt; i++)	{
		C_elm_user_call_prop* p_prop = &tnm_get_cur_call()->user_prop_list[i];
		item_no ++;

		cd.item_list.resize(item_no + 1);
		cd.item_list[item_no].prop_id = i;			// プロパティＩＤ
		cd.item_list[item_no].form = p_prop->form;	// 型
		cd.item_list[item_no].size = 0;				// サイズ
		cd.item_list[item_no].name = Gp_lexer->get_user_call_prop_name(p_prop->scn_no, p_prop->prop_id);	// 名前

		switch (cd.item_list[item_no].form)	{
			case FM_INT:
				cd.item_list[item_no].value = tostr(p_prop->Int);
				break;
			case FM_STR:
				cd.item_list[item_no].value = p_prop->str;
				break;
			case FM_INTLIST:
			{
				C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_int_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].prop_id = 0;	// プロパティＩＤ
					cd.item_list[item_no].value = tostr(p_int_list->get_value(j));
					cd.item_list[item_no].name = p_int_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// 名前
				}
				break;
			}
			case FM_STRLIST:
			{
				C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
				for (int j = 0; j < (int)p_str_list->get_size(); j++)	{
					item_no ++;
					cd.item_list.resize(item_no + 1);
					cd.item_list[item_no].prop_id = 0;	// プロパティＩＤ
					cd.item_list[item_no].value = p_str_list->get_value(j);
					cd.item_list[item_no].name = p_str_list->get_element_name() + _T("[") + tostr(j) + _T("]");	// 名前
				}
				break;
			}
			default:
			{
				S_element element = p_prop->element;
				cd.item_list[item_no].value = tostr(ARRAY<int>(element.begin(), element.end()));
			}
		}
	}

	// リストビューのアイテム数を設定
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// 前回と異なるものを再表示
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// 型
		if (change || cd.item_list[i].form != m_ld.item_list[i].form)	{
			switch (cd.item_list[i].form)	{
				case FM_INT:		lv.set_cell_text(i, 0, _T("int"));		break;
				case FM_STR:		lv.set_cell_text(i, 0, _T("str"));		break;
				case FM_INTLIST:	lv.set_cell_text(i, 0, _T("intlist"));	break;
				case FM_STRLIST:	lv.set_cell_text(i, 0, _T("strlist"));	break;
			}
		}

		// 名前
		if (change || cd.item_list[i].name != m_ld.item_list[i].name)
			lv.set_cell_text(i, 1, cd.item_list[i].name);

		// 値
		if (change || cd.item_list[i].value != m_ld.item_list[i].value)
			lv.set_cell_text(i, 2, cd.item_list[i].value);
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_user_call_flag::on_dblclk_lv()
{
	// 選択状態取得
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// マウスカーソルの乗っているセル番号
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// アイテムを変更する
	if (select_cnt == 1 && select_no == item_no && column_no == 2)	{
		int user_prop_cnt = tnm_get_cur_call()->user_prop_list.get_size();
		if (0 <= item_no && item_no < user_prop_cnt)	{
			int user_prop_id = item_no;
			C_elm_user_call_prop* p_prop = &tnm_get_cur_call()->user_prop_list[user_prop_id];

			// 型によって分岐
			if (p_prop)	{
				if (p_prop->form == FM_INT)	{
					int value = str_to_int(lv.get_cell_text(item_no, column_no), 0);
					lv.edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);
				}
				else if (p_prop->form == FM_STR)	{
					TSTR value = lv.get_cell_text(item_no, column_no);
					lv.edit_item_text_edit_with_str(item_no, column_no, value);
				}
			}
		}
	}
}

// ****************************************************************
// リストビューの値を変更した
// ================================================================
void C_db_wnd_info_user_call_flag::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	if (column_no == 2)	{
		int user_prop_cnt = tnm_get_cur_call()->user_prop_list.get_size();
		if (0 <= item_no && item_no < user_prop_cnt)	{
			int user_prop_id = item_no;
			C_elm_user_call_prop* p_prop = &tnm_get_cur_call()->user_prop_list[user_prop_id];

			// 型によって分岐
			if (p_prop)	{
				if (p_prop->form == FM_INT)	{
					p_prop->Int = str_to_int(value, 0);
				}
				else if (p_prop->form == FM_STR)	{
					p_prop->str = value;
				}
			}
		}
	}
}
