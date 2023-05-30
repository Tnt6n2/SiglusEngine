#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_input.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_timer.h"
#include	"element/elm_call.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_init.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_dialog.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"
#include	"localize/localize.h"

// ****************************************************************
// 現在のプロセスを取得
// ================================================================
C_tnm_proc* tnm_get_cur_proc()
{
	return &Gp_local->cur_proc;
}

// ****************************************************************
// 現在のコールを取得（インラインにするべき？）
// ================================================================
C_elm_call* tnm_get_cur_call()
{
	return Gp_call_list->get_sub(Gp_call_list->get_call_cnt() - 1, true);
}

// ****************************************************************
// 現在のコール数を取得（インラインにするべき？）
// ================================================================
int tnm_get_cur_call_cnt()
{
	return Gp_call_list->get_call_cnt();
}

// ****************************************************************
// シーン処理：Scene.pck を読み直す
// ================================================================
bool tnm_reload_scene_pck()
{
	// ディレクトリが変わった！（分割システム）
	if (Gp_local_save->append_dir != Gp_dir->append_dir)
	{
		// ディレクトリを設定
		Gp_dir->append_dir = Gp_local_save->append_dir;
		Gp_dir->append_name = Gp_local_save->append_name;
		// シーンを初期化する
		Gp_lexer->init();
		// シーンを読み込む
		if (!Gp_lexer->load_scn(LCL_SCENE_PCK))
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("シーンデータ（") LCL_SCENE_PCK _T("）が開けませんでした。"));
		}
	}

	return true;
}

// ****************************************************************
// シーン処理：ジャンプ
// ================================================================
bool tnm_scene_proc_jump(int scn_no, int z_no)
{
	// シーンにジャンプ
	Gp_lexer->jump_to_scn(scn_no, z_no);

	return true;
}

bool tnm_scene_proc_jump(CTSTR& scn_name, int z_no)
{
	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
	}

	// シーンにジャンプ
	if (!tnm_scene_proc_jump(scn_no, z_no))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：ラベルコール
// ================================================================
bool tnm_scene_proc_gosub(int label_no, int ret_form_code)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// コール元の情報を設定
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();						// コール情報をセーブ
	Gp_lexer->jump_to_label(label_no);		// コール先にジャンプ

	// コール先の情報を設定
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_gosub;

	// デバッグ情報
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = src_scn_no;
	p_dst_call->dst_label_no = label_no;

	return true;
}

// ****************************************************************
// シーン処理：シーンコール
// ================================================================
bool tnm_scene_proc_farcall(int scn_no, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// コール元の情報を設定
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();						// コール情報をセーブ
	Gp_lexer->jump_to_scn(scn_no, z_no);	// コール先にジャンプ

	// コール先の情報を設定
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_farcall;
	p_dst_call->excall_flag = ex_call_flag;
	p_dst_call->frame_action_flag = frame_action_flag;
	if (ex_call_flag)	{

		// ＥＸコール中フラグを立てる
		Gp_global->ex_call_flag = true;

		// スクリプトに進む
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}
	else if (frame_action_flag)	{

		// スクリプトに進む
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}

	// デバッグ情報
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = scn_no;
	p_dst_call->dst_label_no = z_no;

	return true;
}

bool tnm_scene_proc_farcall(CTSTR& scn_name, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
	}

	// コール
	if (!tnm_scene_proc_farcall(scn_no, z_no, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：ユーザコマンドコール
// ================================================================
bool tnm_scene_proc_call_user_cmd(int scn_no, int user_cmd_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// コール元の情報を設定
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();									// コール情報をセーブ
	Gp_lexer->jump_to_user_cmd(scn_no, user_cmd_no);	// コール先にジャンプ

	// コール先の情報を設定
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_user_cmd;
	p_dst_call->excall_flag = ex_call_flag;
	p_dst_call->frame_action_flag = frame_action_flag;
	if (ex_call_flag)	{

		// ＥＸコール中フラグを立てる
		Gp_global->ex_call_flag = true;

		// スクリプトに進む
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}
	else if (frame_action_flag)	{

		// スクリプトに進む
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}

	// デバッグ情報
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = scn_no;
	p_dst_call->dst_label_no = user_cmd_no;
			
	return true;
}

bool tnm_scene_proc_call_user_cmd(int scn_no, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// ユーザコマンド番号を取得
	int user_cmd_no = Gp_lexer->get_user_cmd_no(scn_no, user_cmd_name);
	if (user_cmd_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("ユーザコマンド ") + user_cmd_name + _T(" が見つかりませんでした。"));
	}

	// コール
	if (!tnm_scene_proc_call_user_cmd(scn_no, user_cmd_no, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

bool tnm_scene_proc_call_user_cmd(CTSTR& scn_name, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("シーン ") + scn_name + _T(" が見つかりませんでした。"));
	}

	if (!tnm_scene_proc_call_user_cmd(scn_no, user_cmd_name, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：リターン
// ================================================================
bool tnm_scene_proc_return()
{
	// コールの種類による処理
	C_elm_call* p_cur_call = tnm_get_cur_call();
	if (p_cur_call->excall_flag == 1)	{

		// ＥＸコールフラグを降ろす
		Gp_global->ex_call_flag = false;

		// プロセスを元に戻す
		tnm_pop_proc();

		// ★キークリア
		Gp_input->clear();
	}
	else if (p_cur_call->frame_action_flag == 1)	{

		// プロセスを元に戻す
		tnm_pop_proc();
	}

	// リターン
	if (!tnm_load_call())	// コール情報をロード
		return false;

	return true;
}

// ****************************************************************
// シーン処理：コールスタックを初期化
// ================================================================
void tnm_scene_init_call_stack()
{
	Gp_call_list->set_call_cnt(1);	// 初期値は１
}

// ****************************************************************
// シーン処理：コールスタックを削除
// ================================================================
void tnm_scene_del_call_stack(int del_cnt)
{
	if (del_cnt <= 0)
		return;

	int cur_cnt = Gp_call_list->get_call_cnt();
	int dst_cnt = std::max(cur_cnt - del_cnt, 1);	// １以下には減らせない
	Gp_call_list->set_call_cnt(dst_cnt);
}

// ****************************************************************
// シーン処理：コールスタックの個数を設定
// ================================================================
void tnm_scene_set_call_stack_cnt(int dst_cnt)
{
	if (dst_cnt >= 1)	{	// １以下には減らせない

		// 減らす方向のみに設定できる
		int cur_cnt = Gp_call_list->get_call_cnt();
		if (dst_cnt < cur_cnt)	{
			Gp_call_list->set_call_cnt(dst_cnt);
		}
	}
}

// ****************************************************************
// シーン処理：コールスタックの個数を取得
// ================================================================
int tnm_scene_get_call_stack_cnt()
{
	return Gp_call_list->get_call_cnt();
}

// ****************************************************************
// シーン処理：リスタート
// ================================================================
bool tnm_scene_proc_restart_func(int scn_no, int z_no)
{
	// シーンにジャンプ
	Gp_lexer->jump_to_scn(scn_no, z_no);

	// スクリプト開始
	tnm_push_proc(TNM_PROC_TYPE_SCRIPT);

	return true;
}

// ****************************************************************
// シーン処理：スタートシーンからスタート
// ================================================================
bool tnm_scene_proc_start_from_start_scene()
{
	// ローカル初期化
	tnm_init_local();

	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(Gp_global->start_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_global->start_scene_name + _T(".ss が見つかりませんでした。"));
	}

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("スタートシーンから開始します。"));

	// シーンから始める
	tnm_scene_proc_restart_func(scn_no, Gp_global->start_z_no);

	// ★割り込み警告処理！
	C_tnm_proc warning_proc;
	warning_proc.type = TNM_PROC_TYPE_START_WARNING;
	warning_proc.option = 0;
	tnm_push_proc(warning_proc);

	return true;
}

// ****************************************************************
// シーン処理：スタートシーンからリスタート
// ================================================================
bool tnm_scene_proc_restart_from_start_scene()
{
	// ローカル終了処理
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(true);

	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(Gp_global->start_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_global->start_scene_name + _T(".ss が見つかりませんでした。"));
	}

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("スタートシーンから再開します。"));

	// シーンから始める
	tnm_scene_proc_restart_func(scn_no, Gp_global->start_z_no);

	// ★割り込み警告処理！
	C_tnm_proc warning_proc;
	warning_proc.type = TNM_PROC_TYPE_START_WARNING;
	warning_proc.option = 0;
	tnm_push_proc(warning_proc);

	return true;
}

// ****************************************************************
// シーン処理：メニューシーンからリスタート
// ================================================================
bool tnm_scene_proc_restart_from_menu_scene()
{
	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(true);

	// アペンドを初期化する
	if (Gp_cmd_line->select_scene_pck_set)
	{
		Gp_local_save->append_dir = Gp_cmd_line->select_scene_pck;
		Gp_local_save->append_name = _T("[dummy]");

		for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			if (Gp_select_ini->m_append_list[i].m_dir == Gp_local_save->append_dir)
			{
				Gp_local_save->append_name = Gp_select_ini->m_append_list[i].m_name;
			}
		}
	}
	else
	{
		Gp_local_save->append_dir = Gp_select_ini->first_dir();	// 先頭のディレクトリを探す
		Gp_local_save->append_name = Gp_select_ini->first_name();
	}

	// Scene.pck を読み直す
	if (!tnm_reload_scene_pck())
		return false;

	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(Gp_ini->menu_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_ini->menu_scene_name + _T(".ss が見つかりませんでした。"));
	}

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("メニューシーンからリスタートします。"));

	// シーンから始める
	return tnm_scene_proc_restart_func(scn_no, Gp_ini->menu_scene_z_no);
}

// ****************************************************************
// シーン処理：指定のシーンからリスタート（シーン番号を指定）
// ================================================================
bool tnm_scene_proc_restart(int scn_no, int z_no)
{
	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(true);

	// シーンから始める
	return tnm_scene_proc_restart_func(scn_no, z_no);
}

// ****************************************************************
// シーン処理：指定のシーンからリスタート（シーン名を指定）
// ================================================================
bool tnm_scene_proc_restart(CTSTR& scn_name, int z_no)
{
	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(true);

	// シーン番号を取得
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// スクリプト処理を止める
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss が見つかりませんでした。"));
	}

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("シーン ") + scn_name + _T(" からリスタートします。"));

	// シーンから始める
	return tnm_scene_proc_restart_func(scn_no, z_no);
}

// ****************************************************************
// シーン処理：システムコマンドシーンを呼ぶ
// ================================================================
bool tnm_scene_proc_call_syscom_scene()
{
	// コール
	if (!tnm_scene_proc_farcall(Gp_ini->cancel_scene_name, Gp_ini->cancel_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：フリックシーンを呼ぶ
// ================================================================
bool tnm_scene_proc_call_flick_scene(int no)
{
	// コール
	if (!tnm_scene_proc_farcall(Gp_ini->flick_scene_name[no], Gp_ini->flick_scene_z_no[no], FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：セーブシーンを呼ぶ
// ================================================================
bool tnm_scene_proc_call_save_scene()
{
	// コール
	if (!tnm_scene_proc_farcall(Gp_ini->save_scene_name, Gp_ini->save_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：ロードシーンを呼ぶ
// ================================================================
bool tnm_scene_proc_call_load_scene()
{
	// コール
	if (!tnm_scene_proc_farcall(Gp_ini->load_scene_name, Gp_ini->load_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// シーン処理：環境設定シーンを呼ぶ
// ================================================================
bool tnm_scene_proc_call_config_scene()
{
	// コール
	if (!tnm_scene_proc_farcall(Gp_ini->config_scene_name, Gp_ini->config_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// セーブロード処理：セーブ
// ================================================================
bool tnm_saveload_proc_save(int save_no)
{
	// セーブ可能かどうかチェック
	if (!tnm_is_save_enable(save_no))
		return false;

	// ローカルセーブ
	tnm_save_local_on_file(save_no);

	// グローバルも同時にセーブする（ローカルセーブと同期を取るため。必須！）
	tnm_save_global_on_file();

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(save_no) + _T(" 番にセーブしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：クイックセーブ
// ================================================================
bool tnm_saveload_proc_quick_save(int quick_save_no)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// セーブ可能かどうかチェック
	if (!tnm_is_save_enable(save_no))
		return false;

	// ローカルセーブ
	tnm_save_local_on_file(save_no);

	// グローバルも同時にセーブする（ローカルセーブと同期を取るため。必須！）
	tnm_save_global_on_file();

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(quick_save_no) + _T(" 番にクイックセーブしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：エンドセーブ
// ================================================================
bool tnm_saveload_proc_end_save()
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// セーブ可能かどうかチェック
	if (!tnm_is_save_enable(save_no))
		return false;

	// ローカルセーブ
	tnm_save_local_on_file(save_no);

	// グローバルも同時にセーブする（ローカルセーブと同期を取るため。必須！）
	tnm_save_global_on_file();

	// 情報を表示
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("エンドセーブしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：ロード
// ================================================================
bool tnm_saveload_proc_load(int save_no)
{
	// ロード可能かどうかチェック
	if (!tnm_is_load_enable(save_no))
		return false;

	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ファイルからロードする
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck を読み直す
	if (!tnm_reload_scene_pck())
		return false;

	// ファイルからローカルデータ読み込む
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// 情報
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(save_no) + _T(" 番をロードしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：クイックロード
// ================================================================
bool tnm_saveload_proc_quick_load(int quick_save_no)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// ロード可能かどうかチェック
	if (!tnm_is_load_enable(save_no))
		return false;

	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ファイルからロードする
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck を読み直す
	if (!tnm_reload_scene_pck())
		return false;

	// ファイルからローカルデータ読み込む
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// 情報
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(quick_save_no) + _T(" 番をクイックロードしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：エンドロード
// ================================================================
bool tnm_saveload_proc_end_load()
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// ロード可能かどうかチェック
	if (!tnm_is_load_enable(save_no))
		return false;

	// ローカル終了
	tnm_finish_local();

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ファイルからロードする
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck を読み直す
	if (!tnm_reload_scene_pck())
		return false;

	// ファイルからローカルデータ読み込む
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// 情報
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("エンドロードしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：前の選択肢に戻る
// ================================================================
bool tnm_saveload_proc_return_to_sel()
{
	if (!tnm_is_exist_sel_save())
		return false;

	S_tnm_local_save temp_save;

	// ローカル終了
	tnm_finish_local();

	// 選択肢セーブをテンポラリに保存
	tnm_get_sel_save(&temp_save);

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ローカルセーブデータにテンポラリをコピー
	tnm_set_local_save(&temp_save);

	// ローカルセーブデータからデータをロードする
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// エラーを設定
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("選択肢に戻りました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：インナーセーブを作成する
// ================================================================
bool tnm_saveload_proc_create_inner_save(int inner_save_no)
{
	// ローカルセーブをインナーセーブにコピーする
	tnm_savedata_system_copy_local_save_to_inner_save(inner_save_no);

	// エラーを設定
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("%d 番にインナーセーブ（%d）。"), inner_save_no, tnm_savedata_system_get_inner_save_size(inner_save_no)));

	return true;
}

// ****************************************************************
// セーブロード処理：インナーセーブをロードする
// ================================================================
bool tnm_saveload_proc_load_inner_save(int inner_save_no)
{
	if (!tnm_savedata_system_check_inner_save(inner_save_no))
		return false;

	S_tnm_local_save temp_save;

	// ローカル終了
	tnm_finish_local();

	// インナーセーブをテンポラリに取得
	tnm_savedata_system_get_inner_save(inner_save_no, &temp_save);

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ローカルセーブデータにテンポラリをコピー
	tnm_set_local_save(&temp_save);

	// ローカルセーブデータからデータをロードする
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// エラーを設定
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("インナーセーブをロードしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：インナーセーブをクリアする
// ================================================================
bool tnm_saveload_proc_clear_inner_save(int inner_save_no)
{
	// インナーセーブをクリアする
	tnm_savedata_system_clear_inner_save(inner_save_no);

	return true;
}

// ****************************************************************
// セーブロード処理：インナーセーブをコピーする
// ================================================================
bool tnm_saveload_proc_copy_inner_save(int from, int to)
{
	// インナーセーブをコピーする
	tnm_savedata_system_copy_inner_save(from, to);

	return true;
}

// ****************************************************************
// セーブロード処理：インナーセーブがあるかを判定する
// ================================================================
bool tnm_saveload_proc_check_inner_save(int inner_save_no)
{
	// インナーセーブがあるかを判定する
	return tnm_savedata_system_check_inner_save(inner_save_no);
}

// ****************************************************************
// セーブロード処理：バックログロード
// ================================================================
bool tnm_saveload_proc_backlog_load(S_tid save_id)
{
	// ロードできないなら失敗
	if (!tnm_check_backlog_save(save_id))
		return false;

	S_tnm_local_save temp_save;

	// ローカル終了
	tnm_finish_local();

	// バックログセーブをテンポラリに取得
	temp_save = *Gp_backlog_save->map[save_id];

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ローカルセーブデータにテンポラリをコピー
	tnm_set_local_save(&temp_save);

	// ローカルセーブデータからデータをロードする
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// エラーを設定
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("メッセージバックセーブをロードしました。"));

	return true;
}

// ****************************************************************
// セーブロード処理：セーブポイント履歴をロードする
// ================================================================
bool tnm_saveload_proc_load_save_history(S_tnm_local_save* p_local_save)
{
	if (p_local_save->save_stream.empty())
		return false;

	// ローカル終了
	tnm_finish_local();

	// セーブポイント履歴をテンポラリに保存
	S_tnm_local_save temp_save = *p_local_save;

	// ローカル再初期化
	tnm_reinit_local(false);		// ★再構築しない

	// ローカルセーブデータにテンポラリをコピー
	tnm_set_local_save(&temp_save);

	// ローカルセーブデータからロードする
	tnm_load_local();				// ★ここで再構築が行われる

	// メッセージバックセーブを整理する
	tnm_remove_backlog_save();

	// エラーを設定
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブポイント履歴からロードしました。"));

	return true;
}

// ****************************************************************
// セーブポイントを立てる
// ================================================================
void tnm_set_save_point()
{
	// ローカルセーブ
	tnm_save_local();
}

// ****************************************************************
// セーブポイントをクリアする
// ================================================================
void tnm_clear_save_point()
{
	// ローカルセーブデータを初期化
	tnm_init_local_save();
}

// ****************************************************************
// セーブポイントを判定する
// ================================================================
bool tnm_check_save_point()
{
	// ローカルセーブデータがあるかどうかを返す
	return tnm_is_exist_local_save();
}

// ****************************************************************
// 選択肢ポイントを立てる
// ================================================================
void tnm_set_sel_point()
{
	// ローカルセーブデータに選択肢セーブデータをコピー
	tnm_copy_local_save_to_sel_save();
}

// ****************************************************************
// 選択肢ポイントをクリアする
// ================================================================
void tnm_clear_sel_point()
{
	// 選択肢セーブデータを初期化
	tnm_init_sel_save();
}

// ****************************************************************
// 選択肢ポイントを判定する
// ================================================================
bool tnm_check_sel_point()
{
	// 選択肢セーブデータがあるかどうかを返す
	return tnm_is_exist_sel_save();
}

// ****************************************************************
// 選択肢ポイントをストックする
// ================================================================
void tnm_stack_sel_point()
{
	tnm_copy_local_save_to_sel_save_stock();
}

// ****************************************************************
// 選択肢ポイントを降ろす
// ================================================================
void tnm_drop_sel_point()
{
	tnm_copy_sel_save_stock_to_sel_save();
	tnm_init_sel_save_stock();
}

// ****************************************************************
// シーンをエディタで開く
// ================================================================
bool tnm_open_by_editor(bool open_dialog)
{
	// ss フォルダやエディタパスが設定されていない場合
	if (Gp_config->ss_path.empty() || Gp_config->editor_path.empty())	{

		// 設定ダイアログを開く
		if (open_dialog)	{
			tnm_dlg_open_debug_setting_dialog();
		}
	}
	// メッセージバック中
	else if (tnm_msg_back_is_open())	{
		int scn_no = tnm_msg_back_get_scn_no();
		int line_no = tnm_msg_back_get_line_no();
		if (scn_no >= 0 && line_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
			open_file_by_editor(ss_path, line_no, Gp_config->editor_path);
		}
	}
	// その他
	else	{
		// エディタで ss を開く
		int scn_no = Gp_lexer->get_cur_scn_no();
		int line_no = Gp_lexer->get_cur_line_no();
		if (scn_no >= 0 && line_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
			open_file_by_editor(ss_path, line_no, Gp_config->editor_path);
		}
	}

	return true;
}

// ****************************************************************
// 声を関連付けで開く
// ================================================================
bool tnm_open_koe(bool open_dialog)
{
	// 声パスが設定されていない場合
	if (Gp_config->koe_path.empty())	{

		// 設定ダイアログを開く
		if (open_dialog)	{
			tnm_dlg_open_debug_setting_dialog();
		}
	}
	// メッセージバック中
	else if (tnm_msg_back_is_open())	{
		int koe_no = tnm_msg_back_get_koe_no();
		if (koe_no >= 0)	{
			int dir_no = koe_no / 100000;
			
			TSTR path = Gp_config->koe_path + str_format(_T("\\%04d\\Z%09d.wav"), dir_no, koe_no);
			if (!shell_execute_open(path))	{
				tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("声ファイルが開けませんでした。\n") + get_last_error_msg());
			}
		}
	}
	// その他
	else	{
		// エディタで wav を開く
		int koe_no = Gp_local->pod.cur_koe_no;
		if (koe_no >= 0)	{
			int dir_no = koe_no / 100000;
			
			TSTR path = Gp_config->koe_path + str_format(_T("\\%04d\\Z%09d.wav"), dir_no, koe_no);

			// ↓ボツ。ＥＸＥを指定して開くのは無理っぽい…。（起動オプションがモジュールによって違う）
			//if (!Gp_config->koe_tool_path.empty())	{
			//	if (!shell_execute_open_by_module(path, Gp_config->koe_tool_path))	{
			//		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("声ファイルが開けませんでした。\n") + get_last_error_msg());
			//	}
			//}
			//else	{

				// ↓採用。デフォルトの関連付けで開く。
				if (!shell_execute_open(path))	{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("声ファイルが開けませんでした。\n") + get_last_error_msg());
				}
			//}
		}
	}

	return true;
}

















