#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_form_code.h"
#include	"data/tnm_local_data.h"
#include	"dialog/db_wnd_info_proc_stack.h"


#define PROC(def, id, key)	_T(#def),

const TCHAR* G_proc_name[] = 
{
#include	"data/tnm_proc_def_00.h"
};

#undef PROC


// ****************************************************************
// デバッグ情報ウィンドウ：プロセススタック
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_proc_stack")

// 初期化
void C_db_wnd_info_proc_stack::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("プロセススタック情報"));

	// リストビュー項目
	listview_add_column(_T("proc"), _T("プロセス"), 200);
}

// 初期化ファンクション
void C_db_wnd_info_proc_stack::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_proc_stack::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_proc_stack::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在のデータを作る
	cd.item_list.resize(Gp_local->proc_stack.size() + 1);
	for (int i = 0; i < (int)Gp_local->proc_stack.size(); i++)	{
		cd.item_list[i].proc = Gp_local->proc_stack[i].type;
	}
	cd.item_list[Gp_local->proc_stack.size()].proc = Gp_local->cur_proc.type;

	// リストビューのアイテム数を調整する
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		int col_no = -1;
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// プロセス
		col_no ++;
		TSTR proc_name;
		if (change || cd.item_list[i].proc != m_ld.item_list[i].proc)	{
			if (0 <= cd.item_list[i].proc && cd.item_list[i].proc < sizeof_array(G_proc_name))	{
				proc_name = G_proc_name[cd.item_list[i].proc];
				lv.set_cell_text(i, col_no, proc_name);
			}
		}
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}

