#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_screen.h"
#include	"engine/ifc_error.h"
#include	"engine/ifc_stack.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// コマンド処理：スクリーン
// ================================================================
bool tnm_command_proc_screen(C_elm_screen* p_screen, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_screen;
	}
	else	{

		switch (elm_begin[0])	{

			case ELM_SCREEN_EFFECT:
				tnm_command_proc_effect_list(&p_screen->effect_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_SHAKE:
				p_screen->shake().start(p_ai->al_begin[0].Int, true);
				break;
			case ELM_SCREEN_QUAKE:
				tnm_command_proc_quake_list(&p_screen->quake_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;

	// ★以下、互換性のために持たせています。エフェクトの０番に直接アクセスできます。
			case ELM_SCREEN_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_pos_x());
				else if (al_id == 1)	p_screen->effect_list()[0].set_pos_x(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_pos_y());
				else if (al_id == 1)	p_screen->effect_list()[0].set_pos_y(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_pos_z());
				else if (al_id == 1)	p_screen->effect_list()[0].set_pos_z(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_MONO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_mono());
				else if (al_id == 1)	p_screen->effect_list()[0].set_mono(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_REVERSE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_reverse());
				else if (al_id == 1)	p_screen->effect_list()[0].set_reverse(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_BRIGHT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_bright());
				else if (al_id == 1)	p_screen->effect_list()[0].set_bright(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_DARK:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_dark());
				else if (al_id == 1)	p_screen->effect_list()[0].set_dark(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_r());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_r(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_g());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_g(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_b());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_b(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_RATE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_rate());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_rate(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_ADD_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_add_r());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_add_r(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_ADD_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_add_g());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_add_g(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_COLOR_ADD_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_screen->effect_list()[0].get_color_add_b());
				else if (al_id == 1)	p_screen->effect_list()[0].set_color_add_b(p_ai->al_begin[0].Int);
				break;
			case ELM_SCREEN_X_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].x(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_Y_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].y(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_Z_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].z(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_MONO_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].mono(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_REVERSE_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].reverse(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_BRIGHT_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].bright(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_DARK_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].dark(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_R_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_r(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_G_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_g(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_B_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_b(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_RATE_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_rate(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_ADD_R_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_add_r(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_ADD_G_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_add_g(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_SCREEN_COLOR_ADD_B_EVE:
				tnm_command_proc_int_event(&p_screen->effect_list()[0].color_add_b(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			default:
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(screen)"));
				break;
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：エフェクトリスト
// ================================================================
bool tnm_command_proc_effect_list(C_elm_effect_list* p_effect_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_effect_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_effect* p_effect = p_effect_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_effect)
			tnm_command_proc_effect(p_effect, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EFFECTLIST_RESIZE)	{
		p_effect_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_EFFECTLIST_GET_SIZE)	{
		tnm_stack_push_int(p_effect_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(effectlist)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：エフェクト
// ================================================================
bool tnm_command_proc_effect(C_elm_effect* p_effect, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_effect;
	}
	else	{

		switch (elm_begin[0])	{

			case ELM_EFFECT_INIT:
				p_effect->reinit();		// 再初期化
				break;

			case ELM_EFFECT_WIPE_COPY:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_wipe_copy());
				else if (al_id == 1)	p_effect->set_wipe_copy(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_WIPE_ERASE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_wipe_erase());
				else if (al_id == 1)	p_effect->set_wipe_erase(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_pos_x());
				else if (al_id == 1)	p_effect->set_pos_x(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_pos_y());
				else if (al_id == 1)	p_effect->set_pos_y(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_pos_z());
				else if (al_id == 1)	p_effect->set_pos_z(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_MONO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_mono());
				else if (al_id == 1)	p_effect->set_mono(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_REVERSE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_reverse());
				else if (al_id == 1)	p_effect->set_reverse(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_BRIGHT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_bright());
				else if (al_id == 1)	p_effect->set_bright(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_DARK:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_dark());
				else if (al_id == 1)	p_effect->set_dark(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_r());
				else if (al_id == 1)	p_effect->set_color_r(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_g());
				else if (al_id == 1)	p_effect->set_color_g(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_b());
				else if (al_id == 1)	p_effect->set_color_b(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_RATE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_rate());
				else if (al_id == 1)	p_effect->set_color_rate(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_ADD_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_add_r());
				else if (al_id == 1)	p_effect->set_color_add_r(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_ADD_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_add_g());
				else if (al_id == 1)	p_effect->set_color_add_g(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_COLOR_ADD_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_color_add_b());
				else if (al_id == 1)	p_effect->set_color_add_b(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_X_EVE:
				tnm_command_proc_int_event(&p_effect->x(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_Y_EVE:
				tnm_command_proc_int_event(&p_effect->y(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_Z_EVE:
				tnm_command_proc_int_event(&p_effect->z(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_MONO_EVE:
				tnm_command_proc_int_event(&p_effect->mono(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_REVERSE_EVE:
				tnm_command_proc_int_event(&p_effect->reverse(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_BRIGHT_EVE:
				tnm_command_proc_int_event(&p_effect->bright(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_DARK_EVE:
				tnm_command_proc_int_event(&p_effect->dark(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_R_EVE:
				tnm_command_proc_int_event(&p_effect->color_r(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_G_EVE:
				tnm_command_proc_int_event(&p_effect->color_g(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_B_EVE:
				tnm_command_proc_int_event(&p_effect->color_b(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_RATE_EVE:
				tnm_command_proc_int_event(&p_effect->color_rate(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_ADD_R_EVE:
				tnm_command_proc_int_event(&p_effect->color_add_r(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_ADD_G_EVE:
				tnm_command_proc_int_event(&p_effect->color_add_g(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_EFFECT_COLOR_ADD_B_EVE:
				tnm_command_proc_int_event(&p_effect->color_add_b(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;

			case ELM_EFFECT_BEGIN_ORDER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_begin_order());
				else if (al_id == 1)	p_effect->set_begin_order(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_END_ORDER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_end_order());
				else if (al_id == 1)	p_effect->set_end_order(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_BEGIN_LAYER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_begin_layer());
				else if (al_id == 1)	p_effect->set_begin_layer(p_ai->al_begin[0].Int);
				break;
			case ELM_EFFECT_END_LAYER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_effect->get_end_layer());
				else if (al_id == 1)	p_effect->set_end_layer(p_ai->al_begin[0].Int);
				break;
			default:
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(effect)"));
				break;
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：クエイクリスト
// ================================================================
bool tnm_command_proc_quake_list(C_elm_quake_list* p_quake_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_quake_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_quake* p_quake = p_quake_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_quake)
			tnm_command_proc_quake(p_quake, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(quakelist)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：クエイク
// ================================================================
bool tnm_command_proc_quake(C_elm_quake* p_quake, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_quake;
	}
	else if (
		elm_begin[0] == ELM_QUAKE_START || 
		elm_begin[0] == ELM_QUAKE_START_WAIT || 
		elm_begin[0] == ELM_QUAKE_START_WAIT_KEY || 
		elm_begin[0] == ELM_QUAKE_START_NOWAIT || 
		elm_begin[0] == ELM_QUAKE_START_ALL || 
		elm_begin[0] == ELM_QUAKE_START_ALL_WAIT || 
		elm_begin[0] == ELM_QUAKE_START_ALL_WAIT_KEY || 
		elm_begin[0] == ELM_QUAKE_START_ALL_NOWAIT
		)	{

		int type = 0, time = 1000, cnt = 0, end_cnt = 0;
		int begin_order = 0, end_order = 0;

		// 引数を調べる
		if (
			elm_begin[0] == ELM_QUAKE_START || 
			elm_begin[0] == ELM_QUAKE_START_WAIT || 
			elm_begin[0] == ELM_QUAKE_START_WAIT_KEY || 
			elm_begin[0] == ELM_QUAKE_START_NOWAIT)
		{
			switch (al_id)	{
				default:
				case 2:		end_order = p_ai->al_begin[5].Int;
							begin_order = p_ai->al_begin[4].Int;
				case 1:		end_cnt = p_ai->al_begin[3].Int;
				case 0:		cnt = p_ai->al_begin[2].Int;
							time = p_ai->al_begin[1].Int;
							type = p_ai->al_begin[0].Int;
			}
		}
		else	{
			begin_order = INT_MIN;
			end_order = INT_MAX;

			switch (al_id)	{
				default:
				case 2:		end_order = p_ai->al_begin[5].Int;
							begin_order = p_ai->al_begin[4].Int;
				case 1:		end_cnt = p_ai->al_begin[3].Int;
				case 0:		cnt = p_ai->al_begin[2].Int;
							time = p_ai->al_begin[1].Int;
							type = p_ai->al_begin[0].Int;
			}
		}

		// ウェイトフラグを調べる
		bool wait_flag = false;
		bool key_flag = false;
		if (
			elm_begin[0] == ELM_QUAKE_START_WAIT || 
			elm_begin[0] == ELM_QUAKE_START_ALL_WAIT)
		{
			wait_flag = true;
		}
		else if (
			elm_begin[0] == ELM_QUAKE_START_WAIT_KEY || 
			elm_begin[0] == ELM_QUAKE_START_ALL_WAIT_KEY)
		{
			wait_flag = true;
			key_flag = true;
		}

		// オプションを調べる
		C_tnm_prop* p_opt = p_ai->al_begin[p_ai->al_end - p_ai->al_begin - 1].exp_list.get_sub().get();
		int opt_cnt = p_ai->al_begin[p_ai->al_end - p_ai->al_begin - 1].exp_cnt;

		// タイプによって分岐
		if (false);
		else if (type == 0)	{
			int power = opt_cnt >= 1 ? p_opt[0].Int : 0;
			int vec = opt_cnt >= 2 ? p_opt[1].Int : 0;
			p_quake->start_vec(vec, power, time, cnt, end_cnt, begin_order, end_order, wait_flag, key_flag);
		}
		else if (type == 1)	{
			int power = opt_cnt >= 1 ? p_opt[0].Int : 0;
			int vec = opt_cnt >= 2 ? p_opt[1].Int : 0;
			p_quake->start_dir(vec, power, time, cnt, end_cnt, begin_order, end_order, wait_flag, key_flag);
		}
		else if (type == 2)	{
			int power = opt_cnt >= 1 ? p_opt[0].Int : 0;
			int center_x = opt_cnt >= 2 ? p_opt[1].Int : 0;
			int center_y = opt_cnt >= 3 ? p_opt[2].Int : 0;
			p_quake->start_zoom(power, time, cnt, end_cnt, center_x, center_y, begin_order, end_order, wait_flag, key_flag);
		}
	}
	else if (elm_begin[0] == ELM_QUAKE_END)	{
		p_quake->end(p_ai->al_end - p_ai->al_begin >= 1 ? p_ai->al_begin[0].Int : 0);
	}
	else if (elm_begin[0] == ELM_QUAKE_WAIT)	{
		p_quake->wait(0);
	}
	else if (elm_begin[0] == ELM_QUAKE_WAIT_KEY)	{
		p_quake->wait(1);
	}
	else if (elm_begin[0] == ELM_QUAKE_CHECK)	{
		tnm_stack_push_int(p_quake->check());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(quake)"));
	}

	return true;
}
