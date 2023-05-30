#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_btn.h"
#include	"dialog/db_wnd_info_button.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ボタン
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_button")

// ****************************************************************
// デバッグ情報ウィンドウ：ボタン
// ================================================================

// 初期化
void C_db_wnd_info_btn::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ボタンマネージャ情報"));

	// リストビュー項目
	listview_add_column(_T("state"), _T("ステート"), 200);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_btn::on_init_func()
{
	// 項目を追加
	lv.add_item(0, _T("ボタンの個数"));
	lv.add_item(0, _T("アクティブなボタンのGUID"));
	lv.add_item(0, _T("アクティブなボタンのオーダー"));
	lv.add_item(0, _T("アクティブなボタンのレイヤー"));
	lv.add_item(0, _T("アクティブなボタンのステート"));
	lv.add_item(0, _T("今当たった"));
	lv.add_item(0, _T("今押された"));
}

// 閉じるファンクション
void C_db_wnd_info_btn::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_btn::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;
	int item_no = -1;

	// 現在の値を取得
	cd.btn_cnt = Gp_btn_mng->get_btn_cnt();
	cd.active_btn_guid = Gp_btn_mng->get_active_btn_guid();
	cd.active_btn_sorter = Gp_btn_mng->get_active_btn_sorter();
	cd.active_btn_state = Gp_btn_mng->get_active_btn_state();
	cd.hit_now = Gp_btn_mng->is_hit_this_frame();
	cd.pushed_now = Gp_btn_mng->is_pushed_this_frame();

	// ボタンの個数
	item_no ++;
	if (frame_first_flag || cd.btn_cnt != m_ld.btn_cnt)
		lv.set_cell_text(item_no, 1, tostr(cd.btn_cnt));

	// アクティブなボタンのGUID
	item_no ++;
	if (frame_first_flag || cd.active_btn_guid != m_ld.active_btn_guid)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_guid));

	// アクティブなボタンのオーダー
	item_no ++;
	if (frame_first_flag || cd.active_btn_sorter.order != m_ld.active_btn_sorter.order)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_sorter.order));

	// アクティブなボタンのレイヤー
	item_no ++;
	if (frame_first_flag || cd.active_btn_sorter.layer != m_ld.active_btn_sorter.layer)
		lv.set_cell_text(item_no, 1, tostr(cd.active_btn_sorter.layer));

	// アクティブなボタンのステート
	item_no ++;
	if (frame_first_flag || cd.active_btn_state != m_ld.active_btn_state)	{
		switch (cd.active_btn_state)	{
			case TNM_BTN_STATE_NORMAL:	lv.set_cell_text(item_no, 1, _T("通常"));			break;
			case TNM_BTN_STATE_HIT:		lv.set_cell_text(item_no, 1, _T("当たっている"));	break;
			case TNM_BTN_STATE_PUSH:	lv.set_cell_text(item_no, 1, _T("押している"));		break;
			default:					lv.set_cell_text(item_no, 1, _T("???"));			break;
		}
	}

	// 今回
	item_no ++;
	if (frame_first_flag || cd.hit_now != m_ld.hit_now)
		lv.set_cell_text(item_no, 1, cd.hit_now ? _T("当たった") : _T(""));

	item_no ++;
	if (frame_first_flag || cd.pushed_now != m_ld.pushed_now)
		lv.set_cell_text(item_no, 1, cd.pushed_now ? _T("押した") : _T(""));

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

