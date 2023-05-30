#pragma		once

#include	"elm_mwnd_sel_item.h"

class	C_tnm_save_stream;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�I����
// ================================================================
class C_elm_mwnd_select
{
public:

	// ����������
	void	init();								// ������
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h
	void	copy(C_elm_mwnd_select* src);		// �R�s�[
	void	set_template(int template_no);		// �e���v���[�g�̒l��ݒ�

	// �t���[������
	void	frame_init();									// �t���[������������
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow);	// �t���[������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^���C�x���g����
	void	regist_button();				// �{�^����o�^
	void	button_event(bool wait_flag);	// �{�^���C�x���g����

	// �I��������
	bool	is_empty();						// �󂩂ǂ�������
	void	clear_sel();					// �I�����̃N���A
	void	set_def_pos(C_point pos);		// �I�����̏����ʒu��ݒ�
	void	add_sel(CTSTR& text, int item_type, int color);		// �I�����̒ǉ�
	void	set_cancel_enable(bool enable);	// �L�����Z���\�t���O��ݒ�
	void	restruct_item(int extend_type);	// �A�C�e�����č\�z
	void	set_disp();						// �\������
	TSTR	get_msg();						// ���b�Z�[�W�̎擾
	C_size	get_msg_size();					// ���b�Z�[�W�T�C�Y�̎擾
	bool	check_not_appear_moji();		// �܂��o�����Ă��Ȃ����������݂��邩���肷��

private:

	void	init_work_variable();			// �����ϐ�������
	void	restruct_template();			// �e���v���[�g���č\�z

	// �����X�e�[�^�X
	struct STATE
	{
		C_size	moji_cnt;			// ������
		C_point	moji_pos;			// �����ʒu
		int		moji_size;			// �����̑傫��
		C_size	moji_space;			// �����Ԃ̃X�y�[�X
		int		moji_color;			// �����F
		int		shadow_color;		// �����e�F
		int		fuchi_color;		// �������F
	};

	int				m_template_no;			// �e���v���[�g�ԍ�
	STATE			m_def;					// �f�t�H���g�̕����X�e�[�^�X
	STATE			m_cur;					// ���݂̕����X�e�[�^�X
	TSTR			m_msg;					// ���b�Z�[�W
	C_size			m_msg_size;				// ���b�Z�[�W�T�C�Y
	int				m_disp_item_cnt;		// �\�������A�C�e����
	bool			m_cancel_enable_flag;	// �L�����Z���\�t���O

	C_tnm_class_array<C_elm_mwnd_select_item>	m_item_list;			// �A�C�e�����X�g
};

