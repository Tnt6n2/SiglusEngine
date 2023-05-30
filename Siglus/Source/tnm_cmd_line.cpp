#include	"pch.h"
#include	"data/tnm_cmd_line.h"

// ****************************************************************
// コマンドライン
// ================================================================
C_tnm_cmd_line::C_tnm_cmd_line()
{
	select_scene_pck_set = false;
	start_scene_set = false;
	start_z_no_set = false;
	start_z_no = 0;
	debug_on_set = false;
	debug_off_set = false;
	work_dir_set = false;
	no_interval_set = false;
	end_start_set = false;
	log_set = false;
	window_mode_set = false;
	fullscreen_mode_set = false;
	not_disp_overflow_error_set = false;
	multi_user_id_set = false;
	double_start_set = false;
}

// ****************************************************************
// コマンドラインを解析
// ================================================================
bool C_tnm_cmd_line::analize_command_line()
{
	ARRAY<TSTR> pair_list;
	TSTR::iterator itr_pair_begin = G_app.command_line.begin();

	enum STATE
	{
		STATE_NORMAL,
		STATE_QUOTE,
		STATE_QUOTE_ESCAPE,
	}	state = STATE_NORMAL;

	// スペースで分ける
	for (TSTR::iterator itr = G_app.command_line.begin(); ; ++itr)	{

		// STATE_NORMAL 以外でコマンドラインが終了した！
		if (itr == G_app.command_line.end() && state != STATE_NORMAL)	{
			return false;
		}
		else if (state == STATE_QUOTE)	{

			if (false);
			else if (*itr == _T('"'))	{
				state = STATE_NORMAL;
			}
			else if (*itr == _T('\\'))	{
				state = STATE_QUOTE_ESCAPE;
			}
		}
		else if (state == STATE_QUOTE_ESCAPE)	{
			state = STATE_QUOTE;
		}
		else if (state == STATE_NORMAL)	{

			if (itr == G_app.command_line.end() || *itr == _T(' '))	{
				TSTR str;
				int num;

				TSTR pair = str_trim_ret(TSTR(itr_pair_begin, itr), _T(' '));

				// ペアを解析
				C_str_analizer anl(pair.begin(), pair.end());

				if (false);
				else if (anl.get_format(_T("/select=%s"), &str))	{
					select_scene_pck_set = true;
					select_scene_pck = str;
				}
				else if (anl.get_format(_T("/start=%s"), &str))	{
					start_scene_set = true;
					start_scene = str;
				}
				else if (anl.get_format(_T("/z_no=%d"), &num))	{
					start_z_no_set = true;
					start_z_no = num;
				}
				else if (anl.get_format(_T("/debug_on")))	{
					debug_on_set = true;
				}
				else if (anl.get_format(_T("/debug_off")))	{
					debug_off_set = true;
				}
				else if (anl.get_format(_T("/dp=%s"), &str))	{
					debug_password = str;
				}
				else if (anl.get_format(_T("/dk=%s"), &str))	{
					debug_key = str;
				}
				else if (anl.get_format(_T("/work_dir=%s"), &str))	{
					work_dir_set = true;
					work_dir = str;
				}
				else if (anl.get_format(_T("/no_interval")))	{
					no_interval_set = true;
				}
				else if (anl.get_format(_T("/end_start")))	{
					end_start_set = true;
				}
				else if (anl.get_format(_T("/log")))	{
					log_set = true;
				}
				else if (anl.get_format(_T("/window")))	{
					window_mode_set = true;
				}
				else if (anl.get_format(_T("/fullscreen")))	{
					fullscreen_mode_set = true;
				}
				else if (anl.get_format(_T("/overflow_error_off")))	{
					not_disp_overflow_error_set = true;
				}
				else if (anl.get_format(_T("/user_id=%s"), &str))	{
					multi_user_id_set = true;
					multi_user_id = str;
				}
				else if (anl.get_format(_T("/double_start")))	{
					double_start_set = true;
				}

				// 開始位置を設定
				itr_pair_begin = itr;
			}
			else if (*itr == _T('"'))	{
				state = STATE_QUOTE;
			}
			else if (*itr == _T('\\'))	{
				state = STATE_QUOTE_ESCAPE;
			}
		}

		if (itr == G_app.command_line.end())
			break;
	}

	return true;
}