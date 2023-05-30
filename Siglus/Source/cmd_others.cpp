#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_cg_table.h"
#include	"element/elm_database.h"
#include	"element/elm_counter.h"
#include	"element/elm_int_event.h"
#include	"element/elm_frame_action.h"
#include	"element/elm_g00_buf.h"
#include	"element/elm_mask.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_bgm_table.h"
#include	"engine/eng_g00_buf.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// コマンド処理：整数リスト
// ================================================================
bool tnm_command_proc_int_list(C_elm_int_list* p_int_list, int bit, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_int_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];

		// ビット変数の場合の処理（bit == 32 で通常の変数）
		switch (bit)	{
			case 1:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_bit_1(index));		// GET
				else if (al_id == 1)		p_int_list->set_bit_1(index, p_ai->al_begin[0].Int);	// SET
				break;
			case 2:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_bit_2(index));		// GET
				else if (al_id == 1)		p_int_list->set_bit_2(index, p_ai->al_begin[0].Int);	// SET
				break;
			case 4:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_bit_4(index));		// GET
				else if (al_id == 1)		p_int_list->set_bit_4(index, p_ai->al_begin[0].Int);	// SET
				break;
			case 8:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_bit_8(index));		// GET
				else if (al_id == 1)		p_int_list->set_bit_8(index, p_ai->al_begin[0].Int);	// SET
				break;
			case 16:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_bit_16(index));		// GET
				else if (al_id == 1)		p_int_list->set_bit_16(index, p_ai->al_begin[0].Int);	// SET
				break;
			case 32:
				if (al_id == 0)				tnm_stack_push_int(p_int_list->get_value(index));		// GET
				else if (al_id == 1)		p_int_list->set_value(index, p_ai->al_begin[0].Int);	// SET
				break;
		}
	}
	else if (elm_begin[0] == ELM_INTLIST_BIT)	{
		tnm_command_proc_int_list(p_int_list, 1, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_INTLIST_BIT2)	{
		tnm_command_proc_int_list(p_int_list, 2, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_INTLIST_BIT4)	{
		tnm_command_proc_int_list(p_int_list, 4, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_INTLIST_BIT8)	{
		tnm_command_proc_int_list(p_int_list, 8, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_INTLIST_BIT16)	{
		tnm_command_proc_int_list(p_int_list, 16, elm_top, elm_begin + 1, elm_end, p_ai);
	}

	// 操作
	else if (elm_begin[0] == ELM_INTLIST_INIT)	{
		p_int_list->reinit();
	}
	else if (elm_begin[0] == ELM_INTLIST_RESIZE)	{
		p_int_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_INTLIST_GET_SIZE)	{
		int size = 0;
		switch (bit)	{
			case 1:
				size = p_int_list->get_size() * 32;
				break;
			case 2:
				size = p_int_list->get_size() * 16;
				break;
			case 4:
				size = p_int_list->get_size() * 8;
				break;
			case 8:
				size = p_int_list->get_size() * 4;
				break;
			case 16:
				size = p_int_list->get_size() * 2;
				break;
			case 32:
				size = p_int_list->get_size();
				break;
		}
		tnm_stack_push_int(size);
	}
	else if (elm_begin[0] == ELM_INTLIST_CLEAR)	{
		int value = (al_id == 0) ? 0 : p_ai->al_begin[2].Int;
		for (int index = p_ai->al_begin[0].Int; index <= p_ai->al_begin[1].Int; index ++)	{
			switch (bit)	{
				case 1:
					p_int_list->set_bit_1(index, value);
					break;
				case 2:
					p_int_list->set_bit_2(index, value);
					break;
				case 4:
					p_int_list->set_bit_4(index, value);
					break;
				case 8:
					p_int_list->set_bit_8(index, value);
					break;
				case 16:
					p_int_list->set_bit_16(index, value);
					break;
				case 32:
					p_int_list->set_value(index, value);
					break;
			}
		}
	}
	else if (elm_begin[0] == ELM_INTLIST_SETS)	{
		int index = p_ai->al_begin[0].Int;
		for (C_tnm_prop* arg = p_ai->al_begin + 1; arg < p_ai->al_end; arg ++, index ++)	{
			switch (bit)	{
				case 1:
					p_int_list->set_bit_1(index, arg->Int);
					break;
				case 2:
					p_int_list->set_bit_2(index, arg->Int);
					break;
				case 4:
					p_int_list->set_bit_4(index, arg->Int);
					break;
				case 8:
					p_int_list->set_bit_8(index, arg->Int);
					break;
				case 16:
					p_int_list->set_bit_16(index, arg->Int);
					break;
				case 32:
					p_int_list->set_value(index, arg->Int);
					break;
			}
		}
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_int_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：整数リスト（整数イベントリストを受け取るバージョン）
// ================================================================
bool tnm_command_proc_int_list(C_elm_int_event_list* p_int_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_int_event_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_int_event* p_int_event = p_int_event_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_int_event)	{
			switch (al_id)	{
				case 0:		tnm_stack_push_int(p_int_event->get_value());	break;	// GET
				case 1:		p_int_event->set_value(p_ai->al_begin[0].Int);	break;	// SET
			}
		}
		else	{
			// ▲失敗したので戻り値に応じた値をスタックに入れておこう
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_INTLIST_RESIZE)	{
		p_int_event_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_INTLIST_CLEAR)	{
		int value = (al_id == 0) ? 0 : p_ai->al_begin[2].Int;
		for (int index = p_ai->al_begin[0].Int; index <= p_ai->al_begin[1].Int; index ++)	{
			C_elm_int_event* p_int_event = p_int_event_list->get_sub(index, p_ai->disp_out_of_range_error);
			if (p_int_event)
				p_int_event->set_value(value);
		}
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_int_event_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：整数イベント
// ================================================================
bool tnm_command_proc_int_event(C_elm_int_event* p_int_event, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = (C_tnm_element *)p_int_event;		// int_event はエレメントではありません。高速化のためです。この処理は無理やりです。
	}
	//else if (elm_begin[0] == ELM___SET)	{	// 代入

	//	if (al_id == 0)	{
	//		info_box(_T("代入するよ！") + tostr(p_ai->al_begin[0].Int));
	//	}
	//}
	//else if (elm_begin[0] == ELM___TRANS)	{	// 変換

	//	if (al_id == 0)	{
	//		info_box(_T("変換するよ！") + tostr(p_ai->al_begin[0].Int));
	//		tnm_stack_push_int(12345);
	//	}
	//}
	else if (elm_begin[0] == ELM_INTEVENT_SET)	{

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:
					p_int_event->set_value(arg->Int);	// 初期値を設定する
					break;
			}
		}

		// イベント開始
		p_int_event->set_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, 0);
	}
	else if (elm_begin[0] == ELM_INTEVENT_SET_REAL)	{

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:
					p_int_event->set_value(arg->Int);	// 初期値を設定する
					break;
			}
		}

		// イベント開始
		p_int_event->set_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, 1);
	}
	else if (elm_begin[0] == ELM_INTEVENT_LOOP)	{
		p_int_event->loop_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int, 0);
	}
	else if (elm_begin[0] == ELM_INTEVENT_LOOP_REAL)	{
		p_int_event->loop_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int, 1);
	}
	else if (elm_begin[0] == ELM_INTEVENT_TURN)	{
		p_int_event->turn_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int, 0);
	}
	else if (elm_begin[0] == ELM_INTEVENT_TURN_REAL)	{
		p_int_event->turn_event(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int, 1);
	}
	else if (elm_begin[0] == ELM_INTEVENT_END)	{
		p_int_event->end_event();
	}
	else if (elm_begin[0] == ELM_INTEVENT_WAIT)	{

		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_EVENT_WAIT;
		proc.element = S_element(elm_top, elm_end - 1);
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_INTEVENT_WAIT_KEY)	{

		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_EVENT_WAIT;
		proc.element = S_element(elm_top, elm_end - 1);
		proc.key_skip_enable_flag = true;
		proc.return_value_flag = true;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_INTEVENT_CHECK)	{
		tnm_stack_push_int(p_int_event->check_event() ? 1 : 0);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(intevent)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：整数イベントリスト
// ================================================================
bool tnm_command_proc_int_event_list(C_elm_int_event_list* p_int_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_int_event_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_int_event* p_event = p_int_event_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_event)	{
			tnm_command_proc_int_event(p_event, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// ▲失敗したので戻り値に応じた値をスタックに入れておこう
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_INTEVENTLIST_RESIZE)	{
		p_int_event_list->resize(p_ai->al_begin[0].Int);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_int_event_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：文字列リスト
// ================================================================
bool tnm_command_proc_str_list(C_elm_str_list* p_str_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_str_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		TSTR* p_str = p_str_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_str)	{
			tnm_command_proc_str(*p_str, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// ▲失敗したので戻り値に応じた値をスタックに入れておこう
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_STRLIST_INIT)	{
		p_str_list->reinit();
	}
	else if (elm_begin[0] == ELM_STRLIST_RESIZE)	{
		p_str_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_STRLIST_GET_SIZE)	{
		tnm_stack_push_int(p_str_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_str_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：文字列
// ================================================================
bool tnm_command_proc_str(TSTR& str, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		if (al_id == 0)			tnm_stack_push_str(str);				// GET
		else if (al_id == 1)	str = p_ai->al_begin[0].str;			// SET
	}
	else if (elm_begin[0] == ELM_STR_UPPER)	{
		tnm_stack_push_str(str_to_upper_ret(str));
	}
	else if (elm_begin[0] == ELM_STR_LOWER)	{
		tnm_stack_push_str(str_to_lower_ret(str));
	}
	else if (elm_begin[0] == ELM_STR_CNT)	{
		tnm_stack_push_int((int)str.size());
	}
	else if (elm_begin[0] == ELM_STR_LEN)	{
		int len = 0;
		for (TSTR::iterator itr = str.begin(); itr != str.end(); ++itr)	{
			len += is_hankaku(*itr) ? 1 : 2;
		}

		tnm_stack_push_int(len);
	}
	else if (elm_begin[0] == ELM_STR_LEFT)	{
		int len = p_ai->al_begin[0].Int;
		len = limit(0, len, (int)str.size());		// 長さを補正

		tnm_stack_push_str(str.substr(0, len));		// 文字列を決定
	}
	else if (elm_begin[0] == ELM_STR_LEFT_LEN)	{
		int len = 0;
		TSTR result;
		for (TSTR::iterator itr = str.begin(); itr != str.end(); ++itr)	{
			len += is_hankaku(*itr) ? 1 : 2;
			if (len > p_ai->al_begin[0].Int)
				break;
			result += *itr;
		}

		tnm_stack_push_str(result);
	}
	else if (elm_begin[0] == ELM_STR_RIGHT)	{
		int pos = (int)str.size() - p_ai->al_begin[0].Int;
		pos = limit(0, pos, (int)str.size());			// 開始位置を補正
		tnm_stack_push_str(str.substr(pos));		// 文字列を決定
	}
	else if (elm_begin[0] == ELM_STR_RIGHT_LEN)	{
		int len = 0;
		TSTR result;
		for (TSTR::reverse_iterator itr = str.rbegin(); itr != str.rend(); ++itr)	{
			len += is_hankaku(*itr) ? 1 : 2;
			if (len > p_ai->al_begin[0].Int)
				break;
			result = *itr + result;
		}

		tnm_stack_push_str(result);
	}
	else if (elm_begin[0] == ELM_STR_MID)	{

		// 最後まで
		if (al_id == 0)	{
			int s = p_ai->al_begin[0].Int;
			s = limit(0, s, (int)str.size());				// 開始位置を補正
			tnm_stack_push_str(str.substr(s));			// 文字列を決定
		}
		// 途中まで
		else	{
			int s = p_ai->al_begin[0].Int;
			int e = p_ai->al_begin[0].Int + p_ai->al_begin[1].Int;
			s = limit(0, s, (int)str.size());				// 開始位置を補正
			e = limit(s, e, (int)str.size());				// 終了位置を補正
			tnm_stack_push_str(str.substr(s, e - s));	// 文字列を決定
		}
	}
	else if (elm_begin[0] == ELM_STR_MID_LEN)	{
		int len = 0;
		TSTR result;
		for (TSTR::iterator itr = str.begin(); itr != str.end(); ++itr)	{
			if (len >= p_ai->al_begin[0].Int)	{
				len += is_hankaku(*itr) ? 1 : 2;
				if (al_id == 1 && len > p_ai->al_begin[0].Int + p_ai->al_begin[1].Int)
					break;
				result += *itr;
			}
			else	{
				len += is_hankaku(*itr) ? 1 : 2;
			}
		}

		tnm_stack_push_str(result);
	}
	else if (elm_begin[0] == ELM_STR_SEARCH)	{

		// 小文字に変換！！
		TSTR str_1 = str_to_lower_ret(str);
		TSTR str_2 = str_to_lower_ret(p_ai->al_begin[0].str);

		// 検索
		int pos = (int)str_1.find(str_2);
		tnm_stack_push_int(pos == TSTR::npos ? -1 : pos);
	}
	else if (elm_begin[0] == ELM_STR_SEARCH_LAST)	{

		// 小文字に変換！！
		TSTR str_1 = str_to_lower_ret(str);
		TSTR str_2 = str_to_lower_ret(p_ai->al_begin[0].str);

		// 逆から検索
		int pos = (int)str_1.rfind(str_2);
		tnm_stack_push_int(pos == TSTR::npos ? -1 : pos);
	}
	else if (elm_begin[0] == ELM_STR_GET_CODE)	{
		int pos = p_ai->al_begin[0].Int;
		tnm_stack_push_int(pos < (int)str.size() ? str[pos] : -1);
	}
	else if (elm_begin[0] == ELM_STR_TONUM)	{
		tnm_stack_push_int(str_to_int(str, 0));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(str)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：数学
// ================================================================
TSTR tnm_tostr(int num, int len, TCHAR fill_moji)
{
	TSTR str;

	if (num == 0)	{
		str = (TSTR() + fill_moji) * (len - 1) + _T("0");
	}
	else if (num > 0)	{
		int keta = get_keta(num);
		str = (TSTR() + fill_moji) * (len - keta) + tostr(num);
	}
	else if (num < 0)	{
		int keta = get_keta(num);
		str = _T("-") + (TSTR() + fill_moji) * (len - keta - 1) + tostr(- num);
	}

	return str;
}

int tnm_4sya5nyu(double value)
{
	if (value > 0)	return (int)(value + 0.5f);
	if (value < 0)	return (int)(value - 0.5f);
	return 0;
}

bool tnm_command_proc_math(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	switch (elm_begin[0])	{

		case ELM_MATH_MAX:
			tnm_stack_push_int(std::max(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
			break;

		case ELM_MATH_MIN:
			tnm_stack_push_int(std::min(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
			break;

		case ELM_MATH_LIMIT:
			if (p_ai->al_begin[0].Int < p_ai->al_begin[2].Int)
				tnm_stack_push_int(limit(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int));
			else
				tnm_stack_push_int(limit(p_ai->al_begin[2].Int, p_ai->al_begin[1].Int, p_ai->al_begin[0].Int));
			break;

		case ELM_MATH_ABS:
			tnm_stack_push_int(std::abs(p_ai->al_begin[0].Int));
			break;

		case ELM_MATH_RAND:
		{
			int min = p_ai->al_begin[0].Int;
			int max = p_ai->al_begin[1].Int;
			if (min > max)	{
				std::swap(min, max);
			}

			tnm_stack_push_int(rand() % (max - min + 1) + min);
			break;
		}

		case ELM_MATH_SQRT:
		{
			int value = std::max(p_ai->al_begin[0].Int, 0);
			int ret = (int)(std::sqrt((double)value) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_LOG:
		{
			int value = std::max(p_ai->al_begin[0].Int, 1);
			int ret = (int)(std::log((double)value) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_LOG2:
		{
			int value = std::max(p_ai->al_begin[0].Int, 1);
			int ret = (int)(std::log((double)value) / std::log(2.0f) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_LOG10:
		{
			int value = std::max(p_ai->al_begin[0].Int, 1);
			int ret = (int)(std::log10((double)value) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}

		case ELM_MATH_SIN:
		{
			int ret = (int)(std::sin(D3DXToRadian((float)p_ai->al_begin[0].Int) / TNM_ANGLE_UNIT) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_COS:
		{
			int ret = (int)(std::cos(D3DXToRadian((float)p_ai->al_begin[0].Int) / TNM_ANGLE_UNIT) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_TAN:
		{
			int ret = (int)(std::tan(D3DXToRadian((float)p_ai->al_begin[0].Int) / TNM_ANGLE_UNIT) * p_ai->al_begin[1].Int);
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_ARCSIN:
		{
			if (p_ai->al_begin[1].Int == 0)	{
				tnm_stack_push_int(0);
			}
			else	{
				double value = limit((double)-1.0f, (double)p_ai->al_begin[0].Int / p_ai->al_begin[1].Int, (double)1.0f);
				int ret = tnm_4sya5nyu(D3DXToDegree(std::asin(value)) * TNM_ANGLE_UNIT);
				tnm_stack_push_int(ret);
			}
			break;
		}
		case ELM_MATH_ARCCOS:
		{
			if (p_ai->al_begin[1].Int == 0)	{
				tnm_stack_push_int(0);
			}
			else	{
				double value = limit((double)-1.0f, (double)p_ai->al_begin[0].Int / p_ai->al_begin[1].Int, (double)1.0f);
				int ret = tnm_4sya5nyu(D3DXToDegree(std::acos(value)) * TNM_ANGLE_UNIT);
				tnm_stack_push_int(ret);
			}
			break;
		}
		case ELM_MATH_ARCTAN:
		{
			if (p_ai->al_begin[1].Int == 0)	{
				tnm_stack_push_int(0);
			}
			else	{
				double value = (double)p_ai->al_begin[0].Int / p_ai->al_begin[1].Int;
				int ret = tnm_4sya5nyu(D3DXToDegree(std::atan(value)) * TNM_ANGLE_UNIT);
				tnm_stack_push_int(ret);
			}
			break;
		}
		case ELM_MATH_DISTANCE:
		{
			int x1 = p_ai->al_begin[0].Int;
			int y1 = p_ai->al_begin[1].Int;
			int x2 = p_ai->al_begin[2].Int;
			int y2 = p_ai->al_begin[3].Int;
			int ret = (int)std::sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_ANGLE:
		{
			int x1 = p_ai->al_begin[0].Int;
			int y1 = p_ai->al_begin[1].Int;
			int x2 = p_ai->al_begin[2].Int;
			int y2 = p_ai->al_begin[3].Int;
			int ret = tnm_4sya5nyu(D3DXToDegree(std::atan2((double)(y2 - y1), (double)(x2 - x1))) * TNM_ANGLE_UNIT);
			ret = (ret + 360 * TNM_ANGLE_UNIT) % (360 * TNM_ANGLE_UNIT);	// -180～180 → 0～360
			tnm_stack_push_int(ret);
			break;
		}
		case ELM_MATH_LINEAR:
		{
			// こちらは結果を四捨五入しません。正確な計算向け。

			int x0 = p_ai->al_begin[0].Int;
			int x1 = p_ai->al_begin[1].Int;
			int y1 = p_ai->al_begin[2].Int;
			int x2 = p_ai->al_begin[3].Int;
			int y2 = p_ai->al_begin[4].Int;
			if (x1 == x2)		tnm_stack_push_int(y1);
			//else if (x0 == x1)	tnm_stack_push_int(y1);
			//else if (x0 == x2)	tnm_stack_push_int(y2);
			else				tnm_stack_push_int((int)((double)(y2 - y1) * (x0 - x1) / (x2 - x1) + y1));
			//else				tnm_stack_push_int((int)((double)(y2 - y1) / (x2 - x1) * (x0 - x1) + y1));
			break;
		}
		case ELM_MATH_TIMETABLE:
		{
			double now_time = (double)p_ai->al_begin[0].Int;	// 現在時刻
			double rep_time = (double)p_ai->al_begin[1].Int;	// 修正時刻
			double start_value = (double)p_ai->al_begin[2].Int;	// 開始値
			double ret_value = (double)start_value;

			// 時刻を補正する
			now_time -= rep_time;

			// 可変引数部分を順に調べる
			for (C_tnm_prop* arg = p_ai->al_begin + 3; arg < p_ai->al_end; ++arg)	{
				
				double start_time = (double)arg->exp_list[0].Int;	// 開始時刻
				double end_time = (double)arg->exp_list[1].Int;		// 終了時刻
				double end_value = (double)arg->exp_list[2].Int;	// 終了値
				int speed_type = (arg->exp_cnt >= 4 ? arg->exp_list[3].Int : 0);

				if (now_time < start_time)	{		// 開始時刻より前の場合（'<=' ではなく '<' を使うことで後優先にしている）
					ret_value = start_value;		// 開始値を返す（既に値が入っているのでこの代入はいらないかもしれない）
					break;	// ここで終了
				}
				else if (now_time >= end_time)	{	// 終了時刻を過ぎている場合
					ret_value = end_value;			// 終了値を返す

					// 次のテーブルを見に行くので終了しない
				}
				else	{

					// フレーム計算
					if (speed_type == 0)	{		// 線形
						ret_value = (end_value - start_value) * (now_time - start_time) / (end_time - start_time) + start_value;
					}
					else if (speed_type == 1)	{	// 加速
						ret_value = (end_value - start_value) * (now_time - start_time) * (now_time - start_time) / (end_time - start_time) / (end_time - start_time) + start_value;
					}
					else if (speed_type == 2)	{	// 減速
						ret_value = - (end_value - start_value) * (now_time - end_time) * (now_time - end_time) / (end_time - start_time) / (end_time - start_time) + end_value;
					}
					break;	// ここで終了
				}

				// 今回の終了値が次の開始値
				start_value = end_value;
			}

			// 値を四捨五入（最後にカクつくのを防ぐため）
			if (ret_value >= 0)
				tnm_stack_push_int((int)(ret_value + 0.5));
			else
				tnm_stack_push_int((int)(ret_value - 0.5));

			break;
		}
		case ELM_MATH_TOSTR:
		{
			if (al_id == 0)	{
				tnm_stack_push_str(tostr(p_ai->al_begin[0].Int));
			}
			else if (al_id == 1)	{	// 長さ指定バージョン
				TSTR str = tnm_tostr(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, _T(' '));
				tnm_stack_push_str(str);
			}
			break;
		}
		case ELM_MATH_TOSTR_ZERO:
		{
			TSTR str = tnm_tostr(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, _T('0'));
			tnm_stack_push_str(str);
			break;
		}
		case ELM_MATH_TOSTR_ZEN:
		{
			if (al_id == 0)	{
				tnm_stack_push_str(str_to_zenkaku_ret(tostr(p_ai->al_begin[0].Int)));
			}
			else if (al_id == 1)	{	// 長さ指定バージョン
				TSTR str = tnm_tostr(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, _T(' '));
				tnm_stack_push_str(str_to_zenkaku_ret(str));
			}
			break;
		}
		case ELM_MATH_TOSTR_ZEN_ZERO:
		{
			TSTR str = tnm_tostr(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, _T('0'));
			tnm_stack_push_str(str_to_zenkaku_ret(str));
			break;
		}
		case ELM_MATH_TOSTR_BY_CODE:
		{
			TSTR str = tostr_moji((TCHAR)p_ai->al_begin[0].Int);
			tnm_stack_push_str(str);
			break;
		}
		default:
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(math)"));
			break;
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：ＣＧテーブル
// ================================================================
bool tnm_command_proc_cg_table(C_tnm_cg_table* p_cg_table, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);

	else if (elm_begin[0] == ELM_CGTABLE_FLAG)	{
		tnm_command_proc_int_list(p_cg_table->get_ptr_to_flag(), 32, elm_top, elm_begin + 1, elm_end, p_ai);
	}

	else if (elm_begin[0] == ELM_CGTABLE_SET_DISABLE)	{
		Gp_local->pod.cg_table_off_flag = true;
	}
	else if (elm_begin[0] == ELM_CGTABLE_SET_ENABLE)	{
		Gp_local->pod.cg_table_off_flag = false;
	}
	else if (elm_begin[0] == ELM_CGTABLE_SET_ALL_FLAG)	{
		p_cg_table->set_flag_value_for_all(p_ai->al_begin[0].Int);
	}

	// 情報を取得
	else if (elm_begin[0] == ELM_CGTABLE_GET_CG_CNT)	{
		tnm_stack_push_int(p_cg_table->get_cg_cnt());
	}
	else if (elm_begin[0] == ELM_CGTABLE_GET_LOOK_CNT)	{
		tnm_stack_push_int(p_cg_table->get_look_cnt());
	}
	else if (elm_begin[0] == ELM_CGTABLE_GET_LOOK_PERCENT)	{
		tnm_stack_push_int(p_cg_table->get_look_percent());
	}

	// 名前からフラグ番号を取得
	else if (elm_begin[0] == ELM_CGTABLE_GET_FLAG_NO_BY_NAME)	{
		tnm_stack_push_int(p_cg_table->get_flag_no_from_name(p_ai->al_begin[0].str));
	}

	// 名前から情報を取得／設定
	else if (elm_begin[0] == ELM_CGTABLE_GET_LOOK_BY_NAME)	{
		int flag_value = 0;
		bool res = p_cg_table->get_flag_value_from_name(p_ai->al_begin[0].str, &flag_value);
		tnm_stack_push_int(res ? flag_value : -1);
	}
	else if (elm_begin[0] == ELM_CGTABLE_SET_LOOK_BY_NAME)	{
		p_cg_table->set_flag_value_from_name(p_ai->al_begin[0].str, p_ai->al_begin[1].Int);
	}

	// フラグから名前を取得
	else if (elm_begin[0] == ELM_CGTABLE_GET_NAME_BY_FLAG_NO)	{
		tnm_stack_push_str(p_cg_table->get_name_from_flag_no(p_ai->al_begin[0].Int));
	}

	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(cgtable)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：ＢＧＭテーブル
// ================================================================
bool tnm_command_proc_bgm_table(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);

	else if (elm_begin[0] == ELM_BGMTABLE_GET_BGM_CNT)	{
		tnm_stack_push_int(tnm_bgm_table_get_bgm_cnt());
	}
	else if (elm_begin[0] == ELM_BGMTABLE_SET_LISTEN_BY_NAME)	{
		tnm_bgm_table_set_listened(p_ai->al_begin[0].str, p_ai->al_begin[1].Int ? true : false);
	}
	else if (elm_begin[0] == ELM_BGMTABLE_SET_ALL_FLAG)	{
		tnm_bgm_table_set_all_flag(p_ai->al_begin[0].Int ? true : false);
	}
	else if (elm_begin[0] == ELM_BGMTABLE_GET_LISTEN_BY_NAME)	{
		tnm_stack_push_int(tnm_bgm_table_get_listened(p_ai->al_begin[0].str));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(bgmtable)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：データベースリスト
// ================================================================
bool tnm_command_proc_database_list(C_elm_database_list* p_database_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_database_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_database* p_database = p_database_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_database)
			tnm_command_proc_database(p_database, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_DATABASELIST_GET_SIZE)	{
		tnm_stack_push_int(p_database_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_database_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：データベース
// ================================================================
bool tnm_command_proc_database(C_elm_database* p_database, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_database;
	}
	else	{

		switch (elm_begin[0])	{
			case ELM_DATABASE_GET_NUM:
			{
				int num = 0;
				p_database->get_data(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, &num);
				tnm_stack_push_int(num);
				break;
			}
			case ELM_DATABASE_GET_STR:
			{
				TSTR str;
				p_database->get_data(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, str);
				tnm_stack_push_str(str);
				break;
			}
			case ELM_DATABASE_CHECK_ITEM:
			{
				int res = p_database->check_item_no(p_ai->al_begin[0].Int);
				tnm_stack_push_int(res);
				break;
			}
			case ELM_DATABASE_CHECK_COLUMN:
			{
				int res = p_database->check_column_no(p_ai->al_begin[0].Int);
				tnm_stack_push_int(res);
				break;
			}
			case ELM_DATABASE_FIND_NUM:
			{
				int res = p_database->find_num(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
				tnm_stack_push_int(res);
				break;
			}
			case ELM_DATABASE_FIND_STR:
			{
				int res = p_database->find_str(p_ai->al_begin[0].Int, p_ai->al_begin[1].str);
				tnm_stack_push_int(res);
				break;
			}
			case ELM_DATABASE_FIND_STR_REAL:
			{
				int res = p_database->find_str_real(p_ai->al_begin[0].Int, p_ai->al_begin[1].str);
				tnm_stack_push_int(res);
				break;
			}
			default:
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_database->get_element_name());
				break;
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：カウンタリスト
// ================================================================
bool tnm_command_proc_counter_list(C_elm_counter_list* p_counter_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_counter_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_counter* p_counter = p_counter_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_counter)
			tnm_command_proc_counter(p_counter, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_COUNTERLIST_GET_SIZE)	{
		tnm_stack_push_int(p_counter_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_counter_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：カウンタ
// ================================================================
bool tnm_command_proc_counter(C_elm_counter* p_counter, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_counter;
	}
	else if (elm_begin[0] == ELM_COUNTER_SET)	{
		p_counter->set_count(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_COUNTER_GET)	{
		tnm_stack_push_int(p_counter->get_count());
	}
	else if (elm_begin[0] == ELM_COUNTER_RESET)	{
		p_counter->reset();
	}
	else if (elm_begin[0] == ELM_COUNTER_START)	{
		p_counter->start();
	}
	else if (elm_begin[0] == ELM_COUNTER_START_REAL)	{
		p_counter->start_real();
	}
	else if (elm_begin[0] == ELM_COUNTER_START_FRAME)	{
		p_counter->start_frame(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_COUNTER_START_FRAME_REAL)	{
		p_counter->start_frame_real(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_COUNTER_START_FRAME_LOOP)	{
		p_counter->start_frame_loop(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_COUNTER_START_FRAME_LOOP_REAL)	{
		p_counter->start_frame_loop_real(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_COUNTER_STOP)	{
		p_counter->stop();
	}
	else if (elm_begin[0] == ELM_COUNTER_RESUME)	{
		p_counter->resume();
	}
	else if (elm_begin[0] == ELM_COUNTER_WAIT)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_COUNT_WAIT;
		proc.element = p_counter->get_element();
		proc.option = p_ai->al_begin[0].Int;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_COUNTER_WAIT_KEY)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_COUNT_WAIT;
		proc.element = p_counter->get_element();
		proc.option = p_ai->al_begin[0].Int;
		proc.key_skip_enable_flag = true;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_COUNTER_CHECK_VALUE)	{
		int time = p_ai->al_begin[0].Int;
		tnm_stack_push_int(p_counter->get_count() - time >= 0 ? 1 : 0);
	}
	else if (elm_begin[0] == ELM_COUNTER_CHECK_ACTIVE)	{
		int time = p_ai->al_begin[0].Int;
		tnm_stack_push_int(p_counter->is_active() ? 1 : 0);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_counter->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：フレームアクションリスト
// ================================================================
bool tnm_command_proc_frame_action_list(C_elm_frame_action_list* p_frame_action_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_frame_action_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_frame_action* p_frame_action = p_frame_action_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_frame_action)	{
			tnm_command_proc_frame_action(p_frame_action, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// 失敗したので戻り値に応じた値をスタックに入れておく
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_FRAMEACTIONLIST_RESIZE)	{
		p_frame_action_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_FRAMEACTIONLIST_GET_SIZE)	{
		tnm_stack_push_int(p_frame_action_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_frame_action_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：フレームアクション
// ================================================================
bool tnm_command_proc_frame_action(C_elm_frame_action* p_frame_action, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);
	else if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_frame_action;
	}
	else if (elm_begin[0] == ELM_FRAMEACTION_START)	{

		int end_time = p_ai->al_begin[0].Int;
		TSTR scn_name = Gp_lexer->get_scn_name(Gp_lexer->get_cur_scn_no());
		TSTR cmd_name = p_ai->al_begin[1].str;
		C_tnm_prop_list arg_list;
		arg_list.get_sub().assign(p_ai->al_begin + 2, p_ai->al_end);
		if (Gp_lexer->get_user_cmd_no(Gp_lexer->get_cur_scn_no(), cmd_name) < 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("コマンド \"") + cmd_name + _T("\" が見つかりません。\nフレームアクションの設定に失敗しました。"));
		}
		else	{
			p_frame_action->set_param(end_time, 0, scn_name, cmd_name, arg_list);
		}
	}
	else if (elm_begin[0] == ELM_FRAMEACTION_START_REAL)	{

		int end_time = p_ai->al_begin[0].Int;
		TSTR scn_name = Gp_lexer->get_scn_name(Gp_lexer->get_cur_scn_no());
		TSTR cmd_name = p_ai->al_begin[1].str;
		C_tnm_prop_list arg_list;
		arg_list.get_sub().assign(p_ai->al_begin + 2, p_ai->al_end);
		if (Gp_lexer->get_user_cmd_no(Gp_lexer->get_cur_scn_no(), cmd_name) < 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("コマンド \"") + cmd_name + _T("\" が見つかりません。\nフレームアクションの設定に失敗しました。"));
		}
		else	{
			p_frame_action->set_param(end_time, 1, scn_name, cmd_name, arg_list);
		}
	}
	else if (elm_begin[0] == ELM_FRAMEACTION_END)	{
		p_frame_action->reinit(true);
	}
	else if (elm_begin[0] == ELM_FRAMEACTION_COUNTER)	{
		tnm_command_proc_counter(&p_frame_action->m_counter, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_FRAMEACTION_IS_END_ACTION)	{
		tnm_stack_push_int(p_frame_action->is_end_action() ? 1 : 0);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_frame_action->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：Ｇ００バッファリスト
// ================================================================
bool tnm_command_proc_g00_buf_list(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	// グローバルなＧ００バッファリスト専用の処理です。
	C_elm_g00_buf_list* p_g00_buf_list = Gp_g00_buf_list;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_g00_buf_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_g00_buf* p_g00_buf = Gp_g00_buf_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_g00_buf)	{
			tnm_command_proc_g00_buf(p_g00_buf, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// 失敗したので戻り値に応じた値をスタックに入れておく
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_G00BUFLIST_GET_SIZE)	{
		tnm_stack_push_int(tnm_g00_buf_get_g00_buf_cnt());
	}
	else if (elm_begin[0] == ELM_G00BUFLIST_FREE_ALL)	{
		tnm_g00_buf_free_all_buf();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_g00_buf_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：Ｇ００バッファ
// ================================================================
bool tnm_command_proc_g00_buf(C_elm_g00_buf* p_g00_buf, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);
	else if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_g00_buf;
	}
	else if (elm_begin[0] == ELM_G00BUF_LOAD)	{
		tnm_g00_buf_load_file(p_g00_buf, p_ai->al_begin[0].str);
	}
	else if (elm_begin[0] == ELM_G00BUF_FREE)	{
		tnm_g00_buf_free_buf(p_g00_buf);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_g00_buf->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：マスクリスト
// ================================================================
bool tnm_command_proc_mask_list(C_elm_mask_list* p_mask_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_mask_list;
		return true;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_mask* p_mask = p_mask_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_mask)	{
			tnm_command_proc_mask(p_mask, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// 失敗したので戻り値に応じた値をスタックに入れておく
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_MASKLIST_GET_SIZE)	{
		tnm_stack_push_int(p_mask_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_mask_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：マスク
// ================================================================
bool tnm_command_proc_mask(C_elm_mask* p_mask, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);
	else if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_mask;
	}
	else if (elm_begin[0] == ELM_MASK_INIT)	{
		p_mask->reinit();
	}
	else if (elm_begin[0] == ELM_MASK_CREATE)	{
		p_mask->create(p_ai->al_begin[0].str);
	}
	else if (elm_begin[0] == ELM_MASK_X)	{
		int al_id = p_ai->al_id;

		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_mask->get_x());
		else if (al_id == 1)	p_mask->set_x(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_MASK_Y)	{
		int al_id = p_ai->al_id;

		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_mask->get_y());
		else if (al_id == 1)	p_mask->set_y(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_MASK_X_EVE)	{
		tnm_command_proc_int_event(&p_mask->x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_MASK_Y_EVE)	{
		tnm_command_proc_int_event(&p_mask->y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_mask->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：ファイル
// ================================================================
bool tnm_command_proc_file(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);
	else if (elm_begin[0] == ELM_FILE_PRELOAD_OMV)	{
		tnm_preload_omv(p_ai->al_begin[0].str);
	}

	return true;
}

