#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_prop.h"
#include	"engine/ifc_steam.h"
#include	"engine/flow_command.h"

// ****************************************************************
// コマンド処理：Steam
// ================================================================
bool tnm_command_proc_steam(int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	switch (elm_begin[0])	{

#if __USE_STEAM

	case ELM_STEAM_SET_ACHIEVEMENT:
		steam_set_achievement(p_ai->al_begin[0].str);
		break;
	case ELM_STEAM_RESET_ALL_STATUS:
		steam_reset_all_status();
		break;
#endif

	default:
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("無効なコマンドが指定されました。(steam)"));
		break;

	}

	return true;
}
