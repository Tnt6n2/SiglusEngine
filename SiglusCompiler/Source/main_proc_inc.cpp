#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"

// ****************************************************************
// 処理：inc ファイル解析
// ================================================================
bool C_tnm_compiler_main::proc_inc(S_tnms_ia_data* piad)
{
	// ia データを初期化
	piad->init();

	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;
	bool utf8 = G_data->project[G_data->cur_project_no].utf8;
	bool test_check = G_dlg->ci.chk_test.get_check();

	// inc ファイル解析開始
	set_log(_T("- inc ファイルを解析します。"));

	// inc ファイル解析
	C_inc_file_analizer ia;
	int inc_cnt = (int)G_data->inc_list.size();
	ARRAY<TSTR> inc_file_list(inc_cnt), inc_name_list(inc_cnt), inc_path_list(inc_cnt);
	ARRAY<S_tnms_ia_data_2> ia_data_2(inc_cnt);
	for (int i = 0; i < (int)inc_cnt; i++)
	{
		inc_file_list[i] = G_data->inc_list[i];
		inc_name_list[i] = erase_extension_ret(G_data->inc_list[i]);
		inc_path_list[i] = scn_dir_path + _T("\\") + G_data->inc_list[i];

		// inc ファイル解析開始
		set_log(inc_file_list[i]);

		TSTR text;
		utf8 ? C_file::read_full_text_UTF8(inc_path_list[i], text) : C_file::read_full_text(inc_path_list[i], text);
		if (!ia.analize_step_1(text, FM_GLOBAL, piad, &ia_data_2[i]))
		{
			set_log_jump(inc_name_list[i] + _T(" line(") + tostr(ia.get_error_line()) + _T("): ") + ia.get_error_str(), inc_path_list[i], inc_path_list[i], ia.get_error_line());
			return false;
		}
		
		// inc ファイルの解析結果を出力（実際は作った日付が分かればいいので、内容は何でもいい）
		TSTR inc_out_path = tmp_dir_path + _T("\\inc\\") + inc_name_list[i] + _T(".txt");
		create_dir_from_file_path(inc_out_path);
		utf8 ? C_file::write_full_text_in_UTF8(inc_out_path, _T("OK")) : C_file::write_full_text_in_MB(inc_out_path, _T("OK"));
	}

	// inc ファイル解析ステップ２開始
	set_log(_T("property と command の解析中…。"));
	for (int i = 0; i < (int)inc_cnt; i++)
	{
		if (!ia.analize_step_2(piad, &ia_data_2[i]))
		{
			set_log_jump(inc_name_list[i] + _T(" line(") + tostr(ia.get_error_line()) + _T("): ") + ia.get_error_str(), inc_path_list[i], inc_path_list[i], ia.get_error_line());
			return false;
		}
	}

	return true;
}

