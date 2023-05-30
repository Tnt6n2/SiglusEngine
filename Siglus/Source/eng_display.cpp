#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_icon.h"

#include	"engine/eng_system.h"
#include	"engine/eng_display.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_editbox.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_emote.h"

#include	"localize/localize.h"

// ****************************************************************
// スクリーンサイズを変更開始処理
// ================================================================
static int G_change_screen_semaphore = 0;

bool tnm_change_screen_size_start(bool draw_black)
{
	// 誰かが処理を実行中なので処理をキャンセル
	// この関数内の Sleep 中に、環境設定ダイアログの「ウィンドウ」ボタンを押したとき用の処理です。
	if (G_change_screen_semaphore > 0)
		return false;

	// 簡易セマフォを立てます。
	G_change_screen_semaphore ++;

	// エディットボックスを全て隠します。（復帰処理はフレーム処理でやってくれます。）
	tnm_editbox_set_hide_all();

	if (draw_black)	{

		// 画面を黒にクリアします。
		Gp_wnd->m_view.clear_render_target();

		// 再描画をかけます。
		Gp_wnd->invalidate_rect(NULL, false);
		G_app.default_message_loop();
	}

	// E-mote を解放します。
#if __USE_EMOTE
	tnm_emote_on_device_lost();
#endif

	// Direct3D エフェクトを一時解放します。
	if (!Gp_wnd->m_view.m_default_effect.on_lost_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("エフェクトの解放に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// Direct3D テキストを一時解放します。
	if (!G_debug_text.on_lost_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("テキストの解放に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// Direct3D リソースを一時解放します。
	G_rsm.release();

	return true;
}

// ****************************************************************
// スクリーンサイズを変更終了処理
// ================================================================
bool tnm_change_screen_size_end_sub()
{
	// 再描画をかけます。

	// ★処理をコメントアウトしました。（2010/4/23）
	// ここで WM_CLOSE が処理されると、デバイスの復元に失敗して落ちます。
	//Gp_wnd->invalidate_rect(NULL, false);
	//Gapp.default_message_loop();

	// Direct3D スクリーンサイズを設定します。
	if (!G_d3d.device.change_screen_size(Gp_global->total_screen_size))
		return false;

	// Direct3D デバイスが復元するまで繰り返す
	while (1)	{

		// デバイスを復元します。
		if (!G_d3d.device.restore())
			throw _T("デバイスの復元に失敗しました。\n") + get_last_error_msg();		// 致命的エラー！

		// デバイスが消失している場合は再度繰り返します。
		if (G_d3d.device.is_lost())	{
			Sleep(100);
			continue;
		}

		// 成功
		break;
	}

	// Direct3D ステートを初期化します。
	G_d3d.state.init();

	// Direct3D ステートを初期設定を行います。
	if (!G_d3d.state.set_template())
		return false;

	// Direct3D リソースを復元します。
	if (!G_rsm.resume())
		return false;

	// Direct3D テキストを復元します。
	if (!G_debug_text.on_reset_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("テキストの復元に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// Direct3D エフェクトを復元します。
	if (!Gp_wnd->m_view.m_default_effect.on_reset_device())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("エフェクトの復元に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 警告
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★Direct3D デバイスを復元しました。"));

	return true;
}

bool tnm_change_screen_size_end()
{
	bool res = tnm_change_screen_size_end_sub();

	// 簡易セマフォを降ろします。
	G_change_screen_semaphore --;

	return res;
}

// ****************************************************************
// 環境設定のスクリーンモードを補正する
// ================================================================
void tnm_correct_fullscreen_resolution()
{
	// 範囲チェック
	if (Gp_config->fullscreen_display_no < 0 || (int)Gp_system_info->display_list.size() <= Gp_config->fullscreen_display_no)	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_no < 0 || (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size() <= Gp_config->fullscreen_resolution_no)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}

	// 値を更新する
	Gp_config->fullscreen_display_cnt = (int)Gp_system_info->display_list.size();
	Gp_config->fullscreen_resolution_cnt = (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size();
	Gp_config->fullscreen_resolution = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list[Gp_config->fullscreen_resolution_no].resolution;
}

// ****************************************************************
// スクリーンサイズモードをウィンドウに変更する
// ================================================================
//		window_pos: ウィンドウモードの場合のみ指定（NULL で中央）
//		first: 起動時
// ================================================================
bool tnm_update_screen_size_mode_window_sub_sub(C_point* window_pos, bool first)
{
	// ウィンドウスタイルを変更します。
	Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	Gp_wnd->set_style(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME);
	Gp_wnd->set_top_most(false);

	// スクリーンサイズ補正
	tnm_rep_screen_size(TNM_SCREEN_SIZE_MODE_WINDOW, &Gp_config->screen_size_scale, &Gp_config->screen_size_free);

	// スクリーンサイズを求めます。
	C_size screen_size;
	screen_size.cx = Gp_global->game_screen_size.cx * Gp_config->screen_size_scale.cx / 100;
	screen_size.cy = Gp_global->game_screen_size.cy * Gp_config->screen_size_scale.cy / 100;

	// ウィンドウサイズを設定します。
	Gp_wnd->set_client_size(screen_size);

	// ウィンドウ位置を指定します。
	// 指定がない場合は、ウィンドウが存在するモニタの中央に配置します。
	// 起動時は ini にあわせるのでこれを行いません。

	if (first)	{}	// NOP
	else if (window_pos)	{
		Gp_wnd->set_window_pos(*window_pos);
	}
	else	{
		Gp_wnd->set_pos_my_desktop_center();
	}

	// 成功すればスクリーンサイズを設定します。
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_global->screen_size_mode_window = Gp_config->screen_size_mode_window;
	Gp_global->screen_size_scale = Gp_config->screen_size_scale;
	Gp_global->screen_size_free = Gp_config->screen_size_free;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// 必ず再取得！変わっていることがあります。
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// 必ず再取得！変わっていることがあります。

	return true;
}

bool tnm_update_screen_size_mode_window_sub(C_point* window_pos, bool first)
{
	// 解像度を変更していた場合、元に戻す
	if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
		ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
	}

	// スクリーンサイズを変更
	tnm_update_screen_size_mode_window_sub_sub(window_pos, first);

	return true;
}

// ****************************************************************
// スクリーンサイズモードをフリーに変更する
// ================================================================
//		window_pos: ウィンドウモードの場合のみ指定（NULL で中央）
//		first: 起動時
// ================================================================
bool tnm_update_screen_size_mode_free_sub_sub(C_point* window_pos, bool first)
{
	// ウィンドウスタイルを変更します。
	Gp_wnd->set_menu(Gp_wnd->m_wnd_menu.get_handle());
	Gp_wnd->set_style(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME);
	Gp_wnd->set_top_most(false);

	// スクリーンサイズ補正
	tnm_rep_screen_size(TNM_SCREEN_SIZE_MODE_FREE, &Gp_config->screen_size_scale, &Gp_config->screen_size_free);

	// ウィンドウサイズを設定します。
	Gp_wnd->set_client_size(Gp_config->screen_size_free);

	// ウィンドウ位置を指定します。
	// 指定がない場合は、ウィンドウが存在するモニタの中央に配置します。
	// 起動時は ini にあわせるのでこれを行いません。

	if (first)	{}	// NOP
	else if (window_pos)	{
		Gp_wnd->set_window_pos(*window_pos);
	}
	else	{
		Gp_wnd->set_pos_my_desktop_center();
	}

	// 成功すればスクリーンサイズを設定します。
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_global->screen_size_mode_window = Gp_config->screen_size_mode_window;
	Gp_global->screen_size_scale = Gp_config->screen_size_scale;
	Gp_global->screen_size_free = Gp_config->screen_size_free;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// 必ず再取得！変わっていることがあります。
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// 必ず再取得！変わっていることがあります。

	return true;
}

bool tnm_update_screen_size_mode_free_sub(C_point* window_pos, bool first)
{
	// 解像度を変更していた場合、元に戻す
	if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
	{
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
		ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
	}

	// スクリーンサイズを変更
	tnm_update_screen_size_mode_free_sub_sub(window_pos, first);

	return true;
}

// ****************************************************************
// スクリーンサイズモードをフルスクリーンに変更する
// ================================================================
//		first: 起動時
// ================================================================
bool tnm_update_screen_size_mode_fullscreen_sub_sub(C_point* window_pos, bool first)
{
	// ウィンドウスタイルを変更します。
	Gp_wnd->set_menu(NULL);
	Gp_wnd->set_style(WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN);

#ifndef _DEBUG	// プログラム側デバッグ版では、フルスクリーンにしても最前面に移動しないようにする
//	Gp_wnd->set_top_most(true);		// 最前面に移動
#endif

	C_rect client_rect = Gp_wnd->get_client_rect();
	C_rect window_rect = Gp_wnd->client_to_screen(client_rect);

	// モニタの矩形を取得します。
	C_rect desktop_rect;
	if (window_pos)
		get_desktop_rect_from_pos(&desktop_rect, *window_pos);	// 指定の位置のモニタ矩形を取得
	else
		Gp_wnd->get_my_desktop_rect(&desktop_rect);				// モニタの中央から矩形を取得

	// クライアント矩形を設定します。
	Gp_wnd->set_client_rect(desktop_rect);

	// 成功したのでシステムデータを更新します。
	Gp_global->screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	Gp_global->fullscreen_change_resolution = Gp_config->fullscreen_change_resolution;
	Gp_global->fullscreen_display_no = Gp_config->fullscreen_display_no;
	Gp_global->fullscreen_resolution_no = Gp_config->fullscreen_resolution_no;
	Gp_global->total_screen_size = Gp_wnd->get_client_rect().size();	// 必ず再取得！変わっていることがあります。
	Gp_global->total_window_size = Gp_wnd->get_window_rect().size();	// 必ず再取得！変わっていることがあります。

	// ダイアログに反映させる
	Gp_global->dialog_fullscreen_change_resolution = Gp_global->fullscreen_change_resolution;
	Gp_global->dialog_fullscreen_display_no = Gp_global->fullscreen_display_no;
	Gp_global->dialog_fullscreen_resolution_no = Gp_global->fullscreen_resolution_no;

	return true;
}

bool tnm_update_screen_size_mode_fullscreen_sub(C_point* window_pos, bool first, bool warning)
{
	// １．仮想フルスクリーン
	if (!Gp_config->fullscreen_change_resolution)	{

		// 解像度を変更していた場合、元に戻す
		if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
			if (Gp_global->fullscreen_change_resolution)	{
				if (Gp_global->fullscreen_display_no >= 0)	{
					TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
					ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("解像度を元に戻しました。（") + desplay_name + _T("）"));
				}
			}
		}

		// 単純にフルスクリーンにする
		tnm_update_screen_size_mode_fullscreen_sub_sub(window_pos, first);
	}

	// ２．解像度を変更する
	else	{

		int display_no = Gp_config->fullscreen_display_no;
		int resolution_no = Gp_config->fullscreen_resolution_no;
		C_size resolution = Gp_config->fullscreen_resolution;

		if (display_no < 0 || (int)Gp_system_info->resolution_table.size() <= display_no)
			return false;

		// 現在の状態を記憶しておく（復元用）
		int old_screen_size_mode = Gp_global->screen_size_mode;
		int old_screen_size_mode_window = Gp_global->screen_size_mode_window;
		C_size old_screen_size_scale = Gp_global->screen_size_scale;
		C_size old_screen_size_free = Gp_global->screen_size_free;
		C_point old_window_pos = Gp_wnd->get_window_rect().left_top();
		bool old_change_resolution = Gp_global->fullscreen_change_resolution;
		int old_display_no = Gp_global->fullscreen_display_no;
		int old_resolution_no = Gp_global->fullscreen_resolution_no;

		// デフォルトの解像度を取得する
		TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[display_no].DeviceName);
		int default_resolution_no = Gp_system_info->resolution_table[display_no].default_resolution_no;
		C_size default_resolution = Gp_system_info->resolution_table[display_no].resolution_list[default_resolution_no].resolution;

		// 現在の解像度を取得する
		DEVMODE old_dm = {0};
		old_dm.dmSize = sizeof(DEVMODE);
		if (EnumDisplaySettingsEx(desplay_name.c_str(), ENUM_CURRENT_SETTINGS, &old_dm, EDS_RAWMODE) == 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("現在の解像度が取得できませんでした。"));
			return false;
		}

		// 新しい解像度を指定
		DEVMODE new_dm = {0};
		new_dm.dmSize = sizeof(new_dm);
		new_dm.dmPelsWidth = resolution.cx;
		new_dm.dmPelsHeight = resolution.cy;
		new_dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

		// 解像度が変われば警告を出す
		if (warning && (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight))	{

			int res = tnm_game_message_box(LCL_STR_CHANGE_RESOLUTION_WARNING_STR, MB_OKCANCEL | MB_DEFBUTTON2);
			if (res == IDCANCEL)	{

				// 解像度を元に戻す
				Gp_config->screen_size_mode = old_screen_size_mode;
				Gp_config->screen_size_mode_window = old_screen_size_mode_window;
				Gp_config->screen_size_scale = old_screen_size_scale;
				Gp_config->screen_size_free = old_screen_size_free;
				Gp_config->fullscreen_change_resolution = old_change_resolution;
				Gp_config->fullscreen_display_no = old_display_no;
				Gp_config->fullscreen_resolution_no = old_resolution_no;
				tnm_correct_fullscreen_resolution();	// 値を補正する

				return true;
			}
		}

		// 違うディスプレイを選んだ場合、解像度を元に戻す
		if (Gp_global->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
			if (Gp_global->fullscreen_change_resolution)	{
				if (Gp_global->fullscreen_display_no >= 0 && display_no != Gp_global->fullscreen_display_no)	{
					TSTR desplay_name = MBSTR_to_TSTR(Gp_system_info->display_list[Gp_global->fullscreen_display_no].DeviceName);
					ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL);
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("解像度を元に戻しました。（") + desplay_name + _T("）"));
				}
			}
		}

		// 解像度が変われば変更する
		if (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight)	{

			if (new_dm.dmPelsWidth == default_resolution.cx && new_dm.dmPelsHeight == default_resolution.cy)	{

				if (ChangeDisplaySettingsEx(desplay_name.c_str(), NULL, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("解像度の変更に失敗しました。"));
					return false;
				}

				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("解像度を変更しました。（デフォルト）") + desplay_name + _T("）"));
			}
			else	{

				if (ChangeDisplaySettingsEx(desplay_name.c_str(), &new_dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("解像度の変更に失敗しました。"));
					return false;
				}

				C_size resolution = C_size(new_dm.dmPelsWidth, new_dm.dmPelsHeight);
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("解像度を変更しました。（") + tostr(resolution) + _T("）") + desplay_name + _T("）"));
			}
		}

		// 新しい解像度について詳細を受け取る
		if (EnumDisplaySettingsEx(desplay_name.c_str(), ENUM_CURRENT_SETTINGS, &new_dm, EDS_RAWMODE) == 0)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("新しい解像度が再取得できませんでした。"));
			return false;
		}

		// 指定のモニタでフルスクリーンにする
		tnm_update_screen_size_mode_fullscreen_sub_sub(&C_point(new_dm.dmPosition.x, new_dm.dmPosition.y), first);

		// 解像度が変わっているときのみ確認ダイアログを出す
		// いいえを選択すると解像度を元に戻せる
		if (warning && (new_dm.dmPelsWidth != old_dm.dmPelsWidth || new_dm.dmPelsHeight != old_dm.dmPelsHeight))	{

			// ここで抜けて一旦描画に入ります。
			// 次のフレームで確認ダイアログ → 「いいえ」なら元に戻す処理を行います。

			// ↓処理を行うための準備
			Gp_global->change_display_mode_proc_cnt = 2;	// ２フレームまわす（１フレーム目＝描画、２フレーム目で確認ダイアログ）
			Gp_global->change_display_mode_proc_old_screen_size_mode = old_screen_size_mode;
			Gp_global->change_display_mode_proc_old_screen_size_mode_window = old_screen_size_mode_window;
			Gp_global->change_display_mode_proc_old_screen_size_scale = old_screen_size_scale;
			Gp_global->change_display_mode_proc_old_screen_size_free = old_screen_size_free;
			Gp_global->change_display_mode_proc_old_window_pos = old_window_pos;
			Gp_global->change_display_mode_proc_old_change_resolution = old_change_resolution;
			Gp_global->change_display_mode_proc_old_display_no = old_display_no;
			Gp_global->change_display_mode_proc_old_resolution_no = old_resolution_no;

			// ゲームタイマーを進めない
			Gp_global->alt_menu_wait_flag = true;
		}
	}

	return true;
}

// ****************************************************************
// スクリーンサイズモードを変更する
// ================================================================
//		起動時、もしくは解像度変更を元に戻すときに使っています。
//		ウィンドウ位置を指定できます。
// ================================================================
bool tnm_update_screen_size_mode_sub(C_point* window_pos, bool first, bool warning)
{
	// ウィンドウ／フルスクリーンを適用
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)	{
		return tnm_update_screen_size_mode_window_sub(window_pos, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)	{
		return tnm_update_screen_size_mode_free_sub(window_pos, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		return tnm_update_screen_size_mode_fullscreen_sub(window_pos, first, warning);
	}

	return false;
}

// ****************************************************************
// スクリーンサイズモードを変更：呼び出し側
// ================================================================
//		開始 → 変更 → 終了　の流れを行います。
// ================================================================

// ウィンドウサイズが変わったときの処理（強制）
void tnm_change_screen_size_mode_by_force(bool first)
{
	// 解像度変更中は処理しない
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))
		return;

	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)	{
		tnm_update_screen_size_mode_window_sub(NULL, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)	{
		tnm_update_screen_size_mode_free_sub(NULL, first);
	}
	else if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)	{
		tnm_update_screen_size_mode_fullscreen_sub(NULL, first, false);
	}

	tnm_change_screen_size_end();
}

// 「ウィンドウ」ボタンを押したときの処理（ウィンドウまたはフリーになります）
void tnm_change_screen_size_mode_window(bool first)
{
	// 解像度変更中は処理しない
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// 現在と同じ値なら処理しない
	if (Gp_config->screen_size_mode == Gp_config->screen_size_mode_window && Gp_config->screen_size_scale == Gp_global->screen_size_scale)
		return;

	// 環境設定の値を更新する
	Gp_config->screen_size_mode = Gp_config->screen_size_mode_window;
	Gp_config->screen_size_scale = Gp_config->screen_size_scale;

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))	{ return; }

	switch (Gp_config->screen_size_mode)
	{
	case TNM_SCREEN_SIZE_MODE_WINDOW:
		tnm_update_screen_size_mode_window_sub(NULL, first);
		break;
	case TNM_SCREEN_SIZE_MODE_FREE:
		tnm_update_screen_size_mode_free_sub(NULL, first);
		break;
	}

	tnm_change_screen_size_end();
}

// 「50%」などのボタンを押したときの処理（ウィンドウになります）
void tnm_change_screen_size_mode_window(C_size screen_size_scale, bool first)
{
	// 解像度変更中は処理しない
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// 現在と同じ値なら処理しない
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW && Gp_config->screen_size_scale == screen_size_scale)
		return;

	// 環境設定の値を更新する
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_config->screen_size_mode_window = TNM_SCREEN_SIZE_MODE_WINDOW;
	Gp_config->screen_size_scale = screen_size_scale;

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_window_sub(NULL, first);
	tnm_change_screen_size_end();
}

// 「フルスクリーン」ボタンを押したときの処理
void tnm_change_screen_size_mode_fullscreen(bool first, bool warning)
{
	// 解像度変更中は処理しない
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// 現在と同じ値なら処理しない
	if (
		Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL && 
		Gp_config->fullscreen_change_resolution == Gp_global->dialog_fullscreen_change_resolution && 
		Gp_config->fullscreen_display_no == Gp_global->dialog_fullscreen_display_no && 
		Gp_config->fullscreen_resolution_no == Gp_global->dialog_fullscreen_resolution_no
		)
	{
		return;
	}

	// 環境設定の値を更新する
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_FULL;
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW || Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
	{
		Gp_config->screen_size_mode_window = Gp_config->screen_size_mode;
	}
	Gp_config->fullscreen_change_resolution = Gp_global->dialog_fullscreen_change_resolution;
	Gp_config->fullscreen_display_no = Gp_global->dialog_fullscreen_display_no;
	Gp_config->fullscreen_resolution_no = Gp_global->dialog_fullscreen_resolution_no;
	tnm_correct_fullscreen_resolution();	// 値を補正する

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_fullscreen_sub(NULL, first, warning);	// モニタの中心を基準にフルスクリーンにする
	tnm_change_screen_size_end();
}

// 「Alt+Enter」を押したときの処理
void tnm_switch_screen_size_mode(bool first)
{
	// フルスクリーンならウィンドウに
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
	{
		tnm_change_screen_size_mode_window(first);
	}
	// ウィンドウならフルスクリーンに
	else
	{
		tnm_change_screen_size_mode_fullscreen(first, true);
	}
}

// ウィンドウサイズを変更したときの処理
void tnm_change_screen_size_mode_free(C_point window_pos, C_size screen_size_free, bool first)
{
	// 解像度変更中は処理しない
	if (Gp_global->change_display_mode_proc_cnt)
		return;

	// 現在と同じ値なら処理しない
	if (Gp_config->screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE && Gp_config->screen_size_free == screen_size_free)
		return;

	// 環境設定の値を更新する
	Gp_config->screen_size_mode = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_config->screen_size_mode_window = TNM_SCREEN_SIZE_MODE_FREE;
	Gp_config->screen_size_free = screen_size_free;

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))	{ return; }
	tnm_update_screen_size_mode_free_sub(&window_pos, first);
	tnm_change_screen_size_end();
}

// ****************************************************************
// スクリーンモードを更新
// ================================================================
//		ゲーム初期化時はここを通ります。
//		前回の起動からディスプレイの個数などが変わった場合は変更を初期化します。
// ================================================================
void tnm_update_screen_size_mode()
{
	if (Gp_config->fullscreen_display_cnt != (int)Gp_system_info->display_list.size())	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_display_no < 0 || (int)Gp_system_info->display_list.size() <= Gp_config->fullscreen_display_no)	{
		Gp_config->fullscreen_display_no = 0;
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_cnt != (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size())	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution_no < 0 || (int)Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list.size() <= Gp_config->fullscreen_resolution_no)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}
	if (Gp_config->fullscreen_resolution != Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].resolution_list[Gp_config->fullscreen_resolution_no].resolution)	{
		Gp_config->fullscreen_resolution_no = Gp_system_info->resolution_table[Gp_config->fullscreen_display_no].default_resolution_no;
	}

	// 値を更新する
	tnm_correct_fullscreen_resolution();	// 値を補正する

	// 環境設定の値を適用する
	if (!tnm_change_screen_size_start(true))	{	return;	}
	tnm_update_screen_size_mode_sub(NULL, true, true);
	tnm_change_screen_size_end();
}

// ****************************************************************
// スクリーンサイズを元に戻す
// ================================================================
//		解像度変更前に記憶した状態に戻します。
// ================================================================
class Cchange_resolution_warning_dialog : public C_modal_dialog
{
public:
	Cc_static	stc_text;
	Cc_button	btn_ok;
	Cc_button	btn_cancel;

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
	bool		on_init_dialog();
	bool		on_close();
	bool		on_command(WPARAM wp, LPARAM lp);
	bool		on_timer();

private:
	int			rest_cnt;
};

LRESULT Cchange_resolution_warning_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_TIMER:			if (!on_timer())			return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

bool Cchange_resolution_warning_dialog::on_init_dialog()
{
	set_text(tnm_get_game_name());
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	bind_control(&stc_text, IDC_STC_CHANGE_RESOLUTION_TEXT);
	bind_control(&btn_ok, IDC_CHANGE_RESOLUTION_OK);
	bind_control(&btn_cancel, IDC_CHANGE_RESOLUTION_CANCEL);

	rest_cnt = 20;	// 20秒で元に戻ります。
	on_timer();		// 最初に on_timer を１度呼んでおく

	::SetTimer(m_h_wnd, 0, 1000, NULL);

	btn_cancel.set_focus();

	return true;
}

bool Cchange_resolution_warning_dialog::on_close()
{
	EndDialog(m_h_wnd, IDCANCEL);

	// デフォルトの動作を実行しない！
	return false;
}

bool Cchange_resolution_warning_dialog::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);

	switch (ctrl_id)	{
		case IDC_CHANGE_RESOLUTION_OK:		EndDialog(m_h_wnd, IDOK);		return false;
		case IDC_CHANGE_RESOLUTION_CANCEL:	EndDialog(m_h_wnd, IDCANCEL);	return false;
		case IDCANCEL:						EndDialog(m_h_wnd, IDCANCEL);	return false;	// ★Esc キーを押すとここを通る
	}

	return true;
}

bool Cchange_resolution_warning_dialog::on_timer()
{
	if (rest_cnt == 0)	{
		EndDialog(m_h_wnd, IDCANCEL);
		return false;
	}

	TSTR str = LCL_STR_CHANGE_RESOLUTION_AFTER_STR(rest_cnt);
		
	stc_text.set_text(str);

	rest_cnt--;

	return true;
}

void tnm_reset_screen_size_mode()
{
	// 全てのダイアログに対してラジオボタンのフォーカスをはずす
	// メッセージボックスが出る前に処理する必要があります。
	tnm_dlg_kill_focus_for_all_radio();

	tnm_dlg_set_enable(false);		// ★ダイアログ操作禁止

	// 解像度を変更した通知を出す
	Cchange_resolution_warning_dialog dlg;
	int res = dlg.create(LCL_IDD_CHANGE_RESOLUTION_WARNING, _T("change_resolution_warning"), false, G_app.h_wnd);
	if (res == IDCANCEL)	{

		// 解像度を元に戻す
		Gp_config->screen_size_mode = Gp_global->change_display_mode_proc_old_screen_size_mode;
		Gp_config->screen_size_mode_window = Gp_global->change_display_mode_proc_old_screen_size_mode_window;
		Gp_config->screen_size_scale = Gp_global->change_display_mode_proc_old_screen_size_scale;
		Gp_config->screen_size_free = Gp_global->change_display_mode_proc_old_screen_size_free;
		C_point window_pos = Gp_global->change_display_mode_proc_old_window_pos;
		Gp_config->fullscreen_change_resolution = Gp_global->change_display_mode_proc_old_change_resolution;
		Gp_config->fullscreen_display_no = Gp_global->change_display_mode_proc_old_display_no;
		Gp_config->fullscreen_resolution_no = Gp_global->change_display_mode_proc_old_resolution_no;
		tnm_correct_fullscreen_resolution();	// 値を補正する

		// 環境設定の値を適用する
		if (!tnm_change_screen_size_start(true))	{	return;	}
		tnm_update_screen_size_mode_sub(&window_pos, false, false);
		tnm_change_screen_size_end();
	}

	tnm_dlg_set_enable(true);		// ★ダイアログ操作有効
}

// ****************************************************************
// スクリーンサイズ補正
// ================================================================
bool tnm_rep_screen_size(int screen_size_mode, C_size* screen_size_scale, C_size* screen_size_free)
{
	// フルスクリーンモードは関係ない
	if (screen_size_mode == TNM_SCREEN_SIZE_MODE_FULL)
		return false;

	if (screen_size_mode == TNM_SCREEN_SIZE_MODE_WINDOW)
	{
		return tnm_rep_screen_size_window(screen_size_scale);
	}
	else if (screen_size_mode == TNM_SCREEN_SIZE_MODE_FREE)
	{
		return tnm_rep_screen_size_free(screen_size_free);
	}

	return false;
}

bool tnm_rep_screen_size_window(C_size* screen_size_scale)
{
	// スクリーンサイズ比率をモードに変換する（規定外の場合、１００％に補正する）
	bool change_flag = false;
	int size_mode = tnm_format_screen_size_to_screen_mode(*screen_size_scale);
	if (size_mode == -1)	// 規定外の場合、１００％に補正する
	{
		screen_size_scale->cx = 100;
		screen_size_scale->cy = 100;
		size_mode = 2;
		change_flag = true;
	}

	// デスクトップのスクリーンサイズ内に納まるかチェックする（納まらなければ、サイズを下げていきます）
	int display_w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int display_h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	C_size window_size, client_size;
	while (size_mode)
	{
		window_size = tnm_get_window_size_from_size_mode(size_mode);
		client_size = tnm_get_screen_size_from_size_mode(size_mode);
		if (client_size.cx <= display_w && window_size.cy <= display_h)
			break;	// ★横はクライアントサイズ、縦はウィンドウサイズで判定します。
		change_flag = true;
		size_mode--;
	}

	// サイズが変わったので値を補正する
	if (change_flag)
		tnm_format_screen_mode_to_screen_size_scale(size_mode, screen_size_scale);

	return change_flag;
}

bool tnm_rep_screen_size_free(C_size* screen_size_free)
{
	bool change_flag = false;

	// ▲とりあえず何もしない…。

	return change_flag;
}

// ****************************************************************
// スクリーンサイズ比率からスクリーンモードに変換（取得？）
// ================================================================
int tnm_format_screen_size_to_screen_mode(C_size screen_size_scale)
{
	if (screen_size_scale.cx == 50 && screen_size_scale.cy == 50)			{	return 0;	}
	else if (screen_size_scale.cx == 75 && screen_size_scale.cy == 75)		{	return 1;	}
	else if (screen_size_scale.cx == 100 && screen_size_scale.cy == 100)	{	return 2;	}
	else if (screen_size_scale.cx == 150 && screen_size_scale.cy == 150)	{	return 3;	}
	else if (screen_size_scale.cx == 200 && screen_size_scale.cy == 200)	{	return 4;	}
	return -1;		// 規定外は -1 が戻ります。
}

// ****************************************************************
// スクリーンモードからスクリーンサイズ比率に変換
// ================================================================
void tnm_format_screen_mode_to_screen_size_scale(int size_mode, C_size* screen_size_scale)
{
	switch (size_mode)	{
		case 0:		// 50%
			screen_size_scale->cx = 50;
			screen_size_scale->cy = 50;
			break;
		case 1:		// 75%
			screen_size_scale->cx = 75;
			screen_size_scale->cy = 75;
			break;
		case 2:		// 100%
			screen_size_scale->cx = 100;
			screen_size_scale->cy = 100;
			break;
		case 3:		// 150%
			screen_size_scale->cx = 150;
			screen_size_scale->cy = 150;
			break;
		case 4:		// 200%
			screen_size_scale->cx = 200;
			screen_size_scale->cy = 200;
			break;
	}
}

// ****************************************************************
// スクリーンサイズモードからスクリーンサイズを取得
// ================================================================
C_size tnm_get_screen_size_from_size_mode(int size_mode)
{
	C_size screen_size = Gp_global->game_screen_size;

	switch (size_mode)	{
		case 0:		// 50%
			return screen_size / 2;
		case 1:		// 75%
			return screen_size / 2 + screen_size / 4;
		case 2:		// 100%
			return screen_size;
		case 3:		// 150%
			return screen_size * 3 / 2;
		case 4:		// 200%
			return screen_size * 2;
	}
	return screen_size;
}

// ****************************************************************
// スクリーンサイズモードからウィンドウサイズを取得
// ================================================================
C_size tnm_get_window_size_from_size_mode(int size_mode)
{
	// スクリーンサイズを求める
	C_size client_size = tnm_get_screen_size_from_size_mode(size_mode);

	// ウィンドウサイズに変換する
	C_rect window_rect = C_rect(0, 0, client_size.cx, client_size.cy);
	::AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME, Gp_global->debug_flag ? TRUE : FALSE);
	return window_rect.size();
}

// ****************************************************************
// スクリーンサイズが使えるかどうかを判定
// ================================================================
bool tnm_get_window_size_from_size_scale(C_size client_size_scale)
{
	C_size client_size = Gp_global->game_screen_size;
	client_size.cx = client_size.cx * client_size_scale.cx / 100;
	client_size.cy = client_size.cy * client_size_scale.cy / 100;

	// ウィンドウサイズに変換する
	C_rect window_rect = C_rect(0, 0, client_size.cx, client_size.cy);
	::AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE | WS_THICKFRAME, Gp_global->debug_flag ? TRUE : FALSE);

	int default_resolution_no = Gp_system_info->resolution_table[0].default_resolution_no;
	C_size default_resolution = Gp_system_info->resolution_table[0].resolution_list[default_resolution_no].resolution;
	return (client_size.cx <= default_resolution.cx && window_rect.height() <= default_resolution.cy) ? 1 : 0;
}

// ****************************************************************
// 垂直同期を待つ設定の変更処理
// ================================================================
void tnm_set_wait_display_vsync_sub(bool wait_display_vsync_flag)
{
	G_d3d.device.change_present_interval(wait_display_vsync_flag ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE);
}

void tnm_set_wait_display_vsync(bool wait_display_vsync_flag)
{
	if (Gp_global->wait_display_vsync_total == wait_display_vsync_flag)
		return;

	// 垂直同期を待つ設定を変更する
	if (tnm_change_screen_size_start(false))	{
		tnm_set_wait_display_vsync_sub(wait_display_vsync_flag);
		tnm_change_screen_size_end();

		// 成功したのでシステムデータを更新する
		Gp_global->wait_display_vsync_total = wait_display_vsync_flag;
	}
}
