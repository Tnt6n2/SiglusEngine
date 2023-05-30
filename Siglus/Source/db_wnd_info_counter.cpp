#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_counter.h"
#include	"element/elm_excall.h"
#include	"dialog/db_wnd_info_counter.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// デバッグ情報ウィンドウ：カウンタ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_counter")

// 初期化
void C_db_wnd_info_counter::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("カウンタ情報"));
}

// 初期化ファンクション
void C_db_wnd_info_counter::on_init_func()
{
	// リストビュー項目
	tnm_init_lv_element(FM_COUNTERLIST, true, &lv);
}

// 閉じるファンクション
void C_db_wnd_info_counter::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_counter::frame()
{
	if (!m_h_wnd)
		return;

	// リストビューを更新
	int item_no = 0;
	tnm_update_lv_list_table(Gp_counter_list, &lv, &item_no, false);
	if (Gp_excall->is_ready())	{	// ＥＸコール中はＥＸカウンタも表示
		tnm_update_lv_list_table(&Gp_excall->m_counter_list, &lv, &item_no, false);
	}
	tnm_reduce_item_cnt(get_lv(), item_no);
}

