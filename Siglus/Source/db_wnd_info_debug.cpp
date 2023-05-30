#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_debug.h"
#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：デバッグ設定
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_debug")

// 初期化
void C_db_wnd_info_debug::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_DEBUG);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_debug::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_debug::on_init()
{
	// アイテム登録
	bind_control(&ci.stc_ss, IDC_STC_SS, _T("stc_ss"), 7);
	bind_control(&ci.edt_ss, IDC_EDT_SS, _T("edt_ss"), 8);
	bind_control(&ci.stc_editor, IDC_STC_EDITOR, _T("stc_editor"), 7);
	bind_control(&ci.edt_editor, IDC_EDT_EDITOR, _T("edt_editor"), 8);
	bind_control(&ci.stc_koe, IDC_STC_KOE, _T("stc_koe"), 7);
	bind_control(&ci.edt_koe, IDC_EDT_KOE, _T("edt_koe"), 8);
	bind_control(&ci.stc_koe_tool, IDC_STC_KOE_TOOL, _T("stc_koe_tool"), 7);
	bind_control(&ci.edt_koe_tool, IDC_EDT_KOE_TOOL, _T("edt_koe_tool"), 8);
	bind_control(&ci.btn_open, IDC_BTN_OPEN, _T("btn_open"), 3);

	// セパレータ登録

	// アイテム設定ロード
	ci.edt_ss.set_text(Gp_config->ss_path);
	ci.edt_editor.set_text(Gp_config->editor_path);
	ci.edt_koe.set_text(Gp_config->koe_path);
	ci.edt_koe_tool.set_text(Gp_config->koe_tool_path);

	// セパレータ設定ロード

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// アイテムフォーカス

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_debug::on_close()
{
	// ステータスの取得と保存

	// セパレータ設定セーブ

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// コマンド
bool C_db_wnd_info_debug::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_EDT_SS:
			on_edit_ss_path();
			break;
		case IDC_EDT_EDITOR:
			on_edit_editor_path();
			break;
		case IDC_EDT_KOE:
			on_edit_koe_path();
			break;
		case IDC_EDT_KOE_TOOL:
			on_edit_koe_tool_path();
			break;
		case IDC_BTN_OPEN:
			on_btn_open();
			break;
	}

	return true;
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_debug::load_state()
{
	// ※この関数は不要なら削除しても良いです。
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_debug::save_state()
{
	// ※この関数は不要なら削除しても良いです。
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_debug::frame()
{
	// ※この関数は不要なら削除しても良いです。
}

// ****************************************************************
// ss フォルダを設定した
// ================================================================
bool C_db_wnd_info_debug::on_edit_ss_path()
{
	TSTR ss_path = ci.edt_ss.get_text();
	Gp_config->ss_path = ss_path;

	return true;
}

// ****************************************************************
// エディタフォルダを設定した
// ================================================================
bool C_db_wnd_info_debug::on_edit_editor_path()
{
	TSTR editor_path = ci.edt_editor.get_text();
	Gp_config->editor_path = editor_path;

	return true;
}

// ****************************************************************
// 声フォルダを設定した
// ================================================================
bool C_db_wnd_info_debug::on_edit_koe_path()
{
	TSTR koe_path = ci.edt_koe.get_text();
	Gp_config->koe_path = koe_path;

	return true;
}

// ****************************************************************
// 声ツールフォルダを設定した
// ================================================================
bool C_db_wnd_info_debug::on_edit_koe_tool_path()
{
	TSTR koe_tool_path = ci.edt_koe_tool.get_text();
	Gp_config->koe_tool_path = koe_tool_path;

	return true;
}

// ****************************************************************
// 開くボタンを押した
// ================================================================
bool C_db_wnd_info_debug::on_btn_open()
{
	// シーンをエディタで開く
	tnm_open_by_editor(false);

	return true;
}

