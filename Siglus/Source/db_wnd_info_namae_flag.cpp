#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"element/elm_flag.h"
#include	"engine/eng_flag.h"
#include	"dialog/db_wnd_info_namae_flag.h"
#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：名前フラグ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_namae_flag")

// 初期化
void C_db_wnd_info_namae_flag::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_NAMAE_FLAG);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_namae_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_namae_flag::on_init()
{
	// リストビューのIDと名前、タブのページ名
	int lv_id[PAGE_CNT] = {IDC_LV_GLOBAL, IDC_LV_LOCAL};
	TCHAR* lv_name[PAGE_CNT] = {_T("lv_global"), _T("lv_local")};
	TCHAR* tab_caption[PAGE_CNT] = {_T("＊"), _T("％")};

	// 初期化
	for (int i = 0; i < PAGE_CNT; i++)
		m_first[i] = true;

	// アイテム登録
	bind_control(&ci.tab, IDC_TAB, _T("tab"), 5);
	for (int i = 0; i < PAGE_CNT; i++)
		bind_control(&ci.lv_flag[i], lv_id[i], lv_name[i], 5, 1);

	// セパレータ登録

	// タブの設定
	for (int i = 0; i < PAGE_CNT; i++)
		ci.tab.add_item(tab_caption[i]);

	C_rect tab_rect = screen_to_client(ci.tab.get_display_rect());	// タブの表示領域を求める
	tab_rect.left -= 2;		// 微調整

	// アイテム設定ロード
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv_flag[i].easy_setting(true, true, false, true);
		ci.lv_flag[i].add_column(_T("flag"), _T("フラグ"), 100);
		ci.lv_flag[i].add_column(_T("value"), _T("値"), 100);
		ci.lv_flag[i].load_ini();

		// リストビューの初期位置を設定
		ci.lv_flag[i].set_def_rltv_rect(tab_rect);
	}

	// セパレータ設定ロード

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// アイテムフォーカス
	change_page(m_page_no);

	// サイズの更新
	send_wm_size();

	// ウィンドウ表示
	update();	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_namae_flag::on_close()
{
	// ステータスの取得と保存
	for (int i = 0; i < PAGE_CNT; i++)
		ci.lv_flag[i].save_ini();

	// セパレータ設定セーブ

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// コマンド
bool C_db_wnd_info_namae_flag::on_command(WPARAM wp, LPARAM lp)
{
//	switch (LOWORD(wp))	{
//	}
	return true;
}

// 通知
bool C_db_wnd_info_namae_flag::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	switch (ctrl_id)	{
		case IDC_TAB:
			switch (hdr->code)	{
				case TCN_SELCHANGE:
					on_change_page(ci.tab.get_selected_item_no());
					break;
			}
			break;
		case IDC_LV_GLOBAL:
		case IDC_LV_LOCAL:
			switch (hdr->code)	{
				case NM_DBLCLK:
					on_dblclk_lv(ctrl_id, FM_STR);
					break;
				case LVN_CHANGE_TEXT:
					change_str_flag(ctrl_id, ((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->new_text);
					break;
			}
			break;
	}

	return true;
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_namae_flag::load_state()
{
	m_page_no = C_ini_file::body().load_int(_T("page_no"), 0, regist_state.window_name);
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_namae_flag::save_state()
{
	C_ini_file::body().save_int(_T("page_no"), m_page_no, regist_state.window_name);
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_namae_flag::frame()
{
	if (!is_open())
		return;

	STATE cd;

	// ページ数制限
	if (m_page_no < 0 || PAGE_CNT <= m_page_no)
		return;

	// アルファベット全角
	static TCHAR* g_alpha = _T("ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ");

	// 文字列フラグ
	TSTR flag_name;		// 名前フラグ名
	switch (m_page_no)	{
		case 0:		flag_name = _T("＊");	cd.str_flag[m_page_no] = Gp_flag->global_namae.get_sub();	break;
		case 1:		flag_name = _T("％");	cd.str_flag[m_page_no] = Gp_flag->local_namae.get_sub();	break;
	}

	// リストビューのアイテム数を設定
	if (m_first[m_page_no] || cd.str_flag[m_page_no].size() != m_ld.str_flag[m_page_no].size())	{
		ci.lv_flag[m_page_no].set_item_cnt((int)cd.str_flag[m_page_no].size());
	}
	// 増えた分に関してフラグ名を表示
	for (int i = m_first[m_page_no] ? 0 : (int)m_ld.str_flag[m_page_no].size(); i < (int)cd.str_flag[m_page_no].size(); i++)	{
		TSTR this_flag_name;

		if (0 <= i && i < 26)	{
			this_flag_name = flag_name + g_alpha[i];
		}
		else	{
			int m1 = i / 26 - 1;
			int m2 = i % 26;
			this_flag_name = flag_name + g_alpha[m1] + g_alpha[m2];
		}

		ci.lv_flag[m_page_no].set_cell_text(i, 0, this_flag_name);
	}
	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.str_flag[m_page_no].size(); i++)	{
		if (m_first[m_page_no] || i >= (int)m_ld.str_flag[m_page_no].size() || cd.str_flag[m_page_no][i] != m_ld.str_flag[m_page_no][i])	{
			ci.lv_flag[m_page_no].set_cell_text(i, 1, cd.str_flag[m_page_no][i]);
		}
	}

	// 前回の値を保存（現在のページのみ）
	m_ld.str_flag[m_page_no] = cd.str_flag[m_page_no];

	// 初回フラグ
	m_first[m_page_no] = false;
}

// ****************************************************************
// コントロールＩＤからリストビューを取得する
// ================================================================
C_tnm_dbg_listview* C_db_wnd_info_namae_flag::get_lv_by_ctrl_id(int ctrl_id)
{
	C_tnm_dbg_listview* p_lv = NULL;
	switch (ctrl_id)	{
		case IDC_LV_GLOBAL:	p_lv = &ci.lv_flag[0];		break;
		case IDC_LV_LOCAL:	p_lv = &ci.lv_flag[1];		break;
	}
	return p_lv;
}

// ****************************************************************
// コントロールＩＤから文字列リストを取得する
// ================================================================
C_elm_str_list* C_db_wnd_info_namae_flag::get_str_list_by_ctrl_id(int ctrl_id)
{
	C_elm_str_list* p_str_list = NULL;
	switch (ctrl_id)	{
		case IDC_LV_GLOBAL:	p_str_list = &Gp_flag->global_namae;	break;
		case IDC_LV_LOCAL:	p_str_list = &Gp_flag->local_namae;		break;
	}
	return p_str_list;
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_namae_flag::on_dblclk_lv(int ctrl_id, int form)
{
	C_tnm_dbg_listview* p_lv = get_lv_by_ctrl_id(ctrl_id);
	if (p_lv)	{

		// 選択状態取得
		int select_cnt = p_lv->get_selected_item_cnt();
		int select_no = p_lv->get_selected_item_no();

		// マウスカーソルの乗っているセル番号
		int onmouse_item_no, onmouse_column_no;
		C_point mp = p_lv->screen_to_client(get_mouse_pos());
		p_lv->get_cell_by_point(mp.x, mp.y, &onmouse_item_no, &onmouse_column_no);

		// アイテムを変更する
		if (select_cnt == 1 && select_no == onmouse_item_no && onmouse_column_no == 1)	{
			if (false);
			else if (form == FM_STR)	{
				TSTR value = p_lv->get_cell_text(onmouse_item_no, onmouse_column_no);
				p_lv->edit_item_text_edit(onmouse_item_no, onmouse_column_no);
			}
		}
	}
}

// ****************************************************************
// 文字列フラグを変更する
// ================================================================
void C_db_wnd_info_namae_flag::change_str_flag(int ctrl_id, int flag_no, CTSTR& value)
{
	C_elm_str_list* p_str_list = get_str_list_by_ctrl_id(ctrl_id);
	if (p_str_list)	{
		if (0 <= flag_no && flag_no < p_str_list->get_size())
			(*p_str_list)[flag_no] = value;
	}
}

// ****************************************************************
// ページが変更された
// ================================================================
void C_db_wnd_info_namae_flag::on_change_page(int page_no)
{
	// 現在のページを非表示にする
	ci.lv_flag[m_page_no].set_show(false);

	// ページ番号を更新
	m_page_no = page_no;

	// 新しいページを表示する
	ci.lv_flag[m_page_no].set_show(true);
}

// ****************************************************************
// ページを変更する
// ================================================================
void C_db_wnd_info_namae_flag::change_page(int page_no)
{
	// タブコントロールを移動
	ci.tab.set_selected_item_no(page_no);

	// ページが変更されたときの処理を行う
	on_change_page(page_no);
}

