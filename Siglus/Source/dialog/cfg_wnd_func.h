#pragma		once

// ****************************************************************
// コンフィグウィンドウ：ファンクション
// ================================================================
class C_cfg_wnd_func
{

protected:

	C_dialog*		p_dlg;
	HWND			h_wnd;
	int				func_mod;
	bool			base_mode_flag;
	bool			full_mode_flag;
	bool			modal_mode_flag;
	bool			wide_language_flag;
	bool			wide_koe_wnd_flag;		// 音声の設定ウィンドウをワイド化するフラグ

	int				frame_start_time;
	int				frame_time_past;

public:

	C_cfg_wnd_func()
	{
		p_dlg = NULL;
		h_wnd = NULL;
		full_mode_flag = false;
		modal_mode_flag = false;
		wide_language_flag = false;
	}

	void		on_init_func(C_dialog* p_dlg_, int func_mod_, bool is_base_mode, bool is_full_mode, bool is_modal_mode);		// 初期化
	void		on_close_func();									// 閉じる
	bool		on_command_func(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify_func(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel_func(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll_func(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll_func(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic_func(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	void		frame_func();										// フレーム

protected:

	void		refresh_dialog_func(int dialog_width, int dialog_height);

	virtual		void		on_init_funcfunc();										// 初期化
	virtual		void		on_close_funcfunc();									// 閉じる
	virtual		bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// コマンド
	virtual		bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// 通知
	virtual		bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// マウスホイール
	virtual		bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｈスクロール
	virtual		bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｖスクロール
	virtual		LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	virtual		void		frame_funcfunc();										// フレーム
};
