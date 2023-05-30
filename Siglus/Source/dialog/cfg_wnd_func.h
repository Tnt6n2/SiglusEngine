#pragma		once

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V����
// ================================================================
class C_cfg_wnd_func
{

protected:

	C_dialog*		p_dlg;
	HWND			h_wnd;
	int				func_mod;
	bool			base_mode_flag;
	bool			full_mode_flag;
	bool			modal_mode_flag;
	bool			wide_language_flag;
	bool			wide_koe_wnd_flag;		// �����̐ݒ�E�B���h�E�����C�h������t���O

	int				frame_start_time;
	int				frame_time_past;

public:

	C_cfg_wnd_func()
	{
		p_dlg = NULL;
		h_wnd = NULL;
		full_mode_flag = false;
		modal_mode_flag = false;
		wide_language_flag = false;
	}

	void		on_init_func(C_dialog* p_dlg_, int func_mod_, bool is_base_mode, bool is_full_mode, bool is_modal_mode);		// ������
	void		on_close_func();									// ����
	bool		on_command_func(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify_func(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_mousewheel_func(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	bool		on_h_scroll_func(WPARAM wp, LPARAM lp);				// �g�X�N���[��
	bool		on_v_scroll_func(WPARAM wp, LPARAM lp);				// �u�X�N���[��
	LRESULT		on_ctlcolorstatic_func(WPARAM wp, LPARAM lp);		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
	void		frame_func();										// �t���[��

protected:

	void		refresh_dialog_func(int dialog_width, int dialog_height);

	virtual		void		on_init_funcfunc();										// ������
	virtual		void		on_close_funcfunc();									// ����
	virtual		bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// �R�}���h
	virtual		bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// �ʒm
	virtual		bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
	virtual		bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// �g�X�N���[��
	virtual		bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// �u�X�N���[��
	virtual		LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
	virtual		void		frame_funcfunc();										// �t���[��
};
