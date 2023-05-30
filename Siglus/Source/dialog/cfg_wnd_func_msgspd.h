#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�������x
// ================================================================
class C_cfg_wnd_func_msgspd : public C_cfg_wnd_func
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
		Cc_group	grp_msgspd;
		Cc_slider	sld_msgspd;
		Cc_static	stc_msgspd_sample;
		Cc_static	stc_msgspd_slow;
		Cc_static	stc_msgspd_fast;
		Cc_check	chk_msgspd_nowait;
		Cc_button	btn_msgspd_init;
		Cc_button	btn_msgspd_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
		int			now_time;			// ���݂̕\���^�C��
		TSTR		now_text;			// ���݂̕\������Ă��镶����
	}	work;

	void		setup_dialog();				// �_�C�A���O���\�z
	void		update_dialog();			// �_�C�A���O�X�V
	TSTR		get_sample_text();			// �T���v���e�L�X�g

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
