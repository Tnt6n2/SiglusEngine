#pragma		once

#include	"data/tnm_timer.h"
#include	"data/tnm_proc_def.h"
#include	"element/elm_prop.h"

class	C_tnm_save_stream;

// ****************************************************************
// プロセス
// ================================================================
class C_tnm_proc
{
public:
	C_tnm_proc()
	{
		type = TNM_PROC_TYPE_NONE;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = 0;
	}

	explicit C_tnm_proc(TNM_PROC_TYPE type_)
	{
		type = type_;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = 0;
	}

	C_tnm_proc(TNM_PROC_TYPE type_, int option_)
	{
		type = type_;
		arg_list_id = 0;
		key_skip_enable_flag = false;
		skip_disable_flag = false;
		return_value_flag = false;
		option = option_;
	}

	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	TNM_PROC_TYPE				type;			// プロセスのタイプ
	S_element					element;		// プロセスに関するエレメント
	int							arg_list_id;	// プロセスに関する引数ＩＤ
	C_tnm_prop_list				arg_list;		// プロセスに関する引数

	bool						key_skip_enable_flag;	// キーでスキップできる
	bool						skip_disable_flag;		// すっ飛ばせない
	bool						return_value_flag;		// 終了時に値を戻す
	int							option;					// オプション
	S_tid						option_time_id;			// オプションとなるID
};

