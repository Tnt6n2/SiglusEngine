#pragma		once

#include	"dialog/cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�t�H���g
// ================================================================
class C_cfg_wnd_func_font : public C_cfg_wnd_func
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

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_group	grp_font;
		Cc_listbox	lst_font;
		Cc_check	chk_proportional;
		Cc_static	stc_proportional;
		Cc_check	chk_font_futoku;
		Cc_radio	rad_font_shadow_none;
		Cc_radio	rad_font_shadow_shadow;
		Cc_radio	rad_font_shadow_fuchidoru;
		Cc_radio	rad_font_shadow_fuchidoru_shadow;
		Cc_static	stc_font_sample;
		Cc_button	btn_font_init;
		Cc_button	btn_font_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
		bool				frame_first_flag;	// ����t���[���t���O
		C_font_name_list	fnlst;		// �t�H���g�����X�g
		TSTR				cur_font_name;		// ���݂̃t�H���g��

		TSTR		now_text;			// ���݂̕\������Ă��镶����
	}	work;

	void		setup_dialog();				// �_�C�A���O���\�z
	void		update_dialog();			// �_�C�A���O�X�V
	void		set_radio_shadow();			// �e�̃��W�I�{�^����ݒ�
	int			get_radio_shadow();			// �e�̃��W�I�{�^�����擾
	void		create_font_list();			// �t�H���g���X�g���쐬
	void		update_font();				// �t�H���g���X�V
	void		update_font_list();			// �t�H���g���X�g���X�V
	void		set_font_list();			// ���X�g�����݂̃t�H���g�ɐݒ�
	void		get_font_list();			// ���X�g����t�H���g�擾

	// ��Ɨp�t�H���g
	void		init_work_font();						// ������
	void		free_work_font();						// �J��
	void		load_work_font(CTSTR& font_name);		// �ǂݍ���

	// ��Ɨp�t�H���g�ϐ�
	struct WORK_FONT
	{
		HFONT		original_font;		// ������Ԃ̃t�H���g
		HFONT		now_font;			// ���݂̃t�H���g
		TSTR		now_font_name;		// ���݂̃t�H���g�̖��O
	}	work_font;

};
