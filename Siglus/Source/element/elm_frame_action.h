#pragma		once

#include	"elm_prop.h"
#include	"elm_list.h"
#include	"elm_counter.h"

class	C_tnm_save_stream;

// ****************************************************************
// �t���[���A�N�V����
// ================================================================
class C_elm_frame_action : public C_tnm_element
{
public:

	void	init(S_element element, CTSTR& name, S_element target);	// ������
	void	reinit(bool finish_flag);								// �ď�����
	void	finish();												// �I��
	void	save(C_tnm_save_stream& stream);						// �Z�[�u
	void	load(C_tnm_save_stream& stream);						// ���[�h
	void	copy(const C_elm_frame_action* src, bool end_flag);		// �R�s�[
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	void	frame();												// �t���[������

	// �p�����[�^��ݒ�
	void	set_param(int end_time, int real_flag, CTSTR& scn_name, CTSTR& user_cmd_name, C_tnm_prop_list& arg_list);
	void	do_action();											// �A�N�V���������s
	void	do_action(int scn_no, int user_cmd_no);					// �A�N�V���������s
	bool	is_end_action()	{	return m_end_action_flag;	}		// �G���h�A�N�V���������𔻒�

	S_element			m_target;			// �^�[�Q�b�g
	int					m_scn_no;			// �V�[���ԍ�
	int					m_cmd_no;			// �R�}���h�ԍ�
	int					m_end_time;			// �I������
	TSTR				m_scn_name;			// �V�[����
	TSTR				m_cmd_name;			// �R�}���h��
	C_tnm_prop_list		m_arg_list;			// �������X�g
	C_elm_counter		m_counter;			// �J�E���^
	bool				m_end_action_flag;	// �G���h�A�N�V�������t���O

private:
	void	restruct();						// �č\�z
};

// ****************************************************************
// �t���[���A�N�V�������X�g
// ================================================================
class C_elm_frame_action_list : public C_elm_list_ex<C_elm_frame_action>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target);	// ������
	void	reinit(bool finish_flag, bool restruct_flag);											// �ď�����
	void	finish();																				// �I��

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end, bool restruct_flag);		// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	S_element	m_target;
};
