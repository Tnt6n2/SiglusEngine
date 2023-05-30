#pragma		once

#include	"data/tnm_sorter.h"

#include	"element/elm_group_def.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;
class	C_elm_object_list;

// ****************************************************************
// �O���[�v
// ================================================================
//		�{�^���̃O���[�v�ł��B
// ================================================================

// �O���[�v�̌Œ�p�����[�^
struct C_elm_group_def_param
{
	int				group_no;			// �O���[�v�ԍ�
	S_element		target_object;		// �^�[�Q�b�g�ƂȂ�I�u�W�F�N�g
};

// �O���[�v�̒ʏ�p�����[�^
struct C_elm_group_param
{
	S_tnm_sorter	sorter;				// �\�[�^�[
	int				cancel_priority;	// �L�����Z���̗D��x
	int				cancel_se_no;		// �L�����Z�����̌��ʉ��ԍ�
	int				decided_button_no;	// ���肵���{�^���ԍ�

	int				result;				// ���ʁB-1 = �L�����Z���A0 = none�A1 = ����
	int				result_button_no;	// ���肵���{�^���ԍ��iv1 �Ə����d�l���Ⴂ�܂��j

	bool			doing;				// �ғ���
	bool			pause_flag;			// �|�[�Y��
	bool			wait_flag;			// �E�F�C�g�t���O
	bool			cancel_flag;		// �L�����Z���\�t���O
	S_element		target_object;		// �^�[�Q�b�g�ƂȂ�I�u�W�F�N�g
};

// �O���[�v�̍�ƃp�����[�^
struct C_elm_group_work_param
{
	int			last_hit_button_no;
	int			last_pushed_button_no;
	int			hit_button_no;
	int			pushed_button_no;
};

// �O���[�v
class C_elm_group : public C_tnm_element
{
public:

	// �������ƍ쐬����
	void	init(S_element element, CTSTR& name, int group_no, S_element target_object);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(C_elm_group* src);
	void	button_frame_init();

	// �\�[�^�[�̎擾�^�ݒ�
	void			set_sorter(S_tnm_sorter sorter)	{	m_param.sorter = sorter;		}
	S_tnm_sorter	get_sorter()					{	return m_param.sorter;			}
	void			set_order(int value)			{	m_param.sorter.order = value;	}
	int				get_order()						{	return m_param.sorter.order;	}
	void			set_layer(int value)			{	m_param.sorter.layer = value;	}
	int				get_layer()						{	return m_param.sorter.layer;	}

	// �I��������
	void	init_sel();
	void	start()							{	m_param.doing = true;	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;	}
	void	end()							{	m_param.doing = false;	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;	}
	void	set_wait_flag(bool flag)		{	m_param.wait_flag = flag;			}
	void	set_cancel_flag(bool flag)		{	m_param.cancel_flag = flag;			}
	void	set_cancel_se_no(int se_no)		{	m_param.cancel_se_no = se_no;		}
	void	set_cancel_priority(int prior)	{	m_param.cancel_priority = prior;	}
	void	decide(int button_no);
	void	cancel();
	bool	is_doing()					{	return m_param.doing && !m_param.pause_flag;				}

	// �L�����Z������
	void	cancel_check();

	// �I�����֘A�̃p�����[�^���擾
	int		get_hit_button_no()			{	return m_work.hit_button_no;								}
	int		get_last_hit_button_no()	{	return m_work.last_hit_button_no;							}
	int		get_pushed_button_no()		{	return m_work.pushed_button_no;								}
	int		get_last_pushed_button_no()	{	return m_work.last_pushed_button_no;						}
	bool	get_decided()				{	return m_param.decided_button_no >= 0;						}
	int		get_decided_button_no()		{	return m_param.decided_button_no;							}

	int		get_result()				{	return m_param.result;										}
	int		get_result_button_no()		{	return m_param.result_button_no;							}

	bool	get_canceled()				{	return m_param.decided_button_no == TNM_GROUP_CANCELED;		}
	bool	get_doing_flag()			{	return m_param.doing;										}
	bool	get_pause_flag()			{	return m_param.pause_flag;									}
	bool	get_wait_flag()				{	return m_param.wait_flag;									}
	bool	get_cancel_flag()			{	return m_param.cancel_flag;									}
	int		get_cancel_se_no()			{	return m_param.cancel_se_no;								}
	int		get_cancel_priority()		{	return m_param.cancel_priority;								}

	// �I�����֘A�̃p�����[�^��ݒ�
	void	set_hit_button_no(int value)	{	m_work.hit_button_no = value;		}
	void	set_pushed_button_no(int value)	{	m_work.pushed_button_no = value;	}

private:
	C_elm_group_def_param	m_def;		// �Œ�p�����[�^
	C_elm_group_param		m_param;	// �ʏ�p�����[�^
	C_elm_group_work_param	m_work;		// ��ƃp�����[�^
};

// ****************************************************************
// �O���[�v���X�g
// ================================================================
class C_elm_group_list : public C_elm_list<C_elm_group>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target_object);	// ������

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	S_element	m_target_object;
};
