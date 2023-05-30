#pragma		once

#include	"IA.h"
#include	"CA.h"
#include	"LA.h"
#include	"SA.h"

// コンパイラ：メイン
class	C_tnm_compiler_main;
extern	C_tnm_compiler_main*	G_main;

// コンパイラ：ダイアログ
class	C_tnm_compiler_dialog;
extern	C_tnm_compiler_dialog*	G_dlg;

// コンパイラ：データ
class	C_tnm_compiler_data;
extern	C_tnm_compiler_data*	G_data;

// スクリプト：コンパイルデータ
struct	C_tnms_scn_compile_data
{
	int				lv_item_no;
	TSTR			scn_name;
	S_tnms_ia_data	ia_data;
	S_tnms_ca_data	ca_data;
	S_tnms_la_data	la_data;
	S_tnms_sa_data	sa_data;
};

// プロジェクト設定モード
enum	PROJECT_SETTING_MODE
{
	PROJECT_SETTING_MODE_SETTING,
	PROJECT_SETTING_MODE_CREATE,
	PROJECT_SETTING_MODE_DELETE,
};


// ****************************************************************
// メイン
// ================================================================
class C_tnm_compiler_main
{
public:
	// コンストラクタ
	C_tnm_compiler_main();									// コンストラクタ

	// 処理
	void	save_ini();										// ini セーブ
	void	load_ini();										// ini ロード
	void	get_project_item_state();						// プロジェクト関連のアイテム状態を取得
	void	set_project_item_state();						// プロジェクト関連のアイテム状態を設定
	void	update_dialog_for_window_title();				// ウィンドウタイトルに ss のパスを表示
	void	update_dialog_for_all_btn(bool is_cancel);		// 全部ボタン（再検索→自動選択or全選択→ビルド→実行）（再検索→自動選択→ビルド→実行）
	void	update_dialog_for_start_item();					// スタート関連のアイテム
	void	update_dialog();								// ダイアログを更新
	bool	update_scn_list();								// シーンリストを更新

	// 操作
	void	change_project(int no);							// プロジェクトを変更
	bool	on_setting_project();							// プロジェクト設定
	bool	on_create_project();							// プロジェクト作成（新規プロジェクト）
	bool	on_delete_project();							// プロジェクト削除
	bool	on_command_find();								// 再検索
	bool	on_command_sel_all();							// 全選択
	bool	on_command_unsel_all();							// 全解除
	bool	on_command_sel_auto();							// 自動選択
	bool	on_command_build();								// ビルド
	bool	on_command_run();								// 実行
	bool	on_command_all(bool is_sel_auto);				// 全部（再検索→自動選択or全選択→ビルド→実行）
	bool	on_command_manual();							// マニュアル
	bool	start_auto_process();							// 自動操作を実行
	bool	explain_under();								// 右下についての説明

	// ログ処理
	void	set_log(CTSTR& text);							// ログを設定
	void	set_log_jump(CTSTR& text, CTSTR& detail_file_path, CTSTR& jump_file_path, int line_no);

	// ビューから開く
	void	open_ini_by_ini_lv(int row_no);					// ini ビューから ini を開く
	void	open_inc_by_inc_lv(int row_no);					// inc ビューから inc を開く
	void	open_scn_by_scn_lv(int row_no);					// scn ビューから scn を開く
	void	open_scn_by_log_lv(int row_no);					// ログビューから scn を開く

	// フォルダを開く
	bool	open_folder(CTSTR& path);						// フォルダオープン
	bool	open_folder_from_scn_path();					// scn パスのフォルダを開く
	bool	open_folder_from_tmp_path();					// tmp パスのフォルダを開く
	bool	open_folder_from_out_path();					// out パスのフォルダを開く
	bool	open_folder_from_exe_path();					// exe パスのフォルダを開く
	bool	open_folder_from_work_path();					// work パスのフォルダを開く
	bool	open_folder_from_editor_path();					// editor パスのフォルダを開く
	bool	open_folder_from_manual_path();					// manual パスのフォルダを開く

	// ポップアップメニュー処理
	void	popup_menu_proc(CTSTR& start_scene_str = _T(""));

	// エラーの出た行の詳細をステータスバーに表示
	void	show_err_detail(int row_no);					// エラーの出た行の詳細をステータスバーに表示

	// プロジェクトが選択されているかどうか判定する
	bool	check_current_project(bool is_warning);			// プロジェクトが選択されているかどうか判定する

	// 暗号関連
	bool	load_exe_angou_dat(TSTR& angou_str);			// 暗号.dat の読み込み
	void	manual_exe_angou();								// 暗号についての説明
	void	func_exe_angou();								// ＥＸＥを暗号化する

private:

	// 操作の前後
	bool	on_command_befor_proc(int mod);					// 操作の前処理
	bool	on_command_after_proc(int mod);					// 操作の後処理

	// 処理
	bool	set_project(PROJECT_SETTING_MODE proc_mode);				// プロジェクト設定
	bool	proc_find();												// 再検索
	bool	proc_sel_all(bool mod);										// 全選択／全解除
	bool	proc_sel_auto();											// 自動選択
	bool	proc_build();												// ビルド
	bool	proc_build_after();											// ビルド後の処理
	bool	proc_run(CTSTR& start_scene_str = _T(""), int z_no = 0);	// 実行
	bool	proc_all(bool is_sel_auto);									// 全部（再検索→自動選択or全選択→ビルド→実行）

	// 処理：ビルド関連：Gameexe.ini 解析
	bool	proc_gei();										// Gameexe.ini 解析

	// 処理：ビルド関連：inc ファイル解析
	bool	proc_inc(S_tnms_ia_data* piad);					// inc ファイル解析

	// 処理：ビルド関連：コンパイル
	struct COMPILE_FUNK_STRUCT
	{
		TSTR	scn_dir_path;
		TSTR	tmp_dir_path;
		TSTR	scn_file_path;
		TSTR	scn_file_name;
		TSTR	ca_file_path;
		bool	utf8;
		bool	test_check;
	};
	bool	proc_compile(S_tnms_ia_data* piad, int* p_error_cnt_ret);															// コンパイル本体
	bool	proc_compile_funk_ca_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data, CTSTR scn_text);	// コンパイル：文字解析
	bool	proc_compile_funk_la_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// コンパイル：字句解析
	bool	proc_compile_funk_sa_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// コンパイル：構文解析
	bool	proc_compile_funk_ma_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);					// コンパイル：意味解析
	bool	proc_compile_funk_hankaku_param_format(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data);			// コンパイル：半角文字列パラメータの変換（※エラーはありえないのでエラー処理はまったく考えていません）

	// 処理：ビルド関連：リンク
	bool	proc_link(S_tnms_ia_data* piad);			// リンク

	// プロセスフラグ
	bool	proc_execut_flag;		// プロセス実行中フラグ
	bool	proc_cancel_flag;		// プロセスキャンセルフラグ

	// シーンコンパイルデータ
};
