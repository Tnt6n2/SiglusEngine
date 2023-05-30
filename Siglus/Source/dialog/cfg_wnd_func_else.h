#pragma		once

#include	"data/tnm_def.h"
#include	"dialog/cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F���̑�
// ================================================================
class Ccfg_wnd_func_else : public C_cfg_wnd_func
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
		Cc_group	grp_else;
		Cc_check	chk_message_chrcolor;
		Cc_check	chk_object_disp[TNM_OBJECT_DISP_CNT];
		Cc_check	chk_global_extra_switch[TNM_GLOBAL_EXTRA_SWITCH_CNT];

		Cc_group	grp_global_extra_mode[TNM_GLOBAL_EXTRA_MODE_CNT];
		Cc_radio	rad_global_extra_mode[TNM_GLOBAL_EXTRA_MODE_CNT][TNM_GLOBAL_EXTRA_MODE_ITEM_CNT];

		Cc_button	btn_else_init;
		Cc_button	btn_else_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		update_dialog();				// �_�C�A���O�X�V
	void		setup_item();					// �A�C�e���̐ݒ�
	void		set_radio_mode(int mode_no);	// ���[�h�̃��W�I�{�^����ݒ�
	int			get_radio_mode(int mode_no);	// ���[�h�̃��W�I�{�^�����擾

};
