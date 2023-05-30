#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�t�@���N�V�����F��ʃ��[�h
// ================================================================
class C_cfg_wnd_func_screen : public C_cfg_wnd_func
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
		Cc_group			grp_screen_mode;
		Cc_radio			rad_screen_mode_fullscreen;
		Cc_radio			rad_screen_mode_window;
		Cc_group			grp_window_size;
		Cc_radio			rad_window_size_050;
		Cc_radio			rad_window_size_075;
		Cc_radio			rad_window_size_100;
		Cc_radio			rad_window_size_150;
		Cc_radio			rad_window_size_200;

		Cc_group			grp_fullscreen_resolution;
		Cc_check			chk_fullscreen_change_resolution;
		Cc_button			btn_change_resolution;
		Cc_button			btn_change_resolution_info;
		Cc_static			stc_fullscreen_display;
		Cc_static			stc_fullscreen_resolution;
		Cc_combobox			cmb_fullscreen_display;
		Cc_combobox			cmb_fullscreen_resolution;

		Cc_group			grp_fullscreen_detail;

		Cc_group			grp_fullscreen_disp;
		Cc_radio			rad_fullscreen_disp_normal;
		Cc_radio			rad_fullscreen_disp_fit;
		Cc_radio			rad_fullscreen_disp_udlrfit;
		Cc_radio			rad_fullscreen_disp_udfit;
		Cc_radio			rad_fullscreen_disp_lrfit;
		Cc_radio			rad_fullscreen_disp_custom;

		Cc_group			grp_fullscreen_custom;
		Cc_static			stc_fullscreen_custom_scale_ud;
		Cc_button			btn_fullscreen_custom_scale_ud;
		Cc_static			stc_fullscreen_custom_scale_ud_val;
		Cc_slider			sld_fullscreen_custom_scale_ud;
		Cc_static			stc_fullscreen_custom_scale_lr;
		Cc_button			btn_fullscreen_custom_scale_lr;
		Cc_static			stc_fullscreen_custom_scale_lr_val;
		Cc_slider			sld_fullscreen_custom_scale_lr;
		Cc_check			chk_fullscreen_custom_scale;
		Cc_static			stc_fullscreen_custom_move_ud;
		Cc_button			btn_fullscreen_custom_move_ud;
		Cc_static			stc_fullscreen_custom_move_ud_val;
		Cc_slider			sld_fullscreen_custom_move_ud;
		Cc_static			stc_fullscreen_custom_move_lr;
		Cc_button			btn_fullscreen_custom_move_lr;
		Cc_static			stc_fullscreen_custom_move_lr_val;
		Cc_slider			sld_fullscreen_custom_move_lr;

		Cc_button			btn_fullscreen_init;
		Cc_button			btn_screen_close;
	}	ci;

	// ��ƕϐ�
	struct WORK
	{
		bool	window_size_item_enable_flag;
		bool	fullscreen_resolution_item_enable_flag;
		bool	fullscreen_mode_item_enable_flag;
		bool	fullscreen_custom_item_enable_flag;
		int		screen_mode_change_start_time;
		bool	screen_mode_item_enable;
	}	work;

	void		update_dialog();						// �_�C�A���O�X�V
	void		set_radio_screen_mode();				// ��ʃ��[�h�̃��W�I�{�^����ݒ�
	bool		check_radio_screen_mode();				// ��ʃ��[�h�̃��W�I�{�^���𔻒�
	void		set_radio_window_size();				// �E�B���h�E�T�C�Y�̃��W�I�{�^����ݒ�
	int			get_radio_window_size();				// �E�B���h�E�T�C�Y�̃��W�I�{�^�����擾
	void		set_radio_fullscreen_disp();			// �t���X�N���[���\�����[�h�̃��W�I�{�^����ݒ�
	int			get_radio_fullscreen_disp();			// �t���X�N���[���\�����[�h�̃��W�I�{�^�����擾
	void		set_fullscreen_item_enable();			// �t���X�N���[���\���A�C�e���̗L���E�֎~
	void		set_screen_mode_btn_enable();			// ��ʃT�C�Y�ɉ����ĉ�ʃ��[�h�{�^����L���E�֎~
	void		change_screen_mode_func();				// ��ʃT�C�Y��ύX������
	void		enable_screen_mode_item(bool flag);		// ��ʃT�C�Y��ύX����A�C�e����L���E�֎~
	void		set_screen_mode_item();					// ��ʃT�C�Y��ύX����A�C�e���̗L���E�֎~��ݒ肷��
	int			get_now_resolution_mode();				// ���݂̉𑜓x���[�h���擾
	void		make_resolution_combobox_list();		// �𑜓x�̃R���{�{�b�N�X�̃��X�g���쐬
	void		dropdown_resolution_combobox();			// �𑜓x�̃R���{�{�b�N�X���h���b�v�_�E������

	void		set_fullscreen_display_no(int display_no);			// �t���X�N���[���̃f�B�X�v���C�ԍ���ݒ�
	void		set_fullscreen_resolution_no(int resolution_no);	// �t���X�N���[���̉𑜓x�ԍ���ݒ�

	void		update_text_info();						// �e�L�X�g���̍X�V
};
