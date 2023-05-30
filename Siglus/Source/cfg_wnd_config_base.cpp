#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"dialog/cfg_wnd.h"
#include	"dialog/cfg_wnd_config_base.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ベース
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_config_base")

// 初期化
void C_cfg_wnd_config_base::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_BASE);
	cfg_wnd_full_mode_flag = false;		// ★ false は通常の環境設定モードです。
	init_child_window();
}

// ウィンドウプロシージャ
LRESULT C_cfg_wnd_config_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
		case WM_MOUSEWHEEL:		if (!on_mousewheel(wp, lp))	return FALSE;	break;

		// ラジオボタンのフォーカスを外す為の処理
		case WM_ACTIVATE:
			if (LOWORD(wp) == WA_INACTIVE)	{
				int tab_no = ci.tab_base.get_selected_item_no();
				if (0 <= tab_no && tab_no < (int)child_window_list.size())	{
					child_window_list[tab_no]->kill_focus_for_radio_item();
					kill_focus_for_radio_item();
				}
			}
			break;

#if 0
		case WM_CTLCOLORDLG:
			{
//				HDC hdc = GetDC((HWND)lp);
//				SetBkMode(hdc, TRANSPARENT);
//				ReleaseDC((HWND)lp, hdc);
//				SetBkMode((HDC)wp, TRANSPARENT);
//				return (LRESULT)GetSysColorBrush(COLOR_3DFACE);
//				return (LRESULT)GetSysColorBrush(COLOR_3DHIGHLIGHT);
				return (LRESULT)GetStockObject(WHITE_BRUSH);
//				return (LRESULT)0;
			}
			break;
		case WM_CTLCOLORSTATIC:
			{
//				SetBkMode((HDC)wp, TRANSPARENT);
//				return (LRESULT)GetSysColorBrush(COLOR_3DHIGHLIGHT);
				return (LRESULT)GetStockObject(WHITE_BRUSH);
			}
			break;
#endif

	}

	return C_tnm_dlg::window_proc(msg, wp, lp);
}

// 初期化
bool C_cfg_wnd_config_base::on_init()
{
	// ステータスバーのサイズグリップ作成
	statusbar.create_sizegrip(m_h_wnd);

	// アイテム登録
	bind_control(&ci.tab_base, IDC_TAB_BASE, _T("TAB_BASE"), 5);
	bind_control(&ci.stc_game_version, IDC_STC_GAME_VERSION, _T(""), 1);
	bind_control(&ci.btn_all_config_init, IDC_BTN_ALL_CONFIG_INIT, _T(""), 3);
	bind_control(&ci.btn_close, IDC_BTN_CLOSE, _T(""), 3);

	// 子ウィンドウを登録（ウィンドウリスト作成）（タブを追加）
	regist_child_window();

	// タブに親のクラスポインターを設定
	ci.tab_base.parent_class = this;

	// 現在選択されているタブの設定
	int tab_no = ci.tab_base.load_ini_int(_T("TAB_SELECT_NO"), 0);
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;	}
	ci.tab_base.set_selected_item_no(tab_no);

	// バージョン情報
	ci.stc_game_version.set_text_adjust_width(Gp_ini->game_version_str, 20);	// 20 くらい余裕を持たせる

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// ウィンドウタイトル
	if (cfg_wnd_full_mode_flag)	{
		set_text(_T("全環境設定（デバッグ用）"));
	}

	// ウィンドウ表示
	update();
	set_show(true);

	// 子ウィンドウを開く
	open_child_window();

	return true;
}

// 閉じる
bool C_cfg_wnd_config_base::on_close()
{
	// 全ての子ウィンドウを閉じる
	close_all_child_window();

	// 子ウィンドウリスト開放
	child_window_list.clear();

	// アイテム設定セーブ
	ci.tab_base.save_ini_int(_T("TAB_SELECT_NO"), ci.tab_base.get_selected_item_no());

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// コマンド
bool C_cfg_wnd_config_base::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_ALL_CONFIG_INIT:
			// 子の「初期状態に戻す」を呼び出す
			cfg_wnd_base_screen.cfg_wnd_func_screen.init_state();
			cfg_wnd_base_volume_bgmfade.cfg_wnd_func_volume.init_state();
			cfg_wnd_base_volume_bgmfade.cfg_wnd_func_bgmfade.init_state();
			cfg_wnd_base_volume.cfg_wnd_func_volume.init_state();
			cfg_wnd_base_font_msgspd.cfg_wnd_func_font.init_state();
			cfg_wnd_base_font_msgspd.cfg_wnd_func_msgspd.init_state();
			cfg_wnd_base_mwndbk.cfg_wnd_func_mwndbk.init_state();
			cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_koemode.init_state();
			cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_chrkoe.init_state();
			cfg_wnd_base_koemode.cfg_wnd_func_koemode.init_state();
			cfg_wnd_base_chrkoe.cfg_wnd_func_chrkoe.init_state();
			cfg_wnd_base_automode.cfg_wnd_func_automode.init_state();
			cfg_wnd_base_jitan.cfg_wnd_func_jitan.init_state();
			cfg_wnd_base_else.cfg_wnd_func_else.init_state();
			cfg_wnd_base_system.cfg_wnd_func_system.init_state();
			break;
		case IDC_BTN_CLOSE:
			close();
			break;
	}
	return true;
}

// 通知
bool C_cfg_wnd_config_base::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	// タブコントロール
	if (hdr->hwndFrom == ci.tab_base.get_handle())	{
		switch (hdr->code)	{
			case TCN_SELCHANGE:
				open_child_window();
				break;
		}
	}

	return true;
}

// マウスホイール
bool C_cfg_wnd_config_base::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (child_window_list.empty())
		return true;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))
		return true;

	// タブにフォーカスがある時で、音声のときはキャラクターリストをスクロールさせる
	if (child_window_list[tab_no] == &cfg_wnd_base_koemode_chrkoe && GetFocus() == ci.tab_base.get_handle())	{
		cfg_wnd_base_koemode_chrkoe.cfg_wnd_func_chrkoe.on_mousewheel_character_list(wp, lp);
		cfg_wnd_base_chrkoe.cfg_wnd_func_chrkoe.on_mousewheel_character_list(wp, lp);
		return false;
	}

	return true;
}

// フレーム
void C_cfg_wnd_config_base::frame()
{
	if (!m_h_wnd)
		return;

	// 子のフレーム
	cfg_wnd_base_screen.frame();					// ベース：画面モード
	cfg_wnd_base_font_msgspd.frame();				// ベース：メッセージ（フォント、文字速度）
	cfg_wnd_base_volume_bgmfade.frame();			// ベース：音量（音量、ＢＧＭフェード）
	cfg_wnd_base_koemode_chrkoe.frame();			// ベース：音声（音声モード、キャラクター音声）
	cfg_wnd_base_koemode.frame();					// ベース：音声モード
	cfg_wnd_base_chrkoe.frame();					// ベース：キャラクター音声
	cfg_wnd_base_volume.frame();					// ベース：音量
	cfg_wnd_base_mwndbk.frame();					// ベース：ウィンドウ背景色
	cfg_wnd_base_automode.frame();					// ベース：オートモード
	cfg_wnd_base_jitan.frame();						// ベース：時短再生
	cfg_wnd_base_else.frame();						// ベース：その他
	cfg_wnd_base_system.frame();					// ベース：システム
}

// ****************************************************************
// 子ウィンドウを初期化
// ================================================================
void C_cfg_wnd_config_base::init_child_window()
{
	all_child_window_list.push_back(&cfg_wnd_base_screen);			// ベース：画面モード
	all_child_window_list.push_back(&cfg_wnd_base_font_msgspd);		// ベース：メッセージ（フォント、文字速度）
	all_child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	// ベース：音量（音量、ＢＧＭフェード）
	all_child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	// ベース：音声（音声モード、キャラクター音声）
	all_child_window_list.push_back(&cfg_wnd_base_koemode);			// ベース：音声モード
	all_child_window_list.push_back(&cfg_wnd_base_chrkoe);			// ベース：キャラクター音声
	all_child_window_list.push_back(&cfg_wnd_base_volume);			// ベース：音量
	all_child_window_list.push_back(&cfg_wnd_base_mwndbk);			// ベース：ウィンドウ背景色
	all_child_window_list.push_back(&cfg_wnd_base_automode);		// ベース：オートモード
	all_child_window_list.push_back(&cfg_wnd_base_jitan);			// ベース：時短再生
	all_child_window_list.push_back(&cfg_wnd_base_else);			// ベース：その他
	all_child_window_list.push_back(&cfg_wnd_base_system);			// ベース：システム

	for (int i = 0; i < (int)all_child_window_list.size(); i++)	{
		all_child_window_list[i]->init();											// 初期化
		all_child_window_list[i]->cfg_wnd_full_mode_flag = cfg_wnd_full_mode_flag;	// フルモードの設定
	}
}	

// ****************************************************************
// 子ウィンドウを登録
// ================================================================
void C_cfg_wnd_config_base::regist_child_window()
{
	// 全環境設定（デバッグ用）
	if (cfg_wnd_full_mode_flag)
	{
		child_window_list.push_back(&cfg_wnd_base_screen);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SCREEN_STR);
		child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);
		child_window_list.push_back(&cfg_wnd_base_font_msgspd);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_MESSAGE_STR);
		child_window_list.push_back(&cfg_wnd_base_mwndbk);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR);
		child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);
		child_window_list.push_back(&cfg_wnd_base_automode);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR);
		child_window_list.push_back(&cfg_wnd_base_jitan);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_JITAN_STR);
		child_window_list.push_back(&cfg_wnd_base_else);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_OTHERS_STR);
		child_window_list.push_back(&cfg_wnd_base_system);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SYSTEM_STR);
	}
	// 環境設定
	else
	{
		if (Gp_ini->config.tab_exist_screen)	{
			child_window_list.push_back(&cfg_wnd_base_screen);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SCREEN_STR);
		}
		if (Gp_ini->config.tab_exist_volume)	{
			switch (Gp_ini->config.dlg_style_volume)	{
				case 0:		child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
				case 1:		child_window_list.push_back(&cfg_wnd_base_volume);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
				default:	child_window_list.push_back(&cfg_wnd_base_volume_bgmfade);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOLUME_STR);	break;
			}
		}
		if (Gp_ini->config.tab_exist_message)	{
			child_window_list.push_back(&cfg_wnd_base_font_msgspd);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_MESSAGE_STR);
		}
		if (Gp_ini->config.tab_exist_mwndbk)	{
			child_window_list.push_back(&cfg_wnd_base_mwndbk);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_BACKGROUND_COLOR_STR);
		}
		if (Gp_ini->config.tab_exist_koe)	{
			switch (Gp_ini->config.dlg_style_koe)	{
				case 0:		child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				case 1:		child_window_list.push_back(&cfg_wnd_base_koemode);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				case 2:		child_window_list.push_back(&cfg_wnd_base_chrkoe);			ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
				default:	child_window_list.push_back(&cfg_wnd_base_koemode_chrkoe);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_VOICE_STR);	break;
			}
		}
		if (Gp_ini->config.tab_exist_automode)	{
			child_window_list.push_back(&cfg_wnd_base_automode);	ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_AUTO_MODE_STR);
		}
		if (Gp_ini->config.tab_exist_jitan)	{
			child_window_list.push_back(&cfg_wnd_base_jitan);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_JITAN_STR);
		}
		if (Gp_ini->config.tab_exist_else)	{
			child_window_list.push_back(&cfg_wnd_base_else);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_OTHERS_STR);
		}
		if (Gp_ini->config.tab_exist_system)	{
			child_window_list.push_back(&cfg_wnd_base_system);		ci.tab_base.add_item(LCL_STR_CONFIG_DIALOG_SYSTEM_STR);
		}
	}
}

// ****************************************************************
// 子ウィンドウを開く
// ================================================================
void C_cfg_wnd_config_base::open_child_window()
{
	if (child_window_list.empty())
		return;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;		ci.tab_base.set_selected_item_no(tab_no);	}

	child_window_list[tab_no]->open(m_h_wnd);
	adjust_child_window();
	close_all_child_window(child_window_list[tab_no]);
}

// ****************************************************************
// 全ての子ウィンドウを閉じる
// ================================================================
void C_cfg_wnd_config_base::close_all_child_window(C_cfg_wnd_solo* no_close_window)
{
	if (child_window_list.empty())
		return;

	for (int i = 0; i < (int)child_window_list.size(); i++)	{
		if (child_window_list[i] != no_close_window)	{
			child_window_list[i]->close();
		}
	}
}

// ****************************************************************
// 子ウィンドウを調整する
// ================================================================
void C_cfg_wnd_config_base::adjust_child_window()
{
	if (child_window_list.empty())
		return;

	int tab_no = ci.tab_base.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < (int)child_window_list.size()))	{	tab_no = 0;		ci.tab_base.set_selected_item_no(tab_no);	}

	ARRAY<C_cfg_wnd_solo *>::iterator itr = child_window_list.begin() + tab_no;
	C_rect rect = screen_to_client(ci.tab_base.get_display_rect());	// タブの表示領域を求める
	rect.left -= 2;		// 微調整
	child_window_list[tab_no]->set_window_rect(rect);
}

// ****************************************************************
// タブベースのウィンドウプロシージャ
// ================================================================
LRESULT C_cfg_wnd_config_base::C_tab_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SIZE:
		case WM_MOVE:
			parent_class->adjust_child_window();
			break;
	}
	return Cc_tab::window_proc(msg, wp, lp);
}

