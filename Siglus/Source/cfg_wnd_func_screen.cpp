#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_system.h"
#include	"engine/eng_display.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_screen.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：画面モード
// ================================================================

// 初期化
void C_cfg_wnd_func_screen::on_init_funcfunc()
{
	work.window_size_item_enable_flag = true;
	work.fullscreen_resolution_item_enable_flag = true;
	work.fullscreen_mode_item_enable_flag = true;
	work.fullscreen_custom_item_enable_flag = true;

	// アイテム登録
	p_dlg->bind_control(&ci.grp_screen_mode, IDC_GRP_SCREEN_MODE, _T(""), 7);
	p_dlg->bind_control(&ci.grp_window_size, IDC_GRP_WINDOW_SIZE, _T(""), 8);

	p_dlg->bind_control(&ci.grp_fullscreen_resolution, IDC_GRP_FULLSCREEN_RESOLUTION, _T(""), 8);
	p_dlg->bind_control(&ci.grp_fullscreen_detail, IDC_GRP_FULLSCREEN_DETAIL, _T(""), 5);
	p_dlg->bind_control(&ci.grp_fullscreen_disp, IDC_GRP_FULLSCREEN_DISP, _T(""), 7);
	p_dlg->bind_control(&ci.grp_fullscreen_custom, IDC_GRP_FULLSCREEN_CUSTOM, _T(""), 8);

	p_dlg->bind_control(&ci.rad_screen_mode_fullscreen, IDC_RAD_SCREEN_MODE_FULLSCREEN, _T(""), 7);
	p_dlg->bind_control(&ci.rad_screen_mode_window, IDC_RAD_SCREEN_MODE_WINDOW, _T(""), 7);

	p_dlg->bind_control(&ci.rad_window_size_050, IDC_RAD_WINDOW_SIZE_050, _T(""), 7);
	p_dlg->bind_control(&ci.rad_window_size_075, IDC_RAD_WINDOW_SIZE_075, _T(""), 7);
	p_dlg->bind_control(&ci.rad_window_size_100, IDC_RAD_WINDOW_SIZE_100, _T(""), 7);
	p_dlg->bind_control(&ci.rad_window_size_150, IDC_RAD_WINDOW_SIZE_150, _T(""), 7);
	p_dlg->bind_control(&ci.rad_window_size_200, IDC_RAD_WINDOW_SIZE_200, _T(""), 7);

	p_dlg->bind_control(&ci.chk_fullscreen_change_resolution, IDC_CHK_FULLSCREEN_CHANGE_RESOLUTION, _T(""), 7);
	p_dlg->bind_control(&ci.btn_change_resolution, IDC_BTN_CHANGE_RESOLUTION, _T(""), 9);
	p_dlg->bind_control(&ci.btn_change_resolution_info, IDC_BTN_CHANGE_RESOLUTION_INFO, _T(""), 9);
	p_dlg->bind_control(&ci.stc_fullscreen_display, IDC_STC_FULLSCREEN_DISPLAY, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_resolution, IDC_STC_FULLSCREEN_RESOLUTION, _T(""), 7);
	p_dlg->bind_control(&ci.cmb_fullscreen_display, IDC_CMB_FULLSCREEN_DISPLAY, _T(""), 8);
	p_dlg->bind_control(&ci.cmb_fullscreen_resolution, IDC_CMB_FULLSCREEN_RESOLUTION, _T(""), 8);

	p_dlg->bind_control(&ci.rad_fullscreen_disp_normal, IDC_RAD_FULLSCREEN_DISP_NORMAL, _T(""), 7);
	p_dlg->bind_control(&ci.rad_fullscreen_disp_fit, IDC_RAD_FULLSCREEN_DISP_FIT, _T(""), 7);
	p_dlg->bind_control(&ci.rad_fullscreen_disp_udlrfit, IDC_RAD_FULLSCREEN_DISP_UDLRFIT, _T(""), 7);
	p_dlg->bind_control(&ci.rad_fullscreen_disp_udfit, IDC_RAD_FULLSCREEN_DISP_UDFIT, _T(""), 7);
	p_dlg->bind_control(&ci.rad_fullscreen_disp_lrfit, IDC_RAD_FULLSCREEN_DISP_LRFIT, _T(""), 7);
	p_dlg->bind_control(&ci.rad_fullscreen_disp_custom, IDC_RAD_FULLSCREEN_DISP_CUSTOM, _T(""), 7);

	p_dlg->bind_control(&ci.stc_fullscreen_custom_scale_ud, IDC_STC_FULLSCREEN_CUSTOM_SCALE_UD, _T(""), 7);
	p_dlg->bind_control(&ci.btn_fullscreen_custom_scale_ud, IDC_BTN_FULLSCREEN_CUSTOM_SCALE_UD, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_scale_ud_val, IDC_STC_FULLSCREEN_CUSTOM_SCALE_UD_VAL, _T(""), 7);
	p_dlg->bind_control(&ci.sld_fullscreen_custom_scale_ud, IDC_SLD_FULLSCREEN_CUSTOM_SCALE_UD, _T(""), 8);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_scale_lr, IDC_STC_FULLSCREEN_CUSTOM_SCALE_LR, _T(""), 7);
	p_dlg->bind_control(&ci.btn_fullscreen_custom_scale_lr, IDC_BTN_FULLSCREEN_CUSTOM_SCALE_LR, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_scale_lr_val, IDC_STC_FULLSCREEN_CUSTOM_SCALE_LR_VAL, _T(""), 7);
	p_dlg->bind_control(&ci.sld_fullscreen_custom_scale_lr, IDC_SLD_FULLSCREEN_CUSTOM_SCALE_LR, _T(""), 8);
	p_dlg->bind_control(&ci.chk_fullscreen_custom_scale, IDC_CHK_FULLSCREEN_CUSTOM_SCALE, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_move_ud, IDC_STC_FULLSCREEN_CUSTOM_MOVE_UD, _T(""), 7);
	p_dlg->bind_control(&ci.btn_fullscreen_custom_move_ud, IDC_BTN_FULLSCREEN_CUSTOM_MOVE_UD, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_move_ud_val, IDC_STC_FULLSCREEN_CUSTOM_MOVE_UD_VAL, _T(""), 7);
	p_dlg->bind_control(&ci.sld_fullscreen_custom_move_ud, IDC_SLD_FULLSCREEN_CUSTOM_MOVE_UD, _T(""), 8);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_move_lr, IDC_STC_FULLSCREEN_CUSTOM_MOVE_LR, _T(""), 7);
	p_dlg->bind_control(&ci.btn_fullscreen_custom_move_lr, IDC_BTN_FULLSCREEN_CUSTOM_MOVE_LR, _T(""), 7);
	p_dlg->bind_control(&ci.stc_fullscreen_custom_move_lr_val, IDC_STC_FULLSCREEN_CUSTOM_MOVE_LR_VAL, _T(""), 7);
	p_dlg->bind_control(&ci.sld_fullscreen_custom_move_lr, IDC_SLD_FULLSCREEN_CUSTOM_MOVE_LR, _T(""), 8);

	p_dlg->bind_control(&ci.btn_fullscreen_init, IDC_BTN_FULLSCREEN_INIT, _T(""), 3);

	p_dlg->bind_control(&ci.btn_screen_close, IDC_BTN_SCREEN_CLOSE, _T(""), 3);

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_screen_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }
	ci.sld_fullscreen_custom_scale_ud.set_slider_range(50, 300);
	ci.sld_fullscreen_custom_scale_lr.set_slider_range(50, 300);
	ci.sld_fullscreen_custom_move_ud.set_slider_range(-100, 100);
	ci.sld_fullscreen_custom_move_lr.set_slider_range(-100, 100);

	// ディスプレイのコンボボックスのリスト作成
	for (int i = 0; i < (int)Gp_system_info->display_list.size(); i++)	{
		ci.cmb_fullscreen_display.insert_item(MBSTR_to_TSTR(Gp_system_info->display_list[i].DeviceName), i, false);
	}
	ci.cmb_fullscreen_display.set_sel_no(Gp_config->fullscreen_display_no);

	// ▲解像度：解像度についてのボタン

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void C_cfg_wnd_func_screen::on_close_funcfunc()
{
}

// コマンド
bool C_cfg_wnd_func_screen::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{

		case IDC_CHK_FULLSCREEN_CHANGE_RESOLUTION:
			Gp_global->dialog_fullscreen_change_resolution = ci.chk_fullscreen_change_resolution.get_check();
			return true;

		case IDC_BTN_CHANGE_RESOLUTION_INFO:
			tnm_game_info_box(LCL_STR_RESOLUTION_EXPLAIN_STR(Gp_ini->screen_size.cx, Gp_ini->screen_size.cy));
			return true;

		case IDC_CMB_FULLSCREEN_DISPLAY:
			if (HIWORD(wp) == CBN_SELENDOK)	{
				set_fullscreen_display_no(ci.cmb_fullscreen_display.get_sel_no());
				make_resolution_combobox_list();
			}
			return true;

		case IDC_CMB_FULLSCREEN_RESOLUTION:
			if (HIWORD(wp) == CBN_SELENDOK)	{
				set_fullscreen_resolution_no(ci.cmb_fullscreen_resolution.get_sel_no());
				make_resolution_combobox_list();
			}
			else if (HIWORD(wp) == CBN_SELENDCANCEL)	{
				make_resolution_combobox_list();
			}
			else if (HIWORD(wp) == CBN_DROPDOWN)	{
				dropdown_resolution_combobox();
			}
			return true;

		case IDC_BTN_CHANGE_RESOLUTION:
			change_screen_mode_func();
			tnm_change_screen_size_mode_fullscreen(false, true);
			return true;

		case IDC_RAD_SCREEN_MODE_FULLSCREEN:
			change_screen_mode_func();
			tnm_change_screen_size_mode_fullscreen(false, true);
			return true;

		case IDC_RAD_SCREEN_MODE_WINDOW:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(false);
			return true;

		case IDC_RAD_WINDOW_SIZE_050:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(C_size(50, 50), false);
			return true;

		case IDC_RAD_WINDOW_SIZE_075:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(C_size(75, 75), false);
			return true;

		case IDC_RAD_WINDOW_SIZE_100:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(C_size(100, 100), false);
			return true;

		case IDC_RAD_WINDOW_SIZE_150:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(C_size(150, 150), false);
			return true;

		case IDC_RAD_WINDOW_SIZE_200:
			change_screen_mode_func();
			tnm_change_screen_size_mode_window(C_size(200, 200), false);
			return true;

		case IDC_RAD_FULLSCREEN_DISP_NORMAL:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_NORMAL;
			return true;

		case IDC_RAD_FULLSCREEN_DISP_FIT:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_FIT;
			return true;

		case IDC_RAD_FULLSCREEN_DISP_UDLRFIT:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_UDLRFIT;
			return true;

		case IDC_RAD_FULLSCREEN_DISP_UDFIT:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_UDFIT;
			return true;

		case IDC_RAD_FULLSCREEN_DISP_LRFIT:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_LRFIT;
			return true;

		case IDC_RAD_FULLSCREEN_DISP_CUSTOM:
			Gp_config->fullscreen_mode = TNM_FULLSCREEN_MODE_CUSTOM;
			return true;

		case IDC_BTN_FULLSCREEN_CUSTOM_SCALE_UD:
			Gp_config->fullscreen_scale.cy = 100;
			if (Gp_config->fullscreen_scale_sync_switch)	{	// 倍率同期
				Gp_config->fullscreen_scale.cx = Gp_config->fullscreen_scale.cy;
				ci.sld_fullscreen_custom_scale_lr.set_slider_pos(Gp_config->fullscreen_scale.cx);
				ci.sld_fullscreen_custom_scale_ud.set_slider_pos(Gp_config->fullscreen_scale.cy);
			}
			update_text_info();
			return true;

		case IDC_BTN_FULLSCREEN_CUSTOM_SCALE_LR:
			Gp_config->fullscreen_scale.cx = 100;
			if (Gp_config->fullscreen_scale_sync_switch)	{	// 倍率同期
				Gp_config->fullscreen_scale.cy = Gp_config->fullscreen_scale.cx;
				ci.sld_fullscreen_custom_scale_lr.set_slider_pos(Gp_config->fullscreen_scale.cx);
				ci.sld_fullscreen_custom_scale_ud.set_slider_pos(Gp_config->fullscreen_scale.cy);
			}
			update_text_info();
			return true;

		case IDC_BTN_FULLSCREEN_CUSTOM_MOVE_UD:
			Gp_config->fullscreen_move.cy = 0;
			update_text_info();
			return true;

		case IDC_BTN_FULLSCREEN_CUSTOM_MOVE_LR:
			Gp_config->fullscreen_move.cx = 0;
			update_text_info();
			return true;

		case IDC_CHK_FULLSCREEN_CUSTOM_SCALE:
			Gp_config->fullscreen_scale_sync_switch = ci.chk_fullscreen_custom_scale.get_check();
			return true;

		case IDC_BTN_FULLSCREEN_INIT:
			init_state();
			return true;

		case IDC_BTN_SCREEN_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// 通知
bool C_cfg_wnd_func_screen::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool C_cfg_wnd_func_screen::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool C_cfg_wnd_func_screen::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
	if (false);
	else if ((HWND)lp == ci.sld_fullscreen_custom_scale_ud.get_handle())	{
		Gp_config->fullscreen_scale.cy = ci.sld_fullscreen_custom_scale_ud.get_slider_pos();
		if (Gp_config->fullscreen_scale_sync_switch)	{	// 倍率同期
			Gp_config->fullscreen_scale.cx = Gp_config->fullscreen_scale.cy;
			ci.sld_fullscreen_custom_scale_lr.set_slider_pos(Gp_config->fullscreen_scale.cx);
		}
		update_text_info();
		return true;
	}
	else if ((HWND)lp == ci.sld_fullscreen_custom_scale_lr.get_handle())	{
		Gp_config->fullscreen_scale.cx = ci.sld_fullscreen_custom_scale_lr.get_slider_pos();
		if (Gp_config->fullscreen_scale_sync_switch)	{	// 倍率同期
			Gp_config->fullscreen_scale.cy = Gp_config->fullscreen_scale.cx;
			ci.sld_fullscreen_custom_scale_ud.set_slider_pos(Gp_config->fullscreen_scale.cy);
		}
		update_text_info();
		return true;
	}
	else if ((HWND)lp == ci.sld_fullscreen_custom_move_ud.get_handle())		{
		Gp_config->fullscreen_move.cy = ci.sld_fullscreen_custom_move_ud.get_slider_pos();
		update_text_info();
		return true;
	}
	else if ((HWND)lp == ci.sld_fullscreen_custom_move_lr.get_handle())		{
		Gp_config->fullscreen_move.cx = ci.sld_fullscreen_custom_move_lr.get_slider_pos();
		update_text_info();
		return true;
	}
	return false;
}

// Ｖスクロール
bool C_cfg_wnd_func_screen::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func_screen::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void C_cfg_wnd_func_screen::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 画面サイズを変更するアイテムの有効・禁止を設定する（画面サイズに応じて画面モードボタンを有効・禁止）
	set_screen_mode_item();

	// フルスクリーン表示アイテムの有効・禁止
	set_fullscreen_item_enable();

	// 外部で変更された
	bool update_text_info_flag = false;
	if (!check_radio_screen_mode())	{
		set_radio_screen_mode();
	}
	int size_mode = (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW) ? tnm_format_screen_size_to_screen_mode(Gp_config->screen_size_scale) : -1;
	if (size_mode != get_radio_window_size())	{
		set_radio_window_size();
	}

	if (Gp_global->dialog_fullscreen_change_resolution != ci.chk_fullscreen_change_resolution.get_check())	{	// コンフィグではなくシステムを見る
		ci.chk_fullscreen_change_resolution.set_check(Gp_global->dialog_fullscreen_change_resolution);
	}

	if (!ci.cmb_fullscreen_display.send_message(CB_GETDROPPEDSTATE, 0, 0))	{		// ドロップダウンリストが開いている時は処理しない
		if (Gp_global->dialog_fullscreen_display_no != ci.cmb_fullscreen_display.get_sel_no())	{	// コンフィグではなくシステムを見る
			ci.cmb_fullscreen_display.set_sel_no(Gp_global->dialog_fullscreen_display_no);
			make_resolution_combobox_list();
		}
	}

	if (!ci.cmb_fullscreen_resolution.send_message(CB_GETDROPPEDSTATE, 0, 0))	{		// ドロップダウンリストが開いている時は処理しない
		if (Gp_global->dialog_fullscreen_resolution_no != ci.cmb_fullscreen_resolution.get_sel_no())	{	// コンフィグではなくシステムを見る
			ci.cmb_fullscreen_resolution.set_sel_no(Gp_global->dialog_fullscreen_resolution_no);
		}
	}

	if (Gp_config->fullscreen_mode != get_radio_fullscreen_disp())	{
		set_radio_fullscreen_disp();
	}
	if (Gp_config->fullscreen_scale.cy != ci.sld_fullscreen_custom_scale_ud.get_slider_pos())	{
		ci.sld_fullscreen_custom_scale_ud.set_slider_pos(Gp_config->fullscreen_scale.cy);
		update_text_info_flag = true;
	}
	if (Gp_config->fullscreen_scale.cx != ci.sld_fullscreen_custom_scale_lr.get_slider_pos())	{
		ci.sld_fullscreen_custom_scale_lr.set_slider_pos(Gp_config->fullscreen_scale.cx);
		update_text_info_flag = true;
	}
	if (Gp_config->fullscreen_scale_sync_switch != ci.chk_fullscreen_custom_scale.get_check())	{
		ci.chk_fullscreen_custom_scale.set_check(Gp_config->fullscreen_scale_sync_switch);
	}
	if (Gp_config->fullscreen_move.cy != ci.sld_fullscreen_custom_move_ud.get_slider_pos())	{
		ci.sld_fullscreen_custom_move_ud.set_slider_pos(Gp_config->fullscreen_move.cy);
		update_text_info_flag = true;
	}
	if (Gp_config->fullscreen_move.cx != ci.sld_fullscreen_custom_move_lr.get_slider_pos())	{
		ci.sld_fullscreen_custom_move_lr.set_slider_pos(Gp_config->fullscreen_move.cx);
		update_text_info_flag = true;
	}
	if (update_text_info_flag)	{	update_text_info();	}
}

// 初期状態に戻す
void C_cfg_wnd_func_screen::init_state()
{
//	tnm_init_config_screen_mode();
	tnm_init_config_fullscreen();
	Gp_global->dialog_fullscreen_change_resolution = Gp_config->fullscreen_change_resolution;
	Gp_global->dialog_fullscreen_display_no = Gp_config->fullscreen_display_no;
	Gp_global->dialog_fullscreen_resolution_no = Gp_config->fullscreen_resolution_no;
	update_dialog();
}

// ダイアログ更新
void C_cfg_wnd_func_screen::update_dialog()
{
	if (!h_wnd)	{	return;	}
	set_radio_screen_mode();
	set_radio_window_size();

	ci.chk_fullscreen_change_resolution.set_check(Gp_global->dialog_fullscreen_change_resolution);

	ci.cmb_fullscreen_display.set_sel_no(Gp_global->dialog_fullscreen_display_no);
	make_resolution_combobox_list();

	set_radio_fullscreen_disp();
	ci.sld_fullscreen_custom_scale_ud.set_slider_pos(Gp_config->fullscreen_scale.cy);
	ci.sld_fullscreen_custom_scale_lr.set_slider_pos(Gp_config->fullscreen_scale.cx);
	ci.chk_fullscreen_custom_scale.set_check(Gp_config->fullscreen_scale_sync_switch);
	ci.sld_fullscreen_custom_move_ud.set_slider_pos(Gp_config->fullscreen_move.cy);
	ci.sld_fullscreen_custom_move_lr.set_slider_pos(Gp_config->fullscreen_move.cx);
	set_fullscreen_item_enable();
	set_screen_mode_item();
	update_text_info();
}

// ****************************************************************
// フルスクリーンのディスプレイ番号を設定する
// ================================================================
void C_cfg_wnd_func_screen::set_fullscreen_display_no(int display_no)
{
	int old_dialog_fullscreen_display_no = Gp_global->dialog_fullscreen_display_no;

	// ディスプレイ番号を変更
	if (Gp_global->dialog_fullscreen_display_no != display_no)	{
		Gp_global->dialog_fullscreen_display_no = display_no;
	}

	// ディスプレイ番号を補正
	if (Gp_global->dialog_fullscreen_display_no < 0 || (int)Gp_system_info->display_list.size() <= Gp_global->dialog_fullscreen_display_no)	{
		Gp_global->dialog_fullscreen_display_no = 0;
	}

	// ディスプレイ番号が変われば解像度をデフォルトに設定する
	if (Gp_global->dialog_fullscreen_display_no != old_dialog_fullscreen_display_no)	{
		Gp_global->dialog_fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].default_resolution_no;
	}

	// 解像度番号を設定
	set_fullscreen_resolution_no(Gp_global->dialog_fullscreen_resolution_no);
}

// ****************************************************************
// フルスクリーンの解像度番号を設定する
// ================================================================
void C_cfg_wnd_func_screen::set_fullscreen_resolution_no(int resolution_no)
{
	// 解像度番号を設定
	Gp_global->dialog_fullscreen_resolution_no = resolution_no;

	// 解像度番号を補正
	if (Gp_global->dialog_fullscreen_resolution_no < 0 || (int)Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].resolution_list.size() <= Gp_global->dialog_fullscreen_resolution_no)	{
		Gp_global->dialog_fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].default_resolution_no;
	}
}

// ****************************************************************
// 画面モードのラジオボタンを設定
// ================================================================
void C_cfg_wnd_func_screen::set_radio_screen_mode()
{
	if (!h_wnd)	{	return;	}
	ci.rad_screen_mode_fullscreen.set_check(false);
	ci.rad_screen_mode_window.set_check(false);
	switch (Gp_config->screen_size_mode)	{
		case TNM_SCREEN_SIZE_MODE_FULL:		ci.rad_screen_mode_fullscreen.set_check(true);		break;		// フルスクリーン
		case TNM_SCREEN_SIZE_MODE_WINDOW:	ci.rad_screen_mode_window.set_check(true);			break;		// ウィンドウ
		case TNM_SCREEN_SIZE_MODE_FREE:		ci.rad_screen_mode_window.set_check(true);			break;		// フリー
	}
}

// ****************************************************************
// 画面モードのラジオボタンを取得
// ================================================================
bool C_cfg_wnd_func_screen::check_radio_screen_mode()
{
	if (!h_wnd)	{ return true; }

	if (ci.rad_screen_mode_fullscreen.get_check()) {
		if (Gp_config->screen_size_mode != TNM_SCREEN_SIZE_MODE_FULL) {
			return false;
		}
	}
	if (ci.rad_screen_mode_window.get_check()) {
		if (Gp_config->screen_size_mode != TNM_SCREEN_SIZE_MODE_WINDOW && Gp_config->screen_size_mode != TNM_SCREEN_SIZE_MODE_FREE) {
			return false;
		}
	}

	return true;
}

// ****************************************************************
// ウィンドウサイズのラジオボタンを設定
// ================================================================
void C_cfg_wnd_func_screen::set_radio_window_size()
{
	if (!h_wnd)	{	return;	}
	ci.rad_window_size_050.set_check(false);
	ci.rad_window_size_075.set_check(false);
	ci.rad_window_size_100.set_check(false);
	ci.rad_window_size_150.set_check(false);
	ci.rad_window_size_200.set_check(false);

	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)
	{
		int size_mode = tnm_format_screen_size_to_screen_mode(Gp_config->screen_size_scale);
		switch (size_mode)	{
		case 0:		ci.rad_window_size_050.set_check(true);	break;
		case 1:		ci.rad_window_size_075.set_check(true);	break;
		case 2:		ci.rad_window_size_100.set_check(true);	break;
		case 3:		ci.rad_window_size_150.set_check(true);	break;
		case 4:		ci.rad_window_size_200.set_check(true);	break;
		}
	}
}

// ****************************************************************
// ウィンドウサイズのラジオボタンを取得
// ================================================================
int C_cfg_wnd_func_screen::get_radio_window_size()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_window_size_050.get_check())			{	return 0;	}
	else if (ci.rad_window_size_075.get_check())	{	return 1;	}
	else if (ci.rad_window_size_100.get_check())	{	return 2;	}
	else if (ci.rad_window_size_150.get_check())	{	return 3;	}
	else if (ci.rad_window_size_200.get_check())	{	return 4;	}
	return -1;
}

// ****************************************************************
// フルスクリーン表示モードのラジオボタンを設定
// ================================================================
void C_cfg_wnd_func_screen::set_radio_fullscreen_disp()
{
	if (!h_wnd)	{	return;	}
	ci.rad_fullscreen_disp_normal.set_check(false);
	ci.rad_fullscreen_disp_fit.set_check(false);
	ci.rad_fullscreen_disp_udlrfit.set_check(false);
	ci.rad_fullscreen_disp_udfit.set_check(false);
	ci.rad_fullscreen_disp_lrfit.set_check(false);
	ci.rad_fullscreen_disp_custom.set_check(false);
	switch (Gp_config->fullscreen_mode)	{
		case TNM_FULLSCREEN_MODE_NORMAL:	ci.rad_fullscreen_disp_normal.set_check(true);	break;		// 原寸
		case TNM_FULLSCREEN_MODE_FIT:		ci.rad_fullscreen_disp_fit.set_check(true);		break;		// 全画面
		case TNM_FULLSCREEN_MODE_UDLRFIT:	ci.rad_fullscreen_disp_udlrfit.set_check(true);	break;		// 全画面フィット
		case TNM_FULLSCREEN_MODE_UDFIT:		ci.rad_fullscreen_disp_udfit.set_check(true);	break;		// 上下フィット
		case TNM_FULLSCREEN_MODE_LRFIT:		ci.rad_fullscreen_disp_lrfit.set_check(true);	break;		// 左右フィット
		case TNM_FULLSCREEN_MODE_CUSTOM:	ci.rad_fullscreen_disp_custom.set_check(true);	break;		// カスタム
	}
}

// ****************************************************************
// フルスクリーン表示モードのラジオボタンを取得
// ================================================================
int C_cfg_wnd_func_screen::get_radio_fullscreen_disp()
{
	if (!h_wnd)	{	return -1;	}
	if (ci.rad_fullscreen_disp_normal.get_check())			{	return TNM_FULLSCREEN_MODE_NORMAL;	}
	else if (ci.rad_fullscreen_disp_fit.get_check())		{	return TNM_FULLSCREEN_MODE_FIT;		}
	else if (ci.rad_fullscreen_disp_udlrfit.get_check())	{	return TNM_FULLSCREEN_MODE_UDLRFIT;	}
	else if (ci.rad_fullscreen_disp_udfit.get_check())		{	return TNM_FULLSCREEN_MODE_UDFIT;	}
	else if (ci.rad_fullscreen_disp_lrfit.get_check())		{	return TNM_FULLSCREEN_MODE_LRFIT;	}
	else if (ci.rad_fullscreen_disp_custom.get_check())		{	return TNM_FULLSCREEN_MODE_CUSTOM;	}
	return -1;
}

// ****************************************************************
// フルスクリーン表示アイテムの有効・禁止
// ================================================================
void C_cfg_wnd_func_screen::set_fullscreen_item_enable()
{
	bool flag;

	// ウィンドウサイズのアイテム
	flag = (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW) ? true : false;
	if (work.window_size_item_enable_flag != flag)	{
		work.window_size_item_enable_flag = flag;
//		ci.grp_window_size.set_enable(flag);
//		ci.rad_window_size_050.set_enable(flag);
//		ci.rad_window_size_075.set_enable(flag);
//		ci.rad_window_size_100.set_enable(flag);
//		ci.rad_window_size_150.set_enable(flag);
//		ci.rad_window_size_200.set_enable(flag);
	}

	// フルスクリーン解像度のアイテム
	flag = Gp_global->dialog_fullscreen_change_resolution;
	if (work.fullscreen_resolution_item_enable_flag != flag)	{
		work.fullscreen_resolution_item_enable_flag = flag;
		ci.btn_change_resolution.set_enable(flag);
		ci.stc_fullscreen_display.set_enable(flag);
		ci.stc_fullscreen_resolution.set_enable(flag);
		ci.cmb_fullscreen_display.set_enable(flag);
		ci.cmb_fullscreen_resolution.set_enable(flag);
	}

	// フルスクリーンモードのアイテム
	flag = (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL) ? true : false;
	if (work.fullscreen_mode_item_enable_flag != flag)	{
		work.fullscreen_mode_item_enable_flag = flag;
		ci.grp_fullscreen_detail.set_enable(flag);
		ci.grp_fullscreen_disp.set_enable(flag);
		ci.rad_fullscreen_disp_normal.set_enable(flag);
		ci.rad_fullscreen_disp_fit.set_enable(flag);
		ci.rad_fullscreen_disp_udlrfit.set_enable(flag);
		ci.rad_fullscreen_disp_udfit.set_enable(flag);
		ci.rad_fullscreen_disp_lrfit.set_enable(flag);
		ci.rad_fullscreen_disp_custom.set_enable(flag);
	}

	// フルスクリーンのカスタム用アイテム
	flag = (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL) ? true : false;
	if (!ci.rad_fullscreen_disp_custom.get_check())	{	flag = false;	}
	if (work.fullscreen_custom_item_enable_flag != flag)	{
		work.fullscreen_custom_item_enable_flag = flag;
		ci.grp_fullscreen_custom.set_enable(flag);
		ci.stc_fullscreen_custom_scale_ud.set_enable(flag);
		ci.btn_fullscreen_custom_scale_ud.set_enable(flag);
		ci.stc_fullscreen_custom_scale_ud_val.set_enable(flag);
		ci.sld_fullscreen_custom_scale_ud.set_enable(flag);
		ci.stc_fullscreen_custom_scale_lr.set_enable(flag);
		ci.btn_fullscreen_custom_scale_lr.set_enable(flag);
		ci.stc_fullscreen_custom_scale_lr_val.set_enable(flag);
		ci.sld_fullscreen_custom_scale_lr.set_enable(flag);
		ci.chk_fullscreen_custom_scale.set_enable(flag);
		ci.stc_fullscreen_custom_move_ud.set_enable(flag);
		ci.btn_fullscreen_custom_move_ud.set_enable(flag);
		ci.stc_fullscreen_custom_move_ud_val.set_enable(flag);
		ci.sld_fullscreen_custom_move_ud.set_enable(flag);
		ci.stc_fullscreen_custom_move_lr.set_enable(flag);
		ci.btn_fullscreen_custom_move_lr.set_enable(flag);
		ci.stc_fullscreen_custom_move_lr_val.set_enable(flag);
		ci.sld_fullscreen_custom_move_lr.set_enable(flag);
	}
}

// ****************************************************************
// 画面サイズに応じて画面モードボタンを有効・禁止
// ================================================================
void C_cfg_wnd_func_screen::set_screen_mode_btn_enable()
{
	// プライマリディスプレイのデフォルトの解像度を取得
	int default_resolution_no = Gp_system_info->resolution_table[0].default_resolution_no;
	C_size default_resolution = Gp_system_info->resolution_table[0].resolution_list[default_resolution_no].resolution;
	C_size window_size, client_size;

	// ★横はクライアントサイズ、縦はウィンドウサイズで判定します。

	// 50%（必ず使える）
	//window_size = tnm_get_window_size_from_size_mode(0);
	//client_size = tnm_get_screen_size_from_size_mode(0);
	//ci.rad_window_size_050.set_enable((client_size.cx <= default_resolution.cx && window_size.cy <= default_resolution.cy) ? true : false);

	// デバッグモード時は全部使える
	//if (Gp_global->debug_flag)	{
	//	ci.rad_window_size_075.set_enable(true);
	//	ci.rad_window_size_100.set_enable(true);
	//	ci.rad_window_size_150.set_enable(true);
	//	ci.rad_window_size_200.set_enable(true);
	//}
	//else	{

		// 75%
		window_size = tnm_get_window_size_from_size_mode(1);
		client_size = tnm_get_screen_size_from_size_mode(1);
		ci.rad_window_size_075.set_enable((client_size.cx <= default_resolution.cx && window_size.cy <= default_resolution.cy) ? true : false);

		// 100%
		window_size = tnm_get_window_size_from_size_mode(2);
		client_size = tnm_get_screen_size_from_size_mode(2);
		ci.rad_window_size_100.set_enable((client_size.cx <= default_resolution.cx && window_size.cy <= default_resolution.cy) ? true : false);

		// 150%
		window_size = tnm_get_window_size_from_size_mode(3);
		client_size = tnm_get_screen_size_from_size_mode(3);
		ci.rad_window_size_150.set_enable((client_size.cx <= default_resolution.cx && window_size.cy <= default_resolution.cy) ? true : false);

		// 200%
		window_size = tnm_get_window_size_from_size_mode(4);
		client_size = tnm_get_screen_size_from_size_mode(4);
		ci.rad_window_size_200.set_enable((client_size.cx <= default_resolution.cx && window_size.cy <= default_resolution.cy) ? true : false);
	//}
}

// ****************************************************************
// 画面サイズを変更した場合にアイテムを禁止状態にする
// ================================================================
void C_cfg_wnd_func_screen::change_screen_mode_func()
{
//	Gp_global->screen_size_mode_change_start_time = ::timeGetTime();
//	enable_screen_mode_item(false);

	// ↑トラブルが多いので消しました。

	// キーボードでラジオボタンを操作した場合、
	//		フォーカスが移動する → WM_COMMAND → 選択が移動する
	// となります。WM_COMMAND で「禁止状態」にすると、元の選択が消えないのに次が選択され、
	// 結果、両方のラジオが選択されるという状態になってしまうようです。
	// XP、7 両方で再現。なんでやねん。
}

// ****************************************************************
// 画面サイズを変更するアイテムを有効・禁止
// ================================================================
void C_cfg_wnd_func_screen::enable_screen_mode_item(bool flag)
{
	ci.rad_screen_mode_fullscreen.set_enable(flag);
	ci.rad_screen_mode_window.set_enable(flag);
	ci.rad_window_size_050.set_enable(flag);
	ci.rad_window_size_075.set_enable(flag);
	ci.rad_window_size_100.set_enable(flag);
	ci.rad_window_size_150.set_enable(flag);
	ci.rad_window_size_200.set_enable(flag);
}

// ****************************************************************
// 画面サイズを変更するアイテムの有効・禁止を設定する
// ================================================================
void C_cfg_wnd_func_screen::set_screen_mode_item()
{
	DWORD past_time = ::timeGetTime() - Gp_global->screen_size_mode_change_start_time;
	if (past_time >= 500)	{
		ci.rad_screen_mode_fullscreen.set_enable(true);
		ci.rad_screen_mode_window.set_enable(true);
		ci.rad_window_size_050.set_enable(true);	// 50 のボタンは set_screen_mode_btn_enable() の中で設定されないのでここで処理する
		set_screen_mode_btn_enable();
	}
	else	{
		enable_screen_mode_item(false);
	}
}

// ****************************************************************
// 現在の解像度モードを取得
// ================================================================
int C_cfg_wnd_func_screen::get_now_resolution_mode()
{
	if (!(0 <= Gp_global->dialog_fullscreen_display_no && Gp_global->dialog_fullscreen_display_no < (int)Gp_system_info->display_list.size()))	{	return -1;	}

	// 現在の解像度を取得する
	DEVMODE dm = {0};
	dm.dmSize = sizeof(DEVMODE);
	if (EnumDisplaySettingsEx(MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->dialog_fullscreen_display_no].DeviceName).c_str(), ENUM_CURRENT_SETTINGS, &dm, EDS_RAWMODE) == 0)	{	return -1;	}

	ARRAY<S_tnm_resolution>& resolution_list = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].resolution_list;
	for (int i = 0; i < (int)resolution_list.size(); i++)	{
		if (resolution_list[i].resolution.cx == dm.dmPelsWidth && resolution_list[i].resolution.cy == dm.dmPelsHeight)	{
			return i;
		}
	}

	return -1;
}

// ****************************************************************
// 解像度のコンボボックスのリストを作成
// ================================================================
void C_cfg_wnd_func_screen::make_resolution_combobox_list()
{
	// リストクリア
	ci.cmb_fullscreen_resolution.clear_item_list();

	if (!(0 <= Gp_global->dialog_fullscreen_display_no && Gp_global->dialog_fullscreen_display_no < (int)Gp_system_info->display_list.size()))
		return;

	ARRAY<S_tnm_resolution>& resolution_list = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].resolution_list;

	// リスト作成
	for (int i =0; i < (int)resolution_list.size(); i++)
		ci.cmb_fullscreen_resolution.insert_item(tostr(resolution_list[i].resolution.cx) + _T(" × ") + tostr(resolution_list[i].resolution.cy), i, false);

	// 現在の選択状態を反映
	ci.cmb_fullscreen_resolution.set_sel_no(Gp_global->dialog_fullscreen_resolution_no);
}

// ****************************************************************
// 解像度のコンボボックスをドロップダウンした
// ================================================================
void C_cfg_wnd_func_screen::dropdown_resolution_combobox()
{
	if (!(0 <= Gp_global->dialog_fullscreen_display_no && Gp_global->dialog_fullscreen_display_no < (int)Gp_system_info->display_list.size()))
		return;

	int now_mode = get_now_resolution_mode();
	int default_mode = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].default_resolution_no;

	ARRAY<S_tnm_resolution> resolution_list = Gp_system_info->resolution_table[Gp_global->dialog_fullscreen_display_no].resolution_list;
	TSTR resolution_str;
	for (int i = 0; i < (int)resolution_list.size(); i++)	{
		resolution_str = tostr(resolution_list[i].resolution.cx) + _T(" × ") + tostr(resolution_list[i].resolution.cy);
		if (i == now_mode)	{
			ci.cmb_fullscreen_resolution.set_item_text(i, resolution_str + _T(" - ") + LCL_STR_SCREEN_DIALOG_CURRENT_RESOLUTION_STR);
		}
		else if (i == default_mode)	{
			ci.cmb_fullscreen_resolution.set_item_text(i, resolution_str + _T(" - ") + LCL_STR_SCREEN_DIALOG_DEFAULT_RESOLUTION_STR);
		}
		else	{
			ci.cmb_fullscreen_resolution.set_item_text(i, resolution_str);
		}
	}

	ci.cmb_fullscreen_resolution.set_sel_no(Gp_global->dialog_fullscreen_resolution_no);
}

// ****************************************************************
// テキスト情報の更新
// ================================================================
void C_cfg_wnd_func_screen::update_text_info()
{
	if (!h_wnd)
		return;

	ci.stc_fullscreen_custom_scale_ud_val.set_text(tostr(Gp_config->fullscreen_scale.cy) + _T(" %"));
	ci.stc_fullscreen_custom_scale_lr_val.set_text(tostr(Gp_config->fullscreen_scale.cx) + _T(" %"));
	ci.stc_fullscreen_custom_move_ud_val.set_text(tostr(Gp_config->fullscreen_move.cy) + _T(" %"));
	ci.stc_fullscreen_custom_move_lr_val.set_text(tostr(Gp_config->fullscreen_move.cx) + _T(" %"));
}

