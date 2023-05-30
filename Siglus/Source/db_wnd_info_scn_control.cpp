#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_syscom.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_scn_control.h"
#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�[���R���g���[��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_scn_control")


// ������
void C_db_wnd_info_scn_control::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_SCN_CONTROL);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_scn_control::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_scn_control::on_init()
{
	// �A�C�e���o�^
	bind_control(&ci.lst_scn_name, IDC_LST_SCN_NAME, _T(""), 5);
	bind_control(&ci.lst_z_no, IDC_LST_Z_NO, _T(""), 6);
	bind_control(&ci.btn_restart, IDC_BTN_RESTART, _T(""), 2);
	bind_control(&ci.btn_select_start, IDC_BTN_SELECT_START, _T(""), 2);

	// �Z�p���[�^�o�^
	ci.spa_v00.set_hv_mod(true, &ci.lst_scn_name, &ci.lst_z_no);
	bind_separater(&ci.spa_v00, _T("spa_v00"));

	// �V�[�������X�g�쐬
	make_scn_name_list();

	// �V�[�������X�g�̏����I��ݒ�
	int scn_no = Gp_lexer->get_scn_no(m_state.scn_name);
	if (scn_no == -1)	{
		scn_no = Gp_lexer->get_scn_no(Gp_ini->start_scene_name);	// ini �̕����g���I
	}
	ci.lst_scn_name.set_selected_item_no(scn_no);

	// �y�ԍ����X�g�쐬
	make_z_no_list();

	// �y�ԍ����X�g�̏����I��ݒ�
	ci.lst_z_no.set_selected_item_no(get_item_no_from_z_no(m_state.z_no));

	// �A�C�e���ݒ胍�[�h
	int top = C_ini_file::body().load_int(_T("scn_name_list_top"), 0, REGIST_WINDOW_NAME);
	ci.lst_scn_name.send_message(LB_SETTOPINDEX, top, 0);
	top = C_ini_file::body().load_int(_T("z_no_list_top"), 0, REGIST_WINDOW_NAME);
	ci.lst_z_no.send_message(LB_SETTOPINDEX, top, 0);

	// �Z�p���[�^�ݒ胍�[�h
	separater_manager.load_ini();

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �A�C�e���t�H�[�J�X

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_scn_control::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�
	get_scn_name();
	get_z_no();
	save_state();

	// �A�C�e���ݒ�Z�[�u
	int top = (int)ci.lst_scn_name.send_message(LB_GETTOPINDEX, 0, 0);
	C_ini_file::body().save_int(_T("scn_name_list_top"), top, REGIST_WINDOW_NAME);
	top = (int)ci.lst_z_no.send_message(LB_GETTOPINDEX, 0, 0);
	C_ini_file::body().save_int(_T("z_no_list_top"), top, REGIST_WINDOW_NAME);

	// �Z�p���[�^�ݒ�Z�[�u
	separater_manager.save_ini();

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_scn_control::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_BTN_RESTART:
			tnm_syscom_restart_from_start();
			break;

		case IDC_BTN_SELECT_START:
			tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
			break;

		case IDC_LST_SCN_NAME:
			switch (HIWORD(wp))	{
				case LBN_SELCHANGE:	make_z_no_list();	break;
				case LBN_DBLCLK:
					tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
					break;
			}
			break;
		case IDC_LST_Z_NO:
			switch (HIWORD(wp))	{
				case LBN_DBLCLK:
					tnm_syscom_restart_from_scene(get_scn_no(), get_z_no());
					break;
			}
			break;
	}
	return true;
}

// �V�[�������X�g�쐬
void C_db_wnd_info_scn_control::make_scn_name_list()
{
	int scn_cnt = Gp_lexer->get_scn_cnt();
	for (int scn_no = 0; scn_no < scn_cnt; scn_no++)	{
		TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
		ci.lst_scn_name.add_item(scn_name);
	}
}

// �y�ԍ����X�g�쐬
void C_db_wnd_info_scn_control::make_z_no_list()
{
	ci.lst_z_no.clear();	// ���X�g�{�b�N�X�N���A
	m_z_no_list.clear();		// �n�b�V���N���A

	int scn_no = ci.lst_scn_name.get_selected_item_no();	// �V�[�������X�g�̑I��ԍ��擾
	if (scn_no == LB_ERR)	{	return;	}

	for (int z_no = 0; z_no < TNM_Z_LABEL_CNT; z_no++)	{
		if (Gp_lexer->is_exist_z(scn_no, z_no))	{
			ci.lst_z_no.add_item(str_format(_T("#z%02d"), z_no));
			m_z_no_list.push_back(z_no);
		}
	}
}

// ���ۂ̂y�ԍ�����y�ԍ����X�g�̃A�C�e���ԍ����擾
int C_db_wnd_info_scn_control::get_item_no_from_z_no(int z_no)
{
	ARRAY<int>::iterator itr = std::find(m_z_no_list.begin(), m_z_no_list.end(), z_no);
	if (itr == m_z_no_list.end())	{	return -1;	}
	return (int)(itr - m_z_no_list.begin());
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_scn_control::load_state()
{
	m_state.scn_name = C_ini_file::body().load_str(_T("scn_name"), Gp_global->start_scene_name, REGIST_WINDOW_NAME);
	m_state.z_no = C_ini_file::body().load_int(_T("z_no"), Gp_global->start_z_no, REGIST_WINDOW_NAME);
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_scn_control::save_state()
{
	C_ini_file::body().save_str(_T("scn_name"), m_state.scn_name, REGIST_WINDOW_NAME);
	C_ini_file::body().save_int(_T("z_no"), m_state.z_no, REGIST_WINDOW_NAME);
}

// ****************************************************************
// �V�[�����擾
// ================================================================
TSTR C_db_wnd_info_scn_control::get_scn_name()
{
	if (!m_h_wnd)	{	return m_state.scn_name;	}
	int scn_no = ci.lst_scn_name.get_selected_item_no();
	m_state.scn_name = (scn_no == LB_ERR) ? Gp_ini->start_scene_name : Gp_lexer->get_scn_name(scn_no);	// ini �̕����g���I
	if (m_state.scn_name.empty())	{	m_state.scn_name = Gp_ini->start_scene_name;	}
	return m_state.scn_name;
}

// ****************************************************************
// �V�[���ԍ��擾
// ================================================================
int C_db_wnd_info_scn_control::get_scn_no()
{
	get_scn_name();
	int scn_no = Gp_lexer->get_scn_no(m_state.scn_name);
	if (scn_no == -1)	{
		scn_no = Gp_lexer->get_scn_no(Gp_ini->start_scene_name);	// ini �̕����g���I
	}
	return scn_no;
}

// ****************************************************************
// �y�ԍ��擾
// ================================================================
int C_db_wnd_info_scn_control::get_z_no()
{
	if (!m_h_wnd)	{	return m_state.z_no;	}
	int z_no = ci.lst_z_no.get_selected_item_no();
	m_state.z_no = (z_no == LB_ERR) ? 0 : m_z_no_list[z_no];
	return m_state.z_no;
}

