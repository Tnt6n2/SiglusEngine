#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�E�B���h�E�w�i�F
// ================================================================
class Ccfg_wnd_func_mwndbk : public C_cfg_wnd_func
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
		Cc_group	grp_mwndbk;
		Cc_static	stc_mwndbk_r;
		Cc_slider	sld_mwndbk_r;
		Cc_static	stc_mwndbk_g;
		Cc_slider	sld_mwndbk_g;
		Cc_static	stc_mwndbk_b;
		Cc_slider	sld_mwndbk_b;
		Cc_static	stc_mwndbk_tr;
		Cc_slider	sld_mwndbk_tr;
		Cc_button	btn_mwndbk_init;
		Cc_button	btn_mwndbk_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		update_dialog();			// �_�C�A���O�X�V
};
