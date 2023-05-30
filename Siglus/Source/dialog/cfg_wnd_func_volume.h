#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F����
// ================================================================
class Ccfg_wnd_func_volume : public C_cfg_wnd_func
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

	void		init_state();				// ������Ԃɖ߂�

	void		rep_window_size_func(int x_rep);		// �u�E�B���h�E�T�C�Y�v�Ɓu�������{�^���v�Ɓu����{�^���v�𒲐�����

	int			bgmfade_item_x_rep;						// �a�f�l�t�F�[�h�̍��W�����p�ɋL��

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_group	grp_volume;
		Cc_group	grp_volume_all;
		Cc_slider	sld_volume_all;
		Cc_check	chk_volume_all;
		Cc_group	grp_volume_bgm;
		Cc_slider	sld_volume_bgm;
		Cc_check	chk_volume_bgm;
		Cc_group	grp_volume_koe;
		Cc_slider	sld_volume_koe;
		Cc_check	chk_volume_koe;
		Cc_group	grp_volume_pcm;
		Cc_slider	sld_volume_pcm;
		Cc_check	chk_volume_pcm;
		Cc_group	grp_volume_se;
		Cc_slider	sld_volume_se;
		Cc_check	chk_volume_se;
		Cc_group	grp_volume_mov;
		Cc_slider	sld_volume_mov;
		Cc_check	chk_volume_mov;
		Cc_button	btn_volume_init;
		Cc_button	btn_volume_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		setup_dialog();				// �_�C�A���O���\�z
	void		update_dialog();			// �_�C�A���O���X�V
};
