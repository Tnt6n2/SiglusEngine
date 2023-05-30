#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"engine/eng_bgm_table.h"
#include	"dialog/db_wnd_info_bgm_table.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ＢＧＭテーブル
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_bgm_table")

// 初期化
void C_db_wnd_info_bgm_table::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ＢＧＭテーブル"));

	// リストビュー項目
	listview_add_column(_T("name"), _T("登録名"), 100);
	listview_add_column(_T("flag"), _T("ﾌﾗｸﾞ"), 50);
}

// 初期化ファンクション
void C_db_wnd_info_bgm_table::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_bgm_table::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_bgm_table::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在のデータを作る
	int flag_cnt = tnm_bgm_table_get_bgm_cnt();
	cd.flag_list.resize(flag_cnt);
	for (int i = 0; i < flag_cnt; i++)	{
		cd.flag_list[i] = tnm_bgm_table_get_listened(i) ? 1 : 0;
	}

	// リストビューのアイテム数を調整する
	if (frame_first_flag || cd.flag_list.size() != m_ld.flag_list.size())
		lv.set_item_cnt((int)cd.flag_list.size());

	// 増えた分に関して登録名を表示
	for (int i = frame_first_flag ? 0 : (int)m_ld.flag_list.size(); i < (int)cd.flag_list.size(); i++)
		lv.set_cell_text(i, 0, Gp_ini->bgm[i].regist_name);

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.flag_list.size(); i++)	{
		int column_no = 0;

		// カウンタ
		column_no ++;
		if (frame_first_flag || i >= (int)m_ld.flag_list.size() || cd.flag_list[i] != m_ld.flag_list[i])
			lv.set_cell_text(i, column_no, cd.flag_list[i] ? _T("●") : _T("×"));
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

