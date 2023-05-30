#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�a�f�l�t�F�[�h
// ================================================================
class Ccfg_wnd_func_bgmfade : public C_cfg_wnd_func
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

	void		init_state();			// ������Ԃɖ߂�

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_group	grp_bgmfade;
		Cc_static	stc_bgmfade;
		Cc_slider	sld_bgmfade;
		Cc_check	chk_bgmfade;
		Cc_button	btn_bgmfade_init;
		Cc_button	btn_bgmfade_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		setup_dialog();				// �_�C�A���O���\�z
	void		update_dialog();			// �_�C�A���O���X�V
};
