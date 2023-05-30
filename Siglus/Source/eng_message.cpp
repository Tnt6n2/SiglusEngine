#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_namae.h"

#include	"element/elm_flag.h"
#include	"element/elm_stage.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_btn_sel.h"

#include	"element/elm_msg_back.h"

#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/flow_command.h"
#include	"engine/eng_flag.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���݂̃��b�Z�[�W�E�B���h�E���擾
// ================================================================
C_elm_mwnd* tnm_get_cur_mwnd()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_mwnd.begin(), Gp_local->cur_mwnd.end(), true);

	return p_mwnd;
}

C_elm_mwnd* tnm_get_cur_sel_mwnd()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_sel_mwnd.begin(), Gp_local->cur_sel_mwnd.end(), true);

	return p_mwnd;
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W�u���b�N�̐擪����
// ================================================================
// ���b�Z�[�W�u���b�N�́A���A��O���A���O�A���b�Z�[�W�A�̉�̂��Ƃł��B
// ���̏��ɋL�q���ꂽ�ꍇ�A�擪�́u���v�R�}���h�̒��O�ɐ擪�������s���܂��B
// �擪�����̍Ō�ŃZ�[�u�|�C���g�𗧂ĂĂ��܂��B
// �N���A�ȂǁA�Z�[�u�O�ɍs���ė~�������e�������ŋL�q���܂��B
// ���̍Đ��ȂǁA���[�h��ɂ��s���ė~���������́A�e�������ɋL�q���܂��B
// ================================================================
void tnm_msg_proc_start_msg_block(C_elm_mwnd* p_mwnd)
{
	// ���ɃX�^�[�g�������s���Ă���Ή������Ȃ�
	if (p_mwnd->is_msg_block_started())
		return;

	// �N���A�����t���O�������Ă���Ȃ�N���A����
	if (p_mwnd->get_clear_ready_flag())
	{
		p_mwnd->clear();

		// �t�����b�Z�[�W���N���A
		Gp_local->cur_full_message.clear();
	}

	// ���b�Z�[�W�����炩�ɓ��肫��Ȃ��ꍇ�̓N���A����i�I�[�o�[�t���[�p�j
	else if (p_mwnd->get_over_flow_check_size() > 0)
	{
		if (!p_mwnd->add_msg_check(false))
		{
			p_mwnd->clear();

			// �t�����b�Z�[�W���N���A
			Gp_local->cur_full_message.clear();
		}
	}

	// �m�x���ɑ΂��ẴN���A����
	p_mwnd->clear_for_novel_one_msg();

	// �Ō�ɕ�����\���������Ԃ�ݒ�
	p_mwnd->set_last_moji_disp_time();

	// ���b�Z�[�W�o�b�N��i�߂�
	tnm_msg_back_next();

	// ================================================================
	// ����L�[���N���A����
	// ----------------------------------------------------------------
	// ���̏����͕K�{�ł��B���̏������Ȃ��ƁA�Â��N���b�N�Ń��b�Z�[�W���X�L�b�v����Ă��܂��܂��B
	// �����̏����́A�`_WAIT �v���Z�X�̐擪�ŃL�[�N���A���s�����Ƃŉ�����Ă��܂����A
	// MSG_WAIT �͓���ŁA�����ŃL�[�N���A���s��Ȃ��܂���B
	// �����ŃL�[�N���A���s���ƁA���b�Z�[�Wnl���b�Z�[�Wnl�c�Ȃǂ̏����ŁA���b�Z�[�W���ƂɃL�[���N���A����Ă��܂��A
	// �u�N���b�N�ŃX�L�b�v����v�@�\�������Ȃ��Ȃ邽�߂ł��B
	// ----------------------------------------------------------------
	Gp_input->clear_ex_key(VK_EX_DECIDE);

	// ���b�Z�[�W�u���b�N�̐擪��������
	p_mwnd->set_msg_block_start_flag();

	// �Z�[�u�|�C���g���N���A����
	tnm_init_local_save();

	// �Z�[�u�|�C���g�𗧂Ă�
	if (!Gp_local->pod.dont_set_save_point)
	{
		tnm_set_save_point();
	}
		
	// ��BGM ���Ȃ��ĂȂ��`�F�b�N
	if (!tnm_check_bgm_playing())
	{ 
		Gp_global->__bgm_stop_cntr++;
		if (Gp_global->__bgm_stop_cntr >= 10)
		{
			//write_debug_log(_T("��10�s�ȏ� BGM �����Ă��܂���I"));
			//tnm_set_error(TNM_ERROR_TYPE_WARNING, _T("��10�s�ȏ� BGM �����Ă��܂���I"));
			Gp_global->__bgm_stop_cntr = 0;
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W�o�o�u���b�N�̐擪����
// ================================================================
// ���b�Z�[�W�o�o�u���b�N�́APP �R�}���h�ŋ�؂��郁�b�Z�[�W�P�ʂł��B
// �I�[�g���[�h��A���b�Z�[�W���x�̌v�Z�P�ʂł��B
// ================================================================
void tnm_msg_proc_start_msg_pp_block(C_elm_mwnd* p_mwnd)
{
	// �I�[�g���[�h�J�E���g���I��
	Gp_global->is_auto_mode_count_start = false;

	// ���̍Đ����I����Ă���̑҂����ԃJ�E���g�I��
	Gp_global->is_koe_only_count_start = false;

	// �����܂ŃI�[�g���[�h��i�߂�
	p_mwnd->set_auto_mode_end_moji_cnt();	// �������}���`���b�Z�[�W

	// ���b�Z�[�W���x�v�Z�����Z�b�g
	p_mwnd->set_progress_start_flag(false);
}

// ****************************************************************
// ���b�Z�[�W�F����ݒ�
// ================================================================
void tnm_msg_proc_koe(C_elm_mwnd* p_mwnd, int koe_no, int chara_no, int rf_scn_no, int rf_flag_no, bool wait_flag, bool key_skip_flag, bool no_auto_mode_flag, bool return_value_flag)
{
	if (!p_mwnd)
		return;

	// ���Ǒ����菈���i���b�Z�[�W�u���b�N�Ɏ����Ă����������ǁA�S�ĂɊ��ǃt���O��������̂��Ȃ��c�j
	{
		// �u���݂̊��ǃt���O�ԍ��v��ݒ�i���Ǒ����肪�\���̔���Ɏg���܂��j
		Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
		Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

		// ������̍X�V
		tnm_update_read_skip();

		// ������������J�n����
		tnm_update_start_skip();
	}

	// �����Đ����邩�ǂ����𔻒肷��
	bool play_flag = tnm_is_koe_play(chara_no);

	// ���Z�Đ��̃��[�g�����߂�
	int jitan_rate = tnm_is_auto_mode() ?
		(Gp_config->jitan_auto_mode_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL): 
		(Gp_config->jitan_normal_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL);

	// �����Đ�����i�Đ����Ȃ��ꍇ�̓t�@�C���`�F�b�N�̂݁j
	tnm_play_koe(koe_no, chara_no, jitan_rate, 0, wait_flag, key_skip_flag, return_value_flag, false, play_flag ? false : true);

	// ���b�Z�[�W�ɐ���ݒ�
	p_mwnd->set_koe(koe_no, play_flag, no_auto_mode_flag);

	// ���a�f�l�t�F�[�h�Q���K�v�t���O�𗧂Ă�
	Gp_global->bgmfade2_need_flag = true;

	// ���b�Z�[�W�Ɋ��ǃt���O��ǉ��i�N���A���ɂ��̊��ǃt���O�������܂��j
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// ���b�Z�[�W�o�b�N�ɐ���ݒ肷��
	if (!Gp_local->pod.msg_back_off)
	{
		tnm_msg_back_add_koe(koe_no, chara_no, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}

	// �u���݂̐��ԍ��v��ݒ�i���Đ��{�^���ōĐ����鐺�ł��j
	Gp_local->pod.cur_koe_no = koe_no;
	Gp_local->pod.cur_chr_no = chara_no;
}

// ****************************************************************
// ���b�Z�[�W�F�d�w����ݒ�
// ================================================================
void tnm_msg_proc_ex_koe(C_elm_mwnd* p_mwnd, int koe_no, int chara_no, bool wait_flag, bool key_skip_flag, bool return_value_flag)
{
	// �����Đ�����i�Đ����Ȃ��ꍇ�̓t�@�C���`�F�b�N�̂݁j
	tnm_play_koe(koe_no, chara_no, TNM_JITAN_RATE_NORMAL, 0, wait_flag, key_skip_flag, return_value_flag, true, false);
}

// ****************************************************************
// ���b�Z�[�W�F��O����ݒ�
// ================================================================
void tnm_msg_proc_face(C_elm_mwnd* p_mwnd, int face_no, CTSTR& file_path)
{
	if (!p_mwnd)
		return;

	// ��O����ݒ�
	p_mwnd->set_face(face_no, file_path);
}

// ****************************************************************
// ���b�Z�[�W�F���O�t���O��W�J
// ================================================================
//		���`�⁓�`�Ȃǂ�ϊ����܂��B
// ================================================================
void tnm_msg_proc_expand_namae_flag(TSTR& msg)
{
	TSTR new_msg;

	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end(); )	{

		if (*itr == _T('��') || *itr == _T('��'))	{

			// �P�����ڂ��A���t�@�x�b�g�H
			if (itr + 1 != msg.end() && is_zenkaku_large_alpha(*(itr + 1)))	{
				TSTR namae;

				// �Q�����ڂ��A���t�@�x�b�g�H
				if (itr + 2 != msg.end() && is_zenkaku_large_alpha(*(itr + 2)))	{
					namae = *Gp_flag->get_namae_flag_by_name(TSTR(itr, itr + 3));		// �u���`�`�v
					itr += 3;
				}
				else	{
					namae = *Gp_flag->get_namae_flag_by_name(TSTR(itr, itr + 2));		// �u���`�v
					itr += 2;
				}

				// ���O�̉������ڂ�\������H
				if (itr != msg.end() && is_zenkaku_num(*itr))	{

					int moji_no = 0;
					while (itr != msg.end())	{
						if (is_zenkaku_num(*itr))	{
							moji_no = moji_no * 10 + *itr - _T('�O');
							++ itr;
						}
						else	{
							break;
						}
					}

					if (moji_no < (int)namae.length())
						namae = namae.substr(moji_no, 1);
					else
						namae.clear();
				}

				// ���O������
				new_msg += namae;
			}
			else	{
				new_msg += *itr;	// �u���v�̂�
				++itr;
			}
		}
		else	{
			new_msg += *itr;	// �ʏ�̕���
			++itr;
		}
	}

	msg = new_msg;
}

// ****************************************************************
// ���b�Z�[�W�F���O��ݒ�
// ================================================================
void tnm_msg_proc_name(C_elm_mwnd* p_mwnd, TSTR name)
{
	if (!p_mwnd)
		return;

	// ���ɖ��O������ꍇ�͖��O�R�}���h�𖳎�����
	if (!p_mwnd->get_name().empty())
		return;

	// �L�����N�^�[�����̖��O�̕��������J���邩�ǂ�������i���J�t���O���n�m�ɂ��܂��j
	tnm_is_chrkoe_name_check(name);

	// ���O��ϊ��iini �ɓo�^���ꂽ���O�j
	int color_mode, moji_color_no, shadow_color_no, fuchi_color_no;
	name = Gp_namae->change(name, &color_mode, &moji_color_no, &shadow_color_no, &fuchi_color_no);

	// �L�����N�^�����F��ݒ�
	p_mwnd->set_chara_moji_color(moji_color_no);
	p_mwnd->set_chara_shadow_color(shadow_color_no);
	p_mwnd->set_chara_fuchi_color(fuchi_color_no);

	// ���O�t���O��W�J
	tnm_msg_proc_expand_namae_flag(name);

	// ���O�̕\�����[�h���擾
	int name_disp_mode = p_mwnd->get_name_disp_mode();

	// �m�x���̏ꍇ�A���O�\�����[�h�����O�E�B���h�E�̏ꍇ�́u���b�Z�[�W�̐擪�ɕ\���v�ɕύX
	if (p_mwnd->get_novel_flag() == 1 && name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME)
	{
		name_disp_mode = TNM_MWND_NAME_DISP_TYPE_MSG;
	}

	// ���ʂ����O�����߂�
	TSTR name_with_bracket;
	int name_bracket_type = p_mwnd->get_name_bracket();
	if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
	{
		name_with_bracket = _T("�y") + name + _T("�z");
	}
	else
	{
		name_with_bracket = name;
	}
	
	// �Z�[�u���b�Z�[�W�ɖ��O��ǉ��i���O���u�\�����Ȃ��v�̏ꍇ�͒ǉ����Ȃ��j
	if (name_disp_mode != TNM_MWND_NAME_DISP_TYPE_NONE)
	{
		tnm_save_local_msg(name_with_bracket);
	}

	// ���O�E�B���h�E�ɖ��O��\��
	if (name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME)
	{
		p_mwnd->set_name(name_with_bracket);
	}
	// ���b�Z�[�W�̐擪�ɖ��O��\��
	else if (name_disp_mode == TNM_MWND_NAME_DISP_TYPE_MSG)
	{
		p_mwnd->add_name_to_msg(name_with_bracket, NULL);
		p_mwnd->set_line_head();		// �s�̐擪��ݒ肷��
	}

	// ���b�Z�[�W�o�b�N�ɖ��O��ݒ肷��
	if (!Gp_local->pod.msg_back_off)
	{
		// ���b�Z�[�W�E�B���h�E�ɏ�������ꍇ
		if (Gp_ini->msg_back.name_disp_mode == TNM_MSGBK_NAME_DISP_TYPE_DEFAULT)
		{
			if (name_disp_mode != TNM_MWND_NAME_DISP_TYPE_NONE)
			{
				if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_NONE)
				{
					tnm_msg_back_add_name(name, name, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
				}
				else if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
				{
					tnm_msg_back_add_name(name, _T("�y") + name + _T("�z"), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
				}
			}
			else
			{
				tnm_msg_back_add_name(name, _T(""), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
		}
		// �I���̏ꍇ
		else if (Gp_ini->msg_back.name_disp_mode == TNM_MSGBK_NAME_DISP_TYPE_ON)
		{
			if (Gp_ini->msg_back.name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_NONE)
			{
				tnm_msg_back_add_name(name, name, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
			else if (Gp_ini->msg_back.name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
			{
				tnm_msg_back_add_name(name, _T("�y") + name + _T("�z"), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W��ǉ�
// ================================================================
//		���b�Z�[�W�R�}���h�����ꂽ�ꍇ�̏����ł��B
//		�P�D�N���A�����t���O�������Ă���΃��b�Z�[�W���N���A���܂��B
//		�Q�D�V���Ƀ��b�Z�[�W��ǉ����܂��B
//		�R�D�ŏ��̃��b�Z�[�W�������ꍇ�̓Z�[�u�|�C���g�𗧂Ă܂��B
// ================================================================
void tnm_msg_proc_print(C_elm_mwnd* p_mwnd, TSTR msg, int rf_scn_no, int rf_flag_no, bool msg_back_flag)
{
	if (!p_mwnd)
		return;

	TSTR over_flow_msg;

	// ���Ǒ����菈���i���b�Z�[�W�u���b�N�Ɏ����Ă����������ǁA�S�ĂɊ��ǃt���O��������̂��Ȃ��c�j
	{
		// �u���݂̊��ǃt���O�ԍ��v��ݒ�i���Ǒ����肪�\���̔���Ɏg���܂��j
		Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
		Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

		// ������̍X�V
		tnm_update_read_skip();

		// ������������J�n����
		tnm_update_start_skip();
	}

	// ���O�t���O��W�J
	tnm_msg_proc_expand_namae_flag(msg);

	// �Z�[�u�f�[�^�Ƀ��b�Z�[�W��ǉ�
	tnm_save_local_msg(msg);

	// �t�����b�Z�[�W��ݒ�
	Gp_local->cur_full_message += msg;

	// ���b�Z�[�W��ǉ�
	p_mwnd->add_msg(msg, &over_flow_msg);

	// �Ō�ɕ�����\���������Ԃ�ݒ�
	p_mwnd->set_last_moji_disp_time();

	// �I�[�o�[�t���[�����ꍇ�̓L�[�҂��������I�ɑ}��
	if (!over_flow_msg.empty())
	{
		// �I�[�o�t���[����
		tnm_msg_proc_over_flow_msg(p_mwnd, msg, over_flow_msg, rf_scn_no, rf_flag_no);
	}

	// ���b�Z�[�W�Ɋ��ǃt���O��ǉ��i�N���A���ɂ��̊��ǃt���O�������܂��j
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// ���b�Z�[�W�o�b�N�Ƀ��b�Z�[�W��ݒ肷��
	if (msg_back_flag && !Gp_local->pod.msg_back_off)
	{
		tnm_msg_proc_msg_back_add_msg(msg, p_mwnd->get_debug_msg(), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}

	// ���b�Z�[�W�E�B���h�E�̎��������J�n
	p_mwnd->set_auto_proc_ready();

	// ���b�Z�[�W��i�߂�
	p_mwnd->msg_frame();

	// �����I�Ƀ��b�Z�[�W�E�F�C�g
	// �I�[�o�[�t���[���͏�Ń��b�Z�[�W�E�F�C�g�����Ă�̂œ���Ȃ�
	// �񓯊����[�h�̏ꍇ�̓��b�Z�[�W�E�F�C�g�����Ȃ�
	if (!Gp_local->pod.async_msg_mode && !Gp_local->pod.multi_msg_mode && over_flow_msg.empty())
	{
		tnm_msg_proc_command_msg_wait(p_mwnd);
	}
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W�o�b�N��ǉ�
// ================================================================
//		���b�Z�[�W�o�b�N����̃��[�h�Ɋւ��܂�
//		���b�Z�[�W�����A�܂��� msgbk.add_msg �Ȃǂ��� �Ă΂�܂�
// ================================================================

void tnm_msg_proc_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	tnm_msg_back_add_msg(msg, debug_msg, scn_no, line_no);

	// ���b�Z�[�W�o�b�N�Z�[�u���g���ꍇ
	if (Gp_ini->backlog_save_cnt > 0)
	{
		if (!tnm_msg_back_get_save_id_check_flag())
		{
			int interval = std::max(Gp_ini->backlog_save_interval, 1);

			// ���u���ɕۑ����邽�߂̎d�|��
			if (Gp_local->pod.msg_back_save_cntr++ % interval == 0)
			{
				// ���[�J���Z�[�u������ꍇ
				if (!Gp_local_save->save_stream.empty())
				{
					// ���b�Z�[�W�o�b�N�Z�[�u�p�̃A�C�e�������
					BSP<S_tnm_local_save> save_item = BSP<S_tnm_local_save>(new S_tnm_local_save);
					*save_item = *Gp_local_save;

					// ���b�Z�[�W�o�b�N�Z�[�u�ɃA�C�e����ǉ�����
					Gp_backlog_save->map.insert(std::pair<S_tid, BSP<S_tnm_local_save>>(Gp_local_save->save_id, save_item));
					// ���b�Z�[�W�o�b�N�ƃA�C�e�����֘A�t����
					tnm_msg_back_set_save_id(Gp_local_save->save_id);
					// �o�b�N���O�Z�[�u�𐮗�����
					tnm_remove_backlog_save();
				}
			}

			tnm_msg_back_set_save_id_check_flag(true);
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�F�I�[�o�[�t���[
// ================================================================
//		�I�[�o�[�t���[�����Ƃ��Ɏ����Œǉ������R�}���h�ł��B
// ================================================================
void tnm_msg_proc_over_flow_msg(C_elm_mwnd* p_mwnd, CTSTR& msg, CTSTR& over_flow_msg, int rf_scn_no, int rf_flag_no)
{
	// �I�[�o�[�t���[���X���C�h���b�Z�[�W�ő���^�C�v
	if (p_mwnd->is_slide_msg())
	{
		C_tnm_proc print_proc;
		print_proc.type = TNM_PROC_TYPE_COMMAND;
		print_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____OVER_FLOW_PRINT);
		print_proc.arg_list_id = 1;
		print_proc.arg_list.resize(3);
		print_proc.arg_list[0].form = FM_STR;
		print_proc.arg_list[0].str = over_flow_msg;
		print_proc.arg_list[1].form = FM_INT;
		print_proc.arg_list[1].Int = rf_scn_no;
		print_proc.arg_list[2].form = FM_INT;
		print_proc.arg_list[2].Int = rf_flag_no;
		tnm_push_proc(print_proc);				// �S�D���b�Z�[�W

		C_tnm_proc msg_block_proc;
		msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
		msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_PP_BLOCK);
		tnm_push_proc(msg_block_proc);			// �R�D���b�Z�[�W�u���b�N�̐擪����

		C_tnm_proc slide_msg_proc;
		slide_msg_proc.type = TNM_PROC_TYPE_COMMAND;
		slide_msg_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____SLIDE_MSG);
		tnm_push_proc(slide_msg_proc);			// �Q�D�X���C�h���b�Z�[�W

		// �P�����ł��\�����Ă����烁�b�Z�[�W��҂�
		if (msg != over_flow_msg)
		{
			C_tnm_proc msg_wait_proc;
			msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
			msg_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
		}
	}

	// �I�[�o�[�t���[���L�[�҂��ő҂^�C�v
	else
	{
		// �X���C�h���b�Z�[�W�łȂ���΃G���[���o��
		tnm_set_error(TNM_ERROR_TYPE_OVERFLOW, _T("���b�Z�[�W���I�[�o�[�t���[���܂����B"));

		C_tnm_proc print_proc;
		print_proc.type = TNM_PROC_TYPE_COMMAND;
		print_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____OVER_FLOW_PRINT);
		print_proc.arg_list_id = 1;
		print_proc.arg_list.resize(3);
		print_proc.arg_list[0].form = FM_STR;
		print_proc.arg_list[0].str = over_flow_msg;
		print_proc.arg_list[1].form = FM_INT;
		print_proc.arg_list[1].Int = rf_scn_no;
		print_proc.arg_list[2].form = FM_INT;
		print_proc.arg_list[2].Int = rf_flag_no;
		tnm_push_proc(print_proc);				// �T�D���b�Z�[�W

		// �P�����ł��\�����Ă�����L�[�҂�������
		if (msg != over_flow_msg)	{

			C_tnm_proc msg_block_proc;
			msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
			msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_BLOCK);
			tnm_push_proc(msg_block_proc);			// �S�D���b�Z�[�W�u���b�N�̐擪����

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);		// �R�D�N���A�����t���O�𗧂Ă�

			// �m�x�����[�h
			if (p_mwnd->get_novel_flag() == 1)
			{
				p_mwnd->set_key_icon_mode(1);	// �y�[�W�҂��A�C�R��
			}
			// �ʏ탂�[�h
			else
			{
				p_mwnd->set_key_icon_mode(0);	// �L�[�҂��A�C�R��
			}

			C_tnm_proc msg_key_wait_proc;
			msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
			msg_key_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_key_wait_proc);		// �Q�D���b�Z�[�W�L�[�҂�

			C_tnm_proc msg_wait_proc;
			msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
			msg_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
		}

		// ����ȊO�̏ꍇ�̓N���A�̂݁i���������͎~�߂Ȃ��j
		else
		{
			C_tnm_proc msg_block_proc;
			msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
			msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_BLOCK);
			tnm_push_proc(msg_block_proc);			// �S�D���b�Z�[�W�u���b�N�̐擪����

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);		// �R�D�N���A�����t���O�𗧂Ă�
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�F���s
// ================================================================
void tnm_msg_proc_command_nl(C_elm_mwnd* p_mwnd)
{
	p_mwnd->new_line_no_indent();

	if (Gp_ini->mwnd.sub[p_mwnd->get_mwnd_no()].msg_back_insert_nl)	{
		Gp_msg_back->add_new_line_no_indent(Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}
}

void tnm_msg_proc_command_nli(C_elm_mwnd* p_mwnd)
{
	p_mwnd->new_line_indent();

	if (Gp_ini->mwnd.sub[p_mwnd->get_mwnd_no()].msg_back_insert_nl)	{
		Gp_msg_back->add_new_line_indent(Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}
}

// ****************************************************************
// ���b�Z�[�W�F���r
// ================================================================
//		���r�R�}���h�̏����ł��B
// ================================================================
void tnm_msg_proc_ruby(C_elm_mwnd* p_mwnd, TSTR ruby)
{
	if (!p_mwnd)
		return;

	// ���O�t���O��W�J
	tnm_msg_proc_expand_namae_flag(ruby);

	// ���r��\������
	p_mwnd->start_ruby(ruby);

	// ���b�Z�[�W�E�B���h�E�̎��������J�n
	p_mwnd->set_auto_proc_ready();
}

void tnm_msg_proc_end_ruby(C_elm_mwnd* p_mwnd)
{
	if (!p_mwnd)
		return;

	// ���r���I������
	p_mwnd->end_ruby();

	// ���b�Z�[�W�E�B���h�E�̎��������J�n
	p_mwnd->set_auto_proc_ready();
}

// ****************************************************************
// ���b�Z�[�W�F���O�t���O��W�J
// ================================================================
int tnm_zenkaku_num_moji_to_int(TCHAR moji)
{
	switch (moji)	{
		case _T('�O'):	return 0;
		case _T('�P'):	return 1;
		case _T('�Q'):	return 2;
		case _T('�R'):	return 3;
		case _T('�S'):	return 4;
		case _T('�T'):	return 5;
		case _T('�U'):	return 6;
		case _T('�V'):	return 7;
		case _T('�W'):	return 8;
		case _T('�X'):	return 9;
		default:		return 0;
	}
}

bool tnm_msg_proc_expand_name_flag(TSTR::const_iterator& itr_ret, TSTR::const_iterator end, int* moji_type_ret, int* moji_code_ret)
{
	int moji_type = TNM_MOJI_TYPE_MOJI;
	int moji_code = -1;

	// ���݂̃C�e���[�^��ۑ����Ă���
	TSTR::const_iterator itr = itr_ret;

	// �G��������
	if (itr != end && *itr == _T('��'))	{
		++ itr;
		if (itr != end && *itr == _T('�`') || itr != end && *itr == _T('�a'))	{
			moji_type = *itr == _T('�`') ? TNM_MOJI_TYPE_EMOJI_A : TNM_MOJI_TYPE_EMOJI_B;
			++ itr;
			if (itr != end && is_zenkaku_num(*itr))	{
				moji_code = tnm_zenkaku_num_moji_to_int(*itr);
				++ itr;
				for (int i = 0;;)	{
					if (itr != end && is_zenkaku_num(*itr))	{
						moji_code = moji_code * 10 + tnm_zenkaku_num_moji_to_int(*itr);
						++ itr;
					}
					else	{
						break;
					}
				}
			}
		}
		if (moji_code >= 0)	{
			*moji_type_ret = moji_type;
			*moji_code_ret = moji_code;
			itr_ret = itr;
			return true;
		}
	}

	// �G��������Ȃ������̂ŃC�e���[�^��߂�
	itr = itr_ret;

	// �ʏ�̕�������
	if (itr != end)	{
		*moji_type_ret = TNM_MOJI_TYPE_MOJI;
		*moji_code_ret = *itr++;
		itr_ret = itr;
		return true;
	}

	return false;
}

// ****************************************************************
// ���b�Z�[�W�F�I����
// ================================================================
//		�I�����R�}���h�̏����ł��B
// ================================================================
void tnm_msg_proc_sel(C_elm_mwnd* p_mwnd, ARRAY<S_tnm_sel_param>& sel_list, int rf_scn_no, int rf_flag_no, bool cancel_enable, bool close_mwnd)
{
	if (!p_mwnd)
		return;

	// �N���A�����t���O�������Ă���Ȃ�N���A����
	if (p_mwnd->get_clear_ready_flag())
		tnm_msg_proc_clear(p_mwnd);

	// �I�������N���A
	p_mwnd->clear_sel();

	// �I����������
	p_mwnd->ready_sel();

	// �I������ǉ����Ă���
	for (ARRAY<S_tnm_sel_param>::iterator itr = sel_list.begin(); itr != sel_list.end(); ++itr)	{
		TSTR msg = itr->str;

		// ���O�t���O��W�J
		tnm_msg_proc_expand_namae_flag(msg);

		// �I������ǉ�
		p_mwnd->add_sel(msg, itr->type, itr->color);
	}

	// �I�����𐮗�����
	p_mwnd->restruct_sel();

	// �L�����Z���\�t���O��ݒ�
	p_mwnd->set_sel_cancel_enable(cancel_enable);

	// �u���݂̊��ǃt���O�ԍ��v��ݒ�
	Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
	Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

	// ���b�Z�[�W�Ɋ��ǃt���O��ǉ�
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// ������̍X�V
	tnm_update_read_skip();

	// ������������J�n����
	tnm_update_start_skip();

	// ���������J�n
	p_mwnd->set_auto_proc_ready();

	// �v���Z�X�̐ݒ�i�R�D���b�Z�[�W�E�B���h�E�����j
	if (close_mwnd)	{
		C_tnm_proc proc_close;
		proc_close.type = TNM_PROC_TYPE_COMMAND;
		proc_close.element = p_mwnd->get_element() + S_element(ELM_MWND_CLOSE);
		tnm_push_proc(proc_close);
	}

	// �v���Z�X�̐ݒ�i�Q�D�N���A�����t���O�𗧂Ă�j

	// �m�x�����[�h
	if (p_mwnd->get_novel_flag() == 1)	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____NOVEL_CLEAR);
		tnm_push_proc(clear_ready_proc);
	}
	// �ʏ탂�[�h
	else	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
		tnm_push_proc(clear_ready_proc);
	}

	// �v���Z�X�̐ݒ�i�P�D�I�����j
	C_tnm_proc proc_sel;
	proc_sel.type = TNM_PROC_TYPE_SEL;
	proc_sel.element = p_mwnd->get_element();
	tnm_push_proc(proc_sel);
}

// ****************************************************************
// ���b�Z�[�W�F�{�^���I����
// ================================================================
//		�{�^���I�����R�}���h�̏����ł��B
// ================================================================
void tnm_msg_proc_selbtn_ready(C_elm_btn_select* p_btn_sel, int template_no, ARRAY<S_tnm_sel_param>& sel_list, bool cancel_enable, bool capture_flag, CTSTR& sel_start_scn, int sel_start_z_no)
{
	if (!p_btn_sel)
		return;

	// �{�^���I������������
	p_btn_sel->reinit();
	p_btn_sel->set_template(template_no);

	// �I������ǉ����Ă���
	for (ARRAY<S_tnm_sel_param>::iterator itr = sel_list.begin(); itr != sel_list.end(); ++itr)	{
		TSTR msg = itr->str;

		// ���O�t���O��W�J
		tnm_msg_proc_expand_namae_flag(msg);

		// �I������ǉ�
		p_btn_sel->add_sel(msg, itr->type, itr->color);
	}

	// �I�����𐮗�����
	p_btn_sel->restruct_item();

	// �L�����Z���\�t���O��ݒ肷��
	p_btn_sel->set_cancel_enable(cancel_enable);

	// �L���v�`���[�t���O��ݒ肷��
	p_btn_sel->set_capture_flag(capture_flag);

	// �I���J�n���ɌĂ΂��R�}���h��ݒ肷��
	p_btn_sel->set_sel_start_call(sel_start_scn, sel_start_z_no);
}

void tnm_msg_proc_selbtn_start(C_elm_btn_select* p_btn_sel, int rf_scn_no, int rf_flag_no, int sync_type)
{
	// ���݂̊��ǃt���O�ԍ���ݒ�
	Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
	Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

	// ���ǃt���O��o�^����
	p_btn_sel->regist_read_flag(rf_scn_no, rf_flag_no);

	// ���Ǒ�������X�V����
	tnm_update_read_skip();

	// ������������J�n����
	tnm_update_start_skip();

	// �I�������o��������
	p_btn_sel->start_sel(sync_type);

	// �v���Z�X�̐ݒ�i�P�D�I�����j
	C_tnm_proc proc_sel;
	proc_sel.type = TNM_PROC_TYPE_SEL_BTN;
	tnm_push_proc(proc_sel);
}


// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W���N���A����
// ================================================================
//		���b�Z�[�W���N���A�������鏈���ł��B
//		�P�D���ǃt���O�𗧂Ă܂��B
//		�Q�D�����~���܂��B
// ================================================================
void tnm_msg_proc_clear_ready(C_elm_mwnd* p_mwnd)
{
	// �N���A�����t���O�𗧂Ă�
	p_mwnd->set_clear_ready_flag();

	// ���蓖�Ă�ꂽ���ǃt���O�𗧂Ă�
	p_mwnd->set_read_flag();

	// ���ǃt���O���N���A����
	p_mwnd->clear_read_flag();

	// �}���`���b�Z�[�W���I������
	Gp_local->pod.multi_msg_mode = false;

	// �񓯊����b�Z�[�W���I�����邩��
	if (Gp_local->pod.async_msg_mode_once)	{
		Gp_local->pod.async_msg_mode = false;
		Gp_local->pod.async_msg_mode_once = false;
	}

	// �X�L�b�v�g���K�[�ɂ�鑁������~����
	Gp_global->skip_because_skip_trigger = false;

	// �����~����i�V�X�e���̐ݒ�ɂ��j
	if (tnm_is_stop_koe())	{
		tnm_stop_koe();

		// ���a�f�l�t�F�[�h�Q���K�v�t���O���~�낷
		Gp_global->bgmfade2_need_flag = false;
	}

	// ���݂̐��i�V�X�e���R�}���h�́u�����Đ��v�ōĐ����鐺�j���N���A����
	Gp_local->pod.cur_koe_no = -1;
	Gp_local->pod.cur_chr_no = -1;

	// �I�[�g���[�h�̕��������N���A
	Gp_local->pod.auto_mode_moji_cnt = 0;

	// ���b�Z�[�W�u���b�N�̐擪���������t���O���N���A
	p_mwnd->clear_msg_block_start_flag();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���b�Z�[�W���N���A�����i�m�x���j
// ================================================================
//		���b�Z�[�W���N���A�������鏈���ł��B
//		�P�D���ǃt���O�𗧂Ă܂��B
//		�Q�D�����~���܂��B
// ================================================================
void tnm_msg_proc_clear_ready_novel(C_elm_mwnd* p_mwnd)
{
	// ���ǃt���O�𗧂Ă�
	p_mwnd->set_read_flag();

	// ���ǃt���O���N���A����
	p_mwnd->clear_read_flag();

	// �}���`���b�Z�[�W���I������
	Gp_local->pod.multi_msg_mode = false;

	// �����~����i�V�X�e���̐ݒ�ɂ��j
	if (tnm_is_stop_koe())	{
		tnm_stop_koe();
	}

	// ���݂̐����N���A����
	Gp_local->pod.cur_koe_no = -1;
	Gp_local->pod.cur_chr_no = -1;

	// ���b�Z�[�W�u���b�N�̐擪���������t���O���N���A
	p_mwnd->clear_msg_block_start_flag();

	// ���s���͂���ł݂�
	p_mwnd->new_line_no_indent();
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W���N���A
// ================================================================
//		���b�Z�[�W���N���A���鏈���ł��B
// ================================================================
void tnm_msg_proc_clear(C_elm_mwnd* p_mwnd)
{
	if (!p_mwnd)
		return;

	// �t�����b�Z�[�W���N���A
	Gp_local->cur_full_message.clear();

	// ���b�Z�[�W���N���A����
	p_mwnd->clear();
}

// ****************************************************************
// ���b�Z�[�W�F���b�Z�[�W�E�B���h�E�����
// ================================================================
void tnm_msg_proc_close(C_elm_mwnd* p_mwnd, bool wait_flag)
{
	if (!p_mwnd)
		return;

	// �E�B���h�E���J���Ă��Ȃ���Ή������Ȃ�
	if (!p_mwnd->get_window_appear_flag())
		return;

	// ���ǃt���O�𗧂Ă�
	p_mwnd->set_read_flag();

	// ���b�Z�[�W�E�B���h�E�����
	p_mwnd->close();

	// ���A�j���҂���Ԃɂ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_CLOSE_WAIT;
		proc.element = p_mwnd->get_element();
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// ���b�Z�[�W�F�S�Ẵ��b�Z�[�W�E�B���h�E�����
// ================================================================
void tnm_msg_proc_close_all(bool wait_flag)
{
	// ���ׂẴE�B���h�E�ɑ΂��Ĕ���
	for (int stage_no = 0; stage_no < (int)Gp_stage_list->get_size(); stage_no ++)	{
		for (int mwnd_no = 0; mwnd_no < (int)(*Gp_stage_list)[stage_no].m_mwnd_list.get_size(); mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			// ���ǃt���O�𗧂Ă�
			p_mwnd->set_read_flag();

			// ���b�Z�[�W�E�B���h�E�����
			p_mwnd->close();
		}
	}
	// ���A�j���҂���Ԃɂ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_CLOSE_WAIT_ALL;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// ���b�Z�[�W�F�q����
// ================================================================
void tnm_msg_proc_command_msg_wait(C_elm_mwnd* p_mwnd)
{
	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
}

void tnm_msg_proc_command_pp(C_elm_mwnd* p_mwnd)
{
	p_mwnd->set_key_icon_mode(0);	// �L�[�҂��A�C�R��

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// �Q�D���b�Z�[�W�L�[�҂�

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
}

void tnm_msg_proc_command_r(C_elm_mwnd* p_mwnd)
{
	if (Gp_local->pod.ignore_r_flag)
		return;

	// �m�x�����[�h
	if (p_mwnd->get_novel_flag() == 1)	{

		// ���b�Z�[�W�����炩�ɓ���Ȃ��ꍇ�� PAGE �R�}���h�̓���ɂȂ�
		if (p_mwnd->get_over_flow_check_size() > 0 && !p_mwnd->add_msg_check(true))	{
			p_mwnd->set_key_icon_mode(1);	// �y�[�W�҂��A�C�R��

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);	// �R�D�N���A�����t���O�𗧂Ă�
		}
		else	{
			p_mwnd->set_key_icon_mode(0);	// �L�[�҂��A�C�R��

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____NOVEL_CLEAR);
			tnm_push_proc(clear_ready_proc);	// �R�D�N���A�����t���O�𗧂Ă�
		}
	}
	// �ʏ탂�[�h
	else	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
		tnm_push_proc(clear_ready_proc);	// �R�D�N���A�����t���O�𗧂Ă�
	}

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// �Q�D���b�Z�[�W�L�[�҂�

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
}

void tnm_msg_proc_command_page(C_elm_mwnd* p_mwnd)
{
	// �m�x�����[�h
	if (p_mwnd->get_novel_flag() == 1)	{
		p_mwnd->set_key_icon_mode(1);	// �y�[�W�҂��A�C�R��
	}
	// �ʏ탂�[�h
	else	{
		p_mwnd->set_key_icon_mode(0);	// �L�[�҂��A�C�R��
	}

	C_tnm_proc clear_ready_proc;
	clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
	clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
	tnm_push_proc(clear_ready_proc);		// �R�D�N���A�����t���O�𗧂Ă�

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// �Q�D���b�Z�[�W�L�[�҂�

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// �P�D���b�Z�[�W�\���I���҂�
}

