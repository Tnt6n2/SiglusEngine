#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sprite.h"

class	C_elm_mwnd_moji;
struct	S_tnm_render_param;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���O
// ================================================================
class C_elm_mwnd_name
{
public:

	// ����������
	void	init();									// ������
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h
	void	copy(C_elm_mwnd_name* src);				// �R�s�[
	void	set_template(int template_no);			// �e���v���[�g�̒l��ݒ�
	void	restruct_template();					// �e���v���[�g�̍č\�z

	// �t���[������
	void	frame_init();							// �t���[������������
	void	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow);

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);		// �X�v���C�g�c���[�ɐݒ�

	// ���O����
	bool	is_empty();					// �󂩂ǂ�������
	void	clear_name();				// ���O�̃N���A
	void	set_name(CTSTR& name);		// ���O�̐ݒ�
	TSTR	get_name();					// ���O�̎擾
	void	clear_chara_moji_color();	// �L�����N�^�����F��ݒ�
	void	clear_chara_shadow_color();	// �L�����N�^�e�F��ݒ�
	void	clear_chara_fuchi_color();	// �L�����N�^���F��ݒ�
	void	set_chara_moji_color(int color_no);		// �L�����N�^�����F��ݒ�
	void	set_chara_shadow_color(int color_no);	// �L�����N�^�e�F��ݒ�
	void	set_chara_fuchi_color(int color_no);	// �L�����N�^���F��ݒ�
	C_rect	get_msg_rect();				// ���b�Z�[�W��`�̎擾

private:

	void	init_work_variable();				// �����ϐ�������
	void	init_work_variable_sub();			// �����ϐ��������r�t�a

	// �v���C�x�[�g�����ϐ�
	struct STATUS
	{
		C_point	moji_pos;			// �����ʒu
		int		moji_size;			// �����̑傫��
		C_size	moji_space;			// �����Ԃ̃X�y�[�X
		int		moji_cnt;			// ������
		int		moji_align;			// ���������ʒu
		int		moji_color_no;		// �����F
		int		shadow_color_no;	// �e�F
		int		fuchi_color_no;		// ���F
	};

	int						m_template_no;				// �e���v���[�g�ԍ�
	STATUS					m_def;						// �f�t�H���g�̕����X�e�[�^�X
	STATUS					m_cur;						// ���݂̕����X�e�[�^�X
	int						m_chara_moji_color_no;		// �L�����N�^�����F
	int						m_chara_shadow_color_no;	// �L�����N�^�e�F
	int						m_chara_fuchi_color_no;		// �L�����N�^���F
	TSTR					m_name;						// ���O
	C_rect					m_msg_rect;					// ���b�Z�[�W��`

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;	// �\�������������X�g

	C_tnm_sprite			m_spr_moji_root;
	C_tnm_sprite			m_spr_shadow_root;
};

