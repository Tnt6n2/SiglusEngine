#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_counter.h"

// ****************************************************************
// �J�E���^�F������
// ================================================================
void C_elm_counter::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_COUNTER, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �J�E���^�F�ď�����
// ================================================================
void C_elm_counter::reinit()
{
	// �p�����[�^
	m_param.is_running = false;
	m_param.real_flag = false;
	m_param.frame_mode = false;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = 0;
	m_param.frame_end_value = 0;
	m_param.frame_time = 0;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;
}

// ****************************************************************
// �J�E���^�F�Z�[�u
// ================================================================
void C_elm_counter::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// �J�E���^�F���[�h
// ================================================================
void C_elm_counter::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// �J�E���^�F�R�s�[
// ================================================================
void C_elm_counter::copy(const C_elm_counter* src)
{
	m_param = src->m_param;
}

// ****************************************************************
// �J�E���^�F���Z�b�g
// ================================================================
void C_elm_counter::reset()
{
	// �p�����[�^
	m_param.is_running = false;
	m_param.real_flag = false;
	m_param.frame_mode = false;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g

	// frame_mode ������������̂͏d�v�ł��B
	// ��������������Ȃ��ꍇ�Aget_count ���t���[���̍ŏI�l��Ԃ������܂��B
	// 0 ��Ԃ��悤�ɂ��Ȃ���΂����܂���B
}

// ****************************************************************
// �J�E���^�F�J�n
// ================================================================
void C_elm_counter::start()
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = false;
	m_param.frame_mode = false;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�J�n�i�����ԁj
// ================================================================
void C_elm_counter::start_real()
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = true;
	m_param.frame_mode = false;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�t���[���J�n
// ================================================================
void C_elm_counter::start_frame(int start_value, int end_value, int frame_time)
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = false;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�t���[���J�n�i�����ԁj
// ================================================================
void C_elm_counter::start_frame_real(int start_value, int end_value, int frame_time)
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = true;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�t���[���J�n�i���[�v�j
// ================================================================
void C_elm_counter::start_frame_loop(int start_value, int end_value, int frame_time)
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = false;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = true;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�t���[���J�n�i���[�v�j�i�����ԁj
// ================================================================
void C_elm_counter::start_frame_loop_real(int start_value, int end_value, int frame_time)
{
	// �p�����[�^
	m_param.is_running = true;			// �J�n
	m_param.real_flag = true;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = true;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// �ꎞ�p�����[�^
	m_param.cur_time = 0;				// ���Z�b�g
}

// ****************************************************************
// �J�E���^�F�ꎞ��~
// ================================================================
void C_elm_counter::stop()
{
	m_param.is_running = false;		// ��~
}

// ****************************************************************
// �J�E���^�F���݂̒l����ĊJ
// ================================================================
void C_elm_counter::resume()
{
	m_param.is_running = true;		// �J�n
}

// ****************************************************************
// �J�E���^�F���Ԃ�i�߂�
// ================================================================
void C_elm_counter::update_time(int past_game_time, int past_real_time)
{
	// ���s���Ȃ獇�v���������Z
	if (m_param.is_running)	{
		m_param.cur_time += m_param.real_flag ? past_real_time : past_game_time;
	}

	// �t���[���̏ꍇ�A���Ԃ�������~�߂�
	if (m_param.frame_mode && !m_param.frame_loop_flag)	{
		if (m_param.cur_time >= m_param.frame_time)	{
			m_param.is_running = false;
		}
	}
}

// ****************************************************************
// �J�E���^�F�l���擾
// ================================================================
int C_elm_counter::get_count() const
{
	// �t���[���̏ꍇ
	if (m_param.frame_mode)	{

		int value = 0;

		if (m_param.frame_time <= 0)									// ��O�����F�t���[���ɂ����鎞�Ԃ��O�ȉ�
			return m_param.frame_end_value;
		if (m_param.frame_start_value == m_param.frame_end_value)		// ��O�����F�t���[���̊J�n�l�ƏI���l������
			return m_param.frame_end_value;

		// �l���v�Z
		value = (int)((double)(m_param.frame_end_value - m_param.frame_start_value) * m_param.cur_time / m_param.frame_time);

		// ���[�v�̏ꍇ
		if (m_param.frame_loop_flag)	{
			value %= m_param.frame_end_value - m_param.frame_start_value;
			value += m_param.frame_start_value;
		}
		// ���[�v�łȂ��ꍇ
		else	{
			value += m_param.frame_start_value;
			if (m_param.frame_start_value > m_param.frame_end_value)
				value = limit(m_param.frame_end_value, value, m_param.frame_start_value);
			else
				value = limit(m_param.frame_start_value, value, m_param.frame_end_value);
		}

		return value;
	}

	// �J�E���^�̏ꍇ
	else	{

		return m_param.cur_time;
	}
}

// ****************************************************************
// �J�E���^�F�l��ݒ�
// ================================================================
void C_elm_counter::set_count(int value)
{
	// �t���[���̏ꍇ
	if (m_param.frame_mode)	{

		if (m_param.frame_end_value == m_param.frame_start_value)	{	// ��O�����F�t���[���ɂ����鎞�Ԃ��O�ȉ�
			m_param.cur_time = 0;
			return;
		}

		// ���[�v�̏ꍇ
		if (m_param.frame_loop_flag)	{
			m_param.cur_time = (value - m_param.frame_start_value) * m_param.frame_time / (m_param.frame_end_value - m_param.frame_start_value);
			m_param.cur_time = limit(0, m_param.cur_time, m_param.frame_time - 1);	// ���[�v�̏ꍇ�� -1 �܂ł����s���Ȃ�
		}
		// ���[�v�łȂ��ꍇ
		else	{
			m_param.cur_time = (value - m_param.frame_start_value) * m_param.frame_time / (m_param.frame_end_value - m_param.frame_start_value);
			m_param.cur_time = limit(0, m_param.cur_time, m_param.frame_time);		// ���[�v�łȂ��ꍇ�͍Ō�܂ōs��
		}

	}
	else	{
		m_param.cur_time = value;
	}
}

// ****************************************************************
// �J�E���^�F�A�N�e�B�u���ǂ�������
// ================================================================
bool C_elm_counter::is_active() const
{
	return m_param.is_running;
}

// ****************************************************************
// �J�E���^���X�g�F�T�u�̏�����
// ================================================================
void C_elm_counter_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �J�E���^���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_counter_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �J�E���^���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_counter_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �J�E���^���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_counter_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
