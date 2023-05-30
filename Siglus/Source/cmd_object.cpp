#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_object.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// コマンド処理：オブジェクトリスト
// ================================================================
bool tnm_command_proc_object_list(C_elm_object_list* p_object_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_object_list;
	}
	else if (elm_begin[0] == ELM_UP)	{

		C_tnm_element* p_parent = p_object_list->get_parent();

		// ▲とりあえず考えられるフォーム
		if (false);
		else if (p_parent->get_element_form() == FM_STAGE)	{
			return tnm_command_proc_stage((C_elm_stage *)p_parent, elm_top, elm_begin + 1, elm_end, p_ai);
		}
		else if (p_parent->get_element_form() == FM_OBJECT)	{
			return tnm_command_proc_object((C_elm_object *)p_parent, elm_top, elm_begin + 1, elm_end, p_ai);
		}
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_object* p_obj = p_object_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_obj)	{
			tnm_command_proc_object(p_obj, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else	{
			// 失敗したので戻り値に応じた値をスタックに入れておく
			switch (p_ai->ret_form_code)	{
				case FM_INT:	tnm_stack_push_int(0);			break;
				case FM_STR:	tnm_stack_push_str(_T(""));		break;
			}
		}
	}
	else if (elm_begin[0] == ELM_OBJECTLIST_RESIZE)	{
		p_object_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_OBJECTLIST_GET_SIZE)	{
		tnm_stack_push_int(p_object_list->get_size());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_object_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：オブジェクト
// ================================================================
bool tnm_command_proc_object(C_elm_object* p_obj, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_obj;
	}
	else if (elm_begin[0] == ELM_UP)	{

		C_tnm_element* p_parent = p_obj->get_parent();

		// ▲とりあえず考えられるフォーム
		if (false);
		else if (p_parent->get_element_form() == FM_OBJECTLIST)	{
			return tnm_command_proc_object_list((C_elm_object_list *)p_parent, elm_top, elm_begin + 1, elm_end, p_ai);
		}
	}
	else if (!p_obj->is_use())	{

		// 使わない場合は何もしない！
	}
	else	{

		switch (elm_begin[0])	{
			case ELM_OBJECT_WIPE_COPY:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_wipe_copy());
				else if (al_id == 1)	p_obj->set_wipe_copy(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_WIPE_ERASE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_wipe_erase());
				else if (al_id == 1)	p_obj->set_wipe_erase(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CLICK_DISABLE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_click_disable());
				else if (al_id == 1)	p_obj->set_click_disable(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_DISP:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_disp());
				else if (al_id == 1)	p_obj->set_disp(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_PATNO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_pat_no());
				else if (al_id == 1)	p_obj->set_pat_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_WORLD:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_world_no());
				else if (al_id == 1)	p_obj->set_world_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_ORDER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_order());
				else if (al_id == 1)	p_obj->set_order(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_LAYER:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_layer());
				else if (al_id == 1)	p_obj->set_layer(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_pos_x());
				else if (al_id == 1)	p_obj->set_pos_x(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_pos_y());
				else if (al_id == 1)	p_obj->set_pos_y(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_pos_z());
				else if (al_id == 1)	p_obj->set_pos_z(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_X_REP:
				tnm_command_proc_int_list(&p_obj->x_rep_event_list(), elm_begin, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Y_REP:
				tnm_command_proc_int_list(&p_obj->y_rep_event_list(), elm_begin, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Z_REP:
				tnm_command_proc_int_list(&p_obj->z_rep_event_list(), elm_begin, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SET_POS:
				if (false);
				else if (al_id == 0)	{	p_obj->set_pos_x(p_ai->al_begin[0].Int);	p_obj->set_pos_y(p_ai->al_begin[1].Int);										}
				else if (al_id == 1)	{	p_obj->set_pos_x(p_ai->al_begin[0].Int);	p_obj->set_pos_y(p_ai->al_begin[1].Int);	p_obj->set_pos_z(p_ai->al_begin[2].Int);	}
				break;
			case ELM_OBJECT_CENTER_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_x());
				else if (al_id == 1)	p_obj->set_center_x(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CENTER_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_y());
				else if (al_id == 1)	p_obj->set_center_y(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CENTER_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_z());
				else if (al_id == 1)	p_obj->set_center_z(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_CENTER:
				if (false);
				else if (al_id == 0)	{	p_obj->set_center_x(p_ai->al_begin[0].Int);	p_obj->set_center_y(p_ai->al_begin[1].Int);										}
				else if (al_id == 1)	{	p_obj->set_center_x(p_ai->al_begin[0].Int);	p_obj->set_center_y(p_ai->al_begin[1].Int);	p_obj->set_center_z(p_ai->al_begin[2].Int);	}
				break;
			case ELM_OBJECT_CENTER_REP_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_rep_x());
				else if (al_id == 1)	p_obj->set_center_rep_x(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CENTER_REP_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_rep_y());
				else if (al_id == 1)	p_obj->set_center_rep_y(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CENTER_REP_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_center_rep_z());
				else if (al_id == 1)	p_obj->set_center_rep_z(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_CENTER_REP:
				if (false);
				else if (al_id == 0)	{	p_obj->set_center_rep_x(p_ai->al_begin[0].Int);	p_obj->set_center_rep_y(p_ai->al_begin[1].Int);										}
				else if (al_id == 1)	{	p_obj->set_center_rep_x(p_ai->al_begin[0].Int);	p_obj->set_center_rep_y(p_ai->al_begin[1].Int);	p_obj->set_center_rep_z(p_ai->al_begin[2].Int);	}
				break;
			case ELM_OBJECT_SCALE_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_scale_x());
				else if (al_id == 1)	p_obj->set_scale_x(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SCALE_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_scale_y());
				else if (al_id == 1)	p_obj->set_scale_y(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SCALE_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_scale_z());
				else if (al_id == 1)	p_obj->set_scale_z(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_SCALE:
				if (false);
				else if (al_id == 0)	{	p_obj->set_scale_x(p_ai->al_begin[0].Int);	p_obj->set_scale_y(p_ai->al_begin[1].Int);											}
				else if (al_id == 1)	{	p_obj->set_scale_x(p_ai->al_begin[0].Int);	p_obj->set_scale_y(p_ai->al_begin[1].Int);	p_obj->set_scale_z(p_ai->al_begin[2].Int);	}
				break;
			case ELM_OBJECT_ROTATE_X:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_rotate_x());
				else if (al_id == 1)	p_obj->set_rotate_x(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_ROTATE_Y:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_rotate_y());
				else if (al_id == 1)	p_obj->set_rotate_y(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_ROTATE_Z:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_rotate_z());
				else if (al_id == 1)	p_obj->set_rotate_z(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_ROTATE:
				if (false);
				else if (al_id == 0)	{	p_obj->set_rotate_x(p_ai->al_begin[0].Int);	p_obj->set_rotate_y(p_ai->al_begin[1].Int);											}
				else if (al_id == 1)	{	p_obj->set_rotate_x(p_ai->al_begin[0].Int);	p_obj->set_rotate_y(p_ai->al_begin[1].Int);	p_obj->set_rotate_z(p_ai->al_begin[2].Int);	}
				break;
			case ELM_OBJECT_CLIP_USE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_clip_use());
				else if (al_id == 1)	p_obj->set_clip_use(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CLIP_LEFT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_clip_left());
				else if (al_id == 1)	p_obj->set_clip_left(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CLIP_TOP:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_clip_top());
				else if (al_id == 1)	p_obj->set_clip_top(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CLIP_RIGHT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_clip_right());
				else if (al_id == 1)	p_obj->set_clip_right(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CLIP_BOTTOM:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_clip_bottom());
				else if (al_id == 1)	p_obj->set_clip_bottom(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_CLIP:
				p_obj->set_clip_use(p_ai->al_begin[0].Int);
				p_obj->set_clip_left(p_ai->al_begin[1].Int);
				p_obj->set_clip_top(p_ai->al_begin[2].Int);
				p_obj->set_clip_right(p_ai->al_begin[3].Int);
				p_obj->set_clip_bottom(p_ai->al_begin[4].Int);
				break;
			case ELM_OBJECT_SRC_CLIP_USE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_src_clip_use());
				else if (al_id == 1)	p_obj->set_src_clip_use(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SRC_CLIP_LEFT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_src_clip_left());
				else if (al_id == 1)	p_obj->set_src_clip_left(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SRC_CLIP_TOP:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_src_clip_top());
				else if (al_id == 1)	p_obj->set_src_clip_top(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SRC_CLIP_RIGHT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_src_clip_right());
				else if (al_id == 1)	p_obj->set_src_clip_right(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SRC_CLIP_BOTTOM:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_src_clip_bottom());
				else if (al_id == 1)	p_obj->set_src_clip_bottom(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_SET_SRC_CLIP:
				p_obj->set_src_clip_use(p_ai->al_begin[0].Int);
				p_obj->set_src_clip_left(p_ai->al_begin[1].Int);
				p_obj->set_src_clip_top(p_ai->al_begin[2].Int);
				p_obj->set_src_clip_right(p_ai->al_begin[3].Int);
				p_obj->set_src_clip_bottom(p_ai->al_begin[4].Int);
				break;

			case ELM_OBJECT_TR:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_tr());
				else if (al_id == 1)	p_obj->set_tr(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_TR_REP:
				tnm_command_proc_int_list(&p_obj->tr_rep_event_list(), elm_begin, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_MONO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_mono());
				else if (al_id == 1)	p_obj->set_mono(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_REVERSE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_reverse());
				else if (al_id == 1)	p_obj->set_reverse(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_BRIGHT:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_bright());
				else if (al_id == 1)	p_obj->set_bright(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_DARK:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_dark());
				else if (al_id == 1)	p_obj->set_dark(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_r());
				else if (al_id == 1)	p_obj->set_color_r(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_g());
				else if (al_id == 1)	p_obj->set_color_g(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_b());
				else if (al_id == 1)	p_obj->set_color_b(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_RATE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_rate());
				else if (al_id == 1)	p_obj->set_color_rate(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_ADD_R:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_add_r());
				else if (al_id == 1)	p_obj->set_color_add_r(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_ADD_G:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_add_g());
				else if (al_id == 1)	p_obj->set_color_add_g(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_COLOR_ADD_B:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_color_add_b());
				else if (al_id == 1)	p_obj->set_color_add_b(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_MASK_NO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_mask_no());
				else if (al_id == 1)	p_obj->set_mask_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_TONECURVE_NO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_tone_curve_no());
				else if (al_id == 1)	p_obj->set_tone_curve_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_CULLING:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_culling());
				else if (al_id == 1)	p_obj->set_culling(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_ALPHA_TEST:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_alpha_test());
				else if (al_id == 1)	p_obj->set_alpha_test(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_ALPHA_BLEND:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_alpha_blend());
				else if (al_id == 1)	p_obj->set_alpha_blend(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_BLEND:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_blend_type());
				else if (al_id == 1)	p_obj->set_blend_type(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_LIGHT_NO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_light_no());
				else if (al_id == 1)	p_obj->set_light_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_FOG_USE:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->get_fog_use());
				else if (al_id == 1)	p_obj->set_fog_use(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_PATNO_EVE:
				tnm_command_proc_int_event(&p_obj->get_pat_no_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_X_EVE:
				tnm_command_proc_int_event(&p_obj->get_x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Y_EVE:
				tnm_command_proc_int_event(&p_obj->get_y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Z_EVE:
				tnm_command_proc_int_event(&p_obj->get_z_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_X_REP_EVE:
				tnm_command_proc_int_event_list(&p_obj->x_rep_event_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Y_REP_EVE:
				tnm_command_proc_int_event_list(&p_obj->y_rep_event_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_Z_REP_EVE:
				tnm_command_proc_int_event_list(&p_obj->z_rep_event_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_X_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_Y_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_Z_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_z_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_REP_X_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_rep_x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_REP_Y_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_rep_y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CENTER_REP_Z_EVE:
				tnm_command_proc_int_event(&p_obj->get_center_rep_z_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SCALE_X_EVE:
				tnm_command_proc_int_event(&p_obj->get_scale_x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SCALE_Y_EVE:
				tnm_command_proc_int_event(&p_obj->get_scale_y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SCALE_Z_EVE:
				tnm_command_proc_int_event(&p_obj->get_scale_z_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_ROTATE_X_EVE:
				tnm_command_proc_int_event(&p_obj->get_rotate_x_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_ROTATE_Y_EVE:
				tnm_command_proc_int_event(&p_obj->get_rotate_y_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_ROTATE_Z_EVE:
				tnm_command_proc_int_event(&p_obj->get_rotate_z_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CLIP_LEFT_EVE:
				tnm_command_proc_int_event(&p_obj->get_clip_left_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CLIP_TOP_EVE:
				tnm_command_proc_int_event(&p_obj->get_clip_top_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CLIP_RIGHT_EVE:
				tnm_command_proc_int_event(&p_obj->get_clip_right_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_CLIP_BOTTOM_EVE:
				tnm_command_proc_int_event(&p_obj->get_clip_bottom_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SRC_CLIP_LEFT_EVE:
				tnm_command_proc_int_event(&p_obj->get_src_clip_left_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SRC_CLIP_TOP_EVE:
				tnm_command_proc_int_event(&p_obj->get_src_clip_top_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SRC_CLIP_RIGHT_EVE:
				tnm_command_proc_int_event(&p_obj->get_src_clip_right_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_SRC_CLIP_BOTTOM_EVE:
				tnm_command_proc_int_event(&p_obj->get_src_clip_bottom_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_TR_EVE:
				tnm_command_proc_int_event(&p_obj->get_tr_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_TR_REP_EVE:
				tnm_command_proc_int_event_list(&p_obj->tr_rep_event_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_MONO_EVE:
				tnm_command_proc_int_event(&p_obj->get_mono_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_REVERSE_EVE:
				tnm_command_proc_int_event(&p_obj->get_reverse_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_BRIGHT_EVE:
				tnm_command_proc_int_event(&p_obj->get_bright_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_DARK_EVE:
				tnm_command_proc_int_event(&p_obj->get_dark_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_R_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_r_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_G_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_g_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_B_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_b_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_RATE_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_rate_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_ADD_R_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_add_r_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_ADD_G_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_add_g_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_COLOR_ADD_B_EVE:
				tnm_command_proc_int_event(&p_obj->get_color_add_b_event(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;
			case ELM_OBJECT_ALL_EVE:

				if (false);
				else if (elm_begin[1] == ELM_ALLEVENT_END)	{
					p_obj->end_all_event();
				}
				else if (elm_begin[1] == ELM_ALLEVENT_WAIT)	{
					C_tnm_proc proc;
					proc.type = TNM_PROC_TYPE_ALL_EVENT_WAIT;
					proc.element = S_element(elm_top, elm_end - 2);	// オブジェクト
					tnm_push_proc(proc);
				}
				else if (elm_begin[1] == ELM_ALLEVENT_CHECK)	{
					tnm_stack_push_int(p_obj->check_all_event() ? 1 : 0);
				}
				else	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(allevent)"));
				}
				break;

			case ELM_OBJECT_GET_PAT_CNT:
				tnm_stack_push_int(p_obj->get_pat_cnt());
				break;
			case ELM_OBJECT_GET_SIZE_X:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_size_x(0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_size_x(p_ai->al_begin[0].Int));	break;
					default:	tnm_stack_push_int(0);											break;
				}
				break;
			case ELM_OBJECT_GET_SIZE_Y:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_size_y(0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_size_y(p_ai->al_begin[0].Int));	break;
					default:	tnm_stack_push_int(0);											break;
				}
				break;
			case ELM_OBJECT_GET_SIZE_Z:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_size_z(0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_size_z(p_ai->al_begin[0].Int));	break;
					default:	tnm_stack_push_int(0);											break;
				}
				break;
			case ELM_OBJECT_GET_PIXEL_COLOR_R:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_pixel_color_r(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, 0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_pixel_color_r(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int));	break;
					default:	tnm_stack_push_int(0);																								break;
				}
				break;
			case ELM_OBJECT_GET_PIXEL_COLOR_G:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_pixel_color_g(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, 0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_pixel_color_g(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int));	break;
					default:	tnm_stack_push_int(0);																								break;
				}
				break;
			case ELM_OBJECT_GET_PIXEL_COLOR_B:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_pixel_color_b(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, 0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_pixel_color_b(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int));	break;
					default:	tnm_stack_push_int(0);																								break;
				}
				break;
			case ELM_OBJECT_GET_PIXEL_COLOR_A:
				switch (al_id)	{
					case 0:		tnm_stack_push_int(p_obj->get_pixel_color_a(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, 0));						break;
					case 1:		tnm_stack_push_int(p_obj->get_pixel_color_a(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int));	break;
					default:	tnm_stack_push_int(0);																								break;
				}
				break;
			case ELM_OBJECT_GET_FILE_NAME:
				tnm_stack_push_str(p_obj->get_file_path());
				break;
			case ELM_OBJECT_F:
				tnm_command_proc_int_list(&p_obj->F(), 32, elm_begin, elm_begin + 1, elm_end, p_ai);
				break;

		//
		// メソッド
		//
			case ELM_OBJECT_INIT:
				p_obj->reinit(true);
				break;
			case ELM_OBJECT_FREE:
				p_obj->free();
				break;
			case ELM_OBJECT_INIT_PARAM:
				p_obj->init_param(true);
				break;
			case ELM_OBJECT_CREATE:
			{
				TSTR file_name = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create コマンドで指定したファイル名が空です。"));
				}
				else	{

					// ピクチャオブジェクトを作成する
					p_obj->create_pct(file_name);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 3:		p_obj->set_pat_no(p_ai->al_begin[4].Int);
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}
				}
				break;
			}

			case ELM_OBJECT_CREATE_RECT:
			{
				// 自動初期化する！
				p_obj->reinit(true);

				// 矩形オブジェクトを作成する
				p_obj->create_rect(C_rect(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int), C_argb(p_ai->al_begin[7].Int, p_ai->al_begin[4].Int, p_ai->al_begin[5].Int, p_ai->al_begin[6].Int));

				// パラメータを設定する
				switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[10].Int);
								p_obj->set_pos_x(p_ai->al_begin[9].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[8].Int);
				}
				break;
			}
			case ELM_OBJECT_CREATE_STRING:
			{
				TSTR str = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// 文字オブジェクトを作成する
				p_obj->create_string(str);

				// パラメータを設定する
				switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
								p_obj->set_pos_x(p_ai->al_begin[2].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
				}
				break;
			}
			case ELM_OBJECT_CREATE_NUMBER:
			{
				TSTR file_name = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_number コマンドで指定したファイル名が空です。"));
				}
				else	{

					// 数値オブジェクトを作成する
					p_obj->create_number(file_name);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}
				}
				break;
			}
			case ELM_OBJECT_CREATE_WEATHER:
			{
				TSTR file_name = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_weather コマンドで指定したファイル名が空です。"));
				}
				else	{

					// 環境オブジェクトを作成する
					p_obj->create_weather(file_name);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}
				}
				break;
			}
			case ELM_OBJECT_CREATE_MESH:
			{
				TSTR file_name = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_mesh コマンドで指定したファイル名が空です。"));
				}
				else	{

					// メッシュオブジェクトを作成する
					p_obj->create_mesh(file_name);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}
				}
				break;
			}
			case ELM_OBJECT_CREATE_BILLBOARD:
			{
				TSTR file_name = p_ai->al_begin[0].str;

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_billboard コマンドで指定したファイル名が空です。"));
				}
				else	{
					
					// ビルボードオブジェクトを作成する
					p_obj->create_billboard(file_name);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}
					break;
				}
			}
			case ELM_OBJECT_CREATE_SAVE_THUMB:
			{
				// 自動初期化する！
				p_obj->reinit(true);

				// セーブサムネイルオブジェクトを作成する
				p_obj->create_save_thumb(p_ai->al_begin[0].Int);

				// パラメータを設定する
				switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
								p_obj->set_pos_x(p_ai->al_begin[2].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
				}
				break;
			}
			case ELM_OBJECT_CREATE_CAPTURE_THUMB:
			{
				// 自動初期化する！
				p_obj->reinit(true);

				// サムネイルオブジェクトを作成する
				p_obj->create_thumb(p_ai->al_begin[0].Int);

				// パラメータを設定する
				switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
								p_obj->set_pos_x(p_ai->al_begin[2].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
				}
				break;
			}
			case ELM_OBJECT_CREATE_CAPTURE:
			{
				// 自動初期化する！
				p_obj->reinit(true);

				// キャプチャーオブジェクトを作成する
				p_obj->create_capture();

				// パラメータを設定する
				switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[2].Int);
								p_obj->set_pos_x(p_ai->al_begin[1].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[0].Int);
				}
				break;
			}
			case ELM_OBJECT_CREATE_MOVIE:
			case ELM_OBJECT_CREATE_MOVIE_LOOP:
			case ELM_OBJECT_CREATE_MOVIE_WAIT:
			case ELM_OBJECT_CREATE_MOVIE_WAIT_KEY:
			{
				TSTR file_name = p_ai->al_begin[0].str;
				bool loop_flag = false;			// ループしない
				bool wait_flag = false;			// 再生終了を待つ
				bool key_skip_flag = false;		// キーでスキップできる
				bool auto_init_flag = true;		// 自動初期化する
				bool real_time_flag = true;		// 実時間
				bool ready_only_flag = false;	// 準備のみ
				
				// ループ
				if (elm_begin[0] == ELM_OBJECT_CREATE_MOVIE_LOOP)
					loop_flag = true;

				// ウェイト
				if (elm_begin[0] == ELM_OBJECT_CREATE_MOVIE_WAIT || elm_begin[0] == ELM_OBJECT_CREATE_MOVIE_WAIT_KEY)
					wait_flag = true;
				if (elm_begin[0] == ELM_OBJECT_CREATE_MOVIE_WAIT_KEY)
					key_skip_flag = true;

				// 名前つき引数
				for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
					switch (arg->id)	{
						case 0:		auto_init_flag = arg->Int ? true : false;	break;
						case 1:		real_time_flag = arg->Int ? true : false;	break;
						case 2:		ready_only_flag = arg->Int ? true : false;	break;
					}
				}

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_movie コマンドで指定したファイル名が空です。"));
				}
				else	{

					// ムービーオブジェクトを作成する
					p_obj->create_movie(file_name, loop_flag, auto_init_flag, real_time_flag, ready_only_flag);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
						case 2:		p_obj->set_pos_y(p_ai->al_begin[3].Int);
									p_obj->set_pos_x(p_ai->al_begin[2].Int);
						case 1:		p_obj->set_disp(p_ai->al_begin[1].Int);
					}

					// ムービー待ち
					if (wait_flag)	{
						p_obj->wait_movie(key_skip_flag, key_skip_flag);
					}
				}
				break;
			}

			case ELM_OBJECT_CREATE_EMOTE:
			{
				int width = p_ai->al_begin[0].Int;
				int height = p_ai->al_begin[1].Int;
				TSTR file_name = p_ai->al_begin[2].str;
				int rep_x = 0, rep_y = 0;

				// 名前つき引数
				for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)
				{
					switch (arg->id)	{
					case 0:		rep_x = arg->Int;	break;
					case 1:		rep_y = arg->Int;	break;
					}
				}

				// 自動初期化する！
				p_obj->reinit(true);

				// ファイル名判定
				if (file_name.empty())
				{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("create_emote コマンドで指定したファイル名が空です。"));
				}
				else
				{
					// E-mote オブジェクトを作成する
					p_obj->create_emote(width, height, file_name);
					p_obj->emote_set_rep_pos(rep_x, rep_y);

					// パラメータを設定する
					switch (al_id)	{	// no break !!
					case 2:		p_obj->set_pos_y(p_ai->al_begin[5].Int);
								p_obj->set_pos_x(p_ai->al_begin[4].Int);
					case 1:		p_obj->set_disp(p_ai->al_begin[3].Int);
					}
				}
				break;
			}

			case ELM_OBJECT_CREATE_COPY_FROM:
			{
				// 自動初期化する！
				p_obj->reinit(true);

				// オブジェクトの実ポインタを取得
				C_elm_object* src_obj = (C_elm_object *)tnm_get_element_ptr(p_ai->al_begin[0].element.begin(), p_ai->al_begin[0].element.end(), true);
				if (!src_obj)	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("オブジェクトのコピーに失敗しました。"));
				}
				else	{

					// オブジェクトのコピーを作成する
					p_obj->copy(src_obj, false);
				}
				break;
			}
			case ELM_OBJECT_CHANGE_FILE:
				p_obj->change_file(p_ai->al_begin[0].str);
				break;

			case ELM_OBJECT_EXIST_TYPE:
				tnm_stack_push_int(p_obj->get_type() == TNM_OBJECT_TYPE_NONE ? 0 : 1);
				break;

			// 文字列
			case ELM_OBJECT_SET_STRING:
				p_obj->set_string(p_ai->al_begin[0].str);
				break;
			case ELM_OBJECT_GET_STRING:
				tnm_stack_push_str(p_obj->get_string());
				break;
			case ELM_OBJECT_SET_STRING_PARAM:
			{
				int moji_color = Gp_ini->mwnd.moji_color;
				int shadow_color = Gp_ini->mwnd.shadow_color;
				int fuchi_color = Gp_ini->mwnd.fuchi_color;
				int shadow_mode = -1;

				switch (al_id)	{	// no break !!
				case 1:		fuchi_color = p_ai->al_begin[7].Int;
				case 0:		shadow_mode = p_ai->al_begin[6].Int;
							shadow_color = p_ai->al_begin[5].Int;
							moji_color = p_ai->al_begin[4].Int;
				}

				p_obj->set_string_param(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int
					, moji_color, shadow_color, fuchi_color, shadow_mode
					);
				break;
			}

			// 数値
			case ELM_OBJECT_SET_NUMBER:
				p_obj->set_number(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_GET_NUMBER:
				tnm_stack_push_int(p_obj->get_number());
				break;
			case ELM_OBJECT_SET_NUMBER_PARAM:
				p_obj->set_number_param(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int, p_ai->al_begin[5].Int);
				break;

			// 環境
			case ELM_OBJECT_SET_WEATHER_PARAM_TYPE_A:
			{
				int cnt = 0;
				int pat_mode = 0, pat_no_00 = 0, pat_no_01 = 0, pat_time = 0;
				int move_time_x = 0, move_time_y = 0;
				int sin_time_x = 0, sin_power_x = 0, sin_time_y = 0, sin_power_y = 0;
				int scale_x = TNM_SCALE_UNIT, scale_y = TNM_SCALE_UNIT;
				int active_time = 45000;
				int real_time = 0;

				// 名前つき引数
				for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
					switch (arg->id)	{
						case 0:		cnt = arg->Int;					break;
						case 1:		pat_mode = arg->Int;			break;
						case 2:		pat_no_00 = arg->Int;			break;
						case 3:		pat_no_01 = arg->Int;			break;
						case 4:		pat_time = arg->Int;			break;
						case 5:		move_time_x = arg->Int;			break;
						case 6:		move_time_y = arg->Int;			break;
						case 7:		sin_time_x = arg->Int;			break;
						case 8:		sin_power_x = arg->Int;			break;
						case 9:		sin_time_y = arg->Int;			break;
						case 10:	sin_power_y = arg->Int;			break;
						case 11:	real_time = arg->Int;			break;
						case 12:	scale_x = arg->Int;				break;
						case 13:	scale_y = arg->Int;				break;
						case 14:	active_time = arg->Int;			break;
					}
				}
		
				p_obj->set_weather_param_type_a(cnt, pat_mode, pat_no_00, pat_no_01, pat_time, move_time_x, move_time_y, sin_time_x, sin_power_x, sin_time_y, sin_power_y, scale_x, scale_y, active_time, real_time ? true : false);
				break;
			}
			case ELM_OBJECT_SET_WEATHER_PARAM_TYPE_B:
			{
				int cnt = 0;
				int pat_mode = 0, pat_no_00 = 0, pat_no_01 = 0, pat_time = 0;
				int center_x = 0, center_y = 0, appear_range = 100;
				int move_time = 1000;
				int sin_time_x = 0, sin_power_x = 0, sin_time_y = 0, sin_power_y = 0;
				int center_rotate = 0;
				int zoom_min = 1000, zoom_max = 1000;
				int scale_x = TNM_SCALE_UNIT, scale_y = TNM_SCALE_UNIT;
				int real_time = 0;

				// 名前つき引数
				for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
					switch (arg->id)	{
						case 0:		cnt = arg->Int;					break;
						case 1:		pat_mode = arg->Int;			break;
						case 2:		pat_no_00 = arg->Int;			break;
						case 3:		pat_no_01 = arg->Int;			break;
						case 4:		pat_time = arg->Int;			break;
						case 5:		move_time = arg->Int;			break;
						case 6:										break;	// 欠番
						case 7:		sin_time_x = arg->Int;			break;
						case 8:		sin_power_x = arg->Int;			break;
						case 9:		sin_time_y = arg->Int;			break;
						case 10:	sin_power_y = arg->Int;			break;
						case 11:	center_x = arg->Int;			break;
						case 12:	center_y = arg->Int;			break;
						case 13:	appear_range = arg->Int;		break;
						case 14:	zoom_min = arg->Int;			break;
						case 15:	zoom_max = arg->Int;			break;
						case 16:	center_rotate = arg->Int;		break;
						case 17:	real_time = arg->Int;			break;
						case 18:	scale_x = arg->Int;				break;
						case 19:	scale_y = arg->Int;				break;
					}
				}
		
				p_obj->set_weather_param_type_b(cnt, pat_mode, pat_no_00, pat_no_01, pat_time, center_x, center_y, appear_range, move_time, sin_time_x, sin_power_x, sin_time_y, sin_power_y, center_rotate, zoom_min, zoom_max, scale_x, scale_y, real_time ? true : false);
				break;
			}

			// ムービー
			case ELM_OBJECT_PAUSE_MOVIE:
				p_obj->pause_movie();
				break;
			case ELM_OBJECT_RESUME_MOVIE:
				p_obj->resume_movie();
				break;
			case ELM_OBJECT_SEEK_MOVIE:
				p_obj->seek_movie(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_GET_MOVIE_SEEK_TIME:
				tnm_stack_push_int(p_obj->get_movie_seek_time());
				break;
			case ELM_OBJECT_CHECK_MOVIE:
				tnm_stack_push_int(p_obj->check_movie());
				break;
			case ELM_OBJECT_WAIT_MOVIE:
				p_obj->wait_movie(false, false);
				break;
			case ELM_OBJECT_WAIT_MOVIE_KEY:
				p_obj->wait_movie(true, true);
				break;
			case ELM_OBJECT_END_MOVIE_LOOP:
				p_obj->end_movie_loop();
				break;
			case ELM_OBJECT_SET_MOVIE_AUTO_FREE:
				p_obj->set_movie_auto_free(p_ai->al_begin[0].Int ? true : false);
				break;

			// フレームアクション
			case ELM_OBJECT_FRAME_ACTION:
				tnm_command_proc_frame_action(p_obj->get_frame_action(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;

			case ELM_OBJECT_FRAME_ACTION_CH:
				tnm_command_proc_frame_action_list(p_obj->get_frame_action_ch(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;

			// ボタン
			case ELM_OBJECT_CLEAR_BUTTON:
				p_obj->init_button_param();
				break;
			case ELM_OBJECT_SET_BUTTON:
			{
				int button_no = 0, group_no = 0, action_no = 0, se_no = 0;

				switch (al_id)	{	// no break !!
					case 2:		se_no = p_ai->al_begin[3].Int;
								action_no = p_ai->al_begin[2].Int;
					case 1:		group_no = p_ai->al_begin[1].Int;
					case 0:		button_no = p_ai->al_begin[0].Int;
				}

				// ボタンパラメータを設定する
				p_obj->set_button_param(button_no, group_no, action_no, se_no);

				break;
			}
			case ELM_OBJECT_SET_BUTTON_GROUP:
			{
				if (al_id == 0)	{
					p_obj->set_button_group_no(p_ai->al_begin[0].Int);
				}
				else if (al_id == 1)	{
					p_obj->set_button_group(p_ai->al_begin[0].element);
				}
				break;
			}
			case ELM_OBJECT_SET_BUTTON_PUSHKEEP:
				p_obj->set_button_push_keep(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_GET_BUTTON_PUSHKEEP:
				tnm_stack_push_int(p_obj->get_button_push_keep());
				break;
			case ELM_OBJECT_SET_BUTTON_ALPHA_TEST:
				p_obj->set_button_alpha_test(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_GET_BUTTON_ALPHA_TEST:
				tnm_stack_push_int(p_obj->get_button_alpha_test());
				break;
			case ELM_OBJECT_SET_BUTTON_STATE_NORMAL:
				p_obj->set_button_state(TNM_BTN_STATE_NORMAL);
				break;
			case ELM_OBJECT_SET_BUTTON_STATE_SELECT:
				p_obj->set_button_state(TNM_BTN_STATE_SELECT);
				break;
			case ELM_OBJECT_SET_BUTTON_STATE_DISABLE:
				p_obj->set_button_state(TNM_BTN_STATE_DISABLE);
				break;
			case ELM_OBJECT_GET_BUTTON_STATE:
				tnm_stack_push_int(p_obj->get_button_state());
				break;
			case ELM_OBJECT_GET_BUTTON_HIT_STATE:
				tnm_stack_push_int(p_obj->get_button_hit_state());
				break;
			case ELM_OBJECT_GET_BUTTON_REAL_STATE:
				tnm_stack_push_int(p_obj->get_button_real_state());
				break;
			case ELM_OBJECT_SET_BUTTON_CALL:
			{
				TSTR scn_name = Gp_lexer->get_scn_name(Gp_lexer->get_cur_scn_no());
				TSTR cmd_name = p_ai->al_begin[0].str;

				// ボタンパラメータを設定する
				p_obj->set_button_decided_action(scn_name, cmd_name);

				break;
			}
			case ELM_OBJECT_CLEAR_BUTTON_CALL:
				p_obj->clear_button_decide_action();
				break;

			// ＧＡＮ
			case ELM_OBJECT_LOAD_GAN:
			{
				C_tnm_gan* p_gan = &p_obj->gan();
				p_gan->load_gan(p_ai->al_begin[0].str);
				break;
			}
			case ELM_OBJECT_START_GAN:
			{
				int set_no = 0;
				bool loop_flag = true;
				bool real_time_flag = false;

				switch (al_id)	{	// no break !!
				case 1:		real_time_flag = p_ai->al_begin[2].Int ? true : false;
				case 0:		loop_flag = p_ai->al_begin[1].Int ? true : false;
							set_no = p_ai->al_begin[0].Int;
				}

				C_tnm_gan* p_gan = &p_obj->gan();
				p_gan->start_anm(set_no, loop_flag, real_time_flag);
				break;
			}

			// ヒント
			case ELM_OBJECT_CLEAR_HINTS:
			{
				p_obj->clear_hints();
			}
			case ELM_OBJECT_ADD_HINTS:
			{
				// 名前つき引数
				for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
					switch (arg->id)	{
						case 0:
							if (arg->Int)	{	p_obj->add_hints(TNM_OBJECT_FLAGS_NO_EVENT);	}
							else			{	p_obj->sub_hints(TNM_OBJECT_FLAGS_NO_EVENT);	}
							break;
					}
				}
				;
				break;
			}

			// E-mote
			case ELM_OBJECT_EMOTE_PLAY_TIMELINE:
			{
				int buf = p_ai->al_begin[0].Int;
				TSTR timeline = p_ai->al_begin[1].str;
				int option = 0;

				switch (al_id)	{	// no break !!
				case 1:		option = p_ai->al_begin[2].Int;
				}

				p_obj->emote_play_timeline(buf, timeline, option);
				break;
			}

			case ELM_OBJECT_EMOTE_STOP_TIMELINE:
			{
				if (al_id == 0)
				{
					p_obj->emote_stop_timeline_all();
				}
				else
				{
					int buf = p_ai->al_begin[0].Int;

					p_obj->emote_stop_timeline(buf);
				}
				break;
			}

			case ELM_OBJECT_EMOTE_CHECK_PLAYING:
			{
				tnm_stack_push_int(p_obj->emote_check_playing() ? 1 : 0);

				break;
			}
			case ELM_OBJECT_EMOTE_WAIT_PLAYING:
				p_obj->emote_wait(false, false);
				break;
			case ELM_OBJECT_EMOTE_WAIT_PLAYING_KEY:
				p_obj->emote_wait(true, true);
				break;
			case ELM_OBJECT_EMOTE_SKIP:
				p_obj->emote_skip();
				break;
			case ELM_OBJECT_EMOTE_PASS:
				p_obj->emote_pass();
				break;
			case ELM_OBJECT_EMOTE_KOE_CHARA_NO:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->emote_get_koe_chara_no());
				else if (al_id == 1)	p_obj->emote_set_koe_chara_no(p_ai->al_begin[0].Int);
				break;
			case ELM_OBJECT_EMOTE_MOUTH_VOLUME:
				if (false);
				else if (al_id == 0)	tnm_stack_push_int(p_obj->emote_get_koe_mouth_volume());
				else if (al_id == 1)	p_obj->emote_set_koe_mouth_volume(p_ai->al_begin[0].Int);
				break;

			// 子供
			case ELM_OBJECT_CHILD:
				tnm_command_proc_object_list(&p_obj->child_list(), elm_top, elm_begin + 1, elm_end, p_ai);
				break;

			// 子供のソートタイプ
			case ELM_OBJECT_SET_CHILD_SORT_TYPE_DEFAULT:
				p_obj->set_child_sort_type(TNM_OBJECT_SORT_TYPE_DEFAULT);
				break;
			case ELM_OBJECT_SET_CHILD_SORT_TYPE_TEST:
				p_obj->set_child_sort_type(TNM_OBJECT_SORT_TYPE_NONE);
				break;

			// デバッグ用
			case ELM_OBJECT_GET_ELEMENT_NAME:
				tnm_stack_push_str(p_obj->get_element_name());
				break;

			default:
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_obj->get_element_name());
				break;
		}
	}

	return true;
}


