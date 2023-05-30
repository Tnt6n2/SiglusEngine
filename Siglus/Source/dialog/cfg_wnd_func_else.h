#pragma		once

#include	"data/tnm_def.h"
#include	"dialog/cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：その他
// ================================================================
class Ccfg_wnd_func_else : public C_cfg_wnd_func
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
		Cc_group	grp_else;
		Cc_check	chk_message_chrcolor;
		Cc_check	chk_object_disp[TNM_OBJECT_DISP_CNT];
		Cc_check	chk_global_extra_switch[TNM_GLOBAL_EXTRA_SWITCH_CNT];

		Cc_group	grp_global_extra_mode[TNM_GLOBAL_EXTRA_MODE_CNT];
		Cc_radio	rad_global_extra_mode[TNM_GLOBAL_EXTRA_MODE_CNT][TNM_GLOBAL_EXTRA_MODE_ITEM_CNT];

		Cc_button	btn_else_init;
		Cc_button	btn_else_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		update_dialog();				// ダイアログ更新
	void		setup_item();					// アイテムの設定
	void		set_radio_mode(int mode_no);	// モードのラジオボタンを設定
	int			get_radio_mode(int mode_no);	// モードのラジオボタンを取得

};
