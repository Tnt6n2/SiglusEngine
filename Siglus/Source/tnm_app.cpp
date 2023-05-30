#include	"pch.h"
#include	"tnm_app.h"
#include	"data/tnm_gan_manager.h"
#include	"engine/eng_config.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_sound.h"
#include	"../resource.h"

// ****************************************************************
// �O���[�o���ȃA�N�Z�X�̂��߂̃|�C���^
// ================================================================
class	C_tnm_select_ini;
class	C_tnm_ini;
class	C_tnm_reg_ini;
class	C_tnm_cmd_line;
class	C_tnm_eng;

C_tnm_select_ini*		Gp_select_ini;
C_tnm_ini*				Gp_ini;
C_tnm_reg_ini*			Gp_reg_ini;
C_tnm_cmd_line*			Gp_cmd_line;
C_tnm_eng*				Gp_eng;

// ****************************************************************
// �Í��}���ׂ̈̌����R�[�h
// ================================================================

#include	"tnm_exe_angou.h"

volatile BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE00;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE01;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE02;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE03;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE04;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE05;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE06;		// ���Í��}���ׂ̈̌����R�[�h
volatile BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE] = TNM_EXE_ANGOU_FIND_CODE07;		// ���Í��}���ׂ̈̌����R�[�h


// ****************************************************************
// �ƂȂ܃A�v���P�[�V�����F���C��
// ================================================================
bool C_tnm_app::main()
{
	// �O���[�o���ȃ|�C���^��ݒ肷��
	Gp_reg_ini = &m_reg_ini;
	Gp_select_ini = &m_select_ini;
	Gp_ini = &m_ini;
	Gp_cmd_line = &m_cmd_line;
	Gp_eng = &m_eng;

	//
	// �܂��̓R�}���h���C����ݒ�t�@�C������͂��܂��B
	// �f�B���N�g���̐ݒ�A�Q�[�����A�G���[�������@�Ȃǂ͂܂����肳��Ă��Ȃ��̂ŁA
	// �ُ펞�̓��b�Z�[�W�{�b�N�X���o���đ��₩�ɏI�����܂��B
	//

	// �R�}���h���C������͂���
	if (!Gp_cmd_line->analize_command_line())
	{
		message_box(_T("�R�}���h���C���̉�͂Ɏ��s���܂����B"), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Reg.ini ����͂���
	if (!Gp_reg_ini->analize())
	{
		message_box(_T("�ݒ�t�@�C�� Reg.ini �̉�͂Ɏ��s���܂����B\n�Q�[�����ăZ�b�g�A�b�v���邱�Ƃł��̖��͉�������\��������܂��B"), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Select.ini ����͂���
	if (!Gp_select_ini->analize())
	{
		message_box(_T("�ݒ�t�@�C�� Select.ini �̉�͂Ɏ��s���܂����B\n") + get_last_error_msg(), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	// Gameexe.dat ����͂���
	if (!Gp_ini->analize())
	{
		message_box(_T("�ݒ�t�@�C�� Gameexe.dat �̉�͂Ɏ��s���܂����B\n") + get_last_error_msg(), _T("VisualArt's SiglusEngine"), MB_ICONWARNING);
		return false;
	}

	//
	// �Q�[�������m�肵�܂����B
	// �ȍ~�̃G���[�����ł́A���b�Z�[�W�{�b�N�X�̃L���v�V�����ɃQ�[�������g���ĉ������B
	//

	// ���ɋN�����Ă��� Siglus �G���W������������
	if (!Gp_cmd_line->double_start_set)
	{
		TSTR window_name = tnm_get_window_name();
		HWND another_h_wnd = ::FindWindow(window_name.c_str(), NULL);
		if (another_h_wnd)
		{
			// �R�}���h���C���ōċN�����w�肳��Ă���ꍇ
			if (Gp_cmd_line->end_start_set)
			{
				//  ���ɋN�����Ă��� Siglus �G���W�����I������
				::SendMessage(another_h_wnd, WM_COMMAND, MAKELONG(0, IDC_END), 0);

				// �I������̂�҂i���̊Ԃɐݒ�Ȃǂ�ۑ����Ă����ƐM���Ă�j
				Sleep(200);
			}

			// �ʏ�͊��ɋN�����Ă���̂ŋN�����Ȃ�
			else
			{
				message_box(Gp_ini->game_name + _T(" �͊��ɋN������Ă��܂��B"), Gp_ini->game_name, MB_ICONWARNING);
				return false;
			}
		}
	}

	// �G���W���̃��C���������s��
	m_eng.main();

	return true;
}



