#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_element_code.h"
#include	"data/tnm_def.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_btn_template.h"
#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_call.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_syscom.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// �I�u�W�F�N�g�F�{�^���p�����[�^�̏�����
// ================================================================
void C_elm_object::init_button_param()
{
	m_op.button.sys_type = 0;
	m_op.button.sys_type_opt = 0;
	m_op.button.button_no = 0;
	m_op.button.elm_group.clear();
	m_op.button.action_no = -1;
	m_op.button.se_no = -1;
	m_op.button.push_keep = 0;
	m_op.button.state = TNM_BTN_STATE_NORMAL;
	m_op.button.mode = 0;
	m_op.button.cut_no = 0;
	m_op.button.decided_action_scn_no = -1;
	m_op.button.decided_action_cmd_no = -1;
	m_op.button.decided_action_z_no = 0;
	m_op.button.excall_enable = 0;
	m_op.button.alpha_test = 0;

	// �{�^���A�N�V����
	m_op.button_decided_action_scn_name.clear();
	m_op.button_decided_action_cmd_name.clear();
}

// ****************************************************************
// �I�u�W�F�N�g�F�{�^���p�����[�^�̐ݒ�
// ================================================================
void C_elm_object::set_button_param(int button_no, int group_no, int action_no, int se_no)
{
	m_op.button.button_no = button_no;
	m_op.button.action_no = action_no;
	m_op.button.se_no = se_no;

	// �O���[�v��ݒ肷��
	set_button_group_no(group_no);
}

// ****************************************************************
// �{�^������A�N�V������ݒ�
// ================================================================
bool C_elm_object::set_button_decided_action(CTSTR& scn_name, CTSTR& cmd_name)
{
	m_op.button_decided_action_scn_name = scn_name;
	m_op.button_decided_action_cmd_name = cmd_name;
	m_op.button.decided_action_z_no = -1;

	if (!restruct_button_action())
		return false;

	return true;
}

bool C_elm_object::set_button_decided_action(CTSTR& scn_name, int z_no)
{
	m_op.button_decided_action_scn_name = scn_name;
	m_op.button_decided_action_cmd_name.clear();
	m_op.button.decided_action_z_no = z_no;

	if (!restruct_button_action())
		return false;

	return true;
}

// ****************************************************************
// �{�^���A�N�V�������N���A
// ================================================================
bool C_elm_object::clear_button_decide_action()
{
	m_op.button_decided_action_scn_name.clear();
	m_op.button_decided_action_cmd_name.clear();
	m_op.button.decided_action_z_no = -1;

	if (!restruct_button_action())
		return false;

	return true;
}

// ****************************************************************
// �{�^���̃O���[�v��ݒ�
// ================================================================
void C_elm_object::set_button_group_no(int group_no)
{
	if (group_no >= 0)	{
		m_op.button.elm_group = m_op_def.elm_group_list + S_element(ELM_ARRAY, group_no);
	}
	else	{
		m_op.button.elm_group.clear();
	}
}
void C_elm_object::set_button_group(const S_element& element)
{
	m_op.button.elm_group = element;
}

// ****************************************************************
// �{�^���̃O���[�v�ԍ����擾
// ================================================================
int	C_elm_object::get_button_group_no()
{
	if (m_op.button.elm_group.is_empty())	{
		return -1;
	}
	else	{
		return m_op.button.elm_group.back();
	}
}

// ****************************************************************
// �{�^���A�N�V�������č\�z
// ================================================================
bool C_elm_object::restruct_button_action()
{
	m_op.button.decided_action_scn_no = Gp_lexer->get_scn_no(m_op.button_decided_action_scn_name);
	m_op.button.decided_action_cmd_no = Gp_lexer->get_user_cmd_no(m_op.button.decided_action_scn_no, m_op.button_decided_action_cmd_name);

	return true;
}

// ****************************************************************
// �{�^����o�^����
// ================================================================
void C_elm_object::regist_button(int min_order)
{
	// �O���[�v���g���I�u�W�F�N�g�n
	if (m_op_def.use_group_flag)
	{
		// �O���[�v���擾����
		C_elm_group* p_group = get_group();
		if (p_group)
		{
			if (p_group->is_doing())
			{
				if (m_button.sorter().order >= min_order)
				{
					Gp_btn_mng->regist_button(&m_button);
				}
			}
		}
	}
	// �O���[�v���g��Ȃ��V�X�e���n
	else
	{
		if (m_op.button.action_no >= 0)
		{
			if (m_button.sorter().order >= min_order)
			{
				Gp_btn_mng->regist_button(&m_button);
			}
		}
	}

	// �q��
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
	{
		m_child_list[i].regist_button(min_order);
	}
}

// ****************************************************************
// �{�^���X�e�[�g�i������j���擾����
// ================================================================
int C_elm_object::get_button_hit_state()
{
	// �A�N�V�����ԍ��������Ȃ�ʏ���
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return TNM_BTN_STATE_NORMAL;

	// �ʏ�̓������Ԃ��擾
	return Gp_btn_mng->get_hit_state(&m_button);
}

// ****************************************************************
// �{�^���X�e�[�g�i���v�j���擾����
// ================================================================
int C_elm_object::get_button_real_state()
{
	C_tnm_btn_action_template* p_templete = NULL;
	S_tnm_btn_action_template_pat* p_action = NULL;

	// �A�N�V�����ԍ��������Ȃ�ʏ���
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return TNM_BTN_STATE_NORMAL;

	// �g���Ȃ���Ԕ���
	if (false);
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_READ_SKIP)	{
		if (!tnm_syscom_read_skip_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_AUTO_MODE)	{
		if (!tnm_syscom_auto_mode_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_RETURN_SEL)	{
		if (!tnm_syscom_return_to_sel_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_HIDE_MWND)	{
		if (!tnm_syscom_hide_mwnd_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_SAVE)	{
		if (!tnm_syscom_open_save_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOAD)	{
		if (!tnm_syscom_open_load_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_SAVE)	{
		if (!tnm_syscom_quick_save_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_LOAD)	{
		if (!tnm_syscom_quick_load_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_MSG_BACK)	{
		if (!tnm_syscom_msg_back_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_KOE_PLAY)	{
		if (!tnm_syscom_koe_play_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_CONFIG)	{
		if (!tnm_syscom_open_config_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH)	{
		if (!tnm_syscom_local_extra_switch_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE)	{
		if (!tnm_syscom_local_extra_mode_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}

	// �X�e�[�g�ɂ���ĕ���
	if (false);
	else if (m_op.button.state == TNM_BTN_STATE_SELECT)	{
		return TNM_BTN_STATE_SELECT;
	}
	else if (m_op.button.state == TNM_BTN_STATE_DISABLE)	{
		return TNM_BTN_STATE_DISABLE;
	}

	// �G��Ȃ��ꍇ
	if (Gp_local->pod.mwnd_btn_touch_disable)
		return TNM_BTN_STATE_NORMAL;

	// �O���[�v���擾
	C_elm_group* p_group = get_group();

	// ��������O���[�v�����肳��Ă����ꍇ
	if (p_group && p_group->get_decided())	{

		// �������g�����肳��Ă����ꍇ�� PUSH
		if (p_group->get_decided_button_no() == m_op.button.button_no)	{
			return TNM_BTN_STATE_PUSH;
		}
	}
	if (p_group && p_group->get_hit_button_no() >= 0 && p_group->get_hit_button_no() == m_op.button.button_no)	{
		return TNM_BTN_STATE_HIT;
	}
	if (p_group && p_group->get_pushed_button_no() >= 0 && p_group->get_pushed_button_no() == m_op.button.button_no)	{
		return TNM_BTN_STATE_PUSH;
	}

	// �ʏ�̓������Ԃ��擾
	return Gp_btn_mng->get_hit_state(&m_button);
}

S_tnm_btn_action_template_pat* C_elm_object::get_action(int btn_state)
{
	C_tnm_btn_action_template* p_templete = NULL;
	S_tnm_btn_action_template_pat* p_action = NULL;

	// �A�N�V�����ԍ��������Ȃ�A�N�V�����Ȃ�
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return NULL;

	// �e���v���[�g���擾
	p_templete = &Gp_btn_action_list->m_sub[m_op.button.action_no];

	// �A�N�V������Ԃ�
	return p_templete->get_action(btn_state);
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
void C_elm_object::button_event()
{
	int btn_state = get_button_real_state();
	if (btn_state == TNM_BTN_STATE_SELECT || btn_state == TNM_BTN_STATE_DISABLE)
		return;

	// �G��Ȃ��ꍇ�����܂�
	if (Gp_local->pod.mwnd_btn_touch_disable)
		return;

	// �{�^���̏ꍇ�̂ݏ���
	if (m_op.button.action_no >= 0)	{

		// �O���[�v���擾
		C_elm_group* p_group = get_group();

		// �O���[�v���Ȃ��i�V�X�e���{�^���Ȃǁj�������́A�O���[�v�������ĉғ����̏ꍇ
		if (p_group == NULL || p_group->is_doing())	{

			// ��������
			if (Gp_btn_mng->is_hit_this_frame(&m_button))	{
				if (p_group == NULL || m_op.button.button_no != p_group->get_last_hit_button_no())	{

					// ���ʉ�
					if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
						int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].hit_no;
						tnm_play_se(se_no);
					}
				}
			}

			// �������Ă���
			if (p_group && Gp_btn_mng->is_hit(&m_button))	{
				p_group->set_hit_button_no(m_op.button.button_no);
			}

			// �����ꂽ
			if (Gp_btn_mng->is_pushed_this_frame(&m_button))	{
				if (p_group == NULL || m_op.button.button_no != p_group->get_last_pushed_button_no())	{

					// ���ʉ�
					if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
						int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].push_no;
						tnm_play_se(se_no);
					}
				}
			}

			// ������Ă���
			if (p_group && Gp_btn_mng->is_pushed(&m_button))	{
				p_group->set_pushed_button_no(m_op.button.button_no);
			}

			// ���肳�ꂽ
			if (Gp_btn_mng->is_decided_this_frame(&m_button))	{

				// ���ʉ�
				if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
					int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].decide_no;
					tnm_play_se(se_no);
				}

				// �O���[�v�ɑ����Ă���Ȃ�O���[�v�ɒʒm
				if (p_group)
					p_group->decide(m_op.button.button_no);

				// �{�^������A�N�V����
				if (false);
				else if (m_op.button.decided_action_scn_no >= 0 && m_op.button.decided_action_cmd_no >= 0)	{

					// �w��̃R�}���h���R�[��
					tnm_scene_proc_call_user_cmd(m_op.button.decided_action_scn_no, m_op.button.decided_action_cmd_no, FM_VOID, true, false);
				}
				else if (m_op.button.decided_action_scn_no >= 0 && m_op.button.decided_action_z_no >= 0)	{

					// �w��̃V�[�����R�[��
					tnm_scene_proc_farcall(m_op.button.decided_action_scn_no, m_op.button.decided_action_z_no, FM_VOID, true, false);
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_READ_SKIP)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_read_skip_set_onoff_flag(true);				// ���Ǒ�����J�n
					}
					else	{
						tnm_syscom_read_skip_set_onoff_flag(false);				// ���Ǒ������~
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_AUTO_MODE)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_auto_mode_set_onoff_flag(true);				// �I�[�g���[�h�J�n
					}
					else	{
						tnm_syscom_auto_mode_set_onoff_flag(false);				// �I�[�g���[�h��~
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_HIDE_MWND)	{

					tnm_syscom_hide_mwnd_set_onoff_flag(true);					// ���b�Z�[�W�E�B���h�E���B��
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_SAVE)	{

					tnm_syscom_open_save(false, true, true);					// �i���Z�[�u�F�l�v�{�^���j�_�C�A���O���Ă΂��ꍇ�̓L���v�`���[����
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOAD)	{

					tnm_syscom_open_load(false, true);							// ���[�h
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_SAVE)	{

					tnm_syscom_quick_save(m_op.button.sys_type_opt, true, true);		// �N�C�b�N�Z�[�u
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_LOAD)	{

					tnm_syscom_quick_load(m_op.button.sys_type_opt, true, true, true);	// �N�C�b�N���[�h
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_RETURN_SEL)	{

					tnm_syscom_return_to_sel(true, true, true);							// �O�̑I�����ɖ߂�
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_MSG_BACK)	{

					tnm_syscom_open_msg_back();									// ���b�Z�[�W�o�b�N
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_KOE_PLAY)	{

					tnm_syscom_koe_play();										// �����Đ�
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_CONFIG)	{

					tnm_syscom_open_config();									// ���ݒ�
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_local_extra_switch_set_onoff_flag(m_op.button.sys_type_opt, true);		// ���[�J���ėp�X�C�b�`
					}
					else	{
						tnm_syscom_local_extra_switch_set_onoff_flag(m_op.button.sys_type_opt, false);		// ���[�J���ėp�X�C�b�`
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE)	{

					tnm_syscom_local_extra_mode_increment(m_op.button.sys_type_opt);		// ���[�J���ėp���[�h
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH)	{
					if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_switch_flag.size())	{
						Gp_config->global_extra_switch_flag[m_op.button.sys_type_opt].value = !Gp_config->global_extra_switch_flag[m_op.button.sys_type_opt].value;
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE)	{
					if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_mode_flag.size())	{
						Gp_config->global_extra_mode_flag[m_op.button.sys_type_opt] ++;
						Gp_config->global_extra_mode_flag[m_op.button.sys_type_opt] %= Gp_ini->config.global_extra_mode[m_op.button.sys_type_opt].item_cnt;
					}
				}
			}
		}
	}

	// �q��
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].button_event();
}

// ****************************************************************
// ���݂̃{�^����Ԃ𔻒�
// ================================================================
bool C_elm_object::is_hit()
{
	return Gp_btn_mng->is_hit(&m_button);
}
bool C_elm_object::is_pushed()
{
	return Gp_btn_mng->is_pushed(&m_button);
}
bool C_elm_object::is_decided()
{
	return Gp_btn_mng->is_decided(&m_button);
}
bool C_elm_object::is_hit_this_frame()
{
	return Gp_btn_mng->is_hit_this_frame(&m_button);
}
bool C_elm_object::is_pushed_this_frame()
{
	return Gp_btn_mng->is_pushed_this_frame(&m_button);
}
bool C_elm_object::is_decided_this_frame()
{
	return Gp_btn_mng->is_decided_this_frame(&m_button);
}

// ****************************************************************
// ��������O���[�v���擾
// ================================================================
C_elm_group* C_elm_object::get_group()
{
	// �O���[�v���g���I�u�W�F�N�g�n
	if (m_op_def.use_group_flag)
	{
		// �O���[�v���擾����
		if (!m_op.button.elm_group.is_empty())
		{
			if (m_op.button.elm_group.code[0] == ELM_UP)
			{
				// ���΃p�X
				S_tnm_command_proc_arg_struct arg;
				arg.disp_out_of_range_error = false;
				if (tnm_command_proc_object(this, m_op.button.elm_group.begin(), m_op.button.elm_group.begin(), m_op.button.elm_group.end(), &arg))
				{
					C_elm_group* p_group = (C_elm_group *)arg.p_ret_element;
					if (p_group)
					{
						return p_group;
					}
				}
			}
			else
			{
				// ��΃p�X
				C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(m_op.button.elm_group.begin(), m_op.button.elm_group.end(), false);
				if (p_group)
				{
					return p_group;
				}
			}
		}
	}

	return NULL;
}

