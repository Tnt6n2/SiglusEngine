#pragma		once

#include	"data/tnm_element.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;

// ****************************************************************
// �J�E���^�F�p�����[�^
// ================================================================
struct S_tnm_counter_param
{
	// �ʏ�p�����[�^
	bool	is_running;			// ���s���t���O
	bool	real_flag;			// �����ԃ��[�h
	bool	frame_mode;			// �t���[�����[�h
	bool	frame_loop_flag;	// ���[�v�t���O
	int		frame_start_value;	// �J�n�l
	int		frame_end_value;	// �I���l
	int		frame_time;			// �t���[������

	// �ꎞ�p�����[�^
	int		cur_time;			// ���v����
};

// ****************************************************************
// �J�E���^
// ================================================================
class C_elm_counter : public C_tnm_element
{
public:

	// �G�������g�̊�{����
	void	init(S_element element, CTSTR& name);					// ������
	void	reinit();												// �ď�����
	void	save(C_tnm_save_stream& stream);						// �Z�[�u
	void	load(C_tnm_save_stream& stream);						// ���[�h
	void	copy(const C_elm_counter* src);							// �R�s�[
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ�i�߂�

	// �J�E���^�̑���
	void	reset();			// ���Z�b�g
	void	start();			// �v���J�n
	void	start_real();		// �v���J�n�i�����ԁj
	void	start_frame(int start_value, int end_value, int frame_time);
	void	start_frame_real(int start_value, int end_value, int frame_time);
	void	start_frame_loop(int start_value, int end_value, int frame_time);
	void	start_frame_loop_real(int start_value, int end_value, int frame_time);
	void	stop();				// �ꎞ��~
	void	resume();			// ���݂̒l����ĊJ

	// ��Ԏ擾
	void	set_count(int count);
	int		get_count() const;
	bool	is_active() const;

	// �p�����[�^�擾
	const S_tnm_counter_param&	param() const	{	return m_param;	}

private:

	S_tnm_counter_param		m_param;
};


// ****************************************************************
// �J�E���^���X�g
// ================================================================
class C_elm_counter_list : public C_elm_list<C_elm_counter>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

