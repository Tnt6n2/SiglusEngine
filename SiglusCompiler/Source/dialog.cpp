#include	"pch.h"
#include	"../resource.h"

#include	"tnm_common_header.h"

#include	"main.h"
#include	"dialog.h"
#include	"data.h"

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT	C_tnm_compiler_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
		case WM_INITMENU:		if (!on_initmenu(wp, lp))	return FALSE;	break;
	}

	return C_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// �E�B���h�E�����ꂽ
// ================================================================
bool C_tnm_compiler_dialog::on_init_dialog()
{
	// ���C���E�B���h�E�ɐݒ�
	set_main_window();

	// �A�C�R����ݒ肷��
	set_icon_from_resource(IDI_ICON1);

	// �R���g���[���̊��蓖��
	bind_control(&ci.cmb_project, IDC_CMB_PROJECT, _T(""), 8, 1);
	bind_control(&ci.btn_project_config, IDC_BTN_PROJECT_CONFIG, _T(""), 9, 1);
	bind_control(&ci.btn_new_project, IDC_BTN_NEW_PROJECT, _T(""), 9, 1);
	bind_control(&ci.btn_del_project, IDC_BTN_DEL_PROJECT, _T(""), 9, 1);

	bind_control(&ci.chk_debug_on, IDC_CHK_DEBUG_ON, _T("chk_debug_on"), 7, 1);
	bind_control(&ci.chk_debug_off, IDC_CHK_DEBUG_OFF, _T("chk_debug_off"), 7, 1);
	bind_control(&ci.chk_execute, IDC_CHK_EXECUTE, _T("chk_execute"), 7, 1);
	bind_control(&ci.chk_start_scene, IDC_CHK_START_SCENE, _T(""), 7, 1);
	bind_control(&ci.edt_start_scene, IDC_EDT_START_SCENE, _T(""), 8, 2);	// 2 = �X�^�[�g�V�[���̃`�F�b�N�{�b�N�X�̏�Ԃ̉e�����󂯂܂��B
	bind_control(&ci.edt_start_z_no, IDC_EDT_START_Z_NO, _T(""), 9, 2);		// 2 = �X�^�[�g�V�[���̃`�F�b�N�{�b�N�X�̏�Ԃ̉e�����󂯂܂��B
	bind_control(&ci.spn_start_z_no, IDC_SPN_START_Z_NO, _T(""), 9, 2);		// 2 = �X�^�[�g�V�[���̃`�F�b�N�{�b�N�X�̏�Ԃ̉e�����󂯂܂��B

	bind_control(&ci.btn_find, IDC_BTN_FIND, _T(""), 7, 1);
	bind_control(&ci.btn_sel_auto, IDC_BTN_SEL_AUTO, _T(""), 7, 1);
	bind_control(&ci.btn_compile, IDC_BTN_COMPILE, _T(""), 7, 1);
	bind_control(&ci.btn_run, IDC_BTN_RUN, _T(""), 7, 1);

	bind_control(&ci.btn_all_sel_all, IDC_BTN_ALL_SEL_ALL, _T(""), 7, 0);		// 0 = �L�����Z���{�^���ɂȂ�܂�
	bind_control(&ci.btn_all_sel_auto, IDC_BTN_ALL_SEL_AUTO, _T(""), 4, 0);		// 0 = �L�����Z���{�^���ɂȂ�܂�

	bind_control(&ci.btn_manual, IDC_BTN_MANUAL, _T(""), 9, 1);

	bind_control(&ci.lv_ini, IDC_LV_INI, _T("lv_ini"), 7, 1);
	bind_control(&ci.lv_inc, IDC_LV_INC, _T("lv_inc"), 7, 1);
	bind_control(&ci.lv_scn, IDC_LV_SCN, _T("lv_scn"), 4, 1);
	bind_control(&ci.lv_log, IDC_LV_LOG, _T("lv_log"), 5, 1);

	bind_control(&ci.edt_detail1, IDC_EDT_STATUS, _T(""), 2, 1);
	bind_control(&ci.edt_detail2, IDC_EDT_DETAIL, _T(""), 2, 1);
	bind_control(&ci.stc_version, IDC_STC_VERSION, _T(""), 1, 1);

	bind_control(&ci.chk_test, IDC_CHK_TEST, _T("chk_test"), 3, 1);
	bind_control(&ci.chk_no_interval, IDC_CHK_NO_INTERVAL, _T("chk_no_interval"), 3, 1);
	bind_control(&ci.chk_easy_link, IDC_CHK_EASY_LINK, _T("chk_easy_link"), 3, 1);
	bind_control(&ci.chk_overflow_error_off, IDC_CHK_OVERFLOW_ERROR_OFF, _T("chk_overflow_error_off"), 3, 1);
	bind_control(&ci.btn_explain_under, IDC_BTN_EXPLAIN_UNDER, _T("btn_explain_under"), 3, 1);

	// �_�C�A���O�Z�p���[�^
	bind_separater(&ci.dlgsep00, _T("dlgsep00"));		ci.dlgsep00.set_hv_mod(true);
	ci.dlgsep00.add_control_first(&ci.lv_ini);
	ci.dlgsep00.add_control_first(&ci.lv_inc);
	ci.dlgsep00.add_control_first(&ci.lv_scn);
	ci.dlgsep00.add_control_second(&ci.lv_log);

	bind_separater(&ci.dlgsep01, _T("dlgsep01"));		ci.dlgsep01.set_hv_mod(false);
	ci.dlgsep01.add_control_first(&ci.lv_ini);
	ci.dlgsep01.add_control_second(&ci.lv_inc);

	bind_separater(&ci.dlgsep02, _T("dlgsep02"));		ci.dlgsep02.set_hv_mod(false);
	ci.dlgsep02.add_control_first(&ci.lv_inc);
	ci.dlgsep02.add_control_second(&ci.lv_scn);

	// ���X�g�r���[�̐ݒ�
	ci.lv_ini.easy_setting(true, false, false, false);
	ci.lv_ini.add_column(_T("dummy"), _T("�_�~�["), 0);
	ci.lv_ini.add_column(_T("ini"), _T("ini dat txt"), ci.lv_ini.get_client_rect().width());
	ci.lv_inc.easy_setting(true, false, false, false);
	ci.lv_inc.add_column(_T("dummy"), _T("�_�~�["), 0);
	ci.lv_inc.add_column(_T("inc"), _T("inc"), ci.lv_inc.get_client_rect().width());
	ci.lv_scn.easy_setting(true, false, false, false);
	ci.lv_scn.add_column(_T("dummy"), _T("�_�~�["), 0);
	ci.lv_scn.add_column(_T("id"), _T("id"), 40);
	ci.lv_scn.add_column(_T("scn"), _T("scn"), ci.lv_scn.get_client_rect().width() - 40);
	ci.lv_log.easy_setting(true, false, false, false);
	ci.lv_log.add_column(_T("dummy"), _T("�_�~�["), 0);
	ci.lv_log.add_column(_T("result"), _T("����"), ci.lv_log.get_client_rect().width());

	// �X�^�[�g�̂y�ԍ��X�s���R���g���[���̐ݒ�
	ci.spn_start_z_no.set_edit_range(0, 99);

	// �o�[�W����
	ci.stc_version.set_text(_T("(c)VisualArt's Ver.") + SIGLUS_VERSION);

	// ini ���[�h
	G_main->load_ini();

	// �_�C�A���O���X�V
	G_main->update_dialog();		// �i�_�C�A���O�̏���������̌Ăяo���j

	// GUI ���[�h�̏ꍇ�̓_�C�A���O��\��
	if (G_data->gui_flag)	{
		set_show(true);
	}

	// CUI ���[�h�̏ꍇ�͎�������J�n�I
	if (!G_data->gui_flag)	{
		G_main->start_auto_process();
	}

	return true;
}

// ****************************************************************
// �E�B���h�E��������
// ================================================================
bool C_tnm_compiler_dialog::on_close()
{
	// GUI �̏ꍇ�̂� ini �Z�[�u
	if (G_data->gui_flag)	{
		G_main->save_ini();
	}

	return true;
}

// ****************************************************************
// �R���g���[�������삳�ꂽ
// ================================================================
bool C_tnm_compiler_dialog::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{

		// �v���W�F�N�g�̃R���{�{�b�N�X
		case IDC_CMB_PROJECT:
			if (HIWORD(wp) == CBN_SELCHANGE)	{
				G_main->change_project(ci.cmb_project.get_sel_no());
			}
			break;

		// �v���W�F�N�g�̐ݒ�^�V�K�쐬
		case IDC_BTN_PROJECT_CONFIG:	G_main->on_setting_project();	break;		// �v���W�F�N�g�ݒ�
		case IDC_BTN_NEW_PROJECT:		G_main->on_create_project();	break;		// �v���W�F�N�g�쐬
		case IDC_BTN_DEL_PROJECT:		G_main->on_delete_project();	break;		// �v���W�F�N�g�폜

		// �u�f�o�b�O �n�m�v�̃`�F�b�N�{�b�N�X
		case IDC_CHK_DEBUG_ON:
			if (ci.chk_debug_on.get_check())	{	ci.chk_debug_off.set_check(false);	}
			break;

		// �u�f�o�b�O �n�e�e�v�̃`�F�b�N�{�b�N�X
		case IDC_CHK_DEBUG_OFF:
			if (ci.chk_debug_off.get_check())	{	ci.chk_debug_on.set_check(false);	}
			break;

		// �u���s����v�̃`�F�b�N�{�b�N�X
		case IDC_CHK_EXECUTE:
			G_main->update_dialog_for_all_btn(false);
			break;

		// �u�X�^�[�g�V�[���v�̃`�F�b�N�{�b�N�X
		case IDC_CHK_START_SCENE:
			G_main->update_dialog_for_start_item();
			break;

		// �R���p�C������
		case IDC_BTN_FIND:			G_main->on_command_find();		break;		// �Č���
		case IDC_BTN_SEL_AUTO:		G_main->on_command_sel_auto();	break;		// �����I��
		case IDC_BTN_COMPILE:		G_main->on_command_build();		break;		// �r���h
		case IDC_BTN_RUN:			G_main->on_command_run();		break;		// ���s
		case IDC_BTN_ALL_SEL_ALL:	G_main->on_command_all(false);	break;		// �S���i�Č������S�I�����r���h�����s�j
		case IDC_BTN_ALL_SEL_AUTO:	G_main->on_command_all(true);	break;		// �S���i�Č����������I�����r���h�����s�j
		case IDC_BTN_MANUAL:		G_main->on_command_manual();	break;		// �}�j���A��

		// �E���ɂ��Ă̐���
		case IDC_BTN_EXPLAIN_UNDER:
			G_main->explain_under();
			break;

		// �Í����ɂ��Ă̐���
		case IDM_EXE_ANGOU_MANUAL:
			G_main->manual_exe_angou();
			break;

		// �d�w�d���Í�������
		case IDM_EXE_ANGOU_FUNC:
			G_main->func_exe_angou();
			break;
	}

	return true;
}

// ****************************************************************
// �R���g���[������ʒm������
// ================================================================
bool C_tnm_compiler_dialog::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* p_nm = (NMHDR*)lp;
	if (p_nm->code == LVN_ITEMCHANGED)	{
		NM_LISTVIEW* p_nmlv = (NM_LISTVIEW*)lp;
		if (p_nmlv->hdr.hwndFrom == ci.lv_log.get_handle())	{
			if (p_nmlv->uNewState & LVIS_FOCUSED)	{
				int row_no = p_nmlv->iItem;
				G_main->show_err_detail(row_no);
			}
		}
	}
	else if (p_nm->code == NM_RCLICK)	{
		//NM_LISTVIEW* p_nmlv = (NM_LISTVIEW*)lp;
		//if (p_nmlv->hdr.hwndFrom == ci.lv_ini.get_handle())	{
		//	G_main->popup_menu_proc();
		//}
		//if (p_nmlv->hdr.hwndFrom == ci.lv_inc.get_handle())	{
		//	G_main->popup_menu_proc();
		//}
		//if (p_nmlv->hdr.hwndFrom == ci.lv_scn.get_handle())	{
		//	C_point mp = get_mouse_pos();
		//	mp = ci.lv_scn.screen_to_client(mp);
		//	int row_no, col_no;
		//	TSTR scn_name;
		//	if (ci.lv_scn.get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
		//		if (0 <= row_no && row_no < (int)G_data->scn_list.size())	{
		//			scn_name = erase_extension_ret(G_data->scn_list[row_no].scn_name);
		//		}
		//	}
		//	G_main->popup_menu_proc(scn_name);
		//}
		//if (p_nmlv->hdr.hwndFrom == ci.lv_log.get_handle())	{
		//	G_main->popup_menu_proc();
		//}
	}
	return true;
}

// ****************************************************************
// ���j���[�̏��������b�Z�[�W
// ================================================================
bool C_tnm_compiler_dialog::on_initmenu(WPARAM wp, LPARAM lp)
{
	HMENU h_menu = GetMenu(get_handle());
	if ((HMENU)wp == h_menu)	{
		// �������i�������Ă��܂���j
	}
	return true;
}

// ****************************************************************
// �R���g���[���̗L��������ݒ肷��
// ================================================================
void C_tnm_compiler_dialog::set_enable_control(bool enable)
{
	for (int i = 0; i < (int)m_child_list.size(); i++)	{
		if (m_child_list[i]->get_group_no() == 1)	{
			m_child_list[i]->set_enable(enable);
		}
		else if (m_child_list[i]->get_group_no() == 2)	{
			m_child_list[i]->set_enable((enable && ci.chk_start_scene.get_check()) ? true : false);
		}
	}
}

// ****************************************************************
// �X�e�[�^�X�o�[�̓��e��ݒ肷��
// ================================================================
void C_tnm_compiler_dialog::set_detail1_text(CTSTR& text)
{
	ci.edt_detail1.set_text(text);
}
void C_tnm_compiler_dialog::set_detail2_text(CTSTR& text)
{
	ci.edt_detail2.set_text(text);
}

// ****************************************************************
// ini �r���[
// ================================================================

// �v���V�[�W��
LRESULT	C_tnm_compiler_ini_lv::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// case WM_SIZE:
		// ���d�v�Ȏ��Ȃ̂ŁA�����Ƃ��Ďc���Ă����܂��B
		// �����ڃT�C�Y�̒����� WM_SIZE �ŏ�������ׂ����Ǝv���̂ł����A�Ƃ�������Ń��X�g�̕\�����o�O��܂��B
		// ���Č����@�F���X�g�����܂ŃX�N���[�������遨�E�B���h�E�̏c�T�C�Y���L���遨�Ȃ������X�g�̃g�b�v�|�W�V�������������Ȏ��Ɂc
		// �������F���L�̂悤�� WM_PAINT �ŏ������ĉ������B

		case WM_PAINT:
			set_column_width_just(1, false);
			break;
		case WM_LBUTTONDBLCLK:
		{
			C_point mp = get_mouse_pos();
			mp = screen_to_client(mp);
			int row_no, col_no;
			if (get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
				G_main->open_ini_by_ini_lv(row_no);
			}
			break;
		}
		case WM_CONTEXTMENU:
			G_main->popup_menu_proc();
			return FALSE;
	}

	return Cc_listview::window_proc(msg, wp, lp);
}

// ****************************************************************
// inc �r���[
// ================================================================

// �v���V�[�W��
LRESULT	C_tnm_compiler_inc_lv::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_PAINT:
			set_column_width_just(1, false);
			break;
		case WM_LBUTTONDBLCLK:
		{
			C_point mp = get_mouse_pos();
			mp = screen_to_client(mp);
			int row_no, col_no;
			if (get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
				G_main->open_inc_by_inc_lv(row_no);
			}
			break;
		}
		case WM_CONTEXTMENU:
			G_main->popup_menu_proc();
			return FALSE;
	}

	return Cc_listview::window_proc(msg, wp, lp);
}

// ****************************************************************
// scn �r���[
// ================================================================

// �v���V�[�W��
LRESULT	C_tnm_compiler_scn_lv::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_PAINT:
			set_column_width_just(-1, false);	// -1: �Ō�̃R�����𒲐�����
			break;
		case WM_LBUTTONDBLCLK:
		{
			C_point mp = get_mouse_pos();
			mp = screen_to_client(mp);
			int row_no, col_no;
			if (get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
				G_main->open_scn_by_scn_lv(row_no);
			}
			break;
		}
		case WM_CONTEXTMENU:
		{
			C_point mp = get_mouse_pos();
			mp = screen_to_client(mp);
			int row_no, col_no;
			TSTR scn_name;
			if (get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
				if (0 <= row_no && row_no < (int)G_data->scn_list.size())	{
					scn_name = erase_extension_ret(G_data->scn_list[row_no].scn_name);
				}
			}
			G_main->popup_menu_proc(scn_name);
			return FALSE;
		}
	}

	return Cc_listview::window_proc(msg, wp, lp);
}

// ****************************************************************
// ���O�r���[
// ================================================================

// �v���V�[�W��
LRESULT	C_tnm_compiler_log_lv::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_PAINT:
			set_column_width_just(2, false);
			break;
		case WM_LBUTTONDBLCLK:
		{
			C_point mp = get_mouse_pos();
			mp = screen_to_client(mp);
			int row_no, col_no;
			if (get_cell_by_point(mp.x, mp.y, &row_no, &col_no))	{
				G_main->open_scn_by_log_lv(row_no);
			}
			break;
		}
		case WM_CONTEXTMENU:
			G_main->popup_menu_proc();
			return FALSE;
	}

	return Cc_listview::window_proc(msg, wp, lp);
}

