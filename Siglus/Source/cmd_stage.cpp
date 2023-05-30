#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_stage.h"
#include	"engine/eng_input.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/flow_command.h"

// ****************************************************************
// コマンド処理：ステージリスト
// ================================================================
bool tnm_command_proc_stage_list(C_elm_stage_list* p_stage_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_stage_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_stage* p_stage = p_stage_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_stage)
			tnm_command_proc_stage(p_stage, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_stage_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：ステージ
// ================================================================
bool tnm_command_proc_stage(C_elm_stage* p_stage, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_stage;
	}
	else if (elm_begin[0] == ELM_STAGE_OBJECT)	{
		tnm_command_proc_object_list(&p_stage->m_object_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_MWND)	{
		tnm_command_proc_mwnd_list(&p_stage->m_mwnd_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_OBJBTNGROUP)	{
		tnm_command_proc_group_list(&p_stage->m_group_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_BTNSELITEM)	{
		tnm_command_proc_btnsel_item_list(&p_stage->m_btn_sel.m_item_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_WORLD)	{
		tnm_command_proc_world_list(&p_stage->m_world_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_EFFECT)	{
		tnm_command_proc_effect_list(&p_stage->m_effect_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_QUAKE)	{
		tnm_command_proc_quake_list(&p_stage->m_quake_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_STAGE_CREATE_OBJECT)	{
		p_stage->m_object_list.resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_STAGE_CREATE_MWND)	{
		p_stage->m_mwnd_list.resize(p_ai->al_begin[0].Int);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_stage->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：グループリスト
// ================================================================
bool tnm_command_proc_group_list(C_elm_group_list* p_group_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_group_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_group* p_group = p_group_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_group)
			tnm_command_proc_group(p_group, elm_begin[1], elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_GROUPLIST_ALLOC)	{
		p_group_list->clear();
		p_group_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_GROUPLIST_FREE)	{
		p_group_list->clear();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_group_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：グループ
// ================================================================
bool tnm_command_proc_group(C_elm_group* p_group, int group_no, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_group;
		return true;
	}

	switch (elm_begin[0])	{

		case ELM_GROUP_SEL:

			Gp_input->now.use();						// ★入力を消費する
			p_group->init_sel();						// 選択を初期化
			p_group->set_wait_flag(true);				// 選択が終了するまで待つ
			p_group->start();							// 選択開始
			tnm_push_proc(TNM_PROC_TYPE_SEL_BTN_OBJ);	// 状態変更
			Gp_local->cur_proc.element = S_element(elm_top, elm_end - 1);
			break;

		case ELM_GROUP_SEL_CANCEL:
		{
			int se_no = (p_ai->al_id > 0 ? p_ai->al_begin[0].Int : -1);

			Gp_input->now.use();						// ★入力を消費する
			p_group->init_sel();						// 選択を初期化
			p_group->set_wait_flag(true);				// 選択が終了するまで待つ
			p_group->set_cancel_flag(true);				// キャンセル可能
			p_group->set_cancel_se_no(se_no);			// キャンセル効果音
			p_group->start();							// 選択開始
			tnm_push_proc(TNM_PROC_TYPE_SEL_BTN_OBJ);	// 状態変更
			Gp_local->cur_proc.element = p_group->get_element();
			break;
		}

		case ELM_GROUP_INIT:
			p_group->reinit();							// 再初期化
			break;

		case ELM_GROUP_START:
			Gp_input->now.use();						// ★入力を消費する
			p_group->init_sel();						// 選択を初期化
			p_group->start();							// 選択開始
			break;

		case ELM_GROUP_START_CANCEL:
		{
			int se_no = (p_ai->al_id > 0 ? p_ai->al_begin[0].Int : -1);

			Gp_input->now.use();						// ★入力を消費する
			p_group->init_sel();						// 選択を初期化
			p_group->set_cancel_flag(true);				// キャンセル可能
			p_group->set_cancel_se_no(se_no);			// キャンセル効果音
			p_group->start();							// 選択開始
			break;
		}

		case ELM_GROUP_END:
			p_group->end();								// 選択終了（初期化はしない）
			break;

		case ELM_GROUP_GET_HIT_NO:
		{
			int res = p_group->get_hit_button_no();
			tnm_stack_push_int(res);
			break;
		}

		case ELM_GROUP_GET_PUSHED_NO:
		{
			int res = p_group->get_pushed_button_no();
			tnm_stack_push_int(res);
			break;
		}

		case ELM_GROUP_GET_DECIDED_NO:
		{
			int res = p_group->get_decided_button_no();
			tnm_stack_push_int(res);
			break;
		}

		case ELM_GROUP_GET_RESULT:
		{
			int res = p_group->get_result();
			tnm_stack_push_int(res);
			break;
		}

		case ELM_GROUP_GET_RESULT_BUTTON_NO:
		{
			int res = p_group->get_result_button_no();
			tnm_stack_push_int(res);
			break;
		}

		case ELM_GROUP_ORDER:
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(p_group->get_order());
			else if (al_id == 1)	p_group->set_order(p_ai->al_begin[0].Int);
			break;

		case ELM_GROUP_LAYER:
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(p_group->get_layer());
			else if (al_id == 1)	p_group->set_layer(p_ai->al_begin[0].Int);
			break;

		case ELM_GROUP_CANCEL_PRIORITY:
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(p_group->get_cancel_priority());
			else if (al_id == 1)	p_group->set_cancel_priority(p_ai->al_begin[0].Int);
			break;

		default:
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_group->get_element_name());
			break;
	}

	return true;
}


