#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_int_event.h"

// ****************************************************************
// �����C�x���g�F������
// ================================================================
void C_elm_int_event::init(int def_value)
{
	// �Œ�p�����[�^��ݒ�
	m_def_value = def_value;

	// �ŏ�����
	reinit();
}

// ****************************************************************
// �����C�x���g�F�ď�����
// ================================================================
void C_elm_int_event::reinit()
{
	m_value = m_def_value;
	m_cur_time = 0;
	m_end_time = 0;
	m_delay_time = 0;
	m_start_value = m_def_value;
	m_cur_value = m_def_value;
	m_end_value = m_def_value;
	m_loop_type = E_tnm_int_event_loop_type_none;
	m_speed_type = E_tnm_int_event_speed_type_none;
	m_real_flag = 0;
}

// ****************************************************************
// �����C�x���g�F�C�x���g��ݒ�
// ================================================================
void C_elm_int_event::set_event(int value, int total_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = total_time;
	m_delay_time = delay_time;
	m_start_value = m_value;
	m_cur_value = m_value;
	m_end_value = value;
	m_loop_type = E_tnm_int_event_loop_type_oneshot;
	m_speed_type = speed_type;
	m_real_flag = real_flag;

	// ���݂̒l���X�V
	m_value = value;
}

// ****************************************************************
// �����C�x���g�F���[�v�C�x���g��ݒ�
// ================================================================
void C_elm_int_event::loop_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = loop_time;
	m_delay_time = delay_time;
	m_start_value = start_value;
	m_cur_value = start_value;
	m_end_value = end_value;
	m_loop_type = E_tnm_int_event_loop_type_loop;
	m_speed_type = speed_type;
	m_real_flag = real_flag;
}

// ****************************************************************
// �����C�x���g�F�^�[���C�x���g��ݒ�
// ================================================================
void C_elm_int_event::turn_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = loop_time;
	m_delay_time = delay_time;
	m_start_value = start_value;
	m_cur_value = start_value;
	m_end_value = end_value;
	m_loop_type = E_tnm_int_event_loop_type_turn;
	m_speed_type = speed_type;
	m_real_flag = real_flag;
}

// ****************************************************************
// �����C�x���g�F�C�x���g���I��
// ================================================================
void C_elm_int_event::end_event()
{
	m_loop_type = E_tnm_int_event_loop_type_none;
}

// ****************************************************************
// �����C�x���g�F�C�x���g�𔻒�
// ================================================================
/*
bool C_elm_int_event::check_event()
{
	return m_loop_type != E_tnm_int_event_loop_type_none;
}
*/

// ****************************************************************
// �����C�x���g�F���Ԃ�i�߂�
// ================================================================
void C_elm_int_event::update_time_sub(int game_past_time, int real_past_time)
{
	// ������i�߂�
	if (m_real_flag == 0)	{
		m_cur_time += game_past_time;
	}
	else	{
		m_cur_time += real_past_time;
	}
}

// ****************************************************************
// �����C�x���g�F�t���[������
// ================================================================
//		���C���ł��� frame() �̓w�b�_�Œ�`����Ă��܂��B�i�C�����C���j
// ================================================================
void C_elm_int_event::frame_sub()
{
	// �p�����[�^���擾
	int end_time = m_end_time;
	int start_value = m_start_value;
	int end_value = m_end_value;
	int cur_time = m_cur_time - m_delay_time;

	// �������Ŏ������O�ȏ�ɕ␳���Ă͂����܂���B
	// �C�x���g���ԁ��O�A�C�����ԁ��P�O�O�O�̏ꍇ�A�����ɃC�x���g���I����Ă��܂�����ł��B
	// �i�P�O�O�O�ؕb�҂ׂ��ł��j

	// �����V���b�g�̏ꍇ�͏I���������߂��Ă�����C�x���g�I��
	if (m_loop_type == E_tnm_int_event_loop_type_oneshot)	{
		if (cur_time - end_time >= 0)	{
			m_loop_type = E_tnm_int_event_loop_type_none;
			return;
		}
	}

	// �J�n�����ɂȂ��Ă��Ȃ���ΊJ�n�l
	if (cur_time <= 0)	{
		m_cur_value = start_value;
		return;
	}

	// �����V���b�g�̏ꍇ�͏I���������߂��Ă�����I���l
	//if (m_loop_type == E_tnm_int_event_loop_type_oneshot)	{
	//	if (cur_time - end_time >= 0)	{
	//		m_cur_value = end_value;
	//		return;
	//	}
	//}

	// ���[�v�̏ꍇ�̎��ԕ␳
	if (m_loop_type == E_tnm_int_event_loop_type_loop)	{
		cur_time %= end_time;
	}

	// �^�[���̏ꍇ�̎��ԕ␳
	if (m_loop_type == E_tnm_int_event_loop_type_turn)	{
		cur_time %= end_time * 2;

		if (cur_time - end_time > 0)	{
			cur_time = end_time - (cur_time - end_time);
		}
	}

	if (false);
	else if (m_speed_type == 0)	{	// ���`
		m_cur_value = (int)((double)(end_value - start_value) * (cur_time) / (end_time) + start_value);
	}
	else if (m_speed_type == 1)	{	// ����
		m_cur_value = (int)((double)(end_value - start_value) * (cur_time) * (cur_time) / (end_time) / (end_time) + start_value);
	}
	else if (m_speed_type == 2)	{	// ����
		m_cur_value = (int)(- (double)(end_value - start_value) * (cur_time - end_time) * (cur_time - end_time) / (end_time) / (end_time) + end_value);
	}
}

// ****************************************************************
// �������X�g�C�x���g�F������
// ================================================================
void C_elm_int_event_list::init(S_element element, CTSTR& name, int size, bool extend_enable, int def_value)
{
	// �Œ�p�����[�^
	m_def_value = def_value;

	// ���̏�����
	C_elm_list<C_elm_int_event>::init(element, FM_INTEVENTLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// �����C�x���g���X�g�F������
// ================================================================
void C_elm_int_event_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(m_def_value);
}

// ****************************************************************
// �����C�x���g���X�g�F�ď�����
// ================================================================
void C_elm_int_event_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �����C�x���g���X�g�F�Z�[�u
// ================================================================
void C_elm_int_event_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	// �����C�x���g�͂o�n�c�Ȃ̂� memcpy �ňꊇ�Z�[�u����
	stream.save(m_sub.get() + begin, (end - begin) * sizeof(C_elm_int_event));
}

// ****************************************************************
// �����C�x���g���X�g�F���[�h
// ================================================================
void C_elm_int_event_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	// �����C�x���g�͂o�n�c�Ȃ̂� memcpy �ňꊇ���[�h����
	stream.load(m_sub.get() + begin, (end - begin) * sizeof(C_elm_int_event));
}
