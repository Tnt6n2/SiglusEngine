#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_sprite.h"
#include	"element/elm_object.h"

class	C_tnm_save_stream;
class	C_elm_mwnd_moji;

// ****************************************************************
// �{�^���I�����A�C�e���F�p�����[�^
// ================================================================
struct S_tnm_btn_select_item_param
{
	int		item_type;
	int		moji_color;
};

// ****************************************************************
// �{�^���I�����A�C�e��
// ================================================================
class C_elm_btn_select_item : public C_tnm_element
{
public:

	// �q�G�������g
	C_elm_object_list		m_object_list;		// �I�u�W�F�N�g

	// ����������
	void	init(S_element element, CTSTR& name);	// ������
	void	reinit(bool restruct_flag);				// �ď�����
	void	finish();								// �I��
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h
	void	copy(C_elm_btn_select_item* src);		// �R�s�[
	void	set_template(int template_no);			// �e���v���[�g�̒l��ݒ�

	// �t���[������
	void	frame_init();											// �t���[������������
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	void	frame(C_point window_pos, const S_tnm_render_param* parent_trp, int hit_color, int action_no, bool draw_futoku, bool draw_shadow);	// �t���[������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^���C�x���g����
	void	regist_button();					// �{�^����o�^
	bool	button_event();						// �{�^���C�x���g����

	// �A�C�e������
	void	set_msg(CTSTR& msg, int item_type, int moji_size, C_point moji_pos, C_size moji_space, int x_align, int y_align, int moji_color, int shadow_color, int fuchi_color);	// ���b�Z�[�W��ݒ�
	TSTR	get_msg()				{ return m_msg; }				// ���b�Z�[�W���擾
	void	set_pos(C_point pos)	{ m_pos = pos; }				// �\�����W��ݒ�
	C_point	get_pos()				{ return m_pos; }				// �\�����W���擾
	void	set_disp();												// �I������\������
	C_size	get_item_size();										// �A�C�e���T�C�Y���擾����
	int		get_item_type()			{ return m_param.item_type; }	// �A�C�e���̏�Ԃ��擾����

private:

	void	init_work_variable();			// �����ϐ�������
	void	restruct_template();			// �e���v���[�g���č\�z

	int							m_template_no;
	TSTR						m_base_file;
	TSTR						m_filter_file;
	TSTR						m_msg;				// ���b�Z�[�W
	S_tnm_btn_select_item_param	m_param;			// �p�����[�^
	C_point						m_pos;				// �\�����W
	C_size						m_item_size;		// �A�C�e���T�C�Y
	C_tnm_btn					m_button;			// �{�^��

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// �������X�g

	BSP<C_d3d_album>	m_album_base;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_base;
	C_tnm_sprite		m_spr_filter;
};

// ****************************************************************
// �{�^���I�����A�C�e�����X�g
// ================================================================
class C_elm_btn_select_item_list : public C_elm_list_ex<C_elm_btn_select_item>
{
public:
	void	finish();												// �I��

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end, bool restruct_flag);		// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h
};

