#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_error.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// �G���W���F�G���[����
// ================================================================
bool tnm_set_error(TNM_ERROR_TYPE error_type, CTSTR& error_str)
{
	return Gp_error->set_error(error_type, error_str);
}

