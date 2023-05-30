#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"

#include	"engine/eng_system.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_dummy.h"

#include	"utility/util_dummy_file.h"

#include	"../resource.h"

// ****************************************************************
// ダミーファイルチェック（サブ）
// ================================================================
bool tnm_check_dummy_file_sub(CTSTR& file_name, int key, CTSTR& code)
{
	// ディスクドライブを探す
	TCHAR disc_letter = tnm_search_disc_drive();

	// ディスクを見つけてもレターの変更は行いません。
	// 最小セットアップの場合は、起動時に探しに行ってパスを設定しているはず。

	// ディスクが見つからない場合は失敗
	if (disc_letter == 0)
		return false;

	// ダミーファイルのパスを設定
	int version = 1;
	TSTR dummy_dir = TSTR() + disc_letter + _T(":\\dummy");
	if (file_get_type(dummy_dir) == 0)	{
		version = 2;	// dummy フォルダがあったのでバージョンは２以上
	}
	else	{
		// dummy フォルダがなかったのでバージョン１
		version = 1;
		// フォルダをルートに設定しなおす
		dummy_dir = TSTR() + disc_letter + _T(":");
	}

	// ダミーファイルをチェック
	C_dummy_file_checker dummy_file_checker;
	if (!dummy_file_checker.check_dummy_file(version, dummy_dir, file_name, key, code))	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ダミーファイルが見つかりませんでした。"));
		return false;
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ダミーファイルの判定に成功しました。"));
	return true;
}

// ****************************************************************
// ダミーファイルチェック
// ================================================================
bool tnm_check_dummy_file(CTSTR& file_name, int key, CTSTR& code)
{
	bool return_value = false;

	// コンピュータＩＤとダミーチェックＩＤを比較する
	int computer_id = get_computer_id();
	if (check_dummy_check_flag(computer_id))
		return true;	// 既に完了している場合は何もしない

	// チェック中ダイアログを作成
	C_dialog dlg;
	dlg.create(IDD_DUMMY, _T("dummy"), false, false, G_app.h_wnd);
	dlg.set_text(Gp_ini->game_name);
	dlg.update();

	// 結果が出るまでチェックを繰り返す
	while (1)	{

		// チェック中ダイアログを表示
		dlg.set_show(true);

		// ダイアログを更新するためメッセージループ
		G_app.default_message_loop();

		// 格好悪いのでちょっと待つ
		Sleep(1000);

		// ダミーファイルをチェックする
		bool dummy_check = tnm_check_dummy_file_sub(file_name, key, code);
		
		// チェック中ダイアログを非表示
		dlg.set_show(false);

		// ディスクが見つからなかった
		if (!dummy_check)	{

			// 警告
			TSTR str = str_replace_ret(Gp_ini->dummy_check_str, _T("\\n"), _T("\n"));
			int res = tnm_game_message_box(str, MB_OKCANCEL | MB_ICONINFORMATION);
			if (res == IDCANCEL)	{

				// ゲームを終了するフラグ
				Gp_global->game_end_flag = true;
				// 警告なしでゲームを終了するフラグ
				Gp_global->game_end_no_warning = true;
				// エンドセーブなしでゲームを終了するフラグ
				Gp_global->game_end_save_done = true;
				// ループを抜けるよう
				tnm_push_proc(TNM_PROC_TYPE_NONE);

				return_value = false;
				break;
			}
		}
		// 見つけた
		else	{

			// ダミーチェックＩＤにコンピュータＩＤを設定
			set_dummy_check_flag(computer_id);

			// 認証に成功
			TSTR str = str_replace_ret(Gp_ini->dummy_check_ok_str, _T("\\n"), _T("\n"));
			tnm_game_message_box(str, MB_OK | MB_ICONINFORMATION);

			// 何もせずに終了
			return_value = true;
			break;
		}
	}

	// チェック中ダイアログを閉じる
	dlg.close();

	return return_value;
}

