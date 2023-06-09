#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"

TSTR C_tnm_dir::get_thumb_file_name(int thumb_no)
{
	if (Gp_ini->save_thumb_type == TNM_SAVE_THUMB_TYPE_PNG)	{
		return str_format(_T("%010d"), thumb_no) + _T(".png");
	}
	else	{
		return str_format(_T("%010d"), thumb_no) + _T(".bmp");
	}
}

TSTR C_tnm_dir::get_save_thumb_file_name(int save_no)
{
	if (Gp_ini->save_thumb_type == TNM_SAVE_THUMB_TYPE_PNG)	{
		return str_format(_T("%04d"), save_no) + _T(".png");
	}
	else	{
		return str_format(_T("%04d"), save_no) + _T(".bmp");
	}
}

