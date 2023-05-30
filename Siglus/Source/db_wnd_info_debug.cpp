#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_debug.h"
#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�f�o�b�O�ݒ�
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_debug")

// ������
void C_db_wnd_info_debug::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_DEBUG);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_debug::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_debug::on_init()
{
	// �A�C�e���o�^
	bind_control(&ci.stc_ss, IDC_STC_SS, _T("stc_ss"), 7);
	bind_control(&ci.edt_ss, IDC_EDT_SS, _T("edt_ss"), 8);
	bind_control(&ci.stc_editor, IDC_STC_EDITOR, _T("stc_editor"), 7);
	bind_control(&ci.edt_editor, IDC_EDT_EDITOR, _T("edt_editor"), 8);
	bind_control(&ci.stc_koe, IDC_STC_KOE, _T("stc_koe"), 7);
	bind_control(&ci.edt_koe, IDC_EDT_KOE, _T("edt_koe"), 8);
	bind_control(&ci.stc_koe_tool, IDC_STC_KOE_TOOL, _T("stc_koe_tool"), 7);
	bind_control(&ci.edt_koe_tool, IDC_EDT_KOE_TOOL, _T("edt_koe_tool"), 8);
	bind_control(&ci.btn_open, IDC_BTN_OPEN, _T("btn_open"), 3);

	// �Z�p���[�^�o�^

	// �A�C�e���ݒ胍�[�h
	ci.edt_ss.set_text(Gp_config->ss_path);
	ci.edt_editor.set_text(Gp_config->editor_path);
	ci.edt_koe.set_text(Gp_config->koe_path);
	ci.edt_koe_tool.set_text(Gp_config->koe_tool_path);

	// �Z�p���[�^�ݒ胍�[�h

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �A�C�e���t�H�[�J�X

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_debug::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�

	// �Z�p���[�^�ݒ�Z�[�u

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_debug::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_EDT_SS:
			on_edit_ss_path();
			break;
		case IDC_EDT_EDITOR:
			on_edit_editor_path();
			break;
		case IDC_EDT_KOE:
			on_edit_koe_path();
			break;
		case IDC_EDT_KOE_TOOL:
			on_edit_koe_tool_path();
			break;
		case IDC_BTN_OPEN:
			on_btn_open();
			break;
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_debug::load_state()
{
	// �����̊֐��͕s�v�Ȃ�폜���Ă��ǂ��ł��B
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_debug::save_state()
{
	// �����̊֐��͕s�v�Ȃ�폜���Ă��ǂ��ł��B
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_debug::frame()
{
	// �����̊֐��͕s�v�Ȃ�폜���Ă��ǂ��ł��B
}

// ****************************************************************
// ss �t�H���_��ݒ肵��
// ================================================================
bool C_db_wnd_info_debug::on_edit_ss_path()
{
	TSTR ss_path = ci.edt_ss.get_text();
	Gp_config->ss_path = ss_path;

	return true;
}

// ****************************************************************
// �G�f�B�^�t�H���_��ݒ肵��
// ================================================================
bool C_db_wnd_info_debug::on_edit_editor_path()
{
	TSTR editor_path = ci.edt_editor.get_text();
	Gp_config->editor_path = editor_path;

	return true;
}

// ****************************************************************
// ���t�H���_��ݒ肵��
// ================================================================
bool C_db_wnd_info_debug::on_edit_koe_path()
{
	TSTR koe_path = ci.edt_koe.get_text();
	Gp_config->koe_path = koe_path;

	return true;
}

// ****************************************************************
// ���c�[���t�H���_��ݒ肵��
// ================================================================
bool C_db_wnd_info_debug::on_edit_koe_tool_path()
{
	TSTR koe_tool_path = ci.edt_koe_tool.get_text();
	Gp_config->koe_tool_path = koe_tool_path;

	return true;
}

// ****************************************************************
// �J���{�^����������
// ================================================================
bool C_db_wnd_info_debug::on_btn_open()
{
	// �V�[�����G�f�B�^�ŊJ��
	tnm_open_by_editor(false);

	return true;
}

