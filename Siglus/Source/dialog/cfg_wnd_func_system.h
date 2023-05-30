#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：システム
// ================================================================
class C_cfg_wnd_func_system : public C_cfg_wnd_func
{
public:
	void		on_init_funcfunc();										// 初期化
	void		on_close_funcfunc();									// 閉じる
	bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	void		frame_funcfunc();										// フレーム

	void		init_state();							// 初期状態に戻す

private:

	// コントロールアイテム
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

	void		update_dialog();			// ダイアログ更新

	void		set_radio_movie_play_type();	// ムービーの再生方法のラジオボタンを設定
	int			get_radio_movie_play_type();	// ムービーの再生方法のラジオボタンを取得
};
