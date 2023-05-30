#pragma		once

// ****************************************************************
// ウィンドウメニュー
// ================================================================
class C_tnm_wnd_menu : public C_menu
{
public:

	void		create();						// メニュー作成
	void		destroy();						// メニュー破棄
	void		on_command(DWORD menu_id);		// コマンド
	void		update();						// メニュー更新

private:

	// システム
	C_menu		sub_menu_system;
	C_menu		sub_menu_system_syscommenu;
	C_menu		sub_menu_system_config_solo;
	C_menu		sub_menu_system_config_modal;
	C_menu		sub_menu_system_debug;
	C_menu		sub_menu_system_debug_syscommenu_full_mode;

	// デバッグ
	C_menu		sub_menu_debug;

	// ウィンドウ
	C_menu		sub_menu_window;
	C_menu		sub_menu_window_d3d;
	C_menu		sub_menu_window_system;
	C_menu		sub_menu_window_flag;
	C_menu		sub_menu_window_flag_ini;
	C_menu		sub_menu_window_object;

	// コマンド
	C_menu		sub_menu_command;
};


