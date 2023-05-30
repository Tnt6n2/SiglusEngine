#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_timer.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_object.h"

class C_save_stream;

// ****************************************************************
// �G�������g�F���b�Z�[�W�o�b�N
// ================================================================
class C_elm_msg_back : public C_tnm_element
{
public:
	// �G�������g��{����
	void	init();								// ������
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h

	// �t���[������
	void	frame_init();											// �t���[��������
	void	frame(int time, bool draw_futoku, bool draw_shadow);	// �t���[������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);			// �X�v���C�g�c���[�ɐݒ�

	// �{�^������
	void	regist_button(int min_order);	// �{�^����o�^
	void	button_event();					// �{�^���C�x���g����

	// �X�N���[���̗]�g���������Ă��邩�ǂ����𔻒肷��
	bool	check_aftereffects();

	// ��������
	void	remove();

	// ��������
	void	open();			// �J��
	void	close();		// ����
	bool	is_open()		{	return m_window_appear;		}
	bool	is_exist_msg()	{	return m_history_cnt > 0;	}

	// ���b�Z�[�W����
	bool	add_koe(int koe_no, int chara_no, int scn_no, int line_no);					// ����ǉ�
	bool	add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no);	// ���O��ǉ�
	bool	add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no);				// ���b�Z�[�W��ǉ�
	bool	add_new_line_indent(int scn_no, int line_no);								// ���s��ǉ�
	bool	add_new_line_no_indent(int scn_no, int line_no);							// ���s��ǉ�
	bool	add_pct(CTSTR& file_name, C_point pos);

	void	set_save_id(S_tid save_id);
	void	set_save_id_check_flag(bool save_id_check_flag);
	bool	get_save_id_check_flag();

	void	clear_msg();				// ���b�Z�[�W���N���A
	void	ready_msg();				// ���b�Z�[�W��ǉ�����
	void	next_msg();					// ���̃��b�Z�[�W��
	void	page_up_key_down();
	void	page_dw_key_down();
	void	target_up();
	void	target_down();
	void	target_up_for_wheel();
	void	target_down_for_wheel();

	// �����擾
	int		get_koe_no();
	TSTR	get_original_name();
	TSTR	get_disp_name();
	TSTR	get_debug_msg();
	int		get_scn_no();
	int		get_line_no();

	// �q�X�g���[�\����
	struct S_history
	{
		bool					pct_flag;			// �摜�t���O�i0=������A1=�摜�j
		TSTR					msg_str;			// ���b�Z�[�W�i������j�^�t�@�C����
		TSTR					original_name;		// �I���W�i���̖��O
		TSTR					disp_name;			// �\���p�̖��O
		C_elm_mwnd_msg			msg;				// ���b�Z�[�W
		C_point					pct_pos;			// �摜�̈ʒu
		int						total_pos;			// �S�̂ł̈ʒu
		int						height;				// ����
		C_elm_object			koe_btn;			// ���{�^���^�摜
		C_elm_object			load_btn;			// ���[�h�{�^���^�摜
		C_tnm_pod_array<int>	koe_no_list;		// ���ԍ����X�g
		C_tnm_pod_array<int>	chr_no_list;		// �L�����N�^�ԍ����X�g
		int						koe_play_no;		// ���Đ��ԍ�
		TSTR					debug_msg;			// �f�o�b�O���b�Z�[�W�i������j
		int						scn_no;				// �V�[����
		int						line_no;			// �s�ԍ�
		S_tid					save_id;			// �֘A�t����Z�[�u�f�[�^��ID
		bool					save_id_check_flag;	// �֘A�t����Z�[�u�f�[�^��ID���`�F�b�N����
	};

	// �Z�p���[�^�\����
	struct S_separator
	{
		int						total_pos;			// �S�̂ł̈ʒu
		int						height;				// ����
		C_tnm_sprite			sprite;				// �X�v���C�g
	};

private:

	// �E�B���h�E�X�e�[�g
	struct STATE
	{
		C_rect	window_rect;		// �E�B���h�E�̈ʒu�i�X�N���[������̍��W�j
		C_rect	disp_margin;		// �\����`
		int		msg_pos;			// ���b�Z�[�W�̈ʒu
		C_point	koe_btn_pos;		// ���{�^���̈ʒu
		C_point	load_btn_pos;		// ���[�h�{�^���̈ʒu
	};

	STATE				m_def;				// �f�t�H���g�̃E�B���h�E�X�e�[�g
	STATE				m_cur;				// ���݂̃E�B���h�E�X�e�[�g
	int					m_history_cnt_max;	// �����̍ő吔

	// �p�����[�^
	ARRAY<S_history>	m_history_list;				// ����
	int					m_history_cnt;				// ����
	int					m_history_start_pos;		// �����J�n�ʒu
	int					m_history_last_pos;			// �����I���ʒu
	int					m_history_insert_pos;		// ����������ʒu
	int					m_new_msg_flag;				// �V�������b�Z�[�W�t���O

	// �Z�p���[�^
	ARRAY<S_separator>	m_separator_list;			// �Z�p���[�^

	// ��ƃp�����[�^
	bool				m_window_appear;			// �E�B���h�E�o���t���O
	int					m_msg_total_height;			// ���b�Z�[�W�S�̂̍���
	int					m_target_no;				// �^�[�Q�b�g�ԍ�
	int					m_mouse_target_no;			// �}�E�X���������Ă���^�[�Q�b�g�ԍ�

	int					m_scroll_pos;				// �X�N���[���ʒu
	int					m_slider_pos;				// �X���C�_�[�ʒu
	bool				m_slider_operating;			// �X���C�_�[�𑀍쒆
	int					m_slider_start_slider_pos;	// �X���C�_�[����J�n���̃X���C�_�[�ʒu
	int					m_slider_start_mouse_pos;	// �X���C�_�[����J�n���̃}�E�X�ʒu

	bool				m_drag_ready_flag;			// �h���b�O�����t���O
	bool				m_drag_flag;				// �h���b�O��
	C_point				m_drag_start_mouse_pos;		// �h���b�O�J�n���̃}�E�X�ʒu
	int					m_drag_start_scroll_pos;	// �h���b�O�J�n���̃X�N���[���ʒu

	bool				m_drag_aftereffects_flag;				// �h���b�O�̗]�g�F�]�g�t���O
	DWORD				m_drag_aftereffects_start_time;			// �h���b�O�̗]�g�F�v���J�n�̎���
	int					m_drag_aftereffects_mouse_pos;			// �h���b�O�̗]�g�F�v���J�n�̃}�E�X�ʒu
	int					m_drag_aftereffects_distance;			// �h���b�O�̗]�g�F�]�g�̋���
	int					m_drag_aftereffects_start_scroll_pos;	// �h���b�O�̗]�g�F�]�g�̊J�n�̃X�N���[���ʒu
	int					m_drag_aftereffects_end_scroll_pos;		// �h���b�O�̗]�g�F�]�g�̏I���̃X�N���[���ʒu

	bool				m_page_up_key;
	bool				m_page_dw_key;
	int					m_target_up_start_time;		// PageUp �̊J�n����
	int					m_target_dw_start_time;		// PageDown �̊J�n����

	BSP<C_d3d_album>	m_album_waku;
	C_tnm_sprite		m_spr_waku;
	BSP<C_d3d_album>	m_album_filter;
	C_tnm_sprite		m_spr_filter;

	BSP<C_d3d_album>	m_album_separator;
	BSP<C_d3d_album>	m_album_separator_top;
	BSP<C_d3d_album>	m_album_separator_bottom;

	C_elm_object		m_slider;
	C_elm_object		m_close_btn;
	C_elm_object		m_msg_up_btn;
	C_elm_object		m_msg_down_btn;
	C_elm_object		m_ex_btn_1;
	C_elm_object		m_ex_btn_2;
	C_elm_object		m_ex_btn_3;
	C_elm_object		m_ex_btn_4;

	void	restruct_template();				// �e���v���[�g���č\�z
	void	init_work_variable();				// �����ϐ�������

	void	create_msg();
	void	target_up_sub(int cnt);
	void	target_down_sub(int cnt);
	void	update_pos_from_target();
	void	update_pos_from_slider();
	void	update_pos_from_scroll();
	void	calc_scroll_pos_from_target();
	void	calc_scroll_pos_from_slider();
	void	calc_slider_pos_from_scroll();
	void	calc_target_no_from_scroll();
	void	msg_click_event();
	void	load_call(S_tid save_id);
	void	ex_btn_call(int btn_no);
};




