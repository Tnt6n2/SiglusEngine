#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// �����C�x���g�F�X�s�[�h�^�C�v
// ================================================================
enum E_tnm_int_event_speed_type
{
	E_tnm_int_event_speed_type_none = -1,
	E_tnm_int_event_speed_type_linear = 0,
	E_tnm_int_event_speed_type_speed_up = 1,
	E_tnm_int_event_speed_type_speed_down = 2,
};

// ****************************************************************
// �����C�x���g�F���[�v�^�C�v
// ================================================================
enum E_tnm_int_event_loop_type
{
	E_tnm_int_event_loop_type_none = -1,
	E_tnm_int_event_loop_type_oneshot = 0,
	E_tnm_int_event_loop_type_loop = 1,
	E_tnm_int_event_loop_type_turn = 2,

	// �R�Ԃ� Flix �Łu�h��v���쐬��
	// �P�O�O�Ԉȍ~�� Flix �ŋg�c���񂪎���
};

// ****************************************************************
// �����C�x���g
// ================================================================
class C_elm_int_event	// �������̂��߃G�������g���p�����Ȃ��I
{
public:

	// ������
	void	init(int def_value);

	// �ď�����
	void	reinit();

	// ���Ԃ��X�V
	void	update_time_sub(int past_game_time, int past_real_time);
	void	update_time(int past_game_time, int past_real_time)
	{
		// ���蕔�������C�����C�������邱�Ƃō�������
		if (m_loop_type != E_tnm_int_event_loop_type_none)
			update_time_sub(past_game_time, past_real_time);
	}

	// �t���[������
	void	frame_sub();
	void	frame()
	{
		// ���蕔�������C�����C�������邱�Ƃō�������
		m_cur_value = m_value;
		if (m_loop_type != E_tnm_int_event_loop_type_none)
			frame_sub();
	}
	void	frame_no_event()
	{
		// �C�x���g���Ȃ����Ƃ��������Ă���ꍇ�͂�������Ăяo�����������ł��B
		m_cur_value = m_value;
	}

	// �l��ݒ�^�擾
	void	set_value(int value)	{	m_value = value;	}
	int		get_value() const		{	return m_value;		}

	// ���v�l���擾
	int		get_total_value() const	{	return m_cur_value;	}

	// �C�x���g
	void	set_event(int value, int total_time, int delay_time, int speed_type, int real_flag);
	void	loop_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag);
	void	turn_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag);
	void	end_event();
	bool	check_event()	{	return m_loop_type != E_tnm_int_event_loop_type_none;	}	// ������

	int		m_def_value;
	int		m_value;
	int		m_cur_time;
	int		m_end_time;
	int		m_delay_time;
	int		m_start_value;
	int		m_cur_value;
	int		m_end_value;
	int		m_loop_type;
	int		m_speed_type;
	int		m_real_flag;
};

// ****************************************************************
// �����C�x���g���X�g
// ================================================================
class C_elm_int_event_list : public C_elm_list<C_elm_int_event>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, int def_value);	// ������

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	int		m_def_value;
};
