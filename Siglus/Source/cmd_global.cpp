#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_fog.h"
#include	"element/elm_flag.h"
#include	"element/elm_stage.h"
#include	"element/elm_sound.h"
#include	"engine/eng_input.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_stack.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_database.h"
#include	"engine/eng_bgm_table.h"
#include	"engine/eng_mask.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_fog.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_editbox.h"

// ****************************************************************
// �R�}���h����
// ================================================================

// ������ set ����
bool tnm_command_proc_set_int(int* elm_begin, int* elm_end, int value)
{
	C_tnm_prop prop[1];
	prop[0].form = FM_INT;
	prop[0].Int = value;
	tnm_command_proc(elm_begin, elm_end, 1, prop, prop + 1, NULL, FM_VOID);

	return true;
}

// �G�������g�R�[�h����G�������g�ւ̃|�C���^���擾����
C_tnm_element* tnm_get_element_ptr(int* elm_begin, int* elm_end, bool disp_out_of_range_error)
{
	S_tnm_command_proc_arg_struct arg;
	arg.disp_out_of_range_error = disp_out_of_range_error;
	tnm_command_proc(elm_begin, elm_end, &arg);

	// �|�C���^��Ԃ�
	return arg.p_ret_element;
}

// �p�����[�^�𒼐ڎw�肷��
bool tnm_command_proc(int* elm_begin, int* elm_end)
{
	S_tnm_command_proc_arg_struct arg;
	tnm_command_proc(elm_begin, elm_end, &arg);
	return true;
}

// �p�����[�^�𒼐ڎw�肷��
bool tnm_command_proc(int* elm_begin, int* elm_end, int al_id)
{
	S_tnm_command_proc_arg_struct arg;
	arg.al_id = al_id;
	tnm_command_proc(elm_begin, elm_end, &arg);
	return true;
}

// �p�����[�^�𒼐ڎw�肷��
bool tnm_command_proc(int* elm_begin, int* elm_end, int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, C_tnm_prop* named_al_end, int ret_form_code)
{
	S_tnm_command_proc_arg_struct arg;
	arg.al_id = al_id;
	arg.al_begin = al_begin;
	arg.al_end = al_end;
	arg.named_al_end = named_al_end;
	arg.ret_form_code = ret_form_code;
	tnm_command_proc(elm_begin, elm_end, &arg);

	return true;
}

// ****************************************************************
// �R�}���h�����F���C��
// ================================================================
bool tnm_command_proc(int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;
	int* elm_top = elm_begin;

	if (elm_begin == elm_end)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�R�}���h���s���ł��B(global)"));
		return true;
	}

	// �G�������g�R�[�h�ɂ���ĕ���
	switch (elm_begin[0])	{

		// �t���O
		case ELM_GLOBAL_A:
			tnm_command_proc_int_list(&Gp_flag->A, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_B:
			tnm_command_proc_int_list(&Gp_flag->B, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_C:
			tnm_command_proc_int_list(&Gp_flag->C, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_D:
			tnm_command_proc_int_list(&Gp_flag->D, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_E:
			tnm_command_proc_int_list(&Gp_flag->E, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_F:
			tnm_command_proc_int_list(&Gp_flag->F, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_X:
			tnm_command_proc_int_list(&Gp_flag->X, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_G:
			tnm_command_proc_int_list(&Gp_flag->G, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_Z:
			tnm_command_proc_int_list(&Gp_flag->Z, 32, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_S:
			tnm_command_proc_str_list(&Gp_flag->S, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_M:
			tnm_command_proc_str_list(&Gp_flag->M, elm_top, elm_begin + 1, elm_end, p_ai);
			break;

		// ���O�t���O
		case ELM_GLOBAL_NAMAE_GLOBAL:
			tnm_command_proc_str_list(&Gp_flag->global_namae, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_NAMAE_LOCAL:
			tnm_command_proc_str_list(&Gp_flag->local_namae, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_NAMAE:	// strref ��Ԃ��R�}���h
		{
			S_element element = Gp_flag->get_element_by_name(p_ai->al_begin[0].str);
			tnm_stack_push_element(element);
			break;
		}

		// �X�e�[�W
		case ELM_GLOBAL_STAGE:
			tnm_command_proc_stage_list(Gp_stage_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_BACK:
			tnm_command_proc_stage(&(*Gp_stage_list)[TNM_STAGE_BACK], elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_FRONT:
			tnm_command_proc_stage(&(*Gp_stage_list)[TNM_STAGE_FRONT], elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_NEXT:
			tnm_command_proc_stage(&(*Gp_stage_list)[TNM_STAGE_NEXT], elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���b�Z�[�W�o�b�N
		case ELM_GLOBAL_MSGBK:
			tnm_command_proc_msgbk(Gp_msg_back, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �X�N���[��
		case ELM_GLOBAL_SCREEN:
			tnm_command_proc_screen(Gp_screen, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �J�E���^
		case ELM_GLOBAL_COUNTER:
			tnm_command_proc_counter_list(Gp_counter_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �t���[���A�N�V����
		case ELM_GLOBAL_FRAME_ACTION:
			tnm_command_proc_frame_action(Gp_frame_action, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_FRAME_ACTION_CH:
			tnm_command_proc_frame_action_list(Gp_frame_action_ch, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		case ELM_GLOBAL_TIMEWAIT:
		{
			C_tnm_proc proc;
			proc.type = TNM_PROC_TYPE_TIME_WAIT;
			proc.option = Gp_timer->local_game_time + p_ai->al_begin[0].Int;
			tnm_push_proc(proc);
			break;
		}
		case ELM_GLOBAL_TIMEWAIT_KEY:
		{
			C_tnm_proc proc;
			proc.type = TNM_PROC_TYPE_TIME_WAIT;
			proc.option = Gp_timer->local_game_time + p_ai->al_begin[0].Int;
			proc.key_skip_enable_flag = true;
			tnm_push_proc(proc);
			break;
		}

		// ���w
		case ELM_GLOBAL_MATH:
			tnm_command_proc_math(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �f�[�^�x�[�X
		case ELM_GLOBAL_DATABASE:
			tnm_command_proc_database_list(Gp_database_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �b�f�e�[�u��
		case ELM_GLOBAL_CGTABLE:
			tnm_command_proc_cg_table(Gp_cg_table, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �a�f�l�e�[�u��
		case ELM_GLOBAL_BGMTABLE:
			tnm_command_proc_bgm_table(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �f�O�O�o�b�t�@
		case ELM_GLOBAL_G00BUF:
			tnm_command_proc_g00_buf_list(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �}�X�N
		case ELM_GLOBAL_MASK:
			tnm_command_proc_mask_list(Gp_mask_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �G�f�B�b�g�{�b�N�X
		case ELM_GLOBAL_EDITBOX:
			tnm_command_proc_editbox_list(Gp_editbox_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �t�@�C��
		case ELM_GLOBAL_FILE:
			tnm_command_proc_file(elm_top, elm_begin + 1, elm_end, p_ai);
			break;

		// BGM
		case ELM_GLOBAL_BGM:
			tnm_command_proc_bgm(&Gp_sound->m_bgm, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ��
		case ELM_GLOBAL_KOE_ST:
			tnm_command_proc_koe(&Gp_sound->m_koe, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���ʉ�
		case ELM_GLOBAL_PCM:
			tnm_command_proc_pcm(&Gp_sound->m_pcm, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���ʉ��`�����l��
		case ELM_GLOBAL_PCMCH:
			tnm_command_proc_pcmch_list(&Gp_sound->m_pcmch_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���ʉ��C�x���g
		case ELM_GLOBAL_PCMEVENT:
			tnm_command_proc_pcm_event_list(Gp_pcm_event_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �V�X�e����
		case ELM_GLOBAL_SE:
			tnm_command_proc_se(&Gp_sound->m_se, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���[�r�[
		case ELM_GLOBAL_MOV:
			tnm_command_proc_mov(&Gp_sound->m_mov, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ����
		case ELM_GLOBAL_INPUT:
			tnm_command_proc_input(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �}�E�X
		case ELM_GLOBAL_MOUSE:
			tnm_command_proc_mouse(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �L�[
		case ELM_GLOBAL_KEY:
			tnm_command_proc_key_list(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �X�N���v�g
		case ELM_GLOBAL_SCRIPT:
			tnm_command_proc_script(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �V�X�e���R�}���h
		case ELM_GLOBAL_SYSCOM:
			tnm_command_proc_syscom(elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �V�X�e��
		case ELM_GLOBAL_SYSTEM:
			tnm_command_proc_system(elm_top, elm_begin + 1, elm_end, p_ai);
			break;

		// �R�[��
		case ELM_GLOBAL_CALL:
			tnm_command_proc_call_list(Gp_call_list, elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// ���݂̃R�[��
		case ELM_GLOBAL_CUR_CALL:
			tnm_command_proc_call(tnm_get_cur_call(), elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		// �d�w�R�[��
		case ELM_GLOBAL_EXCALL:
			tnm_command_proc_excall(Gp_excall, elm_top, elm_begin + 1, elm_end, p_ai);
			break;

		// Steam
		case ELM_GLOBAL_STEAM:
			tnm_command_proc_steam(elm_top, elm_begin + 1, elm_end, p_ai);
			break;

		// ================================================================
		// �d�w��
		// ----------------------------------------------------------------
		case ELM_GLOBAL_EXKOE:
		case ELM_GLOBAL_EXKOE_PLAY_WAIT:
		case ELM_GLOBAL_EXKOE_PLAY_WAIT_KEY:
		{
			tnm_command_proc_ex_koe(&Gp_sound->m_koe, elm_top, elm_begin, elm_end, p_ai);
			break;
		}

		// ================================================================
		// �V�[������R�}���h
		// ----------------------------------------------------------------
		case ELM_GLOBAL_NOP:
			break;

		case ELM_GLOBAL_OWARI:
			tnm_syscom_end_game(false, false, false);		// �x���Ȃ��ŏI������
			break;

		case ELM_GLOBAL_RETURNMENU:
			if (false);
			else if (al_id == 0)	{
				tnm_syscom_return_to_menu(false, false, true, false);			// ���j���[�V�[������J�n
			}
			else if (al_id == 1)	{
				int scn_no = Gp_lexer->get_scn_no(p_ai->al_begin[0].str);
				tnm_syscom_restart_from_scene(scn_no, 0);						// �V�[������J�n
			}
			else if (al_id == 2)	{
				int scn_no = Gp_lexer->get_scn_no(p_ai->al_begin[0].str);
				tnm_syscom_restart_from_scene(scn_no, p_ai->al_begin[1].Int);	// �V�[������J�n
			}
			break;

		case ELM_GLOBAL_JUMP:
			if (false);
			else if (al_id == 0)	tnm_scene_proc_jump(p_ai->al_begin[0].str, 0);
			else if (al_id == 1)	tnm_scene_proc_jump(p_ai->al_begin[0].str, p_ai->al_begin[1].Int);
			break;
		case ELM_GLOBAL_FARCALL:
			tnm_command_proc_farcall(al_id, p_ai->al_begin, p_ai->al_end, FM_INT);	// �߂�l��ςނ̂� return �ōs��
			break;
		case ELM_GLOBAL_GET_SCENE_NAME:
			{
				int scn_no = Gp_lexer->get_cur_scn_no();
				if (0 <= scn_no && scn_no < Gp_lexer->get_scn_cnt())	{
					tnm_stack_push_str(Gp_lexer->get_scn_name(scn_no));
				}
				else	{
					tnm_stack_push_str(_T(""));
				}
			}
			break;
		case ELM_GLOBAL_GET_LINE_NO:
			{
				int line_no = Gp_lexer->get_cur_line_no();
				tnm_stack_push_int(line_no);
			}
			break;
		case ELM_GLOBAL_SET_TITLE:
			Gp_local->scene_title =	p_ai->al_begin[0].str;
			break;
		case ELM_GLOBAL_GET_TITLE:
			tnm_stack_push_str(Gp_local->scene_title);
			break;

		case ELM_GLOBAL_SAVEPOINT:
			// �X�^�b�N�� 1 ��ς݂܂��B
			tnm_stack_push_int(1);
			// �Z�[�u���܂��B1 ���Z�[�u����܂��B
			tnm_set_save_point();
			// �X�^�b�N���� 1 �����o���܂��B
			tnm_stack_pop_int();
			// �X�^�b�N�� 0 ��ς݂܂��B
			tnm_stack_push_int(0);

			// ������̐���
			// savepoint �R�}���h�́Aint �l��Ԃ��܂��B
			// ���ʂɎ��s�����ꍇ�A0 ���ς܂ꂽ��ԂɂȂ�̂ŁA�K�� 0 ���Ԃ���܂��B
			// ���̃Z�[�u�|�C���g�Ń��[�h�����ꍇ�A1 ���ς܂�Ă����Ԃ����[�h����邽�߁A1 ���Ԃ���܂��B
			// ����𗘗p���āA�u���[�h����v�𔻒�ł��܂��B
			break;

		case ELM_GLOBAL_CLEAR_SAVEPOINT:
			tnm_clear_save_point();
			break;
		case ELM_GLOBAL_CHECK_SAVEPOINT:
			tnm_stack_push_int(tnm_check_save_point() ? 1 : 0);
			break;

		case ELM_GLOBAL_SELPOINT:
			tnm_set_sel_point();
			break;
		case ELM_GLOBAL_CLEAR_SELPOINT:
			tnm_clear_sel_point();
			break;
		case ELM_GLOBAL_CHECK_SELPOINT:
			tnm_stack_push_int(tnm_check_sel_point() ? 1 : 0);
			break;

		case ELM_GLOBAL_STACK_SELPOINT:
			tnm_stack_sel_point();
			break;
		case ELM_GLOBAL_DROP_SELPOINT:
			tnm_drop_sel_point();
			break;

		// ================================================================
		// �`��R�}���h
		// ----------------------------------------------------------------
		case ELM_GLOBAL_DISP:
			tnm_push_proc(TNM_PROC_TYPE_DISP);			// �t���[���𔲂���
			break;

		case ELM_GLOBAL_FRAME:
			Gp_global->disp_flag = false;				// �`����s��Ȃ�
			Gp_global->present_flag = false;			// ��ʍX�V���s��Ȃ�
			tnm_push_proc(TNM_PROC_TYPE_DISP);			// �t���[���𔲂���
			break;

		case ELM_GLOBAL_CAPTURE:

			// �L���v�`���[���s��
			if (Gp_ini->use_save_thumb)	{
				tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_CAPTURE, true);
			}
			break;

		case ELM_GLOBAL_CAPTURE_FROM_FILE:

			// �L���v�`���[���s��
			if (Gp_ini->use_save_thumb)
			{
				BSP<C_d3d_album> album = tnm_load_pct_d3d(p_ai->al_begin[0].str, false, false);
				if (album && album->get_texture_cnt() > 0)
				{
					Gp_wnd->m_capture_texture_for_save_thumb = album->get_texture(0);
				}
			}
			break;

		case ELM_GLOBAL_CAPTURE_FREE:
			tnm_syscom_free_capture(TNM_CAPTURE_PRIOR_CAPTURE);
			break;

		case ELM_GLOBAL_CAPTURE_FOR_OBJECT:
		{
			S_tnm_sorter capture_sorter(INT_MAX, INT_MAX);
			bool disp = true;	// ��ʍX�V���s��

			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
					case 0:		capture_sorter.order = arg->Int;	break;
					case 1:		capture_sorter.layer = arg->Int;	break;
					case 2:		disp = arg->Int ? true : false;		break;
				}
			}

			tnm_syscom_create_capture_for_object(capture_sorter, disp);
			break;
		}
		case ELM_GLOBAL_CAPTURE_FOR_OBJECT_FREE:
			tnm_syscom_free_capture_for_object();
			break;

		case ELM_GLOBAL_CAPTURE_FOR_TWEET:
			tnm_syscom_create_capture_for_tweet();
			break;
		case ELM_GLOBAL_CAPTURE_FREE_FOR_TWEET:
			tnm_syscom_free_capture_for_tweet();
			break;

		case ELM_GLOBAL_CAPTURE_FOR_LOCAL_SAVE:
		{
			S_tnm_sorter capture_sorter(INT_MAX, INT_MAX);
			int width = Gp_ini->screen_size.cx;
			int height = Gp_ini->screen_size.cy;
			bool disp = true;	// ��ʍX�V���s��

			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
					case 0:		capture_sorter.order = arg->Int;	break;
					case 1:		capture_sorter.layer = arg->Int;	break;
					case 2:		disp = arg->Int ? true : false;		break;
					case 3:		width = arg->Int;					break;
					case 4:		height = arg->Int;					break;
				}
			}

			tnm_syscom_create_capture_for_local_save(capture_sorter, width, height, disp);
			break;
		}
			

		// ���C�v
		case ELM_GLOBAL_WIPE:
		case ELM_GLOBAL_WIPE_ALL:
		case ELM_GLOBAL_MASK_WIPE:
		case ELM_GLOBAL_MASK_WIPE_ALL:
			tnm_command_proc_wipe(elm_top, elm_begin, elm_end, p_ai);
			break;

		case ELM_GLOBAL_WIPE_END:
			Gp_wipe->end();
			break;

		case ELM_GLOBAL_WAIT_WIPE:
			tnm_command_proc_wait_wipe(elm_top, elm_begin, elm_end, p_ai);
			break;

		case ELM_GLOBAL_CHECK_WIPE:
			tnm_stack_push_int(Gp_wipe->check() ? 1 : 0);
			break;

		case ELM_GLOBAL_MESSAGE_BOX:
			message_box(p_ai->al_begin[0].str, Gp_ini->game_name, MB_OK);
			break;

		// ================================================================
		// ���b�Z�[�W�E�B���h�E�֘A
		// ----------------------------------------------------------------
		case ELM_GLOBAL_SET_MWND:

			// ���݂̃E�B���h�E��ݒ�
			if (false);
			else if (al_id == 0)	Gp_local->cur_mwnd = p_ai->al_begin[0].element;
			else if (al_id == 1)	Gp_local->cur_mwnd = S_element(ELM_GLOBAL_FRONT, ELM_STAGE_MWND, ELM_ARRAY, p_ai->al_begin[0].Int);
			break;

		case ELM_GLOBAL_GET_MWND:
		{
			C_elm_mwnd* p_mwnd = tnm_get_cur_mwnd();
			if (!p_mwnd)	{
				tnm_stack_push_int(-1);
			}
			else	{
				tnm_stack_push_int(p_mwnd->get_mwnd_no());
			}
			break;
		}

		case ELM_GLOBAL_SET_SEL_MWND:

			// ���݂̑I�����E�B���h�E��ݒ�
			if (false);
			else if (al_id == 0)	Gp_local->cur_sel_mwnd = p_ai->al_begin[0].element;
			else if (al_id == 1)	Gp_local->cur_sel_mwnd = S_element(ELM_GLOBAL_FRONT, ELM_STAGE_MWND, ELM_ARRAY, p_ai->al_begin[0].Int);
			break;

		case ELM_GLOBAL_GET_SEL_MWND:
		{
			C_elm_mwnd* p_mwnd = tnm_get_cur_sel_mwnd();
			if (!p_mwnd)	{
				tnm_stack_push_int(-1);
			}
			else	{
				tnm_stack_push_int(p_mwnd->get_mwnd_no());
			}
			break;
		}

		case ELM_GLOBAL_SET_WAKU:
		{
			int element[] = {ELM_MWND_SET_WAKU};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_OPEN:
		{
			int element[] = {ELM_MWND_OPEN};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_OPEN_WAIT:
		{
			int element[] = {ELM_MWND_OPEN_WAIT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_OPEN_NOWAIT:
		{
			int element[] = {ELM_MWND_OPEN_NOWAIT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLOSE:
		{
			int element[] = {ELM_MWND_CLOSE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLOSE_WAIT:
		{
			int element[] = {ELM_MWND_CLOSE_WAIT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLOSE_NOWAIT:
		{
			int element[] = {ELM_MWND_CLOSE_NOWAIT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_END_CLOSE:
		{
			int element[] = {ELM_MWND_END_CLOSE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_MSG_BLOCK:
		{
			int element[] = {ELM_MWND_MSG_BLOCK};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_MSG_PP_BLOCK:
		{
			int element[] = {ELM_MWND_MSG_PP_BLOCK};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLEAR:
		{
			int element[] = {ELM_MWND_CLEAR};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SET_NAMAE:
		{
			int element[] = {ELM_MWND_SET_NAMAE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_PRINT:
		{
			int element[] = {ELM_MWND_PRINT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_RUBY:
		{
			int element[] = {ELM_MWND_RUBY};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_MSGBTN:
		{
			int element[] = { ELM_MWND_MSGBTN };
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_NL:
		{
			int element[] = {ELM_MWND_NL};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_NLI:
		{
			int element[] = {ELM_MWND_NLI};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_INDENT:
		{
			int element[] = {ELM_MWND_INDENT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLEAR_INDENT:
		{
			int element[] = {ELM_MWND_CLEAR_INDENT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_WAIT_MSG:
		{
			int element[] = {ELM_MWND_WAIT_MSG};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_PP:
		{
			int element[] = {ELM_MWND_PP};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_R:
		{
			int element[] = {ELM_MWND_R};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_PAGE:
		{
			int element[] = {ELM_MWND_PAGE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_REP_POS:
		{
			int element[] = {ELM_MWND_REP_POS};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SIZE:
		{
			int element[] = {ELM_MWND_SIZE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_COLOR:
		{
			int element[] = {ELM_MWND_COLOR};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_MULTI_MSG:
		{
			int element[] = {ELM_MWND_MULTI_MSG};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_NEXT_MSG:
		{
			int element[] = {ELM_MWND_NEXT_MSG};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_START_SLIDE_MSG:
		{
			int element[] = {ELM_MWND_START_SLIDE_MSG};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_END_SLIDE_MSG:
		{
			int element[] = {ELM_MWND_END_SLIDE_MSG};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_KOE:
		{
			int element[] = {ELM_MWND_KOE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_KOE_PLAY_WAIT:
		{
			int element[] = {ELM_MWND_KOE_PLAY_WAIT};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_KOE_PLAY_WAIT_KEY:
		{
			int element[] = {ELM_MWND_KOE_PLAY_WAIT_KEY};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_KOE_SET_VOLUME:
		{
			if (false);
			else if (al_id == 0)	Gp_sound->m_koe.set_volume(p_ai->al_begin[0].Int);
			else if (al_id == 1)	Gp_sound->m_koe.set_volume(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int);
			break;
		}
		case ELM_GLOBAL_KOE_SET_VOLUME_MAX:
		{
			if (false);
			else if (al_id == 0)	Gp_sound->m_koe.set_volume_max();
			else if (al_id == 1)	Gp_sound->m_koe.set_volume_max(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL_KOE_SET_VOLUME_MIN:
		{
			if (false);
			else if (al_id == 0)	Gp_sound->m_koe.set_volume_min();
			else if (al_id == 1)	Gp_sound->m_koe.set_volume_min(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL_KOE_GET_VOLUME:
		{
			tnm_stack_push_int(Gp_sound->m_koe.get_volume());
			break;
		}
		case ELM_GLOBAL_KOE_STOP:
		{
			if (false);
			else if (al_id == 0)	Gp_sound->m_koe.stop();
			else if (al_id == 1)	Gp_sound->m_koe.stop(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL_KOE_WAIT:
		{
			Gp_sound->m_koe.wait(false, false);		// �l��Ԃ��Ȃ�
			break;
		}
		case ELM_GLOBAL_KOE_WAIT_KEY:
		{
			Gp_sound->m_koe.wait(true, true);		// �l��Ԃ�
			break;
		}
		case ELM_GLOBAL_KOE_CHECK:
		{
			tnm_stack_push_int(Gp_sound->m_koe.check());
			break;
		}
		case ELM_GLOBAL_KOE_CHECK_GET_KOE_NO:
		{
			tnm_stack_push_int(Gp_sound->m_koe.get_koe_no());
			break;
		}
		case ELM_GLOBAL_KOE_CHECK_GET_CHARA_NO:
		{
			tnm_stack_push_int(Gp_sound->m_koe.get_chara_no());
			break;
		}
		case ELM_GLOBAL_KOE_CHECK_IS_EX_KOE:
		{
			tnm_stack_push_int(Gp_sound->m_koe.get_ex_koe_flag());
			break;
		}
		case ELM_GLOBAL_CLEAR_FACE:
		{
			int element[] = {ELM_MWND_CLEAR_FACE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SET_FACE:
		{
			int element[] = {ELM_MWND_SET_FACE};
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_CLEAR_MSGBK:
		{
			tnm_msg_back_clear();
			break;
		}
		case ELM_GLOBAL_INSERT_MSGBK_IMG:
		{
			TSTR file_name;
			int x = 0, y = 0;
			switch (p_ai->al_id)	{	// no break!
				case 1:		x = p_ai->al_begin[1].Int;
				case 0:		file_name = p_ai->al_begin[0].str;
			}

			tnm_msg_back_add_pct(file_name, x, 0);
			break;
		}
		case ELM_GLOBAL_SEL:
		{
			int element[] = {ELM_MWND_SEL};				// �I��p�̃E�B���h�E�őI������
			tnm_command_proc_mwnd(tnm_get_cur_sel_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SEL_CANCEL:
		{
			int element[] = {ELM_MWND_SEL_CANCEL};		// �I��p�̃E�B���h�E�őI������
			tnm_command_proc_mwnd(tnm_get_cur_sel_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SELMSG:
		{
			int element[] = {ELM_MWND_SELMSG};			// ���b�Z�[�W�p�̃E�B���h�E�őI������
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SELMSG_CANCEL:
		{
			int element[] = {ELM_MWND_SELMSG_CANCEL};	// ���b�Z�[�W�p�̃E�B���h�E�őI������
			tnm_command_proc_mwnd(tnm_get_cur_mwnd(), element, element, element + sizeof(element), p_ai);
			break;
		}
		case ELM_GLOBAL_SELBTN:
		case ELM_GLOBAL_SELBTN_READY:
		case ELM_GLOBAL_SELBTN_CANCEL:
		case ELM_GLOBAL_SELBTN_CANCEL_READY:
		{
			// ���͂������
			//Gp_input->now.use();

			ARRAY<S_tnm_sel_param> sel_list;
			int sel_cnt = 0;
			int template_no = 0;
			int sync_type = 0;

			S_tnm_sel_param param;
			param.type = 0;
			int arg_no = 0;
			C_tnm_prop* itr = p_ai->al_begin;

			if (itr->form == FM_INT)	{
				template_no = itr->Int;
				++ itr;
			}

			// �I������ǉ�
			for (; itr != p_ai->al_end; ++itr)	{

				// ������̏ꍇ�͑I�����𑝂₷
				if (itr->form == FM_STR)	{
					sel_list.resize(++sel_cnt);
					sel_list.back().str = itr->str;
					sel_list.back().type = TNM_SEL_ITEM_TYPE_ON;		// �f�t�H���g
					sel_list.back().color = -1;							// �f�t�H���g���g��
					arg_no = 0;
				}
				else if (itr->form == FM_INT)	{
					switch (arg_no)	{
						case 1:		sel_list.back().type = itr->Int;	break;
						case 2:		sel_list.back().color = itr->Int;	break;
					}
				}
				arg_no ++;
			}

			// ���O������
			bool capture_flag = false;
			TSTR sel_start_call_scn;
			int sel_start_call_z_no = 0;
			for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
				switch (arg->id)	{
					case 1:		capture_flag = arg->Int ? true : false;		break;
					case 2:		sel_start_call_scn = arg->str;				break;
					case 3:		sel_start_call_z_no = arg->Int;				break;
					case 4:		sync_type = arg->Int;						break;
				}
			}
			// �L�����Z���\�t���O��ݒ肷��
			bool cancel_enable_flag = (elm_begin[0] == ELM_GLOBAL_SELBTN || elm_begin[0] == ELM_GLOBAL_SELBTN_READY) ? false : true;

			// �V�[����
			TSTR sel_start_scn = Gp_lexer->get_scn_name(Gp_lexer->get_cur_scn_no());
			// �{�^���I�������擾
			C_elm_btn_select* p_btn_sel = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_btn_sel;

			// �I���J�n
			if (elm_begin[0] == ELM_GLOBAL_SELBTN || elm_begin[0] == ELM_GLOBAL_SELBTN_CANCEL)	{

				// ���ǃt���O���擾����
				int rf_scn_no = Gp_lexer->get_cur_scn_no();		// ���ǃt���O�̃V�[���ԍ��͌��݂̃V�[���ԍ�
				int rf_flag_no = Gp_lexer->pop_ret<int>();		// ���ǃt���O�̃t���O�ԍ����擾

				tnm_msg_proc_selbtn_ready(p_btn_sel, template_no, sel_list, cancel_enable_flag, capture_flag, sel_start_call_scn, sel_start_call_z_no);
				tnm_msg_proc_selbtn_start(p_btn_sel, rf_scn_no, rf_flag_no, sync_type);
			}
			else if (elm_begin[0] == ELM_GLOBAL_SELBTN_READY || elm_begin[0] == ELM_GLOBAL_SELBTN_CANCEL_READY)	{

				// �����̂݁i�����R�}���h�ɂ͊��ǃt���O�͊��蓖�Ă��Ă��Ȃ��j
				tnm_msg_proc_selbtn_ready(p_btn_sel, template_no, sel_list, cancel_enable_flag, false, _T(""), 0);
			}
			break;
		}
		case ELM_GLOBAL_SELBTN_START:
		{
			// ���ǃt���O���擾����
			int rf_scn_no = Gp_lexer->get_cur_scn_no();		// ���ǃt���O�̃V�[���ԍ��͌��݂̃V�[���ԍ�
			int rf_flag_no = Gp_lexer->pop_ret<int>();		// ���ǃt���O�̃t���O�ԍ����擾
			int sync_type = 0;

			// �{�^���I�������擾
			C_elm_btn_select* p_btn_sel = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_btn_sel;

			// �I���J�n
			tnm_msg_proc_selbtn_start(p_btn_sel, rf_scn_no, rf_flag_no, sync_type);
			break;
		}
		case ELM_GLOBAL_GET_LAST_SEL_MSG:
		{
			tnm_stack_push_str(Gp_global->last_sel_msg);
			break;
		}

		case ELM_GLOBAL_INIT_CALL_STACK:
		{
			tnm_scene_init_call_stack();
			break;
		}
		case ELM_GLOBAL_DEL_CALL_STACK:
		{
			tnm_scene_del_call_stack(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL_SET_CALL_STACK_CNT:
		{
			tnm_scene_set_call_stack_cnt(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL_GET_CALL_STACK_CNT:
		{
			int call_stack_cnt = tnm_scene_get_call_stack_cnt();
			tnm_stack_push_int(call_stack_cnt);
			break;
		}

		case ELM_GLOBAL___FOG_NAME:
		{
			if (false);
			else if (al_id == 0)	tnm_stack_push_str(Gp_fog->get_fog_name());
			else if (al_id == 1)	Gp_fog->create_fog(p_ai->al_begin[0].str);
			break;
		}
		case ELM_GLOBAL___FOG_X:
		{
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(Gp_fog->get_fog_x());
			else if (al_id == 1)	Gp_fog->set_fog_x(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL___FOG_X_EVE:
		{
			tnm_command_proc_int_event(&Gp_fog->x_eve(), elm_top, elm_begin + 1, elm_end, p_ai);
			break;
		}
		case ELM_GLOBAL___FOG_NEAR:
		{
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(Gp_fog->get_fog_near());
			else if (al_id == 1)	Gp_fog->set_fog_near(p_ai->al_begin[0].Int);
			break;
		}
		case ELM_GLOBAL___FOG_FAR:
		{
			if (false);
			else if (al_id == 0)	tnm_stack_push_int(Gp_fog->get_fog_far());
			else if (al_id == 1)	Gp_fog->set_fog_far(p_ai->al_begin[0].Int);
			break;
		}

		case ELM_GLOBAL______TEST:
		{
			//C_elm_object* p_obj = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_object_list[0];
			//p_obj->reinit(true);
			//p_obj->create_weather(_T("snow"));
			//p_obj->set_weather_param_type_a(1000, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 20, false);
			//p_obj->set_disp(1);
			break;
		}

		default:

			if (false);
			// ���[�U�v���p�e�B
			else if (get_elm_owner(elm_begin[0]) == ELM_OWNER_USER_PROP)	{
				tnm_command_proc_user_prop(get_elm_code(elm_begin[0]), elm_top, elm_begin + 1, elm_end, p_ai);
			}
			// ���[�U�R�}���h
			else if (get_elm_owner(elm_begin[0]) == ELM_OWNER_USER_CMD)	{
				tnm_command_proc_user_cmd(get_elm_code(elm_begin[0]), elm_top, elm_begin + 1, elm_end, p_ai);
			}
			else	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�����ȃR�}���h���w�肳��܂����B(global)"));
			}
			break;
	}

	return true;
}





