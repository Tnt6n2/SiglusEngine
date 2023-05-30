#include	"pch.h"
#include	"ifc_eng.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_font.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_display.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/flow_script.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_input.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_font.h"
#include	"engine/eng_etc.h"
#include	"engine/ifc_steam.h"

#if __USE_EMOTE
#include	"engine/eng_emote.h"
#endif

#include	"../resource.h"

// ****************************************************************
// �t���[������
// ================================================================
//		���͂��擾
//		�^�C�}�[���X�V�i�ł��邾�������s���B���͂̏�������ɕK�v�j
// ================================================================
//		�q�b�g���`�F�b�N�i�O��̃q�b�g��`�ƁA����̓��͏�Ԃ���v�Z�j
//		�q�b�g�C�x���g�i���������Ƃ��Ɍ��ʉ���炷�c�Ȃǁj
//		�e�G�������g�̃t���[��������
//		�X�N���v�g����
//		�e�G�������g�̃t���[�������i�����Ńq�b�g��`���v�Z�j
// ================================================================
bool C_tnm_eng::frame()
{
	if (!frame_local())
		return false;

	if (!frame_global())
		return false;

	return true;
}

// ****************************************************************
// �t���[�����[�J������
// ================================================================
//		���͂��擾
//		�^�C�}�[���X�V�i�ł��邾�������s���B���͂̏�������ɕK�v�j
// ================================================================
//		�q�b�g���`�F�b�N�i�O��̃q�b�g��`�ƁA����̓��͏�Ԃ���v�Z�j
//		�q�b�g�C�x���g�i���������Ƃ��Ɍ��ʉ���炷�c�Ȃǁj
//		�e�G�������g�̃t���[��������
//		�X�N���v�g����
//		�e�G�������g�̃t���[�������i�����Ńq�b�g��`���v�Z�j
// ================================================================

// �y���t�H���g���g������
bool tnm_is_use_light_font()
{
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
		return false;	// �L���v�`���[���s���Ƃ��͌y���t�H���g�͎g��Ȃ�

	int type = Gp_local->cur_proc.type;
	if (type == TNM_PROC_TYPE_SEL || type == TNM_PROC_TYPE_SEL_BTN || type == TNM_PROC_TYPE_SEL_BTN_OBJ)
		return false;	// �I�������͌y���t�H���g�͎g��Ȃ�

	int proc_cnt = (int)Gp_local->proc_stack.size();
	for (int i = 0; i < proc_cnt; i++)	{
		int type = Gp_local->proc_stack[i].type;
		if (type == TNM_PROC_TYPE_SEL || type == TNM_PROC_TYPE_SEL_BTN || type == TNM_PROC_TYPE_SEL_BTN_OBJ)
			return false;	// �I�������͌y���t�H���g�͎g��Ȃ�
	}

	if (tnm_is_skipping_without_skip_trigger())
		return true;	// �����蒆�͌y���t�H���g���g���i�X�L�b�v�g���K�[�͏����j

	return false;	// ���ʂ͌y���t�H���g���g��Ȃ�
}

bool C_tnm_eng::frame_local()
{
	// D3D �f�o�C�X�`�F�b�N
	if (!tnm_check_device())
		return false;

	// Steam
#if __USE_STEAM
	steam_frame_proc();
#endif

	// �O��̏���ۑ�����
	m_global.total_game_screen_pos_last = m_global.total_game_screen_pos;
	m_global.total_game_screen_size_last = m_global.total_game_screen_size;

	// �t���[���ϐ�������������
	m_global.active_flag = false;
	m_global.focused_flag = false;
	m_global.focused_flag_on_edit_box = false;
	m_global.editbox_hide_flag_total = false;
	m_global.disp_flag = true;
	m_global.present_flag = true;
	m_global.do_frame_action_flag = false;
	m_global.do_load_after_call_flag = false;
	m_global.sel_start_call_flag = false;
	m_global.msg_wait_skip_by_click = false;

	// �A�N�e�B�u����
	HWND h_active_wnd = GetActiveWindow();
	m_global.active_flag = (h_active_wnd == Gp_wnd->get_handle());

	// �t�H�[�J�X�E�B���h�E���G�f�B�b�g�{�b�N�X���ǂ����𔻒肷��
	// �u�G�f�B�b�g�{�b�N�X�Ƀt�H�[�J�X������ꍇ�� Ctrl �X�L�b�v���s��Ȃ��v�����Ɏg��
	HWND h_focused_wnd = GetFocus();
	if (h_focused_wnd)	{
		if (h_focused_wnd == Gp_wnd->get_handle())	{
			m_global.focused_flag = true;
		}
		else	{
			TCHAR class_name[256] = {0};
			GetClassName(h_focused_wnd, class_name, 256);
			if (TSTR(class_name) == WC_EDIT)	{
				m_global.focused_flag_on_edit_box = true;
			}
		}
	}

	// �𑜓x��ύX��������̏���
	if (m_global.change_display_mode_proc_cnt)	{
		Gp_global->alt_menu_wait_flag = true;
		m_global.change_display_mode_proc_cnt --;
		if (m_global.change_display_mode_proc_cnt == 0)	{
			tnm_reset_screen_size_mode();
		}
	}

#if _DEBUG
	// Shift�������Ă���Ԃ͉������Ȃ�
	if (is_key_down(VK_SHIFT))	{
		m_global.disp_flag = false;
		return true;
	}
#endif

	// �u���[�N���͉������Ȃ��i�X�e�b�v���s���͏����ł���j
	if (m_global.is_breaking && !m_global.break_step_flag)	{
		m_global.disp_flag = false;
		return true;
	}

	// ���[�r�[�������͉������Ȃ�
	if (m_sound.m_mov.is_readying())	{
		m_global.disp_flag = false;
		return true;
	}

	// �O��̃}�E�X����ݒ肵�Ă���
	m_last_input = Gp_input->now;
	m_last_input.mouse.pos = m_cur_input.mouse.pos;		// ���΍��W��

	// ���͂��X�V
	Gp_input->frame();

	// �P�t���[���Ƀ{�^�����������������������ꍇ�A�u�������v�������I�ɃL�����Z��
	if (Gp_input->now.mouse.left.get_down_up_stock() == 2 && m_last_input.mouse.left.get_down_up_stock() == 0)	{
		Gp_input->now.mouse.left.unset_up();
		Gp_input->next.mouse.left.set_real_state(1);
	}

	// �^�C�}�[�����i���͂��g���Ă�̂ł��̈ʒu�j
	timer_proc();

	// ����̐V�K�}�E�X�����L�����Ă���
	m_cur_input = Gp_input->now;

	// �L�[�{�[�h�Ń}�E�X�J�[�\���ړ��i�����ł͂܂��}�E�X�͐�΍��W�ɔz�u����Ă���j
	mouse_move_proc();

	// �}�E�X���W���N���C�A���g���W�ɒ�������
	Gp_cur_input->mouse.pos = Gp_wnd->screen_to_client(Gp_cur_input->mouse.pos);

	// �E�B���h�E�T�C�Y�ɂ���ă}�E�X���W���ύX�����
	Gp_cur_input->mouse.pos.x = (Gp_cur_input->mouse.pos.x - m_global.total_game_screen_pos.x) * m_global.game_screen_size.cx / m_global.total_game_screen_size.cx;
	Gp_cur_input->mouse.pos.y = (Gp_cur_input->mouse.pos.y - m_global.total_game_screen_pos.y) * m_global.game_screen_size.cy / m_global.total_game_screen_size.cy;

	// �P�t���[�������N���b�N���֎~���鏈��
	// �R���e�L�X�g���j���[���������߂̃N���b�N�͂����ŃL�����Z������܂��B
	// Enter �L�[�Ȃǂ̓��C���E�B���h�E�ɂ͗��Ȃ��̂ŏ�������K�v�͂���܂���B
	if (m_global.skip_click_1_frame)	{
		m_global.skip_click_1_frame = false;
		Gp_input->now.mouse.left.use();
		Gp_input->now.mouse.right.use();
	}

	// �}�E�X�𓮂����΃I�[�g���[�h�J�E���g�����Z�b�g
	if (Gp_cur_input->mouse.pos != m_last_input.mouse.pos)	{
		m_global.auto_mode_cursor_off = false;							// �I�[�g���[�h���ɏ����Ă����J�[�\���𕜋A�����܂��B
	}
	// �L�[�������΃I�[�g���[�h�J�E���g�����Z�b�g
	//if (Gp_input->check_ex_key_down(VK_EX_DECIDE) || Gp_input->check_ex_key_down(VK_EX_CANCEL))	{
	//	m_global.auto_mode_count_start_time = m_timer.real_time;		// ������
	//	m_global.auto_mode_cursor_off = false;							// �I�[�g���[�h���ɏ����Ă����J�[�\���𕜋A�����܂��B
	//}
	// �L�[�������΁u�N���b�N����܂Ŏ�����������֎~����v�t���O���~�낷
	if (tnm_input_check_key_down(VK_EX_DECIDE) || tnm_input_check_key_down(VK_EX_CANCEL) || tnm_input_is_key_down(VK_CONTROL))	{
		m_global.cant_auto_skip_before_click = false;		// ����Ŏ��������肪�L���ɂȂ�
	}
	// �����Ń}�E�X�J�[�\��������
	if (Gp_cur_input->mouse.pos != m_last_input.mouse.pos || tnm_input_check_key_down(VK_EX_DECIDE) || tnm_input_check_key_down(VK_EX_CANCEL))	{
		m_global.mouse_cursor_hide_time = 0;
	}
	else	{
		m_global.mouse_cursor_hide_time += Gp_timer->local_real_time_past;
	}

	// �f�o�b�O�L�[����
	if (!debug_key_proc())
		return false;

	// �V�X�e���L�[����
	if (!system_key_proc())
		return false;

	// �{�^������
	if (!button_proc())
		return false;

	// �L�����Z���R�[�������i�|�b�v�A�b�v���̓Q�[����������Ȃ��ăV�X�e�������ɂ���ׂ������H�j
	if (!cancel_call_proc())
		return false;

	// �{�^���q�b�g�e�X�g
	if (!button_hit_test_proc())
		return false;

	// �{�^���C�x���g����
	if (!button_event_proc())
		return false;

	// ���N���b�N�ő������~�i�E�N���b�N�͉E�N���b�N���j���[�̌Ăяo�����D��ł��B������͎~�܂�܂���j
	if (!m_local.pod.not_stop_skip_by_click)	{
		if (m_syscom.read_skip_flag && tnm_input_use_key_down(VK_EX_DECIDE))	{
			tnm_syscom_read_skip_set_onoff_flag(false);		// ��������~�߂�
			m_global.cant_auto_skip_before_click = true;	// �u�N���b�N����܂Ŏ�����������֎~����v�t���O�𗧂Ă�i���ɃA�N�V�������N�����Ύ��������肪�ĊJ�����j
		}
	}

	// ������łȂ��Ȃ烁�b�Z�[�W�X�L�b�v���x�����ɖ߂�
	if (!tnm_is_skipping())	{
		Gp_global->disp_because_msg_wait_cnt_max = 0;
	}

	// �X�L�b�v�g���K�[
	if (!Gp_global->ex_call_flag && Gp_local->pod.skip_trigger)	{

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

		// �\����������Έꊇ�\�������ɐi�ނ̂ł��B
		if (not_disp_check)	{

			// �N���b�N�𔻒�
			if (tnm_input_use_key_down_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{

				// �N���b�N�ŕ��͂�\��������t���O
				Gp_global->msg_wait_skip_by_click = true;
			}

			// �N���b�N�⑁���莞�͕��͂��ꊇ�\��
			if (Gp_global->msg_wait_skip_by_click)	{

				// ���ׂẴE�B���h�E�ɑ΂��Ĕ���
				for (int stage_no = 0; stage_no < stage_cnt; stage_no ++)	{
					int mwnd_cnt = (*Gp_stage_list)[stage_no].m_mwnd_list.get_size();
					for (int mwnd_no = 0; mwnd_no < mwnd_cnt; mwnd_no++)	{
						C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

						// �N���b�N�����̂ł��ׂĂ̕�����\��
						p_mwnd->jump_to_msg_wait_by_nowait();
					}
				}
			}
		}
		// �\�������Ȃ���΃X�L�b�v�g���K�[�����Ȃ̂ł��B
		else	{

			// �N���b�N�𔻒�iup �̂݁I�j
			if (tnm_input_use_key_up(VK_EX_DECIDE) || (Gp_config->system.wheel_next_message_flag && Gp_input->now.mouse.wheel.use_down_stock()))	{
				Gp_global->skip_because_skip_trigger = true;
			}
			else	{

				C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_cur_mwnd();
				if (p_mwnd)	{
				
					// �L�[�҂��J�[�\���̃A�j�����J�n����
					p_mwnd->set_key_icon_appear(true);
				}
			}
		}
	}

	// ================================================================
	// ���G�������g�̃t���[������������
	// ================================================================
	{
		// �X�e�[�W�̃t���[��������
		int stage_cnt = m_stage_list.get_size();
		for (int i = 0; i < stage_cnt; i++)
			m_stage_list[i].frame_init();

		// ���b�Z�[�W�o�b�N�̃t���[��������
		m_msg_back.frame_init();

		// �d�w�R�[���X�e�[�W�̃t���[��������
		int excall_stage_cnt = m_excall.m_stage_list.get_size();
		for (int i = 0; i < excall_stage_cnt; i++)
			m_excall.m_stage_list[i].frame_init();
	}

	// ================================================================
	// ���G�������g�̎��Ԃ�i�߂�
	// ================================================================

	// �G�������g�̌o�ߎ��Ԃ����߂�
	int element_past_real_time = m_timer.local_real_time_past;
	int element_past_game_time = m_timer.local_game_time_past;
	int element_past_wipe_time = m_timer.local_wipe_time_past;
	{
		// ���C�v�̎��Ԃ�i�߂�
		m_wipe.update_time(element_past_wipe_time);

		// excall �ȊO�ɂ��āA���Ԓ�~���͐i�߂Ȃ��B���������C�v�����͗�O�i�݌v�~�X���ȁc�j
		if (!m_local.pod.time_stop_flag)	{

			// �J�E���^�̎��Ԃ�i�߂�
			if (!m_local.pod.counter_time_stop_flag)	{
				int counter_cnt = m_counter_list.get_size();
				for (int i = 0; i < counter_cnt; i++)
					m_counter_list[i].update_time(element_past_game_time, element_past_real_time);
			}

			// �}�X�N�̎��Ԃ�i�߂�
			int mask_cnt = m_mask_list.get_size();
			for (int i = 0; i < mask_cnt; i++)
				m_mask_list[i].update_time(element_past_game_time, element_past_real_time);

			// �X�N���[���̎��Ԃ�i�߂�
			m_screen.update_time(element_past_game_time, element_past_real_time);

			// �t���[���A�N�V�����̎��Ԃ�i�߂�
			if (!m_local.pod.frame_action_time_stop_flag)	{
				m_frame_action.update_time(element_past_game_time, element_past_real_time);

				// �t���[���A�N�V�����`�����l���̎��Ԃ�i�߂�
				int frame_action_ch_cnt = m_frame_action_ch_list.get_size();
				for (int i = 0; i < frame_action_ch_cnt; i++)
					m_frame_action_ch_list[i].update_time(element_past_game_time, element_past_real_time);
			}

			// �X�e�[�W�̎��Ԃ�i�߂�
			if (!m_local.pod.stage_time_stop_flag)	{
				m_stage_list[TNM_STAGE_BACK].update_time(element_past_game_time, element_past_real_time);
				m_stage_list[TNM_STAGE_FRONT].update_time(element_past_game_time, element_past_real_time);
				if (m_wipe.is_wipe_doing())	{
					m_stage_list[TNM_STAGE_NEXT].update_time(element_past_game_time, element_past_real_time);
				}
			}

			// �t�H�O�̎��Ԃ�i�߂�
			m_fog.update_time(element_past_game_time, element_past_real_time);

			// �a�f�l�̎��Ԃ�i�߂�
			m_sound.m_bgm.update_time(element_past_real_time);

			// ���ʉ��`�����l���̎��Ԃ�i�߂�
			int pcmch_cnt = m_sound.m_pcmch_list.get_size();
			for (int i = 0; i < pcmch_cnt; i++)
				m_sound.m_pcmch_list[i].update_time(element_past_real_time);
		}

		// �d�w�R�[���̎��Ԃ�i�߂�
		if (m_excall.is_ready())	{

			// �J�E���^�̎��Ԃ�i�߂�
			int excall_counter_cnt = m_excall.m_counter_list.get_size();
			for (int i = 0; i < excall_counter_cnt; i++)
				m_excall.m_counter_list[i].update_time(element_past_game_time, element_past_real_time);

			// �t���[���A�N�V�����̎��Ԃ�i�߂�
			m_excall.m_frame_action.update_time(element_past_game_time, element_past_real_time);

			// �t���[���A�N�V�����`�����l���̎��Ԃ�i�߂�
			int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size(); 
			for (int i = 0; i < excall_frame_action_ch_cnt; i++)
				m_excall.m_frame_action_ch_list[i].update_time(element_past_game_time, element_past_real_time);

			// �X�e�[�W�̎��Ԃ�i�߂�
			m_excall.m_stage_list[TNM_STAGE_BACK].update_time(element_past_game_time, element_past_real_time);
			m_excall.m_stage_list[TNM_STAGE_FRONT].update_time(element_past_game_time, element_past_real_time);
			if (m_wipe.is_wipe_doing())	{
				m_excall.m_stage_list[TNM_STAGE_NEXT].update_time(element_past_game_time, element_past_real_time);
			}
		}
	}

	// ================================================================
	// ���V�X�e�����g��Ȃ��������͂��X�N���v�g�ɓn��
	// ================================================================
	// �X�N���v�g�ɓ��͂�n��
	// 2010/09/28 �����Ɉڂ��܂����B
	// �������̒��œ��͂�i�߂Ă񂾂��ǁc�B
	// ���t���[�� input.next ���������Ă��ˁB����΂���ˁB�P���[�v�� disp �Q����������͎�肱�ڂ���H
	// �ǂ����悤�B
	// ================================================================
	script_input_proc();

	// ================================================================
	// �����C������
	// ================================================================
	frame_main_proc();

	// ================================================================
	// ���t���[���A�N�V��������
	// ================================================================
	frame_action_proc();

	// ================================================================
	// ���G�������g�̃t���[������
	// ================================================================
	{
		// �����p�����[�^�i������΂����̕����N�I���e�B�[�𒲐�����j
		bool draw_futoku = m_global.total_font_futoku;
		bool draw_shadow = (m_global.total_font_shadow == TNM_FONT_SHADOW_MODE_NONE) ? false : true;

		if (tnm_is_use_light_font())
		{
			draw_futoku = false;
			draw_shadow = false;
		}

		// ���C�v�̃t���[������
		m_wipe.frame();

		// �}�X�N�̃t���[������
		int mask_cnt = m_mask_list.get_size(); 
		for (int i = 0; i < mask_cnt; i++)
			m_mask_list[i].frame();

		// �X�N���[���̃t���[������
		m_screen.frame();

		// �t���[���A�N�V�����̃t���[������
		m_frame_action.frame();

		// �t���[���A�N�V�����`�����l���̃t���[������
		int frame_action_ch_cnt = m_frame_action_ch_list.get_size();
		for (int i = 0; i < frame_action_ch_cnt; i++)
			m_frame_action_ch_list[i].frame();

		// �X�e�[�W�̃t���[�������i���[�r�[�Đ����͉������Ȃ��j
		if (!m_sound.m_mov.is_playing())	{
			m_stage_list[TNM_STAGE_FRONT].frame(draw_futoku, draw_shadow, 0);
			if (m_wipe.is_wipe_doing())	{
				m_stage_list[TNM_STAGE_NEXT].frame(draw_futoku, draw_shadow, 0);
			}
		}

		// �t�H�O�̃t���[������
		m_fog.frame();

		// ���ʉ��C�x���g�̃t���[������
		int pcm_event_cnt = m_pcm_event_list.get_size();
		for (int i = 0; i < pcm_event_cnt; i++)
			m_pcm_event_list[i].frame(element_past_game_time, element_past_real_time);

		// �G�f�B�b�g�{�b�N�X�̃t���[������
		int editbox_cnt = m_editbox_list.get_size();
		for (int i = 0; i < editbox_cnt; i++)
			m_editbox_list[i].frame();

		// �d�w�R�[���̃t���[������
		if (m_excall.is_ready())	{

			// �t���[���A�N�V�����̃t���[������
			m_excall.m_frame_action.frame();

			// �t���[���A�N�V�����`�����l���̃t���[������
			int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size();
			for (int i = 0; i < excall_frame_action_ch_cnt; i++)
				m_excall.m_frame_action_ch_list[i].frame();
		
			// �X�e�[�W�̃t���[������
			m_excall.m_stage_list[TNM_STAGE_FRONT].frame(draw_futoku, draw_shadow, Gp_ini->excall_order);
			if (m_wipe.is_wipe_doing())	{
				m_excall.m_stage_list[TNM_STAGE_NEXT].frame(draw_futoku, draw_shadow, Gp_ini->excall_order);
			}
		}

		// ���b�Z�[�W�o�b�N�̃t���[������
		m_msg_back.frame(m_timer.local_game_time, draw_futoku, draw_shadow);
	}

	return true;
}

// ****************************************************************
// �t���[���O���[�o������
// ================================================================
//		�Q�[���^�C�}�[���~�܂��Ă�����A�𑜓x�؂�ւ����������肵�Ă�
//		��ɒʂ�t���[�������������܂��B
// ================================================================
bool C_tnm_eng::frame_global()
{
	// �}�E�X�J�[�\���̃t���[������
	tnm_cursor_frame(m_timer.real_time);

	// �T�E���h�̃t���[������
	m_sound.frame();

	// �T�E���h�̃{�����[�����X�V
	tnm_update_sound_volume(false);

	// �t�H���g�̐؂�ւ�
	tnm_update_font(false);

	// ���������̐؂�ւ�����
	bool wait_display_vsync_total = m_local.pod.wait_display_vsync_off_flag ? false : m_global.wait_display_vsync_def;
	tnm_set_wait_display_vsync(wait_display_vsync_total);

	// �X�N���[���T�C�Y�̐؂�ւ�����
	screen_size_proc();

	// �r�f�I�������̎g�p�ʂ����߂�
	const C_d3d_device_state& device_state = G_d3d.device.state();
	int video_memory_default_size = device_state.vm_size;
	m_global.video_memory_rest_size = (int)G_d3d.device->GetAvailableTextureMem();
	m_global.video_memory_use_size = (video_memory_default_size - m_global.video_memory_rest_size) / 1024 / 1024;
	if (m_global.video_memory_use_size > m_global.video_memory_use_max_size)
		m_global.video_memory_use_max_size = m_global.video_memory_use_size;

	// �t���[�����[�g���v�Z�i���b�Z�[�W�X�L�b�v�p�j
	{
		static DWORD frame_cnt = 0;
		static DWORD last = timeGetTime();
		DWORD now = timeGetTime();
		int second_past_cnt = (now - last) / 100;

		if (second_past_cnt > 0)	{
			m_global.frame_rate_100msec[m_global.frame_rate_100msec_index] = frame_cnt / second_past_cnt;
			m_global.frame_rate_100msec_index++;
			if (m_global.frame_rate_100msec_index >= 10)	{
				m_global.frame_rate_100msec_index = 0;
			}
			m_global.frame_rate_100msec_total = 0;
			for (int i = 0; i < 10; i++)	{
				m_global.frame_rate_100msec_total += m_global.frame_rate_100msec[i];
			}
			last += second_past_cnt * 100;
			frame_cnt = 0;
		}

		frame_cnt ++;
	}

	return true;
}

// ****************************************************************
// �t���[�����C������
// ================================================================
bool C_tnm_eng::frame_main_proc()
{
	while (1)	{

		if (false);

		// �X�N���v�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SCRIPT)	{

			// �u���[�N���ł��X�e�b�v���s�t���O�������Ă��Ȃ��ꍇ
			if (m_global.is_breaking && !m_global.break_step_flag)	{
				m_global.disp_flag = false;	// �`����s��Ȃ��I
				break;						// �����𔲂���
			}

			// �X�N���v�g����
			tnm_proc_script();
		}
		// �\��
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_DISP)	{
			tnm_pop_proc();		// ���������ɂ��̏������I������
			break;				// �\���̂��߂Ɉ�x���������𔲂���
		}
		// �L���v�`���[�̂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_CAPTURE_ONLY)	{
			tnm_pop_proc();		// ���������ɂ��̏������I������
			break;				// �\���̂��߂Ɉ�x���������𔲂���
		}
		// �Q�[���J�n���̃��C�v���J�n����
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_START_WIPE)	{
			if (!tnm_game_start_wipe_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �Q�[���I�����̃��C�v���J�n����
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_END_WIPE)	{
			if (!tnm_game_end_wipe_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �Q�[���^�C�}�[�J�n
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_GAME_TIMER_START)	{
			if (!tnm_game_timer_start_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_LOAD)	{
			if (!tnm_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �ȈՃ��[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_EASY_LOAD)	{
			if (!tnm_easy_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �N�C�b�N���[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_QUICK_LOAD)	{
			if (!tnm_quick_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �G���h���[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_LOAD)	{
			if (!tnm_end_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �C���i�[���[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_INNER_LOAD)	{
			if (!tnm_inner_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �o�b�N���O���[�h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BACKLOG_LOAD)	{
			if (!tnm_backlog_load_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���j���[�ɖ߂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_RETURN_TO_MENU)	{
			if (!tnm_return_to_menu_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �O�̑I�����ɖ߂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_RETURN_TO_SEL)	{
			if (!tnm_return_to_sel_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �^�C���E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_TIME_WAIT)	{
			if (!tnm_time_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �L�[�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_KEY_WAIT)	{
			if (!tnm_key_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �J�E���g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_COUNT_WAIT)	{
			if (!tnm_counter_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���b�Z�[�W�E�B���h�E�J���A�j���҂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_OPEN_WAIT)	{
			if (!tnm_mwnd_open_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���b�Z�[�W�E�B���h�E���A�j���҂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_CLOSE_WAIT)	{
			if (!tnm_mwnd_close_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �S�Ẵ��b�Z�[�W�E�B���h�E���A�j���҂�
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MWND_CLOSE_WAIT_ALL)	{
			if (!tnm_mwnd_close_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���b�Z�[�W�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MESSAGE_WAIT)	{
			if (!tnm_message_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���b�Z�[�W�L�[�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MESSAGE_KEY_WAIT)	{
			if (!tnm_message_key_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �m�x���q����
		//else if (m_local.cur_proc.type == TNM_PROC_TYPE_NOVEL_R)	{
		//	if (!tnm_novel_r_proc())
		//		break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		//}
		// �a�f�l�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BGM_WAIT)	{
			if (!tnm_bgm_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �a�f�l�t�F�[�h�A�E�g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_BGM_FADE_OUT_WAIT)	{
			if (!tnm_bgm_fade_out_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_KOE_WAIT)	{
			if (!tnm_koe_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���ʉ��E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCM_WAIT)	{
			if (!tnm_pcm_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���ʉ��`�����l���E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCMCH_WAIT)	{
			if (!tnm_pcmch_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���ʉ��`�����l���t�F�[�h�A�E�g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCMCH_FADE_OUT_WAIT)	{
			if (!tnm_pcmch_fade_out_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���ʉ��C�x���g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_PCM_EVENT_WAIT)	{
			if (!tnm_pcm_event_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���[�r�[�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_MOV_WAIT)	{
			if (!tnm_mov_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���C�v�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_WIPE_WAIT)	{
			if (!tnm_wipe_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �V�F�C�N�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SHAKE_WAIT)	{
			if (!tnm_shake_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �N�G�C�N�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_QUAKE_WAIT)	{
			if (!tnm_quake_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �C�x���g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_EVENT_WAIT)	{
			if (!tnm_event_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �S�C�x���g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_ALL_EVENT_WAIT)	{
			if (!tnm_all_event_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// ���[�r�[�I�u�W�F�N�g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_OBJ_MOV_WAIT)	{
			if (!tnm_obj_mov_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// E-mote �I�u�W�F�N�g�E�F�C�g
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_OBJ_EMOTE_WAIT)	{
			if (!tnm_obj_emote_wait_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �R�}���h
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_COMMAND)	{
			C_tnm_proc proc = m_local.cur_proc;		// �v���Z�X�̏����L�����Ă�����
			tnm_pop_proc();							// �v���Z�X�������
			if (!tnm_command_proc(proc.element.begin(), proc.element.end(), proc.arg_list_id, 
				proc.arg_list.get_sub().get(), 
				proc.arg_list.get_sub().get_end(), 
				NULL, FM_VOID)
				)
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �I��������
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL)	{
			if (!tnm_sel_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �{�^���I��������
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL_BTN)	{
			if (!tnm_sel_btn_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �{�^���I�u�W�F�N�g�I��������
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SEL_BTN_OBJ)	{
			if (!tnm_sel_btn_obj_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �Z�[�u�_�C�A���O���J��
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_SAVE_DIALOG)	{
			if (!tnm_save_dialog_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		// �G���h�Z�[�u�i�Q�i�K�ځj
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_SAVE)	{
			if (!tnm_end_save_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_END_GAME)	{
			if (!tnm_end_game_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_START_WARNING)	{
			if (!tnm_start_warning_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}
		else if (m_local.cur_proc.type == TNM_PROC_TYPE_TWEET_CAPTURE_AFTER)	{
			if (!tnm_tweet_capture_after_proc())
				break;		// �v���Z�X���I����ĂȂ��̂Ŕ�����
		}

		// ���̑��̏���
		else
			break;			// �����𔲂���
	}

	return true;
}

// ****************************************************************
// �^�C�}�[����
// ================================================================
bool C_tnm_eng::timer_proc()
{
	const int amari_unit = 16;

	// �O��̎������L��
	int real_time_last = m_timer.real_time;

	// ����̎��������v�Z����
	m_timer.real_time = ::timeGetTime();
	m_timer.real_time_past = m_timer.real_time - real_time_last;

	// ���v���C���Ԃ��v�Z����
	m_timer.global_real_time += m_timer.real_time_past;

	// �o�߂������Ԃ����߂�
	m_timer.local_real_time_past = m_timer.real_time_past * amari_unit;		// ��U�P�U�{�i�߂Ă���
	m_timer.local_game_time_past = m_timer.real_time_past * amari_unit;		// ��U�P�U�{�i�߂Ă���
	m_timer.local_wipe_time_past = m_timer.real_time_past * amari_unit;		// ��U�P�U�{�i�߂Ă���

	// Ctrl �L�[�ł�����΂��t���O�𗧂Ă�i�G�f�B�b�g�{�b�N�X���A�N�e�B�u�ȂƂ��͑����肵�Ȃ��j
	m_global.ctrl_skip_flag = false;
	m_global.cs_skip_flag = false;
	if (tnm_input_is_key_down(VK_SHIFT) && tnm_input_is_key_down(VK_CONTROL) && !m_global.focused_flag_on_edit_box)	{
		m_global.cs_skip_flag = true;
	}
	else if (tnm_input_is_key_down(VK_CONTROL) && !m_global.focused_flag_on_edit_box)	{
		m_global.ctrl_skip_flag = true;
	}

	// Alt ���j���[���J���Ă����Ԃ̓Q�[�����Ԃ͐i�܂Ȃ�
	// ������ Ctrl �֎~���͎��Ԃ�i�߂�
	if (m_global.alt_menu_wait_flag && !Gp_local->pod.ctrl_disable)	{
		m_global.alt_menu_wait_flag = false;
		m_timer.local_game_time_past = 0;
		m_timer.local_wipe_time_past = 0;
	}

	// �Q�[���^�C�}�[��~���͓����Ȃ�
	if (!m_global.game_timer_move_flag)	{
		m_timer.local_real_time_past = 0;
		m_timer.local_game_time_past = 0;
	}

	// �����蒆�̓Q�[�����Ԃ͂R�Q�{���Ői��
	if (tnm_is_skipping())	{
		m_timer.local_game_time_past *= 32;
		m_timer.local_wipe_time_past *= 32;
	}
	// �f�o�b�O�L�[�ɂ���Ď����Ԃ���уQ�[�����ԍ��̐i�݂͕ς��
	else if (m_global.debug_flag)	{
		if (tnm_input_is_key_down(VK_HOME) && tnm_input_is_key_down(VK_END))	{
			m_timer.local_real_time_past = 0;		// Home + End: ���Ԃ͐i�܂Ȃ�
			m_timer.local_game_time_past = 0;
			m_timer.local_wipe_time_past = 0;
		}
		else	{
			if (tnm_input_is_key_down(VK_INSERT))	{	m_timer.local_real_time_past *= 4;	m_timer.local_game_time_past *= 4;	m_timer.local_wipe_time_past *= 4;	}		// Insert:	�S�{��
			if (tnm_input_is_key_down(VK_DELETE))	{	m_timer.local_real_time_past *= 2;	m_timer.local_game_time_past *= 2;	m_timer.local_wipe_time_past *= 2;	}		// Delete:	�Q�{��
			if (tnm_input_is_key_down(VK_HOME))		{	m_timer.local_real_time_past /= 4;	m_timer.local_game_time_past /= 4;	m_timer.local_wipe_time_past /= 4;	}		// Home:	�P�^�S�{��
			if (tnm_input_is_key_down(VK_END))		{	m_timer.local_real_time_past /= 8;	m_timer.local_game_time_past /= 8;	m_timer.local_wipe_time_past /= 8;	}		// End:		�P�^�W�{��
		}
	}

	// �P�U�{�i�߂��̂łP�U�Ŋ���i���łɗ]����L�����Ă����j
	m_timer.local_real_time_amari += m_timer.local_real_time_past % amari_unit;
	m_timer.local_real_time_past = m_timer.local_real_time_past / amari_unit;
	if (m_timer.local_real_time_amari >= amari_unit)	{
		m_timer.local_real_time_amari -= amari_unit;
		m_timer.local_real_time_past += 1;
	}
	m_timer.local_game_time_amari += m_timer.local_game_time_past % amari_unit;
	m_timer.local_game_time_past = m_timer.local_game_time_past / amari_unit;
	if (m_timer.local_game_time_amari >= amari_unit)	{
		m_timer.local_game_time_amari -= amari_unit;
		m_timer.local_game_time_past += 1;
	}
	m_timer.local_wipe_time_amari += m_timer.local_wipe_time_past % amari_unit;
	m_timer.local_wipe_time_past = m_timer.local_wipe_time_past / amari_unit;
	if (m_timer.local_wipe_time_amari >= amari_unit)	{
		m_timer.local_wipe_time_amari -= amari_unit;
		m_timer.local_wipe_time_past += 1;
	}

	// �Q�[�����Ԃ�i�߂�
	m_timer.local_real_time += m_timer.local_real_time_past;
	m_timer.local_game_time += m_timer.local_game_time_past;
	m_timer.local_wipe_time += m_timer.local_wipe_time_past;

	return true;
}

// ****************************************************************
// �}�E�X�ړ�����
// ================================================================
bool C_tnm_eng::mouse_move_proc()
{
	// ���C���E�B���h�E����A�N�e�B�u�ȂƂ��͓������Ȃ��i�_�C�A���O��ɂ���ꍇ�̓A�N�e�B�u�Ȃ̂œ������܂��j
	if (!m_global.active_flag)
		return true;

	// ���C���E�B���h�E�Ƀt�H�[�J�X���Ȃ��Ƃ��͓������Ȃ��i�G�f�B�b�g�{�b�N�X��Ƀt�H�[�J�X������ꍇ�͓������܂���j
	if (!m_global.focused_flag)
		return true;

	// �֎~�t���O�������Ă���Γ������Ȃ�
	if (m_local.pod.cursor_move_by_key_disable)
		return true;

	// �t�F�b�`�������s���ꍇ�̓t�F�b�`����
	if (tnm_is_fetch_proc())	{

		// �t�F�b�`����
		tnm_fetch_proc();
	}
	else	{

		double speed = 0.5f;
		double mouse_move_left = m_global.mouse_move_left_amari;
		double mouse_move_right = m_global.mouse_move_right_amari;
		double mouse_move_up = m_global.mouse_move_up_amari;
		double mouse_move_down = m_global.mouse_move_down_amari;
		if (tnm_input_is_key_down(VK_LEFT))	{
			mouse_move_left += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_RIGHT))	{
			mouse_move_right += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_UP))	{
			mouse_move_up += (double)m_timer.real_time_past * speed;
		}
		if (tnm_input_is_key_down(VK_DOWN))	{
			mouse_move_down += (double)m_timer.real_time_past * speed;
		}
		int mouse_move_left_pixel = (int)mouse_move_left;
		int mouse_move_right_pixel = (int)mouse_move_right;
		int mouse_move_up_pixel = (int)mouse_move_up;
		int mouse_move_down_pixel = (int)mouse_move_down;
		m_global.mouse_move_left_amari = mouse_move_left - mouse_move_left_pixel;
		m_global.mouse_move_right_amari = mouse_move_right - mouse_move_right_pixel;
		m_global.mouse_move_up_amari = mouse_move_up - mouse_move_up_pixel;
		m_global.mouse_move_down_amari = mouse_move_down - mouse_move_down_pixel;
		if (mouse_move_left_pixel != 0 || mouse_move_right_pixel != 0 || mouse_move_up_pixel != 0 || mouse_move_down_pixel != 0)	{

			C_point new_pos = Gp_cur_input->mouse.pos;
			new_pos.x -= mouse_move_left_pixel;
			new_pos.x += mouse_move_right_pixel;
			new_pos.y -= mouse_move_up_pixel;
			new_pos.y += mouse_move_down_pixel;
			::SetCursorPos(new_pos.x, new_pos.y);

			// ���߂Ēl���擾
			::GetCursorPos(&new_pos);
			Gp_cur_input->mouse.pos = new_pos;
		}
	}

	return true;
}

// ****************************************************************
// �V�X�e���L�[����
// ================================================================
bool C_tnm_eng::system_key_proc()
{
	// �t���X�N���[�� ( Alt + Enter )
	// ���̏����̓X�N���v�g�ŋ֎~�ł��Ȃ�
	if (Gp_input->now.keyboard.key[VK_MENU].is_down())
	{
		if (Gp_input->now.keyboard.key[VK_RETURN].use_down_stock())
		{
			tnm_switch_screen_size_mode(false);
		}
	}

	// �Q�[������
	if (m_global.game_timer_move_flag)
	{
		// ���b�Z�[�W�o�b�N
		system_key_proc_msg_back();

		// ���b�Z�[�W�E�B���h�E���B��
		system_key_proc_hide_mwnd();

		// �V���[�g�J�b�g�L�[
		system_key_proc_shortcut();
	}

	return true;
}

// ****************************************************************
// �V�X�e���L�[�����F�V���[�g�J�b�g�L�[
// ================================================================
bool C_tnm_eng::system_key_proc_shortcut()
{
	// �d�w�R�[�����͎g���Ȃ�
	if (m_global.ex_call_flag)
		return true;

	// ���b�Z�[�W�o�b�N���͎g���Ȃ�
	if (m_msg_back.is_open())
		return true;

	// ���[�r�[�Đ����͎g���Ȃ�
	if (m_sound.m_mov.is_playing())
		return true;

	// ���b�Z�[�W�E�B���h�E���B���Ă���ꍇ�͎g���Ȃ�
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return true;
	
	// �V���[�g�J�b�g���֎~����Ă���ꍇ�͎g���Ȃ�
	if (m_local.pod.shortcut_disable)
		return true;

	// �V���[�g�J�b�g�L�[�̔���
	for (int i = 0; i < Gp_ini->shortcut_cnt; i++)
	{
		int key_code = Gp_ini->shortcut_key[i];
		if (0 <= key_code && key_code < 256)
		{
			int cond = Gp_ini->shortcut_cond[i];
			if (
				(cond == 0 && tnm_input_use_key_down(key_code)) || 
				(cond == 1 && tnm_input_use_key_down_up(key_code))
			)	{
				if (!Gp_ini->shortcut_scn[i].empty())
				{
					if (!Gp_ini->shortcut_cmd[i].empty())
					{
						tnm_scene_proc_call_user_cmd(Gp_ini->shortcut_scn[i], Gp_ini->shortcut_cmd[i], FM_VOID, true, false);
					}
					else
					{
						tnm_scene_proc_farcall(Gp_ini->shortcut_scn[i], Gp_ini->shortcut_z_no[i], FM_VOID, true, false);
					}
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// �V�X�e���L�[�����F���b�Z�[�W�o�b�N
// ================================================================
bool C_tnm_eng::system_key_proc_msg_back()
{
	// �d�w�R�[�������g����
	// �d�w�R�[������ syscom.open_msg_back �Ń��b�Z�[�W�o�b�N���J�����Ƃ����邽�߁A
	// �uPageUp �{�^���Ń��b�Z�[�W�o�b�N���J���v�͋֎~����ׂ������A���b�Z�[�W�o�b�N�̑��쎩�͍̂s�Ȃ��K�v������

	// ���[�r�[�Đ����̓��b�Z�[�W�o�b�N�͎g���Ȃ�
	if (m_sound.m_mov.is_playing())
		return true;

	// �d�w�R�[���F���b�Z�[�W�E�B���h�E�������̏ꍇ�͎g���Ȃ�
	if (tnm_excall_is_hide_mwnd())
		return true;

	// �d�w�R�[���F���b�Z�[�W�o�b�N���̏ꍇ
	if (tnm_excall_is_msg_back() && Gp_local->cur_proc.type == TNM_PROC_TYPE_MSG_BACK)
	{
		// ���b�Z�[�W�o�b�N���\������Ă���ꍇ
		if (!Gp_local->pod.msg_back_disp_off)
		{
			// �X�N���[���̗]�g���������Ă��邩�ǂ����𔻒肷��
			if (!m_msg_back.check_aftereffects())
			{
				// PageUp �L�[�Ń^�[�Q�b�g���ړ�
				if (tnm_input_is_key_down(VK_PRIOR))
				{
					m_msg_back.page_up_key_down();
				}
				// PageDown �L�[�Ń^�[�Q�b�g���ړ�
				if (tnm_input_is_key_down(VK_NEXT))
				{
					m_msg_back.page_dw_key_down();
				}
				// �z�C�[���t�o�Ń^�[�Q�b�g���ړ�
				if (Gp_input->now.mouse.wheel.use_up_stock())
				{
					m_msg_back.target_up_for_wheel();
				}
				// �z�C�[���c�n�v�m�Ń^�[�Q�b�g���ړ�
				if (Gp_input->now.mouse.wheel.use_down_stock())
				{
					m_msg_back.target_down_for_wheel();
				}
			}

			// �L�����Z���L�[�ŕ���
			if (tnm_input_use_key_down(VK_EX_CANCEL) || tnm_input_use_key_down(VK_SPACE))
			{
				// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N�����
				tnm_syscom_close_msg_back();
				// �I�[�g���[�h�J�E���^�����Z�b�g
				Gp_global->auto_mode_count_start_time = Gp_timer->real_time;	// ������
			}
		}
	}
	// �d�w�R�[���F���b�Z�[�W�o�b�N���ł͂Ȃ��ꍇ
	else
	{
		// �d�w�R�[���F���b�Z�[�W�E�B���h�E�������̏ꍇ�͎g���Ȃ�
		// �d�w�R�[���F�d�w�R�[�����̏ꍇ�͎g���Ȃ�
		if (!tnm_excall_is_hide_mwnd() && !tnm_excall_is_excall())
		{
			// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N���\�H
			if (tnm_syscom_msg_back_is_enable())
			{
				// PageUp �L�[���}�E�X�z�C�[���Ń��b�Z�[�W�o�b�N���J��
				if (tnm_input_use_key_down(VK_PRIOR) || Gp_input->now.mouse.wheel.use_up_stock())
				{
					// �V�X�e���R�}���h�F���b�Z�[�W�o�b�N���J��
					tnm_syscom_open_msg_back();
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// �V�X�e���L�[�����F���b�Z�[�W�E�B���h�E���B��
// ================================================================
bool C_tnm_eng::system_key_proc_hide_mwnd()
{
	// ���[�r�[�Đ����́u���b�Z�[�W�E�B���h�E���B���v�͎g���Ȃ�
	if (m_sound.m_mov.is_playing())
		return true;

	// �d�w�R�[���F�u�d�w�R�[���v���́u���b�Z�[�W�E�B���h�E���B���v�͎g���Ȃ�
	if (tnm_excall_is_excall())
		return true;

	// �d�w�R�[���F�u���b�Z�[�W�o�b�N�v���́u���b�Z�[�W�E�B���h�E���B���v�͎g���Ȃ�
	if (tnm_excall_is_msg_back())
		return true;

	// �d�w�R�[���F�u���b�Z�[�W�E�B���h�E���B���v���̏ꍇ
	if (tnm_excall_is_hide_mwnd())
	{
		// ����L�[�A�L�����Z���L�[�A�X�y�[�X�L�[�ŕ��A
		// ��down_up �d�v�Bdown �������ƁA���� up �ŕ���{�^�����܂������Ă��܂��B
		if (tnm_input_use_key_down_up(VK_EX_DECIDE) || tnm_input_use_key_down_up(VK_EX_CANCEL) || tnm_input_use_key_down_up(VK_SPACE))
		{
			// �V�X�e���R�}���h�F�u���b�Z�[�W�E�B���h�E���B���v���I�t
			tnm_syscom_hide_mwnd_set_onoff_flag(false);
			// �I�[�g���[�h�J�E���^�����Z�b�g
			m_global.auto_mode_count_start_time = m_timer.real_time;	// ������
		}
	}

	// �d�w�R�[���F�u���b�Z�[�W�E�B���h�E���B���v���łȂ��ꍇ
	else
	{
		// �V�X�e���R�}���h�F�u���b�Z�[�W�E�B���h�E���B���v���g���邩
		if (tnm_syscom_hide_mwnd_is_enable())
		{
			// �X�y�[�X�L�[�Ń��b�Z�[�W�E�B���h�E���B��
			if (tnm_input_use_key_down(VK_SPACE))
			{
				// �V�X�e���R�}���h�F��������~�߂�
				tnm_syscom_read_skip_set_onoff_flag(false);
				// �V�X�e���R�}���h�F�u���b�Z�[�W�E�B���h�E���B���v
				tnm_syscom_hide_mwnd_set_onoff_flag(true);
			}
		}
	}

	return true;
}

// ****************************************************************
// �f�o�b�O�L�[����
// ================================================================
bool C_tnm_eng::debug_key_proc()
{
	// �f�o�b�O�ł̂ݗL��
	if (m_global.debug_flag)
	{
		// �ŏ�����͂��߂� ( F1 )
		if (tnm_input_use_key_down(VK_F1))	{
			tnm_syscom_restart_from_start();
		}
		// �u���[�N ( F3 )
		if (tnm_input_use_key_down(VK_F3))	{
			tnm_syscom_change_break();
		}
		// �X�e�b�v���s ( 1 )
		if (m_global.is_breaking && tnm_input_check_key_down('1'))	{
			tnm_syscom_do_one_step();
		}
		// �G���[�_�C�A���O ( F8 )
		if (tnm_input_use_key_down(VK_F8))	{
			tnm_dlg_open_error_info_dialog();
		}
		// ss ���J�� ( O )
		if (tnm_input_use_key_down('O'))	{

			// �V�[�����G�f�B�^�ŊJ��
			tnm_open_by_editor(true);
		}
		// koe ���J�� ( K )
		if (tnm_input_use_key_down('K'))	{

			// �����֘A�t���ŊJ��
			tnm_open_koe(true);
		}
		// ���݂̃e�L�X�g���N���b�v�{�[�h�ɃR�s�[���� ( P )
		if (tnm_input_use_key_down('P'))	{

			TSTR str;
			str += _T("����������������������������������������������������������������\n");
			str += _T("��");

			// �Q�[����
			str += Gp_ini->game_name;

			// �V�[���^�C�g��
			if (!Gp_local->scene_title.empty())
				str += _T(" ") + Gp_local->scene_title;

			// ���t
			SYSTEMTIME system_time;
			GetLocalTime(&system_time);
			str += _T(" [") + get_sys_time_str() + _T("]");

			// ���b�Z�[�W�o�b�N���̓��b�Z�[�W�o�b�N����擾���܂��B
			if (Gp_msg_back->is_open())	{
				str += _T("�i�����b�Z�[�W�o�b�N���j\n");

				// �V�[����
				int scn_no = Gp_msg_back->get_scn_no();
				if (scn_no >= 0)	{
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					int line_no = Gp_msg_back->get_line_no();
					str += scn_name + _T(".ss (") + tostr(line_no) + _T("�s��)\n\n");
				}

				// ���b�Z�[�W
				int koe_no = Gp_msg_back->get_koe_no();				// ��
				if (koe_no >= 0)	{
					str += str_format(_T("KOE(%09d)"), koe_no);
				}
				TSTR name = Gp_msg_back->get_original_name();		// ���O
				if (!name.empty())	{
					str += _T("�y") + name + _T("�z\n");
				}
				TSTR msg = Gp_msg_back->get_debug_msg();			// ���b�Z�[�W
				if (!msg.empty())	{
					str += msg + _T("\n");
				}
			}
			else	{
				str += _T("\n");

				// �V�[����
				int scn_no = Gp_lexer->get_cur_scn_no();
				if (scn_no >= 0)	{
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					int line_no = Gp_lexer->get_cur_line_no();
					str += scn_name + _T(".ss (") + tostr(line_no) + _T("�s��)\n\n");
				}

				// ���݂̃��b�Z�[�W
				C_elm_mwnd* p_mwnd = tnm_get_cur_mwnd();
				if (p_mwnd)	{

					int koe_no = p_mwnd->get_koe_no();				// ��
					if (koe_no >= 0)	{
						str += str_format(_T("KOE(%09d)"), koe_no);
					}
					TSTR name = p_mwnd->get_name();					// ���O
					if (!name.empty())	{
						str += _T("�y") + name + _T("�z\n");
					}
					TSTR msg = p_mwnd->get_debug_msg();				// ���b�Z�[�W
					if (!msg.empty())	{
						str += msg + _T("\n");
					}
				}
			}

			str += _T("\n");
			::str_copy_to_clip_board_in_MB(str);
		}
	}

	return true;
}

// ****************************************************************
// �{�^������
// ================================================================
bool C_tnm_eng::button_proc()
{
	// �{�^���}�l�[�W���̃t���[���������i�{�^���̓o�^�́A�K����������ōs���Ă��������I�j
	m_btn_mng.frame_init();

	// �Q�[���������Ă��Ȃ��ꍇ�͏������Ȃ�
	if (!m_global.game_timer_move_flag)
		return true;

	// �X�e�[�W�̃{�^���t���[������������
	int stage_cnt = m_stage_list.get_size();
	for (int i = 0; i < stage_cnt; i++)
		m_stage_list[i].button_frame_init();

	// �d�w�R�[���̃{�^���t���[������������
	if (m_excall.is_ready())	{

		// �X�e�[�W�̃{�^���t���[������������
		int excall_stage_cnt = m_excall.m_stage_list.get_size();
		for (int i = 0; i < excall_stage_cnt; i++)
			m_excall.m_stage_list[i].button_frame_init();
	}

	// �{�^���o�^����
	button_regist_proc();

	return true;
}

// ****************************************************************
// �{�^���o�^����
// ================================================================
struct S_pair
{
	int		index;
	int		value;
};

bool pair_value_less_func(const S_pair& lhs, const S_pair& rhs)
{
	return lhs.value < rhs.value;
}
bool pair_value_greater_func(const S_pair& lhs, const S_pair& rhs)
{
	return lhs.value > rhs.value;
}

bool C_tnm_eng::button_regist_proc()
{
	// ���Ȃ��Ȃ��̃}�}�������[�h�������悤�� Gameexe.ini �Őݒ�ł���悤�ɂ��Ȃ��Ɓc�B
	// 2016/05/21 �Ώ��ς݁B���b�Z�[�W�o�b�N����̃I�u�W�F�N�g�̂ݓ�����悤�ɂ��܂����B

	// ���b�Z�[�W�o�b�N���܂��͂d�w�R�[�����̏ꍇ
	if (m_msg_back.is_open() || m_global.ex_call_flag)
	{
		// ���b�Z�[�W�o�b�N���J���Ă���
		if (m_msg_back.is_open() && !Gp_local->pod.msg_back_disp_off)
		{
			// ���b�Z�[�W�o�b�N�̃{�^���o�^����
			m_msg_back.regist_button(m_excall.is_ready() ? Gp_ini->excall_order : INT_MIN);
		}
		// �d�w�X�e�[�W����������Ă���
		if (m_excall.is_ready())
		{
			// �X�e�[�W�̃{�^���o�^����
			m_excall.m_stage_list[TNM_STAGE_FRONT].regist_button(m_msg_back.is_open() ? Gp_ini->msg_back_order : INT_MIN);
		}
	}
	// ���̑��̏ꍇ�̓t�����g�ɑ΂��ď���
	else
	{
		// ���b�Z�[�W�o�b�N���J���Ă���ꍇ�̓��b�Z�[�W�o�b�N����̃{�^���̂݉ғ�

		// �X�e�[�W�̃{�^���o�^����
		m_stage_list[TNM_STAGE_FRONT].regist_button(INT_MIN);
	}

	return true;
}

// ****************************************************************
// �{�^�������蔻�菈��
// ================================================================
bool C_tnm_eng::button_hit_test_proc()
{
	// �Q�[���������Ă��Ȃ��ꍇ�͏������Ȃ�
	if (!m_global.game_timer_move_flag)
		return true;

	m_btn_mng.hit_test_proc();

	return true;
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
bool C_tnm_eng::button_event_proc()
{
	// �Q�[���������Ă��Ȃ��ꍇ�͏������Ȃ�
	if (!m_global.game_timer_move_flag)
		return true;

	// ���b�Z�[�W�o�b�N���J���Ă���
	if (m_msg_back.is_open())	{

		// ���b�Z�[�W�o�b�N�̃{�^���C�x���g����
		if (!Gp_local->pod.msg_back_disp_off)	{
			m_msg_back.button_event();
		}
	}

	// �d�w�R�[�����ielse �ł͂Ȃ��I�}�}�������[�h�΍�j
	if (m_global.ex_call_flag)	{

		// �d�w�R�[������������Ă���ꍇ
		if (m_excall.is_ready())	{

			// �I�𒆂̏ꍇ�̓^�[�Q�b�g�̂l�v���擾
			C_elm_mwnd* p_mwnd = NULL;
			if (Gp_local->cur_proc.type == TNM_PROC_TYPE_SEL)
				p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

			// �I�u�W�F�N�g�̃{�^���C�x���g����
			m_excall.m_stage_list[TNM_STAGE_FRONT].button_event(p_mwnd);

			// �O���[�v�̃L�����Z������
			{
				C_elm_group_list* p_gl = &m_excall.m_stage_list[TNM_STAGE_FRONT].m_group_list;
				int excall_group_cnt = p_gl->get_size();

				// �D��x���ɕ��ёւ���
				ARRAY<S_pair> pair_list(excall_group_cnt);
				for (int i = 0; i < excall_group_cnt; i++)	{
					pair_list[i].index = i;
					pair_list[i].value = (*p_gl)[i].get_cancel_priority();
				}
				std::stable_sort(pair_list.begin(), pair_list.end(), pair_value_greater_func);

				// �D��x���ɃL�����Z������
				for (int i = 0; i < excall_group_cnt; i++)	{
					int index = pair_list[i].index;
					(*p_gl)[index].cancel_check();
				}
			}
		}
	}

	// ���̑��̏ꍇ�̓t�����g�ɑ΂��ď���
	else	{

		// �I�𒆂̏ꍇ�̓^�[�Q�b�g�̂l�v���擾
		C_elm_mwnd* p_mwnd = NULL;
		if (Gp_local->cur_proc.type == TNM_PROC_TYPE_SEL)
			p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_proc.element.begin(), Gp_local->cur_proc.element.end(), true);

		// �t�����g�̃{�^���C�x���g����
		m_stage_list[TNM_STAGE_FRONT].button_event(p_mwnd);

		// �O���[�v�̃L�����Z������
		{
			C_elm_group_list* p_gl = &m_stage_list[TNM_STAGE_FRONT].m_group_list;
			int group_cnt = p_gl->get_size();

			// �D��x���ɕ��ёւ���
			ARRAY<S_pair> pair_list(group_cnt);
			for (int i = 0; i < group_cnt; i++)	{
				pair_list[i].index = i;
				pair_list[i].value = (*p_gl)[i].get_cancel_priority();
			}
			std::stable_sort(pair_list.begin(), pair_list.end(), pair_value_greater_func);

			// �D��x���ɃL�����Z������
			for (int i = 0; i < group_cnt; i++)	{
				int index = pair_list[i].index;
				(*p_gl)[index].cancel_check();
			}
		}
	}

	return true;
}

// ****************************************************************
// �L�����Z���R�[������
// ================================================================
//		�E�N���b�N���������Ƃ��ɃL�����Z���V�[�����Ăт܂��B
// ================================================================
bool C_tnm_eng::cancel_call_proc()
{
	// �Q�[���������Ă��Ȃ��ꍇ�́u�L�����Z���R�[���v�͎g���Ȃ�
	if (!m_global.game_timer_move_flag)
		return true;

	// ���[�r�[�Đ����́u�L�����Z���R�[���v�͎g���Ȃ�
	if (m_sound.m_mov.is_playing())
		return true;

	// �d�w�R�[���F�u�d�w�R�[���v���́u�L�����Z���R�[���v�͎g���Ȃ�
	if (tnm_excall_is_excall())
		return true;

	// �d�w�R�[���F�u���b�Z�[�W�o�b�N�v���́u�L�����Z���R�[���v�͎g���Ȃ�
	if (tnm_excall_is_msg_back())
		return true;

	// �d�w�R�[���F�u���b�Z�[�W�E�B���h�E���B���v���́u�L�����Z���R�[���v�͎g���Ȃ�
	if (tnm_excall_is_hide_mwnd())
		return true;

	// ���[�J���F�V�X�e�����j���[���g���Ȃ��ꍇ�́u�L�����Z���R�[���v�͎g���Ȃ�
	if (m_local.pod.syscom_menu_disable)
		return true;

	// �t���b�N
	if (Gp_input->now.mouse.left.check_flick_stock())
	{
		double angle = Gp_input->now.mouse.left.get_flick_angle();

		for (int i = 0; i < Gp_ini->flick_scene_cnt; i++)
		{
			if (Gp_ini->flick_scene_angle[i] == 1)		// �� / 2
			{
				if (angle < -D3DX_PI / 2 || D3DX_PI / 2 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 2)		// �� / 2
			{
				if (-D3DX_PI / 2 <= angle && angle < D3DX_PI / 2)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 3)		// �� / 2
			{
				if (0 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 4)		// �� / 2
			{
				if (angle < 0)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 5)		// �� / 4
			{
				if (angle < -D3DX_PI * 3 / 4 || D3DX_PI * 3 / 4 <= angle)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 6)		// �� / 4
			{
				if (D3DX_PI * 1 / 4 <= angle && angle < D3DX_PI * 3 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 7)		// �� / 4
			{
				if (-D3DX_PI * 1 / 4 <= angle && angle < D3DX_PI * 1 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}
			else if (Gp_ini->flick_scene_angle[i] == 8)		// �� / 4
			{
				if (-D3DX_PI * 3 / 4 <= angle && angle < -D3DX_PI * 1 / 4)
				{
					Gp_input->now.mouse.left.use_flick_stock();

					// �V�X�e���R�}���h�F��������~�߂�
					tnm_syscom_read_skip_set_onoff_flag(false);
					// �t���b�N�V�[�����Ă�
					tnm_scene_proc_call_flick_scene(i);
				}
			}

		}
	}

	// �L�����Z���L�[�ŃV�X�e���R�}���h�V�[�����J��
	if (tnm_input_use_key_down_up(VK_EX_CANCEL))
	{
		// �V�X�e���R�}���h�F��������~�߂�
		tnm_syscom_read_skip_set_onoff_flag(false);
		// �V�X�e���R�}���h�F�V�X�e���R�}���h���J��
		tnm_syscom_open();
	}

	return true;
}

// ****************************************************************
// �t���[���A�N�V��������
// ================================================================
bool C_tnm_eng::frame_action_proc()
{
	// ================================================================
	// �����[�h����R�[������
	// ================================================================
	if (m_global.do_load_after_call_flag)	{

		if (!Gp_ini->load_after_call_scene.empty())	{

			// �V�[�����R�[��
			tnm_scene_proc_farcall(Gp_ini->load_after_call_scene, Gp_ini->load_after_call_z_no, FM_VOID, false, true);

			// �ēx�X�N���v�g�����s����
			tnm_proc_script();
		}
	}

	// ================================================================
	// ���I�����J�n�R�[������
	// ================================================================
	if (m_global.sel_start_call_flag)	{

		m_stage_list[TNM_STAGE_FRONT].m_btn_sel.do_sel_start_call();
	}

	// ================================================================
	// ���t���[���A�N�V��������
	// ================================================================

	// �Q�[���^�C�}�[�������Ă���܂��̓t���[���A�N�V���������s����t���O�������Ă���ꍇ�̂ݏ���
	if (m_global.game_timer_move_flag || m_global.do_frame_action_flag)	{

		// �`����s���Ƃ��̂ݏ���
		if (m_global.disp_flag)	{

			// �t���[���A�N�V����
			m_frame_action.do_action();

			// �t���[���A�N�V�����`�����l��
			int frame_action_ch_cnt = m_frame_action_ch_list.get_size(); 
			for (int i = 0; i < frame_action_ch_cnt; i++)
				m_frame_action_ch_list[i].do_action();

			// �X�e�[�W�̃t���[���A�N�V����
			int stage_cnt = m_stage_list.get_size();
			for (int i = 0; i < stage_cnt; i++)
				m_stage_list[i].frame_action();

			// �d�w�R�[���̃t���[���A�N�V����
			if (m_excall.is_ready())	{

				// �t���[���A�N�V����
				m_excall.m_frame_action.do_action();

				// �t���[���A�N�V�����`�����l��
				int excall_frame_action_ch_cnt = m_excall.m_frame_action_ch_list.get_size(); 
				for (int i = 0; i < excall_frame_action_ch_cnt; i++)
					m_excall.m_frame_action_ch_list[i].do_action();
			
				// �X�e�[�W�̃t���[���A�N�V����
				int excall_stage_cnt = m_excall.m_stage_list.get_size();
				for (int s = 0; s < excall_stage_cnt; s++)
					m_excall.m_stage_list[s].frame_action();
			}
		}
	}

	return true;
}

// ****************************************************************
// �X�N���v�g���͂̏���
// ================================================================
bool C_tnm_eng::script_input_proc()
{
	// ���͂̌��݂̏�Ԃ��ڂ�����
	m_script_input.next = Gp_input->now;
	// ���}�E�X�̈ʒu�����͓���
	m_script_input.next.mouse.pos = Gp_cur_input->mouse.pos;

	// �t���[������
	m_script_input.frame();

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y����
// ================================================================
bool C_tnm_eng::screen_size_proc()
{
	WINDOWPLACEMENT wp = {0};
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(Gp_wnd->get_handle(), &wp);
	C_rect window_rect = wp.rcNormalPosition;

	// �E�B���h�E�T�C�Y���ύX���ꂽ�i�^�u���b�g����]�����Ƃ��Ȃǁj
	if (m_global.total_window_size != window_rect.size())
	{
		tnm_change_screen_size_mode_by_force(true);		// �����I�ɕύX����
	}

	// �E�B���h�E���f�X�N�g�b�v�Ɏ��܂�悤�ɂ���
	Gp_wnd->rep_window_pos_outside_desktop();

	// ���v�Q�[����ʃT�C�Y�����߂�
	{
		m_global.total_game_screen_pos = C_point(0, 0);
		m_global.total_game_screen_size = m_global.total_screen_size;

		// �E�B���h�E���[�h
		if (m_config.screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)
		{
//			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
//			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
//			m_global.total_game_screen_pos.x = 0;
//			m_global.total_game_screen_pos.y = 0;

			float scale_x = (float)m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			float scale_y = (float)m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			float scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = (int)ceil(m_global.game_screen_size.cx * scale / 1000);
			m_global.total_game_screen_size.cy = (int)ceil(m_global.game_screen_size.cy * scale / 1000);
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���[���[�h
		else if (m_config.screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
		{
//			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
//			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
//			m_global.total_game_screen_pos.x = 0;
//			m_global.total_game_screen_pos.y = 0;

			float scale_x = (float)m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			float scale_y = (float)m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			float scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = (int)ceil(m_global.game_screen_size.cx * scale / 1000);
			m_global.total_game_screen_size.cy = (int)ceil(m_global.game_screen_size.cy * scale / 1000);
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F����
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_NORMAL)
		{
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F�S���
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_FIT)
		{
			int scale_x = m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			int scale_y = m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			int scale = std::min(scale_x, scale_y);
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F�S��ʃt�B�b�g
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_UDLRFIT)
		{
			m_global.total_game_screen_size.cx = m_global.total_screen_size.cx;
			m_global.total_game_screen_size.cy = m_global.total_screen_size.cy;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F�㉺�t�B�b�g
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_UDFIT)
		{
			int scale = m_global.total_screen_size.cy * 1000 / m_global.game_screen_size.cy;
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F���E�t�B�b�g
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_LRFIT)
		{
			int scale = m_global.total_screen_size.cx * 1000 / m_global.game_screen_size.cx;
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * scale / 1000;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * scale / 1000;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2;
		}
		// �t���X�N���[�����[�h�F�J�X�^��
		else if (m_config.fullscreen_mode == TNM_FULLSCREEN_MODE_CUSTOM)
		{
			m_global.total_game_screen_size.cx = m_global.game_screen_size.cx * m_config.fullscreen_scale.cx / 100;
			m_global.total_game_screen_size.cy = m_global.game_screen_size.cy * m_config.fullscreen_scale.cy / 100;
			m_global.total_game_screen_pos.x = (m_global.total_screen_size.cx - m_global.total_game_screen_size.cx) / 2 + m_global.game_screen_size.cx * m_config.fullscreen_move.cx / 100;
			m_global.total_game_screen_pos.y = (m_global.total_screen_size.cy - m_global.total_game_screen_size.cy) / 2 + m_global.game_screen_size.cy * m_config.fullscreen_move.cy / 100;
		}
	}

	// �Q�[���X�N���[����`���ς������F�X�ƍX�V
	if (m_global.total_game_screen_pos != m_global.total_game_screen_pos_last || m_global.total_game_screen_size != m_global.total_game_screen_size_last)	{

		// ���[�r�[�̋�`���X�V
		//m_sound.m_mov.update_rect();

		// �g�k�䗦���v�Z
		float size_rate_x = (float)m_global.total_game_screen_size.cx / m_global.game_screen_size.cx;
		float size_rate_y = (float)m_global.total_game_screen_size.cy / m_global.game_screen_size.cy;

		m_sound.m_mov.set_size_rate(size_rate_x, size_rate_y);
		m_sound.m_mov.set_total_game_screen_pos(m_global.total_game_screen_pos);
		m_sound.m_mov.update_rect_org();

		// �G�f�B�b�g�{�b�N�X
		for (int i = 0; i < m_editbox_list.get_size(); i++)
			m_editbox_list[i].update_rect();
	}

	return true;
}

