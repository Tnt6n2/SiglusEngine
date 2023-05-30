#pragma		once

#include	"data/tnm_sorter.h"
#include	"data/tnm_element.h"
#include	"element/elm_list.h"
#include	"element/elm_int_event.h"

class	C_tnm_save_stream;

// ****************************************************************
// �V�F�C�N
// ================================================================
struct S_tnm_shake
{
	void	init();
	void	start(int shake_no, bool wait_flag);
	void	copy(const S_tnm_shake* src);
	void	end();
	void	update_time(int past_time);
	void	frame();

	// �p�����[�^
	int		m_no;				// �V�F�C�N�ԍ�
	int		m_cur_time;			// �V�F�C�N�̌��ݎ���

	// ��ƒl
	int		m_cur_shake_x;		// ���݂̃V�F�C�N�w
	int		m_cur_shake_y;		// ���݂̃V�F�C�N�x
};

// ****************************************************************
// �N�G�C�N
// ================================================================

// �萔
const int	TNM_QUAKE_TYPE_NONE = -1;
const int	TNM_QUAKE_TYPE_VEC = 0;		// ��A�E��A�E�A�E���A���A�����A���A����
const int	TNM_QUAKE_TYPE_DIR = 1;		// �㉺�A���E
const int	TNM_QUAKE_TYPE_ZOOM = 2;
const int	TNM_QUAKE_TYPE_ROTATE = 3;

const int	TNM_QUAKE_VEC_U = 0;
const int	TNM_QUAKE_VEC_RU = 1;
const int	TNM_QUAKE_VEC_R = 2;
const int	TNM_QUAKE_VEC_RD = 3;
const int	TNM_QUAKE_VEC_D = 4;
const int	TNM_QUAKE_VEC_LD = 5;
const int	TNM_QUAKE_VEC_L = 6;
const int	TNM_QUAKE_VEC_LU = 7;

// �ʏ�p�����[�^
struct C_elm_quake_param
{
	int		type;				// �N�G�C�N�^�C�v
	int		vec;				// ����
	int		power;				// �N�G�C�N�̋���
	int		cur_time;			// �N�G�C�N���ݎ���
	int		total_time;			// �N�G�C�N���v����
	int		end_flag;			// �N�G�C�N��~�t���O
	int		end_cur_time;		// �N�G�C�N��~���ݎ���
	int		end_total_time;		// �N�G�C�N��~���v����
	int		cnt;				// �N�G�C�N��
	int		end_cnt;			// �N�G�C�N�I����
	int		center_x;			// �N�G�C�N���S�w
	int		center_y;			// �N�G�C�N���S�x
	int		begin_order;		// �J�n�I�[�_�[
	int		end_order;			// �I���I�[�_�[
};

// �ꎞ�p�����[�^
struct C_elm_quake_work_param
{
	int		cur_pos_x;			// ���݂̈ʒu�w
	int		cur_pos_y;			// ���݂̈ʒu�x
	int		cur_scale;			// ���݂̃X�P�[��
	int		cur_rotate;			// ���݂̉�]
};

// �N�G�C�N
class C_elm_quake : public C_tnm_element
{
public:

	// �G�������g��{����
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(const C_elm_quake* p_src);
	void	update_time(int past_time);
	void	frame();
	void	get_rp(S_d3d_render_param* p_rp);
	bool	is_quake_doing()	{	return m_param.type >= 0;	}

	// �X�N���v�g�R�}���h
	void	start_vec(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_skip_flag);
	void	start_dir(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_skip_flag);
	void	start_zoom(int power, int time, int cnt, int end_cnt, int center_x, int center_y, int begin_order, int end_order, bool wait_flag, bool key_flag);
	void	end(int time);
	void	wait(bool key_skip_flag);
	int		check();

	// �p�����[�^�擾
	int		get_type() const		{	return m_param.type;			}
	int		get_begin_order() const	{	return m_param.begin_order;		}
	int		get_end_order() const	{	return m_param.end_order;		}

private:

	// �p�����[�^
	C_elm_quake_param		m_param;
	C_elm_quake_work_param	m_work;
};

// ****************************************************************
// �N�G�C�N���X�g
// ================================================================
class C_elm_quake_list : public C_elm_list<C_elm_quake>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// �G�t�F�N�g�p�����[�^
// ================================================================

// �ʏ�p�����[�^
struct C_elm_effect_param
{
	C_elm_int_event		x;
	C_elm_int_event		y;
	C_elm_int_event		z;
	C_elm_int_event		mono;
	C_elm_int_event		reverse;
	C_elm_int_event		bright;
	C_elm_int_event		dark;
	C_elm_int_event		color_r;
	C_elm_int_event		color_g;
	C_elm_int_event		color_b;
	C_elm_int_event		color_rate;
	C_elm_int_event		color_add_r;
	C_elm_int_event		color_add_g;
	C_elm_int_event		color_add_b;

	S_tnm_sorter		begin_sorter;	// �J�n�\�[�^�[
	S_tnm_sorter		end_sorter;		// �I���\�[�^�[
	int					wipe_copy;		// ���C�v�R�s�[
	int					wipe_erase;		// ���C�v����
};

// ****************************************************************
// �G�t�F�N�g
// ================================================================
class C_elm_effect : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(const C_elm_effect* p_src);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	bool	is_use();	// �g���Ă��邩�𔻒�
	void	get_rp(S_d3d_render_param* p_rp);

	// �p�����[�^��ݒ�
	void	set_pos_x(int value)		{	m_param.x.set_value(value);								}
	void	set_pos_y(int value)		{	m_param.y.set_value(value);								}
	void	set_pos_z(int value)		{	m_param.z.set_value(value);								}
	void	set_mono(int value)			{	m_param.mono.set_value(limit(0, value, 255));			}
	void	set_reverse(int value)		{	m_param.reverse.set_value(limit(0, value, 255));		}
	void	set_bright(int value)		{	m_param.bright.set_value(limit(0, value, 255));			}
	void	set_dark(int value)			{	m_param.dark.set_value(limit(0, value, 255));			}
	void	set_color_r(int value)		{	m_param.color_r.set_value(limit(0, value, 255));		}
	void	set_color_g(int value)		{	m_param.color_g.set_value(limit(0, value, 255));		}
	void	set_color_b(int value)		{	m_param.color_b.set_value(limit(0, value, 255));		}
	void	set_color_rate(int value)	{	m_param.color_rate.set_value(limit(0, value, 255));		}
	void	set_color_add_r(int value)	{	m_param.color_add_r.set_value(limit(0, value, 255));	}
	void	set_color_add_g(int value)	{	m_param.color_add_g.set_value(limit(0, value, 255));	}
	void	set_color_add_b(int value)	{	m_param.color_add_b.set_value(limit(0, value, 255));	}
	void	set_begin_order(int value)	{	m_param.begin_sorter.order = value;						}
	void	set_begin_layer(int value)	{	m_param.begin_sorter.layer = value;						}
	void	set_end_order(int value)	{	m_param.end_sorter.order = value;						}
	void	set_end_layer(int value)	{	m_param.end_sorter.layer = value;						}
	void	set_wipe_copy(int value)	{	m_param.wipe_copy = value ? 1 : 0;						}
	void	set_wipe_erase(int value)	{	m_param.wipe_erase = value ? 1 : 0;						}

	// �p�����[�^���擾
	int		get_pos_x() const				{	return m_param.x.get_value();			}
	int		get_pos_y() const				{	return m_param.y.get_value();			}
	int		get_pos_z() const				{	return m_param.z.get_value();			}
	int		get_mono() const				{	return m_param.mono.get_value();		}
	int		get_reverse() const				{	return m_param.reverse.get_value();		}
	int		get_bright() const				{	return m_param.bright.get_value();		}
	int		get_dark() const				{	return m_param.dark.get_value();		}
	int		get_color_r() const				{	return m_param.color_r.get_value();		}
	int		get_color_g() const				{	return m_param.color_g.get_value();		}
	int		get_color_b() const				{	return m_param.color_b.get_value();		}
	int		get_color_rate() const			{	return m_param.color_rate.get_value();	}
	int		get_color_add_r() const			{	return m_param.color_add_r.get_value();	}
	int		get_color_add_g() const			{	return m_param.color_add_g.get_value();	}
	int		get_color_add_b() const			{	return m_param.color_add_b.get_value();	}
	int		get_begin_order() const			{	return m_param.begin_sorter.order;		}
	int		get_begin_layer() const			{	return m_param.begin_sorter.layer;		}
	int		get_end_order() const			{	return m_param.end_sorter.order;		}
	int		get_end_layer() const			{	return m_param.end_sorter.layer;		}
	int		get_wipe_copy() const			{	return m_param.wipe_copy;				}
	int		get_wipe_erase() const			{	return m_param.wipe_erase;				}

	S_tnm_sorter	get_begin_sorter() const	{	return m_param.begin_sorter;			}
	S_tnm_sorter	get_end_sorter() const		{	return m_param.end_sorter;				}

	// �C�x���g�̌v�Z�l���擾
	int		get_x_total_value() const				{	return m_param.x.get_total_value();				}
	int		get_y_total_value() const				{	return m_param.y.get_total_value();				}
	int		get_z_total_value() const				{	return m_param.z.get_total_value();				}
	int		get_mono_total_value() const			{	return m_param.mono.get_total_value();			}
	int		get_reverse_total_value() const			{	return m_param.reverse.get_total_value();		}
	int		get_bright_total_value() const			{	return m_param.bright.get_total_value();		}
	int		get_dark_total_value() const			{	return m_param.dark.get_total_value();			}
	int		get_color_r_total_value() const			{	return m_param.color_r.get_total_value();		}
	int		get_color_g_total_value() const			{	return m_param.color_g.get_total_value();		}
	int		get_color_b_total_value() const			{	return m_param.color_b.get_total_value();		}
	int		get_color_rate_total_value() const		{	return m_param.color_rate.get_total_value();	}
	int		get_color_add_r_total_value() const		{	return m_param.color_add_r.get_total_value();	}
	int		get_color_add_g_total_value() const		{	return m_param.color_add_g.get_total_value();	}
	int		get_color_add_b_total_value() const		{	return m_param.color_add_b.get_total_value();	}

	// �q�G�������g
	C_elm_int_event&	x()				{	return m_param.x;			}
	C_elm_int_event&	y()				{	return m_param.y;			}
	C_elm_int_event&	z()				{	return m_param.z;			}
	C_elm_int_event&	mono()			{	return m_param.mono;		}
	C_elm_int_event&	reverse()		{	return m_param.reverse;		}
	C_elm_int_event&	bright()		{	return m_param.bright;		}
	C_elm_int_event&	dark()			{	return m_param.dark;		}
	C_elm_int_event&	color_r()		{	return m_param.color_r;		}
	C_elm_int_event&	color_g()		{	return m_param.color_g;		}
	C_elm_int_event&	color_b()		{	return m_param.color_b;		}
	C_elm_int_event&	color_rate()	{	return m_param.color_rate;	}
	C_elm_int_event&	color_add_r()	{	return m_param.color_add_r;	}
	C_elm_int_event&	color_add_g()	{	return m_param.color_add_g;	}
	C_elm_int_event&	color_add_b()	{	return m_param.color_add_b;	}

private:

	// �p�����[�^
	C_elm_effect_param	m_param;
};

// ****************************************************************
// �G�t�F�N�g���X�g
// ================================================================
class C_elm_effect_list : public C_elm_list<C_elm_effect>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

// ****************************************************************
// �X�N���[��
// ================================================================
class C_elm_screen : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	update_time(int past_game_time, int past_real_time);
	void	frame();

	// �q�G�������g
	C_elm_effect_list&	effect_list()	{	return m_effect_list;	}
	S_tnm_shake&		shake()			{	return m_shake;			}
	C_elm_quake_list&	quake_list()	{	return m_quake_list;	}

	// �q�G�������g�iconst�j
	const C_elm_effect_list&	effect_list() const	{	return m_effect_list;	}
	const S_tnm_shake&			shake() const		{	return m_shake;			}
	const C_elm_quake_list&		quake_list() const	{	return m_quake_list;	}

private:

	// �q�G�������g
	C_elm_effect_list	m_effect_list;
	S_tnm_shake			m_shake;
	C_elm_quake_list	m_quake_list;
};






