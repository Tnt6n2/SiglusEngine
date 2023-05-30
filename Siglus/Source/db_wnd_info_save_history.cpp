#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_save.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_save_history.h"

// ****************************************************************
// デバッグ情報ウィンドウ：システム
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_save_history")

// 初期化
void C_db_wnd_info_save_history::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("セーブポイント履歴"));

	// リストビュー項目
	listview_add_column(_T("state"), _T(""), 25);
	listview_add_column(_T("save_pont"), _T("セーブポイント"), 160);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_save_history::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}
	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// 初期化ファンクション
void C_db_wnd_info_save_history::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_save_history::on_close_func()
{
}

// ＮＯＴＩＦＹ
bool C_db_wnd_info_save_history::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* nm = (NMHDR*)lp;
	if (nm->code == NM_DBLCLK)	{
		NM_LISTVIEW* nmlv = (NM_LISTVIEW*)lp;
		if (nmlv->hdr.hwndFrom == lv.get_handle())	{
			load(nmlv->iItem);
		}
	}
	return true;
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_save_history::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在の値を取得する
	cd.m_save_history_top = Gp_save_history->top;
	cd.m_save_history_cnt = Gp_save_history->cnt;

	// 項目を順に設定する
	int item_no = -1;

	// 履歴リスト更新
	item_no ++;
	if (frame_first_flag || Gp_save_history->update_flag || cd.m_save_history_top != m_ld.m_save_history_top || cd.m_save_history_cnt != m_ld.m_save_history_cnt)	{

		// リスト数調整
		if (lv.get_item_cnt() != cd.m_save_history_cnt)	{
			lv.set_item_cnt(cd.m_save_history_cnt);
		}
		else if (cd.m_save_history_top != m_ld.m_save_history_top)	{
			lv.delete_item(0);
			lv.add_item();
		}

		// リスト設定
		if (cd.m_save_history_cnt > 0)	{
			int index = cd.m_save_history_top;
			SYSTEMTIME *time;
			for (int i = 0; i < cd.m_save_history_cnt; i++)	{
				switch (Gp_save_history->item[index].flag)	{
					case 0:		lv.set_cell_text(i, 0, _T(""));		break;
					case 1:		lv.set_cell_text(i, 0, _T("☆"));	break;
					case 2:		lv.set_cell_text(i, 0, _T("★"));	break;
				}
				time = &Gp_save_history->item[index].systemtime;
				TSTR str = str_format(_T("%04d/%02d/%02d (%02d:%02d:%02d)"), time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
				if (!Gp_save_history->item[index].local_save.save_msg.empty())	{
					str += _T("　") + Gp_save_history->item[index].local_save.save_msg;
				}
				lv.set_cell_text(i, 1, str);
				index++;
				if (index >= cd.m_save_history_cnt)	{
					index = 0;
				}
			}
			index--;
			if (index < 0)	{
				index = cd.m_save_history_cnt - 1;
			}
			lv.set_item_visible(cd.m_save_history_cnt - 1);
		}

		Gp_save_history->update_flag = false;
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

// ****************************************************************
// ロード
// ================================================================
void C_db_wnd_info_save_history::load(int click_index)
{
	if (Gp_save_history->cnt <= 0 || click_index < 0 || Gp_save_history->cnt <= click_index)
		return;

	// ロードする履歴番号
	int index = (Gp_save_history->top + click_index) % Gp_save_history->cnt;

	// フラグ処理
	if (Gp_save_history->item[index].flag != 2)	{
		for (int i = 0; i < Gp_save_history->cnt; i++)	{
			if (Gp_save_history->item[i].flag > 0)	{
				Gp_save_history->item[i].flag--;
			}
		}
	}

	// 履歴セットフラグを立てる
	Gp_save_history->update_flag = true;
	Gp_save_history->item[index].flag = 2;

	// ロード（デバッグ用：簡易版）
	tnm_saveload_proc_load_save_history(&Gp_save_history->item[index].local_save);

	// ２．簡易ロードの後処理
	tnm_push_proc(TNM_PROC_TYPE_EASY_LOAD);

	// １．描画を行う
	tnm_push_proc(TNM_PROC_TYPE_DISP);
}

