#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_group.h"
#include	"element/elm_stage.h"
#include	"element/elm_excall.h"
#include	"dialog/db_wnd_info_group.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// デバッグ情報ウィンドウ：グループ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_group")

// 初期化
void C_db_wnd_info_group::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("グループ情報"));
}

// 初期化ファンクション
void C_db_wnd_info_group::on_init_func()
{
	// リストビューを初期化
	tnm_init_lv_element(FM_GROUPLIST, true, get_lv());
}

// 閉じるファンクション
void C_db_wnd_info_group::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_group::frame(C_elm_group_list** p_group_list_list, int list_cnt)
{
	if (!m_h_wnd)
		return;

	// フロントのグループ情報を表示する
	int item_no = 0;
	int stage_no = TNM_STAGE_FRONT;
	tnm_update_lv_list_table(&(*Gp_stage_list)[stage_no].m_group_list, get_lv(), &item_no, false);
	if (Gp_excall->is_ready())	{	// ＥＸコール中はＥＸグループも表示
		tnm_update_lv_list_table(&Gp_excall->m_stage_list[stage_no].m_group_list, get_lv(), &item_no, false);
	}
	tnm_reduce_item_cnt(get_lv(), item_no);
}

