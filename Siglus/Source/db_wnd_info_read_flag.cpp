#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_read_flag.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"engine/eng_dialog.h"
#include	"dialog/db_wnd_info_read_flag.h"
#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：既読フラグ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_read_flag")

// 初期化
void C_db_wnd_info_read_flag::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_READ_FLAG);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_read_flag::window_proc(UINT msg, WPARAM wp, LPARAM lp)
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
bool C_db_wnd_info_read_flag::on_init()
{
	// 初期化
	m_first = true;

	// アイテム登録
	bind_control(&ci.lv_scn, IDC_LV_SCN, _T(""), 4);
	bind_control(&ci.lv_read_flag, IDC_LV_READ_FLAG, _T(""), 5);
	bind_control(&ci.btn_find, IDC_BTN_FIND_NOT_READ, _T(""), 3);
	bind_control(&ci.stc_status, IDC_STC_STATUS, _T(""), 2);

	// リストビューの設定
	ci.lv_scn.add_column(_T("scn_name"), _T("ｼｰﾝ名"), 100);
	ci.lv_scn.add_column(_T("finish"), _T("未"), 30);
	ci.lv_scn.add_column(_T("read"), _T("残り"), 100);
	ci.lv_scn.easy_setting(true, false, false, false);
	ci.lv_read_flag.add_column(_T("flag"), _T("ﾌﾗｸﾞ(行番号)"), 100);
	ci.lv_read_flag.add_column(_T("dummy"), _T("dummy"), 0);
	ci.lv_read_flag.easy_setting(true, false, true, false);

	// セパレータ
	ci.separater.set_hv_mod(true);
	ci.separater.add_control_first(&ci.lv_scn);
	ci.separater.add_control_second(&ci.lv_read_flag);
	ci.separater.add_control_second(&ci.btn_find);
	bind_separater(&ci.separater, _T("separater"));

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();

	// セパレータ設定ロード
	separater_manager.load_ini();

	// アイテムフォーカス

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_read_flag::on_close()
{
	// ウィンドウ矩形の保存
	save_ini_window_rect();

	// セパレータ設定セーブ
	separater_manager.save_ini();

	return true;
}

// コマンド
bool C_db_wnd_info_read_flag::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);

	if (ctrl_id == IDC_BTN_FIND_NOT_READ)	{

		if (0 <= m_cd.cur_scn_no && m_cd.cur_scn_no < (int)m_cd.scn_data.size() && m_cd.scn_data[m_cd.cur_scn_no].flag_cnt > 0)	{
			int focused_pos = ci.lv_read_flag.get_focused_item_no();
			for (int i = focused_pos + 1; i < m_cd.scn_data[m_cd.cur_scn_no].flag_cnt; i++)	{
				if (m_cd.scn_data[m_cd.cur_scn_no].read_flag[i] == 0)	{
					ci.lv_read_flag.set_item_select(i, true);
					ci.lv_read_flag.set_item_focus(i, true);
					ci.lv_read_flag.set_item_visible(i);
					return true;
				}
			}
			for (int i = 0; i < focused_pos; i++)	{
				if (m_cd.scn_data[m_cd.cur_scn_no].read_flag[i] == 0)	{
					ci.lv_read_flag.set_item_select(i, true);
					ci.lv_read_flag.set_item_focus(i, true);
					ci.lv_read_flag.set_item_visible(i);
					return true;
				}
			}
		}
	}

	return true;
}

// 通知
bool C_db_wnd_info_read_flag::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	if (false);
	else if (hdr->idFrom == IDC_LV_SCN)	{
		NMLISTVIEW* p_lv = (NMLISTVIEW *)lp;

		if (false);
		else if (p_lv->hdr.code == LVN_ITEMCHANGED)	{	// シーンを変更した
			m_cd.cur_scn_no = p_lv->iItem;
		}
		else if (p_lv->hdr.code == NM_DBLCLK)	{		// シーンをダブルクリックした

			// 選択状態取得
			int select_cnt = ci.lv_scn.get_selected_item_cnt();
			int select_no = ci.lv_scn.get_selected_item_no();

			// マウスカーソルの乗っているセル番号
			int onmouse_item_no, onmouse_column_no;
			C_point mp = ci.lv_scn.screen_to_client(get_mouse_pos());
			ci.lv_scn.get_cell_by_point(mp.x, mp.y, &onmouse_item_no, &onmouse_column_no);

			// シーンを開く
			if (select_cnt == 1 && select_no == onmouse_item_no)	{

				// ss フォルダやエディタパスが設定されていない場合
				if (Gp_config->ss_path.empty() || Gp_config->editor_path.empty())	{

					// デバッグ設定ダイアログを開く
					tnm_dlg_open_debug_setting_dialog();
				}
				// その他
				else	{
					// エディタで ss を開く
					int scn_no = select_no;
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
					open_file_by_editor(ss_path, 1, Gp_config->editor_path);	// １行目
				}
			}
		}
	}
	else if (hdr->idFrom == IDC_LV_READ_FLAG)	{
		NMLISTVIEW* p_lv = (NMLISTVIEW *)lp;

		if (false);
		else if (p_lv->hdr.code == LVN_ITEMCHANGED)	{	// アイテムの状態を変更した

			if (((p_lv->uOldState >> 12) == 1) && ((p_lv->uNewState >> 12) > 1))	{		// チェックされた
				(*Gp_read_flag)[m_cd.cur_scn_no][p_lv->iItem] = 1;
			}
			else if (((p_lv->uOldState >> 12) > 1) && ((p_lv->uNewState >> 12) == 1))	{	// チェックがはずされた
				(*Gp_read_flag)[m_cd.cur_scn_no][p_lv->iItem] = 0;
			}
		}
		else if (p_lv->hdr.code == NM_DBLCLK)	{		// 既読フラグをダブルクリックした

			// 選択状態取得
			int select_cnt = ci.lv_read_flag.get_selected_item_cnt();
			int select_no = ci.lv_read_flag.get_selected_item_no();

			// マウスカーソルの乗っているセル番号
			int onmouse_item_no, onmouse_column_no;
			C_point mp = ci.lv_read_flag.screen_to_client(get_mouse_pos());
			ci.lv_read_flag.get_cell_by_point(mp.x, mp.y, &onmouse_item_no, &onmouse_column_no);

			// シーンを開く
			if (select_cnt == 1 && select_no == onmouse_item_no)	{

				// ss フォルダやエディタパスが設定されていない場合
				if (Gp_config->ss_path.empty() || Gp_config->editor_path.empty())	{

					// デバッグ設定ダイアログを開く
					tnm_dlg_open_debug_setting_dialog();
				}
				// その他
				else	{

					// エディタで ss を開く
					int scn_no = m_cd.cur_scn_no;
					int line_no = Gp_lexer->get_read_flag_line_no(scn_no, select_no);
					TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
					TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
					open_file_by_editor(ss_path, line_no, Gp_config->editor_path);
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_read_flag::load_state()
{
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_read_flag::save_state()
{
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_db_wnd_info_read_flag::frame()
{
	if (!m_h_wnd)
		return;

	if (m_first)	{

		// シーンリストビューを構築
		int scn_cnt = Gp_lexer->get_scn_cnt();
		ci.lv_scn.set_item_cnt(scn_cnt);
		for (int i = 0; i < scn_cnt; i++)	{
			TSTR scn_name = Gp_lexer->get_scn_name(i);
			ci.lv_scn.set_cell_text(i, 0, scn_name);
		}

		// 初期化
		m_cd.scn_data.resize(scn_cnt);
		m_cd.cur_scn_no = 0;
	}

	// 全シーンの既読データを取得
	int total_flag_cnt = 0, total_read_cnt = 0;
	int scn_cnt = Gp_lexer->get_scn_cnt();
	for (int i = 0; i < scn_cnt; i++)	{
		m_cd.scn_data[i].read_flag = (*Gp_read_flag)[i];
		m_cd.scn_data[i].flag_cnt = (int)(*Gp_read_flag)[i].size();
		m_cd.scn_data[i].read_cnt = 0;
		for (int j = 0; j < m_cd.scn_data[i].flag_cnt; j++)	{
			m_cd.scn_data[i].read_cnt += (*Gp_read_flag)[i][j];
		}

		// シーンビューを更新
		if (m_first || m_cd.scn_data[i].read_cnt != m_ld.scn_data[i].read_cnt)	{
			TSTR str = m_cd.scn_data[i].read_cnt != m_cd.scn_data[i].flag_cnt ? _T("★") : _T("　　");
			ci.lv_scn.set_cell_text(i, 1, str);
			str = tostr(m_cd.scn_data[i].flag_cnt - m_cd.scn_data[i].read_cnt) + _T(" / ") + tostr(m_cd.scn_data[i].flag_cnt);
			ci.lv_scn.set_cell_text(i, 2, str);
		}

		total_flag_cnt += m_cd.scn_data[i].flag_cnt;
		total_read_cnt += m_cd.scn_data[i].read_cnt;
	}
	
	// 既読フラグビューを構築
	if (m_first || m_ld.cur_scn_no != m_cd.cur_scn_no)
		create_read_flag_view();

	// 現在のシーンの既読フラグを取得
	int rf_cnt = Gp_lexer->get_read_flag_cnt(m_cd.cur_scn_no);
	m_cd.read_flag.resize(rf_cnt);
	for (int i = 0; i < rf_cnt; i++)
		m_cd.read_flag[i] = (int)(*Gp_read_flag)[m_cd.cur_scn_no][i];

	// 既読フラグビューを更新
	for (int i = 0; i < rf_cnt; i++)	{
		bool change = m_first || i >= (int)m_ld.read_flag.size();
		if (change || m_cd.read_flag[i] != m_ld.read_flag[i])	{
			ci.lv_read_flag.set_item_check(i, m_cd.read_flag[i] > 0);
		}
	}

	// ステータス
	int cur_scn_flag_cnt = m_cd.scn_data[m_cd.cur_scn_no].flag_cnt;
	int cur_scn_read_cnt = m_cd.scn_data[m_cd.cur_scn_no].read_cnt;

	m_cd.status = Gp_lexer->get_scn_name(m_cd.cur_scn_no) + _T(": ") + tostr(cur_scn_read_cnt) + _T(" / ") + tostr(cur_scn_flag_cnt);
	m_cd.status += _T(" 全体: ") + tostr(total_read_cnt) + _T(" / ") + tostr(total_flag_cnt) + _T(" (") + tostr(total_read_cnt * 100 / total_flag_cnt) + _T("%) 残り") + tostr(total_flag_cnt - total_read_cnt);
	if (m_first || m_ld.status != m_cd.status)
		ci.stc_status.set_text(m_cd.status);

	// 更新
	m_ld = m_cd;
	m_first = false;
}

// ****************************************************************
// 既読フラグビューを構築
// ================================================================
void C_db_wnd_info_read_flag::create_read_flag_view()
{
	ci.lv_read_flag.clear();
	m_ld.read_flag.clear();

	int rf_cnt = Gp_lexer->get_read_flag_cnt(m_cd.cur_scn_no);
	ci.lv_read_flag.set_item_cnt(rf_cnt);
	for (int i = 0; i < rf_cnt; i++)	{
		int line_no = Gp_lexer->get_read_flag_line_no(m_cd.cur_scn_no, i);
		ci.lv_read_flag.set_cell_text(i, 0, str_format(_T("%05d(%d)"), i, line_no));
	}
}
