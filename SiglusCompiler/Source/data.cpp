#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"
#include	"../resource.h"

// ****************************************************************
// �f�[�^�F�R���X�g���N�^
// ================================================================
C_tnm_compiler_data::C_tnm_compiler_data()
{
	cur_project_no = -1;
	gui_flag = true;

	// �R�}���h���C�����
	cl_project_no_set = false;
	cl_log_set = false;
	cl_project_no = -1;
	cl_log.clear();
}


