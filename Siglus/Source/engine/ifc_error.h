#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// エラー処理
// ================================================================
bool	tnm_set_error(TNM_ERROR_TYPE error_type, CTSTR& error_str);
void	write_debug_log(CTSTR& str);

