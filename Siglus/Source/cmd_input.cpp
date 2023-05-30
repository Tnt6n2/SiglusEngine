#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_editbox.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// コマンド処理：入力
// ================================================================
bool tnm_command_proc_input(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);
	else if (elm_begin[0] == ELM_INPUT_CLEAR)	{
		Gp_script_input->clear();
	}
	else if (elm_begin[0] == ELM_INPUT_NEXT)	{
		Gp_script_input->frame();
	}
	else if (elm_begin[0] == ELM_INPUT_DECIDE)	{
		tnm_command_proc_key(VK_EX_DECIDE, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_INPUT_CANCEL)	{
		tnm_command_proc_key(VK_EX_CANCEL, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(input)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：マウス
// ================================================================
bool tnm_command_proc_mouse(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);

	else if (elm_begin[0] == ELM_MOUSE_CLEAR)	{
		Gp_script_input->now.mouse.clear();
	}
	else if (elm_begin[0] == ELM_MOUSE_NEXT)	{
		Gp_script_input->now.mouse.frame();
	}
	else if (elm_begin[0] == ELM_MOUSE_POS_X)	{
		tnm_stack_push_int(Gp_script_input->now.mouse.pos.x);
	}
	else if (elm_begin[0] == ELM_MOUSE_POS_Y)	{
		tnm_stack_push_int(Gp_script_input->now.mouse.pos.y);
	}
	else if (elm_begin[0] == ELM_MOUSE_GET_POS_X)	{
		tnm_stack_push_int(Gp_script_input->now.mouse.pos.x);
	}
	else if (elm_begin[0] == ELM_MOUSE_GET_POS_Y)	{
		tnm_stack_push_int(Gp_script_input->now.mouse.pos.y);
	}
	else if (elm_begin[0] == ELM_MOUSE_GET_POS)	{
		int mx = Gp_script_input->now.mouse.pos.x;
		int my = Gp_script_input->now.mouse.pos.y;

		S_element* p_element_x = &p_ai->al_begin[0].element;		// コピー先のフラグエレメント
		S_element* p_element_y = &p_ai->al_begin[1].element;		// コピー先のフラグエレメント
		tnm_command_proc_set_int(p_element_x->begin(), p_element_x->end(), mx);
		tnm_command_proc_set_int(p_element_y->begin(), p_element_y->end(), my);
	}
	else if (elm_begin[0] == ELM_MOUSE_SET_POS)	{
		C_point mp = C_point(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);

		// ウィンドウサイズを適用（５０％とかのやつ）
		mp.x = mp.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
		mp.y = mp.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;

		// スクリーン座標に変換
		mp = Gp_wnd->client_to_screen(mp);

		::SetCursorPos(mp.x, mp.y);
	}
	else if (elm_begin[0] == ELM_MOUSE_WHEEL)	{
		tnm_stack_push_int(Gp_script_input->now.mouse.wheel.get_down_cnt() - Gp_script_input->now.mouse.wheel.get_up_cnt());
	}
	else if (elm_begin[0] == ELM_MOUSE_LEFT)	{
		tnm_command_proc_key(VK_LBUTTON, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_MOUSE_RIGHT)	{
		tnm_command_proc_key(VK_RBUTTON, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(mouse)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：キーリスト
// ================================================================
bool tnm_command_proc_key_list(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (false);

	else if (elm_begin[0] == ELM_ARRAY)	{
		int key_no = elm_begin[1];
		if (key_no < 0 || 256 <= key_no)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("key[") + tostr(key_no) + _T("] にアクセスしました。"));
		}
		else	{
			tnm_command_proc_key(key_no, elm_top, elm_begin + 2, elm_end, p_ai);
		}
	}
	else if (elm_begin[0] == ELM_KEYLIST_WAIT)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_KEY_WAIT;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_KEYLIST_WAIT_FORCE)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_KEY_WAIT;
		proc.skip_disable_flag = true;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_KEYLIST_CLEAR)	{
		Gp_script_input->now.keyboard.clear();
	}
	else if (elm_begin[0] == ELM_KEYLIST_NEXT)	{
		Gp_script_input->now.keyboard.frame();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(keylist)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：キー
// ================================================================
bool tnm_command_proc_key(int key_no, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (key_no == VK_EX_DECIDE || key_no == VK_EX_CANCEL)	{

		// キーの処理を行う
		if (false);

		else if (elm_begin[0] == ELM_KEY_ON_DOWN)	{
			tnm_stack_push_int(Gp_script_input->check_ex_key_down(key_no) ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_ON_UP)	{
			tnm_stack_push_int(Gp_script_input->check_ex_key_up(key_no) ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_ON_DOWN_UP)	{
			tnm_stack_push_int(Gp_script_input->check_ex_key_down_up(key_no) ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_IS_DOWN)	{
			tnm_stack_push_int(Gp_script_input->is_ex_key_down(key_no));
		}
		else if (elm_begin[0] == ELM_KEY_IS_UP)	{
			tnm_stack_push_int(Gp_script_input->is_ex_key_up(key_no));
		}
		else	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(key)"));
		}
	}
	else	{

		// key_no によってマウスとキーボードに振り分ける
		C_input_state::BUTTON* p_button = NULL;
		if (false);
		else if (key_no == VK_LBUTTON)	{
			p_button = &Gp_script_input->now.mouse.left;
		}
		else if (key_no == VK_RBUTTON)	{
			p_button = &Gp_script_input->now.mouse.right;
		}
		else	{
			p_button = &Gp_script_input->now.keyboard.key[key_no];
		}

		// キーの処理を行う
		if (false);

		else if (elm_begin[0] == ELM_KEY_ON_DOWN)	{
			tnm_stack_push_int(p_button->check_down_stock() ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_ON_UP)	{
			tnm_stack_push_int(p_button->check_up_stock() ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_ON_DOWN_UP)	{
			tnm_stack_push_int(p_button->check_down_up_stock() ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_IS_DOWN)	{
			tnm_stack_push_int(p_button->is_down());
		}
		else if (elm_begin[0] == ELM_KEY_IS_UP)	{
			tnm_stack_push_int(p_button->is_up());
		}
		else if (elm_begin[0] == ELM_KEY_ON_FLICK)	{
			tnm_stack_push_int(p_button->check_flick_stock() ? 1 : 0);
		}
		else if (elm_begin[0] == ELM_KEY_GET_FLICK_ANGLE)	{
			tnm_stack_push_int((int)( (180.0 - p_button->get_flick_angle() / D3DX_PI * 180) * TNM_ANGLE_UNIT));
		}
		else if (elm_begin[0] == ELM_KEY_GET_FLICK_PIXEL)	{
			tnm_stack_push_int((int)(p_button->get_flick_pixel()));
		}
		else if (elm_begin[0] == ELM_KEY_GET_FLICK_MM)	{
			tnm_stack_push_int((int)(p_button->get_flick_mm()));
		}
		else	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(key)"));
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：エディットボックスリスト
// ================================================================
bool tnm_command_proc_editbox_list(C_elm_editbox_list* p_editbox_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_editbox_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		int index = elm_begin[1];
		C_elm_editbox* p_editbox = p_editbox_list->get_sub(index, p_ai->disp_out_of_range_error);
		if (p_editbox)	{
			tnm_command_proc_editbox(p_editbox, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// 失敗したので戻り値に応じた値をスタックに入れておく
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_EDITBOXLIST_CLEAR_INPUT)	{
		int editbox_cnt = p_editbox_list->get_size();
		for (int i = 0; i < editbox_cnt; i++)
			(*p_editbox_list)[i].clear_input();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_editbox_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：エディットボックス
// ================================================================
bool tnm_command_proc_editbox(C_elm_editbox* p_editbox, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_editbox;
	}
	else if (elm_begin[0] == ELM_EDITBOX_CREATE)	{
		C_rect rect = C_rect::by_size(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int);
		p_editbox->create(rect, p_ai->al_begin[4].Int);
	}
	else if (elm_begin[0] == ELM_EDITBOX_DESTROY)	{
		p_editbox->destroy();
	}
	else if (elm_begin[0] == ELM_EDITBOX_SET_TEXT)	{
		p_editbox->set_text(p_ai->al_begin[0].str);
	}
	else if (elm_begin[0] == ELM_EDITBOX_GET_TEXT)	{
		tnm_stack_push_str(p_editbox->get_text());
	}
	else if (elm_begin[0] == ELM_EDITBOX_SET_FOCUS)	{
		p_editbox->set_focus();
	}
	else if (elm_begin[0] == ELM_EDITBOX_CLEAR_INPUT)	{
		p_editbox->clear_input();
	}
	else if (elm_begin[0] == ELM_EDITBOX_CHECK_DECIDED)	{
		tnm_stack_push_int(p_editbox->is_decided() ? 1 : 0);
	}
	else if (elm_begin[0] == ELM_EDITBOX_CHECK_CANCELED)	{
		tnm_stack_push_int(p_editbox->is_canceled() ? 1 : 0);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_editbox->get_element_name());
	}

	return true;
}
