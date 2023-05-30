#pragma		once

#include	"cfg_wnd_func.h"

struct S_tnm_save_header;

// ****************************************************************
// �V�X�e���E�B���h�E�F�t�@���N�V�����F�Z�[�u���[�h
// ================================================================
class C_sys_wnd_func_saveload : public C_cfg_wnd_func
{
public:
	void		on_init_funcfunc();										// ������
	void		on_close_funcfunc();									// ����
	bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// �g�X�N���[��
	bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// �u�X�N���[��
	LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
	void		frame_funcfunc();										// �t���[��

	void		init_state();							// ������Ԃɖ߂�

	int			select_return_value;					// ���[�_���ł̑I���f�[�^�ԍ��i-1 = �L�����Z���j

private:

	// �Z�[�u���[�h�^�u
	class Cc_tab_saveload : public Cc_tab
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �Z�[�u���[�h���X�g
	class Cc_lst_saveload : public Cc_listview
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �Z�[�u���[�h�֘A�̃`�F�b�N�{�b�N�X
	class Cc_chk_saveload : public Cc_check
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �Z�[�u���[�h�֘A�̃{�^��
	class Cc_btn_saveload : public Cc_button
	{
	public:
		C_sys_wnd_func_saveload *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_tab_saveload		tab_saveload;
		Cc_lst_saveload		lst_saveload;
		Cc_chk_saveload		chk_warning;
		Cc_chk_saveload		chk_dblclick;
		Cc_btn_saveload		btn_new;
		Cc_btn_saveload		btn_saveload;
		Cc_btn_saveload		btn_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
		int		page_cnt;
		int		data_cnt_par_page;
		int		data_index_keta_mode;

		int		now_page_no;
		int		data_top_no;
		int		data_end_no;
		ARRAY<S_tnm_save_header>	data_header_list;
		ARRAY<bool>					data_header_exist;

		int		new_save_data_no;
		int		new_tab_page_no;
	}	work;

	void		update_dialog();						// �_�C�A���O�X�V
	void		create_data_list();						// �f�[�^���X�g�쐬
	void		set_save_bnt_enable();					// �Z�[�u�{�^���̋֎~�E����
	TSTR		create_data_list_str(int data_no, S_tnm_save_header *p_header);	// �f�[�^���X�g�̕�����쐬

	void		execute_saveload();						// �Z�[�u���[�h�̎��s
	void		execute_saveload_func(int data_no);		// �Z�[�u���[�h�̎��s

	void		set_focus_list(bool up_down_flag);		// ���X�g���t�H�[�J�X
	void		set_focus_tab(bool left_right_flag);	// �^�u���t�H�[�J�X

	void		escape_key_func();						// �G�X�P�[�v�L�[
	void		cursor_control(WPARAM wp);				// �J�[�\���R���g���[��
	bool		on_getdlgcode(WPARAM wp, LPARAM lp, LRESULT *ret);	// GETDLGCODE

};
