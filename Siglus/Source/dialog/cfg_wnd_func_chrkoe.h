#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�L�����N�^�[����
// ================================================================
class Ccfg_wnd_func_chrkoe : public C_cfg_wnd_func
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

	void		on_mousewheel_character_list(WPARAM wp, LPARAM lp);	// �L�����N�^�[���X�g�̃}�E�X�z�C�[��

private:

	// �L�����N�^�[���X�g
	class Cc_chalst : public Cc_static
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �L�����N�^�[�`�F�b�N�{�b�N�X
	class Cc_character_chk : public Cc_check
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �L�����N�^�[�X���C�_�[
	class Cc_character_sld : public Cc_slider
	{
	public:
		Ccfg_wnd_func_chrkoe*	p_parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_group	grp_chrkoe;
		Cc_chalst	lst_chrkoe;
		Cc_scroll	scr_chrkoe;
		Cc_button	btn_chrkoe_all_on;
		Cc_button	btn_chrkoe_all_off;
		Cc_button	btn_chrkoe_init;
		Cc_button	btn_chrkoe_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
		int			character_cnt;
		int			character_list_scrool_pos;
		int			character_list_width;
		int			character_chkbox_max_width;

		// �L�����N�^�[�\����
		struct CHARACTER_STRUCT
		{
			int					ini_no;
			Cc_character_chk	item_chk;
			Cc_character_sld	item_sld;
		};

		ARRAY<CHARACTER_STRUCT>		character;

	}	work;

	void		setup_dialog();				// �_�C�A���O���\�z
	void		update_dialog();			// �_�C�A���O�X�V
	void		focus_scrollbar();			// �X�N���[���o�[�Ƀt�H�[�J�X�����킹��

	// �L�����N�^�[���X�g
	void		update_character_list();	// �L�����N�^�[���X�g�̍X�V
	void		resize_character_list();	// �L�����N�^�[���X�g�̃��T�C�Y
	bool		on_command_character_list(WPARAM wp, LPARAM lp);	// �L�����N�^�[���X�g�̃R�}���h
	bool		on_h_scroll_character_list(WPARAM wp, LPARAM lp);	// �L�����N�^�[���X�g�̂g�X�N���[��

};
