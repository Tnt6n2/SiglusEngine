#pragma		once

#include	"db_wnd_info.h"
#include	"db_wnd_info_trace_text_window.h"


// ****************************************************************
// �f�o�b�O���E�B���h�E�F�g���[�X
// ================================================================
class C_db_wnd_info_trace : public C_db_wnd_info
{

public:

	void		init();						// ������
	void		load_state();				// �X�e�[�^�X�ǂݍ���
	void		save_state();				// �X�e�[�^�X�ۑ�
	void		frame();					// �t���[��

private:

	// �e�L�X�g�^�u
	class Ctab_text : public Cc_tab
	{
	public:
		C_db_wnd_info_trace *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_check	chk_break;
		Cc_check	chk_click;
		Cc_check	chk_trace;
		Cc_combobox	cmb_font_size;
		Cc_button	btn_next;
		Ctab_text	tab_text;
	}	ci;

	// �g���[�X���
	struct TRACE_STATE
	{
		int		scn_no;
		int		line_no;
		bool	is_breaking;
	};

	// ��ƕϐ�
	struct WORK
	{
		TRACE_STATE		cd;					// ���݂̃g���[�X���
		TRACE_STATE		ld;					// �O��̃g���[�X���
		int				page_cnt;			// �y�[�W���i�^�u���j
		bool			next_click_state;	// ��ʂ��N���b�N�������
		bool			next_key_state;		// ��ʂ��N���b�N�������
	}	work;

	// �e�L�X�g�E�B���h�E
	BSP<C_db_wnd_info_trace_text_window>		p_text_window;


	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	void		set_break(bool is_breaking);					// �u���[�N��Ԃ�ݒ�

	void		next_step();									// ���̃X�e�b�v�֐i��

	void		draw_scn_for_trace(int scn_no, int line_no);	// �e�L�X�g�`��i�g���[�X���j
	void		draw_scn_for_change_select_tab(int tab_no);		// �e�L�X�g�`��i�^�u�ύX���j
	void		draw_text_for_change_font_size(int font_size);	// �e�L�X�g�`��i�����T�C�Y�ύX���j

	void		set_focus_draw_target();						// �`��^�[�Q�b�g���t�H�[�J�X����
	bool		check_focus_draw_target();						// �`��^�[�Q�b�g���t�H�[�J�X����Ă��邩���肷��

	void		set_tab_str();									// �^�u�̕�����ύX
};

