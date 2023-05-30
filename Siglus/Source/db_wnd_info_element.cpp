#include	"pch.h"
#include	"ifc_eng.h"
#include	"dialog/db_wnd_info_element.h"
#include	"dialog/db_wnd_info_element_lv.h"

#include	"element/elm_list.h"
#include	"element/elm_flag.h"
#include	"element/elm_mask.h"
#include	"element/elm_stage.h"
#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_screen.h"
#include	"element/elm_world.h"
#include	"element/elm_sound.h"
#include	"element/elm_pcm_event.h"
#include	"element/elm_frame_action.h"
#include	"element/elm_call.h"
#include	"element/elm_excall.h"

#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_mask.h"
#include	"engine/ifc_sound.h"

#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：エレメント
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_element")

// 初期化
void C_db_wnd_info_element::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_ELEMENT);

	// 初期化
	h_selected_item = NULL;
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_element::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_element::on_init()
{
	// アイテム登録
	bind_control(&tv_element, IDC_TR_ELEMENT, _T("tv_element"), 4);
	bind_control(&lv_element, IDC_LV_ELEMENT, _T("lv_element"), 5);
	bind_control(&lv_list, IDC_LV_LIST, _T("lv_list"), 2);
	bind_control(&lv_int_list, IDC_LV_INT_LIST, _T("lv_int_list"), 2);
	bind_control(&lv_str_list, IDC_LV_STR_LIST, _T("lv_str_list"), 2);
	bind_control(&lv_counter_list, IDC_LV_COUNTER_LIST, _T("lv_counter_list"), 2);
	bind_control(&lv_mask_list, IDC_LV_MASK_LIST, _T("lv_mask_list"), 2);
	bind_control(&lv_object_list, IDC_LV_OBJECT_LIST, _T("lv_object_list"), 2);
	bind_control(&lv_group_list, IDC_LV_GROUP_LIST, _T("lv_group_list"), 2);
	bind_control(&lv_mwnd_list, IDC_LV_MWND_LIST, _T("lv_mwnd_list"), 2);
	bind_control(&lv_effect_list, IDC_LV_EFFECT_LIST, _T("lv_effect_list"), 2);
	bind_control(&lv_quake_list, IDC_LV_QUAKE_LIST, _T("lv_quake_list"), 2);
	bind_control(&lv_world_list, IDC_LV_WORLD_LIST, _T("lv_world_list"), 2);
	bind_control(&lv_pcmch_list, IDC_LV_PCMCH_LIST, _T("lv_pcmch_list"), 2);
	bind_control(&lv_pcm_event_list, IDC_LV_PCM_EVENT_LIST, _T("lv_pcm_event_list"), 2);
	bind_control(&lv_frame_action_list, IDC_LV_FRAME_ACTION_LIST, _T("lv_frame_action_list"), 2);
	bind_control(&lv_call_list, IDC_LV_CALL_LIST, _T("lv_call_list"), 2);
	bind_control(&btn_open_element, IDC_BTN_OPEN_ELEMENT, _T("btn_open_element"), 8);
	bind_control(&btn_open_list, IDC_BTN_OPEN_LIST, _T("btn_open_list"), 2);
	bind_separater(&separater_1, _T("separater_1"));
	bind_separater(&separater_2, _T("separater_2"));

	// ツリービューの設定
	tv_element.add_style(TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT | TVS_TRACKSELECT);

	// リストビューを登録
	m_lv_list.push_back(&lv_list);
	m_lv_list.push_back(&lv_int_list);
	m_lv_list.push_back(&lv_str_list);
	m_lv_list.push_back(&lv_counter_list);
	m_lv_list.push_back(&lv_mask_list);
	m_lv_list.push_back(&lv_object_list);
	m_lv_list.push_back(&lv_group_list);
	m_lv_list.push_back(&lv_mwnd_list);
	m_lv_list.push_back(&lv_effect_list);
	m_lv_list.push_back(&lv_quake_list);
	m_lv_list.push_back(&lv_world_list);
	m_lv_list.push_back(&lv_pcmch_list);
	m_lv_list.push_back(&lv_pcm_event_list);
	m_lv_list.push_back(&lv_frame_action_list);
	m_lv_list.push_back(&lv_call_list);

	// リストビューの大きさを整える
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin() + 1; itr != m_lv_list.end(); ++itr)
		(*itr)->set_def_rltv_rect(lv_list.get_rltv_rect());

	// サイズを整える（set_def_rltv_rect とセパレータの登録の間にこれが必要）
	send_wm_size();

	// デフォルトではリスト用のリストビューは表示されない
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		(*itr)->set_show(false);

	// リストビューのコラムを設定
	tnm_init_lv_element(FM_VOID, true, &lv_element);
	tnm_init_lv_element(FM_INTLIST, true, &lv_int_list);
	tnm_init_lv_element(FM_STRLIST, true, &lv_str_list);
	tnm_init_lv_element(FM_COUNTERLIST, true, &lv_counter_list);
	tnm_init_lv_element(FM_MASKLIST, true, &lv_mask_list);
	tnm_init_lv_element(FM_OBJECTLIST, true, &lv_object_list);
	tnm_init_lv_element(FM_GROUPLIST, true, &lv_group_list);
	tnm_init_lv_element(FM_MWNDLIST, true, &lv_mwnd_list);
	tnm_init_lv_element(FM_EFFECTLIST, true, &lv_effect_list);
	tnm_init_lv_element(FM_QUAKELIST, true, &lv_quake_list);
	tnm_init_lv_element(FM_WORLDLIST, true, &lv_world_list);
	tnm_init_lv_element(FM_PCMCHLIST, true, &lv_pcmch_list);
	tnm_init_lv_element(FM_PCMEVENTLIST, true, &lv_pcm_event_list);
	tnm_init_lv_element(FM_FRAMEACTIONLIST, true, &lv_frame_action_list);
	tnm_init_lv_element(FM_CALLLIST, true, &lv_call_list);

	// リストビューの設定を行う
	lv_element.easy_setting(true, true, false, true);
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		(*itr)->easy_setting(true, true, false, true);

	// セパレータ登録
	separater_1.set_hv_mod(true);
	separater_1.add_control_first(&tv_element);
	separater_1.add_control_second(&lv_element);
	separater_1.add_control_second(&btn_open_element);
	separater_1.add_control_second(&btn_open_list);
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		separater_1.add_control_second(*itr);
	separater_2.set_hv_mod(false);
	separater_2.add_control_first(&lv_element);
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		separater_2.add_control_second(*itr);

	// アイテム設定ロード
	load_ini();

	// アイテムフォーカス

	// ウィンドウ表示
	update();	set_show(true);

	return true;
}

// 閉じる
bool C_db_wnd_info_element::on_close()
{
	// ステータスの取得と保存
	save_ini();

	return true;
}

// コマンド
bool C_db_wnd_info_element::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_OPEN_ELEMENT:
			open_element_by_another_wnd();
			break;
		case IDC_BTN_OPEN_LIST:
			open_list_by_another_wnd();
			break;
	}
	return true;
}

// 通知
bool C_db_wnd_info_element::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* nmhdr = (NMHDR *)lp;

	if (nmhdr->hwndFrom == tv_element.get_handle())	{
		NM_TREEVIEW* nmtv = (NM_TREEVIEW *)lp;
		if (nmhdr->code == TVN_SELCHANGED)	{
			if (nmtv->itemNew.state & TVIS_SELECTED)	{
				h_selected_item = nmtv->itemNew.hItem;
				restruct_lv();
			}
		}
	}
	else if (nmhdr->hwndFrom == lv_element.get_handle())		on_notify_lv(&lv_element, wp, lp, false);		// 通常表示
	else if (nmhdr->hwndFrom == lv_int_list.get_handle())		on_notify_lv(&lv_int_list, wp, lp, true);		// テーブル表示
	else if (nmhdr->hwndFrom == lv_str_list.get_handle())		on_notify_lv(&lv_str_list, wp, lp, true);		// テーブル表示
	else if (nmhdr->hwndFrom == lv_counter_list.get_handle())	on_notify_lv(&lv_counter_list, wp, lp, true);	// テーブル表示
	else if (nmhdr->hwndFrom == lv_object_list.get_handle())	on_notify_lv(&lv_object_list, wp, lp, true);	// テーブル表示
	else if (nmhdr->hwndFrom == lv_effect_list.get_handle())	on_notify_lv(&lv_effect_list, wp, lp, true);	// テーブル表示
	else if (nmhdr->hwndFrom == lv_world_list.get_handle())		on_notify_lv(&lv_world_list, wp, lp, true);		// テーブル表示

	return true;
}

// 通知（リストビュー）
bool C_db_wnd_info_element::on_notify_lv(C_tnm_dbg_listview* p_lv, WPARAM wp, LPARAM lp, bool table_flag)
{
	NMHDR* nmhdr = (NMHDR *)lp;

	if (nmhdr->code == NM_DBLCLK)	{
		on_dblclk_lv(p_lv, table_flag);
	}
	else if (nmhdr->code == LVN_CHANGE_TEXT)	{
		on_change_lv_value(p_lv, ((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->column_no, ((NMLVCHANGE *)lp)->new_text, table_flag);
	}

	return true;
}

// ****************************************************************
// ini セーブ
// ================================================================
void C_db_wnd_info_element::save_ini()
{
	// 子ウィンドウの保存
	int wnd_no = 0;
	for (ARRAY< BSP<C_db_wnd_info_element_lv> >::iterator itr = m_opened_lv_list.begin(); itr != m_opened_lv_list.end(); ++itr)	{
		if ((*itr)->is_open())	{

			// エレメントコード
			TSTR code_key = str_format(_T("element_%04d_code"), wnd_no);
			TSTR code_str = tostr((*itr)->m_element_form);
			for (int i = 0; i < (*itr)->m_element.code_cnt; i++)	{
				code_str += _T(",") + tostr((*itr)->m_element.code[i]);
			}
			code_str += TSTR() + _T(":") + tostr((*itr)->m_bit);
			code_str += TSTR() + _T(":") + tostr((*itr)->m_top_index);
			code_str += TSTR() + _T(":") + ((*itr)->m_table_flag ? _T("1") : _T("0"));
			save_ini_str(code_key, code_str);

			// エレメント名前
			TSTR name_key = str_format(_T("element_%04d_name"), wnd_no);
			save_ini_str(name_key, (*itr)->m_element_name);

			// ウィンドウ矩形
			TSTR rect_key = str_format(_T("element_%04d_rect"), wnd_no);
			save_ini_rect(rect_key, (*itr)->get_window_rect());

			wnd_no ++;
		}
	}

	// 子ウィンドウ終端（過去のデータを消すために必須です。）
	TSTR name_key = str_format(_T("element_%04d_name"), wnd_no);
	save_ini_str(name_key, _T(""));

	// セパレータ設定セーブ
	separater_1.save_ini();
	separater_2.save_ini();

	// ウィンドウ矩形の保存
	save_ini_window_rect();
}

// ****************************************************************
// ini ロード
// ================================================================
void C_db_wnd_info_element::load_ini()
{
	// セパレータ設定ロード
	separater_1.load_ini();
	separater_2.load_ini();

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	
	// 子ウィンドウの読み込み
	for (int i = 0; ; i++)	{

		// エレメントコード
		TSTR code_key = str_format(_T("element_%04d_code"), i);
		TSTR code_str = load_ini_str(code_key, _T(""));
		if (code_str.empty())
			break;

		int num, bit = 32, top_index = 0;
		bool table_flag;
		S_element element;
		int element_form;
		C_str_analizer anl(code_str.begin(), code_str.end());
		if (anl.get_integer(&element_form))	{
			while (1)	{
				if (!anl.check_moji(_T(',')))	break;
				if (!anl.get_integer(&num))		break;
				element = element + S_element(num);
			}
		}
		if (!anl.check_moji(_T(':')))		break;
		if (!anl.get_integer(&bit))			break;
		if (!anl.check_moji(_T(':')))		break;
		if (!anl.get_integer(&top_index))	break;
		if (!anl.check_moji(_T(':')))		break;
		if (!anl.get_integer(&num))			break;
		table_flag = num ? true : false;

		// エレメント名
		TSTR name_key = str_format(_T("element_%04d_name"), i);
		TSTR element_name = load_ini_str(name_key, _T(""));

		// エレメント名が空ならそれは終端記号なのです
		if (element_name.empty())
			break;

		// エレメント矩形
		TSTR rect_key = str_format(_T("element_%04d_rect"), i);
		C_rect window_rect = load_ini_rect(rect_key, C_rect(0, 0, 0, 0));
		if (window_rect == C_rect(0, 0, 0, 0))
			break;

		// 別ウィンドウを作成
		BSP<C_db_wnd_info_element_lv> lv(new C_db_wnd_info_element_lv);
		m_opened_lv_list.push_back(lv);

		// ウィンドウの設定を行って開く
		lv->m_element = element;
		lv->m_element_form = element_form;
		lv->m_element_name = element_name;
		lv->m_bit = bit;
		lv->m_top_index = top_index;
		lv->m_table_flag = table_flag;
		lv->open();
		lv->set_text(lv->m_element_name);
		lv->set_window_rect(window_rect);

		// ウィンドウを初期化する
		tnm_init_lv_element(element_form, lv->m_table_flag, lv->get_lv());
	}
}

// ****************************************************************
// エレメント：リストエレメントどうかを判定
// ================================================================
bool tnm_form_is_list(int form)
{
	switch (form)	{
		case FM_INTLIST:
		case FM_INTEVENTLIST:
		case FM_STRLIST:
		case FM_COUNTERLIST:
		case FM_MASKLIST:
		case FM_STAGELIST:
		case FM_OBJECTLIST:
		case FM_GROUPLIST:
		case FM_MWNDLIST:
		case FM_EFFECTLIST:
		case FM_QUAKELIST:
		case FM_WORLDLIST:
		case FM_PCMCHLIST:
		case FM_PCMEVENTLIST:
		case FM_FRAMEACTIONLIST:
		case FM_CALLLIST:
			return true;
	}

	return false;
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_db_wnd_info_element::frame()
{
	// 自身のウィンドウを更新する
	if (is_open())	{

		// ツリービューを更新する
		update_tv();

		// リストビューを更新する
		update_lv();
	}

	// 別ウィンドウで開いているリストビューを更新する
	for (int i = 0; i < (int)m_opened_lv_list.size(); i++)	{

		int item_no = 0, column_no = 1;

		// エレメントの実体を取得
		S_tnm_command_proc_arg_struct arg;
		arg.disp_out_of_range_error = false;
		tnm_command_proc(m_opened_lv_list[i]->m_element.code, m_opened_lv_list[i]->m_element.code + m_opened_lv_list[i]->m_element.code_cnt, &arg);
		C_tnm_element* p_element = (C_tnm_element *)arg.p_ret_element;

		// エレメント個別情報
		if (m_opened_lv_list[i]->m_table_flag)	{
			tnm_update_lv_element_table(p_element, 
				m_opened_lv_list[i]->m_element_form, 
				m_opened_lv_list[i]->m_bit, 
				m_opened_lv_list[i]->m_top_index, 
				m_opened_lv_list[i].get()->get_lv(), 
				&item_no);
		}
		else	{
			tnm_update_lv_element(p_element, 
				m_opened_lv_list[i]->m_element_form, 
				m_opened_lv_list[i]->m_bit, 
				m_opened_lv_list[i]->m_top_index, 
				m_opened_lv_list[i].get()->get_lv(), 
				&item_no, &column_no, &item_no);
		}
	}
}

// ****************************************************************
// ツリーを更新する
// ================================================================
void C_db_wnd_info_element::update_tv()
{
	// グローバル
	update_tv_global(TVI_ROOT, true);
}

// グローバル
void C_db_wnd_info_element::update_tv_global(HTREEITEM h_item, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// Ａフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->A, is_opened);

	// Ｂフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->B, is_opened);

	// Ｃフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->C, is_opened);

	// Ｄフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->D, is_opened);

	// Ｅフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->E, is_opened);

	// Ｆフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->F, is_opened);

	// Ｘフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->X, is_opened);

	// Ｓフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->S, is_opened);

	// Ｇフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->G, is_opened);

	// Ｚフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->Z, is_opened);

	// Ｍフラグ
	h_child = update_tv_element(h_item, h_child, &Gp_flag->M, is_opened);

	// Ｌフラグ
	h_child = update_tv_element(h_item, h_child, &tnm_get_cur_call()->L, is_opened);

	// Ｋフラグ
	h_child = update_tv_element(h_item, h_child, &tnm_get_cur_call()->K, is_opened);

	// カウンタ
	h_child = update_tv_element(h_item, h_child, Gp_counter_list, is_opened);

	// マスク
	h_child = update_tv_element(h_item, h_child, Gp_mask_list, is_opened);

	// バックステージ
	h_child = update_tv_element(h_item, h_child, &(*Gp_stage_list)[TNM_STAGE_BACK], is_opened);

	// フロントステージ
	h_child = update_tv_element(h_item, h_child, &(*Gp_stage_list)[TNM_STAGE_FRONT], is_opened);

	// ネクストステージ
	h_child = update_tv_element(h_item, h_child, &(*Gp_stage_list)[TNM_STAGE_NEXT], is_opened);

	// スクリーン
	h_child = update_tv_element(h_item, h_child, Gp_screen, is_opened);

	// BGM
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_bgm, is_opened);

	// 声
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_koe, is_opened);

	// 効果音
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_pcm, is_opened);

	// 効果音チャンネル
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_pcmch_list, is_opened);

	// 効果音イベント
	h_child = update_tv_element(h_item, h_child, Gp_pcm_event_list, is_opened);

	// システム音
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_se, is_opened);

	// ムービー
	h_child = update_tv_element(h_item, h_child, &Gp_sound->m_mov, is_opened);

	// フレームアクション
	h_child = update_tv_element(h_item, h_child, Gp_frame_action, is_opened);

	// フレームアクションチャンネル
	h_child = update_tv_element(h_item, h_child, Gp_frame_action_ch, is_opened);

	// コール
	h_child = update_tv_element(h_item, h_child, Gp_call_list, is_opened);

	// ＥＸコール
	h_child = update_tv_element(h_item, h_child, Gp_excall, is_opened);
}

// ステージ
void C_db_wnd_info_element::update_tv_stage(HTREEITEM h_item, C_elm_stage* p_stage, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// オブジェクト
	h_child = update_tv_element(h_item, h_child, &p_stage->m_object_list, is_opened);

	// グループ
	h_child = update_tv_element(h_item, h_child, &p_stage->m_group_list, is_opened);

	// メッセージウィンドウ
	h_child = update_tv_element(h_item, h_child, &p_stage->m_mwnd_list, is_opened);

	// ワールド
	h_child = update_tv_element(h_item, h_child, &p_stage->m_world_list, is_opened);

	// エフェクト
	h_child = update_tv_element(h_item, h_child, &p_stage->m_effect_list, is_opened);

	// クエイク
	h_child = update_tv_element(h_item, h_child, &p_stage->m_quake_list, is_opened);
}

// オブジェクト
void C_db_wnd_info_element::update_tv_object(HTREEITEM h_item, C_elm_object* p_object, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// 修正座標Ｘ
	h_child = update_tv_element(h_item, h_child, &p_object->x_rep_event_list(), is_opened);

	// 修正座標Ｙ
	h_child = update_tv_element(h_item, h_child, &p_object->y_rep_event_list(), is_opened);

	// 修正座標Ｚ
	h_child = update_tv_element(h_item, h_child, &p_object->z_rep_event_list(), is_opened);

	// フレームアクション
	h_child = update_tv_element(h_item, h_child, p_object->get_frame_action(), is_opened);

	// フレームアクションチャンネル
	h_child = update_tv_element(h_item, h_child, p_object->get_frame_action_ch(), is_opened);

	// 子供
	h_child = update_tv_element(h_item, h_child, &p_object->child_list(), is_opened);

	// Ｆフラグ
	h_child = update_tv_element(h_item, h_child, &p_object->F(), is_opened);
}

// メッセージウィンドウ
void C_db_wnd_info_element::update_tv_mwnd(HTREEITEM h_item, C_elm_mwnd* p_mwnd, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// ボタン
	h_child = update_tv_element(h_item, h_child, &p_mwnd->button_list(), is_opened);

	// 顔グラ
	h_child = update_tv_element(h_item, h_child, &p_mwnd->face_list(), is_opened);

	// オブジェクト
	h_child = update_tv_element(h_item, h_child, &p_mwnd->object_list(), is_opened);
}

// スクリーン
void C_db_wnd_info_element::update_tv_screen(HTREEITEM h_item, const C_elm_screen* p_screen, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// エフェクト
	h_child = update_tv_element(h_item, h_child, &p_screen->effect_list(), is_opened);

	// クエイク
	h_child = update_tv_element(h_item, h_child, &p_screen->quake_list(), is_opened);
}

// コール
void C_db_wnd_info_element::update_tv_call(HTREEITEM h_item, const C_elm_call* p_call, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// Ｌフラグ
	h_child = update_tv_element(h_item, h_child, &p_call->L, is_opened);

	// Ｋフラグ
	h_child = update_tv_element(h_item, h_child, &p_call->K, is_opened);
}

// ＥＸコール
void C_db_wnd_info_element::update_tv_excall(HTREEITEM h_item, C_elm_excall* p_excall, bool is_opened)
{
	HTREEITEM h_child = NULL;

	if (!p_excall->is_ready())
		return;

	// カウンタ
	h_child = update_tv_element(h_item, h_child, &p_excall->m_counter_list, is_opened);

	// バックステージ
	h_child = update_tv_element(h_item, h_child, &p_excall->m_stage_list[TNM_STAGE_BACK], is_opened);

	// フロントステージ
	h_child = update_tv_element(h_item, h_child, &p_excall->m_stage_list[TNM_STAGE_FRONT], is_opened);

	// ネクストステージ
	h_child = update_tv_element(h_item, h_child, &p_excall->m_stage_list[TNM_STAGE_NEXT], is_opened);

	// Ｆフラグ
	h_child = update_tv_element(h_item, h_child, &p_excall->m_F, is_opened);
}

// ツリーのトップまたは次のアイテムを取得する
HTREEITEM get_next_item(Cc_treeview* p_tv, HTREEITEM h_parent, HTREEITEM h_child)
{
	if (h_child == NULL)
		return (HTREEITEM)p_tv->send_message(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)h_parent);	// 先頭の子供を取得
	else
		return (HTREEITEM)p_tv->send_message(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)h_child);		// 次の子供を取得
}

// 汎用リスト
template <typename TYPE>
void C_db_wnd_info_element::update_tv_list(HTREEITEM h_item, C_elm_list_base<TYPE>* p_list, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// サブ
	int sub_cnt = p_list->get_size();
	for (int i = 0; i < sub_cnt; i++)
		h_child = update_tv_element(h_item, h_child, &(*p_list)[i], is_opened);	// ここの at 演算子が、テンプレートの型ごとに異なります。

	// まだアイテムがある場合、アイテムを削除する
	while (HTREEITEM h_next_child = get_next_item(&tv_element, h_item, h_child))	{
		tv_element.send_message(TVM_DELETEITEM, 0, (LPARAM)h_next_child);
		m_item_map.erase(h_next_child);
	}
}

// 整数リスト（汎用リストの特殊化）
template <>
void C_db_wnd_info_element::update_tv_list<int>(HTREEITEM h_item, C_elm_list_base<int>* p_list, bool is_opened)
{
	// 整数リストは、ビットを子供として持つ
	HTREEITEM h_child = NULL;

	int cnt;

	// 1bit
	cnt = p_list->get_size() * 32;
	for (int c = 0; c < cnt; c += 1000)
		h_child = update_tv_element(h_item, h_child, p_list, 1, c, str_format(p_list->get_element_name() + _T(".bit1[%05d-%05d]"), c, c + 1000 - 1), is_opened);

	// 2bit
	cnt = p_list->get_size() * 16;
	for (int c = 0; c < cnt; c += 1000)
		h_child = update_tv_element(h_item, h_child, p_list, 2, c, str_format(p_list->get_element_name() + _T(".bit2[%05d-%05d]"), c, c + 1000 - 1), is_opened);

	// 4bit
	cnt = p_list->get_size() * 8;
	for (int c = 0; c < cnt; c += 1000)
		h_child = update_tv_element(h_item, h_child, p_list, 4, c, str_format(p_list->get_element_name() + _T(".bit4[%05d-%05d]"), c, c + 1000 - 1), is_opened);

	// 8bit
	cnt = p_list->get_size() * 4;
	for (int c = 0; c < cnt; c += 1000)
		h_child = update_tv_element(h_item, h_child, p_list, 8, c, str_format(p_list->get_element_name() + _T(".bit8[%05d-%05d]"), c, c + 1000 - 1), is_opened);

	// 16bit
	cnt = p_list->get_size() * 2;
	for (int c = 0; c < cnt; c += 1000)
		h_child = update_tv_element(h_item, h_child, p_list, 16, c, str_format(p_list->get_element_name() + _T(".bit16[%05d-%05d]"), c, c + 1000 - 1), is_opened);
}

// 整数イベントリスト（汎用リストの特殊化）
template <>
void C_db_wnd_info_element::update_tv_list<C_elm_int_event>(HTREEITEM h_item, C_elm_list_base<C_elm_int_event>* p_list, bool is_opened)
{
	// 整数イベントリストは、ツリーには子供を表示しない
}

// 文字列リスト（汎用リストの特殊化）
template <>
void C_db_wnd_info_element::update_tv_list<TSTR>(HTREEITEM h_item, C_elm_list_base<TSTR>* p_list, bool is_opened)
{
	// 文字列リストは、ツリーには子供を表示しない
}

// オブジェクトリスト（汎用リストの特殊化）
template <>
void C_db_wnd_info_element::update_tv_list<C_elm_object>(HTREEITEM h_item, C_elm_list_base<C_elm_object>* p_list, bool is_opened)
{
	HTREEITEM h_child = NULL;

	// オブジェクトリストは、有効な子供のみを表示する

	// サブ
	int sub_cnt = p_list->get_size();
	for (int i = 0; i < sub_cnt; i++)	{
		if ((*p_list)[i].get_type() != TNM_OBJECT_TYPE_NONE || (*p_list)[i].get_child_cnt() > 0)	{
			h_child = update_tv_element(h_item, h_child, &(*p_list)[i], is_opened);	// ここの at 演算子が、テンプレートの型ごとに異なります。
		}
	}

	// まだアイテムがある場合、アイテムを削除する
	while (HTREEITEM h_next_child = get_next_item(&tv_element, h_item, h_child))	{
		tv_element.send_message(TVM_DELETEITEM, 0, (LPARAM)h_next_child);
		m_item_map.erase(h_next_child);
	}
}

// エレメント
HTREEITEM C_db_wnd_info_element::update_tv_element(HTREEITEM h_item, HTREEITEM h_child, const C_tnm_element* p_element, bool is_opened)
{
	// h_child が NULL の場合は、最初の子供
	if (h_child == NULL)
		h_child = (HTREEITEM)tv_element.send_message(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)h_item);	// 先頭の子供を取得
	else
		h_child = (HTREEITEM)tv_element.send_message(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)h_child);	// 次の子供を取得

	// アイテムがなければ作成する
	if (h_child == NULL)	{
		h_child = tv_element.add_item(h_item, TVI_LAST, p_element->get_element_name());
		m_item_map[h_child] = S_treeview_item_element(p_element->get_element(), p_element->get_element_form(), 32, 0, p_element->get_element_name());
	}

	// 見えているアイテムの場合はさらに探る
	if (is_opened)	{

		// 開いているか選択されていれば更新
		TV_ITEM item = {0};
		item.hItem = h_child;
		item.mask = TVIF_STATE;
		item.stateMask = TVIS_EXPANDED | TVIS_SELECTED;
		tv_element.send_message(TVM_GETITEM, 0, (LPARAM)&item);
		is_opened = (item.state & TVIS_EXPANDED) ? true : false;

		// エレメントごとに分岐
		// リストも完全に分岐しないといけません。適切なテンプレート関数を呼ぶためです。
		switch (p_element->get_element_form())	{
			case FM_INTLIST:			update_tv_list(h_child, (C_elm_int_list *)p_element, is_opened);			break;
			case FM_INTEVENTLIST:		update_tv_list(h_child, (C_elm_int_event_list *)p_element, is_opened);		break;
			case FM_STRLIST:			update_tv_list(h_child, (C_elm_str_list *)p_element, is_opened);			break;
			case FM_COUNTERLIST:		update_tv_list(h_child, (C_elm_counter_list *)p_element, is_opened);		break;
			case FM_MASKLIST:			update_tv_list(h_child, (C_elm_mask_list *)p_element, is_opened);			break;
			case FM_STAGELIST:			update_tv_list(h_child, (C_elm_stage_list *)p_element, is_opened);			break;
			case FM_STAGE:				update_tv_stage(h_child, (C_elm_stage *)p_element, is_opened);				break;
			case FM_OBJECTLIST:			update_tv_list(h_child, (C_elm_object_list *)p_element, is_opened);			break;
			case FM_OBJECT:				update_tv_object(h_child, (C_elm_object *)p_element, is_opened);			break;
			case FM_GROUPLIST:			update_tv_list(h_child, (C_elm_group_list *)p_element, is_opened);			break;
			case FM_MWNDLIST:			update_tv_list(h_child, (C_elm_mwnd_list *)p_element, is_opened);			break;
			case FM_MWND:				update_tv_mwnd(h_child, (C_elm_mwnd *)p_element, is_opened);				break;
			case FM_SCREEN:				update_tv_screen(h_child, (C_elm_screen *)p_element, is_opened);			break;
			case FM_EFFECTLIST:			update_tv_list(h_child, (C_elm_effect_list *)p_element, is_opened);			break;
			case FM_QUAKELIST:			update_tv_list(h_child, (C_elm_quake_list *)p_element, is_opened);			break;
			case FM_WORLDLIST:			update_tv_list(h_child, (C_elm_world_list *)p_element, is_opened);			break;
			case FM_PCMCHLIST:			update_tv_list(h_child, (C_elm_pcmch_list *)p_element, is_opened);			break;
			case FM_PCMEVENTLIST:		update_tv_list(h_child, (C_elm_pcm_event_list *)p_element, is_opened);		break;
			case FM_FRAMEACTIONLIST:	update_tv_list(h_child, (C_elm_frame_action_list *)p_element, is_opened);	break;
			case FM_CALLLIST:			update_tv_list(h_child, (C_elm_call_list *)p_element, is_opened);			break;
			case FM_CALL:				update_tv_call(h_child, (C_elm_call *)p_element, is_opened);				break;
			case FM_EXCALL:				update_tv_excall(h_child, (C_elm_excall *)p_element, is_opened);			break;
		}
	}

	return h_child;
}

// エレメント（整数配列）
HTREEITEM C_db_wnd_info_element::update_tv_element(HTREEITEM h_item, HTREEITEM h_child, const C_elm_list_base<int>* p_element, int bit, int top_index, CTSTR& name, bool is_opened)
{
	// h_child が NULL の場合は、最初の子供
	if (h_child == NULL)
		h_child = (HTREEITEM)tv_element.send_message(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)h_item);	// 先頭の子供を取得
	else
		h_child = (HTREEITEM)tv_element.send_message(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)h_child);	// 次の子供を取得

	// アイテムがなければ作成する
	if (h_child == NULL)	{
		h_child = tv_element.add_item(h_item, TVI_LAST, name);
		m_item_map[h_child] = S_treeview_item_element(p_element->get_element(), p_element->get_element_form(), bit, top_index, name);
	}

	return h_child;
}

// ****************************************************************
// リストビューを再構築する
// ================================================================
void C_db_wnd_info_element::restruct_lv()
{
	lv_element.clear();
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		(*itr)->clear();

	// 全てのリスト用のリストビューを非表示にする
	btn_open_list.set_show(false);
	for (ARRAY<C_tnm_dbg_listview *>::iterator itr = m_lv_list.begin(); itr != m_lv_list.end(); ++itr)
		(*itr)->set_show(false);
	
	// ツリービューで選択されたアイテムを探し出す
	std::map<HTREEITEM, S_treeview_item_element>::iterator selected_itr = m_item_map.find(h_selected_item);
	if (selected_itr == m_item_map.end())
		return;
	
	// エレメント用のリストビューを初期化
	tnm_init_lv_element(selected_itr->second.element_form, false, &lv_element);

	// リスト用のリストビューを初期化
	switch (selected_itr->second.element_form)	{
		case FM_INTLIST:			lv_int_list.set_show(true);				btn_open_list.set_show(true);			break;
		case FM_INTEVENTLIST:		lv_int_list.set_show(true);				btn_open_list.set_show(true);			break;		// 整数リストを使いまわし
		case FM_STRLIST:			lv_str_list.set_show(true);				btn_open_list.set_show(true);			break;
		case FM_COUNTERLIST:		lv_counter_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_MASKLIST:			lv_mask_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_OBJECTLIST:			lv_object_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_GROUPLIST:			lv_group_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_MWNDLIST:			lv_mwnd_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_EFFECTLIST:			lv_effect_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_QUAKELIST:			lv_quake_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_WORLDLIST:			lv_world_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_PCMCHLIST:			lv_pcmch_list.set_show(true);			btn_open_list.set_show(true);			break;
		case FM_PCMEVENTLIST:		lv_pcm_event_list.set_show(true);		btn_open_list.set_show(true);			break;
		case FM_FRAMEACTIONLIST:	lv_frame_action_list.set_show(true);	btn_open_list.set_show(true);			break;
		case FM_CALLLIST:			lv_call_list.set_show(true);			btn_open_list.set_show(true);			break;
	}
}

// ****************************************************************
// リストビューを更新する
// ================================================================
void C_db_wnd_info_element::update_lv()
{
	std::map<HTREEITEM, S_treeview_item_element>::iterator itr = m_item_map.find(h_selected_item);
	if (itr != m_item_map.end())	{

		// エレメントの実体を取得（範囲外エラーを出さない）
		C_tnm_element* p_element = tnm_get_element_ptr(itr->second.element.begin(), itr->second.element.end(), false);
		if (!p_element)
			return;

		// エレメント個別情報
		update_lv_element(p_element, itr->second.element_form, itr->second.bit, itr->second.top_index);
	}
}

// エレメント
void C_db_wnd_info_element::update_lv_element(C_tnm_element* p_element, int element_form, int bit, int top_index)
{
	int item_no = 0, column_no = 1;

	// エレメント用のリストビューにエレメント情報を表示
	tnm_update_lv_element(p_element, element_form, bit, top_index, &lv_element, &item_no, &column_no, &item_no);

	// リストエレメントの場合
	if (tnm_form_is_list(element_form))	{

		// テーブルビューにリスト情報をテーブル表示
		int list_item_no = 0;
		switch (element_form)	{
			case FM_INTLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_int_list, &list_item_no);			break;
			case FM_INTEVENTLIST:		tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_int_list, &list_item_no);			break;
			case FM_STRLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_str_list, &list_item_no);			break;
			case FM_COUNTERLIST:		tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_counter_list, &list_item_no);		break;
			case FM_MASKLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_mask_list, &list_item_no);			break;
			case FM_OBJECTLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_object_list, &list_item_no);		break;
			case FM_GROUPLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_group_list, &list_item_no);		break;
			case FM_MWNDLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_mwnd_list, &list_item_no);			break;
			case FM_EFFECTLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_effect_list, &list_item_no);		break;
			case FM_QUAKELIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_quake_list, &list_item_no);		break;
			case FM_WORLDLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_world_list, &list_item_no);		break;
			case FM_PCMCHLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_pcmch_list, &list_item_no);		break;
			case FM_PCMEVENTLIST:		tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_pcm_event_list, &list_item_no);	break;
			case FM_FRAMEACTIONLIST:	tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_frame_action_list, &list_item_no);	break;
			case FM_CALLLIST:			tnm_update_lv_element_table(p_element, element_form, bit, top_index, &lv_call_list, &list_item_no);			break;
		}
	}
}

// ****************************************************************
// リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_element::on_dblclk_lv(C_tnm_dbg_listview* p_lv, bool table_flag)
{
	// 選択状態取得
	int select_cnt = p_lv->get_selected_item_cnt();
	int select_no = p_lv->get_selected_item_no();

	// マウスカーソルの乗っているセル番号
	int item_no, column_no;
	C_point mp = p_lv->screen_to_client(get_mouse_pos());
	p_lv->get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// アイテムを変更する
	if (select_cnt == 1 && select_no == item_no)	{
		
		std::map<HTREEITEM, S_treeview_item_element>::iterator itr = m_item_map.find(h_selected_item);
		if (itr != m_item_map.end())	{

			// エレメント個別情報
			tnm_on_dblclk_lv_element(p_lv, item_no, column_no, itr->second.element_form, itr->second.bit, table_flag);
		}
	}
}

// ****************************************************************
// リストビューの値を変更した
// ================================================================
void C_db_wnd_info_element::on_change_lv_value(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, bool table_flag)
{
	std::map<HTREEITEM, S_treeview_item_element>::iterator itr = m_item_map.find(h_selected_item);
	if (itr != m_item_map.end())	{

		// エレメントの実体を取得（範囲外エラーを出さない）
		C_tnm_element* p_element = tnm_get_element_ptr(itr->second.element.begin(), itr->second.element.end(), false);
		if (!p_element)
			return;

		// エレメント個別情報
		tnm_on_change_lv_value_element(p_lv, item_no, column_no, value, p_element, itr->second.element_form, itr->second.bit, itr->second.top_index, table_flag);
	}
}

// ****************************************************************
// エレメント情報を別ウィンドウで開く
// ================================================================
void C_db_wnd_info_element::open_element_by_another_wnd()
{
	// ツリービューで選択されたアイテムを探し出す
	std::map<HTREEITEM, S_treeview_item_element>::iterator selected_itr = m_item_map.find(h_selected_item);
	if (selected_itr == m_item_map.end())
		return;

	// 別ウィンドウを作成
	BSP<C_db_wnd_info_element_lv> lv(new C_db_wnd_info_element_lv);
	m_opened_lv_list.push_back(lv);

	// ウィンドウの設定を行って開く
	lv->m_element = selected_itr->second.element;
	lv->m_element_form = selected_itr->second.element_form;
	lv->m_element_name = selected_itr->second.element_name;
	lv->m_bit = selected_itr->second.bit;
	lv->m_top_index = selected_itr->second.top_index;
	lv->m_table_flag = false;
	lv->open();
	lv->set_text(lv->m_element_name);

	// ウィンドウを初期化する
	tnm_init_lv_element(lv->m_element_form, lv->m_table_flag, lv->get_lv());
}

// ****************************************************************
// リスト情報を別ウィンドウで開く
// ================================================================
void C_db_wnd_info_element::open_list_by_another_wnd()
{
	// ツリービューで選択されたアイテムを探し出す
	std::map<HTREEITEM, S_treeview_item_element>::iterator selected_itr = m_item_map.find(h_selected_item);
	if (selected_itr == m_item_map.end())
		return;

	// 別ウィンドウを作成
	BSP<C_db_wnd_info_element_lv> lv(new C_db_wnd_info_element_lv);
	m_opened_lv_list.push_back(lv);

	// ウィンドウの設定を行って開く
	lv->m_element = selected_itr->second.element;
	lv->m_element_form = selected_itr->second.element_form;
	lv->m_element_name = selected_itr->second.element_name;
	lv->m_bit = selected_itr->second.bit;
	lv->m_top_index = selected_itr->second.top_index;
	lv->m_table_flag = true;
	lv->open();
	lv->set_text(lv->m_element_name);

	// ウィンドウを初期化する
	tnm_init_lv_element(lv->m_element_form, lv->m_table_flag, lv->get_lv());
}

// ****************************************************************
// マウスが乗っているかを確認する
// ================================================================
bool C_db_wnd_info_element::check_mouse_over(C_point mp)
{
	for (int i = 0; i < (int)m_opened_lv_list.size(); i++)	{
		C_window_base* p_wnd = m_opened_lv_list[i].get();
		if (p_wnd->get_handle() && p_wnd->get_enable())	{
			C_rect wr = p_wnd->get_window_rect();
			if (wr.left <= mp.x && mp.x < wr.right && wr.top <= mp.y && mp.y < wr.bottom)	{
				return true;	// 上に乗っている！
			}
		}
	}

	// 上に乗っていない
	return false;
}

// ****************************************************************
// 別ウィンドウリストビュー：プロシージャ
// ================================================================
LRESULT C_db_wnd_info_element_lv::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:		if (!on_notify(wp, lp))		return FALSE;	break;
	}

	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// ****************************************************************
// 別ウィンドウリストビュー：通知
// ================================================================
bool C_db_wnd_info_element_lv::on_notify(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	NMHDR* hdr = (NMHDR *)lp;

	if (ctrl_id == lv.get_id())	{
		if (hdr->code == NM_DBLCLK)	{
			on_dblclk_lv();
		}
		else if (hdr->code == LVN_CHANGE_TEXT)	{
			on_change_lv_value(((NMLVCHANGE *)lp)->item_no, ((NMLVCHANGE *)lp)->column_no, ((NMLVCHANGE *)lp)->new_text);
		}
	}

	return true;
}

// ****************************************************************
// 別ウィンドウリストビュー：リストビューをダブルクリックした
// ================================================================
void C_db_wnd_info_element_lv::on_dblclk_lv()
{
	// 選択状態取得
	int select_cnt = lv.get_selected_item_cnt();
	int select_no = lv.get_selected_item_no();

	// マウスカーソルの乗っているセル番号
	int item_no, column_no;
	C_point mp = lv.screen_to_client(get_mouse_pos());
	lv.get_cell_by_point(mp.x, mp.y, &item_no, &column_no);

	// アイテムを変更する
	if (select_cnt == 1 && select_no == item_no)	{
		
		// エレメント個別情報
		tnm_on_dblclk_lv_element(&lv, item_no, column_no, m_element_form, m_bit, m_table_flag);
	}
}

// ****************************************************************
// 別ウィンドウリストビュー：リストビューの値を変更した
// ================================================================
void C_db_wnd_info_element_lv::on_change_lv_value(int item_no, int column_no, CTSTR& value)
{
	// エレメントの実体を取得（範囲外エラーを出さない）
	C_tnm_element* p_element = tnm_get_element_ptr(m_element.begin(), m_element.end(), false);
	if (!p_element)
		return;

	// エレメント個別情報
	tnm_on_change_lv_value_element(&lv, item_no, column_no, value, p_element, m_element_form, m_bit, m_top_index, m_table_flag);
}




