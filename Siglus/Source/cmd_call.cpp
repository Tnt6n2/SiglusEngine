#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"element/elm_call.h"
#include	"element/elm_excall.h"
#include	"engine/flow_command.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// コマンド処理：コールリスト
// ================================================================
bool tnm_command_proc_call_list(C_elm_call_list* p_call_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)
	{
		p_ai->p_ret_element = p_call_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)
	{
		C_elm_call* p_call = p_call_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_call)
			tnm_command_proc_call(p_call, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。（calllist）"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：コール
// ================================================================
bool tnm_command_proc_call(C_elm_call* p_call, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)
	{
		p_ai->p_ret_element = p_call;
	}
	else if (elm_begin[0] == ELM_CALL_L)
	{
		tnm_command_proc_int_list(&tnm_get_cur_call()->L, 32, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_CALL_K)
	{
		tnm_command_proc_str_list(&tnm_get_cur_call()->K, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else
	{
		if (false);

		// ユーザコールプロパティ
		else if (get_elm_owner(elm_begin[0]) == ELM_OWNER_CALL_PROP)
		{
			tnm_command_proc_call_prop(get_elm_code(elm_begin[0]), elm_top, elm_begin + 1, elm_end, p_ai);
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(call)"));
		}
	}

	return true;
}

// ****************************************************************
// コマンド処理：ＥＸコール
// ================================================================
bool tnm_command_proc_excall(C_elm_excall* p_excall, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)
	{
		p_ai->p_ret_element = p_excall;
	}
	else if (elm_begin[0] == ELM_ARRAY)
	{
		if (elm_begin[1] == 0)
		{
			return tnm_command_proc_excall(NULL, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else if (elm_begin[1] == 1)
		{
			return tnm_command_proc_excall(Gp_excall, elm_top, elm_begin + 2, elm_end, p_ai);
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("excall[] のインデックスが範囲外です。"));
		}
	}
	else if (elm_begin[0] == ELM_EXCALL_ALLOC)	{
		p_excall->ready();
	}
	else if (elm_begin[0] == ELM_EXCALL_FREE)	{
		p_excall->free();
	}
	else if (elm_begin[0] == ELM_EXCALL_IS_EXCALL)	{
		tnm_stack_push_int(tnm_excall_is_excall() ? 1 : 0);
	}
	else if (elm_begin[0] == ELM_EXCALL_CHECK_ALLOC)	{
		tnm_stack_push_int(p_excall->is_ready() ? 1 : 0);
	}
	else if (p_excall && !p_excall->is_ready())	{
		if (p_ai->disp_out_of_range_error)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("システムコールが準備されていません！"));
		}
	}
	else if (elm_begin[0] == ELM_EXCALL_F)	{
		tnm_command_proc_int_list(p_excall ? &p_excall->m_F : &Gp_flag->F, 32, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_COUNTER)	{
		tnm_command_proc_counter_list(p_excall ? &p_excall->m_counter_list : Gp_counter_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_FRAME_ACTION)	{
		tnm_command_proc_frame_action(p_excall ? &p_excall->m_frame_action : Gp_frame_action, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_FRAME_ACTION_CH)	{
		tnm_command_proc_frame_action_list(p_excall ? &p_excall->m_frame_action_ch_list : Gp_frame_action_ch, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_STAGE)	{
		tnm_command_proc_stage_list(p_excall ? &p_excall->m_stage_list : Gp_stage_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_BACK)	{
		tnm_command_proc_stage(p_excall ? &p_excall->m_stage_list[TNM_STAGE_BACK] : &(*Gp_stage_list)[TNM_STAGE_BACK], elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_FRONT)	{
		tnm_command_proc_stage(p_excall ? &p_excall->m_stage_list[TNM_STAGE_FRONT] : &(*Gp_stage_list)[TNM_STAGE_FRONT], elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_NEXT)	{
		tnm_command_proc_stage(p_excall ? &p_excall->m_stage_list[TNM_STAGE_NEXT] : &(*Gp_stage_list)[TNM_STAGE_NEXT], elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_EXCALL_SCRIPT)	{
		tnm_command_proc_script_excall(elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(excall)"));
	}

	return true;
}
