#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"element/elm_database.h"
#include	"engine/eng_database.h"

void tnm_database_init()
{
	Gp_database_list->init(S_element(ELM_GLOBAL_DATABASE), FM_DATABASELIST, _T("database"), Gp_ini->database_cnt, false, NULL);
}

