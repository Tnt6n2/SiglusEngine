#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_prop.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"
#include	"engine/eng_etc.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// コマンド処理：BGM
// ================================================================
bool tnm_command_proc_bgm(C_elm_bgm* p_bgm, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_bgm;
	}
	else if (elm_begin[0] == ELM_BGM_PLAY)	{
		TSTR regist_name;
		bool loop_flag = true;
		bool wait_flag = false;
		int fade_in_time = 0;
		int fade_out_time = 0;
		int start_pos = TNM_BGM_START_POS_INI;

		switch (al_id)	{	// no break!!
			case 2:		fade_out_time = p_ai->al_begin[2].Int;
			case 1:		fade_in_time = p_ai->al_begin[1].Int;
			case 0:		regist_name = p_ai->al_begin[0].str;
		}

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		regist_name = arg->str;					break;
				case 1:		loop_flag = arg->Int ? true : false;	break;
				case 2:		wait_flag = arg->Int ? true : false;	break;
				case 3:		start_pos = arg->Int;					break;
				case 4:		fade_in_time = arg->Int;				break;
				case 5:		fade_out_time = arg->Int;				break;
			}
		}

		if (false);
		else if (wait_flag)	p_bgm->play_wait(regist_name, fade_in_time, fade_out_time, start_pos, false, 0);
		else if (loop_flag)	p_bgm->play(regist_name, fade_in_time, fade_out_time, start_pos, false, 0);
		else				p_bgm->play_oneshot(regist_name, fade_in_time, fade_out_time, start_pos, false, 0);	
	}
	else if (elm_begin[0] == ELM_BGM_PLAY_ONESHOT)	{
		if (false);
		else if (al_id == 0)	p_bgm->play_oneshot(p_ai->al_begin[0].str, 0, 0, TNM_BGM_START_POS_INI, false, 0);
		else if (al_id == 1)	p_bgm->play_oneshot(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, 0, TNM_BGM_START_POS_INI, false, 0);
		else if (al_id == 2)	p_bgm->play_oneshot(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, TNM_BGM_START_POS_INI, false, 0);
	}
	else if (elm_begin[0] == ELM_BGM_PLAY_WAIT)	{
		if (false);
		else if (al_id == 0)	p_bgm->play_wait(p_ai->al_begin[0].str, 0, 0, TNM_BGM_START_POS_INI, false, 0);
		else if (al_id == 1)	p_bgm->play_wait(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, 0, TNM_BGM_START_POS_INI, false, 0);
		else if (al_id == 2)	p_bgm->play_wait(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, TNM_BGM_START_POS_INI, false, 0);
	}
	else if (elm_begin[0] == ELM_BGM_READY)	{
		TSTR regist_name;
		bool loop_flag = true;
		int fade_out_time = 100;
		int start_pos = TNM_BGM_START_POS_INI;

		switch (al_id)	{	// no break!!
			case 2:		fade_out_time = p_ai->al_begin[1].Int;
			case 0:		regist_name = p_ai->al_begin[0].str;
		}

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		regist_name = arg->str;					break;
				case 1:		loop_flag = arg->Int ? true : false;	break;
				// case 2: 欠番（wait）
				case 3:		start_pos = arg->Int;					break;
				// case 4: 欠番（fade_in_time）
				case 5:		fade_out_time = arg->Int;				break;
			}
		}

		if (false);
		else if (loop_flag)	p_bgm->play(regist_name, 0, fade_out_time, start_pos, true, 0);
		else				p_bgm->play_oneshot(regist_name, 0, fade_out_time, start_pos, true, 0);	
	}
	else if (elm_begin[0] == ELM_BGM_READY_ONESHOT)	{
		if (false);
		else if (al_id == 0)	p_bgm->play_oneshot(p_ai->al_begin[0].str, 0, 0, TNM_BGM_START_POS_INI, true, 0);
		else if (al_id == 1)	p_bgm->play_oneshot(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, 0, TNM_BGM_START_POS_INI, true, 0);
		else if (al_id == 2)	p_bgm->play_oneshot(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, TNM_BGM_START_POS_INI, true, 0);
	}
	else if (elm_begin[0] == ELM_BGM_STOP)	{
		if (false);
		else if (al_id == 0)	p_bgm->stop();
		else if (al_id == 1)	p_bgm->stop(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_BGM_PAUSE)	{
		if (false);
		else if (al_id == 0)	p_bgm->pause();
		else if (al_id == 1)	p_bgm->pause(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_BGM_RESUME)	{
		int delay_time = 0;

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		delay_time = arg->Int;
			}
		}

		if (false);
		else if (al_id == 0)	p_bgm->resume(false, 0, delay_time);
		else if (al_id == 1)	p_bgm->resume(false, p_ai->al_begin[0].Int, delay_time);
	}
	else if (elm_begin[0] == ELM_BGM_RESUME_WAIT)	{
		if (false);
		else if (al_id == 0)	p_bgm->resume(true, 0, 0);
		else if (al_id == 1)	p_bgm->resume(true, p_ai->al_begin[0].Int, 0);
	}
	else if (elm_begin[0] == ELM_BGM_WAIT)	{
		p_bgm->wait(false, false);			// 戻り値を返さない
	}
	else if (elm_begin[0] == ELM_BGM_WAIT_KEY)	{
		p_bgm->wait(true, true);			// 戻り値を返す
	}
	else if (elm_begin[0] == ELM_BGM_WAIT_FADE)	{
		p_bgm->wait_fade(false, false);		// 戻り値を返さない
	}
	else if (elm_begin[0] == ELM_BGM_WAIT_FADE_KEY)	{
		p_bgm->wait_fade(true, true);		// 戻り値を返す
	}
	else if (elm_begin[0] == ELM_BGM_CHECK)	{
		tnm_stack_push_int(p_bgm->check());
	}
	else if (elm_begin[0] == ELM_BGM_SET_VOLUME)	{
		if (false);
		else if (al_id == 0)	p_bgm->set_volume(p_ai->al_begin[0].Int);
		else if (al_id == 1)	p_bgm->set_volume(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
	}
	else if (elm_begin[0] == ELM_BGM_SET_VOLUME_MAX)	{
		if (false);
		else if (al_id == 0)	p_bgm->set_volume_max();
		else if (al_id == 1)	p_bgm->set_volume_max(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_BGM_SET_VOLUME_MIN)	{
		if (false);
		else if (al_id == 0)	p_bgm->set_volume_min();
		else if (al_id == 1)	p_bgm->set_volume_min(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_BGM_GET_VOLUME)	{
		tnm_stack_push_int(p_bgm->get_volume());
	}
	else if (elm_begin[0] == ELM_BGM_GET_REGIST_NAME)	{
		tnm_stack_push_str(p_bgm->get_regist_name());
	}
	else if (elm_begin[0] == ELM_BGM_GET_PLAY_POS)	{
		tnm_stack_push_int(p_bgm->get_play_pos());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(bgm)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：KOE
// ================================================================
bool tnm_command_proc_koe(C_elm_koe* p_koe, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_koe;
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(koe)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：PCM
// ================================================================
bool tnm_command_proc_pcm(C_elm_pcm* p_pcm, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_pcm;
	}
	else if (elm_begin[0] == ELM_PCM_PLAY)	{
		p_pcm->play_pcm(p_ai->al_begin[0].str, NULL);
	}
	else if (elm_begin[0] == ELM_PCM_STOP)	{
		p_pcm->stop();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(pcm)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：PCMCH リスト
// ================================================================
bool tnm_command_proc_pcmch_list(C_elm_pcmch_list* p_pcmch_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_pcmch_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_pcmch* p_pcmch = p_pcmch_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_pcmch)
			tnm_command_proc_pcmch(p_pcmch, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(pcmchlist)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：PCMCH
// ================================================================
bool tnm_command_proc_pcmch(C_elm_pcmch* p_pcmch, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_pcmch;
	}
	else if (elm_begin[0] == ELM_PCMCH_PLAY)	{
		TSTR pcm_name;
		TSTR bgm_name;
		int koe_no = -1;
		int se_no = -1;
		int fade_in_time = 0;
		int volume_type = TNM_VOLUME_TYPE_PCM;
		int chara_no = -1;
		bool loop_flag = false;
		bool wait_flag = false;
		bool bgm_fade_target_flag = false;
		bool bgm_fade2_target_flag = false;
		bool bgm_fade_source_flag = false;

		// 引数
		switch (al_id)	{	// no break!!
			case 1:		fade_in_time = p_ai->al_begin[1].Int;
			case 0:		pcm_name = p_ai->al_begin[0].str;
			// case 2:	引数なし
		}

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		loop_flag = arg->Int ? true : false;	break;
				case 1:		wait_flag = arg->Int ? true : false;	break;
				case 2:		fade_in_time = arg->Int;				break;
				case 3:		
					if (TNM_VOLUME_TYPE_NONE <= arg->Int && arg->Int < TNM_VOLUME_TYPE_MAX)	{
						volume_type = arg->Int;		// TNM_VOLUME_TYPE_NONE が来た場合はどこのボリュームにも属さない
					}
					break;
				case 4:		bgm_fade_target_flag = arg->Int ? true : false;		break;
				case 5:		bgm_fade2_target_flag = arg->Int ? true : false;	break;
				case 6:		chara_no = arg->Int;	break;
				case 7:		pcm_name = arg->str;	break;
				case 8:		koe_no = arg->Int;		break;
				case 9:		se_no = arg->Int;		break;
				case 10:	bgm_name = arg->str;	break;
				case 11:	bgm_fade_source_flag = arg->Int ? true : false;		break;
			}
		}

		// 再生
		if (!pcm_name.empty())	{
			p_pcmch->play_pcm(pcm_name, loop_flag, wait_flag, fade_in_time, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, false);
		}
		else if (!bgm_name.empty())	{
			p_pcmch->play_bgm(bgm_name, loop_flag, wait_flag, fade_in_time, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, false);
		}
		else if (koe_no >= 0)	{
			p_pcmch->play_koe(koe_no, loop_flag, wait_flag, fade_in_time, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, false);
		}
		else if (se_no >= 0)	{
			p_pcmch->play_se(se_no, loop_flag, wait_flag, fade_in_time, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, false);
		}
	}
	else if (elm_begin[0] == ELM_PCMCH_PLAY_LOOP)	{
		if (false);
		else if (al_id == 0)	p_pcmch->play_pcm(p_ai->al_begin[0].str, true, false, 0, TNM_VOLUME_TYPE_PCM, -1, false, false, false, false);
		else if (al_id == 1)	p_pcmch->play_pcm(p_ai->al_begin[0].str, true, false, p_ai->al_begin[1].Int, TNM_VOLUME_TYPE_PCM, -1, false, false, false, false);
	}
	else if (elm_begin[0] == ELM_PCMCH_PLAY_WAIT)	{
		if (false);
		else if (al_id == 0)	p_pcmch->play_pcm(p_ai->al_begin[0].str, false, true, 0, TNM_VOLUME_TYPE_PCM, -1, false, false, false, false);
		else if (al_id == 1)	p_pcmch->play_pcm(p_ai->al_begin[0].str, false, true, p_ai->al_begin[1].Int, TNM_VOLUME_TYPE_PCM, -1, false, false, false, false);
	}
	else if (elm_begin[0] == ELM_PCMCH_READY)	{
		TSTR pcm_name;
		TSTR bgm_name;
		int koe_no = -1;
		int se_no = -1;
		int volume_type = TNM_VOLUME_TYPE_PCM;
		int chara_no = -1;
		bool loop_flag = false;
		bool bgm_fade_target_flag = false;
		bool bgm_fade2_target_flag = false;
		bool bgm_fade_source_flag = false;

		// 引数
		switch (al_id)	{	// no break!!
			case 0:		pcm_name = p_ai->al_begin[0].str;
		}

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		loop_flag = arg->Int ? true : false;	break;
				case 3:		
					if (-1 <= arg->Int && arg->Int < TNM_VOLUME_TYPE_MAX)	{
						volume_type = arg->Int;
					}
					break;
				case 4:		bgm_fade_target_flag = arg->Int ? true : false;		break;
				case 5:		bgm_fade2_target_flag = arg->Int ? true : false;	break;
				case 6:		chara_no = arg->Int;	break;
				case 7:		pcm_name = arg->str;	break;
				case 8:		koe_no = arg->Int;		break;
				case 9:		se_no = arg->Int;		break;
				case 10:	bgm_name = arg->str;	break;
				case 11:	bgm_fade_source_flag = arg->Int ? true : false;		break;
			}
		}

		// 再生
		if (!pcm_name.empty())	{
			p_pcmch->play_pcm(pcm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, true);
		}
		else if (!bgm_name.empty())	{
			p_pcmch->play_bgm(bgm_name, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, true);
		}
		else if (koe_no >= 0)	{
			p_pcmch->play_koe(koe_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, true);
		}
		else if (se_no >= 0)	{
			p_pcmch->play_se(se_no, loop_flag, false, 0, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade_source_flag, true);
		}
	}
	else if (elm_begin[0] == ELM_PCMCH_READY_LOOP)	{
		if (false);
		else if (al_id == 0)	p_pcmch->play_pcm(p_ai->al_begin[0].str, true, false, 0, TNM_VOLUME_TYPE_PCM, -1, false, false, false, true);
		else if (al_id == 1)	p_pcmch->play_pcm(p_ai->al_begin[0].str, true, false, p_ai->al_begin[1].Int, TNM_VOLUME_TYPE_PCM, -1, false, false, false, true);
	}
	else if (elm_begin[0] == ELM_PCMCH_STOP)	{
		if (false);
		else if (al_id == 0)	p_pcmch->stop();
		else if (al_id == 1)	p_pcmch->stop(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_PCMCH_PAUSE)	{
		if (false);
		else if (al_id == 0)	p_pcmch->pause();
		else if (al_id == 1)	p_pcmch->pause(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_PCMCH_RESUME)	{
		int delay_time = 0;

		// 名前つき引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		delay_time = arg->Int;
			}
		}

		if (false);
		else if (al_id == 0)	p_pcmch->resume(false, 0, delay_time);
		else if (al_id == 1)	p_pcmch->resume(false, p_ai->al_begin[0].Int, delay_time);
	}
	else if (elm_begin[0] == ELM_PCMCH_RESUME_WAIT)	{
		if (false);
		else if (al_id == 0)	p_pcmch->resume(true, 0, 0);
		else if (al_id == 1)	p_pcmch->resume(true, p_ai->al_begin[0].Int, 0);
	}
	else if (elm_begin[0] == ELM_PCMCH_WAIT)	{
		p_pcmch->wait(false, false);		// 値を返さない
	}
	else if (elm_begin[0] == ELM_PCMCH_WAIT_KEY)	{
		p_pcmch->wait(true, true);			// 値を返す
	}
	else if (elm_begin[0] == ELM_PCMCH_WAIT_FADE)	{
		p_pcmch->wait_fade(false, false);	// 値を返さない
	}
	else if (elm_begin[0] == ELM_PCMCH_WAIT_FADE_KEY)	{
		p_pcmch->wait_fade(true, true);		// 値を返す
	}
	else if (elm_begin[0] == ELM_PCMCH_CHECK)	{
		tnm_stack_push_int(p_pcmch->check());
	}
	else if (elm_begin[0] == ELM_PCMCH_SET_VOLUME)	{
		if (false);
		else if (al_id == 0)	p_pcmch->set_volume(p_ai->al_begin[0].Int);
		else if (al_id == 1)	p_pcmch->set_volume(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
	}
	else if (elm_begin[0] == ELM_PCMCH_SET_VOLUME_MAX)	{
		if (false);
		else if (al_id == 0)	p_pcmch->set_volume_max();
		else if (al_id == 1)	p_pcmch->set_volume_max(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_PCMCH_SET_VOLUME_MIN)	{
		if (false);
		else if (al_id == 0)	p_pcmch->set_volume_min();
		else if (al_id == 1)	p_pcmch->set_volume_min(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_PCMCH_GET_VOLUME)	{
		tnm_stack_push_int(p_pcmch->get_volume());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(pcmch)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：SE
// ================================================================
bool tnm_command_proc_se(C_elm_pcm* p_se, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_se;
	}
	else if (elm_begin[0] == ELM_SE_PLAY)	{
		p_se->play_se(p_ai->al_begin[0].Int, NULL);
	}
	else if (elm_begin[0] == ELM_SE_PLAY_BY_FILE_NAME)	{
		p_se->play_pcm(p_ai->al_begin[0].str, NULL);
	}
	else if (elm_begin[0] == ELM_SE_PLAY_BY_KOE_NO)	{
		p_se->play_koe(p_ai->al_begin[0].Int, NULL);
	}
	else if (elm_begin[0] == ELM_SE_PLAY_BY_SE_NO)	{
		p_se->play_se(p_ai->al_begin[0].Int, NULL);
	}
	else if (elm_begin[0] == ELM_SE_STOP)	{
		if (false);
		else if (al_id == 0)	p_se->stop();
		else if (al_id == 1)	p_se->stop(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_SE_WAIT)	{
		p_se->wait(false);
	}
	else if (elm_begin[0] == ELM_SE_WAIT_KEY)	{
		p_se->wait(true);
	}
	else if (elm_begin[0] == ELM_SE_CHECK)	{
		tnm_stack_push_int(p_se->check());
	}
	else if (elm_begin[0] == ELM_SE_SET_VOLUME)	{
		if (false);
		else if (al_id == 0)	p_se->set_volume(p_ai->al_begin[0].Int);
		else if (al_id == 1)	p_se->set_volume(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
	}
	else if (elm_begin[0] == ELM_SE_SET_VOLUME_MAX)	{
		if (false);
		else if (al_id == 0)	p_se->set_volume_max();
		else if (al_id == 1)	p_se->set_volume_max(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_SE_SET_VOLUME_MIN)	{
		if (false);
		else if (al_id == 0)	p_se->set_volume_min();
		else if (al_id == 1)	p_se->set_volume_min(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_SE_GET_VOLUME)	{
		tnm_stack_push_int(p_se->get_volume());
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(se)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：MOV
// ================================================================
bool tnm_command_proc_mov(C_elm_mov* p_mov, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_mov;
	}
	else if (elm_begin[0] == ELM_MOV_PLAY)	{
		switch (al_id)	{
			case 0:	
				// 画面いっぱいに表示
				p_mov->play(p_ai->al_begin[0].str, 0, 0, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy); 
				break;
			case 1:	
				// 指定したサイズで表示
				p_mov->play(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int);
				break;
		}
	}
	else if (elm_begin[0] == ELM_MOV_PLAY_WAIT)	{
		switch (al_id)	{
			case 0:	
				// 画面いっぱいに表示
				p_mov->play(p_ai->al_begin[0].str, 0, 0, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy); 
				break;
			case 1:	
				// 指定したサイズで表示
				p_mov->play(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int);
				break;
		}

		// ムービー待ち
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MOV_WAIT;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_MOV_PLAY_WAIT_KEY)	{
		switch (al_id)	{
			case 0:	
				// 画面いっぱいに表示
				p_mov->play(p_ai->al_begin[0].str, 0, 0, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy); 
				break;
			case 1:	
				// 指定したサイズで表示
				p_mov->play(p_ai->al_begin[0].str, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int, p_ai->al_begin[4].Int);
				break;
		}

		// ムービー待ち（クリックで飛ばせる）
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MOV_WAIT;
		proc.key_skip_enable_flag = true;
		tnm_push_proc(proc);
	}
	else if (elm_begin[0] == ELM_MOV_STOP)	{
		p_mov->close();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(mov)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：効果音イベントリスト
// ================================================================
bool tnm_command_proc_pcm_event_list(C_elm_pcm_event_list* p_pcm_event_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_pcm_event_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_pcm_event* p_pcm_event = p_pcm_event_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_pcm_event)
			tnm_command_proc_pcm_event(p_pcm_event, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(pcmeventlist)"));
	}

	return true;
}

// ****************************************************************
// コマンド処理：効果音イベント
// ================================================================
bool tnm_command_proc_pcm_event(C_elm_pcm_event* p_pcm_event, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_pcm_event;
	}
	else if (elm_begin[0] == ELM_PCMEVENT_START_ONESHOT || elm_begin[0] == ELM_PCMEVENT_START_LOOP || elm_begin[0] == ELM_PCMEVENT_START_RANDOM)	{

		bool loop_flag = true;
		bool random_flag = true;
		bool bgm_fade_target_flag = false;
		bool bgm_fade2_target_flag = false;
		bool bgm_fade2_source_flag = false;
		bool real_flag = true;				// 初期値：true！
		bool time_type = false;
		int volume_type = TNM_VOLUME_TYPE_PCM;
		int chara_no = -1;

		// 再初期化
		p_pcm_event->reinit();

		int arg_begin, arg_end, pcm_buf_no = -1;
		switch (al_id)	{
			case 0:		arg_begin = 0;	arg_end = (int)(p_ai->al_end - p_ai->al_begin);	pcm_buf_no = -1;					break;
			case 1:		arg_begin = 1;	arg_end = (int)(p_ai->al_end - p_ai->al_begin);	pcm_buf_no = p_ai->al_begin[0].Int;	break;
		}

		// 名前付き引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
				case 0:		loop_flag = arg->Int ? true : false;	break;
				case 1:		random_flag = arg->Int ? true : false;	break;
				case 3:
					if (-1 <= arg->Int && arg->Int < TNM_VOLUME_TYPE_MAX)	{
						volume_type = arg->Int;
					}
					break;
				case 4:		bgm_fade_target_flag = arg->Int ? true : false;		break;
				case 5:		bgm_fade2_target_flag = arg->Int ? true : false;	break;
				case 6:		chara_no = arg->Int;								break;
				case 11:	time_type = arg->Int ? true : false;				break;
				case 12:	bgm_fade2_source_flag = arg->Int ? true : false;	break;
			}
		}

		if (elm_begin[0] == ELM_PCMEVENT_START_ONESHOT || elm_begin[0] == ELM_PCMEVENT_START_LOOP)	{

			int line_cnt = arg_end - arg_begin;
			p_pcm_event->line_list().resize(line_cnt);
			for (int i = 0; i < line_cnt; i++)	{
				int arg_no = i + arg_begin;
				int exp_cnt = p_ai->al_begin[arg_no].exp_cnt;

				if (false);
				else if (exp_cnt == 1)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].min_time = 0;
					p_pcm_event->line_list()[i].max_time = 0;
				}
				else if (exp_cnt == 2)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].min_time = p_ai->al_begin[arg_no].exp_list[1].Int;		// 第２引数を min max 両方に与える！
					p_pcm_event->line_list()[i].max_time = p_ai->al_begin[arg_no].exp_list[1].Int;		// 第２引数を min max 両方に与える！
				}
				else if (exp_cnt == 3)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].min_time = p_ai->al_begin[arg_no].exp_list[1].Int;
					p_pcm_event->line_list()[i].max_time = p_ai->al_begin[arg_no].exp_list[2].Int;
				}
			}
		}
		else	{

			int line_cnt = arg_end - arg_begin;
			p_pcm_event->line_list().resize(line_cnt);
			for (int i = 0; i < line_cnt; i++)	{
				int arg_no = i + arg_begin;
				int exp_cnt = p_ai->al_begin[arg_no].exp_cnt;

				if (false);
				else if (exp_cnt == 1)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].probability = 1;
					p_pcm_event->line_list()[i].min_time = 0;
					p_pcm_event->line_list()[i].max_time = 0;
				}
				else if (exp_cnt == 2)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].probability = p_ai->al_begin[arg_no].exp_list[1].Int;
					p_pcm_event->line_list()[i].min_time = 0;
					p_pcm_event->line_list()[i].max_time = 0;
				}
				else if (exp_cnt == 3)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].probability = p_ai->al_begin[arg_no].exp_list[1].Int;
					p_pcm_event->line_list()[i].min_time = p_ai->al_begin[arg_no].exp_list[2].Int;		// 第３引数を min max 両方に与える！
					p_pcm_event->line_list()[i].max_time = p_ai->al_begin[arg_no].exp_list[2].Int;		// 第３引数を min max 両方に与える！
				}
				else if (exp_cnt == 4)	{
					p_pcm_event->line_list()[i].file_name = p_ai->al_begin[arg_no].exp_list[0].str;
					p_pcm_event->line_list()[i].probability = p_ai->al_begin[arg_no].exp_list[1].Int;
					p_pcm_event->line_list()[i].min_time = p_ai->al_begin[arg_no].exp_list[2].Int;
					p_pcm_event->line_list()[i].max_time = p_ai->al_begin[arg_no].exp_list[3].Int;
				}
			}
		}

		switch (elm_begin[0])	{
			case ELM_PCMEVENT_START_ONESHOT:	p_pcm_event->start_oneshot(pcm_buf_no, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade2_source_flag, real_flag, time_type);	break;
			case ELM_PCMEVENT_START_LOOP:		p_pcm_event->start_loop(pcm_buf_no, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade2_source_flag, real_flag, time_type);		break;
			case ELM_PCMEVENT_START_RANDOM:		p_pcm_event->start_random(pcm_buf_no, volume_type, chara_no, bgm_fade_target_flag, bgm_fade2_target_flag, bgm_fade2_source_flag, real_flag, time_type);		break;
		}
	}
	else if (elm_begin[0] == ELM_PCMEVENT_STOP)	{
		p_pcm_event->stop(p_ai->al_begin[0].Int ? true : false);
	}
	else if (elm_begin[0] == ELM_PCMEVENT_CHECK)	{
		tnm_stack_push_int(p_pcm_event->check() ? 1 : 0);
	}
	else if (elm_begin[0] == ELM_PCMEVENT_WAIT)	{
		p_pcm_event->wait();
	}
	else if (elm_begin[0] == ELM_PCMEVENT_WAIT_KEY)	{
		p_pcm_event->wait_key();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(pcmevent)"));
	}

	return true;
}

