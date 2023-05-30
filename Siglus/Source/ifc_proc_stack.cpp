#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_input.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_proc.h"

#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// プロセス：構造体
// ================================================================
void C_tnm_proc::save(C_tnm_save_stream& stream)
{
	stream.save(type);
	stream.save(element);
	stream.save(arg_list_id);
	arg_list.save(stream);
	stream.save(key_skip_enable_flag);
	stream.save(skip_disable_flag);
	stream.save(return_value_flag);
	stream.save(option);
}

void C_tnm_proc::load(C_tnm_save_stream& stream)
{
	stream.load(type);
	stream.load(element);
	stream.load(arg_list_id);
	arg_list.load(stream);
	stream.load(key_skip_enable_flag);
	stream.load(skip_disable_flag);
	stream.load(return_value_flag);
	stream.load(option);
}


// ****************************************************************
// プロセス：インターフェース：プロセスをプッシュ
// ================================================================
void tnm_push_proc(TNM_PROC_TYPE type, int option)
{
	C_tnm_proc proc;
	proc.type = type;
	proc.option = option;

	tnm_push_proc(proc);
}

void tnm_push_proc(TNM_PROC_TYPE type)
{
	C_tnm_proc proc;
	proc.type = type;

	tnm_push_proc(proc);
}

void tnm_push_proc(C_tnm_proc proc)
{
	// 現在のプロセスを記憶
	Gp_local->proc_stack.push_back(Gp_local->cur_proc);

	// 次のプロセスへ
	Gp_local->cur_proc = proc;

	// プロセスのタイプによっては自動で決定キークリア
	if (TNM_PROC_STATE[proc.type].key_wait)
		Gp_input->clear_ex_key(VK_EX_DECIDE);	// 決定キーのみ

	// ================================================================
	// ★上の処理は以下のような場合に必要です。
	//
	//	while (1)	{
	//		if (input.decide.is_down)	{break}
	//		disp
	//	}
	//	wipe	← クリアがないと、このワイプが、上のクリックでスキップされてしまいます。
	// ================================================================
}

// ****************************************************************
// プロセス：インターフェース：プロセスをポップ
// ================================================================
void tnm_pop_proc()
{
	// 次のプロセスへ
	Gp_local->cur_proc = Gp_local->proc_stack.back();
	Gp_local->proc_stack.pop_back();
}

