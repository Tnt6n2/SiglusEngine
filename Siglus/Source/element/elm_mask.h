#pragma		once

#include	"data/tnm_element.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// �G�������g�F�}�X�N�F�p�����[�^
// ================================================================
struct C_elm_mask_param
{
	C_elm_int_event		x;
	C_elm_int_event		y;
};

// ****************************************************************
// �G�������g�F�}�X�N
// ================================================================
class C_elm_mask : public C_tnm_element
{
public:
	// �G�������g�̊�{����
	void	init(S_element element, CTSTR& name);					// ������
	void	reinit();												// �ď�����
	void	save(C_tnm_save_stream& stream);						// �Z�[�u
	void	load(C_tnm_save_stream& stream);						// ���[�h

	// �}�X�N���쐬
	void	create(CTSTR& file_name);

	// �t���[������
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	void	frame();												// �t���[������

	// �p�����[�^��ݒ�
	void				set_x(int x)			{	m_param.x.set_value(x);		}
	void				set_y(int y)			{	m_param.y.set_value(y);		}

	// �p�����[�^���擾
	int					get_x() const			{	return m_param.x.get_value();	}
	int					get_y() const			{	return m_param.y.get_value();	}
	TSTR				get_mask_name() const	{	return m_mask_name;				}
	BSP<C_d3d_album>	get_mask_album() const	{	return m_album;					}

	// �C�x���g���擾
	C_elm_int_event&	x_event()				{	return m_param.x;	}
	C_elm_int_event&	y_event()				{	return m_param.y;	}

	// ���v�l���擾
	int					get_total_x() const		{	return m_param.x.get_total_value();	}
	int					get_total_y() const		{	return m_param.y.get_total_value();	}

private:

	// �č\�z
	void	restruct();

	// �p�����[�^
	TSTR				m_mask_name;
	C_elm_mask_param	m_param;

	// ��ƃp�����[�^
	BSP<C_d3d_album>	m_album;
};

// ****************************************************************
// �G�������g�F�}�X�N���X�g
// ================================================================
class C_elm_mask_list : public C_elm_list<C_elm_mask>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

