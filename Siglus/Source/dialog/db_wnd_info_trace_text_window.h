#pragma		once

#include	"db_wnd_info.h"
#include	"db_wnd_info_trace_draw_target.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�g���[�X�̃e�L�X�g�E�B���h�E
// ================================================================
class C_db_wnd_info_trace_text_window
{

public:
	C_db_wnd_info_trace_text_window();										// �R���X�g���N�^
	~C_db_wnd_info_trace_text_window();										// �f�X�g���N�^
	LRESULT		message_proc(UINT msg, WPARAM wp, LPARAM lp);				// ���b�Z�[�W�v���V�[�W���i���E�B���h�E�v���V�[�W���ł͂Ȃ��j
	void		create(HWND parent_h_wnd, C_rect rect, int page_cnt);		// �쐬
	void		destroy();													// �j��
	void		resize_text_window_rect(C_rect rect);						// �e�L�X�g�E�B���h�E�����T�C�Y����
	int			get_scn_no(int page_no);									// �V�[���ԍ����擾����
	void		set_focus_draw_target();									// �`��^�[�Q�b�g���t�H�[�J�X����
	bool		check_focus_draw_target();									// �`��^�[�Q�b�g���t�H�[�J�X����Ă��邩���肷��
	void		alloc_seen_file(int alloc_cnt);								// �V�[���t�@�C���̊m��

	// �`��N���A
	void		draw_clear();

	// �V�[���t�@�C���̕`��
	void		draw_seen_file(int page_no, int scn_no, int draw_top_line_no, int target_line_no, CTSTR& font_name, int font_size, bool is_target);

	// �y�[�W�ύX�̃V�[���t�@�C���̍ĕ`��
	void		change_draw_seen_file(int page_no);

	// �V�[���t�@�C���̍ĕ`��
	void		re_draw_seen_file(int top_line_no);
	void		re_draw_seen_file(CTSTR& font_name, int font_size);
	void		re_draw_seen_file();

private:

	// �������i�t�H���g���j
	struct MOJI_STATE
	{
		TSTR		font_name;				// �t�H���g��
		int			font_size;				// �t�H���g�T�C�Y
		HFONT		h_font;					// �t�H���g�n���h��
		int			x_space;				// ����
		int			y_space;				// �s��

		MOJI_STATE();						// �R���X�g���N�^
		~MOJI_STATE();						// �f�X�g���N�^
		void		init();					// ������
		void		free();					// �J��
		bool		set(CTSTR& name, int size);		// �ݒ�
	};

	// �`����
	struct DRAW_STATE
	{
		int			scn_no;					// �^�[�Q�b�g�̃V�[���ԍ�
		int			target_line_no;			// �^�[�Q�b�g�̍s�ԍ�
		int			focus_line_no;			// �t�H�[�J�X�̍s�ԍ�
		int			in_line_no;				// �h�m�̍s�ԍ�
		int			out_line_no;			// �n�t�s�̍s�ԍ�
		int			draw_top_line_no;		// �`��̐擪�s�ԍ�
		int			draw_end_line_no;		// �`��̍ŏI�s�ԍ�
		int			draw_line_cnt;			// �`��̍s��

		DRAW_STATE();						// �R���X�g���N�^
		void		init();					// ������
	};

	// �V�[�����
	struct SEEN_STATE
	{
		ARRAY<TSTR>	text_data;				// �e�L�X�g�f�[�^
		ARRAY<int>	text_state_now;			// �e�L�X�g���i���݁j
		ARRAY<int>	text_state_last;		// �e�L�X�g���i�O��j
		int			draw_cnt;				// �`���
		DRAW_STATE	draw_state;				// �`����

		SEEN_STATE();						// �R���X�g���N�^
		~SEEN_STATE();						// �f�X�g���N�^
		void		init();					// ������
		void		free();					// �J��
		bool		load(int scn_no);		// �ǂݍ���
		void		copy_text_state();		// �e�L�X�g���̃R�s�[
	};

	// ��ƕϐ�
	struct WORK
	{
		HWND				parent_h_wnd;			// �e�̃n���h��
		MOJI_STATE			moji_state;				// �������i�t�H���g���j
		ARRAY<SEEN_STATE>	seen_state;				// �V�[�����
		DRAW_STATE			current_draw_state;		// ���݂̕`����
		int					current_page_no;		// ���݂̃y�[�W�ԍ�
		bool				no_data_draw_flag;		// �m�[�f�[�^�`��t���O
	}	work;

	// �`��^�[�Q�b�g
	C_db_wnd_info_trace_draw_target	draw_target;	// �`��^�[�Q�b�g

	//����������������������������������������������������������������������������������

	void		shift_seen_file();								// �V�[���t�@�C���̃V�t�g

	void		init_current_draw_state();						// �J�����g�̕`�����������

	C_point		change_draw_target_pos(C_point pos);			// �`��^�[�Q�b�g�̍��W�ɕϊ�����
	C_point		change_draw_target_pos(int x, int y);			// �`��^�[�Q�b�g�̍��W�ɕϊ�����
	C_point		change_draw_target_pos(LPARAM lp);				// �`��^�[�Q�b�g�̍��W�ɕϊ�����

	bool		check_draw_target_on_mouse(C_point pos);		// �`��^�[�Q�b�g�Ƀ}�E�X������Ă��邩���肷��
	bool		check_draw_target_on_mouse(int x, int y);		// �`��^�[�Q�b�g�Ƀ}�E�X������Ă��邩���肷��
	bool		check_draw_target_on_mouse(LPARAM lp);			// �`��^�[�Q�b�g�Ƀ}�E�X������Ă��邩���肷��

	void		set_focus_draw_target(WPARAM wp, LPARAM lp);	// �`��^�[�Q�b�g���t�H�[�J�X����
	void		on_mousewheel(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	void		on_vscroll(WPARAM wp, LPARAM lp);				// �c�X�N���[��

	void		draw_text(ARRAY<SEEN_STATE>::iterator seen_state_itr, bool redisp_flag);		// �e�L�X�g�̕`��

	//����������������������������������������������������������������������������������
	// �c�X�N���[���o�[
	class Citem_v_scroll : public Cc_scroll
	{
	public:
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};
	Citem_v_scroll		v_scroll;

	//����������������������������������������������������������������������������������
	// ���X�N���[���o�[
	class Citem_h_scroll : public Cc_scroll
	{
	public:
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};
	Citem_h_scroll		h_scroll;
};

