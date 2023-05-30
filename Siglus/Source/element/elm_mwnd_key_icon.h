#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_sprite.h"

class	C_tnm_save_stream;
struct	S_tnm_render_param;

// ****************************************************************
// �L�[�҂��A�C�R��
// ================================================================
class C_elm_mwnd_key_icon
{
public:

	void	init();									// ������
	void	reinit();								// �ď�����
	void	release();								// ���
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h
	void	copy(C_elm_mwnd_key_icon* src);			// �R�s�[
	void	set_template(int template_no);			// �e���v���[�g��ݒ�

	// �t���[������������
	void	frame_init();
	// �t���[������
	void	frame(int cur_time, C_rect window_rect, const S_tnm_render_param* p_parent_tdp);

	// �X�v���C�g�c���[���擾
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);

	// ���쏈��
	void	set_appear(bool appear);
	void	set_mode(int mode)		{	m_icon_mode = mode;		}
	void	set_pos(C_point pos)	{	m_icon_pos = pos;		}

private:

	// �e���v���[�g���č\�z
	void	restruct_template();

	// �p�����[�^
	int		m_template_no;

	// ��ƕϐ�
	bool	m_icon_appear;	// �\���t���O
	int		m_icon_mode;	// 0:�L�[�҂��A1:�y�[�W�҂�
	C_point	m_icon_pos;
	bool	m_anime_start_flag;
	int		m_anime_start_time;

	BSP<C_d3d_album>	m_album_key_icon;
	BSP<C_d3d_album>	m_album_page_icon;
	C_tnm_sprite		m_sprite_icon;
};
