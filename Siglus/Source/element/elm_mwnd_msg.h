#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_btn.h"
#include	"element/elm_mwnd_moji.h"

class	C_tnm_save_stream;
struct	S_tnm_sorter;
class	C_tnm_sprite;
class	C_elm_group;
class	C_elm_mwnd;
class	C_elm_mwnd_moji;
struct	S_tnm_render_param;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���b�Z�[�W�F�{�^��
// ================================================================
struct C_msg_btn
{
	C_tnm_btn	m_button;
	int			m_btn_no;
	int			m_action_no;
	int			m_se_no;
};

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���b�Z�[�W
// ================================================================
class C_elm_mwnd_msg
{
public:

	// ����������
	void	init(S_element elm_group_list, C_elm_mwnd* p_parent);			// ������
	void	save(C_tnm_save_stream& stream);		// �Z�[�u
	void	load(C_tnm_save_stream& stream);		// ���[�h
	void	copy(C_elm_mwnd_msg* src);				// �R�s�[

	// �f�t�H���g�̃p�����[�^��ݒ�
	void	set_def_param(C_size moji_cnt
		, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color, int ruby_size, int ruby_space
		, int name_moji_size, int name_newline, int name_bracket, C_rect talk_margin);

	// �t���[������
	void	frame_init();							// �t���[������������
	bool	frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow);

	// �e�N�X�`���̃N���A
	void	clear_texture();

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^������
	void	regist_button();

	// ���b�Z�[�W����
	bool	is_empty();						// �󂩂ǂ�������
	void	clear_msg();					// ���b�Z�[�W�̃N���A
	bool	add_msg_check(int check_size, bool new_line_flag);		// ���b�Z�[�W�����邩���`�F�b�N
	void	add_msg(CTSTR& msg, TSTR* ret_over_flow_msg);			// ���b�Z�[�W�̒ǉ�
	void	add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg);	// ���O��ǉ�
	C_size	get_msg_size();					// ���b�Z�[�W�T�C�Y�̎擾
	int		get_msg_height();				// �\���������b�Z�[�W�̍��������߂�
	int		get_moji_cnt();					// ���������擾
	int		get_disp_moji_cnt();			// �\���������������擾
	int		get_hide_moji_cnt();			// �I�[�o�[�t���[�O�̊��ɕ\�����Ă��Ȃ����������擾
	void	disp_one_moji();				// ���̂P�����\��
	void	disp_all_moji();				// �S�Ă̕����̕\��
	void	set_line_head();				// �s�̐擪�ɂ���i�C���f���g�����̔�����s���j
	void	new_line_indent();				// ���s�i�C���f���g����j
	void	new_line_no_indent();			// ���s�i�C���f���g�Ȃ��j
	void	new_line_if_not_line_head();	// ���s�i�s�̐擪�łȂ��ꍇ�̂݁j
	void	set_indent(TCHAR moji);			// �C���f���g��ݒ�
	void	clear_indent();					// �C���f���g���N���A
	void	set_moji_rep_pos(C_point pos);	// �����̈ʒu��ݒ�
	void	set_moji_rep_pos_default();		// �����̈ʒu���f�t�H���g�ɖ߂�
	void	set_moji_size(int size);		// �����̑傫����ݒ�
	void	set_moji_size_default();		// �����̑傫�����f�t�H���g�ɖ߂�
	void	set_moji_color(int color_no);	// �����̐F��ݒ�
	void	set_moji_color_default();		// �����̐F���f�t�H���g�ɖ߂�
	void	clear_chara_moji_color();		// �L�����N�^�����F���N���A
	void	clear_chara_shadow_color();		// �L�����N�^�e�F���N���A
	void	clear_chara_fuchi_color();		// �L�����N�^���F���N���A
	void	set_chara_moji_color(int color_no);		// �L�����N�^�����F��ݒ�
	void	set_chara_shadow_color(int color_no);	// �L�����N�^�e�F��ݒ�
	void	set_chara_fuchi_color(int color_no);	// �L�����N�^���F��ݒ�
	void	start_ruby(CTSTR& ruby);		// ���r���J�n
	void	end_ruby();						// ���r���I��
	void	start_button(int btn_no, int group_no, int action_no, int se_no);		// �{�^�����J�n
	void	end_button();					// �{�^�����I��
	void	slide_msg();					// �X���C�h���b�Z�[�W
	int		get_slide_pixel()				{	return m_def.moji_size + m_def.moji_space.cy;	}
	C_point	get_cur_moji_pos();				// ���݂̕����ʒu���擾
	void	set_cur_moji_pos(C_point pos);	// ���݂̕����ʒu��ݒ�
	bool	check_not_appear_moji();		// �܂��\�����Ă��Ȃ����������݂��邩���肷��
	TSTR	get_debug_msg();				// �f�o�b�O���b�Z�[�W�̎擾

	int		get_last_moji_disp_time()				{	return m_last_moji_disp_time;	}
	void	set_last_moji_disp_time(int time)		{	m_last_moji_disp_time = time;	}
	bool	get_progress_start_flag()				{	return m_progress_start_flag;	}
	void	set_progress_start_flag(bool value)		{	m_progress_start_flag = value;	}
	void	set_cur_msg_type_decided(bool value)	{	m_cur_msg_type_decided = value;	}

	// ���������������^�擾�^�ݒ�
	void	init_moji_cnt();
	void	set_moji_cnt(C_size moji_cnt);
	
	// �{�^������
	void			button_event();
	C_elm_group*	get_group();


private:

	void	init_param();										// �p�����[�^������
	void	init_param_sub();									// �p�����[�^�������r�t�a
	void	add_msg_sub(CTSTR& msg, TSTR* ret_over_flow_msg);	// ���b�Z�[�W�̒ǉ�

	// �����p�����[�^
	struct PARAM
	{
		C_size	moji_cnt;			// ������
		C_point	moji_pos;			// �����ʒu
		C_point	moji_rep_pos;		// �����␳�ʒu
		int		moji_size;			// �����̑傫��
		C_size	moji_space;			// �����Ԃ̃X�y�[�X
		int		moji_color;			// �����F
		int		shadow_color;		// �����e�F
		int		fuchi_color;		// �����e�F
		int		ruby_size;			// ���r�̑傫��
		int		ruby_space;			// ���r�����Ԃ̃X�y�[�X
		int		name_moji_size;		// ���O�̕����T�C�Y
		int		name_newline;		// ���O�̌�ɉ��s����
		int		name_bracket;		// ���O�Ɋ��ʂ�����
		C_rect	talk_margin;		// �Z���t�̃}�[�W���i�m�x�����[�h�j
	};

	// �Œ�p�����[�^
	PARAM					m_def;					// �f�t�H���g�̕����X�e�[�^�X
	C_elm_mwnd*				m_def_parent;			// �e�G�������g
	S_element				m_def_elm_group_list;	// �O���[�v���X�g�̃G�������g

	// �ʏ�p�����[�^
	PARAM					m_cur;					// ���݂̕����X�e�[�^�X
	C_size					m_msg_size;				// ���b�Z�[�W�T�C�Y
	TSTR					m_debug_msg;			// �f�o�b�O���b�Z�[�W
	TSTR					m_ruby;					// ���r

	// �ꎞ�p�����[�^
	int						m_chara_moji_color_no;		// �L�����N�^�����F
	int						m_chara_shadow_color_no;	// �L�����N�^�e�F
	int						m_chara_fuchi_color_no;		// �L�����N�^���F
	int						m_indent_pos;				// �C���f���g�ʒu
	TCHAR					m_indent_moji;				// �C���f���g����
	int						m_indent_cnt;				// �C���f���g�����̏o��������
	int						m_cur_msg_type;				// �Ō�ɕ\���������b�Z�[�W�̃^�C�v�i-1:�N���A��A0:�g�����A1:���O���胁�b�Z�[�W�j
	C_point					m_ruby_start_pos;			// ���r�J�n�ʒu
	int						m_disp_moji_cnt;			// �\������������
	int						m_hide_moji_cnt;			// �B���������i�I�[�o�[�t���[������Ȃǁj
	int						m_last_moji_disp_time;		// �Ō�̕�����\����������
	int						m_btn_no;					// �{�^���ԍ�
	int						m_btn_group_no;				// �{�^���O���[�v�ԍ�
	int						m_btn_action_no;			// �{�^���A�N�V�����ԍ�
	int						m_btn_se_no;				// �{�^�����ʉ��ԍ�

	bool					m_cur_msg_type_decided;		// �Ō�ɕ\���������b�Z�[�W�̃^�C�v�����肵��
	bool					m_line_head_flag;			// �s���t���O
	bool					m_ruby_start_ready;			// ���r�J�n����
	bool					m_button_flag;				// �{�^��������
	bool					m_progress_start_flag;		// ���b�Z�[�W�i�݊J�n�t���O�i���b�Z�[�W�E�B���h�E���J���ă��b�Z�[�W�\�����J�n�����j

	C_tnm_class_array<C_elm_mwnd_moji>	m_moji_list;				// �\�������������X�g

	std::map<int, C_msg_btn>			m_msg_btn;					// ���b�Z�[�W�{�^��
};

