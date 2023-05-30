#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_cmd_line.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_error.h"

#include	"engine/eng_msgbox.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_proc_stack.h"

#include	"localize/localize.h"

HHOOK G_h_hook = NULL;

static void set_yes_no_button_text(HWND h_wnd, CTSTR& yes_text, CTSTR& no_text)
{
	::SetDlgItemText(h_wnd, IDYES, yes_text.c_str());
	::SetDlgItemText(h_wnd, IDNO, no_text.c_str());
}

LRESULT CALLBACK msg_box_hook_proc(int n_code, WPARAM wp, LPARAM lp)
{
	if (n_code >= 0)	{
		set_yes_no_button_text((HWND)wp, _T("続行"), _T("ｹﾞｰﾑ終了"));
	}
	return CallNextHookEx(G_h_hook, n_code, wp, lp);
}

const int TNM_ERROR_DEPTH_NONE = 0;		// エラー表示しない
const int TNM_ERROR_DEPTH_LOG = 1;		// ログにのみ出力
const int TNM_ERROR_DEPTH_BOX = 2;		// メッセージボックスを出す


void write_debug_log(CTSTR& str)
{
	// デバッグ時のみ実行
	if (Gp_global->debug_flag)
	{
		TSTR log;
		C_file log_file;

		// ディレクトリ作成
		if (!create_dir_from_file_path(Gp_dir->get_debug_log_file_path()))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ログファイル書き出し用のフォルダが作成できませんでした。\n") + get_last_error_msg());
		}
		else if (!log_file.open(Gp_dir->get_debug_log_file_path(), _T("at")))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ログファイルが開けませんでした。\n") + get_last_error_msg());
		}
		else
		{
			// 日付を入れる
			log = _T("[") + get_sys_time_str() + _T("] ");

			// シーン番号を追加
			int scn_no = Gp_lexer->get_cur_scn_no();
			if (scn_no >= 0)
			{
				TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
				int line_no = Gp_lexer->get_cur_line_no();
				log += _T("\t(") + scn_name + _T(".ss line=") + tostr(line_no) + _T(")");
			}

			// エラーログに出力
			log_file.write_in_MB(log + _T("\t") + str + _T("\n"));
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str);
		}
	}
}

// ****************************************************************
// エンジン：エラー処理
// ================================================================
//		必ずエラーボックスを出すべきもの（ファイルエラーなど）
//		デバッグ版のみエラーボックスを出すべきもの（オーバーフローなど）
//		デバッグダイアログに表示するだけのもの（成功など）
// ================================================================
void C_tnm_error::init()
{
	m_last_error = TNM_ERROR_TYPE_NONE;
}

bool C_tnm_error::set_error(TNM_ERROR_TYPE error_type, CTSTR& error_str)
{
	// 設定によってはエラーとしないものをはじく

	// パターン番号エラー
	if (error_type == TNM_ERROR_TYPE_PAT_NO)	{
		if (!Gp_global->debug_flag)	{
			return true;	// リリースならエラーではない
		}
		else if (Gp_ini->debug_error_patno_out_of_range == TNM_ERROR_DEPTH_NONE)	{
			return true;	// デバッグ設定によってはエラーではない
		}
	}

	// エラー構造体
	S_tnm_error error;
	error.error_type = error_type;
	error.error_str = error_str;
	
	// 日付を入れる
	error.error_str = _T("[") + get_sys_time_str() + _T("] \n\n") + error.error_str;

	// シーン番号が確定していればエラーに追加（デバッグ時のみ）
	if (Gp_global->debug_flag)	{
		int scn_no = Gp_lexer->get_cur_scn_no();
		if (scn_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			int line_no = Gp_lexer->get_cur_line_no();
			error.error_str += _T("\n\n(") + scn_name + _T(".ss ") + tostr(line_no) + _T("行目)");
		}
	}

	// デバッグ時はメッセージボックスの挙動をフックする
	DWORD additional_flag = 0;
	if (Gp_global->debug_flag)	{
		G_h_hook = ::SetWindowsHookEx(WH_CBT, msg_box_hook_proc, NULL, ::GetCurrentThreadId());
		additional_flag = MB_YESNO;
	}

	// エラーボックスを出す
	int res = 0;
	switch (error_type)	{
		case TNM_ERROR_TYPE_DIVIDED_BY_ZERO:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_OUT_OF_RANGE:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_OVERFLOW:		// 設定によってはエラーボックスを出す
			if (Gp_global->debug_flag && !Gp_global->debug_not_disp_overflow_error)	{
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_FILE_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_PCT_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_SOUND_NOT_FOUND:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_KOE_NOT_FOUND:	// エラーボックスを出さない！！！
			break;
		case TNM_ERROR_TYPE_PAT_NO:
			if (Gp_ini->debug_error_patno_out_of_range == TNM_ERROR_DEPTH_BOX)	{	// ini の設定によってはエラーボックスを出す
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_INFO:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONINFORMATION | additional_flag);
			break;
		case TNM_ERROR_TYPE_FATAL:
			res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			break;
		case TNM_ERROR_TYPE_DEBUG:			// デバッグ時のみエラーボックスを出す
			if (Gp_global->debug_flag)	{
				res = tnm_game_message_box(error.error_str + LCL_STR_GENERAL_SUPPORT_STR, MB_ICONWARNING | additional_flag);
			}
			break;
		case TNM_ERROR_TYPE_WARNING:		// エラーボックスは出さない
			break;
		case TNM_ERROR_TYPE_SUCCESS:		// エラーボックスは出さない
			break;
	}

	// デバッグ時はメッセージボックスのフックを終了する
	if (Gp_global->debug_flag)	{
		::UnhookWindowsHookEx(G_h_hook);	// フック終了
		if (res == IDNO)	{

				// ゲームを終了するフラグ
				Gp_global->game_end_flag = true;
				// 警告なしでゲームを終了するフラグ
				Gp_global->game_end_no_warning = true;
				// エンドセーブなしでゲームを終了するフラグ
				Gp_global->game_end_save_done = true;
				// ループを抜けるよう
				tnm_push_proc(TNM_PROC_TYPE_NONE);

			//tnm_push_proc(TNM_PROC_TYPE_END_GAME);		// ゲームを終了する
		}
	}

	// エラーリストに追加
	if (Gp_global->debug_flag || !Gp_global->debug_decided_flag)
		m_error_list.push_back(error);

	// ログに吐き出す
	if (Gp_cmd_line->log_set)	{

		// セーブフォルダが出来ている場合
		if (!Gp_dir->save_dir.empty())	{

			C_file file;
			if (!file.open(Gp_dir->get_log_file_path(), _T("at")))
				return false;

			// 残っているエラーを吐き出す
			for (int i = 0; i < (int)m_rest_error_list.size(); i++)	{
				file.write_in_MB((m_rest_error_list[i] + _T("\n")).c_str());
			}
			m_rest_error_list.clear();

			// 今回のエラーを吐き出す
			file.write_in_MB((error_str + _T("\n")).c_str());

			// 忘れずにファイルを閉じる！
			file.close();
		}

		// セーブフォルダが出来ていない場合
		else	{

			m_rest_error_list.push_back(error_str);
		}
	}

	// ラストエラーを設定
	m_last_error = error_type;

	return false;
}
