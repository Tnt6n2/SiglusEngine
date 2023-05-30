#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_prop.h"
#include	"element/elm_excall.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/flow_command.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// コマンド処理：スクリプト
// ================================================================
bool tnm_command_proc_script(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	switch (elm_begin[0])
	{
	case ELM_SCRIPT_SET_AUTO_SAVEPOINT_OFF:
		Gp_local->pod.dont_set_save_point = true;
		break;
	case ELM_SCRIPT_SET_AUTO_SAVEPOINT_ON:
		Gp_local->pod.dont_set_save_point = false;
		break;
	case ELM_SCRIPT_SET_SKIP_DISABLE:
		Gp_local->pod.skip_disable = true;
		break;
	case ELM_SCRIPT_SET_SKIP_ENABLE:
		Gp_local->pod.skip_disable = false;
		break;
	case ELM_SCRIPT_SET_SKIP_DISABLE_FLAG:
		Gp_local->pod.skip_disable = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_SKIP_DISABLE_FLAG:
		tnm_stack_push_int(Gp_local->pod.skip_disable ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_CTRL_SKIP_DISABLE:
		Gp_local->pod.ctrl_disable = true;
		break;
	case ELM_SCRIPT_SET_CTRL_SKIP_ENABLE:
		Gp_local->pod.ctrl_disable = false;
		break;
	case ELM_SCRIPT_SET_CTRL_SKIP_DISABLE_FLAG:
		Gp_local->pod.ctrl_disable = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_CTRL_SKIP_DISABLE_FLAG:
		tnm_stack_push_int(Gp_local->pod.ctrl_disable ? 1 : 0);
		break;
	case ELM_SCRIPT_CHECK_SKIP:
		tnm_stack_push_int(tnm_is_skipping() ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_STOP_SKIP_BY_KEY_DISABLE:
		Gp_local->pod.not_stop_skip_by_click = true;
		break;
	case ELM_SCRIPT_SET_STOP_SKIP_BY_KEY_ENABLE:
		Gp_local->pod.not_stop_skip_by_click = false;
		break;
	case ELM_SCRIPT_SET_END_MSG_BY_KEY_DISABLE:
		Gp_local->pod.not_skip_msg_by_click = true;
		break;
	case ELM_SCRIPT_SET_END_MSG_BY_KEY_ENABLE:
		Gp_local->pod.not_skip_msg_by_click = false;
		break;
	case ELM_SCRIPT_SET_SKIP_UNREAD_MESSAGE_FLAG:
		Gp_local->pod.skip_unread_message = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_SKIP_UNREAD_MESSAGE_FLAG:
		tnm_stack_push_int(Gp_local->pod.skip_unread_message ? 1 : 0);
		break;
	case ELM_SCRIPT_START_AUTO_MODE:
		Gp_local->pod.auto_mode_flag = true;
		break;
	case ELM_SCRIPT_END_AUTO_MODE:
		Gp_local->pod.auto_mode_flag = false;
		break;
	case ELM_SCRIPT_SET_AUTO_MODE_MOJI_WAIT:
		Gp_local->pod.auto_mode_moji_wait = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_AUTO_MODE_MOJI_WAIT_DEFAULT:
		Gp_local->pod.auto_mode_moji_wait = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_AUTO_MODE_MOJI_WAIT:
		tnm_stack_push_int(Gp_local->pod.auto_mode_moji_wait);
		break;
	case ELM_SCRIPT_SET_AUTO_MODE_MIN_WAIT:
		Gp_local->pod.auto_mode_min_wait = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_AUTO_MODE_MIN_WAIT_DEFAULT:
		Gp_local->pod.auto_mode_min_wait = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_AUTO_MODE_MIN_WAIT:
		tnm_stack_push_int(Gp_local->pod.auto_mode_min_wait);
		break;
	case ELM_SCRIPT_SET_AUTO_MODE_MOJI_CNT:
		Gp_local->pod.auto_mode_moji_cnt = p_ai->al_begin[0].Int;
		break;

	case ELM_SCRIPT_SET_MOUSE_CURSOR_HIDE_ONOFF:
		Gp_local->pod.mouse_cursor_hide_onoff = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_MOUSE_CURSOR_HIDE_ONOFF_DEFAULT:
		Gp_local->pod.mouse_cursor_hide_onoff = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_MOUSE_CURSOR_HIDE_ONOFF:
		tnm_stack_push_int(Gp_local->pod.mouse_cursor_hide_onoff);
		break;

	case ELM_SCRIPT_SET_MOUSE_CURSOR_HIDE_TIME:
		Gp_local->pod.mouse_cursor_hide_time = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_MOUSE_CURSOR_HIDE_TIME_DEFAULT:
		Gp_local->pod.mouse_cursor_hide_time = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_MOUSE_CURSOR_HIDE_TIME:
		tnm_stack_push_int(Gp_local->pod.mouse_cursor_hide_time);
		break;

	case ELM_SCRIPT_SET_MESSAGE_SPEED:
		Gp_local->pod.msg_speed = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_MESSAGE_SPEED_DEFAULT:
		Gp_local->pod.msg_speed = -1;				// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_MESSAGE_SPEED:
		tnm_stack_push_int(Gp_local->pod.msg_speed);
		break;
	case ELM_SCRIPT_SET_MESSAGE_NOWAIT_FLAG:
		Gp_local->pod.msg_nowait = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_MESSAGE_NOWAIT_FLAG:
		tnm_stack_push_int(Gp_local->pod.msg_nowait ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_MSG_ASYNC_MODE_ON:
		Gp_local->pod.async_msg_mode = true;
		Gp_local->pod.async_msg_mode_once = false;
		break;
	case ELM_SCRIPT_SET_MSG_ASYNC_MODE_ON_ONCE:
		Gp_local->pod.async_msg_mode = true;
		Gp_local->pod.async_msg_mode_once = true;
		break;
	case ELM_SCRIPT_SET_MSG_ASYNC_MODE_OFF:
		Gp_local->pod.async_msg_mode = false;
		Gp_local->pod.async_msg_mode_once = false;
		break;
	case ELM_SCRIPT_SET_HIDE_MWND_DISABLE:
		Gp_local->pod.hide_mwnd_disable = true;
		break;
	case ELM_SCRIPT_SET_HIDE_MWND_ENABLE:
		Gp_local->pod.hide_mwnd_disable = false;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_DISABLE:
		Gp_local->pod.msg_back_disable = true;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_ENABLE:
		Gp_local->pod.msg_back_disable = false;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_OFF:
		Gp_local->pod.msg_back_off = true;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_ON:
		Gp_local->pod.msg_back_off = false;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_DISP_OFF:
		Gp_local->pod.msg_back_disp_off = true;
		break;
	case ELM_SCRIPT_SET_MSG_BACK_DISP_ON:
		Gp_local->pod.msg_back_disp_off = false;
		break;
	case ELM_SCRIPT_SET_MOUSE_DISP_OFF:
		Gp_local->pod.cursor_disp_off = true;
		break;
	case ELM_SCRIPT_SET_MOUSE_DISP_ON:
		Gp_local->pod.cursor_disp_off = false;
		break;
	case ELM_SCRIPT_SET_MOUSE_MOVE_BY_KEY_DISABLE:
		Gp_local->pod.cursor_move_by_key_disable = true;
		break;
	case ELM_SCRIPT_SET_MOUSE_MOVE_BY_KEY_ENABLE:
		Gp_local->pod.cursor_move_by_key_disable = false;
		break;
	case ELM_SCRIPT_SET_KEY_DISABLE:
		if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < 256)	{
			Gp_local->pod.key_disable[p_ai->al_begin[0].Int] = true;
		}
		break;
	case ELM_SCRIPT_SET_KEY_ENABLE:
		if (0 <= p_ai->al_begin[0].Int && p_ai->al_begin[0].Int < 256)	{
			Gp_local->pod.key_disable[p_ai->al_begin[0].Int] = false;
		}
		break;
	case ELM_SCRIPT_SET_MWND_ANIME_OFF_FLAG:
		Gp_local->pod.mwnd_anime_off_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_MWND_ANIME_OFF_FLAG:
		tnm_stack_push_int(Gp_local->pod.mwnd_anime_off_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_MWND_ANIME_ON_FLAG:
		Gp_local->pod.mwnd_anime_on_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_MWND_ANIME_ON_FLAG:
		tnm_stack_push_int(Gp_local->pod.mwnd_anime_on_flag ? 1 : 0);
		break;

	case ELM_SCRIPT_SET_MWND_DISP_OFF_FLAG:
		Gp_local->pod.mwnd_disp_off_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_MWND_DISP_OFF_FLAG:
		tnm_stack_push_int(Gp_local->pod.mwnd_disp_off_flag ? 1 : 0);
		break;

	case ELM_SCRIPT_SET_KOE_DONT_STOP_ON_FLAG:
		Gp_local->pod.koe_dont_stop_on_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_KOE_DONT_STOP_ON_FLAG:
		tnm_stack_push_int(Gp_local->pod.koe_dont_stop_on_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_KOE_DONT_STOP_OFF_FLAG:
		Gp_local->pod.koe_dont_stop_off_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_KOE_DONT_STOP_OFF_FLAG:
		tnm_stack_push_int(Gp_local->pod.koe_dont_stop_off_flag ? 1 : 0);
		break;

	case ELM_SCRIPT_SET_SHORTCUT_ENABLE:
		Gp_local->pod.shortcut_disable = false;
		break;
	case ELM_SCRIPT_SET_SHORTCUT_DISABLE:
		Gp_local->pod.shortcut_disable = true;
		break;
	case ELM_SCRIPT_SET_QUAKE_STOP_FLAG:
		Gp_local->pod.quake_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_QUAKE_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.quake_stop_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_EMOTE_MOUTH_STOP_FLAG:
		Gp_local->pod.emote_mouth_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_EMOTE_MOUTH_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.emote_mouth_stop_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_START_BGMFADE:
		Gp_local->pod.bgmfade_flag = true;
		break;
	case ELM_SCRIPT_END_BGMFADE:
		Gp_local->pod.bgmfade_flag = false;
		break;
	case ELM_SCRIPT_SET_VSYNC_WAIT_OFF_FLAG:
		Gp_local->pod.wait_display_vsync_off_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_VSYNC_WAIT_OFF_FLAG:
		tnm_stack_push_int(Gp_local->pod.wait_display_vsync_off_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_SKIP_TRIGGER:
		Gp_local->pod.skip_trigger = true;
		break;
	case ELM_SCRIPT_IGNORE_R_ON:
		Gp_local->pod.ignore_r_flag = true;
		break;
	case ELM_SCRIPT_IGNORE_R_OFF:
		Gp_local->pod.ignore_r_flag = false;
		break;
	case ELM_SCRIPT_SET_CURSOR_NO:
		Gp_local->pod.cursor_no = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_GET_CURSOR_NO:
		tnm_stack_push_int(Gp_local->pod.cursor_no);
		break;
	case ELM_SCRIPT_SET_TIME_STOP_FLAG:
		Gp_local->pod.time_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_TIME_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.time_stop_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_COUNTER_TIME_STOP_FLAG:
		Gp_local->pod.counter_time_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_COUNTER_TIME_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.counter_time_stop_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_FRAME_ACTION_TIME_STOP_FLAG:
		Gp_local->pod.frame_action_time_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_FRAME_ACTION_TIME_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.frame_action_time_stop_flag ? 1 : 0);
		break;
	case ELM_SCRIPT_SET_STAGE_TIME_STOP_FLAG:
		Gp_local->pod.stage_time_stop_flag = p_ai->al_begin[0].Int ? true : false;
		break;
	case ELM_SCRIPT_GET_STAGE_TIME_STOP_FLAG:
		tnm_stack_push_int(Gp_local->pod.stage_time_stop_flag ? 1 : 0);
		break;

	case ELM_SCRIPT_SET_FONT_NAME:
		Gp_local->font_name = p_ai->al_begin[0].str;
		break;
	case ELM_SCRIPT_SET_FONT_NAME_DEFAULT:
		Gp_local->font_name.clear();		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_NAME:
		tnm_stack_push_str(Gp_local->font_name);
		break;
	case ELM_SCRIPT_SET_FONT_BOLD:
		Gp_local->pod.font_futoku = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_FONT_BOLD_DEFAULT:
		Gp_local->pod.font_futoku = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_BOLD:
		tnm_stack_push_int(Gp_local->pod.font_futoku);
		break;
	case ELM_SCRIPT_SET_FONT_SHADOW:
		Gp_local->pod.font_shadow = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_FONT_SHADOW_DEFAULT:
		Gp_local->pod.font_shadow = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_SHADOW:
		tnm_stack_push_int(Gp_local->pod.font_shadow);
		break;

	default:
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(script)"));
		break;
	}

	return true;
}

bool tnm_command_proc_script_excall(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	switch (elm_begin[0])
	{
	case ELM_SCRIPT_SET_FONT_NAME:
		Gp_excall->m_font_name = p_ai->al_begin[0].str;
		break;
	case ELM_SCRIPT_SET_FONT_NAME_DEFAULT:
		Gp_excall->m_font_name.clear();		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_NAME:
		tnm_stack_push_str(Gp_excall->m_font_name);
		break;
	case ELM_SCRIPT_SET_FONT_BOLD:
		Gp_excall->m_pod.font_futoku = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_FONT_BOLD_DEFAULT:
		Gp_excall->m_pod.font_futoku = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_BOLD:
		tnm_stack_push_int(Gp_excall->m_pod.font_futoku);
		break;
	case ELM_SCRIPT_SET_FONT_SHADOW:
		Gp_excall->m_pod.font_shadow = p_ai->al_begin[0].Int;
		break;
	case ELM_SCRIPT_SET_FONT_SHADOW_DEFAULT:
		Gp_excall->m_pod.font_shadow = -1;		// 環境設定に従う
		break;
	case ELM_SCRIPT_GET_FONT_SHADOW:
		tnm_stack_push_int(Gp_excall->m_pod.font_shadow);
		break;

	default:
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(script)"));
		break;
	}

	return true;
}

