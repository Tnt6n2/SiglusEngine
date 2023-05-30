#pragma		once

#include	"elm_mwnd_key_icon.h"
#include	"elm_object.h"

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�g
// ================================================================
class C_elm_mwnd_waku
{
public:

	// ��{����
	void	init(S_element element, CTSTR& name);				// ������
	void	reinit(bool restruct_flag);							// �ď�����
	void	finish();											// �I��
	void	save(C_tnm_save_stream& stream);					// �Z�[�u
	void	load(C_tnm_save_stream& stream);					// ���[�h
	void	copy_all(C_elm_mwnd_waku* src, bool init_flag);		// �R�s�[�i�S�āj
	void	copy_waku(C_elm_mwnd_waku* src, bool init_flag);	// �R�s�[�i�g�j
	void	copy_key_icon(C_elm_mwnd_waku* src);				// �R�s�[�i�L�[�҂��A�C�R���j
	void	copy_face(C_elm_mwnd_waku* src);					// �R�s�[�i��O���j
	void	copy_object(C_elm_mwnd_waku* src);					// �R�s�[�i�I�u�W�F�N�g�j
	void	set_template(int template_no, bool init_flag);		// �e���v���[�g�̒l��ݒ�

	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ�i�߂�
	void	frame_init();											// �t���[������������
	void	frame(int cur_time, C_rect window_rect, const S_tnm_render_param* p_parent_tdp, bool face_appear_flag);		// �t���[������
	void	do_frame_action();										// �t���[���A�N�V��������

	// �X�v���C�g�c���[���\�z
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);

	// �g
	void	init_waku_file();
	void	set_waku_file(CTSTR& file);
	TSTR	get_waku_file()		{	return m_waku_file;	}

	// �t�B���^�[
	void	init_filter_file();
	void	set_filter_file(CTSTR& file);
	TSTR	get_filter_file()		{	return m_filter_file;	}

	// �{�^������
	void	regist_button();								// �{�^����o�^
	void	button_event();									// �{�^���C�x���g����

	// ��O��
	void	clear_face();
	void	set_face(int face_no, CTSTR& file_path);
	bool	is_exist_face();

	// �L�[�҂��A�C�R��
	void	set_key_icon_appear(bool appear)	{	m_key_icon.set_appear(appear);	}
	void	set_key_icon_mode(int mode)			{	m_key_icon.set_mode(mode);		}
	void	set_key_icon_pos(C_point pos)		{	m_key_icon.set_pos(pos);		}

	// �擾�^�ݒ�
	int					get_template_no()				{	return m_template_no;	}
	int					get_extend_type()				{	return m_extend_type;	}
	bool				set_extend_waku(C_rect* window_rect);
	C_elm_object_list&	button_list()					{	return m_btn_list;		}
	C_elm_object_list&	face_list()						{	return m_face_list;		}
	C_elm_object_list&	object_list()					{	return m_object_list;	}

private:

	void	init_work_variable();	// �����ϐ�������
	void	restruct_template();	// �e���v���[�g���č\�z
	void	restruct_waku();		// �g���č\�z
	void	restruct_filter();		// �t�B���^�[���č\�z

	// �E�B���h�E�X�e�[�g
	struct STATE
	{
		C_rect	filter_margin;			// �t�B���^�[�̃}�[�W���i�E�B���h�E����̍��W�j
		C_argb	filter_color;			// �t�B���^�[�̐F
		bool	filter_config_color;	// ���ݒ�̃t�B���^�[�F��K�p����
		bool	filter_config_tr;		// ���ݒ�̃t�B���^�[�����x��K�p����
	};

	int					m_template_no;			// �e���v���[�g�ԍ�
	S_element			m_element;				// �G�������g
	TSTR				m_name;					// ���O
	int					m_extend_type;			// �E�B���h�E�̉σ^�C�v
	TSTR				m_waku_file;			// �g�̃t�@�C����
	TSTR				m_filter_file;			// �t�B���^�[�̃t�@�C����
	STATE				m_def;					// �f�t�H���g�̃E�B���h�E�X�e�[�g
	STATE				m_cur;					// ���݂̃E�B���h�E�X�e�[�g

	C_elm_mwnd_key_icon		m_key_icon;			// �L�[�҂��A�C�R��
	C_elm_object_list		m_btn_list;			// �{�^��
	C_elm_object_list		m_face_list;		// ��O��
	C_elm_object_list		m_object_list;		// �I�u�W�F�N�g

	BSP<C_d3d_album>	m_album_waku;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_waku;
	C_tnm_sprite		m_spr_filter;
};


