#include	"pch.h"
#include	"../resource.h"

#include	"main.h"
#include	"data.h"
#include	"dialog.h"
#include	"project_setting_dialog.h"

// ****************************************************************
// 開く（作成）
// ================================================================
bool C_tnm_project_setting_dialog::open(HWND parent_h_wnd, PROJECT_SETTING_MODE proc_mode)
{
	work.proc_mode = proc_mode;
	work.return_value = false;
	create(IDD_PROJECT_SETTING, _T("project_setting_dialog"), true, parent_h_wnd);
	return work.return_value;
}

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT	C_tnm_project_setting_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init(wp, lp))		return FALSE;	break;
		case WM_CLOSE:			if (!on_close(wp, lp))		return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// 初期化処理
// ================================================================

#define			DEFAULT_NEW_PROJECT_NAME_STR			_T("＜新規プロジェクト＞")

bool C_tnm_project_setting_dialog::on_init(WPARAM wp, LPARAM lp)
{
	// アイテム登録
	bind_control(&ci.edt_project_name, IDC_EDT_PROJECT_NAME, _T(""), 8);
	bind_control(&ci.edt_scn_path, IDC_EDT_SCN_PATH, _T(""), 8);
	bind_control(&ci.edt_tmp_path, IDC_EDT_TMP_PATH, _T(""), 8);
	bind_control(&ci.edt_out_path, IDC_EDT_OUT_PATH, _T(""), 8);
	bind_control(&ci.edt_out_path_noangou, IDC_EDT_OUT_PATH_NOANGOU, _T(""), 8);
	bind_control(&ci.edt_exe_path, IDC_EDT_EXE_PATH, _T(""), 8);
	bind_control(&ci.edt_work_path, IDC_EDT_WORK_PATH, _T(""), 8);
	bind_control(&ci.edt_editor_path, IDC_EDT_EDITOR_PATH, _T(""), 8);
	bind_control(&ci.edt_manual_path, IDC_EDT_MANUAL_PATH, _T(""), 8);
	bind_control(&ci.edt_gameexe_ini, IDC_EDT_GAMEEXE_INI, _T(""), 8);
	bind_control(&ci.edt_extend_inc, IDC_EDT_EXTEND_INC, _T(""), 8);
	bind_control(&ci.edt_gameexe_dat, IDC_EDT_GAMEEXE_DAT, _T(""), 9);
	bind_control(&ci.edt_scene_pck, IDC_EDT_SCENE_PCK, _T(""), 9);
	bind_control(&ci.stc_dragdrop, IDC_STC_DRAGDROP, _T(""), 5);
	bind_control(&ci.chk_utf8, IDC_CHK_UTF8, _T(""), 1);
	bind_control(&ci.btn_ok, IDC_BTN_OK, _T(""), 2);
	bind_control(&ci.btn_cancel, IDC_BTN_CANCEL, _T(""), 1);

	// アイテム設定
	if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
	{
		// 新規プロジェクトの場合の初期設定
		ci.edt_project_name.set_text(DEFAULT_NEW_PROJECT_NAME_STR);
		ci.edt_gameexe_ini.set_text(_T("Gameexe.ini"));
		ci.edt_gameexe_dat.set_text(_T("Gameexe.dat"));
		ci.edt_scene_pck.set_text(_T("Scene.pck"));

		// 有効なテキストエディターのパスを移植する
		for (ARRAY<S_tnm_compiler_project_data>::iterator itr = G_data->project.begin(); itr != G_data->project.end(); ++itr)
		{
			if (file_get_type(itr->editor_path) == 1)
			{
				ci.edt_editor_path.set_text(itr->editor_path);
				break;
			}
		}

		// 有効なマニュアルのパスを移植する
		for (ARRAY<S_tnm_compiler_project_data>::iterator itr = G_data->project.begin(); itr != G_data->project.end(); ++itr)
		{
			if (file_get_type(itr->manual_path) != -1)
			{
				ci.edt_manual_path.set_text(itr->manual_path);
				break;
			}
		}
	}
	else
	{
		// 既存のプロジェクトの場合の初期設定
		ci.edt_project_name.set_text(G_data->project[G_data->cur_project_no].project_name);
		ci.edt_scn_path.set_text(G_data->project[G_data->cur_project_no].scn_path);
		ci.edt_tmp_path.set_text(G_data->project[G_data->cur_project_no].tmp_path);
		ci.edt_out_path.set_text(G_data->project[G_data->cur_project_no].out_path);
		ci.edt_out_path_noangou.set_text(G_data->project[G_data->cur_project_no].out_path_noangou);
		ci.edt_exe_path.set_text(G_data->project[G_data->cur_project_no].exe_path);
		ci.edt_work_path.set_text(G_data->project[G_data->cur_project_no].work_path);
		ci.edt_editor_path.set_text(G_data->project[G_data->cur_project_no].editor_path);
		ci.edt_manual_path.set_text(G_data->project[G_data->cur_project_no].manual_path);
		ci.edt_gameexe_ini.set_text(G_data->project[G_data->cur_project_no].gameexe_ini);
		ci.edt_extend_inc.set_text(G_data->project[G_data->cur_project_no].extend_inc);
		ci.edt_gameexe_dat.set_text(G_data->project[G_data->cur_project_no].gameexe_dat);
		ci.edt_scene_pck.set_text(G_data->project[G_data->cur_project_no].scene_pck);
		ci.chk_utf8.set_check(G_data->project[G_data->cur_project_no].utf8);
	}

	// ボタンの文字設定
	switch (work.proc_mode)
	{
		case PROJECT_SETTING_MODE_SETTING:
			ci.btn_ok.set_text(_T("ＯＫ ： プロジェクトを 【変更】 する"));
			set_text(_T("プロジェクト設定"));
			break;
		case PROJECT_SETTING_MODE_CREATE:
			ci.btn_ok.set_text(_T("ＯＫ ： プロジェクトを 【追加】 する"));
			set_text(_T("プロジェクト追加"));
			break;
		case PROJECT_SETTING_MODE_DELETE:
			ci.btn_ok.set_text(_T("ＯＫ ： プロジェクトを 【削除】 する"));
			set_text(_T("プロジェクト削除"));
			// アイテムを禁止状態にする
			ci.edt_project_name.set_enable(false);
			ci.edt_scn_path.set_enable(false);
			ci.edt_tmp_path.set_enable(false);
			ci.edt_out_path.set_enable(false);
			ci.edt_out_path_noangou.set_enable(false);
			ci.edt_exe_path.set_enable(false);
			ci.edt_work_path.set_enable(false);
			ci.edt_editor_path.set_enable(false);
			ci.edt_manual_path.set_enable(false);
			ci.stc_dragdrop.set_enable(false);
			ci.chk_utf8.set_enable(false);
			break;
	}

	// ドラッグアンドドロップアイテムへ親クラスを設定
	ci.stc_dragdrop.p_parent_class = this;

	// ウィンドウ矩形ロード
	load_ini_window_rect();

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// ****************************************************************
// クローズ処理
// ================================================================
bool C_tnm_project_setting_dialog::on_close(WPARAM wp, LPARAM lp)
{
	// ＯＫで終了
	if (work.return_value)
	{
		bool update_flag = true;

		// プロジェクト作成の場合
		if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
		{
			if (
				ci.edt_project_name.get_text() == DEFAULT_NEW_PROJECT_NAME_STR &&
				ci.edt_scn_path.get_text() == _T("") &&
				ci.edt_tmp_path.get_text() == _T("") &&
				ci.edt_out_path.get_text() == _T("") &&
				ci.edt_out_path_noangou.get_text() == _T("") &&
				ci.edt_exe_path.get_text() == _T("") &&
				ci.edt_work_path.get_text() == _T("")
			)	{
				// 設定が無いのでキャンセル扱い
				update_flag = false;	// 更新しない
			}
			else
			{
				// プロジェクトを追加して選択する
				G_data->project.resize(G_data->project.size() + 1);
				G_data->cur_project_no = (int)G_data->project.size() - 1;
			}
		}

		// プロジェクト削除の場合
		if (work.proc_mode == PROJECT_SETTING_MODE_DELETE)
		{
			// プロジェクトを削除して１つ前を選択する
			G_dlg->delete_ini_project(_T(""), _T("project_") + tostr(G_data->cur_project_no));	// ini から削除
			G_data->project.erase(G_data->project.begin() + G_data->cur_project_no);
			G_data->cur_project_no--;
			update_flag = false;		// 更新しない
		}

		// プロジェクトの更新
		if (update_flag)
		{
			G_data->project[G_data->cur_project_no].project_name = ci.edt_project_name.get_text();
			G_data->project[G_data->cur_project_no].scn_path = ci.edt_scn_path.get_text();
			G_data->project[G_data->cur_project_no].tmp_path = ci.edt_tmp_path.get_text();
			G_data->project[G_data->cur_project_no].out_path = ci.edt_out_path.get_text();
			G_data->project[G_data->cur_project_no].out_path_noangou = ci.edt_out_path_noangou.get_text();
			G_data->project[G_data->cur_project_no].exe_path = ci.edt_exe_path.get_text();
			G_data->project[G_data->cur_project_no].work_path = ci.edt_work_path.get_text();
			G_data->project[G_data->cur_project_no].editor_path = ci.edt_editor_path.get_text();
			G_data->project[G_data->cur_project_no].manual_path = ci.edt_manual_path.get_text();
			G_data->project[G_data->cur_project_no].gameexe_ini = ci.edt_gameexe_ini.get_text();
			G_data->project[G_data->cur_project_no].extend_inc = ci.edt_extend_inc.get_text();
			G_data->project[G_data->cur_project_no].gameexe_dat = ci.edt_gameexe_dat.get_text();
			G_data->project[G_data->cur_project_no].scene_pck = ci.edt_scene_pck.get_text();
			G_data->project[G_data->cur_project_no].utf8 = ci.chk_utf8.get_check();

			if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
			{
				G_data->project[G_data->cur_project_no].start_check = false;
				G_data->project[G_data->cur_project_no].start_scene = _T("");
			}
		}
	}

	// ウィンドウ矩形セーブ
	save_ini_window_rect();

	return true;
}

// ****************************************************************
// コマンド処理
// ================================================================
bool C_tnm_project_setting_dialog::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_STC_DRAGDROP:
			break;
		case IDC_BTN_OK:
			work.return_value = true;
			close();
			break;
		case IDC_BTN_CANCEL:
			close();
			break;
	}
	return true;
}

// ****************************************************************
// ドラッグアンドドロップで一括パス設定
// ================================================================
void C_tnm_project_setting_dialog::set_path_easy(TSTR path)
{
	if (file_get_type(path) != 0)	{	return;	}

	// ＳＳパス
	TSTR find_path;
	find_path = path + _T("\\ss");
	if (file_get_type(find_path) == 0)	{
		ci.edt_scn_path.set_text(find_path);
	}

	// ＴＭＰパス
	find_path = path + _T("\\tmp");
	if (file_get_type(find_path) == 0)	{
		ci.edt_tmp_path.set_text(find_path);
	}

	// 出力パス
	find_path = path + _T("\\exe\\setupdata\\gamedata");
	if (file_get_type(find_path) == 0)	{
		ci.edt_out_path.set_text(find_path);
		set_path_easy_for_exe_path(find_path);			// ＥＸＥパス
	}
	else	{
		find_path = path + _T("\\exe");
		if (file_get_type(find_path) == 0)	{
			ci.edt_out_path.set_text(find_path);
			set_path_easy_for_exe_path(find_path);		// ＥＸＥパス
		}
	}

	// 作業パス
	find_path = path + _T("\\work");
	if (file_get_type(find_path) == 0)	{
		ci.edt_work_path.set_text(find_path);
	}
}

// ****************************************************************
// ドラッグアンドドロップで一括パス設定（ＥＸＥパス用）
// ================================================================
void C_tnm_project_setting_dialog::set_path_easy_for_exe_path(TSTR path)
{
		TSTR exe_path;
#ifdef _DEBUG
		exe_path = path + _T("\\SiglusEngine_d.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_t.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#elif _TEST
		exe_path = path + _T("\\SiglusEngine_t.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_d.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#else
		exe_path = path + _T("\\SiglusEngine.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine_d.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_t.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#endif
}

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT	C_tnm_project_setting_dialog::Cc_stc_dragdrop::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_DROPFILES:
			p_parent_class->set_path_easy(get_droped_link_path(wp));
			break;
	}

	return Cc_static::window_proc(msg, wp, lp);
}
