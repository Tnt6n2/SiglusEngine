#pragma		once

#include	"data/tnm_array.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_element.h"

#include	"element/elm_mwnd_waku.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_mwnd_name.h"
#include	"element/elm_mwnd_sel.h"

struct	S_tnm_read_flag;
class	C_tnm_save_stream;
class	C_elm_mwnd_list;
class	C_elm_object_list;

// ****************************************************************
// ���b�Z�[�W�E�B���h�E
// ================================================================
class C_elm_mwnd : public C_tnm_element
{
public:
	// �G�������g��{����
	void	init(int mwnd_no, S_element element, CTSTR& name, C_elm_mwnd_list* p_parent, S_element elm_group_list);				// ������
	void	reinit(bool restruct_flag);										// �ď�����
	void	finish();														// �I��
	void	save(C_tnm_save_stream& stream);								// �Z�[�u
	void	load(C_tnm_save_stream& stream);								// ���[�h
	void	copy(C_elm_mwnd* src);											// �R�s�[
	void	set_template(int template_no);									// �e���v���[�g��ݒ�

	// �t���[������
	void	frame_init();											// �t���[������������
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	bool	frame(bool draw_futoku, bool draw_shadow);				// �t���[������
	void	window_anime(bool anime_open, int anime_type, int anime_prog, C_rect *window_rect, C_rect *win_name_rect, C_point *ret_pos_rep, C_point *ret_abs_center, C_size *ret_abs_scale, int *ret_abs_rotate, BYTE *ret_tr_rep);		// �E�B���h�E�A�j������
	void	do_frame_action();										// �t���[���A�N�V��������

	void	msg_frame();											// ���b�Z�[�W��i�߂�
	void	jump_to_msg_wait_by_nowait();							// ���b�Z�[�W�E�F�C�g�܂ŏ�����i�߂�i���O�ς������j
	void	jump_to_msg_wait_by_skip();								// ���b�Z�[�W�E�F�C�g�܂ŏ�����i�߂�i���O�ς������j

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^���C�x���g����
	void	regist_button();
	void	button_event(bool wait_flag);

	// ��������
	void	open();						// �J��
	void	close();					// ����
	void	end_close();				// ������I��
	void	set_auto_proc_ready();		// ���������J�n
	void	clear_auto_proc_ready();	// ���������I��

	// ���b�Z�[�W�u���b�N�t���O
	void	clear_msg_block_start_flag();	// ���b�Z�[�W�u���b�N���J�n�����t���O���N���A
	void	set_msg_block_start_flag();		// ���b�Z�[�W�u���b�N���J�n�����t���O���Z�b�g
	bool	is_msg_block_started();			// ���b�Z�[�W�u���b�N���J�n�����t���O�𔻒�

	// ������
	void	clear_koe();													// ���̃N���A
	void	set_koe(int koe_no, bool play_flag, bool no_auto_mode_flag);	// ���̐ݒ�

	// ���O����
	void	clear_name();				// ���O�̃N���A
	void	set_name(CTSTR& name);		// ���O�̐ݒ�
	TSTR	get_name();					// ���O�̎擾

	// ��O������
	void	clear_face();								// ��O���̃N���A
	void	set_face(int face_no, CTSTR& file_path);	// ��O���̐ݒ�

	// ���b�Z�[�W����
	void	clear_msg();					// ���b�Z�[�W�̃N���A
	bool	add_msg_check(bool new_line_flag);						// ���b�Z�[�W�����邩���`�F�b�N
	void	add_msg(CTSTR& msg, TSTR* ret_over_flow_msg);			// ���b�Z�[�W��ǉ�
	void	add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg);	// ���O�����b�Z�[�W�Ƃ��Ēǉ�
	int		get_moji_cnt();					// ���������擾
	int		get_disp_moji_cnt();			// ���������擾
	void	disp_one_moji();				// �P�̕�����\��
	void	disp_all_moji();				// �S�Ă̕�����\��
	void	set_line_head();				// �s�̐擪�ɂ���i�C���f���g�����̔�����s���j
	void	new_line_indent();				// ���s�i�C���f���g����j
	void	new_line_no_indent();			// ���s�i�C���f���g�Ȃ��j
	void	set_indent();					// �C���f���g��ݒ�
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
	void	start_ruby(CTSTR& str);			// ���r���J�n
	void	end_ruby();						// ���r���I��
	void	start_button(int btn_no, int group_no, int action_no, int se_no);		// �{�^�����J�n
	void	end_button();					// �{�^�����I��
	void	set_last_moji_disp_time();		// �Ō�ɕ\���������Ԃ�ݒ肷��
	TSTR	get_debug_msg();				// �f�o�b�O���b�Z�[�W���擾

	// �}���`���b�Z�[�W
	void	next_msg();						// ���̃��b�Z�[�W�Ƀ^�[�Q�b�g���ڂ�

	// ���b�Z�[�W���x�v�Z����
	void	set_auto_mode_end_moji_cnt()		{ m_auto_mode_end_moji_cnt = m_msg_list[0].get_disp_moji_cnt(); }
	int		get_auto_mode_end_moji_cnt()		{ return m_auto_mode_end_moji_cnt; }
	void	set_progress_start_flag(bool value);

	// �I������
	void	clear_sel();					// �I�������N���A
	void	ready_sel();					// �I����������
	void	add_sel(CTSTR& msg, int item_type, int color);	// �I������ǉ�
	void	set_sel_cancel_enable(bool enable);				// �L�����Z���\�t���O��ݒ�
	void	restruct_sel();					// �I�����𐮂���
	int		get_select_no(C_point point);	// �}�E�X�ʒu���猻�ݑI��ł���I�����ԍ����擾

	// �g�̏���
	void	set_waku(int msg_waku_no, int name_waku_no, bool init_flag);		// �g��ݒ�
	int		get_waku_no()		{ return m_msg_waku.get_template_no(); }	// ���b�Z�[�W�g�ԍ����擾
	int		get_name_waku_no()	{ return m_name_waku.get_template_no(); }	// ���O�g�ԍ����擾
	bool	restruct_msg_waku();				// �g�̍č\�z
	bool	restruct_name_waku();				// ���O�g�̍č\�z

	// �g�̕t������
	void	init_waku_file()			{ m_msg_waku.init_waku_file(); }	// �g�t�@�C����������
	void	set_waku_file(CTSTR& file)	{ m_msg_waku.set_waku_file(file); }	// �g�t�@�C����ݒ�
	TSTR	get_waku_file()				{ return m_msg_waku.get_waku_file(); }	// �g�t�@�C�����擾
	void	init_filter_file()				{ m_msg_waku.init_filter_file(); }	// �t�B���^�[�t�@�C����������
	void	set_filter_file(CTSTR& file)	{ m_msg_waku.set_filter_file(file); }	// �t�B���^�[�t�@�C����ݒ�
	TSTR	get_filter_file()				{ return m_msg_waku.get_filter_file(); }	// �t�B���^�[�t�@�C�����擾
	void	set_key_icon_appear(bool appear);	// �L�[�҂��A�C�R��
	void	set_key_icon_mode(int mode);		// �L�[�҂��A�C�R��

	// ���ʏ���
	bool	is_empty();						// �e�L�X�g���󂩂ǂ����𒲂ׂ�
	void	clear();						// �N���A
	void	clear_for_novel_one_msg();		// �m�x���ɑ΂��čs�P�ʂ̊J�n����
	void	set_clear_ready_flag()				{ m_clear_ready_flag = true; }
	bool	get_clear_ready_flag()				{ return m_clear_ready_flag; }
	bool	check_not_appear_moji();		// �܂��o�����Ă��Ȃ����������݂��邩���肷��

	// �E�B���h�E����
	bool	get_window_appear_flag()			{ return m_window_appear; }
	bool	get_name_appear_flag()				{ return m_name_appear; }
	int		get_koe_no()						{ return m_koe_no; }
	bool	get_koe_play_flag()					{ return m_koe_play_flag; }
	bool	get_koe_no_auto_mode_flag()			{ return m_koe_no_auto_mode_flag; }

	// ���ǃt���O����
	void	add_read_flag(int scn_no, int flag_no);		// ���ǃt���O��ǉ�����
	void	set_read_flag();							// �o�^�������ǃt���O��ݒ肷��
	void	clear_read_flag();							// ���ǃt���O���N���A����

	// �X���C�h���b�Z�[�W
	void	slide_msg();					// �����X���C�h�A�j��
	void	set_slide_msg(bool value, int time)		{ m_slide_msg = value;	m_slide_time = time; }
	bool	is_slide_msg()							{ return m_slide_msg; }

	// �A�j������
	bool	is_open_anime_doing();		// �J���A�j����
	bool	is_close_anime_doing();		// ���A�j����
	bool	is_slide_anime_doing();		// �X���C�h�A�j����
	void	end_open_anime();			// �J���A�j�����I��点��
	void	end_close_anime();			// ���A�j�����I��点��
	void	end_slide_anime();			// �����X���C�h�A�j�����I��点��


	// ���b�Z�[�W�E�B���h�E�ԍ�
	int				get_mwnd_no()					{ return m_mwnd_no; }

	// �\�[�^�[�̎擾�^�ݒ�
	void			set_sorter(S_tnm_sorter value)	{ m_param.sorter = value; }
	S_tnm_sorter	get_sorter()					{ return m_param.sorter; }
	void			set_order(int value)			{ m_param.sorter.order = value; }
	void			set_layer(int value)			{ m_param.sorter.layer = value; }
	void			set_world(int value)			{ m_param.world = value; }
	int				get_order()						{ return m_param.sorter.order; }
	int				get_layer()						{ return m_param.sorter.layer; }
	int				get_world()						{ return m_param.world; }

	// �p�����[�^�̎擾�^�ݒ�
	void	init_window_pos();
	void	init_window_size();
	void	init_open_anime_type();
	void	init_open_anime_time();
	void	init_close_anime_type();
	void	init_close_anime_time();

	void	set_window_pos(C_point pos)			{ m_param.window_pos = pos; }
	void	set_window_size(C_size size)		{ m_param.window_size = size; }
	void	set_open_anime_type(int value)		{ m_param.open_anime_type = value; }
	void	set_open_anime_time(int value)		{ m_param.open_anime_time = value; }
	void	set_close_anime_type(int value)		{ m_param.close_anime_type = value; }
	void	set_close_anime_time(int value)		{ m_param.close_anime_time = value; }

	C_point	get_window_pos()					{ return m_param.window_pos; }
	C_size	get_window_size()					{ return m_param.window_size; }
	int		get_open_anime_type()				{ return m_param.open_anime_type; }
	int		get_open_anime_time()				{ return m_param.open_anime_time; }
	int		get_close_anime_type()				{ return m_param.close_anime_type; }
	int		get_close_anime_time()				{ return m_param.close_anime_time; }
	int		get_default_open_anime_type();
	int		get_default_open_anime_time();
	int		get_default_close_anime_type();
	int		get_default_close_anime_time();

	// ���b�Z�[�W�p�����[�^�̎擾�^�ݒ�
	void	init_window_moji_cnt();
	void	set_window_moji_cnt(C_size moji_cnt);
	C_size	get_window_moji_cnt();

	// ���̑��̃p�����[�^�̎擾�^�ݒ�
	int		get_novel_flag()					{ return m_param.novel_mode; }
	int		get_name_disp_mode()				{ return m_param.name_disp_mode; }	// ���O�̕\�����@�i0:���O�E�B���h�E�ɕ\���A1:���b�Z�[�W�̐擪�ɕ\���A2:�\�����Ȃ��j
	int		get_name_bracket()					{ return m_param.name_bracket; }	// ���O�����ʂł�����i0=���ʂȂ��A1=�y�z�j
	int		get_over_flow_check_size()			{ return m_param.over_flow_check_size; }

	C_elm_object_list&	button_list()			{ return m_msg_waku.button_list(); }
	C_elm_object_list&	face_list()				{ return m_msg_waku.face_list(); }
	C_elm_object_list&	object_list()			{ return m_msg_waku.object_list(); }

private:

	void	restruct_template();			// �e���v���[�g���č\�z
	void	init_work_variable();			// �����ϐ���������

	// �Œ�p�����[�^
	int						m_mwnd_no;
	S_element				m_def_elm_group_list;

	// �ʏ�p�����[�^
	struct	PARAM
	{
		S_tnm_sorter		sorter;					// �\�[�^�[
		int					world;					// ���[���h
		int					novel_mode;				// �m�x�����[�h

		int					extend_type;			// 0:�Œ�T�C�Y�A1:�σT�C�Y
		C_point				window_pos;				// �E�B���h�E�ʒu
		C_size				window_size;			// �E�B���h�E�T�C�Y�i�Œ�T�C�Y�̂݁j
		C_point				msg_pos;				// ���b�Z�[�W�ʒu�i�Œ�T�C�Y�̂݁j
		C_rect				msg_margin;				// ���b�Z�[�W�}�[�W���i�σT�C�Y�̂݁j
		C_size				msg_moji_cnt;			// ���b�Z�[�W�̕�����
		int					name_disp_mode;			// ���O�\�����[�h
		int					name_bracket;			// ���O�����ʂł�����i0=���ʂȂ��A1=�y�z�j

		int					name_extend_type;		// 0:�Œ�T�C�Y�A1:�σT�C�Y
		int					name_window_align;		// �����ʒu
		C_point				name_window_pos;		// �E�B���h�E�ʒu
		C_size				name_window_size;		// �E�B���h�E�T�C�Y�i�Œ�T�C�Y�̂݁j
		C_rect				name_window_rect;		// �E�B���h�E��`
		C_point				name_msg_pos;			// ���b�Z�[�W�ʒu�i�Œ�T�C�Y�̂݁j
		C_point				name_msg_pos_rep;		// ���b�Z�[�W�ʒu�i�Œ�T�C�Y�̂݁j
		C_rect				name_msg_margin;		// ���b�Z�[�W�}�[�W���i�σT�C�Y�̂݁j

		int					over_flow_check_size;	// �I�[�o�[�t���[�Ɣ��f����T�C�Y

		int					face_hide_name;			// ��O����\�������Ƃ����O���B��

		int					open_anime_type;		// ���A�j���̎��
		int					open_anime_time;		// ���A�j���ɂ����鎞��
		int					close_anime_type;		// ���A�j���̎��
		int					close_anime_time;		// ���A�j���ɂ����鎞��
	};
	PARAM				m_param;					// �p�����[�^
	int					m_time;						// ����

	bool				m_msg_block_started_flag;	// ���b�Z�[�W�u���b�N���J�n�����t���O
	bool				m_auto_proc_ready_flag;		// ���������J�n�t���O�i�������������b�Z�[�W�E�B���h�E���J���ă��b�Z�[�W��\���j
	bool				m_window_appear;			// �E�B���h�E�o���t���O�i�������A�J���A�j������ true�A���A�j������ false�j
	bool				m_name_appear;				// ���O�o���t���O
	bool				m_clear_ready_flag;			// �N���A�����t���O
	int					m_auto_mode_end_moji_cnt;	// �����܂Ŋ��ɃI�[�g���[�h�ő҂���
	int					m_target_msg_no;			// �^�[�Q�b�g�̃��b�Z�[�W�ԍ�
	bool				m_slide_msg;				// �X���C�h���b�Z�[�W
	int					m_slide_time;				// �X���C�h�^�C��

	ARRAY<S_tnm_read_flag>	m_read_flag_stock_list;	// �����Ă�����ǃt���O���X�g
	int						m_koe_no;				// ���蓖�Ă�ꂽ���ԍ�
	bool					m_koe_play_flag;		// ���蓖�Ă�ꂽ�����Đ�����t���O
	bool					m_koe_no_auto_mode_flag;// �I�[�g���[�h�̔���ɐ����g��Ȃ��i�Z�[�u���Ȃ��t���O�j

	// �E�B���h�E�A�j��
	int					m_open_anime_type;			// ���s���̃E�B���h�E�J���A�j���^�C�v
	int					m_open_anime_time;			// ���s���̃E�B���h�E�J���A�j������
	int					m_open_anime_start_time;	// ���s���̃E�B���h�E�J���A�j���J�n����
	int					m_close_anime_type;			// ���s���̃E�B���h�E���A�j���^�C�v
	int					m_close_anime_time;			// ���s���̃E�B���h�E���A�j������
	int					m_close_anime_start_time;	// ���s���̃E�B���h�E���A�j���J�n����
	int					m_slide_anime_type;
	int					m_slide_anime_time;
	int					m_slide_anime_start_time;
	int					m_slide_pixel;

	C_tnm_class_array<C_elm_mwnd_msg>	m_msg_list;		// ���b�Z�[�W
	C_elm_mwnd_waku						m_msg_waku;		// ���b�Z�[�W�g
	C_elm_mwnd_name						m_name;			// ���O
	C_elm_mwnd_waku						m_name_waku;	// ���O�g
	C_elm_mwnd_select					m_sel;			// �I����
};

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g
// ================================================================
class C_elm_mwnd_list : public C_elm_list_ex<C_elm_mwnd>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent, S_element elm_group_list);	// ������
	void	finish();

protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end, bool restruct_flag);		// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

private:
	S_element	m_elm_group_list;
};

