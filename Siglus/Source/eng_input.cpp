#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_click_disable.h"
#include	"element/elm_stage.h"
#include	"engine/eng_input.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_syscom.h"

static const int TNM_KEY_ACTION_TYPE_DOWN = 0;
static const int TNM_KEY_ACTION_TYPE_UP = 1;
static const int TNM_KEY_ACTION_TYPE_DOWN_UP = 2;

static const int TNM_KEY_CHECK_TYPE_REAL_STATE = 0;
static const int TNM_KEY_CHECK_TYPE_CHECK_STOCK = 1;
static const int TNM_KEY_CHECK_TYPE_USE_STOCK = 2;

// ****************************************************************
// �V�X�e���I�ɋ֎~�ł��邩�𔻒�
// ================================================================
static bool tnm_input_is_disable_ok()
{
	// �d�w�R�[�����͋֎~�ɂȂ�Ȃ�
	if (Gp_global->ex_call_flag)
		return false;	// disable �ɂȂ�Ȃ�

	// ���b�Z�[�W�o�b�N���͋֎~�ɂȂ�Ȃ�
	if (tnm_msg_back_is_open())
		return false;	// disable �ɂȂ�Ȃ�

	// ���b�Z�[�W�E�B���h�E���B���Ă���ꍇ�͋֎~�ɂȂ�Ȃ�
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return false;	// disable �ɂȂ�Ȃ�

	return true;	// disable �ɂȂ��
}

// ****************************************************************
// �N���b�N�֎~��`�ɓ����Ă���
// ================================================================
static bool tnm_input_check_left_click_disable_rect()
{
	// �֎~��`�𔻒肷��
	C_point mp = Gp_cur_input->mouse.pos;
	C_rect rect = Gp_click_disable->m_rect;
	if (rect.left <= mp.x && mp.x < rect.right && rect.top <= mp.y && mp.y < rect.bottom)
		return true;	// �֎~����Ă�

	// �I�u�W�F�N�g�̃N���b�N�֎~��`�𒲂ׂ�
	C_elm_object* p_object;
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	for (int i = 0; i < p_front->m_object_list.get_size(); i++)	{
		p_object = &p_front->m_object_list[i];
		if (p_object->check_click_disable(mp, true))
			return true;	// �֎~����Ă�
	}
	for (int i = 0; i < p_front->m_mwnd_list.get_size(); i++)	{
		C_elm_mwnd* p_mwnd = &p_front->m_mwnd_list[i];
		for (int j = 0; j < p_mwnd->object_list().get_size(); j++)	{
			p_object = &p_mwnd->object_list()[j];
			if (p_object->check_click_disable(mp, true))
				return true;	// �֎~����Ă�
		}
	}

	return false;	// �֎~����ĂȂ�
}
static bool tnm_input_check_right_click_disable_rect()
{
	// �֎~��`�𔻒肷��
	C_point mp = Gp_cur_input->mouse.pos;
	C_rect rect = Gp_click_disable->m_rect;
	if (rect.left <= mp.x && mp.x < rect.right && rect.top <= mp.y && mp.y < rect.bottom)
		return true;	// �֎~����Ă�

	// �I�u�W�F�N�g�̃N���b�N�֎~��`�𒲂ׂ�
	C_elm_object* p_object;
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	for (int i = 0; i < p_front->m_object_list.get_size(); i++)	{
		p_object = &p_front->m_object_list[i];
		if (p_object->check_click_disable(mp, true))
			return true;	// �֎~����Ă�
	}
	for (int i = 0; i < p_front->m_mwnd_list.get_size(); i++)	{
		C_elm_mwnd* p_mwnd = &p_front->m_mwnd_list[i];
		for (int j = 0; j < p_mwnd->object_list().get_size(); j++)	{
			p_object = &p_mwnd->object_list()[j];
			if (p_object->check_click_disable(mp, true))
				return true;	// �֎~����Ă�
		}
	}

	return false;	// �֎~����ĂȂ�
}

// ****************************************************************
// �L�[���֎~����Ă���
// ================================================================
static bool tnm_input_check_key_disable(int key)
{
	if (Gp_local->pod.key_disable[key])
		return true;	// �֎~����Ă�

	return false;	// �֎~����ĂȂ�
}

// ****************************************************************
// �L�[�̏�Ԃ𔻒�
// ================================================================

// ****************************************************************
// ���͔��菈��
// ================================================================
static bool tnm_input_ex_key_proc_sub(C_input_state::BUTTON* p_key, int action_type, int check_type)
{
	switch (check_type)	{

		case TNM_KEY_CHECK_TYPE_REAL_STATE:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->is_down();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->is_up();
			}
			break;

		case TNM_KEY_CHECK_TYPE_CHECK_STOCK:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->check_down_stock();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->check_up_stock();
				case TNM_KEY_ACTION_TYPE_DOWN_UP:	return p_key->check_down_up_stock();
			}
			break;

		case TNM_KEY_CHECK_TYPE_USE_STOCK:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->use_down_stock();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->use_up_stock();
				case TNM_KEY_ACTION_TYPE_DOWN_UP:	return p_key->use_down_up_stock();
			}
			break;
	}

	return false;
}

static bool tnm_input_ex_key_proc_decide_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE �������� CHECK_STOCK �̏ꍇ�͕��ʂɒ��ׂ�
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_left_click_disable_rect()))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				return true;
			}
		}
	}

	// USE_STOCK �̏ꍇ�� CHECK_STOCK ���ɍs���A�Ō�Ɏ��Ԃ̂������`�`�F�b�N��������� USE_CHECK ���s��
	// ��`�`�F�b�N���Ō�Ɏ����Ă��Ă���_���d�v�ł��B
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_left_click_disable_rect()))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_keyboard_decide_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE �������� CHECK_STOCK �̏ꍇ�͕��ʂɒ��ׂ�
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				return true;
			}
		}
	}

	// USE_STOCK �̏ꍇ�� CHECK_STOCK ���ɍs���A�Ō�Ɏ��Ԃ̂������`�`�F�b�N��������� USE_CHECK ���s��
	// ��`�`�F�b�N���Ō�Ɏ����Ă��Ă���_���d�v�ł��B
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_cancel_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE �������� CHECK_STOCK �̏ꍇ�͕��ʂɒ��ׂ�
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_right_click_disable_rect()))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_ESCAPE)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('Z'))))	{
				return true;
			}
		}
	}

	// USE_STOCK �̏ꍇ�� CHECK_STOCK ���ɍs���A�Ō�Ɏ��Ԃ̂������`�`�F�b�N��������� USE_CHECK ���s��
	// ��`�`�F�b�N���Ō�Ɏ����Ă��Ă���_���d�v�ł��B
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_right_click_disable_rect()))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_ESCAPE)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('Z'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_normal_key_sub(bool disable_ok, int key, int action_type, int check_type)
{
	// REAL_STATE �������� CHECK_STOCK �̏ꍇ�͕��ʂɒ��ׂ�
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(key)))	{
				return true;
			}
		}
	}

	// USE_STOCK �̏ꍇ�� CHECK_STOCK ���ɍs���A�Ō�Ɏ��Ԃ̂������`�`�F�b�N��������� USE_CHECK ���s��
	// ��`�`�F�b�N���Ō�Ɏ����Ă��Ă���_���d�v�ł��B
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(key)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc(int ex_key, int action_type, int check_type)
{
	// �X�N���v�g�ł̋֎~�s�\���\���𔻒�
	bool disable_ok = tnm_input_is_disable_ok();

	// ����L�[
	if (ex_key == VK_EX_DECIDE)	{
		if (tnm_input_ex_key_proc_decide_key_sub(disable_ok, action_type, check_type))	{
			return true;	// �����Ă���
		}
	}
	// �L�[�{�[�h�̌���L�[
	else if (ex_key == VK_EX_KEYBOARD_DECIDE)	{
		if (tnm_input_ex_key_proc_keyboard_decide_key_sub(disable_ok, action_type, check_type))	{
			return true;	// �����Ă���
		}
	}
	// �L�����Z���L�[
	else if (ex_key == VK_EX_CANCEL)	{
		if (tnm_input_ex_key_proc_cancel_key_sub(disable_ok, action_type, check_type))	{
			return true;	// �����Ă���
		}
	}
	// ���ʂ̃L�[
	else	{
		if (tnm_input_ex_key_proc_normal_key_sub(disable_ok, ex_key, action_type, check_type))	{
			return true;	// �����Ă���
		}
	}

	return false;
}

// ****************************************************************
// �e����͔���C���^�[�t�F�C�X
// ================================================================
C_point tnm_input_get_mouse_pos()
{
	return Gp_cur_input->mouse.pos;
}

bool tnm_input_is_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_REAL_STATE);
}
bool tnm_input_is_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_REAL_STATE);
}
bool tnm_input_check_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_check_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_check_key_down_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN_UP, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_use_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
bool tnm_input_use_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
bool tnm_input_use_key_down_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN_UP, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
