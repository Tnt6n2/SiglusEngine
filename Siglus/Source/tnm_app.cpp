#include	"pch.h"
#include	"tnm_app.h"
#include	"data/tnm_gan_manager.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_sound.h"
#include	"../resource.h"

// ****************************************************************
// グローバルなアクセスのためのポインタ
// ================================================================
class	C_tnm_select_ini;
class	C_tnm_ini;
class	C_tnm_reg_ini;
class	C_tnm_cmd_line;
class	C_tnm_eng;

C_tnm_select_ini*		Gp_select_ini;
C_tnm_ini*				Gp_ini;
C_tnm_reg_ini*			Gp_reg_ini;
C_tnm_cmd_line*			Gp_cmd_line;
C_tnm_eng*				Gp_eng;

// ****************************************************************
// 暗号挿入の為の検索コード
// ================================================================

#include	"tnm_exe_angou.h"

volatile BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE00;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE01;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE02;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE03;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE04;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE05;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE06;		// ★暗号挿入の為の検索コード
volatile BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE07;		// ★暗号挿入の為の検索コード


// ****************************************************************
// となまアプリケーション：メイン
// ================================================================
bool C_tnm_app::main()
{
	// グローバルなポインタを設定する
	Gp_reg_ini = &m_reg_ini;
	Gp_select_ini = &m_select_ini;
	Gp_ini = &m_ini;
	Gp_cmd_line = &m_cmd_line;
	Gp_eng = &m_eng;

	//
	// まずはコマンドラインや設定ファイルを解析します。
	// ディレクトリの設定、ゲーム名、エラー処理方法などはまだ決定されていないので、
	// 異常時はメッセージボックスを出して速やかに終了します。
	//

	// コマンドラインを解析する
	if (!Gp_cmd_line->analize_command_line())
	{
		message_box(_T("コマンドラインの解析に失敗しました。"), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Reg.ini を解析する
	if (!Gp_reg_ini->analize())
	{
		message_box(_T("設定ファイル Reg.ini の解析に失敗しました。\nゲームを再セットアップすることでこの問題は解決する可能性があります。"), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Select.ini を解析する
	if (!Gp_select_ini->analize())
	{
		message_box(_T("設定ファイル Select.ini の解析に失敗しました。\n") + get_last_error_msg(), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Gameexe.dat を解析する
	if (!Gp_ini->analize())
	{
		message_box(_T("設定ファイル Gameexe.dat の解析に失敗しました。\n") + get_last_error_msg(), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	//
	// ゲーム名が確定しました。
	// 以降のエラー処理では、メッセージボックスのキャプションにゲーム名を使って下さい。
	//

	// 既に起動している Siglus エンジンを検索する
	if (!Gp_cmd_line->double_start_set)
	{
		TSTR window_name = tnm_get_window_name();
		HWND another_h_wnd = ::FindWindow(window_name.c_str(), NULL);
		if (another_h_wnd)
		{
			// コマンドラインで再起動が指定されている場合
			if (Gp_cmd_line->end_start_set)
			{
				//  既に起動している Siglus エンジンを終了する
				::SendMessage(another_h_wnd, WM_COMMAND, MAKELONG(0, IDC_END), 0);

				// 終了するのを待つ（この間に設定などを保存してくれると信じてる）
				Sleep(200);
			}

			// 通常は既に起動しているので起動しない
			else
			{
				message_box(Gp_ini->game_name + _T(" は既に起動されています。"), Gp_ini->game_name, MB_ICONWARNING);
				return false;
			}
		}
	}

	// エンジンのメイン処理を行う
	m_eng.main();

	return true;
}



