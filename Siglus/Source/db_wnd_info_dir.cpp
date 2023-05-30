#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"engine/eng_dir.h"
#include	"dialog/db_wnd_info_dir.h"

// ****************************************************************
// デバッグ情報ウィンドウ：システム
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_dir")

// 初期化
void C_db_wnd_info_dir::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ディレクトリ情報"));

	// リストビュー項目
	listview_add_column(_T("state"), _T("ステート"), 100);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_dir::on_init_func()
{
	// 項目を追加
	lv.add_item(0, _T("セットアップ"));
	lv.add_item(0, _T("実行フォルダ"));
	lv.add_item(0, _T("データフォルダ"));
	lv.add_item(0, _T("作業フォルダ"));
	lv.add_item(0, _T("セーブフォルダ"));
}

// 閉じるファンクション
void C_db_wnd_info_dir::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_dir::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在の値を取得する
	cd.setup_type = Gp_global->setup_type;
	cd.exe_dir = Gp_dir->exe_dir;
	cd.base_dir = Gp_dir->base_dir;
	cd.work_dir = Gp_dir->work_dir;
	cd.save_dir = Gp_dir->save_dir;

	// 項目を順に設定する
	int item_no = -1;

	// セットアップタイプ
	item_no ++;
	if (frame_first_flag || cd.setup_type != m_ld.setup_type)	{
		switch (cd.setup_type)	{
			case TNM_SETUP_TYPE_FULL:	lv.set_cell_text(item_no, 1, _T("完全セットアップ"));			break;
			case TNM_SETUP_TYPE_MIN:	lv.set_cell_text(item_no, 1, _T("最小セットアップ"));			break;
			default:					lv.set_cell_text(item_no, 1, _T("セットアップされていません"));	break;
		}
	}

	item_no ++;
	if (frame_first_flag || cd.exe_dir != m_ld.exe_dir)
		lv.set_cell_text(item_no, 1, cd.exe_dir);
	item_no ++;
	if (frame_first_flag || cd.base_dir != m_ld.base_dir)
		lv.set_cell_text(item_no, 1, cd.base_dir);
	item_no ++;
	if (frame_first_flag || cd.work_dir != m_ld.work_dir)
		lv.set_cell_text(item_no, 1, cd.work_dir);
	item_no ++;
	if (frame_first_flag || cd.save_dir != m_ld.save_dir)
		lv.set_cell_text(item_no, 1, cd.save_dir);

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}
