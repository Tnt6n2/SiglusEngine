#include	"pch.h"

#include	"tnm_common_header.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_rmenu.h"

#include	"engine/eng_msgbox.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// ＩＤ定義
// ================================================================

// ★定義値が１０００を超えると、デバッグメニューのシステムコマンドメニューに問題が起こります。ご注意下さい。

const int	IDM_SYSCOMMENU_CANCEL			= 0;			// ★項目を選択しなかった場合も 0 が帰るので、キャンセルは 0 固定です。
const int	IDM_SYSCOMMENU_READ_SKIP		= 1;
const int	IDM_SYSCOMMENU_AUTO_SKIP		= 2;
const int	IDM_SYSCOMMENU_AUTO_MODE		= 3;
const int	IDM_SYSCOMMENU_HIDE_MWND		= 4;
const int	IDM_SYSCOMMENU_SAVE				= 5;
const int	IDM_SYSCOMMENU_LOAD				= 6;
const int	IDM_SYSCOMMENU_RETURN_SEL		= 7;
const int	IDM_SYSCOMMENU_CONFIG			= 8;
const int	IDM_SYSCOMMENU_MANUAL			= 9;
const int	IDM_SYSCOMMENU_VERSION			= 10;
const int	IDM_SYSCOMMENU_RETURN_MENU		= 11;
const int	IDM_SYSCOMMENU_GAME_END			= 12;

const int	IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000		= 100;		// メニューの最大項目数が４個なので 100～103 を使用します（2009/10/21）
const int	IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_0000		= 200;		// メニューの最大項目数が４個なので 200～203 を使用します（2009/10/21）
const int	IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000	= 300;		// メニューの最大項目数が４個でアイテムの最大数が８個なので 300～307, 308～315, 316～323, 324～331 を使用します（2009/10/21）

// ****************************************************************
// システムコマンドメニュー：開く
// ================================================================
bool tnm_rmenu_open()
{
	C_popup_menu menu;

	// メニューを作成
	tnm_rmenu_create(menu);

	// 選択開始
	return tnm_rmenu_select(menu);
}

// ****************************************************************
// システムコマンドメニュー：作成
// ================================================================
bool tnm_rmenu_create(C_popup_menu& menu)
{
	// メニューを作成
	menu.create();

	return tnm_rmenu_create_funk(menu);
}

// ****************************************************************
// システムコマンドメニュー：作成ファンクション
// ================================================================
bool tnm_rmenu_create_funk(C_menu& menu, int id_rep)
{
	bool exist_once_flag = false;

	// ▲▲▲システムコマンドメニューの項目の並びはこれでいいのか精査する事。

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (tnm_syscom_read_skip_get_exist_flag() || tnm_syscom_auto_skip_get_exist_flag() || tnm_syscom_auto_mode_get_exist_flag() || tnm_syscom_hide_mwnd_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// 既読文章を早送り
	if (tnm_syscom_read_skip_get_exist_flag())	{
		// 未読スキップがＯＮの場合
		if (Gp_config->system.skip_unread_message_flag)	{
			menu.add_item(IDM_SYSCOMMENU_READ_SKIP + id_rep, Gp_ini->syscommenu.unread_skip.str, tnm_syscom_read_skip_is_enable(), tnm_syscom_read_skip_get_onoff_flag());
		}
		else	{
			menu.add_item(IDM_SYSCOMMENU_READ_SKIP + id_rep, Gp_ini->syscommenu.read_skip.str, tnm_syscom_read_skip_is_enable(), tnm_syscom_read_skip_get_onoff_flag());
		}
	}

	// 自動早送り
	if (tnm_syscom_auto_skip_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_AUTO_SKIP + id_rep, Gp_ini->syscommenu.auto_skip.str, tnm_syscom_auto_skip_is_enable(), tnm_syscom_auto_skip_get_onoff_flag());
	}

	// オートモード
	if (tnm_syscom_auto_mode_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_AUTO_MODE + id_rep, Gp_ini->syscommenu.auto_mode.str, tnm_syscom_auto_mode_is_enable(), tnm_syscom_auto_mode_get_onoff_flag());
	}

	// ウィンドウを隠す
	if (tnm_syscom_hide_mwnd_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_HIDE_MWND + id_rep, Gp_ini->syscommenu.hide_mwnd.str, tnm_syscom_hide_mwnd_is_enable(), tnm_syscom_hide_mwnd_get_onoff_flag());
	}

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (tnm_syscom_open_save_get_exist_flag() || tnm_syscom_open_load_get_exist_flag() || tnm_syscom_return_to_sel_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// セーブ
	if (tnm_syscom_open_save_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_SAVE + id_rep, Gp_ini->syscommenu.save.str, tnm_syscom_open_save_is_enable()) ;
	}

	// ロード
	if (tnm_syscom_open_load_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_LOAD + id_rep, Gp_ini->syscommenu.load.str, tnm_syscom_open_load_is_enable()) ;
	}

	// 前の選択肢に戻る
	if (tnm_syscom_return_to_sel_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_RETURN_SEL + id_rep, Gp_ini->syscommenu.return_to_sel.str, tnm_syscom_return_to_sel_is_enable());
	}

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (tnm_syscom_open_config_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// 環境設定
	if (tnm_syscom_open_config_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_CONFIG + id_rep, Gp_ini->syscommenu.config.str, tnm_syscom_open_config_get_enable_flag(), tnm_dlg_is_config_dialog_opened());
	}

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	// ローカル汎用スイッチ／モードがあるかどうかを判定
	bool local_switch_exist_flag = false;
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < Gp_ini->syscommenu.local_extra_switch_cnt; i++)	{
			if (tnm_syscom_local_extra_switch_get_exist_flag(i))	{
				local_switch_exist_flag = true;
				break;
			}
		}
	}
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		for (int i = 0; i < (int)Gp_ini->syscommenu.local_extra_mode_cnt; i++)	{
			if (tnm_syscom_local_extra_mode_get_exist_flag(i))	{
				local_switch_exist_flag = true;
				break;
			}
		}
	}

	// 存在すればセパレータを入れる
	if (local_switch_exist_flag)	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// ローカル汎用スイッチ
	if (Gp_ini->syscommenu.local_extra_switch_cnt > 0)	{
		for (int i = 0; i < Gp_ini->syscommenu.local_extra_switch_cnt; i++)	{
			if (tnm_syscom_local_extra_switch_get_exist_flag(i))	{
				menu.add_item(IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 + i + id_rep, Gp_ini->syscommenu.local_extra_switch[i].str, tnm_syscom_local_extra_switch_is_enable(i), tnm_syscom_local_extra_switch_get_onoff_flag(i));
			}
		}
	}

	// ローカル汎用モード
	if (Gp_ini->syscommenu.local_extra_mode_cnt > 0)	{
		C_menu sub_menu;
		for (int i = 0; i < (int)Gp_ini->syscommenu.local_extra_mode_cnt; i++)	{
			if (tnm_syscom_local_extra_mode_get_exist_flag(i))	{
				sub_menu.create();
				if (tnm_syscom_local_extra_mode_get_enable_flag(i))	{
					for (int j = 0; j < Gp_ini->syscommenu.local_extra_mode[i].item_cnt; j++)	{
						int value = tnm_syscom_local_extra_mode_get_value(i);
						sub_menu.add_item(IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 + (i * TNM_LOCAL_EXTRA_MODE_ITEM_CNT) + j + id_rep, Gp_ini->syscommenu.local_extra_mode[i].item[j].str, true, (j == value) ? true : false);
					}
				}
				menu.add_sub_menu(IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_0000 + i + id_rep, Gp_ini->syscommenu.local_extra_mode[i].str, sub_menu.get_handle(), tnm_syscom_local_extra_mode_get_enable_flag(i));
			}
		}
	}

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (tnm_syscom_manual_get_exist_flag() || tnm_syscom_version_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// マニュアル
	if (tnm_syscom_manual_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_MANUAL + id_rep, Gp_ini->syscommenu.manual.str, true);
	}

	// バージョン情報
	if (tnm_syscom_version_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_VERSION + id_rep, Gp_ini->syscommenu.version.str, true);
	}

	// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (tnm_syscom_return_to_menu_get_exist_flag() || tnm_syscom_end_game_get_exist_flag() || tnm_syscom_cancel_get_exist_flag())	{
		if (exist_once_flag)
			menu.add_separator();	// セパレータ
		exist_once_flag = true;
	}

	// メニューに戻る
	if (tnm_syscom_return_to_menu_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_RETURN_MENU + id_rep, Gp_ini->syscommenu.return_to_menu.str, tnm_syscom_return_to_menu_get_enable_flag());
	}

	// ゲームを終了する
	if (tnm_syscom_end_game_get_exist_flag())	{
		menu.add_item(IDM_SYSCOMMENU_GAME_END + id_rep, Gp_ini->syscommenu.game_end.str, tnm_syscom_end_game_get_enable_flag());
	}

	// キャンセル
	if (tnm_syscom_cancel_get_exist_flag() || !exist_once_flag)	{
		menu.add_item(IDM_SYSCOMMENU_CANCEL + id_rep, Gp_ini->syscommenu.cancel.str, tnm_syscom_cancel_get_enable_flag());
	}
	return true;
}

// ****************************************************************
// システムコマンドメニュー：選択開始
// ================================================================
bool tnm_rmenu_select(C_popup_menu& menu)
{
	int res = 0;

	// メニューが開いている間は時間を止める！
	if (!Gp_global->alt_menu_wait_flag)
		Gp_global->alt_menu_wait_flag = true;

	// メニューを開き選択開始
	C_point mp = get_mouse_pos();	// ここだけは独自でマウス座標を取り出す
	int syscom_id = menu.select(Gp_wnd->get_handle(), mp.x, mp.y);

	// メニューを開いたので１フレームだけ入力を禁止する
	Gp_global->skip_click_1_frame = true;

	// 処理実行
	return tnm_rmenu_execute(syscom_id);
}

// ****************************************************************
// システムコマンドメニュー：処理実行
// ================================================================
bool tnm_rmenu_execute(int syscom_id)
{
	switch (syscom_id)	{

		// キャンセル
		case IDM_SYSCOMMENU_CANCEL:
			break;

		// 既読スキップ
		case IDM_SYSCOMMENU_READ_SKIP:
			tnm_syscom_read_skip_set_onoff_flag(!tnm_syscom_read_skip_get_onoff_flag());
			break;

		// 自動早送り
		case IDM_SYSCOMMENU_AUTO_SKIP:
			tnm_syscom_auto_skip_set_onoff_flag(!tnm_syscom_auto_skip_get_onoff_flag());
			break;

		// オートモード
		case IDM_SYSCOMMENU_AUTO_MODE:
			tnm_syscom_auto_mode_set_onoff_flag(!tnm_syscom_auto_mode_get_onoff_flag());
			break;

		// メッセージウィンドウを隠す
		case IDM_SYSCOMMENU_HIDE_MWND:
			tnm_syscom_hide_mwnd_set_onoff_flag(!tnm_syscom_hide_mwnd_get_onoff_flag());
			break;

		// セーブ
		case IDM_SYSCOMMENU_SAVE:
			tnm_syscom_open_save(false, true, true);		// （★セーブ：右クリックメニュー）ダイアログが呼ばれる場合はキャプチャーする
			break;

		// ロード
		case IDM_SYSCOMMENU_LOAD:
			tnm_syscom_open_load(false, true);
			break;

		// 前の選択肢に戻る
		case IDM_SYSCOMMENU_RETURN_SEL:
			tnm_syscom_return_to_sel(true, true, true);		// 警告あり
			break;

		// 環境設定
		case IDM_SYSCOMMENU_CONFIG:
			tnm_syscom_open_config();
			break;

		// マニュアル
		case IDM_SYSCOMMENU_MANUAL:
		{
			if (!Gp_ini->manual_path_str.empty())	{
				shell_execute_open(Gp_dir->base_dir + _T("\\") + Gp_ini->manual_path_str);
			}
			break;
		}

		// バージョン情報
		case IDM_SYSCOMMENU_VERSION:
			tnm_game_info_box(Gp_ini->game_name + _T(" ") + Gp_ini->game_version_str + _T("\nSiglus Ver.") + SIGLUS_VERSION);
			break;

		// メニューに戻る
		case IDM_SYSCOMMENU_RETURN_MENU:
			tnm_syscom_return_to_menu(true, true, true, false);		// 警告あり
			break;

		// ゲームを終了する
		case IDM_SYSCOMMENU_GAME_END:
			tnm_syscom_end_game(true, false, false);				// 警告のみあり
			break;

		default:

			if (false);

			// ローカル汎用スイッチ
			else if (IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 <= syscom_id && syscom_id < IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000 + (int)Gp_ini->syscommenu.local_extra_switch_cnt)	{
				int no = syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_SWITCH_0000;
				tnm_syscom_local_extra_switch_turn_onoff_flag(no);
			}

			// ローカル汎用モード
			else if (IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 <= syscom_id && syscom_id < IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000 + ((int)Gp_ini->syscommenu.local_extra_mode_cnt * TNM_LOCAL_EXTRA_MODE_ITEM_CNT))	{
				int value = (syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000) % TNM_LOCAL_EXTRA_MODE_ITEM_CNT;
				int no = (syscom_id - IDM_SYSCOMMENU_LOCAL_EXTRA_MODE_ITEM_0000) / TNM_LOCAL_EXTRA_MODE_ITEM_CNT;
				tnm_syscom_local_extra_mode_set_value(no, value);
			}

			break;
	}
	return true;
}
