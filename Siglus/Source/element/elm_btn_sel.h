#pragma		once

#include	"data/tnm_tree.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_list.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_object.h"

class	C_tnm_save_stream;

// ****************************************************************
// �{�^���I����
// ================================================================
struct S_tnm_btn_select_param
{
	C_point	base_pos;
	C_point	rep_pos;
	int		x_align;
	int		y_align;
	int		max_y_cnt;			// �c�ɒu����ő吔
	int		line_width;			// �P�s�̉���

	int		moji_cnt;			// ������
	C_point	moji_pos;			// �����ʒu
	int		moji_size;			// �����̑傫��
	C_size	moji_space;			// �����Ԃ̃X�y�[�X
	int		moji_x_align;		// �����ʒu�w
	int		moji_y_align;		// �����ʒu�x
	int		moji_color;			// �����F
	int		moji_hit_color;		// �����F
	int		shadow_color;		// �����e�F
	int		fuchi_color;		// �������F

	int		btn_action_no;		// �{�^���A�N�V�����ԍ�

	int		open_anime_type;	// �J���A�j��
	int		open_anime_time;	// �J���A�j������
	int		close_anime_type;	// ���A�j��
	int		close_anime_time;	// ���A�j������
	int		decide_anime_type;	// ����A�j��
	int		decide_anime_time;	// ����A�j������

	int		sync_type;			// �ǂ��܂œ��������邩
};

// ****************************************************************
// �{�^���I����
// ================================================================
class C_elm_btn_select
{
public:

	// �q�G�������g
	C_elm_btn_select_item_list	m_item_list;			// �A�C�e�����X�g

	// ����������
	void	init(S_element element, CTSTR& name);		// ������
	void	reinit();									// �ď�����
	void	finish();									// �I��
	void	save(C_tnm_save_stream& stream);			// �Z�[�u
	void	load(C_tnm_save_stream& stream);			// ���[�h
	void	copy(C_elm_btn_select* src);				// �R�s�[
	void	set_template(int template_no);				// �e���v���[�g�̒l��ݒ�

	// �t���[������
	void	frame_init();											// �t���[������������
	void	update_time(int past_game_time, int past_real_time);	// ���Ԃ��X�V
	void	frame(bool draw_futoku, bool draw_shadow);				// �t���[������

	// �`�揈��
	void	get_sprite_tree(C_tree2<C_tnm_sprite *>& root);	// �X�v���C�g�c���[�ɐݒ�

	// �{�^���C�x���g����
	void	regist_button();					// �{�^����o�^
	void	button_event();						// �{�^���C�x���g����

	// �I��������
	bool	is_empty();								// �󂩂ǂ�������
	void	set_def_pos(C_point pos);				// �I�����̏����ʒu��ݒ�
	void	add_sel(CTSTR& text, int item_type, int color);		// �I�����̒ǉ�
	void	restruct_item();						// �A�C�e�����č\�z
	void	set_cancel_enable(bool enable);			// �L�����Z���\��ݒ肷��
	void	set_capture_flag(bool value);			// �L���v�`���[�t���O��ݒ肷��
	void	set_sel_start_call(CTSTR& scn_name, int z_no);	// �I���J�n���ɌĂ΂��R�}���h��ݒ肷��
	void	do_sel_start_call();							// �I���J�n���ɌĂ΂��R�}���h���R�[������

	void	start_sel(int sync_type);			// �I�����J�n����
	void	close();							// ����
	void	decide(int sel_no);					// ���肷��
	void	end_capture();						// �L���v�`���[���I������
	void	end_open_anime();					// �J���A�j�����I������
	void	end_close_anime();					// ���A�j�����I������
	void	end_decide_anime();					// ����A�j�����I������
	bool	is_capture_doing();					// �L���v�`���[���𔻒�
	bool	is_open_anime_doing();				// �J���A�j�����𔻒�
	bool	is_close_anime_doing();				// ���A�j�����𔻒�
	bool	is_decide_anime_doing();			// ����A�j�����𔻒�
	bool	is_processing();					// ���������ǂ����𔻒�
	void	regist_read_flag(int scn_no, int flag_no);	// ���ǃt���O��o�^����
	void	set_read_flag();							// �o�^�������ǃt���O��ݒ肷��

private:

	void	init_work_variable();			// �����ϐ�������
	void	restruct_template();			// �e���v���[�g���č\�z

	int						m_template_no;				// �e���v���[�g�ԍ�
	S_tnm_btn_select_param	m_def;						// �f�t�H���g�̃p�����[�^
	S_tnm_btn_select_param	m_cur;						// ���݂̃p�����[�^

	// ��ƃp�����[�^
	int						m_open_anime_type;			// ���s���̊J���A�j��
	int						m_open_anime_time;			// ���s���̊J���A�j������
	int						m_open_anime_cur_time;		// ���s���̊J���A�j���J�n����
	int						m_close_anime_type;			// ���s���̕��A�j��
	int						m_close_anime_time;			// ���s���̕��A�j������
	int						m_close_anime_cur_time;		// ���s���̕��A�j���J�n����
	int						m_decide_anime_type;		// ���s���̌���A�j��
	int						m_decide_anime_time;		// ���s���̌���A�j������
	int						m_decide_anime_cur_time;	// ���s���̌���A�j���J�n����
	int						m_decide_sel_no;			// ���肵���I�����ԍ�
	int						m_sync_type;				// �����^�C�v
	bool					m_appear_flag;				// �o���t���O
	bool					m_processing_flag_0;		// �������t���O
	bool					m_processing_flag_1;		// �������t���O�i���A�j���������j
	bool					m_processing_flag_2;		// �������t���O�i����Ɍ���A�j���������j
	bool					m_cancel_enable_flag;		// �L�����Z���\�t���O
	bool					m_capture_flag;				// �L���v�`���[���s���t���O
	TSTR					m_sel_start_call_scn;		// �I���J�n���ɌĂ΂��V�[��
	int						m_sel_start_call_z_no;		// �I���J�n���ɌĂ΂��y�ԍ�
	S_tnm_read_flag			m_read_flag;				// ���ǃt���O

	bool					m_capture_now_flag;			// ����L���v�`���[���s���t���O
};

