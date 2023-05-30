#pragma		once

#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// �t�H�O
// ================================================================
class C_tnm_fog
{
public:
	void	init_local();
	void	save_local(C_tnm_save_stream& stream);
	void	load_local(C_tnm_save_stream& stream);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	// �t�H�O���쐬
	void	create_fog(CTSTR& name);

	// �p�����[�^��ݒ�^�擾
	void	set_fog_x(int fog_x)		{	m_fog_x.set_value(fog_x);	}
	void	set_fog_near(int fog_near)	{	m_fog_near = fog_near;		}
	void	set_fog_far(int fog_far)	{	m_fog_far = fog_far;		}
	int		get_fog_x()					{	return m_fog_x.get_value();	}
	int		get_fog_near()				{	return m_fog_near;			}
	int		get_fog_far()				{	return m_fog_far;			}

	// �C�x���g
	int		get_total_x()				{	return m_fog_x.get_total_value();	}

	// �C�x���g�{��
	C_elm_int_event&	x_eve()			{	return m_fog_x;		}

	// �p�����[�^���擾
	TSTR				get_fog_name();
	BSP<C_d3d_album>	get_fog_album();

private:

	// �č\�z
	void	restruct();

	// �p�����[�^
	TSTR				m_fog_name;
	C_elm_int_event		m_fog_x;
	int					m_fog_near;
	int					m_fog_far;

	// ��ƃp�����[�^
	BSP<C_d3d_album>	m_album;
};


