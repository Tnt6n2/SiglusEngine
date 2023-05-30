#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_wipe.h"
#include	"element/elm_screen.h"
#include	"element/elm_excall.h"
#include	"engine/flow_command.h"

// ****************************************************************
// コマンド処理：ワイプ
// ================================================================
bool tnm_command_proc_wipe(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	int wipe_type = 0;
	int wipe_time = 500, speed_mode = 0, start_time = 0;
	int option[TNM_WIPE_OPTION_MAX] = {0};
	int wipe_range = Gp_excall->is_ready() ? TNM_WIPE_RANGE_EX_CALL : TNM_WIPE_RANGE_NORMAL;
	int begin_order = 0, end_order = 0, begin_layer = INT_MIN, end_layer = INT_MAX;
	bool wait_flag = true;
	int key_wait_mode = -1;		// キースキップ：環境設定に従う
	int with_low_order = 0;
	TSTR mask_file;

	// コマンドによってパラメータを変更
	if (elm_begin[0] == ELM_GLOBAL_WIPE_ALL || elm_begin[0] == ELM_GLOBAL_MASK_WIPE_ALL)	{
		end_order = INT_MAX;	// 全てワイプ
	}

	// 名前なし引数
	if (false);
	else if (elm_begin[0] == ELM_GLOBAL_WIPE || elm_begin[0] == ELM_GLOBAL_WIPE_ALL)	{

		switch (al_id)	{
			default:
			case 3:		speed_mode = p_ai->al_begin[2].Int;	// speed_mode
			case 2:		wipe_time = p_ai->al_begin[1].Int;	// time
			case 1:		wipe_type = p_ai->al_begin[0].Int;	// type
			case 0:		;
		}
		if (al_id >= 3)	{									// option
			for (int e = 0; e < (int)p_ai->al_begin[3].exp_cnt; e++)	{
				option[e] = p_ai->al_begin[3].exp_list[e].Int;
			}
		}
	}
	else if (elm_begin[0] == ELM_GLOBAL_MASK_WIPE || elm_begin[0] == ELM_GLOBAL_MASK_WIPE_ALL)	{

		switch (al_id)	{
			default:
			case 3:		speed_mode = p_ai->al_begin[3].Int;
			case 2:		wipe_time = p_ai->al_begin[2].Int;
			case 1:		wipe_type = p_ai->al_begin[1].Int;
			case 0:		mask_file = p_ai->al_begin[0].str;
		}
		if (al_id >= 3)	{
			for (int a = 0; a < (int)p_ai->al_begin[4].exp_cnt; a++)	{
				option[a] = p_ai->al_begin[4].exp_list[a].Int;
			}
		}
	}

	// 名前つき引数
	for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
		switch (arg->id)	{
			case 0:		wipe_type = arg->Int;	break;
			case 1:		wipe_time = arg->Int;	break;
			case 2:		speed_mode = arg->Int;	break;
			case 3:
				for (int e = 0; e < (int)arg->exp_cnt; e++)	{
					option[e] = arg->exp_list[e].Int;
				}
				break;
			case 4:		begin_order = arg->Int;					break;
			case 5:		end_order = arg->Int;					break;
			case 6:		begin_layer = arg->Int;					break;
			case 7:		end_layer = arg->Int;					break;
			case 8:		wait_flag = arg->Int ? true : false;	break;
			case 9:		key_wait_mode = arg->Int;				break;
			case 10:	with_low_order = arg->Int;				break;
			case 11:	start_time = arg->Int;					break;
		}
	}

	// ワイプコマンドは値を返さない
	bool return_value_flag = false;
	
	if (false);
	else if (elm_begin[0] == ELM_GLOBAL_WIPE || elm_begin[0] == ELM_GLOBAL_WIPE_ALL)	{
		Gp_wipe->start(S_tnm_sorter(begin_order, begin_layer), S_tnm_sorter(end_order, end_layer), wipe_range, with_low_order
			, wipe_type, wipe_time, start_time, speed_mode, option, wait_flag, key_wait_mode, return_value_flag, true);
	}
	else if (elm_begin[0] == ELM_GLOBAL_MASK_WIPE || elm_begin[0] == ELM_GLOBAL_MASK_WIPE_ALL)	{
		Gp_wipe->start_mask(mask_file, S_tnm_sorter(begin_order, begin_layer), S_tnm_sorter(end_order, end_layer), wipe_range, with_low_order
			, wipe_type, wipe_time, start_time, speed_mode, option, wait_flag, key_wait_mode, return_value_flag, true);
	}

	return true;
}

// ****************************************************************
// コマンド処理：ワイプ待ち
// ================================================================
bool tnm_command_proc_wait_wipe(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int key_wait_mode = -1;		// キースキップ：環境設定に従う

	// 名前つき引数
	for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
		switch (arg->id)	{
			case 0:		key_wait_mode = arg->Int;	break;
		}
	}

	// ワイプ待ち
	Gp_wipe->wait(key_wait_mode, true);		// 戻り値：返す

	return true;
}
