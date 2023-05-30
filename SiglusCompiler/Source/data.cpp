#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"
#include	"../resource.h"

// ****************************************************************
// データ：コンストラクタ
// ================================================================
C_tnm_compiler_data::C_tnm_compiler_data()
{
	cur_project_no = -1;
	gui_flag = true;

	// コマンドライン情報
	cl_project_no_set = false;
	cl_log_set = false;
	cl_project_no = -1;
	cl_log.clear();
}


