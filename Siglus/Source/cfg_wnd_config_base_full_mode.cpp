#include	"pch.h"
#include	"dialog/cfg_wnd.h"
#include	"dialog/cfg_wnd_config_base_full_mode.h"

#include	"localize/localize.h"

// ****************************************************************
// �R���t�B�O�E�B���h�E�F�x�[�X�t�����[�h
// ================================================================

#define		REGIST_WINDOW_NAME		_T("cfg_wnd_base_full_mode")

// ������
void C_cfg_wnd_config_base_full_mode::init()
{
	regist(REGIST_WINDOW_NAME, true, true, LCL_IDD_CFG_WIN_BASE);
	cfg_wnd_full_mode_flag = true;		// �� true �̓f�o�b�O�p�̑S���ݒ胂�[�h�ł��B
	init_child_window();
}

