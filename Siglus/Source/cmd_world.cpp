#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_prop.h"
#include	"element/elm_world.h"
#include	"engine/ifc_error.h"
#include	"engine/flow_command.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_stack.h"

// ****************************************************************
// コマンド処理：ワールドリスト
// ================================================================
bool tnm_command_proc_world_list(C_elm_world_list* p_world_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_world_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_world* p_world = p_world_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_world)
			tnm_command_proc_world(p_world, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLDLIST_CREATE_WORLD)	{
		int cnt = (int)p_world_list->get_size();
		p_world_list->resize(cnt + 1);		// ワールドを作成
		tnm_stack_push_int(cnt);			// 古いサイズを返す
	}
	else if (elm_begin[0] == ELM_WORLDLIST_DESTROY_WORLD)	{
		if (p_world_list->get_size() == 0)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("ワールドを破壊しようとしましたが、ワールドが１つもありません。"));
		}
		else	{
			p_world_list->resize(p_world_list->get_size() - 1);
		}
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(world_list)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：ワールド
// ================================================================
bool tnm_command_proc_world(C_elm_world* p_world, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_world;
	}
	else if (elm_begin[0] == ELM_WORLD_INIT)	{
		p_world->reinit();	// 再初期化
	}
	else if (elm_begin[0] == ELM_WORLD_GET_NO)	{
		tnm_stack_push_int(p_world->get_world_no());
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_X)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_eye_x());
		else if (al_id == 1)	p_world->set_camera_eye_x(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_Y)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_eye_y());
		else if (al_id == 1)	p_world->set_camera_eye_y(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_Z)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_eye_z());
		else if (al_id == 1)	p_world->set_camera_eye_z(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_X)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_pint_x());
		else if (al_id == 1)	p_world->set_camera_pint_x(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_Y)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_pint_y());
		else if (al_id == 1)	p_world->set_camera_pint_y(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_Z)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_pint_z());
		else if (al_id == 1)	p_world->set_camera_pint_z(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_X)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_up_x());
		else if (al_id == 1)	p_world->set_camera_up_x(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_Y)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_up_y());
		else if (al_id == 1)	p_world->set_camera_up_y(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_Z)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_up_z());
		else if (al_id == 1)	p_world->set_camera_up_z(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_X_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_eye_x_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_Y_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_eye_y_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_EYE_Z_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_eye_z_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_X_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_pint_x_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_Y_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_pint_y_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_PINT_Z_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_pint_z_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_X_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_up_x_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_Y_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_up_y_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_UP_Z_EVE)	{
		tnm_command_proc_int_event(&p_world->camera_up_z_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_WORLD_CAMERA_VIEW_ANGLE)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_camera_view_angle());
		else if (al_id == 1)	p_world->set_camera_view_angle(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_SET_CAMERA_EYE)	{
		p_world->set_camera_eye(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CALC_CAMERA_EYE)	{
		double distance = (double)p_ai->al_begin[0].Int;
		double rotate_h = D3DXToRadian((double)p_ai->al_begin[1].Int / TNM_ANGLE_UNIT);
		double rotate_v = D3DXToRadian((double)p_ai->al_begin[2].Int / TNM_ANGLE_UNIT);

		p_world->set_camera_eye_x((int)(p_world->get_camera_pint_x() - distance * sin(rotate_h) * cos(rotate_v)));
		p_world->set_camera_eye_y((int)(p_world->get_camera_pint_y() + distance * sin(rotate_v)));
		p_world->set_camera_eye_z((int)(p_world->get_camera_pint_z() - distance * cos(rotate_h) * cos(rotate_v)));
	}
	else if (elm_begin[0] == ELM_WORLD_SET_CAMERA_PINT)	{
		p_world->set_camera_pint(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_CALC_CAMERA_PINT)	{
		// z 軸正の向きを見て時計回りに回転

		double distance = (double)p_ai->al_begin[0].Int;
		double rotate_h = D3DXToRadian((double)p_ai->al_begin[1].Int / TNM_ANGLE_UNIT);
		double rotate_v = D3DXToRadian((double)p_ai->al_begin[2].Int / TNM_ANGLE_UNIT);

		p_world->set_camera_pint_x((int)(p_world->get_camera_eye_x() + distance * sin(rotate_h) * cos(rotate_v)));
		p_world->set_camera_pint_y((int)(p_world->get_camera_eye_y() + distance * sin(rotate_v)));
		p_world->set_camera_pint_z((int)(p_world->get_camera_eye_z() + distance * cos(rotate_h) * cos(rotate_v)));
	}
	else if (elm_begin[0] == ELM_WORLD_SET_CAMERA_UP)	{
		p_world->set_camera_up(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_MONO)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_mono());
		else if (al_id == 1)	p_world->set_mono(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_SET_CAMERA_EVE_XZ_ROTATE)	{
		p_world->set_camera_eve_xz_rotate(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_ORDER)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_order());
		else if (al_id == 1)	p_world->set_order(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_LAYER)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_layer());
		else if (al_id == 1)	p_world->set_layer(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_WIPE_COPY)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_wipe_copy());
		else if (al_id == 1)	p_world->set_wipe_copy(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_WORLD_WIPE_ERASE)	{
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_world->get_wipe_erase());
		else if (al_id == 1)	p_world->set_wipe_erase(p_ai->al_begin[0].Int);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(world)"));
	}

	return true;
}

