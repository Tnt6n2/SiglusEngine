#include	"pch.h"
#include	"element\elm_mwnd.h"
#include	"dialog\db_wnd_info_mwnd.h"

#include	"..\resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：メッセージウィンドウ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_mwnd")

// 初期化
void C_db_wnd_info_mwnd::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_MWND);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_mwnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
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
bool C_db_wnd_info_mwnd::on_init()
{
	// リストビューのIDと名前、タブのページ名
	int lv_id[PAGE_CNT] = {IDC_LV_BACK, IDC_LV_FRONT};
	TCHAR* lv_name[PAGE_CNT] = {_T("lv_back"), _T("lv_front")};
	TCHAR* tab_caption[PAGE_CNT] = {_T("back"), _T("front")};

	// 初期化
	for (int i = 0; i < PAGE_CNT; i++)
		m_first[i] = true;

	// アイテム登録
	bind_control(&ci.tab, IDC_TAB, _T("tab"), 5);
	for (int i = 0; i < PAGE_CNT; i++)
		bind_control(&ci.lv[i], lv_id[i], lv_name[1], 5, 1);

	// セパレータ登録

	// タブの設定
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.tab.add_item(tab_caption[i]);
	}
	C_rect tab_rect = screen_to_client(ci.tab.get_display_rect());	// タブの表示領域を求める
	tab_rect.left -= 2;		// 微調整

	// アイテム設定ロード
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv[i].easy_setting(true, true, false, true);

		int column_width = 20;
		ci.lv[i].add_column(_T("id"), _T("ID"), column_width);
		ci.lv[i].add_column(_T("world"), _T("ﾜｰﾙﾄﾞ"), column_width);
		ci.lv[i].add_column(_T("layer"), _T("ﾚｲﾔｰ"), column_width);
		ci.lv[i].add_column(_T("appear"), _T("出現"), column_width);
		ci.lv[i].add_column(_T("msg"), _T("ﾒｯｾｰｼﾞ"), column_width);
		ci.lv[i].load_ini();

		// リストビューの初期位置を設定
		ci.lv[i].set_def_rltv_rect(tab_rect);
	}

	// セパレータ設定ロード

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// アイテムフォーカス
	change_page(m_page_no);

	// ウィンドウ表示
	update();	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_mwnd::on_close()
{
	// ステータスの取得と保存
	for (int i = 0; i < PAGE_CNT; i++)	{
		ci.lv[i].save_ini();
	}

	// セパレータ設定セーブ

	// ウィンドウ矩形の保存
	save_ini_window_rect();

	return true;
}

// コマンド
bool C_db_wnd_info_mwnd::on_command(WPARAM wp, LPARAM lp)
{
//	switch (LOWORD(wp))	{
//	}
	return true;
}

// 通知
bool C_db_wnd_info_mwnd::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	switch (hdr->code)	{
		case TCN_SELCHANGE:
			on_change_page(ci.tab.get_selected_item_no());
			break;
	}

	return true;
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_mwnd::load_state()
{
	m_page_no = C_ini_file::body().load_int(_T("page_no"), 1, regist_state.window_name);
		// デフォルトのページ番号：1 = front
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_mwnd::save_state()
{
	C_ini_file::body().save_int(_T("page_no"), m_page_no, regist_state.window_name);
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_mwnd::frame(C_elm_mwnd_list** mwnd_list_list, int list_cnt)
{
	if (!is_open())
		return;

	STATE cd;

	int lv_no;			// 表示されているリストビュー
	int stage_no;		// 表示するフラグリスト番号

	switch (m_page_no)	{
		case 0:		lv_no = 0;	stage_no = 0;	break;
		case 1:		lv_no = 1;	stage_no = 1;	break;
	}

	// ================================================================
	// メッセージウィンドウ情報を取得する
	// ----------------------------------------------------------------
	//		appear_flag が立っているもののみリストビューに表示する
	//		リストビューに表示するものだけを取得していく
	// ----------------------------------------------------------------

	cd.mwnd_list[stage_no].clear();
	int mwnd_cnt = mwnd_list_list[stage_no]->get_size();
	for (int i = 0; i < mwnd_cnt; i++)	{
		C_elm_mwnd* src = &(*mwnd_list_list[stage_no])[i];

		// ウィンドウが出現しているものについてのみ
		if (src->get_window_appear_flag())	{
			int now_size = (int)cd.mwnd_list[stage_no].size();
			cd.mwnd_list[stage_no].resize(now_size + 1);
			STATE::SUB* dst = &cd.mwnd_list[stage_no][now_size];
	
			// オブジェクトの情報を取得する
			dst->mwnd_no = i;
			dst->world = src->get_world();
			dst->layer = src->get_layer();
			dst->appear_flag = src->get_window_appear_flag();
			dst->msg = src->get_debug_msg();
				// ▲その他のパラメータ：未実装
		}
	}

	// リストビューのアイテム数を設定
	if (m_first[m_page_no] || cd.mwnd_list[stage_no].size() != m_ld.mwnd_list[stage_no].size())
		ci.lv[lv_no].set_item_cnt((int)cd.mwnd_list[stage_no].size());

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.mwnd_list[stage_no].size(); i++)	{
		int column_no = -1;

		// 初回もしくは増えた分に関しては強制表示
		bool change = m_first[m_page_no] || i >= (int)m_ld.mwnd_list[stage_no].size();

		// わかりやすくするために情報へのポインタを取得しておく
		STATE::SUB* now = &cd.mwnd_list[stage_no][i];
		STATE::SUB* last = change ? NULL : &m_ld.mwnd_list[stage_no][i];

		// メッセージウィンドウ番号
		column_no ++;
		if (change || now->mwnd_no != last->mwnd_no)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->mwnd_no));
		}
	
		// ワールド
		column_no ++;
		if (change || now->world != last->world)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->world));
		}

		// レイヤー
		column_no ++;
		if (change || now->layer != last->layer)	{
			ci.lv[lv_no].set_cell_text(i, column_no, tostr(now->layer));
		}

		// 出現
		column_no ++;
		if (change || now->appear_flag != last->appear_flag)	{
			ci.lv[lv_no].set_cell_text(i, column_no, now->appear_flag ? _T("1") : _T("0"));
		}

		// ファイル名
		column_no ++;
		if (change || now->msg != last->msg)		{
			ci.lv[lv_no].set_cell_text(i, column_no, now->msg);
		}
	}

	// 前回の値を保存（現在のページのみ）
	m_ld.mwnd_list[stage_no] = cd.mwnd_list[stage_no];

	// 初回フラグ
	m_first[m_page_no] = false;
}

void C_db_wnd_info_mwnd::on_change_page(int page_no)
{
	// 現在のページを非表示にする
	ci.lv[m_page_no].set_show(false);

	// ページ番号を更新
	m_page_no = page_no;

	// 新しいページを表示する
	ci.lv[m_page_no].set_show(true);
}

void C_db_wnd_info_mwnd::change_page(int page_no)
{
	// タブコントロールを移動
	ci.tab.set_selected_item_no(page_no);

	// ページが変更されたときの処理を行う
	on_change_page(page_no);
}

