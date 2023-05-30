#include	"pch.h"
#include	"tona3_main.h"
#include	"data.h"
#include	"dialog.h"
#include	"project_setting_dialog.h"
#include	"main.h"
#include	"exe_angou.h"
#include	"tnm_common_header.h"
#include	"BS.h"

#include <boost/algorithm/string.hpp>

#include	"../resource.h"

C_tnm_compiler_main*	G_main;
C_tnm_compiler_data*	G_data;
C_tnm_compiler_dialog*	G_dlg;

#ifdef __SIGLUS
const TCHAR* const TNMC_WINDOW_TITLE_STR = _T("Siglus スクリプトコンパイラ");
const TCHAR* const TNMC_WINDOW_CLASS_STR = _T("visualarts_siglus_compiler_main_dialog_class");
#endif
#ifdef __KINETIC
const TCHAR* const TNMC_WINDOW_TITLE_STR = _T("Flix スクリプトコンパイラ");
const TCHAR* const TNMC_WINDOW_CLASS_STR = _T("visualarts_siglus_compiler_main_dialog_class");	// クラス Siglus / Flix 共通
#endif

// ****************************************************************
// メイン
// ================================================================
int tona_main()
{
	// 初期設定
	G_app.app_name = _T("siglus_compiler");		// アプリ名は Siglus / Flix 共通

	// GUI フラグ
	bool cl_cui_set = false;
	bool cl_project_no_set = false;
	bool cl_log_set = false;
	bool cl_full_set = false;
	int cl_project_no = -1;
	TSTR cl_log;

	// コマンドラインからモードを取得
	C_str_analizer cmd_anl(G_app.command_line.begin(), G_app.command_line.end());
	while (1)	{
		TSTR str;
		int num = 0;
		if (cmd_anl.check_moji(_T('/')))	{
			if (cmd_anl.check_str(_T("cui")))	{
				cl_cui_set = true;
			}
			else if (cmd_anl.check_str(_T("project_no")))	{
				if (cmd_anl.check_moji(_T('=')))	{
					if (cmd_anl.get_integer(&num))	{
						cl_project_no_set = true;
						cl_project_no = num;
					}
				}
			}
			else if (cmd_anl.check_str(_T("log")))	{
				if (cmd_anl.check_moji(_T('=')))	{
					if (cmd_anl.get_dquote_str(str, _T('\\')))	{
						cl_log_set = true;
						cl_log = str;
					}
				}
			}
			else if (cmd_anl.check_str(_T("full")))	{
				cl_full_set = true;
			}
			else	{
				break;
			}
		}
		else	{
			break;
		}
	}

	// ミューテックスを利用して多重起動防止
	C_mutex mutex;
	if (!mutex.create_mutex(_T("siglus_compiler")))	{
		message_box(_T("ミューテックスの作成に失敗しました。"), TNMC_WINDOW_TITLE_STR, MB_ICONWARNING);
		return EXIT_FAILURE;
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)	{

		// CUI モードの場合はエラー
		if (cl_cui_set)	{

			message_box(_T("SiglusCompiler.exe は既に起動しています。"), TNMC_WINDOW_TITLE_STR, MB_ICONWARNING);
			return EXIT_FAILURE;
		}
		// GUI モードの場合は、コンパイラを見つけてアクティブにする
		else	{

			// 既に起動している Siglus コンパイラを検索する
			HWND another_h_wnd = ::FindWindow(TNMC_WINDOW_CLASS_STR, NULL);
			if (another_h_wnd)	{
				SetForegroundWindow(another_h_wnd);

				// 自身は終了する
				return EXIT_FAILURE;
			}
		}
	}

	// タイマーの精度を上げる
	TIMECAPS caps = {0};
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);

	// 初期設定
	C_ini_file::body().load_ini_file();

	// メインの構築
	C_tnm_compiler_main main;		G_main = &main;
	C_tnm_compiler_data data;		G_data = &data;
	C_tnm_compiler_dialog dialog;	G_dlg = &dialog;

	// GUI フラグを設定
	G_data->gui_flag = !cl_cui_set;
	G_data->cl_project_no_set = cl_project_no_set;
	G_data->cl_project_no = cl_project_no;
	G_data->cl_log_set = cl_log_set;
	G_data->cl_log = cl_log;
	G_data->cl_full_set = cl_full_set;

	// 独自のダイアログクラスを登録する
	WNDCLASS wc;
	::GetClassInfo(G_app.h_inst, _T("#32770"), &wc);	// ダイアログのクラス情報を取得して
	wc.lpszClassName = TNMC_WINDOW_CLASS_STR;			// クラス名だけ変えたクラスを
	if (!::RegisterClass(&wc))	{						// 独自のダイアログクラスとして登録する！
		set_last_error_win(_T("RegisterClass"));
		error_box(_T("メインダイアログのクラスの登録に失敗しました。\n") + get_last_error_msg());
	}
	else	{

		// ダイアログの作成
		if (!G_dlg->create(IDD_TNM_COMPILER, _T("main_dialog"), true, true))	{
			error_box(_T("メインダイアログの作成に失敗しました。\n") + get_last_error_msg());
		}
		else	{

			// メッセージループ
			while (G_app.default_message_loop())
				Sleep(1);

			// 終了処理
			C_ini_file::body().save_ini_file();
		}

		// 独自のダイアログクラスを抹消する
		::UnregisterClass(TNMC_WINDOW_CLASS_STR, G_app.h_inst);
	}

	// タイマーの制度を元に戻す
	timeEndPeriod(caps.wPeriodMin);

	return EXIT_SUCCESS;
}

// ****************************************************************
// コンストラクタ
// ================================================================
C_tnm_compiler_main::C_tnm_compiler_main()
{
	// プロセスフラグ
	proc_execut_flag = false;	// プロセス実行中フラグ
	proc_cancel_flag = false;	// プロセスキャンセルフラグ
}

// ****************************************************************
// ini セーブ
// ================================================================
void C_tnm_compiler_main::save_ini()
{
	// プロジェクト関連のアイテム状態を取得
	get_project_item_state();

	// プロジェクトの設定
	G_dlg->save_ini_int(_T("project_cnt"), (int)G_data->project.size());
	G_dlg->save_ini_int(_T("cur_project_no"), G_data->cur_project_no);
	for (int i = 0; i < (int)G_data->project.size(); ++i)
	{
		G_dlg->save_ini_str(_T("project_name"), G_data->project[i].project_name, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("scn_path"), G_data->project[i].scn_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("tmp_path"), G_data->project[i].tmp_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("out_path"), G_data->project[i].out_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("out_path_noangou"), G_data->project[i].out_path_noangou, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("exe_path"), G_data->project[i].exe_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("work_path"), G_data->project[i].work_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("editor_path"), G_data->project[i].editor_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("manual_path"), G_data->project[i].manual_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("build_after"), G_data->project[i].build_after, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("angou_str"), G_data->project[i].angou_str, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("gameexe_ini"), G_data->project[i].gameexe_ini, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("extend_inc"), G_data->project[i].extend_inc, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("gameexe_dat"), G_data->project[i].gameexe_dat, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("scene_pck"), G_data->project[i].scene_pck, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("start_scene"), G_data->project[i].start_scene, _T("project_") + tostr(i));

		G_dlg->save_ini_int(_T("start_z_no"), G_data->project[i].start_z_no, _T("project_") + tostr(i));

		G_dlg->save_ini_bool(_T("start_check"), G_data->project[i].start_check, _T("project_") + tostr(i));
		G_dlg->save_ini_bool(_T("angou_check"), G_data->project[i].angou_check, _T("project_") + tostr(i));
		G_dlg->save_ini_bool(_T("utf8"), G_data->project[i].utf8, _T("project_") + tostr(i));
	}

	// ウィンドウ座標
	G_dlg->save_ini_window_rect();

	// ダイアログ
	G_dlg->ci.dlgsep00.save_ini();
	G_dlg->ci.dlgsep01.save_ini();
	G_dlg->ci.dlgsep02.save_ini();
	G_dlg->ci.chk_debug_on.save_ini();
	G_dlg->ci.chk_debug_off.save_ini();
	G_dlg->ci.chk_execute.save_ini();
	G_dlg->ci.lv_scn.save_ini();
}

// ****************************************************************
// ini ロード
// ================================================================
void C_tnm_compiler_main::load_ini()
{
	// プロジェクトの設定をロード
	G_data->project.resize(G_dlg->load_ini_int(_T("project_cnt"), 0));
	G_data->cur_project_no = G_dlg->load_ini_int(_T("cur_project_no"), -1);
	for (int i = 0; i < (int)G_data->project.size(); ++i)
	{
		G_data->project[i].project_name = G_dlg->load_ini_str(_T("project_name"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].scn_path = G_dlg->load_ini_str(_T("scn_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].tmp_path = G_dlg->load_ini_str(_T("tmp_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].out_path = G_dlg->load_ini_str(_T("out_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].out_path_noangou = G_dlg->load_ini_str(_T("out_path_noangou"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].exe_path = G_dlg->load_ini_str(_T("exe_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].work_path = G_dlg->load_ini_str(_T("work_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].editor_path = G_dlg->load_ini_str(_T("editor_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].manual_path = G_dlg->load_ini_str(_T("manual_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].build_after = G_dlg->load_ini_str(_T("build_after"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].angou_str = G_dlg->load_ini_str(_T("angou_str"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].start_scene = G_dlg->load_ini_str(_T("start_scene"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].gameexe_ini = G_dlg->load_ini_str(_T("gameexe_ini"), _T("Gameexe.ini"), _T("project_") + tostr(i));
		G_data->project[i].extend_inc = G_dlg->load_ini_str(_T("extend_inc"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].gameexe_dat = G_dlg->load_ini_str(_T("gameexe_dat"), _T("Gameexe.dat"), _T("project_") + tostr(i));
		G_data->project[i].scene_pck = G_dlg->load_ini_str(_T("scene_pck"), _T("Scene.pck"), _T("project_") + tostr(i));

		G_data->project[i].start_z_no = G_dlg->load_ini_int(_T("start_z_no"), 0, _T("project_") + tostr(i));

		G_data->project[i].start_check = G_dlg->load_ini_bool(_T("start_check"), false, _T("project_") + tostr(i));
		G_data->project[i].angou_check = G_dlg->load_ini_bool(_T("angou_check"), false, _T("project_") + tostr(i));
		G_data->project[i].utf8 = G_dlg->load_ini_bool(_T("utf8"), false, _T("project_") + tostr(i));
	}

	// コマンドラインの情報を上書き
	if (G_data->cl_project_no_set)
	{
		G_data->cur_project_no = G_data->cl_project_no;
	}

	// ウィンドウ座標
	G_dlg->load_ini_window_rect();

	// ダイアログの情報をロード
	G_dlg->ci.dlgsep00.load_ini();
	G_dlg->ci.dlgsep01.load_ini();
	G_dlg->ci.dlgsep02.load_ini();
	G_dlg->ci.chk_debug_on.load_ini(true);		// デフォルトでデバッグＯＮ
	G_dlg->ci.chk_debug_off.load_ini(false);
	G_dlg->ci.chk_execute.load_ini(true);
	G_dlg->ci.lv_scn.load_ini();

	// カレントプロジェクトを補正する
	if (G_data->project.empty())
	{
		G_data->cur_project_no = -1;	// プロジェクトが空なら -1 に初期化する
	}
	else if (!check_current_project(false))
	{
		G_data->cur_project_no = 0;		// 目的のプロジェクトが無ければ 0 に補正する。
	}
}

// ****************************************************************
// プロジェクト関連のアイテム状態を取得
// ================================================================
void C_tnm_compiler_main::get_project_item_state()
{
	if (!(0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size()))
		return;

	// スタートシーンのアイテム状態を取得
	G_data->project[G_data->cur_project_no].start_check = G_dlg->ci.chk_start_scene.get_check();
	G_data->project[G_data->cur_project_no].start_scene = G_dlg->ci.edt_start_scene.get_text();
	G_data->project[G_data->cur_project_no].start_z_no = str_to_int(G_dlg->ci.edt_start_z_no.get_text(), 0);
}

// ****************************************************************
// プロジェクト関連のアイテム状態を設定
// ================================================================
void C_tnm_compiler_main::set_project_item_state()
{
	if (!(0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size()))
		return;

	// スタートシーンのアイテム状態を設定
	G_dlg->ci.chk_start_scene.set_check(G_data->project[G_data->cur_project_no].start_check);
	G_dlg->ci.edt_start_scene.set_text(G_data->project[G_data->cur_project_no].start_scene);
	G_dlg->ci.edt_start_z_no.set_text(tostr(G_data->project[G_data->cur_project_no].start_z_no));
}

// ****************************************************************
// ダイアログを更新
// ================================================================

// ウィンドウタイトルに ss のパスを表示
void C_tnm_compiler_main::update_dialog_for_window_title()
{
	if (G_data->cur_project_no < 0)
		return;

	TSTR ss_path_str = _T(" ( ss = ") + G_data->project[G_data->cur_project_no].scn_path + _T(" )");
	if (0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size())
		ss_path_str = _T(" ( SS フォルダ ： ") + G_data->project[G_data->cur_project_no].scn_path + _T(" )");

	G_dlg->set_text(TNMC_WINDOW_TITLE_STR + ss_path_str);
}

// 全部ボタン（再検索→自動選択or全選択→ビルド→実行）
void C_tnm_compiler_main::update_dialog_for_all_btn(bool is_cancel)
{
	if (is_cancel)	{
		G_dlg->ci.btn_all_sel_all.set_text(_T("キャンセル"));
		G_dlg->ci.btn_all_sel_auto.set_text(_T("キャンセル"));
	}
	else	{
		if (G_dlg->ci.chk_execute.get_check())	{
			G_dlg->ci.btn_all_sel_all.set_text(_T("全ｺﾝﾊﾟｲﾙ\n実行"));
			G_dlg->ci.btn_all_sel_auto.set_text(_T("更新分を\nコンパイル\n実行"));
		}
		else	{
			G_dlg->ci.btn_all_sel_all.set_text(_T("全ｺﾝﾊﾟｲﾙ"));
			G_dlg->ci.btn_all_sel_auto.set_text(_T("更新分を\nコンパイル"));
		}
	}
}

// スタート関連のアイテム
void C_tnm_compiler_main::update_dialog_for_start_item()
{
	bool check_state = G_dlg->ci.chk_start_scene.get_check();
	G_dlg->ci.edt_start_scene.set_enable(check_state);
	G_dlg->ci.edt_start_z_no.set_enable(check_state);
	G_dlg->ci.spn_start_z_no.set_enable(check_state);
}

void C_tnm_compiler_main::update_dialog()
{
	// プロジェクト選択コンボ
	G_dlg->ci.cmb_project.clear_item_list();
	for (int i = 0; i < (int)G_data->project.size(); ++i)
		G_dlg->ci.cmb_project.insert_item_to_bottom(G_data->project[i].project_name + _T("　　（SSﾌｫﾙﾀﾞ ： ") + G_data->project[i].scn_path + _T("）"), false);

	if (G_data->cur_project_no >= 0)
		G_dlg->ci.cmb_project.set_sel_no(G_data->cur_project_no);

	// プロジェクト関連のアイテム状態を設定
	set_project_item_state();

	// ウィンドウタイトルに ss のパスを表示
	update_dialog_for_window_title();

	// スタート関連のアイテム
	update_dialog_for_start_item();

	// 全部ボタン（再検索→自動選択or全選択→ビルド→実行）
	update_dialog_for_all_btn(false);

	// シーンリスト更新
	update_scn_list();		// （ダイアログの更新からの呼び出し）
}

// ****************************************************************
// 操作：プロジェクトを変更
// ================================================================
void C_tnm_compiler_main::change_project(int no)
{
	// プロジェクト関連のアイテム状態を取得
	get_project_item_state();

	// プロジェクト番号を設定
	G_data->cur_project_no = no;

	// ダイアログを更新
	update_dialog();			// （プロジェクトを変更からの呼び出し）
}

// ****************************************************************
// 操作：プロジェクト設定
// ================================================================
bool C_tnm_compiler_main::on_setting_project()
{
	set_project(PROJECT_SETTING_MODE_SETTING);		// プロジェクトを設定
	return true;
}

// ****************************************************************
// 操作：プロジェクト作成（新規プロジェクト）
// ================================================================
bool C_tnm_compiler_main::on_create_project()
{
	set_project(PROJECT_SETTING_MODE_CREATE);	// プロジェクトを設定
	return true;
}

// ****************************************************************
// 操作：プロジェクト削除
// ================================================================
bool C_tnm_compiler_main::on_delete_project()
{
	set_project(PROJECT_SETTING_MODE_DELETE);		// プロジェクトを設定
	return true;
}

// ****************************************************************
// 操作の前処理
// ================================================================
bool C_tnm_compiler_main::on_command_befor_proc(int mod)
{
	// プロセスフラグ
	proc_execut_flag = true;	// プロセス実行中フラグ
	proc_cancel_flag = false;	// プロセスキャンセルフラグ

	// ログのクリア
	G_dlg->ci.lv_log.clear();
	G_data->error_jump_list.clear();

	// 情報をクリア
	G_dlg->ci.edt_detail1.clear_text();
	G_dlg->ci.edt_detail2.clear_text();

	// ダイアログの操作を無効にする
	G_dlg->set_enable_control(false);

	// 全部ボタンをキャンセルボタンにする
	update_dialog_for_all_btn(true);

	return true;
}

// ****************************************************************
// 操作の後処理
// ================================================================
bool C_tnm_compiler_main::on_command_after_proc(int mod)
{
	// 全部ボタンを元に戻す
	update_dialog_for_all_btn(false);

	// ダイアログの操作を有効にする
	G_dlg->set_enable_control(true);

	// プロセスフラグ
	proc_execut_flag = false;	// プロセス実行中フラグ
	proc_cancel_flag = false;	// プロセスキャンセルフラグ

	return true;
}

// ****************************************************************
// 操作：再検索
// ================================================================
bool C_tnm_compiler_main::on_command_find()
{
	on_command_befor_proc(0);
	update_scn_list();		// シーンリスト更新（再検索からの呼び出し）
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// 操作：全選択
// ================================================================
bool C_tnm_compiler_main::on_command_sel_all()
{
	on_command_befor_proc(0);
	proc_sel_all(true);			// 全選択
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// 操作：全解除
// ================================================================
bool C_tnm_compiler_main::on_command_unsel_all()
{
	on_command_befor_proc(0);
	proc_sel_all(false);		// 全解除
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// 操作：自動選択
// ================================================================
bool C_tnm_compiler_main::on_command_sel_auto()
{
	on_command_befor_proc(0);
	proc_sel_auto();		// 自動選択
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// 操作：ビルド
// ================================================================
bool C_tnm_compiler_main::on_command_build()
{
	on_command_befor_proc(0);
	proc_build();			// ビルド
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// 操作：実行
// ================================================================
bool C_tnm_compiler_main::on_command_run()
{
	proc_run();				// 実行
	return true;
}

// ****************************************************************
// 操作：全部（再検索→自動選択or全選択→ビルド→実行）
// ================================================================
bool C_tnm_compiler_main::on_command_all(bool is_sel_auto)
{
	// プロセスのキャンセル
	if (proc_execut_flag)	{	
		proc_cancel_flag = true;
		return false;
	}

	on_command_befor_proc(0);
	proc_all(is_sel_auto);	// 全部（再検索→自動選択or全選択→ビルド→実行）
	on_command_after_proc(0);

	return true;
}

// ****************************************************************
// 操作：マニュアル
// ================================================================
bool C_tnm_compiler_main::on_command_manual()
{
	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	// マニュアルパスの取得
	TSTR manual_path = G_data->project[G_data->cur_project_no].manual_path;
	// マニュアルを開く
	shell_execute_open(manual_path);
	return true;
}

// ****************************************************************
// 処理：自動操作
// ================================================================
bool C_tnm_compiler_main::start_auto_process()
{
	//
	// CUI モードでのみ呼ばれる操作です。
	//

	bool compile_ok_flag = false;

	if (G_data->cl_full_set)	{
		compile_ok_flag = proc_all(false);	// 全部（再検索→全選択→ビルド）
	}
	else	{
		compile_ok_flag = proc_all(true);	// 全部（再検索→自動選択→ビルド）
	}

	// ログを出力する
	if (G_data->cl_log_set && !G_data->cl_log.empty())	{
		if (!::create_dir_from_dir_path(G_data->cl_log))	{
			error_box(G_data->cl_log + _T(" が作成できません。ログが出力できませんでした。\n") + get_last_error_msg());
		}
		else	{

			TSTR log_path = G_data->cl_log + _T("\\log.txt");
			TSTR error_path = G_data->cl_log + _T("\\error.dat");

			// ログファイルを削除する
			if (file_get_type(log_path) == 1)	{
				if (!::filesys_delete_file(log_path))	{
					error_box(log_path + _T(" が削除できません。ログが出力できませんでした。\n") + get_last_error_msg());
					goto output_end;
				}
			}
			if (file_get_type(error_path) == 1)	{
				if (!::filesys_delete_file(error_path))	{
					error_box(error_path + _T(" が削除できません。ログが出力できませんでした。\n") + get_last_error_msg());
					goto output_end;
				}
			}

			// ログを出力する
			if (!C_file::write_full_text_in_MB(log_path, G_data->cui_full_log))	{
				error_box(log_path + _T(" に書き込めません。ログが出力できませんでした。\n") + get_last_error_msg());
				goto output_end;
			}
			if (!compile_ok_flag)	{	// コンパイルに失敗した場合 error.dat を出力
				if (!C_file::write_full_text_in_MB(error_path, _T("")))	{
					error_box(error_path + _T(" に書き込めません。ログが出力できませんでした。\n") + get_last_error_msg());
					goto output_end;
				}
			}
		}
	}

output_end:

	// ダイアログを閉じて終了する
	G_dlg->close();

	return true;
}

// ****************************************************************
// 処理：プロジェクトの設定
// ================================================================
bool C_tnm_compiler_main::set_project(PROJECT_SETTING_MODE proc_mode)
{
	// プロジェクトが選択されていなければ失敗
	if ((proc_mode == PROJECT_SETTING_MODE_SETTING || proc_mode == PROJECT_SETTING_MODE_DELETE) && !check_current_project(true))
		return false;

	// プロジェクト関連のアイテム状態を取得
	get_project_item_state();

	// プロジェクト設定ダイアログを開く
	C_tnm_project_setting_dialog psdlg;
	if (!psdlg.open(G_dlg->get_handle(), proc_mode))
		return false;	// キャンセルされたらここで戻る

	// ダイアログを更新
	update_dialog();					// （プロジェクトの設定からの呼び出し）
	return true;
}

// ****************************************************************
// ファイルリストのソート関数
// ================================================================

struct FN_STRUCT
{
	TSTR	extend_name;
	TSTR	file_name;
	TSTR	sort_str;

	TSTR	file_path()
	{
		return (extend_name.empty() ? _T("") : extend_name + _T("\\")) + file_name;
	}
};

bool fn_list_sort_func(FN_STRUCT& lhs, FN_STRUCT& rhs)
{
	if (lhs.sort_str < rhs.sort_str)			{	return true;	}
	else if (lhs.sort_str > rhs.sort_str)		{	return false;	}
	return false;
}

void make_fn_list(CTSTR& path, CTSTR& extend_name, CTSTR& extension, ARRAY<FN_STRUCT>* fn_list)
{
	fn_list->clear();

	// ファイルリストを作成する
	{
		C_file_find ff;
		ff.find(path, _T("*"), extension, false, true, false);
		int file_cnt = ff.get_file_cnt();
		if (file_cnt <= 0)
			return;

		fn_list->resize(file_cnt);
		ARRAY<FN_STRUCT>::iterator itr = fn_list->begin();
		for (int i = 0; i < file_cnt; i++)	{
			itr->file_name = ff[i].rltv_path;
			itr->sort_str = str_to_lower_ret(itr->file_path());
			itr++;
		}
	}

	// エクステンドのファイルリストを作成する
	if (!extend_name.empty())	{

		C_file_find ff;
		ff.find(path + _T("\\") + extend_name, _T("*"), extension, false, true, false);
		int file_cnt = ff.get_file_cnt();
		if (file_cnt > 0)	{

			int old_size = fn_list->size();
			fn_list->resize(old_size + file_cnt);
			ARRAY<FN_STRUCT>::iterator itr = fn_list->begin() + old_size;
			for (int i = 0; i < file_cnt; i++)	{
				itr->extend_name = extend_name;
				itr->file_name = ff[i].rltv_path;
				itr->sort_str = str_to_lower_ret(itr->file_path());
				itr++;
			}
		}
	}

	// ファイル名をソートする
	std::sort(fn_list->begin(), fn_list->end(), fn_list_sort_func);
}

// ****************************************************************
// 処理：シーンリスト更新
// ================================================================
bool C_tnm_compiler_main::update_scn_list()
{
	G_dlg->ci.lv_ini.clear();
	G_dlg->ci.lv_inc.clear();
	G_dlg->ci.lv_scn.clear();
	G_data->ini_list.clear();
	G_data->inc_list.clear();
	G_data->scn_list.clear();

	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))	{	return false;	}

	// スクリプトフォルダの取得
	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scn_dir_path.empty())
		return false;

	// エクステンド名の取得（空でも問題なし）
	TSTR extend_name = _T("");

	ARRAY<FN_STRUCT> fn_list;
	int row;

	// ini ファイルリストの取得
	make_fn_list(scn_dir_path, extend_name, _T("ini"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// dat ファイルリストの取得
	make_fn_list(scn_dir_path, extend_name, _T("dat"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// txt ファイルリストの取得
	make_fn_list(scn_dir_path, extend_name, _T("txt"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// inc ファイルリストの取得
	make_fn_list(scn_dir_path, extend_name, _T("inc"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_inc.add_item();
		G_dlg->ci.lv_inc.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->inc_list.push_back(fn_list[i].file_path());
	}
	
	// 拡張 inc
	TSTR extend_inc = G_data->project[G_data->cur_project_no].extend_inc;
	std::list<TSTR> extend_inc_list;
	boost::split(extend_inc_list, extend_inc, boost::is_any_of(_T(";")));
	for (std::list<TSTR>::iterator itr = extend_inc_list.begin(); itr != extend_inc_list.end(); ++itr)
	{
		G_data->inc_list.push_back(*itr);
	}

	// SSID 重複チェックデータ
	std::map<int, ARRAY<TSTR>> ssid_cheku_map;

	// ss ファイルリストの取得
	make_fn_list(scn_dir_path, extend_name, _T("ss"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)	{

		// SSID を取得する
		// // #SCENE_SCRIPT_ID = 0001 （スクリプトＩＤ：この行は削除したり、変更したりしないで下さい）
		int ssid = SCN_ID_NONE;	// ID不明
		TSTR file_path = scn_dir_path + _T("\\") + fn_list[i].file_path();
		C_file file;
		if (file.open(file_path, _T("rt")))	{
			char buf[1024] = {0};
			if (fgets(buf, 1024, file.get_fp()))	{
				if (strlen(buf) >= 26)	{
					if (strncmp(buf, "// #SCENE_SCRIPT_ID = ", 22) == 0)	{
						ssid = str_to_int(MBSTR_to_TSTR(buf + 22, 4), SCN_ID_NONE);
					}
				}
			}
		}

		row = G_dlg->ci.lv_scn.add_item();
		G_dlg->ci.lv_scn.set_cell_text(row, 1, ssid == SCN_ID_NONE ? _T("---") : str_format(_T("%04d"), ssid));
		G_dlg->ci.lv_scn.set_cell_text(row, 2, fn_list[i].file_path());
		G_data->scn_list.push_back(S_scn_info(fn_list[i].file_path(), ssid));

		// ssid 重複チェック用マップに入れる
		if (ssid != SCN_ID_NONE)	{
			ssid_cheku_map[ssid].push_back(fn_list[i].file_path());
		}
	}

	// ssid 重複チェック
	TSTR error_str;
	for (std::map<int, ARRAY<TSTR>>::iterator itr = ssid_cheku_map.begin(); itr != ssid_cheku_map.end(); ++itr)	{
		if (itr->second.size() >= 2)	{
			error_str += _T("■SSID = ") + tostr(itr->first) + _T("\n");
			for (int i = 0; i < (int)itr->second.size(); i++)	{
				error_str += itr->second[i] + _T(" ");
			}
			error_str += _T("\n\n");
		}
	}
	if (!error_str.empty())	{
		error_box(_T("SSID の重複が見つかりました！\n\n") + error_str + _T("この ss で台本を作成することはできません。"), MB_OK);
	}

	return true;
}

// ****************************************************************
// 処理：全選択／全解除
// ================================================================
bool C_tnm_compiler_main::proc_sel_all(bool mod)
{
	G_dlg->ci.lv_scn.set_item_select_all(mod);

	return true;
}

// ****************************************************************
// 処理：自動選択
// ================================================================
bool C_tnm_compiler_main::proc_sel_auto()
{
	FILETIME tmp;
	bool all_update_flag = false;

	// inc ファイル解析開始
	set_log(_T("- 更新されたファイルを自動選択します。"));

	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;

	// 選択を全解除
	G_dlg->ci.lv_scn.set_item_select_all(false);

	// 更新された inc ファイルを探す（見つかった場合は ss を全選択する）
	FILETIME inc_update, tmp_update;
	TSTR inc_name, inc_path, tmp_path;
	for (int i = 0; i < (int)G_data->inc_list.size(); i++)	{
		inc_name = erase_extension_ret(G_data->inc_list[i]);
		inc_path = scn_dir_path + _T("\\") + G_data->inc_list[i];
		tmp_path = tmp_dir_path + _T("\\inc\\") + inc_name + _T(".txt");
		if (file_get_type(tmp_path) == 1)	{
			file_get_time(inc_path, &tmp, &tmp, &inc_update);
			file_get_time(tmp_path, &tmp, &tmp, &tmp_update);
			if (inc_update.dwHighDateTime > tmp_update.dwHighDateTime || (inc_update.dwHighDateTime == tmp_update.dwHighDateTime && inc_update.dwLowDateTime > tmp_update.dwLowDateTime))	{
				all_update_flag = true;		// 全選択フラグ
				break;
			}
		}
		else	{
			all_update_flag = true;		// 全選択フラグ
			break;
		}
	}
	if (all_update_flag)	{
		set_log(_T("inc ファイルが更新されたので全選択しました。"));
		G_dlg->ci.lv_scn.set_item_select_all(true);	// 全選択
		return true;
	}

	// 更新された ss のみコンパイルを行う（更新日付を比較して選択する）
	FILETIME scn_update, dat_update;
	TSTR scn_name, scn_path, dat_path;
	for (int i = 0; i < (int)G_data->scn_list.size(); i++)	{
		scn_name = erase_extension_ret(G_data->scn_list[i].scn_name);
		scn_path = scn_dir_path + _T("\\") + scn_name + _T(".ss");
		if (G_dlg->ci.chk_easy_link.get_check())	{
			dat_path = tmp_dir_path + _T("\\bs\\") + scn_name + _T(".dat");
		}
		else	{
			dat_path = tmp_dir_path + _T("\\bs\\") + scn_name + _T(".lzss");
		}
		// dat が存在する場合
		if (file_get_type(dat_path) == 1)	{
			// 更新時間を取得
			file_get_time(scn_path, &tmp, &tmp, &scn_update);
			file_get_time(dat_path, &tmp, &tmp, &dat_update);
			// scn より dat の方が更新時間が古いので選択
			if (scn_update.dwHighDateTime > dat_update.dwHighDateTime || (scn_update.dwHighDateTime == dat_update.dwHighDateTime && scn_update.dwLowDateTime > dat_update.dwLowDateTime))	{
				G_dlg->ci.lv_scn.set_item_select(i, true);
			}
		}
		// dat が存在しない場合
		else	{
			// 必ず選択する
			G_dlg->ci.lv_scn.set_item_select(i, true);
		}
	}
	return true;
}

// ****************************************************************
// 処理：ビルド
// ================================================================
bool C_tnm_compiler_main::proc_build()
{
	bool error = false;
	int error_cnt = 0;

	int start_time = ::GetTickCount();

	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	// フォルダの取得
	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scn_dir_path.empty())	{
		error_box(_T("scn フォルダを指定してください。"));
		return false;
	}
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	if (tmp_dir_path.empty())	{
		error_box(_T("tmp フォルダを指定してください。"));
		return false;
	}
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;
	if (out_dir_path.empty())	{
		error_box(_T("出力フォルダを指定してください。"));
		return false;
	}

	// Gameexe.ini 解析
	if (!proc_gei())	{
		set_log(_T("Gameexe.ini の解析に失敗しました。"));
		return false;
	}

	// inc ファイル解析
	S_tnms_ia_data ia_data;
	if (!proc_inc(&ia_data))	{
		set_log(_T("inc ファイルの解析に失敗しました。"));
		return false;
	}

	// コンパイル
	if (!proc_compile(&ia_data, &error_cnt))	{
		set_log(_T("コンパイルに失敗しました。"));
		error = true;	// エラーフラグを立てるだけ！
	}

	// キャンセル
	if (proc_cancel_flag)	{
		set_log(_T("キャンセルしました。"));
		return false;
	}

	// リンク
	if (!proc_link(&ia_data))	{
		set_log(_T("★★★リンクに失敗しました！★★★"));
		return false;
	}

	if (error)	{
		set_log(_T("★★★") + tostr(error_cnt) + _T("個のファイルにエラーが見つかりました！★★★"));
		return false;
	}

	DWORD past_time = ::GetTickCount() - start_time;

	set_log(_T("全ての処理が正常に成功しました。（経過時間：") + tostr(past_time) + _T("）"));

	// 処理：ビルド後の処理
	if (!proc_build_after())	{
		set_log(_T("ビルド後の処理に失敗しました。"));
		return false;
	}

	return true;
}

// ****************************************************************
// 処理：ビルド後の処理
// ================================================================
bool C_tnm_compiler_main::proc_build_after()
{
	if (!G_data->project[G_data->cur_project_no].build_after.empty())	{
		::shell_execute_open(G_data->project[G_data->cur_project_no].build_after);
		set_log(_T("ビルド後の処理を行いました。"));
	}

	return true;
}

// ****************************************************************
// \ → \\ に置換する
// ================================================================
TSTR escape_yen(CTSTR& str)
{
	TSTR ret_str;

	for (TSTR::const_iterator itr = str.begin(); itr != str.end(); ++itr)	{
		if (*itr == _T('\\'))
			ret_str += _T("\\\\");
		else
			ret_str += *itr;
	}
	
	return ret_str;
}

// ****************************************************************
// 処理：実行
// ================================================================
bool C_tnm_compiler_main::proc_run(CTSTR& start_scene_str, int z_no)
{
	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	// フォルダの取得
	TSTR exe_path = G_data->project[G_data->cur_project_no].exe_path;
	if (exe_path.empty())	{
		error_box(_T("使用する exe を指定してください。"));
		return false;
	}

	// コマンドラインの指定
	TSTR command_line;
	if (G_dlg->ci.chk_debug_on.get_check())	{
		command_line += _T(" /debug_on");
		command_line += _T(" /dp=\"aQpbZF$ijgDSLL19hY#)An36;Ij@ln+G\"");
		command_line += _T(" /dk=\"7hG)o!bNa>tvV{wcc-D>EXpPfq7Bn*sU\"");
	}
	if (G_dlg->ci.chk_debug_off.get_check())	{
		command_line += _T(" /debug_off");
	}

	if (!G_data->project[G_data->cur_project_no].work_path.empty())	{
		command_line += _T(" /work_dir=\"") + escape_yen(G_data->project[G_data->cur_project_no].work_path) + _T("\"");
	}

	if (!start_scene_str.empty())	{
		command_line += _T(" /start=\"") + start_scene_str + _T("\"");
		command_line += _T(" /z_no=") + tostr(z_no);
	}
	else if (G_dlg->ci.chk_start_scene.get_check() && !G_dlg->ci.edt_start_scene.get_text().empty())	{
		command_line += _T(" /start=\"") + G_dlg->ci.edt_start_scene.get_text() + _T("\"");
		command_line += _T(" /z_no=") + G_dlg->ci.edt_start_z_no.get_text();
	}

	if (G_dlg->ci.chk_no_interval.get_check())	{
		command_line += _T(" /no_interval");
	}
	if (G_dlg->ci.chk_overflow_error_off.get_check())	{
		command_line += _T(" /overflow_error_off");
	}

	command_line += _T(" /end_start");

	// exe 実行
	open_process(exe_path + command_line);

	return true;
}

// ****************************************************************
// 処理：全部（再検索→自動選択or全選択→ビルド→実行）
// ================================================================
bool C_tnm_compiler_main::proc_all(bool is_sel_auto)
{
#if 0
	G_name_cntr.clear();
#endif

	// 処理：シーンリスト更新
	if (!update_scn_list())		// （全部処理の再検索からの呼び出し）
		return false;

	// 処理：自動選択
	if (is_sel_auto)	{
		if (!proc_sel_auto())
			return false;
	}
	// 処理：全選択
	else	{
		proc_sel_all(true);
	}

	// 処理：ビルド
	if (!proc_build())
		return false;

	// キャンセル
	if (proc_cancel_flag)
		return false;

#if 0
	// 名前マップの内容を吐き出す
	TSTR scr_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scr_dir_path.empty())
		return false;

	C_file file;
	if (!file.open(scr_dir_path + _T("\\___namae.csv"), _T("wt")))	{
		message_box(_T("open error"), _T(""), MB_OK);
	}
	for (std::map<TSTR, int>::iterator itr = G_name_cntr.begin(); itr != G_name_cntr.end(); ++itr)	{
		TSTR str = itr->first + _T(",") + tostr(itr->second) + _T("\n");
		file.write_in_MB(str);
	}
	file.close();
#endif

	// 処理：実行（GUI のみ）
	if (G_data->gui_flag && G_dlg->ci.chk_execute.get_check())	{
		if (!proc_run())
			return false;
	}

	return true;
}

// ****************************************************************
// ログの設定
// ================================================================
void C_tnm_compiler_main::set_log(CTSTR& text)
{
	set_log_jump(text, _T(""), _T(""), -1);
}

void C_tnm_compiler_main::set_log_jump(CTSTR& text, CTSTR& detail_file_path, CTSTR& jump_file_path, int line_no)
{
	// ジャンプリストに追加
	S_error_jump error_jump;
	error_jump.detail_file_path = detail_file_path;
	error_jump.jump_file_path = jump_file_path;
	error_jump.line_no = line_no;
	G_data->error_jump_list.push_back(error_jump);

	// リストビューにアイテムを追加
	int line = G_dlg->ci.lv_log.add_item();
	G_dlg->ci.lv_log.set_cell_text_visible(line, 1, text);

	// ログを保存
	if (!G_data->gui_flag && G_data->cl_log_set)	{
		G_data->cui_full_log += text + _T("\n");
	}

	// メッセージループに１回まわす
	G_app.default_message_loop();
}

// ****************************************************************
// ini ビューから ini を開く
// ================================================================
void C_tnm_compiler_main::open_ini_by_ini_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->ini_list.size()))
		return;

	if (!check_current_project(true))	{	return;	}		// プロジェクトが選択されていなければ失敗
	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->ini_list[row_no];
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// inc ビューから inc を開く
// ================================================================
void C_tnm_compiler_main::open_inc_by_inc_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->inc_list.size()))
		return;

	if (!check_current_project(true))
		return;		// プロジェクトが選択されていなければ失敗

	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->inc_list[row_no];
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// scn ビューから scn を開く
// ================================================================
void C_tnm_compiler_main::open_scn_by_scn_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->scn_list.size()))
		return;
	if (!check_current_project(true))
		return;		// プロジェクトが選択されていなければ失敗

	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->scn_list[row_no].scn_name;
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// ログビューから scn を開く
// ================================================================
void C_tnm_compiler_main::open_scn_by_log_lv(int row_no)
{
	if (row_no < 0 || (int)G_data->error_jump_list.size() <= row_no)
		return;

	if (G_data->error_jump_list[row_no].jump_file_path.empty())
		return;

	if (!check_current_project(true))
		return;		// プロジェクトが選択されていなければ失敗

	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(G_data->error_jump_list[row_no].jump_file_path, G_data->error_jump_list[row_no].line_no, editor_path);
}

// ****************************************************************
// フォルダを開く
// ================================================================

// フォルダオープン
bool C_tnm_compiler_main::open_folder(CTSTR& path)
{
	if (file_get_type(path) != 0)	{
		error_box(_T("フォルダが見つかりません。\n\n") + path, MB_OK);
		return false;
	}
	shell_execute_open(path);
	return true;
}

// scn パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_scn_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].scn_path;
	return open_folder(path);
}

// tmp パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_tmp_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].tmp_path;
	return open_folder(path);
}

// out パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_out_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].out_path;
	return open_folder(path);
}

// exe パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_exe_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].exe_path);
	return open_folder(path);
}

// work パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_work_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].work_path;
	return open_folder(path);
}

// editor パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_editor_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].editor_path);
	return open_folder(path);
}

// manual パスのフォルダを開く
bool C_tnm_compiler_main::open_folder_from_manual_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].manual_path);
	return open_folder(path);
}

// ****************************************************************
// ポップアップメニュー処理
// ================================================================

const int	COMPILER_POPUP_MENU_SEL_START_SCENE = 10;

const int	COMPILER_POPUP_MENU_RUN_START_SCENE = 20;
const int	COMPILER_POPUP_MENU_RUN_START_Z_NO = 21;

const int	COMPILER_POPUP_MENU_SET_START_SCENE = 30;
const int	COMPILER_POPUP_MENU_SET_START_Z_NO = 31;

const int	COMPILER_POPUP_MENU_ALL_SELECT = 40;
const int	COMPILER_POPUP_MENU_ALL_UNSELECT = 41;

const int	COMPILER_POPUP_MENU_OPEN_FOLDER_SCN = 50;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_TMP = 51;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_OUT = 52;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_EXE = 53;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_WORK = 54;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL = 55;

const int	COMPILER_POPUP_MENU_DELETE_SAVEDATA = 60;


const int	COMPILER_POPUP_MENU_RUN_Z_NO_TOP = 1000;
const int	COMPILER_POPUP_MENU_SET_Z_NO_TOP = 2000;


void C_tnm_compiler_main::popup_menu_proc(CTSTR& start_scene_str)
{
	C_popup_menu menu;
	menu.create();

	bool project_selected = check_current_project(false);

	// スタートシーン関連の準備
	TSTR sel_start_scene_column_str;
	bool scene_enable;
	if (start_scene_str.empty())	{
		sel_start_scene_column_str = _T("No Select");
		scene_enable = false;
	}
	else	{
		sel_start_scene_column_str = start_scene_str;
		scene_enable = true;
	}

	// Ｚ番号リスト作成
	ARRAY<int> z_list;
	if (!start_scene_str.empty() && project_selected)	{
		TSTR path = G_data->project[G_data->cur_project_no].tmp_path + _T("\\bs\\") + start_scene_str + _T(".dat");
		if (file_get_type(path) == 1)	{
			BUFFER buf;
			if (C_file::read_full_data(path, buf))	{
				S_tnm_scn_header *header = (S_tnm_scn_header *)buf.get();
				if (header->z_label_cnt > 0)	{
					int *label = (int *)(buf.get() + header->z_label_list_ofs);
					for (int i = 0; i < header->z_label_cnt; i++)	{
						if (label[i] > 0)	{	// 0 は未定義のラベルです。
							z_list.push_back(i);
						}
					}
					// #Z00 しかないならリストをクリアする
					if ((int)z_list.size() == 1 && z_list[0] == 0)	{
						z_list.clear();
					}
				}
				buf.clear();
			}
		}
	}

	// 「セーブデータを削除する」の準備
	TSTR savedata_path;
	bool savedata_delete = false;
	if (project_selected)	{
		if (file_get_type(G_data->project[G_data->cur_project_no].work_path) == 0)	{
			savedata_path = G_data->project[G_data->cur_project_no].work_path + _T("\\savedata");
			if (file_get_type(savedata_path) == 0)	{
				savedata_delete = true;
			}
		}
		else if (file_get_type(G_data->project[G_data->cur_project_no].exe_path) == 1)	{
			savedata_path = get_dir_path(G_data->project[G_data->cur_project_no].exe_path) + _T("\\savedata");
			if (file_get_type(savedata_path) == 0)	{
				savedata_delete = true;
			}
		}
	}

	// 「Ｚ番号」も指定して実行する
	C_popup_menu menu_run_z_no;
	menu_run_z_no.create();
	for (int i = 0; i < (int)z_list.size(); i++)	{
		menu_run_z_no.add_item(COMPILER_POPUP_MENU_RUN_Z_NO_TOP + i, _T("#Z") + str_format(_T("%02d"), z_list[i]));
	}

	// スタートシーンに「Ｚ番号」も設定する
	C_popup_menu menu_set_z_no;
	menu_set_z_no.create();
	for (int i = 0; i < (int)z_list.size(); i++)	{
		menu_set_z_no.add_item(COMPILER_POPUP_MENU_SET_Z_NO_TOP + i, _T("#Z") + str_format(_T("%02d"), z_list[i]));
	}

//	menu.add_item(COMPILER_POPUP_MENU_SEL_START_SCENE, sel_start_scene_column_str, false);
//	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_RUN_START_SCENE, _T("実行する (#Z00)"), scene_enable);
	menu.add_sub_menu(COMPILER_POPUP_MENU_RUN_START_Z_NO, _T("　└ Ｚ番号も指定 →"), menu_run_z_no.get_handle(), z_list.empty() ? false : scene_enable);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_SET_START_SCENE, _T("[ ｽﾀｰﾄｼｰﾝ ] に設定する (#Z00)"), scene_enable);
	menu.add_sub_menu(COMPILER_POPUP_MENU_SET_START_Z_NO, _T("　└ Ｚ番号も指定 →"), menu_set_z_no.get_handle(), z_list.empty() ? false : scene_enable);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_ALL_SELECT, _T("ＳＳの全選択"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_ALL_UNSELECT, _T("ＳＳの全解除"), project_selected);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_SCN, _T("ＳＳフォルダを開く"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_TMP, _T("ＴＭＰフォルダを開く"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_OUT, _T("出力フォルダを開く"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_EXE, _T("ＥＸＥフォルダを開く"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_WORK, _T("作業フォルダを開く"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL, _T("マニュアルフォルダを開く"), project_selected);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_DELETE_SAVEDATA, _T("セーブデータを削除する"), savedata_delete);

	C_point mp = get_mouse_pos();

	int res = menu.select(G_dlg->get_handle(), mp.x, mp.y);

	switch (res)	{
		case COMPILER_POPUP_MENU_SEL_START_SCENE:		// 『選択されているシーン』
			break;
		case COMPILER_POPUP_MENU_RUN_START_SCENE:		// 実行する
			proc_run(start_scene_str, 0);
			break;
		case COMPILER_POPUP_MENU_SET_START_SCENE:		// スタートシーンに設定する
			G_dlg->ci.chk_start_scene.set_check(true);
			G_dlg->ci.edt_start_scene.set_text(start_scene_str);
			G_dlg->ci.edt_start_z_no.set_text(_T("0"));
			G_main->update_dialog_for_start_item();
			break;
		case COMPILER_POPUP_MENU_ALL_SELECT:			// 全選択
			G_main->on_command_sel_all();
			break;
		case COMPILER_POPUP_MENU_ALL_UNSELECT:			// 全解除
			G_main->on_command_unsel_all();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_SCN:		// ＳＳフォルダを開く
			G_main->open_folder_from_scn_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_TMP:		// ＴＭＰフォルダを開く
			G_main->open_folder_from_tmp_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_OUT:		// 出力フォルダを開く
			G_main->open_folder_from_out_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_EXE:		// ＥＸＥフォルダを開く
			G_main->open_folder_from_exe_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_WORK:		// 作業フォルダを開く
			G_main->open_folder_from_work_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL:	// マニュアルフォルダを開く
			G_main->open_folder_from_manual_path();
			break;
		case COMPILER_POPUP_MENU_DELETE_SAVEDATA:
			if (info_box(_T("セーブデータを削除してもよろしいですか？\n\n") + savedata_path, MB_YESNO) == IDYES)	{
				filesys_remove_directory_with_contain_files(savedata_path, true, true, true);
			}
			break;
		default:
			// Ｚ番号も指定してスタートシーンに設定する
			if (res >= COMPILER_POPUP_MENU_SET_Z_NO_TOP)	{
				int list_no = res - COMPILER_POPUP_MENU_SET_Z_NO_TOP;
				int z_no = 0;
				if (0 <= list_no && list_no < (int)z_list.size())	{
					z_no = z_list[list_no];
				}
				G_dlg->ci.chk_start_scene.set_check(true);
				G_dlg->ci.edt_start_scene.set_text(start_scene_str);
				G_dlg->ci.edt_start_z_no.set_text(tostr(z_no));
				G_main->update_dialog_for_start_item();
			}
			// Ｚ番号も指定して実行する
			else if (res >= COMPILER_POPUP_MENU_RUN_Z_NO_TOP)	{
				int list_no = res - COMPILER_POPUP_MENU_RUN_Z_NO_TOP;
				int z_no = 0;
				if (0 <= list_no && list_no < (int)z_list.size())	{
					z_no = z_list[list_no];
				}
				proc_run(start_scene_str, z_no);
			}
			break;
	}

	menu_set_z_no.destroy();
	menu_run_z_no.destroy();
	menu.destroy();
}

// ****************************************************************
// エラーの出た行の詳細をステータスバーに表示
// ================================================================
void C_tnm_compiler_main::show_err_detail(int row_no)
{
	if (row_no < 0 || (int)G_data->error_jump_list.size() <= row_no)
		return;

	bool utf8 = G_data->project[G_data->cur_project_no].utf8;

	G_dlg->ci.edt_detail1.clear_text();
	G_dlg->ci.edt_detail2.clear_text();

	if (G_data->error_jump_list[row_no].detail_file_path.empty())
		return;

	ARRAY<TSTR> ca_text;
	utf8 ? C_file::read_full_text_UTF8(G_data->error_jump_list[row_no].detail_file_path, ca_text) : C_file::read_full_text(G_data->error_jump_list[row_no].detail_file_path, ca_text);
	int line_no = G_data->error_jump_list[row_no].line_no;
	if (line_no - 1 < 0 || (int)ca_text.size() <= line_no - 1)
		return;

	TSTR text = ca_text[line_no - 1];

	// タブをスペースに変換
	str_replace(text, _T("\t"), _T(" "));

	// 連続スペースを置換
	str_replace(text, _T("  "), _T(" "));

	// ステータスバーに設定
	G_dlg->set_detail1_text(text);
	G_dlg->set_detail2_text(G_dlg->ci.lv_log.get_cell_text(row_no, 1));
}

// ****************************************************************
// プロジェクトが選択されているかどうか判定する
// ================================================================
bool C_tnm_compiler_main::check_current_project(bool is_warning)
{
	if (G_data->cur_project_no == -1)
		return false;

	if (0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size())
		return true;

	if (is_warning)	{	error_box(_T("プロジェクトを選択してください。"));	}
	return false;
}

// ****************************************************************
// 右下についての説明
// ================================================================
bool C_tnm_compiler_main::explain_under()
{
	MessageBox(G_dlg->get_handle(),
		_T("■垂直同期を待たない\n")
		_T("内部テスト用です。\n")
		_T("\n")
		_T("■圧縮しない\n")
		_T("Scene.pck を圧縮しません。コンパイルが速くなります。\n")
		_T("圧縮しない場合、ゲームウィンドウのキャプションに [★無圧縮★] と表示されます。（デバッグ時のみです。）\n")
		_T("製品版では必ずチェックをはずして下さい。\n")
		_T("\n")
		_T("これらのチェックは、コンパイラを起動する度にオフに戻ります。")
		, _T("右下のボタンについての説明"), MB_OK);

	return true;
}

// ****************************************************************
// 暗号.dat の読み込み
// ================================================================
bool C_tnm_compiler_main::load_exe_angou_dat(TSTR& angou_str)
{
	bool utf8 = G_data->project[G_data->cur_project_no].utf8;

	angou_str.clear();
	TSTR exe_angou_dat_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\暗号.dat");
	TSTR exe_angou_dat;
	if (!(utf8 ? C_file::read_full_text_UTF8(exe_angou_dat_path, exe_angou_dat) : C_file::read_full_text(exe_angou_dat_path, exe_angou_dat)))
	{
		return false;
	}

	TSTR::iterator itr = exe_angou_dat.begin();
	TSTR::iterator end = exe_angou_dat.end();
	while (1)	{
		if (itr == end)	{	break;	}
		if (*itr == _T('\n'))	{	break;	}
		angou_str += *itr++;
	}
	return true;
}

// ****************************************************************
// 暗号についての説明
// ================================================================
void C_tnm_compiler_main::manual_exe_angou()
{
	MessageBox(G_dlg->get_handle(),
		_T("■説明\n\n")
		_T("　イベントデータ ( Scene.pck ) と実行ＥＸＥ ( SiglusEngine.exe ) を暗号化します。\n")
		_T("　これにより、暗号が一致しなければ動かなくなります。\n\n")
		_T("■目的\n\n")
		_T("　コピープロテクトを施した製品に、プロテクトのかかっていない SiglusEngine.exe をコピーすると、普通に動作してしまいます。\n")
		_T("　また、製品版の修正パッチを出したとき、「体験版に」「製品版の修正パッチを当てる」ことで、無料で全てのシナリオが読めてしまうことがあります。\n")
		_T("　（組み方にもよります。通常は、ファイルエラーなどが出まくるのでゲームにならないと思います。）\n")
		_T("　これらを防ぐためです。\n")
		_T("　(2011/9/14) プロテクトをかけないタイトルの場合も、暗号化が必須になりました。\n\n")
		_T("　必ず各タイトルごとに暗号の文字列は変えて下さい。\n\n")
		_T("　無料配布する体験版などは暗号化する必要はありません。\n")
		_T("　（しても問題はないですが、製品版とは異なる暗号にして下さい。）\n\n")
		_T("■手順\n\n")
		_T("　１．ss ファイルと同じ場所に暗号ファイル（暗号.dat）を作成して下さい。\n\n")
		_T("　　　テキストファイルを新規作成し名前を 「 暗号.dat 」 に変更して下さい。\n\n")
		_T("　　　作成したファイルの１行目に暗号の文字列を記述して下さい。\n")
		_T("　　　全角なら４文字以上、半角なら８文字以上、全角半角は混在しても問題ありません。\n\n")
		_T("　　　ただし、必ず各タイトルごとに暗号の文字列は変えて下さい。\n\n")
		_T("　　　例： サンプルProject\n\n")
		_T("　２．イベントデータを暗号化する\n\n")
		_T("　　　コンパイル時に暗号ファイルの存在の有無で自動的に暗号化します。\n\n")
		_T("　　　暗号化しない場合は暗号ファイル（暗号.dat）を削除もしくは名前を変更して下さい。\n\n")
		_T("　３．ＥＸＥを暗号化する\n\n")
		_T("　　　メニュー項目の [ ＥＸＥを暗号化する ] を選択して下さい。\n\n")
		_T("　　　★１度暗号化されたＥＸＥを再暗号化する事はできません。\n\n")
		_T("　　　暗号を元に戻したい場合 や 違う暗号にしたい場合は、暗号化されていない元のＥＸＥを持ってきてコピーして下さい。\n\n")
		_T("■補足\n\n")
		_T("　暗号化されていないイベントデータは、暗号化されたＥＸＥでも動きます。\n")
		_T("　暗号化されたイベントデータを、暗号化されていないＥＸＥで動かすと予期せぬ動作でＥＸＥが強制終了します。\n")
		, _T("暗号化についての説明"), MB_OK);
}

// ****************************************************************
// ＥＸＥを暗号化する
// ================================================================
void C_tnm_compiler_main::func_exe_angou()
{
	TSTR exe_angou_str;
	if (!G_main->load_exe_angou_dat(exe_angou_str))	{
		MessageBox(G_dlg->get_handle(),
			_T("暗号の文字列がありません。\n\n")
			_T("ss ファイルと同じ場所に暗号ファイル（暗号.dat）が見つかりません。\n")
			, _T("エラー"), MB_OK | MB_ICONWARNING);
	}
	else if(exe_angou_str.empty())	{
		MessageBox(G_dlg->get_handle(),
			_T("暗号の文字列がありません。\n\n")
			_T("暗号ファイル（暗号.dat）の１行目に暗号の文字列が記述されていません。\n")
			, _T("エラー"), MB_OK | MB_ICONWARNING);
	}
	else	{
		TSTR exe_path = G_data->project[G_data->cur_project_no].exe_path;
		MBSTR mb_exe_angou_str = TSTR_to_MBSTR(exe_angou_str);
		if((int)mb_exe_angou_str.size() < 8)	{
			TSTR mess = 
				_T("暗号の文字列が短過ぎます。\n\n")
				_T("≪暗号の文字列≫\n\n")
				_T("　　") + exe_angou_str + _T("\n\n")
				_T("全角なら４文字以上、半角なら８文字以上、全角半角は混在しても問題ありません。\n\n")
				_T("例： 株式会社ﾋﾞｼﾞｭｱﾙArt's\n\n");
			MessageBox(G_dlg->get_handle(), mess.c_str(), _T("エラー"), MB_OK | MB_ICONWARNING);
		}
		else if(exe_path.empty())	{
			MessageBox(G_dlg->get_handle(), _T("プロジェクトの設定で「実行ファイル」 にＥＸＥファイルを指定して下さい。\n"), _T("エラー"), MB_OK | MB_ICONWARNING);
		}
		else if(file_get_type(exe_path) != 1)	{
			TSTR mess = 
				_T("ＥＸＥファイルが見つかりません。\n\n")
				_T("「実行ファイル」 に指定されているＥＸＥのパスを確認して下さい。\n\n")
				_T("　") + exe_path + _T("　　\n");
			MessageBox(G_dlg->get_handle(), mess.c_str(), _T("エラー"), MB_OK | MB_ICONWARNING);
		}
		else	{
			TSTR mess =
				_T("「実行ファイル」 に指定されているＥＸＥファイルを暗号化します。\n\n")
				_T("≪ＥＸＥのパス≫\n\n")
				_T("　　") + exe_path + _T("　　\n\n")
				_T("≪暗号の文字列≫\n\n")
				_T("　　") + exe_angou_str + _T("　　\n\n")
				_T("★１度暗号化されたＥＸＥファイルを再暗号化する事はできません。\n\n")
				_T("暗号を元に戻したい場合 や 違う暗号にしたい場合は、\n")
				_T("暗号化されていない元のＥＸＥファイルを持ってきてコピーして下さい。\n");
			int res = MessageBox(G_dlg->get_handle(), mess.c_str(), _T("ＥＸＥの暗号化"), MB_OKCANCEL | MB_ICONINFORMATION);
			if (res == IDOK)	{
				 C_tnms_exe_angou::angou(G_dlg->get_handle(), exe_path, exe_angou_str);
			}
		}
	}
}

