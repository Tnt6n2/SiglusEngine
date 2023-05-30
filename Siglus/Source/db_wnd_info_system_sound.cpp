#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_sound.h"
#include	"dialog/db_wnd_info_system_sound.h"


// ****************************************************************
// デバッグ情報ウィンドウ：システム：サウンド
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_system_sound")

// 初期化
void C_db_wnd_info_system_sound::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ボリュームチャンネル"));

	// リストビュー項目
	listview_add_column(_T("type"), _T("ﾀｲﾌﾟ"), 100);
	listview_add_column(_T("volume"), _T("ﾎﾞﾘｭｰﾑ"), 100);
	listview_add_column(_T("check"), _T("ﾁｪｯｸ"), 50);
}

// 初期化ファンクション
void C_db_wnd_info_system_sound::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_system_sound::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_system_sound::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// サイズは固定
	cd.item_list.resize(22);

	// リストビューのアイテム数を調整する
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// アイテム名を設定する
	if (frame_first_flag)	{
		lv.set_cell_text(0, 0, _T("-1（全体）"));
		lv.set_cell_text(1, 0, _T("0（BGM）"));
		lv.set_cell_text(2, 0, _T("1（KOE）"));
		lv.set_cell_text(3, 0, _T("2（PCM）"));
		lv.set_cell_text(4, 0, _T("3（SE）"));
		lv.set_cell_text(5, 0, _T("4（MOV）"));
		lv.set_cell_text(6, 0, _T("16（汎用）"));
		lv.set_cell_text(7, 0, _T("17（汎用）"));
		lv.set_cell_text(8, 0, _T("18（汎用）"));
		lv.set_cell_text(9, 0, _T("19（汎用）"));
		lv.set_cell_text(10, 0, _T("20（汎用）"));
		lv.set_cell_text(11, 0, _T("21（汎用）"));
		lv.set_cell_text(12, 0, _T("22（汎用）"));
		lv.set_cell_text(13, 0, _T("23（汎用）"));
		lv.set_cell_text(14, 0, _T("24（汎用）"));
		lv.set_cell_text(15, 0, _T("25（汎用）"));
		lv.set_cell_text(16, 0, _T("26（汎用）"));
		lv.set_cell_text(17, 0, _T("27（汎用）"));
		lv.set_cell_text(18, 0, _T("28（汎用）"));
		lv.set_cell_text(19, 0, _T("29（汎用）"));
		lv.set_cell_text(20, 0, _T("30（汎用）"));
		lv.set_cell_text(21, 0, _T("31（汎用）"));
	}

	// 現在のデータを作る
	for (int i = 0; i < 22; i++)	{
		if (i == 0)	{
			cd.item_list[0].volume = Gp_config->all_sound_user_volume;
			cd.item_list[0].check = Gp_config->play_all_sound_check;
		}
		else if (1 <= i && i <= 5)	{
			cd.item_list[i].volume = Gp_config->sound_user_volume[i - 1];
			cd.item_list[i].check = Gp_config->play_sound_check[i - 1];
		}
		else if (6 <= i && i <= 21)	{
			cd.item_list[i].volume = Gp_config->sound_user_volume[i + 10];
			cd.item_list[i].check = Gp_config->play_sound_check[i + 10];
		}
	}

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// ボリューム
		if (change || cd.item_list[i].volume != m_ld.item_list[i].volume)	{
			lv.set_cell_text(i, 1, tostr(cd.item_list[i].volume));
		}
		// 再生
		if (change || cd.item_list[i].check != m_ld.item_list[i].check)	{
			lv.set_cell_text(i, 2, cd.item_list[i].check ? _T("オン") : _T("オフ"));
		}
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}
