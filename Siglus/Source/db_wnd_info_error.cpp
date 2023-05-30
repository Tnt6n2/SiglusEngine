#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_error.h"
#include	"engine/ifc_error.h"
#include	"dialog/db_wnd_info_error.h"

#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�G���[
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_error")

// ������
void C_db_wnd_info_error::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_ERROR);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_error::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_error::on_init()
{
	// ������
	m_ld.error_cnt = 0;

	// �A�C�e���o�^
	bind_control(&ci.lv, IDC_LV, _T("lv"), 5);
	bind_control(&ci.chk_only_error, IDC_CHK_ONLY_ERROR, _T("chk_only_error"), 1);
	ci.lv.easy_setting(true, true, false, true);
	ci.lv.add_column(_T("type"), _T("����"), 100);
	ci.lv.add_column(_T("error"), _T("�G���["), 300);

	// �Z�p���[�^�o�^

	// �A�C�e���ݒ胍�[�h
	ci.lv.load_ini();
	ci.chk_only_error.load_ini(false);

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
bool C_db_wnd_info_error::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�
	ci.lv.save_ini();
	ci.chk_only_error.save_ini();

	// �Z�p���[�^�ݒ�Z�[�u

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// �R�}���h
bool C_db_wnd_info_error::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_ONLY_ERROR:
			clear_lv();
			break;
	}
	return true;
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_error::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	cd.error_cnt = (int)Gp_error->m_error_list.size();
	if (cd.error_cnt != m_ld.error_cnt)	{

		bool error_only = ci.chk_only_error.get_check();
		for (int i = m_ld.error_cnt; i < cd.error_cnt; i++)	{
			if (!error_only || Gp_error->m_error_list[i].error_type != TNM_ERROR_TYPE_SUCCESS)	{
				add_lv_item(Gp_error->m_error_list[i].error_type, Gp_error->m_error_list[i].error_str);
			}
		}
	}

	m_ld = cd;
}

// ****************************************************************
// �G���[�r���[���N���A
// ================================================================
void C_db_wnd_info_error::clear_lv()
{
	ci.lv.clear();

	// �p�����[�^��ݒ肷��
	m_ld.error_cnt = 0;
}

void C_db_wnd_info_error::add_lv_item(TNM_ERROR_TYPE error_type, CTSTR& error_str)
{
	int item_no = ci.lv.add_item();

	switch (error_type)	{
		case TNM_ERROR_TYPE_SUCCESS:			ci.lv.set_cell_text(item_no, 0, _T(""));				break;
		case TNM_ERROR_TYPE_DIVIDED_BY_ZERO:	ci.lv.set_cell_text(item_no, 0, _T("��0���Z"));			break;
		case TNM_ERROR_TYPE_OUT_OF_RANGE:		ci.lv.set_cell_text(item_no, 0, _T("���͈͊O����"));	break;
		case TNM_ERROR_TYPE_FILE_NOT_FOUND:		ci.lv.set_cell_text(item_no, 0, _T("��̧�ٴװ"));		break;
		case TNM_ERROR_TYPE_PCT_NOT_FOUND:		ci.lv.set_cell_text(item_no, 0, _T("��̧�ٴװ"));		break;
		case TNM_ERROR_TYPE_SOUND_NOT_FOUND:	ci.lv.set_cell_text(item_no, 0, _T("��̧�ٴװ"));		break;
		case TNM_ERROR_TYPE_KOE_NOT_FOUND:		ci.lv.set_cell_text(item_no, 0, _T("��̧�ٴװ"));		break;
		default:								ci.lv.set_cell_text(item_no, 0, _T("���װ"));			break;
	}

	ci.lv.set_cell_text_visible(item_no, 1, error_str);
}

