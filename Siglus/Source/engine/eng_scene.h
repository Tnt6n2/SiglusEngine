#pragma		once

#include	"data/tnm_proc.h"

// ****************************************************************
// シーン処理
// ================================================================

struct	S_tid;
class	C_tnm_save_stream;
struct	S_tnm_local_save;
class	C_elm_call;

// シーンの情報を取得

// 現在のプロセスを取得
C_tnm_proc* tnm_get_cur_proc();

// 現在のコールを取得
C_elm_call* tnm_get_cur_call();

// 現在のコール数を取得
int tnm_get_cur_call_cnt();

// Scene.pck を読み直す
bool	tnm_reload_scene_pck();

// シーン遷移処理
bool	tnm_scene_proc_gosub(int label_no, int ret_form_code);		// ラベルコール
bool	tnm_scene_proc_jump(int scn_no, int z_no);					// シーンジャンプ（シーン番号を指定）
bool	tnm_scene_proc_jump(CTSTR& scn_name, int z_no);				// シーンジャンプ（シーン名を指定）
bool	tnm_scene_proc_call(int scn_no, int sub_no);				// サブルーチンコール（サブルーチン番号を指定）
bool	tnm_scene_proc_call(int scn_no, CTSTR& sub_name);			// サブルーチンコール（サブルーチン名を指定）
bool	tnm_scene_proc_farcall(int scn_no, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);							// シーンコール（シーン番号を指定）
bool	tnm_scene_proc_farcall(CTSTR& scn_name, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);					// シーンコール（シーン名を指定）
bool	tnm_scene_proc_call_user_cmd(int scn_no, int user_cmd_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag);			// ユーザコマンドコール（ユーザコマンド番号を指定）
bool	tnm_scene_proc_call_user_cmd(int scn_no, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag);		// ユーザコマンドコール（ユーザコマンド名を指定）
bool	tnm_scene_proc_call_user_cmd(CTSTR& scn_name, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag);	// ユーザコマンドコール（シーン名を指定）
bool	tnm_scene_proc_return();									// リターン

// コールスタック処理
void	tnm_scene_init_call_stack();
void	tnm_scene_del_call_stack(int cnt);
void	tnm_scene_set_call_stack_cnt(int cnt);
int		tnm_scene_get_call_stack_cnt();

bool	tnm_scene_proc_start_from_start_scene();					// スタートシーンからスタート
bool	tnm_scene_proc_restart_from_start_scene();					// スタートシーンからリスタート
bool	tnm_scene_proc_restart_from_menu_scene();					// メニューシーンからリスタート
bool	tnm_scene_proc_restart(CTSTR& scn_name, int z_no);			// 指定のシーンからリスタート（シーン名を指定）
bool	tnm_scene_proc_restart(int scn_no, int z_no);				// 指定のシーンからリスタート（シーン番号を指定）

bool	tnm_scene_proc_call_syscom_scene();							// システムコマンドシーンを呼ぶ
bool	tnm_scene_proc_call_flick_scene(int no);					// フリックシーンを呼ぶ
bool	tnm_scene_proc_call_save_scene();							// セーブシーンを呼ぶ
bool	tnm_scene_proc_call_load_scene();							// ロードシーンを呼ぶ
bool	tnm_scene_proc_call_config_scene();							// 環境設定シーンを呼ぶ

// セーブロード処理
bool	tnm_saveload_proc_save(int save_no);									// セーブ
bool	tnm_saveload_proc_load(int save_no);									// ロード
bool	tnm_saveload_proc_quick_save(int save_no);								// クイックセーブ
bool	tnm_saveload_proc_quick_load(int save_no);								// クイックロード
bool	tnm_saveload_proc_end_save();											// エンドセーブ
bool	tnm_saveload_proc_end_load();											// エンドロード
bool	tnm_saveload_proc_backlog_load(S_tid save_id);							// バックログロード
bool	tnm_saveload_proc_load_save_history(S_tnm_local_save* p_local_save);	// セーブポイント履歴からロードする
bool	tnm_saveload_proc_return_to_sel();										// 前の選択肢に戻る

bool	tnm_saveload_proc_create_inner_save(int inner_save_no);					// インナーセーブを作成する
bool	tnm_saveload_proc_load_inner_save(int inner_save_no);					// インナーセーブをロードする
bool	tnm_saveload_proc_clear_inner_save(int inner_save_no);					// インナーセーブをクリアする
bool	tnm_saveload_proc_copy_inner_save(int from, int to);					// インナーセーブをコピーする
bool	tnm_saveload_proc_check_inner_save(int inner_save_no);					// インナーセーブを判定する

// セーブポイント
void	tnm_set_save_point();					// セーブポイントを立てる
void	tnm_clear_save_point();					// セーブポイントをクリアする
bool	tnm_check_save_point();					// セーブポイントを判定する

// 選択肢ポイント
void	tnm_set_sel_point();					// 選択肢ポイントを立てる
void	tnm_clear_sel_point();					// 選択肢ポイントをクリアする
bool	tnm_check_sel_point();					// 選択肢ポイントを判定する
void	tnm_stack_sel_point();					// 選択肢ポイントを積む
void	tnm_drop_sel_point();					// 選択肢ポイントを降ろす

// シーンをエディタで開く
bool	tnm_open_by_editor(bool open_dialog);
bool	tnm_open_koe(bool open_dialog);
