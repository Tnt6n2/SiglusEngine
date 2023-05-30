#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_btn.h"

#include	"element/elm_mwnd_moji.h"

class	C_tnm_save_stream;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�I�����F�A�C�e��
// ================================================================
class C_elm_mwnd_select_item
{
public:

	// ����������
	void	init();								// ������
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h
	void	copy(C_elm_mwnd_select_item* src);	// �R�s�[

	// �t���[������
	void	frame_init();									// �t���[������������
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int dark_rep, bool draw_futoku, bool draw_shadow);	// �t���[������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^���C�x���g����
	void	regist_button();					// �{�^����o�^
	bool	button_event(bool wait_flag);		// �{�^���C�x���g����
	bool	is_active_hit();					// �A�N�e�B�u�ȃq�b�g�𔻒�

	// �A�C�e������
	void	set_msg(CTSTR& msg, int item_type, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color);	// ���b�Z�[�W��ݒ�
	void	set_hit_rect(C_rect hit_rect);	// �������`��ݒ�
	void	set_pos(C_point pos);			// �\�����W��ݒ�
	void	set_disp();						// �I������\������
	TSTR	get_msg();						// ���b�Z�[�W���擾����
	C_size	get_msg_size();					// ���b�Z�[�W�T�C�Y���擾����
	int		get_item_type()	{ return m_item_type; }

private:

	int					m_item_type;
	C_point				m_pos;				// �\�����W
	TSTR				m_msg;				// ���b�Z�[�W
	C_size				m_msg_size;			// ���b�Z�[�W�T�C�Y
	C_rect				m_hit_rect_temp;	// �������`�i���j
	C_tnm_btn			m_button;			// �{�^��

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// �������X�g
};

