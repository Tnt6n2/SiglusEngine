#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/cfg_wnd_func_system.h"

#include	"localize/localize.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：ムービーの再生方法
// ================================================================

// 初期化
void C_cfg_wnd_func_system::on_init_funcfunc()
{
	// アイテム登録
	p_dlg->bind_control(&ci.grp_system, IDC_GRP_SYSTEM, _T(""), 5);
	p_dlg->bind_control(&ci.grp_system_movie, IDC_GRP_SYSTEM_MOVIE, _T(""), 2);

	p_dlg->bind_control(&ci.chk_system_sleep, IDC_CHK_SYSTEM_SLEEP, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_no_wipe_anime, IDC_CHK_SYSTEM_NO_WIPE_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_skip_wipe_anime, IDC_CHK_SYSTEM_SKIP_WIPE_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_no_mwnd_anime, IDC_CHK_SYSTEM_NO_MWND_ANIME, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_wheel_next_message, IDC_CHK_SYSTEM_WHEEL_NEXT_MESSAGE, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_koe_dont_stop, IDC_CHK_SYSTEM_KOE_DONT_STOP, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_skip_unread_message, IDC_CHK_SYSTEM_SKIP_UNREAD_MESSAGE, _T(""), 7);
	p_dlg->bind_control(&ci.chk_system_play_silent_sound, IDC_CHK_SYSTEM_PLAY_SILENT_SOUND, _T(""), 7);
	p_dlg->bind_control(&ci.btn_system_init, IDC_BTN_SYSTEM_INIT, _T(""), 3);
	p_dlg->bind_control(&ci.rad_system_movie_play_type_mci, IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_MCI, _T(""), 1);
	p_dlg->bind_control(&ci.rad_system_movie_play_type_wmp, IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_WMP, _T(""), 1);
	p_dlg->bind_control(&ci.btn_system_close, IDC_BTN_SYSTEM_CLOSE, _T(""), 3);

	// 文字列を設定後、テキストサイズに合わせてコントロールの幅を調整する
	ci.chk_system_sleep.set_text_adjust_width(Gp_ini->config.system.sleep.str, 20);
	ci.chk_system_no_wipe_anime.set_text_adjust_width(Gp_ini->config.system.no_wipe_anime.str, 20);
	ci.chk_system_skip_wipe_anime.set_text_adjust_width(Gp_ini->config.system.skip_wipe_anime.str, 20);
	ci.chk_system_no_mwnd_anime.set_text_adjust_width(Gp_ini->config.system.no_mwnd_anime.str, 20);
	ci.chk_system_wheel_next_message.set_text_adjust_width(Gp_ini->config.system.wheel_next_message.str, 20);
	ci.chk_system_koe_dont_stop.set_text_adjust_width(Gp_ini->config.system.koe_dont_stop.str, 20);
	ci.chk_system_skip_unread_message.set_text_adjust_width(Gp_ini->config.system.skip_unread_message.str, 20);
	ci.chk_system_play_silent_sound.set_text_adjust_width(Gp_ini->config.system.play_silent_sound.str, 20);

	// 使用しない項目を削除し、項目を詰める
	struct item_struct
	{
		bool exist;
		C_control* ctrl;
		C_rect rect;
	};
	ARRAY<item_struct>	item;
	item.resize(8);
	item[0].exist = Gp_ini->config.system.sleep.exist;
	item[1].exist = Gp_ini->config.system.no_wipe_anime.exist;
	item[2].exist = Gp_ini->config.system.skip_wipe_anime.exist;
	item[3].exist = Gp_ini->config.system.no_mwnd_anime.exist;
	item[4].exist = Gp_ini->config.system.wheel_next_message.exist;
	item[5].exist = Gp_ini->config.system.koe_dont_stop.exist;
	item[6].exist = Gp_ini->config.system.skip_unread_message.exist;
	item[7].exist = Gp_ini->config.system.play_silent_sound.exist;
	item[0].ctrl = &ci.chk_system_sleep;
	item[1].ctrl = &ci.chk_system_no_wipe_anime;
	item[2].ctrl = &ci.chk_system_skip_wipe_anime;
	item[3].ctrl = &ci.chk_system_no_mwnd_anime;
	item[4].ctrl = &ci.chk_system_wheel_next_message;
	item[5].ctrl = &ci.chk_system_koe_dont_stop;
	item[6].ctrl = &ci.chk_system_skip_unread_message;
	item[7].ctrl = &ci.chk_system_play_silent_sound;
	if (full_mode_flag)	{		// デバッグ用フルモード
		for (int i = 0; i < (int)item.size(); i++)	{
			item[i].exist = true;
		}
	}
	for (int i = 0; i < (int)item.size(); i++)	{
		item[i].rect = item[i].ctrl->get_rltv_rect();
	}
	int y_rep = 0;
	int last_item_bottom = item[0].rect.top;
	for (int i = 0; i < (int)item.size(); i++)	{
		if (item[i].exist)	{
			item[i].ctrl->add_rltv_pos_with_def_rect(0, -y_rep);
			last_item_bottom = item[i].ctrl->get_rltv_rect().bottom;
		}
		else	{
			item[i].ctrl->set_show(false);		// 消去
			if (i < ((int)item.size() - 1))	{	y_rep += item[i + 1].rect.top - item[i].rect.top;	}	// 次の項目との距離
			else							{	y_rep += item[i].rect.height();						}	// 最後の項目は高
		}
	}
	if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
		ci.grp_system.add_window_size_with_def_size(0, -y_rep);		// グループボックス
		ci.btn_system_init.add_rltv_pos_with_def_rect(0, -y_rep);	// 初期化ボタン
		ci.btn_system_close.add_rltv_pos_with_def_rect(0, -y_rep);	// 閉じるボタン
		ci.grp_system_movie.add_rltv_pos_with_def_rect(0, -y_rep);					// ムービーの再生方法：グループボックス
		ci.rad_system_movie_play_type_mci.add_rltv_pos_with_def_rect(0, -y_rep);	// ムービーの再生方法：ラジオボタン：ＭＣＩ
		ci.rad_system_movie_play_type_wmp.add_rltv_pos_with_def_rect(0, -y_rep);	// ムービーの再生方法：ラジオボタン：ＷＭＰ
		p_dlg->add_window_size_with_def_size(0, -y_rep);			// ウィンドウサイズ
		p_dlg->send_wm_size();										// ★これ重要！（起動時、ウィンドウがリサイズしなかった場合、アイテムの座標が狂うので強制的にリサイズさせる）
	}

	// ムービーの再生方法のアイテムを消去
	if (!(Gp_ini->config.exist_mov || full_mode_flag))	{	// デバッグ用フルモード
		ci.grp_system_movie.set_show(false);
		ci.rad_system_movie_play_type_mci.set_show(false);
		ci.rad_system_movie_play_type_wmp.set_show(false);
		int yy_rep;
		if (func_mod == CFG_WND_FUNC_MOD_SOLO)	{
			yy_rep = ci.grp_system_movie.get_rltv_rect().bottom - last_item_bottom;
			ci.btn_system_init.add_rltv_pos_with_def_rect(0, -yy_rep);	// 初期化ボタン
			ci.btn_system_close.add_rltv_pos_with_def_rect(0, -yy_rep);	// 閉じるボタン
		}
		else	{
			yy_rep = ci.grp_system_movie.get_rltv_rect().bottom - ci.grp_system.get_rltv_rect().bottom;
		}
		p_dlg->add_window_size_with_def_size(0, -yy_rep);			// ウィンドウサイズ
		p_dlg->send_wm_size();										// ★これ重要！（起動時、ウィンドウがリサイズしなかった場合、アイテムの座標が狂うので強制的にリサイズさせる）
	}

	// アイテムの設定
	if (modal_mode_flag)	{ ci.btn_system_close.set_text(LCL_STR_MODAL_DIALOG_DECIDE_BTN_STR); }

	// ダイアログを更新
	update_dialog();
}

// 閉じる
void C_cfg_wnd_func_system::on_close_funcfunc()
{
}

// コマンド
bool C_cfg_wnd_func_system::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_SYSTEM_SLEEP:
			Gp_config->system.sleep_flag = ci.chk_system_sleep.get_check();
			return true;

		case IDC_CHK_SYSTEM_NO_WIPE_ANIME:
			Gp_config->system.no_wipe_anime_flag = ci.chk_system_no_wipe_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_SKIP_WIPE_ANIME:
			Gp_config->system.skip_wipe_anime_flag = ci.chk_system_skip_wipe_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_NO_MWND_ANIME:
			Gp_config->system.no_mwnd_anime_flag = ci.chk_system_no_mwnd_anime.get_check();
			return true;

		case IDC_CHK_SYSTEM_WHEEL_NEXT_MESSAGE:
			Gp_config->system.wheel_next_message_flag = ci.chk_system_wheel_next_message.get_check();
			return true;

		case IDC_CHK_SYSTEM_KOE_DONT_STOP:
			Gp_config->system.koe_dont_stop_flag = ci.chk_system_koe_dont_stop.get_check();
			return true;

		case IDC_CHK_SYSTEM_SKIP_UNREAD_MESSAGE:
			Gp_config->system.skip_unread_message_flag = ci.chk_system_skip_unread_message.get_check();
			return true;

		case IDC_CHK_SYSTEM_PLAY_SILENT_SOUND:
			Gp_system_config->ds_play_silent_sound_flag = ci.chk_system_play_silent_sound.get_check();
			return true;

		case IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_MCI:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_MCI;
			return true;

		case IDC_RAD_SYSTEM_MOVIE_PLAY_TYPE_WMP:
			Gp_system_config->movie_type = TNM_MOVIE_PLAY_TYPE_WMP;
			return true;

		case IDC_BTN_SYSTEM_INIT:
			init_state();
			return true;

		case IDC_BTN_SYSTEM_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// 通知
bool C_cfg_wnd_func_system::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// マウスホイール
bool C_cfg_wnd_func_system::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｈスクロール
bool C_cfg_wnd_func_system::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// Ｖスクロール
bool C_cfg_wnd_func_system::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// スタティックコントロールのカラー変更
LRESULT C_cfg_wnd_func_system::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// フレーム
void C_cfg_wnd_func_system::frame_funcfunc()
{
	if (!h_wnd)
		return;

	// 外部で変更された
	if (Gp_config->system.sleep_flag != ci.chk_system_sleep.get_check())	{
		ci.chk_system_sleep.set_check(Gp_config->system.sleep_flag);
	}
	if (Gp_config->system.no_wipe_anime_flag != ci.chk_system_no_wipe_anime.get_check())	{
		ci.chk_system_no_wipe_anime.set_check(Gp_config->system.no_wipe_anime_flag);
	}
	if (Gp_config->system.skip_wipe_anime_flag != ci.chk_system_skip_wipe_anime.get_check())	{
		ci.chk_system_skip_wipe_anime.set_check(Gp_config->system.skip_wipe_anime_flag);
	}
	if (Gp_config->system.no_mwnd_anime_flag != ci.chk_system_no_mwnd_anime.get_check())	{
		ci.chk_system_no_mwnd_anime.set_check(Gp_config->system.no_mwnd_anime_flag);
	}
	if (Gp_config->system.wheel_next_message_flag != ci.chk_system_wheel_next_message.get_check())	{
		ci.chk_system_wheel_next_message.set_check(Gp_config->system.wheel_next_message_flag);
	}
	if (Gp_config->system.koe_dont_stop_flag != ci.chk_system_koe_dont_stop.get_check())	{
		ci.chk_system_koe_dont_stop.set_check(Gp_config->system.koe_dont_stop_flag);
	}
	if (Gp_config->system.skip_unread_message_flag != ci.chk_system_skip_unread_message.get_check())	{
		ci.chk_system_skip_unread_message.set_check(Gp_config->system.skip_unread_message_flag);
	}
	if (Gp_system_config->ds_play_silent_sound_flag != ci.chk_system_play_silent_sound.get_check())	{
		ci.chk_system_play_silent_sound.set_check(Gp_system_config->ds_play_silent_sound_flag);
	}
	if (Gp_system_config->movie_type != get_radio_movie_play_type())	{
		set_radio_movie_play_type();
	}
}

// 初期状態に戻す
void C_cfg_wnd_func_system::init_state()
{
	tnm_init_config_system();

	// 以下もダイアログに存在するため特殊に初期化
	tnm_init_system_config_play_silent_sound();

	// ダイアログを更新
	update_dialog();
}

// ダイアログ更新
void C_cfg_wnd_func_system::update_dialog()
{
	if (!h_wnd)
		return;

	ci.chk_system_sleep.set_check(Gp_config->system.sleep_flag);
	ci.chk_system_no_wipe_anime.set_check(Gp_config->system.no_wipe_anime_flag);
	ci.chk_system_skip_wipe_anime.set_check(Gp_config->system.skip_wipe_anime_flag);
	ci.chk_system_no_mwnd_anime.set_check(Gp_config->system.no_mwnd_anime_flag);
	ci.chk_system_wheel_next_message.set_check(Gp_config->system.wheel_next_message_flag);
	ci.chk_system_koe_dont_stop.set_check(Gp_config->system.koe_dont_stop_flag);
	ci.chk_system_skip_unread_message.set_check(Gp_config->system.skip_unread_message_flag);
	set_radio_movie_play_type();
}

// ****************************************************************
// ムービーの再生方法のラジオボタンを設定
// ================================================================
void C_cfg_wnd_func_system::set_radio_movie_play_type()
{
	if (!h_wnd)
		return;

	ci.rad_system_movie_play_type_mci.set_check(false);
	ci.rad_system_movie_play_type_wmp.set_check(false);
	switch (Gp_system_config->movie_type)	{
		case TNM_MOVIE_PLAY_TYPE_MCI:	ci.rad_system_movie_play_type_mci.set_check(true);	break;
		case TNM_MOVIE_PLAY_TYPE_WMP:	ci.rad_system_movie_play_type_wmp.set_check(true);	break;
	}
}

// ****************************************************************
// ムービーの再生方法のラジオボタンを取得
// ================================================================
int C_cfg_wnd_func_system::get_radio_movie_play_type()
{
	if (!h_wnd)
		return -1;

	if (ci.rad_system_movie_play_type_mci.get_check())		{	return TNM_MOVIE_PLAY_TYPE_MCI;	}
	else if (ci.rad_system_movie_play_type_wmp.get_check())	{	return TNM_MOVIE_PLAY_TYPE_WMP;	}
	return -1;
}


