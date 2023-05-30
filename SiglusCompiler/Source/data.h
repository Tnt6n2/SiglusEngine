#pragma		once

const int SCN_ID_NONE = -1;		// 初期値

// ****************************************************************
// プロジェクトデータ
// ================================================================
struct S_tnm_compiler_project_data
{
	TSTR	project_name;
	TSTR	scn_path;
	TSTR	tmp_path;
	TSTR	out_path;
	TSTR	out_path_noangou;
	TSTR	exe_path;
	TSTR	work_path;
	TSTR	editor_path;
	TSTR	manual_path;
	TSTR	build_after;
	TSTR	angou_str;
	TSTR	gameexe_ini;
	TSTR	extend_inc;
	TSTR	gameexe_dat;
	TSTR	scene_pck;
	TSTR	start_scene;

	int		start_z_no;

	bool	start_check;
	bool	angou_check;
	bool	utf8;
};

// ****************************************************************
// データ
// ================================================================

// エラージャンプ構造体
struct S_error_jump
{
	TSTR	detail_file_path;
	TSTR	jump_file_path;
	int		line_no;
};

// シーン情報
struct S_scn_info
{
	S_scn_info()
	{
		scn_id = SCN_ID_NONE;
	}
	S_scn_info(CTSTR& scn_name_, int scn_id_)
	{
		scn_name = scn_name_;
		scn_id = scn_id_;
	}

	TSTR	scn_name;
	int		scn_id;
};

class C_tnm_compiler_data
{
public:
	C_tnm_compiler_data();

	// プロジェクト設定
	ARRAY<S_tnm_compiler_project_data>	project;

	int		cur_project_no;		// 現在のプロジェクト番号
	bool	gui_flag;			// GUI フラグ

	// コマンドライン情報
	bool	cl_project_no_set;	// コマンドライン情報：プロジェクト番号：設定されている
	bool	cl_log_set;			// コマンドライン情報：出力先：設定されている
	bool	cl_full_set;		// コマンドライン情報：フルコンパイル：設定されている
	int		cl_project_no;		// コマンドライン情報：プロジェクト番号
	TSTR	cl_log;				// コマンドライン情報：出力先

	// CUI モード用のログ
	TSTR	cui_full_log;		// CUI モードのログ

	// ini リスト
	ARRAY<TSTR>	ini_list;

	// inc リスト
	ARRAY<TSTR>	inc_list;

	// scn リスト
	ARRAY<S_scn_info>	scn_list;

	// エラージャンプリスト
	ARRAY<S_error_jump>	error_jump_list;
};
