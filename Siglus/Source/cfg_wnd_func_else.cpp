#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_else.h"
#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：その他
// ================================================================

// 初期化
void Ccfg_wnd_func_else::on_init_funcfunc()
{
	// アイテム設定
	setup_item();

	// アイテムの設定

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void Ccfg_wnd_func_else::on_close_funcfunc()
{
}

// コマンド
bool Ccfg_wnd_func_else::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	if ((HWND)lp == ci.chk_message_chrcolor.get_handle())	{
		Gp_config->message_chrcolor_flag = ci.chk_message_chrcolor.get_check();
		return true;
	}
	for (int i = 0; i < TNM_OBJECT_DISP_CNT; i++)	{
		if ((HWND)lp == ci.chk_object_disp[i].get_handle())	{
			Gp_config->object_disp_flag[i].value = ci.chk_object_disp[i].get_check();
			return true;
		}
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		if ((HWND)lp == ci.chk_global_extra_switch[i].get_handle())	{
			Gp_config->global_extra_switch_flag[i].value = ci.chk_global_extra_switch[i].get_check();
			return true;
		}
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_CNT; i++)	{
		for (int j = 0; j < TNM_GLOBAL_EXTRA_MODE_ITEM_CNT; j++)	{
			if ((HWND)lp == ci.rad_global_extra_mode[i][j].get_handle())	{
				Gp_config->global_extra_mode_flag[i] = j;
				return true;
			}
		}
	}
	switch (LOWORD(wp))	{
		case IDC_BTN_ELSE_INIT:
			init_state();
			return true;

		case IDC_BTN_ELSE_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// 通知
bool Ccfg_wnd_func_else::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool Ccfg_wnd_func_else::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool Ccfg_wnd_func_else::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool Ccfg_wnd_func_else::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT Ccfg_wnd_func_else::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void Ccfg_wnd_func_else::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// 外部で変更された
	if (Gp_config->message_chrcolor_flag != ci.chk_message_chrcolor.get_check())	{
		ci.chk_message_chrcolor.set_check(Gp_config->message_chrcolor_flag);
	}
	for (int i = 0; i < TNM_OBJECT_DISP_CNT; i++)	{
		if (Gp_config->object_disp_flag[i].value != ci.chk_object_disp[i].get_check())	{
			ci.chk_object_disp[i].set_check(Gp_config->object_disp_flag[i].value);
		}
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		if (Gp_config->global_extra_switch_flag[i].value != ci.chk_global_extra_switch[i].get_check())	{
			ci.chk_global_extra_switch[i].set_check(Gp_config->global_extra_switch_flag[i].value);
		}
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_CNT; i++)	{
		if (Gp_config->global_extra_mode_flag[i] != get_radio_mode(i))	{
			set_radio_mode(i);
		}
	}
}

// 初期状態に戻す
void Ccfg_wnd_func_else::init_state()
{
	tnm_init_config_message_chrcolor();			// 文章の色分け
	tnm_init_config_object_disp();				// オブジェクト表示
	tnm_init_config_global_extra_switch();		// グローバル汎用スイッチ
	tnm_init_config_global_extra_mode();		// グローバル汎用モード
	update_dialog();
}

// ダイアログ更新
void Ccfg_wnd_func_else::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.chk_message_chrcolor.set_check(Gp_config->message_chrcolor_flag);
	for (int i = 0; i < TNM_OBJECT_DISP_CNT; i++)	{
		ci.chk_object_disp[i].set_check(Gp_config->object_disp_flag[i].value);
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		ci.chk_global_extra_switch[i].set_check(Gp_config->global_extra_switch_flag[i].value);
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_CNT; i++)	{
		set_radio_mode(i);
	}
}

// ****************************************************************
// アイテム設定
// ================================================================

#define		ITEM_GRP_BASE_X				0
#define		ITEM_GRP_BASE_Y				0

#define		ITEM_CHKBOX_HEIGHT			14
#define		ITEM_CHKBOX_BASE_X			10
#define		ITEM_CHKBOX_BASE_Y			21
#define		ITEM_CHKBOX_Y_SPACE_NORMAL	23
#define		ITEM_CHKBOX_Y_SPACE_SMALL	22
#define		ITEM_CHKBOX_Y_SPACE			item_chkbox_y_space_val
#define		ITEM_CHKBOX_MARGIN_BOTTOM	12

#define		ITEM_RADIO_HEIGHT			14
#define		ITEM_RADIO_BASE_X			10
#define		ITEM_RADIO_BASE_Y			21
#define		ITEM_RADIO_Y_SPACE			21
#define		ITEM_RADIO_MARGIN_BOTTOM	12

#define		ITEM_CHKBOX_ADD_WIDTH		20	// XP クラシックスタイルで 20 ほど必要
#define		ITEM_RADIO_ADD_WIDTH		20	// XP クラシックスタイルで 20 ほど必要
#define		ITEM_GROUP_ADD_WIDTH		16

#define		ITEM_ADD_BOTTOM_HEIGHT		12

#define		ITEM_GROUP_X_SPACE			12
#define		ITEM_GROUP_Y_SPACE			10

#define		ITEM_BUTTON_X_SPACE			6

#define		WINDOW_MARGIN_LEFT			11
#define		WINDOW_MARGIN_RIGHT			10
#define		WINDOW_MARGIN_TOP			11
#define		WINDOW_MARGIN_BOTTOM		8
#define		WINDOW_MARGIN_BOTTOM_INGP	12

#define		BUTTON_MARGIN_RIGHT			11
#define		BUTTON_MARGIN_TOP			12
#define		BUTTON_MARGIN_BOTTOM		9

#define		SIZE_GRIP_HEIGHT			24

void Ccfg_wnd_func_else::setup_item()
{
	// デフォルトのクライアント矩形
	C_rect window_default_rect = p_dlg->get_client_rect();

	// ボタンアイテム：登録（矩形取得）
	p_dlg->bind_control(&ci.btn_else_init, IDC_BTN_ELSE_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.btn_else_close, IDC_BTN_ELSE_CLOSE, _T(""), 3);
	C_rect btn_init_rect = ci.btn_else_init.get_client_rect();
	C_rect btn_close_rect = ci.btn_else_close.get_client_rect();

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_else_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// フォント
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// --------------------------------------------------------------------------------------------

	int x, y, exist_cnt, width;

	// グローバル汎用モードのアイテム
	struct item_struct
	{
		bool exist;
		C_control* ctrl;
	};
	ARRAY<item_struct>	global_extra_mode_item[TNM_GLOBAL_EXTRA_MODE_CNT];
	int global_extra_mode_item_max_width[TNM_GLOBAL_EXTRA_MODE_CNT];
	int global_extra_mode_item_grp_width[TNM_GLOBAL_EXTRA_MODE_CNT];
	int global_extra_mode_item_grp_height[TNM_GLOBAL_EXTRA_MODE_CNT];
	bool global_extra_mode_group_exist[TNM_GLOBAL_EXTRA_MODE_CNT];
	int global_extra_mode_group_exist_cnt = 0;
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_CNT; i++)	{
		// グローバル汎用モードのアイテム：作成
		// グローバル汎用モードのアイテム：ダイアログに追加（アサイン）
		// グローバル汎用モードのアイテム：フォント設定
		// グローバル汎用モードのアイテム：文字列を設定後、テキストサイズに合わせてコントロールの幅を調整する
		// グローバル汎用モードのアイテム：リスト化
		ci.grp_global_extra_mode[i].create(_T(""), _T(""), ITEM_GRP_BASE_X, ITEM_GRP_BASE_Y, 10, 10, false, WS_CHILD | WS_VISIBLE | WS_GROUP | BS_GROUPBOX, 0, false, false, h_wnd);
		p_dlg->add_child(&ci.grp_global_extra_mode[i], 5);
		SendMessage(ci.grp_global_extra_mode[i].get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
		ci.grp_global_extra_mode[i].set_text(Gp_ini->config.global_extra_mode[i].str);									// グループの項目名設定
		global_extra_mode_item[i].resize(TNM_GLOBAL_EXTRA_MODE_ITEM_CNT);
		for (int j = 0; j < TNM_GLOBAL_EXTRA_MODE_ITEM_CNT; j++)	{
			DWORD add_style = (j == 0) ? WS_GROUP : 0;
			ci.rad_global_extra_mode[i][j].create(   _T(""), _T(""), 0, 0, 10, ITEM_RADIO_HEIGHT, false, WS_CHILD | WS_VISIBLE | add_style | WS_TABSTOP | BS_AUTORADIOBUTTON, 0, false, false, h_wnd);
			p_dlg->add_child(&ci.rad_global_extra_mode[i][j], 7);
			ci.rad_global_extra_mode[i][j].set_text_adjust_width(Gp_ini->config.global_extra_mode[i].item[j].str, ITEM_RADIO_ADD_WIDTH);
			SendMessage(ci.rad_global_extra_mode[i][j].get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
			bool groupe_exist = (Gp_ini->config.global_extra_mode[i].exist || full_mode_flag) ? true : false;			// デバッグ用フルモード
			global_extra_mode_item[i][j].exist = (groupe_exist && j < Gp_ini->config.global_extra_mode[i].item_cnt) ? true : false;
			global_extra_mode_item[i][j].ctrl = &ci.rad_global_extra_mode[i][j];
		}

		if (full_mode_flag)	{		// デバッグ用フルモード
			// ★全てを表示すると多すぎるので表示しない
			//for (int j = 0; j < (int)global_extra_mode_item[i].size(); j++)	{
			//	global_extra_mode_item[i][j].exist = true;
			//}
		}

		// グローバル汎用モードのアイテム：使用しないアイテムの削除と座標計算
		x = ITEM_RADIO_BASE_X;
		y = ITEM_RADIO_BASE_Y;
		exist_cnt = 0;
		global_extra_mode_item_max_width[i] = ci.grp_global_extra_mode[i].get_adjust_width_from_text() + ITEM_GROUP_ADD_WIDTH;		// グループの項目幅
		for (int j = 0; j < (int)global_extra_mode_item[i].size(); j++)	{
			if (global_extra_mode_item[i][j].exist)	{
				width = global_extra_mode_item[i][j].ctrl->get_adjust_width_from_text() + ITEM_RADIO_ADD_WIDTH;
				if (global_extra_mode_item_max_width[i] < width)	{	global_extra_mode_item_max_width[i] = width;	}
				y += ITEM_RADIO_Y_SPACE;
				exist_cnt++;
			}
			else	{
				global_extra_mode_item[i][j].ctrl->set_show(false);		// 消去
			}
		}
		// グローバル汎用モードのアイテム：グループの幅算出
		global_extra_mode_item_grp_width[i] = ITEM_RADIO_BASE_X + ITEM_RADIO_BASE_X;
		global_extra_mode_item_grp_height[i] = ITEM_RADIO_BASE_Y + ITEM_RADIO_MARGIN_BOTTOM;
		if (exist_cnt > 0)	{
			global_extra_mode_item_grp_width[i] += global_extra_mode_item_max_width[i];
			global_extra_mode_item_grp_height[i] += ((exist_cnt - 1) * ITEM_RADIO_Y_SPACE) + ITEM_RADIO_HEIGHT;
			global_extra_mode_group_exist[i] = true;
			global_extra_mode_group_exist_cnt++;
		}
		else	{
			ci.grp_global_extra_mode[i].set_show(false);			// 消去
			global_extra_mode_group_exist[i] = false;
		}
	}

	// --------------------------------------------------------------------------------------------

	// グローバル汎用モードの２または３がある場合、その他の設定のチェックボックスの間隔を小さくする
	// ↓この値は、ITEM_CHKBOX_Y_SPACE で定義されている
	int item_chkbox_y_space_val = (global_extra_mode_group_exist[2] || global_extra_mode_group_exist[3]) ? ITEM_CHKBOX_Y_SPACE_SMALL : ITEM_CHKBOX_Y_SPACE_NORMAL;

	// その他のアイテム：作成
	// その他のアイテム：ダイアログに追加（アサイン）
	// その他のアイテム：フォント設定
	// その他のアイテム：文字列を設定後、テキストサイズに合わせてコントロールの幅を調整する
	ci.grp_else.create(_T(""), _T(""), ITEM_GRP_BASE_X, ITEM_GRP_BASE_Y, 10, 10, false, WS_CHILD | WS_VISIBLE | WS_GROUP | BS_GROUPBOX, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.grp_else, 5);
	SendMessage(ci.grp_else.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));	// グループのフォント設定
	ci.grp_else.set_text(_T("その他"));																// グループの項目名設定

	ci.chk_message_chrcolor.create(_T(""), _T(""), 0, 0, 10, ITEM_CHKBOX_HEIGHT, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
	p_dlg->add_child(&ci.chk_message_chrcolor, 7);
	ci.chk_message_chrcolor.set_text_adjust_width(Gp_ini->config.message_chrcolor.str, ITEM_CHKBOX_ADD_WIDTH);
	SendMessage(ci.chk_message_chrcolor.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));

	for (int i = 0; i < TNM_OBJECT_DISP_CNT; i++)	{
		ci.chk_object_disp[i].create(   _T(""), _T(""), 0, 0, 10, ITEM_CHKBOX_HEIGHT, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		p_dlg->add_child(&ci.chk_object_disp[i], 7);
		ci.chk_object_disp[i].set_text_adjust_width(Gp_ini->config.object_disp[i].str, ITEM_CHKBOX_ADD_WIDTH);
		SendMessage(ci.chk_object_disp[i].get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		ci.chk_global_extra_switch[i].create(  _T(""), _T(""), 0, 0, 10, ITEM_CHKBOX_HEIGHT, false, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_AUTOCHECKBOX, 0, false, false, h_wnd);
		p_dlg->add_child(&ci.chk_global_extra_switch[i], 7);
		ci.chk_global_extra_switch[i].set_text_adjust_width(Gp_ini->config.global_extra_switch[i].str, ITEM_CHKBOX_ADD_WIDTH);
		SendMessage(ci.chk_global_extra_switch[i].get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
	}

	// その他のアイテム：リスト化
	ARRAY<item_struct>	else_item;
	else_item.resize(9);
	else_item[0].exist = Gp_ini->config.message_chrcolor.exist;
	else_item[0].ctrl = &ci.chk_message_chrcolor;
	for (int i = 0; i < TNM_OBJECT_DISP_CNT; i++)	{
		else_item[1 + i].exist = (Gp_ini->config.object_disp_cnt > i) ? Gp_ini->config.object_disp[i].exist : false;
		else_item[1 + i].ctrl = &ci.chk_object_disp[i];
	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_SWITCH_CNT; i++)	{
		else_item[5 + i].exist = (Gp_ini->config.global_extra_switch_cnt > i) ? Gp_ini->config.global_extra_switch[i].exist : false;
		else_item[5 + i].ctrl = &ci.chk_global_extra_switch[i];
	}

	if (full_mode_flag)	{		// デバッグ用フルモード
		// ★全てを表示すると多すぎるので表示しない
		//for (int i = 0; i < (int)else_item.size(); i++)	{
		//	else_item[i].exist = true;
		//}
		else_item[0].exist = true;		// 「文章を色分けする」のみ表示
	}

	// その他のアイテム：使用しないアイテムの削除と座標計算
	x = ITEM_CHKBOX_BASE_X;
	y = ITEM_CHKBOX_BASE_Y;
	exist_cnt = 0;
	int else_item_max_width = ci.grp_else.get_adjust_width_from_text() + ITEM_GROUP_ADD_WIDTH;		// グループの項目幅
	for (int i = 0; i < (int)else_item.size(); i++)	{
		if (else_item[i].exist)	{
			width = else_item[i].ctrl->get_adjust_width_from_text() + ITEM_CHKBOX_ADD_WIDTH;
			if (else_item_max_width < width)	{	else_item_max_width = width;	}
			y += ITEM_CHKBOX_Y_SPACE;
			exist_cnt++;
		}
		else	{
			else_item[i].ctrl->set_show(false);		// 消去
		}
	}

	// その他のアイテム：グループの幅算出
	int else_item_grp_width = ITEM_CHKBOX_BASE_X + ITEM_CHKBOX_BASE_X;
	int else_item_grp_height = ITEM_CHKBOX_BASE_Y + ITEM_CHKBOX_MARGIN_BOTTOM;
	bool else_group_exist;
	if (exist_cnt > 0)	{
		else_item_grp_width += else_item_max_width;
		else_item_grp_height += ((exist_cnt - 1) * ITEM_CHKBOX_Y_SPACE) + ITEM_CHKBOX_HEIGHT;
		else_group_exist = true;
	}
	else	{
		ci.grp_else.set_show(false);				// 消去
		else_group_exist = false;
	}

	// --------------------------------------------------------------------------------------------

	int total_group_exist_cnt = global_extra_mode_group_exist_cnt + ((else_group_exist) ? 1 : 0);

	// ボタンアイテムをグループの枠の中に入れる為の補正
	// グループ数が１つ以下で、ソロモードじゃない時
	bool button_item_in_group_flag = false;
	int button_item_in_group_rep_y = 0;
	int button_margin_right = 0;
	if (total_group_exist_cnt <= 1 && func_mod != CFG_WND_FUNC_MOD_SOLO)	{
		button_item_in_group_rep_y = btn_init_rect.height() + BUTTON_MARGIN_BOTTOM;
		int item_rep_height = BUTTON_MARGIN_TOP + button_item_in_group_rep_y;
		global_extra_mode_item_grp_height[0] += item_rep_height;
		global_extra_mode_item_grp_height[1] += item_rep_height;
		global_extra_mode_item_grp_height[2] += item_rep_height;
		global_extra_mode_item_grp_height[3] += item_rep_height;
		else_item_grp_height += item_rep_height;
		button_margin_right = BUTTON_MARGIN_RIGHT;
		button_item_in_group_flag = true;
	}

	// 左側グループのサイズ
	int left_group_width = 0;
	int left_group_height = 0;
	int left_group_cnt = 0;
	if (global_extra_mode_group_exist[0])	{
		if (left_group_width < global_extra_mode_item_grp_width[0])	{	left_group_width = global_extra_mode_item_grp_width[0];	}
		left_group_height += global_extra_mode_item_grp_height[0];
		left_group_cnt++;
	}
	if (global_extra_mode_group_exist[1])	{
		if (left_group_width < global_extra_mode_item_grp_width[1])	{	left_group_width = global_extra_mode_item_grp_width[1];	}
		left_group_height += global_extra_mode_item_grp_height[1];
		left_group_cnt++;
	}
	if (left_group_cnt > 0)	{	left_group_height += (left_group_cnt - 1) * ITEM_GROUP_Y_SPACE;	}
	global_extra_mode_item_grp_width[0] = left_group_width;
	global_extra_mode_item_grp_width[1] = left_group_width;
	bool left_group_exist = (left_group_cnt) ? true : false;

	// 右側グループのサイズ
	int right_group_width = 0;
	int right_group_height = 0;
	int right_group_cnt = 0;
	if (global_extra_mode_group_exist[2])	{
		if (right_group_width < global_extra_mode_item_grp_width[2])	{	right_group_width = global_extra_mode_item_grp_width[2];	}
		right_group_height += global_extra_mode_item_grp_height[2];
		right_group_cnt++;
	}
	if (global_extra_mode_group_exist[3])	{
		if (right_group_width < global_extra_mode_item_grp_width[3])	{	right_group_width = global_extra_mode_item_grp_width[3];	}
		right_group_height += global_extra_mode_item_grp_height[3];
		right_group_cnt++;
	}
	if (else_group_exist)	{
		if (right_group_width < else_item_grp_width)	{	right_group_width = else_item_grp_width;	}
		right_group_height += else_item_grp_height;
		right_group_cnt++;
	}
	if (right_group_cnt > 0)	{	right_group_height += (right_group_cnt - 1) * ITEM_GROUP_Y_SPACE;	}
	global_extra_mode_item_grp_width[2] = right_group_width;
	global_extra_mode_item_grp_width[3] = right_group_width;
	else_item_grp_width = right_group_width;
	bool right_group_exist = (right_group_cnt) ? true : false;

	// 左右のグループの高い方を採用する
	int height_rep = left_group_height - right_group_height;
	if (left_group_height < right_group_height)	{
		if (global_extra_mode_group_exist[1])	{
			global_extra_mode_item_grp_height[1] += -height_rep;
		}
		else if (global_extra_mode_group_exist[0])	{
			global_extra_mode_item_grp_height[0] += -height_rep;
		}
	}
	else	{
		if (else_group_exist)	{
			else_item_grp_height += height_rep;
		}
		else if (global_extra_mode_group_exist[3])	{
			global_extra_mode_item_grp_height[3] += height_rep;
		}
		else if (global_extra_mode_group_exist[2])	{
			global_extra_mode_item_grp_height[2] += height_rep;
		}
	}

	// 全アイテムの幅を求める
	int total_item_width  = 0;
	int total_width_cnt = 0;
	if (left_group_exist)	{
		total_item_width += left_group_width;
		total_width_cnt++;
	}
	if (right_group_exist)	{
		total_item_width += right_group_width;
		total_width_cnt++;
	}
	if (total_width_cnt > 0)	{	total_item_width += (total_width_cnt - 1) * ITEM_GROUP_X_SPACE;	}

	// 全アイテムの高さを求める
	int total_item_height = 0;
	if (left_group_exist) {
		if (total_item_height < left_group_height)	{	total_item_height = left_group_height;	}
	}
	if (right_group_exist) {
		if (total_item_height < right_group_height)	{	total_item_height = right_group_height;	}
	}

	// ウィンドウサイズ算出
	int client_width = WINDOW_MARGIN_LEFT + total_item_width + WINDOW_MARGIN_RIGHT;
	int window_width = client_width;
	int button_item_y;
	int window_height;
	if (button_item_in_group_flag)	{		// ボタンアイテムをグループの枠の中に入れる場合
		button_item_y = WINDOW_MARGIN_TOP + (total_item_height - button_item_in_group_rep_y);
		window_height = WINDOW_MARGIN_TOP + total_item_height + WINDOW_MARGIN_BOTTOM_INGP;
	}
	else	{
		button_item_y = WINDOW_MARGIN_TOP + total_item_height + BUTTON_MARGIN_TOP;
		window_height = button_item_y + btn_init_rect.height() + WINDOW_MARGIN_BOTTOM;
		if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{	window_height += SIZE_GRIP_HEIGHT;	}		// サイズグリップ分を加算
	}

	// デフォルトのクライアント矩形より小さいので補正する
	if (window_width < window_default_rect.width())		{
		int widht_rep = window_default_rect.width() - window_width;
		if (right_group_exist)	{
			global_extra_mode_item_grp_width[2] += widht_rep;
			global_extra_mode_item_grp_width[3] += widht_rep;
			else_item_grp_width += widht_rep;
		}
		else if (left_group_exist)	{
			global_extra_mode_item_grp_width[0] += widht_rep;
			global_extra_mode_item_grp_width[1] += widht_rep;
		}
		client_width += widht_rep;
		window_width = window_default_rect.width();
	}
	if (window_height < window_default_rect.height())	{		// デフォルトのクライアント矩形より小さいので補正する
		int height_rep = window_default_rect.height() - window_height;

		if (global_extra_mode_group_exist[1])	{
			global_extra_mode_item_grp_height[1] += height_rep;
		}
		else if (global_extra_mode_group_exist[0])	{
			global_extra_mode_item_grp_height[0] += height_rep;
		}

		if (else_group_exist)	{
			else_item_grp_height += height_rep;
		}
		else if (global_extra_mode_group_exist[3])	{
			global_extra_mode_item_grp_height[3] += height_rep;
		}
		else if (global_extra_mode_group_exist[2])	{
			global_extra_mode_item_grp_height[2] += height_rep;
		}

		button_item_y += height_rep;
		window_height = window_default_rect.height();
	}

	// ウィンドウサイズ設定
	p_dlg->set_client_size(C_size(window_width, window_height));
	p_dlg->set_def_rect_from_now_rect();

	// ▲：項目タイトル幅を考慮していない
	// ▲：閉じると初期化ボタンの幅分ない場合どうなる

	// --------------------------------------------------------------------------------------------
	// 各グループの基点座標を求める
	int global_extra_mode_group_x[TNM_GLOBAL_EXTRA_MODE_CNT];
	int global_extra_mode_group_y[TNM_GLOBAL_EXTRA_MODE_CNT];
	int else_group_x;
	int else_group_y;

	global_extra_mode_group_x[0] = WINDOW_MARGIN_LEFT;
	global_extra_mode_group_x[1] = WINDOW_MARGIN_LEFT;
	global_extra_mode_group_y[0] = WINDOW_MARGIN_TOP;
	global_extra_mode_group_y[2] = WINDOW_MARGIN_TOP;
	if (global_extra_mode_group_exist[0])	{
		global_extra_mode_group_y[1] = global_extra_mode_group_y[0] + global_extra_mode_item_grp_height[0] + ITEM_GROUP_Y_SPACE;
	}
	else	{
		global_extra_mode_group_y[1] = global_extra_mode_group_y[0];
	}

	if (left_group_exist)	{
		global_extra_mode_group_x[2] = global_extra_mode_group_x[0] + left_group_width + ITEM_GROUP_X_SPACE;
	}
	else	{
		global_extra_mode_group_x[2] = global_extra_mode_group_x[0];
	}
	global_extra_mode_group_x[3] = global_extra_mode_group_x[2];
	else_group_x = global_extra_mode_group_x[2];

	if (global_extra_mode_group_exist[2])	{
		global_extra_mode_group_y[3] = global_extra_mode_group_y[2] + global_extra_mode_item_grp_height[2] + ITEM_GROUP_Y_SPACE;
	}
	else	{
		global_extra_mode_group_y[3] = global_extra_mode_group_y[2];
	}

	if (global_extra_mode_group_exist[3])	{
		else_group_y = global_extra_mode_group_y[3] + global_extra_mode_item_grp_height[3] + ITEM_GROUP_Y_SPACE;
	}
	else	{
		else_group_y = global_extra_mode_group_y[3];
	}

	// --------------------------------------------------------------------------------------------
	// その他のアイテム：座標の設定
	x = else_group_x + ITEM_CHKBOX_BASE_X;
	y = else_group_y + ITEM_CHKBOX_BASE_Y;
	for (int i = 0; i < (int)else_item.size(); i++)	{
		if (else_item[i].exist)	{
			else_item[i].ctrl->set_rltv_pos_with_def_rect(x, y);
			y += ITEM_CHKBOX_Y_SPACE;
		}
	}

	// その他のアイテム：グループのサイズと座標の設定
	ci.grp_else.set_window_size_with_def_size(else_item_grp_width, else_item_grp_height);
	ci.grp_else.set_rltv_pos_with_def_rect(else_group_x, else_group_y);

	// --------------------------------------------------------------------------------------------
	// グローバル汎用モードのアイテム
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_CNT; i++)	{
		// グローバル汎用モードのアイテム：座標の設定
		x = global_extra_mode_group_x[i] + ITEM_RADIO_BASE_X;
		y = global_extra_mode_group_y[i] + ITEM_RADIO_BASE_Y;
		for (int j = 0; j < (int)global_extra_mode_item[i].size(); j++)	{
			if (global_extra_mode_item[i][j].exist)	{
				global_extra_mode_item[i][j].ctrl->set_rltv_pos_with_def_rect(x, y);
				y += ITEM_RADIO_Y_SPACE;
			}
		}
		// その他のアイテム：グループのサイズと座標の設定
		ci.grp_global_extra_mode[i].set_window_size_with_def_size(global_extra_mode_item_grp_width[i], global_extra_mode_item_grp_height[i]);
		ci.grp_global_extra_mode[i].set_rltv_pos_with_def_rect(global_extra_mode_group_x[i], global_extra_mode_group_y[i]);
	}

	// グローバル汎用モードのアイテム：調整モードの設定
	if (else_group_exist || global_extra_mode_group_exist[2] || global_extra_mode_group_exist[3])	{
		if (global_extra_mode_group_exist[1])	{
			ci.grp_global_extra_mode[0].set_adjust_way(7);
			ci.grp_global_extra_mode[1].set_adjust_way(4);
		}
		else	{
			ci.grp_global_extra_mode[0].set_adjust_way(4);
		}
	}
	else	{
		if (global_extra_mode_group_exist[1])	{
			ci.grp_global_extra_mode[0].set_adjust_way(8);
			ci.grp_global_extra_mode[1].set_adjust_way(5);
		}
		else	{
			ci.grp_global_extra_mode[0].set_adjust_way(5);
		}
	}

	if (else_group_exist || global_extra_mode_group_exist[3])	{
		ci.grp_global_extra_mode[2].set_adjust_way(8);
	}
	else	{
		ci.grp_global_extra_mode[2].set_adjust_way(5);
	}

	if (else_group_exist)	{
		ci.grp_global_extra_mode[3].set_adjust_way(8);
	}
	else	{
		ci.grp_global_extra_mode[3].set_adjust_way(5);
	}

	// --------------------------------------------------------------------------------------------
	// ボタンアイテム：座標の設定
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		int close_button_x = client_width - btn_close_rect.width() - WINDOW_MARGIN_RIGHT;
		ci.btn_else_close.set_rltv_pos_with_def_rect(close_button_x, button_item_y);
		int init_button_x = close_button_x - btn_init_rect.width() - ITEM_BUTTON_X_SPACE;
		ci.btn_else_init.set_rltv_pos_with_def_rect(init_button_x - button_margin_right, button_item_y);
	}
	else	{
		int init_button_x = client_width - btn_init_rect.width() - WINDOW_MARGIN_RIGHT;
		ci.btn_else_init.set_rltv_pos_with_def_rect(init_button_x - button_margin_right, button_item_y);
	}
}

// ****************************************************************
// モードのラジオボタンを設定
// ================================================================
void Ccfg_wnd_func_else::set_radio_mode(int mode_no)
{
	if (!h_wnd)	{	return;	}
	if (!(mode_no >= 0 && mode_no < TNM_GLOBAL_EXTRA_MODE_CNT))	{	return;	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_ITEM_CNT; i++)	{
		ci.rad_global_extra_mode[mode_no][i].set_check(false);
	}
	int mode = Gp_config->global_extra_mode_flag[mode_no];
	if (!(mode >= 0 && mode < TNM_GLOBAL_EXTRA_MODE_ITEM_CNT))	{	return;	}
	ci.rad_global_extra_mode[mode_no][mode].set_check(true);
}

// ****************************************************************
// モードのラジオボタンを取得
// ================================================================
int Ccfg_wnd_func_else::get_radio_mode(int mode_no)
{
	if (!h_wnd)	{	return -1;	}
	for (int i = 0; i < TNM_GLOBAL_EXTRA_MODE_ITEM_CNT; i++)	{
		if (ci.rad_global_extra_mode[mode_no][i].get_check())	{	return i;	}
	}
	return -1;
}



