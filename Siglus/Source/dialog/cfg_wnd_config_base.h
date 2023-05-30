#pragma		once

#include	"dialog/cfg_wnd_solo.h"

#include	"dialog/cfg_wnd_base_screen.h"
#include	"dialog/cfg_wnd_base_font_msgspd.h"
#include	"dialog/cfg_wnd_base_volume_bgmfade.h"
#include	"dialog/cfg_wnd_base_volume.h"
#include	"dialog/cfg_wnd_base_koemode_chrkoe.h"
#include	"dialog/cfg_wnd_base_koemode.h"
#include	"dialog/cfg_wnd_base_chrkoe.h"
#include	"dialog/cfg_wnd_base_mwndbk.h"
#include	"dialog/cfg_wnd_base_automode.h"
#include	"dialog/cfg_wnd_base_jitan.h"
#include	"dialog/cfg_wnd_base_else.h"
#include	"dialog/cfg_wnd_base_system.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X
// ================================================================
class C_cfg_wnd_config_base : public C_tnm_dlg
{
public:
	void		init();						// ������
	void		frame();					// �t���[��

	ARRAY<C_cfg_wnd_solo *>::iterator	get_child_window_list_begin()		{	return child_window_list.begin();		}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_child_window_list_end()			{	return child_window_list.end();			}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_all_child_window_list_begin()	{	return all_child_window_list.begin();	}
	ARRAY<C_cfg_wnd_solo *>::iterator	get_all_child_window_list_end()		{	return all_child_window_list.end();		}

protected:

	bool		cfg_wnd_full_mode_flag;

	void		init_child_window();		// �q�E�B���h�E��������
	
	

private:

	void		regist_child_window();		// �q�E�B���h�E��o�^
	void		open_child_window();		// �q�E�B���h�E���J��
	void		close_all_child_window(C_cfg_wnd_solo* no_close_window = NULL);	// �S�Ă̎q�E�B���h�E�����
	void		adjust_child_window();		// �q�E�B���h�E�𒲐�����

	// �e�L�X�g�^�u
	class C_tab_base : public Cc_tab
	{
	public:
		C_cfg_wnd_config_base *parent_class;
	protected:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		C_tab_base	tab_base;
		Cc_button	btn_all_config_init;
		Cc_button	btn_close;
		Cc_button	stc_game_version;
	}	ci;

	// �q�E�B���h�E���X�g
	ARRAY<C_cfg_wnd_solo *>	all_child_window_list;
	ARRAY<C_cfg_wnd_solo *>	child_window_list;

	// �q�E�B���h�E
	C_cfg_wnd_base_screen			cfg_wnd_base_screen;			// �x�[�X�F��ʃ��[�h
	C_cfg_wnd_base_font_msgspd		cfg_wnd_base_font_msgspd;		// �x�[�X�F���b�Z�[�W�i�t�H���g�A�������x�j
	Ccfg_wnd_base_volume_bgmfade	cfg_wnd_base_volume_bgmfade;	// �x�[�X�F���ʁi���ʁA�a�f�l�t�F�[�h�j
	Ccfg_wnd_base_volume			cfg_wnd_base_volume;			// �x�[�X�F����
	Ccfg_wnd_base_koemode_chrkoe	cfg_wnd_base_koemode_chrkoe;	// �x�[�X�F�����i�������[�h�A�L�����N�^�[�����j
	Ccfg_wnd_base_koemode			cfg_wnd_base_koemode;			// �x�[�X�F�������[�h
	Ccfg_wnd_base_chrkoe			cfg_wnd_base_chrkoe;			// �x�[�X�F�L�����N�^�[����
	Ccfg_wnd_base_mwndbk			cfg_wnd_base_mwndbk;			// �x�[�X�F�E�B���h�E�w�i�F
	Ccfg_wnd_base_automode			cfg_wnd_base_automode;			// �x�[�X�F�I�[�g���[�h
	Ccfg_wnd_base_jitan				cfg_wnd_base_jitan;				// �x�[�X�F���Z�Đ�
	Ccfg_wnd_base_else				cfg_wnd_base_else;				// �x�[�X�F���̑�
	Ccfg_wnd_base_system			cfg_wnd_base_system;			// �x�[�X�F�V�X�e��

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_mousewheel(WPARAM wp, LPARAM lp);			// �}�E�X�z�C�[��
};
