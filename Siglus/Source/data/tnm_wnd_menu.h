#pragma		once

// ****************************************************************
// �E�B���h�E���j���[
// ================================================================
class C_tnm_wnd_menu : public C_menu
{
public:

	void		create();						// ���j���[�쐬
	void		destroy();						// ���j���[�j��
	void		on_command(DWORD menu_id);		// �R�}���h
	void		update();						// ���j���[�X�V

private:

	// �V�X�e��
	C_menu		sub_menu_system;
	C_menu		sub_menu_system_syscommenu;
	C_menu		sub_menu_system_config_solo;
	C_menu		sub_menu_system_config_modal;
	C_menu		sub_menu_system_debug;
	C_menu		sub_menu_system_debug_syscommenu_full_mode;

	// �f�o�b�O
	C_menu		sub_menu_debug;

	// �E�B���h�E
	C_menu		sub_menu_window;
	C_menu		sub_menu_window_d3d;
	C_menu		sub_menu_window_system;
	C_menu		sub_menu_window_flag;
	C_menu		sub_menu_window_flag_ini;
	C_menu		sub_menu_window_object;

	// �R�}���h
	C_menu		sub_menu_command;
};


