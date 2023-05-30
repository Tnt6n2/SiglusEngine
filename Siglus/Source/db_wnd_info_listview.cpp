#include	"pch.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：汎用リストビュー
// ================================================================

#define		IDC_LISTVIEW		1000

// 開く（作成）
void C_db_wnd_info_listview::open(HWND parent_h_wnd)
{
	if (m_h_wnd)
		return;

	// ダイアログテンプレートを使ってダイアログを組み立てる
	C_dialog_template dt;
	DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | DS_SHELLFONT;
	dt.init_param(0, 0, 100, 160, caption_name_str, style, WS_EX_TOOLWINDOW, -1, 8, _T("MS Shell Dlg"));

	// リストビューを作成する
	style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT;
	dt.add_control(0, 0, 100, 160, (PTR)_T("SysListView32"), IDC_LISTVIEW, style, 0, NULL);

	// ダイアログを作成する
	create_indirect(&dt, regist_state.window_name, regist_state.limit_min, regist_state.attach, parent_h_wnd);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_listview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_listview::on_init()
{
	// フレーム初回フラグ
	frame_first_flag = true;

	// アイテム登録
	bind_control(&lv, IDC_LISTVIEW, _T("listview"), 5);

	// リストビューの簡易設定
	lv.easy_setting(true, true, false, true);

	// リストビューのコラム設定
	for (int i = 0; i < (int)lvc_list.size(); i++)	{
		lv.add_column(lvc_list[i].name, lvc_list[i].caption, lvc_list[i].width);
	}

	// アイテム設定ロード
	lv.load_ini();

	// ファンクション
	on_init_func();

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_listview::on_close()
{
	// アイテム設定セーブ
	lv.save_ini();

	// ファンクション
	on_close_func();

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// ****************************************************************
// キャプション名登録
// ================================================================
void C_db_wnd_info_listview::regist_caption_name(CTSTR& name)
{
	caption_name_str = name;
}

// ****************************************************************
// リストビューの項目追加
// ================================================================
void C_db_wnd_info_listview::listview_add_column(CTSTR& name, CTSTR& caption, int width)
{
	LISTVIEW_COLUMN lvc;
	lvc.name = name;
	lvc.caption = caption;
	lvc.width = width;
	lvc_list.push_back(lvc);
}


