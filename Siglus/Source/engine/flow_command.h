#pragma		once

#include	"tnm_code.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"

class	C_tnm_element;
class	C_tnm_prop;
class	C_tnm_prop_list;
class	C_elm_int_list;
class	C_elm_int_event;
class	C_elm_int_event_list;
class	C_elm_str_list;
class	C_elm_database;
class	C_elm_database_list;
class	C_elm_counter;
class	C_elm_counter_list;
class	C_elm_g00_buf;
class	C_elm_g00_buf_list;
class	C_elm_mask;
class	C_elm_mask_list;
class	C_elm_screen;
class	C_elm_world;
class	C_elm_world_list;
class	C_elm_stage;
class	C_elm_stage_list;
class	C_elm_object;
class	C_elm_object_list;
class	C_elm_mwnd;
class	C_elm_mwnd_list;
class	C_elm_group;
class	C_elm_group_list;
class	C_elm_btn_select_item;
class	C_elm_btn_select_item_list;
class	C_elm_effect;
class	C_elm_effect_list;
class	C_elm_quake;
class	C_elm_quake_list;
class	C_elm_frame_action;
class	C_elm_frame_action_list;
class	C_elm_bgm;
class	C_elm_koe;
class	C_elm_pcm;
class	C_elm_pcmch;
class	C_elm_pcmch_list;
class	C_elm_pcm_event;
class	C_elm_pcm_event_list;
class	C_elm_se;
class	C_elm_mov;
class	C_elm_editbox;
class	C_elm_editbox_list;
class	C_elm_call;
class	C_elm_call_list;
class	C_elm_excall;
class	C_tnm_cg_table;

// ****************************************************************
// エンジン：コマンド処理：構造体
// ================================================================
struct S_tnm_command_proc_arg_struct
{
	int				al_id;				// 引数リストID
	C_tnm_prop*		al_begin;			// 引数リスト開始点
	C_tnm_prop*		al_end;				// 引数リスト終了点
	C_tnm_prop*		named_al_end;		// 名前つき引数リスト終了点
	int				ret_form_code;		// 戻り値の型
	C_tnm_element*	p_ret_element;		// 戻す実エレメントへのポインタ
	bool			disp_out_of_range_error;	// 範囲外エラーを表示する

	S_tnm_command_proc_arg_struct()
	{
		al_id = 0;
		al_begin = NULL;
		al_end = NULL;
		named_al_end = NULL;
		ret_form_code = FM_VOID;
		p_ret_element = NULL;
		disp_out_of_range_error = true;
	}
};

// ****************************************************************
// エンジン：コマンド処理
// ================================================================

// パラメータを直接指定する
bool	tnm_command_proc(int* elm_begin, int* elm_end);
bool	tnm_command_proc(int* elm_begin, int* elm_end, int al_id);
bool	tnm_command_proc(int* elm_begin, int* elm_end, int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, C_tnm_prop* named_al_end, int ret_form_code);

// パラメータを構造体で指定する
bool	tnm_command_proc(int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_int_list(C_elm_int_list* p_int_list, int bit, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_int_list(C_elm_int_event_list* p_int_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_int_event(C_elm_int_event* p_int_event, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_int_event_list(C_elm_int_event_list* p_int_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_str_list(C_elm_str_list* p_str_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_str(TSTR& str, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_math(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_cg_table(C_tnm_cg_table* p_cg_table, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_bgm_table(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_database_list(C_elm_database_list* p_database_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_database(C_elm_database* p_database, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_counter_list(C_elm_counter_list* p_counter_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_counter(C_elm_counter* p_counter, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_frame_action_list(C_elm_frame_action_list* p_frame_action_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_frame_action(C_elm_frame_action* p_frame_action, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_g00_buf_list(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_g00_buf(C_elm_g00_buf* p_g00_buf, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mask_list(C_elm_mask_list* p_mask_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mask(C_elm_mask* p_mask, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_world_list(C_elm_world_list* p_world_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_world(C_elm_world* p_world, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_msgbk(C_elm_msg_back* p_msgbk, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_stage_list(C_elm_stage_list* p_stage_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_stage(C_elm_stage* p_stage, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_object_list(C_elm_object_list* p_object_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_object(C_elm_object* p_object, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_object_event(C_elm_object* p_object, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mwnd_list(C_elm_mwnd_list* p_mwnd_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mwnd(C_elm_mwnd* p_mwnd, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_group_list(C_elm_group_list* p_group_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_group(C_elm_group* p_group, int group_no, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_btnsel_item_list(C_elm_btn_select_item_list* p_item_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_btnsel_item(C_elm_btn_select_item* p_item, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_screen(C_elm_screen* p_effect, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_effect_list(C_elm_effect_list* p_effect_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_effect(C_elm_effect* p_effect, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_quake_list(C_elm_quake_list* p_quake_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_quake(C_elm_quake* p_quake, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_wipe(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_wait_wipe(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_editbox_list(C_elm_editbox_list* editbox_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_editbox(C_elm_editbox* p_editbox, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_bgm(C_elm_bgm* p_bgm, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_koe(C_elm_koe* p_koe, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_ex_koe(C_elm_koe* p_koe, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_pcm(C_elm_pcm* p_pcm, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_pcmch_list(C_elm_pcmch_list* p_pcmch_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_pcmch(C_elm_pcmch* p_pcmch, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_pcm_event_list(C_elm_pcm_event_list* p_pcm_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_pcm_event(C_elm_pcm_event* p_pcm_event, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_se(C_elm_pcm* p_se, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mov(C_elm_mov* p_mov, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_input(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_mouse(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_key_list(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_key(int key_no, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_script(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_script_excall(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_syscom(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_system(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_call_list(C_elm_call_list* p_call_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_call(C_elm_call* p_call, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_excall(C_elm_excall* p_excall, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_steam(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_file(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_user_cmd(int user_cmd_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_user_prop(int user_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_call_prop(int call_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_prop(C_tnm_prop* p_prop, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai);
bool	tnm_command_proc_gosub(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list, int ret_form_code);
bool	tnm_command_proc_farcall(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code);
bool	tnm_command_proc_farcall_ex(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code);
bool	tnm_command_proc_return(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list);

// 整数を代入する
bool	tnm_command_proc_set_int(int* elm_begin, int* elm_end, int value);

