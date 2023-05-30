#pragma		once

#include	"data/tnm_proc.h"

// ****************************************************************
// プロセススタック：インターフェース
// ================================================================

// プロセスをプッシュ
void	tnm_push_proc(TNM_PROC_TYPE type);

// プロセスをプッシュ
void	tnm_push_proc(TNM_PROC_TYPE type, int option);

// プロセスをプッシュ
void	tnm_push_proc(C_tnm_proc proc);

// プロセスをポップ
void	tnm_pop_proc();
