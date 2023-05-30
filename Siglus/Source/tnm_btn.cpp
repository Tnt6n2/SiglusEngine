#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_input.h"
#include	"engine/eng_input.h"

// ****************************************************************
// �{�^��
// ================================================================
C_tnm_btn::C_tnm_btn()
{
	m_rotate = 0;
	m_rect_2_use = false;
	m_push_keep = false;
	m_is_decided = false;
}

C_tnm_btn::~C_tnm_btn()
{
}

// ������
void C_tnm_btn::init()
{
	m_guid.init();
	m_rect = C_rect(0, 0, 0, 0);
	m_rect_2 = C_rect(0, 0, 0, 0);
	m_center = C_point(0, 0);
	m_rotate = 0;
	m_sorter = S_tnm_sorter(0, 0);
	m_rect_2_use = false;
	m_push_keep = false;
	m_is_decided = false;
	m_texture.reset();
}

// �t���[��������
void C_tnm_btn::frame_init()
{
	m_rect = C_rect(0, 0, 0, 0);
	m_rect_2 = C_rect(0, 0, 0, 0);
	m_center = C_point(0, 0);
	m_rotate = 0;
	m_sorter = S_tnm_sorter(0, 0);
	m_rect_2_use = false;
	m_push_keep = false;
	m_texture.reset();
}

// �p�����[�^��ݒ�
void C_tnm_btn::set_rect(C_rect rect)
{
	m_rect = rect;
}

void C_tnm_btn::set_rect_2(bool use, C_rect rect)
{
	m_rect_2_use = use;
	m_rect_2 = rect;
}

void C_tnm_btn::set_center(C_point center)
{
	m_center = center;
}

void C_tnm_btn::set_rotate(int rotate)
{
	m_rotate = rotate;
}

void C_tnm_btn::set_sorter(S_tnm_sorter sorter)
{
	m_sorter = sorter;
}

void C_tnm_btn::set_push_keep(bool push_keep)
{
	m_push_keep = push_keep;
}

void C_tnm_btn::set_param(C_rect rect, S_tnm_sorter sorter)
{
	m_rect = rect;
	m_sorter = sorter;
}

void C_tnm_btn::set_texture(BSP<C_d3d_texture> texture)
{
	m_texture = texture;
}

// �q�b�g�e�X�g
bool C_tnm_btn::hit_test(C_point pos)
{
	bool hit = true;

	// ��Q��`���ɔ���
	if (m_rect_2_use)
	{
		hit &= m_rect_2.is_in(pos);
	}

	// ��P��`�͉�]���邱�Ƃ�����
	if (hit && m_rotate)
	{
		// �}�E�X�ʒu�� center �ɍ��킹�ċt��]����
		double rotate_rad = -(double)m_rotate * D3DX_PI / 1800;

		C_point new_pos;
		new_pos.x = (LONG)((double)(pos.x - m_center.x) * cos(rotate_rad) - (double)(pos.y - m_center.y) * sin(rotate_rad) + m_center.x);
		new_pos.y = (LONG)((double)(pos.x - m_center.x) * sin(rotate_rad) + (double)(pos.y - m_center.y) * cos(rotate_rad) + m_center.y);

		pos = new_pos;
	}

	// �e�N�X�`�����w�肳��Ă���ꍇ�̓e�N�X�`�����Q��
	if (hit && m_texture)
	{
		bool texture_hit = false;

		C_rect texture_rect = m_texture->get_rect();
		C_point pos_on_texture = pos - m_center;
		pos_on_texture.x = pos_on_texture.x * m_texture->get_size().cx / m_rect.width();
		pos_on_texture.y = pos_on_texture.y * m_texture->get_size().cy / m_rect.height();

		if (texture_rect.is_in(pos_on_texture))
		{
			C_argb color = m_texture->get_color(pos_on_texture.x, pos_on_texture.y);
			if (color.a > 0)
			{
				texture_hit = true;
			}
		}

		hit &= texture_hit;
	}
	// �e�N�X�`�����ݒ肳��Ă��Ȃ��ꍇ�͋�`�Ŕ���
	else
	{
		hit &= m_rect.is_in(pos);
	}

	return hit;
}


// ****************************************************************
// �{�^���}�l�[�W���F������
// ================================================================
void C_tnm_btn_mng::init()
{
	m_last_mouse_push_flag = false;
	m_last_active_btn_guid = -1;
}

// ****************************************************************
// �{�^���}�l�[�W���F�t���[��������
// ================================================================
bool C_tnm_btn_mng::frame_init()
{
	m_btn_list.clear();

	return true;
}

// ****************************************************************
// �{�^���}�l�[�W���F�{�^����o�^
// ================================================================
void C_tnm_btn_mng::regist_button(C_tnm_btn* btn)
{
	m_btn_list.push_back(btn);
}

// ****************************************************************
// �{�^���}�l�[�W���F�{�^�����A�N�e�B�u���ǂ����𔻒�
// ================================================================
bool C_tnm_btn_mng::is_active(C_tnm_btn* p_btn)
{
	return m_active_btn_guid != -1 && m_active_btn_guid == p_btn->get_guid();
}

// ****************************************************************
// �{�^���}�l�[�W���F�������Ԃ��擾
// ================================================================
int C_tnm_btn_mng::get_hit_state(C_tnm_btn* p_btn)
{
	// �A�N�V������I��
	if (false);
	else if (is_active(p_btn) && get_active_btn_state() == TNM_BTN_STATE_HIT)	{
		return TNM_BTN_STATE_HIT;
	}
	else if (is_active(p_btn) && get_active_btn_state() == TNM_BTN_STATE_PUSH)	{
		return TNM_BTN_STATE_PUSH;
	}
	else	{
		return TNM_BTN_STATE_NORMAL;
	}
}

// ****************************************************************
// �{�^���}�l�[�W���F�����蔻�菈��
// ================================================================
bool C_tnm_btn_mng::hit_test_proc()
{
	// �t���[��������
	m_hit_this_frame = false;
	m_pushed_this_frame = false;
	m_decided_this_frame = false;

	// ����̃X�e�[�g���擾
	m_active_btn_guid = -1;
	m_active_btn_sorter.order = 0;
	m_active_btn_sorter.layer = 0;
	m_active_btn_state = TNM_BTN_STATE_NORMAL;

	// ���͂��擾
	C_point mouse_pos = tnm_input_get_mouse_pos();
	bool mouse_push_flag = m_last_mouse_push_flag;
	if (tnm_input_is_key_up(VK_EX_DECIDE))		mouse_push_flag = false;
	if (tnm_input_is_key_down(VK_EX_DECIDE))	mouse_push_flag = true;

	// �P�t���[�����Ɂu�����ĕ������v�ꍇ�A�u�������v��Ԃɂ���
	// �����I�Ɂu�������v�͎��t���[���ɏ��������
	// ����̓^�b�`�p�l���΍�ł��B�ؖ{����̂�������łP�^�b�`�����ꍇ�A
	// �������Ƃ��̂P�t���[�����ɉ����������������܂��B
	//if (m_last_mouse_push_flag == false && mouse_push_flag == false)	{
	//	if (tnm_input_check_key_down_up(VK_EX_DECIDE))	{
	//		mouse_push_flag = true;							// �u�������v
	//	}
	//}

	// ���������肵���t���O�i�u���������v�u�������v�u���肵���v�u�L�����Z�������v�Ȃǂ̏����������������Ȃ��悤�ɂ��邽�߂̃t���O�j
	bool end = false;

	// �{�^����GUID�����炩���ߗ^���Ă���
	for (int b = 0; b < (int)m_btn_list.size(); b++)	{
		C_tnm_btn* btn = m_btn_list[b];

		// �{�^����GUID��^����
		btn->create_guid();
	}

	// �P�D�O�񉟂��Ă��č�������Ă��Ȃ��ꍇ�A�A�N�e�B�u���ێ�����
	if (m_last_mouse_push_flag && mouse_push_flag)	{

		// �A�N�e�B�u���ێ�����
		m_active_btn_guid = m_last_active_btn_guid;

		// �S�{�^���ɑ΂��ď������s��
		for (int b = 0; b < (int)m_btn_list.size(); b++)	{
			C_tnm_btn* btn = m_btn_list[b];

			// �A�N�e�B�u�ȃ{�^���̏ꍇ
			if (m_active_btn_guid == btn->get_guid())	{

				m_active_btn_sorter = btn->m_sorter;		// �A�N�e�B�u�ȃ{�^���̃\�[�^�[
				m_active_btn_state = (btn->m_push_keep || btn->hit_test(mouse_pos)) ? TNM_BTN_STATE_PUSH : TNM_BTN_STATE_NORMAL;

				// ���A�N�e�B�u�Ȃ̂� TNM_HIT_STATE_NORMAL �Ƃ́H
				//		�A�N�e�B�u�ȃq�b�g�������ƁATNM_HIT_STATE_PUSH ��ԂɂȂ�܂��B
				//		�������܂܃}�E�X����������ƁA�A�N�e�B�u��ۂ����܂܉����Ă��Ȃ���ԁA�܂� TNM_HIT_STATE_NORMAL �ɂȂ�܂��B
			}
		}
	}

	// �Q�D����ȊO�̏ꍇ�́A�A�N�e�B�u���ێ����Ȃ���������Ȃ�
	else	{

		// �Q�|�P�D�O�񉟂��Ă������Ǎ��񗣂����ꍇ�A���蔻����s��
		if (m_last_mouse_push_flag && !mouse_push_flag)	{

			// �S�{�^���ɑ΂��ď������s��
			for (int b = 0; b < (int)m_btn_list.size(); b++)	{
				C_tnm_btn* btn = m_btn_list[b];

				// �O��A�N�e�B�u�������{�^���̏ꍇ
				if (m_last_active_btn_guid == btn->get_guid())	{

					tnm_input_use_key_up(VK_EX_DECIDE);		// ���͂�����

					// �O�񓖂����Ă����ꍇ
					if (m_last_active_btn_state == TNM_BTN_STATE_PUSH)	{

						m_active_btn_guid = btn->get_guid();		// �A�N�e�B�u�ȃ{�^����GUID
						m_active_btn_sorter = btn->m_sorter;		// �A�N�e�B�u�ȃ{�^���̃\�[�^�[
						m_active_btn_state = TNM_BTN_STATE_PUSH;	// ������
						m_decided_this_frame = true;				// ���񌈒肳�ꂽ
						btn->m_is_decided = true;					// ���肳�ꂽ
						end = true;									// �������s����
					}

					break;							// ���[�v�A�E�g
				}
			}
		}

		// �Q�|�Q�D�܂��N���A�N�e�B�u�ɂȂ��Ă��Ȃ��ꍇ�A�A�N�e�B�u�ȃ{�^���𔻒肷��
		if (!end && m_active_btn_guid == -1)	{

			// �}�E�X����ʓ��ɓ����Ă���ꍇ�̂݃{�^���ɓ�����
			if (0 <= mouse_pos.x && mouse_pos.x < Gp_global->game_screen_size.cx && 0 <= mouse_pos.y && mouse_pos.y < Gp_global->game_screen_size.cy)
			{
				// �S�{�^���ɑ΂��ď������s��
				for (int b = 0; b < (int)m_btn_list.size(); b++)	{
					C_tnm_btn* btn = m_btn_list[b];

					// �������Ă���
					if (btn->hit_test(mouse_pos) && Gp_global->active_flag)	{

						// �ŏ��̃q�b�g�������͌��݂̃q�b�g����̃\�[�^�[�ɂ���
						if (m_active_btn_guid == -1 || btn->m_sorter >= m_active_btn_sorter)	{		// ��D��I '>' �ł͂Ȃ� '>='

							m_active_btn_guid = btn->get_guid();	// �A�N�e�B�u�ȃ{�^����GUID
							m_active_btn_sorter = btn->m_sorter;	// �A�N�e�B�u�ȃ{�^���̃\�[�^�[
						}
					}
				}
			}

			// �����܂łŃA�N�e�B�u�ȃ{�^�������肵���͂�
			if (m_active_btn_guid >= 0)	{

				// �{�^�����������ꍇ
				if (tnm_input_check_key_down(VK_EX_DECIDE))	{

					// �A�N�e�B�u�ȃ{�^���̃X�e�[�g��ύX
					m_active_btn_state = TNM_BTN_STATE_PUSH;

					// ���񉟂�������
					if (m_last_active_btn_guid != m_active_btn_guid || m_last_active_btn_state != m_active_btn_state || tnm_input_check_key_down(VK_EX_DECIDE))	{
						m_pushed_this_frame = true;				// ���񉟂����t���O
						tnm_input_use_key_down(VK_EX_DECIDE);	// ���͂�����
						end = true;								// �������s����
					}
				}

				// �{�^���������Ă��Ȃ��ꍇ
				else	{

					// �A�N�e�B�u�ȃ{�^���̃X�e�[�g��ύX
					m_active_btn_state = TNM_BTN_STATE_HIT;

					// ����q�b�g��������
					if (m_last_active_btn_guid != m_active_btn_guid || m_last_active_btn_state != m_active_btn_state)	{
						m_hit_this_frame = true;		// ���񓖂������t���O
						end = true;						// �������s����
					}
				}

			}
		}
	}

	// �X�e�[�g��ێ����Ă���
	m_last_active_btn_guid = m_active_btn_guid;
	m_last_active_btn_sorter = m_active_btn_sorter;
	m_last_active_btn_state = m_active_btn_state;
	m_last_mouse_push_flag = mouse_push_flag;

	return true;
}

