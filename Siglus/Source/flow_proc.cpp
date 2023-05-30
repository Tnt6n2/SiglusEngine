#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_wipe.h"

#include	"element/elm_stage.h"
#include	"element/elm_editbox.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"

#include	"engine/eng_input.h"
#include	"engine/eng_message.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_editbox.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_dialog.h"

#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// �O���錾
// ================================================================
bool	tnm_message_key_wait_proc_sub();
bool	tnm_message_key_wait_proc_sub_sub(C_elm_mwnd* p_mwnd);

// ****************************************************************
// �v���Z�X�����F�Q�[���I�����̃��C�v���J�n����
// ================================================================
bool tnm_game_end_wipe_proc()
{
	// ���̃v���Z�X��
	tnm_pop_proc();

	// �G�f�B�b�g�{�b�N�X��S�j�󂷂�
	for (int i = 0; i < Gp_editbox_list->get_size(); i++)
		(*Gp_editbox_list)[i].destroy();

	// �����~�߂�
	Gp_sound->m_bgm.stop(Gp_ini->load_wipe_time);
	Gp_sound->m_koe.stop();
	Gp_sound->m_pcm.stop();
	for (int i = 0; i < Gp_sound->m_pcmch_list.get_size(); i++)
		Gp_sound->m_pcmch_list[i].stop();

	// �Q�[���I�����̃��C�v�J�n
	int option[TNM_WIPE_OPTION_MAX] = {0};
	Gp_wipe->start(S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX), TNM_WIPE_RANGE_SYSTEM_OUT, 1, Gp_ini->load_wipe_type, Gp_ini->load_wipe_time, 0, 0, option, true, 0, false, false);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�Q�[���J�n���̃��C�v���J�n����
// ================================================================
bool tnm_game_start_wipe_proc()
{
	// ���̃v���Z�X��
	tnm_pop_proc();

	// �Q�[���J�n���̃��C�v�J�n
	int option[TNM_WIPE_OPTION_MAX] = {0};
	Gp_wipe->start(S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX), TNM_WIPE_RANGE_SYSTEM_IN, 1, Gp_ini->load_wipe_type, Gp_ini->load_wipe_time, 0, 0, option, true, 0, false, false);

	// �V�X�e�����C�v�t���O���~�낷
	Gp_global->system_wipe_flag = false;

	// ������
	// ���[�h�������_�ŁA�l�N�X�g�X�e�[�W�͍��̂͂��ł��B
	// ��̃��C�v�ł́A�t�����g���l�N�X�g�̃R�s�[���s���Ă��Ȃ��̂ŁA�K��������̃��C�v�ɂȂ�܂��B
	// �Ȃ̂ŁA�V�X�e�����C�v�t���O�͂����ō~�낵�Ă����v�Ȃ̂ł��B

	// �l�N�X�g�X�e�[�W��ۑ�����Ȃ�A�u�V�X�e�����C�v�t���O���~�낷�v��\�񂵁A
	// ���C�v��ɃV�X�e�����C�v�t���O���~�낳�Ȃ���΂Ȃ�܂���B

	return true;
}

// ****************************************************************
// �v���Z�X�����F�Q�[���^�C�}�[�J�n
// ================================================================
bool tnm_game_timer_start_proc()
{
	// ���̃v���Z�X��
	tnm_pop_proc();

	// ���Q�[���^�C�}�[�J�n
	Gp_global->game_timer_move_flag = true;

	return true;
}

// ****************************************************************
// �v���Z�X�����F���j���[�ɖ߂�
// ================================================================
bool tnm_return_to_menu_proc()
{
	// ���j���[�ɖ߂�
	if (Gp_local->cur_proc.option == 1)
	{
		// ���b�Z���o�b�N�͏��������Ȃ��t���O
		Gp_global->reinit_msgbk_except_flag = true;
		tnm_scene_proc_restart_from_menu_scene();
		Gp_global->reinit_msgbk_except_flag = false;
	}
	else
	{
		tnm_scene_proc_restart_from_menu_scene();
	}

	// �u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �V�X�e�����C�v�t���O���~�낷
	Gp_global->system_wipe_flag = false;

	// ������
	// �u���j���[�ɖ߂�v�́u���[�h�v�ƈႢ�A�u�Q�[���J�n���̃��C�v�v������܂���B
	// �Ȃ̂ŁA�����ŃV�X�e�����C�v�t���O���~�낵�Ă����K�v������܂��B

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F���[�h
// ================================================================
bool tnm_load_proc()
{
	// ���[�h
	tnm_saveload_proc_load(Gp_local->cur_proc.option);

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�ȈՃ��[�h�i�f�o�b�O�p�j
// ================================================================
bool tnm_easy_load_proc()
{
	// �������[�h�����͏I����Ă�
	// �����ł͌�n�����s��

	// ���̃v���Z�X��
	tnm_pop_proc();

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�N�C�b�N���[�h
// ================================================================
bool tnm_quick_load_proc()
{
	// �N�C�b�N���[�h
	tnm_saveload_proc_quick_load(Gp_local->cur_proc.option);

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�G���h���[�h
// ================================================================
bool tnm_end_load_proc()
{
	// �G���h���[�h
	tnm_saveload_proc_end_load();

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�C���i�[���[�h
// ================================================================
bool tnm_inner_load_proc()
{
	// �C���i�[���[�h
	tnm_saveload_proc_load_inner_save(Gp_local->cur_proc.option);

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�o�b�N���O���[�h
// ================================================================
bool tnm_backlog_load_proc()
{
	// �o�b�N���O���[�h
	tnm_saveload_proc_backlog_load(Gp_local->cur_proc.option_time_id);

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�O�̑I�����ɖ߂�
// ================================================================
bool tnm_return_to_sel_proc()
{
	// �O�̑I�����ɖ߂�
	tnm_saveload_proc_return_to_sel();

	// �Q�D�u�Q�[���^�C�}�[�J�n�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_TIMER_START);

	// �P�D�u�Q�[���J�n���̃��C�v�v��\��
	tnm_push_proc(TNM_PROC_TYPE_GAME_START_WIPE);

	// �V�X�e�����C�v�t���O�𗧂Ă�
	Gp_global->system_wipe_flag = true;

	// �P�񂾂��t���[���A�N�V�������s��
	Gp_global->do_frame_action_flag = true;

	// ���[�h�A�t�^�[�R�[�����s��
	Gp_global->do_load_after_call_flag = true;

	// �P��`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�^�C���E�F�C�g
// ================================================================
//		���Ԍo�߂�҂��܂��B
//		timewait �R�}���h�Ŕ������܂��B
// ================================================================
bool tnm_time_wait_proc()
{
	if (false);

	// �X�L�b�v���͎��̃v���Z�X��
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���Ԃ������玟�̃v���Z�X��
	else if (Gp_timer->local_game_time - Gp_local->cur_proc.option >= 0)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X��
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(-1);	// ���ʁF�L�����Z���L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F�L�[�E�F�C�g
// ================================================================
//		�L�[���͂�҂��܂��B������΂��\�ł��B
//		keywait �Ŕ������܂��B
// ================================================================
bool tnm_key_wait_proc()
{
	if (false);

	// �X�L�b�v���͎��̃v���Z�X��
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[���������玟�̃v���Z�X��
	else if (tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F�J�E���^�E�F�C�g
// ================================================================
//		�J�E���^���I������̂�҂��܂��B
//		counter[0].wait �R�}���h�Ŕ������܂��B
// ================================================================
bool tnm_counter_wait_proc()
{
	C_elm_counter* p_counter = (C_elm_counter *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �J�E���^���Ȃ���Ύ��̃v���Z�X��
	if (!p_counter)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X��
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �J�E���g���I�������玟�̃v���Z�X��
	else if (p_counter->get_count() - Gp_local->cur_proc.option >= 0)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X��
	else if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���b�Z�[�W�E�B���h�E�J���A�j���҂�
// ================================================================
//		���b�Z�[�W�E�B���h�E�̊J���A�j�����I���̂�҂��܂��B
//		open_wait �R�}���h�ȂǂŔ������܂��B
// ================================================================
bool tnm_mwnd_open_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���b�Z�[�W�E�B���h�E���Ȃ���Ύ��̃v���Z�X��
	if (!p_mwnd)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���̓A�j�����I��点��
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_mwnd->end_open_anime();
	}

	// �A�j�����I�������玟�̃v���Z�X��
	if (!p_mwnd->is_open_anime_doing())	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���b�Z�[�W�E�B���h�E���A�j���҂�
// ================================================================
//		���b�Z�[�W�E�B���h�E�̕��A�j�����I���̂�҂��܂��B
//		close_wait �R�}���h�ȂǂŔ������܂��B
// ================================================================
bool tnm_mwnd_close_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���b�Z�[�W�E�B���h�E���Ȃ���Ύ��̃v���Z�X��
	if (!p_mwnd)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���̓A�j�����I��点��
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_mwnd->end_close_anime();
	}

	// �A�j�����I�������玟�̃v���Z�X��
	if (!p_mwnd->is_close_anime_doing())	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

bool tnm_mwnd_close_wait_all_proc()
{
	bool not_close_exist = false;

	// ���ׂẴE�B���h�E�ɑ΂��Ĕ���
	int stage_cnt = Gp_stage_list->get_size();
	for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
		int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
		for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			// �X�L�b�v���̓A�j�����I��点��
			if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
				p_mwnd->end_close_anime();
			}

			// �A�j�����I�����Ă��邩�ǂ�������
			if (p_mwnd->is_close_anime_doing())	{
				not_close_exist = true;
			}
		}
	}

	// �S�ẴE�B���h�E�̃A�j�����I����Ă���Ύ��̃v���Z�X��
	if (!not_close_exist)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���b�Z�[�W�E�F�C�g
// ================================================================
//		���b�Z�[�W�̕\���I����҂v���Z�X�ł��B
//		msg_wait, pp, r �R�}���h�ȂǂŔ������܂��B
// ================================================================
bool tnm_message_wait_proc()
{
	int stage_cnt = Gp_stage_list->get_size();

	bool not_disp_check = false;		// ���b�Z�[�W�\�������ǂ����𔻒�

	// ���ׂẴE�B���h�E�ɑ΂��Ĕ���
	for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
		int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
		for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			if (p_mwnd->check_not_appear_moji())	{
				not_disp_check = true;		// ���b�Z�[�W�\���������������I
				break;
			}
		}
	}

	// �N���b�N�ŕ��͂��ꊇ�\������
	if (!Gp_local->pod.not_skip_msg_by_click)	{
		if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

			// �N���b�N�ŕ��͂��ꊇ�\��������t���O
			Gp_global->msg_wait_skip_by_click = true;
		}
	}

	// �N���b�N�⑁���莞�͕��͂��ꊇ�\��
	if (not_disp_check && (Gp_global->msg_wait_skip_by_click || tnm_is_skipping_msg()))	{
		not_disp_check = false;
		
		// ���ׂẴE�B���h�E�ɑ΂��Ĕ���
		for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
			int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
			for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
				C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

				// �N���b�N�����̂ł��ׂĂ̕�����\��
				p_mwnd->jump_to_msg_wait_by_nowait();

				// �܂���\���̕��������邩����i�I�[�o�[�t���[���⃁�b�Z�[�W�E�B���h�E�A�j�����̏ꍇ�͂�����ʂ�̂ł��j
				if (p_mwnd->check_not_appear_moji())	{
					not_disp_check = true;
				}
			}
		}
	}

	// �������̃E�B���h�E�����݂��Ȃ��ꍇ�͎��̃v���Z�X��
	// �����̔���ɑ����蒆���ǂ��������Ă͂����܂���B�i�I�[�o�[�t���[���͂Q�xdisp�������Ȃ��Ƃ����Ȃ��̂ł��j
	if (!not_disp_check)	{

		// ���ׂẴE�B���h�E�ɑ΂��ď���
		for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
			int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
			for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
				C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

				// �L�[�҂��J�[�\���̃A�j�����~����
				p_mwnd->set_key_icon_appear(false);
			}
		}

		// ���̃v���Z�X��
		tnm_pop_proc();

		return true;
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F���b�Z�[�W�L�[�E�F�C�g
// ================================================================
//		���b�Z�[�W�̕\�����I����̃L�[�҂��ł��B
//		�L�[�҂��A�C�R���������܂��B
//		pp, r �R�}���h�ȂǂŔ������܂��B
// ================================================================
bool tnm_message_key_wait_proc()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���b�Z�[�W�E�B���h�E���Ȃ���Ύ��̃v���Z�X��
	if (!p_mwnd)	{

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v�g���K�[�͂����I�t�ɂ���
	Gp_local->pod.skip_trigger = false;

	// ���͂�i�߂邩�ǂ����𔻒�
	if (tnm_message_key_wait_proc_sub())	{

		// �L�[�҂��J�[�\���̃A�j�����~����
		p_mwnd->set_key_icon_appear(false);

		// �i�߂��玟�̃v���Z�X��
		tnm_pop_proc();

#if 1
		// �`��ɍs�����H�i�������̂��߁A����ɂP�񂵂��`�悵�Ȃ��̂ł��B�j

		if (Gp_global->wait_display_vsync_total)	{	// ����������҂ꍇ
			Gp_global->disp_because_msg_wait_cnt ++;

			if (::timeGetTime() - Gp_timer->real_time > 100)	{		// 500ms �ȏ�o���Ă����狭���I�ɂO�ɖ߂��ĕ`�悷��
				Gp_global->disp_because_msg_wait_cnt = 0;
				Gp_global->disp_because_msg_wait_cnt_max = 0;

				return false;		// ���������P�x�ʂ��ĕ`��ɍs��
			}
			else if (Gp_global->disp_because_msg_wait_cnt >= Gp_global->disp_because_msg_wait_cnt_max / 100)	{
				Gp_global->disp_because_msg_wait_cnt = 0;

				// 30 fps �t�߂�ۂ�
				if (false);
				else if (Gp_global->frame_rate_100msec_total < 30)	{
					Gp_global->disp_because_msg_wait_cnt_max -= (30 - Gp_global->frame_rate_100msec_total) * 10;	// �x���̂ŕ`��񐔂����炵�Ă݂�
					if (Gp_global->disp_because_msg_wait_cnt_max < 0)
						Gp_global->disp_because_msg_wait_cnt_max = 0;
				}
				else	{
					Gp_global->disp_because_msg_wait_cnt_max += (Gp_global->frame_rate_100msec_total - 29);	// �����̂ŕ`��񐔂𑝂₵�Ă݂�
					if (Gp_global->disp_because_msg_wait_cnt_max > 3000)
						Gp_global->disp_because_msg_wait_cnt_max = 3000;	// �ő�łR�O���b�Z�[�W�܂Ŕ�΂�
				}

				return false;		// ���������P�x�ʂ��ĕ`��ɍs��
			}
			else	{
				return true;		// ���̃v���Z�X��
			}
		}
		else	{
			return false;		// ���������P�x�ʂ��ĕ`��ɍs��
		}
#endif

		return true;	// ���̃v���Z�X��
	}
	else	{

		// �L�[�҂��J�[�\���̃A�j�����J�n����
		p_mwnd->set_key_icon_appear(true);
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���b�Z�[�W�L�[�E�F�C�g�i�T�u�j
// ================================================================
//		�ȉ��̂Q�̋��ʏ��������ł��B
//		�E���b�Z�[�W�̕\�����I����̃L�[�҂�
//		�E�I�[�o�[�t���[���̃L�[�҂�
// ================================================================
//		�߂�l�F���͂����ɐi�߂�ꍇ�� true
// ================================================================
bool tnm_message_key_wait_proc_sub()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);
	if (!p_mwnd)
		return true;	// ���͂�i�߂�

	// ���ɐi�߂邩�ǂ����𔻒�
	if (!tnm_message_key_wait_proc_sub_sub(p_mwnd))
		return false;	// ���͂�i�߂Ȃ�

	// -- ���ɐi�߂�ꍇ�A�e��J�E���g�����Z�b�g����

	// �I�[�g���[�h�J�E���g���I��
	Gp_global->is_auto_mode_count_start = false;

	// ���̍Đ����I����Ă���̑҂����ԃJ�E���g�I��
	Gp_global->is_koe_only_count_start = false;

	// �����܂ŃI�[�g���[�h��i�߂�
	p_mwnd->set_auto_mode_end_moji_cnt();	// �������}���`���b�Z�[�W

	// ���b�Z�[�W���x�v�Z�����Z�b�g
	p_mwnd->set_progress_start_flag(false);

	// ���͂����ɐi�߂�
	return true;
}

bool tnm_message_key_wait_proc_sub_sub(C_elm_mwnd* p_mwnd)
{
	// �}�E�X�𓮂����΃I�[�g���[�h�J�E���g�����Z�b�g�i�A�N�e�B�u���̂݁j
	if (Gp_global->active_flag && Gp_cur_input->mouse.pos != Gp_last_input->mouse.pos)	{
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// ������
	}
	// ���̍Đ����̓I�[�g���[�h�J�E���g�����Z�b�g
	if (Gp_sound->m_koe.is_playing())	{
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// ������
	}
	// �I�[�g���[�h�J�E���g���J�n���Ă��Ȃ���ΊJ�n����
	if (!Gp_global->is_auto_mode_count_start)	{
		Gp_global->is_auto_mode_count_start = true;						// �I�[�g���[�h�J�E���g���J�n
		Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// ������
	}
	// ���b�Z�[�W�E�B���h�E�������Ă���Ԃ͐i�܂Ȃ�
	if (tnm_syscom_hide_mwnd_get_onoff_flag())	{

		// ���͂�i�߂Ȃ�
		return false;
	}

	// �����蒆�Ȃ烁�b�Z�[�W��i�߂�
	if (tnm_is_skipping_msg())	{

		// ���͂����ɐi�߂�
		return true;
	}

	// �L�[�{�[�h�̌���L�[�Ń��b�Z�[�W��i�߂�
	if (tnm_input_use_key_down_up(VK_EX_KEYBOARD_DECIDE))	{

		// �L�[�{�[�h�Ői�񂾏ꍇ�݂̂̓}�E�X�J�[�\��������
		Gp_global->auto_mode_cursor_off = true;

		// ���͂����ɐi�߂�
		return true;
	}

	// �L�[�{�[�h�ȊO�̌���L�[�A�܂��̓z�C�[���Ŏ��ɐi��
	if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

		// ���͂����ɐi�߂�
		return true;
	}

	// �u�����̂݁v�Ń��b�Z�[�W���\������Ă��Ȃ��ꍇ�͎����Ŏ��ɐi��
	if (Gp_config->koe_mode == TNM_KOE_MODE_KOEONLY && p_mwnd->get_koe_no() >= 0 && p_mwnd->get_koe_play_flag())	{

		// ���̍Đ����I���Ύ��ɐi��
		if (!Gp_sound->m_koe.is_playing())	{

			// ���̍Đ����I����Ă���̑҂����ԃJ�E���g���J�n���Ă��Ȃ���ΊJ�n����
			if (!Gp_global->is_koe_only_count_start)	{
				Gp_global->is_koe_only_count_start = true;					// ���̍Đ����I����Ă���̑҂����ԃJ�E���g�J�n
				Gp_global->koe_only_count_start_time = Gp_timer->real_time;	// ������
			}

			// ���̍Đ����I����Ă���̑҂����Ԃ��I���Ύ��ɐi��
			int wait_time = TNM_KOE_ONLY_WAIT;
			if (Gp_timer->real_time - Gp_global->koe_only_count_start_time > wait_time)	{

				// ���͂����ɐi�߂�
				return true;
			}
		}
	}

	// �I�[�g���[�h���̏ꍇ�͎��Ԃ��o�ĂΎ��֐i��
	if (tnm_is_auto_mode())	{

		// ��������ꍇ�͐����I���Ύ��֐i��
		if (p_mwnd->get_koe_no() >= 0 && p_mwnd->get_koe_play_flag() && !p_mwnd->get_koe_no_auto_mode_flag())	{

			// ���̍Đ����I���Ύ��ɐi��
			if (!Gp_sound->m_koe.is_playing())	{

				// ���̍Đ����I����Ă���̑҂����ԃJ�E���g���J�n���Ă��Ȃ���ΊJ�n����
				if (!Gp_global->is_koe_only_count_start)	{
					Gp_global->is_koe_only_count_start = true;					// ���̍Đ����I����Ă���̑҂����ԃJ�E���g�J�n
					Gp_global->koe_only_count_start_time = Gp_timer->real_time;	// ������
				}

				// ���̍Đ����I����Ă���̑҂����Ԃ��I���Ύ��ɐi��
				int wait_time = Gp_config->auto_mode_min_wait;
				if (Gp_timer->real_time - Gp_global->koe_only_count_start_time > wait_time)	{

					// �I�[�g���[�h�Ői�񂾂̂Ń}�E�X�J�[�\��������
					Gp_global->auto_mode_cursor_off = true;

					// ���͂����ɐi�߂�
					return true;
				}
			}
		}

		// �����Ȃ��ꍇ�̓I�[�g���[�h�Ōv�Z
		else	{

			int wait_time = 0;

			// �I�[�g���[�h�p�̕��������ʂɐݒ肳��Ă���ꍇ
			if (Gp_local->pod.auto_mode_moji_cnt > 0)	{

				int moji_cnt = Gp_local->pod.auto_mode_moji_cnt;
				wait_time = tnm_get_auto_mode_moji_wait() * moji_cnt + tnm_get_auto_mode_min_wait();
			}
			else	{

				// �҂ׂ������������߂Ď��Ԃ��v�Z����
				int moji_cnt = p_mwnd->get_disp_moji_cnt() - p_mwnd->get_auto_mode_end_moji_cnt();
				wait_time = tnm_get_auto_mode_moji_wait() * moji_cnt + tnm_get_auto_mode_min_wait();
			}

			if (Gp_timer->real_time - Gp_global->auto_mode_count_start_time > wait_time)	{
				// �������̕s������ '>=' �ɂ��Ă��͂����܂���B
				// �Ȃ��Ȃ�A�O�b�ɐݒ肵���Ƃ��ɁA�u�����Đ���������^�C�}�[�����Z�b�g�v�Ƃ����������ɂ��ւ�炸
				// �I�[�g���[�h�ŕ��͂�i�߂Ă��܂�����ł��B

				// �I�[�g���[�h�Ői�񂾂̂Ń}�E�X�J�[�\��������
				Gp_global->auto_mode_cursor_off = true;

				// ���͂����ɐi�߂�
				return true;
			}
		}
	}

	// ���͂�i�߂Ȃ�
	return false;
}

// ****************************************************************
// �v���Z�X�����F�a�f�l�E�F�C�g
// ================================================================
bool tnm_bgm_wait_proc()
{
	C_elm_bgm* p_bgm = (C_elm_bgm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �a�f�l���Ȃ���Ύ��̃v���Z�X��
	if (!p_bgm)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �a�f�l���I����Ă��玟�̃v���Z�X��
	if (!p_bgm->is_playing())	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi�a�f�l�͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi�a�f�l�͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F�a�f�l�t�F�[�h�A�E�g�E�F�C�g
// ================================================================
bool tnm_bgm_fade_out_wait_proc()
{
	C_elm_bgm* p_bgm = (C_elm_bgm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �a�f�l���Ȃ���Ύ��̃v���Z�X��
	if (!p_bgm)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �a�f�l�̃t�F�[�h�A�E�g���I����Ă��玟�̃v���Z�X��
	if (!p_bgm->is_fade_out_doing())	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi�a�f�l�͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi�a�f�l�͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���E�F�C�g
// ================================================================
bool tnm_koe_wait_proc()
{
	C_elm_koe* p_koe = &Gp_sound->m_koe;

	// �����Ȃ���Ύ��̃v���Z�X��
	if (!p_koe)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �����I����Ă��玟�̃v���Z�X��
	if (!p_koe->is_playing())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi���͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi���͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���ʉ��E�F�C�g
// ================================================================
bool tnm_pcm_wait_proc()
{
	C_elm_pcm* p_pcm = (C_elm_pcm *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���ʉ����Ȃ���Ύ��̃v���Z�X��
	if (!p_pcm)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���ʉ����I����Ă��玟�̃v���Z�X��
	if (!p_pcm->is_playing())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���ʉ��`�����l���E�F�C�g
// ================================================================
bool tnm_pcmch_wait_proc()
{
	C_elm_pcmch* p_pcmch = (C_elm_pcmch *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���ʉ����Ȃ���Ύ��̃v���Z�X��
	if (!p_pcmch)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���ʉ����I����Ă��玟�̃v���Z�X��
	if (!p_pcmch->is_playing())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���ʉ��t�F�[�h�A�E�g�E�F�C�g
// ================================================================
bool tnm_pcmch_fade_out_wait_proc()
{
	C_elm_pcmch* p_pcmch = (C_elm_pcmch *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���ʉ����Ȃ���Ύ��̃v���Z�X��
	if (!p_pcmch)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���ʉ��̃t�F�[�h�A�E�g���I����Ă��玟�̃v���Z�X��
	if (!p_pcmch->is_fade_out_doing())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���ʉ��C�x���g�E�F�C�g
// ================================================================
bool tnm_pcm_event_wait_proc()
{
	C_elm_pcm_event* p_pcm_event = (C_elm_pcm_event *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// ���ʉ��C�x���g���Ȃ���Ύ��̃v���Z�X��
	if (!p_pcm_event)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���ʉ��C�x���g���I����Ă��玟�̃v���Z�X��
	if (!p_pcm_event->check())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X�ցi���ʉ��͎~�߂Ȃ��j
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�玟�̃v���Z�X�ցi�C�x���g�͎~�߂Ȃ��j
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���[�r�[�E�F�C�g
// ================================================================
bool tnm_mov_wait_proc()
{
	// ���[�r�[�͂P�����Ȃ�
	C_elm_mov* p_mov = &Gp_sound->m_mov;

	// ���[�r�[���Ȃ���Ύ��̃v���Z�X��
	if (!p_mov)	{

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���[�r�[���I����Ă��玟�̃v���Z�X��
	if (!p_mov->is_playing())	{

		// ���[�r�[�����
		p_mov->close();

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		// ���̃v���Z�X��
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A����L�[�������ꂽ�玟�̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// �������� down_up �͏d�v�ł��B
		// up �������ƁAAlt+Enter ���ɁAEnter �� up �Ń��[�r�[���I����Ă��܂��܂��B

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		// ���[�r�[�����
		p_mov->close();

		// ���̃v���Z�X��
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�����Z���L�[�������ꂽ�玟�̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// �������� down_up �͏d�v�ł��B
		// up �������ƁAAlt+Enter ���ɁAEnter �� up �Ń��[�r�[���I����Ă��܂��܂��B

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(-1);	// ���ʁF�L�����Z���L�[
		}

		// ���[�r�[�����
		p_mov->close();

		// ���̃v���Z�X��
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �f�o�b�O���A'M' �L�[�������ꂽ�玟�̃v���Z�X��
	if (Gp_global->debug_flag && tnm_input_use_key_down_up(_T('M')))	{

		// �������� down_up �͏d�v�ł��B
		// up �������ƁAAlt+Enter ���ɁAEnter �� up �Ń��[�r�[���I����Ă��܂��܂��B

		// �L�[�҂��Ȃ猋�ʂ�Ԃ�
		if (Gp_local->cur_proc.key_skip_enable_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		// ���[�r�[�����
		p_mov->close();

		// ���̃v���Z�X��
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���̃v���Z�X�𑱂���
	return false;
}

// ****************************************************************
// �v���Z�X�����F���C�v�҂�
// ================================================================
//		���C�v���I������̂�҂��܂��B
// ================================================================
bool tnm_wipe_wait_proc()
{
	if (false);

	// �X�L�b�v���͎��̃v���Z�X��
	else if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		Gp_wipe->end();		// ���C�v���I������
		tnm_pop_proc();
	}

	// ���C�v���I�����Ă����玟�̃v���Z�X��
	else if (Gp_wipe->m_type == -1)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		Gp_wipe->end();		// ���C�v���I������
		tnm_pop_proc();
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�烏�C�v���I�����Ď��̃v���Z�X��
	else if (Gp_wipe->is_wipe_doing() && Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		Gp_wipe->end();		// ���C�v���I������
		tnm_pop_proc();
	}

	// ���K���������P�x�ʂ��ĕ`��ɍs���I
	return false;
}

// ****************************************************************
// �v���Z�X�����F�V�F�C�N�E�F�C�g
// ================================================================
//		�V�F�C�N���I������̂�҂��܂��B
// ================================================================
bool tnm_shake_wait_proc()
{
	// �V�F�C�N�͂P�����Ȃ�
	S_tnm_shake* p_shake = &Gp_screen->shake();

	// �V�F�C�N���I�����Ă����玟�̃v���Z�X��
	if (p_shake->m_no == -1)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X��
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_shake->end();
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ��V�F�C�N���I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_shake->end();
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�N�G�C�N�E�F�C�g
// ================================================================
//		�N�G�C�N���I������̂�҂��܂��B
// ================================================================
bool tnm_quake_wait_proc()
{
	// �N�G�C�N���擾
	C_elm_quake* p_quake = (C_elm_quake *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �N�G�C�N���Ȃ���Ύ��̃v���Z�X��
	if (!p_quake)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �N�G�C�N���I�����Ă����玟�̃v���Z�X��
	if (p_quake->get_type() == TNM_QUAKE_TYPE_NONE)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �X�L�b�v���͎��̃v���Z�X��
	if (tnm_is_skipping() && !Gp_local->cur_proc.skip_disable_flag)	{
		p_quake->end(0);
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ��V�F�C�N���I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_quake->end(0);
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�C�x���g�E�F�C�g
// ================================================================
//		�C�x���g���I������̂�҂��܂��B
// ================================================================
bool tnm_event_wait_proc()
{
	C_elm_int_event* p_event = (C_elm_int_event *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �C�x���g��������Ȃ������玟�̃v���Z�X��
	if (!p_event)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}
	// �C�x���g���I�����Ă����玟�̃v���Z�X��
	if (!p_event->check_event())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ��V�F�C�N���I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		p_event->end_event();
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�S�̃C�x���g�E�F�C�g
// ================================================================
//		�S�̃C�x���g���I������̂�҂��܂��B
// ================================================================
bool tnm_all_event_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �C�x���g��������Ȃ������玟�̃v���Z�X��
	if (!p_object)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �C�x���g���I�����Ă����玟�̃v���Z�X��
	if (!p_object->check_all_event())	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ��V�F�C�N���I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{
		p_object->end_all_event();
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�I�u�W�F�N�g���[�r�[�E�F�C�g
// ================================================================
//		�I�u�W�F�N�g���[�r�[���I������̂�҂��܂��B
// ================================================================
bool tnm_obj_mov_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �I�u�W�F�N�g��������Ȃ������玟�̃v���Z�X��
	if (!p_object)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���[�r�[���I�����Ă����玟�̃v���Z�X��
	if (!p_object->check_movie())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ�烀�[�r�[���I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		p_object->init_type(true);
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�I�u�W�F�N�g E-mote �E�F�C�g
// ================================================================
//		�I�u�W�F�N�g E-mote ���I������̂�҂��܂��B
// ================================================================
bool tnm_obj_emote_wait_proc()
{
	C_elm_object* p_object = (C_elm_object *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �I�u�W�F�N�g��������Ȃ������玟�̃v���Z�X��
	if (!p_object)	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// ���[�r�[���I�����Ă����玟�̃v���Z�X��
	if (!p_object->emote_check_playing())	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(0);	// ���ʁF�^�C���I�[�o�[
		}

		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�[�҂��̏ꍇ�A�L�[�������ꂽ��A�j�����I�����Ď��̃v���Z�X��
	if (Gp_local->cur_proc.key_skip_enable_flag && tnm_input_use_key_down_up(VK_EX_DECIDE))	{

		// ���ʂ�Ԃ�
		if (Gp_local->cur_proc.return_value_flag)	{
			tnm_stack_push_int(1);	// ���ʁF����L�[
		}

		p_object->emote_pass();
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�I����
// ================================================================
bool tnm_sel_proc()
{
	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�{�^���I����
// ================================================================
bool tnm_sel_btn_proc()
{
	// �t�����g�̃{�^���I�������Ď�����
	C_elm_btn_select* p_sel = &(*Gp_stage_list)[TNM_STAGE_FRONT].m_btn_sel;

	// �I�����I����Ă���Ύ��̃v���Z�X��
	if (!p_sel->is_processing())	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�{�^���I�u�W�F�N�g�I����
// ================================================================
bool tnm_sel_btn_obj_proc()
{
	// �^�[�Q�b�g�̃O���[�v���擾
	C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

	// �O���[�v��������Ȃ������玟�̃v���Z�X��
	if (!p_group)	{
		tnm_pop_proc();
		return true;	// ���̃v���Z�X��
	}

	// �L�����Z���\�ȏꍇ
	if (p_group->get_cancel_flag() && tnm_input_use_key_down_up(VK_EX_CANCEL))	{

		// ���ʉ�
		tnm_play_se(p_group->get_cancel_se_no());
		if (p_group->get_wait_flag())	{
			p_group->end();				// �I���I��
			tnm_stack_push_int(-1);		// ���ʂ��X�^�b�N�ɐς�
			tnm_pop_proc();				// ���̃v���Z�X��
			return true;
		}
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�Z�[�u�_�C�A���O���J��
// ================================================================
bool tnm_save_dialog_proc()
{
	// �Z�[�u�_�C�A���O���J���i�L���v�`���[�͍s��Ȃ��j
	tnm_open_save_dialog(false);

	// ���̃v���Z�X��
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// �v���Z�X�����F�G���h�Z�[�u�i�Q�i�K�ځj
// ================================================================
bool tnm_end_save_proc()
{
	// �G���h�Z�[�u�Q�i�K�ځi�L���v�`���[���s��Ȃ��j
	tnm_syscom_end_save_period_2(false);

	// ���̃v���Z�X��
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// �v���Z�X�����F�Q�[���I��
// ================================================================
bool tnm_end_game_proc()
{
	// �I���
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;

	// ���̃v���Z�X��
	tnm_pop_proc();
	return true;
}

// ****************************************************************
// �v���Z�X�����F�J�n���̌x�����
// ================================================================
bool tnm_start_warning_proc()
{
	static TCHAR* warning_file_01 = _T("___SYSEVE_WARNING");
	static TCHAR* warning_file_02 = _T("___SYSEVE_BLACK");

	if (false);
	else if (Gp_local->cur_proc.option == 0)	{

		// ���̃X�e�[�g��
		Gp_local->cur_proc.option = 1;

		// �x���摜���Ȃ���΃v���Z�X�𔲂���i�G���[���o���Ȃ��j
		if (!tnm_check_pct(warning_file_01, false))	{
			tnm_pop_proc();
			return true;
		}

		// �e�푀����֎~
		Gp_local->pod.ctrl_disable = true;
		Gp_local->pod.syscom_menu_disable = true;
		Gp_local->pod.hide_mwnd_disable = true;
		Gp_local->pod.msg_back_disable = true;
		Gp_local->pod.shortcut_disable = true;

		// �^�C���E�F�C�g
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT;
		proc.option = Gp_timer->local_game_time + 1000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 1)	{

		// ���̃X�e�[�g��
		Gp_local->cur_proc.option = 2;

		// �x���p�̃I�u�W�F�N�g������
		(*Gp_stage_list)[TNM_STAGE_BACK].m_object_list[0].create_pct(warning_file_01);
		(*Gp_stage_list)[TNM_STAGE_BACK].m_object_list[0].set_disp(1);

		// ���C�v
		int option[TNM_WIPE_OPTION_MAX] = {0};
		Gp_wipe->start(S_tnm_sorter(0, INT_MIN), S_tnm_sorter(0, INT_MAX), TNM_WIPE_RANGE_NORMAL, false
			, 0, 1000, 0, 0, option, true, 0, false, true);
	}
	else if (Gp_local->cur_proc.option == 2)	{

		// ���̃X�e�[�g��
		Gp_local->cur_proc.option = 3;

		// ���̉摜���Ȃ���΍ŏI�X�e�[�g��
		if (!tnm_check_pct(warning_file_02, false))	{
			Gp_local->cur_proc.option = 99;
		}

		// �^�C���E�F�C�g
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT; 
		proc.option = Gp_timer->local_game_time + 3000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 3)	{

		// ���̃X�e�[�g��
		Gp_local->cur_proc.option = 4;

		// ���C�v
		int option[TNM_WIPE_OPTION_MAX] = {0};
		Gp_wipe->start(S_tnm_sorter(0, INT_MIN), S_tnm_sorter(0, INT_MAX), TNM_WIPE_RANGE_NORMAL, false
			, 0, 1000, 0, 0, option, true, 0, false, true);
	}
	else if (Gp_local->cur_proc.option == 4)	{

		// �ŏI�̃X�e�[�g��
		Gp_local->cur_proc.option = 99;

		// �^�C���E�F�C�g
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_TIME_WAIT; 
		proc.option = Gp_timer->local_game_time + 1000;
		proc.key_skip_enable_flag = false;
		tnm_push_proc(proc);
	}
	else if (Gp_local->cur_proc.option == 99)	{

		// �e�푀�������
		Gp_local->pod.ctrl_disable = false;
		Gp_local->pod.syscom_menu_disable = false;
		Gp_local->pod.hide_mwnd_disable = false;
		Gp_local->pod.msg_back_disable = false;
		Gp_local->pod.shortcut_disable = false;

		// ���̃v���Z�X��
		tnm_pop_proc();
		return true;
	}

	return false;	// ���̃v���Z�X�𑱂���
}

// ****************************************************************
// �v���Z�X�����F�c�C�[�g�p�̃L���v�`���[�̎��̏���
// ================================================================
bool tnm_tweet_capture_after_proc()
{
	// Twitter �_�C�A���O���J��
	tnm_dlg_open_twitter_dialog();

	// ���̃v���Z�X��
	tnm_pop_proc();
	return true;
}
