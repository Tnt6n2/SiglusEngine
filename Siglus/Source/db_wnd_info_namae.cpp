#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_namae.h"
#include	"dialog/db_wnd_info_namae.h"

// ****************************************************************
// デバッグ情報ウィンドウ：【名前】
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_namae")

// 初期化
void C_db_wnd_info_namae::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("【名前】情報"));

	// リストビュー項目
	listview_add_column(_T("namae_str"), _T("名前"), 100);
	listview_add_column(_T("change_str"), _T("変換名"), 100);
	listview_add_column(_T("exist"), _T("存在"), 60);
	listview_add_column(_T("moji_color_mod"), _T("モード"), 60);
	listview_add_column(_T("moji_color"), _T("文字色"), 60);
	listview_add_column(_T("shadow_color"), _T("影色"), 60);
}

// 初期化ファンクション
void C_db_wnd_info_namae::on_init_func()
{
	if (Gp_namae->namae_data_base_list.empty())
		return;

	int list_cnt = (int)Gp_namae->namae_data_base_list.size();
	for (int i = 0; i < list_cnt; i++)	{

		// 登録名
		lv.add_item(0, Gp_namae->namae_data_base_list[i].regist_name_str);

		// 登録状態
		lv.set_cell_text(i, 1, Gp_namae->namae_data_base_list[i].regist_flag ? Gp_namae->namae_data_base_list[i].change_name_str : _T("（★未登録）"));

		// 存在
		lv.set_cell_text(i, 2, Gp_namae->namae_data_base_list[i].exist_flag ? _T("○") : _T("（★存在しない）"));

		// 登録情報
		if (Gp_namae->namae_data_base_list[i].regist_flag)	{
			lv.set_cell_text(i, 3, tostr(Gp_namae->namae_data_base_list[i].color_mod));
			lv.set_cell_text(i, 4, tostr(Gp_namae->namae_data_base_list[i].moji_color_no));
			lv.set_cell_text(i, 5, tostr(Gp_namae->namae_data_base_list[i].shadow_color_no));
		}
	}
}

// 閉じるファンクション
void C_db_wnd_info_namae::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_namae::frame()
{
	if (!m_h_wnd)
		return;

	// 初回フラグをおろす
	frame_first_flag = false;
}
