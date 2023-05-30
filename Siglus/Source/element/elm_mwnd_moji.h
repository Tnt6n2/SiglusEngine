#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_sprite.h"

struct S_tnm_render_param;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F����
// ================================================================
class C_elm_mwnd_moji
{
public:
	C_elm_mwnd_moji();

	// ����������
	void	init();								// ������
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h

	// �t���[������
	void	frame_init();
	bool	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int dark_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow);

	// �e�N�X�`���[�N���A
	void	clear_texture();

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& moji_root, C_tree2<C_tnm_sprite *>& shadow_root);	// �X�v���C�g�c���[�ɐݒ�

	// �����̐ݒ�
	void	set_moji(int moji_type, int moji_code, int moji_size, int moji_color, int shadow_color, int fuchi_color, C_point moji_pos);
	void	set_appear(bool flag);
	TCHAR	get_moji()					{ return m_moji.code; }
	C_point	get_moji_pos()				{ return m_moji.pos; }
	int		get_moji_size()				{ return m_moji.size; }
	void	set_moji_pos(C_point pos)	{ m_moji.pos = pos; }
	bool	get_ruby_flag()				{ return m_ruby_flag; }
	void	set_ruby_flag(bool flag)	{ m_ruby_flag = flag; }

private:

	C_tnm_moji		m_moji;					// ����
	bool			m_appeared_flag;		// �o�������t���O
	bool			m_ruby_flag;			// ���r�t���O

	C_tnm_sprite	m_spr_moji;				// �����X�v���C�g
	C_tnm_sprite	m_spr_shadow;			// �e�X�v���C�g
	C_tnm_sprite	m_spr_fuchi;			// ���X�v���C�g
};

