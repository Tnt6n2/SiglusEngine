#pragma		once

#include	"data/tnm_def.h"
#include	"data/tnm_guid.h"
#include	"data/tnm_sorter.h"

// ****************************************************************
// �{�^���^�{�^���}�l�[�W��
// ================================================================
//		�ꊇ���ă{�^���𔻒肷�邽�߂̃V�X�e���ł��B
//		�e�G�������g�́A�{�^���iC_tnm_btn�j�������Ă��܂��B
// ================================================================


// ****************************************************************
// �{�^��
// ================================================================
//		�����蔻����s���I�u�W�F�N�g�Ɏ������܂��B
// ================================================================
class C_tnm_btn
{
	friend	class	C_tnm_btn_mng;

public:
	C_tnm_btn();
	~C_tnm_btn();

	void	init();											// ������
	void	frame_init();									// ������
	void	set_rect(C_rect rect);							// ��`��ݒ�
	void	set_rect_2(bool use, C_rect rect);				// ��`���̂Q��ݒ�
	void	set_center(C_point center);						// ���S��ݒ�
	void	set_rotate(int rotate);							// ��]��ݒ�
	void	set_sorter(S_tnm_sorter sorter);				// �\�[�^�[��ݒ�
	void	set_push_keep(bool push_keep);					// �v�b�V���L�[�v��ݒ�
	void	set_param(C_rect rect, S_tnm_sorter sorter);	// �p�����[�^��ݒ�
	void	set_texture(BSP<C_d3d_texture> texture);		// �e�N�X�`����ݒ�
	bool	hit_test(C_point pos);							// �q�b�g�e�X�g

	bool	is_decided()		{ return m_is_decided; }
	void	create_guid()		{ m_guid.create_guid(); }
	int		get_guid()			{ return m_guid.get_guid(); }

	const	C_rect& rect() const			{ return m_rect; }
	const	S_tnm_sorter& sorter() const	{ return m_sorter; }

private:
	C_tnm_guid		m_guid;			// GUID
	C_rect			m_rect;			// �������`
	C_rect			m_rect_2;		// �������`���̂Q
	C_point			m_center;		// ��`�̉�]
	int				m_rotate;		// ��`�̉�]
	S_tnm_sorter	m_sorter;		// �\�[�^�[
	bool			m_rect_2_use;	// �������`���̂Q���g��
	bool			m_push_keep;	// �����ꂽ���L�[�v
	bool			m_is_decided;	// ���肳�ꂽ

	BSP<C_d3d_texture>	m_texture;		// �e�N�X�`��
};

// ****************************************************************
// �{�^���}�l�[�W��
// ================================================================
class C_tnm_btn_mng
{
public:
	void	init();
	bool	frame_init();
	bool	hit_test_proc();

	// �{�^����o�^����
	void	regist_button(C_tnm_btn* btn);
	int		get_btn_cnt()	{	return (int)m_btn_list.size();	}

	// �A�N�e�B�u�ȃ{�^���̃p�����[�^���擾
	int				get_active_btn_guid()	{	return m_active_btn_guid;		}
	S_tnm_sorter	get_active_btn_sorter()	{	return m_active_btn_sorter;		}
	int				get_active_btn_state()	{	return m_active_btn_state;		}

	// �{�^���̃X�e�[�g�����߂�
	bool	is_active(C_tnm_btn* btn);
	int		get_hit_state(C_tnm_btn* btn);

	// ���ݓ������Ă��邩�ǂ����Ȃǂ𔻒�
	bool	is_hit(C_tnm_btn* btn)			{	return m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid && m_active_btn_state == TNM_BTN_STATE_HIT;	}
	bool	is_pushed(C_tnm_btn* btn)		{	return m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid && m_active_btn_state == TNM_BTN_STATE_PUSH;	}
	bool	is_decided(C_tnm_btn* btn)		{	return btn->is_decided();	}

	// �{�^�������񓖂��������ǂ����Ȃǂ𔻒�
	bool	is_hit_this_frame(C_tnm_btn* btn)		{	return m_hit_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;			}	// �{�^���F��������
	bool	is_pushed_this_frame(C_tnm_btn* btn)	{	return m_pushed_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;		}	// �{�^���F�����ꂽ
	bool	is_decided_this_frame(C_tnm_btn* btn)	{	return m_decided_this_frame && m_active_btn_guid >= 0 && btn->m_guid.get_guid() == m_active_btn_guid;		}	// �{�^���F���肵��
	bool	is_hit_this_frame()						{	return m_hit_this_frame;				}
	bool	is_pushed_this_frame()					{	return m_pushed_this_frame;				}
	bool	is_decided_this_frame()					{	return m_decided_this_frame;			}

private:
	int				m_active_btn_guid;			// �A�N�e�B�u�ȃ{�^����GUID
	S_tnm_sorter	m_active_btn_sorter;		// �A�N�e�B�u�ȃ{�^���̃\�[�^�[
	int				m_active_btn_state;			// �A�N�e�B�u�ȃ{�^���̃X�e�[�g

	bool			m_last_mouse_push_flag;		// �O��̃}�E�X�������Ă���t���O
	int				m_last_active_group_guid;	// �O��̃A�N�e�B�u�ȃO���[�v��GUID
	int				m_last_active_btn_guid;		// �O��̃A�N�e�B�u�ȃ{�^����GUID
	S_tnm_sorter	m_last_active_btn_sorter;	// �O��̃A�N�e�B�u�ȃ{�^���̃\�[�^�[
	int				m_last_active_btn_state;	// �O��̃A�N�e�B�u�ȃ{�^���̃X�e�[�g

	bool			m_hit_this_frame;			// ���񓖂�����
	bool			m_pushed_this_frame;		// ���񉟂��ꂽ
	bool			m_decided_this_frame;		// ���񌈒肳�ꂽ

	ARRAY<C_tnm_btn *>		m_btn_list;
};

