#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�V�X�e��
// ================================================================
class Ccfg_wnd_func_movtype : public C_cfg_wnd_func
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

	bool		support_return_value;					// ���[�r�[�T�|�[�g�p�̕Ԃ�l

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_static	stc_movtype_mess00;
		Cc_static	stc_movtype_mess01;
		Cc_radio	rad_movtype_mci;
		Cc_radio	rad_movtype_wmp;
		Cc_button	btn_movtype_close;
		Cc_button	btn_movtype_deside;
		Cc_button	btn_movtype_cancel;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		update_dialog();			// �_�C�A���O�X�V

	void		set_radio_movtype();		// ���[�r�[�̍Đ����@�̃��W�I�{�^����ݒ�
	int			get_radio_movtype();		// ���[�r�[�̍Đ����@�̃��W�I�{�^�����擾
};
