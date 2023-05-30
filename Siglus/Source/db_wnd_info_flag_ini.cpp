#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_element_code.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_flag_ini.h"
#include	"element/elm_flag.h"
#include	"engine/eng_flag.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"dialog/db_wnd_info_flag_ini.h"

// ****************************************************************
// デバッグ情報ウィンドウ：flag.ini
// ================================================================

// 初期化
void C_db_wnd_info_flag_ini::init()
{
	// リストビュー項目
	listview_add_column(_T("Flag"), _T("ﾌﾗｸﾞ"), 50);
	listview_add_column(_T("value"), _T("値"), 50);
	listview_add_column(_T("comment"), _T("ｺﾒﾝﾄ"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_flag_ini::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_flag_ini::on_close_func()
{
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_flag_ini::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// 通知
bool C_db_wnd_info_flag_ini::on_notify(WPARAM wp, LPARAM lp)
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
void C_db_wnd_info_flag_ini::frame()
{
	if (!m_h_wnd)
		return;

	// Flag.ini のデータを表示する
	for (int i = 0; i < (int)Gp_flag_ini->page[m_page_no].flag.size(); i++)	{
		if (lv.get_item_cnt() <= i)	{
			lv.add_item();
		}
		
		// Flag.ini のフラグを取得
		S_flag_ini_flag_data* p_flag = &Gp_flag_ini->page[m_page_no].flag[i];
		if (!p_flag->name.empty())	{

			int user_prop_id = Gp_lexer->get_user_prop_no(p_flag->name);
			if (user_prop_id >= 0)	{

				C_tnm_prop* p_prop = tnm_flag_get_user_prop(user_prop_id);
				if (p_prop != NULL)	{

					if (false);
					else if (p_prop->form == FM_INT)	{
						lv.set_cell_text(i, 0, p_flag->name);
						lv.set_cell_text(i, 1, tostr(p_prop->Int));
					}
					else if (p_prop->form == FM_STR)	{
						lv.set_cell_text(i, 0, p_flag->name);
						lv.set_cell_text(i, 1, tostr(p_prop->str));
					}
					else if (p_prop->form == FM_INTLIST)	{
						int index = p_flag->index;

						switch (p_flag->bit_cnt)	{
							case 32:	lv.set_cell_text(i, 0, str_format(_T("%s[%04d]"), p_flag->name.c_str(), p_flag->index));		break;
							case 16:	lv.set_cell_text(i, 0, str_format(_T("%s.bit16[%04d]"), p_flag->name.c_str(), p_flag->index));	break;
							case 8:		lv.set_cell_text(i, 0, str_format(_T("%s.bit8[%04d]"), p_flag->name.c_str(), p_flag->index));	break;
							case 4:		lv.set_cell_text(i, 0, str_format(_T("%s.bit4[%04d]"), p_flag->name.c_str(), p_flag->index));	break;
							case 2:		lv.set_cell_text(i, 0, str_format(_T("%s.bit2[%04d]"), p_flag->name.c_str(), p_flag->index));	break;
							case 1:		lv.set_cell_text(i, 0, str_format(_T("%s.bit1[%04d]"), p_flag->name.c_str(), p_flag->index));	break;
						}
					
						C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
						int flag_cnt = p_int_list->get_size() * 32 / p_flag->bit_cnt;
						if (0 <= index && index < flag_cnt)	{
							switch (p_flag->bit_cnt)	{
								case 32:	lv.set_cell_text(i, 1, tostr((*p_int_list)[index]));			break;
								case 16:	lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_16(index)));	break;
								case 8:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_8(index)));	break;
								case 4:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_4(index)));	break;
								case 2:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_2(index)));	break;
								case 1:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_1(index)));	break;
							}
						}
						else	{
							lv.set_cell_text(i, 1, _T("範囲外"));
						}
					}
					else if (p_prop->form == FM_STRLIST)	{
						lv.set_cell_text(i, 0, str_format(_T("%s[%04d]"), p_flag->name.c_str(), p_flag->index));
						C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
						if (0 <= p_flag->index && p_flag->index < p_str_list->get_size())	{
							lv.set_cell_text(i, 1, (*p_str_list)[p_flag->index]);
						}
						else	{
							lv.set_cell_text(i, 1, _T(""));
						}
					}
					else	{
						lv.set_cell_text(i, 0, p_flag->name);
						lv.set_cell_text(i, 1, _T("???"));
					}
				}
				else	{
					lv.set_cell_text(i, 0, p_flag->name);
					lv.set_cell_text(i, 1, _T("???"));
				}
			}
			else	{
				lv.set_cell_text(i, 0, p_flag->name);
				lv.set_cell_text(i, 1, _T("???"));
			}
		}
		else if (p_flag->element.code_cnt >= 3)	{

			// エレメントを取得
			C_tnm_element* p_element = tnm_get_element_ptr(p_flag->element.begin(), p_flag->element.end() - 2, false);
			if (p_element)	{

				if (false);
				else if (p_element->get_element_form() == FM_INTLIST)	{

					// intlist を取得
					C_elm_int_list* p_int_list = (C_elm_int_list *)p_element;
					int index = p_flag->element.code[p_flag->element.code_cnt - 1];

					switch (p_flag->bit_cnt)	{
						case 32:	lv.set_cell_text(i, 0, str_format(_T("%s[%04d]"), p_int_list->get_element_name().c_str(), index));			break;
						case 16:	lv.set_cell_text(i, 0, str_format(_T("%s.bit16[%04d]"), p_int_list->get_element_name().c_str(), index));	break;
						case 8:		lv.set_cell_text(i, 0, str_format(_T("%s.bit8[%04d]"), p_int_list->get_element_name().c_str(), index));		break;
						case 4:		lv.set_cell_text(i, 0, str_format(_T("%s.bit4[%04d]"), p_int_list->get_element_name().c_str(), index));		break;
						case 2:		lv.set_cell_text(i, 0, str_format(_T("%s.bit2[%04d]"), p_int_list->get_element_name().c_str(), index));		break;
						case 1:		lv.set_cell_text(i, 0, str_format(_T("%s.bit[%04d]"), p_int_list->get_element_name().c_str(), index));		break;
					}

					int flag_cnt = p_int_list->get_size() * 32 / p_flag->bit_cnt;
					if (0 <= index && index < flag_cnt)	{
						switch (p_flag->bit_cnt)	{
							case 32:	lv.set_cell_text(i, 1, tostr((*p_int_list)[index]));			break;
							case 16:	lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_16(index)));	break;
							case 8:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_8(index)));	break;
							case 4:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_4(index)));	break;
							case 2:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_2(index)));	break;
							case 1:		lv.set_cell_text(i, 1, tostr(p_int_list->get_bit_1(index)));	break;
						}
					}
					else	{
						lv.set_cell_text(i, 1, _T("範囲外"));
					}
				}
				else if (p_element->get_element_form() == FM_STRLIST)	{

					// strlist を取得
					C_elm_str_list* p_str_list = (C_elm_str_list *)p_element;
					int index = p_flag->element.code[p_flag->element.code_cnt - 1];
					lv.set_cell_text(i, 0, str_format(_T("%s[%04d]"), p_str_list->get_element_name().c_str(), index));
					if (0 <= index && index < p_str_list->get_size())	{
						lv.set_cell_text(i, 1, (*p_str_list)[index]);
					}
				}
			}
		}

		lv.set_cell_text(i, 2, p_flag->comment);
	}
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_flag_ini::on_dblclk_lv()
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

		// Flag.ini のフラグを取得
		S_flag_ini_flag_data* p_flag = &Gp_flag_ini->page[m_page_no].flag[item_no];
		if (!p_flag->name.empty())	{

			int user_prop_id = Gp_lexer->get_user_prop_no(p_flag->name);
			if (user_prop_id >= 0)	{

				C_tnm_prop* p_prop = tnm_flag_get_user_prop(user_prop_id);
				if (p_prop != NULL)	{

					if (false);
					else if (p_prop->form == FM_INT || p_prop->form == FM_INTLIST)	{
						int value = str_to_int(lv.get_cell_text(item_no, column_no), 0);
						lv.edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);
					}
					else if (p_prop->form == FM_STR || p_prop->form == FM_STRLIST)	{
						TSTR value = lv.get_cell_text(item_no, column_no);
						lv.edit_item_text_edit_with_str(item_no, column_no, value);
					}
				}
			}
		}
		else if (p_flag->element.code_cnt >= 3)	{

			// エレメントを取得
			C_tnm_element* p_element = tnm_get_element_ptr(p_flag->element.begin(), p_flag->element.end() - 2, false);
			if (p_element)	{

				if (false);
				else if (p_element->get_element_form() == FM_INTLIST)	{
					int value = str_to_int(lv.get_cell_text(item_no, column_no), 0);
					lv.edit_item_text_edit_for_num(item_no, column_no, INT_MIN, INT_MAX, value);
				}
				else if (p_element->get_element_form() == FM_STRLIST)	{
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
void C_db_wnd_info_flag_ini::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	// Flag.ini のフラグを取得
	if (0 <= item_no && item_no < (int)Gp_flag_ini->page[m_page_no].flag.size())	{
		S_flag_ini_flag_data* p_flag = &Gp_flag_ini->page[m_page_no].flag[item_no];

		if (!p_flag->name.empty())	{

			int user_prop_id = Gp_lexer->get_user_prop_no(p_flag->name);
			if (user_prop_id >= 0)	{

				C_tnm_prop* p_prop = tnm_flag_get_user_prop(user_prop_id);
				if (p_prop != NULL)	{

					if (p_prop->form == FM_INT)	{
						p_prop->Int = str_to_int(value, 0);
					}
					else if (p_prop->form == FM_STR)	{
						p_prop->str = value;
					}
					else if (p_prop->form == FM_INTLIST)	{
						C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get();
						if (p_int_list)	{
							int index = p_flag->index;
							int flag_cnt = p_int_list->get_size() * 32 / p_flag->bit_cnt;
							if (0 <= index && index < flag_cnt)	{
								switch (p_flag->bit_cnt)	{
									case 32:	(*p_int_list)[index] = str_to_int(value, 0);			break;
									case 16:	p_int_list->set_bit_16(index, str_to_int(value, 0));	break;
									case 8:		p_int_list->set_bit_8(index, str_to_int(value, 0));		break;
									case 4:		p_int_list->set_bit_4(index, str_to_int(value, 0));		break;
									case 2:		p_int_list->set_bit_2(index, str_to_int(value, 0));		break;
									case 1:		p_int_list->set_bit_1(index, str_to_int(value, 0));		break;
								}
							}
						}
					}
					else if (p_prop->form == FM_STRLIST)	{
						C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get();
						if (p_str_list && 0 <= p_flag->index && p_flag->index < p_str_list->get_size())	{
							(*p_str_list)[p_flag->index] = value;
						}
					}
				}
			}
		}
		else if (p_flag->element.code_cnt >= 3)	{

			// エレメントを取得
			C_tnm_element* p_element = tnm_get_element_ptr(p_flag->element.begin(), p_flag->element.end() - 2, false);
			if (p_element)	{

				if (false);
				else if (p_element->get_element_form() == FM_INTLIST)	{

					// intlist を取得
					C_elm_int_list* p_int_list = (C_elm_int_list *)p_element;
					if (p_int_list)	{
						int index = p_flag->element.code[p_flag->element.code_cnt - 1];
						int flag_cnt = p_int_list->get_size() * 32 / p_flag->bit_cnt;
						if (0 <= index && index < flag_cnt)	{
							switch (p_flag->bit_cnt)	{
								case 32:	(*p_int_list)[index] = str_to_int(value, 0);			break;
								case 16:	p_int_list->set_bit_16(index, str_to_int(value, 0));	break;
								case 8:		p_int_list->set_bit_8(index, str_to_int(value, 0));		break;
								case 4:		p_int_list->set_bit_4(index, str_to_int(value, 0));		break;
								case 2:		p_int_list->set_bit_2(index, str_to_int(value, 0));		break;
								case 1:		p_int_list->set_bit_1(index, str_to_int(value, 0));		break;
							}
						}
					}
				}
				else if (p_element->get_element_form() == FM_STRLIST)	{

					// strlist を取得
					C_elm_str_list* p_str_list = (C_elm_str_list *)p_element;
					if (p_str_list)	{
						int index = p_flag->element.code[p_flag->element.code_cnt - 1];
						(*p_str_list)[index] = value;
					}
				}
			}
		}
	}
}

