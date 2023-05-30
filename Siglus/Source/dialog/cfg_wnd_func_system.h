#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F�V�X�e��
// ================================================================
class C_cfg_wnd_func_system : public C_cfg_wnd_func
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
		Cc_group	grp_system;
		Cc_check	chk_system_sleep;
		Cc_check	chk_system_no_wipe_anime;
		Cc_check	chk_system_skip_wipe_anime;
		Cc_check	chk_system_no_mwnd_anime;
		Cc_check	chk_system_wheel_next_message;
		Cc_check	chk_system_koe_dont_stop;
		Cc_check	chk_system_skip_unread_message;
		Cc_check	chk_system_play_silent_sound;
		Cc_button	btn_system_init;
		Cc_group	grp_system_movie;
		Cc_radio	rad_system_movie_play_type_mci;
		Cc_radio	rad_system_movie_play_type_wmp;
		Cc_button	btn_system_close;
	}	ci;

	void		update_dialog();			// �_�C�A���O�X�V

	void		set_radio_movie_play_type();	// ���[�r�[�̍Đ����@�̃��W�I�{�^����ݒ�
	int			get_radio_movie_play_type();	// ���[�r�[�̍Đ����@�̃��W�I�{�^�����擾
};
