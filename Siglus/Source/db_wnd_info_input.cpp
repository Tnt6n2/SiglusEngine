#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"engine/eng_input.h"
#include	"dialog/db_wnd_info_input.h"

// ****************************************************************
// デバッグ情報ウィンドウ：入力
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_input")

// ****************************************************************
// デバッグ情報ウィンドウ：入力
// ================================================================

// 初期化
void C_db_wnd_info_input::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("入力情報"));

	// リストビュー項目
	listview_add_column(_T("state"), _T("ステート"), 100);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_input::on_init_func()
{
	lv.add_item(0, _T("（システム）マウス位置"));
	lv.add_item(0, _T("（システム）マウス左押した"));
	lv.add_item(0, _T("（システム）マウス左放した"));
	lv.add_item(0, _T("（システム）マウス左押して放した"));
	lv.add_item(0, _T("（システム）マウス左押している"));
	lv.add_item(0, _T("（システム）マウス右押した"));
	lv.add_item(0, _T("（システム）マウス右放した"));
	lv.add_item(0, _T("（システム）マウス右押して放した"));
	lv.add_item(0, _T("（システム）マウス右押している"));
	lv.add_item(0, _T("（スクリプト）マウス位置"));
	lv.add_item(0, _T("（スクリプト）マウス左押した"));
	lv.add_item(0, _T("（スクリプト）マウス左放した"));
	lv.add_item(0, _T("（スクリプト）マウス左押して放した"));
	lv.add_item(0, _T("（スクリプト）マウス左押している"));
	lv.add_item(0, _T("（スクリプト）マウス右押した"));
	lv.add_item(0, _T("（スクリプト）マウス右放した"));
	lv.add_item(0, _T("（スクリプト）マウス右押して放した"));
	lv.add_item(0, _T("（スクリプト）マウス右押している"));
}

// 閉じるファンクション
void C_db_wnd_info_input::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_input::frame()
{
	if (!m_h_wnd)
		return;

	// 現在のデータを作る
	STATE cd;
	cd.mouse_pos = Gp_cur_input->mouse.pos;
	cd.mouse_left_down = Gp_input->now.mouse.left.get_down_stock();
	cd.mouse_left_up = Gp_input->now.mouse.left.get_up_stock();
	cd.mouse_left_down_up = Gp_input->now.mouse.left.get_down_up_stock();
	cd.mouse_left_state = Gp_input->now.mouse.left.get_real_state();
	cd.mouse_right_down = Gp_input->now.mouse.right.get_down_stock();
	cd.mouse_right_up = Gp_input->now.mouse.right.get_up_stock();
	cd.mouse_right_down_up = Gp_input->now.mouse.right.get_down_up_stock();
	cd.mouse_right_state = Gp_input->now.mouse.right.get_real_state();
	cd.script_mouse_pos = Gp_script_input->now.mouse.pos;
	cd.script_mouse_left_down = Gp_script_input->now.mouse.left.get_down_stock();
	cd.script_mouse_left_up = Gp_script_input->now.mouse.left.get_up_stock();
	cd.script_mouse_left_down_up = Gp_script_input->now.mouse.left.get_down_up_stock();
	cd.script_mouse_left_state = Gp_script_input->now.mouse.left.get_real_state();
	cd.script_mouse_right_down = Gp_script_input->now.mouse.right.get_down_stock();
	cd.script_mouse_right_up = Gp_script_input->now.mouse.right.get_up_stock();
	cd.script_mouse_right_down_up = Gp_script_input->now.mouse.right.get_down_up_stock();
	cd.script_mouse_right_state = Gp_script_input->now.mouse.right.get_real_state();

	int item_no = -1;

	// マウス
	item_no ++;
	if (frame_first_flag || cd.mouse_pos != m_ld.mouse_pos)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_pos));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_down != m_ld.mouse_left_down)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_down));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_up != m_ld.mouse_left_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_down_up != m_ld.mouse_left_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_down_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_state != m_ld.mouse_left_state)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_state));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_down != m_ld.mouse_right_down)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_down));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_up != m_ld.mouse_right_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_down_up != m_ld.mouse_right_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_down_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_state != m_ld.mouse_right_state)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_state));

	// スクリプトのマウス
	item_no ++;
	if (frame_first_flag || cd.script_mouse_pos != m_ld.script_mouse_pos)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_pos));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_down != m_ld.script_mouse_left_down)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_down));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_up != m_ld.script_mouse_left_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_down_up != m_ld.script_mouse_left_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_down_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_state != m_ld.script_mouse_left_state)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_state));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_down != m_ld.script_mouse_right_down)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_down));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_up != m_ld.script_mouse_right_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_down_up != m_ld.script_mouse_right_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_down_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_state != m_ld.script_mouse_right_state)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_state));

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

