#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_btn_sel.h"
#include	"element/elm_msg_back.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_msg_back.h"

// ****************************************************************
// コマンド処理：メッセージウィンドウリスト
// ================================================================
bool tnm_command_proc_mwnd_list(C_elm_mwnd_list* p_mwnd_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_mwnd_list;
	}
	else if (elm_begin[0] == ELM_UP)	{

		C_tnm_element* p_parent = p_mwnd_list->get_parent();

		// ▲とりあえず考えられるフォーム
		if (false);
		else if (p_parent->get_element_form() == FM_STAGE)	{
			return tnm_command_proc_stage((C_elm_stage *)p_parent, elm_top, elm_begin + 1, elm_end, p_ai);
		}
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_mwnd* p_mwnd = p_mwnd_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_mwnd)
			tnm_command_proc_mwnd(p_mwnd, elm_top, elm_begin + 2, elm_end, p_ai);
	}
	else if (elm_begin[0] == ELM_MWNDLIST_CLOSE)	{
		tnm_msg_proc_close_all(true);
	}
	else if (elm_begin[0] == ELM_MWNDLIST_CLOSE_WAIT)	{
		tnm_msg_proc_close_all(true);
	}
	else if (elm_begin[0] == ELM_MWNDLIST_CLOSE_NOWAIT)	{
		tnm_msg_proc_close_all(false);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_mwnd_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：メッセージウィンドウ
// ================================================================
//		r = pp + clear
//		  = msg_wait + msg_key_wait + clear
// ================================================================
bool tnm_command_proc_mwnd(C_elm_mwnd* p_mwnd, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (p_mwnd == NULL)
		return true;

	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_mwnd;
		return true;
	}

	int al_id = p_ai->al_id;

	switch (elm_begin[0])	{

	case ELM_UP:
	{
		C_tnm_element* p_parent = p_mwnd->get_parent();

		// ▲とりあえず考えられるフォーム
		if (false);
		else if (p_parent->get_element_form() == FM_MWNDLIST)	{
			return tnm_command_proc_mwnd_list((C_elm_mwnd_list *)p_parent, elm_top, elm_begin + 1, elm_end, p_ai);
		}
		break;
	}

	// 枠を設定する
	case ELM_MWND_SET_WAKU:
	{
		int msg_waku_no = TNM_USE_DEFAULT, name_waku_no = TNM_USE_DEFAULT;
		switch (al_id)	{
			case 2:		name_waku_no = p_ai->al_begin[1].Int;
			case 1:		msg_waku_no = p_ai->al_begin[0].Int;
		}

		p_mwnd->set_waku(msg_waku_no, name_waku_no, true);
		break;
	}

	// 枠ファイル
	case ELM_MWND_INIT_WAKU_FILE:
	{
		p_mwnd->init_waku_file();
		break;
	}
	case ELM_MWND_SET_WAKU_FILE:
	{
		TSTR waku_file = p_ai->al_begin[0].str;

		p_mwnd->set_waku_file(waku_file);
		break;
	}
	case ELM_MWND_GET_WAKU_FILE:
	{
		TSTR waku_file = p_mwnd->get_waku_file();
		tnm_stack_push_str(waku_file);
		break;
	}
	// フィルターファイル
	case ELM_MWND_INIT_FILTER_FILE:
	{
		p_mwnd->init_filter_file();
		break;
	}
	case ELM_MWND_SET_FILTER_FILE:
	{
		TSTR waku_file = p_ai->al_begin[0].str;

		p_mwnd->set_filter_file(waku_file);
		break;
	}
	case ELM_MWND_GET_FILTER_FILE:
	{
		TSTR filter_file = p_mwnd->get_filter_file();
		tnm_stack_push_str(filter_file);
		break;
	}

	// メッセージウィンドウを開く
	case ELM_MWND_OPEN:
	{
		p_mwnd->open();

		// 開きアニメ待ち状態にする
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_OPEN_WAIT;
		proc.element = p_mwnd->get_element();
		tnm_push_proc(proc);
		break;
	}

	case ELM_MWND_OPEN_WAIT:
	{
		p_mwnd->open();

		// 開きアニメ待ち状態にする
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_OPEN_WAIT;
		proc.element = p_mwnd->get_element();
		tnm_push_proc(proc);
		break;
	}

	case ELM_MWND_OPEN_NOWAIT:
	{
		p_mwnd->open();
		break;
	}

	case ELM_MWND_CHECK_OPEN:
	{
		tnm_stack_push_int(p_mwnd->get_window_appear_flag() ? 1 : 0);
		break;
	}

	// メッセージウィンドウを閉じる
	case ELM_MWND_CLOSE:
		tnm_msg_proc_close(p_mwnd, true);
		break;
	case ELM_MWND_CLOSE_WAIT:
		tnm_msg_proc_close(p_mwnd, true);
		break;
	case ELM_MWND_CLOSE_NOWAIT:
		tnm_msg_proc_close(p_mwnd, false);
		break;
	case ELM_MWND_END_CLOSE:
		p_mwnd->end_close();
		break;

	// メッセージブロックを開始する
	case ELM_MWND_MSG_BLOCK:
		tnm_msg_proc_start_msg_block(p_mwnd);
		break;
	case ELM_MWND_MSG_PP_BLOCK:
		tnm_msg_proc_start_msg_pp_block(p_mwnd);
		break;

	// メッセージをクリアする
	case ELM_MWND_CLEAR:
		tnm_msg_proc_clear_ready(p_mwnd);
		break;
	case ELM_MWND____NOVEL_CLEAR:
		tnm_msg_proc_clear_ready_novel(p_mwnd);
		break;

	// メッセージを表示する
	case ELM_MWND_PRINT:
	{
		TSTR msg;

		if (false);
		else if (al_id == 0)	msg = tostr(p_ai->al_begin[0].Int);
		else if (al_id == 1)	msg = p_ai->al_begin[0].str;

		int rf_scn_no = Gp_lexer->get_cur_scn_no();		// 既読フラグのシーン番号は現在のシーン番号
		int rf_flag_no = Gp_lexer->pop_ret<int>();		// 既読フラグのフラグ番号を取得

		tnm_msg_proc_print(p_mwnd, msg, rf_scn_no, rf_flag_no, true);
		break;
	}

	// メッセージを表示する（オーバーフロー直後）
	case ELM_MWND____OVER_FLOW_PRINT:
	{
		TSTR msg;

		if (false);
		else if (al_id == 0)	msg = tostr(p_ai->al_begin[0].Int);
		else if (al_id == 1)	msg = p_ai->al_begin[0].str;

		int rf_scn_no = p_ai->al_begin[1].Int;
		int rf_flag_no = p_ai->al_begin[2].Int;

		// メッセージバックに追加しない
		tnm_msg_proc_print(p_mwnd, msg, rf_scn_no, rf_flag_no, false);
		break;
	}

	// 名前を表示する（オーバーフロー直後）
	case ELM_MWND____OVER_FLOW_NAMAE:
	{
		TSTR msg;

		if (false);
		else if (al_id == 0)	msg = tostr(p_ai->al_begin[0].Int);
		else if (al_id == 1)	msg = p_ai->al_begin[0].str;

		// メッセージバックに追加しない
		tnm_msg_proc_name(p_mwnd, msg);
		break;
	}

	// ルビを設定する
	case ELM_MWND_RUBY:
		if (al_id == 0)	{	// ルビ終了
			tnm_msg_proc_end_ruby(p_mwnd);
		}
		else	{			// ルビ開始
			TSTR ruby = p_ai->al_begin[0].str;
			tnm_msg_proc_ruby(p_mwnd, ruby);
		}
		break;

	// メッセージ待ちなど
	case ELM_MWND_WAIT_MSG:
		tnm_msg_proc_command_msg_wait(p_mwnd);
		break;
	case ELM_MWND_PP:
		tnm_msg_proc_command_pp(p_mwnd);
		break;
	case ELM_MWND_R:
		tnm_msg_proc_command_r(p_mwnd);
		break;
	case ELM_MWND_PAGE:
		tnm_msg_proc_command_page(p_mwnd);
		break;

	// テキストを改行する
	case ELM_MWND_NL:
		tnm_msg_proc_command_nl(p_mwnd);
		break;
	case ELM_MWND_NLI:
		tnm_msg_proc_command_nli(p_mwnd);
		break;

	// インデント
	case ELM_MWND_INDENT:
		p_mwnd->set_indent();
		break;
	case ELM_MWND_CLEAR_INDENT:
		p_mwnd->clear_indent();
		break;

	// マルチメッセージ
	case ELM_MWND_MULTI_MSG:
		Gp_local->pod.multi_msg_mode = true;
		break;
	case ELM_MWND_NEXT_MSG:
		p_mwnd->next_msg();
		break;

	// スライドメッセージ
	case ELM_MWND_START_SLIDE_MSG:
		p_mwnd->set_slide_msg(true, p_ai->al_begin[0].Int);
		break;
	case ELM_MWND_END_SLIDE_MSG:
		p_mwnd->set_slide_msg(false, 0);
		break;
	case ELM_MWND____SLIDE_MSG:
		p_mwnd->slide_msg();
		break;

	// 選択肢
	case ELM_MWND_SEL:
	case ELM_MWND_SEL_CANCEL:
	case ELM_MWND_SELMSG:
	case ELM_MWND_SELMSG_CANCEL:
	{
		ARRAY<S_tnm_sel_param> sel_list;
		int sel_cnt = 0;

		// パラメータを解析する
		S_tnm_sel_param param;
		param.type = 0;
		int arg_no = 0;
		for (C_tnm_prop* itr = p_ai->al_begin; itr != p_ai->al_end; ++itr)	{

			// 文字列の場合は選択肢を増やす
			if (itr->form == FM_STR)	{
				sel_list.resize(++sel_cnt);
				sel_list.back().str = itr->str;
				sel_list.back().type = TNM_SEL_ITEM_TYPE_ON;		// デフォルト
				sel_list.back().color = -1;							// デフォルト
				arg_no = 0;
			}
			else if (itr->form == FM_INT)	{
				switch (arg_no)	{
					case 1:		sel_list.back().type = itr->Int;	break;
					case 2:		sel_list.back().color = itr->Int;	break;
				}
			}
			arg_no ++;
		}

		// 既読フラグ処理
		int rf_scn_no = Gp_lexer->get_cur_scn_no();		// 既読フラグのシーン番号は現在のシーン番号
		int rf_flag_no = Gp_lexer->pop_ret<int>();		// 既読フラグのフラグ番号を取得

		// 選択開始
		switch (elm_begin[0])	{
			case ELM_MWND_SEL:				tnm_msg_proc_sel(p_mwnd, sel_list, rf_scn_no, rf_flag_no, false, true);		break;
			case ELM_MWND_SEL_CANCEL:	 	tnm_msg_proc_sel(p_mwnd, sel_list, rf_scn_no, rf_flag_no, true, true);		break;
			case ELM_MWND_SELMSG:			tnm_msg_proc_sel(p_mwnd, sel_list, rf_scn_no, rf_flag_no, false, false);	break;
			case ELM_MWND_SELMSG_CANCEL: 	tnm_msg_proc_sel(p_mwnd, sel_list, rf_scn_no, rf_flag_no, true, false);		break;
		}
		break;
	}

	// 文字位置補正
	case ELM_MWND_REP_POS:
		if (false);
		else if (al_id == 0)	p_mwnd->set_moji_rep_pos_default();
		else if (al_id == 1)	p_mwnd->set_moji_rep_pos(C_point(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
		break;

	// 文字サイズ
	case ELM_MWND_SIZE:
		if (false);
		else if (al_id == 0)	p_mwnd->set_moji_size_default();
		else if (al_id == 1)	p_mwnd->set_moji_size(p_ai->al_begin[0].Int);
		break;

	// 文字色
	case ELM_MWND_COLOR:
		if (false);
		else if (al_id == 0)	p_mwnd->set_moji_color_default();
		else if (al_id == 1)	p_mwnd->set_moji_color(p_ai->al_begin[0].Int);
		break;

	// 文字色
	case ELM_MWND_MSGBTN:
		if (false);
		else if (al_id == 0)	p_mwnd->end_button();
		else if (al_id == 1)	p_mwnd->start_button(p_ai->al_begin[0].Int, 0, 0, 0);
		else if (al_id == 2)	p_mwnd->start_button(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, 0, 0);
		else if (al_id == 3)	p_mwnd->start_button(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int, p_ai->al_begin[2].Int, p_ai->al_begin[3].Int);
		break;

		// 名前を設定する
	case ELM_MWND_SET_NAMAE:
	{
		tnm_msg_proc_name(p_mwnd, p_ai->al_begin[0].str);
		break;
	}

	// 声を再生する
	case ELM_MWND_KOE:
	{
		int koe_no = 0, chr_no = -1;
		bool koe_no_auto_mode_flag = false;
		switch (al_id)	{	// no break!
			case 1:		chr_no = p_ai->al_begin[1].Int;
			case 0:		koe_no = p_ai->al_begin[0].Int;
		}

		// 名前付き引数
		for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
			switch (arg->id)	{
			case 0:		koe_no_auto_mode_flag = arg->Int ? true : false;	break;
			}
		}

		int rf_scn_no = Gp_lexer->get_cur_scn_no();		// 既読フラグのシーン番号は現在のシーン番号
		int rf_flag_no = Gp_lexer->pop_ret<int>();		// 既読フラグのフラグ番号を取得

		tnm_msg_proc_koe(p_mwnd, koe_no, chr_no, rf_scn_no, rf_flag_no, false, false, koe_no_auto_mode_flag, false);
		break;
	}
	case ELM_MWND_KOE_PLAY_WAIT:
	{
		int koe_no = 0, chr_no = -1;
		switch (al_id)	{	// no break!
			case 1:		chr_no = p_ai->al_begin[1].Int;
			case 0:		koe_no = p_ai->al_begin[0].Int;
		}

		int rf_scn_no = Gp_lexer->get_cur_scn_no();		// 既読フラグのシーン番号は現在のシーン番号
		int rf_flag_no = Gp_lexer->pop_ret<int>();		// 既読フラグのフラグ番号を取得

		tnm_msg_proc_koe(p_mwnd, koe_no, chr_no, rf_scn_no, rf_flag_no, true, false, false, false);
		break;
	}
	case ELM_MWND_KOE_PLAY_WAIT_KEY:
	{
		int koe_no = 0, chr_no = -1;
		switch (al_id)	{	// no break!
			case 1:		chr_no = p_ai->al_begin[1].Int;
			case 0:		koe_no = p_ai->al_begin[0].Int;
		}

		int rf_scn_no = Gp_lexer->get_cur_scn_no();		// 既読フラグのシーン番号は現在のシーン番号
		int rf_flag_no = Gp_lexer->pop_ret<int>();		// 既読フラグのフラグ番号を取得

		tnm_msg_proc_koe(p_mwnd, koe_no, chr_no, rf_scn_no, rf_flag_no, true, true, false, true);
		break;
	}

	// 顔グラ
	case ELM_MWND_CLEAR_FACE:
		p_mwnd->clear_face();
		break;

	case ELM_MWND_SET_FACE:
	{
		int face_no = 0;
		TSTR face_file;

		if (al_id == 0)	{
			face_file = p_ai->al_begin[0].str;
		}
		else if (al_id == 1)	{
			face_no = p_ai->al_begin[0].Int;
			face_file = p_ai->al_begin[1].str;
		}

		tnm_msg_proc_face(p_mwnd, face_no, face_file);
		break;
	}

	// レイヤー
	case ELM_MWND_LAYER:
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_mwnd->get_layer());
		else if (al_id == 1)	p_mwnd->set_layer(p_ai->al_begin[0].Int);
		break;

	// ワールド
	case ELM_MWND_WORLD:
		if (false);
		else if (al_id == 0)	tnm_stack_push_int(p_mwnd->get_world());
		else if (al_id == 1)	p_mwnd->set_world(p_ai->al_begin[0].Int);
		break;

	// ボタン
	case ELM_MWND_BUTTON:
		tnm_command_proc_object_list(&p_mwnd->button_list(), elm_top, elm_begin + 1, elm_end, p_ai);
		break;

	// 顔グラ
	case ELM_MWND_FACE:
		tnm_command_proc_object_list(&p_mwnd->face_list(), elm_top, elm_begin + 1, elm_end, p_ai);
		break;

	// オブジェクト
	case ELM_MWND_OBJECT:
		tnm_command_proc_object_list(&p_mwnd->object_list(), elm_top, elm_begin + 1, elm_end, p_ai);
		break;

	// パラメータ
	case ELM_MWND_INIT_WINDOW_POS:
		p_mwnd->init_window_pos();
		break;
	case ELM_MWND_INIT_WINDOW_SIZE:
		p_mwnd->init_window_size();
		break;
	case ELM_MWND_INIT_WINDOW_MOJI_CNT:
		p_mwnd->init_window_moji_cnt();
		break;
	case ELM_MWND_SET_WINDOW_POS:
		p_mwnd->set_window_pos(C_point(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
		break;
	case ELM_MWND_SET_WINDOW_SIZE:
		p_mwnd->set_window_size(C_size(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
		break;
	case ELM_MWND_SET_WINDOW_MOJI_CNT:
		p_mwnd->set_window_moji_cnt(C_size(p_ai->al_begin[0].Int, p_ai->al_begin[1].Int));
		break;
	case ELM_MWND_GET_WINDOW_POS_X:
		tnm_stack_push_int(p_mwnd->get_window_pos().x);
		break;
	case ELM_MWND_GET_WINDOW_POS_Y:
		tnm_stack_push_int(p_mwnd->get_window_pos().y);
		break;
	case ELM_MWND_GET_WINDOW_SIZE_X:
		tnm_stack_push_int(p_mwnd->get_window_size().cx);
		break;
	case ELM_MWND_GET_WINDOW_SIZE_Y:
		tnm_stack_push_int(p_mwnd->get_window_size().cy);
		break;
	case ELM_MWND_GET_WINDOW_MOJI_CNT_X:
		tnm_stack_push_int(p_mwnd->get_window_moji_cnt().cx);
		break;
	case ELM_MWND_GET_WINDOW_MOJI_CNT_Y:
		tnm_stack_push_int(p_mwnd->get_window_moji_cnt().cy);
		break;

	// 開閉アニメ
	case ELM_MWND_INIT_OPEN_ANIME_TYPE:
		p_mwnd->init_open_anime_type();
		break;
	case ELM_MWND_INIT_OPEN_ANIME_TIME:
		p_mwnd->init_open_anime_time();
		break;
	case ELM_MWND_INIT_CLOSE_ANIME_TYPE:
		p_mwnd->init_close_anime_type();
		break;
	case ELM_MWND_INIT_CLOSE_ANIME_TIME:
		p_mwnd->init_close_anime_time();
		break;
	case ELM_MWND_SET_OPEN_ANIME_TYPE:
		p_mwnd->set_open_anime_type(p_ai->al_begin[0].Int);
		break;
	case ELM_MWND_SET_OPEN_ANIME_TIME:
		p_mwnd->set_open_anime_time(p_ai->al_begin[0].Int);
		break;
	case ELM_MWND_SET_CLOSE_ANIME_TYPE:
		p_mwnd->set_close_anime_type(p_ai->al_begin[0].Int);
		break;
	case ELM_MWND_SET_CLOSE_ANIME_TIME:
		p_mwnd->set_close_anime_time(p_ai->al_begin[0].Int);
		break;
	case ELM_MWND_GET_OPEN_ANIME_TYPE:
		tnm_stack_push_int(p_mwnd->get_open_anime_type());
		break;
	case ELM_MWND_GET_OPEN_ANIME_TIME:
		tnm_stack_push_int(p_mwnd->get_open_anime_time());
		break;
	case ELM_MWND_GET_CLOSE_ANIME_TYPE:
		tnm_stack_push_int(p_mwnd->get_close_anime_type());
		break;
	case ELM_MWND_GET_CLOSE_ANIME_TIME:
		tnm_stack_push_int(p_mwnd->get_close_anime_time());
		break;
	case ELM_MWND_GET_DEFAULT_OPEN_ANIME_TYPE:
		tnm_stack_push_int(p_mwnd->get_default_open_anime_type());
		break;
	case ELM_MWND_GET_DEFAULT_OPEN_ANIME_TIME:
		tnm_stack_push_int(p_mwnd->get_default_open_anime_time());
		break;
	case ELM_MWND_GET_DEFAULT_CLOSE_ANIME_TYPE:
		tnm_stack_push_int(p_mwnd->get_default_close_anime_type());
		break;
	case ELM_MWND_GET_DEFAULT_CLOSE_ANIME_TIME:
		tnm_stack_push_int(p_mwnd->get_default_close_anime_time());
		break;

	default:
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。") + p_mwnd->get_element_name());
		break;
	}

	return true;
}

// ****************************************************************
// コマンド処理：ボタン選択肢アイテムリスト
// ================================================================
bool tnm_command_proc_btnsel_item_list(C_elm_btn_select_item_list* p_item_list, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_item_list;
	}
	else if (elm_begin[0] == ELM_ARRAY)	{
		C_elm_btn_select_item* p_item = p_item_list->get_sub(elm_begin[1], p_ai->disp_out_of_range_error);
		if (p_item)	{
			tnm_command_proc_btnsel_item(p_item, elm_top, elm_begin + 2, elm_end, p_ai);
		}
	}
	else if (elm_begin[0] == ELM_GROUPLIST_ALLOC)	{
		p_item_list->clear();
		p_item_list->resize(p_ai->al_begin[0].Int);
	}
	else if (elm_begin[0] == ELM_GROUPLIST_FREE)	{
		p_item_list->clear();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(btnselitem_list)") + p_item_list->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：ボタン選択肢アイテム
// ================================================================
bool tnm_command_proc_btnsel_item(C_elm_btn_select_item* p_item, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = p_item;
	}
	else if (elm_begin[0] == ELM_BTNSELITEM_OBJECT)	{
		tnm_command_proc_object_list(&p_item->m_object_list, elm_top, elm_begin + 1, elm_end, p_ai);
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(btnselitem)") + p_item->get_element_name());
	}

	return true;
}

// ****************************************************************
// コマンド処理：メッセージバック
// ================================================================
static void tnm_get_debug_open_scene(S_tnm_command_proc_arg_struct* p_ai, int* p_ret_scene_no, int* p_ret_line_no)
{
	TSTR scene_name;
	int scene_no = 0, line_no = 0;

	// 名前付き引数
	for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
		switch (arg->id)	{
		case 0:		scene_name = arg->str;	break;
		case 1:		line_no = arg->Int;		break;
		}
	}

	// シーン番号を取得
	if (!scene_name.empty())	{
		scene_no = Gp_lexer->get_scn_no(scene_name);
		if (scene_no == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("ss ファイル ") + scene_name + _T(" が見つかりません。"));
		}
	}
	else	{
		scene_no = Gp_lexer->get_cur_scn_no();
	}
	// 行番号を取得
	if (line_no <= 0)	{
		line_no = Gp_lexer->get_cur_line_no();
	}

	// 結果を返す
	*p_ret_scene_no = scene_no;
	*p_ret_line_no = line_no;
}

bool tnm_command_proc_msgbk(C_elm_msg_back* p_msgbk, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	if (elm_begin == elm_end)	{
		p_ai->p_ret_element = (C_tnm_element *)p_msgbk;
	}
	else if (elm_begin[0] == ELM_MSGBK_INSERT_MSG)	{
		TSTR msg = p_ai->al_begin[0].str;
		int scene_no = 0, line_no = 0;

		// デバッグで開くシーンを取得
		tnm_get_debug_open_scene(p_ai, &scene_no, &line_no);
		// 正常な場合のみ処理
		if (scene_no >= 0)	{
			tnm_msg_back_next();
			tnm_msg_proc_msg_back_add_msg(msg, msg, scene_no, line_no);
		}
	}
	else if (elm_begin[0] == ELM_MSGBK_ADD_KOE)	{
		int koe_no = 0, chara_no = -1;
		int scene_no = 0, line_no = 0;

		switch (p_ai->al_id)	{	// no break!
		case 1:		chara_no = p_ai->al_begin[1].Int;
		case 0:		koe_no = p_ai->al_begin[0].Int;
		}

		// デバッグで開くシーンを取得
		tnm_get_debug_open_scene(p_ai, &scene_no, &line_no);
		// 正常な場合のみ処理
		if (scene_no >= 0)	{
			tnm_msg_back_add_koe(koe_no, chara_no, scene_no, line_no);
		}
	}
	else if (elm_begin[0] == ELM_MSGBK_ADD_NAMAE)	{
		TSTR name = p_ai->al_begin[0].str;
		int scene_no = 0, line_no = 0;

		// デバッグで開くシーンを取得
		tnm_get_debug_open_scene(p_ai, &scene_no, &line_no);
		// 正常な場合のみ処理
		if (scene_no >= 0)	{
			tnm_msg_back_add_name(name, name, scene_no, line_no);
		}
	}
	else if (elm_begin[0] == ELM_MSGBK_ADD_MSG)	{
		TSTR msg = p_ai->al_begin[0].str;
		int scene_no = 0, line_no = 0;

		// デバッグで開くシーンを取得
		tnm_get_debug_open_scene(p_ai, &scene_no, &line_no);
		// 正常な場合のみ処理
		if (scene_no >= 0)	{
			tnm_msg_proc_msg_back_add_msg(msg, msg, scene_no, line_no);
		}
	}
	else if (elm_begin[0] == ELM_MSGBK_GO_NEXT_MSG)	{

		tnm_msg_back_next();
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(msgbk)") + p_msgbk->get_element_name());
	}

	return true;
}
