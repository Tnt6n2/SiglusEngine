#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_syscom.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_scn_control.h"
#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：シーンコントロール
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_scn_control")


// 初期化
void C_db_wnd_info_scn_control::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_SCN_CONTROL);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_scn_control::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_scn_control::on_init()
{
	// アイテム登録
	bind_control(&ci.lst_scn_name, IDC_LST_SCN_NAME, _T(""), 5);
	bind_control(&ci.lst_z_no, IDC_LST_Z_NO, _T(""), 6);
	bind_control(&ci.btn_restart, IDC_BTN_RESTART, _T(""), 2);
	bind_control(&ci.btn_select_start, IDC_BTN_SELECT_START, _T(""), 2);

	// セパレータ登録
	ci.spa_v00.set_hv_mod(true, &ci.lst_scn_name, &ci.lst_z_no);
	bind_separater(&ci.spa_v00, _T("spa_v00"));

	// シーン名リスト作成
	make_scn_name_list();

	// シーン名リストの初期選択設定
	int scn_no = Gp_lexer->get_scn_no(m_state.scn_name);
	if (scn_no == -1)	{
		scn_no = Gp_lexer->get_scn_no(Gp_ini->start_scene_name);	// ini の方を使う！
	}
	ci.lst_scn_name.set_selected_item_no(scn_no);

	// Ｚ番号リスト作成
	make_z_no_list();

	// Ｚ番号リストの初期選択設定
	ci.lst_z_no.set_selected_item_no(get_item_no_from_z_no(m_state.z_no));

	// アイテム設定ロード
	int top = C_ini_file::body().load_int(_T("scn_name_list_top"), 0, REGIST_WINDOW_NAME);
	ci.lst_scn_name.send_message(LB_SETTOPINDEX, top, 0);
	top = C_ini_file::body().load_int(_T("z_no_list_top"), 0, REGIST_WINDOW_NAME);
	ci.lst_z_no.send_message(LB_SETTOPINDEX, top, 0);

	// セパレータ設定ロード
	separater_manager.load_ini();

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// アイテムフォーカス

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_scn_control::on_close()
{
	// ステータスの取得と保存
	get_scn_name();
	get_z_no();
	save_state();

	// アイテム設定セーブ
	int top = (int)ci.lst_scn_name.send_message(LB_GETTOPINDEX, 0, 0);
	C_ini_file::body().save_int(_T("scn_name_list_top"), top, REGIST_WINDOW_NAME);
	top = (int)ci.lst_z_no.send_message(LB_GETTOPINDEX, 0, 0);
	C_ini_file::body().save_int(_T("z_no_list_top"), top, REGIST_WINDOW_NAME);

	// セパレータ設定セーブ
	separater_manager.save_ini();

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// コマンド
bool C_db_wnd_info_scn_control::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_RESTART:
			tnm_syscom_restart_from_start();
			break;

		case IDC_BTN_SELECT_START:
			tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
			break;

		case IDC_LST_SCN_NAME:
			switch (HIWORD(wp))	{
				case LBN_SELCHANGE:	make_z_no_list();	break;
				case LBN_DBLCLK:
					tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
					break;
			}
			break;
		case IDC_LST_Z_NO:
			switch (HIWORD(wp))	{
				case LBN_DBLCLK:
					tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
					break;
			}
			break;
	}
	return true;
}

// シーン名リスト作成
void C_db_wnd_info_scn_control::make_scn_name_list()
{
	int scn_cnt = Gp_lexer->get_scn_cnt();
	for (int scn_no = 0; scn_no < scn_cnt; scn_no++)	{
		TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
		ci.lst_scn_name.add_item(scn_name);
	}
}

// Ｚ番号リスト作成
void C_db_wnd_info_scn_control::make_z_no_list()
{
	ci.lst_z_no.clear();	// リストボックスクリア
	m_z_no_list.clear();		// ハッシュクリア

	int scn_no = ci.lst_scn_name.get_selected_item_no();	// シーン名リストの選択番号取得
	if (scn_no == LB_ERR)	{	return;	}

	for (int z_no = 0; z_no < TNM_Z_LABEL_CNT; z_no++)	{
		if (Gp_lexer->is_exist_z(scn_no, z_no))	{
			ci.lst_z_no.add_item(str_format(_T("#z%02d"), z_no));
			m_z_no_list.push_back(z_no);
		}
	}
}

// 実際のＺ番号からＺ番号リストのアイテム番号を取得
int C_db_wnd_info_scn_control::get_item_no_from_z_no(int z_no)
{
	ARRAY<int>::iterator itr = std::find(m_z_no_list.begin(), m_z_no_list.end(), z_no);
	if (itr == m_z_no_list.end())	{	return -1;	}
	return (int)(itr - m_z_no_list.begin());
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_scn_control::load_state()
{
	m_state.scn_name = C_ini_file::body().load_str(_T("scn_name"), Gp_global->start_scene_name, REGIST_WINDOW_NAME);
	m_state.z_no = C_ini_file::body().load_int(_T("z_no"), Gp_global->start_z_no, REGIST_WINDOW_NAME);
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_scn_control::save_state()
{
	C_ini_file::body().save_str(_T("scn_name"), m_state.scn_name, REGIST_WINDOW_NAME);
	C_ini_file::body().save_int(_T("z_no"), m_state.z_no, REGIST_WINDOW_NAME);
}

// ****************************************************************
// シーン名取得
// ================================================================
TSTR C_db_wnd_info_scn_control::get_scn_name()
{
	if (!m_h_wnd)	{	return m_state.scn_name;	}
	int scn_no = ci.lst_scn_name.get_selected_item_no();
	m_state.scn_name = (scn_no == LB_ERR) ? Gp_ini->start_scene_name : Gp_lexer->get_scn_name(scn_no);	// ini の方を使う！
	if (m_state.scn_name.empty())	{	m_state.scn_name = Gp_ini->start_scene_name;	}
	return m_state.scn_name;
}

// ****************************************************************
// シーン番号取得
// ================================================================
int C_db_wnd_info_scn_control::get_scn_no()
{
	get_scn_name();
	int scn_no = Gp_lexer->get_scn_no(m_state.scn_name);
	if (scn_no == -1)	{
		scn_no = Gp_lexer->get_scn_no(Gp_ini->start_scene_name);	// ini の方を使う！
	}
	return scn_no;
}

// ****************************************************************
// Ｚ番号取得
// ================================================================
int C_db_wnd_info_scn_control::get_z_no()
{
	if (!m_h_wnd)	{	return m_state.z_no;	}
	int z_no = ci.lst_z_no.get_selected_item_no();
	m_state.z_no = (z_no == LB_ERR) ? 0 : m_z_no_list[z_no];
	return m_state.z_no;
}

