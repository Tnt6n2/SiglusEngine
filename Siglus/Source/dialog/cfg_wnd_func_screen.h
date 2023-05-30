#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：画面モード
// ================================================================
class C_cfg_wnd_func_screen : public C_cfg_wnd_func
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

	// 作業変数
	struct WORK
	{
		bool	window_size_item_enable_flag;
		bool	fullscreen_resolution_item_enable_flag;
		bool	fullscreen_mode_item_enable_flag;
		bool	fullscreen_custom_item_enable_flag;
		int		screen_mode_change_start_time;
		bool	screen_mode_item_enable;
	}	work;

	void		update_dialog();						// ダイアログ更新
	void		set_radio_screen_mode();				// 画面モードのラジオボタンを設定
	bool		check_radio_screen_mode();				// 画面モードのラジオボタンを判定
	void		set_radio_window_size();				// ウィンドウサイズのラジオボタンを設定
	int			get_radio_window_size();				// ウィンドウサイズのラジオボタンを取得
	void		set_radio_fullscreen_disp();			// フルスクリーン表示モードのラジオボタンを設定
	int			get_radio_fullscreen_disp();			// フルスクリーン表示モードのラジオボタンを取得
	void		set_fullscreen_item_enable();			// フルスクリーン表示アイテムの有効・禁止
	void		set_screen_mode_btn_enable();			// 画面サイズに応じて画面モードボタンを有効・禁止
	void		change_screen_mode_func();				// 画面サイズを変更したよ
	void		enable_screen_mode_item(bool flag);		// 画面サイズを変更するアイテムを有効・禁止
	void		set_screen_mode_item();					// 画面サイズを変更するアイテムの有効・禁止を設定する
	int			get_now_resolution_mode();				// 現在の解像度モードを取得
	void		make_resolution_combobox_list();		// 解像度のコンボボックスのリストを作成
	void		dropdown_resolution_combobox();			// 解像度のコンボボックスをドロップダウンした

	void		set_fullscreen_display_no(int display_no);			// フルスクリーンのディスプレイ番号を設定
	void		set_fullscreen_resolution_no(int resolution_no);	// フルスクリーンの解像度番号を設定

	void		update_text_info();						// テキスト情報の更新
};
